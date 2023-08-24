
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "pthread.h"
#include <unistd.h>
#include "check.h"


#include "boatlog.h"
//#include "../../boatcheck/boatcheck.h"
#include "boatosal.h"
boatQueue testQueueId;
boatQueue taskQueueId;
#define BOAT_QUEUE_MAX_SIZE 10
#define BOAT_QUEUE_MAX_NUMBER 5
BUINT8 queueMsgTx[BOAT_QUEUE_MAX_SIZE];
BUINT8 queueMsgRx[BOAT_QUEUE_MAX_SIZE];
BUINT32 queueMsgLen;

BOAT_RESULT boatQueueInitQueueIdZero(boatQueue *queueRef);
BOAT_RESULT boatQueueInitQueueNameNULL(boatQueue *queueRef);



START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0010InitQueueSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0010InitQueueSuccess\r\n");
	
	rtnVal = boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	boatQueueDelete(&testQueueId);
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0010InitQueueSuccess finished\r\n");

}
END_TEST


START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0011InitQueueFailQueueAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0011InitQueueFailQueueAddrNULL\r\n");
	
	rtnVal = boatQueueInit(0, "linux-default", BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0011InitQueueFailQueueAddrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0013InitQueueFailMaxSizeZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0013InitQueueFailMaxSizeZero\r\n");
	
	rtnVal = boatQueueInit(&testQueueId, "linux-default", 0, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0013InitQueueFailMaxSizeZero finished\r\n");
}
END_TEST


START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0014InitQueueFailMaxNumberZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0014InitQueueFailMaxNumberZero\r\n");
	
	rtnVal = boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, 0); //!/// maxseize 10 ,  maxNumber 5
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0014InitQueueFailMaxNumberZero finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0015InitQueueFailQueueCreateReturnZero)
{
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Try test_BoAT_OSAL_linuxDefault_05Queue_test_0029InitQueueGetLimit to test \
		 test_BoAT_OSAL_linuxDefault_06Queue_test_0015InitQueueFailQueueCreateReturnZero\r\n");
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0015InitQueueFailQueueCreateReturnZero finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0016DeinitQueueSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0016DeinitQueueSuccess\r\n");
	
	rtnVal = boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	
	
	rtnVal = boatQueueDelete(&testQueueId);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0016DeinitQueueSuccess finished\r\n");

}
END_TEST


START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0017DeinitQueueFailQueueAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0017DeinitQueueFailQueueAddrNULL\r\n");

	
	rtnVal = boatQueueDelete(0);	///// test 0 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0017DeinitQueueFailQueueAddrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0018DeinitQueueFailQueueIdEqualZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0018DeinitQueueFailQueueIdEqualZero\r\n");
	
	
	rtnVal = boatQueueInitQueueIdZero(&testQueueId);	///// QueueId =  0;
	
	rtnVal = boatQueueDelete(&testQueueId);	
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0018DeinitQueueFailQueueIdEqualZero finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0019QueueSendSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0019QueueSendSuccess\r\n");
	

	rtnVal = boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	
	queueMsgLen = 1;
	queueMsgTx[0] = 'A';
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 0);	///// test send 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0019QueueSendSuccess finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0020QueueSendFailQueueAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0020QueueSendFailQueueAddrNULL\r\n");


	rtnVal = boatQueueSend(0,queueMsgTx,queueMsgLen,0);	///// test send 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0020QueueSendFailQueueAddrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0021QueueSendFailQueueIdEqualZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0021QueueSendFailQueueIdEqualZero\r\n");
	
	rtnVal = boatQueueInitQueueIdZero(&testQueueId);	///// create a queueID = 0
	
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 0);	
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0021QueueSendFailQueueIdEqualZero finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0022QueueSendFailMsgPtrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0022QueueSendFailMsgPtrNULL\r\n");
	
	rtnVal = boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	///// set initial number 1 , can call receive one time
	
	rtnVal = boatQueueSend(&testQueueId,NULL,queueMsgLen,0);	
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0022QueueSendFailMsgPtrNULL finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0023QueueSendFailMsgLenGTMaxSize)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0023QueueSendFailMsgLenGTMaxSize\r\n");
	
	rtnVal = boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	///// set initial number 1 , can call receive one time
	
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,BOAT_QUEUE_MAX_SIZE+1,0);	
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0023QueueSendFailMsgLenGTMaxSize finished\r\n");
}
END_TEST


