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
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * @file chassis_loader.hpp
 * @brief Module loader interface
 * */

#define MAX_LAG 10

#pragma once



#include "agent-framework/module/loader/loader.hpp"

#include <atomic>
#include <condition_variable>



namespace agent {
namespace chassis {
namespace loader {

class ChassisLoader : public agent_framework::module::loader::Loader {
public:
    ~ChassisLoader();


    bool load(const json::Value&) override;


    /*!
     * Block thread of execution until all chassis components are fully loaded.
     * After chassis is loaded, all blocked threads are notified and continue
     * their execution. This is needed to avoid premature event sending
     * (before tree stabilization takes places).
     * */
    static void wait_for_complete();


private:
    static std::mutex m_mutex;
    static std::condition_variable m_cv;
    static std::atomic<bool> m_is_loaded;
};

}
}
}
