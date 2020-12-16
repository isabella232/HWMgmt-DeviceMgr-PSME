*** Settings ***
Documentation  Utilities for SNMP testing.

Resource  ../../lib/rest_client.robot
Resource  ../../lib/utils.robot

*** Keywords ***

Get SNMP URI List
    [Documentation]  Get all SNMP URIs and return them as list.

    # Sample output:
    #   "data": [
    #     "/xyz/openbmc_project/network/snmp/manager/e9767624",
    #     "/xyz/openbmc_project/network/snmp/manager/31f4ce8b"
    #   ],

    @{snmp_uri_list}=  Read Properties  ${SNMP_MANAGER_URI}

    [Return]  @{snmp_uri_list}

Configure SNMP Manager On BMC
    [Documentation]  Configure SNMP manager on BMC.
    [Arguments]  ${snmp_ip}  ${port}  ${expected_result}

    # Description of argument(s):
    # snmp_ip          SNMP manager IP.
    # port             Network port where SNMP manager is listening.
    # expected_result  Expected status of SNMP configuration.

    @{snmp_parm_list}=  Create List  ${snmp_ip}  ${port}

    ${data}=  Create Dictionary  data=@{snmp_parm_list}

    ${resp}=  OpenBMC Post Request
    ...  ${SNMP_MANAGER_URI}action/Client  data=${data}

    Run Keyword If  '${expected_result}' == 'error'
    ...      Should Be Equal As Strings
    ...      ${resp.status_code}  ${HTTP_BAD_REQUEST}
    ...      msg=Allowing the configuration of an invalid SNMP.
    ...  ELSE
    ...      Should Be Equal As Strings  ${resp.status_code}  ${HTTP_OK}
    ...      msg=Not allowing the configuration of a valid SNMP.

Get List Of SNMP Manager And Port Configured On BMC
    [Documentation]  Get list of SNMP managers and return the list.

    @{snmp_uri_list}=  Get SNMP URI List
    @{ip_and_port_list}=  Create List

    # Sample output of snmp_uri_list enumeration.
    # {
    #  "data": {
    #    "/xyz/openbmc_project/network/snmp/manager/92ae7a66": {
    #      "Address": "10.6.6.6",
    #      "AddressFamily": "xyz.openbmc_project.Network.Client.IPProtocol.IPv4",
    #      "Port": 186
    #    },

    : FOR  ${snmp_uri}  IN   @{snmp_uri_list}
    \  ${ip}=  Read Attribute  ${snmp_uri}  Address
    \  ${port}=  Read Attribute  ${snmp_uri}  Port
    \  Append To List  ${ip_and_port_list}  ${ip}  ${port}

    [Return]  @{ip_and_port_list}

Verify SNMP Manager
    [Documentation]  Verify SNMP manager configured on BMC.
    [Arguments]  ${snmp_ip}  ${port}

    # Description of argument(s):
    # snmp_ip  SNMP manager IP.
    # port     Network port where SNMP manager is listening.

    @{ip_and_port}=  Create List  ${snmp_ip}  ${port}

    @{ip_and_port_list}=  Get List Of SNMP Manager And Port Configured On BMC

    List Should Contain Sub List  ${ip_and_port_list}  ${ip_and_port}
    ...  msg=Valid SNMP manager is not found on BMC.

Delete SNMP Manager And Object
    [Documentation]  Delete SNMP manager.
    [Arguments]  ${snmp_ip}  ${port}

    # Description of argument(s):
    # snmp_ip  SNMP manager IP.
    # port     Network port where SNMP manager is listening.

    @{snmp_uri_list}=  Get SNMP URI List

    # Find SNMP object having this IP and port.

    : FOR  ${snmp_uri}  IN  @{snmp_uri_list}
    \  ${ip}=  Read Attribute  ${snmp_uri}  Address
    \  ${port1}=  Read Attribute  ${snmp_uri}  Port
    \  Run Keyword If  "${snmp_ip}" == "${ip}" and "${port}" == "${port1}"
    ...  Exit For Loop

    # If the given IP and port is not configured, return.
    # Otherwise, delete the IP and object.

    Run Keyword And Return If  '${snmp_ip}' != '${ip}'
    ...  Pass Execution  SNMP manager to be deleted is not configured.

    OpenBMC Delete Request  ${snmp_uri}

    # Verify whether deleted SNMP is removed from BMC system.
    @{ip_and_port}=  Create List  ${snmp_ip}  ${port}
    @{ip_and_port_list}=  Get List Of SNMP Manager And Port Configured On BMC

    ${status}=  Run Keyword And Return Status  List Should Contain Sub List
    ...  ${ip_and_port_list}  ${ip_and_port}

    Should Be Equal  ${status}  ${False}  msg=SNMP manager is not deleted.
