/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_WEBUI_BRAVE_SHIELDS_COOKIE_CONSENT_PAGE_HANDLER_H_
#define BRAVE_BROWSER_UI_WEBUI_BRAVE_SHIELDS_COOKIE_CONSENT_PAGE_HANDLER_H_

#include "base/memory/weak_ptr.h"
#include "brave/components/brave_shields/common/cookie_consent_blocker.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "ui/webui/mojo_bubble_web_ui_controller.h"

class CookieConsentPageHandler
    : public brave_shields::mojom::CookieConsentPageHandler {
 public:
  CookieConsentPageHandler(
      mojo::PendingReceiver<brave_shields::mojom::CookieConsentPageHandler>
          receiver,
      base::WeakPtr<ui::MojoBubbleWebUIController::Embedder> embedder);

  CookieConsentPageHandler(const CookieConsentPageHandler&) = delete;
  CookieConsentPageHandler& operator=(const CookieConsentPageHandler&) = delete;

  ~CookieConsentPageHandler() override;

  // brave_shields::mojom::CookieConsentPageHandler:
  void ShowUI() override;
  void CloseUI() override;
  void EnableCookieConsentBlocking() override;
  void OnCookieConsentBlockingDeclined() override;

 private:
  void SaveUserInteraction();

  mojo::Receiver<brave_shields::mojom::CookieConsentPageHandler> receiver_;
  base::WeakPtr<ui::MojoBubbleWebUIController::Embedder> embedder_;
};

#endif  // BRAVE_BROWSER_UI_WEBUI_BRAVE_SHIELDS_COOKIE_CONSENT_PAGE_HANDLER_H_
