/*
 * Copyright 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <memory>
#include <unordered_map>

#include "hci/acl_manager.h"
#include "hci/address.h"
#include "l2cap/internal/classic_link.h"
#include "l2cap/internal/scheduler_fifo.h"
#include "l2cap/l2cap_layer.h"
#include "os/handler.h"
#include "os/log.h"

#include "l2cap/internal/classic_link_manager.h"

namespace bluetooth {
namespace l2cap {
namespace internal {

void ClassicLinkManager::ConnectFixedChannelServices(hci::Address device,
                                                     PendingFixedChannelConnection pending_fixed_channel_connection) {
  // Check if there is any service registered
  auto fixed_channel_services = service_manager_->GetRegisteredServices();
  if (fixed_channel_services.empty()) {
    // If so, return error
    pending_fixed_channel_connection.handler_->Post(common::BindOnce(
        std::move(pending_fixed_channel_connection.on_fail_callback_),
        ClassicFixedChannelManager::ConnectionResult{
            .connection_result_code = ClassicFixedChannelManager::ConnectionResultCode::FAIL_NO_SERVICE_REGISTERED}));
    return;
  }
  // Otherwise, check if device has an ACL connection
  auto* link = GetLink(device);
  if (link != nullptr) {
    // If device already have an ACL connection
    // Check if all registered services have an allocated channel and allocate one if not already allocated
    int num_new_channels = 0;
    for (auto& fixed_channel_service : fixed_channel_services) {
      if (link->fixed_channel_allocator_.IsChannelInUse(fixed_channel_service.first)) {
        // This channel is already allocated for this link, do not allocated twice
        continue;
      }
      // Allocate channel for newly registered fixed channels
      auto* classic_fixed_channel_impl =
          link->fixed_channel_allocator_.AllocateChannel(fixed_channel_service.first, SecurityPolicy());
      fixed_channel_service.second->NotifyChannelCreation(classic_fixed_channel_impl->GetChannelInterface());
      num_new_channels++;
    }
    // Declare connection failure if no new channels are created
    if (num_new_channels == 0) {
      pending_fixed_channel_connection.handler_->Post(
          common::BindOnce(std::move(pending_fixed_channel_connection.on_fail_callback_),
                           ClassicFixedChannelManager::ConnectionResult{
                               .connection_result_code =
                                   ClassicFixedChannelManager::ConnectionResultCode::FAIL_ALL_SERVICES_HAVE_CHANNEL}));
    }
    // No need to create ACL connection, return without saving any pending connections
    return;
  }
  // If not, create new ACL connection
  // Add request to pending link list first
  auto pending_link = pending_links_.find(device);
  if (pending_link == pending_links_.end()) {
    // Create pending link if not exist
    pending_links_.try_emplace(device);
    pending_link = pending_links_.find(device);
  }
  pending_link->second.pending_fixed_channel_connections_.push_back(std::move(pending_fixed_channel_connection));
  // Then create new ACL connection
  acl_manager_->CreateConnection(device);
}

ClassicLink* ClassicLinkManager::GetLink(const hci::Address device) {
  if (links_.find(device) == links_.end()) {
    return nullptr;
  }
  return &links_.find(device)->second;
}

void ClassicLinkManager::OnConnectSuccess(std::unique_ptr<hci::AclConnection> acl_connection) {
  // Same link should not be connected twice
  hci::Address address = acl_connection->GetAddress();
  ASSERT_LOG(GetLink(address) == nullptr, "%s is connected twice without disconnection",
             acl_connection->GetAddress().ToString().c_str());
  auto* link_queue_up_end = acl_connection->GetAclQueueEnd();
  links_.try_emplace(address, handler_, std::move(acl_connection), std::make_unique<Fifo>(link_queue_up_end, handler_));
  auto* link = GetLink(address);
  // Allocate and distribute channels for all registered fixed channel services
  auto fixed_channel_services = service_manager_->GetRegisteredServices();
  for (auto& fixed_channel_service : fixed_channel_services) {
    ClassicFixedChannelImpl* classic_fixed_channel_impl =
        link->fixed_channel_allocator_.AllocateChannel(fixed_channel_service.first, SecurityPolicy());
    fixed_channel_service.second->NotifyChannelCreation(classic_fixed_channel_impl->GetChannelInterface());
  }
  // Remove device from pending links list, if any
  auto pending_link = pending_links_.find(address);
  if (pending_link == pending_links_.end()) {
    // This an incoming connection, exit
    return;
  }
  // This is an outgoing connection, remove entry in pending link list
  pending_links_.erase(pending_link);
}

void ClassicLinkManager::OnConnectFail(hci::Address device, hci::ErrorCode reason) {
  // Notify all pending links for this device
  auto pending_link = pending_links_.find(device);
  if (pending_link == pending_links_.end()) {
    // There is no pending link, exit
    return;
  }
  for (auto& pending_fixed_channel_connection : pending_link->second.pending_fixed_channel_connections_) {
    pending_fixed_channel_connection.handler_->Post(
        common::BindOnce(std::move(pending_fixed_channel_connection.on_fail_callback_),
                         ClassicFixedChannelManager::ConnectionResult{
                             .connection_result_code = ClassicFixedChannelManager::ConnectionResultCode::FAIL_HCI_ERROR,
                             .hci_error = reason}));
  }
  // Remove entry in pending link list
  pending_links_.erase(pending_link);
}

void ClassicLinkManager::OnDisconnect(hci::Address device, hci::ErrorCode status) {
  links_.erase(device);
}

}  // namespace internal
}  // namespace l2cap
}  // namespace bluetooth
