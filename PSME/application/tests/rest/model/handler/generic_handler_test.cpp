
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
 * */

#include "mocks.hpp"
#include "resources.hpp"

#include "agent-framework/module/enum/common.hpp"
#include "agent-framework/module/compute_components.hpp"
#include "agent-framework/module/requests/common/get_collection.hpp"

#include "psme/rest/model/finder.hpp"
#include "psme/rest/model/handlers/generic_handler.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define PSME_REST_EVENTING_SUBSCRIPTION_MANAGER
#include "../src/rest/model/handlers/handler_manager.cpp"

namespace psme {
namespace rest {
namespace model {
namespace handler {

using SubscriptionManager = psme::rest::eventing::manager::SubscriptionManager;
using namespace psme::rest::eventing;
using namespace agent_framework::model;
using namespace agent_framework::module;
using agent_framework::eventing::Notification;

class DatabaseTester {
public:
    static void drop_all() {
        Database::SPtr db = Database::create("*drop_all");
        AlwaysMatchKey key{};
        db->drop(key);
        db->remove();
    }
};


// Required by gtest scheme for fixture tests
class HandlerTest : public ::testing::Test {
public:
    HandlerTest() { }

    ~HandlerTest();

    void SetUp() {
        DatabaseTester::drop_all();
    }

    void reset_id_policy() {
        static_cast<handler::ManagerHandler *>(handler::HandlerManager::get_instance()->get_handler(
                enums::Component::Manager))->m_id_policy.reset();
        static_cast<handler::SystemHandler *>(handler::HandlerManager::get_instance()->get_handler(
                enums::Component::System))->m_id_policy.reset();
        static_cast<handler::ProcessorHandler *>(handler::HandlerManager::get_instance()->get_handler(
                enums::Component::Processor))->m_id_policy.reset();
    }

    void TearDown() {
        auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
        agent->clear();
        agent_framework::module::get_manager<agent_framework::model::Manager>().clear_entries();
        agent_framework::module::get_manager<agent_framework::model::System>().clear_entries();
        agent_framework::module::get_manager<agent_framework::model::Processor>().clear_entries();

        reset_id_policy();

        SubscriptionManager::get_instance()->clear();
    }

    void BuildTreeWithAddEvent(bool full = true) {
        auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");

        if (full) {
            agent->m_responses = {
                    Manager1,
                    // System collection
                    R"([
                                    {"subcomponent": "system_1_uuid"},
                                    {"subcomponent": "system_2_uuid"}
                                ])",
                    System1,
                    System2,
                    // Processor collection
                    R"([
                                    {"subcomponent": "processor_1_uuid"},
                                    {"subcomponent": "processor_2_uuid"}]
                                )",
                    Processor1,
                    Processor2,

                    Manager2,
                    // Systems collection
                    R"([{
                                    "subcomponent": "system_3_uuid"
                                }])",
                    System3,
                    // Processor collection
                    R"([{"subcomponent": "processor_1_system_3_uuid"}])",
                    Processor1OnSystem3
            };
        }
        else {
            agent->m_responses = {
                    Manager1,
                    // System collection
                    R"([
                                    {"subcomponent": "system_1_uuid"},
                                    {"subcomponent": "system_2_uuid"}
                                ])",
                    System1,
                    System2,
                    // Processor collection
                    R"([
                                    {"subcomponent": "processor_1_uuid"},
                                    {"subcomponent": "processor_2_uuid"}]
                                )",
                    Processor1,
                    Processor2,

                    Manager2,
                    // Systems collection
                    R"([])",
            };
        }

        EventData event;

        event.set_type("Manager");
        event.set_notification(Notification::Add);
        event.set_parent("");
        event.set_component("manager_1_uuid");
        auto handler = handler::HandlerManager::get_instance()->get_handler(event.get_type());
        handler->handle(agent, event);

        event.set_type("Manager");
        event.set_notification(Notification::Add);
        event.set_parent("");
        event.set_component("manager_2_uuid");
        handler->handle(agent, event);
    }

    void BuildTreeWithDifferentCollectionNames() {
        auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");

        agent->m_responses = {
                Manager3,
                // FastSystem collection
                R"([
                            {"subcomponent": "system_5_uuid"}
                        ])",
                System5,
                R"([
                            {"subcomponent": "processor_1_uuid"}
                        ])",
                Processor1,
                R"([
                            {"subcomponent": "processor_2_uuid"}
                        ])",
                Processor2,
                // SlowSystem collection
                R"([
                            {"subcomponent": "system_6_uuid"}
                        ])",
                System6,
                R"([
                            {"subcomponent": "processor_3_uuid"}
                        ])",
                Processor2,
                R"([
                            {"subcomponent": "processor_4_uuid"}
                        ])",
                Processor1,
        };

        EventData event;

        event.set_type("Manager");
        event.set_notification(Notification::Add);
        event.set_parent("");
        event.set_component("manager_3_uuid");
        auto handler = handler::HandlerManager::get_instance()->get_handler(event.get_type());
        handler->handle(agent, event);
    }
};

