#include "boatosal.h"
#include "boatlog.h"
#include "boattypes.h"
#include "fibo_opencpu.h"

boatTimer testTimerID;
boatTimer testTimerid5,testTimerid6,testTimerid7;
boatTask testTaskId;

void boatTaskInitTimeridZero(boatTimer *Task);
void boatTaskInitTimeridInvalid(boatTimer *timerRef);

void boatAssert_int_gt(BOAT_RESULT rtnVal, BOAT_RESULT gtVal);
void boatAssert_int_eq(BOAT_RESULT rtnVal, BOAT_RESULT eqVal);
BOAT_RESULT boat_Timestamp(BSINT64 *timestamp);


void showtimestamp(void)
{
	union data{
    BUINT8 ch[8];
	BSINT32 I32;
    BSINT64 ts;
	} ud;
	ud.ts = 0;

	boat_Timestamp(&ud.ts);
	BoatPrintf(0,"[boat][timer] timestamp[%ld] \r\n",ud.I32);
}

char testflag = 0;
void onetimeFunc(void *arg)
{
	testflag = 5;
	//showtimestamp();
	BoatPrintf(0,"[boat][timer] [test] This is onetimeFunc\r\n");

}

void limitFunc(void *arg)
{
	testflag = 5;
	//showtimestamp();
	//BoatPrintf(0,"[boat][timer] [test] This is onetimeFunc\r\n");

}



void periodtimeFunc(void *arg)
{
	testflag = 5;
	//showtimestamp();
	BoatPrintf(0,"[boat][timer] [test] This is periodtimeFunc\r\n");

}


void onetimeFunc5(void *arg)
{
	testflag = 5;
	//showtimestamp();
	BoatPrintf(0,"[boat][timer] [test] This is onetimeFunc\r\n");

}

void onetimeFunc6(void *arg)
{
	testflag = 6;
	//showtimestamp();
	BoatPrintf(0,"[boat][timer] [test] This is onetimeFunc\r\n");

}

void onetimeFunc7(void *arg)
{
	testflag = 7;
	//showtimestamp();
	BoatPrintf(0,"[boat][timer] [test] This is onetimeFunc\r\n");

}



