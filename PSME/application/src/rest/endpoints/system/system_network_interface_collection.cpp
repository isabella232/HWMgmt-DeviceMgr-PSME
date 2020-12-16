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

#include "psme/rest/endpoints/system/system_network_interface_collection.hpp"
#include "psme/rest/constants/constants.hpp"

using namespace psme::rest::endpoint;
using namespace psme::rest::constants;



namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#EthernetInterfaceCollection.EthernetInterfaceCollection";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#EthernetInterfaceCollection.EthernetInterfaceCollection";
    r[Common::NAME] = "Ethernet Network Interface Collection";
    r[Common::DESCRIPTION] = "Collection of System Ethernet Interfaces";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}
}

SystemNetworkInterfaceCollection::SystemNetworkInterfaceCollection(const std::string& path) : EndpointBase(path) {}
SystemNetworkInterfaceCollection::~SystemNetworkInterfaceCollection() {}

void SystemNetworkInterfaceCollection::get(const server::Request& req, server::Response& res) {
#if 0	//Use Manager ethernet interface //
    auto json = ::make_prototype();

    json[Common::ODATA_ID] = PathBuilder(req).build();

    auto system_uuid = psme::rest::model::Find<agent_framework::model::System>(req.params[PathParam::SYSTEM_ID]).get_uuid();

    auto keys = agent_framework::module::ComputeComponents::get_instance()->
                        get_network_interface_manager().get_ids(system_uuid);

    json[Collection::ODATA_COUNT] =
                                    static_cast<std::uint32_t>(keys.size());
    for (const auto& key : keys) {
        json::Value link_elem(json::Value::Type::OBJECT);
        link_elem[Common::ODATA_ID] = PathBuilder(req).append(key).build();
        json[Collection::MEMBERS].push_back(std::move(link_elem));
    }

    set_response(res, json);
#else
    auto json = ::make_prototype();
    json[Common::ODATA_ID] = PathBuilder(req).build();
    json[Collection::ODATA_COUNT] = 1;
    json::Value link_elem(json::Value::Type::OBJECT);
    link_elem[Common::ODATA_ID] = "/redfish/v1/Managers/1/EthernetInterfaces/1";
    json[Collection::MEMBERS].push_back(std::move(link_elem));
    set_response(res, json);
	
#endif
}
