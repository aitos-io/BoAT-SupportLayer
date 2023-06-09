/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/
#include "boatdal.h"
#include "boatlog.h"
#include "boattypes.h"
#include "fibo_opencpu.h"
#include "../../boatcheck/boatcheck.h"

boatUart testUartId;
boatUartConfig testUartConfig;
BUINT8 boatRxFlg = 0;
//! BUINT16 uartSpeed;	//! 100000 = 100k, 400000 = 400k, 3400000 = 3.4M
//! BUINT8	uartSlaveAddrBits; //! 0: 7 bits, 1: 10 bits
//! BUINT8	uartSlaveDevRegisterAddrLen; //! 0: 1 byte, 1: 2 bytes, 2: 4 byte

void ck_assert_int_eq(BOAT_RESULT rtnVal, BOAT_RESULT eqVal);
void boatUartInitUartIDZero(boatUart *Uart);

void boatUartInitUartIdNagtive(boatUart *uart);

void testUartRxCb(boatUart *uartRef,  unsigned char *data, BUINT32 len)
{
	for(int i =0; i < len ; i++)
	{
		BoatLog(BOAT_LOG_NORMAL,"[boat][uart] rd:%x \r\n",data[i]);		
	}
	if (len == 7)
	{
		if(data[0] == 0)
		{
			if(data[1] == 'a')
			{
				if(data[2] == 'i')
				{
					if(data[3] == 't')
					{
						if(data[4] == 'o')
						{
							if(data[5] == 's')
							{
								if(data[6] == 0xff)
								{
									boatRxFlg = 1;
								}
							}
						}
					}
				}
			}
		}
	}

	BoatLog(BOAT_LOG_NORMAL,"[boat][uart] testUartRxCb \r\n");
}

