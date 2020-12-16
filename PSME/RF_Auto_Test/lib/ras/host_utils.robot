*** Settings ***
Documentation       Utility for error injection scenarios through HOST & BMC.
Resource            ../../lib/rest_client.robot
Resource            ../../lib/utils.robot
Variables           ../../lib/ras/variables.py
Library             ../../lib/bmc_ssh_utils.py
Library             ../../lib/gen_print.py
Library             ../../lib/gen_robot_print.py

Library             OperatingSystem

*** Keywords ***

Getscom Operations On OS
    [Documentation]  Executes getscom command on OS with the given
    ...              input command.
    [Arguments]      ${input_cmd}
    # Description of arguments:
    # input_cmd      -l|--list-chips
    #                -c|--chip <chip-id> <addr>

    ${output}  ${stderr}  ${rc}=  OS Execute Command  getscom ${input_cmd}
    [Return]  ${output}

Gard Operations On OS
    [Documentation]  Executes opal-gard command on OS with the given
    ...              input command.
    [Arguments]      ${input_cmd}
    # Description of arguments:
    # input_cmd      list/clear all/show <gard_record_id>

    ${output}  ${stderr}  ${rc}=  OS Execute Command  opal-gard ${input_cmd}
    [Return]  ${output}

Putscom Operations On OS
    [Documentation]  Executes putscom command on OS with the given
    ...              input arguments.
    [Arguments]      ${proc_chip_id}  ${fru}  ${address}
    # Description of arguments:
    # proc_chip_id        Processor ID (e.g '0', '8').
    # fru            FRU value (e.g. 2011400).
    # address        Chip address (e.g 4000000000000000).

    ${cmd}=  Catenate  putscom -c 0x${proc_chip_id} 0x${fru} 0x${address}
    Start Command  ${cmd}

Get ProcChipId From OS
    [Documentation]  Get processor chip ID values based on the input.
    [Arguments]      ${chip_type}  ${master_proc_chip}
    # Description of arguments:
    # chip_type         The chip type (Processor/Centaur).
    # master_proc_chip  Processor chip type ('True' or 'False').

    ${cmd}=  Catenate  -l | grep -i ${chip_type} | cut -c1-8
    ${proc_chip_id}=  Getscom Operations On OS  ${cmd}
    # Example output:
    # getscom -l | grep processor | cut -c1-8
    # 00000008     - False
    # 00000000     - True

    ${proc_ids}=  Split String  ${proc_chip_id}
    ${proc_id}=  Run Keyword If  '${master_proc_chip}' == 'True'
    \  ...  Get From List  ${proc_ids}  1
    \  ...  ELSE  Get From List  ${proc_ids}  0

    # Example output:
    # 00000008
    [Return]  ${proc_id}

Get Core IDs From OS
    [Documentation]  Get Core IDs corresponding to the input processor chip ID.
    [Arguments]      ${proc_chip_id}
    # Description of argument(s):
    # proc_chip_id        Processor ID (e.g '0', '8').

    ${cmd}=  Catenate  set -o pipefail ; ${probe_cpu_file_path}
    ...    | grep -i 'CHIP ID: ${proc_chip_id}' | cut -c21-22
    ${output}  ${stderr}  ${rc}=  OS Execute Command  ${cmd}
    ${core_ids}=  Split String  ${output}
    # Example output:
    # ['2', '3', '4', '5', '6']
    [Return]  ${core_ids}

FIR Address Translation Through HOST
    [Documentation]  Do FIR address translation through host for given FIR,
    ...              core value & target type.
    [Arguments]  ${fir}  ${core_id}  ${target_type}
    # Description of argument(s):
    # fir          FIR (Fault isolation register) value (e.g. 2011400).
    # core_id      Core ID (e.g. 9).
    # target_type  Target type (e.g. 'EQ', 'EX', 'C').

    ${cmd}=  Catenate  set -o pipefail ; ${addr_translation_file_path} ${fir}
    ...  ${core_id} | grep -i ${target_type}
    ${output}  ${stderr}  ${rc}=  OS Execute Command  ${cmd}
    ${translated_addr}=  Split String  ${output}  :${SPACE}0x
    # Example output:
    # 0x10010c00
    [Return]  ${translated_addr[1]}

