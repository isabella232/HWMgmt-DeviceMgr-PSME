*** Settings ***
Library           Collections
Library           String
Library           RequestsLibrary.RequestsKeywords
Library           OperatingSystem
Variables         ../data/variables.py

*** Variables ***

${OPENBMC_MODEL}  ${EMPTY}
${OPENBMC_HOST}   ${EMPTY}
${DBUS_PREFIX}    ${EMPTY}
${PORT}           ${EMPTY}
# AUTH_SUFFIX here is derived from variables.py
${AUTH_URI}       https://${OPENBMC_HOST}${AUTH_SUFFIX}
${OPENBMC_USERNAME}    admin
${OPENBMC_PASSWORD}    redfish
${REST_USERNAME}       admin
${REST_PASSWORD}       redfish
${IPMI_PASSWORD}       redfish
${MACHINE_TYPE}    palmetto
${DBUS_POLL_INTERVAL}      15s
${OPENBMC_REBOOT_TIMEOUT}   ${10}
# IPMI_COMMAND here is set to "External" by default. User
# can override to "Dbus" from command line.
${IPMI_COMMAND}    External
# IPMI chipher default.
${IPMI_CIPHER_LEVEL}  ${3}

# PDU related parameters
${PDU_TYPE}         ${EMPTY}
${PDU_IP}           ${EMPTY}
${PDU_USERNAME}     ${EMPTY}
${PDU_PASSWORD}     ${EMPTY}
${PDU_SLOT_NO}      ${EMPTY}

# User define input SSH and HTTPS related parameters
${SSH_PORT}         ${EMPTY}
${HTTPS_PORT}       ${EMPTY}
${OPENBMC_SERIAL_HOST}      ${EMPTY}
${OPENBMC_SERIAL_PORT}      ${EMPTY}

# OS related parameters.
${OS_HOST}          ${EMPTY}
${OS_USERNAME}      ${EMPTY}
${OS_PASSWORD}      ${EMPTY}
${OS_WAIT_TIMEOUT}  ${15*60}

# Networking related parameters
${NETWORK_PORT}        80
${PACKET_TYPE}         tcp
${NETWORK_RETRY_TIME}  1
${NETWORK_TIMEOUT}     5
# BMC debug tarball parameter
${DEBUG_TARBALL_PATH}  ${EMPTY}

# Upload Image parameters
${TFTP_SERVER}                  ${EMPTY}
${PNOR_TFTP_FILE_NAME}          ${EMPTY}
${BMC_TFTP_FILE_NAME}           ${EMPTY}
${IMAGE_FILE_PATH}              ${EMPTY}
${ALTERNATE_IMAGE_FILE_PATH}    ${EMPTY}
${PNOR_IMAGE_FILE_PATH}         ${EMPTY}
${BMC_IMAGE_FILE_PATH}          ${EMPTY}
${BAD_IMAGES_DIR_PATH}          ${EMPTY}
${SKIP_UPDATE_IF_ACTIVE}        false

# Parameters for doing N-1 and N+1 code updates.
${N_MINUS_ONE_IMAGE_FILE_PATH}    ${EMPTY}
${N_PLUS_ONE_IMAGE_FILE_PATH}     ${EMPTY}

# The caller must set this to the string "true" in order to delete images. The
# code is picky.
${DELETE_OLD_PNOR_IMAGES}   false
${DELETE_OLD_GUARD_FILE}    false

# Caller can specify a value for LAST_KNOWN_GOOD_VERSION to indicate that if
# the machine already has that version on it, the update should be skipped.
${LAST_KNOWN_GOOD_VERSION}  ${EMPTY}

# By default field mode is disabled.
${FIELD_MODE}               ${False}

# LDAP related variables.
${LDAP_BASE_DN}             ${EMPTY}
${LDAP_BIND_DN}             ${EMPTY}
${LDAP_SERVER_HOST}         ${EMPTY}
${LDAP_SECURE_MODE}         ${EMPTY}
${LDAP_BIND_DN_PASSWORD}    ${EMPTY}
${LDAP_SEARCH_SCOPE}        ${EMPTY}
${LDAP_TYPE}                ${EMPTY}
${LDAP_USER}                ${EMPTY}
${LDAP_USER_PASSWORD}       ${EMPTY}

*** Keywords ***
Get Inventory Schema
    [Documentation]  Get inventory schema.
    [Arguments]    ${machine}
    [Return]    &{INVENTORY}[${machine}]

Get Inventory Items Schema
    [Documentation]  Get inventory items schema.
    [Arguments]    ${machine}
    [Return]    &{INVENTORY_ITEMS}[${machine}]

Get Sensor Schema
    [Documentation]  Get sensors schema.
    [Arguments]    ${machine}
    [Return]    &{SENSORS}[${machine}]
