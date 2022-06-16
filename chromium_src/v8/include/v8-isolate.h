/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_CHROMIUM_SRC_V8_INCLUDE_V8_ISOLATE_H_
#define BRAVE_CHROMIUM_SRC_V8_INCLUDE_V8_ISOLATE_H_

#define SetRAILMode                                                         \
  NotUsed();                                                                \
                                                                            \
  struct V8_EXPORT ExecutingScript {                                        \
    int script_id = 0;                                                      \
    int script_position = 0;                                                \
  };                                                                        \
                                                                            \
  struct V8_EXPORT PageGraphBackend {                                       \
    using EvalScriptCompiledCallback = void (*)(Isolate * isolate,          \
                                                const int parent_script_id, \
                                                const int script_id,        \
                                                Local<String> source);      \
    using BuiltInFuncCallCallback =                                         \
        void (*)(Isolate * isolate,                                         \
                 const char* builtin_name,                                  \
                 const std::vector<std::string>& args);                     \
    using BuiltInFuncResponseCallback = void (*)(Isolate * isolate,         \
                                                 const char* builtin_name,  \
                                                 const std::string& value); \
                                                                            \
    EvalScriptCompiledCallback eval_script_compiled_cb = nullptr;           \
    BuiltInFuncCallCallback builtin_func_call_cb = nullptr;                 \
    BuiltInFuncResponseCallback builtin_func_response_cb = nullptr;         \
  };                                                                        \
                                                                            \
  ExecutingScript GetExecutingScript(bool include_position = false);        \
  std::vector<ExecutingScript> GetAllExecutingScripts();                    \
  void SetPageGraphBackend(PageGraphBackend* page_graph_backend);           \
  void SetRAILMode

#include "src/v8/include/v8-isolate.h"

#undef SetRAILMode

#endif  // BRAVE_CHROMIUM_SRC_V8_INCLUDE_V8_ISOLATE_H_
