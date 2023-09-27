#include "boatosal.h"
#include "boatlog.h"
#include "boattypes.h"
#include "app_azure_rtos.h"

#ifndef ROTSTASKSTACKTOTALSIZE
#define ROTSTASKSTACKTOTALSIZE 1024 * 50
#endif
#define RTOSSTACKPOOLINITED 1
static char rtosStackPoolState = 0;

#if defined ( __ICCARM__ )
#pragma data_alignment=4
#endif
__ALIGN_BEGIN static UCHAR  task_stack_byte_pool_buffer[ROTSTASKSTACKTOTALSIZE] __ALIGN_END;
static TX_BYTE_POOL task_stack_byte_pool;

struct taskFun{
	TX_THREAD  th;
	void (*taskfunc)(void *);
	void *argv;
};
typedef struct taskFun boatTaskFun;
#define MAXGBTF 5
boatTaskFun gbtf[MAXGBTF] = {0};

boatTaskFun *getgbtf(void)
{
	for(char i = 0; i < MAXGBTF; i++)
	{
		if(gbtf[i].taskfunc == NULL)
			return &gbtf[i];
	}
	return NULL;	
}


void taskTample(ULONG argv)
{
	boatTaskFun *pbtf;
	pbtf = (boatTaskFun *) argv; 
	if(pbtf->taskfunc != NULL)
	{
		boatTaskFun btf;
		btf = *pbtf;
		pbtf->argv = NULL;
		pbtf->taskfunc = NULL;
		btf.taskfunc(btf.argv);
	}
}
BOAT_RESULT boatTaskCreat(boatTask *taskRef, char *taskName, BUINT32 stackSize, BUINT32 priority, void (*taskfunc)(void *), void *argv)
{
	if(rtosStackPoolState != RTOSSTACKPOOLINITED)	// init task stack pool space
	{
		if (tx_byte_pool_create(&task_stack_byte_pool, "Nx App memory pool", task_stack_byte_pool_buffer, ROTSTASKSTACKTOTALSIZE) != TX_SUCCESS)
		{
			return BOAT_ERROR;// need a error number for stack pool initializing fail.
		}
		rtosStackPoolState = RTOSSTACKPOOLINITED;
	}
	
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

	boatTaskFun *btf;
	btf = getgbtf();
	if(btf == NULL) 
		return BOAT_ERROR;
	
	btf->taskfunc = taskfunc;
	btf->argv = argv;

	void *stackStart = NULL;
	if (tx_byte_allocate(&task_stack_byte_pool, (VOID **) &stackStart, stackSize, TX_NO_WAIT) != TX_SUCCESS)
	{
		return BOAT_ERROR;// need a error number for task stack allocating fail.
	}

	int ret;
	ret = tx_thread_create(&taskRef->taskId, taskName, taskTample, (ULONG)btf, stackStart, stackSize,
						priority, priority, //A value equal to the thread priority disables preemption-threshold.
						TX_NO_TIME_SLICE, TX_AUTO_START);

    //	ret = pthread_create(&th, &attr, (void *)taskfunc, argv);
    if (ret != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "ERROR: Create queueTaskReceiveSleep2s error\n");
		return BOAT_ERROR;
    }
    else
    {
		taskRef->stackPtr = stackStart;
        BoatLog(BOAT_LOG_NORMAL, "INFO: Create thread succ.\n");
		return BOAT_SUCCESS;
    }

	
}

BOAT_RESULT boatTaskDelete(boatTask *taskRef)
{
	tx_byte_release(taskRef->stackPtr);
	tx_thread_terminate(&taskRef->taskId);
	return BOAT_SUCCESS;
}

