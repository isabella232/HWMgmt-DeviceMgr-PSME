/*!
 * @brief Implementation of DeletePortVlan command.
 *
 * File contains all implementations of methods for DeletePortVlan command.
 *
 * @copyright Copyright (c) 2016-2017 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @header{Files}
 * @file delete_port_vlan.cpp
 */

#include "agent-framework/module/network_components.hpp"
#include "agent-framework/command-ref/registry.hpp"
#include "agent-framework/command-ref/network_commands.hpp"
#include "api/netlink/switch_vlan.hpp"
#include "api/netlink/switch_port_info.hpp"

using namespace agent_framework::command_ref;
using namespace agent_framework::module;
using namespace agent_framework::model;
using namespace agent_framework;
using namespace agent::network;
using namespace agent::network::api::netlink;

namespace {
void delete_port_vlan(const DeletePortVlan::Request& request, DeletePortVlan::Response&) {

    auto network_components = NetworkComponents::get_instance();
    auto& port_vlan_manager = network_components->get_port_vlan_manager();
    const auto& port_manager = network_components->get_port_manager();
    auto port_vlan = port_vlan_manager.get_entry(request.get_port_vlan());
    auto port = port_manager.get_entry(port_vlan.get_parent_uuid());

    SwitchVlan vlan(SwitchVlan::VlanId(port_vlan.get_vlan_id()));
    vlan.remove_port(port.get_port_identifier());

    /* if the vlan is removed from a downlink port check all other
     * downlink ports to see if we need to remove the vlan from CPU port */
    if (enums::PortType::Downstream == port.get_port_type()) {
        bool exists = false;
        for (auto uuid : port_manager.get_keys()) {
            auto other_port = port_manager.get_entry(uuid);
            SwitchPortInfo port_info(other_port.get_port_identifier());
            if ((enums::PortType::Downstream == other_port.get_port_type())
                && (other_port.get_port_identifier() !=
                    port.get_port_identifier())
                && port_info.vlan_exists(vlan.get_vlan_id())) {
                exists = true;
                break;
            }
        }
        if (!exists) {
            /* the vlan is removed from all downlinks ports, remove it from
             * the CPU port as well */
            vlan.remove_port(SwitchPortInfo::CPU_PORT);
        }
    }

    /* remove port-vlan from network manager */
    port_vlan_manager.remove_entry(port_vlan.get_uuid());
}
}

REGISTER_COMMAND(DeletePortVlan, ::delete_port_vlan);
