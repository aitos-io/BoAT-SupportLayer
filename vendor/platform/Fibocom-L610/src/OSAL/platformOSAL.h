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

//!@brief Abstract definition of mutex object based on Ficocom L610 openCPU OS
#ifndef PLATFORM_OSAL_MUTEX
#define PLATFORM_OSAL_MUTEX
struct boatPlatformMutex{
    BUINT32 mutexID; //! fibocom-L610 mutex descriptor
};
#endif

//!@brief Abstract definition of task object based on Ficocom L610 openCPU OS
#ifndef PLATFORM_OSAL_TASK
#define PLATFORM_OSAL_TASK
struct boatPlatformTask{
    BSINT32 taskID; //! fibocom-L610 thread descriptor
};
#endif

//!@brief Abstract definition of timer object based on Ficocom L610 openCPU OS
#ifndef PLATFORM_OSAL_TIMER
#define PLATFORM_OSAL_TIMER
struct boatPlatformTimer{
    BUINT32 timerID; //! fibocom-L610 timer descriptor
};
#endif


#if 0

//!@brief Abstract definition of semaphore object based on Ficocom L610 openCPU OS
#ifndef PLATFORM_OSAL_SEMA
#define PLATFORM_OSAL_SEMA
struct boat_platform_sem{
    sem_t semid; //! fibocom-L610 semaphore descriptor
};
#endif


//!@brief Abstract definition of mutex object based on Ficocom L610 openCPU OS
#ifndef PLATFORM_OSAL_MUTEX
#define PLATFORM_OSAL_MUTEX
struct boatPlatformMutex{
    BUINT32 mutexID; //! fibocom-L610 mutex descriptor
};
#endif

//!@brief Abstract definition of message queue object based on Ficocom L610 openCPU OS
#ifndef PLATFORM_OSAL_QUEUE
#define PLATFORM_OSAL_QUEUE
struct boat_platform_queue{
    mqd_t queueid; //! fibocom-L610 message queue descriptor
    char *name; //! The name of message queue
};
#endif




#endif
#endif
