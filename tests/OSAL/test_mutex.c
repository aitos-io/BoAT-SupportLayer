#include "boatOSAL.h"
#include "boatlog.h"
#include "boattypes.h"
#include "fibo_opencpu.h"



BOAT_RESULT boatMutexInitMutexidZero(boatMutex *mutex);
boatMutex testMutexId;


void boatAssert_int_eq(BOAT_RESULT rtnVal, BOAT_RESULT eqVal)
{
	/////fibo_taskSleep(500);
	if(rtnVal != eqVal)
	{
		BoatPrintf(0,"[boat]Boat Assert the rtnVal:%d not equal %d, test failed\r\n",rtnVal,eqVal);
	}
	else
	{
		BoatPrintf(0,"[boat]Boat Assert the rtnVal:%d equal %d, test successful\r\n",rtnVal,eqVal);
	}
		
}

void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0010InitMutexSuccess(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0010InitMutexSuccess\r\n");
	
	rtnVal = boatMutexInit(&testMutexId);
	boatAssert_int_eq(rtnVal,0);
	
	boatMutexDestroy(&testMutexId);
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0010InitMutexSuccess finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0011InitMutexFailMutexAddrNULL(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0011InitMutexFailMutexAddrNULL\r\n");
	
	rtnVal = boatMutexInit(0);	///// input 0 to test retrun -1
	boatAssert_int_eq(rtnVal,-1);
	
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0011InitMutexFailMutexAddrNULL finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0012InitMutexFailMutexCreateReturnZero(void)
{
	BoatPrintf(0,"Try test_BoAT_OSAL_FibocomL610_01Mutex_test_0029InitMutexGetLimit to test \
		 test_BoAT_OSAL_FibocomL610_01Mutex_test_0012InitMutexFailMutexCreateReturnZero\r\n");
	
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0012InitMutexFailMutexCreateReturnZero finished\r\n");
	return 0;
}


void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0013DeinitMutexSuccess(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0013DeinitMutexSuccess\r\n");
	
	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexDestroy(&testMutexId);
	boatAssert_int_eq(rtnVal,0);
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0013DeinitMutexSuccess finished\r\n");
	return 0;

}

void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0014DeinitMutexFailMutexAddrNULL(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0014DeinitMutexFailMutexAddrNULL\r\n");

	
	rtnVal = boatMutexDestroy(0);	///// test 0 
	boatAssert_int_eq(rtnVal,-1);
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0014DeinitMutexFailMutexAddrNULL finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0015DeinitMutexFailMutexidEqualZero(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0015DeinitMutexFailMutexidEqualZero\r\n");
	
	
	rtnVal = boatMutexInitMutexidZero(&testMutexId);	///// Mutexid =  0;
	
	rtnVal = boatMutexDestroy(&testMutexId);	
	boatAssert_int_eq(rtnVal,-1);
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0015DeinitMutexFailMutexidEqualZero finished\r\n");
	return 0;
}



void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0017MutexLockSuccess(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0017MutexLockSuccess\r\n");
	
	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexLock(&testMutexId,0);	///// test lock 
	boatAssert_int_eq(rtnVal,0);

	/////rtnVal = boatMutexUnlock(&testMutexId);	///// test unlock 

	rtnVal = boatMutexDestroy(&testMutexId);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0017MutexLockSuccess finished\r\n");
	return 0;
}


void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0018MutexTimedLockSuccess(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0018MutexTimedLockSuccess\r\n");
	
	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexLock(&testMutexId,1);	///// test timed lock 
	boatAssert_int_eq(rtnVal,0);

	/////rtnVal = boatMutexUnlock(&testMutexId);	///// test unlock 

	rtnVal = boatMutexDestroy(&testMutexId);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0018MutexTimedLockSuccess finished\r\n");
	return 0;
}


void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0019MutexLockFailMutexAddrNULL(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0019MutexLockFailMutexAddrNULL\r\n");
	
	rtnVal = boatMutexLock(0,0);	///// test lock 
	boatAssert_int_eq(rtnVal,-1);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0019MutexLockFailMutexAddrNULL finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0020MutexLockFailMutexidEqualZero(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0020MutexLockFailMutexidEqualZero\r\n");
	
	rtnVal = boatMutexInitMutexidZero(&testMutexId);	///// create a mutexID = 0
	
	rtnVal = boatMutexLock(&testMutexId,0);	///// test lock 
	boatAssert_int_eq(rtnVal,-1);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0020MutexLockFailMutexidEqualZero finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0021MutexTimedLockFailMutexAddrNULL(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0021MutexTimedLockFailMutexAddrNULL\r\n");
	
	rtnVal = boatMutexLock(0,1);	///// test timed lock 
	boatAssert_int_eq(rtnVal,-1);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0021MutexTimedLockFailMutexAddrNULL finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0022MutexTimedLockFailMutexidEqualZero(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0022MutexTimedLockFailMutexidEqualZero\r\n");
	
	rtnVal = boatMutexInitMutexidZero(&testMutexId);	///// create a mutexID = 0
	
	rtnVal = boatMutexLock(&testMutexId,1);	///// test timed lock 
	boatAssert_int_eq(rtnVal,-1);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0022MutexTimedLockFailMutexidEqualZero finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0023MutexTimedLockFailTimeout(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0023MutexTimedLockFailTimeout\r\n");
	
	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexLock(&testMutexId,0);	///// test lock 
	rtnVal = boatMutexLock(&testMutexId,1);	///// test timed lock 
	boatAssert_int_eq(rtnVal,-1);			///// the rtnVal must check!

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0023MutexTimedLockFailTimeout timeout [%x]\r\n",rtnVal);
	/////rtnVal = boatMutexUnlock(&testMutexId);	///// test unlock 

	rtnVal = boatMutexDestroy(&testMutexId);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0023MutexTimedLockFailTimeout finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0024MutexUnlockSuccess(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0024MutexUnlockSuccess\r\n");
	

	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexLock(&testMutexId,0);	///// test lock 

	rtnVal = boatMutexUnlock(&testMutexId);	///// test unlock 
	boatAssert_int_eq(rtnVal,0);

	rtnVal = boatMutexDestroy(&testMutexId);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0024MutexUnlockSuccess finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0025MutexUnlockFailMutexAddrNULL(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0025MutexUnlockFailMutexAddrNULL\r\n");


	rtnVal = boatMutexUnlock(0);	///// test unlock 
	boatAssert_int_eq(rtnVal,-1);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0025MutexUnlockFailMutexAddrNULL finished\r\n");
	return 0;
}

void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0026MutexUnlockFailMutexidEqualZero(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0026MutexUnlockFailMutexidEqualZero\r\n");
	
	rtnVal = boatMutexInitMutexidZero(&testMutexId);	///// create a mutexID = 0
	
	rtnVal = boatMutexUnlock(&testMutexId);	
	boatAssert_int_eq(rtnVal,-1);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0026MutexUnlockFailMutexidEqualZero finished\r\n");
	return 0;
}


// 多次打开关闭锁查看其ID 变化
void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0027ReuseSingleMutex(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0027ReuseSingleMutex\r\n");
	
	for (int i = 0;i < 15;i++)
	{
		//fibo_taskSleep(900);
		BoatPrintf(0,"Testing mutex times[%d]\r\n",i);
		fibo_taskSleep(800);
		rtnVal = boatMutexInit(&testMutexId);
		//boatAssert_int_eq(rtnVal,0);
		rtnVal = boatMutexLock(&testMutexId,0);
		rtnVal = boatMutexDestroy(&testMutexId);
		boatAssert_int_eq(rtnVal,0);
	}
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0027ReuseSingleMutex finished\r\n");
	return 0;
}

static void mutex_task_lock_sleep_2s(void *param)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing mutex task lock id[%x]\r\n",param);
	boatMutex *ptestMutexId = (boatMutex *)param;

	for (int i = 0;i < 5;i++)
	{
		fibo_taskSleep(1900);
		BoatPrintf(0,"Testing mutex task lock times[%d] id[%x]\r\n",i,ptestMutexId);
		//fibo_taskSleep(800);
		//rtnVal = boatMutexInit(ptestMutexId);
		//boatAssert_int_eq(rtnVal,0);
		rtnVal = boatMutexLock(ptestMutexId,0);
		fibo_taskSleep(2000);///// sleep 2 seconds
		rtnVal = boatMutexUnlock(ptestMutexId);
		//rtnVal = boatMutexDestroy(ptestMutexId);
		BoatPrintf(0,"mutex_task_lock_sleep_2s unlock rtnVal[%x]\r\n",rtnVal);
		
	}
	BoatPrintf(0,"Testing mutex_task_lock_sleep_2s finished\r\n");
	fibo_taskSleep(1000);///// sleep 2 seconds

	fibo_thread_delete();

	return ;	
}

void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0028MultiTaskUseOneMutex(void)
{
	
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0028MultiTaskUseOneMutex\r\n");

	boatMutexInit(&testMutexId);

	fibo_thread_create(mutex_task_lock_sleep_2s, "mutex_sleep_2s", 1024 * 8, (void *)&testMutexId, OSI_PRIORITY_NORMAL);
	
	for (int i = 0;i < 6;i++)
	{
		fibo_taskSleep(1900);
		BoatPrintf(0,"Testing mutex main lock times[%d]\r\n",i);
		boatMutexLock(&testMutexId,0);
		BoatPrintf(0,"Testing mutex main return from lock times[%d]\r\n",i);
		fibo_taskSleep(2000);///// sleep 2 seconds
		boatMutexUnlock(&testMutexId);
	}
	boatMutexDestroy(&testMutexId);
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0028MultiTaskUseOneMutex finished\r\n");
	return 0;
}


// 检查FIBOCOM 创建 mutex 的极限
void *test_BoAT_OSAL_FibocomL610_01Mutex_test_0029InitMutexGetLimit(void)
{
	BOAT_RESULT rtnVal;

	
	char *p=NULL;
	//if (p == NULL)
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0029InitMutexGetLimit [%x]\r\n",p);

	for (int i = 0;i < 50000;i++)
	{
		//fibo_taskSleep(900);
		fibo_taskSleep(5);
		p = fibo_malloc(0x1000);
		rtnVal = boatMutexInit(&testMutexId);
		BoatPrintf(0,"Testing mutex times[%04d][%d][%x][%x]\r\n",i,rtnVal,testMutexId.mutexID,p);
	}
	return NULL;
}

void testMutexEntry(void)
{
	
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0010InitMutexSuccess();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0011InitMutexFailMutexAddrNULL();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0012InitMutexFailMutexCreateReturnZero();
	fibo_taskSleep(1000);
	
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0013DeinitMutexSuccess();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0014DeinitMutexFailMutexAddrNULL();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0015DeinitMutexFailMutexidEqualZero();
	fibo_taskSleep(1000);

	
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0017MutexLockSuccess();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0018MutexTimedLockSuccess();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0019MutexLockFailMutexAddrNULL();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0020MutexLockFailMutexidEqualZero();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0021MutexTimedLockFailMutexAddrNULL();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0022MutexTimedLockFailMutexidEqualZero();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0023MutexTimedLockFailTimeout();
	fibo_taskSleep(1000);
	
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0024MutexUnlockSuccess();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0025MutexUnlockFailMutexAddrNULL();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0026MutexUnlockFailMutexidEqualZero();
	fibo_taskSleep(1000);

	test_BoAT_OSAL_FibocomL610_01Mutex_test_0027ReuseSingleMutex();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0028MultiTaskUseOneMutex();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_01Mutex_test_0029InitMutexGetLimit();
	

	return ;
}