HandlerTest::~HandlerTest() { }

using namespace testing;
using namespace psme::rest::eventing;

#define CHECK_REQUESTS \
    for (size_t i = 0; i < std::min(expectedReq.size(), agent->m_requests.size()); i++) { \
        ASSERT_EQ(expectedReq[i], agent->m_requests[i]) << "Request[" << i << "] is incorrect"; \
    } \
    ASSERT_EQ(expectedReq.size(), agent->m_requests.size()) << "Invalid number of request";

#define CHECK_EVENTS \
    {\
        auto _events = SubscriptionManager::get_instance()->m_events; \
        for (size_t i = 0; i < std::min(expectedEvents.size(), _events.size()); i++) { \
            ASSERT_EQ(expectedEvents[i].first, _events[i].get_type()) << "Event[" << i << "] type is incorrect"; \
            ASSERT_EQ(expectedEvents[i].second, _events[i].get_origin_of_condition()) << "Event[" << i << "] 'origin of condition' is incorrect"; \
        } \
        ASSERT_EQ(expectedEvents.size(), _events.size()) << "Invalid number of events"; \
    }

#define CHECK_COLLECTIONS \
    {\
        for (size_t i = 0; i < std::min(expectedCollections.size(), agent->m_collections.size()); i++) { \
            ASSERT_EQ(expectedCollections[i], agent->m_collections[i]) << "Collection request[" << i << "] was not expected"; \
        } \
        ASSERT_EQ(expectedCollections.size(), agent->m_collections.size()) << "Invalid number of collections"; \
    }

TEST_F(HandlerTest, AddEverything) {
    BuildTreeWithAddEvent();

    auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto expectedReq = std::vector<std::string> {"manager_1_uuid",
                                              "manager_1_uuid",
                                              "system_1_uuid",
                                              "system_2_uuid",
                                              "system_2_uuid",
                                              "processor_1_uuid", "processor_2_uuid",
                                              "manager_2_uuid",
                                              "manager_2_uuid",
                                              "system_3_uuid",
                                              "system_3_uuid",
                                              "processor_1_system_3_uuid"};

    CHECK_REQUESTS;

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {{EventType::ResourceAdded, "/redfish/v1/Managers/1"}, {EventType::ResourceAdded, "/redfish/v1/Managers/2"}};
    CHECK_EVENTS;


    ASSERT_EQ("system_1_uuid", Find<agent_framework::model::System>("1").get_uuid());
    ASSERT_EQ("system_2_uuid", Find<agent_framework::model::System>("2").get_uuid());
    ASSERT_EQ("system_3_uuid", Find<agent_framework::model::System>("3").get_uuid());
    ASSERT_EQ("processor_1_uuid", Find<agent_framework::model::Processor>("1").via<agent_framework::model::System>("2").get_uuid());
    ASSERT_EQ("processor_2_uuid", Find<agent_framework::model::Processor>("2").via<agent_framework::model::System>("2").get_uuid());
    ASSERT_EQ("processor_1_system_3_uuid", Find<agent_framework::model::Processor>("1").via<agent_framework::model::System>("3").get_uuid());

    ASSERT_EQ(agent_framework::model::enums::Component::Manager, Find<agent_framework::model::System>("1").get_one()->get_parent_type());
    ASSERT_EQ(agent_framework::model::enums::Component::Manager, Find<agent_framework::model::System>("2").get_one()->get_parent_type());
    ASSERT_EQ(agent_framework::model::enums::Component::Manager, Find<agent_framework::model::System>("3").get_one()->get_parent_type());
    ASSERT_EQ(agent_framework::model::enums::Component::System, Find<agent_framework::model::Processor>("1").via<agent_framework::model::System>("2").get_one()->get_parent_type());
    ASSERT_EQ(agent_framework::model::enums::Component::System, Find<agent_framework::model::Processor>("2").via<agent_framework::model::System>("2").get_one()->get_parent_type());
    ASSERT_EQ(agent_framework::model::enums::Component::System, Find<agent_framework::model::Processor>("1").via<agent_framework::model::System>("3").get_one()->get_parent_type());
}

