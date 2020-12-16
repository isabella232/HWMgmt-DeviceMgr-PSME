"""
 NOTE: most of the code is original to dmidecode (https://pypi.python.org/pypi/dmidecode),
 under MIT license.

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

TYPE = { 17: 'memory_device' }

def _parse_dmi(content, type):
    info = []
    lines = iter(content.strip().splitlines())
    while True:
        try:
            line = lines.next()
        except StopIteration:
            break

        if line.startswith('Handle 0x'):
            typ = int(line.split(',', 2)[1].strip()[len('DMI type'):])
            if typ == type:
                info.append((TYPE[typ], _parse_handle_section(lines)))
    return info


def _parse_handle_section(lines):
    """
    Parse a section of dmidecode output

    * 1st line contains address, type and size
    * 2nd line is title
    * line started with one tab is one option and its value
    * line started with two tabs is a member of list
    """
    data = {
        '_title': lines.next().rstrip(),
        }

    for line in lines:
        line = line.rstrip()
        if line.startswith('\t\t'):
            data[k].append(line.lstrip())
        elif line.startswith('\t'):
            k, v = [i.strip() for i in line.lstrip().split(':', 1)]
            if v:
                data[k] = v
            else:
                data[k] = []
        else:
            break

    return data

def _profile(type):
    dmi = ShellCommand("dmidecode")
    dmi.execute()
    info = _parse_dmi(dmi.get_stdout(), type)
    return info

def get_memory_info(serialNumber):
    info = _profile(17)
    for slot in info:
        if slot[1]['Serial Number'] == serialNumber:
            return slot[1]
    return info
