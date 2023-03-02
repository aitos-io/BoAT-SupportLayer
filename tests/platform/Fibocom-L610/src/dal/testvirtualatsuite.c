#include "fibo_opencpu.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


#include "boatlog.h"
#include "../../boatcheck/boatcheck.h"
#include "boatdal.h"

static UINT32 g_virtualAT_lock = 0;

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
    BoatLog(BOAT_LOG_NORMAL,"virtualAT callback_1=>%s,len:%d",content,len);
    g_callback_1_flag = 1;
    if(NULL != strstr(content,"OK"))
    {
        if(g_virtualAT_lock)
        {
            fibo_sem_signal(g_virtualAT_lock);
        }
    }

}

void dalVirtualATCallback2(char *content,BUINT32 len)
{
    BoatLog(BOAT_LOG_NORMAL,"virtualAT callback_2=>%s,len:%d",content,len);
    g_callback_2_flag = 1;

    if(NULL != strstr(content,"OK"))
    {
        if(g_virtualAT_lock)
        {
            fibo_sem_signal(g_virtualAT_lock);
        }
    }
}


static void openVirtualAT_in_thread1(void *param)
{
    BOAT_RESULT result;
    result = boatVirtualATOpen(dalVirtualATCallback1);
    g_openVirtualAT_result_thread1 = result;

    if(g_virtualAT_lock)
    {
        fibo_sem_signal(g_virtualAT_lock);
    }

    fibo_thread_delete();
    
}

static void openVirtualAT_in_thread2(void *param)
{
    BOAT_RESULT result;
    result = boatVirtualATOpen(dalVirtualATCallback2);
    g_openVirtualAT_result_thread2 = result;

    if(g_virtualAT_lock)
    {
        fibo_sem_signal(g_virtualAT_lock);
    }

    fibo_thread_delete();
    
}

static void sendVirtualAT_in_thread(void *param)
{
    BOAT_RESULT result;
    result = boatVirtualATSend(g_cmd,strlen(g_cmd));
    ck_assert_int_eq(result,BOAT_SUCCESS);

    if(g_virtualAT_lock)
    {
        fibo_sem_signal(g_virtualAT_lock);
    }

    fibo_thread_delete();
}


START_TEST(test_001_OpenVirtualAT_0001_OpenVirtualATSuccess)
{

    BOAT_RESULT  result;
    result = boatVirtualATOpen(dalVirtualATCallback1);
    ck_assert_int_eq(result,BOAT_SUCCESS);

    boatVirtualATClose();

}
END_TEST

START_TEST(test_001_OpenVirtualAT_0002_OpenVirtualATFailureTwice)
{
    initThreadOpenVirtualATFlag();
    if (g_virtualAT_lock == 0)
    {
        g_virtualAT_lock = fibo_sem_new(0);
    }


    fibo_thread_create(openVirtualAT_in_thread1, "openVirtualAT_in_thread1", 1024*1, NULL, OSI_PRIORITY_NORMAL);

    fibo_sem_wait(g_virtualAT_lock);

    ck_assert_int_eq(g_openVirtualAT_result_thread1,BOAT_SUCCESS);

    fibo_thread_create(openVirtualAT_in_thread2, "openVirtualAT_in_thread2", 1024*1, NULL, OSI_PRIORITY_NORMAL);

    fibo_sem_wait(g_virtualAT_lock);

    ck_assert_int_eq(g_openVirtualAT_result_thread2,BOAT_ERROR_DAL_VIRTUAL_BUSY);

    boatVirtualATClose();

    fibo_sem_free(g_virtualAT_lock);
    g_virtualAT_lock = 0;

    initThreadOpenVirtualATFlag();

}
END_TEST

START_TEST(test_002_SendCmd_0001_SendCmdSuccess)
{

    BOAT_RESULT  result;
    initCallbackFlag();

    if (g_virtualAT_lock == 0)
    {
        g_virtualAT_lock = fibo_sem_new(0);
    }

    result = boatVirtualATOpen(dalVirtualATCallback1);
    ck_assert_int_eq(result,BOAT_SUCCESS);
    
    result = boatVirtualATSend(g_cmd,strlen(g_cmd));
    ck_assert_int_eq(result,BOAT_SUCCESS);

    fibo_sem_wait(g_virtualAT_lock);
    ck_assert_int_eq(g_callback_1_flag,1);
    ck_assert_int_eq(g_callback_2_flag,0);

    boatVirtualATClose();
    if(g_virtualAT_lock)
    {
        fibo_sem_free(g_virtualAT_lock);
        g_virtualAT_lock = 0;
    }

    initCallbackFlag();

}
END_TEST

