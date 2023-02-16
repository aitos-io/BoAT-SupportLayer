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

boatI2c testI2CID;
boatI2cConfig testI2CConfig;
//! BUINT16 i2cSpeed;	//! 100000 = 100k, 400000 = 400k, 3400000 = 3.4M
//! BUINT8	i2cSlaveAddrBits; //! 0: 7 bits, 1: 10 bits
//! BUINT8	i2cSlaveDevRegisterAddrLen; //! 0: 1 byte, 1: 2 bytes, 2: 4 byte

void boatAssert_int_eq(BOAT_RESULT rtnVal, BOAT_RESULT eqVal);
void boatI2CInitI2CIDZero(boatI2c *I2C);


void testI2CConfigSet(boatI2cConfig *config, BUINT32 speed, BUINT8 SlavAddrBits, BUINT8 SlaveRegAddrLen)
{
	config->i2cSpeed = speed;
	config->i2cSlaveAddrBits = SlavAddrBits;
	config->i2cSlaveDevRegisterAddrLen = SlaveRegAddrLen;	
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0010_boatI2COpenPort1_Successful_speed100k_SlaveAddrBits7_RegAddr2Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0010_boatI2COpenPort1_Successful_speed100k_SlaveAddrBits7_RegAddr2Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0010_boatI2COpenPort1_Successful_speed100k_SlaveAddrBits7_RegAddr2Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0011_boatI2COpenPort1_Successful_speed400k_SlaveAddrBits7_RegAddr2Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0011_boatI2COpenPort1_Successful_speed400k_SlaveAddrBits7_RegAddr2Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 400000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0011_boatI2COpenPort1_Successful_speed400k_SlaveAddrBits7_RegAddr2Bytes finished\r\n");
	
	return NULL;
}



void *test_BoAT_DAL_FibocomL610_01I2C_test_0012_boatI2COpenPort2_Successful_speed100k_SlaveAddrBits7_RegAddr2Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0012_boatI2COpenPort2_Successful_speed100k_SlaveAddrBits7_RegAddr2Bytes\r\n");	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 2, connect to NXP sm chip
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0012_boatI2COpenPort2_Successful_speed100k_SlaveAddrBits7_RegAddr2Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0013_boatI2COpenPort2_Successful_speed400k_SlaveAddrBits7_RegAddr2Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0013_boatI2COpenPort2_Successful_speed400k_SlaveAddrBits7_RegAddr2Bytes\r\n");	
	testI2CConfigSet(&testI2CConfig, 400000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 2, connect to NXP sm chip	
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0013_boatI2COpenPort2_Successful_speed400k_SlaveAddrBits7_RegAddr2Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0014_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits7_RegAddr1Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0014_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits7_RegAddr1Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 0); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0014_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits7_RegAddr1Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0015_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits7_RegAddr4Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0015_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits7_RegAddr4Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 2); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0015_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits7_RegAddr4Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0016_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits10_RegAddr1Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0016_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits10_RegAddr1Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 1, 0); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0016_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits10_RegAddr1Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0017_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits10_RegAddr2Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0017_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits10_RegAddr2Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 1, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0017_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits10_RegAddr2Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0018_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits10_RegAddr4Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0018_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits10_RegAddr4Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 1, 2); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0018_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits10_RegAddr4Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0019_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits7_RegAddr1Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0019_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits7_RegAddr1Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 400000, 0, 0); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0019_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits7_RegAddr1Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0020_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits7_RegAddr4Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0020_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits7_RegAddr4Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 400000, 0, 2); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0020_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits7_RegAddr4Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0021_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits10_RegAddr1Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0021_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits10_RegAddr1Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 400000, 1, 0); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0021_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits10_RegAddr1Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0022_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits10_RegAddr2Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0022_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits10_RegAddr2Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 400000, 1, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0022_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits10_RegAddr2Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0023_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits10_RegAddr4Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0023_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits10_RegAddr4Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 400000, 1, 2); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0023_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits10_RegAddr4Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0024_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits7_RegAddr1Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0024_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits7_RegAddr1Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 0); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0024_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits7_RegAddr1Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0025_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits7_RegAddr4Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0025_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits7_RegAddr4Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 2); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0025_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits7_RegAddr4Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0026_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits10_RegAddr1Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0026_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits10_RegAddr1Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 1, 0); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0026_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits10_RegAddr1Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0027_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits10_RegAddr2Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0027_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits10_RegAddr2Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 1, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0027_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits10_RegAddr2Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0028_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits10_RegAddr4Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0028_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits10_RegAddr4Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 1, 2); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0028_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits10_RegAddr4Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0029_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits7_RegAddr1Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0029_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits7_RegAddr1Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 400000, 0, 0); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0029_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits7_RegAddr1Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0030_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits7_RegAddr4Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0030_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits7_RegAddr4Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 400000, 0, 2); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0030_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits7_RegAddr4Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0031_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits10_RegAddr1Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0031_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits10_RegAddr1Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 400000, 1, 0); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0031_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits10_RegAddr1Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0032_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits10_RegAddr2Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0032_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits10_RegAddr2Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 400000, 1, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0032_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits10_RegAddr2Bytes finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0033_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits10_RegAddr4Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0033_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits10_RegAddr4Bytes\r\n");
	
	testI2CConfigSet(&testI2CConfig, 400000, 1, 2); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	fibo_taskSleep(500);
	boatAssert_int_eq(rtnVal,0);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0033_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits10_RegAddr4Bytes finished\r\n");
	
	return NULL;
}


