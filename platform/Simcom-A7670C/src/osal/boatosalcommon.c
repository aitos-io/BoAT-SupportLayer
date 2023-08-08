/**
****************************************************************************************
* @FilePath: boat_osal_common.c
* @Author: aitos
* @Date: 2023-02-15 17:15:16
* @LastEditors:
* @LastEditTime: 2023-02-15 17:15:16
* @Descripttion:
****************************************************************************************
*/
#include "boattypes.h"
#include "boatlog.h"
#include "boaterrcode.h"
#include "rand.h"

#include "simcom_tcpip_old.h"
#include "simcom_ssl.h"
#include "simcom_os.h"
#include "simcom_file.h"


/**
****************************************************************************************
* @brief:
*  This function generate 32 bit random
* @return
*  This function random generate by platform
****************************************************************************************
*/
uint32_t random32(void)
{

    static uint32_t seed = 0;
	if (seed == 0)
	{
		seed = sAPI_Time(NULL);
	}
	// Linear congruential generator from Numerical Recipes
	// https://en.wikipedia.org/wiki/Linear_congruential_generator
	seed = 1664525 * seed + 1013904223;

	return seed;
}

/**
****************************************************************************************
* @brief:
*  This function generate random data with fixed length.
* @param[in] *output
*  output of random data
* @param[in] outputLen
*  length of random that wanted to generate
* @param[in] *rsvd
*  no used until now
* @return
*  This function returns BoAT_SUCCESS if successfully executed.
*  Otherwise it returns one of the error codes. Refer to header file boaterrcode.h
*  for details.
****************************************************************************************
*/
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
    return (malloc(size));
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
    free(mem_ptr);
}

/**
****************************************************************************************
* @brief:
* @param[in] second
* @return
****************************************************************************************
*/
void BoatSleep(BUINT32 second)
{
    sAPI_DelayUs(1000000 * second);
}
/**
****************************************************************************************
* @brief:
* @return
****************************************************************************************
*/
long int BoatGetTimes(void)
{
    long int ret =(long int)  sAPI_Time(NULL);
    BoatLog(BOAT_LOG_NORMAL, " BoatGetTimes = %ld", ret);
    return ret;
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
    sAPI_DelayUs(1000 * ms);
}