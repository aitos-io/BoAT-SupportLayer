#include "fibo_opencpu.h"
#include "boattypes.h"
#include "boatlog.h"
#include "boaterrcode.h"
#include "boatdal.h"

UINT32 (* dalBleServiceCallback)(UINT8 *buf,UINT32 len);

/*
* Mark the status of the BLE service. 
* It is BOAT_TRUE when it is  otherwise it is BOAT_FALSE
*/
static BBOOL g_bleService_is_initialized  = BOAT_FALSE;

static BUINT16 g_acl_handle = 0;

static char wifimgr_value[1024];
static char wifimgr_ccc_cfg[1024] = {0};
static char device_name[1024] = "L610_ble";

static uint8_t test_service[2] = {0x1811&0xff, 0x1811 >> 8};
//static uint8_t add_service[2] = {0x180d&0xff, 0x180d >> 8};

static gatt_chara_def_short_t wifi_mgr_chara = {{
	ATT_CHARA_PROP_READ | ATT_CHARA_PROP_WRITE,
	0,
	0,
	0x2a06&0xff,
	0x2a06>>8
}};

static gatt_chara_def_short_t wifi_mgr_chara1 = {{
	ATT_CHARA_PROP_READ | ATT_CHARA_PROP_NOTIFY,
	0,
	0,
	0x2a00&0xff,
	0x2a00>>8
}};

// static gatt_chara_def_short_t wifi_mgr_chara2 = {{
// 	ATT_CHARA_PROP_READ | ATT_CHARA_PROP_WRITE,
// 	0,
// 	0,
// 	0x2906&0xff,
// 	0x2906>>8
// }};

UINT8 data_write_callback(void *param)
{
	//UINT8 datalen = 0;
	//OSI_PRINTFI("[ble_wrapper][%s:%d]wifimgr_value=%s", __FUNCTION__, __LINE__,wifimgr_value);
	BoatLog(BOAT_LOG_NORMAL,"wifimgr_value = %s",wifimgr_value);

    if(param != NULL)
    {
		gatt_srv_write_notify_t *pAttr = (gatt_srv_write_notify_t *)param;
		//OSI_PRINTFI("[ble_wrapper][%s:%d]pAttr->->valueLen = %d", __FUNCTION__, __LINE__,pAttr->valueLen);
        dalBleServiceCallback((UINT8 *)wifimgr_value,(UINT32)(pAttr->valueLen));

	}
	return 0;	
}


UINT8 wifimgr_value_read_cb(void *param)
{
    if(param != NULL)
    {
		gatt_srv_write_notify_t *pAttr = (gatt_srv_write_notify_t *)param;
		//OSI_PRINTFI("[ble_wrapper][%s:%d]pAttr->->valueLen = %d", __FUNCTION__, __LINE__,pAttr->valueLen);
		BoatLog(BOAT_LOG_NORMAL,"pAttr->->valueLen = %d",pAttr->valueLen);
	}
	memset(wifimgr_value,0,sizeof(wifimgr_value));
	memcpy(wifimgr_value,"abcd123456",10);
    //OSI_PRINTFI("[ble_wrapper][%s:%d] param = %p,wifimgr_value=%s", __FUNCTION__, __LINE__,param,wifimgr_value);
	BoatLog(BOAT_LOG_NORMAL,"param = %p,wifimgr_value=%s",param,wifimgr_value);
	
	//OSI_PRINTFI("[ble_wrapper][%s:%d] current thread ID = 0x%x", __FUNCTION__, __LINE__,fibo_thread_id());
	
	return 0;
}


