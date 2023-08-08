
#include "stdio.h"
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatdal.h"
#include "boatlog.h"


#include "simcom_os.h"
#include "simcom_debug.h"
#include "simcom_uart.h"
#include "simcom_common.h"
#include "simcom_system.h"

static char atResBuf[512]={0};

BUINT32 (* dalVirtualAtCallback)(char *content,BUINT32 len);


static void at_recv_task(void)
{
    char r_data[512] = {0};
    int result = 0;
    int event = 0;
    int len = 0;

    while(1)
    {
       result = ol_os_msgq_recv(at_msgq, &event, sizeof(int), 5*200);
       if(result == mbtk_os_success && event == OL_EVENT_AT_RESP)
       {
            memset(r_data, 0x0, sizeof(r_data));
            len = ol_read_at_response(r_data,511);
            dalVirtualAtCallback(r_data,len);
       }
    }
}

/*!*****************************************************************************
@brief Init a BoAT virtual AT channel

Function: boatVirtualAtOpen()

     The boatVirtualAtOpen() function open/create a Virtual at channel\n
     used to send AT command to Module and receive AT response from Module.

@return
    This function returns BOAT_SUCCESS if the initialization is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.  

@param[in] rxCallback
    The rxCallback is the virtual AT receiving call back function defined by user.
    This function is used to get AT response from Module.
*******************************************************************************/
//! BOAT_RESULT BoAT_virtualAT_init(BoAT_virtualAT_rx_callback fun_cb);
BOAT_RESULT boatVirtualAtOpen(boatVirtualAtRxCallback rxCallback)
{
    dalVirtualAtCallback = rxCallback;
    return BOAT_SUCCESS;
}


/*!*****************************************************************************
@brief Sending AT command to Module through virtualAT channel

Function: boatVirtualAtSend()

     The boatVirtualAtSend() sends AT command to Module.

@return
    This function returns BOAT_SUCCESS if the transmission is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] cmd
    The cmd points to the first address of the AT command.

@param[in] len
	The len indicates the number of cmd.

*******************************************************************************/
//! BOAT_RESULT BoAT_virtualAT_send(char *cmd,int timeout);
BOAT_RESULT boatVirtualAtSend(char *cmd, BUINT16 len)
{
    if(((cmd == NULL) || (len == 0)) || (strlen(cmd) != len))
    {
        return BOAT_ERROR_DAL_INVALID_ARGUMENT;
    }

    memset(atResBuf,0,sizeof(atResBuf));

    int ret = sAPI_SendATCMDWaitResp(10,cmd,150,NULL,1,NULL,atResBuf,sizeof(atResBuf));
    if(ret == 0)
    {
        dalVirtualAtCallback(atResBuf,strlen(atResBuf));
    }
    else
    {
        return BOAT_ERROR;
    }


    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief Sending AT command to Module through virtualAT channel

Function: boatVirtualAtTimedSend()

     The boatVirtualAtTimedSend() sends AT command to Module.

@return
    This function returns BOAT_SUCCESS if the transmission is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.

@param[in] cmd
    The cmd points to the first address of the AT command.

@param[in] len
	The len indicates the number of bytes of cmd.

@param[in] timeout
    The timeout indicates the timeout time of AT command response.
    0: invalid , return error
    n: set timeout time to n millisecond

*******************************************************************************/
BOAT_RESULT boatVirtualAtTimedSend(char *cmd, BUINT16 len, BUINT16 timeout)
{
    if(((cmd == NULL) || (len == 0)) || (strlen(cmd) != len))
    {
        return BOAT_ERROR_DAL_INVALID_ARGUMENT;
    }

    memset(atResBuf,0,sizeof(atResBuf));

    int ret = sAPI_SendATCMDWaitResp(10,cmd,timeout,NULL,1,NULL,atResBuf,sizeof(atResBuf));
    if(ret == 0)
    {
        dalVirtualAtCallback(atResBuf,strlen(atResBuf));
    }
    else
    {
        return BOAT_ERROR;
    }


    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief Deinit a BoAT virtual AT channel

Function: boatVirtualAtClose()

     The boatVirtualAtClose() function close a virtual AT channel.

@return
    This function returns BOAT_SUCCESS if the closing is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.    

*******************************************************************************/
//! BOAT_RESULT BoAT_virtualAT_deinit(void);
BOAT_RESULT boatVirtualAtClose(void)
{

    return BOAT_SUCCESS;
    
}