/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_VIEWS_BRAVE_SHIELDS_COOKIE_CONSENT_BROWSER_HELPER_H_
#define BRAVE_BROWSER_UI_VIEWS_BRAVE_SHIELDS_COOKIE_CONSENT_BROWSER_HELPER_H_

#include <memory>
#include <string>

#include "base/scoped_observation.h"
#include "chrome/browser/ui/browser_user_data.h"
#include "chrome/browser/ui/tabs/tab_strip_model_observer.h"
#include "ui/views/widget/widget.h"
#include "ui/views/widget/widget_observer.h"
#include "url/gurl.h"

class CookieConsentBlockerUI;

template <typename T>
class WebUIBubbleManagerT;

namespace content {
class WebContents;
}

namespace brave_shields {

class CookieConsentBrowserHelper
    : public BrowserUserData<CookieConsentBrowserHelper>,
      public TabStripModelObserver,
      public views::WidgetObserver {
 public:
  explicit CookieConsentBrowserHelper(Browser* browser);

  CookieConsentBrowserHelper(const CookieConsentBrowserHelper&) = delete;
  CookieConsentBrowserHelper& operator=(const CookieConsentBrowserHelper&) =
      delete;

  ~CookieConsentBrowserHelper() override;

  static void CreateForBrowserIfNecessary(Browser* browser);

  bool ShowBubble();

  // TabStripObserver:
  void TabChangedAt(content::WebContents* web_contents,
                    int index,
                    TabChangeType change_type) override;

  // views::WidgetObserver:
  void OnWidgetDestroying(views::Widget* widget) override;

 private:
  friend class BrowserUserData<CookieConsentBrowserHelper>;

  bool MaybeShowBubbleOnTabLoad(const GURL& url);

  std::unique_ptr<WebUIBubbleManagerT<CookieConsentBlockerUI>> bubble_manager_;
  base::ScopedObservation<views::Widget, views::WidgetObserver>
      bubble_observation_{this};

  BROWSER_USER_DATA_KEY_DECL();
};

}  // namespace brave_shields

#endif  // BRAVE_BROWSER_UI_VIEWS_BRAVE_SHIELDS_COOKIE_CONSENT_BROWSER_HELPER_H_
