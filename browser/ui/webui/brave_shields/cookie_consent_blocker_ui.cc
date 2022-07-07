/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/webui/brave_shields/cookie_consent_blocker_ui.h"

#include <memory>
#include <utility>

#include "brave/browser/ui/webui/brave_shields/cookie_consent_page_handler.h"
#include "brave/components/brave_shields/resources/cookie_consent_blocker/grit/cookie_consent_blocker_generated_map.h"
#include "brave/components/constants/webui_url_constants.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/webui/favicon_source.h"
#include "chrome/browser/ui/webui/webui_util.h"
#include "components/favicon_base/favicon_url_parser.h"
#include "components/grit/brave_components_resources.h"
#include "components/grit/brave_components_strings.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_ui.h"

namespace {

static constexpr webui::LocalizedString kStrings[] = {
    {"cookieConsentTitle", IDS_BRAVE_SHIELDS_COOKIE_CONSENT_TITLE},
    {"cookieConsentHeader", IDS_BRAVE_SHIELDS_COOKIE_CONSENT_HEADER},
    {"cookieConsentText", IDS_BRAVE_SHIELDS_COOKIE_CONSENT_TEXT},
    {"cookieConsentLearnMore", IDS_BRAVE_SHIELDS_COOKIE_CONSENT_LEARN_MORE},
    {"cookieConsentButtonText", IDS_BRAVE_SHIELDS_COOKIE_CONSENT_BUTTON_TEXT},
    {"cookieConsentNoThanks", IDS_BRAVE_SHIELDS_COOKIE_CONSENT_NO_THANKS},
    {"cookieConsentNote", IDS_BRAVE_SHIELDS_COOKIE_CONSENT_NOTE}};

}  // namespace

CookieConsentBlockerUI::CookieConsentBlockerUI(content::WebUI* web_ui)
    : MojoBubbleWebUIController(web_ui, true) {
  auto* profile = Profile::FromWebUI(web_ui);

  auto* source = content::WebUIDataSource::Create(kCookieConsentBlockerHost);
  source->AddLocalizedStrings(kStrings);

  webui::SetupWebUIDataSource(
      source,
      base::make_span(kCookieConsentBlockerGenerated,
                      kCookieConsentBlockerGeneratedSize),
      IDR_COOKIE_CONSENT_BLOCKER_HTML);

  content::WebUIDataSource::Add(web_ui->GetWebContents()->GetBrowserContext(),
                                source);

  content::URLDataSource::Add(
      profile, std::make_unique<FaviconSource>(
                   profile, chrome::FaviconUrlFormat::kFavicon2));
}

CookieConsentBlockerUI::~CookieConsentBlockerUI() = default;

WEB_UI_CONTROLLER_TYPE_IMPL(CookieConsentBlockerUI)

void CookieConsentBlockerUI::BindInterface(
    mojo::PendingReceiver<CookieConsentPageHandlerFactory> receiver) {
  page_factory_receiver_.reset();
  page_factory_receiver_.Bind(std::move(receiver));
}

void CookieConsentBlockerUI::CreatePageHandler(
    mojo::PendingReceiver<brave_shields::mojom::CookieConsentPageHandler>
        receiver) {
  page_handler_ = std::make_unique<CookieConsentPageHandler>(
      std::move(receiver), embedder());
}