void *test_BoAT_DAL_FibocomL610_01I2C_test_0034_boatI2COpen_Failed_portNumber_notEQ1_notEQ2_speed100k_SlaveAddrBits7_RegAddr2Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0034_boatI2COpen_Failed_portNumber_notEQ1_notEQ2_speed100k_SlaveAddrBits7_RegAddr2Bytes\r\n");
	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	for(BUINT8 i = 0;;i++)
	{
		if((i==1) || (i==2)) continue;
		rtnVal = boatI2cOpen(&testI2CID, i, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
		if(rtnVal == BOAT_SUCCESS)
		{
			BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0034_boatI2COpen_Failed_portNumber_notEQ1_notEQ2_speed100k_SlaveAddrBits7_RegAddr2Bytes failed[%x] \r\n",i);
			break;////return NULL;
		}
		/////boatAssert_int_eq(rtnVal,0);
		if(i==0xff) break;
		/////fibo_taskSleep(100);
	}
	/////boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,-1);
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0034_boatI2COpen_Failed_portNumber_notEQ1_notEQ2_speed100k_SlaveAddrBits7_RegAddr2Bytes finished -- Successful\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0035_boatI2COpen_Failed_port1_speed_notEQ100k_notEQ400k_SlaveAddrBits_RegAddr2Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0035_boatI2COpen_Failed_port1_speed_notEQ100k_notEQ400k_SlaveAddrBits_RegAddr2Bytes\r\n");
	for(BUINT32 i = 0;;i++)
	{
		if((i==100000) || (i==400000)) continue;
		testI2CConfigSet(&testI2CConfig, i, 0, 1); //! speed:i, slave addr bits:7, register address length: 2 bytes
		rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
		if(rtnVal == BOAT_SUCCESS)
		{
			BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0035_boatI2COpen_Failed_port1_speed_notEQ100k_notEQ400k_SlaveAddrBits_RegAddr2Bytes failed[%x] \r\n",i);
			break;/////return NULL;
		}
		/////boatAssert_int_eq(rtnVal,0);
		if(i==0xffffffff) break;
		/////fibo_taskSleep(100);
	}
	/////boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,-1);
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0035_boatI2COpen_Failed_port1_speed_notEQ100k_notEQ400k_SlaveAddrBits_RegAddr2Bytes finished -- Successful\r\n");
	
	return NULL;
}


