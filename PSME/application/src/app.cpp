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

#include "app.hpp"
#include "psme/core/agent/agent_manager.hpp"
#include "agent-framework/eventing/events_queue.hpp"
#include "psme/rest/eventing/config/subscription_config.hpp"
#include "psme/rest/account/config/account_config.hpp"
#include "psme/rest/session/manager/session_manager.hpp"
#include "psme/rest/log/manager/log_manager.hpp"
#include "psme/rest/session/config/session_config.hpp"
#include "psme/rest/account/config/role_config.hpp"
#include "psme/rest/eventing/event_service.hpp"
#include "psme/rest/registries/config/registry_configurator.hpp"
#include "psme/rest/registries/config/base_configuration.hpp"
#include "psme/ssdp/ssdp_config_loader.hpp"
#include "default_configuration.hpp"
#include "configuration/configuration.hpp"
#include "configuration/configuration_validator.hpp"
#include "agent-framework/logger_ext.hpp"
#include "agent-framework/version.hpp"
#include "agent-framework/service_uuid.hpp"
#include "ssdp/ssdp_service.hpp"
#include <jsonrpccpp/server/connectors/httpserver.h>
#include <csignal>
#include <iostream>
#include <string>
#ifdef BAL
#include "ecbal_api_dist_helper/ecbal_api_dist_helper.hpp"
using namespace ecbal_api_dist_helper;
#endif
#include "ecrf_pal_helper/api/ecrf_pal_helper.hpp"

using namespace ecrf_pal_helper;
using namespace agent_framework::eventing;

namespace {
const json::Value& load_configuration(int argc, const char** argv) {
    /* Initialize configuration */
    log_info(GET_LOGGER("app"), agent_framework::generic::Version::build_info());
    auto& basic_config = configuration::Configuration::get_instance();
    basic_config.set_default_configuration(psme::app::DEFAULT_CONFIGURATION);
    basic_config.set_default_file(psme::app::DEFAULT_FILE);
    basic_config.set_default_env_file(psme::app::DEFAULT_ENV_FILE);

    while (argc > 1) {
        basic_config.add_file(argv[argc - 1]);
        --argc;
    }
    const json::Value& configuration = basic_config.to_json();

    json::Value json_schema;
    if (configuration::string_to_json(psme::app::DEFAULT_VALIDATOR_JSON, json_schema)) {
        log_info(GET_LOGGER("app"), "JSON Schema load!");
        configuration::SchemaErrors errors;
        configuration::SchemaValidator validator;
        configuration::SchemaReader reader;
        reader.load_schema(json_schema, validator);
        validator.validate(configuration, errors);

        if (!errors.is_valid()) {
            std::cerr << "Configuration invalid: "
                      << errors.to_string() << std::endl;
            exit(-1);
        }
    }
    return configuration;
}

}

using namespace psme::app;

App::App(int argc, const char* argv[])
    try : m_configuration(load_configuration(argc, argv)) {
        init();
    }
    catch(std::exception& e) {
        std::cerr << "\nFailed to initialize Application: " << e.what() << "\n";
        exit(-1);
    }

App::~App() { cleanup(); }

void App::init_logger() {
    logger_cpp::LoggerLoader loader(m_configuration);
    logger_cpp::LoggerFactory::instance().set_loggers(loader.load());
    logger_cpp::LoggerFactory::set_main_logger_name("app");
}

void App::init_network_change_notifier() {
    try {
        m_network_change_notifier = net::NetworkChangeNotifier::create();
    } catch (const std::exception&) {
        log_error(GET_LOGGER("app"), "Failed to initialize NetworkChangeNotifier.");
    }
}

void App::init_ssdp_service() {
    const auto& service_uuid = agent_framework::generic::ServiceUuid::get_instance()->get_service_uuid();
    m_ssdp_service.reset(
        new ssdp::SsdpService{ssdp::load_ssdp_config(m_configuration, service_uuid)});
    if (m_network_change_notifier) {
        m_network_change_notifier->add_listener(m_ssdp_service);
    }
}

void App::init_eventing_server() {
    using psme::app::eventing::EventingServer;
    m_eventing_server.reset(new EventingServer{m_configuration});
    psme::command::CommandJson::Map::set_implementation("Eventing");
    m_eventing_server->add(psme::command::CommandJson::Map::get_instance());
}

void App::init_registration_server() {
    using jsonrpc::HttpServer;
    using psme::app::registration::RegistrationServer;
    m_reg_server_connector.reset(new HttpServer{
                        m_configuration["registration"]["port"].as_int(),"","",1});
    m_registration_server.reset(new RegistrationServer{*m_reg_server_connector});
    psme::command::CommandJson::Map::set_implementation(
                        m_configuration["commands"]["generic"].as_string());
    m_registration_server->add(psme::command::CommandJson::Map::get_instance());
}

