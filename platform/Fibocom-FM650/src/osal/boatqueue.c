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
#include <sys/time.h>

#include "boatosal.h"
#include "boatlog.h"

/////BOAT_RESULT boatQueueInit(boatQueue *queueRef, char *queueName, BUINT32 maxSize, BUINT32 maxNumber);
BOAT_RESULT boatQueueInit(boatQueue *queueRef, char *queueName, BUINT32 maxSize, BUINT32 maxNumber){
    struct mq_attr mqattr;    // use the deafult oflag , blocked 
    int ret = 0;
    char name[256];

    if((queueRef == NULL) || (queueName == NULL) || (maxSize == 0) || (maxNumber == 0)) // CHECK PARAMETERS
    {
		BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:	 mq_open parameter err[%p==NULL] [%p==NULL] [%d==0] [%d==0]\r\n",queueRef,queueName, maxSize, maxNumber);
        return BOAT_ERROR;
    }
    if(queueName[0] != '/')
    {
        if(strlen(queueName) < 256-1)
        {
            memset(name,0,sizeof(name));
            memcpy(name+1,queueName,strlen(queueName));
            name[0] = '/';
            queueName = name;
        }
        else
        {
            return BOAT_ERROR;
        }
    }
    BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_open queueName[%p] [%p] [%s] \r\n",queueName,name,queueName);
    

    mqattr.mq_maxmsg = maxNumber;
    mqattr.mq_msgsize = maxSize;
    
    queueRef->queueId = mq_open(queueName, O_CREAT | O_EXCL | O_RDWR , 0666 , &mqattr);
    if(queueRef->queueId == -1) // OPEN FAILED
    {
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_open errno %02x \r\n",errno);
        
        if(errno == EEXIST)    // queue name is exist
        {
            queueRef->queueId = mq_open(queueName, O_RDWR);
            if(queueRef->queueId == -1) // OPEN FAILED
            {
                ret = mq_unlink(queueName);
                if(ret == -1)
                {
                    BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_unlink[%s] errno [%d][%02x] \r\n",queueName,errno,errno);
                }
                else
                {
                    BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_unlink[%s] succ\r\n",queueName);
                }
                /////return BOAT_ERROR_QUEUE - EEXIST;
                return BOAT_ERROR;                
            }
        }
        // name slash error
        else
        {
            BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_open errno name[%s] id[%p] [%d][%02x] \r\n",queueName,queueRef,errno,errno);
            return BOAT_ERROR;
        }
    }
    BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_open succ [%d][%02x]\r\n",queueRef->queueId,queueRef->queueId);
    
    queueRef->name = (char *)calloc(1,(size_t)(strlen(queueName)+1)); // strlen is ok because of mq_open returnning is right
    if(queueRef->name == NULL)    // calloc failed
    {
        ret = mq_close(queueRef->queueId);
        if(ret == -1)
        {
            BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_close errno %02x \r\n",errno);
        }
        else
        {
            BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_close succ\r\n");
        }

        ret = mq_unlink(queueName);
        if(ret == -1)
        {
            BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_unlink[%s] errno [%d][%02x] \r\n",queueRef->name,errno,errno);
        }
        else
        {
            BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_unlink[%s] succ\r\n",queueName);
        }

        return BOAT_ERROR;
    }
    //memset(queueRef->name,0,strlen(queueName)+1);
    memcpy(queueRef->name,queueName,strlen(queueName));    // no +1

    return 0;

}

/////BOAT_RESULT boatQueueDelete(boatQueue *queueRef);
BOAT_RESULT boatQueueDelete(boatQueue *queueRef)
{
    int ret = 0;
	if(queueRef == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]:    boatQueueDelete The boatQueue is null");
    	return BOAT_ERROR;
    }

    if((queueRef->queueId == 0) || (queueRef->name == NULL))    // parameter check
    {
		BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:	 boatQueueDelete parameter err queueId[%x==NULL] name[%x==NULL]\r\n",queueRef->queueId,*(int *)queueRef->name);
        return BOAT_ERROR;
    }
    ret = mq_close(queueRef->queueId);
    if(ret == -1)
    {
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_close errno %02x \r\n",errno);
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_close succ\r\n");
    }
    ret = mq_unlink(queueRef->name);
    if(ret == -1)
    {
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_unlink[%s] errno [%d][%02x] \r\n",queueRef->name,errno,errno);
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    mq_unlink[%s] succ\r\n",queueRef->name);
    }
    free(queueRef->name);
    queueRef->queueId = -1;
    queueRef->name = NULL;

    return 0;
}

