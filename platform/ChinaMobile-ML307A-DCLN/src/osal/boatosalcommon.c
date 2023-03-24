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

#include "cm_os.h"
#include "cm_ntp.h"
#include "cm_rtc.h"
#include "cm_mem.h"

/**
****************************************************************************************
* @brief:
* @return
****************************************************************************************
*/
static void __boat_cm_ntp_cb(cm_ntp_event_e event, void *event_param, void *cb_param)
{
    if (CM_NTP_EVENT_SYNC_OK == event)
    {
        BoatLog(BOAT_LOG_NORMAL,"[NTP][%s] get net time %s\n", (char *)cb_param, event_param); 
    }
    else if (CM_NTP_EVENT_SETTIME_FAIL == event)
    {
        BoatLog(BOAT_LOG_CRITICAL,"[NTP][%s] get net time %s OK, but set time fail\n", (char *)cb_param, event_param); 
    }
    else
    {
        BoatLog(BOAT_LOG_CRITICAL,"[NTP][%s] error\n", (char *)cb_param); 
    }
}


long int BoatGetTimes()
{

    uint16_t port = 123;
    uint32_t timeout = 6000;
    uint32_t dns_priority = 1;
    BBOOL set_rtc = 1;

    cm_ntp_set_cfg(CM_NTP_CFG_SERVER, "cn.ntp.org.cn"); 
    cm_ntp_set_cfg(CM_NTP_CFG_PORT, &port);
    cm_ntp_set_cfg(CM_NTP_CFG_TIMEOUT, &timeout);
    cm_ntp_set_cfg(CM_NTP_CFG_DNS, &dns_priority);
    cm_ntp_set_cfg(CM_NTP_CFG_SET_RTC, &set_rtc);
    cm_ntp_set_cfg(CM_NTP_CFG_CB, __boat_cm_ntp_cb);
    cm_ntp_set_cfg(CM_NTP_CFG_CB_PARAM, "SYNCHTIME");

    if (0 != cm_ntp_sync())
    {
        BoatLog(BOAT_LOG_CRITICAL,"[NTP]cm_ntp_sync() fail\n");
    }
    uint64_t  ret = cm_rtc_get_current_time();
    BoatLog(BOAT_LOG_NORMAL, " Currrent RTC time  = %ld", ret);
    return ret;
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
	if (seed == 0)
	{
		seed = BoatGetTimes();
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
    cm_heap_stats_t heap_status={0};
    
    int32_t  ret = cm_mem_get_heap_stats(&heap_status);
    if(ret == 0)
    {
        if(size < heap_status.free)
        {
            return cm_malloc(size);
        }
    }

    return NULL;
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
    cm_free(mem_ptr);
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
    osDelay(200 * second);
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
    osDelay(ms * 200/1000);
}