void App::init_rest_server() {
    using psme::rest::server::RestServer;
    m_rest_server.reset(new RestServer);
}

void App::init_rest_event_service() {
    using psme::rest::eventing::EventService;
    m_rest_event_service.reset(new EventService());
}

void App::init_subscription_config() {
    using rest::eventing::config::SubscriptionConfig;
    SubscriptionConfig::get_instance()->set_config_file(
            m_configuration["subscription-config-file"].as_string());
    SubscriptionConfig::get_instance()->load();
}

void App::init_accounts_config() {
    using rest::account::config::AccountConfig;
//    AccountConfig::get_instance()->set_config_file(
//            m_configuration["account-config-file"].as_string());
    AccountConfig::get_instance()->load_admin();
    AccountConfig::get_instance()->loadAccounts();
}

void App::init_session_service() 
{
    using namespace psme::rest::session::manager;
    using rest::session::config::SessionConfig;	
    SessionManager::get_instance()->StartSessionThread();
    SessionConfig::get_instance()->loadSessionsConfig();
}

void App::init_roles_config() {
    using rest::account::config::RoleConfig;
    RoleConfig::get_instance()->loadPreRoles();
}

void App::init_registries() {
    using namespace rest::registries;
    const std::string& base_configuration{make_base_configuration()};
    RegistryConfigurator::get_instance()->load(base_configuration);
}

void App::init_ec_log_helper() 
{
    using namespace psme::rest::log::manager;
    LogManager::get_instance()->init();
}

void App::init() {
    try {
        init_logger();
        init_ec_log_helper();
        agent_framework::generic::ServiceUuid::get_instance();
        init_network_change_notifier();
        init_ssdp_service();
        init_eventing_server();
        init_rest_event_service();
        init_registration_server();
        init_subscription_config();
        init_roles_config();
        init_accounts_config();
        init_session_service();	
        init_registries();
        init_rest_server();
        m_model_watcher.reset(new rest::model::Watcher());
    }
    catch (std::exception& e) {
        log_error(GET_LOGGER("app"),
                "Failed to initialize Application: " << e.what());
        cleanup();
        exit(-1);
    }
    catch (...) {
        log_error(GET_LOGGER("app"),
                "Failed to initialize Application: Unknown exception.");
        cleanup();
        exit(-1);
    }
}
void App::init_bal()
{
#ifdef BAL
    auto &pOLT = Olt_Device::Olt_Device::get_instance();
    if (pOLT.is_bal_lib_init() != true)
    {
        log_error(GET_LOGGER("app"), "Failed to find BAL LIB in lib/ !!!");
    }
    else
        log_info(GET_LOGGER("app"), "BAL API exist.");
#endif
}
void App::init_ecrf_pal()
{
    auto &pecRF_Pal = Switch::Switch::get_instance();
}

void App::run() {
    try {
        if (m_network_change_notifier) {
            m_network_change_notifier->start();
        }
        m_eventing_server->start();
        m_rest_event_service->start();
        m_registration_server->start();
        m_rest_server->start();
        m_model_watcher->start();
        m_ssdp_service->start();
        wait_for_termination();
        log_info(LOGUSR, "Stopping PSME Application...");
    }
    catch (std::exception& e) {
        log_error(GET_LOGGER("app"), e.what());
    }
    catch (...) {
        log_error(GET_LOGGER("app"), "Unknown exception.");
    }
}

void App::wait_for_termination() {
    log_debug(LOGUSR, "Waiting for termination signal...");

    sigset_t sset;
    sigemptyset(&sset);
    sigaddset(&sset, SIGINT);
    sigaddset(&sset, SIGQUIT);
    sigaddset(&sset, SIGTERM);
    sigprocmask(SIG_BLOCK, &sset, NULL);
    int sig;
    sigwait(&sset, &sig);
}

void App::statics_cleanup() {
    psme::command::Command::Map::cleanup();
    psme::command::CommandJson::Map::cleanup();
    configuration::Configuration::cleanup();
    logger_cpp::LoggerFactory::cleanup();
}

void App::cleanup() {
    if (m_registration_server) {
        m_registration_server->stop();
        m_registration_server.reset();
    }
    if (m_eventing_server) {
        m_eventing_server->stop();
        m_eventing_server.reset();
    }
    if (m_rest_event_service) {
        m_rest_event_service->stop();
        m_rest_event_service.reset();
    }
    if (m_rest_server) {
        m_rest_server->stop();
        m_rest_server.reset();
    }
    if (m_ssdp_service) {
        m_ssdp_service->stop();
        m_ssdp_service.reset();
    }
    if (m_model_watcher) {
        m_model_watcher->stop();
        m_model_watcher.reset();
    }
    if (m_network_change_notifier) {
        m_network_change_notifier->stop();
        m_network_change_notifier.reset();
    }
    statics_cleanup();
}
