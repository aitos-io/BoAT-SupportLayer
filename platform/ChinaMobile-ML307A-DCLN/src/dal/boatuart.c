
#include "boatplatformdal.h"

#ifdef PLATFORM_DAL_UART

#include "stdio.h"
#include "boattypes.h"
#include "boatlog.h"
#include "boatdal.h"


#include "cm_os.h"
#include "cm_uart.h"
#include "cm_mem.h"
#include "cm_iomux.h"


//uart0
#if (OPENCPU_MAIN_URAT == 0)
#define OPENCPU_TEST_UARTTX_IOMUX CM_IOMUX_PIN_18, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_UARTRX_IOMUX CM_IOMUX_PIN_17, CM_IOMUX_FUNC_FUNCTION1
#endif

//uart1
#if (OPENCPU_MAIN_URAT == 1)
#define OPENCPU_TEST_UARTTX_IOMUX CM_IOMUX_PIN_28, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_UARTRX_IOMUX CM_IOMUX_PIN_29, CM_IOMUX_FUNC_FUNCTION1
#endif

#if (OPENCPU_MAIN_URAT == 2)
#define OPENCPU_TEST_UARTTX_IOMUX CM_IOMUX_PIN_50, CM_IOMUX_FUNC_FUNCTION3
#define OPENCPU_TEST_UARTRX_IOMUX CM_IOMUX_PIN_51, CM_IOMUX_FUNC_FUNCTION3
#endif

#define OPENCPU_UART_WEKEUP_PIN CM_IOMUX_PIN_17

#define OPENCPU_TEST_PWM0_IOMUX CM_IOMUX_PIN_74, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_PWM1_IOMUX CM_IOMUX_PIN_75, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_TEST_PWM2_IOMUX CM_IOMUX_PIN_21, CM_IOMUX_FUNC_FUNCTION2




static void (*boatUartRecvCallback)(boatUart *uartRef,  unsigned char *data, BUINT32 len);

static  cm_uart_dev_e  g_ML307_port;

static uint8_t *g_recvBuf_ptr = NULL;
static int g_recvBuf_len = 0;

/*!*****************************************************************************
@brief Init a BoAT uart device

Function: boatUartInit()

     The boatUartInit() function open a uart devie and \n
     init the boatUart descriptor.

@return
    This function returns BOAT_SUCCESS if the initialization is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.  

@param[in] uartRef
    A boatUart struct pointer,the uartRef is the boatUart descriptor \n
    used to indicate which uart device is opened.

@param[in] port
    Port is the number of uart device, start at 0

@param[in] config
    The config is the configuration parameters of uart device,including:\n
    	uart device number 
    	specific rate 
    	data bit 
    	stop bit
    	check bit

@param[in] rxCallback
    The rxCallback is the uart receiving call back function defined by user.
    This function is used to get data from uart device.

*******************************************************************************/
static void cm_serial_uart_callback(void *param, uint32_t type)
{
    int32_t ret = 0;
    if (CM_UART_EVENT_TYPE_RX_ARRIVED & type)
    {
        //Read uart data
        ret = cm_uart_read(g_ML307_port,g_recvBuf_ptr,g_recvBuf_len,1000);
        BoatLog(BOAT_LOG_CRITICAL,"cm_uart_read get data =>%s",g_recvBuf_ptr);
        BoatLog(BOAT_LOG_CRITICAL,"cm_uart_read get data len=%d",ret);
        if(ret < 0)
        {
            BoatLog(BOAT_LOG_CRITICAL,"UART read data Failed");
        }
        else
        {
            boatUart uartRef = {0};
            uartRef.uartPort = g_ML307_port;
            boatUartRecvCallback(&uartRef,g_recvBuf_ptr,ret);
        }
        
    }

}