void *test_BoAT_DAL_FibocomL610_01I2C_test_0036_boatI2COpen_Failed_port1_speed100k_SlaveAddrBits_notEQ0_notEQ1_RegAddr2Bytes(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0036_boatI2COpen_Failed_port1_speed100k_SlaveAddrBits_notEQ0_notEQ1_RegAddr2Bytes\r\n");
	for(BUINT8 i = 0;;i++)
	{
		if((i==1) || (i==0)) continue;
		testI2CConfigSet(&testI2CConfig, 100000, i, 1); //! speed:100k, slave addr bits:i, register address length: 2 bytes
		rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
		if(rtnVal == BOAT_SUCCESS)
		{
			BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0036_boatI2COpen_Failed_port1_speed100k_SlaveAddrBits_notEQ0_notEQ1_RegAddr2Bytes failed[%x] \r\n",i);
			/////boatAssert_int_eq(rtnVal,-1);
			break;
		}
		
		if(i==0xff) break;
		/////fibo_taskSleep(100);
	}
	/////boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,-1);
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0036_boatI2COpen_Failed_port1_speed100k_SlaveAddrBits_notEQ0_notEQ1_RegAddr2Bytes finished -- Successful\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0037_boatI2COpen_Failed_port1_speed100k_SlaveAddrBits_SlaveAddrBits7_RegAddr_notEQ0_notEQ1_notEQ2(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0037_boatI2COpen_Failed_port1_speed100k_SlaveAddrBits_SlaveAddrBits7_RegAddr_notEQ0_notEQ1_notEQ2\r\n");
	for(BUINT8 i = 0;;i++)
	{
		if((i==1) || (i==0) || (i==2)) continue;
		testI2CConfigSet(&testI2CConfig, 100000, 0, i); //! speed:100k, slave addr bits:0, register address length: i
		rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
		if(rtnVal == BOAT_SUCCESS)
		{
			BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0037_boatI2COpen_Failed_port1_speed100k_SlaveAddrBits_SlaveAddrBits7_RegAddr_notEQ0_notEQ1_notEQ2 failed[%x] \r\n",i);
			break;/////return NULL;
		}
		/////boatAssert_int_eq(rtnVal,0);
		if(i==0xff) break;
		/////fibo_taskSleep(100);
	}
	/////boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,-1);
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0037_boatI2COpen_Failed_port1_speed100k_SlaveAddrBits_SlaveAddrBits7_RegAddr_notEQ0_notEQ1_notEQ2 finished -- Successful\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0047_boatI2CClose_Successful(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0047_boatI2CClose_Successful\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	rtnVal = boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,0);
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0047_boatI2CClose_Successful finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0048_boatI2CClose_Failed_I2CRefNULL(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0048_boatI2CClose_Failed_I2CRefNULL\r\n");
	
	rtnVal = boatI2cClose(0);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,-1);
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0048_boatI2CClose_Failed_I2CRefNULL finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0049_boatI2CClose_Failed_I2CIDZero(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0049_boatI2CClose_Failed_I2CIDZero\r\n");
	boatI2CInitI2CIDZero(&testI2CID);
	/////testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	/////rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	rtnVal = boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,-1);
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0049_boatI2CClose_Failed_I2CIDZero finished\r\n");
	
	return NULL;
}

#define testSlaveAddr 0x48
void *test_BoAT_DAL_FibocomL610_01I2C_test_0050_boatI2CMasterWrite_Successful(void)
{
	BOAT_RESULT rtnVal = 0;
	BUINT8  td[8]; 

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0050_boatI2CMasterWrite_Successful\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	if(rtnVal == BOAT_SUCCESS)
	{
		rtnVal = boatI2cMasterWrite(&testI2CID,testSlaveAddr,0,td,sizeof(td));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
		boatAssert_int_eq(rtnVal,0);
	}
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0050_boatI2CMasterWrite_Successful finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0051_boatI2CMasterWrite_Successful_DataLenZero(void)
{
	BOAT_RESULT rtnVal = 0;
	BUINT8  td[8]; 

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0051_boatI2CMasterWrite_Successful_DataLenZero\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	if(rtnVal == BOAT_SUCCESS)
	{
		rtnVal = boatI2cMasterWrite(&testI2CID,testSlaveAddr,0,td,0);//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
		boatAssert_int_eq(rtnVal,0);
	}
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0051_boatI2CMasterWrite_Successful_DataLenZero finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0052_boatI2CMasterWrite_Failed_I2CRefNULL(void)
{
	BOAT_RESULT rtnVal = 0;
	BUINT8  td[8]; 

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0052_boatI2CMasterWrite_Failed_I2CRefNULL\r\n");

	rtnVal = boatI2cMasterWrite(0,testSlaveAddr,0,td,sizeof(td));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
	boatAssert_int_eq(rtnVal,-1);

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0052_boatI2CMasterWrite_Failed_I2CRefNULL finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0053_boatI2CMasterWrite_Failed_SlaveAddrInvalid(void)//! slave address > 0x7f is invalid
{
	BOAT_RESULT rtnVal = 0;
	BOAT_RESULT rtnValOpen;
	BUINT8  td[8]; 

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0053_boatI2CMasterWrite_Failed_SlaveAddrInvalid\r\n");

	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:i, register address length: 2 bytes
	rtnValOpen = boatI2cOpen(&testI2CID, 1, testI2CConfig); //! I2C port number 1, no peripheral i2c device connected
	if(rtnValOpen == BOAT_SUCCESS)
	{
		for(BUINT16 i = 0;;i++)
		{
			if((i < 0x80)) continue;
			rtnVal = boatI2cMasterWrite(&testI2CID,i,0,td,sizeof(td));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
			if(rtnVal == BOAT_SUCCESS)
			{
				break;
			}
			if(i==0xffff) break;
		}
		
		
	}
	boatAssert_int_eq(rtnVal,-1);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0053_boatI2CMasterWrite_Failed_SlaveAddrInvalid finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0054_boatI2CMasterWrite_Failed_RegAddrInvalid(void)//! slave address > 0x7f is invalid
{
	BOAT_RESULT rtnVal = 0;
	BOAT_RESULT rtnValOpen;
	BUINT8  td[8]; 

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0054_boatI2CMasterWrite_Failed_RegAddrInvalid\r\n");

	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:i, register address length: 2 bytes
	rtnValOpen = boatI2cOpen(&testI2CID, 1, testI2CConfig); //! I2C port number 1, no peripheral i2c device connected
	if(rtnValOpen == BOAT_SUCCESS)
	{
		for(BUINT32 i = 0;;i++)
		{
			if((i < 0x10000)) continue;
			rtnVal = boatI2cMasterWrite(&testI2CID,testSlaveAddr,i,td,sizeof(td));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
			if(rtnVal == BOAT_SUCCESS)
			{
				break;
			}
			if(i==0xffffffff) break;
		}
		
		
	}
	boatAssert_int_eq(rtnVal,-1);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0054_boatI2CMasterWrite_Failed_RegAddrInvalid finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0055_boatI2CMasterWrite_Failed_DataNULL(void)//! slave address > 0x7f is invalid
{
	BOAT_RESULT rtnVal = 0;
	BOAT_RESULT rtnValOpen;


	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0055_boatI2CMasterWrite_Failed_DataNULL\r\n");

	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:i, register address length: 2 bytes
	rtnValOpen = boatI2cOpen(&testI2CID, 1, testI2CConfig); //! I2C port number 1, no peripheral i2c device connected
	if(rtnValOpen == BOAT_SUCCESS)
	{
		rtnVal = boatI2cMasterWrite(&testI2CID,testSlaveAddr,0,NULL,8);//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
	}
	boatAssert_int_eq(rtnVal,-1);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0055_boatI2CMasterWrite_Failed_DataNULL finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0056_boatI2CMasterWrite_Failed_DataLenInvalid(void)//! slave address > 0x7f is invalid
{
	BOAT_RESULT rtnVal = 0;
	BOAT_RESULT rtnValOpen;
	BUINT8  td[8]; 


	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0056_boatI2CMasterWrite_Failed_DataLenInvalid\r\n");

	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:i, register address length: 2 bytes
	rtnValOpen = boatI2cOpen(&testI2CID, 1, testI2CConfig); //! I2C port number 1, no peripheral i2c device connected
	if(rtnValOpen == BOAT_SUCCESS)
	{
		for(BUINT16 i = 0;;i++)
		{
			if((i < 9)) continue;
			rtnVal = boatI2cMasterWrite(&testI2CID,testSlaveAddr,0,td,i);//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
			if(rtnVal == BOAT_SUCCESS)
			{
				break;
			}
			if(i==0xffff) break;
		}
		
		
	}
	boatAssert_int_eq(rtnVal,-1);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0056_boatI2CMasterWrite_Failed_DataLenInvalid finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0057_boatI2CMasterWrite_Failed_FibocomWriteFailed(void)//! slave address > 0x7f is invalid
{
	BOAT_RESULT rtnVal = 0;
	BOAT_RESULT rtnValOpen;
	BUINT8  td[8]; 


	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0057_boatI2CMasterWrite_Failed_FibocomWriteFailed\r\n");

	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:i, register address length: 2 bytes
	rtnValOpen = boatI2cOpen(&testI2CID, 1, testI2CConfig); //! I2C port number 1, no peripheral i2c device connected
	if(rtnValOpen == BOAT_SUCCESS)
	{
		//! do test without peripheral i2c device
		rtnVal = boatI2cMasterWrite(&testI2CID,testSlaveAddr,0,td,sizeof(td));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
	}
	boatAssert_int_eq(rtnVal,-1);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0057_boatI2CMasterWrite_Failed_FibocomWriteFailed finished\r\n");
	
	return NULL;
}


void *test_BoAT_DAL_FibocomL610_01I2C_test_0058_boatI2CMasterRead_Successful(void)
{
	BOAT_RESULT rtnVal = 0;
	BUINT8  rd[8]; 

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0058_boatI2CMasterRead_Successful\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	if(rtnVal == BOAT_SUCCESS)
	{
		rtnVal = boatI2cMasterRead(&testI2CID,testSlaveAddr,0,rd,sizeof(rd));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
		boatAssert_int_eq(rtnVal,0);
	}
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0058_boatI2CMasterRead_Successful finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0059_boatI2CMasterRead_Successful_DataLenZero(void)
{
	BOAT_RESULT rtnVal = 0;
	BUINT8  rd[8]; 

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0059_boatI2CMasterRead_Successful_DataLenZero\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	if(rtnVal == BOAT_SUCCESS)
	{
		rtnVal = boatI2cMasterRead(&testI2CID,testSlaveAddr,0,rd,0);//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
		boatAssert_int_eq(rtnVal,0);
	}
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0059_boatI2CMasterRead_Successful_DataLenZero finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0060_boatI2CMasterRead_Failed_I2CRefNULL(void)
{
	BOAT_RESULT rtnVal = 0;
	BUINT8  rd[8]; 

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0060_boatI2CMasterRead_Failed_I2CRefNULL\r\n");

	rtnVal = boatI2cMasterRead(0,testSlaveAddr,0,rd,sizeof(rd));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
	boatAssert_int_eq(rtnVal,-1);

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0060_boatI2CMasterRead_Failed_I2CRefNULL finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0061_boatI2CMasterRead_Failed_SlaveAddrInvalid(void)//! slave address > 0x7f is invalid
{
	BOAT_RESULT rtnVal = 0;
	BOAT_RESULT rtnValOpen;
	BUINT8  rd[8]; 

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0061_boatI2CMasterRead_Failed_SlaveAddrInvalid\r\n");

	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:i, register address length: 2 bytes
	rtnValOpen = boatI2cOpen(&testI2CID, 1, testI2CConfig); //! I2C port number 1, no peripheral i2c device connected
	if(rtnValOpen == BOAT_SUCCESS)
	{
		for(BUINT16 i = 0;;i++)
		{
			if((i < 0x80)) continue;
			rtnVal = boatI2cMasterRead(&testI2CID,i,0,rd,sizeof(rd));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
			if(rtnVal == BOAT_SUCCESS)
			{
				break;
			}
			if(i==0xffff) break;
		}
		
		
	}
	boatAssert_int_eq(rtnVal,-1);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0061_boatI2CMasterRead_Failed_SlaveAddrInvalid finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0062_boatI2CMasterRead_Failed_RegAddrInvalid(void)//! slave address > 0x7f is invalid
{
	BOAT_RESULT rtnVal = 0;
	BOAT_RESULT rtnValOpen;
	BUINT8  rd[8]; 

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0062_boatI2CMasterRead_Failed_RegAddrInvalid\r\n");

	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:i, register address length: 2 bytes
	rtnValOpen = boatI2cOpen(&testI2CID, 1, testI2CConfig); //! I2C port number 1, no peripheral i2c device connected
	if(rtnValOpen == BOAT_SUCCESS)
	{
		for(BUINT32 i = 0;;i++)
		{
			if((i < 0x10000)) continue;
			rtnVal = boatI2cMasterRead(&testI2CID,testSlaveAddr,i,rd,sizeof(rd));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
			if(rtnVal == BOAT_SUCCESS)
			{
				break;
			}
			if(i==0xffffffff) break;
		}
		
		
	}
	boatAssert_int_eq(rtnVal,-1);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0062_boatI2CMasterRead_Failed_RegAddrInvalid finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0063_boatI2CMasterRead_Failed_DataNULL(void)//! slave address > 0x7f is invalid
{
	BOAT_RESULT rtnVal = 0;
	BOAT_RESULT rtnValOpen;


	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0063_boatI2CMasterRead_Failed_DataNULL\r\n");

	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:i, register address length: 2 bytes
	rtnValOpen = boatI2cOpen(&testI2CID, 1, testI2CConfig); //! I2C port number 1, no peripheral i2c device connected
	if(rtnValOpen == BOAT_SUCCESS)
	{
		rtnVal = boatI2cMasterRead(&testI2CID,testSlaveAddr,0,NULL,8);//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
	}
	boatAssert_int_eq(rtnVal,-1);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0063_boatI2CMasterRead_Failed_DataNULL finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0064_boatI2CMasterRead_Failed_DataLenInvalid(void)//! slave address > 0x7f is invalid
{
	BOAT_RESULT rtnVal = 0;
	BOAT_RESULT rtnValOpen;
	BUINT8  rd[8]; 


	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0064_boatI2CMasterRead_Failed_DataLenInvalid\r\n");

	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:i, register address length: 2 bytes
	rtnValOpen = boatI2cOpen(&testI2CID, 1, testI2CConfig); //! I2C port number 1, no peripheral i2c device connected
	if(rtnValOpen == BOAT_SUCCESS)
	{
		for(BUINT16 i = 0;;i++)
		{
			if((i < 9)) continue;
			rtnVal = boatI2cMasterRead(&testI2CID,testSlaveAddr,0,rd,i);//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
			if(rtnVal == BOAT_SUCCESS)
			{
				break;
			}
			if(i==0xffff) break;
		}
		
		
	}
	boatAssert_int_eq(rtnVal,-1);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0064_boatI2CMasterRead_Failed_DataLenInvalid finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0065_boatI2CMasterRead_Failed_FibocomReadFailed(void)//! slave address > 0x7f is invalid
{
	BOAT_RESULT rtnVal = 0;
	BOAT_RESULT rtnValOpen;
	BUINT8  rd[8]; 


	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0065_boatI2CMasterRead_Failed_FibocomReadFailed\r\n");

	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:i, register address length: 2 bytes
	rtnValOpen = boatI2cOpen(&testI2CID, 1, testI2CConfig); //! I2C port number 1, no peripheral i2c device connected
	if(rtnValOpen == BOAT_SUCCESS)
	{
		//! do test without peripheral i2c device
		rtnVal = boatI2cMasterRead(&testI2CID,testSlaveAddr,0,rd,sizeof(rd));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
	}
	boatAssert_int_eq(rtnVal,-1);
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0065_boatI2CMasterRead_Failed_FibocomReadFailed finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0066_boatI2CDataWR_NXP(void)
{
	BOAT_RESULT rtnVal = 0;
	/////BUINT8  td[8]; 
	BUINT8  rd[8]; 

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0058_boatI2CMasterRead_Successful\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	if(rtnVal == BOAT_SUCCESS)
	{
		// 得到正确的写入数据后再开放write调用
		//!rtnVal = boatI2cMasterWrite(&testI2CID,testSlaveAddr,0,td,sizeof(td));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
		boatAssert_int_eq(rtnVal,0);

		rtnVal = boatI2cMasterRead(&testI2CID,testSlaveAddr,0,rd,sizeof(rd));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
		boatAssert_int_eq(rtnVal,0);
	}
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0058_boatI2CMasterRead_Successful finished\r\n");
	
	return NULL;
}


void *test_BoAT_DAL_FibocomL610_01I2C_test_0067_boatI2CFicbocmReopen_ErrorRtnVal(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0067_boatI2CFicbocmReopen_ErrorRtnVal\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	boatAssert_int_eq(rtnVal,0);

	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	boatAssert_int_eq(rtnVal,-1);

	rtnVal = boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,0);
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0067_boatI2CFicbocmReopen_ErrorRtnVal finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0068_boatI2CFicbocmReclose_ErrorRtnVal(void)
{
	BOAT_RESULT rtnVal = 0;

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0068_boatI2CFicbocmReclose_ErrorRtnVal\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 2, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	boatAssert_int_eq(rtnVal,0);

	rtnVal = boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,0);

	rtnVal = boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	boatAssert_int_eq(rtnVal,-1);

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0068_boatI2CFicbocmReclose_ErrorRtnVal finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0069_boatI2CFicbocmWrite_lenInvalid_ErrorRtnVal(void)
{
	BOAT_RESULT rtnVal = 0;
	/////BUINT8  td[8]; 

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0069_boatI2CFicbocmWrite_lenInvalid_ErrorRtnVal\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	if(rtnVal == BOAT_SUCCESS)
	{
		// 得到正确的写入数据后再开放write调用
		//!rtnVal = boatI2cMasterWrite(&testI2CID,testSlaveAddr,0,td,sizeof(td));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
		boatAssert_int_eq(rtnVal,0);
	}
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0069_boatI2CFicbocmWrite_lenInvalid_ErrorRtnVal finished\r\n");
	
	return NULL;
}

void *test_BoAT_DAL_FibocomL610_01I2C_test_0070_boatI2CFicbocmRead_lenInvalid_ErrorRtnVal(void)
{
	BOAT_RESULT rtnVal = 0;
	BUINT8  rd[8]; 

	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0070_boatI2CFicbocmRead_lenInvalid_ErrorRtnVal\r\n");
	
	testI2CConfigSet(&testI2CConfig, 100000, 0, 1); //! speed:100k, slave addr bits:7, register address length: 2 bytes
	rtnVal = boatI2cOpen(&testI2CID, 1, testI2CConfig);	//! I2C port number 1, no peripheral i2c device connected
	if(rtnVal == BOAT_SUCCESS)
	{
		BoatPrintf(0,"[boat][i2c] boatI2cOpen in lenInvalid_ErrorRtnVal succ \r\n");
		rtnVal = boatI2cMasterRead(&testI2CID,testSlaveAddr,0,rd,sizeof(rd));//!(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen)
		boatAssert_int_eq(rtnVal,0);
	}
	fibo_taskSleep(100);
	BoatPrintf(0,"[boat][i2c] boatI2cOpen finished \r\n");
	boatI2cClose(&testI2CID);///// must destroy the timer once you stop using it.
	BoatPrintf(0,"[boat][i2c] Testing test_BoAT_DAL_FibocomL610_01I2C_test_0070_boatI2CFicbocmRead_lenInvalid_ErrorRtnVal finished\r\n");
	
	return NULL;
}


void testI2CEntry(void)
{
#if 0
	test_BoAT_DAL_FibocomL610_01I2C_test_0010_boatI2COpenPort1_Successful_speed100k_SlaveAddrBits7_RegAddr2Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0011_boatI2COpenPort1_Successful_speed400k_SlaveAddrBits7_RegAddr2Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0012_boatI2COpenPort2_Successful_speed100k_SlaveAddrBits7_RegAddr2Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0013_boatI2COpenPort2_Successful_speed400k_SlaveAddrBits7_RegAddr2Bytes();
	fibo_taskSleep(1000);
	
	test_BoAT_DAL_FibocomL610_01I2C_test_0014_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits7_RegAddr1Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0015_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits7_RegAddr4Bytes();
	fibo_taskSleep(1000);

	test_BoAT_DAL_FibocomL610_01I2C_test_0016_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits10_RegAddr1Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0017_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits10_RegAddr2Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0018_boatI2COpen_Fail_Port1_speed100k_SlaveAddrBits10_RegAddr4Bytes();
	fibo_taskSleep(1000);
	
	test_BoAT_DAL_FibocomL610_01I2C_test_0019_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits7_RegAddr1Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0020_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits7_RegAddr4Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0021_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits10_RegAddr1Bytes();
	test_BoAT_DAL_FibocomL610_01I2C_test_0022_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits10_RegAddr2Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0023_boatI2COpen_Fail_Port1_speed400k_SlaveAddrBits10_RegAddr4Bytes();
	fibo_taskSleep(1000);
	
	test_BoAT_DAL_FibocomL610_01I2C_test_0024_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits7_RegAddr1Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0025_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits7_RegAddr4Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0026_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits10_RegAddr1Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0027_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits10_RegAddr2Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0028_boatI2COpen_Fail_Port2_speed100k_SlaveAddrBits10_RegAddr4Bytes();
	fibo_taskSleep(1000);
	
	test_BoAT_DAL_FibocomL610_01I2C_test_0029_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits7_RegAddr1Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0030_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits7_RegAddr4Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0031_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits10_RegAddr1Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0032_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits10_RegAddr2Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0033_boatI2COpen_Fail_Port2_speed400k_SlaveAddrBits10_RegAddr4Bytes();
	fibo_taskSleep(1000);

	test_BoAT_DAL_FibocomL610_01I2C_test_0034_boatI2COpen_Failed_portNumber_notEQ1_notEQ2_speed100k_SlaveAddrBits7_RegAddr2Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0035_boatI2COpen_Failed_port1_speed_notEQ100k_notEQ400k_SlaveAddrBits_RegAddr2Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0036_boatI2COpen_Failed_port1_speed100k_SlaveAddrBits_notEQ0_notEQ1_RegAddr2Bytes();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0037_boatI2COpen_Failed_port1_speed100k_SlaveAddrBits_SlaveAddrBits7_RegAddr_notEQ0_notEQ1_notEQ2();
	fibo_taskSleep(1000);

	test_BoAT_DAL_FibocomL610_01I2C_test_0047_boatI2CClose_Successful();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0048_boatI2CClose_Failed_I2CRefNULL();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0049_boatI2CClose_Failed_I2CIDZero();
	fibo_taskSleep(1000);

	test_BoAT_DAL_FibocomL610_01I2C_test_0050_boatI2CMasterWrite_Successful();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0051_boatI2CMasterWrite_Successful_DataLenZero();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0052_boatI2CMasterWrite_Failed_I2CRefNULL();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0053_boatI2CMasterWrite_Failed_SlaveAddrInvalid();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0054_boatI2CMasterWrite_Failed_RegAddrInvalid();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0055_boatI2CMasterWrite_Failed_DataNULL();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0056_boatI2CMasterWrite_Failed_DataLenInvalid();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0057_boatI2CMasterWrite_Failed_FibocomWriteFailed();

	test_BoAT_DAL_FibocomL610_01I2C_test_0058_boatI2CMasterRead_Successful();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0059_boatI2CMasterRead_Successful_DataLenZero();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0060_boatI2CMasterRead_Failed_I2CRefNULL();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0061_boatI2CMasterRead_Failed_SlaveAddrInvalid();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0062_boatI2CMasterRead_Failed_RegAddrInvalid();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0063_boatI2CMasterRead_Failed_DataNULL();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0064_boatI2CMasterRead_Failed_DataLenInvalid();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0065_boatI2CMasterRead_Failed_FibocomReadFailed();		
	fibo_taskSleep(1000);


	test_BoAT_DAL_FibocomL610_01I2C_test_0066_boatI2CDataWR_NXP();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0067_boatI2CFicbocmReopen_ErrorRtnVal();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0068_boatI2CFicbocmReclose_ErrorRtnVal();
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0069_boatI2CFicbocmWrite_lenInvalid_ErrorRtnVal();
#endif
	fibo_taskSleep(1000);
	test_BoAT_DAL_FibocomL610_01I2C_test_0070_boatI2CFicbocmRead_lenInvalid_ErrorRtnVal();
	fibo_taskSleep(1000);
}