// This test should prove that we can handle ResourceAdded event
// and set parent component type correctly, despite the fact that
// it is not part of the event
TEST_F(HandlerTest, AddAssetInTheMiddleOfTreeWhenParentIsKnown) {
    BuildTreeWithAddEvent();

    //first remove system2/processor2
    auto handler = handler::HandlerManager::get_instance()->get_handler(agent_framework::model::enums::Component::Processor);
    handler->remove("processor_2_uuid");
    SubscriptionManager::get_instance()->clear();

    // now add again
    SubscriptionManager::get_instance()->clear();
    EventData event;
    event.set_type("Processor");
    event.set_notification(Notification::Add);
    event.set_parent("system_2_uuid");
    event.set_component("processor_2_uuid");

    auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    agent->clear();
    agent->m_responses = {
        Processor2
    };
    handler->handle(agent, event);

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {{EventType::ResourceAdded, "/redfish/v1/Systems/2/Processors/2"}};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, RemoveLeaf) {
    BuildTreeWithAddEvent();

    auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");

    ASSERT_EQ("processor_1_uuid", Find<agent_framework::model::Processor>("1").via<agent_framework::model::System>("2").get_uuid());
    ASSERT_EQ("processor_2_uuid", Find<agent_framework::model::Processor>("2").via<agent_framework::model::System>("2").get_uuid());

    SubscriptionManager::get_instance()->clear();
    EventData event;
    event.set_type("Processor");
    event.set_notification(Notification::Remove);
    event.set_parent("system_2_uuid");
    event.set_component("processor_1_uuid");

    auto handler = handler::HandlerManager::get_instance()->get_handler(event.get_type());
    handler->handle(agent, event);

    ASSERT_THROW(Find<agent_framework::model::Processor>("1").via<agent_framework::model::System>("2").get_uuid(), agent_framework::exceptions::NotFound);
    ASSERT_EQ("processor_2_uuid", Find<agent_framework::model::Processor>("2").via<agent_framework::model::System>("2").get_uuid());

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {{EventType::ResourceRemoved, "/redfish/v1/Systems/2/Processors/1"}};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, RemoveInnerNode) {
    BuildTreeWithAddEvent();


    ASSERT_EQ(2, get_manager<Manager>().get_entry_count());
    ASSERT_EQ(3, get_manager<System>().get_entry_count());
    ASSERT_EQ(3, get_manager<Processor>().get_entry_count());
    ASSERT_EQ("system_1_uuid", Find<agent_framework::model::System>("1").get_uuid());
    ASSERT_EQ("system_2_uuid", Find<agent_framework::model::System>("2").get_uuid());

    SubscriptionManager::get_instance()->clear();

    EventData event;
    event.set_type("System");
    event.set_notification(Notification::Remove);
    event.set_parent("manager_1_uuid");
    event.set_component("system_2_uuid");
    auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()->get_handler(event.get_type());
    handler->handle(agent, event);


    ASSERT_EQ("system_1_uuid", Find<agent_framework::model::System>("1").get_uuid());
    ASSERT_THROW(Find<agent_framework::model::System>("2").get_uuid(), agent_framework::exceptions::NotFound);
    ASSERT_THROW(Find<agent_framework::model::Processor>("1").via<agent_framework::model::System>("2").get_uuid(), agent_framework::exceptions::NotFound);
    ASSERT_THROW(Find<agent_framework::model::Processor>("2").via<agent_framework::model::System>("2").get_uuid(), agent_framework::exceptions::NotFound);

    ASSERT_EQ(2, get_manager<Manager>().get_entry_count());
    ASSERT_EQ(3-1, get_manager<System>().get_entry_count());
    ASSERT_EQ(3-2, get_manager<Processor>().get_entry_count());

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {{EventType::ResourceRemoved, "/redfish/v1/Systems/2"}};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, TestProperIdsAfterAgentReconnection) {
    BuildTreeWithAddEvent();

    const auto agent_gami_id = psme::core::agent::AgentManager::get_instance().get_agent("anything")->get_gami_id();

    // check if all resources have been added to the model
    ASSERT_EQ(2, get_manager<Manager>().get_entry_count());
    ASSERT_EQ(3, get_manager<System>().get_entry_count());
    ASSERT_EQ(3, get_manager<Processor>().get_entry_count());
    ASSERT_EQ("system_1_uuid", Find<agent_framework::model::System>("1").get_uuid());
    ASSERT_EQ("system_2_uuid", Find<agent_framework::model::System>("2").get_uuid());
    ASSERT_EQ("system_3_uuid", Find<agent_framework::model::System>("3").get_uuid());
    ASSERT_EQ("processor_1_uuid", Find<agent_framework::model::Processor>("1").via<agent_framework::model::System>("2").get_uuid());
    ASSERT_EQ("processor_2_uuid", Find<agent_framework::model::Processor>("2").via<agent_framework::model::System>("2").get_uuid());
    ASSERT_EQ("processor_1_system_3_uuid", Find<agent_framework::model::Processor>("1").via<agent_framework::model::System>("3").get_uuid());

    // remove agent to simulate connection error
    handler::HandlerManager::get_instance()->remove_agent_data(agent_gami_id);
    psme::core::agent::AgentManager::get_instance().remove_agent(agent_gami_id);

    // check if all resources have been removed from the model
    ASSERT_EQ(0, get_manager<Manager>().get_entry_count());
    ASSERT_EQ(0, get_manager<System>().get_entry_count());
    ASSERT_EQ(0, get_manager<Processor>().get_entry_count());
    ASSERT_THROW(Find<agent_framework::model::System>("1").get_uuid(), agent_framework::exceptions::NotFound);
    ASSERT_THROW(Find<agent_framework::model::System>("2").get_uuid(), agent_framework::exceptions::NotFound);
    ASSERT_THROW(Find<agent_framework::model::System>("3").get_uuid(), agent_framework::exceptions::NotFound);
    ASSERT_THROW(Find<agent_framework::model::Processor>("1").via<agent_framework::model::System>("2").get_uuid(), agent_framework::exceptions::NotFound);
    ASSERT_THROW(Find<agent_framework::model::Processor>("2").via<agent_framework::model::System>("2").get_uuid(), agent_framework::exceptions::NotFound);

    // reconnect agent
    BuildTreeWithAddEvent();

    // check if all resources have been added to the model with updated ids
    ASSERT_EQ(2, get_manager<Manager>().get_entry_count());
    ASSERT_EQ(3, get_manager<System>().get_entry_count());
    ASSERT_EQ(3, get_manager<Processor>().get_entry_count());
    ASSERT_EQ("system_1_uuid", Find<agent_framework::model::System>("1").get_uuid());
    ASSERT_EQ("system_2_uuid", Find<agent_framework::model::System>("2").get_uuid());
    ASSERT_EQ("system_3_uuid", Find<agent_framework::model::System>("3").get_uuid());
    ASSERT_EQ("processor_1_uuid", Find<agent_framework::model::Processor>("1").via<agent_framework::model::System>("2").get_uuid());
    ASSERT_EQ("processor_2_uuid", Find<agent_framework::model::Processor>("2").via<agent_framework::model::System>("2").get_uuid());
}

