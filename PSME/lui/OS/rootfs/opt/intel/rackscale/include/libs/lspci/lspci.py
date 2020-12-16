"""
 * @section LICENSE
 *
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
 * @section DESCRIPTION
"""

from include.common.shell_command import ShellCommand
from include.common.globals import *

LSUPCI_VENDOR_DEV_SECTION = 3

def _parse_lspci(content):
    list = []
    lines = iter(content.strip().splitlines())
    while True:
        try:
            line = lines.next()
        except StopIteration:
            break
        info = line.split(' ')[LSUPCI_VENDOR_DEV_SECTION].split(":")
        list.append({DEV_VENDOR_ID : info[DEVICE_VENDOR_ID_SECTION], DEV_DEVICE_ID : info[DEVICE_DEV_ID_SECTION]})
    return list

def get_lspci_info():
    lspci = ShellCommand("lspci -n")
    lspci.execute()
    info = _parse_lspci(lspci.get_stdout())
    return info
