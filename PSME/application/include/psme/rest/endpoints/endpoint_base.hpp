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

#pragma once
#include "psme/rest/server/request.hpp"
#include "psme/rest/server/response.hpp"
#include "psme/rest/server/methods_handler.hpp"
#include "psme/rest/model/finder.hpp"
#include "psme/rest/endpoints/utils.hpp"

#include "agent-framework/module/chassis_components.hpp"
#include "agent-framework/module/compute_components.hpp"
#include "agent-framework/module/network_components.hpp"
#include "agent-framework/module/storage_components.hpp"
#include "agent-framework/module/pnc_components.hpp"

#include <json/json.hpp>
#include <cstring>
#include <string>
#include <memory>
#include <regex>

namespace psme {
namespace rest {
namespace endpoint {

using psme::rest::server::MethodsHandler;
using psme::rest::server::Request;
using psme::rest::server::Response;

/*!
 * A class representing the rest api endpoint
 */
class EndpointBase : public MethodsHandler {
public:

    /*!
     * @brief Constructor
     * @param path The endpoint path to be matched against request URL to select this endpoint.
     * */
    explicit EndpointBase(const std::string& path);

    /*!
     * @brief Destructor
     */
    virtual ~EndpointBase();

    void get(const Request& request, Response& response) override;

    void del(const Request& request, Response& response) override;

    void post(const Request& request, Response& response) override;

    void patch(const Request& request, Response& response) override;

    void put(const Request& request, Response& response) override;

    bool authen_check(const Request& request,const std::string& method) override;

protected:

    /*!
     * @brief the method updates the time of modification
     */
    void update_modified_time();

    /*!
     * @brief the method return the time of modification
     *
     * @return the modification time
     */
    const std::string& get_modified_time() const {
        return m_modified_time;
    }

    /*!
     * @brief the method fills a Response with json data
     *
     * @param response server::Response the Response to be filled with json content
     * @param json json::Value the json content
     */
    void set_response(server::Response& response, const json::Value& json) {
        response << json::Serializer(json);
    }

private:
    std::string m_modified_time{};
};

}
}
}

