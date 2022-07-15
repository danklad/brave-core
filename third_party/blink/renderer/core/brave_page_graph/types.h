/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_THIRD_PARTY_BLINK_RENDERER_CORE_BRAVE_PAGE_GRAPH_TYPES_H_
#define BRAVE_THIRD_PARTY_BLINK_RENDERER_CORE_BRAVE_PAGE_GRAPH_TYPES_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "third_party/blink/renderer/core/core_export.h"

class GURL;

namespace blink {
using DOMNodeId = int;
class KURL;
enum class ResourceType : uint8_t;
class WebString;
}  // namespace blink

namespace v8 {
class Isolate;
}

namespace WTF {
class String;
}  // namespace WTF

namespace brave_page_graph {

class Element;
class Edge;
class EdgeRequestResponse;
class EdgeRequestStart;
class GraphItem;
class Node;
class NodeHTML;

typedef std::string ItemDesc;
typedef std::string ItemName;
typedef std::string GraphMLId;

typedef enum {
  kGraphMLAttrDefAttrName = 0,
  kGraphMLAttrDefBeforeNodeId,
  kGraphMLAttrDefBinding,
  kGraphMLAttrDefBindingEvent,
  kGraphMLAttrDefBindingType,
  kGraphMLAttrDefBlockType,
  kGraphMLAttrDefCallArgs,
  kGraphMLAttrDefEdgeType,
  kGraphMLAttrDefEventListenerId,
  kGraphMLAttrDefFrameId,
  kGraphMLAttrDefHost,
  kGraphMLAttrDefIncognito,
  kGraphMLAttrDefIsDeleted,
  kGraphMLAttrDefIsStyle,
  kGraphMLAttrDefKey,
  kGraphMLAttrDefMethodName,
  kGraphMLAttrDefNodeId,
  kGraphMLAttrDefNodeTag,
  kGraphMLAttrDefNodeText,
  kGraphMLAttrDefNodeType,
  kGraphMLAttrDefPageGraphEdgeId,
  kGraphMLAttrDefPageGraphNodeId,
  kGraphMLAttrDefPageGraphEdgeTimestamp,
  kGraphMLAttrDefPageGraphNodeTimestamp,
  kGraphMLAttrDefParentNodeId,
  kGraphMLAttrDefPrimaryPattern,
  kGraphMLAttrDefRequestId,
  kGraphMLAttrDefRequestType,
  kGraphMLAttrDefResourceType,
  kGraphMLAttrDefResponseHash,
  kGraphMLAttrDefRule,
  kGraphMLAttrDefScriptIdForEdge,
  kGraphMLAttrDefScriptIdForNode,
  kGraphMLAttrDefScriptPosition,
  kGraphMLAttrDefScriptType,
  kGraphMLAttrDefSecondaryPattern,
  kGraphMLAttrDefSource,
  kGraphMLAttrDefStatus,
  kGraphMLAttrDefSuccess,
  kGraphMLAttrDefURL,
  kGraphMLAttrDefValue,
  kGraphMLAttrDefUnknown,
  kGraphMLAttrDefSize,
  kGraphMLAttrDefHeaders,
} GraphMLAttrDef;

typedef enum {
  kGraphMLAttrTypeString = 0,
  kGraphMLAttrTypeBoolean,
  kGraphMLAttrTypeInt,
  kGraphMLAttrTypeLong,
  kGraphMLAttrTypeFloat,
  kGraphMLAttrTypeDouble,
  kGraphMLAttrTypeUnknown
} GraphMLAttrType;
CORE_EXPORT std::string GraphMLAttrTypeToString(
    const GraphMLAttrType type) noexcept;

typedef enum {
  kGraphMLAttrForTypeNode = 0,
  kGraphMLAttrForTypeEdge,
  kGraphMLAttrForTypeUnknown
} GraphMLAttrForType;
CORE_EXPORT std::string GraphMLForTypeToString(
    const GraphMLAttrForType type) noexcept;

typedef enum {
  kRequestTypeAJAX = 0,
  kRequestTypeAudio,
  kRequestTypeCSS,
  kRequestTypeDocument,
  kRequestTypeFont,
  kRequestTypeImage,
  kRequestTypeScriptClassic,
  kRequestTypeScriptModule,
  kRequestTypeSubdocument,
  kRequestTypeSVG,
  kRequestTypeVideo,
  kRequestTypeUnknown
} RequestType;
CORE_EXPORT std::string RequestTypeToString(const RequestType type) noexcept;

CORE_EXPORT std::string ResourceTypeToString(
    const blink::ResourceType type) noexcept;

typedef enum {
  kScriptTypeClassic = 0,
  kScriptTypeEval,
  kScriptTypeModule,
  kScriptTypeExtension,
  kExternalFile,
  kInline,
  kInlineInsideDocumentWrite,
  kInlineInsideGeneratedElement,
  kInternal,
  kJavascriptUrl,
  kEvalForScheduledAction,
  kInspector,
  kScriptTypeUnknown
} ScriptType;
CORE_EXPORT std::string ScriptTypeToString(const ScriptType type) noexcept;

typedef enum {
  kElementTypeDefault = 0,
  kElementTypeFrameOwner,
} ElementType;

typedef enum {
  kRequestStatusStart = 0,
  kRequestStatusComplete,
  kRequestStatusError,
  kRequestStatusBlocked
} RequestStatus;
CORE_EXPORT std::string RequestStatusToString(
    const RequestStatus status) noexcept;

typedef enum {
  kStorageLocationCookie = 0,
  kStorageLocationLocalStorage,
  kStorageLocationSessionStorage
} StorageLocation;
CORE_EXPORT std::string StorageLocationToString(
    const StorageLocation location) noexcept;

typedef enum {
  kJSBuiltInDateNow = 0,
  kJSBuiltInDateConstructor,
  kJSBuiltInDateParse,
  kJSBuiltInDateUTC,
  kJSBuiltInDatePrototypeSetDate,
  kJSBuiltInDatePrototypeSetFullYear,
  kJSBuiltInDatePrototypeSetHours,
  kJSBuiltInDatePrototypeSetMilliseconds,
  kJSBuiltInDatePrototypeSetMinutes,
  kJSBuiltInDatePrototypeSetMonth,
  kJSBuiltInDatePrototypeSetSeconds,
  kJSBuiltInDatePrototypeSetTime,
  kJSBuiltInDatePrototypeSetUTCDate,
  kJSBuiltInDatePrototypeSetUTCFullYear,
  kJSBuiltInDatePrototypeSetUTCHours,
  kJSBuiltInDatePrototypeSetUTCMilliseconds,
  kJSBuiltInDatePrototypeSetUTCMinutes,
  kJSBuiltInDatePrototypeSetUTCMonth,
  kJSBuiltInDatePrototypeSetUTCSeconds,
  kJSBuiltInDatePrototypeToDateString,
  kJSBuiltInDatePrototypeToISOString,
  kJSBuiltInDatePrototypeToString,
  kJSBuiltInDatePrototypeToTimeString,
  kJSBuiltInDatePrototypeToLocaleDateString,
  kJSBuiltInDatePrototypeToLocalString,
  kJSBuiltInDatePrototypeToLocalTimeString,
  kJSBuiltInDatePrototypeToUTCString,
  kJSBuiltInDatePrototypeGetYear,
  kJSBuiltInDatePrototypeSetYear,
  kJSBuiltInDatePrototypeToJSON,
  kJSBuiltInJSONParse,
  kJSBuiltInJSONStringify,
} JSBuiltIn;
CORE_EXPORT JSBuiltIn
JSBuiltInFromString(const std::string& built_in_str) noexcept;
CORE_EXPORT const std::string& JSBuiltInToSting(
    const JSBuiltIn built_in) noexcept;

typedef const char* BindingType;
CORE_EXPORT extern const BindingType kBindingTypeAttribute;
CORE_EXPORT extern const BindingType kBindingTypeConstant;
CORE_EXPORT extern const BindingType kBindingTypeConstructor;
CORE_EXPORT extern const BindingType kBindingTypeMethod;

typedef const char* BindingEvent;
CORE_EXPORT extern const BindingEvent kBindingEventAttributeGet;
CORE_EXPORT extern const BindingEvent kBindingEventAttributeSet;
CORE_EXPORT extern const BindingEvent kBindingEventConstantGet;
CORE_EXPORT extern const BindingEvent kBindingEventConstructorCall;
CORE_EXPORT extern const BindingEvent kBindingEventMethodCall;

typedef const char* Binding;

typedef unsigned SourceCodeHash;
typedef unsigned UrlHash;
typedef int ScriptId;
typedef int ScriptPosition;
typedef int EventListenerId;
typedef uint64_t PageGraphId;
typedef std::string MethodName;
typedef std::string RequestURL;
typedef uint64_t InspectorId;

struct CORE_EXPORT FingerprintingRule {
  const std::string& primary_pattern;
  const std::string& secondary_pattern;
  const std::string& source;
  const bool incognito;

