#include "boatOSAL.h"
#include "boatlog.h"
#include "boattypes.h"
#include "fibo_opencpu.h"

#define MULTITASKNUM 4
boatTask testTaskId;
boatTask testTaskIdArray[MULTITASKNUM];
char *taskNameArray[MULTITASKNUM] = {"thread_multi1","thread_multi2","thread_multi3","thread_multi4"};

void boatTaskInitTaskidNagtive(boatTask *Task);
void boatAssert_int_gt(BOAT_RESULT rtnVal, BOAT_RESULT gtVal)
{
	fibo_taskSleep(500);
	if(rtnVal > gtVal)
	{
		BoatPrintf(0,"[boat]Boat Assert the rtnVal:%d is greater than %d, test Succ\r\n",rtnVal,gtVal);
	}
	else
	{
		BoatPrintf(0,"[boat]Boat Assert the rtnVal:%d is equal or less than %d, test Failed\r\n",rtnVal,gtVal);
	}
		
}


void boatAssert_int_eq(BOAT_RESULT rtnVal, BOAT_RESULT eqVal);

static void task_hello_thread(void *param)
{
	
	BoatPrintf(0,"[boat][task] [test] This is task_hello_thread\r\n");


	BoatPrintf(0,"[boat][task] This is task_hello_thread end\r\n");

	boatTaskDelete(&testTaskId);

	//fibo_thread_delete();	
}

static void task_test_delete_thread(void *param)
{
	int rtnVal = 0;
	BoatPrintf(0,"[boat][task] This is task_test_delete_thread\r\n");
	rtnVal = boatTaskDelete(&testTaskId);
	boatAssert_int_eq(rtnVal,0);	///// equal  0 , successful
	BoatPrintf(0,"[boat][task] This is task_test_delete_thread end\r\n");
}