START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0024QueueSendFailMsgLenZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0024QueueSendFailMsgLenZero\r\n");
	
	rtnVal = boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	///// set initial number 1 , can call receive one time
	
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,0,0);	
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0024QueueSendFailMsgLenZero finished\r\n");
}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0025QueueSendFailMqSendReturnZero)
{
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Try test_BoAT_OSAL_linuxDefault_05Queue_test_0029InitQueueGetLimit to test \
		 test_BoAT_OSAL_linuxDefault_06Queue_test_0025QueueSendFailMqSendReturnZero\r\n");
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0025QueueSendFailMqSendReturnZero finished\r\n");

}
END_TEST



START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0026QueueReceiveSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0026QueueReceiveSuccess\r\n");
	
	rtnVal = boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	///// set initial number 1 , can call receive one time
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	queueMsgLen = 1;
	queueMsgTx[0] = 'A';
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 0);	///// test send 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	rtnVal = boatQueueReceive(&testQueueId,queueMsgRx,sizeof(queueMsgRx),0);	///// test receive 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0026QueueReceiveSuccess finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0027QueueTimedReceiveSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0027QueueTimedReceiveSuccess\r\n");
	
	rtnVal = boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	////// init 1 , can receive one time
	
	queueMsgLen = 1;
	queueMsgTx[0] = 'A';
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 0);	///// test send 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	rtnVal = boatQueueReceive(&testQueueId,queueMsgRx,sizeof(queueMsgRx),1000);	///// test timed receive 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0027QueueTimedReceiveSuccess finished\r\n");

}
END_TEST


START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0028QueueReceiveFailQueueAddrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0028QueueReceiveFailQueueAddrNULL\r\n");
	
	rtnVal = boatQueueReceive(0,queueMsgRx,queueMsgLen,0);	///// test receive 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0028QueueReceiveFailQueueAddrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0029QueueReceiveFailQueueIdEqualZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0029QueueReceiveFailQueueIdEqualZero\r\n");
	
	rtnVal = boatQueueInitQueueIdZero(&testQueueId);	///// create a queueID = 0
	
	rtnVal = boatQueueReceive(&testQueueId,queueMsgRx,sizeof(queueMsgRx),0);	///// test receive 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0029QueueReceiveFailQueueIdEqualZero finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0035QueueReceiveFailQueueNameNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0035QueueReceiveFailQueueNameNULL\r\n");
	
	rtnVal = boatQueueInitQueueNameNULL(&testQueueId);	///// create a queueID = 0
		
	rtnVal = boatQueueReceive(&testQueueId,queueMsgRx,sizeof(queueMsgRx),0);	///// test receive 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
		
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0035QueueReceiveFailQueueNameNULL finished\r\n");

}
END_TEST


START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0030QueueReceiveFailMsgPtrNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0030QueueReceiveFailMsgPtrNULL\r\n");
	
	rtnVal = boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5
	
	rtnVal = boatQueueReceive(&testQueueId,0,queueMsgLen,1000);	///// test timed receive 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	rtnVal = boatQueueDelete(&testQueueId);
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0030QueueReceiveFailMsgPtrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0031QueueTimedReceiveFailTimeout)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0031QueueTimedReceiveFailTimeout\r\n");
	
	rtnVal = boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5	
	
	rtnVal = boatQueueReceive(&testQueueId,queueMsgRx,sizeof(queueMsgRx),1000);	///// test timed receive 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);			///// the rtnVal must check!

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0031QueueTimedReceiveFailTimeout timeout [%x]\r\n",rtnVal);

	rtnVal = boatQueueDelete(&testQueueId);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0031QueueTimedReceiveFailTimeout finished\r\n");

}
END_TEST



