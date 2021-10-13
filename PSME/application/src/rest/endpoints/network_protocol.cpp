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

#include "psme/rest/endpoints/network_protocol.hpp"
#include "psme/rest/constants/constants.hpp"
#include "json/json.hpp"
#include "configuration/configuration.hpp"
#include <map>
#include "ecsys_helper/ecsys_helper.hpp"
#include "psme/rest/utils/ec_common_utils.hpp"
using namespace psme::rest::utils;
using namespace psme::rest;
using namespace psme::rest::constants;
using namespace agent_framework::model::enums;
using namespace ecsys_helper;

namespace {

constexpr const char SERVER[] = "server";
constexpr const char CONNECTORS[] = "connectors";
constexpr const char PORT[] = "port";
constexpr const char USE_SSL[] = "use-ssl";
constexpr const int DEFAULT_SSDP_PORT = 1900;
constexpr const char SSDP_SERVICE[] = "ssdp-service";
constexpr const char ENABLED[] = "enabled";
constexpr const char TTL[] = "ttl";
constexpr const char ANNOUNCE_INTERVAL_SECONDS[] = "announce-interval-seconds";

json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#ManagerNetworkProtocol.ManagerNetworkProtocol";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#ManagerNetworkProtocol.v1_0_2.ManagerNetworkProtocol";
    r[Common::ID] = json::Value::Type::NIL;
    r[Common::NAME] = "Manager Network Protocol";
    r[Common::DESCRIPTION] = "Manager Network Protocol description";
    r[Common::STATUS][Common::STATE] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL;
    r[NetworkProtocol::HOST_NAME] = json::Value::Type::NIL;
    r[NetworkProtocol::FQDN] = json::Value::Type::NIL;
    r[NetworkProtocol::HTTP][NetworkProtocol::PROTOCOL_ENABLED] = json::Value::Type::NIL;
    r[NetworkProtocol::HTTP][NetworkProtocol::PORT] = json::Value::Type::NIL;
    r[NetworkProtocol::HTTPS][NetworkProtocol::PROTOCOL_ENABLED] = json::Value::Type::NIL;
    r[NetworkProtocol::HTTPS][NetworkProtocol::PORT] = json::Value::Type::NIL;
    r[NetworkProtocol::IPMI][NetworkProtocol::PROTOCOL_ENABLED] = json::Value::Type::NIL;
    r[NetworkProtocol::IPMI][NetworkProtocol::PORT] = json::Value::Type::NIL;
    r[NetworkProtocol::SSH][NetworkProtocol::PROTOCOL_ENABLED] = json::Value::Type::NIL;
    r[NetworkProtocol::SSH][NetworkProtocol::PORT] = json::Value::Type::NIL;
    r[NetworkProtocol::SNMP][NetworkProtocol::PROTOCOL_ENABLED] = json::Value::Type::NIL;
    r[NetworkProtocol::SNMP][NetworkProtocol::PORT] = json::Value::Type::NIL;
    r[NetworkProtocol::VIRTUAL_MEDIA][NetworkProtocol::PROTOCOL_ENABLED] = json::Value::Type::NIL;
    r[NetworkProtocol::VIRTUAL_MEDIA][NetworkProtocol::PORT] = json::Value::Type::NIL;
    r[NetworkProtocol::SSDP][NetworkProtocol::PROTOCOL_ENABLED] = json::Value::Type::NIL;
    r[NetworkProtocol::SSDP][NetworkProtocol::PORT] = json::Value::Type::NIL;
    r[NetworkProtocol::SSDP][NetworkProtocol::NOTIFY_IPV6_SCOPE] = json::Value::Type::NIL;
    r[NetworkProtocol::SSDP][NetworkProtocol::NOTIFY_MULTICAST_INTERVAL] = json::Value::Type::NIL;
    r[NetworkProtocol::SSDP][NetworkProtocol::NOTIFY_TTL] = json::Value::Type::NIL;
    r[NetworkProtocol::TELNET][NetworkProtocol::PROTOCOL_ENABLED] = json::Value::Type::NIL;
    r[NetworkProtocol::TELNET][NetworkProtocol::PORT] = json::Value::Type::NIL;
    r[NetworkProtocol::KVMIP][NetworkProtocol::PROTOCOL_ENABLED] = json::Value::Type::NIL;
    r[NetworkProtocol::KVMIP][NetworkProtocol::PORT] = json::Value::Type::NIL;
    r[Common::OEM] = json::Value::Type::OBJECT;
    return r;
}

json::Value read_protocols_for_drawer_manager() {
    json::Value r{};
    r[constants::NetworkProtocol::HTTPS][constants::NetworkProtocol::PROTOCOL_ENABLED] = true;

    const json::Value connectors =
        configuration::Configuration::get_instance().
        to_json()[SERVER][CONNECTORS];

    for (const auto& connector : connectors) {
        if (connector[USE_SSL].as_bool()) {
            r[constants::NetworkProtocol::HTTPS][constants::NetworkProtocol::PORT] =
                connector[PORT].as_uint();
        }
        else {
            r[constants::NetworkProtocol::HTTP][constants::NetworkProtocol::PORT] =
                connector[PORT].as_uint();
        }
    }

    return r;
}