void *test_BoAT_OSAL_FibocomL610_02Task_test_0010_boatTaskCreat_Successful_PriorityLow(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0010_boatTaskCreat_Successful_PriorityLow\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_LOW, task_hello_thread, NULL);
	fibo_taskSleep(1000);
	boatAssert_int_eq(rtnVal,0);	///// equal  0 , successful
	
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0010_boatTaskCreat_Successful_PriorityLow finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_02Task_test_0011_boatTaskCreat_Successful_PriorityNormal(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0011_boatTaskCreat_Successful_PriorityNormal\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_hello_thread, NULL);
	boatAssert_int_eq(rtnVal,0);	///// equal  0 , successful
	
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0011_boatTaskCreat_Successful_PriorityNormal finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_02Task_test_0012_boatTaskCreat_Successful_PriorityHigh(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0012_boatTaskCreat_Successful_PriorityHigh\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_HIGH, task_hello_thread, NULL);
	boatAssert_int_eq(rtnVal,0);	///// equal  0 , successful
	
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0012_boatTaskCreat_Successful_PriorityHigh finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_02Task_test_0013_boatTaskCreat_Failed_TaskAddrNULL(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0013_boatTaskCreat_Failed_TaskAddrNULL\r\n");
	
	rtnVal = boatTaskCreat(0, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_hello_thread, NULL);
	boatAssert_int_eq(rtnVal,-1);	///// equal -1 , error
	
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0013_boatTaskCreat_Failed_TaskAddrNULL finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_02Task_test_0014_boatTaskCreat_Failed_TaskNameNULL(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0014_boatTaskCreat_Failed_TaskNameNULL\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, NULL, 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_hello_thread, NULL);
	boatAssert_int_eq(rtnVal,-1);	///// equal -1 , error
	
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0014_boatTaskCreat_Failed_TaskNameNULL finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_02Task_test_0015_boatTaskCreat_Failed_TaskStackNULL(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0015_boatTaskCreat_Failed_TaskStackNULL\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 0, BOAT_TASK_PRIORITY_NORMAL, task_hello_thread, NULL);
	boatAssert_int_eq(rtnVal,-1);	///// equal -1 , error
	
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0015_boatTaskCreat_Failed_TaskStackNULL finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_02Task_test_0016_boatTaskCreat_Failed_TaskFuncNULL(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0016_boatTaskCreat_Failed_TaskFuncNULL\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, NULL, NULL);
	boatAssert_int_eq(rtnVal,-1);	///// equal -1 , error
	
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0016_boatTaskCreat_Failed_TaskFuncNULL finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_02Task_test_0017_boatTaskCreat_Failed_FibocomTaskidNagtive(void)
{
	BoatPrintf(0,"[boat][task] Try test_BoAT_OSAL_FibocomL610_02Task_test_0023_boatTaskCreat_Limit \
		to test test_BoAT_OSAL_FibocomL610_02Task_test_0017_boatTaskCreat_Failed_FibocomTaskidNagtive \r\n");	
	
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0017_boatTaskCreat_Failed_FibocomTaskidNagtive finished\r\n");
	return 0;
}


void *test_BoAT_OSAL_FibocomL610_02Task_test_0018_boatTaskDelete_Successful(void)
{

	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0018_boatTaskDelete_Successful \r\n");	
	
	boatTaskCreat(&testTaskId, "task_test_delete", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_test_delete_thread, NULL);
	
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0018_boatTaskDelete_Successful finished\r\n");
	return 0;

}

void *test_BoAT_OSAL_FibocomL610_02Task_test_0019_boatTaskDelete_Failed_TaskAddrEqualNULL(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0019_boatTaskDelete_Failed_TaskAddrEqualNULL\r\n");
	
	rtnVal = boatTaskDelete(0);
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,-1);	///// equal -1 , error
		
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0019_boatTaskDelete_Failed_TaskAddrEqualNULL finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_02Task_test_0020_boatTaskDelete_Failed_TaskidNagtive(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0020_boatTaskDelete_Failed_TaskidNagtive\r\n");
	boatTaskInitTaskidNagtive(&testTaskId);
	rtnVal = boatTaskDelete(&testTaskId);
	boatAssert_int_eq(rtnVal,-1);	///// equal -1 , error
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0020_boatTaskDelete_Failed_TaskidNagtive finished\r\n");
	return 0;
}


static void task_loop_sleep_500ms_loopnum(void *param)
{
	BSINT32 loopnum = 3;/////param;

	if(param != NULL)
	{
		loopnum = (BSINT32)param;
	}

	BoatPrintf(0,"[boat][task] Testing task_loop_sleep_500ms_loopnum &param[%x][%x]\r\n",&param,param);

	for (int i = 0;i < loopnum;i++)
	{
		BoatPrintf(0,"[boat][task] task_loop_sleep_500ms_loopnum[%x] loop [%d] [%x]\r\n",&param,i,loopnum);
		fibo_taskSleep(500);
		
	}
	BoatPrintf(0,"[boat][task] Testing task_loop_sleep_500ms_loopnum finished[%x]\r\n",&param);

	fibo_thread_delete();

	return ;	
}

static void task_loop_sleep_500ms_endlessloop(void *param)
{
	
	BoatPrintf(0,"[boat][task] Testing task_loop_sleep_500ms_endlessloop &param[%x][%x]\r\n",&param,param);
	
	while(1)
	{
		//BoatPrintf(0,"[boat][task] task_loop_sleep_500ms_endlessloop[%x] loop [%x]\r\n",&param,i++);
		fibo_taskSleep(50000);
		
	}
	BoatPrintf(0,"[boat][task] Testing task_loop_sleep_500ms_endlessloop finished\r\n");

	fibo_thread_delete();

	return ;	
}

void *test_BoAT_OSAL_FibocomL610_02Task_test_0021_MultiTaskUseOneTaskFunc(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0021_MultiTaskUseOneTaskFunc\r\n");

	for (int i = 0;i < MULTITASKNUM;i++)
	{
		
		//rtnVal = boatTaskCreat(&testTaskIdArray[i], taskNameArray[i], 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_loop_sleep_500ms_loopnum, 10);
		rtnVal = boatTaskCreat(&testTaskIdArray[i], "theSame", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_loop_sleep_500ms_loopnum, (void *)10);
		boatAssert_int_eq(rtnVal,0);	///// qeual 0
		fibo_taskSleep(200);

	}
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0021_MultiTaskUseOneTaskFunc finished\r\n");
	return 0;
}


// 检查FIBOCOM 创建 Task 的极限
void *test_BoAT_OSAL_FibocomL610_02Task_test_0023_boatTaskCreat_Limit(void)
{
	BOAT_RESULT rtnVal;

	char *p = NULL;

	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0023_boatTaskCreat_Limit [%x]\r\n",p);

	for (int i = 0;i < 100;i++)
	{
		p = fibo_malloc(0x8000);
		rtnVal = boatTaskCreat(&testTaskId, "task_endlessloop", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_loop_sleep_500ms_endlessloop, NULL);
		BoatPrintf(0,"[boat][task] [%04d]Testing Task create rtnVal[%d][%x][%x]\r\n",i,rtnVal,testTaskId.taskID,p);	
		if(rtnVal != 0)
		{
			break;
		}
		fibo_taskSleep(50);

	}
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_02Task_test_0023_boatTaskCreat_Limit finished\r\n");
	return NULL;
}

void testTaskEntry(void)
{

	test_BoAT_OSAL_FibocomL610_02Task_test_0010_boatTaskCreat_Successful_PriorityLow();

	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_02Task_test_0011_boatTaskCreat_Successful_PriorityNormal();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_02Task_test_0012_boatTaskCreat_Successful_PriorityHigh();
	fibo_taskSleep(1000);
	
	
	test_BoAT_OSAL_FibocomL610_02Task_test_0013_boatTaskCreat_Failed_TaskAddrNULL();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_02Task_test_0014_boatTaskCreat_Failed_TaskNameNULL();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_02Task_test_0015_boatTaskCreat_Failed_TaskStackNULL();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_02Task_test_0016_boatTaskCreat_Failed_TaskFuncNULL();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_02Task_test_0017_boatTaskCreat_Failed_FibocomTaskidNagtive();
	fibo_taskSleep(1000);

	test_BoAT_OSAL_FibocomL610_02Task_test_0018_boatTaskDelete_Successful();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_02Task_test_0019_boatTaskDelete_Failed_TaskAddrEqualNULL();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_02Task_test_0020_boatTaskDelete_Failed_TaskidNagtive();
	fibo_taskSleep(1000);

	test_BoAT_OSAL_FibocomL610_02Task_test_0021_MultiTaskUseOneTaskFunc();
	fibo_taskSleep(10000);

	/////test_BoAT_OSAL_FibocomL610_02Task_test_0023_boatTaskCreat_Limit(); ///// if malloc failed, the AT+SPREF= CMD cannot make the module running 
	                                                                        ///// download mode,must pull high edge on the USB_BOOT pin
	

	return ;
}

