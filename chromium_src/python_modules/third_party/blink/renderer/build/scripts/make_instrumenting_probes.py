# Copyright (c) 2022 The Brave Authors. All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this file,
# You can obtain one at http://mozilla.org/MPL/2.0/. */

import re

import lib.override_utils as override_utils


def _brave_extend_config(config):
    config["settings"]["includes"].extend([
        "brave/third_party/blink/renderer/core/brave_page_graph/blink_converters.h",
        "brave/third_party/blink/renderer/core/brave_page_graph/probe_types.h",
        "brave/third_party/blink/renderer/core/brave_page_graph/types.h",
    ])
    config["observers"]["PageGraphAgent"] = {
        "include_path":
        "brave/third_party/blink/renderer/core/brave_page_graph",
        "probes": [
            "NodeCreated",
            "DidCommitLoad",
            "DidInsertDOMNode",
            "WillRemoveDOMNode",
            "ConsoleMessageAdded",
            "DidModifyDOMAttr",
            "DidRemoveDOMAttr",
            "WillSendNavigationRequest",
            "PrepareRequest",
            "DidReceiveResourceResponse",
            "DidReceiveData",
            "DidReceiveBlob",
            "DidFinishLoading",
            "DidFailLoading",
            "RegisterPageGraphNodeFullyCreated",
            "RegisterPageGraphElmForLocalScript",
            "RegisterPageGraphElmForRemoteScript",
            "RegisterPageGraphModuleScriptForDescendant",
            "RegisterPageGraphScriptCompilation",
            "RegisterPageGraphModuleCompilation",
            "RegisterPageGraphScriptCompilationFromAttr",
            "RegisterPageGraphBindingEvent",
            "RegisterPageGraphWebAPICallWithResult",
            "RegisterPageGraphEventListenerAdd",
            "RegisterPageGraphEventListenerRemove",
            "RegisterPageGraphRemoteFrameCreated",
        ]
    }

    return config


def _brave_extend_pidl_source(source):
    insert_after = "void DidClearDocumentOfWindowObject([Keep] LocalFrame*);"
    idx = source.find(insert_after)
    assert idx != -1
    idx += len(insert_after)
    ext = """
        void RegisterPageGraphBindingEvent(ExecutionContext*, base::StringPiece name, const char* type, const char* event);
        void RegisterPageGraphWebAPICallWithResult(ExecutionContext*, base::StringPiece name, const PageGraphBlinkReceiverData& receiver_data, const PageGraphBlinkArgs& args, const ExceptionState* exception_state, const absl::optional<String>& result);
        void RegisterPageGraphNodeFullyCreated([Keep] Node* node);
        void RegisterPageGraphElmForLocalScript(ExecutionContext*, DOMNodeId dom_node_id, const String& source_text);
        void RegisterPageGraphElmForRemoteScript(ExecutionContext*, DOMNodeId dom_node_id, const KURL& url);
        void RegisterPageGraphModuleScriptForDescendant(ExecutionContext*, int script_id, const KURL& url);
        void RegisterPageGraphScriptCompilation(ExecutionContext*, const ClassicScript& classic_script, v8::Local<v8::Script> script);
        void RegisterPageGraphScriptCompilationFromAttr([Keep] EventTarget*, const String& function_name, const String& script_body, v8::Local<v8::Function> compiled_function);
        void RegisterPageGraphModuleCompilation(ExecutionContext*, const ModuleScriptCreationParams& params, v8::Local<v8::Module> script);
        void RegisterPageGraphEventListenerAdd([Keep] EventTarget*, const String& event_type, RegisteredEventListener* registered_listener);
        void RegisterPageGraphEventListenerRemove([Keep] EventTarget*, const String& event_type, RegisteredEventListener* registered_listener);
        void RegisterPageGraphRemoteFrameCreated(ExecutionContext*, HTMLFrameOwnerElement*);
    """

    return source[:idx] + ext + source[idx:]


@override_utils.override_function(globals())
def load_config(original_function, file_name):
    config = original_function(file_name)
    if file_name.endswith("core_probes.json5"):
        config = _brave_extend_config(config)
    return config


@override_utils.override_function(globals())
def load_model_from_idl(original_function, source):
    source = _brave_extend_pidl_source(source)
    return original_function(source)
