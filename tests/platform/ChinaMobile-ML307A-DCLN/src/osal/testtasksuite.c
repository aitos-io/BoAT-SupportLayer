#include "boatosal.h"
#include "boatlog.h"
#include "boattypes.h"
#include "cm_os.h"
#include "cm_mem.h"
#include "boatcheck.h"


#define MULTITASKNUM 4
boatTask testTaskId;
boatTask testTaskIdArray[MULTITASKNUM];
char *taskNameArray[MULTITASKNUM] = {"thread_multi1","thread_multi2","thread_multi3","thread_multi4"};

void boatTaskInitTaskidNagtive(boatTask *Task);

static void task_hello_thread(void *param)
{
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] [test] This is task_hello_thread\r\n");


	BoatLog(BOAT_LOG_NORMAL, "[boat][task] This is task_hello_thread end\r\n");

	boatTaskDelete(&testTaskId);

	//fibo_thread_delete();	
}

static void task_test_delete_thread(void *param)
{
	int rtnVal = 0;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] This is task_test_delete_thread\r\n");
	rtnVal = boatTaskDelete(&testTaskId);
	ck_assert_int_eq(rtnVal,0);	///// equal  0 , successful
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] This is task_test_delete_thread end\r\n");
}


START_TEST(test_BoAT_OSAL_ML307A_02Task_test_0010_boatTaskCreat_Successful_PriorityLow)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0010_boatTaskCreat_Successful_PriorityLow\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_LOW, task_hello_thread, NULL);
	BoatSleepMs(1000);
	ck_assert_int_eq(rtnVal,0);	///// equal  0 , successful
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0010_boatTaskCreat_Successful_PriorityLow finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_02Task_test_0011_boatTaskCreat_Successful_PriorityNormal)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0011_boatTaskCreat_Successful_PriorityNormal\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_hello_thread, NULL);
	ck_assert_int_eq(rtnVal,0);	///// equal  0 , successful
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0011_boatTaskCreat_Successful_PriorityNormal finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_02Task_test_0012_boatTaskCreat_Successful_PriorityHigh)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0012_boatTaskCreat_Successful_PriorityHigh\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_HIGH, task_hello_thread, NULL);
	ck_assert_int_eq(rtnVal,0);	///// equal  0 , successful
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0012_boatTaskCreat_Successful_PriorityHigh finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_02Task_test_0013_boatTaskCreat_Failed_TaskAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0013_boatTaskCreat_Failed_TaskAddrNULL\r\n");
	
	rtnVal = boatTaskCreat(0, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_hello_thread, NULL);
	ck_assert_int_eq(rtnVal,-1);	///// equal -1 , error
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0013_boatTaskCreat_Failed_TaskAddrNULL finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_02Task_test_0014_boatTaskCreat_Failed_TaskNameNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0014_boatTaskCreat_Failed_TaskNameNULL\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, NULL, 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_hello_thread, NULL);
	ck_assert_int_eq(rtnVal,-1);	///// equal -1 , error
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0014_boatTaskCreat_Failed_TaskNameNULL finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_02Task_test_0015_boatTaskCreat_Failed_TaskStackNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0015_boatTaskCreat_Failed_TaskStackNULL\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 0, BOAT_TASK_PRIORITY_NORMAL, task_hello_thread, NULL);
	ck_assert_int_eq(rtnVal,-1);	///// equal -1 , error
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0015_boatTaskCreat_Failed_TaskStackNULL finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_02Task_test_0016_boatTaskCreat_Failed_TaskFuncNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0016_boatTaskCreat_Failed_TaskFuncNULL\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, NULL, NULL);
	ck_assert_int_eq(rtnVal,-1);	///// equal -1 , error
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0016_boatTaskCreat_Failed_TaskFuncNULL finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_02Task_test_0017_boatTaskCreat_Failed_FibocomTaskidNagtive)
{
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Try test_BoAT_OSAL_ML307A_02Task_test_0023_boatTaskCreat_Limit \
		to test test_BoAT_OSAL_ML307A_02Task_test_0017_boatTaskCreat_Failed_FibocomTaskidNagtive \r\n");	
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0017_boatTaskCreat_Failed_FibocomTaskidNagtive finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_02Task_test_0018_boatTaskDelete_Successful)
{

	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0018_boatTaskDelete_Successful \r\n");	
	
	boatTaskCreat(&testTaskId, "task_test_delete", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_test_delete_thread, NULL);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0018_boatTaskDelete_Successful finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_02Task_test_0019_boatTaskDelete_Failed_TaskAddrEqualNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0019_boatTaskDelete_Failed_TaskAddrEqualNULL\r\n");
	
	rtnVal = boatTaskDelete(0);
	BoatSleepMs(500);
	ck_assert_int_eq(rtnVal,-1);	///// equal -1 , error
		
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0019_boatTaskDelete_Failed_TaskAddrEqualNULL finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_02Task_test_0020_boatTaskDelete_Failed_TaskidNagtive)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0020_boatTaskDelete_Failed_TaskidNagtive\r\n");
	boatTaskInitTaskidNagtive(&testTaskId);
	rtnVal = boatTaskDelete(&testTaskId);
	ck_assert_int_eq(rtnVal,-1);	///// equal -1 , error
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0020_boatTaskDelete_Failed_TaskidNagtive finished\r\n");
}
END_TEST

