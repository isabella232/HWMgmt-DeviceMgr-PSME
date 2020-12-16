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
 * @file fabric.hpp
 * @brief literals for Fabric endpoints
 * */

#pragma once

namespace psme {
namespace rest {
namespace constants {

/*!
 * @brief common constant literals for Fabric endpoints.
 */
namespace FabricCommon {
extern const char FIRMWARE_VERSION[];
extern const char ACTIONS[];
extern const char ALLOWABLE_RESET_TYPES[];
extern const char FORCE_OFF[];
extern const char FORCE_RESTART[];
extern const char FORCE_ON[];
extern const char SKU[];
extern const char TARGET[];
}

/*!
 * @brief Constant literals for Fabric endpoint.
 */
namespace Fabric {
extern const char ZONES[];
extern const char ENDPOINTS[];
extern const char SWITCHES[];
extern const char MAX_ZONES[];
extern const char FABRIC_TYPE[];
}

/*!
 * @brief Constant literals for Switch endpoint.
 */
namespace Switch {
extern const char DOMAIN_ID[];
extern const char TOTAL_SWITCH_WIDTH[];
extern const char IS_MANAGED[];
extern const char REDUNDANCY[];
extern const char SWITCH_RESET[];
extern const char SWITCH_RESET_ENDPOINT[];
extern const char GRACEFUL_RESTART[];
extern const char ON[];
extern const char GRACEFUL_SHUTDOWN[];
extern const char NMI[];
extern const char PUSH_POWER_BUTTON[];
extern const char INDICATOR_LED[];
extern const char PORTS[];
extern const char POWER_STATE[];
extern const char SWITCH_TYPE[];
}

/*!
 * @brief Constant literals for Zone endpoint.
 */
namespace Zone {
extern const char INVOLVED_SWITCHES[];
}

/*!
 * @brief Constant literals for Port endpoint.
 */
namespace Port {
extern const char PORT_RESET[];
extern const char PORT_RESET_ENDPOINT[];
extern const char MAX_SPEED[];
extern const char CURRENT_SPEED[];
extern const char WIDTH[];
extern const char PORT_ID[];
extern const char PORT_PROTOCOL[];
extern const char PORT_TYPE[];
extern const char PCIE_CONNECTION_ID[];
extern const char ASSOCIATED_ENDPOINTS[];
extern const char CONNECTED_SWITCHES[];
extern const char CONNECTED_SWITCH_PORTS[];
}

/*!
 * @brief Constant literals for Endpoint endpoint.
 */
namespace Endpoint {
extern const char ENDPOINT_PROTOCOL[];
extern const char PCI_ID[];
extern const char CONNECTED_ENTITIES[];
extern const char ENTITY_TYPE[];
extern const char ENTITY_ROLE[];
extern const char PCI_FUNCTION_NUMBER[];
extern const char PCI_CLASS_CODE[];
extern const char ENTITY_PCI_ID[];
extern const char ENTITY_LINK[];
extern const char REDUNDANCY[];
extern const char HOST_RESERVATION_MEMORY_BYTES[];
}

}
}
}
