
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

      MODULE EEProbe
      PUBLIC EEPROBE_getTime
      CONTAINS
      
      integer FUNCTION EEPROBE_getTime()

        real :: t
        integer :: it
      
        call CPU_TIME(t)

        t = t * 1000000
        it = INT(t)
        
        EEPROBE_getTime = it
        return
      END FUNCTION EEPROBE_getTime
      
      END MODULE
      
c$$$ ----------------------------------------------------------------------------------