  FingerprintingRule(const std::string& primary_pattern,
                     const std::string& secondary_pattern,
                     const std::string& source,
                     const bool incognito);

  bool operator==(const FingerprintingRule& other) const;
  bool operator<(const FingerprintingRule& other) const;
  bool operator>(const FingerprintingRule& other) const;

  std::string ToString() const;
};

struct CORE_EXPORT EventListener {
  const std::string& event_type;
  const ScriptId listener_script_id;

  EventListener(const std::string& event_type,
                ScriptId const listener_script_id)
      : event_type(event_type), listener_script_id(listener_script_id) {}
};

typedef std::vector<const Edge*> EdgeList;
typedef std::vector<Node*> NodeList;
typedef std::vector<std::unique_ptr<const Edge>> EdgeUniquePtrList;
typedef std::vector<std::unique_ptr<Node>> NodeUniquePtrList;
typedef std::vector<const GraphItem*> GraphItemList;
typedef std::vector<NodeHTML*> HTMLNodeList;
typedef std::vector<blink::DOMNodeId> DOMNodeIdList;
typedef std::vector<ScriptId> ScriptIdList;
typedef std::map<const std::string, const std::string> AttributeMap;
typedef std::map<EventListenerId, const EventListener> EventListenerMap;

typedef std::map<SourceCodeHash, ScriptId> HashToScriptIdMap;
typedef std::map<ScriptId, SourceCodeHash> ScriptIdToHashMap;
typedef std::map<SourceCodeHash, UrlHash> SourceToUrlMap;
typedef std::map<UrlHash, SourceCodeHash> UrlToSourceMap;

}  // namespace brave_page_graph

#endif  // BRAVE_THIRD_PARTY_BLINK_RENDERER_CORE_BRAVE_PAGE_GRAPH_TYPES_H_
