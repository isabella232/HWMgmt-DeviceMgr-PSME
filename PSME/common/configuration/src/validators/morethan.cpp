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
 *
 * @file morethan.cpp
 *
 * @brief MoreThanValidator implementation
 * */

#include "configuration/validators/morethan.hpp"
#include "json/json.hpp"

using namespace configuration;

bool MoreThanValidator::is_valid(const json::Value& value) const {
    if (!value.is_number()) {
        return false;
    }
    const auto val = value.as_double();
    return val > m_min_value;
}

std::string MoreThanValidator::get_error() const {
    return "Field value is not greater than " + std::to_string(m_min_value) + ".";
}
