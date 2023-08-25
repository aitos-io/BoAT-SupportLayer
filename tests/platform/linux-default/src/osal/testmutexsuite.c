#include <unistd.h>
#include "boatosal.h"
#include "boatlog.h"
#include "boattypes.h"
#include "check.h"


START_TEST(test_BoAT_OSAL_LinuxDefault_01Mutex_test_0010InitMutexSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0010InitMutexSuccess\r\n");
	boatMutex testMutexId;
	rtnVal = boatMutexInit(&testMutexId);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	boatMutexDestroy(&testMutexId);
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0010InitMutexSuccess finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_01Mutex_test_0011InitMutexFailMutexAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0011InitMutexFailMutexAddrNULL\r\n");
	
	rtnVal = boatMutexInit(NULL);	///// input 0 to test retrun -1
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0011InitMutexFailMutexAddrNULL finished\r\n");
	
}
END_TEST



START_TEST(test_BoAT_OSAL_LinuxDefault_01Mutex_test_0013DeinitMutexSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0013DeinitMutexSuccess\r\n");
	boatMutex testMutexId;
	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexDestroy(&testMutexId);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0013DeinitMutexSuccess finished\r\n");
	

}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_01Mutex_test_0014DeinitMutexFailMutexAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0014DeinitMutexFailMutexAddrNULL\r\n");
	
	rtnVal = boatMutexDestroy(NULL);	///// test 0 
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0014DeinitMutexFailMutexAddrNULL finished\r\n");
	
}
END_TEST


START_TEST(test_BoAT_OSAL_LinuxDefault_01Mutex_test_0017MutexLockSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0017MutexLockSuccess\r\n");
	boatMutex testMutexId;
	rtnVal = boatMutexInit(&testMutexId);	
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	
	rtnVal = boatMutexLock(&testMutexId,0);	///// test lock 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);


	rtnVal = boatMutexUnlock(&testMutexId);	///// test unlock 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatMutexDestroy(&testMutexId);

	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0017MutexLockSuccess finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_01Mutex_test_0018MutexTimedLockSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0018MutexTimedLockSuccess\r\n");
	boatMutex testMutexId;
	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexLock(&testMutexId,5);	///// test timed lock 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatMutexUnlock(&testMutexId);	///// test unlock 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatMutexDestroy(&testMutexId);

	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0018MutexTimedLockSuccess finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_01Mutex_test_0019MutexLockFailMutexAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0019MutexLockFailMutexAddrNULL\r\n");
	
	rtnVal = boatMutexLock(NULL,0);	///// test lock 
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);

	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0019MutexLockFailMutexAddrNULL finished\r\n");
	
}
END_TEST


START_TEST(test_BoAT_OSAL_LinuxDefault_01Mutex_test_0021MutexTimedLockFailMutexAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0021MutexTimedLockFailMutexAddrNULL\r\n");
	
	rtnVal = boatMutexLock(NULL,1000);	///// test timed lock 
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);

	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0021MutexTimedLockFailMutexAddrNULL finished\r\n");
	
}
END_TEST



START_TEST(test_BoAT_OSAL_LinuxDefault_01Mutex_test_0023MutexTimedLockSuccessTimeout)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0023MutexTimedLockFailTimeout\r\n");
	boatMutex testMutexId;
	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexLock(&testMutexId,5000);	///// test timed lock 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);			///// the rtnVal must check!

	rtnVal = boatMutexDestroy(&testMutexId);

	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0023MutexTimedLockFailTimeout finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_01Mutex_test_0024MutexTimedLockFailTimeout)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0023MutexTimedLockFailTimeout\r\n");
	boatMutex testMutexId;
	rtnVal = boatMutexInit(&testMutexId);	
	
	rtnVal = boatMutexLock(&testMutexId,5000);	///// test timed lock 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);			///// the rtnVal must check!

	rtnVal = boatMutexLock(&testMutexId,5000);	///// test timed lock 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);			///// the rtnVal must check!

	rtnVal = boatMutexDestroy(&testMutexId);

	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0023MutexTimedLockFailTimeout finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_01Mutex_test_0025MutexUnlockFailMutexAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0025MutexUnlockFailMutexAddrNULL\r\n");


	rtnVal = boatMutexUnlock(NULL);	///// test unlock 
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);

	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0025MutexUnlockFailMutexAddrNULL finished\r\n");
	
}
END_TEST