gatt_element_t config_wifi_service[]={

	{
		//creat service and fill UUID
	    sizeof(test_service),
	   	ATT_PM_READABLE,
	    {ATT_UUID_PRIMARY},
	    ATT_FMT_SHORT_UUID | ATT_FMT_GROUPED,
	    //ATT_FMT_GROUPED,
	    (void *)test_service,
	    NULL,
	    NULL
	},
	{
		//creat chara and fill permission
	    sizeof(wifi_mgr_chara),
		ATT_PM_READABLE,
	    {ATT_UUID_CHAR},
	    ATT_FMT_SHORT_UUID | ATT_FMT_FIXED_LENGTH,
	    (void *)&wifi_mgr_chara,
	    NULL,//cb
	    NULL//read callback
	},
	{
	    sizeof(wifimgr_value),
		ATT_PM_READABLE | ATT_PM_WRITEABLE,
	    {0x2a06},
	    ATT_FMT_SHORT_UUID | ATT_FMT_WRITE_NOTIFY | ATT_FMT_FIXED_LENGTH | ATT_FMT_CONFIRM_NOTITY,
	    (void *)wifimgr_value,
	    data_write_callback,
	    wifimgr_value_read_cb
	},
	{
		//des
	    sizeof(wifimgr_ccc_cfg),
	    ATT_PM_READABLE | ATT_PM_WRITEABLE,
	    {ATT_UUID_CLIENT},
	    ATT_FMT_SHORT_UUID | ATT_FMT_WRITE_NOTIFY|ATT_FMT_FIXED_LENGTH | ATT_FMT_CONFIRM_NOTITY,
	    (void *)wifimgr_ccc_cfg,
	    //wifi_changed_cb,
	    NULL,
	    NULL
	},
	{
	    sizeof(wifi_mgr_chara1),
		ATT_PM_READABLE,
	    {ATT_UUID_CHAR},
	    ATT_FMT_SHORT_UUID | ATT_FMT_FIXED_LENGTH,
	    (void *)&wifi_mgr_chara1,
	    NULL,
	    NULL
	},
	{
	    sizeof(device_name),
		ATT_PM_READABLE,
	    {0x2a00},
	    ATT_FMT_SHORT_UUID | ATT_FMT_WRITE_NOTIFY | ATT_FMT_FIXED_LENGTH | ATT_FMT_CONFIRM_NOTITY,
	    (void *)device_name,
	    NULL,
	    NULL
	},
	{
	    sizeof(wifimgr_ccc_cfg),
	    ATT_PM_READABLE | ATT_PM_WRITEABLE,
	    {ATT_UUID_CLIENT},
	    ATT_FMT_SHORT_UUID | ATT_FMT_WRITE_NOTIFY|ATT_FMT_FIXED_LENGTH | ATT_FMT_CONFIRM_NOTITY,
	    (void *)wifimgr_ccc_cfg,
	    //wifi_changed_cb,
	    NULL,
	    NULL
	},
#if 0
	{
		//creat service and fill UUID
	    sizeof(add_service),
	   	ATT_PM_READABLE,
	    {ATT_UUID_PRIMARY},
	    ATT_FMT_SHORT_UUID | ATT_FMT_GROUPED,
	    (void *)add_service,
	    NULL,
	    NULL
	},
	{
		//creat chara and fill permission
	    sizeof(wifi_mgr_chara2),
		ATT_PM_READABLE,
	    {ATT_UUID_CHAR},
	    ATT_FMT_SHORT_UUID | ATT_FMT_FIXED_LENGTH,
	    (void *)&wifi_mgr_chara2,
	    wifi_changed_cb,//cb
	    wifimgr_value_read_cb//read callback
	},
	{
		//fill chara value 
	    sizeof(wifimgr_value),
		ATT_PM_READABLE | ATT_PM_WRITEABLE,
	    {0x2906},
	    ATT_FMT_SHORT_UUID | ATT_FMT_WRITE_NOTIFY | ATT_FMT_FIXED_LENGTH,
	    (void *)wifimgr_value,
	    data_write_callback,
	    NULL
	},
	{
		//des
	    sizeof(wifimgr_ccc_cfg),
	    ATT_PM_READABLE | ATT_PM_WRITEABLE,
	    {ATT_UUID_CLIENT},
	    ATT_FMT_SHORT_UUID | ATT_FMT_WRITE_NOTIFY|ATT_FMT_FIXED_LENGTH,
	    (void *)wifimgr_ccc_cfg,
	    wifi_changed_cb,
	    NULL
	},
#endif
};

//extern UINT32 g_msg_lock;

