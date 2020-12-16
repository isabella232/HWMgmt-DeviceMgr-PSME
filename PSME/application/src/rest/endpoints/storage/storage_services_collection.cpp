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

#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/storage/storage_services_collection.hpp"

using namespace psme::rest::endpoint;
using namespace psme::rest::constants;



namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#StorageServiceCollection.StorageServiceCollection";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#StorageServiceCollection.StorageServiceCollection";
    r[Common::NAME] = "Storage Services Collection";
    r[Common::DESCRIPTION] = "Collection of Storage Services";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}
}

StorageServiceCollection::StorageServiceCollection(const std::string& path) : EndpointBase(path) {}
StorageServiceCollection::~StorageServiceCollection() {}

void StorageServiceCollection::get(const server::Request& req, server::Response& res) {
    auto json = ::make_prototype();

    json[Common::ODATA_ID] = PathBuilder(req).build();

    auto service_ids = agent_framework::module::StorageComponents::get_instance() -> get_storage_services_manager().get_ids();

    json[Collection::ODATA_COUNT] = std::uint32_t(service_ids.size());

    for (const auto& id : service_ids ){
        json::Value link;
        link[Common::ODATA_ID] = PathBuilder(req).append(id).build();
        json[Collection::MEMBERS].push_back(std::move(link));
    }

    set_response(res, json);
}