static void *mutex_task_lock_sleep_2s(void *param)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_VERBOSE,"Testing mutex task lock id[%p]\r\n",param);
	boatMutex *ptestMutexId = (boatMutex *)param;

	for (int i = 0;i < 5;i++)
	{
		usleep(2000);
		BoatLog(BOAT_LOG_VERBOSE,"Testing mutex task lock times[%d] id[%p]\r\n",i,ptestMutexId);
		//fibo_taskSleep(800);
		//rtnVal = boatMutexInit(ptestMutexId);
		//ck_assert_int_eq(rtnVal,0);
		rtnVal = boatMutexLock(ptestMutexId,0);
		sleep(2);
		rtnVal = boatMutexUnlock(ptestMutexId);
		//rtnVal = boatMutexDestroy(ptestMutexId);
		BoatLog(BOAT_LOG_VERBOSE,"mutex_task_lock_sleep_2s unlock rtnVal[%x]\r\n",rtnVal);
		
	}
	BoatLog(BOAT_LOG_VERBOSE,"Testing mutex_task_lock_sleep_2s finished\r\n");
	sleep(2);
	(void)rtnVal;


	return NULL;	
}

START_TEST(test_BoAT_OSAL_LinuxDefault_01Mutex_test_0028MultiTaskUseOneMutex)
{
	
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0028MultiTaskUseOneMutex\r\n");

	boatMutex testMutexId;

	boatMutexInit(&testMutexId);

	//fibo_thread_create(mutex_task_lock_sleep_2s, "mutex_sleep_2s", 1024 * 8, (void *)&testMutexId, OSI_PRIORITY_NORMAL);
	pthread_t th;
    pthread_attr_t attr;
											
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	pthread_create(&th,&attr,mutex_task_lock_sleep_2s,&testMutexId);
	
	for (int i = 0;i < 6;i++)
	{
		usleep(2000);
		BoatLog(BOAT_LOG_VERBOSE,"Testing mutex main lock times[%d]\r\n",i);
		boatMutexLock(&testMutexId,0);
		BoatLog(BOAT_LOG_VERBOSE,"Testing mutex main return from lock times[%d]\r\n",i);
		sleep(2);
		boatMutexUnlock(&testMutexId);
	}
	boatMutexDestroy(&testMutexId);
	BoatLog(BOAT_LOG_VERBOSE,"Testing test_BoAT_OSAL_LinuxDefault_01Mutex_test_0028MultiTaskUseOneMutex finished\r\n");
	
}
END_TEST


Suite *makeMutextest_suite(void)
{
    /* Create Suite */
    Suite *sMutextest = suite_create("mutex_test");

    /* Create test cases */
    TCase *tcMutextest_api = tcase_create("mutex_test_api");

	tcase_set_timeout(tcMutextest_api,100);

    /* Add a test case to the Suite */
    suite_add_tcase(sMutextest,tcMutextest_api);

    /* Test cases are added to the test set */

	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_LinuxDefault_01Mutex_test_0010InitMutexSuccess);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_LinuxDefault_01Mutex_test_0011InitMutexFailMutexAddrNULL);

	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_LinuxDefault_01Mutex_test_0013DeinitMutexSuccess);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_LinuxDefault_01Mutex_test_0014DeinitMutexFailMutexAddrNULL);


	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_LinuxDefault_01Mutex_test_0017MutexLockSuccess);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_LinuxDefault_01Mutex_test_0018MutexTimedLockSuccess);

	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_LinuxDefault_01Mutex_test_0019MutexLockFailMutexAddrNULL);

	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_LinuxDefault_01Mutex_test_0021MutexTimedLockFailMutexAddrNULL);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_LinuxDefault_01Mutex_test_0023MutexTimedLockSuccessTimeout);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_LinuxDefault_01Mutex_test_0024MutexTimedLockFailTimeout);
	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_LinuxDefault_01Mutex_test_0025MutexUnlockFailMutexAddrNULL);

	tcase_add_test(tcMutextest_api, test_BoAT_OSAL_LinuxDefault_01Mutex_test_0028MultiTaskUseOneMutex);	

    return sMutextest;

}
