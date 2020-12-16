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
 * */

#pragma once



#include "agent-framework/module/storage_components.hpp"
#include "agent-framework/module/requests/storage/get_physical_drive_info.hpp"



namespace psme {
namespace rest {
namespace model {
namespace handler {

using PhysicalDriveHandlerBase = GenericHandler <
agent_framework::model::requests::GetPhysicalDriveInfo,
agent_framework::model::PhysicalDrive,
IdPolicy<agent_framework::model::enums::Component::PhysicalDrive, NumberingZone::PARENT_SPACE>>;

/*!
 * @brief PhysicalDriveHandler template specialization
 *
 * Physical drives come from two sources
 * - elements of sub-collection of StorageServices
 * - elements of sub-collection of Physical drives (recursive definition)
 * Above is the reason why we need separate implementation.
 */
class PhysicalDriveHandler : public PhysicalDriveHandlerBase {
public:
    PhysicalDriveHandler() : PhysicalDriveHandlerBase() {}


    virtual ~PhysicalDriveHandler();


protected:
    using StorageComponents = agent_framework::module::StorageComponents;


    /*!
     * @brief collection might be strong or weak
     *
     * @param[in] parent_component Component of parent node
     */
    bool is_strong_collection(const Component parent_component) {
        return (Component::StorageServices == parent_component);
    }


    /*!
     * brief Specialization of fetch siblings
     * @param ctx - keeps data that is required during processing and need to be passed down to sub-handlers
     * @param parent_uuid - uuid of parent for components we want to retrieve
     * @param collection_name name of the collection
     * Physical Drive can be either sub-component of StorageService or sub-component of LogicalDrive
     * This code handles both cases
     */
    void fetch_siblings(Context& ctx, const std::string& parent_uuid, const std::string& collection_name) override {
        if (is_strong_collection(ctx.get_parent_component())) {
            return PhysicalDriveHandlerBase::fetch_siblings(ctx, parent_uuid,
                                                            collection_name);
        }
        else if (Component::LogicalDrive == ctx.get_parent_component()) {
            fetch_parent_children(ctx, parent_uuid, collection_name,
                                  StorageComponents::get_instance()->get_physical_drive_members_manager());
        }
    }


    /*!
     * @brief Overrides remove_agent_data() from GenericManager
     *
     * This override is necessary to properly clean the contents of
     * LogicalDrivesMembersManager
     * */
    void remove_agent_data(const std::string& gami_id) override {
        PhysicalDriveHandlerBase::remove_agent_data(gami_id);

        StorageComponents::get_instance()->
            get_physical_drive_members_manager().clean_resources_for_agent(gami_id);
    }


    /*!
     * @brief removes a component and all its descendants from the model
     *
     * @param uuid component's uuid
     * */
    void remove(const std::string& uuid) override {
        // remove links where physical to remove is a child of other drive
        StorageComponents::get_instance()->get_physical_drive_members_manager().remove_child(uuid);

        PhysicalDriveHandlerBase::remove(uuid);
    }


    bool do_accept_recursively(ResourceVisitor& visitor,
                               const std::string& parent_uuid,
                               const Component parent_component) override {
        if (is_strong_collection(parent_component)) {
            return PhysicalDriveHandlerBase::do_accept_recursively(visitor, parent_uuid, parent_component);
        }
        else {
            auto& manager = StorageComponents::get_instance()->get_physical_drive_members_manager();
            auto children = manager.get_children(parent_uuid);
            for (const std::string& child_uuid : children) {
                if (!do_accept(visitor, child_uuid)) {
                    return false; // break
                }
            }
        }
        return true;
    }
};


PhysicalDriveHandler::~PhysicalDriveHandler() {}

}
}
}
}

