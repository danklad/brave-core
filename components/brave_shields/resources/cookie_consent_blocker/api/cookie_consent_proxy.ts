// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as mojom from 'gen/brave/components/brave_shields/common/cookie_consent_blocker.mojom.m.js'

let instance: CookieConsentProxy|null = null

export class CookieConsentProxy {
  handler: mojom.CookieConsentPageHandlerRemote

  constructor (handler: mojom.CookieConsentPageHandlerRemote) {
    this.handler = handler
  }

  static getInstance (): CookieConsentProxy {
    if (!instance) {
      const handler = new mojom.CookieConsentPageHandlerRemote()
      mojom.CookieConsentPageHandlerFactory.getRemote().createPageHandler(
        handler.$.bindNewPipeAndPassReceiver())
      instance = new CookieConsentProxy(handler)
    }
    return instance
  }
}
