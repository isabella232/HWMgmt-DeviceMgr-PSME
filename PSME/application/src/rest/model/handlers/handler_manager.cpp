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
 * @file handler_manager.cpp
 * @brief HandlerManager class  implementation
 * */
#include "psme/rest/model/handlers/handler_manager.hpp"
#include "psme/rest/model/handlers/generic_handler_deps.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"
#include "psme/rest/model/handlers/drive_handler.hpp"
#include "psme/rest/model/handlers/endpoint_handler.hpp"
#include "psme/rest/model/handlers/logical_drive_handler.hpp"
#include "psme/rest/model/handlers/physical_drive_handler.hpp"
#include "psme/rest/model/handlers/ethernet_switch_port_handler.hpp"
#include "psme/rest/model/handlers/storage_subsystem_handler.hpp"
#include "psme/rest/model/handlers/zone_handler.hpp"
#include "psme/rest/model/handlers/pcie_function_handler.hpp"
#include "psme/rest/model/handlers/port_handler.hpp"
#include "psme/rest/model/handlers/manager_handler.hpp"
#include "psme/rest/model/handlers/task_handler.hpp"
#include "psme/rest/model/handlers/acl_handler.hpp"
#include "agent-framework/module/requests/compute.hpp"
#include "agent-framework/module/requests/storage.hpp"
#include "agent-framework/module/requests/network.hpp"
#include "agent-framework/module/requests/pnc.hpp"
#include "agent-framework/module/requests/chassis.hpp" 
#include "configuration/configuration.hpp"


namespace psme {
namespace rest {
namespace model {
namespace handler {

    using namespace agent_framework::model;
    using Component = agent_framework::model::enums::Component;

    // aliases for common handlers
    using ChassisHandler = GenericHandler
        <requests::GetChassisInfo, Chassis,
            IdPolicy<agent_framework::model::enums::Component::Chassis, NumberingZone::SHARED>>;

//Step 3.
    using FanHandler = GenericHandler
        <requests::GetFanInfo, Fan,
            IdPolicy<agent_framework::model::enums::Component::Fan, NumberingZone::PARENT_SPACE>>; 

    using ThermalZoneHandler = GenericHandler
        <requests::GetThermalZoneInfo, ThermalZone,
            IdPolicy<agent_framework::model::enums::Component::ThermalZone, NumberingZone::PARENT_SPACE>>; 

    using PowerZoneHandler = GenericHandler
        <requests::GetPowerZoneInfo, PowerZone,
            IdPolicy<agent_framework::model::enums::Component::PowerZone, NumberingZone::PARENT_SPACE>>; 

    using PsuHandler = GenericHandler
        <requests::GetPsuInfo, Psu,
            IdPolicy<agent_framework::model::enums::Component::PSU, NumberingZone::PARENT_SPACE>>; 

    // aliases for compute handlers
    using SystemHandler = GenericHandler
        <requests::GetSystemInfo, System,
            IdPolicy<Component::System, NumberingZone::SHARED>>;
    using ProcessorHandler = GenericHandler
        <requests::GetProcessorInfo, Processor,
            IdPolicy<Component::Processor, NumberingZone::PARENT_SPACE>>;
    using MemoryHandler = GenericHandler
        <requests::GetMemoryInfo, Memory,
            IdPolicy<Component::Memory, NumberingZone::PARENT_SPACE>>;
    using NetworkInterfaceHandler = GenericHandler
        <requests::GetNetworkInterfaceInfo, NetworkInterface,
            IdPolicy<Component::NetworkInterface, NumberingZone::PARENT_SPACE>>;
    using StorageControllerHandler = GenericHandler
        <requests::GetStorageControllerInfo, StorageController,
            IdPolicy<Component::StorageController, NumberingZone::PARENT_SPACE>>;
    using NetworkDeviceHandler = GenericHandler
        <requests::GetNetworkDeviceInfo, NetworkDevice,
            IdPolicy<Component::NetworkDevice, NumberingZone::PARENT_SPACE>>;
    using NetworkDeviceFunctionHandler = GenericHandler
        <requests::GetNetworkDeviceFunctionInfo, NetworkDeviceFunction,
            IdPolicy<Component::NetworkDeviceFunction, NumberingZone::PARENT_SPACE>>;
    // StorageSubsystemHandler is defined in storage_subsystem_handler.hpp
    // DriveHandler is defined in drive_handler.hpp

