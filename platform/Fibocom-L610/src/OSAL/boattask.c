/**
****************************************************************************************
* @FilePath: boatTimer copy.c
* @Author: aitos
* @Date: 2022-12-23 09:46:53
* @LastEditors:
* @LastEditTime: 2022-12-23 09:46:53
* @Descripttion:
****************************************************************************************
*/
/**
****************************************************************************************
* @FilePath: boatTask.c
* @Author: aitos
* @Date: 2022-12-22 16:10:46
* @LastEditors:
* @LastEditTime: 2022-12-22 16:10:46
* @Descripttion:
****************************************************************************************
*/
#include "boat_timer.h"
#include "boatiotsdk.h"
#include "boatosal.h"
#include "boatlog.h"


BOAT_RESULT boatTaskCreat(boatTask *taskRef, char *taskName, BUINT32 stackSize, BUINT32 priority, void (*taskfunc)(void *), void *argv)
/////BOAT_RESULT boat_thread_creat(void *pvTaskCode, INT8 *pcName, UINT32 usStackDepth, void *pvParameters, UINT32 uxPriority)
{
	BSINT32 threadid = 0;

	if(( taskRef == NULL) || (taskName == NULL) || (stackSize == 0) || (taskfunc == NULL))
	{
		BoatPrintf(0,"[boat][task] boatTaskCreat bad paramters\r\n");
		return BOAT_ERROR;
	}
	
	if(priority == BOAT_TASK_PRIORITY_HIGH)
		priority = OSI_PRIORITY_HIGH;
	else if(priority == BOAT_TASK_PRIORITY_NORMAL)
		priority = OSI_PRIORITY_NORMAL;
	else
		priority = OSI_PRIORITY_LOW;
/*	
	char pp[6];
	pp[0] = 'A';
	pp[1] = 'B';
	pp[2] = 'C';
	pp[3] = 'D';
	pp[4] = 'E';
	pp[5] = 0;
*/


	
    threadid = fibo_thread_create(taskfunc, taskName, stackSize, argv, priority);
	/////BoatPrintf(0,"[boat][task] pt boatTaskCreat [%s] priority[%x][%x]\r\n",pp,priority,(BUINT32)threadid);
	BoatPrintf(0,"[boat][task] tp boatTaskCreat TEST priority[%x][%x]\r\n",priority,(BUINT32)threadid);
	if(threadid == 0)
	{
		BoatPrintf(0,"[boat][task] boatTaskCreat SUCC id[%x]\r\n",threadid);
		taskRef->taskID = threadid;
		return BOAT_SUCCESS;
	}
	
	BoatPrintf(0,"[boat][task] boatTaskCreat Failed id[%x]\r\n",threadid);
	taskRef->taskID = threadid;	///// must be a nagtive integer
	return BOAT_ERROR;
	
/////    return fibo_thread_create(pvTaskCode, pcName, usStackDepth, pvParameters, uxPriority);
}

BOAT_RESULT boatTaskDelete(boatTask *taskRef)
/////void boat_thread_delete(void)
{
	if( taskRef == NULL)
	{
		BoatPrintf(0,"[boat][task] boatTaskDelete bad taskRef\r\n");
		return BOAT_ERROR;
	}
	/* ///// delete these code is better for L610 TASK application
	if( taskRef->taskID != 0)
	{
		BoatPrintf(0,"[boat][task] boatTaskDelete bad taskID\r\n");
		return BOAT_ERROR;
	}*/
	/////taskRef->taskID = -1;	///// set taskID to -1 , invalid value
    fibo_thread_delete();	///// no return value
    
	return BOAT_SUCCESS;
}

void boatTaskInitTaskidNagtive(boatTask *Task)
{
	Task->taskID =-1;	///// 230123 modified to -1
}

