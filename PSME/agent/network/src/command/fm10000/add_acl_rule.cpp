/*!
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2016-2017 Intel Corporation
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
 * @section DESCRIPTION
 * */

#include "agent-framework/module/network_components.hpp"
#include "agent-framework/command-ref/registry.hpp"
#include "agent-framework/command-ref/network_commands.hpp"
#include "api/acl.hpp"
#include "tree_stability/network_tree_stabilizer.hpp"

#include <vector>
#include <safe-string/safe_lib.hpp>
#include <net/if.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <netinet/in.h>
#include <netinet/ether.h>



using namespace agent_framework::command_ref;
using namespace agent_framework::module;
using namespace agent_framework::model;
using namespace agent_framework::model::literals;
using namespace agent_framework::model::attribute;
using namespace agent_framework;
using namespace agent::network;

namespace {

api::Acl::ActionType get_acl_action(const AddAclRule::Request& request) {
    if (!request.get_action().has_value()) {
        THROW(agent_framework::exceptions::InvalidValue, "fm10000",
              "Action is a mandatory parameter for adding ACL rule.");
    }

    const auto& action_enum = request.get_action().value();
    switch (action_enum) {
        case enums::AclAction::Permit:
            return api::Acl::ActionType::ACTION_PERMIT;
        case enums::AclAction::Deny:
            return api::Acl::ActionType::ACTION_DENY;
        case enums::AclAction::Forward:
            return api::Acl::ActionType::ACTION_FORWARD;
        case enums::AclAction::Mirror:
            return api::Acl::ActionType::ACTION_MIRROR;
        default:
            THROW(agent_framework::exceptions::InvalidValue, "fm10000",
                  "Invalid action for adding ACL rule: '" + std::string(action_enum.to_string()) + "'.");
    }
}


string get_port_identifier(const string& port_uuid) {
    auto network_manager = NetworkComponents::get_instance();
    auto& port_manager = network_manager->get_port_manager();
    const auto port = port_manager.get_entry(port_uuid);
    return port.get_port_identifier();
}


string get_destination_port(const AddAclRule::Request& request) {
    if (!request.get_forward_mirror_port().has_value()) {
        THROW(agent_framework::exceptions::InvalidValue, "fm10000",
              "Forward/mirror port is required for actions Forward and Mirror.");
    }
    const string& port_uuid = request.get_forward_mirror_port();
    return get_port_identifier(port_uuid);
}


api::Acl::MirrorType get_mirror_type(const AddAclRule::Request& request) {
    if (!request.get_mirror_type().has_value()) {
        THROW(agent_framework::exceptions::InvalidValue, "fm10000",
              "Mirror type is required for action Mirror.");
    }
    const auto& mirror_type_enum = request.get_mirror_type().value();
    switch (mirror_type_enum) {
        case enums::AclMirrorType::Egress:
            return api::Acl::MirrorType::MIRROR_EGRESS;
        case enums::AclMirrorType::Ingress:
            return api::Acl::MirrorType::MIRROR_INGRESS;
        case enums::AclMirrorType::Bidirectional:
            return api::Acl::MirrorType::MIRROR_BIDIRECTIONAL;
        case enums::AclMirrorType::Redirect:
            return api::Acl::MirrorType::MIRROR_REDIRECT;
        default:
            THROW(agent_framework::exceptions::InvalidValue, "fm10000",
                  "Invalid mirror type for adding ACL rule: '" + std::string(mirror_type_enum.to_string()) + "'.");
    }
}


void check_if_rule_id_exists(const string& acl_uuid, const uint16_t rule_id) {
    auto network_manager = NetworkComponents::get_instance();
    auto& rule_manager = network_manager->get_acl_rule_manager();
    for (const auto& rule_uuid : rule_manager.get_keys(acl_uuid)) {
        auto model_rule = rule_manager.get_entry(rule_uuid);
        if (model_rule.get_rule_id() == rule_id) {
            log_error(GET_LOGGER("fm10000"), "Rule ID already exists (UUID = " << acl_uuid << ").");
            throw agent_framework::exceptions::InvalidValue("Rule ID already exists.");
        }
    }
}


uint16_t get_rule_id(const AddAclRule::Request& request) {
    if (request.get_rule_id().has_value()) {
        auto rule_id = uint16_t(request.get_rule_id());
        check_if_rule_id_exists(request.get_acl(), rule_id);
        return rule_id;
    }
    return 0;
}


string get_acl_name(const AddAclRule::Request& request) {
    const auto acl_name = NetworkComponents::get_instance()->get_acl_manager().get_entry(request.get_acl()).get_name();
    if (acl_name.has_value()) {
        return acl_name.value();
    }
    else {
        // This should never happen
        throw std::logic_error("Empty ACL name (memory corruption possible)");
    }
}


void get_vlan_id(const AddAclRule::Request& request,
                 std::vector<api::Acl::Condition>& conditions) {
    const auto& vlan_id = request.get_vlan_id();
    if (vlan_id.get_id().has_value()) {
        api::Acl::Condition condition{};
        condition.type = api::Acl::ConditionType::CONDITION_VLAN;
        condition.value = vlan_id.get_id();
        if (vlan_id.get_mask().has_value()) {
            condition.mask = vlan_id.get_mask();
        }
        conditions.push_back(condition);
    }
}


uint64_t get_ip_from_string(const string& ip_str) {
    struct in_addr addr{};
    if (0 == inet_aton(ip_str.c_str(), &addr)) {
        THROW(agent_framework::exceptions::InvalidValue, "fm10000",
              "Unable to parse IP string: '" + ip_str + "'.");
    }
    return addr.s_addr;
}


void get_ip(const AclAddress& ip,
            api::Acl::Condition& condition) {
    string ip_str = ip.get_address();
    condition.value = get_ip_from_string(ip_str);
    if (ip.get_mask().has_value()) {
        string mask = ip.get_mask();
        condition.mask = get_ip_from_string(mask);
    }
}


void get_source_ip(const AddAclRule::Request& request,
                   std::vector<api::Acl::Condition>& conditions) {
    const auto& ip = request.get_source_ip();
    if (ip.get_address().has_value()) {
        api::Acl::Condition condition{};
        condition.type = api::Acl::ConditionType::CONDITION_IP_SRC;
        get_ip(ip, condition);
        conditions.push_back(condition);
    }
}


void get_destination_ip(const AddAclRule::Request& request,
                        std::vector<api::Acl::Condition>& conditions) {
    const auto& ip = request.get_destination_ip();
    if (ip.get_address().has_value()) {
        api::Acl::Condition condition{};
        condition.type = api::Acl::ConditionType::CONDITION_IP_DEST;
        get_ip(ip, condition);
        conditions.push_back(condition);
    }
}


uint64_t get_mac_from_string(const string& mac_str) {
    uint64_t ret{};
    struct ether_addr* address = ether_aton(mac_str.c_str());
    if (address == NULL) {
        THROW(agent_framework::exceptions::InvalidValue, "fm10000",
              "MAC address must be in format ab:cd:ef:gh:ij:kl, got " + mac_str);
    }
    std::reverse(address->ether_addr_octet, address->ether_addr_octet + ETH_ALEN);
    if (EOK != memcpy_s(&ret, sizeof(ret), address->ether_addr_octet, ETH_ALEN)) {
        throw std::runtime_error("Cannot copy memory with MAC address.");
    }
    return ret;
}


void get_mac(const AclAddress& mac, api::Acl::Condition& condition) {
    string mac_str = mac.get_address();
    condition.value = get_mac_from_string(mac_str);
    if (mac.get_mask().has_value()) {
        string mask = mac.get_mask();
        condition.mask = get_mac_from_string(mask);
    }
}


void get_source_mac(const AddAclRule::Request& request,
                    std::vector<api::Acl::Condition>& conditions) {
    const auto& mac = request.get_source_mac();
    if (mac.get_address().has_value()) {
        api::Acl::Condition condition{};
        condition.type = api::Acl::ConditionType::CONDITION_MAC_SRC;
        get_mac(mac, condition);
        conditions.push_back(condition);
    }
}


void get_destination_mac(const AddAclRule::Request& request,
                         std::vector<api::Acl::Condition>& conditions) {
    const auto& mac = request.get_destination_mac();
    if (mac.get_address().has_value()) {
        api::Acl::Condition condition{};
        condition.type = api::Acl::ConditionType::CONDITION_MAC_DEST;
        get_mac(mac, condition);
        conditions.push_back(condition);
    }
}


void get_port(const AclPort& port, api::Acl::Condition& condition) {
    condition.value = port.get_port();
    if (port.get_mask().has_value()) {
        condition.mask = port.get_mask();
    }
}


void get_source_port(const AddAclRule::Request& request,
                     std::vector<api::Acl::Condition>& conditions) {
    const auto& port = request.get_source_port();
    if (port.get_port().has_value()) {
        api::Acl::Condition condition{};
        condition.type = api::Acl::ConditionType::CONDITION_L4_PORT_SRC;
        get_port(port, condition);
        conditions.push_back(condition);
    }
}


void get_destination_port(const AddAclRule::Request& request,
                          std::vector<api::Acl::Condition>& conditions) {
    const auto& port = request.get_destination_port();
    if (port.get_port().has_value()) {
        api::Acl::Condition condition{};
        condition.type = api::Acl::ConditionType::CONDITION_L4_PORT_DEST;
        get_port(port, condition);
        conditions.push_back(condition);
    }
}


void get_protocol(const AddAclRule::Request& request,
                  std::vector<api::Acl::Condition>& conditions) {
    if (request.get_protocol().has_value()) {
        api::Acl::Condition condition{};
        condition.type = api::Acl::ConditionType::CONDITION_PROTOCOL;
        condition.value = request.get_protocol().value();
        conditions.push_back(condition);
    }
}


std::vector<api::Acl::Condition> get_conditions(const AddAclRule::Request& request) {
    std::vector<api::Acl::Condition> conditions;
    get_vlan_id(request, conditions);
    get_source_ip(request, conditions);
    get_destination_ip(request, conditions);
    get_source_mac(request, conditions);
    get_destination_mac(request, conditions);
    get_source_port(request, conditions);
    get_destination_port(request, conditions);
    get_protocol(request, conditions);
    if (0 == conditions.size()) {
        THROW(agent_framework::exceptions::InvalidValue, "fm10000",
              "At least one condition is required for adding ACL rule.");
    }
    return conditions;
}


std::vector<string> get_mirrored_ports(const AddAclRule::Request& request) {
    std::vector<string> mirrored_ports{};
    for (const auto& port_uuid : request.get_mirrored_ports()) {
        mirrored_ports.push_back(get_port_identifier(port_uuid));
    }
    if (0 == mirrored_ports.size()) {
        THROW(agent_framework::exceptions::InvalidValue, "fm10000",
              "At least one mirror port is required for action Mirror.");
    }
    return mirrored_ports;
}


void add_mirror_rule(const AddAclRule::Request& request) {
    auto mirror_type = get_mirror_type(request);
    auto destination_port = get_destination_port(request);
    auto mirrored_ports = get_mirrored_ports(request);
    auto rule_id = get_rule_id(request);
    auto conditions = get_conditions(request);

    try {
        auto acl_name = get_acl_name(request);
        api::Acl::add_mirror_rule(acl_name, rule_id, mirror_type,
                                  destination_port, mirrored_ports,
                                  conditions);
    }
    catch (const std::exception& e) {
        THROW(agent_framework::exceptions::Fm10000Error, "fm10000",
              string("Failed to create ACL rule on switch: ") + e.what());
    }
}


void add_forward_rule(const AddAclRule::Request& request) {
    auto destination_port = get_destination_port(request);
    auto rule_id = get_rule_id(request);
    auto conditions = get_conditions(request);

    try {
        auto acl_name = get_acl_name(request);
        api::Acl::add_rule(acl_name, rule_id,
                           api::Acl::ActionType::ACTION_FORWARD,
                           destination_port, conditions);
    }
    catch (const std::exception& e) {
        THROW(agent_framework::exceptions::Fm10000Error, "fm10000",
              string("Failed to create ACL rule on switch: ") + e.what());
    }
}


void add_permit_or_deny_rule(const AddAclRule::Request& request,
                             const api::Acl::ActionType& action) {
    auto rule_id = get_rule_id(request);
    auto conditions = get_conditions(request);

    try {
        auto acl_name = get_acl_name(request);
        api::Acl::add_rule(acl_name, rule_id, action, "", conditions);
    }
    catch (const std::exception& e) {
        THROW(agent_framework::exceptions::Fm10000Error, "fm10000",
              string("Failed to create ACL rule on switch: ") + e.what());
    }
}


void add_rule_on_switch(const AddAclRule::Request& request) {
    auto acl_action = get_acl_action(request);
    switch (acl_action) {
        case api::Acl::ActionType::ACTION_MIRROR:
            add_mirror_rule(request);
            break;
        case api::Acl::ActionType::ACTION_FORWARD:
            add_forward_rule(request);
            break;
        case api::Acl::ActionType::ACTION_PERMIT:
        case api::Acl::ActionType::ACTION_DENY:
            add_permit_or_deny_rule(request, acl_action);
            break;
        default:
            break;
    }
}


static uint32_t get_new_rule_id(const string& acl_uuid,
                                const api::Acl::Rules& rules) {
    auto network_manager = NetworkComponents::get_instance();
    auto& rule_manager = network_manager->get_acl_rule_manager();
    for (const auto& rule : rules) {
        bool found{false};
        for (const auto& rule_uuid : rule_manager.get_keys(acl_uuid)) {
            auto model_rule = rule_manager.get_entry(rule_uuid);
            if (model_rule.get_rule_id() == rule.get_rule_id()) {
                found = true;
                break;
            }
        }
        // if not found in the model, this is our new rule id
        if (!found) {
            return rule.get_rule_id();
        }
    }
    log_error(GET_LOGGER("fm10000"),
              string("Unable to find new rule id assigned by the switch"));
    return 0;
}


static uint32_t find_rule_id(const AddAclRule::Request& request) {
    if (request.get_rule_id().has_value()) {
        return request.get_rule_id().value();
    }

    // get rule id as assigned by the switch API
    api::Acl acl{};
    const auto acl_name = NetworkComponents::get_instance()->get_acl_manager().get_entry(request.get_acl()).get_name();

    try {
        acl.read_acl_rule_list(acl_name.value());
        return get_new_rule_id(request.get_acl(), acl.get_rules());
    }
    catch (const std::experimental::bad_optional_access&) {
        // This code should never execute
        log_error(GET_LOGGER("fm10000"),
                  "Failed reading ACL rule id from switch: empty ACL name (memory corruption possible)");
        return 0;
    }
    catch (const std::exception& e) {
        log_error(GET_LOGGER("fm10000"),
                  string("Failed reading ACL rule id from switch: ") +
                  e.what());
        return 0;
    }
}


void init_acl_rule(const AddAclRule::Request& request,
                   model::AclRule& rule) {
    rule.set_rule_id(find_rule_id(request));
    rule.set_action(request.get_action());
    rule.set_forward_mirror_port(request.get_forward_mirror_port());
    rule.set_mirrored_ports(request.get_mirrored_ports());
    if (request.get_mirror_type().has_value()) {
        rule.set_mirror_type(request.get_mirror_type());
    }
    rule.set_vlan_id(request.get_vlan_id());
    rule.set_source_ip(request.get_source_ip());
    rule.set_destination_ip(request.get_destination_ip());
    rule.set_source_mac(request.get_source_mac());
    rule.set_destination_mac(request.get_destination_mac());
    rule.set_source_port(request.get_source_port());
    rule.set_destination_port(request.get_destination_port());
    rule.set_protocol(request.get_protocol());
    rule.set_status({enums::State::Enabled, enums::Health::OK});
}


void add_rule(const AddAclRule::Request& request, AddAclRule::Response& response) {
    auto network_manager = NetworkComponents::get_instance();
    auto& acl_manager = network_manager->get_acl_manager();
    auto& rule_manager = network_manager->get_acl_rule_manager();

    // check if given ACL UUID exists
    acl_manager.get_entry(request.get_acl());

    model::AclRule rule(request.get_acl());

    add_rule_on_switch(request);

    init_acl_rule(request, rule);

    const std::string rule_uuid = rule.get_uuid();

    rule_manager.add_entry(std::move(rule));
    const std::string rule_persistent_uuid = NetworkTreeStabilizer().stabilize_acl_rule(rule_uuid);

    response.set_rule(rule_persistent_uuid);
}
}

REGISTER_COMMAND(AddAclRule, ::add_rule
);