static void *queueTaskReceiveSleep2s(void *param)
{
	BOAT_RESULT rtnVal;
	BUINT8 testMsgTx[BOAT_QUEUE_MAX_SIZE];
	BUINT8 testMsgRx[BOAT_QUEUE_MAX_SIZE];
	BUINT32 testMsgLen;

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue][task] Testing queue task receive id[%p]\r\n",param);

	struct queueTestStruTask{
		boatQueue *p;
		int counter;
	} *queueTest;

	queueTest = (struct queueTestStruTask *) param;
	boatQueue *ptestQueueId = queueTest->p;/////(boatQueue *)param;

	boatQueueInit(&taskQueueId, "linux-default-task", BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5

	for (int i = 0;i < 5;i++)
	{
		/////sleep(50);
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing queue task receive times[%d] id[%p]\r\n",i,ptestQueueId);
		testMsgLen = 1;
		rtnVal = boatQueueReceive(ptestQueueId,testMsgRx,sizeof(testMsgRx),0);
		sleep(1);///// sleep 2 seconds
		queueTest->counter --;
		testMsgTx[0] = testMsgRx[0]+1;
		rtnVal = boatQueueSend(&taskQueueId,testMsgTx,testMsgLen,0);
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue] queueTaskReceiveSleep2s send rtnVal[%x]\r\n",rtnVal);		
	}
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing queueTaskReceiveSleep2s finished\r\n");
	sleep(1);///// sleep 2 seconds
	boatQueueDelete(&taskQueueId);
	(void)rtnVal;

	return NULL;	
}



START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0032MultiTaskUseOneQueue)
{
	int i;
	struct queueTestStru{
		boatQueue *p;
		int counter;
	} queueTest;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0032MultiTaskUseOneQueue\r\n");

	boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, BOAT_QUEUE_MAX_NUMBER); //!/// maxseize 10 ,  maxNumber 5
	queueTest.counter = 0;
	queueTest.p = &testQueueId;


	pthread_t th;
    pthread_attr_t attr;
											
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_create(&th, &attr, queueTaskReceiveSleep2s, &queueTest);
    if (ret != 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "ERROR: Create queueTaskReceiveSleep2s error\n");
    }
    else
    {
        BoatLog(BOAT_LOG_NORMAL, "INFO: Create thread succ.\n");
    }
	
	/////fibo_thread_create(queueTaskReceiveSleep2s, "queue_sleep_2s", 1024 * 8, &queueTest, OSI_PRIORITY_NORMAL);
	
	sleep(1);
	queueMsgTx[0] = 'A';

	for (i = 0;i < 5;i++)
	{
		/////////sleep(50);
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing queue main receive times[%d]\r\n",i);
		queueMsgLen = 1;
		boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 0);
		boatQueueReceive(&taskQueueId,queueMsgRx,sizeof(queueMsgRx),0);
		queueMsgTx[0] = queueMsgRx[0]+1;
		sleep(1);///// sleep 2 seconds
		queueTest.counter ++;
		BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing queue main return from receive times[%d]\r\n",i);
	}
	boatQueueDelete(&testQueueId);
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] [%d][%c] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0032MultiTaskUseOneQueue finished\r\n",
		queueTest.counter,queueMsgTx[0]);

	ck_assert_int_eq(queueTest.counter,0);
	ck_assert_int_eq(queueMsgTx[0],'A'+10);
	sleep(1);
	
	
}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0033QueueTimedSendSuccess)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0033QueueTimedSendSuccess\r\n");
	

	rtnVal = boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, 1); //!/// maxseize 10 ,  maxNumber 5	
	queueMsgLen = 1;
	queueMsgTx[0] = 'A';
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 1321);	///// test send 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatQueueDelete(&testQueueId);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0033QueueTimedSendSuccess finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0034QueueTimedSendFaileTimeout)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0034QueueTimedSendFaileTimeout\r\n");
	

	rtnVal = boatQueueInit(&testQueueId, "linux-default", BOAT_QUEUE_MAX_SIZE, 1); //!/// maxseize 10 ,  maxNumber 5	
	queueMsgLen = 1;
	queueMsgTx[0] = 'A';
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 1321);	///// test send 
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	rtnVal = boatQueueSend(&testQueueId,queueMsgTx,queueMsgLen, 2200);	///// test send 
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	rtnVal = boatQueueDelete(&testQueueId);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0034QueueTimedSendFaileTimeout finished\r\n");

}
END_TEST

