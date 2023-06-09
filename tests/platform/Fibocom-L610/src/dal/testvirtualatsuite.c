#include "fibo_opencpu.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


#include "boatlog.h"
#include "../../boatcheck/boatcheck.h"
#include "boatdal.h"

static UINT32 g_virtualAt_lock = 0;

static int g_callback_1_flag = 0;
static int g_callback_2_flag = 0; 

static BOAT_RESULT g_openVirtualAT_result_thread1 = BOAT_ERROR;
static BOAT_RESULT g_openVirtualAT_result_thread2 = BOAT_ERROR;

static char *g_cmd = "AT\r\n";

static void initThreadOpenVirtualATFlag(void)
{
    g_openVirtualAT_result_thread1 = BOAT_ERROR;
    g_openVirtualAT_result_thread2 = BOAT_ERROR;
}

static void initCallbackFlag(void)
{
    g_callback_1_flag = 0;
    g_callback_2_flag = 0;
}


void dalVirtualATCallback1(char *content,BUINT32 len)
{
    BoatLog(BOAT_LOG_NORMAL,"virtualAt callback_1=>%s,len:%d",content,len);
    g_callback_1_flag = 1;
    if(NULL != strstr(content,"OK"))
    {
        if(g_virtualAt_lock)
        {
            fibo_sem_signal(g_virtualAt_lock);
        }
    }

}

void dalVirtualATCallback2(char *content,BUINT32 len)
{
    BoatLog(BOAT_LOG_NORMAL,"virtualAt callback_2=>%s,len:%d",content,len);
    g_callback_2_flag = 1;

    if(NULL != strstr(content,"OK"))
    {
        if(g_virtualAt_lock)
        {
            fibo_sem_signal(g_virtualAt_lock);
        }
    }
}


static void openVirtualAT_in_thread1(void *param)
{
    BOAT_RESULT result;
    result = boatVirtualAtOpen(dalVirtualATCallback1);
    g_openVirtualAT_result_thread1 = result;

    if(g_virtualAt_lock)
    {
        fibo_sem_signal(g_virtualAt_lock);
    }

    fibo_thread_delete();
    
}

static void openVirtualAT_in_thread2(void *param)
{
    BOAT_RESULT result;
    result = boatVirtualAtOpen(dalVirtualATCallback2);
    g_openVirtualAT_result_thread2 = result;

    if(g_virtualAt_lock)
    {
        fibo_sem_signal(g_virtualAt_lock);
    }

    fibo_thread_delete();
    
}

static void sendVirtualAT_in_thread(void *param)
{
    BOAT_RESULT result;
    result = boatVirtualAtSend(g_cmd,strlen(g_cmd));
    ck_assert_int_eq(result,BOAT_SUCCESS);

    if(g_virtualAt_lock)
    {
        fibo_sem_signal(g_virtualAt_lock);
    }

    fibo_thread_delete();
}


START_TEST(test_001_OpenVirtualAT_0001_OpenVirtualATSuccess)
{

    BOAT_RESULT  result;
    result = boatVirtualAtOpen(dalVirtualATCallback1);
    ck_assert_int_eq(result,BOAT_SUCCESS);

    boatVirtualAtClose();

}
END_TEST

START_TEST(test_001_OpenVirtualAT_0002_OpenVirtualATFailureTwice)
{
    initThreadOpenVirtualATFlag();
    if (g_virtualAt_lock == 0)
    {
        g_virtualAt_lock = fibo_sem_new(0);
    }


    fibo_thread_create(openVirtualAT_in_thread1, "openVirtualAT_in_thread1", 1024*1, NULL, OSI_PRIORITY_NORMAL);

    fibo_sem_wait(g_virtualAt_lock);

    ck_assert_int_eq(g_openVirtualAT_result_thread1,BOAT_SUCCESS);

    fibo_thread_create(openVirtualAT_in_thread2, "openVirtualAT_in_thread2", 1024*1, NULL, OSI_PRIORITY_NORMAL);

    fibo_sem_wait(g_virtualAt_lock);

    ck_assert_int_eq(g_openVirtualAT_result_thread2,BOAT_ERROR_DAL_VIRTUAL_BUSY);

    boatVirtualAtClose();

    fibo_sem_free(g_virtualAt_lock);
    g_virtualAt_lock = 0;

    initThreadOpenVirtualATFlag();

}
END_TEST

