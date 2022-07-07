// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'

import { CloseIcon } from '../icons/close_icon'
import { ShieldsIcon } from '../icons/shields_icon'
import { getLocale } from '../../../../../common/locale'
import Button from '$web-components/button'

import * as S from './style'

interface Props {
  onEnable: () => void
  onDismiss: () => void
  onDecline: () => void
  onAnimationComplete: () => void
}

function MainPanel (props: Props) {
  const [enabled, setEnabled] = React.useState(false)

  const onRootMounted = (elem: HTMLElement | null) => {
    if (elem) {
      elem.style.setProperty('--available-height', screen.availHeight + 'px')
    }
  }

  const openLearnMore = () => {
    chrome.tabs.create({
      url: 'https://brave.com/privacy-features/',
      active: true
    })
  }

  const onEnablePressed = () => {
    setEnabled(true)
    setTimeout(() => { props.onAnimationComplete() }, 3500)
    props.onEnable()
  }

  return (
    <S.Root ref={onRootMounted} className={enabled ? 'success' : ''}>
      <S.TitleBar>
        <div><ShieldsIcon /></div>
        <S.TitleBarText>{getLocale('cookieConsentTitle')}</S.TitleBarText>
        <button onClick={props.onDismiss}><CloseIcon /></button>
      </S.TitleBar>
      <S.Content>
        <S.CookieGraphic />
        <S.Header>
          {getLocale('cookieConsentHeader')}
        </S.Header>
        <S.Description>
          {getLocale('cookieConsentText')}
          <br />
          <a href='#' onClick={openLearnMore}>
            {getLocale('cookieConsentLearnMore')}
          </a>
        </S.Description>
        <S.OptIn>
          <Button
            isPrimary
            isCallToAction
            scale='large'
            onClick={onEnablePressed}
          >
            {getLocale('cookieConsentButtonText')}
          </Button>
        </S.OptIn>
        <S.Decline>
          <button onClick={props.onDecline}>
            {getLocale('cookieConsentNoThanks')}
          </button>
        </S.Decline>
        <S.Note>
          {getLocale('cookieConsentNote')}
        </S.Note>
      </S.Content>
      {enabled && <S.Animation />}
    </S.Root>
  )
}

export default MainPanel
