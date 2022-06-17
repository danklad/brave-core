// Copyright (c) 2021 The Brave Authors. All rights reserved.
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this file,
// you can obtain one at http://mozilla.org/MPL/2.0/.

import * as React from 'react'
import { useSelector } from 'react-redux'

// utils
import { addLogoToToken } from '../../utils/asset-utils'

// Constants
import {
  BraveWallet,
  WalletState,
  BuySupportedChains
} from '../../constants/types'

// Hooks
import usePricing from './pricing'
import useBalance from './balance'
import { useIsMounted } from './useIsMounted'
import { useLib } from './useLib'

export function useAssets () {
  // redux
  const {
    selectedAccount,
    networkList,
    selectedNetwork,
    userVisibleTokensInfo,
    transactionSpotPrices: spotPrices
  } = useSelector((state: { wallet: WalletState }) => state.wallet)

  // custom hooks
  const isMounted = useIsMounted()
  const { getBuyAssets } = useLib()
  const { computeFiatAmount } = usePricing(spotPrices)
  const getBalance = useBalance(networkList)

  // state
  const [wyreAssetOptions, setWyreAssetOptions] = React.useState<BraveWallet.BlockchainToken[]>([])
  const [rampAssetOptions, setRampAssetOptions] = React.useState<BraveWallet.BlockchainToken[]>([])

  // memos
  const assetsByNetwork = React.useMemo(() => {
    if (!userVisibleTokensInfo) {
      return []
    }
    // We also filter by coinType here because localhost
    // networks share the same chainId.
    return userVisibleTokensInfo.filter((token) =>
      token.chainId === selectedNetwork.chainId &&
      token.coin === selectedNetwork.coin
    )
  }, [userVisibleTokensInfo, selectedNetwork])

  const assetsByValueAndNetwork = React.useMemo(() => {
    if (!assetsByNetwork) {
      return []
    }

    if (!selectedAccount) {
      return []
    }

    return assetsByNetwork.sort(function (a, b) {
      const aBalance = getBalance(selectedAccount, a)
      const bBalance = getBalance(selectedAccount, b)

      const bFiatBalance = computeFiatAmount(bBalance, b.symbol, b.decimals)
      const aFiatBalance = computeFiatAmount(aBalance, a.symbol, a.decimals)

      return bFiatBalance.toNumber() - aFiatBalance.toNumber()
    })
  }, [selectedAccount, assetsByNetwork, getBalance, computeFiatAmount])

  const buyAssetOptions = React.useMemo(() => {
    return [...rampAssetOptions, ...wyreAssetOptions].filter(asset => asset.chainId === selectedNetwork.chainId)
  }, [rampAssetOptions, wyreAssetOptions, selectedNetwork])

  // methods
  const getAllBuyOptionsCurrentNetwork = React.useCallback(async () => {
    // Prevent calling getBuyAssets if the selectedNetwork is not supported.
    if (!BuySupportedChains.includes(selectedNetwork.chainId)) {
      return
    }

    const wyreRegistryTokens = await getBuyAssets(BraveWallet.OnRampProvider.kWyre, selectedNetwork.chainId)
    const rampRegistryTokens = await getBuyAssets(BraveWallet.OnRampProvider.kRamp, selectedNetwork.chainId)
    const wyreAssetOptions = wyreRegistryTokens.map(addLogoToToken)
    const rampAssetOptions = rampRegistryTokens.map(addLogoToToken)
    if (isMounted) {
      setWyreAssetOptions(wyreAssetOptions)
      setRampAssetOptions(rampAssetOptions)
    }
  }, [getBuyAssets, selectedNetwork, isMounted])

  React.useEffect(() => {
    getAllBuyOptionsCurrentNetwork()
  }, [selectedNetwork])

  return {
    sendAssetOptions: assetsByNetwork,
    buyAssetOptions,
    rampAssetOptions,
    wyreAssetOptions,
    panelUserAssetList: assetsByValueAndNetwork,
    getAllBuyOptionsCurrentNetwork
  }
}

export default useAssets
