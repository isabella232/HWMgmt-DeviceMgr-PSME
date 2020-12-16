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
 * */

#include "psme/rest/validators/schemas/common.hpp"
#include "psme/rest/constants/constants.hpp"



using namespace psme::rest;
using namespace psme::rest::validators::schema;


const jsonrpc::ProcedureValidator& SimpleObjectSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "simple_object",
        jsonrpc::PARAMS_BY_NAME,
        constants::Common::ODATA_ID, VALID_JSON_STRING,
        nullptr
    };
    return procedure;
}


const jsonrpc::ProcedureValidator& EmptyObjectSchema::get_procedure() {
    static jsonrpc::ProcedureValidator procedure{
        "empty_object",
        jsonrpc::PARAMS_BY_NAME,
        nullptr
    };
    return procedure;
}
