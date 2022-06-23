/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_PROCESSORS_CONTEXTUAL_TEXT_EMBEDDING_TEXT_EMBEDDING_PROCESSOR_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_PROCESSORS_CONTEXTUAL_TEXT_EMBEDDING_TEXT_EMBEDDING_PROCESSOR_H_

#include <string>

#include "base/memory/raw_ptr.h"
#include "bat/ads/internal/processors/processor_interface.h"

namespace ads {

namespace resource {
class TextEmbedding;
}  // namespace resource

namespace processor {

class TextEmbedding final : public ProcessorInterface<std::string> {
 public:
  explicit TextEmbedding(resource::TextEmbedding* resource);
  ~TextEmbedding() override;
  TextEmbedding(const TextEmbedding&) = delete;
  TextEmbedding& operator=(const TextEmbedding&) = delete;

  bool IsEmbeddingEnabled();
  
  void Process(const std::string& text) override;

 private:
  raw_ptr<resource::TextEmbedding> resource_ = nullptr;
};

}  // namespace processor
}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_PROCESSORS_CONTEXTUAL_TEXT_EMBEDDING_TEXT_EMBEDDING_PROCESSOR_H_
