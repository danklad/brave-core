/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_SCHEDULED_ACTION_H_
#define BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_SCHEDULED_ACTION_H_

namespace blink {
class ScriptFetchOptions;
}

#define arguments_                                  \
  arguments_;                                       \
  ScriptFetchOptions GetScriptFetchOptions() const; \
  int parent_script_id_ = 0

#include "src/third_party/blink/renderer/bindings/core/v8/scheduled_action.h"

#undef arguments_

#endif  // BRAVE_CHROMIUM_SRC_THIRD_PARTY_BLINK_RENDERER_BINDINGS_CORE_V8_SCHEDULED_ACTION_H_