START_TEST(test_002_SendCmd_0002_SendCmdFailureNotOpen)
{
    BOAT_RESULT  result;
    result = boatVirtualATSend(g_cmd,strlen(g_cmd));
    ck_assert_int_eq(result,BOAT_ERROR_DAL_VIRTUAL_CLOSED);
}
END_TEST

START_TEST(test_002_SendCmd_0003_SendCmdFailureNotOwner)
{
    initThreadOpenVirtualATFlag();
    initCallbackFlag();

    if (g_virtualAT_lock == 0)
    {
        g_virtualAT_lock = fibo_sem_new(0);
    }

    fibo_thread_create(openVirtualAT_in_thread1, "openVirtualAT_in_thread1", 1024*1, NULL, OSI_PRIORITY_NORMAL);

    fibo_sem_wait(g_virtualAT_lock);

    fibo_thread_create(sendVirtualAT_in_thread, "sendVirtualAT_in_thread", 1024*1, NULL, OSI_PRIORITY_NORMAL);
    
    fibo_sem_wait(g_virtualAT_lock);

    ck_assert_int_eq(g_callback_1_flag,1);
    ck_assert_int_eq(g_callback_2_flag,0);

    boatVirtualATClose();

    fibo_sem_free(g_virtualAT_lock);
    g_virtualAT_lock = 0;

    initCallbackFlag();
    initThreadOpenVirtualATFlag();

}
END_TEST

START_TEST(test_002_SendCmd_0004_SendCmdFailureCmdNULL)
{
    BOAT_RESULT  result;
    result = boatVirtualATSend(NULL,strlen(g_cmd));
    ck_assert_int_eq(result,BOAT_ERROR_DAL_INVALID_ARGUMENT);

}
END_TEST


START_TEST(test_002_SendCmd_0005_SendCmdFailureLenWrong)
{

    BOAT_RESULT  result;
    result = boatVirtualATSend(g_cmd,0);
    ck_assert_int_eq(result,BOAT_ERROR_DAL_INVALID_ARGUMENT);

}
END_TEST

START_TEST(test_003_CloseVirtualAT_0001_CloseVirtualATSuccess)
{
    BOAT_RESULT result;
    result = boatVirtualATClose();
    ck_assert_int_eq(result,BOAT_SUCCESS);
}
END_TEST

Suite *make_virtualAT_test_suite(void)
{
    /* Create Suite */
    Suite *s_virtualAT_test = suite_create("virtualAT_test");

    /* Create test cases */
    TCase *tc_virtualAT_test_api = tcase_create("virtualAT_test_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_virtualAT_test,tc_virtualAT_test_api);

    /* Test cases are added to the test set */

    tcase_add_test(tc_virtualAT_test_api,test_001_OpenVirtualAT_0001_OpenVirtualATSuccess);
    tcase_add_test(tc_virtualAT_test_api,test_001_OpenVirtualAT_0002_OpenVirtualATFailureTwice);
    tcase_add_test(tc_virtualAT_test_api,test_002_SendCmd_0001_SendCmdSuccess);
    tcase_add_test(tc_virtualAT_test_api,test_002_SendCmd_0002_SendCmdFailureNotOpen);
    tcase_add_test(tc_virtualAT_test_api,test_002_SendCmd_0003_SendCmdFailureNotOwner);
    tcase_add_test(tc_virtualAT_test_api,test_002_SendCmd_0004_SendCmdFailureCmdNULL);
    tcase_add_test(tc_virtualAT_test_api,test_002_SendCmd_0005_SendCmdFailureLenWrong);    
    tcase_add_test(tc_virtualAT_test_api,test_003_CloseVirtualAT_0001_CloseVirtualATSuccess);

    return s_virtualAT_test;

}

int runVirtualATTests(void)
{
    SRunner *sr = NULL;
    int failed_number = 0;

    Suite *suiteVirtualATtest = make_virtualAT_test_suite();

    sr = srunner_create(suiteVirtualATtest);

    srunner_run_all(sr,0);


    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed_number;
}
