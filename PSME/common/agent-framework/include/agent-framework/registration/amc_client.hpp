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
 * @file amc_client.hpp
 * @brief JSONRPC client for communication with AMC (Asset Management Core) application.
 * */

#pragma once
#include "agent-framework/logger_ext.hpp"
#include "agent-framework/registration/registration_response.hpp"
#include "agent-framework/registration/heartbeat_response.hpp"

#include <chrono>
#include <memory>

namespace jsonrpc {
    class IClientConnector;
    class Client;
}

namespace agent_framework {
namespace generic {

/*!
 * @brief JSONRPC client for communication
 * with AMC (Asset Management Core) application.
 **/
class AmcClient {
public:
    /*!
     * @brief Constructor.
     * @param url The url of AMC application to send JSONRPC requests
     */
    explicit AmcClient(const std::string& url);

    /*!
     * @brief Sends registration command
     * @return RegisterResponse
     * */
    RegistrationResponse attach() const;

    /*!
     * @brief Sends heart beat message
     * @return HeartBeatResponse
     * */
    HeartBeatResponse heart_beat() const;

    /*!
     * @brief Destructor.
     */
    virtual ~AmcClient();

private:
    std::string m_url{};
    std::shared_ptr<jsonrpc::IClientConnector> m_http_connector{};
    std::shared_ptr<jsonrpc::Client> m_jsonrpc_client{};
};

}
}

