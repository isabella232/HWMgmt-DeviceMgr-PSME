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
 * @file command/registration/json/register_agent.cpp
 *
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * @brief JSON command RegisterAgent implementation
 * */
#include "agent-framework/logger_ext.hpp" 
#include "psme/command/registration/json/register_agent.hpp"
#include "psme/command/registration/register_agent.hpp"

using namespace psme;
using namespace psme::command::registration::json;

RegisterAgent::RegisterAgent() :
    CommandJson(registration::TAG, Procedure(
                registration::RegisterAgent::TAG,
                jsonrpc::PARAMS_BY_NAME,
                jsonrpc::JSON_STRING,
                "gamiId", jsonrpc::JSON_STRING,
                "version", jsonrpc::JSON_STRING,
                "vendor", jsonrpc::JSON_STRING,
                "ipv4address", jsonrpc::JSON_STRING,
                "port", jsonrpc::JSON_INTEGER,
                "capabilities", jsonrpc::JSON_ARRAY,
                nullptr)) {
    }

void RegisterAgent::method(const Json::Value& params, Json::Value& result) {

    Command* command = get_command();

    registration::RegisterAgent::Request request{};
    registration::RegisterAgent::Response response{};

    request.m_gami_id = params["gamiId"].asString();
    request.m_version = params["version"].asString();
    request.m_vendor = params["vendor"].asString();
    request.m_ipv4address = params["ipv4address"].asString();
    request.m_port = params["port"].asInt();

    for (const auto& param : params["capabilities"]) {
        request.m_capabilities.emplace_back(param.asString());
    }

    command->execute(request, response);

    result["ipv4address"] = response.get_ipv4address();
    result["port"] = response.get_port();
    result["version"] = response.get_version();
}

void RegisterAgent::notification(const Json::Value&) { }

static CommandJson::Register<RegisterAgent> g;
