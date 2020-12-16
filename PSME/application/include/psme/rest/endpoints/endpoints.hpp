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



#include "redfish.hpp"
#include "root.hpp"
#include "metadata_root.hpp"
#include "metadata.hpp"
#include "event_service.hpp"

//viclin add begin
#include "account_service.hpp"
#include "account_accounts_collection.hpp"
#include "account_roles_collection.hpp"
#include "account.hpp"
#include "role.hpp"
#include "olt.hpp"
//viclin add end

#include "task_service.hpp"
#include "task_collection.hpp"
#include "task.hpp"
#include "message_registry_file_collection.hpp"
#include "message_registry_file.hpp"
#include "event_subscriptions_collection.hpp"
#include "event_subscription.hpp"
#include "test_event_subscription.hpp"
#include "manager.hpp"
#include "manager_collection.hpp"
#include "manager_network_interface.hpp"
#include "manager_network_interface_collection.hpp"
#include "manager_serial_interface_collection.hpp"
#include "manager_serial_interface.hpp"
#include "manager_log_services_collection.hpp"
#include "manager_log_services.hpp"
#include "manager_log_services_entries_collection.hpp"
#include "manager_log_services_entries.hpp"

#include "monitor.hpp"
#include "network_protocol.hpp"
#include "odata_service_document.hpp"

#include "system/system.hpp"
#include "system/systems_collection.hpp"
#include "system/system_reset.hpp"
#include "system/log_reset.hpp"
#include "system/memory.hpp"
#include "system/memory_collection.hpp"
#include "system/system_network_interface.hpp"
#include "system/system_network_interface_collection.hpp"
#include "system/processor.hpp"
#include "system/processors_collection.hpp"
#include "system/storage_subsystem.hpp"
#include "system/storage_subsystems_collection.hpp"
#include "system/volumes_collection.hpp"
#include "system/system_network_interface.hpp"
#include "system/system_network_interface_collection.hpp"
#include "system/network_interface.hpp"
#include "system/network_interfaces_collection.hpp"
#include "system/network_device_function.hpp"
#include "system/network_device_functions_collection.hpp"

#include "storage/physical_drive.hpp"
#include "storage/physical_drives_collection.hpp"
#include "storage/logical_drive_collection.hpp"
#include "storage/logical_drive.hpp"
#include "storage/storage_services.hpp"
#include "storage/storage_services_collection.hpp"
#include "storage/remote_target.hpp"
#include "storage/remote_target_collection.hpp"

#include "ethernet/ethernet_switch.hpp"
#include "ethernet/ethernet_switch_collection.hpp"
#include "ethernet/ethernet_switch_port.hpp"
#include "ethernet/pon_port_onus.hpp"
#include "ethernet/ethernet_switch_port_collection.hpp"
#include "ethernet/ethernet_switch_port_onus.hpp"
#include "ethernet/ethernet_switch_port_onus_omci.hpp"
#include "ethernet/olt_flow.hpp"
#include "ethernet/olt_flow_id.hpp"
#include "ethernet/ethernet_switch_port_onus_collection.hpp"
#include "ethernet/vlan_network_interface.hpp"
#include "ethernet/vlan_network_interface_collection.hpp"
#include "ethernet/mgmt_vlan_network_interface_collection.hpp"
#include "ethernet/mgmt_vlan_network_interface.hpp"
#include "ethernet/acl_collection.hpp"
#include "ethernet/acl.hpp"
#include "ethernet/acl_bind.hpp"
#include "ethernet/rule_collection.hpp"
#include "ethernet/rule.hpp"
#include "ethernet/static_mac_collection.hpp"
#include "ethernet/static_mac.hpp"
#include "chassis/chassis.hpp"
#include "chassis/chassis_collection.hpp"
#include "chassis/thermal_zone.hpp"
#include "chassis/thermal_zones_collection.hpp"
#include "chassis/thermal_collection.hpp"
#include "chassis/psu_collection.hpp"
#include "chassis/temperature_collection.hpp"
#include "chassis/chassis_reset.hpp"
#include "session_service.hpp"
#include "sessions.hpp"
#include "chassis/power_zone.hpp"
#include "chassis/power_zones_collection.hpp"
#include "chassis/drive.hpp"
#include "chassis/drives_collection.hpp"
#include "chassis/drive_secure_erase.hpp"
#include "chassis/pcie_device.hpp"
#include "chassis/pcie_devices_collection.hpp"
#include "chassis/pcie_function.hpp"
#include "chassis/pcie_functions_collection.hpp"
#include "fabric/fabric.hpp"
#include "fabric/fabrics_collection.hpp"
#include "fabric/endpoint.hpp"
#include "fabric/endpoints_collection.hpp"
#include "fabric/switch.hpp"
#include "fabric/switches_collection.hpp"
#include "fabric/switch_reset.hpp"
#include "fabric/port.hpp"
#include "fabric/port_reset.hpp"
#include "fabric/ports_collection.hpp"
#include "fabric/zone.hpp"
#include "fabric/zones_collection.hpp"

