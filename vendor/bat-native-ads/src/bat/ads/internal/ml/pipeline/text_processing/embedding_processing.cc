/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ml/pipeline/text_processing/embedding_processing.h"

#include <iostream>

#include "base/check.h"
#include "base/values.h"
#include "bat/ads/internal/ml/data/text_data.h"
#include "bat/ads/internal/ml/data/vector_data.h"
#include "bat/ads/internal/ml/pipeline/pipeline_embedding_info.h"
#include "bat/ads/internal/ml/pipeline/pipeline_util.h"
#include "third_party/abseil-cpp/absl/types/optional.h"
#include "bat/ads/internal/base/strings/string_strip_util.h"
#include "bat/ads/internal/base/strings/string_html_parse_util.h"

namespace ads {
namespace ml {
namespace pipeline {

// static
std::unique_ptr<EmbeddingProcessing> EmbeddingProcessing::CreateFromValue(
    base::Value resource_value,
    std::string* error_message) {
  DCHECK(error_message);

  auto text_processing = std::make_unique<EmbeddingProcessing>();
  if (!text_processing->FromValue(std::move(resource_value))) {
    *error_message = "Failed to parse text embedding pipeline JSON";
    return {};
  }

  return text_processing;
}

bool EmbeddingProcessing::IsInitialized() const {
  return is_initialized_;
}

EmbeddingProcessing::EmbeddingProcessing() : is_initialized_(false) {}

EmbeddingProcessing::~EmbeddingProcessing() = default;

void EmbeddingProcessing::SetInfo(const PipelineEmbeddingInfo& info) {
  version_ = info.version;
  timestamp_ = info.timestamp;
  locale_ = info.locale;
  embeddings_dim_ = info.embeddings_dim;
  embeddings_ = info.embeddings;
}

bool EmbeddingProcessing::FromValue(base::Value resource_value) {
  absl::optional<PipelineEmbeddingInfo> pipeline_embedding_info =
      ParsePipelineEmbedding(std::move(resource_value));

  if (pipeline_embedding_info.has_value()) {
    SetInfo(pipeline_embedding_info.value());
    is_initialized_ = true;
  } else {
    is_initialized_ = false;
  }

  return is_initialized_;
}

std::string EmbeddingProcessing::CleanText(const std::string& text, bool is_html) {
  std::string cleaned_text = text;
  if (is_html) {
    cleaned_text = ParseTagAttribute(cleaned_text, "og:title", "content");
  }
  cleaned_text = StripNonAlphaCharacters(cleaned_text);
  std::transform(cleaned_text.begin(), cleaned_text.end(), cleaned_text.begin(), ::tolower);
  return cleaned_text;
}

VectorData EmbeddingProcessing::EmbedText(const std::string& text) const {

  std::vector<float> embedding_initialize(embeddings_dim_, 0.0);
  VectorData embedding_vector = VectorData(embedding_initialize);

  if (!IsInitialized()) {
    return embedding_vector;
  }

  float n_tokens = 0.0;
  char text_c[text.length() + 1];
  std::strcpy(text_c, text.c_str());
  char *token = std::strtok(text_c, " ");
  while (token != NULL) {

    std::cout << "\n";
    std::cout << token;

    const auto iter = embeddings_.find(token);
    if (iter != embeddings_.end()) {

      std::cout << " - token found";
      embedding_vector.VectorAddElementWise(iter->second);
      n_tokens += 1.0;
    }
    token = strtok(NULL, " ");
  }

  if (n_tokens == 0.0) {
    return embedding_vector;
  }

  embedding_vector.VectorDivideByScalar(n_tokens);
  return embedding_vector;  
}

}  // namespace pipeline
}  // namespace ml
}  // namespace ads
