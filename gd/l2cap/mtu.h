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
#pragma once

#include <cstdint>

namespace bluetooth {
namespace l2cap {

using mtu_t = uint16_t;

constexpr mtu_t kDefaultMinimumClassicMtu = 48;
constexpr mtu_t kDefaultMinimumLeMtu = 23;
constexpr mtu_t kMinimumClassicMtu = 48;
constexpr mtu_t kDefaultClassicMtu = 672;
constexpr mtu_t kMinimumLeMtu = 23;

}  // namespace l2cap
}  // namespace bluetooth
