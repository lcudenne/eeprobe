#!/usr/bin/env python3

    # EEProbe: Energy Efficient Probe for MPI
    # Copyright (C) 2020 Loic Cudennec

    # This program is free software: you can redistribute it and/or modify
    # it under the terms of the GNU General Public License as published by
    # the Free Software Foundation, either version 3 of the License, or
    # any later version.

    # This program is distributed in the hope that it will be useful,
    # but WITHOUT ANY WARRANTY; without even the implied warranty of
    # MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    # GNU General Public License for more details.

    # You should have received a copy of the GNU General Public License
    # along with this program.  If not, see <https://www.gnu.org/licenses/>.


# ----------------------------------------------------------------------------------

# Enum
from enum import Enum

# MPI
from mpi4py import MPI

# monotonic
from time import monotonic

# sleep
from time import sleep

# ----------------------------------------------------------------------------------

class EEPROBE_Enable(Enum):
    EEPROBE_ENABLE = 0
    EEPROBE_DISABLE = 1

# ----------------------------------------------------------------------------------

def EEPROBE_getTime():
    return int(round(monotonic() * 1E6))


# ----------------------------------------------------------------------------------

class EEProbe:

    def __init__(self):
        self.last_yield_time = 0
        self.max_yield_time = 1000
        self.min_yield_time = 0
        self.inc_yield_time = 1
        self.total_sleep_time = 0

    
    def probe(self, comm, source, tag, status=None,
              enable=EEPROBE_Enable.EEPROBE_ENABLE):

        current_yield_duration = self.min_yield_time

        if enable == EEPROBE_Enable.EEPROBE_ENABLE:

            while not comm.Iprobe(source = source, tag = tag, status = status):

                start = EEPROBE_getTime()
                
                sleep(current_yield_duration / 1000000000)

                self.total_sleep_time = self.total_sleep_time + (EEPROBE_getTime() - start)
                
                current_yield_duration = current_yield_duration + self.inc_yield_time
                if current_yield_duration > self.max_yield_time:
                    current_yield_duration = self.max_yield_time

            self.last_yield_time = current_yield_duration

        else:

            comm.probe(source, tag)


# ----------------------------------------------------------------------------------
