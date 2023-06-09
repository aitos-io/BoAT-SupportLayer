#include "fibo_opencpu.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


#include "boatlog.h"
#include "../../boatcheck/boatcheck.h"
#include "boatdal.h"
#include "boatosal.h"

static UINT32 g_bleService_lock = 0;

UINT32 g_msg_lock = 0;

static int g_bleService_callback1_flag = 0;
static int g_bleService_callback2_flag = 0;

static int g_recvMsg_flag = 0;

static BOAT_RESULT g_initBleService_result_thread1 = BOAT_ERROR;
static BOAT_RESULT g_initBleService_result_thread2 = BOAT_ERROR;

static void initCallbackFlag(void)
{
    g_bleService_callback1_flag = 0;
    g_bleService_callback2_flag = 0;
}

static void initThreadInitBleServiceResult(void)
{
    g_initBleService_result_thread1 = BOAT_ERROR;
    g_initBleService_result_thread2 = BOAT_ERROR;
 }

int dalBleServiceCallback1(BUINT8 *msg,BUINT32 len )
{
    BoatLog(BOAT_LOG_NORMAL,"bleService get callback_1 msg:%s,len:%d",msg,len);

    g_bleService_callback1_flag = 1;

    return 0;

}
int dalBleServiceCallback2(BUINT8 *msg,BUINT32 len )
{
    BoatLog(BOAT_LOG_NORMAL,"bleService get callback_2 msg:%s,len:%d",msg,len);

    g_bleService_callback2_flag = 1;

    return 0;

}

int dalBleServiceCallback3(BUINT8 *msg,BUINT32 len )
{
    BoatLog(BOAT_LOG_NORMAL,"bleService get callback_3 msg:%s,len:%d",msg,len);

    //g_bleService_callback2_flag = 1;
    g_recvMsg_flag = 1;

    return 0;

}


static void initBleService_in_thread1(void *param)
{
    BOAT_RESULT result;
    result = boatBleServiceInit(dalBleServiceCallback1);
    g_initBleService_result_thread1 = result;

    if(g_bleService_lock)
    {
        fibo_sem_signal(g_bleService_lock);
    }

    fibo_thread_delete();
    
}

static void initBleService_in_thread2(void *param)
{
    BOAT_RESULT result;
    result = boatBleServiceInit(dalBleServiceCallback2);
    g_initBleService_result_thread2 = result;

    if(g_bleService_lock)
    {
        fibo_sem_signal(g_bleService_lock);
    }

    
    fibo_thread_delete();
    
}

START_TEST(test_001_InitBleService_0001_InitBleServiceSuccess)
{
    BOAT_RESULT result;

    result = boatBleServiceInit(dalBleServiceCallback1);
    ck_assert_int_eq(result,BOAT_SUCCESS);

    BoatSleep(2);

    boatBleServiceDeinit();

}
END_TEST

START_TEST(test_001_InitBleService_0002_InitBleServiceFailureTwice)
{
    initThreadInitBleServiceResult();

    BoatSleep(3);

    if(g_bleService_lock == 0)
    {
        g_bleService_lock = fibo_sem_new(0);
    }

    fibo_thread_create(initBleService_in_thread1, "initBleService_in_thread1", 1024*4, NULL, OSI_PRIORITY_NORMAL);

    fibo_sem_wait(g_bleService_lock);

    ck_assert_int_eq(g_initBleService_result_thread1,BOAT_SUCCESS);

    BoatSleep(3);

    fibo_thread_create(initBleService_in_thread2, "initBleService_in_thread2", 1024*2, NULL, OSI_PRIORITY_NORMAL);

    fibo_sem_wait(g_bleService_lock);

    ck_assert_int_eq(g_initBleService_result_thread2,BOAT_ERROR_DAL_BLESERVICE_INITIALIZED);

    boatBleServiceDeinit();
    BoatSleep(2);
    
    if(g_bleService_lock)
    {
        fibo_sem_free(g_bleService_lock);
         g_bleService_lock = 0;
    }
   

    initThreadInitBleServiceResult();

}
END_TEST

START_TEST(test_002_TransmitMsg_0001_SendNotificationSuccess)
{
    BOAT_RESULT result;

    BoatSleep(3);

    if(g_msg_lock == 0)
    {
        g_msg_lock = fibo_sem_new(0);
    }

    result = boatBleServiceInit(dalBleServiceCallback1);
    ck_assert_int_eq(result,BOAT_SUCCESS);

    fibo_sem_wait(g_msg_lock);
    BoatSleep(15);
    BoatLog(BOAT_LOG_NORMAL,"Ready Send Notification_msg!");

    result = boatBleServiceSendNOTIFICATION((BUINT8 *)"Notification_msg_from_L610",strlen("Notification_msg_from_L610"));
    ck_assert_int_eq(result,BOAT_SUCCESS);

    if(g_msg_lock)
    {
        fibo_sem_free(g_msg_lock);
        g_msg_lock = 0;
    }

    BoatSleep(10);
    
    boatBleServiceDeinit();

}
END_TEST

