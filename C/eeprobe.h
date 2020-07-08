
    /* EEProbe: Energy Efficient Probe for MPI */
    /* Copyright (C) 2020 Loïc Cudennec */

    /* This program is free software: you can redistribute it and/or modify */
    /* it under the terms of the GNU General Public License as published by */
    /* the Free Software Foundation, either version 3 of the License, or */
    /* any later version. */

    /* This program is distributed in the hope that it will be useful, */
    /* but WITHOUT ANY WARRANTY; without even the implied warranty of */
    /* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
    /* GNU General Public License for more details. */

    /* You should have received a copy of the GNU General Public License */
    /* along with this program.  If not, see <https://www.gnu.org/licenses/>. */


/* ---------------------------------------------------------------------------------- */


#ifndef EEPROBE_H
#define EEPROBE_H

/* MPI */
#include "mpi.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ---------------------------------------------------------------------------------- */

  /**
   * Enum type used to enable or disable the micro-sleep mechanism.
   */
typedef enum {EEPROBE_ENABLE, EEPROBE_DISABLE} EEPROBE_Enable;

/* ---------------------------------------------------------------------------------- */

  /**
   * Calculate the total amount of sleep duration since the beginning
   * Use EEPROBE_getTotalSleepTime() to read this value.
   */
#define EEPROBE_ENABLE_TOTAL_SLEEP_TIME 1

/* ---------------------------------------------------------------------------------- */

  /**
   * EEPROBE_Probe takes the same parameters
   * as the default MPI Probe function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous and returns when a
   * matching message is being delivered by the MPI runtime.
   *
   * @param source Source rank or MPI_ANY_SOURCE (integer).
   * @param tag Tag value or MPI_ANY_TAG (integer).
   * @param comm Communicator (handle).
   * @param status Status object (status).
   * @param enable Enable or disable the micro-sleep mechanism (Switch only).
   * @return MPI routine error value.
   */
int
EEPROBE_Probe(int source, int tag, MPI_Comm comm, MPI_Status * status);
int
EEPROBE_Probe_Switch(int source, int tag, MPI_Comm comm, MPI_Status * status,
		     EEPROBE_Enable enable);


/* ---------------------------------------------------------------------------------- */

  /**
   * EEPROBE_Wait takes the same parameters
   * as the default MPI Wait function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous and waits for an MPI
   * send or receive to complete.
   *
   * @param request The request handle.
   * @param status Status object (status).
   * @param enable Enable or disable the micro-sleep mechanism (Switch only).
   * @return MPI routine error value.
   */
