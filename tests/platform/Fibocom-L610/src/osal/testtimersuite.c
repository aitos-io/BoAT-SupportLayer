#include "boatosal.h"
#include "boatlog.h"
#include "boattypes.h"
#include "fibo_opencpu.h"
#include "../../boatcheck/boatcheck.h"


boatTimer testTimerID;
boatTimer testTimerid5,testTimerid6,testTimerid7;
boatTask testTaskId;

void boatTimerInitTimeridZero(boatTimer *Task);
void boatTimerInitTimeridInvalid(boatTimer *timerRef);

BOAT_RESULT boatTimestamp(BSINT64 *timestamp);


void showtimestamp(void)
{
	union data{
    BUINT8 ch[8];
	BSINT32 I32;
    BSINT64 ts;
	} ud;
	ud.ts = 0;

	boatTimestamp(&ud.ts);
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] timestamp[%ld] \r\n",ud.I32);
}

char testflag = 0;
void onetimeFunc(void *arg)
{
	testflag = 5;
	//showtimestamp();
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] [test] This is onetimeFunc\r\n");

}

void limitFunc(void *arg)
{
	testflag = 5;
	//showtimestamp();
	//BoatLog(BOAT_LOG_NORMAL, "[boat][timer] [test] This is onetimeFunc\r\n");

}



void periodtimeFunc(void *arg)
{
	testflag = 5;
	//showtimestamp();
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] [test] This is periodtimeFunc\r\n");

}


void onetimeFunc5(void *arg)
{
	testflag = 5;
	//showtimestamp();
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] [test] This is onetimeFunc\r\n");

}

void onetimeFunc6(void *arg)
{
	testflag = 6;
	//showtimestamp();
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] [test] This is onetimeFunc\r\n");

}

void onetimeFunc7(void *arg)
{
	testflag = 7;
	//showtimestamp();
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] [test] This is onetimeFunc\r\n");

}



static void task_hello_thread(void *param)
{
	int rtnVal = 0;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] This is task_hello_thread tastflag[%d][%x] \r\n",testflag,&rtnVal);
	while(1)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][task] This is task_hello_thread tastflag[%d] \r\n",testflag);
		fibo_taskSleep(208);
		if(testflag == 0)
		{
			BoatLog(BOAT_LOG_NORMAL, "[boat][task] Ending the task_hello_thread tastflag[%d] \r\n",testflag);
			break;
		}
	}
	rtnVal = boatTaskDelete(&testTaskId);

	//fibo_thread_delete();	
}
void createStateDetectTask(void)
{
	boatTaskCreat(&testTaskId, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_LOW, task_hello_thread, NULL);
}

void closeStateDetectTask(void)
{
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] close the detecting task\r\n");
		
	testflag = 0;
}


