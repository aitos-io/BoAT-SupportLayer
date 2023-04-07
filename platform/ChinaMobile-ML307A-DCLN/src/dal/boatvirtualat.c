
#include "boatdal.h"

#ifdef PLATFORM_DAL_VIRTUALAT

#include "stdio.h"
#include "cm_os.h"
#include "cm_virt_at.h"

#include "boattypes.h"
#include "boatlog.h"


/*
* Mark the use status of the virtual AT channel. 
* It is BOAT_TRUE when it is occupied, otherwise it is BOAT_FALSE
*/
static BBOOL g_virtualAT_Channel_is_used = BOAT_FALSE;


BUINT32 (* dalVirtualAtCallback)(char *content,BUINT32 len);

static unsigned char at_data[256]={0};  

static void oc_resp_cb(void * param) 
{  
     
    int32_t get_len = cm_virt_at_get(at_data,sizeof(at_data));

    if(get_len > 0)
    {
        dalVirtualAtCallback((char *)&at_data[0],get_len);
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
    if(rxCallback == NULL)
    {
        return BOAT_ERROR_DAL_INVALID_ARGUMENT;
    }

    if(g_virtualAT_Channel_is_used == BOAT_FALSE)
    {
        /* register callback function */
        dalVirtualAtCallback = rxCallback;

        int32_t ret = cm_virt_at_init(oc_resp_cb);
        if(ret < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL,"Virtual AT init failed!");
            return BOAT_ERROR;
        }

        g_virtualAT_Channel_is_used = BOAT_TRUE;
        return BOAT_SUCCESS;
    }
    else
    {
        /* virtual AT channel is busy */
        return BOAT_ERROR_DAL_VIRTUAL_BUSY;
    }
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

    if(g_virtualAT_Channel_is_used == BOAT_TRUE)
    {
        int32_t send_len = cm_virt_at_send((uint8_t *)cmd,(int32_t)len);
        if(send_len < 0)
        {
            return BOAT_ERROR;
        }
        return BOAT_SUCCESS;
    }
    else
    {
        return BOAT_ERROR_DAL_VIRTUAL_CLOSED;
    }
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
    return BOAT_ERROR_DAL_VIRTUAL_NOT_SUPPORT;
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
    
    if(g_virtualAT_Channel_is_used == BOAT_TRUE)
    {
        g_virtualAT_Channel_is_used = BOAT_FALSE;
        cm_virt_at_deinit();
    }

    return BOAT_SUCCESS;
}
#endif