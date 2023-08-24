#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "boatlog.h"
#include "check.h"
#include "boatosal.h"
#include "boattypes.h"
#include "boaterrcode.h"

START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0010InitSemSuccess)
{

	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0010InitSemSuccess\r\n");
	boatSem testSemId;
	
	rtnVal = boatSemInit(&testSemId,0);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	boatSemDestroy(&testSemId);
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0010InitSemSuccess finished\r\n");

}
END_TEST


START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0011InitSemFailSemAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0011InitSemFailSemAddrNULL\r\n");
	
	rtnVal = boatSemInit(NULL,0);	///// input 0 to test retrun -1
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0011InitSemFailSemAddrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0012InitSemFailSemCreateReturnZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0012InitSemFailSemCreateReturnZero\r\n");
	rtnVal = boatSemInit(0,2);	///// input 0 to test retrun -1
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0012InitSemFailSemCreateReturnZero finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0013DeinitSemSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0013DeinitSemSuccess\r\n");
	boatSem testSemId;
	rtnVal = boatSemInit(&testSemId,0);	
	
	rtnVal = boatSemDestroy(&testSemId);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0013DeinitSemSuccess finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0014DeinitSemFailSemAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0014DeinitSemFailSemAddrNULL\r\n");

	
	rtnVal = boatSemDestroy(0);	///// test 0 
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0014DeinitSemFailSemAddrNULL finished\r\n");

}
END_TEST





START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0017SemWaitSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0017SemWaitSuccess\r\n");
	boatSem testSemId;
	rtnVal = boatSemInit(&testSemId,1);	///// set initial number 1 , can call wait one time
	
	rtnVal = boatSemWait(&testSemId,0);	///// test wait 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	/////rtnVal = boatSemPost(&testSemId);	///// test post 

	rtnVal = boatSemDestroy(&testSemId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0017SemWaitSuccess finished\r\n");

}
END_TEST


START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0018SemTimedWaitSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0018SemTimedWaitSuccess\r\n");
	boatSem testSemId;
	rtnVal = boatSemInit(&testSemId,1);	////// init 1 , can wait one time
	
	rtnVal = boatSemWait(&testSemId,1);	///// test timed wait 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	/////rtnVal = boatSemPost(&testSemId);	///// test post 

	rtnVal = boatSemDestroy(&testSemId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0018SemTimedWaitSuccess finished\r\n");

}
END_TEST


START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0019SemWaitFailSemAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0019SemWaitFailSemAddrNULL\r\n");
	
	rtnVal = boatSemWait(0,0);	///// test wait 
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0019SemWaitFailSemAddrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0021SemTimedWaitFailSemAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0021SemTimedWaitFailSemAddrNULL\r\n");
	
	rtnVal = boatSemWait(0,1);	///// test timed wait 
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0021SemTimedWaitFailSemAddrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0022SemTimedWaitSuccessTimeOut)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0022SemTimedWaitSuccessTimeOut\r\n");
	boatSem testSemId;
	rtnVal = boatSemInit(&testSemId,0);	
	

	rtnVal = boatSemWait(&testSemId,5000);	///// test timed wait 

	ck_assert_int_eq(rtnVal,BOAT_ERROR);			///// the rtnVal must check!

	rtnVal = boatSemDestroy(&testSemId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0022SemTimedWaitSuccessTimeOut finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0023SemTimedWaitSuccessCountNoZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0023SemTimedWaitSuccessCountNoZero\r\n");
	boatSem testSemId;
	rtnVal = boatSemInit(&testSemId,1);	
	

	rtnVal = boatSemWait(&testSemId,5000);	
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);		

	rtnVal = boatSemDestroy(&testSemId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0023SemTimedWaitSuccessCountNoZero finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0024SemPostSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0024SemPostSuccess\r\n");
	
	boatSem testSemId;
	rtnVal = boatSemInit(&testSemId,0);	
	rtnVal = boatSemPost(&testSemId);	///// test post 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatSemDestroy(&testSemId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0024SemPostSuccess finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0025SemPostFailSemAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0025SemPostFailSemAddrNULL\r\n");


	rtnVal = boatSemPost(0);	///// test post 
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0025SemPostFailSemAddrNULL finished\r\n");

}
END_TEST

#if 0

START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0030SemTimedWaitFailSemIdInvalid)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0030SemTimedWaitFailSemIdInvalid\r\n");
	
	rtnVal = boatSemInitSemIdInvalid(&testSemId);	///// create a semID = 0
	
	rtnVal = boatSemWait(&testSemId,1000);	///// test timed wait 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0030SemTimedWaitFailSemIdInvalid finished\r\n");

}
END_TEST


START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0027SemWaitFailSemIdInvalid)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0027SemWaitFailSemIdInvalid\r\n");
	
	rtnVal = boatSemInitSemIdInvalid(&testSemId);	///// create a semID = 0
	
	rtnVal = boatSemWait(&testSemId,0);	///// test wait 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0027SemWaitFailSemIdInvalid finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0029SemPostFailSemIdInvalid)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0029SemPostFailSemIdInvalid\r\n");
	
	rtnVal = boatSemInitSemIdInvalid(&testSemId);	///// create a semID has been deleted
	
	rtnVal = boatSemPost(&testSemId);	
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0029SemPostFailSemIdInvalid finished\r\n");
}
END_TEST


