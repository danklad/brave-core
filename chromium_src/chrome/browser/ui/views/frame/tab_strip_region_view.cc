/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/ui/views/tabs/brave_new_tab_button.h"
#include "brave/browser/ui/views/tabs/brave_tab_search_button.h"

#define NewTabButton BraveNewTabButton
#define TabSearchButton BraveTabSearchButton
#define BRAVE_UI_VIEWS_FRAME_TAB_STRIP_REGION_CHANGE_ORIENTATION \
  layout_manager_->SetOrientation(views::LayoutOrientation::kVertical);
#include "src/chrome/browser/ui/views/frame/tab_strip_region_view.cc"
#undef BRAVE_UI_VIEWS_FRAME_TAB_STRIP_REGION_CHANGE_ORIENTATION
#undef TabSearchButton
#undef NewTabButton