BOAT_RESULT boatQueueSend(const boatQueue *queueRef, unsigned char *msgPtr, BUINT32 msgLen, BUINT32 timeout)
{
    int ret;
    struct mq_attr mqattr;
	if(queueRef == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]:    boatQueueDelete The boatQueue is null");
    	return BOAT_ERROR;
    }

    if((queueRef->queueId <= 0) || (queueRef->name == NULL) || (msgPtr == NULL))    // parameter check
    {
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    BoAT_msgqueue_send parameter err [%d][%s][%s]\r\n",queueRef->queueId,queueRef->name,(char *)msgPtr);
        return BOAT_ERROR;
    }
    ret = mq_getattr(queueRef->queueId,&mqattr);// get the maxsize of currrent mq
    if(ret == -1)
    {
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    BoAT_msgqueue_send mq_getattr() err \r\n");
        return BOAT_ERROR; // NEED ERRCODE IN boaterrcode.h
    }
    if(msgLen > mqattr.mq_msgsize)    // send or not ?
    {
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    BoAT_msgqueue_send mq_msgsize err[%d][%ld] \r\n",msgLen,mqattr.mq_msgsize);
        return BOAT_ERROR;// NEED ERRCODE IN boaterrcode.h oversize
    }
    
    if(timeout == 0)
    {
		ret = mq_send(queueRef->queueId,(char *)msgPtr,msgLen,0); // use the same priority
    }
    else
    {
		struct timespec tm;
	    tm.tv_sec = time(NULL) + timeout / 1000;
        tm.tv_nsec = (timeout % 1000) * 1000 * 1000;
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    boatQueueSend tm[%ld][%ld]time[%ld]timeout[%d]\r\n",tm.tv_sec,tm.tv_nsec,time(NULL),timeout);
        ret = mq_timedsend(queueRef->queueId, (char *)msgPtr, msgLen, 0, &tm);    // use the same priority 0 as mq_send ,blocked
    }

    if(ret == -1)
    {
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    BoAT_msgqueue_send mq_send{errno[%d][%02x]} \r\n",errno,errno);
        if(errno == EAGAIN )
            return BOAT_ERROR; // NEED ERRCODE IN boaterrcode.h
        return BOAT_ERROR; // NEED ERROCDE IN boaterrcode.h
    }
    
    return 0;
}

BOAT_RESULT boatQueueReceive(const boatQueue *queueRef, BUINT8 *msgPtr, BUINT32 msgLen ,BUINT32 timeout)
{
    int ret;
    unsigned int rvprio = 0;
 	if(queueRef == NULL)
    {
    	BoatLog(BOAT_LOG_NORMAL, "[boat][queue]:    boatQueueDelete The boatQueue is null");
    	return BOAT_ERROR;
    }

    if((queueRef->queueId <= 0) || (queueRef->name == NULL) || (msgPtr == NULL))    // parameter check
    {
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    boatQueueReceive parameter err [%d][%s][%p]\r\n",queueRef->queueId,queueRef->name,msgPtr);
        return BOAT_ERROR;
    }
    if(timeout == 0)
    {
        ret = mq_receive(queueRef->queueId,(char *)msgPtr,msgLen,NULL);    // use the same priority 0 as mq_send ,blocked
    }
    else
    {
		struct timespec tm;
	    tm.tv_sec = time(NULL) + timeout / 1000;
        tm.tv_nsec = (timeout % 1000) * 1000 * 1000;
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    boatQueueReceive tm[%ld][%ld]time[%ld]timeout[%d]\r\n",tm.tv_sec,tm.tv_nsec,time(NULL),timeout);
        ret = mq_timedreceive(queueRef->queueId,(char *)msgPtr,msgLen,&rvprio,&tm);    // use the same priority 0 as mq_send ,blocked
    }

    if(ret == -1)
    {
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    boatQueueReceive err [%d][%02x]\r\n",errno,errno);
        BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    boatQueueReceive parameter [%d][%s][%p]\r\n",queueRef->queueId,queueRef->name,msgPtr);
        if(errno == EAGAIN )
            return BOAT_ERROR; // NEED ERRCODE IN boaterrcode.h
        return BOAT_ERROR; // NEED ERROCDE IN boaterrcode.h
    }
	BoatLog(BOAT_LOG_CRITICAL,"[boat][queue]:    boatQueueReceive receive successful\r\n");
    return 0;
}

BOAT_RESULT boatQueueInitQueueIdZero(boatQueue *queueRef)
{
    if(queueRef != NULL)  
    {
        queueRef->queueId = 0;
        queueRef->name = "my";
    	return BOAT_SUCCESS;
    }
    return BOAT_ERROR;
}

BOAT_RESULT boatQueueInitQueueNameNULL(boatQueue *queueRef)
{
    if(queueRef != NULL)  
    {
        queueRef->queueId = 1;
        queueRef->name = NULL;
    	return BOAT_SUCCESS;
    }
    return BOAT_ERROR;
}