TEST_F(HandlerTest, LoadLeaf) {
    BuildTreeWithAddEvent();
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()->get_handler(agent_framework::model::enums::Component::Processor);
    agent-> m_responses = {
        Processor1OnSystem3Modified
    };
    auto rest_id = handler->load(agent, "system_3_uuid", agent_framework::model::enums::Component::System, "processor_1_system_3_uuid");
    EXPECT_EQ(1, rest_id); // should retain rest id

    auto expectedReq = std::vector<std::string> {"processor_1_system_3_uuid"};
    CHECK_REQUESTS;

    auto proc = Find<agent_framework::model::Processor>("1").
        via<agent_framework::model::Manager>("2").via<agent_framework::model::System>("3").get_one();
    EXPECT_EQ("processor_1_system_3_uuid", proc->get_uuid());
    EXPECT_EQ(enums::State::Enabled, proc->get_status().get_state());

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {
        {EventType::StatusChange, "/redfish/v1/Systems/3/Processors/1"},
        {EventType::ResourceUpdated, "/redfish/v1/Systems/3/Processors/1"}
    };

    CHECK_EVENTS;
}

TEST_F(HandlerTest, LoadInternalNonRecursively) {
    BuildTreeWithAddEvent();
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()
            ->get_handler(agent_framework::model::enums::Component::System);
    agent-> m_responses = {
        System2Modified
    };
    auto rest_id = handler->load(agent, "manager_1_uuid", agent_framework::model::enums::Component::Manager, "system_2_uuid");
    EXPECT_EQ(2, rest_id); // should retain rest id

    auto expectedReq = std::vector<std::string> {"system_2_uuid"};
    CHECK_REQUESTS;

    auto system = Find<agent_framework::model::System>("2").
        via<agent_framework::model::Manager>("1").get_one();
    EXPECT_EQ("system_2_uuid", system->get_uuid());
    EXPECT_EQ("B20F21_A0" /* new value from updated system */, system->get_bios_version());

    // no event expected after load
    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {{EventType::ResourceUpdated, "/redfish/v1/Systems/2"}};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, LoadInternalRecursively) {
    BuildTreeWithAddEvent();
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()
            ->get_handler(agent_framework::model::enums::Component::System);
    agent-> m_responses = {
        System2Modified,
        R"([
            {"subcomponent": "processor_1_uuid"},
            {"subcomponent": "processor_2_uuid"}]
        )",
        Processor1,
        Processor2Modified
    };
    auto rest_id = handler->load(agent, "manager_1_uuid", agent_framework::model::enums::Component::Manager, "system_2_uuid", true /* recursively */);
    EXPECT_EQ(2, rest_id); // should retain rest id

    auto expectedReq = std::vector<std::string> {"system_2_uuid", "system_2_uuid", "processor_1_uuid", "processor_2_uuid"};
    CHECK_REQUESTS;

    auto system = Find<agent_framework::model::System>("2").
        via<agent_framework::model::Manager>("1").get_one();
    EXPECT_EQ("system_2_uuid", system->get_uuid());
    EXPECT_EQ("B20F21_A0" /* new value from updated system */, system->get_bios_version());

    auto processor2 = Find<agent_framework::model::Processor>("2").
            via<agent_framework::model::Manager>("1").via<agent_framework::model::System>("2").get_one();
    EXPECT_EQ("processor_2_uuid", processor2->get_uuid());
    EXPECT_EQ("666" /* new value from updated processor */, processor2->get_socket());

    // no event expected after load
    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {
            {EventType::ResourceUpdated, "/redfish/v1/Systems/2"},
            {EventType::ResourceUpdated, "/redfish/v1/Systems/2/Processors/2"}};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, TestUpdateEventIgnoredForUnknownResource) {
    BuildTreeWithAddEvent();
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()
            ->get_handler(agent_framework::model::enums::Component::System);

    EventData event;

    event.set_type("System");
    event.set_notification(Notification::Update);
    event.set_parent("manager_1_uuid");
    event.set_component("unknown_system_uuid");

    agent-> m_responses = {
    };

    handler->handle(agent, event);
    auto expectedReq = std::vector<std::string> {};
    CHECK_REQUESTS;

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, TestUpdateEventNotForwardedWhenNoChange) {
    BuildTreeWithAddEvent();
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()
        ->get_handler(agent_framework::model::enums::Component::System);

    EventData event;

    event.set_type("System");
    event.set_notification(Notification::Update);
    event.set_parent("manager_1_uuid");
    event.set_component("system_2_uuid");

    agent-> m_responses = {
        System2 // identical to original one
    };

    handler->handle(agent, event);
    auto expectedReq = std::vector<std::string> {"system_2_uuid"};
    CHECK_REQUESTS;

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, TestSystemRefreshedAfterUpdateEvent) {
    BuildTreeWithAddEvent();
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()
            ->get_handler(agent_framework::model::enums::Component::System);

    EventData event;

    event.set_type("System");
    event.set_notification(Notification::Update);
    event.set_parent("manager_1_uuid");
    event.set_component("system_2_uuid");

    agent-> m_responses = {
            System2Modified
    };

    handler->handle(agent, event);
    auto expectedReq = std::vector<std::string> {"system_2_uuid"};
    CHECK_REQUESTS;

    auto system = Find<agent_framework::model::System>("2").
            via<agent_framework::model::Manager>("1").get_one();
    EXPECT_EQ("system_2_uuid", system->get_uuid());
    EXPECT_EQ("B20F21_A0" /* new value from updated system */, system->get_bios_version());

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {{EventType::ResourceUpdated, "/redfish/v1/Systems/2"}};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, TestProcessorRefreshedAfterUpdateEvent) {
    BuildTreeWithAddEvent();
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    auto agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()->get_handler(agent_framework::model::enums::Component::Processor);
    agent-> m_responses = {
        Processor1OnSystem3Modified
    };

    EventData event;

    event.set_type("Processor");
    event.set_notification(Notification::Update);
    event.set_parent("system_3_uuid");
    event.set_component("processor_1_system_3_uuid");

    handler->handle(agent, event);
    auto expectedReq = std::vector<std::string> {"processor_1_system_3_uuid"};
    CHECK_REQUESTS;

    auto proc = Find<agent_framework::model::Processor>("1").
        via<agent_framework::model::Manager>("2").via<agent_framework::model::System>("3").get_one();
    EXPECT_EQ("processor_1_system_3_uuid", proc->get_uuid());
    EXPECT_EQ(enums::State::Enabled, proc->get_status().get_state());

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {
        {EventType::StatusChange, "/redfish/v1/Systems/3/Processors/1"},
        {EventType::ResourceUpdated, "/redfish/v1/Systems/3/Processors/1"}
    };

    CHECK_EVENTS;
}



