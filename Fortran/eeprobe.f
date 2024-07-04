
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

      MODULE EEPROBE_Parameters
      implicit none
      
      integer*8 :: EEPROBE_LAST_YIELD_TIME = 0

      integer*8 :: EEPROBE_MAX_YIELD_TIME = 0

      integer*8 :: EEPROBE_MIN_YIELD_TIME = 0

      integer*8 :: EEPROBE_INC_YIELD_TIME = 0

      integer*8 :: EEPROBE_TOTAL_SLEEP_TIME = 0
      
      END MODULE EEPROBE_Parameters

      
c$$$ ----------------------------------------------------------------------------------

      MODULE EEPROBE_Sleep
      
      use iso_c_binding
      implicit none 
      
      interface

         SUBROUTINE usleep(USECONDS) bind(C) 
         use iso_c_binding
         implicit none
         integer(c_int64_t), value :: USECONDS 
         END SUBROUTINE usleep
      
      END interface
      
      END MODULE EEPROBE_Sleep

c$$$ ----------------------------------------------------------------------------------
      
      MODULE EEProbe
      PUBLIC EEPROBE_getTime
      PUBLIC EEPROBE_Probe
      CONTAINS

c$$$ ----------------------------------------------------------------------------------



      
      integer FUNCTION EEPROBE_getTime()

      real :: t
      integer :: it
      
      call CPU_TIME(t)

      t = t * 1000000
      it = INT(t)
        
      EEPROBE_getTime = it
      return

      END FUNCTION EEPROBE_getTime
      
c$$$ ----------------------------------------------------------------------------------

      SUBROUTINE EEPROBE_USleep(USECONDS)

      use EEPROBE_Sleep
      use iso_c_binding
      
      implicit none

      integer(c_int64_t), value :: USECONDS
      
      CALL usleep(USECONDS)
      
      END SUBROUTINE EEPROBE_USleep
      
c$$$ ----------------------------------------------------------------------------------

      SUBROUTINE EEPROBE_Probe(SOURCE, TAG, COMM, STATUS, IERROR,
     &     ENABLE)

      use EEPROBE_Parameters
      
      integer SOURCE, TAG, COMM, STATUS, IERROR
      logical,optional ENABLE
      
      logical :: FLAG = .FALSE.
      
      integer*8 :: current_yield_duration = 0
      integer :: start = 0

      logical :: EENABLE = .TRUE.
      if (present(ENABLE)) then
         EENABLE = ENABLE
      end if

      if (EENABLE) then
         
 10      CALL MPI_IPROBE(SOURCE, TAG, COMM, FLAG, STATUS, IERROR)
         if ((.NOT. FLAG) .AND. (IERROR == MPI_SUCCESS)) then

            start = EEPROBE_getTime()
            
            CALL EEPROBE_USleep(current_yield_duration)

            EEPROBE_TOTAL_SLEEP_TIME = EEPROBE_TOTAL_SLEEP_TIME
     &           + EEPROBE_getTime() - start

            current_yield_duration = current_yield_duration
     &           + EEPROBE_INC_YIELD_TIME
            if (current_yield_duration > EEPROBE_MAX_YIELD_TIME) then
               current_yield_duration = EEPROBE_MAX_YIELD_TIME
            end if
            
            
            goto 10
         end if

         EEPROBE_LAST_YIELD_TIME = current_yield_duration
         
      else

         call MPI_PROBE(SOURCE, TAG, COMM, STATUS, IERROR)

      end if
      

      END SUBROUTINE EEPROBE_Probe

c$$$ ----------------------------------------------------------------------------------

      END MODULE
