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
 * Edgecore DeviceManager
 * Copyright 2020-2021 Edgecore Networks, Inc.
 *
 * This product includes software developed at
 * Edgecore Networks Inc. (http://www.edge-core.com/).
 *
 * */

#include "psme/rest/constants/constants.hpp"

namespace psme {
namespace rest {
namespace constants {


namespace Chassis {
const char CHASSIS_TYPE[] = "ChassisType";
const char SKU[] = "SKU";
const char INDICATOR_LED[] = "IndicatorLED";
const char LOCATION[] = "Location";
const char PARENT_ID[] = "ParentId";
const char CONTAINS[] = "Contains";
const char COMPUTER_SYSTEMS[] = "ComputerSystems";
const char SWITCHES[] = "Switches";
const char MANAGERS_IN_CHASSIS[] = "ManagersInChassis";
const char THERMAL_ZONES[] = "ThermalZones";
const char POWER_ZONES[] = "PowerZones";
const char DRIVES[] = "Drives";
const char STORAGE[] = "Storage";
const char PCIE_DEVICES[] = "PCIeDevices";
/*Nick Added Begin: */
const char FANS[]="Fans"; 
const char PSUS[] = "PowerSupplies";
const char PCTRL[] = "PowerControl";
const char PVOLTAGE[] = "Voltages";
const char PSU[] = "Power";
const char THERMALS[] = "Thermals";
const char TEMPS[] = "Temperatures";
const char THERMAL[] = "Thermal";
const char CHASSIS_RESET[] = "Chassis.Reset";
const char FORCE_OFF[] = "ForceOff";
const char GRACEFUL_SHUTDOWN[] = "GracefulShutdown";
const char FORCE_RESTART[] = "ForceRestart";
const char GRACEFUL_RESTART[] = "GracefulRestart";
const char FORCE_ON[] = "ForceOn";
const char PUSH_POWER_BUTTON[] = "PushPowerButton";
const char TARGET[] = "target";
const char ALLOWABLE_RESET_TYPES[] = "ResetType@Redfish.AllowableValues";
const char POWER_STATE[] = "PowerState";

/*Nick Added End  : */
}

namespace Zone {
const char RACK_LOCATION[] = "RackLocation";
const char UHEIGHT[] = "Uheight";
const char ULOCATION[] = "ULocation";
const char UNIT[] = "Unit";
const char XLOCATION[] = "XLocation";
const char MEMBER_ID[] = "MemberId";
const char RELATED_ITEM[] = "RelatedItem";
}

namespace PowerZone
{
const char POWER_CONTROL[] = "PowerControl";
const char MAX_PSUS_SUPPORTED[] = "MaxPSUsSupported";
const char POWER_AVAILABLE[] = "PowerAvailableWatts";
const char POWER_CAPACITY[] = "PowerCapacity";
const char POWER_CONSUMED[] = "PowerConsumedWatts";
const char POWER_SUPPLIES[] = "PowerSupplies";
const char LAST_POWER_OUTPUT[] = "LastPowerOutputWatts";
const char POWER_REQUESTED_WATTS[] = "PowerRequestedWatts";
const char POWER_AVAILABLE_WATTS[] = "PowerAvailableWatts";
const char POWER_ALLOCATED_WATTS[] = "PowerAllocatedWatts";
const char POWER_METRICS[] = "PowerMetrics";
const char POWER_LIMIT[] = "PowerLimit";
const char READING_VOLTS[] = "ReadingVolts";
const char POWER_SUPPLY_TYPE[] = "PowerSupplyType";
const char LINE_INPUT_VOLTAGE_TYPE[] = "LineInputVoltageType";
const char LINE_INPUT_VOLTAGE[] = "LineInputVoltage";
const char POWER_CAPACITY_WATTS[] = "PowerCapacityWatts";
const char LINE_INPUT_TYPE[] = "InputType";
const char MINIMUM_VOLTAGE[] = "MinimumVoltage";
const char MAXIMUM_VOLTAGE[] = "MaximumVoltage";
const char OUTPUT_WATTAGE[] = "OutputWattage";
const char INTER_VAL_IN_MIN[] = "IntervalInMin";
const char MIN_CONSUMED_WATTS[] = "MinConsumedWatts";
const char MAX_CONSUMED_WATTS[] = "MaxConsumedWatts";
const char AVG_CONSUMED_WATTS[] = "AverageConsumedWatts";
const char LIMIT_IN_WATTS[] = "LimitInWatts";
const char LIMIT_EXCEPTION[] ="LimitException";
const char CORRECTION_IN_MS[] ="CorrectionInMs";
const char POWER_SUPPLY[] = "PowerSupply";
} // namespace PowerZone

namespace ThermalZone {
const char FANS[] = "Fans";
const char FAN_NAME[] = "FanName";
const char PHYSICAL_CONTEXT[] = "PhysicalContext";
const char READING[] = "Reading";
const char READING_UNITS[] = "ReadingUnits";
const char DESIRED_SPEED_RPM[] = "DesiredSpeedRPM";
const char MAX_FANS_SUPPORTED[] = "MaxFansSupported";
const char PWM[] = "PWM";
const char POLICY[] = "Policy";
const char TEMPERATURES[] = "Temperatures";
const char READING_CELSIUS[] = "ReadingCelsius";
const char SENSOR_NUMBER[] = "SensorNumber";
const char STATUS[] = "State";  
const char HEALTH[] = "Health"; 
const char READINGCELSIUS[] = "ReadingCelsius";
}

namespace PncDrive {
const char SECURE_ERASE[] = "#Drive.SecureErase";
const char SECURE_ERASE_ENDPOINT[] = "Drive.SecureErase";
const char TARGET[] = "target";
const char CAPABLE_SPEED[] = "CapableSpeedGbs";
const char INDICATOR_LED[] = "IndicatorLED";
const char ENDPOINTS[] = "Endpoints";
const char VOLUMES[] = "Volumes";
const char MEDIA_TYPE[] = "MediaType";
const char NEGOTIATED_SPEED[] = "NegotiatedSpeedGbs";
const char PROTOCOL[] = "Protocol";
const char CAPACITY_BYTES[] = "CapacityBytes";
const char INFO[] = "Info";
const char INFO_FORMAT[] = "InfoFormat";
const char ERASE_ON_DETACH[] = "EraseOnDetach";
const char DRIVE_ERASED[] = "DriveErased";
const char FIRMWARE_VERSION[] = "FirmwareVersion";
const char IDENTIFIERS[] = "Identifiers";
const char DURABLE_NAME[] = "DurableName";
const char DURABLE_NAME_FORMAT[] = "DurableNameFormat";
const char SKU[] = "SKU";
const char STATUS_INDICATOR[] = "StatusIndicator";
const char REVISION[] = "Revision";
const char FAILURE_PREDICTED[] = "FailurePredicted";
const char HOTSPARE_TYPE[] = "HotspareType";
const char ENCRYPTION_ABILITY[] = "EncryptionAbility";
const char ENCRYPTION_STATUS[] = "EncryptionStatus";
const char ROTATION_SPEED_RPM[] = "RotationSpeedRPM";
const char BLOCK_SIZE_BYTES[] = "BlockSizeBytes";
const char PREDICTED_MEDIA_LIFE_LEFT[] = "PredictedMediaLifeLeftPercent";
const char PCIE_FUNCTION[] = "PCIeFunction";
}

namespace PcieDevice {
const char PCIE_FUNCTIONS[] = "PCIeFunctions";
const char DEVICE_TYPE[] = "DeviceType";
}

namespace PcieFunction {
const char FUNCTION_ID[] = "FunctionId";
const char FUNCTION_TYPE[] = "FunctionType";
const char DEVICE_CLASS[] = "DeviceClass";
const char DEVICE_ID[] = "DeviceId";
const char VENDOR_ID[] = "VendorId";
const char CLASS_CODE[] = "ClassCode";
const char REVISION_ID[] = "RevisionId";
const char SUBSYSTEM_ID[] = "SubsystemId";
const char SUBSYSTEM_VENDOR_ID[] = "SubsystemVendorId";
const char PCIE_DEVICE[] = "PCIeDevice";
}

}
}
}
