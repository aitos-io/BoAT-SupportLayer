#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "boatosal.h"
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"

#ifdef PLATFORM_OSAL_SEM


/*!*****************************************************************************
@brief Init a Boat semaphore

Function: boatSemInit()

    This function create a semaphore instance and init the boat semaphore.

    boatSemInit() MUST be called before any use of BoAT semaphore.
    boatSemDestroy() MUST be called after use of BoAT semaphore.

@return
    This function returns BOAT_SUCCESS if the initialization is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.
    

@param[in] semRef
    A boatSem struct pointer,the boatSemInit() function shall \n
    initialize the boat semaphore referred to by semRef.

@param[in] initialCount
    The value of the initialized boat semaphore shall be initialCount.
        
*******************************************************************************/
BOAT_RESULT boatSemInit(boatSem *semRef,int initialCount)
{
    if((semRef == NULL) || (initialCount < 0))
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    sem_t linux_sem;
    int ret = sem_init(&linux_sem,0,initialCount);
    if(ret != 0)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Sem_init error!");
        return BOAT_ERROR;
    }
    semRef->semid = linux_sem;
    //memcpy(&(semRef->semid),&linux_sem,sizeof(sem_t));

    return BOAT_SUCCESS;

}

/*!*****************************************************************************
@brief Destroy a Boat semaphore

Function: boatSemDestroy()

    This function destroys a semaphore instance and deinit the boat semaphore.

@return
    This function returns BOAT_SUCCESS if the destroying is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.
    

@param[in] semRef
    A boatSem struct pointer,the boatSemDestroy() function shall \n
    destroy the boat semaphore indicated by semRef.

*******************************************************************************/
BOAT_RESULT boatSemDestroy(boatSem *semRef)
{
    if(semRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    int ret = sem_destroy(&(semRef->semid));
    if(ret != 0)
    {
        BoatLog(BOAT_LOG_CRITICAL,"sem_destroy error!");
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief Unlock a Boat semaphore

Function: boatSemPost()

     The boatSemPost() function shall unlock/increase the Boat semaphore referenced by\n
     semRef. 

@return
    This function returns BOAT_SUCCESS if the unlock operation is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to indicate the error, \n
    for reference in boaterrcode.h.    

@param[in] semRef
    A boatSem struct pointer,The boatSemPost() function shall unlock\n
    the Boat semaphore referenced by semRef by performing a semaphore \n
    unlock operation on that BoAT semaphore.

*******************************************************************************/
BOAT_RESULT boatSemPost(boatSem *semRef)
{
    if(semRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    
    int ret = sem_post(&(semRef->semid));
    if(ret != 0)
    {
        BoatLog(BOAT_LOG_CRITICAL,"sem_post error!");
        return BOAT_ERROR;
    }
    
    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief Lock a Boat semaphore

Function: boatSemWait()

     The boatSemWait() function shall lock/reduce the Boat semaphore referenced by
     semRef. 

@return
    This function returns BOAT_SUCCESS if the lock operation is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.\n\n

    If the timeout has already expired by the time of the call, and the \n
    boat semaphore could not be locked immediately, then boatSemWait() \n
    return a timeout error (BOAT_ERROR_TIMEOUT)

@param[in] semRef
    A boatSem struct pointer,The boatSemWait() function shall lock\n
    the Boat semaphore referenced by semRef by performing a semaphore \n
    lock operation on that BoAT semaphore.

@param[in] timeout(ms)
    timeout specifies a limit on the amount of time that the call should \n
    block if the decrement cannot be immediately performed.

*******************************************************************************/

BOAT_RESULT boatSemWait(boatSem *semRef,BUINT32 timeout)
{
    if(semRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    int ret;
    BOAT_RESULT result = BOAT_ERROR;

    if(timeout > 0)
    {
        BUINT32 loop = 0;
        while (1)
        {
            ret = sem_trywait(&(semRef->semid));
            if((0 == ret) || (loop >= timeout))
            {
                if(ret == 0)
                {
                    result = BOAT_SUCCESS;
                }
                else
                {
                    result = BOAT_ERROR;
                }

                break;
            }
            usleep(1000);//sleep 1ms
            loop ++;

        }
        
    }
    else
    {
        ret = sem_wait(&(semRef->semid));
        if(ret != 0)
        {
            BoatLog(BOAT_LOG_CRITICAL,"sem_wait error!");
            return BOAT_ERROR;
        }
        result = BOAT_SUCCESS;
    }

    return result;
}
#endif/////PLATFORM_OSAL_SEM