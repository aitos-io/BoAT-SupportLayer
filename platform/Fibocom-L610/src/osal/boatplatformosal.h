/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

/*!@brief Define the abstract types of OSAL based on Ficocom L610 openCPU OS,
 * and include the external header files.

@file
* platformOSAL.h defines the abstract types for boatosal.h.
* Do not use the types in platform.h directly.
*/

#ifndef __BOATPLATFORMOSAL_H__
#define __BOATPLATFORMOSAL_H__
#include <stdio.h>
#include "boattypes.h"
#include "boaterrcode.h"
/**
****************************************************************************************
* @brief:
*  This function generate random data with fixed length.
* @param[in] *output
*  output of random data
* @param[in] outputLen
*  length of random that wanted to generate
* @param[in] *rsvd
*  no used until now
* @return
*  This function returns BoAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
BOAT_RESULT BoatRandom(BUINT8 *output, BUINT32 outputLen, void *rsvd);
/**
****************************************************************************************
* @brief:
*  This function malloc  size memory
* @param[in] size
* @return
*  return malloced memory, if failed return NULL.
****************************************************************************************
*/
void *BoatMalloc(size_t size);
/**
****************************************************************************************
* @brief:
*  This function free memory
* @param[in] *mem_ptr
* @return
*  This function has no return value
****************************************************************************************
*/
void BoatFree(void *mem_ptr);

/**
****************************************************************************************
* @brief:
* @param[in] second
* @return
****************************************************************************************
*/
void BoatSleep(BUINT32 second);
/**
****************************************************************************************
* @brief:
* @return
****************************************************************************************
*/
long int BoatGetTimes();
/**
****************************************************************************************
* @brief:
* @param[in] ms
* @return
****************************************************************************************
*/
void BoatSleepMs(BUINT32 ms);

//!@brief Abstract definition of mutex object based on Ficocom L610 openCPU OS
#ifndef PLATFORM_OSAL_MUTEX
#define PLATFORM_OSAL_MUTEX
struct boatPlatformMutex{
    BUINT32 mutexID; //! fibocom-L610 mutex descriptor
};
#endif/////PLATFORM_OSAL_MUTEX

//!@brief Abstract definition of task object based on Ficocom L610 openCPU OS
#ifndef PLATFORM_OSAL_TASK
#define PLATFORM_OSAL_TASK
struct boatPlatformTask{
    BSINT32 taskId; //! fibocom-L610 thread descriptor
};
#endif/////PLATFORM_OSAL_TASK

//!@brief Abstract definition of timer object based on Ficocom L610 openCPU OS
#ifndef PLATFORM_OSAL_TIMER
#define PLATFORM_OSAL_TIMER
struct boatPlatformTimer{
    BUINT32 timerId; //! fibocom-L610 timer descriptor
};
#endif/////PLATFORM_OSAL_TIMER



//!@brief Abstract definition of semaphore object based on Ficocom L610 openCPU OS
#ifndef PLATFORM_OSAL_SEM
#define PLATFORM_OSAL_SEM
#define FIBO_SEM_MUTEX

struct boatPlatformSem{
    BUINT32 semId; //! Ficocom L610 semaphore descriptor
};
#endif/////PLATFORM_OSAL_SEM

//!@brief Abstract definition of message queue object based on Ficocom L610 openCPU OS
#ifndef PLATFORM_OSAL_QUEUE
#define PLATFORM_OSAL_QUEUE
struct boatPlatformQueue{
    BUINT32 queueId; //! fibocom-L610 message queue descriptor
    BUINT32 maxSize; //! fibocom L610 put/get message buffer size must qeual maxSize, 
};
#endif/////PLATFORM_OSAL_QUEUE

#endif/////__BOATPLATFORMOSAL_H__
