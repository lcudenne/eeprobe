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


# argv
import sys

# ArgumentParser
import argparse

# glob
import glob


# ----------------------------------------------------------------------------------

dic_c = {'MPI_Probe': 'EEPROBE_Probe',
         'MPI_Wait' : 'EEPROBE_Wait',
         'MPI_Recv' : 'EEPROBE_Recv',
         'MPI_Reduce' : 'EEPROBE_Reduce',
         'MPI_Allreduce' : 'EEPROBE_Allreduce',
         'MPI_Alltoall' : 'EEPROBE_Alltoall',
         'MPI_Alltoallv' : 'EEPROBE_Alltoallv',
         'MPI_Alltoallw' : 'EEPROBE_Alltoallw',
         'MPI_Bcast' : 'EEPROBE_Bcast'}

# ----------------------------------------------------------------------------------

def processFiles(files, filetype, dic):
    for fpath in files:
        filedata=None
        with open(fpath, 'r') as fr:
            filedata = fr.read()
        if filedata:
            count = 0
            for mpi_key in dic:
                count += filedata.count(mpi_key)
                filedata = filedata.replace(mpi_key, dic[mpi_key])
            if count > 0:
                with open(fpath, 'w') as fw:
                    fw.write("#include \"C/eeprobe.h\"\n" + filedata)
                    print('file type ' + filetype + ' path ' + fpath + ' replaced ' + str(count) + ' MPI operation(s)')


# ----------------------------------------------------------------------------------


def walkFileSystem(path):
    cfiles = glob.glob(path + '/**/*.c', recursive=True)
    return cfiles


# ----------------------------------------------------------------------------------

def main(argv):

    parser = argparse.ArgumentParser(description='MPI to EEProbe conversion program. Automatically replace MPI collective operations to equivalent EEP collective operations', usage='python3 ./mpi2eep.py [options]')
    parser.add_argument('--source', type=str, required=True,
                        help='path to the directory containing the source code to process')
    args = parser.parse_args()

    print('mpi2eep: MPI to EEProbe conversion program')

    cfiles = walkFileSystem(args.source)

    if cfiles:
        processFiles(cfiles, "C", dic_c)
    
    
    
# ----------------------------------------------------------------------------------

if __name__  == "__main__":
    main(sys.argv)


# ----------------------------------------------------------------------------------
