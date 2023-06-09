
#include "boatosal.h"

#ifdef PLATFORM_OSAL_QUEUE

#include "stdio.h"
#include "cm_os.h"

#include "boattypes.h"
#include "boatlog.h"


/*!*****************************************************************************
@brief Init a Boat queue

Function: boatQueueInit()

    This function create a queue instance and init the boat queue.

    boatQueueInit() MUST be called before any use of BoAT queue.
    boatQueueDelete() MUST be called after use of BoAT queue.

@return
    This function returns BOAT_SUCCESS if the initialization is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.
    

@param[in] msgQRef
    A boatQueue struct pointer,the boatQueueInit() function shall \n
    initialize the boat queue referred to by msgQRef.

@param[in] queueName
    The name of the initialized boat queue.

@param[in] maxSize
    The maxSize is the max size of one message.

@param[in] maxNumber
    The maxNumber is the max amount of messages on queue.

*******************************************************************************/
BOAT_RESULT boatQueueInit(boatQueue *msgQRef, char *queueName, BUINT32 maxSize, BUINT32 maxNumber)
{
    if(msgQRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(queueName == NULL)
    {
        msgQRef->queueId = osMessageQueueNew(maxNumber,maxSize,NULL);
    }
    else
    {
        osMessageQueueAttr_t queue_Attr;
        queue_Attr.name = queueName;
        msgQRef->queueId = osMessageQueueNew(maxNumber,maxSize,&queue_Attr);
    }

    if(msgQRef->queueId == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Queue init failed!");
        return BOAT_ERROR;
    }

    msgQRef->maxSize = maxSize;

    return BOAT_SUCCESS;

}


/*!*****************************************************************************
@brief Destroy a Boat queue

Function: boatQueueDelete()

    This function destroys a queue instance and deinit the boat queue.

@return
    This function returns BOAT_SUCCESS if the destroying is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.    

@param[in] msgQRef
    A boatQueue struct pointer,the boatQueueDelete() function shall \n
    destroy the boat queue indicated by msgQRef.

*******************************************************************************/
BOAT_RESULT boatQueueDelete(boatQueue *msgQRef)
{
    osStatus_t status;

    if(msgQRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if((msgQRef->queueId == NULL) || \
        (msgQRef->maxSize == 0))
    {
        BoatLog(BOAT_LOG_CRITICAL,"Invalid parameter");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    status = osMessageQueueDelete(msgQRef->queueId);
    if(status != osOK)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Queue delete failed!");
        return BOAT_ERROR;
    }

    msgQRef->queueId = NULL;
    msgQRef->maxSize = 0U;

    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief Send data to a boat queue

Function: boatQueueSend()

     The boatQueueSend() function shall send a message to the Boat queue \n
     referenced by msgQRef. 

@return
    This function returns BOAT_SUCCESS if the send operation is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to indicate the error,\n
    for reference in boaterrcode.h.    

@param[in] msgQRef
    A boatQueue struct pointer,the boatQueueSend() function shall send \n
    data to the boat queue referred to by the boat queue descriptor msgQRef.

@param[in] msgPtr
    The msgPtr pointed to the start address of the data which will be put into\n
    the boat queue.

@param[in] msgLen
    The msgLen specifies the length (by byte) of the data pointed to by msg_ptr.
    The msgLen can not be great than the maxSize which set in boatQueueInit()

@param[in] timeout 
    The timeout specifies a limit on the amount of time (in milliseconds) that the call\n
    should block if the sending cannot be immediately performed.


*******************************************************************************/
BOAT_RESULT boatQueueSend(const boatQueue *msgQRef, BUINT8 *msgPtr, BUINT32 msgLen, BUINT32 timeout)
{
    osStatus_t status;

    if((msgQRef == NULL) || \
        (msgPtr == NULL) || \
            (msgLen == 0))
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if((msgQRef->queueId == NULL) || \
        (msgQRef->maxSize == 0))
    {
        BoatLog(BOAT_LOG_CRITICAL,"Invalid parameter");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(msgQRef->maxSize < msgLen)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Msg is too long");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    status = osMessageQueuePut(msgQRef->queueId,msgPtr,0,timeout*200/1000);
    if(status != osOK)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Queue send failed!");
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief Receive data from a boat queue

Function: boatQueueReceive()

     The boatQueueReceive() function shall receive data from the Boat queue
     referenced by msgQRef. 

@return
    This function returns BOAT_SUCCESS if the operation is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.\n\n

       If the boatQueueReceive() could not receive data frome the queue before\n
       the specified timeout expired, then boatQueueReceive() return a timeout\n
       error (BOAT_ERROR_TIMEOUT)

@param[in] msgQRef
    A boatQueue struct pointer,the boatQueueReceive() function shall receive\n
    data from the boat queue referred to by msgQRef.

@param[in] msgPtr
    The msg_ptr points to the buffer which is used to place the data received\n
    from the boat queue.

@param[in] msgLen
    The msgLen specifies the length (by byte) of the data received form a\n
    boat queue.
    Fibocom API does not use this parameter
    As default, usr must define the msgPtr buffer which size equal the maxSize set in
    boatQueueInit. boatQueueReceive() will copy all data of one message.

@param[in] timeout 
    The timeout specifies a limit on the amount of time (in milliseconds) that the call\n
    should block if the receiving cannot be immediately performed.

*******************************************************************************/
BOAT_RESULT boatQueueReceive(const boatQueue *msgQRef, BUINT8 *msgPtr, BUINT32 msgLen ,BUINT32 timeout)
{
    osStatus_t status;

    if((msgQRef == NULL) || \
        (msgPtr == NULL) || \
            (msgLen == 0))
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if((msgQRef->queueId == NULL) || \
        (msgQRef->maxSize == 0))
    {
        BoatLog(BOAT_LOG_CRITICAL,"Invalid parameter");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(msgQRef->maxSize < msgLen)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Msg is too long");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    status = osMessageQueueGet(msgQRef->queueId,msgPtr,NULL,timeout*200/1000);
    if(status != osOK)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Queue receive failed!");
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}

#endif/////PLATFORM_OSAL_QUEUE