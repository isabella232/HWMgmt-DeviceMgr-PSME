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

#include "agent-framework/module/requests/common.hpp"
#include "agent-framework/module/responses/common.hpp"

#include "psme/rest/constants/constants.hpp"
#include "psme/core/agent/agent_manager.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/reset.hpp"
#include "psme/rest/endpoints/fabric/port_reset.hpp"
#include "psme/rest/server/error/error_factory.hpp"
#include "psme/rest/model/handlers/handler_manager.hpp"

#include "psme/rest/model/handlers/generic_handler_deps.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"



using namespace psme::rest;
using namespace psme::rest::constants;
using namespace psme::rest::validators;


endpoint::PortReset::PortReset(const std::string& path) : EndpointBase(path) {}


endpoint::PortReset::~PortReset() {}


void endpoint::PortReset::post(const server::Request& request, server::Response&) {
    using ResetType = agent_framework::model::enums::ResetType;

    const auto& json = JsonValidator::validate_request_body<schema::ResetPostSchema>(request);

    if (json.is_member(Common::RESET_TYPE)) {
        const auto port = psme::rest::model::Find<agent_framework::model::Port>(request.params[PathParam::PORT_ID])
            .via<agent_framework::model::Fabric>(request.params[PathParam::FABRIC_ID])
            .via<agent_framework::model::Switch>(request.params[PathParam::SWITCH_ID]).get();
        const auto& allowable_reset_types = port.get_allowed_actions();

        const auto reset_type = json[constants::Common::RESET_TYPE].as_string();
        const auto reset_type_enum = ResetType::from_string(reset_type);

        auto predicate = [&reset_type_enum](const ResetType& type) {
            return reset_type_enum == type;
        };
        if (!std::any_of(allowable_reset_types.begin(), allowable_reset_types.end(), predicate)) {
            throw error::ServerException(
                error::ErrorFactory::create_error_for_not_allowable_value(
                    "The Port does not support the requested reset target.",
                    Common::RESET_TYPE, reset_type,
                    endpoint::PathBuilder(constants::FabricCommon::ACTIONS)
                        .append(constants::Port::PORT_RESET)
                        .append(FabricCommon::ALLOWABLE_RESET_TYPES)
                        .build()
                )
            );
        }

        // Request validation cannot pass, because no ResetTypes are supported by the PNC agent
        // (we do not allow Port reset).
        throw agent_framework::exceptions::InvalidValue("Reset on Fabric Port is not supported!");
    }
}

