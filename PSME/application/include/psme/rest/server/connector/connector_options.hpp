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
 * @file connector_options.hpp
 *
 * @brief Declaration of Connector Options for Connector initialization.
 * */

#pragma once
#include "json/json.hpp"
#include <string>

namespace psme {
namespace rest {
namespace server {

/*!
 * @brief ConnectorOptions parsed from JSON configuration file
 * for Connector initialization.
 *
 * Provided implementations (MHDConnector, MHDSSLConnector) of Connector interface are based on
 * <a href="https://www.gnu.org/software/libmicrohttpd">Libmicrohttpd</a> library.
 * Please refer to its documentation for explanation of some options: #PORT,
 * #USE_SSL, #THREAD_MODE, #THREAD_POOL_SIZE, #DEBUG_MODE.
 **/
class ConnectorOptions {
public:
    /*! @brief Property name of port Connector should listen on */
    static constexpr const char PORT[] = "port";
    /*! @brief Property name of redirection port Connector sends redirects to */
    static constexpr const char REDIRECT_PORT[] = "redirect-port";
    /*! @brief Property name of flag indicating connector should listen over secure channel */
    static constexpr const char USE_SSL[] = "use-ssl";
    /*! @brief Property name of Path to directory containing files for ssl Connector */
    static constexpr const char CERTS_DIR[] = "certs-directory";
    /*! @brief Property name of flag indicating if client certificate is required */
    static constexpr const char CLIENT_CERT_REQUIRED[] = "client-cert-required";
    /*! @brief Property name of threading mode */
    static constexpr const char THREAD_MODE[] = "thread-mode";
    /*! @brief Value of threading mode property */
    static constexpr const char THREAD_MODE_SELECT[] = "select";
    /*! @brief Value of threading mode property */
    static constexpr const char THREAD_MODE_THREAD_PER_CONNECTION[] = "thread-per-connection";
    /*! @brief Property name of thread pool size */
    static constexpr const char THREAD_POOL_SIZE[] = "thread-pool-size";
    /*! @brief Property name of flag indicating if debug mode should be enabled */
    static constexpr const char DEBUG_MODE[] = "debug-mode";

    /*! @brief Threading mode of connector */
    enum class ThreadMode {
        SELECT = 0,
        THREAD_PER_CONNECTION
    };

    /*!
     * @brief Constructor.
     * @param connector_config JSON Object with connector options.
     * */
    explicit ConnectorOptions(const json::Value& connector_config);

    /*!
     * @brief Destructor.
     */
    ~ConnectorOptions();

    /*!
     * @brief Default copy Constructor.
     */
    ConnectorOptions(const ConnectorOptions&) = default;

    /*!
     * @brief Default assignment operator.
     */
    ConnectorOptions& operator=(const ConnectorOptions&) = default;

    /*!
     * @brief Default move Constructor.
     */
    ConnectorOptions(ConnectorOptions&&) = default;

    /*!
     * @brief Default move assignment operator.
     */
    ConnectorOptions& operator=(ConnectorOptions&&) = default;

    /*!
     * @brief Threading Mode the Connector should use.
     *
     * @return ThreadMode
     */
    ThreadMode get_thread_mode() const;

    /*!
     * @return Port the Connector should listen on.
     */
    uint16_t get_port() const;

    /*!
     * @return Redirect Port on which other Connector is listening.
     */
    uint16_t get_redirect_port() const;

    /*!
     * @return true if Connector should listen over HTTPS.
     */
    bool use_ssl() const;

    /*!
     * @return Certificates directory location for HTTPS configuration.
     */
    const std::string& get_certs_dir() const;

    /*!
     * @return true if HTTPS Client is required to provide certificate.
     */
    bool is_client_cert_required() const;

    /*!
     * @return Thread pool size of Connector.
     */
    unsigned int get_thread_pool_size() const;

    /*!
     * @return true if debug mode should be enabled for Connector.
     */
    bool use_debug() const;

private:
    uint16_t m_port{443};
    uint16_t m_redirect_port{0};
    bool m_use_ssl{true};
    std::string m_certs_dir{};
    bool m_is_client_cert_required{true};
    unsigned int m_thread_pool_size{0};
    ThreadMode m_thread_mode{ThreadMode::SELECT};
    bool m_use_debug{false};
};

/*! Declaration of Vector with ConnectorOptions elements */
using ConnectorOptionsVec = std::vector<ConnectorOptions>;

}
}
}

