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
 * */

#include "psme/rest/model/finder.hpp"
#include "psme/rest/utils/zone_utils.hpp"
#include "psme/rest/utils/mapper.hpp"
#include "psme/rest/endpoints/endpoints.hpp"



using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::utils;
using namespace agent_framework::model;


std::vector<std::string> ZoneUtils::validate_patch_links_and_get_endpoint_uuids(const json::Value& json) {

    std::vector<std::string> endpoints;
    for (const auto& endpoint_link : json[constants::Fabric::ENDPOINTS].as_array()) {
        auto endpoint_path = endpoint_link[constants::Common::ODATA_ID].as_string();
        try {
            auto params = psme::rest::model::Mapper::get_params(endpoint_path, constants::Routes::ENDPOINT_PATH);
            auto endpoint_uuid =
                psme::rest::model::Find<agent_framework::model::Endpoint>(params[PathParam::ENDPOINT_ID])
                    .via<agent_framework::model::Fabric>(params[PathParam::FABRIC_ID])
                    .get_uuid();

            endpoints.push_back(endpoint_uuid);
        }
        catch (const agent_framework::exceptions::NotFound&) {
            THROW(agent_framework::exceptions::InvalidValue, "rest",
                  "Could not find endpoint: " + endpoint_link[constants::Common::ODATA_ID].as_string());
        }
    }
    return endpoints;
}

std::vector<std::string> ZoneUtils::validate_post_links_and_get_endpoint_uuids(const json::Value& json) {
    // validate links to Switches
    for (const auto& switch_link : json[constants::Zone::INVOLVED_SWITCHES].as_array()) {
        auto switch_path = switch_link[constants::Common::ODATA_ID].as_string();
        try {
            auto params = psme::rest::model::Mapper::get_params(switch_path, constants::Routes::SWITCH_PATH);
            // verify Switch's existence
            psme::rest::model::Find<agent_framework::model::Switch>(params[PathParam::SWITCH_ID])
                .via<agent_framework::model::Fabric>(params[PathParam::FABRIC_ID]).get_uuid();
        }
        catch (const agent_framework::exceptions::NotFound&) {
            THROW(agent_framework::exceptions::InvalidValue, "rest",
                  "Could not find Fabric Switch: " + switch_link[constants::Common::ODATA_ID].as_string());
        }
    }

    std::vector<std::string> endpoints;
    for (const auto& endpoint_link : json[constants::Fabric::ENDPOINTS].as_array()) {
        auto endpoint_path = endpoint_link[constants::Common::ODATA_ID].as_string();
        try {
            auto params = psme::rest::model::Mapper::get_params(endpoint_path, constants::Routes::ENDPOINT_PATH);
            auto endpoint_uuid =
                psme::rest::model::Find<agent_framework::model::Endpoint>(params[PathParam::ENDPOINT_ID])
                    .via<agent_framework::model::Fabric>(params[PathParam::FABRIC_ID])
                    .get_uuid();

            endpoints.push_back(endpoint_uuid);
        }
        catch (const agent_framework::exceptions::NotFound&) {
            THROW(agent_framework::exceptions::InvalidValue, "rest",
                  "Could not find endpoint: " + endpoint_link[constants::Common::ODATA_ID].as_string());
        }
    }
    return endpoints;
}


OptionalField<std::string> ZoneUtils::get_upstream_endpoint(const std::vector<std::string>& endpoint_uuids) {
    auto is_upstream = [](const std::string& endpoint_uuid) {
        const auto endpoint_entitites = agent_framework::module::get_manager<agent_framework::model::Endpoint>()
            .get_entry(endpoint_uuid).get_connected_entities();
        for (const auto& entity : endpoint_entitites) {
            if (entity.get_entity_type() == enums::EntityType::RootComplex) {
                return true;
            }
        }
        return false;
    };
    auto iterator = std::find_if(endpoint_uuids.begin(), endpoint_uuids.end(), is_upstream);
    if (iterator != endpoint_uuids.end()) {
        return *iterator;
    }
    return OptionalField<std::string>{};
}