void dal_bleService_sig_callback(GAPP_SIGNAL_ID_T sig, va_list arg)
{
    switch (sig)
    {	   
		case GAPP_SIG_BLE_SET_ADV_IND:
		{
			UINT8 type = (UINT8)va_arg(arg, int);
			UINT8 state = (UINT8)va_arg(arg, int);
			//OSI_PRINTFI("[ble_wrapper][%s:%d]type=%d,state=%d", __FUNCTION__, __LINE__,type,state);
			BoatLog(BOAT_LOG_NORMAL,"type=%d,state=%d",type,state);				
		}
		break;
		case GAPP_SIG_BT_ON_IND: 
		{

			//OSI_PRINTFI("[ble_wrapper][%s:%d]GAPP_SIG_BT_ON_IND", __FUNCTION__, __LINE__);
			BoatLog(BOAT_LOG_NORMAL,"GAPP_SIG_BT_ON_IND");		

		}
		break;
		case GAPP_SIG_BLE_CONNECT_IND:
		{
			int connect_id = (int)va_arg(arg, int);
			bool g_ble_connect_status = (int)va_arg(arg, int);

			UINT8 *addr = (UINT8 *)va_arg(arg, UINT8 *);
			UINT8 reason = (UINT8 )va_arg(arg, int);
			va_end(arg);
			//OSI_PRINTFI("[ble_wrapper][%s:%d]type=%d,state=%d,%p,%d", __FUNCTION__, __LINE__,connect_id,g_ble_connect_status,addr,reason);
			BoatLog(BOAT_LOG_NORMAL,"type=%d,state=%d,%p,%d",connect_id,g_ble_connect_status,addr,reason);	
			if(addr != NULL)
			{
				//OSI_PRINTFI("[ble_wrapper][%s:%d]type=%d,state=%d,%s,%d", __FUNCTION__, __LINE__,connect_id,g_ble_connect_status,addr,reason);
				BoatLog(BOAT_LOG_NORMAL,"type=%d,state=%d,%p,%d",connect_id,g_ble_connect_status,addr,reason);	
			}

            g_acl_handle = (int)(connect_id);

			// if(g_msg_lock)
			// {
			// 	fibo_sem_signal(g_msg_lock);
			// }
		}
		break;
		
	    default:
	    {
	        break;
	    }
    }

}


/*!*****************************************************************************
@brief Init a BoAT BLE service

Function: boatBleServiceInit()

     The boatBleServiceInit() function init a BLE service \n
     used to register callback function,turn on BT,set ble service name, \n
     create a service and characteristic,open the broadcast.

@return
    This function returns BOAT_SUCCESS if the initialization is successful. \n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.  

@param[in] rxCallback
    The rxCallback is the ble service receiving call back function defined by user.
    This function is used to get response message from ble service by ble client.
*******************************************************************************/
BOAT_RESULT boatBleServiceInit(boatBleServiceRxCallback rxCallback)
{
    INT32 ret;
    UINT32 size;

    if(g_bleService_is_initialized == BOAT_FALSE)
    {

        /* register callback function */
        dalBleServiceCallback = rxCallback;

        /* turn on BT */
        ret = fibo_bt_onoff(1);
        if(ret < 0)
        {
            //OSI_PRINTFI("ble_wrapper:[%s:%d] Turn on BT failed!", __FUNCTION__, __LINE__);
            return BOAT_ERROR_DAL_BT_TURNON_FAILED;
        }
        fibo_taskSleep(2000);

        /* Set ble Name */
        uint8_t name_set[10] = {0};
	    //memset(name_set,0,10);
	    memcpy(name_set,"L610_ble",sizeof("L610_ble"));
	    ret = fibo_ble_set_read_name(0,name_set,0); 

        /* Create serive and characteristic */
        size = sizeof(config_wifi_service)/sizeof(gatt_element_t);
	    //OSI_PRINTFI("ble_wrapper:[%s:%d] size = %d", __FUNCTION__, __LINE__,size);
        fibo_ble_add_service_and_characteristic(config_wifi_service,size);
        fibo_taskSleep(2000);
        
        /* Open broadcast */
        ret = fibo_ble_enable_dev(1); 
        if(ret != 0)
        {
            //OSI_PRINTFI("ble_wrapper:[%s:%d] Open broadcast failed!", __FUNCTION__, __LINE__);
            return BOAT_ERROR_DAL_BT_BROADCAST_OPEN_FAILED;
        }

        g_bleService_is_initialized = BOAT_TRUE;

        return BOAT_SUCCESS;

    }
    else
    {
        return BOAT_ERROR_DAL_BLESERVICE_INITIALIZED;
    }

}


