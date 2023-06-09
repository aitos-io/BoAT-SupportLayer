#include "cm_os.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"


#include "boatlog.h"
#include "boatcheck.h"
#include "boatosal.h"


boatQueue testQueueId;
boatQueue taskQueueId;
#define BOAT_QUEUE_MAX_SIZE 10
#define BOAT_QUEUE_MAX_NUMBER 5
BUINT8 queueMsgTx[BOAT_QUEUE_MAX_SIZE];
BUINT8 queueMsgRx[BOAT_QUEUE_MAX_SIZE];
BUINT32 queueMsgLen;

BOAT_RESULT boatQueueInitQueueIdZero(boatQueue *queueRef);


#if 1
START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0010InitQueueSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0010InitQueueSuccess\r\n");
	
	rtnVal = boatQueueInit(&testQueueId,NULL, BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	boatQueueDelete(&testQueueId);
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0010InitQueueSuccess finished\r\n");

}
END_TEST


START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0011InitQueueFailQueueAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0011InitQueueFailQueueAddrNULL\r\n");
	
	rtnVal = boatQueueInit(0,NULL, BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0011InitQueueFailQueueAddrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0013InitQueueFailMaxSizeZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0013InitQueueFailMaxSizeZero\r\n");
	
	rtnVal = boatQueueInit(&testQueueId,NULL, 0, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0013InitQueueFailMaxSizeZero finished\r\n");
}
END_TEST


START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0014InitQueueFailMaxNumberZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0014InitQueueFailMaxNumberZero\r\n");
	
	rtnVal = boatQueueInit(&testQueueId,NULL, BOAT_QUEUE_MAX_SIZE, 0); //!/// maxseize 10 ,  maxNumber 5
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0014InitQueueFailMaxNumberZero finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0015InitQueueFailQueueCreateReturnZero)
{
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Try test_BoAT_OSAL_ML307A_05Queue_test_0029InitQueueGetLimit to test \
		 test_BoAT_OSAL_ML307A_06Queue_test_0015InitQueueFailQueueCreateReturnZero\r\n");
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0015InitQueueFailQueueCreateReturnZero finished\r\n");

}
END_TEST


START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0016DeinitQueueSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0016DeinitQueueSuccess\r\n");
	
	rtnVal = boatQueueInit(&testQueueId,NULL, BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	
	
	rtnVal = boatQueueDelete(&testQueueId);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0016DeinitQueueSuccess finished\r\n");

}
END_TEST

#if 1
START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0017DeinitQueueFailQueueAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0017DeinitQueueFailQueueAddrNULL\r\n");

	
	rtnVal = boatQueueDelete(0);	///// test 0 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0017DeinitQueueFailQueueAddrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0018DeinitQueueFailQueueIdEqualZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0018DeinitQueueFailQueueIdEqualZero\r\n");
	
	
	rtnVal = boatQueueInitQueueIdZero(&testQueueId);	///// QueueId =  0;
	
	rtnVal = boatQueueDelete(&testQueueId);	
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0018DeinitQueueFailQueueIdEqualZero finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0019QueueSendSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0019QueueSendSuccess\r\n");
	

	rtnVal = boatQueueInit(&testQueueId,NULL, BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	
	queueMsgLen = 1;
	queueMsgTx[0] = 'A';
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 0);	///// test send 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0019QueueSendSuccess finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0020QueueSendFailQueueAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0020QueueSendFailQueueAddrNULL\r\n");


	rtnVal = boatQueueSend(0,queueMsgTx,queueMsgLen,0);	///// test send 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0020QueueSendFailQueueAddrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0021QueueSendFailQueueIdEqualZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0021QueueSendFailQueueIdEqualZero\r\n");
	
	rtnVal = boatQueueInitQueueIdZero(&testQueueId);	///// create a queueID = 0
	
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 0);	
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0021QueueSendFailQueueIdEqualZero finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0022QueueSendFailMsgPtrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0022QueueSendFailMsgPtrNULL\r\n");
	
	rtnVal = boatQueueInit(&testQueueId,NULL, BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	///// set initial number 1 , can call receive one time
	
	rtnVal = boatQueueSend(&testQueueId,NULL,queueMsgLen,0);	
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0022QueueSendFailMsgPtrNULL finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0023QueueSendFailMsgLenGTMaxSize)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0023QueueSendFailMsgLenGTMaxSize\r\n");
	
	rtnVal = boatQueueInit(&testQueueId,NULL, BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	///// set initial number 1 , can call receive one time
	
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,BOAT_QUEUE_MAX_SIZE+1,0);	
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0023QueueSendFailMsgLenGTMaxSize finished\r\n");
}
END_TEST


