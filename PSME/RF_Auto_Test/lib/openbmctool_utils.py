#!/usr/bin/env python

r"""
This module provides many valuable openbmctool.py functions such as
openbmctool_execute_command.
"""

import gen_print as gp
import gen_cmd as gc
import gen_valid as gv
import gen_misc as gm
import var_funcs as vf
import utils as utils
from robot.libraries.BuiltIn import BuiltIn
import re
import tempfile


def openbmctool_execute_command(command_string,
                                *args,
                                **kwargs):
    r"""
    Run the command string as an argument to the openbmctool.py program and
    return the stdout and the return code.

    This function provides several benefits versus calling shell_cmd directly:
    - This function will obtain the global values for OPENBMC_HOST,
      OPENBMC_USERNAME, etc.
    - This function will compose the openbmctool.py command string which
      includes the caller's command_string.
    - The openbmctool.py produces additional text that clutters the output.
      This function will remove such text.  Example:
        Attempting login...
        <actual output>
        User root has been logged out

    NOTE: If you have pipe symbols in your command_string, they must be
    surrounded by a single space on each side (see example below).

    Example code:
    ${rc}  ${output}=  Openbmctool Execute Command  fru status | head -n 2

    Example output:
    #(CDT) 2018/09/19 15:16:58 - Issuing: set -o pipefail ; openbmctool.py -H hostname -U root -P ********
    ...  fru status | tail -n +1 | egrep -v 'Attempting login|User [^ ]+ hasbeen logged out' | head -n 2
    Component     | Is a FRU  | Present  | Functional  | Has Logs
    cpu0          | Yes       | Yes      | Yes         | No

    Description of arguments:
    command_string                  The command string to be passed to the
                                    openbmctool.py program.
    All remaining arguments are passed directly to shell_cmd.  See the
    shell_cmd prolog for details on allowable arguments.  The caller may code
    them directly as in this example:
    openbmctool_execute_command("my command", quiet=1, max_attempts=2).
    Python will do the work of putting these values into args/kwargs.
    """

    if not gv.valid_value(command_string):
        return "", "", 1

    # Get global BMC variable values.
    openbmc_host = BuiltIn().get_variable_value("${OPENBMC_HOST}", default="")
    openbmc_username = BuiltIn().get_variable_value("${OPENBMC_USERNAME}",
                                                    default="")
    openbmc_password = BuiltIn().get_variable_value("${OPENBMC_PASSWORD}",
                                                    default="")
    if not gv.valid_value(openbmc_host):
        return "", "", 1
    if not gv.valid_value(openbmc_username):
        return "", "", 1
    if not gv.valid_value(openbmc_password):
        return "", "", 1

    # Break the caller's command up into separate piped commands.  For
    # example, the user may have specified "fru status | head -n 2" which
    # would be broken into 2 list elements.  We will also break on ">"
    # (re-direct).
    pipeline = map(str.strip, re.split(r' ([\|>]) ', str(command_string)))
    # The "tail" command below prevents a "egrep: write error: Broken pipe"
    # error if the user is piping the output to a sub-process.
    # Use "egrep -v" to get rid of editorial output from openbmctool.py.
    pipeline.insert(1, "| tail -n +1 | egrep -v 'Attempting login|User [^ ]+"
                    " has been logged out'")

    command_string = "set -o pipefail ; python3 $(which openbmctool.py) -H "\
        + openbmc_host + " -U " + openbmc_username + " -P " + openbmc_password\
        + " " + " ".join(pipeline)

    return gc.shell_cmd(command_string, *args, **kwargs)


def openbmctool_execute_command_json(command_string,
                                     *args,
                                     **kwargs):
    r"""
    Run the command string as an argument to the openbmctool.py program, parse
    the JSON output into a dictionary and return the dictionary.

    This function is a wrapper for openbmctool_execute_command (defined
    above).  The caller may provide any command string where the output will
    be JSON data.  This function will convert the JSON data to a python
    object, verify that the 'status' field = "ok" and return the 'data'
    sub-field to the caller.

    See openbmctool_execute_command (above) for all field descriptions.
    """

    rc, output = openbmctool_execute_command(command_string,
                                             *args,
                                             **kwargs)
    json_object = utils.to_json_ordered(output)
    if json_object['status'] != "ok":
        err_msg = "Error found in JSON data returned by the openbmctool.py "
        err_msg += "command. Expected a 'status' field value of \"ok\":\n"
        err_msg += gp.sprint_var(json_object, 1)
        BuiltIn().fail(gp.sprint_error(err_msg))

    return json_object['data']