TEST_F(HandlerTest, PollingNoUpdate) {
    BuildTreeWithAddEvent();
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    psme::core::agent::JsonAgentSPtr agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()->get_handler(
        agent_framework::model::enums::Component::System);
    agent-> m_responses = {
            // Manager 1
            System2,
            // Processor Collection
            R"([
                    {"subcomponent": "processor_1_uuid"},
                    {"subcomponent": "processor_2_uuid"}
               ])",
            Processor1,
            Processor2
    };
    handler->poll(agent, "manager_1_uuid", agent_framework::model::enums::Component::Manager, "system_2_uuid");

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, PollingDetectsLeafRemoved) {
    BuildTreeWithAddEvent();
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    psme::core::agent::JsonAgentSPtr agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()->get_handler(
            agent_framework::model::enums::Component::System);
    agent-> m_responses = {
        System2,
        // Processor collection
        R"([
                {"subcomponent": "processor_1_uuid"}
                //processor 2 removed

          ])",
        Processor1
    };
    handler->poll(agent, "manager_1_uuid", agent_framework::model::enums::Component::Manager, "system_2_uuid");

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {{EventType::ResourceRemoved, "/redfish/v1/Systems/2/Processors/2"}};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, PollingDetectsInnerNodeRemoved) {
    BuildTreeWithAddEvent();

    psme::core::agent::AgentManager::get_instance().get_agent("anything")-> clear();
    SubscriptionManager::get_instance()->clear();

    psme::core::agent::JsonAgentSPtr agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()->get_handler(
            agent_framework::model::enums::Component::Manager);
    agent-> m_responses = {
            Manager1,
            // System collection
            R"([
                    {"subcomponent": "system_1_uuid"}
                ])",
            System1,
            //System2 removed
    };

    handler->poll(agent, "", agent_framework::model::enums::Component::None, "manager_1_uuid");

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {{EventType::ResourceRemoved, "/redfish/v1/Systems/2"}};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, PollingDetectsInnerAndLeafNodesRemoved) {
    BuildTreeWithAddEvent();

    psme::core::agent::AgentManager::get_instance().get_agent("anything")-> clear();
    SubscriptionManager::get_instance()->clear();

    psme::core::agent::JsonAgentSPtr agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()->get_handler(
            agent_framework::model::enums::Component::Manager);
    agent-> m_responses = {
            Manager1,
            // System collection
            R"([
               ])"
            //System1 removed
            //System2 removed
    };

    handler->poll(agent, "", agent_framework::model::enums::Component::None, "manager_1_uuid");

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {{EventType::ResourceRemoved, "/redfish/v1/Systems/1"}, {EventType::ResourceRemoved, "/redfish/v1/Systems/2"}};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, PollingDetectsUpdate) {
    BuildTreeWithAddEvent();
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    psme::core::agent::JsonAgentSPtr agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()->get_handler(
            agent_framework::model::enums::Component::System);
    agent-> m_responses = {
            // Manager 1
            System2Modified,
            // Processor Collection
            R"([
                        {"subcomponent": "processor_1_uuid"},
                        {"subcomponent": "processor_2_uuid"}
                   ])",
            Processor1,
            Processor2
    };
    handler->poll(agent, "manager_1_uuid", agent_framework::model::enums::Component::Manager, "system_2_uuid");

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {{EventType::ResourceUpdated, "/redfish/v1/Systems/2"}};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, PollingDetectsSubtreeAdded) {
    BuildTreeWithAddEvent(false /*only first manager*/);
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    psme::core::agent::JsonAgentSPtr agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()->get_handler(
        agent_framework::model::enums::Component::Manager);
    agent->m_responses = {
        Manager2,
        // Systems collection
        R"([{
                                    "subcomponent": "system_3_uuid"
                                },
            {
                                    "subcomponent": "system_4_uuid"
                                }
        ])",
        System3,
        // Processor collection
        R"([{"subcomponent": "processor_1_system_3_uuid"}])",
        Processor1OnSystem3,
        System4,
        // Processor collection
        R"([{"subcomponent": "processor_1_system_4_uuid"}])",
        Processor1OnSystem4
    };

    handler->poll(agent, "", agent_framework::model::enums::Component::None, "manager_2_uuid");

    // events about added cpus should not be sent
    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {
            {EventType::ResourceAdded, "/redfish/v1/Systems/3"},
            {EventType::ResourceAdded, "/redfish/v1/Systems/4"}
    };
    CHECK_EVENTS;
}


