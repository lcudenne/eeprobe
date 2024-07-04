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
         'MPI_Bcast' : 'EEPROBE_Bcast',
         'MPI_Scatter' : 'EEPROBE_Scatter',
         'MPI_Scatterv' : 'EEPROBE_Scatterv',
         'MPI_Gather' : 'EEPROBE_Gather',
         'MPI_Gatherv' : 'EEPROBE_Gatherv',
         'MPI_Allgather' : 'EEPROBE_Allgather',
         'MPI_Allgatherv' : 'EEPROBE_Allgatherv',
         'MPI_Barrier' : 'EEPROBE_Barrier'}

# ----------------------------------------------------------------------------------

def processFiles(files, filetype, dic, includepath=""):
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
                filedata = filedata.replace("MPI_Finalize();", "printf(\"rank %d EEProbe sleep time (ns) probe %lu wait %lu reduce %lu allreduce %lu alltoall %lu alltoallv %lu bcast %lu\\n\", my_rank, EEPROBE_getTotalSleepTimeProbe(), EEPROBE_getTotalSleepTimeWait(), EEPROBE_getTotalSleepTimeReduce(), EEPROBE_getTotalSleepTimeAllreduce(), EEPROBE_getTotalSleepTimeAlltoall(), EEPROBE_getTotalSleepTimeAlltoallv(), EEPROBE_getTotalSleepTimeBcast());\nMPI_Finalize();")
                with open(fpath, 'w') as fw:
                    fw.write("#include \""+includepath+"eeprobe.h\"\n" + filedata)
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
    parser.add_argument('--includepath', type=str, required=False,
                        help='path to the directory containing the eeprobe.h file')
    args = parser.parse_args()

    print('mpi2eep: MPI to EEProbe conversion program')

    cfiles = walkFileSystem(args.source)

    if cfiles:
        includepath=""
        if args.includepath:
            includepath=args.includepath
        processFiles(cfiles, "C", dic_c, includepath)
    
    
    
# ----------------------------------------------------------------------------------

if __name__  == "__main__":
    main(sys.argv)


# ----------------------------------------------------------------------------------