def get_fru_status():
    r"""
    Get the fru status and return as a list of dictionaries.

    Example robot code:

    ${fru_status}=  Get Fru Status
    Rprint Vars  1  fru_status

    Example result (excerpt):

    fru_status:
      fru_status[0]:
        [component]:             cpu0
        [is_a]:                  Yes
        [fru]:                   Yes
        [present]:               Yes
        [functional]:            No
      fru_status[1]:
        [component]:             cpu0-core0
        [is_a]:                  No
        [fru]:                   Yes
        [present]:               Yes
        [functional]:            No
    ...
    """
    rc, output = openbmctool_execute_command("fru status", print_output=False,
                                             ignore_err=False)
    # Example value for output (partial):
    # Component     | Is a FRU  | Present  | Functional  | Has Logs
    # cpu0          | Yes       | Yes      | Yes         | No
    # cpu0-core0    | No        | Yes      | Yes         | No
    # ...

    # Replace spaces with underscores in field names (e.g. "Is a FRU" becomes
    # "Is_a_FRU").
    output = re.sub("([^ \\|])[ ]([^ ])", "\\1_\\2", output)
    output = re.sub("([^ \\|])[ ]([^ ])", "\\1_\\2", output)

    return vf.outbuf_to_report(output, field_delim="|")


def get_fru_print(parse_json=True):
    r"""
    Get the output of the fru print command and return it either as raw JSON
    data or as a list of dictionaries.

    Example robot code:

    ${fru_print}=  Get Fru Print  parse_json=${False}
    Log to Console  ${fru_print}

    Example result (excerpt):

    {
      "data": {
        "/xyz/openbmc_project/inventory/system": {
          "AssetTag": "",
          "BuildDate": "",
          "Cached": false,
          "FieldReplaceable": false,
          "Manufacturer": "",
          "Model": "xxxxxxxx",
          "PartNumber": "",
          "Present": true,
          "PrettyName": "",
          "SerialNumber": "13183FA"
        },
        "/xyz/openbmc_project/inventory/system/chassis": {
          "AirCooled": true,
          "WaterCooled": false
        },
    ...

    Example robot code:

    ${fru_print}=  Get Fru Print
    Rprint Vars  1  fru_print

    Example result (excerpt):

    fru_print:
      fru_print[0]:
        [data]:
          [/xyz/openbmc_project/inventory/system]:
            [AssetTag]:          <blank>
            [BuildDate]:         <blank>
            [Cached]:            False
            [FieldReplaceable]:  False
            [Manufacturer]:      <blank>
            [Model]:             xxxxxxxx
            [PartNumber]:        <blank>
            [Present]:           True
            [PrettyName]:        <blank>
            [SerialNumber]:      13183FA
          [/xyz/openbmc_project/inventory/system/chassis]:
            [AirCooled]:         True
            [WaterCooled]:       False
    ...

    Description of argument(s):
    parse_json                      Indicates that the raw JSON data should
                                    parsed into a list of dictionaries.
    """

    rc, output = openbmctool_execute_command("fru print", print_output=False,
                                             ignore_err=False)
    if parse_json:
        return gm.json_loads_multiple(output)
    else:
        return output