TEST_F(HandlerTest, PollingQuitsAfterRpcException) {
    BuildTreeWithAddEvent();
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    psme::core::agent::JsonAgentSPtr agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()->get_handler(
        agent_framework::model::enums::Component::Manager);
    agent-> m_responses = {
        Manager1,
        // System collection
        R"([
                                    {"subcomponent": "system_1_uuid"},
                                    {"subcomponent": "system_2_uuid"}
                                ])",
        System1,
        "[JsonRpcException]"
    };
    handler->poll(agent, "", agent_framework::model::enums::Component::None, "manager_1_uuid");

    auto expectedReq = std::vector<std::string> {"manager_1_uuid",
                                                 "manager_1_uuid",
                                                 "system_1_uuid",
                                                 "system_2_uuid"
    };

    CHECK_REQUESTS;

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, PollingKeepsStableIds) {
    BuildTreeWithAddEvent();
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    psme::core::agent::JsonAgentSPtr agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()->get_handler(
            agent_framework::model::enums::Component::System);

    agent-> m_responses = {
            System2,
            // Processors collection
            R"([
                {"subcomponent": "processor_2_uuid"}
            ])",
            Processor2
    };

    handler::HandlerManager::get_instance()->get_handler(
            agent_framework::model::enums::Component::Processor)->remove("processor_1_uuid");
    ASSERT_EQ(2, get_manager<Processor>().get_entry("processor_2_uuid").get_id());
    handler->poll(agent, "manager_1_uuid", agent_framework::model::enums::Component::Manager, "system_2_uuid");
    ASSERT_EQ(2, get_manager<Processor>().get_entry("processor_2_uuid").get_id());

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {{EventType::ResourceRemoved,
                                                                                  "/redfish/v1/Systems/2/Processors/1"}};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, NothingAddedIfJsonRpcExeptionDuringAddEventHandling) {
    BuildTreeWithAddEvent(false /*only first manager*/);
    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();

    psme::core::agent::JsonAgentSPtr agent = psme::core::agent::AgentManager::get_instance().get_agent("anything");
    auto handler = handler::HandlerManager::get_instance()->get_handler(
            agent_framework::model::enums::Component::System);
    agent->m_responses = {
            System3,
            // Processor collection
            R"([{"subcomponent": "processor_1_system_3_uuid"}])",
            "[JsonRpcException]"
    };

    EventData event;
    event.set_type("System");
    event.set_notification(Notification::Add);
    event.set_parent("manager_2_uuid");
    event.set_component("system_3_uuid");
    handler->handle(agent, event);


    auto expectedReq = std::vector<std::string> {"system_3_uuid",
                                                 "system_3_uuid",
                                                 "processor_1_system_3_uuid"
    };

    CHECK_REQUESTS;
    // events about added cpus should not be sent
    auto expectedEvents = std::vector<std::pair<EventType, std::string>> { };
    CHECK_EVENTS;

    ASSERT_EQ(2, get_manager<Manager>().get_entry_count());
    ASSERT_EQ(2, get_manager<System>().get_entry_count());
    ASSERT_EQ(2, get_manager<Processor>().get_entry_count());
    ASSERT_THROW(Find<agent_framework::model::System>("3").via<agent_framework::model::Manager>("2").get_uuid(), agent_framework::exceptions::NotFound);
}