static void task_hello_thread(void *param)
{
	int rtnVal = 0;
	BoatPrintf(0,"[boat][task] This is task_hello_thread tastflag[%d][%x] \r\n",testflag,&rtnVal);
	while(1)
	{
		BoatPrintf(0,"[boat][task] This is task_hello_thread tastflag[%d] \r\n",testflag);
		fibo_taskSleep(208);
		if(testflag == 0)
		{
			BoatPrintf(0,"[boat][task] Ending the task_hello_thread tastflag[%d] \r\n",testflag);
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
	BoatPrintf(0,"[boat][task] close the detecting task\r\n");
		
	testflag = 0;
}


void *test_BoAT_OSAL_FibocomL610_03Timer_test_0010_boatTimerStart_Successful_onetimeTimer(void)
{
	BOAT_RESULT rtnVal;

	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0010_boatTimerStart_Successful_onetimeTimer\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 1500, 0, onetimeFunc, NULL);
	boatAssert_int_eq(rtnVal,0);
	boatTimerDestroy(&testTimerID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0010_boatTimerStart_Successful_onetimeTimer finished\r\n");
	
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0011_boatTimerStart_Successful_PeriodTimer(void)
{
	BOAT_RESULT rtnVal;

	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0011_boatTimerStart_Successful_PeriodTimer\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 500, 2500, onetimeFunc, NULL);
	boatAssert_int_eq(rtnVal,0);

	boatTimerDestroy(&testTimerID);	///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0011_boatTimerStart_Successful_PeriodTimer finished\r\n");
	
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0012_boatTimerStart_Failed_timerRefNULL(void)
{
	BOAT_RESULT rtnVal;

	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0012_boatTimerStart_Failed_timerRefNULL\r\n");
	
	rtnVal = boatTimerStart(0, 0, 2500, onetimeFunc, NULL);
	boatAssert_int_eq(rtnVal,-1);

	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0012_boatTimerStart_Failed_timerRefNULL finished\r\n");
	
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0013_boatTimerStart_Failed_doubleZero(void)
{
	BOAT_RESULT rtnVal;

	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0013_boatTimerStart_Failed_doubleZero\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 0, 0, onetimeFunc, NULL);
	boatAssert_int_eq(rtnVal,-1);

	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0013_boatTimerStart_Failed_doubleZero finished\r\n");
	
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0014_boatTimerStart_Failed_callBackRoutineNull(void)
{
	BOAT_RESULT rtnVal;

	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0014_boatTimerStart_Failed_callBackRoutineNull\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 500, 0, NULL, NULL);
	boatAssert_int_eq(rtnVal,-1);

	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0014_boatTimerStart_Failed_callBackRoutineNull finished\r\n");
	
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0015_boatTimerStart_Failed_onetimeTimer_rtnValZero(void)
{

	BoatPrintf(0,"[boat][task] Try test_BoAT_OSAL_FibocomL610_03Timer_test_0026_boatTimerStart_Limit \
		to test test_BoAT_OSAL_FibocomL610_03Timer_test_0015_boatTimerStart_Failed_onetimeTimer_rtnValZero \r\n");	

	BoatPrintf(0,"[boat][task] in L610 runtime environment the rtnVal will not be 0\r\n");
	
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0015_boatTimerStart_Failed_onetimeTimer_rtnValZero finished\r\n");
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0016_boatTimerStart_Failed_periodTimer_rtnValZero(void)
{

	BoatPrintf(0,"[boat][task] Try test_BoAT_OSAL_FibocomL610_03Timer_test_0026_boatTimerStart_Limit \
		to test test_BoAT_OSAL_FibocomL610_03Timer_test_0016_boatTimerStart_Failed_periodTimer_rtnValNagtive \r\n");	

	BoatPrintf(0,"[boat][task] in L610 runtime environment the rtnVal will not be 0\r\n");
	
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0016_boatTimerStart_Failed_periodTimer_rtnValNagtive finished\r\n");
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0017_boatTimerDestroy_Successful_onetimeTimer(void)
{
	BOAT_RESULT rtnVal;

	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0017_boatTimerDestroy_Successful_onetimeTimer\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 1500, 0, onetimeFunc, NULL);
	fibo_taskSleep(100);
	rtnVal = boatTimerDestroy(&testTimerID);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,0);
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0017_boatTimerDestroy_Successful_onetimeTimer finished\r\n");
	
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0018_boatTimerDestroy_Successful_periodTimer(void)
{
	BOAT_RESULT rtnVal;

	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0018_boatTimerDestroy_Successful_periodTimer\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 0, 2500, onetimeFunc, NULL);
	fibo_taskSleep(100);
	rtnVal = boatTimerDestroy(&testTimerID);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,0);
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0018_boatTimerDestroy_Successful_periodTimer finished\r\n");
	
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0019_boatTimerDestroy_Failed_timerRefNULL(void)
{
	BOAT_RESULT rtnVal;

	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0019_boatTimerDestroy_Failed_timerRefNULL\r\n");
	
	rtnVal = boatTimerDestroy(0);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,-1);
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0019_boatTimerDestroy_Failed_timerRefNULL finished\r\n");
	
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0020_boatTimerDestroy_Failed_timerIDZero(void)
{
	BOAT_RESULT rtnVal;

	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0020_boatTimerDestroy_Failed_timerIDZero\r\n");
	boatTaskInitTimeridZero(&testTimerID);
	rtnVal = boatTimerDestroy(&testTimerID);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,-1);
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0020_boatTimerDestroy_Failed_timerIDZero finished\r\n");
	
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0021_onetimeTimerStartAndDestroy_onetime(void)
{
	BOAT_RESULT rtnVal;

	if(testflag == 0)
	{
		testflag = 100;
		createStateDetectTask();
	}
	
	showtimestamp();
	testflag = 2;
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0021_onetimeTimerStartAndDestroy_onetime\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 1500, 0, onetimeFunc, NULL);
	fibo_taskSleep(100);
	boatAssert_int_eq(rtnVal,0);
	while(1)
	{
		if(testflag == 5)
		{
			
			break;
		}
		else
		{
			BoatPrintf(0,"[boat][timer] [test] This is wait flag loop [%d]\r\n",testflag);
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
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0021_onetimeTimerStartAndDestroy_onetime finished\r\n");
	closeStateDetectTask();
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0022_MultiTimerStartAndDestroy_onetimeTimer(void)
{
	BOAT_RESULT rtnVal;

	if(testflag == 0)
	{
		testflag = 100;
		createStateDetectTask();
	}
	
	showtimestamp();
	testflag = 2;
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0022_MultiTimerStartAndDestroy_onetimeTimer\r\n");
	
	rtnVal = boatTimerStart(&testTimerid5, 1000, 0, onetimeFunc5, NULL);
	boatAssert_int_eq(rtnVal,0);
	rtnVal = boatTimerStart(&testTimerid6, 1500, 0, onetimeFunc6, NULL);
	boatAssert_int_eq(rtnVal,0);
	rtnVal = boatTimerStart(&testTimerid7, 2000, 0, onetimeFunc7, NULL);
	boatAssert_int_eq(rtnVal,0);

	
	fibo_taskSleep(3000);

	showtimestamp();
	rtnVal = boatTimerDestroy(&testTimerid5);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,0);
	rtnVal = boatTimerDestroy(&testTimerid6);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,0);
	rtnVal = boatTimerDestroy(&testTimerid7);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,0);
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0022_MultiTimerStartAndDestroy_onetimeTimer finished\r\n");
	closeStateDetectTask();
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0023_periodTimerStartAndDestroy_onetime(void)
{
	BOAT_RESULT rtnVal;
	
	if(testflag == 0)
	{
		testflag = 100;
		createStateDetectTask();
	}

	showtimestamp();
	testflag = 2;
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0023_periodTimerStartAndDestroy_onetime\r\n");
	
	rtnVal = boatTimerStart(&testTimerID, 500, 500, periodtimeFunc, NULL);
	boatAssert_int_eq(rtnVal,0);
	while(1)
	{
		if(testflag == 5)
		{			
			break;
		}
		else
		{
			BoatPrintf(0,"[boat][timer] [test] This is wait flag loop [%d]\r\n",testflag);
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
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0023_periodTimerStartAndDestroy_onetime finished\r\n");
	closeStateDetectTask();
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0024_MultiTimerStartAndDestroy_periodTimer(void)
{
	BOAT_RESULT rtnVal;

	if(testflag == 0)
	{
		testflag = 100;
		createStateDetectTask();
	}
	
	showtimestamp();
	testflag = 2;
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0024_MultiTimerStartAndDestroy_periodTimer\r\n");
	
	rtnVal = boatTimerStart(&testTimerid5, 0, 1500, onetimeFunc5, NULL);
	boatAssert_int_eq(rtnVal,0);
	fibo_taskSleep(500);
	rtnVal = boatTimerStart(&testTimerid6, 0, 1500, onetimeFunc6, NULL);
	boatAssert_int_eq(rtnVal,0);
	fibo_taskSleep(500);
	rtnVal = boatTimerStart(&testTimerid7, 0, 1500, onetimeFunc7, NULL);
	boatAssert_int_eq(rtnVal,0);
	
	fibo_taskSleep(10000);

	showtimestamp();
	rtnVal = boatTimerDestroy(&testTimerid5);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,0);
	rtnVal = boatTimerDestroy(&testTimerid6);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,0);
	rtnVal = boatTimerDestroy(&testTimerid7);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,0);
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0024_MultiTimerStartAndDestroy_periodTimer finished\r\n");
	closeStateDetectTask();
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0026_boatTimerStart_Limit(void)
{
	BOAT_RESULT rtnVal;

	char *p=NULL;

	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0026_boatTimerStart_Limit [%x]\r\n",p);

	for (int i = 0;i < 200;i++)
	{
		p = fibo_malloc(0x8000);
		rtnVal = boatTimerStart(&testTimerID, 500, 1500, limitFunc, NULL);
		BoatPrintf(0,"[boat][task] [%04d]Testing timer create rtnVal[%d][%x][%x]\r\n",i,rtnVal,testTimerID.timerID,p);	
		if(rtnVal != 0)
		{
			break;
		}
		fibo_taskSleep(50);

	}
	BoatPrintf(0,"[boat][task] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0026_boatTimerStart_Limit finished\r\n");
	return NULL;
}

void *test_BoAT_OSAL_FibocomL610_03Timer_test_0025_boatTimerDestroy_timerIDInvalide(void)
{
	BOAT_RESULT rtnVal;
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0025_boatTimerDestroy_timerIDInvalide\r\n");
	fibo_taskSleep(3000);
	boatTaskInitTimeridInvalid(&testTimerID);

	
	rtnVal = boatTimerDestroy(&testTimerID);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,0);
	BoatPrintf(0,"[boat][timer] Testing test_BoAT_OSAL_FibocomL610_03Timer_test_0025_boatTimerDestroy_timerIDInvalide finished\r\n");
	closeStateDetectTask();
	return NULL;
}

void testTimerEntry(void)
{

	fibo_taskSleep(800);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0010_boatTimerStart_Successful_onetimeTimer();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0011_boatTimerStart_Successful_PeriodTimer();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0012_boatTimerStart_Failed_timerRefNULL();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0013_boatTimerStart_Failed_doubleZero();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0014_boatTimerStart_Failed_callBackRoutineNull();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0015_boatTimerStart_Failed_onetimeTimer_rtnValZero();	
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0016_boatTimerStart_Failed_periodTimer_rtnValZero();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0017_boatTimerDestroy_Successful_onetimeTimer();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0018_boatTimerDestroy_Successful_periodTimer();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0019_boatTimerDestroy_Failed_timerRefNULL();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0020_boatTimerDestroy_Failed_timerIDZero();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0021_onetimeTimerStartAndDestroy_onetime();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0022_MultiTimerStartAndDestroy_onetimeTimer();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0023_periodTimerStartAndDestroy_onetime();
	fibo_taskSleep(1000);
	test_BoAT_OSAL_FibocomL610_03Timer_test_0024_MultiTimerStartAndDestroy_periodTimer();
	fibo_taskSleep(1000);

	/////test_BoAT_OSAL_FibocomL610_03Timer_test_0025_boatTimerDestroy_timerIDInvalide(); 

	/////test_BoAT_OSAL_FibocomL610_03Timer_test_0026_boatTimerStart_Limit();

#if 0

	test_BoAT_OSAL_FibocomL610_03Timer_test_0025_boatTimerDestroy_timerIDInvalide(); 
	
#endif
	return ;
}


