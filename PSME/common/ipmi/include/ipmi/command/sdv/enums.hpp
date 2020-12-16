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
 *
 * @file enums.hpp
 *
 * @brief IPMI sdv enums.
 * */

#pragma once

namespace ipmi {
namespace command {
namespace sdv {

/*!
 * @brief Represents codes for Network Function tailored to IPMI spec.
 */
enum NetFn : std::uint8_t {
    QUANTA = 0x30,
    INTEL = 0x38,
    RACKSCALE = 0x3A
};

/*!
 * @brief Represents codes for IPMI Command.
 */
enum Cmd : std::uint8_t {
    GET_THERMAL_STATE = 0x08,
    GET_FAN_PWM = 0x01,
    GET_SLOT_ID = 0x09,
    GET_NODE_INFO = 0x0A,
    GET_BIOS_VERSION = 0x83,
    GET_PROCESSOR_INFO = 0x1B,
    GET_DIMM_INFO = 0x1D,
    GET_SYSTEM_MAC_INFO = 0x19,
    SET_ISCSI_FIELD = 0x02,
    GET_ISCSI_FIELD = 0x03,
    SET_OOB_BOOT_OPTION = 0x14,
    GET_OOB_BOOT_OPTION = 0x15,
    GET_CABLE_ID = 0x16,
    GET_MDR_DATA_REGIONS_STATUS = 0x20,
    MDR_REGION_READ = 0x22
};

/*!
 * @brief Data Region IDs
 */
enum DataRegionId : std::uint8_t {
    SMBIOS_TABLE = 0x1,
    RESERVED
};

}
}
}