TEST_F(HandlerTest, DifferentCollectionNamesForSameResourceType) {
    psme::core::agent::JsonAgentSPtr agent =
                    psme::core::agent::AgentManager::get_instance().get_agent("anything");
    agent->clear();
    SubscriptionManager::get_instance()->clear();
    BuildTreeWithDifferentCollectionNames();

    auto expectedReq = std::vector<std::string> {"manager_3_uuid",
                                                 "manager_3_uuid",
                                                 "system_5_uuid",
                                                 "system_5_uuid",
                                                 "processor_1_uuid",
                                                 "system_5_uuid",
                                                 "processor_2_uuid",
                                                 "manager_3_uuid",
                                                 "system_6_uuid",
                                                 "system_6_uuid",
                                                 "processor_3_uuid",
                                                 "system_6_uuid",
                                                 "processor_4_uuid",
    };

    CHECK_REQUESTS;

    auto expectedCollections = std::vector<std::pair<std::string, std::string>>
                                       {{"FastSystems", "manager_3_uuid"},
                                        {"FastProcessors", "system_5_uuid"},
                                        {"SlowProcessors", "system_5_uuid"},
                                        {"SlowSystems", "manager_3_uuid"},
                                        {"Anything", "system_6_uuid"},
                                        {"whAtever", "system_6_uuid"}
    };

    CHECK_COLLECTIONS;

    psme::core::agent::AgentManager::get_instance().get_agent("anything")->clear();
    SubscriptionManager::get_instance()->clear();


    ASSERT_EQ("system_5_uuid", Find<agent_framework::model::System>("1").via<agent_framework::model::Manager>("1").get_uuid());

    ASSERT_EQ("system_6_uuid", Find<agent_framework::model::System>("2").via<agent_framework::model::Manager>("1").get_uuid());

    ASSERT_EQ("processor_1_uuid", Find
        <agent_framework::model::Processor>("1").via
        <agent_framework::model::System>("1").get_uuid());

    ASSERT_EQ("processor_2_uuid", Find
        <agent_framework::model::Processor>("2").via
        <agent_framework::model::System>("1").get_uuid());

    ASSERT_EQ("processor_3_uuid", Find
        <agent_framework::model::Processor>("1").via
        <agent_framework::model::System>("2").get_uuid());

    ASSERT_EQ("processor_4_uuid", Find
        <agent_framework::model::Processor>("2").via
        <agent_framework::model::System>("2").get_uuid());

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> { };

    CHECK_EVENTS;
}

TEST_F(HandlerTest, LoadCollectionNonRecursive) {
    BuildTreeWithDifferentCollectionNames();
    psme::core::agent::JsonAgentSPtr agent =
            psme::core::agent::AgentManager::get_instance().get_agent("anything");
    agent->clear();
    SubscriptionManager::get_instance()->clear();

    agent->m_responses = {
        Manager3,
        // FastSystems collection
        R"([
            {"subcomponent": "system_5_uuid"}
        ])",
        System5,
        // SlowSystems collection
        R"([
            {"subcomponent": "system_6_uuid"}
        ])",
        System6,
    };
    auto handler = handler::HandlerManager::get_instance()
            ->get_handler(agent_framework::model::enums::Component::Manager);
    handler->load_collection(agent, "manager_3_uuid", agent_framework::model::enums::Component::Manager, enums::CollectionType::Systems, false);
    auto expectedReq = std::vector<std::string> {
        "manager_3_uuid",
        "manager_3_uuid",
        "system_5_uuid",
        "manager_3_uuid",
        "system_6_uuid"
    };

    CHECK_REQUESTS;

    auto expectedEvents = std::vector<std::pair<EventType, std::string>>{};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, LoadCollectionRecursive) {
    BuildTreeWithDifferentCollectionNames();
    psme::core::agent::JsonAgentSPtr agent =
            psme::core::agent::AgentManager::get_instance().get_agent("anything");
    agent->clear();
    SubscriptionManager::get_instance()->clear();
    agent->m_responses = {
        Manager3,
        // FastSystems collection
        R"([
            {"subcomponent": "system_5_uuid"}
        ])",
        System5,
        R"([
            {"subcomponent": "processor_1_uuid"}
        ])",
        Processor1,
        R"([
            {"subcomponent": "processor_2_uuid"}
        ])",
        Processor2,
        // SlowSystems collection
        R"([
            {"subcomponent": "system_6_uuid"}
        ])",
        System6,
        R"([
            {"subcomponent": "processor_3_uuid"}
        ])",
        Processor2,
        R"([
            {"subcomponent": "processor_4_uuid"}
        ])",
        Processor1,
    };

    auto handler = handler::HandlerManager::get_instance()
            ->get_handler(agent_framework::model::enums::Component::Manager);
    handler->load_collection(agent, "manager_3_uuid", agent_framework::model::enums::Component::Manager, enums::CollectionType::Systems, true);

    auto expectedReq = std::vector<std::string> {
        "manager_3_uuid",
        "manager_3_uuid",
        "system_5_uuid",
        "system_5_uuid",
        "processor_1_uuid",
        "system_5_uuid",
        "processor_2_uuid",
        "manager_3_uuid",
        "system_6_uuid",
        "system_6_uuid",
        "processor_3_uuid",
        "system_6_uuid",
        "processor_4_uuid",
    };

    CHECK_REQUESTS;

    auto expectedEvents = std::vector<std::pair<EventType, std::string>>{};
    CHECK_EVENTS;
}