Inject Error Through HOST
    [Documentation]  Inject checkstop on multiple targets like
    ...              CPU/CME/OCC/NPU/CAPP/MCA etc through HOST.
    ...              Test sequence:
    ...              1. Boot To HOST
    ...              2. Clear any existing gard records
    ...              3. Inject Error on processor/centaur
    [Arguments]      ${fir}  ${chip_address}  ${threshold_limit}
    ...  ${master_proc_chip}=True
    # Description of argument(s):
    # fir                 FIR (Fault isolation register) value (e.g. 2011400).
    # chip_address        chip address (e.g 2000000000000000).
    # threshold_limit     Threshold limit (e.g 1, 5, 32).
    # master_proc_chip    Processor chip type (True' or 'False').

    Delete Error Logs
    Login To OS Host
    Set Auto Reboot  1
    Gard Operations On OS  clear all

    # Fetch processor chip IDs.
    ${proc_chip_id}=  Get ProcChipId From OS  Processor  ${master_proc_chip}

    ${threshold_limit}=  Convert To Integer  ${threshold_limit}
    :FOR  ${count}  IN RANGE  ${threshold_limit}
    \  Run Keyword  Putscom Operations On OS  ${proc_chip_id}  ${fir}
    ...  ${chip_address}
    # Adding delay after each error injection.
    \  Sleep  10s
    # Adding delay to get error log after error injection.
    Sleep  120s

Code Update Unrecoverable Error Inject
    [Documentation]  Inject UE MCACALFIR checkstop on processor through
    ...   host during PNOR code update.

    Inject Error Through HOST  05010800  4000000000000000  1

Disable CPU States Through HOST
    [Documentation]  Disable CPU states through host.

    # Fetch number of states present for cpu0.
    ${cmd}=  Catenate  ls /sys/devices/system/cpu/cpu0/cpuidle|grep state|wc -l
    ${output}  ${stderr}  ${rc}=  OS Execute Command  ${cmd}
    ${no_of_states}=  Convert To Integer  ${output}

    # Disable state for all cpus.
    :FOR  ${count}  IN RANGE  ${no_of_states}
    \  ${cmd}=  Catenate  SEPARATOR=  for file_path in /sys/devices/system/cpu/
     ...  cpu*/cpuidle/state${i}/disable; do echo 1 > $file_path; done
    \  ${output}  ${stderr}  ${rc}=  Run Keyword  OS Execute Command  ${cmd}

Is Opal-PRD Service Enabled
    [Documentation]  Check if Opal-PRD service is running & return either
    ...              'enabled' or 'disabled'.

    ${cmd}=  Catenate  systemctl list-unit-files | grep opal-prd
    ${output}  ${stderr}  ${rc}=  OS Execute Command  ${cmd}
    ${opal_prd_state}=  Split String  ${output}

    # Example output from prior command:
    # opal-prd.service enabled
    [Return]  ${opal_prd_state[1]}

Enable Opal-PRD Service On HOST
    [Documentation]  Enable Opal-PRD service on host.

    OS Execute Command  service opal-prd start
    ${opal_prd_state}=  Is Opal-PRD Service Enabled
    Should Contain  ${opal_prd_state}  enabled

BMC Putscom
    [Documentation]  Executes putscom command through BMC.

    [Arguments]      ${proc_chip_id}  ${fru}  ${chip_address}

    # Description of argument(s):
    # proc_chip_id        Processor ID (e.g '0', '8').
    # fru                 FRU (field replaceable unit) (e.g. '2011400').
    # chip_address        Chip address (e.g. '4000000000000000').

    ${cmd}=  Catenate  pdbg -d p9w -p${proc_chip_id} putscom 0x${fru} 0x${chip_address}

    BMC Execute Command  ${cmd}

Inject Error Through BMC
    [Documentation]  Inject checkstop on multiple targets like
    ...              CPU/CME/OCC/NPU/CAPP/MCA etc through BMC.
    ...              Test sequence:
    ...              1. Boot To HOST.
    ...              2. Clear any existing gard records.
    ...              3. Inject Error on processor/centaur.
    [Arguments]      ${fir}  ${chip_address}  ${threshold_limit}
    ...  ${master_proc_chip}=True
    # Description of argument(s):
    # fir                 FIR (Fault isolation register) value (e.g. '2011400').
    # chip_address        Chip address (e.g. '2000000000000000').
    # threshold_limit     Recoverable error threshold limit (e.g. '1', '5', '32').

    Delete Error Logs
    Login To OS Host
    Set Auto Reboot  1

    Gard Operations On OS  clear all

    ${threshold_limit}=  Convert To Integer  ${threshold_limit}
    :FOR  ${count}  IN RANGE  ${threshold_limit}
    \  BMC Putscom  0  ${fir}
    ...  ${chip_address}
    # Adding delay after each error injection.
    \  Sleep  10s
    # Adding delay to get error log after error injection.
    Sleep  120s
