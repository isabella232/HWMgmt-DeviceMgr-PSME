
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
 * @brief EthernetSwitchPort model interface
 * */
#pragma once
#include "agent-framework/module/model/attributes/model_attributes.hpp"
#include "agent-framework/module/model/resource.hpp"
#include "agent-framework/module/enum/network.hpp"
#include "agent-framework/module/enum/common.hpp"

namespace agent_framework
{
namespace model
{

/*! EthernetSwitchPort */
class EthernetSwitchPort : public Resource
{
public:
    explicit EthernetSwitchPort(const std::string &parent_uuid = {}, enums::Component parent_type = enums::Component::EthernetSwitch);
    ~EthernetSwitchPort();

    EthernetSwitchPort(const EthernetSwitchPort &) = default;
    EthernetSwitchPort &operator=(const EthernetSwitchPort &) = default;
    EthernetSwitchPort(EthernetSwitchPort &&) = default;
    EthernetSwitchPort &operator=(EthernetSwitchPort &&) = default;

    /*!
     * @brief construct an object of class EthernetSwitchPort from JSON
     *
     * @param json the Json::Value deserialized to object
     *
     * @return the newly constructed EthernetSwitchPort object
     */
    static EthernetSwitchPort from_json(const Json::Value &json);

    /*!
     * @brief transform the object to JSon
     *
     * @return the object serialized to Json::Value
     */
    Json::Value to_json() const;

    /*!
     * @brief Get collection name
     * @return collection name
     */
    static const enums::CollectionName &get_collection_name()
    {
        return EthernetSwitchPort::collection_name;
    }

    /*!
     * @brief Get component name
     * @return component name
     */
    static const enums::Component &get_component()
    {
        return EthernetSwitchPort::component;
    }

    /*!
     * @brief Set port identifier
     * @param[in] port_identifier switch port identifier
     * */
    void set_port_identifier(const OptionalField<std::string> &port_identifier)
    {
        m_port_identifier = port_identifier;
    }

    /*!
     * @brief Get port identifier
     * @return Port identifier of switch port
     * */
    const OptionalField<std::string> &get_port_identifier() const
    {
        return m_port_identifier;
    }

    /*!
     * @brief Set port id
     * @param[in] port_id switch port identifier
     * */
    void set_port_id(const OptionalField<std::uint32_t> &port_id)
    {
        m_port_id = port_id;
    }

    /*!
     * @brief Get port id
     * @return Port id of switch port
     * */
    const OptionalField<std::uint32_t> &get_port_id() const
    {
        return m_port_id;
    }

    /*!
     * @brief Set port class
     * @param[in] port_class switch port class
     * */
    void set_port_class(const OptionalField<enums::PortClass> &port_class)
    {
        m_port_class = port_class;
    }

    /*!
     * @brief Get port class
     * @return Port class of switch port
     * */
    const OptionalField<enums::PortClass> &get_port_class() const
    {
        return m_port_class;
    }

    /*!
     * @brief Set port type
     * @param[in] port_type switch port type
     * */
    void set_port_type(const OptionalField<enums::PortType> &port_type)
    {
        m_port_type = port_type;
    }

    /*!
     * @brief Get port type
     * @return Port type of switch port
     * */
    const OptionalField<enums::PortType> &get_port_type() const
    {
        return m_port_type;
    }

    /*!
     * @brief Set port mode
     * @param[in] port_mode switch port mode
     * */
    void set_port_mode(const OptionalField<enums::PortMode> &port_mode)
    {
        m_port_mode = port_mode;
    }

    /*!
     * @brief Get port mode
     * @return Port mode of switch port
     * */
    const OptionalField<enums::PortMode> &get_port_mode() const
    {
        return m_port_mode;
    }

    /*!
     * @brief Set link technology
     * @param[in] link_technology switch link technology
     * */
    void set_link_technology(const OptionalField<enums::LinkTechnology> &link_technology)
    {
        m_link_technology = link_technology;
    }

