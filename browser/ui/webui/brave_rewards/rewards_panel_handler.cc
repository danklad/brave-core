/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/webui/brave_rewards/rewards_panel_handler.h"

#include <utility>

#include "brave/browser/brave_rewards/rewards_panel_coordinator.h"
#include "brave/browser/brave_rewards/rewards_service_factory.h"
#include "brave/components/brave_rewards/browser/rewards_service.h"
#include "chrome/browser/profiles/profile.h"
#include "chrome/browser/ui/browser.h"
#include "chrome/browser/ui/browser_finder.h"

RewardsPanelHandler::RewardsPanelHandler(
    mojo::PendingReceiver<brave_rewards::mojom::PanelHandler> receiver,
    base::WeakPtr<ui::MojoBubbleWebUIController::Embedder> embedder,
    Profile* profile,
    brave_rewards::RewardsPanelCoordinator* panel_coordinator)
    : receiver_(this, std::move(receiver)),
      embedder_(embedder),
      profile_(profile),
      panel_coordinator_(panel_coordinator) {
  DCHECK(embedder_);
  DCHECK(profile_);
}

RewardsPanelHandler::~RewardsPanelHandler() = default;

void RewardsPanelHandler::ShowUI() {
  if (embedder_) {
    embedder_->ShowUI();
  }
}

void RewardsPanelHandler::CloseUI() {
  if (embedder_) {
    embedder_->CloseUI();
  }
}

void RewardsPanelHandler::StartRewards(StartRewardsCallback callback) {
  auto* rewards_service =
      brave_rewards::RewardsServiceFactory::GetForProfile(profile_);
  if (!rewards_service) {
    NOTREACHED();
    std::move(callback).Run();
    return;
  }

  rewards_service->StartProcess(std::move(callback));
}

void RewardsPanelHandler::GetRewardsPanelArgs(
    GetRewardsPanelArgsCallback callback) {
  std::move(callback).Run(panel_coordinator_
                              ? panel_coordinator_->panel_args().Clone()
                              : brave_rewards::mojom::RewardsPanelArgs::New());
}