void testUartConfigSet(boatUartConfig *config, BUINT32 baudrate, BUINT8 databit, 
								BUINT8 stopbit, BUINT8 parity, BUINT32 rxSize, BUINT32 txSize)
{
	config->baudrate = baudrate;
	config->databit = databit;
	config->stopbit = stopbit;	
	config->parity = parity;
	config->rxBufSize = rxSize;
	config->txBufSize = txSize;
}

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0010_boatUartOpen_Successful_ConfigParamete_Right)
{
	BOAT_RESULT rtnVal = 0;

	BoatLog(BOAT_LOG_NORMAL,"[boat][uart] Testing test_BoAT_DAL_FibocomL610_01Uart_test_0010_boatUartOpen_Successful_ConfigParamete_Right\r\n");
	
	BUINT8 databit, stopbit, parity;
	for(databit = 5; databit <=8; databit++)
	{
		for(stopbit = 1; stopbit <=2; stopbit++)
		{
			for(parity = 0; parity <=2; parity++)
			{
				testUartConfigSet(&testUartConfig, 115200, databit, stopbit, parity, 0, 0);
				rtnVal = boatUartInit(&testUartId, 1, testUartConfig, testUartRxCb);
				if(rtnVal == BOAT_ERROR)
				{
					break;
				}				
				boatUartDeinit(&testUartId);///// must destroy the uart once you stop using it.
			}
		}
	}
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	BoatLog(BOAT_LOG_NORMAL,"[boat][uart] Testing test_BoAT_DAL_FibocomL610_01Uart_test_0010_boatUartOpen_Successful_ConfigParamete_Right finished\r\n");
}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0011_boatUartOpen_Failed_uartRefNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0011_boatUartOpen_Failed_uartRefNULL\r\n");
	
	rtnVal =boatUartInit(0, 1, testUartConfig, testUartRxCb);
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0011_boatUartOpen_Failed_uartRefNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0012_boatUartInit_Failed_fiboHalUartInitError)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0012_boatUartInit_Failed_fiboHalUartInitError\r\n");
	testUartConfigSet(&testUartConfig, 115200, 8, 1, 0, 0, 0);
	/////rtnVal =boatUartInit(&testUartId, 1, testUartConfig, testUartRxCb); // 200 port is not exist or a nagtive value
	rtnVal =boatUartInit(0, 1, testUartConfig, testUartRxCb); // 200 port is not exist or a nagtive value
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0012_boatUartInit_Failed_fiboHalUartInitError finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0013_boatUartOpen_Successful_rxBufSizeZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0013_boatUartOpen_Successful_rxBufSizeZero\r\n");
	testUartConfigSet(&testUartConfig, 115200, 8, 1, 0, 0, 5);
	rtnVal =boatUartInit(&testUartId, 1, testUartConfig, testUartRxCb);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0013_boatUartOpen_Successful_rxBufSizeZero finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0014_boatUartOpen_Successful_txBufSizeZero)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0014_boatUartOpen_Successful_txBufSizeZero\r\n");
	testUartConfigSet(&testUartConfig, 115200, 8, 1, 0, 5, 0);
	rtnVal =boatUartInit(&testUartId, 1, testUartConfig, testUartRxCb);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0014_boatUartOpen_Successful_txBufSizeZero finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0015_boatUartOpen_Failed_rxCallbackNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0015_boatUartOpen_Failed_rxCallbackNULL\r\n");
	
	rtnVal =boatUartInit(&testUartId, 1, testUartConfig, NULL);
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0015_boatUartOpen_Failed_rxCallbackNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0018_boatUartOpen_Failed_dataBitsError)
{
	BOAT_RESULT rtnVal = 0;

	BoatLog(BOAT_LOG_NORMAL,"[boat][uart] Testing test_BoAT_DAL_FibocomL610_01Uart_test_0018_boatUartOpen_Failed_dataBitsError\r\n");
	
	for(BUINT8 i = 0; i <= 255; i++)
	{
		if((i >= 5) && (i <= 8)) continue;
		
		testUartConfigSet(&testUartConfig, 115200, i, 1, 0, 0, 0);
		rtnVal = boatUartInit(&testUartId, 1, testUartConfig, testUartRxCb);
		if(rtnVal == BOAT_SUCCESS)
		{
			boatUartDeinit(&testUartId);///// must destroy the uart once you stop using it.
			break;
		}
		BoatLog(BOAT_LOG_NORMAL,"[boat][uart] databit %d ",i);
		if(i==255) break;
	}
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	BoatLog(BOAT_LOG_NORMAL,"[boat][uart] Testing test_BoAT_DAL_FibocomL610_01Uart_test_0018_boatUartOpen_Failed_dataBitsError finished\r\n");
	

}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0017_boatUartOpen_Failed_stopBiteError)
{
	BOAT_RESULT rtnVal = 0;

	BoatLog(BOAT_LOG_NORMAL,"[boat][uart] Testing test_BoAT_DAL_FibocomL610_01Uart_test_0017_boatUartOpen_Failed_stopBiteError\r\n");
	
	for(BUINT8 i = 0;; i++)
	{
		if((i >= 1) && (i <= 2)) continue;
		
		testUartConfigSet(&testUartConfig, 115200, 8, i, 0, 0, 0);
		rtnVal = boatUartInit(&testUartId, 1, testUartConfig, testUartRxCb);
		if(rtnVal == BOAT_SUCCESS)
		{
			boatUartDeinit(&testUartId);///// must destroy the uart once you stop using it.
			break;
		}				
		BoatLog(BOAT_LOG_NORMAL,"[boat][uart] stopbit %d ",i);
		fibo_taskSleep(10);
		if(i==0xff) break;
	}
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	BoatLog(BOAT_LOG_NORMAL,"[boat][uart] Testing test_BoAT_DAL_FibocomL610_01Uart_test_0017_boatUartOpen_Failed_stopBiteError finished\r\n");
	


}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0016_boatUartOpen_Failed_parityError)
{
	BOAT_RESULT rtnVal = BOAT_ERROR;

	BoatLog(BOAT_LOG_NORMAL,"[boat][uart] Testing test_BoAT_DAL_FibocomL610_01Uart_test_0016_boatUartOpen_Failed_parityError\r\n");
	
	for(BUINT8 i = 0;; i++)
	{
		if((i >= 0) && (i <= 2)) continue;
		
		testUartConfigSet(&testUartConfig, 115200, 8, 1, i, 0, 0);
		rtnVal = boatUartInit(&testUartId, 1, testUartConfig, testUartRxCb);
		if(rtnVal == BOAT_SUCCESS)
		{
			boatUartDeinit(&testUartId);///// must destroy the uart once you stop using it.
			break;
		}				
		BoatLog(BOAT_LOG_NORMAL,"[boat][uart] parity %d ",i);
		if(i==0xff) break;
	}
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	BoatLog(BOAT_LOG_NORMAL,"[boat][uart] Testing test_BoAT_DAL_FibocomL610_01Uart_test_0016_boatUartOpen_Failed_parityError finished\r\n");
}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0019_boatUartDeinit_Successful)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0019_boatUartDeinit_Successful\r\n");
	testUartConfigSet(&testUartConfig, 115200, 8, 1, 0, 0, 0);
	rtnVal = boatUartInit(&testUartId, 1, testUartConfig, testUartRxCb);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatUartDeinit(&testUartId);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0019_boatUartDeinit_Successful finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0020_boatUartDeinit_Failed_uartRefNULL)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0020_boatUartDeinit_Failed_uartRefNULL\r\n");
	
	rtnVal =boatUartDeinit(0);
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0020_boatUartDeinit_Failed_uartRefNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0021_boatUartDeinit_Failed_uartIdNagtive)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0021_boatUartDeinit_Failed_uartIdNagtive\r\n");

	boatUartInitUartIdNagtive(&testUartId);
	rtnVal =boatUartDeinit(&testUartId);
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0021_boatUartDeinit_Failed_uartIdNagtive finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0022_boatUartDeinit_Failed_fiboHalUartDeinitError)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0022_boatUartDeinit_Failed_fiboHalUartDeinitError\r\n");

	rtnVal =boatUartDeinit(&testUartId);
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0022_boatUartDeinit_Failed_fiboHalUartDeinitError finished\r\n");

}
END_TEST


