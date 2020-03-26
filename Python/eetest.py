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

# MPI
from mpi4py import MPI

# gethostname
import socket

# argv
import sys

# sleep
from time import sleep

# ----------------------------------------------------------------------------------

from eeprobe import *

# ----------------------------------------------------------------------------------

EEPROBE_TAG = 0

EEPROBE_RANK_SEND = 0

EEPROBE_RANK_RECV = 1

# ----------------------------------------------------------------------------------

EEPROBE_NB_ITER = 24

EEPROBE_INTER_MSG_SLEEP_S = 1
EEPROBE_INTER_MSG_SLEEP_NS = 0

EEPROBE_COUNT = 1024

# ----------------------------------------------------------------------------------

def EEPROBE_sendRecv(enable):
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank ()

    mpbuffer = "\0" * EEPROBE_COUNT

    start_time = EEPROBE_getTime()

    print(str(EEPROBE_getTime() - start_time) + " rank " + str(rank) + " start")

    if (rank == EEPROBE_RANK_SEND):
        for i in range(0, EEPROBE_NB_ITER):
            comm.send(mpbuffer, dest = EEPROBE_RANK_RECV, tag = EEPROBE_TAG)
            print(str(EEPROBE_getTime() - start_time) + " rank " + str(rank) + " send " + str(i))
            sleep(EEPROBE_INTER_MSG_SLEEP_S + (EEPROBE_INTER_MSG_SLEEP_NS / 1000000000))

    else:

        eep = EEProbe()

        if enable == EEPROBE_Enable.EEPROBE_ENABLE:
            print(str(EEPROBE_getTime() - start_time) + " rank " + str(rank) + " min_yield_time " + str(eep.min_yield_time) + " max_yield_time " + str(eep.max_yield_time) + " inc_yield_time " + str(eep.inc_yield_time))
        
        for i in range(0, EEPROBE_NB_ITER):
            eep.probe(comm, source = EEPROBE_RANK_SEND, tag = EEPROBE_TAG, enable = enable)
            mpbuffer = comm.recv(source = EEPROBE_RANK_SEND, tag = EEPROBE_TAG)
            if enable == EEPROBE_Enable.EEPROBE_ENABLE:
                print(str(EEPROBE_getTime() - start_time) + " rank " + str(rank) + " recv " + str(i) + " last_yield_time " + str(eep.last_yield_time) + " total_sleep_time " + str(eep.total_sleep_time))
            else:
                print(str(EEPROBE_getTime() - start_time) + " rank " + str(rank) + " recv " + str(i))

# ----------------------------------------------------------------------------------

def main (argv):
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank ()
    nr = comm.Get_size ()
    hostname = socket.gethostname()
    enable = EEPROBE_Enable.EEPROBE_ENABLE

    if (len(argv) > 1 and argv[1] == "disable"):
        enable = EEPROBE_Enable.EEPROBE_DISABLE
    
    print("rank " + str( rank ) + " on " + hostname)

    if (nr == 2):
        EEPROBE_sendRecv(enable)
    else:
        print("Warning: MPI task nr is " + str(nr) + ". Expected 2. Usage:\nmpirun -np 2 python3 " + argv[0] + "\nmpirun -np 2 python3 " + argv[0] + " disable")




if __name__  == "__main__":
    main(sys.argv)


# ----------------------------------------------------------------------------------
