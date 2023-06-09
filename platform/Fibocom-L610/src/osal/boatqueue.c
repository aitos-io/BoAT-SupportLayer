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

BOAT_RESULT boatQueueInit(boatQueue *queueRef, char *queueName, BUINT32 maxSize, BUINT32 maxNumber)
{
	BUINT32  fiboQueue = 0;
	
	if(queueRef == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue address is illegal in boatQueueInit, bad address:%x,ph",(BUINT32)queueRef);
    	return BOAT_ERROR;
    }	
	if(maxSize == 0)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue maxSize is 0, invalid\r\n");
    	return BOAT_ERROR;
    }	
	if(maxNumber == 0)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue maxNumber is 0, invalid\r\n");
    	return BOAT_ERROR;
    }	
	///// queueName is not used
	(void)queueName;
	
	fiboQueue = fibo_queue_create(maxNumber, maxSize);
	if(fiboQueue > 0)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue create queue succ\r\n");
		queueRef->queueId = fiboQueue;
		queueRef->maxSize = maxSize;
		return BOAT_SUCCESS;
	}
	else
	{
		queueRef->queueId = 0;    	
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue create queue succ\r\n");
		return BOAT_ERROR;		
	}

}
BOAT_RESULT boatQueueDelete(boatQueue *queueRef)
{
	if(queueRef == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue address is illegal in boatQueueInit, bad address:%x,ph",(BUINT32)queueRef);
    	return BOAT_ERROR;
    }
	if(queueRef->queueId == 0)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue queueId is 0 in boatQueueDelete, bad address:%x,ph",(BUINT32)queueRef);
    	return BOAT_ERROR;
    }

	fibo_queue_delete(queueRef->queueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue delete queue succ\r\n");
	queueRef->maxSize = 0;
	queueRef->queueId = 0;

	return BOAT_SUCCESS;
}
BOAT_RESULT boatQueueSend(const boatQueue *queueRef, BUINT8 *msgPtr, BUINT32 msgLen,BUINT32 timeout)
{
	if(queueRef == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue address is illegal in boatQueueInit, bad address:%x,ph",(BUINT32)queueRef);
    	return BOAT_ERROR;
    }

	if(queueRef->queueId == 0)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue queueId is 0 in boatQueueSend, bad address:%x,ph",(BUINT32)queueRef);
    	return BOAT_ERROR;///// BOAT_DATA_INVALID
    }

	//! msgPtr check
	if(msgPtr == NULL)
	{
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue msgPtr is NULL in boatQueueSend");
		return BOAT_ERROR;
	}

	//! msgLen check
	if(msgLen > queueRef->maxSize)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue] L610 queue msgLen great than maxSize of one message!\r\n");
		return BOAT_ERROR;
	}

	//! msgLen check
	/////if(msgLen == 0)
	/////{
	/////	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] L610 queue msgLen equal 0, return succ!\r\n");
	/////	return BOAT_SUCCESS;
	/////}
	
	BSINT32 ret = fibo_queue_put(queueRef->queueId,msgPtr,timeout);
	if(ret != 0)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue] L610 queue put failed!\r\n");
		return BOAT_ERROR;
	}
	
	return BOAT_SUCCESS;
}

BOAT_RESULT boatQueueReceive(const boatQueue *queueRef, BUINT8 *msgPtr, BUINT32 msgLen ,BUINT32 timeout)
{
	if(queueRef == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue address is illegal in boatQueueInit, bad address:%x,ph",(BUINT32)queueRef);
    	return BOAT_ERROR;
    }

	if(queueRef->queueId == 0)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue queueId is 0 in boatQueueDelete, bad address:%x,ph",(BUINT32)queueRef);
    	return BOAT_ERROR;///// BOAT_DATA_INVALID
    }

	//! msgPtr check
	if(msgPtr == NULL)
	{
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue msgPtr is NULL in boatQueueSend");
		return BOAT_ERROR;
	}
	
	//! len check
	///// msgLen is not used
	(void)msgLen;

	BSINT32 ret = fibo_queue_get(queueRef->queueId, msgPtr, timeout);
	if(ret != 0)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue] L610 queue get failed!\r\n");
		return BOAT_ERROR;///// We do not know witch error is, timeout or error
	}
	
	return BOAT_SUCCESS;
}

BOAT_RESULT boatQueueInitQueueIdZero(boatQueue *queueRef)
{
    if(queueRef != NULL)  
    {
        queueRef->queueId = 0;
    	return BOAT_SUCCESS;
    }
    return BOAT_ERROR;
}


