
#include "boatdal.h"

#ifdef PLATFORM_DAL_I2C

#include "stdio.h"
#include "cm_os.h"
#include "cm_i2c.h"
#include "cm_mem.h"

#include "boattypes.h"
#include "boatlog.h"

/*
* Mark the use status of the virtual AT channel. 
* It is BOAT_TRUE when it is occupied, otherwise it is BOAT_FALSE
*/
static BBOOL g_I2C_channel_is_used = BOAT_FALSE;

BOAT_RESULT boatI2cOpen(boatI2c *i2cRef,BUINT8 i2cPortNum,boatI2cConfig i2cConfig)
{ 
    if(i2cRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }

    if(g_I2C_channel_is_used == BOAT_FALSE)
    {
        cm_i2c_dev_e dev[3] = {CM_I2C_DEV_0,CM_I2C_DEV_1,CM_I2C_DEV_2};

        if(i2cPortNum > 2 )
        {
            BoatLog(BOAT_LOG_CRITICAL,"i2cPortNum is Wrong!");
            return BOAT_ERROR_DAL_INVALID_ARGUMENT;
        }

        cm_i2c_cfg_t config = {0};
        config.addr_type = i2cConfig.i2cSlaveAddrBits;
        config.mode = CM_I2C_MODE_MASTER;   //just support master mode
        config.clk = i2cConfig.i2cSpeed;
        int32_t  ret = cm_i2c_open(dev[i2cPortNum],&config);
        if(ret < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL,"I2C channel %d open failed!",i2cPortNum);
            return BOAT_ERROR;
        }

        i2cRef->i2cId = dev[i2cPortNum];

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

    if(g_I2C_channel_is_used == BOAT_TRUE)
    {
        int32_t ret = cm_i2c_close(i2cRef->i2cId);
        if(ret < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL,"I2C channel %d close failed!",i2cRef->i2cId);
            return BOAT_ERROR;
        }

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
            write_Data = cm_malloc(dataLen+2);
            if(write_Data == NULL)
            {
                return BOAT_ERROR_COMMON_OUT_OF_MEMORY;
            }

            memset(write_Data,0U,dataLen+2);

            use_malloc_flag = BOAT_TRUE;
        }


        BUINT16 ML307_reg_addr = (BUINT16)regAddr;
        uint8_t a = (ML307_reg_addr >> 8) & 0xff;
        uint8_t b = ML307_reg_addr & 0xff;
        
        memcpy(write_Data,&a,1);
        memcpy(write_Data+1,&b,1);
        memcpy(write_Data+1+1,data,dataLen);

        int32_t len = cm_i2c_write(i2cRef->i2cId,slaveAddr,write_Data,dataLen+2);
        
        osDelay(10);    //wait for write successful

        if(use_malloc_flag == BOAT_TRUE)
        {
            if(write_Data != NULL)
            {
                cm_free(write_Data);
            }
        }

        if(len != (dataLen+2))
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
        int8_t tmp[2] = {0};

        BUINT16 ML307_reg_addr = (BUINT16)regAddr;
        tmp[0] = (ML307_reg_addr >> 8) & 0xff;
        tmp[1] = ML307_reg_addr & 0xff;

        int32_t ret = cm_i2c_write(i2cRef->i2cId,slaveAddr,tmp,2);
        if(ret < 0)
	    {
		    BoatLog(BOAT_LOG_CRITICAL,"i2c read addr err(w):%08x\r\n", ret);
		    return BOAT_ERROR;
	    }

        ret = cm_i2c_read(i2cRef->i2cId,slaveAddr,data,(uint32_t)dataLen);
        if(ret < 0)
	    {
		    BoatLog(BOAT_LOG_CRITICAL,"i2c read data err(w):%08x\r\n", ret);
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