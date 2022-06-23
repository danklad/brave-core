/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_PROCESSORS_CONTEXTUAL_TEXT_EMBEDDING_TEXT_EMBEDDING_INFO_H_
#define BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_PROCESSORS_CONTEXTUAL_TEXT_EMBEDDING_TEXT_EMBEDDING_INFO_H_

#include <string>

#include "url/gurl.h"
#include "bat/ads/export.h"

namespace ads {

struct ADS_EXPORT TextEmbeddingInfo {
  TextEmbeddingInfo();
  TextEmbeddingInfo(const TextEmbeddingInfo& info);
  TextEmbeddingInfo& operator=(const TextEmbeddingInfo& info);
  ~TextEmbeddingInfo();

  bool operator==(const TextEmbeddingInfo& rhs) const;
  bool operator!=(const TextEmbeddingInfo& rhs) const;

  bool IsValid() const;

  std::string version;
  std::string locale;
  std::string embedding;
};

}  // namespace ads

#endif  // BRAVE_VENDOR_BAT_NATIVE_ADS_SRC_BAT_ADS_INTERNAL_PROCESSORS_CONTEXTUAL_TEXT_EMBEDDING_TEXT_EMBEDDING_INFO_H_
