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

#include "psme/rest/endpoints/chassis/pcie_devices_collection.hpp"

using namespace psme::rest::endpoint;
using namespace psme::rest::constants;



namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#PCIeDevices.PCIeDevices";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#PCIeDeviceCollection.PCIeDeviceCollection";
    r[Common::DESCRIPTION] = "Collection of PCIe Devices";
    r[Common::NAME] = "PCIe Device Collection";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}
}

PcieDevicesCollection::PcieDevicesCollection(const std::string& path) : EndpointBase(path) {}
PcieDevicesCollection::~PcieDevicesCollection() {}

void PcieDevicesCollection::get(const server::Request& req, server::Response& res) {
    auto json = ::make_prototype();

    json[Common::ODATA_ID] = PathBuilder(req).build();

    // the devices are under the same manager as the chassis from the URL
    auto chassis_manager_uuid = psme::rest::model::Find<agent_framework::model::Chassis>(
        req.params[PathParam::CHASSIS_ID]).get_one()->get_parent_uuid();

    auto pcie_device_ids = agent_framework::module::get_manager<agent_framework::model::PcieDevice>().get_ids(chassis_manager_uuid);

    json[Collection::ODATA_COUNT] = std::uint32_t(pcie_device_ids.size());

    for (const auto& id : pcie_device_ids ){
        json::Value link;
        link[Common::ODATA_ID] = PathBuilder(req).append(id).build();
        json[Collection::MEMBERS].push_back(std::move(link));
    }

    set_response(res, json);
}
