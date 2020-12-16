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

#include "agent-framework/threading/threadpool.hpp"
#include "logger/logger_factory.hpp"
#include <exception>

using namespace agent_framework::threading;

Task::Base::~Base() {}

Threadpool::Threadpool(const std::size_t thread_count) :
    m_thread_count(thread_count),
    m_threads(),
    m_tasks() {

    create_threads();
}

Threadpool::~Threadpool() {
    stop(true);
}

void Threadpool::stop(bool force) {
    if (force) {
        stop_threads();
    }

    join_threads();
    m_threads.clear();
}

void Threadpool::create_threads() {
    m_threads.reserve(m_thread_count);
    try {
        for (auto i = 0u; i < m_thread_count; i++) {
            m_threads.emplace_back(std::thread(&Threadpool::run_loop, this));
        }
    } catch (const std::exception& e) {
        log_error(GET_LOGGER("threading"), e.what());
        stop(true);
        throw;
    }
}

void Threadpool::join_threads() {
    for (std::size_t i = 0, size = m_threads.size(); i < size; ++i) {
        m_threads[i].join();
    }
}

void Threadpool::stop_threads() {
    for (auto i = 0u; i < m_thread_count; ++i) {
        m_tasks.push_front(Task());
    }
}

void Threadpool::run_loop() {
    while (1) {
        Task task;
        m_tasks.wait_and_pop(task);
        if (task.is_stop()) {
            return;
        }
        task();
    }
}
