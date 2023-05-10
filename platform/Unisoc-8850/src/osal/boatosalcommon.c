#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
//#include "rand.h"


#include "osi_api.h"

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
	osiThreadSleep(1000 * second);
}

/**
****************************************************************************************
* @brief:
* @return
****************************************************************************************
*/
BUINT64 BoatGetTimes(void)
{
	return osiEpochTime();
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
	osiThreadSleep(ms);
}

BUINT32 random32(void)
{
	static BUINT32 seed = 0;
	if (seed == 0)
	{
		seed = time(NULL);
	}
	// Linear congruential generator from Numerical Recipes
	// https://en.wikipedia.org/wiki/Linear_congruential_generator
	seed = 1664525 * seed + 1013904223;

	return seed;
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