START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0024QueueSendFailMsgLenZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0024QueueSendFailMsgLenZero\r\n");
	
	rtnVal = boatQueueInit(&testQueueId,NULL, BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	///// set initial number 1 , can call receive one time
	
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,0,0);	
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0024QueueSendFailMsgLenZero finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0025QueueSendFailFiboQueuePutReturnZero)
{
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Try test_BoAT_OSAL_ML307A_05Queue_test_0029InitQueueGetLimit to test \
		 test_BoAT_OSAL_ML307A_06Queue_test_0025QueueSendFailFiboQueuePutReturnZero\r\n");
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0025QueueSendFailFiboQueuePutReturnZero finished\r\n");

}
END_TEST



START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0026QueueReceiveSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0026QueueReceiveSuccess\r\n");
	
	rtnVal = boatQueueInit(&testQueueId,NULL, BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	///// set initial number 1 , can call receive one time
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	queueMsgLen = 1;
	queueMsgTx[0] = 'A';
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 0);	///// test send 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	rtnVal = boatQueueReceive(&testQueueId,queueMsgRx,queueMsgLen,0);	///// test receive 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0026QueueReceiveSuccess finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0027QueueTimeReceiveSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0027QueueTimeReceiveSuccess\r\n");
	
	rtnVal = boatQueueInit(&testQueueId,NULL, BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	////// init 1 , can receive one time
	
	queueMsgLen = 1;
	queueMsgTx[0] = 'A';
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 0);	///// test send 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	rtnVal = boatQueueReceive(&testQueueId,queueMsgRx,queueMsgLen,1000);	///// test timed receive 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0027QueueTimeReceiveSuccess finished\r\n");

}
END_TEST


START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0028QueueReceiveFailQueueAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0028QueueReceiveFailQueueAddrNULL\r\n");
	
	rtnVal = boatQueueReceive(0,queueMsgRx,queueMsgLen,0);	///// test receive 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0028QueueReceiveFailQueueAddrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0029QueueReceiveFailQueueIdEqualZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0029QueueReceiveFailQueueIdEqualZero\r\n");
	
	rtnVal = boatQueueInitQueueIdZero(&testQueueId);	///// create a queueID = 0
	
	rtnVal = boatQueueReceive(&testQueueId,queueMsgRx,queueMsgLen,0);	///// test receive 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0029QueueReceiveFailQueueIdEqualZero finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0030QueueTimeReceiveFailMsgPtrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0030QueueTimeReceiveFailMsgPtrNULL\r\n");
	
	rtnVal = boatQueueInit(&testQueueId,NULL, BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5
	
	rtnVal = boatQueueReceive(&testQueueId,0,queueMsgLen,1000);	///// test timed receive 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	rtnVal = boatQueueDelete(&testQueueId);
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0030QueueTimeReceiveFailMsgPtrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0031QueueTimeReceiveFailTimeout)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0031QueueTimeReceiveFailTimeout\r\n");
	
	rtnVal = boatQueueInit(&testQueueId,NULL, BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	
	
	rtnVal = boatQueueReceive(&testQueueId,queueMsgRx,queueMsgLen,1000);	///// test timed receive 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);			///// the rtnVal must check!

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0031QueueTimeReceiveFailTimeout timeout [%x]\r\n",rtnVal);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0031QueueTimeReceiveFailTimeout finished\r\n");

}
END_TEST



static void queueTaskReceiveSleep2s(void *param)
{
	BOAT_RESULT rtnVal;
	BUINT8 testMsgTx[BOAT_QUEUE_MAX_NUMBER];
	BUINT8 testMsgRx[BOAT_QUEUE_MAX_NUMBER];
	BUINT32 testMsgLen;

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue][task] Testing queue task receive id[%x]\r\n",param);

	struct queueTestStruTask{
		boatQueue *p;
		int counter;
	} *queueTest;

	queueTest = (struct queueTestStruTask *) param;
	boatQueue *ptestQueueId = queueTest->p;/////(boatQueue *)param;

	boatQueueInit(&taskQueueId,NULL, BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5

	for (int i = 0;i < 5;i++)
	{
		BoatSleepMs(50);
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing queue task receive times[%d] id[%x]\r\n",i,ptestQueueId);
		testMsgLen = 1;
		rtnVal = boatQueueReceive(ptestQueueId,testMsgRx,testMsgLen,0);
		BoatSleepMs(1500);///// sleep 2 seconds
		queueTest->counter --;
		testMsgTx[0] = testMsgRx[0]+1;
		rtnVal = boatQueueSend(&taskQueueId,testMsgTx,testMsgLen,0);
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue] queueTaskReceiveSleep2s send rtnVal[%x]\r\n",rtnVal);		
	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing queueTaskReceiveSleep2s finished\r\n");
	BoatSleepMs(1000);///// sleep 2 seconds
	boatQueueDelete(&taskQueueId);


	osThreadTerminate(osThreadGetId());

	return ;	
}