int
EEPROBE_Wait(MPI_Request *request, MPI_Status *status);
int
EEPROBE_Wait_Switch(MPI_Request *request, MPI_Status *status, EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

  /**
   * EEPROBE_Recv takes the same parameters
   * as the default MPI Recv function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous and performs a
   * standard-mode blocking receive.
   *
   * @param buf Initial address of receive buffer.
   * @param count Maximum number of elements to receive.
   * @param datatype Datatype of each receive buffer entry.
   * @param source Rank of source.
   * @param tag Message tag.
   * @param comm Communicator (handle).
   * @param status Status object.
   * @param enable Enable or disable the micro-sleep mechanism (Switch only).
   * @return MPI routine error value.
   */
int
EEPROBE_Recv(void *buf, int count, MPI_Datatype datatype,
	     int source, int tag, MPI_Comm comm, MPI_Status *status);
int
EEPROBE_Recv_Switch(void *buf, int count, MPI_Datatype datatype,
		    int source, int tag, MPI_Comm comm, MPI_Status *status,
		    EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */
  
  /**
   * EEPROBE_Reduce takes the same parameters
   * as the default MPI Reduce function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous and reduces values on
   * all processes within a group.
   *
   * @param sendbuf Address of send buffer.
   * @param recvbuf Address of receive buffer.
   * @param count Number of elements in send buffer.
   * @param datatype Data type of elements of send buffer.
   * @param op Reduce operation.
   * @param root Rank of root process.
   * @param comm Communicator (handle).
   * @param enable Enable or disable the micro-sleep mechanism (Switch only).
   * @return MPI routine error value.
   */
int
EEPROBE_Reduce(const void *sendbuf, void *recvbuf, int count,
	       MPI_Datatype datatype, MPI_Op op, int root,
	       MPI_Comm comm);
int
EEPROBE_Reduce_Switch(const void *sendbuf, void *recvbuf, int count,
		      MPI_Datatype datatype, MPI_Op op, int root,
		      MPI_Comm comm, EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

  /**
   * EEPROBE_Allreduce takes the same parameters
   * as the default MPI Allreduce function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous and combines values from
   * all processes and distributes the result back to all processes.
   *
   * @param sendbuf Address of send buffer.
   * @param recvbuf Address of receive buffer.
   * @param count Number of elements in send buffer.
   * @param datatype Data type of elements of send buffer.
   * @param op Reduce operation.
   * @param comm Communicator (handle).
   * @param enable Enable or disable the micro-sleep mechanism (Switch only).
   * @return MPI routine error value.
   */
int
EEPROBE_Allreduce(const void *sendbuf, void *recvbuf, int count,
		  MPI_Datatype datatype, MPI_Op op, MPI_Comm comm);
int
EEPROBE_Allreduce_Switch(const void *sendbuf, void *recvbuf, int count,
			 MPI_Datatype datatype, MPI_Op op, MPI_Comm comm,
			 EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

  /**
   * EEPROBE_Alltoall takes the same parameters
   * as the default MPI Alltoall function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous.
   * All processes send data to all.
   *
   * @param sendbuf Address of send buffer.
   * @param sendcount Number of elements to send to each process.
   * @param sendtype Datatype of send buffer elements.
   * @param recvbuf Address of receive buffer.
   * @param recvcount Number of elements to receive from each process.
   * @param recvtype Datatype of receive buffer elements.
   * @param comm Communicator (handle).
   * @param enable Enable or disable the micro-sleep mechanism (Switch only).
   * @return MPI routine error value.
   */
int
EEPROBE_Alltoall(const void *sendbuf, int sendcount,
		 MPI_Datatype sendtype, void *recvbuf, int recvcount,
		 MPI_Datatype recvtype, MPI_Comm comm);
int
EEPROBE_Alltoall_Switch(const void *sendbuf, int sendcount,
			MPI_Datatype sendtype, void *recvbuf, int recvcount,
			MPI_Datatype recvtype, MPI_Comm comm,
			EEPROBE_Enable enable);
  
/* ---------------------------------------------------------------------------------- */

  /**
   * EEPROBE_Alltoallv takes the same parameters
   * as the default MPI Alltoallv function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous.
   * All processes send different amount of data to, and receive different amount of
   * data from, all processes.
   *
   * @param sendbuf Address of send buffer.
   * @param sendcounts Integer array, where entry i specifies the number of elements to send to rank i.
   * @param sdispls Integer array, where entry i specifies the displacement (offset from sendbuf, in units of sendtype) from which to send data to rank i.
   * @param sendtype Datatype of send buffer elements.
   * @param recvbuf Address of receive buffer.
   * @param recvcounts Integer array, where entry j specifies the number of elements to receive from rank j.
   * @param rdispls Integer array, where entry j specifies the displacement (offset from recvbuf, in units of recvtype) to which data from rank j should be written.
   * @param recvtype Datatype of receive buffer elements.
   * @param comm Communicator (handle).
   * @param enable Enable or disable the micro-sleep mechanism (Switch only).
   * @return MPI routine error value.
   */
int
EEPROBE_Alltoallv(const void *sendbuf, const int sendcounts[],
		  const int sdispls[], MPI_Datatype sendtype,
		  void *recvbuf, const int recvcounts[],
		  const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm);
int
EEPROBE_Alltoallv_Switch(const void *sendbuf, const int sendcounts[],
			 const int sdispls[], MPI_Datatype sendtype,
			 void *recvbuf, const int recvcounts[],
			 const int rdispls[], MPI_Datatype recvtype, MPI_Comm comm,
			 EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

  /**
   * EEPROBE_Alltoallw takes the same parameters
   * as the default MPI Alltoallw function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous.
   * All processes send data of different types to, and receive data of different
   * types from, all processes.
   *
   * @param sendbuf Address of send buffer.
   * @param sendcounts Integer array, where entry i specifies the number of elements to send to rank i.
   * @param sdispls Integer array, where entry i specifies the displacement (offset from sendbuf) from which to send data to rank i.
   * @param sendtype Datatype array, where entry i specifies the datatype to use when sending data to rank i.
   * @param recvbuf Address of receive buffer.
   * @param recvcounts Integer array, where entry j specifies the number of elements to receive from rank j.
   * @param rdispls Integer array, where entry j specifies the displacement (offset from recvbuf) to which data from rank j should be written.
   * @param recvtype Datatype array, where entry j specifies the datatype to use when receiving data from rank j.
   * @param comm Communicator (handle).
   * @param enable Enable or disable the micro-sleep mechanism (Switch only).
   * @return MPI routine error value.
   */
int
EEPROBE_Alltoallw(const void *sendbuf, const int sendcounts[],
		  const int sdispls[], const MPI_Datatype sendtypes[],
		  void *recvbuf, const int recvcounts[], const int rdispls[],
		  const MPI_Datatype recvtypes[], MPI_Comm comm);
int
EEPROBE_Alltoallw_Switch(const void *sendbuf, const int sendcounts[],
			 const int sdispls[], const MPI_Datatype sendtypes[],
			 void *recvbuf, const int recvcounts[], const int rdispls[],
			 const MPI_Datatype recvtypes[], MPI_Comm comm,
			 EEPROBE_Enable enable);

/* ---------------------------------------------------------------------------------- */

  /**
   * EEPROBE_Bcast takes the same parameters
   * as the default MPI Bcast function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous and broadcasts a message
   * from the process with rank root to all other processes of the group.
   *
   * @param buffer Address of send buffer.
   * @param count Number of entries in buffer.
   * @param datatype Data type of buffer.
   * @param root Rank of broadcast root.
   * @param comm Communicator (handle).
   * @param enable Enable or disable the micro-sleep mechanism (Switch only).
   * @return MPI routine error value.
   */
int
EEPROBE_Bcast(void *buffer, int count, MPI_Datatype datatype,
	      int root, MPI_Comm comm);
int
EEPROBE_Bcast_Switch(void *buffer, int count, MPI_Datatype datatype,
		     int root, MPI_Comm comm, EEPROBE_Enable enable);


/* ---------------------------------------------------------------------------------- */

  /**
   * EEPROBE_Barrier takes the same parameters
   * as the default MPI Barrier function and a specific parameter to enable or disable
   * the micro-sleeping mechanism. This function is synchronous and broadcasts a message
   * from the process with rank root to all other processes of the group.
   *
   * @param comm Communicator (handle).
   * @param enable Enable or disable the micro-sleep mechanism (Switch only).
   * @return MPI routine error value.
   */
int
EEPROBE_Barrier(MPI_Comm comm);
int
EEPROBE_Barrier_Switch(MPI_Comm comm, EEPROBE_Enable enable);


/* ---------------------------------------------------------------------------------- */

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

/* ---------------------------------------------------------------------------------- */
  
  /**
   * Returns the total sleep duration since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTime();

  /**
   * Returns the total sleep duration using EEPROBE_Probe since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTimeProbe();

  /**
   * Returns the total sleep duration using EEPROBE_Wait since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTimeWait();

  /**
   * Returns the total sleep duration using EEPROBE_Recv since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTimeRecv();

  /**
   * Returns the total sleep duration using EEPROBE_Reduce since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTimeReduce();

  /**
   * Returns the total sleep duration using EEPROBE_Allreduce since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTimeAllreduce();

  /**
   * Returns the total sleep duration using EEPROBE_Alltoall since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTimeAlltoall();

  /**
   * Returns the total sleep duration using EEPROBE_Alltoallv since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTimeAlltoallv();

  /**
   * Returns the total sleep duration using EEPROBE_Alltoallw since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTimeAlltoallw();

  /**
   * Returns the total sleep duration using EEPROBE_Bcast since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTimeBcast();

  /**
   * Returns the total sleep duration using EEPROBE_Barrier since the beginning of the run.
   * EEPROBE_ENABLE_TOTAL_SLEEP_TIME must be set to 1 in this file, returns 0 otherwise.
   * @return Total sleep duration in nanoseconds.
   */
unsigned long EEPROBE_getTotalSleepTimeBarrier();

/* ---------------------------------------------------------------------------------- */
  
  /**
   * Returns the current system time as delivered by gettimeofday()
   * @return The current system time in microseconds.
   */
unsigned long EEPROBE_getTime();
  
/* ---------------------------------------------------------------------------------- */
  
#ifdef __cplusplus
}
#endif


#endif
