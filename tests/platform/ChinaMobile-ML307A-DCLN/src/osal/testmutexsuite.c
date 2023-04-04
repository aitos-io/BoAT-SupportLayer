#include "boatosal.h"
#include "boatlog.h"
#include "boattypes.h"
#include "fibo_opencpu.h"
#include "../../boatcheck/boatcheck.h"




BOAT_RESULT boatMutexInitMutexidZero(boatMutex *mutex);
boatMutex testMutexId;

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0010InitMutexSuccess)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0010InitMutexSuccess\r\n");
	
	rtnVal = boatMutexInit(&testMutexId);
	ck_assert_int_eq(rtnVal,0);
	
	boatMutexDestroy(&testMutexId);
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0010InitMutexSuccess finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0011InitMutexFailMutexAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0011InitMutexFailMutexAddrNULL\r\n");
	
	rtnVal = boatMutexInit(0);	///// input 0 to test retrun -1
	ck_assert_int_eq(rtnVal,-1);
	
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0011InitMutexFailMutexAddrNULL finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0012InitMutexFailMutexCreateReturnZero)
{
	BoatPrintf(0,"Try test_BoAT_OSAL_FibocomL610_01Mutex_test_0029InitMutexGetLimit to test \
		 test_BoAT_OSAL_FibocomL610_01Mutex_test_0012InitMutexFailMutexCreateReturnZero\r\n");
	
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0012InitMutexFailMutexCreateReturnZero finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0013DeinitMutexSuccess)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0013DeinitMutexSuccess\r\n");
	
	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexDestroy(&testMutexId);
	ck_assert_int_eq(rtnVal,0);
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0013DeinitMutexSuccess finished\r\n");
	

}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0014DeinitMutexFailMutexAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0014DeinitMutexFailMutexAddrNULL\r\n");

	
	rtnVal = boatMutexDestroy(0);	///// test 0 
	ck_assert_int_eq(rtnVal,-1);
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0014DeinitMutexFailMutexAddrNULL finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0015DeinitMutexFailMutexidEqualZero)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0015DeinitMutexFailMutexidEqualZero\r\n");
	
	
	rtnVal = boatMutexInitMutexidZero(&testMutexId);	///// Mutexid =  0;
	
	rtnVal = boatMutexDestroy(&testMutexId);	
	ck_assert_int_eq(rtnVal,-1);
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0015DeinitMutexFailMutexidEqualZero finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0017MutexLockSuccess)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0017MutexLockSuccess\r\n");
	
	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexLock(&testMutexId,0);	///// test lock 
	ck_assert_int_eq(rtnVal,0);

	/////rtnVal = boatMutexUnlock(&testMutexId);	///// test unlock 

	rtnVal = boatMutexDestroy(&testMutexId);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0017MutexLockSuccess finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0018MutexTimedLockSuccess)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0018MutexTimedLockSuccess\r\n");
	
	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexLock(&testMutexId,1);	///// test timed lock 
	ck_assert_int_eq(rtnVal,0);

	/////rtnVal = boatMutexUnlock(&testMutexId);	///// test unlock 

	rtnVal = boatMutexDestroy(&testMutexId);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0018MutexTimedLockSuccess finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0019MutexLockFailMutexAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0019MutexLockFailMutexAddrNULL\r\n");
	
	rtnVal = boatMutexLock(0,0);	///// test lock 
	ck_assert_int_eq(rtnVal,-1);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0019MutexLockFailMutexAddrNULL finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0020MutexLockFailMutexidEqualZero)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0020MutexLockFailMutexidEqualZero\r\n");
	
	rtnVal = boatMutexInitMutexidZero(&testMutexId);	///// create a mutexID = 0
	
	rtnVal = boatMutexLock(&testMutexId,0);	///// test lock 
	ck_assert_int_eq(rtnVal,-1);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0020MutexLockFailMutexidEqualZero finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0021MutexTimedLockFailMutexAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0021MutexTimedLockFailMutexAddrNULL\r\n");
	
	rtnVal = boatMutexLock(0,1);	///// test timed lock 
	ck_assert_int_eq(rtnVal,-1);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0021MutexTimedLockFailMutexAddrNULL finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0022MutexTimedLockFailMutexidEqualZero)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0022MutexTimedLockFailMutexidEqualZero\r\n");
	
	rtnVal = boatMutexInitMutexidZero(&testMutexId);	///// create a mutexID = 0
	
	rtnVal = boatMutexLock(&testMutexId,1);	///// test timed lock 
	ck_assert_int_eq(rtnVal,-1);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0022MutexTimedLockFailMutexidEqualZero finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0023MutexTimedLockFailTimeout)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0023MutexTimedLockFailTimeout\r\n");
	
	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexLock(&testMutexId,0);	///// test lock 
	rtnVal = boatMutexLock(&testMutexId,1);	///// test timed lock 
	ck_assert_int_eq(rtnVal,-1);			///// the rtnVal must check!

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0023MutexTimedLockFailTimeout timeout [%x]\r\n",rtnVal);
	/////rtnVal = boatMutexUnlock(&testMutexId);	///// test unlock 

	rtnVal = boatMutexDestroy(&testMutexId);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0023MutexTimedLockFailTimeout finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0024MutexUnlockSuccess)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0024MutexUnlockSuccess\r\n");
	

	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexLock(&testMutexId,0);	///// test lock 

	rtnVal = boatMutexUnlock(&testMutexId);	///// test unlock 
	ck_assert_int_eq(rtnVal,0);

	rtnVal = boatMutexDestroy(&testMutexId);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0024MutexUnlockSuccess finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0025MutexUnlockFailMutexAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0025MutexUnlockFailMutexAddrNULL\r\n");


	rtnVal = boatMutexUnlock(0);	///// test unlock 
	ck_assert_int_eq(rtnVal,-1);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0025MutexUnlockFailMutexAddrNULL finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0026MutexUnlockFailMutexidEqualZero)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0026MutexUnlockFailMutexidEqualZero\r\n");
	
	rtnVal = boatMutexInitMutexidZero(&testMutexId);	///// create a mutexID = 0
	
	rtnVal = boatMutexUnlock(&testMutexId);	
	ck_assert_int_eq(rtnVal,-1);

	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0026MutexUnlockFailMutexidEqualZero finished\r\n");
	
}
END_TEST