START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0023_boatUartWrite_Successful)
{
	BOAT_RESULT rtnVal;
	BUINT8 tdBuf[7]={0,'a','i','t','o','s',0xff};
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0023_boatUartWrite_Successful\r\n");
	testUartConfigSet(&testUartConfig, 115200, 8, 1, 0, 0, 0);


	rtnVal = boatUartInit(&testUartId, 0, testUartConfig, testUartRxCb);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatUartWrite(&testUartId, tdBuf, sizeof(tdBuf));
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);


	while (1)
	{
		
		BoatLog(BOAT_LOG_NORMAL, "Testing waitting for answer\r\n");
		fibo_taskSleep(2000);
		if(boatRxFlg == 1) break;
	}
	
	rtnVal = boatUartDeinit(&testUartId);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0023_boatUartWrite_Successful finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0024_boatUartWrite_Successful_dataLenZero)
{
	BOAT_RESULT rtnVal;
	BUINT8 tdBuf[10];
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0024_boatUartWrite_Successful_dataLenZero\r\n");
	testUartConfigSet(&testUartConfig, 115200, 8, 1, 0, 0, 0);
	rtnVal = boatUartInit(&testUartId, 1, testUartConfig, testUartRxCb);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatUartWrite(&testUartId, tdBuf, 0);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);

	rtnVal = boatUartDeinit(&testUartId);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0024_boatUartWrite_Successful_dataLenZero finished\r\n");

}
END_TEST


START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0025_boatUartWrite_Failed_uartRefNULL)
{
	BOAT_RESULT rtnVal;
	BUINT8 tdBuf[10];
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0025_boatUartWrite_Failed_uartRefNULL\r\n");
	
	rtnVal = boatUartWrite(0, tdBuf, sizeof(tdBuf));
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0025_boatUartWrite_Failed_uartRefNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0026_boatUartWrite_Failed_dataPtrNULL)
{
	BOAT_RESULT rtnVal;
	BUINT8 tdBuf[10];
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0026_boatUartWrite_Failed_dataPtrNULL\r\n");
	
	rtnVal = boatUartWrite(&testUartId, NULL, sizeof(tdBuf));
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0026_boatUartWrite_Failed_dataPtrNULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0027_boatUartWrite_Failed_uartIdNagtive)
{
	BOAT_RESULT rtnVal;
	BUINT8 tdBuf[10];

	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0027_boatUartWrite_Failed_uartIdNagtive\r\n");

	boatUartInitUartIdNagtive(&testUartId);
	rtnVal = boatUartWrite(&testUartId, tdBuf, sizeof(tdBuf));
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0027_boatUartWrite_Failed_uartIdNagtive finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_FibocomL610_01Uart_test_0028_boatUartWrite_Failed_fiboHalUartDeinitError)
{
	BOAT_RESULT rtnVal;
	BUINT8 tdBuf[10];
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0028_boatUartWrite_Failed_fiboHalUartDeinitError\r\n");

	rtnVal = boatUartWrite(&testUartId, tdBuf, sizeof(tdBuf));
	ck_assert_int_eq(rtnVal,BOAT_ERROR);

	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_FibocomL610_01Uart_test_0028_boatUartWrite_Failed_fiboHalUartDeinitError finished\r\n");

}
END_TEST



Suite *makeUarttest_suite(void)
{
    /* Create Suite */
    Suite *sUarttest = suite_create("uart_test");

    /* Create test cases */
    TCase *tcUarttest_api = tcase_create("uart_test_api");

    /* Add a test case to the Suite */
    suite_add_tcase(sUarttest,tcUarttest_api);

    /* Test cases are added to the test set */
#if 0
	
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0010_boatUartOpen_Successful_ConfigParamete_Right);

	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0011_boatUartOpen_Failed_uartRefNULL);
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0012_boatUartInit_Failed_fiboHalUartInitError);
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0013_boatUartOpen_Successful_rxBufSizeZero);
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0014_boatUartOpen_Successful_txBufSizeZero);
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0015_boatUartOpen_Failed_rxCallbackNULL);


	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0019_boatUartDeinit_Successful);
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0020_boatUartDeinit_Failed_uartRefNULL);
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0021_boatUartDeinit_Failed_uartIdNagtive);
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0022_boatUartDeinit_Failed_fiboHalUartDeinitError);
	/////tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0023_boatUartWrite_Successful);
#endif
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0016_boatUartOpen_Failed_parityError);
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0017_boatUartOpen_Failed_stopBiteError);
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0018_boatUartOpen_Failed_dataBitsError);
#if 0

	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0024_boatUartWrite_Successful_dataLenZero);
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0025_boatUartWrite_Failed_uartRefNULL);
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0026_boatUartWrite_Failed_dataPtrNULL);
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0027_boatUartWrite_Failed_uartIdNagtive);
	tcase_add_test(tcUarttest_api, test_BoAT_DAL_FibocomL610_01Uart_test_0028_boatUartWrite_Failed_fiboHalUartDeinitError);


#endif


    return sUarttest;

}

int runUartTests(void)
{
    SRunner *sr = NULL;
    int failed_number = 0;

    Suite *suiteUarttest = makeUarttest_suite();

    sr = srunner_create(suiteUarttest);

    srunner_run_all(sr,0);


    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed_number;
}


