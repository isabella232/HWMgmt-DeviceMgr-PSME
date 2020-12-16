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

#include "psme/rest/validators/schemas/system.hpp"
#include "psme/rest/constants/constants.hpp"
#include "agent-framework/module/enum/compute.hpp"

using namespace psme::rest;
using namespace psme::rest::validators::schema;
using namespace agent_framework::model;

const jsonrpc::ProcedureValidator& SystemPatchSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "system_patch",
        jsonrpc::PARAMS_BY_NAME,
        constants::System::BOOT, VALID_OPTIONAL(VALID_ATTRIBUTE(BootSchema)),
        constants::System::MEMROY_STATUS, VALID_OPTIONAL(VALID_ATTRIBUTE(MemoryFreeUsagePercentageSchema)),
        constants::System::CPU_STATUS, VALID_OPTIONAL(VALID_ATTRIBUTE(CpuUsagePercentageSchema)),
        constants::System::STORAGE_STATUS, VALID_OPTIONAL(VALID_ATTRIBUTE(StorageUsagePercentageSchema)),
        constants::Common::ASSET_TAG, VALID_OPTIONAL(VALID_JSON_STRING),
        nullptr
    };
    return procedure;
}

const jsonrpc::ProcedureValidator& SystemPatchSchema::BootSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "boot_object",
        jsonrpc::PARAMS_BY_NAME,
        constants::System::BOOT_SOURCE_OVERRIDE_ENABLED, VALID_OPTIONAL(VALID_ENUM(enums::BootOverride)),
        constants::System::BOOT_SOURCE_OVERRIDE_TARGET, VALID_OPTIONAL(VALID_ENUM(enums::BootOverrideTarget)),
        constants::System::BOOT_SOURCE_OVERRIDE_MODE, VALID_OPTIONAL(VALID_ENUM(enums::BootOverrideMode)),
        nullptr
    };
    return procedure;
}

const jsonrpc::ProcedureValidator& SystemPatchSchema::MemoryFreeUsagePercentageSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "boot_object",
        jsonrpc::PARAMS_BY_NAME,
        constants::ChassisThreshold::LOWERTHRESHOLDNONCRITICAL, VALID_JSON_REAL,
        nullptr
    };
    return procedure;
}

const jsonrpc::ProcedureValidator& SystemPatchSchema::CpuUsagePercentageSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "boot_object",
        jsonrpc::PARAMS_BY_NAME,
        constants::ChassisThreshold::UPPERTHRESHOLDNONCRITICAL, VALID_JSON_REAL,
        nullptr
    };
    return procedure;
}

const jsonrpc::ProcedureValidator& SystemPatchSchema::StorageUsagePercentageSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "boot_object",
        jsonrpc::PARAMS_BY_NAME,
        constants::ChassisThreshold::UPPERTHRESHOLDNONCRITICAL, VALID_JSON_REAL,
        nullptr
    };
    return procedure;
}

