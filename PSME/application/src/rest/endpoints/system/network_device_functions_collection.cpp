/*!
 * @brief Definition of Network Device Functions Collection endpoint
 *
 * @copyright Copyright (c) 2017 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
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
 * @header{Files}
 * @file network_device_functions_collection.cpp
 */

#include "psme/rest/endpoints/system/network_device_functions_collection.hpp"
#include "agent-framework/module/compute_components.hpp"

using namespace psme::rest::endpoint;
using namespace psme::rest::constants;
using namespace agent_framework::module;



namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#NetworkDeviceFunctionCollection.NetworkDeviceFunctionCollection";

    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#NetworkDeviceFunctionCollection.NetworkDeviceFunctionCollection";
    r[Common::NAME] = "Network Device Functions Collection";
    r[Common::DESCRIPTION] = "Collection of Network Device Functions";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}
}

NetworkDeviceFunctionsCollection::NetworkDeviceFunctionsCollection(const std::string& path) : EndpointBase(path) {}
NetworkDeviceFunctionsCollection::~NetworkDeviceFunctionsCollection() {}

void NetworkDeviceFunctionsCollection::get(const server::Request& req, server::Response& res) {
    auto json = ::make_prototype();

    json[Common::ODATA_ID] = PathBuilder(req).build();
    auto system = psme::rest::model::Find<agent_framework::model::System>(req.params[PathParam::SYSTEM_ID]).get();
    auto device = psme::rest::model::Find<agent_framework::model::NetworkDevice>(req.params[PathParam::NETWORK_INTERFACE_ID])
        .via(system.get_uuid()).get();
    json[Common::ODATA_CONTEXT] = std::regex_replace(json[Common::ODATA_CONTEXT].as_string(),
                                                     std::regex("__SYSTEM_ID__"), std::to_string(system.get_id()));
    json[Common::ODATA_CONTEXT] = std::regex_replace(json[Common::ODATA_CONTEXT].as_string(),
                                                     std::regex("__NETWORK_INTERFACE_ID__"), std::to_string(device.get_id()));

    auto keys = ComputeComponents::get_instance()->
        get_network_device_function_manager().get_ids(device.get_uuid());

    json[Collection::ODATA_COUNT] =
        static_cast<std::uint32_t>(keys.size());
    for (const auto& key : keys) {
        json::Value link_elem(json::Value::Type::OBJECT);
        link_elem[Common::ODATA_ID] = PathBuilder(req).append(key).build();
        json[Collection::MEMBERS].push_back(std::move(link_elem));
    }
    set_response(res, json);
}
