/*!
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
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * */

#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/utils/lag_utils.hpp"
#include "psme/rest/model/finder.hpp"
#include "psme/rest/utils/mapper.hpp"
#include "psme/rest/endpoints/endpoints.hpp"

using namespace psme::rest;
using namespace psme::rest::error;
using namespace psme::rest::utils;
using namespace agent_framework::model;

std::vector<std::string> LagUtils::get_port_members(const json::Value& json) {
    std::vector<std::string> port_members;
    for (const auto& member : json[constants::Common::LINKS][constants::EthernetSwitchPort::PORT_MEMBERS].as_array()) {
        auto member_path = member[constants::Common::ODATA_ID].as_string();
        try {
#if 0			
            auto params = psme::rest::model::Mapper::get_params(member_path,
                                                                constants::Routes::ETHERNET_SWITCH_PORT_PATH);
            auto port_uuid =
                psme::rest::model::Find<agent_framework::model::EthernetSwitchPort>(params[constants::PathParam::SWITCH_PORT_ID])
                .via<agent_framework::model::EthernetSwitch>(params[constants::PathParam::ETHERNET_SWITCH_ID])
                .get_uuid();
            port_members.push_back(port_uuid);
#else
            port_members.push_back(member_path);
#endif			
        }
        catch (const agent_framework::exceptions::NotFound&) {
            THROW(agent_framework::exceptions::InvalidValue, "rest",
                "Could not find port member: " + member[constants::Common::ODATA_ID].as_string());
        }
    }
    return port_members;
}

void LagUtils::validate_is_physical(const enums::PortClass port_class) {
    if (port_class != enums::PortClass::Physical) {
        throw agent_framework::exceptions::InvalidValue("Port class is not physical.");
    }
}

void LagUtils::validate_is_logical(const enums::PortClass port_class) {
    if (port_class != enums::PortClass::Logical) {
        throw agent_framework::exceptions::InvalidValue("Port class is not logical.");
    }
}

void LagUtils::validate_port_type(const enums::PortType port_type) {
    if (port_type != enums::PortType::Upstream) {
        throw agent_framework::exceptions::InvalidValue("Port type is not upstream.");
    }
}

void LagUtils::validate_collection_not_empty(const std::vector<std::string> port_members) {
    if (port_members.empty()) {
        throw agent_framework::exceptions::InvalidValue("Port members collection cannot be empty.");
    }
}

void LagUtils::validate_is_member_of_port(const std::string& port, const std::string& parent) {
    const auto& lag_manager = agent_framework::module::NetworkComponents::get_instance()->get_port_members_manager();
    if (lag_manager.child_exists(port) && !lag_manager.entry_exists(parent, port)) {
        throw agent_framework::exceptions::InvalidValue("Switch port is already a member of logical port.");
    }
}

void LagUtils::validate_port_members(const std::vector<std::string> port_members, const std::string& parent) {
    bool is_first_member{true};
    std::uint32_t link_speed = 0;

    validate_collection_not_empty(port_members);
    for (const auto& member_uuid : port_members) {
        auto member = agent_framework::module::NetworkComponents::get_instance()->
            get_port_manager().get_entry_reference(member_uuid);
        if(!(member->get_port_class().has_value()) || !(member->get_port_type().has_value())) {
            continue;
        }
        auto member_port_class = member->get_port_class().value();
        auto member_port_type = member->get_port_type().value();
        const auto member_port_speed = member->get_link_speed_mbps();

        validate_is_physical(member_port_class);
        validate_port_type(member_port_type);
        validate_is_member_of_port(member_uuid, parent);

        if (is_first_member) {
            is_first_member = false;
            link_speed = member_port_speed;
        }
        else {
            if (member_port_speed != link_speed) {
                throw agent_framework::exceptions::InvalidValue("Not matching link speed in port members.");
            }
        }
    }
}
