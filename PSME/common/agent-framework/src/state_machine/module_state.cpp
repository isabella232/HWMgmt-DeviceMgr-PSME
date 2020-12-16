/*!
 * @section LICENSE
 *
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
 * @section DESCRIPTION
*/

#include "agent-framework/state_machine/module_state.hpp"

using namespace agent_framework::state_machine;

namespace {
std::array<const char*, 5> g_state_names = {{
    "Unknown",
    "Enabled",
    "UnavailableOffline",
    "Absent",
    "Starting"
}};
}

ModuleState::~ModuleState() {}

const char* ModuleState::get_state_name(enums::State state) {
    return g_state_names[unsigned(state)];
}
