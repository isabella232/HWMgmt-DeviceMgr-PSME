/*!
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
 *
 * @file responses/pnc/delete_zone_endpoint.cpp
 * @brief DeleteZoneEndpoint response
 * */

#include "agent-framework/module/responses/pnc/delete_zone_endpoint.hpp"
#include "agent-framework/module/constants/pnc.hpp"
#include "agent-framework/module/constants/common.hpp"

#include <json/json.h>



using namespace agent_framework::model::responses;
using namespace agent_framework::model::literals;


DeleteZoneEndpoint::DeleteZoneEndpoint(const std::string& task, const attribute::Oem& oem) :
    m_task{task},
    m_oem{oem} {}


Json::Value DeleteZoneEndpoint::to_json() const {
    Json::Value value;
    value[Zone::OEM] = m_oem.to_json();
    if (!m_task.empty()) {
        value[TaskEntry::TASK] = m_task;
    }
    return value;
}


DeleteZoneEndpoint DeleteZoneEndpoint::from_json(const Json::Value& json) {
    return DeleteZoneEndpoint{json[TaskEntry::TASK].asString(), attribute::Oem::from_json(json[Zone::OEM])};
}