START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0010_boatTimerStart_Successful_onetimeTimer)
{
	BOAT_RESULT rtnVal;

	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0010_boatTimerStart_Successful_onetimeTimer\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 1500, 0, onetimeFunc, NULL);
	ck_assert_int_eq(rtnVal,0);
	boatTimerDestroy(&testTimerID);///// must destroy the timer once you stop using it.
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0010_boatTimerStart_Successful_onetimeTimer finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0011_boatTimerStart_Successful_PeriodTimer)
{
	BOAT_RESULT rtnVal;

	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0011_boatTimerStart_Successful_PeriodTimer\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 500, 2500, onetimeFunc, NULL);
	ck_assert_int_eq(rtnVal,0);

	boatTimerDestroy(&testTimerID);	///// must destroy the timer once you stop using it.
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0011_boatTimerStart_Successful_PeriodTimer finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0012_boatTimerStart_Failed_timerRefNULL)
{
	BOAT_RESULT rtnVal;

	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0012_boatTimerStart_Failed_timerRefNULL\r\n");
	
	rtnVal = boatTimerStart(0, 0, 2500, onetimeFunc, NULL);
	ck_assert_int_eq(rtnVal,-1);

	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0012_boatTimerStart_Failed_timerRefNULL finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0013_boatTimerStart_Failed_doubleZero)
{
	BOAT_RESULT rtnVal;

	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0013_boatTimerStart_Failed_doubleZero\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 0, 0, onetimeFunc, NULL);
	ck_assert_int_eq(rtnVal,-1);

	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0013_boatTimerStart_Failed_doubleZero finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0014_boatTimerStart_Failed_callBackRoutineNull)
{
	BOAT_RESULT rtnVal;

	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0014_boatTimerStart_Failed_callBackRoutineNull\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 500, 0, NULL, NULL);
	ck_assert_int_eq(rtnVal,-1);

	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0014_boatTimerStart_Failed_callBackRoutineNull finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0015_boatTimerStart_Failed_onetimeTimer_rtnValZero)
{

	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Try test_BoAT_OSAL_FibocomL610_03Timer_test_0026_boatTimerStart_Limit \
		to test test_BoAT_OSAL_FibocomL610_03Timer_test_0015_boatTimerStart_Failed_onetimeTimer_rtnValZero \r\n");	

	BoatLog(BOAT_LOG_NORMAL, "[boat][task] in L610 runtime environment the rtnVal will not be 0\r\n");
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0015_boatTimerStart_Failed_onetimeTimer_rtnValZero finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0016_boatTimerStart_Failed_periodTimer_rtnValZero)
{

	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Try test_BoAT_OSAL_FibocomL610_03Timer_test_0026_boatTimerStart_Limit \
		to test test_BoAT_OSAL_FibocomL610_03Timer_test_0016_boatTimerStart_Failed_periodTimer_rtnValNagtive \r\n");	

	BoatLog(BOAT_LOG_NORMAL, "[boat][task] in L610 runtime environment the rtnVal will not be 0\r\n");
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0016_boatTimerStart_Failed_periodTimer_rtnValNagtive finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0017_boatTimerDestroy_Successful_onetimeTimer)
{
	BOAT_RESULT rtnVal;

	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0017_boatTimerDestroy_Successful_onetimeTimer\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 1500, 0, onetimeFunc, NULL);
	fibo_taskSleep(100);
	rtnVal = boatTimerDestroy(&testTimerID);///// must destroy the timer once you stop using it.
	ck_assert_int_eq(rtnVal,0);
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0017_boatTimerDestroy_Successful_onetimeTimer finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0018_boatTimerDestroy_Successful_periodTimer)
{
	BOAT_RESULT rtnVal;

	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0018_boatTimerDestroy_Successful_periodTimer\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 0, 2500, onetimeFunc, NULL);
	fibo_taskSleep(100);
	rtnVal = boatTimerDestroy(&testTimerID);///// must destroy the timer once you stop using it.
	ck_assert_int_eq(rtnVal,0);
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0018_boatTimerDestroy_Successful_periodTimer finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0019_boatTimerDestroy_Failed_timerRefNULL)
{
	BOAT_RESULT rtnVal;

	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0019_boatTimerDestroy_Failed_timerRefNULL\r\n");
	
	rtnVal = boatTimerDestroy(0);///// must destroy the timer once you stop using it.
	ck_assert_int_eq(rtnVal,-1);
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0019_boatTimerDestroy_Failed_timerRefNULL finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0020_boatTimerDestroy_Failed_timerIDZero)
{
	BOAT_RESULT rtnVal;

	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0020_boatTimerDestroy_Failed_timerIDZero\r\n");
	boatTimerInitTimeridZero(&testTimerID);
	rtnVal = boatTimerDestroy(&testTimerID);///// must destroy the timer once you stop using it.
	ck_assert_int_eq(rtnVal,-1);
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0020_boatTimerDestroy_Failed_timerIDZero finished\r\n");
	
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0021_onetimeTimerStartAndDestroy_onetime)
{
	BOAT_RESULT rtnVal;

	if(testflag == 0)
	{
		testflag = 100;
		createStateDetectTask();
	}
	
	showtimestamp();
	testflag = 2;
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0021_onetimeTimerStartAndDestroy_onetime\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 1500, 0, onetimeFunc, NULL);
	fibo_taskSleep(100);
	ck_assert_int_eq(rtnVal,0);
	while(1)
	{
		if(testflag == 5)
		{
			
			break;
		}
		else
		{
			BoatLog(BOAT_LOG_NORMAL, "[boat][timer] [test] This is wait flag loop [%d]\r\n",testflag);
			fibo_taskSleep(500);
		}
	}
	fibo_taskSleep(1000);
	testflag = 3;
	fibo_taskSleep(1000);
	testflag = 2;
	fibo_taskSleep(1000);
	testflag = 1;
	fibo_taskSleep(1000);

	showtimestamp();
	boatTimerDestroy(&testTimerID);///// must destroy the timer once you stop using it.
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0021_onetimeTimerStartAndDestroy_onetime finished\r\n");
	closeStateDetectTask();
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0022_MultiTimerStartAndDestroy_onetimeTimer)
{
	BOAT_RESULT rtnVal;

	if(testflag == 0)
	{
		testflag = 100;
		createStateDetectTask();
	}
	
	showtimestamp();
	testflag = 2;
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0022_MultiTimerStartAndDestroy_onetimeTimer\r\n");
	
	rtnVal = boatTimerStart(&testTimerid5, 1000, 0, onetimeFunc5, NULL);
	ck_assert_int_eq(rtnVal,0);
	rtnVal = boatTimerStart(&testTimerid6, 1500, 0, onetimeFunc6, NULL);
	ck_assert_int_eq(rtnVal,0);
	rtnVal = boatTimerStart(&testTimerid7, 2000, 0, onetimeFunc7, NULL);
	ck_assert_int_eq(rtnVal,0);

	
	fibo_taskSleep(3000);

	showtimestamp();
	rtnVal = boatTimerDestroy(&testTimerid5);///// must destroy the timer once you stop using it.
	ck_assert_int_eq(rtnVal,0);
	rtnVal = boatTimerDestroy(&testTimerid6);///// must destroy the timer once you stop using it.
	ck_assert_int_eq(rtnVal,0);
	rtnVal = boatTimerDestroy(&testTimerid7);///// must destroy the timer once you stop using it.
	ck_assert_int_eq(rtnVal,0);
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0022_MultiTimerStartAndDestroy_onetimeTimer finished\r\n");
	closeStateDetectTask();
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0023_periodTimerStartAndDestroy_onetime)
{
	BOAT_RESULT rtnVal;
	
	if(testflag == 0)
	{
		testflag = 100;
		createStateDetectTask();
	}

	showtimestamp();
	testflag = 2;
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0023_periodTimerStartAndDestroy_onetime\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 500, 500, periodtimeFunc, NULL);
	ck_assert_int_eq(rtnVal,0);
	while(1)
	{
		if(testflag == 5)
		{			
			break;
		}
		else
		{
			BoatLog(BOAT_LOG_NORMAL, "[boat][timer] [test] This is wait flag loop [%d]\r\n",testflag);
			fibo_taskSleep(100);
		}
	}
	fibo_taskSleep(1000);
	testflag = 13;
	fibo_taskSleep(1000);
	testflag = 12;
	fibo_taskSleep(1000);
	testflag = 11;
	fibo_taskSleep(1000);

	showtimestamp();
	boatTimerDestroy(&testTimerID);	///// must destroy the timer once you stop using it.
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0023_periodTimerStartAndDestroy_onetime finished\r\n");
	closeStateDetectTask();
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0024_MultiTimerStartAndDestroy_periodTimer)
{
	BOAT_RESULT rtnVal;

	if(testflag == 0)
	{
		testflag = 100;
		createStateDetectTask();
	}
	
	showtimestamp();
	testflag = 2;
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0024_MultiTimerStartAndDestroy_periodTimer\r\n");
	
	rtnVal = boatTimerStart(&testTimerid5, 0, 1500, onetimeFunc5, NULL);
	ck_assert_int_eq(rtnVal,0);
	fibo_taskSleep(500);
	rtnVal = boatTimerStart(&testTimerid6, 0, 1500, onetimeFunc6, NULL);
	ck_assert_int_eq(rtnVal,0);
	fibo_taskSleep(500);
	rtnVal = boatTimerStart(&testTimerid7, 0, 1500, onetimeFunc7, NULL);
	ck_assert_int_eq(rtnVal,0);
	
	fibo_taskSleep(10000);

	showtimestamp();
	rtnVal = boatTimerDestroy(&testTimerid5);///// must destroy the timer once you stop using it.
	ck_assert_int_eq(rtnVal,0);
	rtnVal = boatTimerDestroy(&testTimerid6);///// must destroy the timer once you stop using it.
	ck_assert_int_eq(rtnVal,0);
	rtnVal = boatTimerDestroy(&testTimerid7);///// must destroy the timer once you stop using it.
	ck_assert_int_eq(rtnVal,0);
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0024_MultiTimerStartAndDestroy_periodTimer finished\r\n");
	closeStateDetectTask();
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0026_boatTimerStart_Limit)
{
	BOAT_RESULT rtnVal;

	char *p=NULL;

	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0026_boatTimerStart_Limit [%x]\r\n",p);

	for (int i = 0;i < 200;i++)
	{
		p = fibo_malloc(0x8000);
		rtnVal = boatTimerStart(&testTimerID, 500, 1500, limitFunc, NULL);
		BoatLog(BOAT_LOG_NORMAL, "[boat][task] [%04d]Testing timer create rtnVal[%d][%x][%x]\r\n",i,rtnVal,testTimerID.timerId,p);	
		if(rtnVal != 0)
		{
			break;
		}
		fibo_taskSleep(50);

	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0026_boatTimerStart_Limit finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_FibocomL610_03Timer_test_0025_boatTimerDestroy_timerIDInvalide)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0025_boatTimerDestroy_timerIDInvalide\r\n");
	fibo_taskSleep(3000);
	boatTimerInitTimeridInvalid(&testTimerID);

	
	rtnVal = boatTimerDestroy(&testTimerID);///// must destroy the timer once you stop using it.
	ck_assert_int_eq(rtnVal,0);
	BoatLog(BOAT_LOG_NORMAL, "[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0025_boatTimerDestroy_timerIDInvalide finished\r\n");
	closeStateDetectTask();
}
END_TEST

