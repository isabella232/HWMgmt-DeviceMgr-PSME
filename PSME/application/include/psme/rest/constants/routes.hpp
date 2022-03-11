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
 * @file routes.hpp
 * */

#pragma once



#include <string>



namespace psme {
namespace rest {
namespace constants {

class Routes {
public:
    static const std::string BASE_ROOT;	
    static const std::string REDFISH_PATH;
    static const std::string ROOT_PATH;
    static const std::string ODATA_SERVICE_DOCUMENT;
    static const std::string RPVfoobar;
    static const std::string METADATA_ROOT_PATH;
    static const std::string METADATA_PATH;
    static const std::string EVENT_SERVICE_PATH;
    static const std::string ACCOUNT_SERVICE_PATH; //viclin add begin
    static const std::string ACCOUNT_COLLECTION_PATH;
    static const std::string ACCOUNT_PATH;
    static const std::string ROLE_COLLECTION_PATH;    
    static const std::string ROLE_PATH;            //viclin add end
    static const std::string SUBSCRIPTION_COLLECTION_PATH;
    static const std::string SUBSCRIPTION_PATH;
    static const std::string TEST_EVENT_SUBSCRIPTION_PATH;
    static const std::string TASK_SERVICE_PATH;
    static const std::string TASK_COLLECTION_PATH;
    static const std::string TASK_PATH;
    static const std::string MESSAGE_REGISTRY_FILE_COLLECTION_PATH;
    static const std::string MESSAGE_REGISTRY_FILE_PATH;
    static const std::string MONITOR_PATH;

    static const std::string CHASSIS_COLLECTION_PATH;
    static const std::string CHASSIS_PATH;
    static const std::string DRIVES_COLLECTION_PATH;
    static const std::string DRIVE_PATH;
    static const std::string DRIVE_SECURE_ERASE_PATH;
    static const std::string PCIE_DEVICES_COLLECTION_PATH;
    static const std::string PCIE_DEVICE_PATH;
    static const std::string PCIE_FUNCTIONS_COLLECTION_PATH;
    static const std::string PCIE_FUNCTION_PATH;
    static const std::string FANS_COLLECTION_PATH;
    static const std::string FANS_PATH;

    static const std::string PSU_COLLECTION_PATH;
    static const std::string PSUS_PATH;
    static const std::string THERMALS_COLLECTION_PATH;
    static const std::string THERMAL_COLLECTION_PATH;
    static const std::string THERMALS_PATH;
    static const std::string SESSION_SERVICE_PATH;
    static const std::string SESSION_COLLECTION_PATH;	
    static const std::string SESSION_PATH;
    static const std::string MEMBERS_PATH;
	
    static const std::string UPDATE_SERVICE_PATH;	
    static const std::string UPDATE_SERVICE_FIRMWARE_INVENTORY_PATH;	
    static const std::string UPDATE_SERVICE_SOFTWARE_INVENTORY_PATH;	
    static const std::string UPDATE_SERVICE_FIRMWARE_INVENTORY_MU_PATH;	
    static const std::string UPDATE_SERVICE_FIRMWARE_INVENTORY_NOS_PATH;	
    static const std::string UPDATE_SERVICE_SOFTWARE_INVENTORY_PACKAGE_PATH;	
    static const std::string CHASSIS_RESET_PATH;	
    static const std::string MANAGER_SERIAL_INTERFACE_COLLECTION_PATH;
    static const std::string MANAGER_SERIAL_INTERFACE_PATH;
    static const std::string VLAN_MANAGER_NETWORK_INTERFACE_COLLECTION_PATH;
    static const std::string VLAN_MANAGER_NETWORK_INTERFACE_PATH;
    static const std::string MANAGER_LOG_SERVICE_COLLECTION_PATH;
    static const std::string MANAGER_LOG_SERVICE_PATH;
    static const std::string MANAGER_LOG_SERVICE_ENTRIES_COLLECTION_PATH;	
    static const std::string MANAGER_LOG_SERVICE_ENTRIES_PATH;	
    static const std::string MANAGER_LOG_SERVICE_RESET_PATH;
	
