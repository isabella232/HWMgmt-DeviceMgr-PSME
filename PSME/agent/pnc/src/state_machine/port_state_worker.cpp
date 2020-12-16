/*!
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
 * @file state_machine/port_state_worker.cpp
 * @brief Implementation of the PortStateWorker
 * */

#include "state_machine/port_state_worker.hpp"
#include "discovery/discovery_manager.hpp"
#include "gas/global_address_space_registers.hpp"

#include <thread>
#include <chrono>

using namespace agent::pnc::state_machine;
using namespace agent::pnc::discovery;
using namespace agent::pnc::gas;
using namespace agent::pnc::gas::mrpc;
using namespace agent_framework::model;
using namespace agent_framework::module;
using namespace agent_framework::eventing;
using namespace agent::pnc::tools;

namespace {

GlobalAddressSpaceRegisters get_gas(const std::string& switch_uuid) {
    auto pcie_switch = get_manager<Switch>().get_entry(switch_uuid);
    GlobalAddressSpaceRegisters gas = GlobalAddressSpaceRegisters::get_default(pcie_switch.get_memory_path());
    gas.read_top();
    gas.read_partition(gas.top.output.fields.current_partition_id);
    return gas;
}

constexpr std::size_t SYSFS_UPDATE_TIME_SEC = 10;
constexpr std::size_t UNBIND_DISCOVERY_DELAY_SEC = 1;

}

PortStateWorker::~PortStateWorker() {}

uint8_t PortStateWorker::get_bridge_id(const std::string& switch_uuid, const std::string& port_uuid) const {

    log_debug(GET_LOGGER("port-state-worker"), "\tAction: gathering data...");
    Port port = get_manager<Port>().get_entry(port_uuid);
    auto gas = get_gas(switch_uuid);

    log_debug(GET_LOGGER("port-state-worker"), "\tAction: checking for bindings...");
    PortBindingInfo pbi = m_tools.gas_tool->get_port_binding_info(gas, uint8_t(port.get_phys_port_id()));
    uint8_t bridge_id = m_tools.gas_tool->get_logical_bridge_for_port(pbi);
    log_debug(GET_LOGGER("port-state-worker"), "\tAction: Bound to bridge " << unsigned(bridge_id));
    return bridge_id;
}

void PortStateWorker::ib_discovery(const std::string& switch_uuid, const std::string& port_uuid, uint8_t bridge_id,
    const std::string& drive_uuid) const {

    log_debug(GET_LOGGER("port-state-worker"), "\tAction: discovering (ib)...");
    if (!m_dm.ib_port_device_discovery(switch_uuid, port_uuid, bridge_id, drive_uuid)) {
        throw std::runtime_error{"In-band discovery procedure failed"};
    }
    log_debug(GET_LOGGER("port-state-worker"), "\tAction: In-band discovery successful");
}

uint8_t PortStateWorker::bind_to_host(const std::string& switch_uuid, const std::string& port_uuid) const {

    log_debug(GET_LOGGER("port-state-worker"), "\tAction: gathering data...");
    Port port = get_manager<Port>().get_entry(port_uuid);
    auto gas = get_gas(switch_uuid);

    log_debug(GET_LOGGER("port-state-worker"), "\tAction: binding to an empty bridge...");
    PartitionBindingInfo pbi = m_tools.gas_tool->get_partition_binding_info(
        gas, gas.top.output.fields.current_partition_id);
    uint8_t bridge_id = m_tools.gas_tool->get_available_bridge_id(pbi);
    m_tools.gas_tool->bind_to_partition(gas, uint8_t(port.get_phys_port_id()),
        gas.top.output.fields.current_partition_id, bridge_id);

    std::this_thread::sleep_for(std::chrono::seconds(::SYSFS_UPDATE_TIME_SEC));

    log_debug(GET_LOGGER("port-state-worker"), "\tAction: bound to bridge " << unsigned(bridge_id));
    log_debug(GET_LOGGER("port-state-worker"), "\tAction: binding successful");
    return bridge_id;
}

void PortStateWorker::unbind_from_host(const std::string& switch_uuid, uint8_t bridge_id) const {

    log_debug(GET_LOGGER("port-state-worker"), "\tAction: gathering data...");
    Switch sw = get_manager<Switch>().get_entry(switch_uuid);
    auto gas = get_gas(switch_uuid);

    log_debug(GET_LOGGER("port-state-worker"), "\tAction: unbinding...");
    m_tools.gas_tool->unbind_management_host_driver(sw.get_bridge_path(), bridge_id);
    m_tools.gas_tool->unbind_from_partition(gas, gas.top.output.fields.current_partition_id, bridge_id);

    // wait for system to update sysfs
    std::this_thread::sleep_for(std::chrono::seconds(UNBIND_DISCOVERY_DELAY_SEC));
    log_debug(GET_LOGGER("port-state-worker"), "\tAction: unbinding successful");
}

void PortStateWorker::full_discovery(const std::string& switch_uuid, const std::string& port_uuid,
        uint8_t bridge_id) const {

    log_debug(GET_LOGGER("port-state-worker"), "\tAction: Action: Full (Oob + Ib) discovery...");
    try {
        std::string drive_uuid = oob_discovery(switch_uuid, port_uuid);
        ib_discovery(switch_uuid, port_uuid, bridge_id, drive_uuid);
    }
    catch (...) {
        log_debug(GET_LOGGER("port-state-worker"), "Full discovery procedure failed!");
        throw;
    }
    log_debug(GET_LOGGER("port-state-worker"), "\tAction: Full (Oob + Ib) discovery successful");
}

std::string PortStateWorker::oob_discovery(const std::string& switch_uuid, const std::string& port_uuid) const {

    log_debug(GET_LOGGER("port-state-worker"), "\tAction: gathering data...");
    Port port = get_manager<Port>().get_entry(port_uuid);
    auto gas = get_gas(switch_uuid);

    // perform discovery
    log_debug(GET_LOGGER("port-state-worker"), "\tAction: discovering (oob)...");
    if (!m_dm.oob_port_device_discovery(gas, switch_uuid, port_uuid)) {
        throw std::runtime_error{"Out-of-band discovery procedure failed"};
    }

    // get function and drive uuids
    log_debug(GET_LOGGER("port-state-worker"), "\tAction: retrieving uuids...");
    std::string drive_uuid{};
    try {
        drive_uuid = get_drive_by_dsp_port(port_uuid);
    }
    catch (const std::invalid_argument&) {
        log_info(GET_LOGGER("port-state-worker"), "\tNon drive device found!");
    }
    log_debug(GET_LOGGER("port-state-worker"), "\tAction: oob discovery successful");

    return drive_uuid;
}

void PortStateWorker::remove(const std::string& switch_uuid, const std::string& port_uuid) const {
    log_debug(GET_LOGGER("port-state-worker"), "\tAction: gathering data...");
    auto gas = get_gas(switch_uuid);

    if (!m_dm.remove_devices_on_port(gas, port_uuid)) {
        throw std::runtime_error{"Remove procedure failed"};
    }

    log_debug(GET_LOGGER("port-state-worker"), "Action: removal completed");
}

std::string PortStateWorker::get_drive_by_dsp_port(const std::string& port_uuid) const {

    auto drives = m_tools.model_tool->get_drives_by_dsp_port_uuid(port_uuid);

    // we expect only one drive to be found
    if (1 != drives.size()) {
        log_debug(GET_LOGGER("agent"),
            "Invalid number of drives using specified dsp port, expected 1, found " << drives.size());
        throw std::invalid_argument("Invalid number of drives using specified dsp port");
    }

    return drives.front();
}