// 多次打开关闭锁查看其ID 变化
START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0027ReuseSingleMutex)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0027ReuseSingleMutex\r\n");
	
	for (int i = 0;i < 15;i++)
	{
		//fibo_taskSleep(900);
		BoatPrintf(0,"Testing mutex times[%d]\r\n",i);
		fibo_taskSleep(800);
		rtnVal = boatMutexInit(&testMutexId);
		//ck_assert_int_eq(rtnVal,0);
		rtnVal = boatMutexLock(&testMutexId,0);
		rtnVal = boatMutexDestroy(&testMutexId);
		ck_assert_int_eq(rtnVal,0);
	}
	BoatPrintf(0,"Testing test_BoAT_OSAL_FibocomL610_01Mutex_test_0027ReuseSingleMutex finished\r\n");
	
}
END_TEST

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
		//ck_assert_int_eq(rtnVal,0);
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

START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0028MultiTaskUseOneMutex)
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
	
}
END_TEST


// 检查FIBOCOM 创建 mutex 的极限
START_TEST(test_BoAT_OSAL_FibocomL610_01Mutex_test_0029InitMutexGetLimit)
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
END_TEST

Suite *makeMutextest_suite(void)
{
    /* Create Suite */
    Suite *sMutextest = suite_create("mutex_test");

    /* Create test cases */
    TCase *tcMutextest_api = tcase_create("mutex_test_api");

    /* Add a test case to the Suite */
    suite_add_tcase(sMutextest,tcMutextest_api);

    /* Test cases are added to the test set */

	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0010InitMutexSuccess);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0011InitMutexFailMutexAddrNULL);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0012InitMutexFailMutexCreateReturnZero);

	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0013DeinitMutexSuccess);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0014DeinitMutexFailMutexAddrNULL);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0015DeinitMutexFailMutexidEqualZero);

	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0017MutexLockSuccess);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0018MutexTimedLockSuccess);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0019MutexLockFailMutexAddrNULL);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0020MutexLockFailMutexidEqualZero);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0021MutexTimedLockFailMutexAddrNULL);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0022MutexTimedLockFailMutexidEqualZero);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0023MutexTimedLockFailTimeout);

	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0024MutexUnlockSuccess);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0025MutexUnlockFailMutexAddrNULL);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0026MutexUnlockFailMutexidEqualZero);

	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0027ReuseSingleMutex);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0028MultiTaskUseOneMutex);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_FibocomL610_01Mutex_test_0029InitMutexGetLimit);	

    return sMutextest;

}


int runMutexTests(void)
{

#if 1
    SRunner *sr = NULL;
    int failed_number = 0;

    Suite *suiteMutextest = makeMutextest_suite();

    sr = srunner_create(suiteMutextest);

    srunner_run_all(sr,0);


    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed_number;
#endif

	
}


