/*!
 * @header{License}
 * @copyright Copyright (c) 2016-2017 Intel Corporation.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * @header{Filesystem}
 * @file number_validity_checker.hpp
 * */
#pragma once

#include "agent-framework/validators/procedure_validator.hpp"

namespace jsonrpc {

/*! @brief Number checker */
class NumberValidityChecker final : public ValidityChecker {
public:
    NumberValidityChecker();
    NumberValidityChecker(va_list args);

    virtual void validate(const Json::Value& value) const;

private:
    static bool is_integral(double d);

    ProcedureValidator::NumberType type{};

    /* don't play with pointers.. just dummy flags and union with value */
    bool has_min{false};
    bool has_max{false};

    union ValueUnion {
        int32_t i32;
        uint32_t ui32;
        int64_t i64;
        uint64_t ui64;
        double d;
    };

    ValueUnion min{};
    ValueUnion max{};
};

}
