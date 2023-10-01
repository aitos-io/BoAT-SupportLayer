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

/*!@brief BoAT Support Layer OSAL API for BoAT Infra Arch

@file
platform_linux-default_queue.c defines the OSAL boat queue APIs based on linux OS.
To complie this file you should:
1. Set the -std=gnu99
2. Link with -lrt.
*/
#include <errno.h>
#include <string.h>
#include <stdlib.h>
//#include <sys/time.h>

#include "boatosal.h"
#include "boatlog.h"


#ifndef ROTSQUEUETOTALSIZE
#define ROTSQUEUETOTALSIZE 1024 * 5
#endif
#define RTOSQUEUEPOOLINITED 1
static char rtosQueuePoolState = 0;

#if defined ( __ICCARM__ )
#pragma data_alignment=4
#endif
__ALIGN_BEGIN static UCHAR  queue_byte_pool_buffer[ROTSQUEUETOTALSIZE] __ALIGN_END;
static TX_BYTE_POOL queue_byte_pool;

//! maxSize = one queue msg size
BOAT_RESULT boatQueueInit(boatQueue *queueRef, char *queueName, BUINT32 maxSize, BUINT32 maxNumber)
{
    int ret = 0;
    char name[256];

    if((queueRef == NULL) || (queueName == NULL) || (maxSize == 0) || (maxNumber == 0)) // CHECK PARAMETERS
    {
		BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:	 THREADX QUEUE parameter err[%p==NULL] [%p==NULL] [%d==0] [%d==0]\r\n",queueRef,queueName, maxSize, maxNumber);
        return BOAT_ERROR;
    }
	
	if(rtosQueuePoolState != RTOSQUEUEPOOLINITED)	// init task stack pool space
	{
		if (tx_byte_pool_create(&queue_byte_pool, "Nx App memory pool", queue_byte_pool_buffer, ROTSQUEUETOTALSIZE) != TX_SUCCESS)
		{
			return BOAT_ERROR;// need a error number for stack pool initializing fail.
		}
		rtosQueuePoolState = RTOSQUEUEPOOLINITED;
	}
	
	BCHAR *queueBuf = NULL;
	if (tx_byte_allocate(&queue_byte_pool, (VOID **) &queueBuf, maxSize * maxNumber, TX_NO_WAIT) != TX_SUCCESS)
	{
		return BOAT_ERROR;// need a error number for queue allocating fail.
	}
	queueRef->queueBuf = queueBuf;
	ret = tx_queue_create(&queueRef->queueId,queueRef->name, maxSize, queueBuf, maxSize * maxNumber);
	if(ret != BOAT_SUCCESS)
	{
		tx_byte_release(queueRef->queueBuf);// free queue buf
		queueRef->queueBuf = NULL;
		return BOAT_ERROR;
	}
	/// save queue name?
	return BOAT_SUCCESS;
}

/////BOAT_RESULT boatQueueDelete(boatQueue *queueRef);
BOAT_RESULT boatQueueDelete(boatQueue *queueRef)
{
	BOAT_RESULT ret;
	if(queueRef == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]:    boatQueueDelete The boatQueue is null");
    	return BOAT_ERROR;
    }

	ret = tx_queue_delete(&queueRef->queueId);
	if(ret != BOAT_SUCCESS)
	{
		return BOAT_ERROR;
	}
	tx_byte_release(queueRef->queueBuf);// free queue buf
    queueRef->name = NULL;
    queueRef->queueBuf = NULL;
	return BOAT_SUCCESS;
}

BOAT_RESULT boatQueueSend(const boatQueue *queueRef, unsigned char *msgPtr, BUINT32 msgLen, BUINT32 timeout)
{
	BOAT_RESULT ret;
	(void)msgLen;

 	if((queueRef == NULL) | (msgPtr == NULL))
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]:    boatQueueSend The boatQueue is null");
    	return BOAT_ERROR;
    }
	if(timeout == 0)
		timeout = 0xFFFFFFFF;

	ret = tx_queue_send((TX_QUEUE *)&queueRef->queueId,msgPtr,timeout);
	if(ret != BOAT_SUCCESS)
	{
		return BOAT_ERROR;
	}
	return BOAT_SUCCESS;
	
}

BOAT_RESULT boatQueueReceive(const boatQueue *queueRef, BUINT8 *msgPtr, BUINT32 msgLen ,BUINT32 timeout)
{
	BOAT_RESULT ret;
	(void)msgLen;

 	if((queueRef == NULL) | (msgPtr == NULL))
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]:    boatQueueReceive The boatQueue is null");
    	return BOAT_ERROR;
    }

	if(timeout == 0)
		timeout = 0xFFFFFFFF;
	ret = tx_queue_receive((TX_QUEUE *)&queueRef->queueId, (void *)msgPtr, timeout);
	if(ret != BOAT_SUCCESS)
	{
		return BOAT_ERROR;
	}
	return BOAT_SUCCESS;

	return 0;
}

BOAT_RESULT boatQueueInitQueueIdZero(boatQueue *queueRef)
{
    if(queueRef != NULL)  
    {
        //queueRef->queueId = 0;
        queueRef->name = "my";
    	return BOAT_SUCCESS;
    }
    return BOAT_ERROR;
}

BOAT_RESULT boatQueueInitQueueNameNULL(boatQueue *queueRef)
{
    if(queueRef != NULL)  
    {
        //queueRef->queueId = 1;
        queueRef->name = NULL;
    	return BOAT_SUCCESS;
    }
    return BOAT_ERROR;
}


