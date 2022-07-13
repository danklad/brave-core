/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_PROCESSORS_CONTEXTUAL_TEXT_EMBEDDING_TEXT_EMBEDDING_HTML_EVENTS_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_PROCESSORS_CONTEXTUAL_TEXT_EMBEDDING_TEXT_EMBEDDING_HTML_EVENTS_H_

#include <functional>
#include <vector>

#include "bat/ads/public/interfaces/ads.mojom.h"

namespace base {
class Time;
}  // namespace base

namespace ads {

struct TextEmbeddingInfo;
struct TextEmbeddingEventInfo;

using TextEmbeddingHTMLEventCallback = std::function<void(const bool)>;

void LogTextEmbeddingHTMLEvent(const TextEmbeddingInfo& text_embedding_info, TextEmbeddingHTMLEventCallback callback);

void LogTextEmbeddingHTMLEvent(const TextEmbeddingEventInfo& text_embedding_html_event, TextEmbeddingHTMLEventCallback callback);

void PurgeStaleTextEmbeddingHTMLEvents(TextEmbeddingHTMLEventCallback callback);

void GetTextEmbeddingEventsFromDatabase();

}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_PROCESSORS_CONTEXTUAL_TEXT_EMBEDDING_TEXT_EMBEDDING_HTML_EVENTS_H_