BOAT_RESULT boatUartInit(boatUart *uartRef, BUINT8 port, boatUartConfig config,boatUartRxCallback rxCallback)
{
    int32_t ret = -1;
    char *uartName[3]={"uart0","uart1","uart2"};
    cm_uart_cfg_t uartConfig = {0};
    cm_uart_event_t uartEvent = {0};

    if((uartRef == NULL) || (port < CM_UART_DEV_0) || (port > CM_UART_DEV_2))
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad params!");
        return BOAT_ERROR_DAL_INVALID_ARGUMENT;
    }

    uartRef->uartPort = port;
    g_ML307_port = port;

    uartConfig.baudrate = config.baudrate;
    uartConfig.flow_ctrl = CM_UART_FLOW_CTRL_NONE; //only support NONE_FLOW_CTRL
    uartConfig.parity = config.parity;

    switch (config.stopbit)
    {
    case 1:
        uartConfig.stop_bit = CM_UART_STOP_BIT_ONE;
        break;
    case 2:
        uartConfig.stop_bit = CM_UART_STOP_BIT_TWO;
    default:
        uartConfig.stop_bit = config.stopbit;
        break;
    }
    
    uartConfig.byte_size = config.databit;

    uartEvent.event_type = CM_UART_EVENT_TYPE_RX_ARRIVED|CM_UART_EVENT_TYPE_RX_OVERFLOW;
    uartEvent.event_param = uartName[port];
    uartEvent.event_entry = cm_serial_uart_callback;

    boatUartRecvCallback = rxCallback; 

   // cm_iomux_set_pin_func(OPENCPU_TEST_UARTTX_IOMUX);
   // cm_iomux_set_pin_func(OPENCPU_TEST_UARTRX_IOMUX);

    ret = cm_uart_register_event(uartRef->uartPort, &uartEvent); 
    if(ret != 0)
    {
        BoatLog(BOAT_LOG_CRITICAL,"UART register event Failed.");
        return BOAT_ERROR;
    }

    ret = cm_uart_open(uartRef->uartPort,&uartConfig);
    if(ret != 0)
    {
        BoatLog(BOAT_LOG_CRITICAL,"UART:%d open Failed.",uartRef->uartPort);
        return BOAT_ERROR;
    }

    if(uartRef->uartPort == CM_UART_DEV_0)
    {
        /* 配置uart唤醒功能，使能边沿检测才具备唤醒功能，仅主串口具有唤醒功能，用于唤醒的数据并不能被uart接收，请在唤醒后再进行uart数传 */
        cm_iomux_set_pin_cmd(OPENCPU_UART_WEKEUP_PIN, CM_IOMUX_PINCMD1_LPMEDEG, CM_IOMUX_PINCMD1_FUNC1_LPM_EDGE_RISE);
    }

    if(g_recvBuf_ptr == NULL)
    {
        if(config.rxBufSize > 0)
        {
            g_recvBuf_ptr = cm_malloc(config.rxBufSize);
            g_recvBuf_len = config.rxBufSize;
        }
        else
        {
            g_recvBuf_ptr = cm_malloc(256);
            g_recvBuf_len = 256;
        } 
    }

    return BOAT_SUCCESS;

}

/*!*****************************************************************************
@brief Sending data through the uart

Function: boatUartWrite()

     The boatUartWrite() sends data to the uart device specified by uartRef.

@return
    This function returns BOAT_SUCCESS if the transmission is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.     

@param[in] uartRef
    A boatUart struct pointer,the uartRef indicates which uart device\n
    will be used to send data.

@param[in] data
    The data points to the first address of the sending data.

@param[in] len
    The len is the length of sending data in bytes.
*******************************************************************************/
BOAT_RESULT boatUartWrite(boatUart *uartRef,unsigned char *data, BUINT32 len)
{
    int32_t  ret;
    if((uartRef == NULL) || (data == NULL) || (len == 0))
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad Params!");
        return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
    }
    ret = cm_uart_write(uartRef->uartPort,data,len,1000);
    if(ret != len)
    {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}

/*!*****************************************************************************
@brief Deinit a BoAT uart device

Function: boatUartDeinit()

     The boatUartDeinit() function close a uart devie and \n
     deinit the boatUart descriptor.

@return
    This function returns BOAT_SUCCESS if the deinit is successful.\n
    Otherwise it returns BOAT_ERROR or a negative value to \n
    indicate the error, for reference in boaterrcode.h.     

@param[in] uartRef
    A boatUart struct pointer,the uartRef indicates which uart device\n
    will be closed.

*******************************************************************************/
BOAT_RESULT boatUartDeinit(boatUart *uartRef)
{
    int32_t ret ;
    if(uartRef == NULL)
    {
        BoatLog(BOAT_LOG_CRITICAL,"Bad params!");
        return BOAT_ERROR_DAL_INVALID_ARGUMENT;
    }

    if(g_recvBuf_ptr != NULL)
    {
        cm_free(g_recvBuf_ptr);
    }
    g_recvBuf_ptr = NULL;
    g_recvBuf_len = 0;

    ret = cm_uart_close(uartRef->uartPort);
    if(ret != 0)
    {
        return BOAT_ERROR;
    }

    return BOAT_SUCCESS;
}
#endif//PLATFORM_DAL_UART