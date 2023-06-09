/**
****************************************************************************************
* @FilePath: boattask.c
* @Author: aitos
* @Date: 2023-3-22 11:10:46
* @LastEditors:
* @LastEditTime: 2022-3-22 16:10:46
* @Descripttion:
****************************************************************************************
*/
// #include "boat_timer.h"
#include "boatiotsdk.h"
#include "boatosal.h"
#include "boatlog.h"

#include "cm_os.h"

BOAT_RESULT boatTaskCreat(boatTask *taskRef, char *taskName, BUINT32 stackSize, BUINT32 priority, void (*taskfunc)(void *), void *argv)
/////BOAT_RESULT boat_thread_creat(void *pvTaskCode, INT8 *pcName, UINT32 usStackDepth, void *pvParameters, UINT32 uxPriority)
{
    osThreadId_t  threadid = NULL;

    if ((taskRef == NULL) || (taskName == NULL) || (stackSize == 0) || (taskfunc == NULL))
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][task] boatTaskCreat bad paramters\r\n");
        return BOAT_ERROR;
    }

    if (priority == BOAT_TASK_PRIORITY_HIGH)
        priority = osPriorityHigh;
    else if (priority == BOAT_TASK_PRIORITY_NORMAL)
        priority = osPriorityNormal;
    else
        priority = osPriorityLow;
    /*
        char pp[6];
        pp[0] = 'A';
        pp[1] = 'B';
        pp[2] = 'C';
        pp[3] = 'D';
        pp[4] = 'E';
        pp[5] = 0;
    */
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name = taskName;
    app_task_attr.priority = priority;
    app_task_attr.stack_size = stackSize;

    threadid = osThreadNew(taskfunc,argv,&app_task_attr);
    /////BoatLog(BOAT_LOG_NORMAL,"[boat][task] pt boatTaskCreat [%s] priority[%x][%x]\r\n",pp,priority,(BUINT32)threadid);
    BoatLog(BOAT_LOG_NORMAL, "[boat][task] tp boatTaskCreat TEST priority[%x][%x]\r\n", priority, (BUINT32)threadid);
    if (threadid != NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][task] boatTaskCreat SUCC id[%x]\r\n", threadid);
		taskRef->taskId = threadid;
        return BOAT_SUCCESS;
    }

    BoatLog(BOAT_LOG_NORMAL, "[boat][task] boatTaskCreat Failed id[%x]\r\n", threadid);
	taskRef->taskId = threadid;	///// must be a nagtive integer
    return BOAT_ERROR;

    /////    return fibo_thread_create(pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority);
}

BOAT_RESULT boatTaskDelete(boatTask *taskRef)
/////void boat_thread_delete(void)
{
    if (taskRef == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][task] boatTaskDelete bad taskRef\r\n");
        return BOAT_ERROR;
    }

    osThreadId_t  threadid = NULL;
    threadid = osThreadGetId();
    osThreadTerminate(threadid);

    return BOAT_SUCCESS;
}

void boatTaskInitTaskidNagtive(boatTask *Task)
{
	Task->taskId = NULL;	
}
