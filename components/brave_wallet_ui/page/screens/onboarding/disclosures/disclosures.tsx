// Copyright (c) 2022 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { useHistory } from 'react-router'
import { useDispatch, useSelector } from 'react-redux'

// utils
import { getLocale, getLocaleWithTag } from '../../../../../common/locale'

// routes
import { PageState, WalletRoutes } from '../../../../constants/types'

// actions
import { WalletPageActions } from '../../../actions'

// components
import { Checkbox } from '../../../../components/shared/checkbox/checkbox'
import WalletPageLayout from '../../../../components/desktop/wallet-page-layout/index'
import { NavButton } from '../../../../components/extension/buttons/nav-button/index'
import { OnboardingNewWalletStepsNavigation } from '../components/onboarding-steps-navigation/onboarding-steps-navigation'

// styles
import { LinkText, VerticalSpace } from '../../../../components/shared/style'
import {
  StyledWrapper,
  Title,
  Description,
  NextButtonRow,
  MainWrapper
} from '../onboarding.style'
import {
  CheckboxText
} from './disclosures.style'

export type OnboardingDisclosuresNextSteps =
  | WalletRoutes.OnboardingCreatePassword
  | WalletRoutes.OnboardingImportOrRestore

interface Props {
  nextStep: OnboardingDisclosuresNextSteps
  onBack?: () => void
}

export const OnboardingDisclosures = ({ nextStep, onBack }: Props) => {
  // routing
  const history = useHistory()

  // redux
  const dispatch = useDispatch()
  const walletTermsAcknowledged = useSelector(({ page }: { page: PageState }) => page.walletTermsAcknowledged)

  // state
  const [isResponsibilityCheckboxChecked, setIsResponsibilityCheckboxChecked] = React.useState(
    walletTermsAcknowledged
  )
  const [isTermsCheckboxChecked, setIsTermsCheckboxChecked] = React.useState(
    walletTermsAcknowledged
  )

  // memos
  const isNextStepEnabled = React.useMemo(() => {
    return isResponsibilityCheckboxChecked && isTermsCheckboxChecked
  }, [isResponsibilityCheckboxChecked, isTermsCheckboxChecked])

  const termsOfUseText = React.useMemo(() => {
    const text = getLocaleWithTag('braveWalletTermsOfServiceCheckboxText')
    return <p key={text.duringTag}>
      {text.beforeTag}
      <LinkText
        href='https://brave.com/terms-of-use/'
        target='_blank'
        rel='noopener noreferrer'
      >
        {text.duringTag}
      </LinkText>
      {text.afterTag}
    </p>
  }, [])

  // methods
  const onNext = React.useCallback(() => {
    if (isNextStepEnabled) {
      dispatch(WalletPageActions.agreeToWalletTerms())
      history.push(nextStep)
    }
  }, [isNextStepEnabled, nextStep])

  // render
  return (
    <WalletPageLayout>
      <MainWrapper>
        <StyledWrapper>

          <OnboardingNewWalletStepsNavigation
            goBack={onBack}
            currentStep={WalletRoutes.OnboardingWelcome}
            preventSkipAhead
          />

          <div>
            <Title>
              {getLocale('braveWalletDisclosuresTitle')}
            </Title>
            <Description>
              {getLocale('braveWalletDisclosuresDescription')}
            </Description>
          </div>

          <VerticalSpace space='40px' />

          <div>
            <Checkbox
              isChecked={isResponsibilityCheckboxChecked}
              onChange={setIsResponsibilityCheckboxChecked}
              alignItems='flex-start'
            >
              <CheckboxText>
                <p>
                  {getLocale('braveWalletSelfCustodyDisclosureCheckboxText')}
                </p>
              </CheckboxText>
            </Checkbox>

            <Checkbox
              isChecked={isTermsCheckboxChecked}
              onChange={setIsTermsCheckboxChecked}
              alignItems='flex-start'
            >
              <CheckboxText>
                {termsOfUseText}
              </CheckboxText>
            </Checkbox>

            <VerticalSpace space='120px' />
          </div>

          <NextButtonRow>
            <NavButton
              buttonType='primary'
              text={getLocale('braveWalletButtonContinue')}
              onSubmit={onNext}
              disabled={!isNextStepEnabled}
            />
          </NextButtonRow>

        </StyledWrapper>
      </MainWrapper>
    </WalletPageLayout>
  )
}

export default OnboardingDisclosures