START_TEST(test_002_SendCmd_0001_SendCmdSuccess)
{

    BOAT_RESULT  result;
    initCallbackFlag();

    if (g_virtualAt_lock == 0)
    {
        g_virtualAt_lock = fibo_sem_new(0);
    }

    result = boatVirtualAtOpen(dalVirtualATCallback1);
    ck_assert_int_eq(result,BOAT_SUCCESS);
    
    result = boatVirtualAtSend(g_cmd,strlen(g_cmd));
    ck_assert_int_eq(result,BOAT_SUCCESS);

    fibo_sem_wait(g_virtualAt_lock);
    ck_assert_int_eq(g_callback_1_flag,1);
    ck_assert_int_eq(g_callback_2_flag,0);

    boatVirtualAtClose();
    if(g_virtualAt_lock)
    {
        fibo_sem_free(g_virtualAt_lock);
        g_virtualAt_lock = 0;
    }

    initCallbackFlag();

}
END_TEST

START_TEST(test_002_SendCmd_0002_SendCmdFailureNotOpen)
{
    BOAT_RESULT  result;
    result = boatVirtualAtSend(g_cmd,strlen(g_cmd));
    ck_assert_int_eq(result,BOAT_ERROR_DAL_VIRTUAL_CLOSED);
}
END_TEST

START_TEST(test_002_SendCmd_0003_SendCmdFailureNotOwner)
{
    initThreadOpenVirtualATFlag();
    initCallbackFlag();

    if (g_virtualAt_lock == 0)
    {
        g_virtualAt_lock = fibo_sem_new(0);
    }

    fibo_thread_create(openVirtualAT_in_thread1, "openVirtualAT_in_thread1", 1024*1, NULL, OSI_PRIORITY_NORMAL);

    fibo_sem_wait(g_virtualAt_lock);

    fibo_thread_create(sendVirtualAT_in_thread, "sendVirtualAT_in_thread", 1024*1, NULL, OSI_PRIORITY_NORMAL);
    
    fibo_sem_wait(g_virtualAt_lock);

    ck_assert_int_eq(g_callback_1_flag,1);
    ck_assert_int_eq(g_callback_2_flag,0);

    boatVirtualAtClose();

    fibo_sem_free(g_virtualAt_lock);
    g_virtualAt_lock = 0;

    initCallbackFlag();
    initThreadOpenVirtualATFlag();

}
END_TEST

START_TEST(test_002_SendCmd_0004_SendCmdFailureCmdNULL)
{
    BOAT_RESULT  result;
    result = boatVirtualAtSend(NULL,strlen(g_cmd));
    ck_assert_int_eq(result,BOAT_ERROR_DAL_INVALID_ARGUMENT);

}
END_TEST


START_TEST(test_002_SendCmd_0005_SendCmdFailureLenWrong)
{

    BOAT_RESULT  result;
    result = boatVirtualAtSend(g_cmd,0);
    ck_assert_int_eq(result,BOAT_ERROR_DAL_INVALID_ARGUMENT);

}
END_TEST

START_TEST(test_003_CloseVirtualAT_0001_CloseVirtualATSuccess)
{
    BOAT_RESULT result;
    result = boatVirtualAtClose();
    ck_assert_int_eq(result,BOAT_SUCCESS);
}
END_TEST

Suite *make_virtualAt_test_suite(void)
{
    /* Create Suite */
    Suite *s_virtualAt_test = suite_create("virtualAt_test");

    /* Create test cases */
    TCase *tc_virtualAt_test_api = tcase_create("virtualAt_test_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_virtualAt_test,tc_virtualAt_test_api);

    /* Test cases are added to the test set */

    tcase_add_test(tc_virtualAt_test_api,test_001_OpenVirtualAT_0001_OpenVirtualATSuccess);
    tcase_add_test(tc_virtualAt_test_api,test_001_OpenVirtualAT_0002_OpenVirtualATFailureTwice);
    tcase_add_test(tc_virtualAt_test_api,test_002_SendCmd_0001_SendCmdSuccess);
    tcase_add_test(tc_virtualAt_test_api,test_002_SendCmd_0002_SendCmdFailureNotOpen);
    tcase_add_test(tc_virtualAt_test_api,test_002_SendCmd_0003_SendCmdFailureNotOwner);
    tcase_add_test(tc_virtualAt_test_api,test_002_SendCmd_0004_SendCmdFailureCmdNULL);
    tcase_add_test(tc_virtualAt_test_api,test_002_SendCmd_0005_SendCmdFailureLenWrong);    
    tcase_add_test(tc_virtualAt_test_api,test_003_CloseVirtualAT_0001_CloseVirtualATSuccess);

    return s_virtualAt_test;

}

int runVirtualATTests(void)
{
    SRunner *sr = NULL;
    int failed_number = 0;

    Suite *suiteVirtualATtest = make_virtualAt_test_suite();

    sr = srunner_create(suiteVirtualATtest);

    srunner_run_all(sr,0);


    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed_number;
}
