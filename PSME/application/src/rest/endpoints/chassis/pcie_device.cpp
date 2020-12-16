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

#include "agent-framework/module/constants/pnc.hpp"
#include "agent-framework/module/requests/common.hpp"
#include "agent-framework/module/responses/common.hpp"
#include "psme/rest/utils/status_helpers.hpp"
#include "psme/rest/endpoints/chassis/pcie_device.hpp"
#include "psme/rest/validators/json_validator.hpp"
#include "psme/rest/validators/schemas/pcie_device.hpp"
#include "psme/rest/server/error/error_factory.hpp"



using namespace psme::rest;
using namespace psme::rest::endpoint;
using namespace psme::rest::constants;
using namespace psme::rest::validators;

namespace {
json::Value make_prototype() {
    json::Value r(json::Value::Type::OBJECT);

    r[Common::ODATA_CONTEXT] = "/redfish/v1/$metadata#PCIeDevice.PCIeDevice";
    r[Common::ODATA_ID] = json::Value::Type::NIL;
    r[Common::ODATA_TYPE] = "#PCIeDevice.v1_0_0.PCIeDevice";
    r[Common::NAME] = "PCIe Device";

    r[Common::ASSET_TAG] = json::Value::Type::NIL;
    r[Common::DESCRIPTION] = "PCIe Device Description";
    r[constants::PcieDevice::DEVICE_TYPE] = json::Value::Type::NIL;
    r[constants::FabricCommon::FIRMWARE_VERSION] = json::Value::Type::NIL;
    r[Common::ID] = json::Value::Type::NIL;

    r[Common::LINKS][Common::CHASSIS] = json::Value::Type::ARRAY;
    r[Common::LINKS][constants::PcieDevice::PCIE_FUNCTIONS] = json::Value::Type::ARRAY;
    r[Common::LINKS][Common::OEM] = json::Value::Type::OBJECT;

    r[Common::MANUFACTURER] = json::Value::Type::NIL;
    r[Common::MODEL] = json::Value::Type::NIL;
    r[Common::OEM] = json::Value::Type::OBJECT;
    r[Common::PART_NUMBER] = json::Value::Type::NIL;
    r[FabricCommon::SKU] = json::Value::Type::NIL;
    r[Common::SERIAL] = json::Value::Type::NIL;

    r[Common::STATUS][Common::STATE] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH] = json::Value::Type::NIL;
    r[Common::STATUS][Common::HEALTH_ROLLUP] = json::Value::Type::NIL;

    return r;
}


void fill_links(const agent_framework::model::PcieDevice& device, json::Value& json) {
    if (device.get_chassis().has_value()) {
        try {
            auto chassis_id = agent_framework::module::get_manager<agent_framework::model::Chassis>()
                .get_entry_reference(device.get_chassis().value())->get_id();

            // fill Chassis link
            json::Value chassis_link;
            chassis_link[Common::ODATA_ID] = endpoint::PathBuilder(PathParam::BASE_URL)
                .append(Common::CHASSIS)
                .append(chassis_id).build();
            json[Common::LINKS][Common::CHASSIS].push_back(std::move(chassis_link));

            // fill Functions link
            for (const auto function_id : agent_framework::module::get_manager<agent_framework::model::PcieFunction>().get_ids(device.get_uuid())) {
                json::Value function_link;
                function_link[Common::ODATA_ID] = endpoint::PathBuilder(constants::PathParam::BASE_URL)
                    .append(constants::Common::CHASSIS)
                    .append(chassis_id)
                    .append(constants::Chassis::PCIE_DEVICES)
                    .append(device.get_id())
                    .append(constants::PathParam::FUNCTIONS)
                    .append(function_id).build();
                json[Common::LINKS][constants::PcieDevice::PCIE_FUNCTIONS].push_back(std::move(function_link));
            }
        }
        catch (agent_framework::exceptions::InvalidUuid) {
            log_error(GET_LOGGER("rest"), "Device " + device.get_uuid() + " has chassis "
                                          + device.get_chassis().value() + " which does not exist as a resource");
        }
    }
}