START_TEST(test_002_TransmitMsg_0002_SendIndicationSuccess)
{
    BOAT_RESULT result;

    BoatSleep(3);

    if(g_msg_lock == 0)
    {
        g_msg_lock = fibo_sem_new(0);
    }

    result = boatBleServiceInit(dalBleServiceCallback1);
    ck_assert_int_eq(result,BOAT_SUCCESS);

    fibo_sem_wait(g_msg_lock);
    BoatSleep(15);
    BoatLog(BOAT_LOG_NORMAL,"Ready Send Indication_msg!");

    result = boatBleServiceSendINDICATION((BUINT8 *)"Indication_msg_from_L610",strlen("Indication_msg_from_L610"));
    ck_assert_int_eq(result,BOAT_SUCCESS);

    if(g_msg_lock)
    {
        fibo_sem_free(g_msg_lock);
        g_msg_lock = 0;
    }

    BoatSleep(10);

    boatBleServiceDeinit();
}
END_TEST

START_TEST(test_002_TransmitMsg_0003_SendNotificationFailureBufNull)
{
    BOAT_RESULT result;

    result = boatBleServiceSendNOTIFICATION(NULL,10);
    ck_assert_int_eq(result,BOAT_ERROR_DAL_INVALID_ARGUMENT);

}
END_TEST

START_TEST(test_002_TransmitMsg_0004_SendNotificationFailureLenWrong)
{
    BOAT_RESULT result;

    result = boatBleServiceSendNOTIFICATION((BUINT8 *)"hello",0);
    ck_assert_int_eq(result,BOAT_ERROR_DAL_INVALID_ARGUMENT);

}
END_TEST

START_TEST(test_002_TransmitMsg_0005_SendIndicationFailureBufNull)
{
    BOAT_RESULT result;

    result = boatBleServiceSendINDICATION(NULL,10);
    ck_assert_int_eq(result,BOAT_ERROR_DAL_INVALID_ARGUMENT);


}
END_TEST

START_TEST(test_002_TransmitMsg_0006_SendIndicationFailureLenWrong)
{
    BOAT_RESULT result;

    result = boatBleServiceSendINDICATION((BUINT8 *)"hello",0);
    ck_assert_int_eq(result,BOAT_ERROR_DAL_INVALID_ARGUMENT);

}
END_TEST

START_TEST(test_002_TransmitMsg_0007_recvMsgSuccess)
{
    BOAT_RESULT result;
    BoatSleep(3);

    result = boatBleServiceInit(dalBleServiceCallback3);
    ck_assert_int_eq(result,BOAT_SUCCESS);

    BoatSleep(2);

    int count = 50;
    while(count--)
    {
        if(g_recvMsg_flag)
        {
            BoatLog(BOAT_LOG_NORMAL,"Receive message from the client success!");
            break;
        }
        BoatLog(BOAT_LOG_NORMAL,"Waite for the client to link,and receive message······");
        BoatSleep(1);
    }

    ck_assert(count > 0);

    boatBleServiceDeinit();


}
END_TEST

START_TEST(test_003_DeinitBleService_0001_DeinitBleServiceSuccess)
{
    BOAT_RESULT result;
    result = boatBleServiceDeinit();
    ck_assert_int_eq(result,BOAT_SUCCESS);

}
END_TEST


Suite *make_bleService_test_suite(void)
{
    /* Create Suite */
    Suite *s_bleService_test = suite_create("bleService_test");

    /* Create test cases */
    TCase *tc_bleService_test_api = tcase_create("bleService_test_api");

    /* Add a test case to the Suite */
    suite_add_tcase(s_bleService_test,tc_bleService_test_api);

    /* Test cases are added to the test set */

    tcase_add_test(tc_bleService_test_api,test_001_InitBleService_0001_InitBleServiceSuccess);
    tcase_add_test(tc_bleService_test_api,test_001_InitBleService_0002_InitBleServiceFailureTwice);
    tcase_add_test(tc_bleService_test_api,test_002_TransmitMsg_0001_SendNotificationSuccess);
    tcase_add_test(tc_bleService_test_api,test_002_TransmitMsg_0002_SendIndicationSuccess);
    tcase_add_test(tc_bleService_test_api,test_002_TransmitMsg_0003_SendNotificationFailureBufNull);
    tcase_add_test(tc_bleService_test_api,test_002_TransmitMsg_0004_SendNotificationFailureLenWrong);
    tcase_add_test(tc_bleService_test_api,test_002_TransmitMsg_0005_SendIndicationFailureBufNull);
    tcase_add_test(tc_bleService_test_api,test_002_TransmitMsg_0006_SendIndicationFailureLenWrong);
    tcase_add_test(tc_bleService_test_api,test_002_TransmitMsg_0007_recvMsgSuccess);
    tcase_add_test(tc_bleService_test_api,test_003_DeinitBleService_0001_DeinitBleServiceSuccess);

    return s_bleService_test;

}

int runBleServiceTests(void)
{
    SRunner *sr = NULL;
    int failed_number = 0;

    Suite *suiteBleServicetest = make_bleService_test_suite();

    sr = srunner_create(suiteBleServicetest);

    srunner_run_all(sr,0);


    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed_number;
}

