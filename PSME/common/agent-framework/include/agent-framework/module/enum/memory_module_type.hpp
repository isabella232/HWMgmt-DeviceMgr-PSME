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
 *    http_//www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
 * @file memory_module_type.hpp
 * */

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <ostream>

namespace agent_framework {
namespace model {
namespace enums {

/*! @brief Wrapper for Memory module type enum. */
class MemoryModuleType {
public:

    using underlying_type = std::uint32_t;

    /*!
     * @brief Memory module type enum constructor
     * */
    enum MemoryModuleTypeEnum : std::uint32_t {
        RDIMM,
        UDIMM,
        SO_DIMM,
        LRDIMM,
        Mini_RDIMM,
        Mini_UDIMM,
        SO_RDIMM_72b,
        SO_UDIMM_72b,
        SO_DIMM_16b,
        SO_DIMM_32b,
        Unknown
    };

    /*!
     * @brief Converts Enum to const char* (cstring).
     * @return name of the enum.
     * */
    const char* to_string() const;

    /*!
     * @brief Converts passed string to corresponding Enum.
     * If name in the string won't be recognized will return UNKNOWN.
     * @param string text to be converted to Enum.
     * @return corresponding MemoryModuleTypeEnum or UNKNOWN if not found.
     * */
    static MemoryModuleType from_string(const std::string& string);

    /*!
     * @brief Gets all available Enum values as a vector of strings.
     * @return Returns std::vector of std::string containing Enum values.
     * */
    static std::vector<std::string> get_values();

    /*!
     * @brief Default constructor
     * */
    MemoryModuleType() = delete;

    /*!
     * @brief Builds enum wrapper with enum passed as a argument.
     * @param value uint32_t value of enum.
     * */
    constexpr MemoryModuleType(MemoryModuleTypeEnum value) : m_value(value) {}

    /*!
     * @brief Operation of getting enums
     *
     * @return Enum
     * */
    constexpr operator MemoryModuleTypeEnum() const {
        return static_cast<MemoryModuleTypeEnum>(m_value);
    }

    /*!
     * @brief Is string an allowable value of this enum
     * @param string Value to be checked
     * @return true if the string is a valid value of enum, false otherwise
     * */
    static bool is_allowable_value(const std::string& string);

    friend std::ostream& operator<<(std::ostream& out_stream, const MemoryModuleType& in_enum) {
        out_stream << in_enum.to_string();
        return out_stream;
    }
private:
    std::uint32_t m_value{};
};
}
}
}