void read_ssdp_properties_from_config(json::Value& r) {
    const json::Value config = configuration::Configuration::get_instance().to_json();
    auto& ssdp_config = config[SSDP_SERVICE];
    if (ssdp_config[ENABLED].as_bool()) {
        r[NetworkProtocol::SSDP][NetworkProtocol::PROTOCOL_ENABLED] = true;
        r[NetworkProtocol::SSDP][NetworkProtocol::PORT] = DEFAULT_SSDP_PORT;
        r[NetworkProtocol::SSDP][NetworkProtocol::NOTIFY_TTL] = ssdp_config[TTL];

        // zero interval means no notifications
        if (ssdp_config[ANNOUNCE_INTERVAL_SECONDS] != 0) {
            r[NetworkProtocol::SSDP][NetworkProtocol::NOTIFY_MULTICAST_INTERVAL] = ssdp_config[ANNOUNCE_INTERVAL_SECONDS];
        }
        else
            r[NetworkProtocol::SSDP][NetworkProtocol::NOTIFY_MULTICAST_INTERVAL] = 0;
			
    }
}

}

endpoint::NetworkProtocol::NetworkProtocol(const std::string& path) : EndpointBase(path) {}

endpoint::NetworkProtocol::~NetworkProtocol() {}

void endpoint::NetworkProtocol::get(const server::Request &req, server::Response &res)
{
    auto r = make_prototype();

    r[Common::ODATA_ID] = PathBuilder(req).build();
    r[Common::ID] = Manager::NETWORK_PROTOCOL;

    std::map<NetworkServiceName, std::string> service_name_to_rest_literal {
        {NetworkServiceName::HTTP, constants::NetworkProtocol::HTTP},
        {NetworkServiceName::HTTPS, constants::NetworkProtocol::HTTPS},
        {NetworkServiceName::VirtualMedia, constants::NetworkProtocol::VIRTUAL_MEDIA},
        {NetworkServiceName::SNMP, constants::NetworkProtocol::SNMP},
        {NetworkServiceName::Telnet, constants::NetworkProtocol::TELNET},
        {NetworkServiceName::SSDP, constants::NetworkProtocol::SSDP},
        {NetworkServiceName::IPMI, constants::NetworkProtocol::IPMI},
        {NetworkServiceName::SSH, constants::NetworkProtocol::SSH},
        {NetworkServiceName::KVMIP, constants::NetworkProtocol::KVMIP}};

    auto manager = psme::rest::model::Find<agent_framework::model::Manager>(req.params[PathParam::MANAGER_ID]).get();
    for (const auto &service : manager.get_network_services())
    {
        r[service_name_to_rest_literal[service.get_name()]][constants::NetworkProtocol::PROTOCOL_ENABLED] = service.get_enabled();
        r[service_name_to_rest_literal[service.get_name()]][constants::NetworkProtocol::PORT] = service.get_port();
    }

    if (ManagerInfoType::EnclosureManager == manager.get_manager_type())
    {
        r += read_protocols_for_drawer_manager();
    }

    if (endpoint::utils::is_manager_for_drawer_or_enclosure(manager.get_uuid()))
    {
        read_ssdp_properties_from_config(r);

        char command[HT_BUFFER_LEN] = {0};
        char resultA[HT_BUFFER_LEN] = {0};

		sprintf(command, "%s" ,"mgmt_protocol.sh get ssh_status");	
        EcCommonUtils::exec_shell(command, resultA, 0);
		
		if(!strncmp("Enable", resultA, 6))
		{
                  r["SSH"]["ProtocolEnabled"] = true;
		}
		else
		{
                  r["SSH"]["ProtocolEnabled"] = false;
		}

		sprintf(command, "%s" ,"mgmt_protocol.sh get ssh_port");
		memset(resultA,0x0, sizeof(resultA));
        EcCommonUtils::exec_shell(command, resultA, 0);

		if(strlen(resultA) != 0)
                  r["SSH"]["Port"] = atoi(resultA);

		sprintf(command, "%s" ,"mgmt_protocol.sh get snmp_status");	
        EcCommonUtils::exec_shell(command, resultA, 0);
		
		if(!strncmp("Enable", resultA, 6))
		{
                  r["SNMP"]["ProtocolEnabled"] = true;
		}
		else
		{
                  r["SNMP"]["ProtocolEnabled"] = false;
		}

		sprintf(command, "%s" ,"mgmt_protocol.sh get snmp_port");
		memset(resultA,0x0, sizeof(resultA));
        EcCommonUtils::exec_shell(command, resultA, 0);

		if(strlen(resultA) != 0)
                  r["SNMP"]["Port"] = atoi(resultA);
		
	       sprintf(command, "%s" ,"hostname");
              memset(resultA,0x0, sizeof(resultA));
        EcCommonUtils::exec_shell(command, resultA, 0);

		if(strlen(resultA) != 0)
		{
			resultA[strcspn(resultA, "\r\n")]=0;
			r["HostName"] = resultA;
		}

	        sprintf(command, "%s" ,"hostname -f");
                memset(resultA,0x0, sizeof(resultA));
        EcCommonUtils::exec_shell(command, resultA, 0);

		if(strlen(resultA) != 0)
		{
			resultA[strcspn(resultA, "\r\n")]=0;
			r["FQDN"] = resultA;
		}

             r[Common::STATUS][Common::STATE] = "Enabled";
             r[Common::STATUS][Common::HEALTH] = "OK";
             r[Common::STATUS][Common::HEALTH_ROLLUP] = "OK";		
    }

    res.set_header("Link", "<http://redfish.dmtf.org/schemas/ManagerNetworkProtocol.json>;rel=\"describedby\"");

    set_response(res, r);
}
