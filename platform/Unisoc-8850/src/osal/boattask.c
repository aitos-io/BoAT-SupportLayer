#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include "boatosal.h"
#include "boatlog.h"
#include "boattypes.h"

#include "osi_api.h"

BOAT_RESULT boatTaskCreat(boatTask *taskRef, char *taskName, BUINT32 stackSize, BUINT32 priority, void (*taskfunc)(void *), void *argv)
{
	pthread_t th;
    pthread_attr_t attr;
	int ret;

	(void)priority;
	(void)taskName;

    if (taskRef == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][task] boatTaskCreat bad taskRef\r\n");
        return BOAT_ERROR;
    }
    if (taskfunc == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][task] boatTaskCreat bad taskfunc\r\n");
        return BOAT_ERROR;
    }

    osiThread_t * threadId = osiThreadCreate(taskName, taskfunc, argv, priority, stackSize, 0);
    if(threadId == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][task] boatTaskCreat create failed\r\n");
        return BOAT_ERROR;
    }

    taskRef->taskId = threadId;

    return BOAT_SUCCESS;
	
}

BOAT_RESULT boatTaskDelete(boatTask *taskRef)
{
	//taskRef->taskId = -1;
    taskRef->taskId = NULL;
	return BOAT_SUCCESS;
}

