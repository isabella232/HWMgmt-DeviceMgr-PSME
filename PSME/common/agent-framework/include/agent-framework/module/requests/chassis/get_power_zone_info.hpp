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
 *
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * @file command/chassis/get_power_zone_info.hpp
 * @brief Generic chassis GetPowerZoneInfo command
 * */

#pragma once
#include "agent-framework/module/constants/chassis.hpp"
#include "agent-framework/module/constants/command.hpp"
#include "agent-framework/validators/procedure_validator.hpp"

#include <string>

namespace Json {
    class Value;
}

namespace agent_framework {
namespace model {
namespace requests {

/*! GetPowerZoneInfo request */
class GetPowerZoneInfo {
public:
    explicit GetPowerZoneInfo(const std::string& zone);

    static std::string get_command() {
        return literals::Command::GET_POWER_ZONE_INFO;
    }

    /*!
     * @brief Get power zone UUID from request
     * @return uuid string
     * */
    const std::string& get_power_zone() const {
        return m_zone;
    }

    /*!
     * @brief Transform request to Json
     *
     * @return created Json value
     */
    Json::Value to_json() const;

    /*!
     * @brief create GetPowerZoneInfo form Json
     *
     * @param[in] json the input argument
     *
     * @return new GetPowerZoneInfo
     */
    static GetPowerZoneInfo from_json(const Json::Value& json);

    /*!
     * @brief Returns procedure scheme
     * @return Procedure scheme
     */
    static const jsonrpc::ProcedureValidator& get_procedure() {
        static const jsonrpc::ProcedureValidator procedure{
            get_command(),
            jsonrpc::PARAMS_BY_NAME,
            jsonrpc::JSON_STRING,
            literals::PowerZone::ZONE, jsonrpc::JSON_STRING,
            nullptr};
        return procedure;
    }

    /*!
     * @brief Get switch uuid from request
     * @return uuid string
     * */
    const std::string& get_uuid() const {
        return m_zone;
    }

private:
    std::string m_zone{};
};

}
}
}