def get_fru_list(parse_json=True):
    r"""
    Get the output of the fru list command and return it either as raw JSON
    data or as a list of dictionaries.

    Example robot code:

    ${fru_list}=  Get Fru List  parse_json=${False}
    Log to Console  ${fru_list}

    Example result (excerpt):

    {
      "data": {
        "/xyz/openbmc_project/inventory/system": {
          "AssetTag": "",
          "BuildDate": "",
          "Cached": false,
          "FieldReplaceable": false,
          "Manufacturer": "",
          "Model": "xxxxxxxx",
          "PartNumber": "",
          "Present": true,
          "PrettyName": "",
          "SerialNumber": "13183FA"
        },
        "/xyz/openbmc_project/inventory/system/chassis": {
          "AirCooled": true,
          "WaterCooled": false
        },
    ...

    Example robot code:

    ${fru_list}=  Get Fru List
    Rprint Vars  1  fru_list

    Example result (excerpt):

    fru_list:
      fru_list[0]:
        [data]:
          [/xyz/openbmc_project/inventory/system]:
            [AssetTag]:          <blank>
            [BuildDate]:         <blank>
            [Cached]:            False
            [FieldReplaceable]:  False
            [Manufacturer]:      <blank>
            [Model]:             xxxxxxxx
            [PartNumber]:        <blank>
            [Present]:           True
            [PrettyName]:        <blank>
            [SerialNumber]:      13183FA
          [/xyz/openbmc_project/inventory/system/chassis]:
            [AirCooled]:         True
            [WaterCooled]:       False
    ...

    Description of argument(s):
    parse_json                      Indicates that the raw JSON data should
                                    parsed into a list of dictionaries.
    """

    rc, output = openbmctool_execute_command("fru list", print_output=False,
                                             ignore_err=False)
    if parse_json:
        return gm.json_loads_multiple(output)
    else:
        return output


def get_sensors_print():

    r"""
    Get the output of the sensors print command and return as a list of
    dictionaries.

    Example robot code:

    ${sensors_print}=  Get Sensors Print
    Rprint Vars  1  sensors_print

    Example result (excerpt):

    sensors_print:
      sensors_print[0]:
        [sensor]:                OCC0
        [type]:                  Discrete
        [units]:                 N/A
        [value]:                 Active
        [target]:                Active
      sensors_print[1]:
        [sensor]:                OCC1
        [type]:                  Discrete
        [units]:                 N/A
        [value]:                 Active
        [target]:                Active
    ...
    """
    rc, output = openbmctool_execute_command("sensors print",
                                             print_output=False,
                                             ignore_err=False)
    # Example value for output (partial):
    # sensor                 | type         | units     | value    | target
    # OCC0                   | Discrete     | N/A       | Active   | Active
    # OCC1                   | Discrete     | N/A       | Active   | Active

    return vf.outbuf_to_report(output, field_delim="|")


def get_sensors_list():

    r"""
    Get the output of the sensors list command and return as a list of
    dictionaries.

    Example robot code:

    ${sensors_list}=  Get Sensors List
    Rprint Vars  1  sensors_list

    Example result (excerpt):

    sensors_list:
      sensors_list[0]:
        [sensor]:                OCC0
        [type]:                  Discrete
        [units]:                 N/A
        [value]:                 Active
        [target]:                Active
      sensors_list[1]:
        [sensor]:                OCC1
        [type]:                  Discrete
        [units]:                 N/A
        [value]:                 Active
        [target]:                Active
    ...
    """
    rc, output = openbmctool_execute_command("sensors list",
                                             print_output=False,
                                             ignore_err=False)
    # Example value for output (partial):
    # sensor                 | type         | units     | value    | target
    # OCC0                   | Discrete     | N/A       | Active   | Active
    # OCC1                   | Discrete     | N/A       | Active   | Active

    return vf.outbuf_to_report(output, field_delim="|")


def get_openbmctool_version():
    r"""
    Get the openbmctool.py version and return it.

    Example robot code:
    ${openbmctool_version}=  Get Openbmctool Version
    Rprint Vars  openbmctool_version

    Example result (excerpt):
    openbmctool_version:         1.06
    """
    rc, output = openbmctool_execute_command("-V | cut -f 2 -d ' '",
                                             print_output=False,
                                             ignore_err=False)
    return output


def service_data_files():
    r"""
    Return a complete list of file names that are expected to be created by
    the collect_service_data command.
    """

    return\
        [
            "inventory.txt",
            "sensorReadings.txt",
            "ledStatus.txt",
            "SELshortlist.txt",
            "parsedSELs.txt",
            "bmcFullRaw.txt"
        ]