    static const std::string ONUS_COLLECTION_PATH;
    static const std::string ONUS_COLLECTION_ONUS_PATH;
    static const std::string ONUS_COLLECTION_ONUS_OMCI_PATH;
    static const std::string FLOW_COLLECTION_PATH;
    static const std::string FLOW_ID_PATH;
    static const std::string OLT_PATH;
    static const std::string THERMAL_ZONE_COLLECTION_PATH;
    static const std::string THERMAL_ZONE_PATH;
    static const std::string POWER_ZONE_COLLECTION_PATH;
    static const std::string POWER_ZONE_PATH;

    static const std::string MANAGER_COLLECTION_PATH;
    static const std::string MANAGER_PATH;
    static const std::string MANAGER_NETWORK_INTERFACE_COLLECTION_PATH;
    static const std::string MANAGER_NETWORK_INTERFACE_PATH;
    static const std::string NETWORK_PROTOCOL_PATH;

    static const std::string SYSTEMS_COLLECTION_PATH;
    static const std::string SYSTEM_PATH;
    static const std::string SYSTEM_RESET_PATH;
    static const std::string STORAGE_SUBSYSTEMS_COLLECTION_PATH;
    static const std::string STORAGE_SUBSYSTEM_PATH;
    static const std::string SYSTEM_ETHERNET_INTERFACES_COLLECTION_PATH;
    static const std::string SYSTEM_ETHERNET_INTERFACE_PATH;
    static const std::string MEMORY_COLLECTION_PATH;
    static const std::string MEMORY_PATH;
    static const std::string PROCESSORS_COLLECTION_PATH;
    static const std::string PROCESSOR_PATH;
    static const std::string VOLUMES_COLLECTION_PATH;
    static const std::string NETWORK_INTERFACES_COLLECTION_PATH;
    static const std::string NETWORK_INTERFACE_PATH;
    static const std::string NETWORK_DEVICE_FUNCTIONS_COLLECTION_PATH;
    static const std::string NETWORK_DEVICE_FUNCTION_PATH;

    static const std::string ETHERNET_SWITCHES_COLLECTION_PATH;
    static const std::string ETHERNET_SWITCH_PATH;
    static const std::string ACL_COLLECTION_PATH;
    static const std::string ACL_PATH;
    static const std::string ACL_BIND_PATH;
    static const std::string ACL_UNBIND_PATH;
    static const std::string RULES_COLLECTION_PATH;
    static const std::string RULE_PATH;
    static const std::string ETHERNET_SWITCH_PORT_COLLECTION_PATH;
    static const std::string ETHERNET_SWITCH_PORT_PATH;
    static const std::string STATIC_MAC_COLLECTION_PATH;
    static const std::string STATIC_MAC_PATH;
    static const std::string VLAN_NETWORK_INTERFACE_COLLECTION_PATH;
    static const std::string VLAN_NETWORK_INTERFACE_PATH;

    static const std::string STORAGE_SERVICE_COLLECTION_PATH;
    static const std::string STORAGE_SERVICE_PATH;
    static const std::string PHYSICAL_DRIVES_COLLECTION_PATH;
    static const std::string PHYSICAL_DRIVE_PATH;
    static const std::string LOGICAL_DRIVE_COLLECTION_PATH;
    static const std::string LOGICAL_DRIVE_PATH;
    static const std::string REMOTE_TARGET_COLLECTION_PATH;
    static const std::string REMOTE_TARGET_PATH;

    static const std::string FABRICS_COLLECTION_PATH;
    static const std::string FABRIC_PATH;
    static const std::string ENDPOINTS_COLLECTION_PATH;
    static const std::string ENDPOINT_PATH;
    static const std::string SWITCHES_COLLECTION_PATH;
    static const std::string SWITCH_PATH;
    static const std::string SWITCH_RESET_PATH;
    static const std::string ZONES_COLLECTION_PATH;
    static const std::string ZONE_PATH;
    static const std::string PORTS_COLLECTION_PATH;
    static const std::string PORT_PATH;
    static const std::string PORT_RESET_PATH;
};

}
}
}
