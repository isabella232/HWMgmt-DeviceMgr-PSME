/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2015-2017 Intel Corporation
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
 * @file requests/storage/get_logical_drive_info.cpp
 *
 * @brief Storage GetLogicalDriveInfo request implementation
 * */

#include "agent-framework/module/requests/storage/get_logical_drive_info.hpp"
#include "agent-framework/module/constants/storage.hpp"
#include <json/json.h>

using namespace agent_framework::model::requests;
using namespace agent_framework::model::literals;

GetLogicalDriveInfo::GetLogicalDriveInfo(const std::string& drive): m_drive{drive} {}

Json::Value GetLogicalDriveInfo::to_json() const {
    Json::Value value;
    value[LogicalDrive::DRIVE] = m_drive;
    return value;
}

GetLogicalDriveInfo GetLogicalDriveInfo::from_json(const Json::Value& json) {
    return GetLogicalDriveInfo{
        json[LogicalDrive::DRIVE].asString()};
}
