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
 * @file responses/network/add_acl_rule.cpp
 * @brief AddAclRule response
 * */

#include "agent-framework/module/responses/network/add_acl_rule.hpp"
#include "agent-framework/module/constants/network.hpp"
#include <json/json.h>

using namespace agent_framework::model::responses;
using namespace agent_framework::model::literals;

AddAclRule::AddAclRule(const std::string& rule, Oem oem):
    m_rule(rule),
    m_oem{oem} {}


Json::Value AddAclRule::to_json() const {
    Json::Value value;
    value[AclRule::RULE] = m_rule;
    value[AclRule::OEM] = m_oem.to_json();
    return value;
}

AddAclRule AddAclRule::from_json(const Json::Value& json) {
    return AddAclRule{json[AclRule::RULE].asString(), Oem::from_json(
            json[AclRule::OEM])};
}
