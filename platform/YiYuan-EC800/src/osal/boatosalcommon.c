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
#include <stdint.h>
#include "boattypes.h"
#include "boatlog.h"
#include "boaterrcode.h"

#include <time.h>
//#include "arc4random.h"
#include <stdlib.h>
#include "ql_rtos.h"
#include "ql_platform.h"
#include <stdio.h>

ql_mutex_t mutexstatistics;

#ifdef __CYGWIN__
void random_buffer(uint8_t *buf, size_t len) ;
#else
void __attribute__((weak)) random_buffer(uint8_t *buf, size_t len) ;
#endif
/**
****************************************************************************************
* @brief:
* @return
****************************************************************************************
*/

long int BoatGetTimes()
{
	time_t tm;
    tm = time(NULL);
    BoatLog(BOAT_LOG_NORMAL, " Currrent tm = time(NULL) : %ld", tm);
    return tm;
}

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
    static BUINT32 seed = 0;
	seed += ql_pseudo_random_number();
	seed += ql_pseudo_random_number();
	seed += ql_pseudo_random_number();

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
long int maxMallocSize = 0, curMallocSzie = 0, maxMallocTime = 0, curMallocTime = 0;
#define maxInfo 5000
static struct infoMalloc{
	void *p;
	size_t len;
}mallocInfo[maxInfo];
void BoATMallocStatistics(char *p)
{	
	printf("******** * Malloc statistics %s * ********\r\n", p);
	printf("* Maxtime %ld Maxsize %ld Cursize %ld\r\n",maxMallocTime, maxMallocSize, curMallocSzie);
	printf("***************************************\r\n\r\n");
}


void addInfo(void *p, size_t len)
{
	if(NULL == mutexstatistics)
	{
		ql_rtos_mutex_create(&mutexstatistics);
		if(NULL == mutexstatistics)
		{
			return;
		}
	}
	ql_rtos_mutex_lock(mutexstatistics, 0xFFFFFFFF);
	curMallocSzie += (long)len; 
	if(maxMallocSize < curMallocSzie)
		maxMallocSize = curMallocSzie;
	curMallocTime ++;
	if(maxMallocTime < curMallocTime)
		maxMallocTime = curMallocTime;
	
	for(int i = 0; i < maxInfo; i++)
	{
		if(mallocInfo[i].p == 0)
		{
			mallocInfo[i].p = p;
			mallocInfo[i].len = len;
			//printf("addinfo [%d] %p %ld\n", i, mallocInfo[i].p, (long)mallocInfo[i].len);
			break;
		}
	}
	ql_rtos_mutex_unlock(mutexstatistics);
}

void delInfo(void *p)
{
	if(NULL == mutexstatistics)
	{
		ql_rtos_mutex_create(&mutexstatistics);
		if(NULL == mutexstatistics)
		{
			return;
		}
	}
	ql_rtos_mutex_lock(mutexstatistics, 0xFFFFFFFF);
	for(int i = 0; i < maxInfo; i++)
	{
		
		if((mallocInfo[i].p ==p) && (p != NULL))
		{
			//printf("delinfo %p %ld\n", mallocInfo[i].p, (long)mallocInfo[i].len);
			mallocInfo[i].p = NULL;
			curMallocSzie -= (long)mallocInfo[i].len;
			mallocInfo[i].len = 0;
			curMallocTime --;
			break;
		}
	}
	ql_rtos_mutex_unlock(mutexstatistics);
}

void *BoatMalloc(size_t size)
{

	void *p = malloc(size);
	addInfo(p, size);
#if 0
    void *returnAddress;
	void* returnAddress1 = __builtin_return_address(0);
    __asm__ volatile (
        "MOV %0, lr\n"  // 将返回地址加载到寄存器lr中
        : "=r" (returnAddress)  // 输出操作数
        :
        :
    );
	printf("debug add:  %p:%p\n",returnAddress, returnAddress1);

	BoATMallocStatistics("Malloc");
#endif
	return p;
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
	//curMallocSize -= (long)size;
	//printf("debug del:  %p\n",mem_ptr);
	if(mem_ptr == NULL)
	{
		return;
	}
	delInfo(mem_ptr);
	free(mem_ptr);
#if 0
    void *returnAddress;
	void* returnAddress1 = __builtin_return_address(0);
    __asm__ volatile (
        "MOV %0, lr\n"  // 将返回地址加载到寄存器lr中
        : "=r" (returnAddress)  // 输出操作数
        :
        :
    );
	printf("debug del:  %p:%p\n",returnAddress, returnAddress1);
	BoATMallocStatistics("Free");
#endif
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
    ql_rtos_task_sleep_s(second);
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
    ql_rtos_task_sleep_ms(ms);
}