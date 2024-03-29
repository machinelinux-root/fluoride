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

#define PACKET_TESTING
#include "hci/hci_packets.h"

#include <gtest/gtest.h>
#include <forward_list>
#include <memory>

#include "os/log.h"
#include "packet/bit_inserter.h"
#include "packet/raw_builder.h"

using bluetooth::packet::BitInserter;
using bluetooth::packet::RawBuilder;
using std::vector;

namespace bluetooth {
namespace hci {

std::vector<uint8_t> reset = {0x03, 0x0c, 0x00};
DEFINE_AND_INSTANTIATE_ResetReflectionTest(reset);

std::vector<uint8_t> reset_complete = {0x0e, 0x04, 0x01, 0x03, 0x0c, 0x00};
DEFINE_AND_INSTANTIATE_ResetCompleteReflectionTest(reset_complete);

std::vector<uint8_t> read_buffer_size = {0x05, 0x10, 0x00};
DEFINE_AND_INSTANTIATE_ReadBufferSizeReflectionTest(read_buffer_size);

std::vector<uint8_t> read_buffer_size_complete = {0x0e, 0x0b, 0x01, 0x05, 0x10, 0x00, 0x00,
                                                  0x04, 0x3c, 0x07, 0x00, 0x08, 0x00};
DEFINE_AND_INSTANTIATE_ReadBufferSizeCompleteReflectionTest(read_buffer_size_complete);

std::vector<uint8_t> host_buffer_size = {0x33, 0x0c, 0x07, 0x9b, 0x06, 0xff, 0x14, 0x00, 0x0a, 0x00};
DEFINE_AND_INSTANTIATE_HostBufferSizeReflectionTest(host_buffer_size);

std::vector<uint8_t> host_buffer_size_complete = {0x0e, 0x04, 0x01, 0x33, 0x0c, 0x00};
DEFINE_AND_INSTANTIATE_HostBufferSizeCompleteReflectionTest(host_buffer_size_complete);

std::vector<uint8_t> read_local_version_information = {0x01, 0x10, 0x00};
DEFINE_AND_INSTANTIATE_ReadLocalVersionInformationReflectionTest(read_local_version_information);

std::vector<uint8_t> read_local_version_information_complete = {0x0e, 0x0c, 0x01, 0x01, 0x10, 0x00, 0x09,
                                                                0x00, 0x00, 0x09, 0x1d, 0x00, 0xbe, 0x02};
DEFINE_AND_INSTANTIATE_ReadLocalVersionInformationCompleteReflectionTest(read_local_version_information_complete);

std::vector<uint8_t> read_bd_addr = {0x09, 0x10, 0x00};
DEFINE_AND_INSTANTIATE_ReadBdAddrReflectionTest(read_bd_addr);

std::vector<uint8_t> read_bd_addr_complete = {0x0e, 0x0a, 0x01, 0x09, 0x10, 0x00, 0x14, 0x8e, 0x61, 0x5f, 0x36, 0x88};
DEFINE_AND_INSTANTIATE_ReadBdAddrCompleteReflectionTest(read_bd_addr_complete);

std::vector<uint8_t> read_local_supported_commands = {0x02, 0x10, 0x00};
DEFINE_AND_INSTANTIATE_ReadLocalSupportedCommandsReflectionTest(read_local_supported_commands);

std::vector<uint8_t> read_local_supported_commands_complete = {
    0x0e, 0x44, 0x01, 0x02, 0x10, 0x00, /* Supported commands start here (total 64 bytes) */
    0xff, 0xff, 0xff, 0x03, 0xce, 0xff, 0xef, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xf2, 0x0f, 0xe8, 0xfe,
    0x3f, 0xf7, 0x83, 0xff, 0x1c, 0x00, 0x00, 0x00, 0x61, 0xff, 0xff, 0xff, 0x7f, 0xbe, 0x20, 0xf5,
    0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
DEFINE_AND_INSTANTIATE_ReadLocalSupportedCommandsCompleteReflectionTest(read_local_supported_commands_complete);

std::vector<uint8_t> read_local_extended_features_0 = {0x04, 0x10, 0x01, 0x00};

std::vector<uint8_t> read_local_extended_features_complete_0 = {0x0e, 0x0e, 0x01, 0x04, 0x10, 0x00, 0x00, 0x02,
                                                                0xff, 0xfe, 0x8f, 0xfe, 0xd8, 0x3f, 0x5b, 0x87};

std::vector<uint8_t> write_simple_paring_mode = {0x56, 0x0c, 0x01, 0x01};
DEFINE_AND_INSTANTIATE_WriteSimplePairingModeReflectionTest(write_simple_paring_mode);

std::vector<uint8_t> write_simple_paring_mode_complete = {0x0e, 0x04, 0x01, 0x56, 0x0c, 0x00};
DEFINE_AND_INSTANTIATE_WriteSimplePairingModeCompleteReflectionTest(write_simple_paring_mode_complete);

std::vector<uint8_t> write_le_host_supported = {0x6d, 0x0c, 0x02, 0x01, 0x01};
DEFINE_AND_INSTANTIATE_WriteLeHostSupportReflectionTest(write_le_host_supported);

std::vector<uint8_t> write_le_host_supported_complete = {0x0e, 0x04, 0x01, 0x6d, 0x0c, 0x00};
DEFINE_AND_INSTANTIATE_WriteLeHostSupportCompleteReflectionTest(write_le_host_supported_complete);

std::vector<uint8_t> read_local_extended_features_1 = {0x04, 0x10, 0x01, 0x01};

std::vector<uint8_t> read_local_extended_features_complete_1 = {0x0e, 0x0e, 0x01, 0x04, 0x10, 0x00, 0x01, 0x02,
                                                                0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

std::vector<uint8_t> read_local_extended_features_2 = {0x04, 0x10, 0x01, 0x02};
DEFINE_AND_INSTANTIATE_ReadLocalExtendedFeaturesReflectionTest(read_local_extended_features_0,
                                                               read_local_extended_features_1,
                                                               read_local_extended_features_2);

std::vector<uint8_t> read_local_extended_features_complete_2 = {0x0e, 0x0e, 0x01, 0x04, 0x10, 0x00, 0x02, 0x02,
                                                                0x45, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
DEFINE_AND_INSTANTIATE_ReadLocalExtendedFeaturesCompleteReflectionTest(read_local_extended_features_complete_0,
                                                                       read_local_extended_features_complete_1,
                                                                       read_local_extended_features_complete_2);

std::vector<uint8_t> write_secure_connections_host_support = {0x7a, 0x0c, 0x01, 0x01};
DEFINE_AND_INSTANTIATE_WriteSecureConnectionsHostSupportReflectionTest(write_secure_connections_host_support);

std::vector<uint8_t> write_secure_connections_host_support_complete = {0x0e, 0x04, 0x01, 0x7a, 0x0c, 0x00};
DEFINE_AND_INSTANTIATE_WriteSecureConnectionsHostSupportCompleteReflectionTest(
    write_secure_connections_host_support_complete);

std::vector<uint8_t> le_read_white_list_size = {0x0f, 0x20, 0x00};
DEFINE_AND_INSTANTIATE_LeReadWhiteListSizeReflectionTest(le_read_white_list_size);

std::vector<uint8_t> le_read_white_list_size_complete = {0x0e, 0x05, 0x01, 0x0f, 0x20, 0x00, 0x80};
DEFINE_AND_INSTANTIATE_LeReadWhiteListSizeCompleteReflectionTest(le_read_white_list_size_complete);

std::vector<uint8_t> le_read_buffer_size = {0x02, 0x20, 0x00};
DEFINE_AND_INSTANTIATE_LeReadBufferSizeReflectionTest(le_read_buffer_size);

std::vector<uint8_t> le_read_buffer_size_complete = {0x0e, 0x07, 0x01, 0x02, 0x20, 0x00, 0xfb, 0x00, 0x10};
DEFINE_AND_INSTANTIATE_LeReadBufferSizeCompleteReflectionTest(le_read_buffer_size_complete);

}  // namespace hci
}  // namespace bluetooth