START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0036QueueTimedSendFailMqSendReturnZero)
{
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Try test_BoAT_OSAL_linuxDefault_05Queue_test_0029InitQueueGetLimit to test \
		 test_BoAT_OSAL_linuxDefault_06Queue_test_0036QueueTimedSendFailMqSendReturnZero\r\n");
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0036QueueTimedSendFailMqSendReturnZero finished\r\n");

}
END_TEST


START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0037QueueReceiveFailMqReceiveReturnZero)
{
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Try test_BoAT_OSAL_linuxDefault_05Queue_test_0029InitQueueGetLimit to test \
		 test_BoAT_OSAL_linuxDefault_06Queue_test_0037QueueReceiveFailMqReceiveReturnZero\r\n");
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0037QueueReceiveFailMqReceiveReturnZero finished\r\n");

}
END_TEST


START_TEST(test_BoAT_OSAL_linuxDefault_06Queue_test_0038QueueTimedReceiveFailMqTimedReceiveReturnZero)
{
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Try test_BoAT_OSAL_linuxDefault_05Queue_test_0029InitQueueGetLimit to test \
		 test_BoAT_OSAL_linuxDefault_06Queue_test_0038QueueTimedReceiveFailMqTimedReceiveReturnZero\r\n");
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][queue] Testing test_BoAT_OSAL_linuxDefault_06Queue_test_0038QueueTimedReceiveFailMqTimedReceiveReturnZero finished\r\n");

}
END_TEST



Suite *makeQueuetest_suite(void)
{
    /* Create Suite */
    Suite *sQueuetest = suite_create("queue_test");

    /* Create test cases */
    TCase *tcQueuetest_api = tcase_create("queue_test_api");

    /* Add a test case to the Suite */
    suite_add_tcase(sQueuetest,tcQueuetest_api);
	tcase_set_timeout(tcQueuetest_api,20);

    /* Test cases are added to the test set */

	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0010InitQueueSuccess);
#if 1
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0011InitQueueFailQueueAddrNULL);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0013InitQueueFailMaxSizeZero);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0014InitQueueFailMaxNumberZero);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0015InitQueueFailQueueCreateReturnZero);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0016DeinitQueueSuccess);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0017DeinitQueueFailQueueAddrNULL);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0018DeinitQueueFailQueueIdEqualZero);
	
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0019QueueSendSuccess);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0033QueueTimedSendSuccess);
	
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0020QueueSendFailQueueAddrNULL);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0021QueueSendFailQueueIdEqualZero);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0022QueueSendFailMsgPtrNULL);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0023QueueSendFailMsgLenGTMaxSize);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0024QueueSendFailMsgLenZero);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0025QueueSendFailMqSendReturnZero);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0034QueueTimedSendFaileTimeout);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0036QueueTimedSendFailMqSendReturnZero);
	
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0026QueueReceiveSuccess);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0027QueueTimedReceiveSuccess);
	
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0028QueueReceiveFailQueueAddrNULL);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0029QueueReceiveFailQueueIdEqualZero);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0035QueueReceiveFailQueueNameNULL);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0030QueueReceiveFailMsgPtrNULL);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0037QueueReceiveFailMqReceiveReturnZero);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0031QueueTimedReceiveFailTimeout);
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0038QueueTimedReceiveFailMqTimedReceiveReturnZero);
	
	tcase_add_test(tcQueuetest_api, test_BoAT_OSAL_linuxDefault_06Queue_test_0032MultiTaskUseOneQueue);
#endif
	

    return sQueuetest;

}
