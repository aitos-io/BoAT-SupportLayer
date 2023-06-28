

#include "mbtk_open_at.h"
#include "ol_os_comm_event.h"
#include "mbtk_comm_api.h"
#include "mbtk_os.h"
#include "stdio.h"

#include "boattypes.h"
#include "boaterrcode.h"
#include "boatdal.h"
#include "boatlog.h"


#define AT_TASK_STACK_SIZE  2*1024 
#define AT_TASK_PRIORITY    224

static mbtk_msgqref at_msgq;
typedef struct
{
    int value;
    void *msg;
}msg_struct;



BUINT32 (* dalVirtualAtCallback)(char *content,BUINT32 len);

/*
* Mark the use status of the virtual AT channel. 
* It is BOAT_TRUE when it is occupied, otherwise it is BOAT_FALSE
*/
static BBOOL g_virtualAT_Channel_is_used = BOAT_FALSE;



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
    mbtk_os_status os_status = 0;
    msg_struct msg;
    int result = 0;

    mbtk_taskref demo_task_ref; 

    if(g_virtualAT_Channel_is_used == BOAT_FALSE)
    {
        /* register callback function */
        dalVirtualAtCallback = rxCallback;


        result = ol_os_msgq_creat(&at_msgq, "boat_visual_at", sizeof(int), 20,MBTK_OS_FIFO);
        if(result != mbtk_os_success)
        {
            BoatLog(BOAT_LOG_CRITICAL,"-----ol_os_msgq_creat fail!!!------\n");
            return BOAT_ERROR;
        }

        if(ol_at_init(&at_msgq) != OL_E_NONE)
        {
            BoatLog(BOAT_LOG_CRITICAL,"-----ol_at_init fail!!!------\n");
            ol_os_msgq_delete(at_msgq);
            return BOAT_ERROR;
        }
        //start a thread to recv at message
        char *task_stack_ptr =  NULL;
        task_stack_ptr = ol_malloc(AT_TASK_STACK_SIZE);
        if(task_stack_ptr == NULL)
        {
            BoatLog(BOAT_LOG_CRITICAL,"os_demo malloc task_stack_ptr fail \n");
            ol_os_msgq_delete(at_msgq);
            return BOAT_ERROR;
        }
        ol_os_task_creat(&demo_task_ref, task_stack_ptr, AT_TASK_STACK_SIZE, AT_TASK_PRIORITY, "at_recv_task", at_recv_task, NULL);

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
        ol_send_at_command(cmd);
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
        ol_at_deinit();
        g_virtualAT_Channel_is_used = BOAT_FALSE;
    }

    return BOAT_SUCCESS;
    
}