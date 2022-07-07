/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/webui/brave_shields/cookie_consent_page_handler.h"

#include <utility>

#include "brave/browser/brave_browser_process.h"
#include "brave/components/brave_shields/browser/ad_block_regional_service_manager.h"
#include "brave/components/brave_shields/browser/ad_block_service.h"
#include "brave/components/brave_shields/common/brave_shield_constants.h"
#include "brave/components/brave_shields/common/pref_names.h"
#include "chrome/browser/browser_process.h"
#include "components/prefs/pref_service.h"

CookieConsentPageHandler::CookieConsentPageHandler(
    mojo::PendingReceiver<brave_shields::mojom::CookieConsentPageHandler>
        receiver,
    base::WeakPtr<ui::MojoBubbleWebUIController::Embedder> embedder)
    : receiver_(this, std::move(receiver)), embedder_(embedder) {
  DCHECK(embedder_);
}

CookieConsentPageHandler::~CookieConsentPageHandler() = default;

void CookieConsentPageHandler::ShowUI() {
  if (embedder_) {
    embedder_->ShowUI();
  }
}

void CookieConsentPageHandler::CloseUI() {
  if (embedder_) {
    embedder_->CloseUI();
  }
}

void CookieConsentPageHandler::EnableCookieConsentBlocking() {
  SaveUserInteraction();

  g_brave_browser_process->ad_block_service()
      ->regional_service_manager()
      ->EnableFilterList(brave_shields::kCookieListUuid, true);
}

void CookieConsentPageHandler::OnCookieConsentBlockingDeclined() {
  SaveUserInteraction();
}

void CookieConsentPageHandler::SaveUserInteraction() {
  g_browser_process->local_state()->SetBoolean(
      brave_shields::prefs::kAdBlockCookieConsentBlockerDisplayed, true);
}
