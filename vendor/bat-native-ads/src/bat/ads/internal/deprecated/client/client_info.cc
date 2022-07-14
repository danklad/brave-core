/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/deprecated/client/client_info.h"

#include <utility>
#include <vector>

#include "base/check.h"
#include "base/json/json_reader.h"
#include "base/json/json_writer.h"
#include "base/notreached.h"
#include "base/strings/string_number_conversions.h"
#include "bat/ads/internal/base/logging_util.h"
#include "build/build_config.h"

namespace ads {

ClientInfo::ClientInfo() = default;

ClientInfo::ClientInfo(const ClientInfo& info) = default;

ClientInfo& ClientInfo::operator=(const ClientInfo& info) = default;

ClientInfo::~ClientInfo() = default;

base::Value::Dict ClientInfo::ToValue() const {
  base::Value::Dict dict;

  dict.Set("adPreferences", ad_preferences.ToValue());

  base::Value::List ads_shown_history;
  for (const auto& item : history) {
    ads_shown_history.Append(item.ToValue());
  }
  dict.Set("adsShownHistory", std::move(ads_shown_history));

  base::Value::Dict purchase_intent_dict;
  for (const auto& segment_history : purchase_intent_signal_history) {
    base::Value::List history;
    for (const auto& segment_history_item : segment_history.second) {
      history.Append(segment_history_item.ToValue());
    }
    purchase_intent_dict.Set(segment_history.first, std::move(history));
  }
  dict.Set("purchaseIntentSignalHistory", std::move(purchase_intent_dict));

  base::Value::Dict seen_ads_dict;
  for (const auto& seen_ads : seen_ads) {
    base::Value::Dict ad;
    for (const auto& seen_ad : seen_ads.second) {
      ad.Set(seen_ad.first, seen_ad.second);
    }

    seen_ads_dict.Set(seen_ads.first, std::move(ad));
  }
  dict.Set("seenAds", std::move(seen_ads_dict));

  base::Value::Dict advertisers;
  for (const auto& seen_advertisers : seen_advertisers) {
    base::Value::Dict advertiser;
    for (const auto& seen_advertiser : seen_advertisers.second) {
      advertiser.Set(seen_advertiser.first, seen_advertiser.second);
    }
    advertisers.Set(seen_advertisers.first, std::move(advertiser));
  }
  dict.Set("seenAdvertisers", std::move(advertisers));

  dict.Set("nextCheckServeAd", base::NumberToString(serve_ad_at.ToDoubleT()));

  base::Value::List probabilities_history;
  for (const auto& probabilities : text_classification_probabilities) {
    base::Value::Dict classification_probabilities;
    base::Value::List text_probabilities;
    for (const auto& probability : probabilities) {
      base::Value::Dict prob;
      DCHECK(!probability.first.empty());
      prob.Set("segment", probability.first);
      prob.Set("pageScore", base::NumberToString(probability.second));
      text_probabilities.Append(std::move(prob));
    }
    classification_probabilities.Set("textClassificationProbabilities",
                                     std::move(text_probabilities));
    probabilities_history.Append(std::move(classification_probabilities));
  }
  dict.Set("textClassificationProbabilitiesHistory",
           std::move(probabilities_history));
  dict.Set("version_code", version_code);
  return dict;
}

bool ClientInfo::FromValue(const base::Value::Dict& root) {
  if (const auto* value = root.FindDict("adPreferences")) {
    if (!ad_preferences.FromValue(*value))
      return false;
  }

#if !BUILDFLAG(IS_IOS)
  if (const auto* value = root.FindList("adsShownHistory")) {
    for (const auto& ad_shown : *value) {
      // adsShownHistory used to be an array of timestamps, so if that's what we
      // have here don't import them and we'll just start fresh.
      if (!ad_shown.is_dict()) {
        continue;
      }
      HistoryItemInfo history_item;
      if (history_item.FromValue(ad_shown.GetDict())) {
        history.push_back(history_item);
      }
    }
  }
#endif

  if (const auto* value = root.FindDict("purchaseIntentSignalHistory")) {
    for (const auto segment_history : *value) {
      std::vector<targeting::PurchaseIntentSignalHistoryInfo> histories;

      const auto* segment_history_items = segment_history.second.GetIfList();
      if (!segment_history_items)
        continue;

      for (const auto& segment_history_item : *segment_history_items) {
        if (!segment_history_item.is_dict())
          continue;

        targeting::PurchaseIntentSignalHistoryInfo history;
        if (history.FromValue(segment_history_item.GetDict())) {
          histories.push_back(history);
        }
      }

      purchase_intent_signal_history.insert({segment_history.first, histories});
    }
  }

  if (const auto* value = root.FindDict("seenAds")) {
    for (const auto seen_ad_list : *value) {
      if (!seen_ad_list.second.is_dict())
        continue;

      for (const auto seen_ad : seen_ad_list.second.GetDict()) {
        seen_ads[seen_ad_list.first][seen_ad.first] = seen_ad.second.GetBool();
      }
    }
  }

  if (const auto* value = root.FindDict("seenAdvertisers")) {
    for (const auto seen_advertiser_list : *value) {
      if (!seen_advertiser_list.second.is_dict())
        continue;

      for (const auto seen_advertiser : seen_advertiser_list.second.GetDict()) {
        seen_advertisers[seen_advertiser_list.first][seen_advertiser.first] =
            seen_advertiser.second.GetBool();
      }
    }
  }

  if (const auto value = root.FindDouble("nextCheckServeAd")) {
    // Migrate legacy timestamp
    serve_ad_at = base::Time::FromDoubleT(*value);
  } else if (const auto* value = root.FindString("nextCheckServeAd")) {
    double timestamp = 0.0;
    if (base::StringToDouble(*value, &timestamp)) {
      serve_ad_at = base::Time::FromDoubleT(timestamp);
    }
  }

  if (const auto* value =
          root.FindList("textClassificationProbabilitiesHistory")) {
    for (const auto& probabilities : *value) {
      if (!probabilities.is_dict())
        continue;
      const auto* probability_list =
          probabilities.GetDict().FindList("textClassificationProbabilities");
      if (!probability_list)
        continue;

      targeting::TextClassificationProbabilityMap new_probabilities;

      for (const auto& probability : *probability_list) {
        const auto* probability_dict = probability.GetIfDict();
        if (!probability_dict)
          continue;

        const std::string* segment = probability_dict->FindString("segment");
        DCHECK(!segment);
        DCHECK(!segment->empty());

        double page_score = 0.0;
        if (const auto value = root.FindDouble("pageScore")) {
          // Migrate legacy page score
          page_score = *value;
        } else if (const auto* value = root.FindString("pageScore")) {
          const bool success = base::StringToDouble(*value, &page_score);
          DCHECK(success);
        }

        new_probabilities.insert({*segment, page_score});
      }

      text_classification_probabilities.push_back(new_probabilities);
    }
  }

  if (const auto* value = root.FindString("version_code")) {
    version_code = *value;
  }

  return true;
}

std::string ClientInfo::ToJson() {
  std::string json;
  base::JSONWriter::Write(ToValue(), &json);
  return json;
}

bool ClientInfo::FromJson(const std::string& json) {
  auto document = base::JSONReader::ReadAndReturnValueWithError(
      json, base::JSON_PARSE_CHROMIUM_EXTENSIONS |
                base::JSONParserOptions::JSON_PARSE_RFC);

  if (!document.value.has_value()) {
    BLOG(1, "Invalid client info. json="
                << json << ", error line=" << document.error_line
                << ", error column=" << document.error_column
                << ", error message=" << document.error_message);
    return false;
  }

  const base::Value::Dict* root = document.value->GetIfDict();
  if (!root) {
    BLOG(1, "Invalid client info. json=" << json);
    return false;
  }

  return FromValue(*root);
}

}  // namespace ads