START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0027ReuseSingleSem)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0027ReuseSingleSem\r\n");
	
	for (int i = 0;i < 15;i++)
	{
		//fibo_taskSleep(900);
		BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing sem times[%d]\r\n",i);
		fibo_taskSleep(800);
		rtnVal = boatSemInit(&testSemId,0);
		//ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
		rtnVal = boatSemWait(&testSemId,0);
		rtnVal = boatSemDestroy(&testSemId);
		ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0027ReuseSingleSem finished\r\n");

}
END_TEST
#endif

static void *semTaskWatiSleep2s(void *param)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem][task] Testing sem task wait id[%p]\r\n",param);

	struct semTestStruTask{
		void *p;
		int counter;
	} *semTest;

	semTest = (struct semTestStruTask *) param;
	boatSem *ptestSemId = semTest->p;/////(boatSem *)param;

	for (int i = 0;i < 5;i++)
	{

		usleep(50*1000);
		BoatLog(BOAT_LOG_NORMAL, "[boat][sem][task] Testing sem task wait times[%d] id[%p]\r\n",i,ptestSemId);
		rtnVal = boatSemWait(ptestSemId,0);
		semTest->counter --;
		rtnVal = boatSemPost(ptestSemId);
		sleep(2);
		//rtnVal = boatSemDestroy(ptestSemId);
		BoatLog(BOAT_LOG_NORMAL, "[boat][sem][task] semTaskWatiSleep2s post rtnVal[%x]\r\n",rtnVal);
		
	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem][task] Testing semTaskWatiSleep2s finished\r\n");
	//fibo_taskSleep(1000);///// sleep 2 seconds
	sleep(2);

	//fibo_thread_delete();
	pthread_exit(NULL);
	(void)rtnVal;
	return NULL;	
}



START_TEST(test_BoAT_OSAL_LinuxDefault_04Sem_test_0028MultiTaskUseOneSem)
{
	int i;
	struct semTestStru{
		void *p;
		int counter;
	} semTest;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0028MultiTaskUseOneSem\r\n");
	boatSem testSemId;
	boatSemInit(&testSemId,0);
	semTest.counter = 0;
	semTest.p = &testSemId;
	
	/////boatSemPost(&testSemId);

	//fibo_thread_create(semTaskWatiSleep2s, "sem_sleep_2s", 1024 * 8, &semTest, OSI_PRIORITY_NORMAL);
	pthread_t testThread;
    pthread_attr_t attr;
											
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&testThread,&attr,semTaskWatiSleep2s,&semTest);
	


	for (i = 0;i < 6;i++)
	{
		//fibo_taskSleep(50);
		usleep(50*1000);
		BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing sem main wait times[%d]\r\n",i);
		boatSemPost(&testSemId);
		//fibo_taskSleep(2000);///// sleep 2 seconds
		sleep(2);
		boatSemWait(&testSemId,0);		
		semTest.counter ++;
		BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing sem main return from wait times[%d]\r\n",i);
	}
	boatSemDestroy(&testSemId);
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_OSAL_LinuxDefault_04Sem_test_0028MultiTaskUseOneSem finished\r\n");

	ck_assert_int_eq(semTest.counter,1);
	
}
END_TEST

Suite *makeSemtest_suite(void)
{
    /* Create Suite */
    Suite *sSemtest = suite_create("sem_test");

    /* Create test cases */
    TCase *tcSemtest_api = tcase_create("sem_test_api");

	tcase_set_timeout(tcSemtest_api,100);

    /* Add a test case to the Suite */
    suite_add_tcase(sSemtest,tcSemtest_api);

    /* Test cases are added to the test set */

	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0010InitSemSuccess);
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0011InitSemFailSemAddrNULL);
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0012InitSemFailSemCreateReturnZero);
	
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0013DeinitSemSuccess);
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0014DeinitSemFailSemAddrNULL);
	//tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0015DeinitSemFailSemIdEqualZero);
	
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0017SemWaitSuccess);
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0018SemTimedWaitSuccess);
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0019SemWaitFailSemAddrNULL);
	//tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0020SemWaitFailSemIdEqualZero);
	
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0021SemTimedWaitFailSemAddrNULL);
	//tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0022SemTimedWaitFailSemIdEqualZero);
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0022SemTimedWaitSuccessTimeOut);
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0023SemTimedWaitSuccessCountNoZero);
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0024SemPostSuccess);
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0025SemPostFailSemAddrNULL);
	//tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0026SemPostFailSemIdEqualZero);

	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0028MultiTaskUseOneSem);
#if 0

	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0030SemTimedWaitFailSemIdInvalid);
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0027SemWaitFailSemIdInvalid);
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0029SemPostFailSemIdInvalid);

	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0027ReuseSingleSem)
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0028MultiTaskUseOneSem)
	tcase_add_test(tcSemtest_api,test_BoAT_OSAL_LinuxDefault_04Sem_test_0029InitSemGetLimit)

#endif

    return sSemtest;

}
