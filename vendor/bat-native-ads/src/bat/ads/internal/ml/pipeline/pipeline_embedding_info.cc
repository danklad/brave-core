/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ml/pipeline/pipeline_embedding_info.h"

#include <utility>


namespace ads {
namespace ml {
namespace pipeline {

PipelineEmbeddingInfo::PipelineEmbeddingInfo() = default;

PipelineEmbeddingInfo::PipelineEmbeddingInfo(PipelineEmbeddingInfo&& info) noexcept = default;

PipelineEmbeddingInfo& PipelineEmbeddingInfo::operator=(PipelineEmbeddingInfo&& info) noexcept = default;

PipelineEmbeddingInfo::~PipelineEmbeddingInfo() = default;

PipelineEmbeddingInfo::PipelineEmbeddingInfo(const int version,
                           const std::string& timestamp,
                           const std::string& locale,
                           const int embeddings_dim,
                           const std::map<std::string, VectorData>& embeddings)
    : version(version),
      timestamp(timestamp),
      locale(locale),
      embeddings_dim(embeddings_dim),
      embeddings(embeddings) {
}

}  // namespace pipeline
}  // namespace ml
}  // namespace ads