    // aliases for network handlers
    using EthernetSwitchHandler = GenericHandler
        <requests::GetEthernetSwitchInfo, EthernetSwitch,
            IdPolicy<Component::EthernetSwitch, NumberingZone::SHARED>>;
    using RemoteSwitchHandler = GenericHandler
        <requests::GetRemoteEthernetSwitchInfo, RemoteEthernetSwitch,
            IdPolicy<Component::RemoteEthernetSwitch, NumberingZone::SHARED>>;
    using PortVlanHandler = GenericHandler
        <requests::GetPortVlanInfo, EthernetSwitchPortVlan,
            IdPolicy<Component::EthernetSwitchPortVlan, NumberingZone::PARENT_SPACE>>;
    using VlanHandler = GenericHandler
        <requests::GetVlanInfo, Vlan,
            IdPolicy<Component::Vlan, NumberingZone::PARENT_SPACE>>;
    using AclRuleHandler = GenericHandler
        <requests::GetAclRuleInfo, AclRule,
            IdPolicy<Component::AclRule, NumberingZone::PARENT_SPACE>>;
    using StaticMacHandler = GenericHandler
        <requests::GetPortStaticMacInfo, StaticMac,
            IdPolicy<Component::StaticMac, NumberingZone::PARENT_SPACE>>;

    // aliases for storage handlers
    using StorageServiceHandler = GenericHandler
        <requests::GetStorageServicesInfo, StorageServices,
            IdPolicy<Component::StorageServices, NumberingZone::SHARED>>;
    using IscsiTargetHandler = GenericHandler
        <requests::GetIscsiTargetInfo, IscsiTarget,
            IdPolicy<Component::IscsiTarget, NumberingZone::PARENT_SPACE>>;

    // aliases for pnc handlers
    using FabricHandler = GenericHandler
    <requests::GetFabricInfo, Fabric,
        IdPolicy<Component::Fabric, NumberingZone::SHARED>>;
    using SwitchHandler = GenericHandler
        <requests::GetSwitchInfo, Switch,
            IdPolicy<Component::Switch, NumberingZone::PARENT_SPACE>>;
    // EndpointHandler is defined in endpoint_handler.hpp
    // PortHandler is defined in port_handler.hpp
    // ZoneHandler is defined in zone_handler.hpp
    using PcieDeviceHandler = GenericHandler
        <requests::GetPcieDeviceInfo, PcieDevice,
            IdPolicy<Component::PcieDevice, NumberingZone::PARENT_SPACE>>;
    // PcieFunctionHandler is defined in pcie_function_handler.hpp

    // custom aliases for readability
    using attribute::Array;
    using Collection = attribute::Collection;
    using CollectionType = agent_framework::model::enums::CollectionType;
    using Component = enums::Component;
    using Handlers = std::vector<HandlerInterface*>;
    using HandlersWithCollectionName =
            std::vector<std::pair<std::string, HandlerInterface*>>;
    using HandlerPtr = std::shared_ptr<HandlerInterface>;

    HandlerManager::~HandlerManager() {}

