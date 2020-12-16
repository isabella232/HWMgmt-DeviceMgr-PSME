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
 * @file default_configuration.hpp
 *
 * @brief Application default configuration
 * */

#pragma once
namespace agent {
namespace generic {

/*! Default configuration for the agent */
/*! Logger agent - main logger in AGENT_FRAMEWORK Agent */
static constexpr const char DEFAULT_CONFIGURATION[] = R"({
"agent":{"type":"Network","capabilities":[{"name":"Network","version":""}]},
"commands":{"*":{"implementation":"fm6000"}},
"server":{"port":7777, "ipv4":"localhost"},
"registration":{"ipv4":"localhost","port":8383,"interval":3},
"logger":{"agent":{}},
"modules":[]
})";

/*!
 * @brief Configuration validators
 * Supported validators:
 * min - minimal accepted value
 * max - maximum accepted value
 * type - check value type eg. "int", "string"
 * address - check value as IPv4 or IPv6 address
 * url - check value as URL
 * anyof - check if value is one of array value eg. "anyof" : [ 1, 2, 3 ]
 * anyof validator required "type" property
 *
 * json validator property should contain property:
 * "validator": true
 *
 * Eg. Module port validator
 * "module": {
 *      "port": {
 *          "validator": true,
 *          "min": 0,
 *          "max": 65535
 *      }
 * }
 */

static constexpr char DEFAULT_VALIDATOR_JSON[] = R"({
"logger" : {
    "agent" : {
        "level" : {
            "validator" : true,
            "type" : "string",
            "anyof": ["EMERGENCY",
                      "ALERT",
                      "CRITICAL",
                      "ERROR",
                      "WARNING",
                      "NOTICE",
                      "INFO",
                      "DEBUG"]
        }
    }
},
"registration" : {
    "interval" : {
        "validator": true,
        "max": 65535,
        "min": 0
    }
},
"port" : {
    "validator": true,
    "max": 65535,
    "min": 0
},
"url": {
    "validator": true,
    "type": "string",
    "url": ""
},
"address": {
    "validator": true,
    "address": ""
}
})";

/* Default environment variable for configuration file */
static constexpr const char DEFAULT_ENV_FILE[] =
    "AGENT_NETWORK_CONFIGURATION_FILE";

/* Default configuration file to load when env isn't present or valid */
static constexpr const char DEFAULT_FILE[] = "configuration.json";

}
}

