# EEProbe: Energy Efficient Probe for MPI




    EEProbe: Energy Efficient Probe for MPI
    Copyright (C) 2020 Loïc Cudennec

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.


## Summary

  Autonomous vehicles, smart manufacturing, heterogeneous systems and
  new high-performance embedded computing (HPEC) applications can
  benefit from the reuse of code coming from the high-performance
  computing world. However, unlike for HPC, energy efficiency is
  critical in embedded systems, especially when running on battery
  power. Code base from HPC mostly relies on the MPI message passing
  runtime to deal with distributed systems. MPI has been designed
  primarily for performance and not for energy efficiency. One
  drawback is the way messages are received, in an energy-consuming
  busy-wait fashion. In this work we study a simple approach in which
  receiving processes are put to sleep instead of constantly
  polling. We implement this strategy at the user level to be
  transparent to the MPI runtime and the application. **EEProbe** is
  an open-source implementation of this strategy for C MPI runtimes
  and the mpi4py Python runtime.

## Quick start

The `eetest` program is a basic send-receive MPI application in which
a process periodically sends messages to a receiver. This receiver
uses the `EEProbe` implementation of the micro-sleep strategy to save
CPU time while waiting for messages. The following instructions show
how to build and test this program.

Open a terminal:
```shell
cd C/
make
```

Open a second terminal and run the `top` command to monitor the CPU load:
```shell
top
```

On the first terminal, run the `eetest` program (`EEProbe disabled`):
```shell
mpirun -np 2 ./eetest disable
```

The `top` command should show that the `eetest` command takes 100% of the CPU.

On the first terminal, run the `eetest` program (`EEProbe enabled`):
```shell
mpirun -np 2 ./eetest
```

The `eetest` command should now take only a few percent of the CPU.

The same test using the Python implementation:
```shell
cd Python
mpirun -np 2 python3 ./eetest.py disable
mpirun -np 2 python3 ./eetest.py
```


## Replace the default MPI `Probe` function with `EEProbe` in applications

`EEProbe` has been designed to transparently replace the default MPI
`Probe` function in the user code. The main function is
`EEPROBE_Probe`, which takes the same parameters as `Probe` and a
specific parameter to enable or disable the micro-sleeping
mechanism. This function is synchronous and returns when a matching
message is being delivered by the MPI runtime.

```C
int EEPROBE_Probe(int source, int tag, MPI_Comm comm, MPI_Status * status);
```

The following code snippet shows how to use the `EEPROBE_Probe`
function before receiving a message.

C
```C
#include "eeprobe.h"

void
receiveProcess(char * buffer, int count, int remote_rank) {

 int errno = MPI_SUCCESS;

 MPI_Status status;

 assert(buffer);
 assert(count > 0);

 EEPROBE_Probe(remote_rank, 0, MPI_COMM_WORLD, &status);

 errno = MPI_Recv(buffer, count, MPI_CHAR, remote_rank,
        	  0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

 assert(errno == MPI_SUCCESS);

}
```
Python
```Python
from eeprobe import *

def receiveProcess(mpbuffer, remote_rank):
    comm = MPI.COMM_WORLD
    eep = EEProbe()
    eep.probe(comm, source = remote_rank, tag = 0)
    mpbuffer = comm.recv(source = remote_rank, tag = 0)
```


## Going further

The `EEProbe` function works as follow: 1) a call to the non-blocking
MPI function `Iprobe` to check if an incoming message is available and
2) if no message is available, a call to the system sleep function
`clock_nanosleep`. The sleep duration starts at a minimum value
`min_yield_time` and is incremented after each unsuccessful `Iprobe`
by a given step `inc_yield_time` up to a maximum value
`max_yield_time`. When a message is available the `EEProbe` function
returns and the sleep duration is resett to the minimum value. This
way, the system is able to handle communication bursts as well as long
idle times. The following pseudo-code is close to the actual
implementation.

```C
void EEProbe() {

 int flag = 0;
 unsigned long yield_time = 0;
 long yield_time = min_yield_time;

 while (flag == 0) {

  Iprobe(&flag);

  if (flag == 0) {
   clock_nanosleep(yield_time);
   yield_time += inc_yield_time;
   if (yield_time > max_yield_time) {
    yield_time = max_yield_time;
   }
  } else {
   last_yield_time = yield_time;
  }
  
 }
 
}
```

The default values for `EEProbe` are:

```C
static long _EEPROBE_MAX_YIELD_TIME = 1000;

static long _EEPROBE_MIN_YIELD_TIME = 0;

static long _EEPROBE_INC_YIELD_TIME = 1;
```

The following functions are used to read and modify these values:

```C
  /**
   * Set the smallest yield time.
   * @param min_yield_time In nanoseconds, must be set within range [0;1000000000[
   */
void EEPROBE_setMinYieldTime(long min_yield_time);

  /**
   * Set the maximum yield time.
   * @param max_yield_time In nanoseconds, must be set within range ]0;1000000000[
   */
void EEPROBE_setMaxYieldTime(long max_yield_time);

  /**
   * Set the incremental step time.
   * @param inc_yield_time In nanoseconds, must be set within range ]0;1000000000[
   */
void EEPROBE_setIncYieldTime(long inc_yield_time);

  /**
   * Returns the current minimum yield time.
   * @return Current minimum yield time in nanoseconds.
   */
long EEPROBE_getMinYieldTime();

  /**
   * Returns the current maximum yield time.
   * @return Current maximum yield time in nanoseconds.
   */
long EEPROBE_getMaxYieldTime();

  /**
   * Returns the current incremental step yield time.
   * @return Current incremental step yield time in nanoseconds.
   */
long EEPROBE_getIncYieldTime();

  /**
   * Returns the last yield time applied before receiving a message.
   * @return Last yield time in nanoseconds.
   */
long EEPROBE_getLastYieldTime();

  /**
   * Returns the total sleep duration since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTime();
```