static const std::map<std::string, std::string> gami_to_rest_attributes = {
    {agent_framework::model::literals::PcieDevice::ASSET_TAG, constants::Common::ASSET_TAG}
};

}


endpoint::PcieDevice::PcieDevice(const std::string& path) : EndpointBase(path) {}


endpoint::PcieDevice::~PcieDevice() {}


void endpoint::PcieDevice::get(const server::Request& req, server::Response& res) {
    auto json = ::make_prototype();

    json[Common::ODATA_ID] = PathBuilder(req).build();

    // the devices are under the same manager as the chassis from the URL
    auto chassis_manager_uuid = psme::rest::model::Find<agent_framework::model::Chassis>(
        req.params[PathParam::CHASSIS_ID]).get_one()->get_parent_uuid();

    const auto device =
        psme::rest::model::Find<agent_framework::model::PcieDevice>(req.params[PathParam::DEVICE_ID]).via(chassis_manager_uuid).get();

    json[Common::ASSET_TAG] = device.get_asset_tag();
    json[constants::PcieDevice::DEVICE_TYPE] = device.get_device_class();
    json[constants::FabricCommon::FIRMWARE_VERSION] = device.get_firmware_version();
    json[Common::ID] = req.params[PathParam::DEVICE_ID];
    json[FabricCommon::SKU] = device.get_sku();

    fill_links(device, json);

    const auto& fru = device.get_fru_info();
    json[Common::MANUFACTURER] = fru.get_manufacturer();
    json[Common::MODEL] = fru.get_model_number();
    json[Common::PART_NUMBER] = fru.get_part_number();
    json[Common::SERIAL] = fru.get_serial_number();

    endpoint::status_to_json(device, json);

    json[Common::STATUS][Common::HEALTH_ROLLUP] =
        psme::rest::endpoint::HealthRollup<agent_framework::model::PcieDevice>().get(device.get_uuid());

    set_response(res, json);
}


void endpoint::PcieDevice::patch(const server::Request& request, server::Response& response) {
    auto device = model::Find<agent_framework::model::PcieDevice>(request.params[PathParam::DEVICE_ID]).get();

    const auto json = JsonValidator::validate_request_body<schema::PcieDevicePatchSchema>(request);

    agent_framework::model::attribute::Attributes attributes{};

    if (json.is_member(constants::Common::ASSET_TAG)) {
        const auto& asset_tag = json[constants::Common::ASSET_TAG].as_string();
        attributes.set_value(agent_framework::model::literals::PcieDevice::ASSET_TAG, asset_tag);
    }

    if (!attributes.empty()) {
        agent_framework::model::requests::SetComponentAttributes
            set_component_attributes_request{device.get_uuid(), attributes};

        const auto& gami_agent = psme::core::agent::AgentManager::get_instance()->get_agent(device.get_agent_id());

        auto set_pcie_device_attributes = [&, gami_agent] {
            // Call set component attribute method
            const auto& set_component_attributes_response = gami_agent->
                execute<agent_framework::model::responses::SetComponentAttributes>(set_component_attributes_request);

            const auto& result_statuses = set_component_attributes_response.get_statuses();
            if (!result_statuses.empty()) {
                const auto& error = error::ErrorFactory::create_error_from_set_component_attributes_results(
                    result_statuses, gami_to_rest_attributes);
                throw error::ServerException(error);
            }

            psme::rest::model::handler::HandlerManager::get_instance()->get_handler(
                agent_framework::model::enums::Component::PcieDevice)->
                load(gami_agent,
                     device.get_parent_uuid(),
                     agent_framework::model::enums::Component::Manager,
                     device.get_uuid(),
                     false);
        };

        gami_agent->execute_in_transaction(set_pcie_device_attributes);
    }
    get(request, response);
}
