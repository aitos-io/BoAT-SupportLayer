#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
#include "rand.h"
#include "app_azure_rtos.h"
#include "stm32h5xx_hal.h"

extern RNG_HandleTypeDef hrng;
extern __IO uint32_t BUID[6];
#define MALLOC_APP_MEM_POOL_SIZE 1024 * 100
#if defined ( __ICCARM__ )
#pragma data_alignment=4
#endif
__ALIGN_BEGIN static UCHAR  malloc_byte_pool_buffer[MALLOC_APP_MEM_POOL_SIZE] __ALIGN_END;
static TX_BYTE_POOL malloc_app_byte_pool;

#define BOATMALLOCINITED 1
static char boatMallocInitFlag = 0;


/**
****************************************************************************************
* @brief:
*  This function malloc  size memory
* @param[in] size
* @return
*  return malloced memory, if failed return NULL.
****************************************************************************************
*/
void *BoatMalloc(size_t size)
{
	if(boatMallocInitFlag != BOATMALLOCINITED)
	{
		if (tx_byte_pool_create(&malloc_app_byte_pool, "Nx App memory pool", malloc_byte_pool_buffer, MALLOC_APP_MEM_POOL_SIZE) != TX_SUCCESS)
		{
			/* USER CODE BEGIN NX_Byte_Pool_Error */
			return NULL;//Error_Handler();
			/* USER CODE END NX_Byte_Pool_Error */
		}
		boatMallocInitFlag = BOATMALLOCINITED;
	}
	void *pointer = NULL;
	BOAT_RESULT ret;
	ret = tx_byte_allocate(&malloc_app_byte_pool, &pointer, size, TX_NO_WAIT);
	if (ret != TX_SUCCESS)
	{
		return NULL;//TX_POOL_ERROR;
	}
	return pointer;
}

/**
****************************************************************************************
* @brief:
*  This function free memory
* @param[in] *mem_ptr
* @return
*  This function has no return value
****************************************************************************************
*/
void BoatFree(void *mem_ptr)
{
	tx_byte_release(mem_ptr);
}

/**
****************************************************************************************
* @brief:
* @param[in] second
* @return
* @note: function tx_thread_sleep works only in a thread, otherwise it will return 0x13 immediately.
****************************************************************************************
*/
void BoatSleep(BUINT32 second)
{
	tx_thread_sleep(second * 100);
}

/**
****************************************************************************************
* @brief:
* @return
****************************************************************************************
*/
long int BoatGetTimes(void)
{
	return 0;//return time(NULL);
}

/**
****************************************************************************************
* @brief:
* @param[in] ms
* @return
****************************************************************************************
*/
void BoatSleepMs(BUINT32 ms)
{
	if (ms <= (0xffffffff - 10))
		if((ms % 10) > 5) ms += 10;
	tx_thread_sleep(ms / 10); // The sleep granularity is 10ms
}

BUINT32 random32(void)
{
	uint32_t seed = 0;
	if (HAL_RNG_GenerateRandomNumber(&hrng, &seed) != HAL_OK)
    {
		/* Random number generation error */
		return 0;//Error_Handler();
	}
	//test
	return(BUID[5]);
	return(seed);

}

BUINT32 batorandom32temp(void)
{
	uint32_t seed = 0;
	if (HAL_RNG_GenerateRandomNumber(&hrng, &seed) != HAL_OK)
    {
		/* Random number generation error */
		return 0;//Error_Handler();
	}
	return(seed);

}

BOAT_RESULT BoatRandom(BUINT8 *output, BUINT32 outputLen, void *rsvd)
{
	/* param check */
	if (output == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL, "parameter can't be NULL.");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}

	(void)rsvd;

	random_buffer(output, outputLen);

	return BOAT_SUCCESS;
}