    /*!
     * @brief Get link technology
     * @return Link technology of switch port
     * */
    const OptionalField<enums::LinkTechnology> &get_link_technology() const
    {
        return m_link_technology;
    }

    /*!
     * @brief Set port link speed in Mbps
     * @param[in] link_speed_mbps blade link speed in Mbps
     * */
    void set_link_speed_mbps(const OptionalField<std::uint32_t> &link_speed_mbps)
    {
        m_link_speed_mbps = link_speed_mbps;
    }

    /*!
     * @brief Get link speed in Mbps
     * @return link speed in Mbps
     * */
    const OptionalField<std::uint32_t> &get_link_speed_mbps() const
    {
        return m_link_speed_mbps;
    }

    /*!
     * @brief Set port max speed in Mbps
     * @param[in] max_speed_mbps blade max speed in Mbps
     * */
    void set_max_speed_mbps(const OptionalField<std::uint32_t> &max_speed_mbps)
    {
        m_max_speed_mbps = max_speed_mbps;
    }

    /*!
     * @brief Get max speed in Mbps
     * @return max speed in Mbps
     * */
    const OptionalField<std::uint32_t> &get_max_speed_mbps() const
    {
        return m_max_speed_mbps;
    }

    /*!
     * @brief Set operational state
     * @param[in] operational_state switch operational state
     * */
    void set_operational_state(
        const OptionalField<enums::OperationalState> &operational_state)
    {
        m_operational_state = operational_state;
    }

    /*!
     * @brief Get operational state
     * @return operational state of switch port
     * */
    const OptionalField<enums::OperationalState> &get_operational_state() const
    {
        return m_operational_state;
    }

    /*!
     * @brief Set administrative state
     * @param[in] administrative_state switch administrative state
     * */
    void set_administrative_state(
        const OptionalField<enums::AdministrativeState> &administrative_state)
    {
        m_administrative_state = administrative_state;
    }

    /*!
     * @brief Get administrative state
     * @return administrative state of switch port
     * */
    const OptionalField<enums::AdministrativeState> &get_administrative_state() const
    {
        return m_administrative_state;
    }

    /*!
     * @brief Set port width
     * @param[in] port_width port width
     * */
    void set_port_width(const OptionalField<std::uint32_t> &port_width)
    {
        m_port_width = port_width;
    }

    /*!
     * @brief Get port width
     * @return port width
     * */
    const OptionalField<std::uint32_t> &get_port_width() const
    {
        return m_port_width;
    }

    /*!
     * @brief Set frame size
     * @param[in] frame_size frame size
     * */
    void set_frame_size(const OptionalField<std::uint32_t> &frame_size)
    {
        m_frame_size = frame_size;
    }

    /*!
     * @brief Get frame size
     * @return frame size
     * */
    const OptionalField<std::uint32_t> &get_frame_size() const
    {
        return m_frame_size;
    }

    /*!
     * @brief Set auto sense
     * @param[in] auto_sense auto sense
     * */
    void set_auto_sense(const OptionalField<bool> &auto_sense)
    {
        m_auto_sense = auto_sense;
    }

    /*!
     * @brief Get auto sense
     * @return auto sense
     * */
    const OptionalField<bool> &get_auto_sense() const
    {
        return m_auto_sense;
    }

    /*!
     * @brief Set full duplex flag
     * @param[in] full_duplex to set or NIL to remove
     * */
    void set_full_duplex(const OptionalField<bool> &full_duplex)
    {
        m_full_duplex = full_duplex;
    }

    /*!
     * @brief Get full duplex flag
     * @return full duplex or NIL when not set
     * */
    const OptionalField<bool> &get_full_duplex() const
    {
        return m_full_duplex;
    }

