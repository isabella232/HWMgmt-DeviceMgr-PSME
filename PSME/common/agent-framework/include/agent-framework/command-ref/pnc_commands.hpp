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
 *
 * @file pnc_commands.cpp
 *
 * @brief Declarations of all pnc commands
 * */

#pragma once
#include "agent-framework/command-ref/command.hpp"
#include "agent-framework/module/model/model_pnc.hpp"
#include "agent-framework/module/requests/common.hpp"
#include "agent-framework/module/requests/pnc.hpp"
#include "agent-framework/module/responses/common.hpp"
#include "agent-framework/module/responses/pnc.hpp"

namespace agent_framework {
namespace command_ref {

// declarations of get collection methods
using GetCollection = Command<model::requests::GetCollection, model::attribute::Array<model::attribute::SubcomponentEntry>>;
using GetManagersCollection = Command<model::requests::GetManagersCollection, model::attribute::Array<model::attribute::ManagerEntry>>;
using GetTasksCollection = Command<model::requests::GetTasksCollection, model::attribute::Array<model::attribute::TaskEntry>>;

// declarations of all get info methods
using GetChassisInfo = Command<model::requests::GetChassisInfo, model::Chassis>;
using GetDriveInfo = Command<model::requests::GetDriveInfo, model::Drive>;
using GetManagerInfo = Command<model::requests::GetManagerInfo, model::Manager>;
using GetSystemInfo = Command<model::requests::GetSystemInfo, model::System>;
using GetStorageSubsystemInfo = Command<model::requests::GetStorageSubsystemInfo, model::StorageSubsystem>;
using GetFabricInfo = Command<model::requests::GetFabricInfo, model::Fabric>;
using GetPcieDeviceInfo = Command<model::requests::GetPcieDeviceInfo, model::PcieDevice>;
using GetPcieFunctionInfo = Command<model::requests::GetPcieFunctionInfo, model::PcieFunction>;
using GetEndpointInfo = Command<model::requests::GetEndpointInfo, model::Endpoint>;
using GetPortInfo = Command<model::requests::GetPortInfo, model::Port>;
using GetSwitchInfo = Command<model::requests::GetSwitchInfo, model::Switch>;
using GetZoneInfo = Command<model::requests::GetZoneInfo, model::Zone>;
using GetTaskInfo = Command<model::requests::GetTaskInfo, model::Task>;
using GetTaskResultInfo = Command<model::requests::GetTaskResultInfo, model::responses::GetTaskResultInfo>;

// declarations of all add methods
using AddZone = Command<model::requests::AddZone, model::responses::AddZone>;
using AddZoneEndpoint = Command<model::requests::AddZoneEndpoint, model::responses::AddZoneEndpoint>;

// declarations of all delete methods
using DeleteZone = Command<model::requests::DeleteZone, model::responses::DeleteZone>;
using DeleteZoneEndpoint = Command<model::requests::DeleteZoneEndpoint, model::responses::DeleteZoneEndpoint>;
using DeleteTask = Command<model::requests::DeleteTask, model::responses::DeleteTask>;

// declarations of all set methods
using SetComponentAttributes = Command<model::requests::SetComponentAttributes, model::responses::SetComponentAttributes>;

}
}
