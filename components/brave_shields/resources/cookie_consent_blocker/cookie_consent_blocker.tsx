// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { render } from 'react-dom'
import { initLocale } from 'brave-ui'

import { loadTimeData } from '../../../common/loadTimeData'
import { CookieConsentProxy } from './api/cookie_consent_proxy'
import MainPanel from './components/main-panel'

function App () {
  const [proxy] = React.useState(CookieConsentProxy.getInstance())
  const [openedAt, setOpenedAt] = React.useState(Date.now())

  const closeBubble = () => {
    proxy.handler.closeUI()
  }

  const onEnable = () => {
    proxy.handler.enableCookieConsentBlocking()
  }

  const onDecline = () => {
    proxy.handler.onCookieConsentBlockingDeclined()
    proxy.handler.closeUI()
  }

  const onVisibilityChange = () => {
    if (document.visibilityState === 'visible') {
      const now = Date.now()
      if (now - openedAt > 100) {
        setOpenedAt(now)
      }
      proxy.handler.showUI()
    }
  }

  React.useEffect(() => {
    document.addEventListener('visibilitychange', onVisibilityChange)
    return () => {
      document.removeEventListener('visibilitychange', onVisibilityChange)
    }
  }, [])

  React.useEffect(() => {
    proxy.handler.showUI()
  }, [])

  return (
    <MainPanel
      key={openedAt}
      onEnable={onEnable}
      onDismiss={closeBubble}
      onDecline={onDecline}
      onAnimationComplete={closeBubble}
    />
  )
}

document.addEventListener('DOMContentLoaded', () => {
  initLocale(loadTimeData.data_)
  render(<App />, document.getElementById('mount'))
})
