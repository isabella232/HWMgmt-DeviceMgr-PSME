/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2016-2017 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @section DESCRIPTION
 *
 * @file command/pnc/get_pcie_device_info.cpp
 *
 * @brief Pnc request get device information implementation
 * */

#include "agent-framework/module/requests/pnc/get_pcie_device_info.hpp"
#include "agent-framework/module/constants/pnc.hpp"
#include <json/json.h>

using namespace agent_framework::model::requests;
using namespace agent_framework::model::literals;

GetPcieDeviceInfo::GetPcieDeviceInfo(const std::string& device): m_device{device} {}

Json::Value GetPcieDeviceInfo::to_json() const {
    Json::Value value;
    value[PcieDevice::DEVICE] = m_device;
    return value;
}

GetPcieDeviceInfo GetPcieDeviceInfo::from_json(const Json::Value& json) {
    return GetPcieDeviceInfo{json[PcieDevice::DEVICE].asString()};
}
