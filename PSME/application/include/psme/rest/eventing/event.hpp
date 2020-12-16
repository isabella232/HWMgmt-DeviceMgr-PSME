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
 * @file event.hpp
 * @brief Declaration of Event class for PSME eventing
 * */

#pragma once
#include "agent-framework/module/enum/enum_builder.hpp"
#include <chrono>
#include <memory>
#include <string>
#include <vector>

namespace json {
    class Value;
}

namespace psme {
namespace rest {
namespace eventing {

/*!
 * @brief EventType Type of event
 */
ENUM(EventType, uint32_t, StatusChange, ResourceUpdated, ResourceAdded,
        ResourceRemoved, Alert);

/*!
 * @brief Event class
 * */
class Event {
public:
    /*! @brief Event type */
    static constexpr const char EVENT_TYPE[] = "EventType";
    /*! @brief Event id */
    static constexpr const char EVENT_ID[] = "EventId";
    /*! @brief Event timestamp */
    static constexpr const char EVENT_TIMESTAMP[] = "EventTimestamp";
    /*! @brief Severity */
    static constexpr const char SEVERITY[] = "Severity";
    /*! @brief Message */
    static constexpr const char MESSAGE[] = "Message";
    /*! @brief Message id */
    static constexpr const char MESSAGE_ID[] = "MessageId";
    /*! @brief Message args */
    static constexpr const char MESSAGE_ARGS[] = "MessageArgs";
    /*! @brief Context */
    static constexpr const char CONTEXT[] = "Context";
    /*! @brief Origin of condition */
    static constexpr const char ORIGIN_OF_CONDITION[] = "OriginOfCondition";
/*Nick added for ResourceAdded/ResourceRemoved/Alert/ResourceUpdated/StatusChange/ subscription*/
    static constexpr const char OEM[] = "Oem";

    /*!
     * Constructor
     *
     * @param type Event type
     * @param origin_of_condition Origin of condition
     */
    Event(EventType type, const std::string& origin_of_condition);

    /*!
     * @brief Return event type
     *
     * @return Event type
     */
    EventType get_type() const {
        return m_type;
    }

    /*!
     * @brief Set event type
     *
     * @param type Event type
     */
    void set_type(EventType type) {
        m_type = type;
    }

    /*!
     * @brief Get event id
     *
     * @return Event id
     */
    const std::string& get_event_id() const {
        return m_event_id;
    }

    /*!
     * @brief Set event id
     *
     * @param event_id Event id
     */
    void set_event_id(const std::string& event_id) {
        m_event_id = event_id;
    }

    /*!
     * @brief Get timestamp
     *
     * @return Timestamp
     */
    const std::chrono::steady_clock::time_point& get_timestamp() const {
        return m_timestamp;
    }

    /*!
     * @brief Get severity
     *
     * @return Severity
     */
    const std::string& get_severity() const {
        return m_severity;
    }

    /*!
     * @brief Set severity
     *
     * @param severity Severity
     */
    void set_severity(const std::string& severity) {
        m_severity = severity;
    }

    /*!
     * @brief Get message
     *
     * @return Message
     */
    const std::string& get_message() const {
        return m_message;
    }

    /*!
     * @brief Set message
     *
     * @param message Message
     */
    void set_message(const std::string& message) {
        m_message = message;
    }

    /*!
     * @brief Get message id
     *
     * @return Message id
     */
    const std::string& get_message_id() const {
        return m_message_id;
    }

    /*!
     * @brief Set message id
     *
     * @param message_id Message id
     */
    void set_message_id(const std::string& message_id) {
        m_message_id = message_id;
    }

    /*!
     * @brief Get context
     *
     * @return Context
     */
    const std::string& get_context() const {
        return m_context;
    }

    /*!
     * @brief Set context
     *
     * @param context Context
     */
    void set_context(const std::string& context) {
        m_context = context;
    }

    /*!
     * @brief Get origin of condition
     *
     * @return Origin of condition
     */
    const std::string& get_origin_of_condition() const {
        return m_origin_of_condition;
    }

    /*!
     * @brief Get OEM 
     *
     * @return Json OEM format 
     */
    const Json::Value & get_oem() const {
        return m_oem;
    }

    /*!
     * @brief Set origin of condition
     *
     * @param origin_of_condition Origin of condition
     */
    void set_origin_of_condition(const std::string& origin_of_condition) {
        m_origin_of_condition = origin_of_condition;
    }

    /*!
     * @brief Set OEM 
     *
     * @param Json format value 
     */
    void set_oem(const Json::Value & oem) {
        m_oem= oem;
    }

    /*!
     * @brief Get message args
     *
     * @return Message args
     */
    const std::vector<std::string>& get_message_args() const {
        return m_message_args;
    }

    /*!
     * @brief Add message arg
     *
     * @param message_arg Message arg
     */
    void add_message_arg(const std::string& message_arg) {
        m_message_args.emplace_back(message_arg);
    }

    /*!
     * @brief Set subscriber id
     *
     * @param id Subscriber id
     */
    void set_subscriber_id(const std::string& id) {
        m_subscriber_id = id;
    }

    /*!
     * @brief Get subscriber id
     *
     * @return Subscriber id
     */
    const std::string& get_subscriber_id() const {
        return m_subscriber_id;
    }

    /*!
     * @brief Increment retry attempt
     *
     * @return Number of retry attempts after increment
     */
    unsigned int increment_retry_attempts() {
        return ++m_retry_attempts;
    }

    /*!
     * @brief Get retry attempt count
     *
     * @return Retry attempt count
     */
    unsigned int get_retry_attempts() const {
        return m_retry_attempts;
    }

    /*!
     * @brief Convert Event to JSON representation
     *
     * @return JSON representation
     */
    json::Value to_json() const;

private:
    EventType m_type;
    std::string m_event_id{};
    std::chrono::steady_clock::time_point m_timestamp{std::chrono::steady_clock::now()};
    // hardcoded for now, cannot be really fixed without much effort
    // @TODO: reimplement message handling
    std::string m_severity{"OK"};
    std::string m_message{"Successfully Completed Request"};
    std::string m_message_id{"Base.1.0.0.Success"};
    std::vector<std::string> m_message_args{};
    std::string m_context{};
    std::string m_origin_of_condition{};
    Json::Value m_oem{};
    std::string m_subscriber_id{};
    unsigned int m_retry_attempts{0};
};

/*! Event Vector type */
using EventVec = std::vector<Event>;
/*! Event Unique Pointer type */
using EventUPtr = std::unique_ptr<Event>;

}
}
}


