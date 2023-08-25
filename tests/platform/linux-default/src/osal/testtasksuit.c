#define _GNU_SOURCE 
#include "boatosal.h"
#include "boatlog.h"
#include "boattypes.h"
#include "check.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define MULTITASKNUM 4
#define TEST_STACK_SIZE 8388608 + 1024
boatTask testTaskId;
boatTask testTaskIdArray[MULTITASKNUM];
char *taskNameArray[MULTITASKNUM] = {"thread_multi1","thread_multi2","thread_multi3","thread_multi4"};
size_t  stackSize = 0;

static void taskHelloThread(void *param)
{
    pthread_attr_t attr;
	int ret;
	pthread_t pt;
	pt = pthread_self();
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] [test] This is taskHelloThread\r\n");
	
	ret = pthread_getattr_np(pt,&attr);
	printf ("ret : %d , errno : %d [%x]\r\n", ret, errno, (int)pt);
	ret = pthread_attr_getstacksize(&attr,&stackSize);
	printf ("thread ret : %d , errno : %d %x\r\n", ret, errno, errno);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] This is taskHelloThread end\r\n");

	boatTaskDelete(&testTaskId);
}

static void task_test_delete_thread(void *param)
{
	int rtnVal = 0;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] This is task_test_delete_thread\r\n");
	rtnVal = boatTaskDelete(&testTaskId);
	ck_assert_int_eq(rtnVal,0);	///// equal  0 , successful
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] This is task_test_delete_thread end\r\n");
}


START_TEST(test_BoAT_OSAL_LinuxDefault_02Task_test_0010_boatTaskCreat_Successful_PriorityLow)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0010_boatTaskCreat_Successful_PriorityLow\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_LOW, taskHelloThread, NULL);
	usleep(1000 * 1000);
	ck_assert_int_eq(rtnVal,0);	///// equal  0 , successful
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0010_boatTaskCreat_Successful_PriorityLow finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_02Task_test_0011_boatTaskCreat_Successful_PriorityNormal)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0011_boatTaskCreat_Successful_PriorityNormal\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, taskHelloThread, NULL);
	usleep(1000 * 1000);
	ck_assert_int_eq(rtnVal,0);	///// equal  0 , successful
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0011_boatTaskCreat_Successful_PriorityNormal finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_02Task_test_0012_boatTaskCreat_Successful_PriorityHigh)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0012_boatTaskCreat_Successful_PriorityHigh\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_HIGH, taskHelloThread, NULL);
	usleep(1000 * 1000);
	ck_assert_int_eq(rtnVal,0);	///// equal  0 , successful
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0012_boatTaskCreat_Successful_PriorityHigh finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_02Task_test_0013_boatTaskCreat_Failed_TaskAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0013_boatTaskCreat_Failed_TaskAddrNULL\r\n");
	
	rtnVal = boatTaskCreat(0, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, taskHelloThread, NULL);
	ck_assert_int_eq(rtnVal,-1);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0013_boatTaskCreat_Failed_TaskAddrNULL finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_02Task_test_0015_boatTaskCreat_Successful_TaskStack_GreatThan_PTHREAD_STACK_MIN)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0015_boatTaskCreat_Successful_TaskStack_GreatThan_PTHREAD_STACK_MIN\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", TEST_STACK_SIZE, BOAT_TASK_PRIORITY_NORMAL, taskHelloThread, NULL);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	void *vp;
	int retvv = pthread_join(testTaskId.taskId, (void *)&vp);
	printf("join[%x] ret %d %d\n", (int)testTaskId.taskId, retvv, errno);
	printf("task return %p \n",(int *)vp);
	printf("stacksize %ld\r\n",(long)stackSize);
	usleep(1000 * 1000);
	printf("stacksize %ld\r\n",(long)stackSize);

	ck_assert_int_ge(stackSize,TEST_STACK_SIZE);
	
    pthread_attr_t attr;
	int ret;
	stackSize = 1;
	
	ret = pthread_getattr_np(testTaskId.taskId,&attr);	
	printf ("ret : %d , errno : %d %x\r\n", ret, errno, errno);
	ret = pthread_attr_getstacksize(&attr,&stackSize);
	printf ("ret : %d , size %ld errno : %d %x\r\n", ret, (long)stackSize,errno, errno);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0015_boatTaskCreat_Successful_TaskStack_GreatThan_PTHREAD_STACK_MIN finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_02Task_test_0016_boatTaskCreat_Failed_TaskFuncNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0016_boatTaskCreat_Failed_TaskFuncNULL\r\n");
	
	rtnVal = boatTaskCreat(&testTaskId, "thread_hello", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, NULL, NULL);
	ck_assert_int_eq(rtnVal,-1);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0016_boatTaskCreat_Failed_TaskFuncNULL finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_02Task_test_0017_boatTaskCreat_Failed_LinuxThreadCreateFailed)
{
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Try test_BoAT_OSAL_LinuxDefault_02Task_test_0023_boatTaskCreat_Limit \
		to test test_BoAT_OSAL_LinuxDefault_02Task_test_0017_boatTaskCreat_Failed_LinuxThreadCreateFailed \r\n");	
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0017_boatTaskCreat_Failed_LinuxThreadCreateFailed finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_LinuxDefault_02Task_test_0018_boatTaskDelete_Successful)
{

	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0018_boatTaskDelete_Successful \r\n");	
	
	boatTaskCreat(&testTaskId, "task_test_delete", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, task_test_delete_thread, NULL);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0018_boatTaskDelete_Successful finished\r\n");
}
END_TEST