static void task_loop_sleep_500ms_loopnum(void *param)
{
	BSINT32 loopnum = 3;/////param;

	if(param != NULL)
	{
		loopnum = (BSINT32)param;
	}

	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing task_loop_sleep_500ms_loopnum &param[%x][%x]\r\n",&param,param);

	for (int i = 0;i < loopnum;i++)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][task] task_loop_sleep_500ms_loopnum[%x] loop [%d] [%x]\r\n",&param,i,loopnum);
		BoatSleepMs(500);
		
	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing task_loop_sleep_500ms_loopnum finished[%x]\r\n",&param);

	//fibo_thread_delete();
	osThreadTerminate(osThreadGetId());

	return ;	
}
END_TEST

static void task_loop_sleep_500ms_endlessloop(void *param)
{
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing task_loop_sleep_500ms_endlessloop &param[%x][%x]\r\n",&param,param);
	
	while(1)
	{
		//BoatLog(BOAT_LOG_NORMAL, "[boat][task] task_loop_sleep_500ms_endlessloop[%x] loop [%x]\r\n",&param,i++);
		BoatSleepMs(50000);
		
	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing task_loop_sleep_500ms_endlessloop finished\r\n");

	//fibo_thread_delete();
	osThreadTerminate(osThreadGetId());

	return ;	
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_02Task_test_0021_MultiTaskUseOneTaskFunc)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0021_MultiTaskUseOneTaskFunc\r\n");

	for (int i = 0;i < MULTITASKNUM;i++)
	{
		
		//rtnVal = boatTaskCreat(&testTaskIdArray[i], taskNameArray[i], 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_loop_sleep_500ms_loopnum, 10);
		rtnVal = boatTaskCreat(&testTaskIdArray[i], "theSame", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_loop_sleep_500ms_loopnum, (void *)10);
		ck_assert_int_eq(rtnVal,0);	///// qeual 0
		BoatSleepMs(200);

	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0021_MultiTaskUseOneTaskFunc finished\r\n");
}
END_TEST

// 检查FIBOCOM 创建 Task 的极限
START_TEST(test_BoAT_OSAL_ML307A_02Task_test_0023_boatTaskCreat_Limit)
{
	BOAT_RESULT rtnVal;

	char *p = NULL;

	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0023_boatTaskCreat_Limit [%x]\r\n",p);

	for (int i = 0;i < 100;i++)
	{
		p = cm_malloc(0x8000);
		rtnVal = boatTaskCreat(&testTaskId, "task_endlessloop", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_loop_sleep_500ms_endlessloop, NULL);
		BoatLog(BOAT_LOG_NORMAL, "[boat][task] [%04d]Testing Task create rtnVal[%d][%x][%x]\r\n",i,rtnVal,testTaskId.taskId,p);	
		if(rtnVal != 0)
		{
			break;
		}
		BoatSleepMs(50);

	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_ML307A_02Task_test_0023_boatTaskCreat_Limit finished\r\n");

}
END_TEST

Suite *makeTasktest_suite(void)
{
    /* Create Suite */
    Suite *sTasktest = suite_create("task_test");

    /* Create test cases */
    TCase *tcTasktest_api = tcase_create("task_test_api");

    /* Add a test case to the Suite */
    suite_add_tcase(sTasktest,tcTasktest_api);

    /* Test cases are added to the test set */

	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_ML307A_02Task_test_0010_boatTaskCreat_Successful_PriorityLow);
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_ML307A_02Task_test_0011_boatTaskCreat_Successful_PriorityNormal);
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_ML307A_02Task_test_0012_boatTaskCreat_Successful_PriorityHigh);

	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_ML307A_02Task_test_0013_boatTaskCreat_Failed_TaskAddrNULL);
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_ML307A_02Task_test_0014_boatTaskCreat_Failed_TaskNameNULL);
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_ML307A_02Task_test_0015_boatTaskCreat_Failed_TaskStackNULL);
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_ML307A_02Task_test_0016_boatTaskCreat_Failed_TaskFuncNULL);
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_ML307A_02Task_test_0017_boatTaskCreat_Failed_FibocomTaskidNagtive);

	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_ML307A_02Task_test_0018_boatTaskDelete_Successful);
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_ML307A_02Task_test_0019_boatTaskDelete_Failed_TaskAddrEqualNULL);
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_ML307A_02Task_test_0020_boatTaskDelete_Failed_TaskidNagtive);

	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_ML307A_02Task_test_0021_MultiTaskUseOneTaskFunc);

	/////tcase_add_test(tcTasktest_api, test_BoAT_OSAL_ML307A_02Task_test_0023_boatTaskCreat_Limit);	///// if malloc failed, the AT+SPREF= CMD cannot make the module running 
																											///// download mode,must pull high edge on the USB_BOOT pin
	

    return sTasktest;

}


int runTaskTests(void)
{

#if 1
    SRunner *sr = NULL;
    int failed_number = 0;

    Suite *suiteTasktest = makeTasktest_suite();

    sr = srunner_create(suiteTasktest);

    srunner_run_all(sr,0);


    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed_number;
#endif

	
}

