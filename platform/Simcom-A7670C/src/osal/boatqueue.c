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

BOAT_RESULT boatQueueInit(boatQueue *queueRef, char *queueName, BUINT32 maxSize, BUINT32 maxNumber)
{
	SC_STATUS os_status;
	
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
	
	os_status = sAPI_MsgQCreate(&(queueRef->queueId),queueName,sizeof(SIM_MSG_T),maxNumber, SC_FIFO);
	if(os_status == SC_SUCCESS)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue create queue succ\r\n");
		queueRef->maxSize = sizeof(SIM_MSG_T);
		return BOAT_SUCCESS;
	}
	else
	{
		queueRef->queueId = NULL;    	
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue create queue fail\r\n");
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
	if(queueRef->queueId == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue queueRef is 0 in boatQueueDelete, bad address:%x,ph",(BUINT32)queueRef);
    	return BOAT_ERROR;
    }

	sAPI_MsgQDelete(queueRef->queueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue delete queue succ\r\n");
	queueRef->maxSize = 0;
	queueRef->queueId = NULL;

	return BOAT_SUCCESS;
}
BOAT_RESULT boatQueueSend(const boatQueue *queueRef, BUINT8 *msgPtr, BUINT32 msgLen,BUINT32 timeout)
{
	SC_STATUS os_status;
	if(queueRef == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue address is illegal in boatQueueInit, bad address:%x,ph",(BUINT32)queueRef);
    	return BOAT_ERROR;
    }

	if(queueRef->queueId == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue queueRef is 0 in boatQueueSend, bad address:%x,ph",(BUINT32)queueRef);
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
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue]  queue msgLen great than maxSize of one message!\r\n");
		return BOAT_ERROR;
	}

	SIM_MSG_T msg = {0,0,0,NULL};
	msg.arg2 = msgLen;
	msg.arg3 = msgPtr;
	
	if(timeout > 0)
	{
		os_status = sAPI_MsgQSendSuspend(queueRef->queueId,&msg,timeout);
		if(os_status != SC_SUCCESS)
		{
			BoatLog(BOAT_LOG_NORMAL, "[boat][queue]  queue put failed!\r\n");
			return BOAT_ERROR;
		}
	
	}
	else
	{
		os_status = sAPI_MsgQSend(queueRef->queueId,&msg);
		if(os_status != SC_SUCCESS)
		{
			BoatLog(BOAT_LOG_NORMAL, "[boat][queue]  queue put failed!\r\n");
			return BOAT_ERROR;
		}
	
	}
	
	return BOAT_SUCCESS;
}

BOAT_RESULT boatQueueReceive(const boatQueue *queueRef, BUINT8 *msgPtr, BUINT32 msgLen ,BUINT32 timeout)
{
	SC_STATUS os_status;

	if(queueRef == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue address is illegal in boatQueueInit, bad address:%x,ph",(BUINT32)queueRef);
    	return BOAT_ERROR;
    }

	if(queueRef->queueId == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue queueRef is 0 in boatQueueDelete, bad address:%x,ph",(BUINT32)queueRef);
    	return BOAT_ERROR;///// BOAT_DATA_INVALID
    }

	//! msgPtr check
	if(msgPtr == NULL)
	{
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]The boatQueue msgPtr is NULL in boatQueueSend");
		return BOAT_ERROR;
	}

	 SIM_MSG_T msg = {0,0,0,NULL};
	 msg.arg2 = msgLen;
	 msg.arg3 = msgPtr;

	os_status = sAPI_MsgQRecv(queueRef->queueId,&msg,timeout);
	if(os_status != SC_SUCCESS)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue]  queue get failed!\r\n");
		return BOAT_ERROR;///// We do not know witch error is, timeout or error
	}
	
	return BOAT_SUCCESS;
}

BOAT_RESULT boatQueueInitQueueIdZero(boatQueue *queueRef)
{
    if(queueRef != NULL)  
    {
        queueRef->queueId = NULL;
    	return BOAT_SUCCESS;
    }
    return BOAT_ERROR;
}


