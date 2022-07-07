/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_WEBUI_BRAVE_SHIELDS_COOKIE_CONSENT_BLOCKER_UI_H_
#define BRAVE_BROWSER_UI_WEBUI_BRAVE_SHIELDS_COOKIE_CONSENT_BLOCKER_UI_H_

#include <memory>

#include "brave/components/brave_shields/common/cookie_consent_blocker.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "ui/webui/mojo_bubble_web_ui_controller.h"

class CookieConsentBlockerUI
    : public ui::MojoBubbleWebUIController,
      public brave_shields::mojom::CookieConsentPageHandlerFactory {
 public:
  explicit CookieConsentBlockerUI(content::WebUI* web_ui);
  ~CookieConsentBlockerUI() override;

  CookieConsentBlockerUI(const CookieConsentBlockerUI&) = delete;
  CookieConsentBlockerUI& operator=(const CookieConsentBlockerUI&) = delete;

  void BindInterface(
      mojo::PendingReceiver<CookieConsentPageHandlerFactory> reciever);

 private:
  // brave_shields::mojom::CookieConsentPageHandlerFactory:
  void CreatePageHandler(
      mojo::PendingReceiver<brave_shields::mojom::CookieConsentPageHandler>
          receiver) override;

  std::unique_ptr<brave_shields::mojom::CookieConsentPageHandler> page_handler_;
  mojo::Receiver<CookieConsentPageHandlerFactory> page_factory_receiver_{this};

  WEB_UI_CONTROLLER_TYPE_DECL();
};

#endif  // BRAVE_BROWSER_UI_WEBUI_BRAVE_SHIELDS_COOKIE_CONSENT_BLOCKER_UI_H_