def collect_service_data(verify=False):
    r"""
    Run the collect_service_data command and return a list of files generated
    by the command.

    Description of argument(s):
    verify                          If set, verify that all files which can be
                                    created by collect_service_data did, in
                                    fact, get created.
    """

    # Route the output of collect_service_data to a file for easier parsing.
    temp = tempfile.NamedTemporaryFile()
    temp_file_path = temp.name
    openbmctool_execute_command("collect_service_data > " + temp_file_path,
                                ignore_err=False)
    # Isolate the file paths in the collect_service_data output.  We're
    # looking for output lines like this from which to extract the file paths:
    # Inventory collected and stored in /tmp/dummy--2018-09-26_17.59.18/inventory.txt
    rc, file_paths = gc.shell_cmd("egrep 'collected and' " + temp_file_path
                                  # + " | sed -re 's#.*/tmp#/tmp#g'",
                                  + " | sed -re 's#[^/]*/#/#'",
                                  quiet=1, print_output=0)
    # Example file_paths value:
    # /tmp/dummy--2018-09-26_17.59.18/inventory.txt
    # /tmp/dummy--2018-09-26_17.59.18/sensorReadings.txt
    # etc.
    # Convert from output to list.
    collect_service_data_file_paths =\
        list(filter(None, file_paths.split("\n")))
    if int(verify):
        # Create a list of files by stripping the dir names from the elements
        # of collect_service_data_file_paths.
        files_obtained = [re.sub(r".*/", "", file_path)
                          for file_path in collect_service_data_file_paths]
        files_expected = service_data_files()
        files_missing = list(set(files_expected) - set(files_obtained))
        if len(files_missing) > 0:
            gp.printn("collect_service_data output:\n"
                      + gm.file_to_str(temp_file_path))
            err_msg = "The following files are missing from the list of files"
            err_msg += " returned by collect_service_data:\n"
            err_msg += gp.sprint_var(files_missing)
            err_msg += gp.sprint_var(collect_service_data_file_paths)
            BuiltIn().fail(gp.sprint_error(err_msg))

    return collect_service_data_file_paths


def health_check_fields():
    r"""
    Return a complete list of field names returned by the health_check command.
    """

    return\
        [
            "hardware_status",
            "performance"
        ]


def get_health_check(verify=False):
    r"""
    Get the health_check information and return as a dictionary.

    Example robot code:

    ${health_check}=  Get Health Check
    Rpvars  1  health_check

    Example result:

    health_check:
      [hardware_status]:         OK
      [performance]:             OK

    Description of argument(s):
    verify                          If set, verify that all all expected
                                    field_names are generated by the
                                    health_check command.
    """

    rc, output = openbmctool_execute_command("health_check",
                                             print_output=False,
                                             ignore_err=False)
    health_check = vf.key_value_outbuf_to_dict(output, delim=":")
    if int(verify):
        err_msg = gv.svalid_dict(health_check, health_check_fields())
        if err_msg != "":
            BuiltIn().fail(gp.sprint_error(err_msg))

    return health_check


def remote_logging_view_fields():
    r"""
    Return a complete list of field names returned by the logging
    remote_logging view command.
    """

    return\
        [
            "Address",
            "AddressFamily",
            "Port"
        ]


def get_remote_logging_view(verify=False):
    r"""
    Get the remote_logging view information and return as a dictionary.

    Example robot code:

    ${remote_logging_view}=  Get Remote Logging View
    Rpvars  1  remote_logging_view

    Example result:

    remote_logging_view:
      [Address]:                 <blank>
      [AddressFamily]:           xyz.openbmc_project.Network.Client.IPProtocol.IPv4
      [Port]:                    0

    Description of argument(s):
    verify                          If set, verify that all all expected field
                                    names are generated by the 'logging
                                    remote_logging view' command.
    """

    remote_logging_view =\
        openbmctool_execute_command_json("logging remote_logging view",
                                         print_output=False,
                                         ignore_err=False)

    if int(verify):
        err_msg = gv.svalid_dict(remote_logging_view,
                                 remote_logging_view_fields())
        if err_msg != "":
            BuiltIn().fail(gp.sprint_error(err_msg))

    return remote_logging_view
