*** Settings ***
Documentation    Test Redfish to verify responses for SessionService and Hypermedia.

Resource         ../../lib/resource.robot
Resource         ../../lib/bmc_redfish_resource.robot

*** Test Cases ***

Redfish Login And Logout
    [Documentation]  Login to BMCweb and then logout.
    [Tags]  Redfish_Login_And_Logout

    Redfish.Login
    Redfish.Logout


GET Redfish Hypermedia Without Login
    [Documentation]  GET hypermedia URL without login.
    [Tags]  GET_Redfish_Hypermedia_Without_Login
    [Template]  GET And Verify Redfish Response

    # Expect status      Resource URL Path
    ${HTTP_OK}           /
    ${HTTP_OK}           /redfish
    ${HTTP_OK}           /redfish/v1


GET Redfish SessionService Resource With Login
    [Documentation]  Login to BMCweb and get /redfish/v1/SessionService.
    [Tags]  GET_Redfish_SessionService_Resource_With_Login

    Redfish.Login
    ${resp}=  Redfish.Get  /redfish/v1/SessionService
    Redfish.Logout


GET Redfish SessionService Without Login
    [Documentation]  Get /redfish/v1/SessionService without login
    [Tags]  GET_Redfish_SessionService_Without_Login

    ${resp}=  Redfish.Get  /redfish/v1/SessionService
    ...  valid_status_codes=[${HTTP_UNAUTHORIZED}]


Delete Redfish Session Using Valid login
    [Documentation]  Delete a session using valid login.
    [Tags]  Delete_Redfish_Session_Using_Valid_Login

    Redfish.Login
    ${session_info}=  Get Redfish Session Info

    Redfish.Login

    # Example o/p:
    # [{'@odata.id': '/redfish/v1/SessionService/Sessions/bOol3WlCI8'},
    #  {'@odata.id': '/redfish/v1/SessionService/Sessions/Yu3xFqjZr1'}]
    ${resp_list}=  Redfish_Utils.List Request
    ...  /redfish/v1/SessionService/Sessions

    Redfish.Delete  ${session_info["location"]}

    ${resp}=  Redfish_Utils.List Request  /redfish/v1/SessionService/Sessions
    List Should Not Contain Value  ${resp}  ${session_info["location"]}


*** Keywords ***

GET And Verify Redfish Response
    [Documentation]  GET given resource and verfiy response.
    [Arguments]  ${valid_status_codes}  ${resource_path}

    # Description of argument(s):
    # valid_status_codes            A comma-separated list of acceptable
    #                               status codes (e.g. 200).
    # resource_path                 Redfish resource URL path.

    ${resp}=  Redfish.Get  ${resource_path}
    ...  valid_status_codes=[${valid_status_codes}]
