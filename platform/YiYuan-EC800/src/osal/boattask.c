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

BOAT_RESULT boatTaskCreate(boatTask *taskRef, char *taskName, BUINT32 stackSize, BUINT32 priority, void (*taskfunc)(void *), void *argv)
//QlOSStatus ql_rtos_task_create (ql_task_t *taskRef, u32 stackSize, u8 priority, char *taskName, void (*taskStart)(void*), void* argv);
{
	return ql_rtos_task_create (&taskRef->taskId, stackSize, priority, taskName, taskfunc, argv, 0);
}

BOAT_RESULT boatTaskDelete(boatTask *taskRef)
//QlOSStatus ql_rtos_task_delete (ql_task_t taskRef);

{
	return ql_rtos_task_delete(taskRef->taskId);
}

void boatTaskInitTaskidNagtive(boatTask *Task)
{
	Task->taskId = NULL;	
}