    HandlerManager::HandlerManager() :
    m_component_handlers{
        { enums::Component::Manager,
                                HandlerPtr(new ManagerHandler()) },
        { enums::Component::Chassis,
                                HandlerPtr(new ChassisHandler()) },
//Step 4.
	{ enums::Component::Fan,
                                HandlerPtr(new FanHandler()) },

	{ enums::Component::ThermalZone,
                                HandlerPtr(new ThermalZoneHandler()) },	
                                
 	{ enums::Component::PowerZone,
                                HandlerPtr(new PowerZoneHandler()) },    

 	{ enums::Component::PSU,
                                HandlerPtr(new PsuHandler()) },                             
        { enums::Component::System,
                                HandlerPtr(new SystemHandler()) },
        { enums::Component::Processor,
                                HandlerPtr(new ProcessorHandler()) },
        { enums::Component::Memory,
                                HandlerPtr(new MemoryHandler()) },
        { enums::Component::NetworkInterface,
                                HandlerPtr(new NetworkInterfaceHandler()) },
        { enums::Component::StorageController,
                                HandlerPtr(new StorageControllerHandler()) },
        { enums::Component::StorageSubsystem,
                                HandlerPtr(new StorageSubsystemHandler()) },
        { enums::Component::NetworkDevice,
                                HandlerPtr(new NetworkDeviceHandler()) },
        { enums::Component::NetworkDeviceFunction,
                                HandlerPtr(new NetworkDeviceFunctionHandler()) },
        { enums::Component::Drive,
                                HandlerPtr(new DriveHandler()) },
        { enums::Component::EthernetSwitch,
                                HandlerPtr(new EthernetSwitchHandler()) },
        { enums::Component::EthernetSwitchPort,
                                HandlerPtr(new EthernetSwitchPortHandler()) },
        { enums::Component::EthernetSwitchPortVlan,
                                HandlerPtr(new PortVlanHandler()) },
        { enums::Component::Vlan,
                                HandlerPtr(new VlanHandler()) },
        { enums::Component::Acl,
                                HandlerPtr(new AclHandler()) },
        { enums::Component::AclRule,
                                HandlerPtr(new AclRuleHandler()) },
        { enums::Component::StaticMac,
                                HandlerPtr(new StaticMacHandler()) },
        { enums::Component::RemoteEthernetSwitch,
                                HandlerPtr(new RemoteSwitchHandler()) },
        { enums::Component::NeighborSwitch,
                                HandlerPtr(new RemoteSwitchHandler()) },
        { enums::Component::StorageServices,
                                HandlerPtr(new StorageServiceHandler()) },
        { enums::Component::IscsiTarget,
                                HandlerPtr(new IscsiTargetHandler()) },
        { enums::Component::LogicalDrive,
                                HandlerPtr(new LogicalDriveHandler()) },
        { enums::Component::PhysicalDrive,
                                HandlerPtr(new PhysicalDriveHandler()) },
        { enums::Component::Switch,
                                HandlerPtr(new SwitchHandler()) },
        { enums::Component::Zone,
                                HandlerPtr(new ZoneHandler()) },
        { enums::Component::Port,
                                HandlerPtr(new PortHandler()) },
        { enums::Component::Endpoint,
                                HandlerPtr(new EndpointHandler()) },
        { enums::Component::Fabric,
                                HandlerPtr(new FabricHandler()) },
        { enums::Component::PcieDevice,
                                HandlerPtr(new PcieDeviceHandler()) },
        { enums::Component::PcieFunction,
                                HandlerPtr(new PcieFunctionHandler()) },
        { enums::Component::Task,
                                HandlerPtr(new TaskHandler()) }
    },
    m_collection_handlers{
        { enums::CollectionType::Systems,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::System)->second) },
        { enums::CollectionType::Processors,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Processor)->second) },
        { enums::CollectionType::Memory,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Memory)->second) },
        { enums::CollectionType::NetworkInterfaces,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::NetworkInterface)->second) },
        { enums::CollectionType::EthernetSwitchPortVlans,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::EthernetSwitchPortVlan)->second) },

