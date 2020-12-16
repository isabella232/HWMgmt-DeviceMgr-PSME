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
 * @file requests/compute/get_storage_controller_info.cpp
 *
 * @brief Compute request get controller information implementation
 * */

#include "agent-framework/module/requests/compute/get_storage_controller_info.hpp"
#include "agent-framework/module/constants/compute.hpp"
#include <json/json.h>



using namespace agent_framework::model::requests;
using namespace agent_framework::model::literals;


GetStorageControllerInfo::GetStorageControllerInfo(const std::string& controller) : m_controller{controller} { }


Json::Value GetStorageControllerInfo::to_json() const {
    Json::Value value;
    value[StorageController::CONTROLLER] = m_controller;
    return value;
}


GetStorageControllerInfo GetStorageControllerInfo::from_json(const Json::Value& json) {
    return GetStorageControllerInfo{
        json[StorageController::CONTROLLER].asString()};
}

