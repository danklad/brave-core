/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_THIRD_PARTY_BLINK_RENDERER_CORE_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_SHIELD_NODE_SHIELDS_H_
#define BRAVE_THIRD_PARTY_BLINK_RENDERER_CORE_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_SHIELD_NODE_SHIELDS_H_

#include "third_party/blink/renderer/platform/wtf/casting.h"

#include "brave/third_party/blink/renderer/core/brave_page_graph/types.h"

#include "brave/third_party/blink/renderer/core/brave_page_graph/graph_item/node/node.h"

namespace brave_page_graph {

class PageGraph;

class NodeShields final : public Node {
  friend class PageGraph;

 public:
  NodeShields() = delete;
  ~NodeShields() override;

  ItemName GetItemName() const override;

  bool IsNodeShields() const override;

 protected:
  NodeShields(PageGraph* const graph);
};

}  // namespace brave_page_graph

namespace blink {

template <>
struct DowncastTraits<brave_page_graph::NodeShields> {
  static bool AllowFrom(const brave_page_graph::Node& node) {
    return node.IsNodeShields();
  }
  static bool AllowFrom(const brave_page_graph::GraphItem& graph_item) {
    return IsA<brave_page_graph::NodeShields>(
        DynamicTo<brave_page_graph::Node>(graph_item));
  }
};

}  // namespace blink

#endif  // BRAVE_THIRD_PARTY_BLINK_RENDERER_CORE_BRAVE_PAGE_GRAPH_GRAPH_ITEM_NODE_SHIELD_NODE_SHIELDS_H_
