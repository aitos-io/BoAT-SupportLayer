#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include "boatosal.h"
#include "boatlog.h"
#include "boattypes.h"

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

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    if (stackSize > PTHREAD_STACK_MIN)
    {
		printf("set stack size %d\n",stackSize);
		ssize_t st = (ssize_t )stackSize;
        ret = pthread_attr_setstacksize(&attr, st);
		printf ("setstacksize ret : %d , errno : %d %x\r\n", ret, errno, errno);
    }


    ret = pthread_create(&th, &attr, (void *)taskfunc, argv);
    if (ret != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "ERROR: Create queueTaskReceiveSleep2s error\n");
		return BOAT_ERROR;
    }
    else
    {
		taskRef->taskId = th;
        BoatLog(BOAT_LOG_NORMAL, "INFO: Create thread succ.\n");
		ret = pthread_attr_destroy(&attr);
		printf ("destroy ret : %d , errno : %d %x\r\n", ret, errno, errno);
		return BOAT_SUCCESS;
    }

	
}

BOAT_RESULT boatTaskDelete(boatTask *taskRef)
{
	//taskRef->taskId = -1;
	return BOAT_SUCCESS;
}