//Step 5.
        { enums::CollectionType::Fans,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Fan)->second) },

        { enums::CollectionType::ThermalZones,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::ThermalZone)->second) },		

        { enums::CollectionType::PowerZones,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::PowerZone)->second) },

        { enums::CollectionType::PSUs,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::PSU)->second) },				
		
        { enums::CollectionType::StorageControllers,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::StorageController)->second) },
        { enums::CollectionType::StorageSubsystems,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::StorageSubsystem)->second) },
        { enums::CollectionType::NetworkDevices,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::NetworkDevice)->second) },
        { enums::CollectionType::NetworkDeviceFunctions,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::NetworkDeviceFunction)->second) },
        { enums::CollectionType::Drives,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Drive)->second) },
        { enums::CollectionType::EthernetSwitches,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::EthernetSwitch)->second) },
        { enums::CollectionType::NeighborSwitches,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::NeighborSwitch)->second) },
        { enums::CollectionType::EthernetSwitchPorts,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::EthernetSwitchPort)->second) },
        { enums::CollectionType::PortMembers,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::EthernetSwitchPort)->second) },
        { enums::CollectionType::Vlans,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Vlan)->second) },
        { enums::CollectionType::Acls,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Acl)->second) },
        { enums::CollectionType::Rules,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::AclRule)->second) },
        { enums::CollectionType::StaticMacs,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::StaticMac)->second) },
        { enums::CollectionType::StorageServices,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::StorageServices)->second) },
        { enums::CollectionType::LogicalDrives,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::LogicalDrive)->second) },
        { enums::CollectionType::PhysicalDrives,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::PhysicalDrive)->second) },
        { enums::CollectionType::iSCSITargets,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::IscsiTarget)->second) },
        { enums::CollectionType::Managers,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Manager)->second) },
        { enums::CollectionType::Fabrics,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Fabric)->second) },
        { enums::CollectionType::Chassis,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Chassis)->second) },
        { enums::CollectionType::Switches,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Switch)->second) },
        { enums::CollectionType::Zones,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Zone)->second) },
        { enums::CollectionType::Ports,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Port)->second) },
        { enums::CollectionType::Endpoints,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Endpoint)->second) },
        { enums::CollectionType::PCIeDevices,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::PcieDevice)->second) },
        { enums::CollectionType::PCIeFunctions,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::PcieFunction)->second) },
        { enums::CollectionType::Tasks,
                        HandlerPtr(m_component_handlers
                        .find(enums::Component::Task)->second) }
    }
    {}

    HandlerInterface* HandlerManager::get_handler(Component component) {
        auto handler = m_component_handlers.find(component);
        if (m_component_handlers.end() == handler) {
            THROW(agent_framework::exceptions::InvalidValue, "rest",
                "Handler for component \"" + std::string(component.to_string()) + "\" not defined.");
        }
        return handler->second.get();
    }

    HandlerInterface* HandlerManager::get_handler(CollectionType collection_type) {
        auto handler = m_collection_handlers.find(collection_type);
        if (m_component_handlers.end() == handler) {
            THROW(agent_framework::exceptions::InvalidValue, "rest",
                  "Handler for collection type \"" + std::string(collection_type.to_string()) + "\" not defined.");
        }
        return handler->second.get();
    }

    HandlersWithCollectionName HandlerManager::get_handlers(Array<Collection> collections) {
        HandlersWithCollectionName handlers;
        for (const auto& collection : collections) {
            auto handler = m_collection_handlers.find(collection.get_type());
            if(m_collection_handlers.end() == handler) {
                log_error(GET_LOGGER("rest"),
                    "Handler for collection type: \"" +
                    std::string(collection.get_type().to_string()) +
                    "\" not defined, proceeding with get_handlers().");
            }
            else {
                handlers.push_back(std::make_pair(
                    collection.get_name(), handler->second.get()));
            }
        }
        return handlers;
    }

    Handlers HandlerManager::get_handlers() const {
        Handlers handlers{};
        for(const auto& handler : m_component_handlers) {
            handlers.push_back(handler.second.get());
        }
        return handlers;
    }

    void HandlerManager::remove_agent_data(const std::string& in_gami_id) {
        Handlers handlers = get_handlers();
        for(const auto& handler : handlers) {
            handler->remove_agent_data(in_gami_id);
        }
    }

} /* handler */
} /* model */
} /* rest */
} /* psme */