START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0032MultiTaskUseOneQueue)
{
	int i;
	struct queueTestStru{
		boatQueue *p;
		int counter;
	} queueTest;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0032MultiTaskUseOneQueue\r\n");

	boatQueueInit(&testQueueId,NULL, BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5
	queueTest.counter = 0;
	queueTest.p = &testQueueId;
	
	//fibo_thread_create(queueTaskReceiveSleep2s, "queue_sleep_2s", 1024 * 8, &queueTest, OSI_PRIORITY_NORMAL);
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "queue_sleep_func";
	app_task_attr.stack_size = 1024*8;
	app_task_attr.priority = osPriorityNormal;
	
    osThreadNew((osThreadFunc_t)queueTaskReceiveSleep2s,&queueTest,&app_task_attr);
	
	BoatSleepMs(1200);
	queueMsgTx[0] = 'A';

	for (i = 0;i < 5;i++)
	{
		BoatSleepMs(50);
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing queue main receive times[%d]\r\n",i);
		queueMsgLen = 1;
		boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 0);
		boatQueueReceive(&taskQueueId,queueMsgRx,queueMsgLen,0);
		queueMsgTx[0] = queueMsgRx[0]+1;
		BoatSleepMs(1500);///// sleep 2 seconds
		queueTest.counter ++;
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing queue main return from receive times[%d]\r\n",i);
	}
	boatQueueDelete(&testQueueId);
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] [%d][%c] Testing test_BoAT_OSAL_ML307A_06Queue_test_0032MultiTaskUseOneQueue finished\r\n",
		queueTest.counter,queueMsgTx[0]);

	ck_assert_int_eq(queueTest.counter,0);
	ck_assert_int_eq(queueMsgTx[0],'A'+10);
	BoatSleepMs(1200);
	
	
}
END_TEST

START_TEST(test_BoAT_OSAL_ML307A_06Queue_test_0033QueueTimedSendSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0033QueueTimedSendSuccess\r\n");
	

	rtnVal = boatQueueInit(&testQueueId,NULL, BOAT_QUEUE_MAX_SIZE, 1); //!/// maxseize 10 ,  maxNumber 5	
	queueMsgLen = 1;
	queueMsgTx[0] = 'A';
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 100);	///// test send 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 100);	///// test send 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_ML307A_06Queue_test_0033QueueTimedSendSuccess finished\r\n");

}
END_TEST


#endif

Suite *makeQueuetest_suite(void)
{
    /* Create Suite */
    Suite *sQueuetest = suite_create("queue_test");

    /* Create test cases */
    TCase *tcQueuetest_api = tcase_create("queue_test_api");

    /* Add a test case to the Suite */
    suite_add_tcase(sQueuetest,tcQueuetest_api);

    /* Test cases are added to the test set */

#if 0
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0010InitQueueSuccess);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0011InitQueueFailQueueAddrNULL);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0013InitQueueFailMaxSizeZero);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0014InitQueueFailMaxNumberZero);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0015InitQueueFailQueueCreateReturnZero);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0016DeinitQueueSuccess);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0017DeinitQueueFailQueueAddrNULL);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0018DeinitQueueFailQueueIdEqualZero);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0019QueueSendSuccess);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0020QueueSendFailQueueAddrNULL);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0021QueueSendFailQueueIdEqualZero);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0022QueueSendFailMsgPtrNULL);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0023QueueSendFailMsgLenGTMaxSize);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0024QueueSendFailMsgLenZero);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0025QueueSendFailFiboQueuePutReturnZero);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0026QueueReceiveSuccess);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0027QueueTimeReceiveSuccess);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0028QueueReceiveFailQueueAddrNULL);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0029QueueReceiveFailQueueIdEqualZero);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0030QueueTimeReceiveFailMsgPtrNULL);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0031QueueTimeReceiveFailTimeout);
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0032MultiTaskUseOneQueue);
#endif
	tcase_add_test(tcQueuetest_api,test_BoAT_OSAL_ML307A_06Queue_test_0033QueueTimedSendSuccess);
	

    return sQueuetest;

}
#endif

int runQueueTests(void)
{

#if 1
    SRunner *sr = NULL;
    int failed_number = 0;

    Suite *suiteQueuetest = makeQueuetest_suite();

    sr = srunner_create(suiteQueuetest);

    srunner_run_all(sr,0);


    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed_number;
#endif

	
}

