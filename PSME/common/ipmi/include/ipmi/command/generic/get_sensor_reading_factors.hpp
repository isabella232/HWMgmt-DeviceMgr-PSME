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
 * @file get_sensor_reading_factors.hpp
 * */

#pragma once

#include "ipmi/request.hpp"
#include "ipmi/response.hpp"

#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <iostream>

namespace ipmi {
namespace command {
namespace generic {

namespace request {

/*!
 * @brief Request message for Get Sensor Reading Factors command.
 */
class GetSensorReadingFactors : public Request {
public:

    /*!
     * @brief Default constructor.
     */
    GetSensorReadingFactors();

    /*! Copy constructor. */
    GetSensorReadingFactors(const GetSensorReadingFactors&) = default;

    /*! Assignment operator */
    GetSensorReadingFactors& operator=(const GetSensorReadingFactors&) = default;

    /*!
     * @brief Default destructor.
     */
    virtual ~GetSensorReadingFactors();

    /*!
     * @brief Sets sensor number.
     * @param sensor_number Sensor number.
     */
    void set_sensor_number(std::uint8_t sensor_number) {
        m_sensor_number = sensor_number;
    }

    /*!
     * @brief Sets reading byte
     * @param reading_byte Sensor reading byte
     */
    void set_reading_byte(std::uint8_t reading_byte) {
        m_reading_byte = reading_byte;
    }

    virtual void pack(std::vector<std::uint8_t>& data) const;

private:
    std::uint8_t m_sensor_number{};
    std::uint8_t m_reading_byte{};
};

}

namespace response {

/*!
 * @brief Response message for Get Sensor Reading Factors command.
 */
class GetSensorReadingFactors : public Response {
public:

    /*!
     * @brief Default constructor.
     */
    GetSensorReadingFactors();

    /*! Copy constructor. */
    GetSensorReadingFactors(const GetSensorReadingFactors&) = default;

    /*! Assignment operator */
    GetSensorReadingFactors& operator=(const GetSensorReadingFactors&) = default;

    /*!
     * @brief Default destructor.
     */
    virtual ~GetSensorReadingFactors();

    /*!
     * @brief Gets reading multiplier.
     * @return Sensor reading multiplier.
     */
    std::uint16_t get_multiplier() const {
        return m_multiplier;
    }

    virtual void unpack(const std::vector<std::uint8_t>& data);

private:

    std::uint16_t extract_multiplier(const std::vector<std::uint8_t>& data) const;

    static constexpr std::size_t RESPONSE_SIZE = 8;

    static constexpr std::size_t OFFSET_MULTIPLIER_LSB = 2;
    static constexpr std::size_t OFFSET_MULTIPLIER_MSB = 3;

    std::uint16_t m_multiplier{};
};
}

}
}
}