Suite *makeTimertest_suite(void)
{
    /* Create Suite */
    Suite *sTimertest = suite_create("timer_test");

    /* Create test cases */
    TCase *tcTimertest_api = tcase_create("timer_test_api");

    /* Add a test case to the Suite */
    suite_add_tcase(sTimertest,tcTimertest_api);

    /* Test cases are added to the test set */

	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0010_boatTimerStart_Successful_onetimeTimer);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0011_boatTimerStart_Successful_PeriodTimer);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0012_boatTimerStart_Failed_timerRefNULL);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0013_boatTimerStart_Failed_doubleZero);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0014_boatTimerStart_Failed_callBackRoutineNull);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0015_boatTimerStart_Failed_onetimeTimer_rtnValZero);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0016_boatTimerStart_Failed_periodTimer_rtnValZero);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0017_boatTimerDestroy_Successful_onetimeTimer);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0018_boatTimerDestroy_Successful_periodTimer);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0019_boatTimerDestroy_Failed_timerRefNULL);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0020_boatTimerDestroy_Failed_timerIDZero);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0021_onetimeTimerStartAndDestroy_onetime);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0022_MultiTimerStartAndDestroy_onetimeTimer);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0023_periodTimerStartAndDestroy_onetime);
	tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0024_MultiTimerStartAndDestroy_periodTimer);

	/////tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0025_boatTimerDestroy_timerIDInvalide); 
	/////tcase_add_test(tcTimertest_api, test_BoAT_OSAL_FibocomL610_03Timer_test_0026_boatTimerStart_Limit);
	

    return sTimertest;

}


int runTimerTests(void)
{

#if 1
    SRunner *sr = NULL;
    int failed_number = 0;

    Suite *suiteTimertest = makeTimertest_suite();

    sr = srunner_create(suiteTimertest);

    srunner_run_all(sr,0);


    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed_number;
#endif

	
}

