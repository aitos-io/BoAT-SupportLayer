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

#include "fibo_opencpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
#include "boatosal.h"

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
    BUINT32 fiboSem = 0;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] begin to boatSemInit semRef address[%x]",(BUINT32)semRef);
	if(semRef == NULL)  
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] The boatSem address is illegal in boatSemInit, bad address:%x",(BUINT32)semRef);
    	return BOAT_ERROR;
    }
        
    
    fiboSem = fibo_sem_new(initialCount);
    BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemInit fibo_sem_new return id[%x]",fiboSem);
	if(fiboSem != 0)    /////	fibocom semRef id is a UINT32, if the return value does not equal 0,the value is valid.
    {
    	semRef->semId = fiboSem;
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemInit OK 222,rtnVal[%x]",semRef->semId);
    }
	else
    {
    	semRef->semId = fiboSem;
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemInit failed,rtnVal is 0");
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
	if(semRef->semId == 0)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] The boatSem->semId is 0, bad value\r\n");
    	return BOAT_ERROR;
    }

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] begin to boatSemDestroy ID[%x]",semRef->semId);

	fibo_sem_free(semRef->semId); ///// no return value, void 
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
    BOAT_RESULT ret = BOAT_SUCCESS;
	BSINT32 rtnVal = 0;
    
	if(semRef == NULL)  
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] The boatSem address is illegal in boatSemWait, bad address:%x",(BUINT32)semRef);
    	return BOAT_ERROR;
    }
	if(semRef->semId == 0)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] The boatSem->semId is 0, bad value\r\n");
    	return BOAT_ERROR;
    }

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] begin to boatSemWait ID[%x] timeout[%d]",semRef->semId,timeout);
	if(timeout > 0)    ///// try wait
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemWait call fiboSem_try_wait ID[%x] timeout[%d]",semRef->semId,timeout);
    	rtnVal = fibo_sem_try_wait(semRef->semId,timeout); // it will be blocked untill the semRef is unloked 
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemWait retrun from fiboSem_try_wait ID[%x] timeout[%d]",semRef->semId,timeout);
    	if(rtnVal > 0)    ///// succ
        {
        	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemWait succ ID[%x] timeout[%d] [%d]",semRef->semId,timeout,rtnVal);
        }
    	else///// timeout
        {
        	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemWait time out ID[%x] timeout[%d] [%d]",semRef->semId,timeout,rtnVal);
        	ret = BOAT_ERROR;/////rtnVal + BOAT_ERROR;    ///// need check the error value, define the same TIMEOUT ERROR value for OSAL
        }
    }
	else
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemWait call fibo_Sem_wait ID[%x]",semRef->semId);
    	fibo_sem_wait(semRef->semId); ///// it will be blocked untill the semRef is unloked ,no return value
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemWait return from fibo_Sem_wait ID[%x]",semRef->semId);
    }

	return ret;
    
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
    BOAT_RESULT ret = BOAT_SUCCESS;

	if(semRef == NULL)  
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] The boatSem address is illegal in boatSemPost, bad address:%x",(BUINT32)semRef);
    	return BOAT_ERROR;
    }
	if(semRef->semId == 0)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] The boatSem->semId is 0, bad value\r\n");
    	return BOAT_ERROR;
    }

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemPost call fibo_Sem_post ID[%x] ",semRef->semId);

	fibo_sem_signal(semRef->semId); ///// no return value
    
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] boatSemPost retrun from fibo_Sem_post ID[%x] ",semRef->semId);

	return ret;
}

///// test functions
BOAT_RESULT boatSemInitSemIdZero(boatSem *semRef)
{
    if(semRef != NULL)  
    {
        semRef->semId = 0;
    	return BOAT_SUCCESS;
    }
    return BOAT_ERROR;
}

BOAT_RESULT boatSemInitSemIdInvalid(boatSem *semRef)
{
	boatSem sem;
	boatSemInit(&sem,0);

	semRef->semId = sem.semId+0X2324;

	boatSemDestroy(&sem);
}




