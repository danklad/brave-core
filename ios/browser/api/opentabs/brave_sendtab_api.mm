/* Copyright (c) 2022 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/ios/browser/api/opentabs/brave_sendtab_api.h"
// #include "brave/ios/browser/api/opentabs/brave_sendtab_observer.h"
// #include "brave/ios/browser/api/opentabs/sendtab_session_listener_ios.h"

#include "base/bind.h"
#include "base/strings/sys_string_conversions.h"

#include "components/send_tab_to_self/send_tab_to_self_model.h"
#include "components/send_tab_to_self/send_tab_to_self_sync_service.h"
#include "components/send_tab_to_self/target_device_info.h"

#include "ios/chrome/browser/browser_state/chrome_browser_state.h"
#include "ios/chrome/browser/ui/recent_tabs/synced_sessions.h"
#include "ios/chrome/browser/sync/send_tab_to_self_sync_service_factory.h"

#include "ios/web/public/thread/web_thread.h"
#include "net/base/mac/url_conversions.h"
#include "url/gurl.h"

namespace brave {
namespace ios {
TargetDeviceType DeviceTypeFromSyncDeviceType(
    sync_pb::SyncEnums::DeviceType deviceType) {
  switch (deviceType) {
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_WIN:
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_MAC:
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_LINUX:
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_CROS:
      return TargetDeviceTypePC;
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_PHONE:
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_TABLET:
      return TargetDeviceTypeMobile;
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_UNSET:
    case sync_pb::SyncEnums::DeviceType::SyncEnums_DeviceType_TYPE_OTHER:
      return TargetDeviceTypeUnset;
  }
}
}  // namespace ios
}  // namespace brave

#pragma mark - IOSSendTabTargetDevice

@implementation IOSSendTabTargetDevice

- (instancetype)initWithFullName:(NSString*)fullName
                       shortName:(NSString*)shortName
                      deviceName:(NSString*)deviceName
                         cacheId:(NSString*)cacheId
                      deviceType:(TargetDeviceType)deviceType
                 lastUpdatedTime:(NSDate*)lastUpdatedTime {
  if ((self = [super init])) {
    self.fullName = fullName;
    self.shortName = shortName;
    self.deviceName = deviceName;
    self.cacheId = cacheId;
    self.deviceType = deviceType;
    self.lastUpdatedTime = lastUpdatedTime;
  }

  return self;
}

- (id)copyWithZone:(NSZone*)zone {
  IOSSendTabTargetDevice* targetDevice = [[[self class] allocWithZone:zone] init];

  if (targetDevice) {
    targetDevice.fullName = self.fullName;
    targetDevice.shortName = self.shortName;
    targetDevice.deviceName = self.deviceName;
    targetDevice.cacheId = self.cacheId;
    targetDevice.deviceType = self.deviceType;
    targetDevice.lastUpdatedTime = self.lastUpdatedTime;
  }

  return targetDevice;
}

@end

#pragma mark - BraveSendTabAPI

@interface BraveSendTabAPI () {
  // SendTab Sync Service is needed in order to send session data to
  // different devices - receive device information
  send_tab_to_self::SendTabToSelfSyncService* sendtab_sync_service_;

  // The list of devices with thier names, cache_guids, device types,
  // and active times.
  std::vector<send_tab_to_self::TargetDeviceInfo> target_device_list_;
}
@end

@implementation BraveSendTabAPI

- (instancetype)initWithSyncService:(send_tab_to_self::SendTabToSelfSyncService*)syncService {
  if ((self = [super init])) {
    sendtab_sync_service_ = syncService;
  }
  return self;
}

- (void)dealloc {
  sendtab_sync_service_ = nullptr;
}

// - (id<SendTabSessionStateListener>)addObserver:(id<SendTabSessionStateObserver>)observer {
//   return [[<SendTabSessionListenerImpl alloc] init:observer
//                                        syncService:sync_service_];
// }

// - (void)removeObserver:(id<SendTabSessionStateListener>)observer {
//   [observer destroy];
// }

- (NSArray<IOSSendTabTargetDevice*>*)getListOfSyncedDevices {
  DCHECK(sendtab_sync_service_);

  NSMutableArray<IOSSendTabTargetDevice*>* targetDeviceList = [[NSMutableArray alloc] init];

  target_device_list_ = sendtab_sync_service_->GetSendTabToSelfModel()->GetTargetDeviceInfoSortedList();

  for (const auto& device : target_device_list_) {
        IOSSendTabTargetDevice* targetDevice = [[IOSSendTabTargetDevice alloc] 
            initWithFullName:base::SysUTF8ToNSString(device.full_name)
                   shortName:base::SysUTF8ToNSString(device.short_name)
                  deviceName:base::SysUTF8ToNSString(device.device_name)
                     cacheId:base::SysUTF8ToNSString(device.cache_guid)
                  deviceType:brave::ios::DeviceTypeFromSyncDeviceType(device.device_type)
             lastUpdatedTime:device.last_updated_timestamp.ToNSDate()];

    [targetDeviceList addObject: targetDevice];
  }
  
  return [targetDeviceList copy];
}

@end
