*** Settings ***
Documentation           Synaccess Protocol Distribution Unit (PDU) library

Resource        ../../lib/pdu/pdu.robot
Library         RequestsLibrary.RequestsKeywords

*** Keywords ***
Connect and Login
    [Documentation]  Open PDU connection and login.
    Validate Prereq
    SSHLibrary.Open Connection  ${PDU_IP}
    ${auth}=    Create List     ${PDU_USERNAME}    ${PDU_PASSWORD}
    Create Session    pdu    http://${PDU_IP}   auth=${auth}

Power Cycle
    [Documentation]  Perform PDU power cycle.
    Connect and Login
    ${ret}=    Get Request    pdu    /cmd.cgi?$A4 ${PDU_SLOT_NO}
    ${error_message}=  Catenate  Power cycle of slot ${PDU_SLOT_NO} failed.
    ...  PDU returned RC=${ret}.
    Should Be Equal As Strings  ${ret}  ${HTTP_OK}  msg=${error_message}
