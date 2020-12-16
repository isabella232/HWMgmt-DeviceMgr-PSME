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

#include "psme/rest/constants/constants.hpp"
#include "psme/rest/endpoints/system/system_network_interface.hpp"
#include "psme/rest/utils/status_helpers.hpp"



using namespace psme::rest;
using namespace psme::rest::constants;
using namespace agent_framework::module;



namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#EthernetInterface.EthernetInterface";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#EthernetInterface.v1_4_0.EthernetInterface";
    r[Common::ID] = json::Value::Type::NIL;
    r[Common::NAME] = "Ethernet Interface";
    r[Common::DESCRIPTION] = "Ethernet Interface description";
    r[Common::STATUS][Common::STATE] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL;
    r[Common::OEM] = json::Value::Type::OBJECT;
    r[constants::NetworkInterface::PERMANENT_MAC_ADDRESS] = json::Value::Type::NIL;
    r[constants::Common::MAC_ADDRESS] = json::Value::Type::NIL;
    r[constants::NetworkInterface::SPEED] = json::Value::Type::NIL;
    r[constants::NetworkInterface::AUTO_NEG] = json::Value::Type::NIL;
    r[constants::NetworkInterface::FULL_DUPLEX] = json::Value::Type::NIL;
    r[constants::NetworkInterface::HOST_NAME] = json::Value::Type::NIL;
    r[constants::NetworkInterface::FQDN] = json::Value::Type::NIL;
    r[constants::NetworkInterface::MTU_SIZE] = json::Value::Type::NIL;
    r[constants::NetworkInterface::LINK_STATUS] = json::Value::Type::NIL;

    r[constants::NetworkInterface::IPv4_ADDRESSES] = json::Value::Type::ARRAY;

    r[constants::NetworkInterface::IPv6_STATIC_ADDRESSES] = json::Value::Type::ARRAY;

    r[constants::NetworkInterface::IPv6_ADDRESS_POLICY_TABLE] = json::Value::Type::ARRAY;

    r[constants::NetworkInterface::MAX_IPv6_STATIC_ADDRESSES] = json::Value::Type::NIL;
    r[constants::NetworkInterface::IPv6_ADDRESSES] = json::Value::Type::ARRAY;
    r[constants::NetworkInterface::IPv6_DEFAULT_GATEWAY] = json::Value::Type::NIL;
    r[constants::NetworkInterface::NAME_SERVERS] = json::Value::Type::ARRAY;
    r[constants::NetworkInterface::VLANS] = json::Value::Type::NIL;
    r[constants::NetworkInterface::INTERFACE_ENABLED] = json::Value::Type::NIL;

    json::Value links;
    links[Fabric::ENDPOINTS] = json::Value::Type::ARRAY;
    links[Common::OEM][Common::RACKSCALE][Common::ODATA_TYPE] = "#Intel.Oem.EthernetInterface";
    r[Common::LINKS] = std::move(links);
    return r;
}
}

endpoint::SystemNetworkInterface::SystemNetworkInterface(const std::string& path) : EndpointBase(path) {}
endpoint::SystemNetworkInterface::~SystemNetworkInterface() {}

void endpoint::SystemNetworkInterface::get(const server::Request& req, server::Response& res) {
    auto r = ::make_prototype();

    r[constants::Common::ODATA_ID] = PathBuilder(req).build();
    r[constants::Common::ID] = req.params[PathParam::NIC_ID];

    auto ni = psme::rest::model::Find<agent_framework::model::NetworkInterface>(req.params[PathParam::NIC_ID])
        .via<agent_framework::model::System>(req.params[PathParam::SYSTEM_ID]).get();

    endpoint::status_to_json(ni, r);

    r[constants::NetworkInterface::PERMANENT_MAC_ADDRESS] = ni.get_factory_mac_address();
    r[constants::Common::MAC_ADDRESS] = ni.get_mac_address();
    r[constants::NetworkInterface::SPEED] = ni.get_speed_mbps();
    r[constants::NetworkInterface::AUTO_NEG] = ni.get_autosense();
    r[constants::NetworkInterface::FULL_DUPLEX] = ni.get_full_duplex();
    r[constants::NetworkInterface::MAX_IPv6_STATIC_ADDRESSES] = ni.get_max_ipv6_static_addresses();

    const auto& ipv4_addresses = ni.get_ipv4_addresses();
    for(const auto& ipv4_addr : ipv4_addresses) {
        if (ipv4_addr.get_address().has_value()) {
            json::Value ipv4_address;
            ipv4_address[IpAddress::ADDRESS] = ipv4_addr.get_address();
            ipv4_address[IpAddress::SUBNET_MASK] = ipv4_addr.get_subnet_mask();
            ipv4_address[IpAddress::ADDRESS_ORIGIN] = ipv4_addr.get_address_origin();
            ipv4_address[IpAddress::GATEWAY] = ipv4_addr.get_gateway();
            r[constants::NetworkInterface::IPv4_ADDRESSES].push_back(std::move(ipv4_address));
        }
    }

    const auto& ipv6_addresses = ni.get_ipv6_addresses();
    for(const auto& ipv6_addr : ipv6_addresses) {
        if (ipv6_addr.get_address().has_value()) {
            json::Value ipv6_address;
            ipv6_address[IpAddress::ADDRESS] = ipv6_addr.get_address();
            ipv6_address[IpAddress::PREFIX_LENGTH] = ipv6_addr.get_prefix_length();
            // in GAMI there is DHCP option which has to be shown as DHCPv6
            auto addr_origin = ipv6_addr.get_address_origin();
            ipv6_address[IpAddress::ADDRESS_ORIGIN] =
                addr_origin == agent_framework::model::enums::Ipv6AddressOrigin::DHCP ?
                                   json::Value("DHCPv6") : json::Value(addr_origin);
            ipv6_address[IpAddress::ADDRESS_STATE] = ipv6_addr.get_address_state();
            r[constants::NetworkInterface::IPv6_ADDRESSES].push_back(std::move(ipv6_address));
        }
    }

    r[constants::NetworkInterface::IPv6_DEFAULT_GATEWAY] = ni.get_ipv6_default_gateway();
    if (ni.get_status().get_state() == agent_framework::model::enums::State::Enabled) {
        r[constants::NetworkInterface::INTERFACE_ENABLED] = true;
    }
    else {
       r[constants::NetworkInterface::INTERFACE_ENABLED] = false;
    }

    set_response(res, r);
}
