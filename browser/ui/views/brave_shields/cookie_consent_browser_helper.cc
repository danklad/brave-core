/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/brave_shields/cookie_consent_browser_helper.h"

#include <memory>
#include <string>
#include <utility>

#include "base/containers/flat_set.h"
#include "base/numerics/clamped_math.h"
#include "brave/browser/brave_browser_process.h"
#include "brave/browser/ui/webui/brave_shields/cookie_consent_blocker_ui.h"
#include "brave/components/brave_shields/browser/ad_block_regional_service_manager.h"
#include "brave/components/brave_shields/browser/ad_block_service.h"
#include "brave/components/brave_shields/common/brave_shield_constants.h"
#include "brave/components/brave_shields/common/pref_names.h"
#include "brave/components/constants/webui_url_constants.h"
#include "chrome/browser/browser_process.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/views/bubble/webui_bubble_manager.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "chrome/browser/ui/views/location_bar/location_bar_view.h"
#include "components/grit/brave_components_strings.h"
#include "content/public/browser/web_contents.h"

namespace brave_shields {

namespace {

// The number of unique host names loaded as the primary page in the active tab
// before we show the cookie consent blocker bubble.
constexpr int kTabLoadThreshold = 10;

views::View* GetAnchorView(Browser* browser) {
  DCHECK(browser);
  auto* browser_view = BrowserView::GetBrowserViewForBrowser(browser);
  DCHECK(browser_view);
  auto* location_bar_view = browser_view->GetLocationBarView();
  if (location_bar_view) {
    return location_bar_view;
  }
  return browser_view;
}

class CookieConsentProfileHelper : public base::SupportsUserData::Data {
 public:
  static inline constexpr char kKey[] =
      "BraveCookieConsentBlockerProfileHelperData";

  CookieConsentProfileHelper() {}
  ~CookieConsentProfileHelper() override = default;

  static CookieConsentProfileHelper* GetOrCreateForProfile(Profile* profile) {
    DCHECK(profile);
    auto* helper =
        static_cast<CookieConsentProfileHelper*>(profile->GetUserData(kKey));
    if (!helper) {
      auto instance = std::make_unique<CookieConsentProfileHelper>();
      helper = instance.get();
      profile->SetUserData(kKey, std::move(instance));
    }
    return helper;
  }

  static void RemoveFromProfile(Profile* profile) {
    DCHECK(profile);
    profile->RemoveUserData(kKey);
  }

  base::flat_set<std::string>& host_set() { return host_set_; }
  const base::flat_set<std::string>& host_set() const { return host_set_; }

 private:
  base::flat_set<std::string> host_set_;
};

bool ShouldEventuallyShowBubble() {
  auto* local_state = g_browser_process->local_state();
  if (local_state->GetBoolean(prefs::kAdBlockCookieConsentBlockerDisplayed)) {
    return false;
  }

  auto* regional_service_manager =
      g_brave_browser_process->ad_block_service()->regional_service_manager();
  DCHECK(regional_service_manager);
  if (regional_service_manager->IsFilterListEnabled(kCookieListUuid)) {
    return false;
  }

  return true;
}

void EventuallyRemoveFromBrowser(Browser* browser) {
  DCHECK(browser);
  base::SequencedTaskRunnerHandle::Get()->PostTask(
      FROM_HERE,
      base::BindOnce(
          [](Browser* browser) {
            CookieConsentProfileHelper::RemoveFromProfile(browser->profile());
            CookieConsentBrowserHelper::RemoveFromBrowser(browser);
          },
          browser));
}

}  // namespace

CookieConsentBrowserHelper::CookieConsentBrowserHelper(Browser* browser)
    : BrowserUserData<CookieConsentBrowserHelper>(*browser) {
  GetBrowser().tab_strip_model()->AddObserver(this);
}

CookieConsentBrowserHelper::~CookieConsentBrowserHelper() = default;

void CookieConsentBrowserHelper::CreateForBrowserIfNecessary(Browser* browser) {
  if (ShouldEventuallyShowBubble()) {
    CreateForBrowser(browser);
  }
}

bool CookieConsentBrowserHelper::ShowBubble() {
  // Do not show the bubble if this is not the currently active browser window.
  auto* browser_view = BrowserView::GetBrowserViewForBrowser(&GetBrowser());
  if (!browser_view || !browser_view->IsActive()) {
    return false;
  }

  // Do not show the bubble if the filter list is not yet available, likely
  // because the regional filter list has not yet been donwloaded.
  auto* regional_service_manager =
      g_brave_browser_process->ad_block_service()->regional_service_manager();
  DCHECK(regional_service_manager);
  if (!regional_service_manager->IsFilterListAvailable(kCookieListUuid)) {
    return false;
  }

  if (!bubble_manager_) {
    bubble_manager_ =
        std::make_unique<WebUIBubbleManagerT<CookieConsentBlockerUI>>(
            GetAnchorView(&GetBrowser()), GetBrowser().profile(),
            GURL(kCookieConsentBlockerURL), IDS_BRAVE_SHIELDS);
  }

  if (!bubble_manager_->GetBubbleWidget()) {
    bubble_manager_->ShowBubble();

    DCHECK(!bubble_observation_.IsObserving());
    bubble_observation_.Observe(bubble_manager_->GetBubbleWidget());
  }

  return true;
}

void CookieConsentBrowserHelper::TabChangedAt(
    content::WebContents* web_contents,
    int index,
    TabChangeType change_type) {
  if (GetBrowser().tab_strip_model()->active_index() != index ||
      change_type != TabChangeType::kLoadingOnly || web_contents->IsLoading()) {
    return;
  }

  MaybeShowBubbleOnTabLoad(web_contents->GetLastCommittedURL());
}

void CookieConsentBrowserHelper::OnWidgetDestroying(views::Widget* widget) {
  DCHECK(bubble_observation_.IsObservingSource(widget));
  bubble_observation_.Reset();

  if (ShouldEventuallyShowBubble()) {
    // If we should still show the bubble later, then restart the host count.
    auto* helper = CookieConsentProfileHelper::GetOrCreateForProfile(
        GetBrowser().profile());
    helper->host_set().clear();
  } else {
    EventuallyRemoveFromBrowser(&GetBrowser());
  }
}

bool CookieConsentBrowserHelper::MaybeShowBubbleOnTabLoad(const GURL& url) {
  if (!ShouldEventuallyShowBubble()) {
    EventuallyRemoveFromBrowser(&GetBrowser());
    return false;
  }

  if (!url.SchemeIsHTTPOrHTTPS() || !url.has_host()) {
    return false;
  }

  // Get a pre-profile set of recently loaded hostnames.
  auto* helper =
      CookieConsentProfileHelper::GetOrCreateForProfile(GetBrowser().profile());

  // Add the current host to the set of recently loaded hostnames and check to
  // see whether it's time to show the bubble.
  auto [_, inserted] = helper->host_set().emplace(url.host());
  if (!inserted || helper->host_set().size() < kTabLoadThreshold) {
    return false;
  }

  return ShowBubble();
}

BROWSER_USER_DATA_KEY_IMPL(CookieConsentBrowserHelper);

}  // namespace brave_shields
