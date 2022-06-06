/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_BROWSER_UI_WEBUI_BRAVE_REWARDS_REWARDS_PANEL_HANDLER_H_
#define BRAVE_BROWSER_UI_WEBUI_BRAVE_REWARDS_REWARDS_PANEL_HANDLER_H_

#include "base/memory/raw_ptr.h"
#include "base/memory/weak_ptr.h"
#include "brave/components/brave_rewards/common/brave_rewards_panel.mojom.h"
#include "mojo/public/cpp/bindings/pending_receiver.h"
#include "mojo/public/cpp/bindings/receiver.h"
#include "ui/webui/mojo_bubble_web_ui_controller.h"

class Browser;
class Profile;

namespace brave_rewards {
class RewardsPanelCoordinator;
}

class RewardsPanelHandler : public brave_rewards::mojom::PanelHandler {
 public:
  RewardsPanelHandler(
      mojo::PendingReceiver<brave_rewards::mojom::PanelHandler> receiver,
      base::WeakPtr<ui::MojoBubbleWebUIController::Embedder> embedder,
      Profile* profile,
      brave_rewards::RewardsPanelCoordinator* panel_coordinator);

  RewardsPanelHandler(const RewardsPanelHandler&) = delete;
  RewardsPanelHandler& operator=(const RewardsPanelHandler&) = delete;

  ~RewardsPanelHandler() override;

  // brave_rewards::mojom::PanelHandler:
  void ShowUI() override;
  void CloseUI() override;
  void StartRewards(StartRewardsCallback callback) override;
  void GetRewardsPanelArgs(GetRewardsPanelArgsCallback callback) override;

 private:
  mojo::Receiver<brave_rewards::mojom::PanelHandler> receiver_;
  base::WeakPtr<ui::MojoBubbleWebUIController::Embedder> embedder_;
  raw_ptr<Profile> profile_ = nullptr;
  raw_ptr<brave_rewards::RewardsPanelCoordinator> panel_coordinator_ = nullptr;
};

#endif  // BRAVE_BROWSER_UI_WEBUI_BRAVE_REWARDS_REWARDS_PANEL_HANDLER_H_
