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


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
#include "boatosal.h"

#include "simcom_os.h"
#include "simcom_common.h"

// boatSem semRef;
/**
****************************************************************************************
* @brief:
*  This function creat or init semRef;
* @param[in] *semRef
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
//#define BoatPrintf ;\/\/BoatPrintf
BOAT_RESULT boatSemInit(boatSem *semRef,int initialCount)
{
	SC_STATUS os_status;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] begin to boatSemInit semRef address[%x]",(BUINT32)semRef);
	if(semRef == NULL)  
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] The boatSem address is illegal in boatSemInit, bad address:%x",(BUINT32)semRef);
    	return BOAT_ERROR;
    }
        
    os_status = sAPI_SemaphoreCreate(&(semRef->semId),initialCount, SC_FIFO);
	if(SC_SUCCESS != os_status)  
    {
    	semRef->semId = NULL;
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemInit OK 222,rtnVal[%x]",semRef->semId);
		return BOAT_ERROR;
    }

	return BOAT_SUCCESS;
}

/**
****************************************************************************************
* @brief:
*  This function delete or destory semRef;
* @param[in] semRef
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
BOAT_RESULT boatSemDestroy(boatSem *semRef)
{
	if(semRef == NULL)  
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] The boatSem address is illegal in boatSemInit, bad address:%x",(BUINT32)semRef);
    	return BOAT_ERROR;
    }
	if(semRef->semId == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] The boatSem->semId is 0, bad value\r\n");
    	return BOAT_ERROR;
    }

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] begin to boatSemDestroy ID[%x]",semRef->semId);

	SC_STATUS os_status = sAPI_SemaphoreDelete(semRef->semId); ///// no return value, void 
	if(SC_SUCCESS != os_status) 
	{
		return BOAT_ERROR;
	}

   	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemDestroy ok");
	return BOAT_SUCCESS;
}
/**
****************************************************************************************
* @brief:
*  This function lock/reduce semRef;
* @param[in] *semRef
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
BOAT_RESULT boatSemWait(boatSem *semRef,BUINT32 timeout)
{
  
	if(semRef == NULL)  
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] The boatSem address is illegal in boatSemWait, bad address:%x",(BUINT32)semRef);
    	return BOAT_ERROR;
    }
	if(semRef->semId == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] The boatSem->semId is 0, bad value\r\n");
    	return BOAT_ERROR;
    }

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] begin to boatSemWait ID[%x] timeout[%d]",semRef->semId,timeout);
	SC_STATUS os_status = sAPI_SemaphoreAcquire(semRef->semId,timeout);
	if(SC_SUCCESS != os_status) 
	{
		return BOAT_ERROR;
	}
	return BOAT_SUCCESS;
    
}
/**
****************************************************************************************
* @brief:
    This function unlock/increase semRef
* @param[in] *semRef
* @return
*   This function return BOAT_SUCCESS if executed successfully;
*   Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*   for details.
****************************************************************************************
*/
BOAT_RESULT boatSemPost(boatSem *semRef)
{

	if(semRef == NULL)  
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] The boatSem address is illegal in boatSemPost, bad address:%x",(BUINT32)semRef);
    	return BOAT_ERROR;
    }
	if(semRef->semId == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] The boatSem->semId is 0, bad value\r\n");
    	return BOAT_ERROR;
    }

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemPost call fibo_Sem_post ID[%x] ",semRef->semId);

	SC_STATUS os_status = sAPI_SemaphoreRelease(semRef->semId);
	if(SC_SUCCESS != os_status) 
	{
		return BOAT_ERROR;
	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemPost retrun from fibo_Sem_post ID[%x] ",semRef->semId);

	return BOAT_SUCCESS;
}

///// test functions
BOAT_RESULT boatSemInitSemIdZero(boatSem *semRef)
{
    if(semRef != NULL)  
    {
        semRef->semId = NULL;
    	return BOAT_SUCCESS;
    }
    return BOAT_ERROR;
}


