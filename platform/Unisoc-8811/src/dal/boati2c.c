
#include "boatdal.h"

#ifdef PLATFORM_DAL_I2C

#include "stdio.h"

#include "drv_i2c.h"
#include "drv_names.h"
#include "osi_api.h"

#include "boattypes.h"
#include "boatlog.h"

/*
* Mark the use status of the virtual AT channel. 
* It is BOAT_TRUE when it is occupied, otherwise it is BOAT_FALSE
*/
static BBOOL g_I2C_channel_is_used = BOAT_FALSE;

/* i2cPortNum can be 1\2\3*/
BOAT_RESULT boatI2cOpen(boatI2c *i2cRef,BUINT8 i2cPortNum,boatI2cConfig i2cConfig)
{ 
    if(i2cRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    uint32_t i2cNameArray[3] = {DRV_NAME_I2C1,DRV_NAME_I2C2,DRV_NAME_I2C3};

    if(g_I2C_channel_is_used == BOAT_FALSE)
    {
        drvI2cBps_t speed;
        switch (i2cConfig.i2cSpeed)
        {
        case 100000:
            speed = DRV_I2C_BPS_100K;
            break;
        case 400000:
            speed = DRV_I2C_BPS_400K;
            break;
        default:
            speed = DRV_I2C_BPS_3P5M;
            break;
        }

        drvI2cMaster_t *i2cMaster = drvI2cMasterAcquire(i2cNameArray[i2cPortNum-1],speed);
        if(i2cMaster == NULL)
        {
            BoatLog(BOAT_LOG_CRITICAL,"I2c acquire master failed!");
            return BOAT_ERROR;
        }
       
        i2cRef->i2cInstance = i2cMaster;

        g_I2C_channel_is_used = BOAT_TRUE;
        return BOAT_SUCCESS;
    }
    else
    {
        /* I2C channel is busy */
        return BOAT_ERROR_DAL_I2C_BUSY;
    }
}


BOAT_RESULT boatI2cClose(boatI2c *i2cRef)
{
    if(i2cRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(i2cRef->i2cInstance == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad I2c Instance!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(g_I2C_channel_is_used == BOAT_TRUE)
    {
        drvI2cMasterRelease(i2cRef->i2cInstance);

        g_I2C_channel_is_used = BOAT_FALSE;
        return BOAT_SUCCESS;

    }
    else
    {
        return BOAT_ERROR_DAL_I2C_CLOSED;
    }

}


BOAT_RESULT boatI2cMasterWrite(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 dataLen)
{
    BBOOL use_malloc_flag =BOAT_FALSE;

    if((i2cRef == NULL) || \
        (data == NULL) || \
        (dataLen == 0))
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(g_I2C_channel_is_used == BOAT_TRUE)
    {
        int8_t *write_Data = NULL;

        int8_t tmp_Array[512 + 2] = {0};

        if(dataLen < 512)
        {
            
            write_Data = &(tmp_Array[0]);
        }
        else
        {
            write_Data = malloc(dataLen+2);
            if(write_Data == NULL)
            {
                return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
            }

            memset(write_Data,0U,dataLen+2);

            use_malloc_flag = BOAT_TRUE;
        }


        // BUINT16 ML307_reg_addr = (BUINT16)regAddr;
        // uint8_t a = (ML307_reg_addr >> 8) & 0xff;
        // uint8_t b = ML307_reg_addr & 0xff;
        
        // memcpy(write_Data,&a,1);
        // memcpy(write_Data+1,&b,1);
        // memcpy(write_Data+1+1,data,dataLen);

        memcpy(write_Data,data,dataLen);

        drvI2cSlave_t slave;
        slave.addr_device = slaveAddr;
        slave.reg_16bit = true;
        slave.addr_data = ((BUINT16)regAddr >> 8) & 0xFF;
        slave.addr_data_low = (BUINT16)regAddr & 0xFF;

        //int32_t len = cm_i2c_write(i2cRef->i2cId,slaveAddr,write_Data,dataLen+2);
        bool ret = drvI2cWrite(i2cRef->i2cInstance,&slave,write_Data,dataLen);

        osiThreadSleep(10);    //wait for write successful

        if(use_malloc_flag == BOAT_TRUE)
        {
            if(write_Data != NULL)
            {
                free(write_Data);
            }
        }

        // if(len != (dataLen+2))
        // {
        //     BoatLog(BOAT_LOG_CRITICAL,"I2C write failed!");
        //     return BOAT_ERROR;
        // }

        if(ret == false)
        {
            BoatLog(BOAT_LOG_CRITICAL,"I2C write failed!");
            return BOAT_ERROR;            
        }

        return BOAT_SUCCESS;

    }
    else
    {
        return BOAT_ERROR_DAL_I2C_CLOSED;
    }

}


BOAT_RESULT boatI2cMasterRead(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 dataLen)
{

    if((i2cRef == NULL) || \
        (data == NULL))
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(g_I2C_channel_is_used == BOAT_TRUE)
    {
        // int8_t tmp[2] = {0};

        // BUINT16 ML307_reg_addr = (BUINT16)regAddr;
        // tmp[0] = (ML307_reg_addr >> 8) & 0xff;
        // tmp[1] = ML307_reg_addr & 0xff;

        // int32_t ret = cm_i2c_write(i2cRef->i2cId,slaveAddr,tmp,2);
        // if(ret < 0)
	    // {
		//     BoatLog(BOAT_LOG_CRITICAL,"i2c read addr err(w):%08x\r\n", ret);
		//     return BOAT_ERROR;
	    // }
        drvI2cSlave_t slave;
        slave.addr_device = slaveAddr;
        slave.reg_16bit = true;
        slave.addr_data = ((BUINT16)regAddr >> 8) & 0xFF;
        slave.addr_data_low = (BUINT16)regAddr & 0xFF;


        //ret = cm_i2c_read(i2cRef->i2cId,slaveAddr,data,(uint32_t)dataLen);
        bool ret = drvI2cRead(i2cRef->i2cInstance,&slave,data,(uint32_t)dataLen);
        if(ret == false)
	    {
		    BoatLog(BOAT_LOG_CRITICAL,"i2c read data err(w)\r\n");
		    return BOAT_ERROR;
	    }

        return BOAT_SUCCESS;

    }
    else
    {
        return BOAT_ERROR_DAL_I2C_CLOSED;
    }

}
				
//! BOAT_RESULT boatI2CMasterSend(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen);
//! BOAT_RESULT boatI2CSlaveReceie(boatI2c *i2cRef, BUINT16 slaveAddr, BUINT32 regAddr, BUINT8 *data, BUINT16 datalen);
#endif