static void taskLoopSleep500msLoopnum(void *param)
{
	BSINT32 loopnum = 3;/////param;


	if(param != NULL)
	{
		void *p = &param;
		BSINT32 *bsip = (BSINT32 *)p;
		loopnum = *bsip;
	}
	pthread_t tt = pthread_self();
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing taskLoopSleep500msLoopnum &param[%x][%p][%p]\r\n", (int)tt, &param, param);

	for (int i = 0;i < loopnum;i++)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][task] taskLoopSleep500msLoopnum[%p][%x] loop [%d] [%x]\r\n",&param, (int)tt, i, loopnum);
		usleep(1000 * 500);
		
	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing taskLoopSleep500msLoopnum finished[%p]\r\n",&param);
	(void)tt;
	return ;	
}

START_TEST(test_BoAT_OSAL_LinuxDefault_02Task_test_0021_MultiTaskUseOneTaskFunc)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0021_MultiTaskUseOneTaskFunc\r\n");

	for (int i = 0;i < MULTITASKNUM;i++)
	{
		
		rtnVal = boatTaskCreat(&testTaskIdArray[i], "theSame", 24 * 1024, BOAT_TASK_PRIORITY_NORMAL, taskLoopSleep500msLoopnum, (void *)8);
		ck_assert_int_eq(rtnVal,0);	///// qeual 0
		usleep(1000 * 200);

	}
	usleep(1000 * 1000 * 5);
	BoatLog(BOAT_LOG_NORMAL, "[boat][task] Testing test_BoAT_OSAL_LinuxDefault_02Task_test_0021_MultiTaskUseOneTaskFunc finished\r\n");
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

	tcase_set_timeout(tcTasktest_api,20);

    /* Test cases are added to the test set */
#if 1
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_LinuxDefault_02Task_test_0010_boatTaskCreat_Successful_PriorityLow);
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_LinuxDefault_02Task_test_0011_boatTaskCreat_Successful_PriorityNormal);
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_LinuxDefault_02Task_test_0012_boatTaskCreat_Successful_PriorityHigh);

	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_LinuxDefault_02Task_test_0015_boatTaskCreat_Successful_TaskStack_GreatThan_PTHREAD_STACK_MIN);

	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_LinuxDefault_02Task_test_0013_boatTaskCreat_Failed_TaskAddrNULL);
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_LinuxDefault_02Task_test_0016_boatTaskCreat_Failed_TaskFuncNULL);
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_LinuxDefault_02Task_test_0017_boatTaskCreat_Failed_LinuxThreadCreateFailed);

	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_LinuxDefault_02Task_test_0018_boatTaskDelete_Successful);

#endif
	tcase_add_test(tcTasktest_api, test_BoAT_OSAL_LinuxDefault_02Task_test_0021_MultiTaskUseOneTaskFunc);

    return sTasktest;

}
