/**
****************************************************************************************
* @FilePath: boatTimer.c
* @Author: aitos
* @Date: 2022-12-22 16:10:46
* @LastEditors:
* @LastEditTime: 2022-12-22 16:10:46
* @Descripttion:
****************************************************************************************
*/
// #include "boat_timer.h"
#include "boatiotsdk.h"
#include "boatosal.h"
#include "boatlog.h"

#include "simcom_os.h"
#include "simcom_common.h"

#define BOAT_TASK_PRIORITY    120 

BOAT_RESULT boatTaskCreat(boatTask *taskRef, char *taskName, BUINT32 stackSize, BUINT32 priority, void (*taskfunc)(void *), void *argv)
/////BOAT_RESULT boat_thread_creat(void *pvTaskCode, INT8 *pcName, UINT32 usStackDepth, void *pvParameters, UINT32 uxPriority)
{

    if ((taskRef == NULL) || (taskName == NULL) || (stackSize == 0) || (taskfunc == NULL))
    {
        BoatLog(BOAT_LOG_NORMAL, "[boat][task] boatTaskCreat bad paramters\r\n");
        return BOAT_ERROR;
    }

    if(taskRef->stackPtr == NULL)
    {
        return BOAT_ERROR;
    }
    SC_STATUS os_status = sAPI_TaskCreate(&(taskRef->taskId),taskRef->stackPtr,stackSize,BOAT_TASK_PRIORITY,taskName,taskfunc,argv);
    if(os_status != SC_SUCCESS)
    {
        taskRef->taskId = NULL;
        return BOAT_ERROR;
    }
    
    return BOAT_SUCCESS;

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
    if(taskRef->taskId == NULL)
    {
        return BOAT_ERROR;
    }
    SC_STATUS os_status = sAPI_TaskDelete(taskRef->taskId); ///// no return value
    if(os_status != SC_SUCCESS)
    {
        return BOAT_ERROR;
    }
    return BOAT_SUCCESS;
}

void boatTaskInitTaskidNagtive(boatTask *Task)
{
	Task->taskId =NULL;	///// 230123 modified to -1
}