/*!*****************************************************************************
@brief Sending notification message to ble client by ble service

Function: boatBleServiceSendNOTIFICATION()

     The boatBleServiceSendNOTIFICATION() sends notification message to ble client.

@return
    This function returns BOAT_SUCCESS if the transmission is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] buf
    The cmd points to the first address of the message.

@param[in] len
	The len indicates the number of buf.

*******************************************************************************/
BOAT_RESULT boatBleServiceSendNOTIFICATION(BUINT8 *buf, BUINT16 len)
{
	if((buf == NULL) || (len == 0))
    {
        return BOAT_ERROR_DAL_INVALID_ARGUMENT;
    }

    if(g_bleService_is_initialized == BOAT_TRUE)
    {
	    gatt_le_data_info_t notify;
	    notify.att_handle = wifi_mgr_chara1.value[2] << 8 | wifi_mgr_chara1.value[1];
	    notify.length = len;
	    notify.data = buf;
	    notify.acl_handle = g_acl_handle;//no effect ,one connect acl_handle
	
	    fibo_ble_notify((gatt_le_data_info_t *)&notify,GATT_NOTIFICATION);
	    //OSI_PRINTFI("[ble_wrapper][%s:%d] %s,handle=%x", __FUNCTION__, __LINE__,buf,notify.att_handle);
		BoatLog(BOAT_LOG_NORMAL,"%s,handle=%x",buf,notify.att_handle);
	    return BOAT_SUCCESS;
    }
    else
    {
        return BOAT_ERROR_DAL_BLESERVICE_NOT_INITIALIZED;
    }

}


/*!*****************************************************************************
@brief Sending indication message to ble client by ble service

Function: boatBleServiceSendINDICATION()

     The boatBleServiceSendINDICATION() sends indication message to ble client.

@return
    This function returns BOAT_SUCCESS if the transmission is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] buf
    The cmd points to the first address of the message.

@param[in] len
	The len indicates the number of buf.

*******************************************************************************/
BOAT_RESULT boatBleServiceSendINDICATION(BUINT8 *buf, BUINT16 len)
{
	if((buf == NULL) || (len == 0))
    {
        return BOAT_ERROR_DAL_INVALID_ARGUMENT;
    }
	
    if(g_bleService_is_initialized == BOAT_TRUE)
    {
		memset(device_name,0,sizeof(device_name));
		memcpy(device_name,buf,len);

	    gatt_le_data_info_t indication;
	    indication.att_handle = wifi_mgr_chara1.value[2] << 8 | wifi_mgr_chara1.value[1];
	    indication.length = len;
	    indication.data = (UINT8 *)&device_name[0];
	    indication.acl_handle = g_acl_handle;//no effect ,one connect acl_handle
	
	    fibo_ble_notify((gatt_le_data_info_t *)&indication,GATT_INDICATION);
	    //OSI_PRINTFI("[ble_wrapper][%s:%d] %s,handle=%x", __FUNCTION__, __LINE__,buf,indication.att_handle);
		BoatLog(BOAT_LOG_NORMAL,"%s,handle=%x",buf,indication.att_handle);
	    return BOAT_SUCCESS;
    }
    else
    {
        return BOAT_ERROR_DAL_BLESERVICE_NOT_INITIALIZED;
    }

}

/*!*****************************************************************************
@brief Deinit a BoAT BLE service

Function: boatBleServiceDeinit()

     The boatBleServiceDeinit() function close a ble service.

@return
    This function returns BOAT_SUCCESS if the closing is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.    

*******************************************************************************/
BOAT_RESULT boatBleServiceDeinit(void)
{
    INT32 ret;
    if(g_bleService_is_initialized == BOAT_TRUE)
    {
        ret = fibo_bt_onoff(0);
        if(ret < 0)
        {
            //OSI_PRINTFI("ble_wrapper:[%s:%d] Turn on BT failed!", __FUNCTION__, __LINE__);
            return BOAT_ERROR_DAL_BT_TURNOFF_FAILED;
        }
        g_bleService_is_initialized = BOAT_FALSE;
    }
    
    return BOAT_SUCCESS;

}