TEST_F(HandlerTest, LoadCollectionNonRecursiveAddDelete) {
    BuildTreeWithDifferentCollectionNames();
    psme::core::agent::JsonAgentSPtr agent =
            psme::core::agent::AgentManager::get_instance().get_agent("anything");
    agent->clear();
    SubscriptionManager::get_instance()->clear();

    // Moving System6 to the other collection with different uuid
    agent->m_responses = {
        Manager3,
        // FastSystems collection
        R"([
            {"subcomponent": "system_5_uuid"},
            {"subcomponent": "system_NEW_uuid"}
        ])",
        System5,
        System6,
        // SlowSystems collection
        R"([])",
    };
    auto handler = handler::HandlerManager::get_instance()
            ->get_handler(agent_framework::model::enums::Component::Manager);
    handler->load_collection(agent, "manager_3_uuid", agent_framework::model::enums::Component::Manager, enums::CollectionType::Systems, false);

    auto expectedReq = std::vector<std::string> {
        "manager_3_uuid",
        "manager_3_uuid",
        "system_5_uuid",
        "system_NEW_uuid",
        "manager_3_uuid"
    };

    CHECK_REQUESTS;

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {
        {EventType::ResourceRemoved, "/redfish/v1/Systems/2"},
        {EventType::ResourceAdded, "/redfish/v1/Systems/3"}
    };

    CHECK_EVENTS;
}

TEST_F(HandlerTest, LoadCollectionRecursiveAddDelete) {
    BuildTreeWithDifferentCollectionNames();
    psme::core::agent::JsonAgentSPtr agent =
            psme::core::agent::AgentManager::get_instance().get_agent("anything");
    agent->clear();
    SubscriptionManager::get_instance()->clear();

    // Moving System6 to the other collection with different uuid
    // Removing Processor2 and adding same processor, but with different uuid
    agent->m_responses = {
        Manager3,
        // FastSystems collection
        R"([
            {"subcomponent": "system_5_uuid"},
            {"subcomponent": "system_NEW_uuid"}
        ])",
        System5,
        R"([
            {"subcomponent": "processor_1_uuid"}
        ])",
        Processor1,
        R"([
            {"subcomponent": "processor_NEW_uuid"}
        ])",
        Processor2,
        System6,
        R"([
            {"subcomponent": "processor_3_uuid"}
        ])",
        Processor2,
        R"([
            {"subcomponent": "processor_4_uuid"}
        ])",
        Processor1,
        // SlowSystems collection
        R"([])"
    };

    auto handler = handler::HandlerManager::get_instance()
            ->get_handler(agent_framework::model::enums::Component::Manager);
    handler->load_collection(agent, "manager_3_uuid", agent_framework::model::enums::Component::Manager, enums::CollectionType::Systems, true);

    auto expectedReq = std::vector<std::string> {
        "manager_3_uuid",
        "manager_3_uuid",
        "system_5_uuid",
        "system_5_uuid",
        "processor_1_uuid",
        "system_5_uuid",
        "processor_NEW_uuid",
        "system_NEW_uuid",
        "system_NEW_uuid",
        "processor_3_uuid",
        "system_NEW_uuid",
        "processor_4_uuid",
        "manager_3_uuid"
    };

    CHECK_REQUESTS;

    auto expectedEvents = std::vector<std::pair<EventType, std::string>> {
        {EventType::ResourceRemoved, "/redfish/v1/Systems/1/Processors/2"},
        {EventType::ResourceRemoved, "/redfish/v1/Systems/2"},
        {EventType::ResourceAdded, "/redfish/v1/Systems/1/Processors/3"},
        {EventType::ResourceAdded, "/redfish/v1/Systems/3"}
    };

    CHECK_EVENTS;
}

}
}
}
}
