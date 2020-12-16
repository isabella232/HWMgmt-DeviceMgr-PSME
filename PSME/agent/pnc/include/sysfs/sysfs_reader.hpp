/*!
 * @copyright
 * Copyright (c) 2016-2017 Intel Corporation
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
 * @file sysfs/sysfs_reader.hpp
 * @brief SysfsReader - implementation of the SysfsReaderInterface using libsysfs
 * */

#pragma once

#include "sysfs/sysfs_reader_interface.hpp"

#include "libsysfs.h"

namespace agent {
namespace pnc {
namespace sysfs {

/*!
 * SysfsReader class reading sysfs data for PCI buses
 */
class SysfsReader : public SysfsReaderInterface {
public:
    SysfsReader() {}
    SysfsReader(const SysfsReader&) = default;
    SysfsReader& operator=(const SysfsReader&) = default;
    virtual ~SysfsReader();

    /*!
     * @brief Reads list of all pci devices in the sysfs
     * @param[in] path String describing parents of the devices e.g. "0000:00:00.0/0000:01:00.0/"
     *                 Path should consist of series of devices formatted as 'xxxx:xx:xx.x/'
     * @return List of all PCI devices found in the system
     */
    virtual std::vector<RawSysfsDevice> get_raw_sysfs_devices(const std::string& path = "") const override;

    /*!
     * @brief Reads list of all drives under specific device
     * @param[in] switch_bridge_path Path to the parent switch bridge
     * @param[in] bus_num Switch bus number
     * @param[in] device_num Number of the PCIeDevice to check
     * @return List of all drives found
     */
    virtual std::vector<std::string> get_drives_for_device(
        const std::string& switch_bridge_path, uint8_t bus_num, uint8_t device_num) const;

    /*!
     * @brief Unbinds nvme driver for a specific device
     * @param[in] id Id of the device as in sysfs
     * @return True if unbind was successful
     * */
    virtual bool unbind_nvme_driver(const SysfsId& id) const;

private:

    /* checks if the sys_device has a path that has a part mathing 'path' parameter */
    bool is_path_matched(sysfs_device* sys_device, const std::string& path) const;

    /* updates drive data, if successful, adds drive to the device */
    void update_drive_info(const std::string& drive_name, RawSysfsDevice& device) const;

    /* updates pci device drives (if present), result is stored in provided device reference */
    void update_pci_device_drives(sysfs_device* sys_device, RawSysfsDevice& device) const;

    /* used to check if given device is a virtual function, result is stored in provided device reference */
    void update_pci_device_virtual(sysfs_device* sys_device, RawSysfsDevice& device) const;

    /* updates device configuration space, result is stored in provided device reference */
    bool update_pci_device_config(sysfs_device* sys_device, RawSysfsDevice& device) const;

    /* parses device name to read ids, result is stored in provided device reference */
    bool update_pci_device_ids(sysfs_device* sys_device, RawSysfsDevice& device) const;

    /* updates all information about the device, result is stored in provided device reference */
    bool update_pci_device(sysfs_device* sys_device, RawSysfsDevice& device) const;
};

}
}
}
