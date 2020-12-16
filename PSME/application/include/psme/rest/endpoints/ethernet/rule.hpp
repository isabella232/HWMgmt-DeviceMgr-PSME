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
#include "psme/rest/endpoints/endpoint_base.hpp"


namespace psme {
namespace rest {
namespace endpoint {


#define RULE_MAX 10
#define ACL_MAX 10

/*!
 * A class representing the rest api Rule endpoint
 */
class Rule : public EndpointBase {
public:


    /*!
     * @brief The constructor for Rule class
     */
    explicit Rule(const std::string& path);

    /*!
     * @brief Rule class destructor
     */
    virtual ~Rule();

    void get(const server::Request& request, server::Response& response) override;

    void del(const server::Request& request, server::Response& response) override;

    void patch(const server::Request& request, server::Response& response) override;
};

}
}
}

