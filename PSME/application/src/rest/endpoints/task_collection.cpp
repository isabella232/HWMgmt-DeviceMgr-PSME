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
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * */

#include "psme/rest/endpoints/task_collection.hpp"
#include "psme/rest/utils/status_helpers.hpp"

#include "agent-framework/module/managers/utils/manager_utils.hpp"

#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/sessions.hpp"
using namespace psme::rest::validators;

using namespace psme::rest;
using namespace psme::rest::constants;
using namespace agent_framework::module;
using namespace agent_framework::model::enums;

namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#TaskCollection.TaskCollection";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#TaskCollection.TaskCollection";
    r[Common::NAME] = "Task Collection";
    r[Common::DESCRIPTION] = "Task Collection";
    r[Collection::ODATA_COUNT] = json::Value::Type::NIL;
    r[Collection::MEMBERS] = json::Value::Type::ARRAY;

    return r;
}

}

namespace psme {
namespace rest {
namespace endpoint {

TaskCollection::TaskCollection(const std::string& path) : EndpointBase(path) { }


TaskCollection::~TaskCollection() { }


void TaskCollection::get(const server::Request& req, server::Response& res) {
    auto json = ::make_prototype();
    json[Common::ODATA_ID] = PathBuilder(req).build();

    const auto& tasks_ids = get_manager<agent_framework::model::Task>().get_ids();
    json[Collection::ODATA_COUNT] = std::uint32_t(tasks_ids.size());

    for (const auto& id : tasks_ids) {
        json::Value link;
        link[Common::ODATA_ID] = PathBuilder(req).append(id).build();
        json[Collection::MEMBERS].push_back(std::move(link));
    }

    set_response(res, json);
}

}
}
}
