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
  an open-source implementation of this strategy.

## Quick start

The `eetest` program is a basic send-receive MPI application in which
a process periodically sends messages to a receiver. This receiver
uses the `EEProbe` implementation of the micro-sleep strategy to save
CPU time while waiting for messages. The following instructions show
how to build and test this program.

Open a terminal:
```
cd C/
make
```

Open a second terminal and run the top command to monitor the CPU load:
```
top
```

On the first terminal, run the `eetest` program (`EEProbe disabled`):
```
mpirun --np 2 ./eetest disable
```

The top command should show that the `eetest` command takes 100% of the CPU.

On the first terminal, run the `eetest` program (`EEProbe enabled`):
```
mpirun --np 2 ./eetest
```

The `eetest` command should now take only a few percent of the CPU.

