/*!
 * @brief Definition of Network Interfaces endpoint
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
 * @file network_interface.cpp
 */

#include "psme/rest/endpoints/system/network_interface.hpp"
#include "psme/rest/utils/status_helpers.hpp"


using namespace psme::rest;
using namespace psme::rest::constants;



namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#NetworkInterface.NetworkInterface";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#NetworkInterface.v1_0_0.NetworkInterface";
    r[Common::ID] = json::Value::Type::NIL;
    r[Common::NAME] = "Network Interface";
    r[Common::DESCRIPTION] = "Network Interface";
    r[Common::STATUS][Common::STATE] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL;
    r[NetworkInterface::NETWORK_DEVICE_FUNCTIONS] = json::Value::Type::NIL;
    r[Common::LINKS] = json::Value::Type::OBJECT;

    return r;
}

}

endpoint::NetworkInterface::NetworkInterface(const std::string& path) : EndpointBase(path) {}
endpoint::NetworkInterface::~NetworkInterface() {}

void endpoint::NetworkInterface::get(const server::Request& req, server::Response& res) {
    auto json = make_prototype();
    auto system_id = psme::rest::model::Find<agent_framework::model::System>(req.params[PathParam::SYSTEM_ID])
            .get_one()->get_id();
    auto device = psme::rest::model::Find<agent_framework::model::NetworkDevice>(req.params[PathParam::NETWORK_INTERFACE_ID])
        .via<agent_framework::model::System>(req.params[PathParam::SYSTEM_ID])
        .get();

    json[Common::ODATA_ID] = PathBuilder(req).build();
    json[Common::ODATA_CONTEXT] = std::regex_replace(json[Common::ODATA_CONTEXT].as_string(),
                                                     std::regex("__SYSTEM_ID__"), std::to_string(system_id));

    endpoint::status_to_json(device,json);
    json[constants::Common::ID] = req.params[PathParam::NETWORK_INTERFACE_ID];
    json[constants::NetworkInterface::NETWORK_DEVICE_FUNCTIONS][Common::ODATA_ID] = endpoint::PathBuilder(req).append(
            constants::NetworkInterface::NETWORK_DEVICE_FUNCTIONS).build();

    set_response(res, json);
}