    /*!
     * @brief Set port to be a management port
     * @param[in] is_management_port is the port a management port?
     * */
    void set_is_management_port(const OptionalField<bool> &is_management_port)
    {
        m_is_management_port = is_management_port;
    }

    /*!
     * @brief get is management port boolean
     * @return is management port
     * */
    const OptionalField<bool> &get_is_management_port() const
    {
        return m_is_management_port;
    }

    /*!
     * @brief Set last error code
     * @param[in] last_error_code last error code
     * */
    void set_last_error_code(const OptionalField<std::uint32_t> &last_error_code)
    {
        m_last_error_code = last_error_code;
    }

    /*!
     * @brief Get last error code
     * @return last error code
     * */
    const OptionalField<std::uint32_t> &get_last_error_code() const
    {
        return m_last_error_code;
    }

    /*!
     * @brief Set error cleared
     * @param[in] error_cleared has the error been cleared
     * */
    void set_error_cleared(const OptionalField<bool> &error_cleared)
    {
        m_error_cleared = error_cleared;
    }

    /*!
     * @brief get error cleared boolean
     * @return error cleared
     * */
    const OptionalField<bool> &get_error_cleared() const
    {
        return m_error_cleared;
    }

    /*!
     * @brief Set last state change time
     * @param[in] last_state_change_time switch last state change time
     * */
    void set_last_state_change_time(
        const OptionalField<std::string> &last_state_change_time)
    {
        m_last_state_change_time = last_state_change_time;
    }

    /*!
     * @brief Get last state change time
     * @return Time of last state change
     * */
    const OptionalField<std::string> &get_last_state_change_time() const
    {
        return m_last_state_change_time;
    }

    /*!
     * @brief Set mac address
     * @param[in] mac_address switch port mac address
     * */
    void set_mac_address(const OptionalField<std::string> &mac_address)
    {
        m_mac_address = mac_address;
    }

    /*!
     * @brief Get mac address
     * @return mac address
     * */
    const OptionalField<std::string> &get_mac_address() const
    {
        return m_mac_address;
    }

    /*!
     * @brief Set ipv4 address
     * @param[in] ipv4_address switch port ipv4 address
     * */
    void set_ipv4_address(const attribute::Ipv4Address &ipv4_address)
    {
        m_ipv4_address = ipv4_address;
    }

    /*!
     * @brief Get ipv4 address
     * @return ipv4 address
     * */
    const attribute::Ipv4Address &get_ipv4_address() const
    {
        return m_ipv4_address;
    }

    /*!
     * @brief Set ipv6 address
     * @param[in] ipv6_address switch port ipv6 address
     * */
    void set_ipv6_address(const attribute::Ipv6Address &ipv6_address)
    {
        m_ipv6_address = ipv6_address;
    }

    /*!
     * @brief Get ipv6 address
     * @return ipv6 address
     * */
    const attribute::Ipv6Address &get_ipv6_address() const
    {
        return m_ipv6_address;
    }

    /*!
     * @brief Returns neighbor info.
     * @return Reference to NeighborInfo.
     * */
    const attribute::NeighborInfo &get_neighbor_info() const
    {
        return m_neighbor_info;
    }

    /*!
     * @brief Set neighbor info
     * @param[in] neighbor_info neighbor info
     * */
    void set_neighbor_info(const attribute::NeighborInfo &neighbor_info)
    {
        m_neighbor_info = neighbor_info;
    }

    /*!
     * @brief Set neighbor mac
     * @param[in] neighbor_mac switch port neighbor mac
     * */
    void set_neighbor_mac(const OptionalField<std::string> &neighbor_mac)
    {
        m_neighbor_mac = neighbor_mac;
    }

    /*!
     * @brief Get neighbor mac
     * @return neighbor mac
     * */
    const OptionalField<std::string> &get_neighbor_mac() const
    {
        return m_neighbor_mac;
    }

