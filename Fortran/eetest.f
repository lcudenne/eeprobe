
c$$$    EEProbe: Energy Efficient Probe for MPI
c$$$    Copyright (C) 2020 Loïc Cudennec
c$$$
c$$$    This program is free software: you can redistribute it and/or modify
c$$$    it under the terms of the GNU General Public License as published by
c$$$    the Free Software Foundation, either version 3 of the License, or
c$$$    any later version.
c$$$
c$$$    This program is distributed in the hope that it will be useful,
c$$$    but WITHOUT ANY WARRANTY; without even the implied warranty of
c$$$    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
c$$$    GNU General Public License for more details.
c$$$
c$$$    You should have received a copy of the GNU General Public License
c$$$    along with this program.  If not, see <https://www.gnu.org/licenses/>.


c$$$ ----------------------------------------------------------------------------------

      SUBROUTINE EEPROBE_SendRecv(ENABLE)

      use mpi
      use EEProbe

      logical ENABLE
      
      integer :: EEPROBE_COUNT = 1          
      integer :: EEPROBE_TAG = 0
      integer :: EEPROBE_RANK_SEND = 0
      integer :: EEPROBE_RANK_RECV = 1
      integer :: EEPROBE_NB_ITER = 24
      integer :: EEPROBE_INTER_MSG_SLEEP_S = 1
      
      integer rank, status
      integer :: er = MPI_SUCCESS
      integer start_time, t
      integer :: i = 0
      integer :: buffer = 42
        
      call MPI_COMM_RANK(MPI_COMM_WORLD, rank, er)

      start_time = EEPROBE_getTime()

      t = EEPROBE_getTime() - start_time
      print *, t, "rank", rank, "start"
        
      if (rank == EEPROBE_RANK_SEND) then

 10      if (i <EEPROBE_NB_ITER) then
            call MPI_SEND(buffer, EEPROBE_COUNT, MPI_INTEGER,
     &           EEPROBE_RANK_RECV, EEPROBE_TAG,
     &           MPI_COMM_WORLD, er)
            t = EEPROBE_getTime() - start_time
            print *, t, "rank", rank, "send", i
            i = i + 1
            call sleep(EEPROBE_INTER_MSG_SLEEP_S)
            goto 10
         end if
         
      else
         
 20      if (i <EEPROBE_NB_ITER) then
            call EEPROBE_Probe(EEPROBE_RANK_SEND, EEPROBE_TAG,
     &           MPI_COMM_WORLD, status, er, ENABLE)
            
            call MPI_RECV(buffer, EEPROBE_COUNT, MPI_INTEGER,
     &           EEPROBE_RANK_SEND, EEPROBE_TAG,
     &           MPI_COMM_WORLD, MPI_STATUS_IGNORE, er)
            
            t = EEPROBE_getTime() - start_time
            print *, t, "rank", rank, "recv", i
            i = i + 1
            goto 20
         end if
           
      end if
      
      END SUBROUTINE EEPROBE_SendRecv

      
c$$$ ----------------------------------------------------------------------------------

      PROGRAM eetest

      use mpi
      
      logical :: enable = .TRUE.
      
      CHARACTER(len=32) arg
      CHARACTER(len=32) bin_name
      integer rank, nr, er
      integer*4 status
      character*16 hostname
      
      call getarg(0, bin_name)
      call getarg(1, arg)

      if (arg == "disable") then
         enable = .FALSE.
      end if
        
      call MPI_INIT(er)
      call MPI_COMM_SIZE(MPI_COMM_WORLD, nr, er)
      call MPI_COMM_RANK(MPI_COMM_WORLD, rank, er)
      
      status = hostnm(hostname)

      print "(AI1AA)", "rank ", rank, " on ", hostname

      if (nr == 2) then

         call EEPROBE_SendRecv(enable)
         
      else

         print "(AI1A)", "Warning: MPI task nr is ", nr, ". Expected 2"
         print "(AA)", "mpirun -np 2 ", bin_name

      end if
          
        
      call MPI_FINALIZE(er)
        
      END PROGRAM eetest

c$$$ ----------------------------------------------------------------------------------
