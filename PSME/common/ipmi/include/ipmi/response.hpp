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
 * @file response.hpp
 *
 * @brief Response object from IPMI.
 * */

#pragma once

#include "message.hpp"

#include <cstdint>
#include <stdexcept>
#include <vector>
#include <string>

namespace ipmi {

/*!
 * @brief Response object from IPMI.
 * */
class Response : public Message {
public:

    /*!
     * @brief Represents response completion code,
     */
    enum COMPLETION_CODE : std::uint8_t {
        COMPLETION_CODE_NORMAL = 0x00,
        COMPLETION_CODE_REQUEST_DATA_LENGTH_INVALID = 0xc7,
        COMPLETION_CODE_REQUEST_DATA_FIELD_LENGTH_LIMIT_EXCEEDED = 0xc8,
        COMPLETION_CODE_PARAMETER_OUT_OF_RANGE = 0xc9,
        COMPLETION_CODE_INVALID_DATA_FIELD = 0xcc,
        COMPLETION_CODE_NODE_NOT_PRESENT = 0xd3,
        COMPLETION_CODE_NOT_SUPPORTED_IN_PRESENT_STATE = 0xd5,
        COMPLETION_CODE_UPDATE_IN_PROGRESS = 0x81,
        COMPLETION_CODE_UNSPECIFIED_ERROR = 0xff
    };

    /*!
     * @brief Constructor.
     * @param fn Network function.
     * @param cmd Command.
     * @param response_size Response size.
     */
    Response(NetFn fn, Cmd cmd, std::size_t response_size);

    /*! Copy constructor. */
    Response(const Response&) = default;

    /*! Assignment operator */
    Response& operator=(const Response&) = default;

    /*!
     * @brief destructor.
     */
    virtual ~Response();

    /*!
     * Returns Completion Code of IPMI command. Command sent status.
     *
     * @return number represents IPMI Completion Code.
     */
    virtual COMPLETION_CODE get_completion_code() const;

    /*!
     * Unpacks raw data bytes from vector into Response object.
     *
     * @param data which will be unpacked into object.
     */
    virtual void unpack(const std::vector<std::uint8_t>& data) = 0;

protected:

    /*!
     * @brief offset in data vector where completion code is stored.
     */
    static constexpr std::size_t OFFSET_COMPLETION_CODE = 0;

    /*!
     * @brief code completion received from IPMI MC.
     */
    COMPLETION_CODE m_completion_code{};

    /*!
     * @brief Checks if response data size is equal to expected data size.
     *
     * If response size is 1 byte long return false, if response is correct return true.
     * Otherwise throws runtime_error.
     *
     * @return true if response is correct, if response is error (1byte long) return false. Otherwise throws exception
     */
    virtual bool is_response_correct(const std::vector<std::uint8_t>& data);

    /*!
     * @brief Response length for IPMI error response. Only completion code is received.
     */
    static constexpr std::size_t ERROR_DATA_SIZE = 1;
private:
    const std::size_t DATA_SIZE;

};

}