    /*!
     * @brief Returns VLAN enable
     * @return VLAN enable
     * */
    const OptionalField<bool> &get_vlan_enable() const
    {
        return m_vlan_enable;
    }

    /*!
     * @brief Set vlan enable
     * @param[in] vlan_enable vlan enable
     * */
    void set_vlan_enable(const OptionalField<bool> &vlan_enable)
    {
        m_vlan_enable = vlan_enable;
    }

    /*!
     * @brief Set default vlan
     * @param[in] default_vlan switch port default vlan
     * */
    void set_default_vlan(const OptionalField<std::string> &default_vlan)
    {
        m_default_vlan = default_vlan;
    }

    /*!
     * @brief Get default vlan
     * @return default vlan
     * */
    const OptionalField<std::string> &get_default_vlan() const
    {
        return m_default_vlan;
    }

    /*!
     * @brief Returns trans info.
     * @return Reference to Trans.
     * */
    const attribute::TransInfo &get_trans_info() const
    {
        return m_trans_info;
    }

    /*!
     * @brief Returns pon trans rx_pwr info.
     * @return Reference to Onu_Trans_Rx_Pwr_Info 
     * */
    const attribute::Onu_Trans_Rx_Pwr_Info &get_pon_trans_rx_pwr_info() const
    {
        return m_pon_trans_rx_pwr_info;
    }

    /*!
     * @brief Returns trans info. in jason format
     * @return Reference to Trans.
     * */
    json::Value get_trans_info_json() const;

    /*!
     * @brief Returns pon rx pwr trans info. in jason format
     * @return Reference to Trans.
     * */
    json::Value get_pon_trans_rx_pwr_info_json() const;

    /*!
     * @brief Set  trans info.
     * @param[in] trans_info
     * */
    void set_trans_info(const attribute::TransInfo &trans_info)
    {
        m_trans_info = trans_info;
    }

    /*!
     * @brief Set pon trans rx_pwr info.
     * @param[in] Onu_Trans_Rx_Pwr_Info 
     * */
    void set_trans_rx_pwr_info(const attribute::Onu_Trans_Rx_Pwr_Info &trans_info)
    {
        m_pon_trans_rx_pwr_info = trans_info;
    }

private:
    OptionalField<std::string> m_port_identifier{};
    OptionalField<enums::PortClass> m_port_class{};
    OptionalField<enums::PortType> m_port_type{};
    OptionalField<enums::PortMode> m_port_mode{};
    OptionalField<enums::LinkTechnology> m_link_technology{};
    OptionalField<std::uint32_t> m_link_speed_mbps{};
    OptionalField<std::uint32_t> m_max_speed_mbps{};
    OptionalField<enums::OperationalState> m_operational_state{};
    OptionalField<enums::AdministrativeState> m_administrative_state{};
    OptionalField<std::uint32_t> m_port_width{};
    OptionalField<std::uint32_t> m_frame_size{};
    OptionalField<bool> m_auto_sense{};
    OptionalField<bool> m_full_duplex{};
    OptionalField<bool> m_is_management_port{};
    OptionalField<std::uint32_t> m_last_error_code{};
    OptionalField<bool> m_error_cleared{};
    OptionalField<std::string> m_last_state_change_time{};
    OptionalField<std::string> m_mac_address{};
    attribute::Ipv4Address m_ipv4_address{};
    attribute::Ipv6Address m_ipv6_address{};
    attribute::NeighborInfo m_neighbor_info{};
    attribute::TransInfo m_trans_info{};
    attribute::Onu_Trans_Rx_Pwr_Info m_pon_trans_rx_pwr_info{};

    OptionalField<std::string> m_neighbor_mac{};
    OptionalField<bool> m_vlan_enable{};
    OptionalField<std::string> m_default_vlan{};

    OptionalField<std::uint32_t> m_port_id{};

    static const enums::CollectionName collection_name;
    static const enums::Component component;
};

} // namespace model
} // namespace agent_framework
