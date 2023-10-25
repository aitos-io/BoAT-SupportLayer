#include "boatosal.h"

#ifdef PLATFORM_OSAL_SEM

#include "stdio.h"
#include "osi_api.h"

#include "boattypes.h"
#include "boatlog.h"


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

    if(semRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }


    semRef->semId = osiSemaphoreCreate(1,initialCount);
    if(semRef->semId == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Sem init failed!");
        return BOAT_ERROR;
    }

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
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(semRef->semId == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad sem_Id!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    osiSemaphoreDelete(semRef->semId);
  
    semRef->semId = NULL;
    
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
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(semRef->semId == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad sem_Id!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    osiSemaphoreRelease(semRef->semId);
 
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

@param[in] timeout
    timeout specifies a limit on the amount of time that the call should \n
    block if the decrement cannot be immediately performed.

*******************************************************************************/
BOAT_RESULT boatSemWait(boatSem *semRef,BUINT32 timeout)
{
    bool ret;
    if(semRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(semRef->semId == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad sem_Id!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(timeout > 0)
    {
        ret = osiSemaphoreTryAcquire(semRef->semId,timeout);
    }
    else
    {
        ret = osiSemaphoreAcquire(semRef->semId);
    }


    if(ret == false)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Sem wait failed!");
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;

}
#endif/////PLATFORM_OSAL_SEM