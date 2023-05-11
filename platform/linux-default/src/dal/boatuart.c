/**
****************************************************************************************
* @FilePath: boatuart.c
* @Author: aitos
* @Date: 2023-02-17 16:00:41
* @LastEditors:
* @LastEditTime: 2023-02-17 16:01:03
* @Descripttion: creat boatuart.c
****************************************************************************************
*/
/******************************************************************************
 * Copyright (C) 2018-2021 aitos.io
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>	
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "boattypes.h"
#include "boaterrcode.h"
#include "boatlog.h"
#include "boatdal.h"


#define UART_BUF_LEN_DEFAULT 256
#define UART_PORT_NUMBER_MAX 5
#define UART_DATABITS_MIN 5
#define UART_DATABITS_MAX 8
#define UART_STOPBITS_MIN 1
#define UART_STOPBITS_MAX 2
#define UART_PARITY       2

#define FIBO_UART_MAX 5

const char *uartDevName[]={"/dev/ttyS0", "/dev/ttyS2", "/dev/ttyUSB1"};
#define UART_PORT_MAX  (sizeof(uartDevName) / (sizeof(char *)))
/////answer code: 00 61 69 74 6F 73 FF
/**
 * \brief define
 * @{
 */
static void *boatUartDefaultThread(void *argv)
{
	boatUart *uartRef;
	struct  timeval tv = {0,100*1000};// 100ms
	int rtnVal = 0;
	int errCount = 0;
	int rdLen = 0;
	uartRef = (boatUart *) argv;
	if(uartRef == NULL )
	{
		return NULL;
	}


	while(1)
	{
		if(uartRef->rdBreak == 0)
		{
			break;
		}

		FD_ZERO(&uartRef->recvFds);
		FD_SET(uartRef->uartId,&uartRef->recvFds);
		rtnVal = select(uartRef->uartId+1, &uartRef->recvFds, NULL, NULL, &tv);// select will updata tv
        if(rtnVal < 0)
        {
            printf("select err");// select error
            //usleep(10000);// 10 ms
			errCount++;
			if(errCount > 10)	// Continuous error lasts for 1 second
			{
				break;
			}
            continue;
        }
        else if(rtnVal == 0)
        {
            //usleep(10000);// 10 ms
			errCount = 0;
            continue;
        }
        else                      
        {
			errCount = 0;
            if(FD_ISSET(uartRef->uartId, &uartRef->recvFds))
            {
                rdLen  = read(uartRef->uartId,uartRef->rdBuf,0xff);
				if(rdLen > 0)
				{
					if(uartRef->rdCallback != NULL)
					{
						uartRef->rdCallback((void *)uartRef,uartRef->rdBuf,rdLen);
					}
				}
            }
        }
	}
	printf("uart receive thread quit\r\n");
	return NULL;
}
/**
 * \brief uart setting
 * 
 * \param[in] fd uart descriptor
 * \param[in] baudrate
 *            #{0, 50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800, 
 *              2400, 4800, 9600, 19200, 38400, 57600, 115200, 230400}
 * \param[in] bits
 *            #{5, 6, 7, 8}
 * \param[in] parity
 *            #'n'/'N'
 *            #'o'/'O' ODD
 *            #'e','E' EVEN
 * \param[in] stop
 *            #1: 1 bit
 *            #2: 2 bits
 * \param[in] flow: 
 *            #'n'/'N'
 *
 * \retval succ 0, failed -1
 *
 */
int uart_set(int fd, int baudrate, int bits, char parity, int stop, char flow)
{
    struct termios termios_uart;
    int ret = 0;
    speed_t uart_speed = 0;
    
    /* get uart attr */
    memset(&termios_uart, 0, sizeof(termios_uart));
    ret = tcgetattr(fd, &termios_uart);
    if (ret == -1) {
        printf("tcgetattr failed\n");
        return -1;
    }
    
    //__print_termios(&termios_uart);
    
    /* set baudrate */
    switch (baudrate) {
        case 0:      uart_speed = B0;      break;
        case 50:     uart_speed = B50;     break;
        case 75:     uart_speed = B75;     break;
        case 110:    uart_speed = B110;    break;
        case 134:    uart_speed = B134;    break;
        case 150:    uart_speed = B150;    break;
        case 200:    uart_speed = B200;    break;
        case 300:    uart_speed = B300;    break;
        case 600:    uart_speed = B600;    break;
        case 1200:   uart_speed = B1200;   break;
        case 1800:   uart_speed = B1800;   break;
        case 2400:   uart_speed = B2400;   break;
        case 4800:   uart_speed = B4800;   break;
        case 9600:   uart_speed = B9600;   break;
        case 19200:  uart_speed = B19200;  break;
        case 38400:  uart_speed = B38400;  break;
        case 57600:  uart_speed = B57600;  break;
        case 115200: uart_speed = B115200; break;
        case 230400: uart_speed = B230400; break;
        default: printf("Baud rate not supported\n"); return -1;
    }
    cfsetspeed(&termios_uart, uart_speed);
    
    /* set \data bits */
    switch (bits) {
        case 5:     /* data bits  5 */
            termios_uart.c_cflag &= ~CSIZE; 
            termios_uart.c_cflag |= CS5; 
        break;
        
        case 6:     /* data bits  6 */
            termios_uart.c_cflag &= ~CSIZE;
            termios_uart.c_cflag |= CS6; 
        break;
        
        case 7:     /* data bits  7 */
            termios_uart.c_cflag &= ~CSIZE;
            termios_uart.c_cflag |= CS7; 
        break;
        
        case 8:     /* data bits  8 */
            termios_uart.c_cflag &= ~CSIZE;
            termios_uart.c_cflag |= CS8; 
        break;
        
        default: 
            printf("Data bits not supported\n"); 
            return -1;
    }
    
    /* parity setting */
    switch (parity) {
        case 0:   /* None */
            termios_uart.c_cflag &= ~PARENB;
            termios_uart.c_iflag &= ~INPCK;        /* disable parity */
			termios_uart.c_iflag &= ~ISTRIP;
        break;
        
        case 1:   /* ODD */
            termios_uart.c_cflag |= PARENB;
            termios_uart.c_cflag |= PARODD;
            termios_uart.c_iflag |= INPCK;        /* enable odd */
            termios_uart.c_iflag &= ~ISTRIP;        /* clean bit 8 */
        break;
        
        case 2:   /* EVEN */
            termios_uart.c_cflag |= PARENB;
            termios_uart.c_cflag &= ~PARODD;
            termios_uart.c_iflag |= INPCK;        /* enable even */
            termios_uart.c_iflag &= ~ISTRIP;        /* clean bit 8 */
        break;
        
        default:
            printf("Parity not supported\n"); 
            return -1;
    }
    
    /* stop bit settting */
    switch (stop) {
        case 1: termios_uart.c_cflag &= ~CSTOPB; break; /* 1 stop bit */
        case 2: termios_uart.c_cflag |= CSTOPB;  break; /* 2 stop bits */
        default: printf("Stop bits not supported\n");
    }
    
    /* flow setting */
    switch (flow) {
		default:
			termios_uart.c_cflag &= ~CRTSCTS;
            termios_uart.c_iflag &= ~(IXON | IXOFF | IXANY);

            printf("no flow control as default\n");
    }
    
    /* 其他设置 */
    termios_uart.c_cflag |= CLOCAL;    /* igorn modem control line */
    termios_uart.c_cflag |= CREAD;    /* enable receive */
    
    /* disalbe (implementation-defined) out put , no spceific charactor output,
    	putout raw data only */
    termios_uart.c_oflag &= ~OPOST;  
    
    /**
     *  设置本地模式位原始模式
     *  ICANON: 规范输入模式，如果设置了那么退格等特殊字符会产生实际动作
     *  ECHO: 则将输入字符回送到终端设备
     *  ECHOE: 如果ICANON也设置了，那么收到ERASE字符后会从显示字符中擦除一个字符
     *         通俗点理解就是收到退格键后显示内容会往回删一个字符
     *  ISIG: 使终端产生的信号起作用。（比如按ctrl+c可以使程序退出）
     */
    termios_uart.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    termios_uart.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

    /**
     * 设置等待时间和最小接收字符
     * 这两个值只有在阻塞模式下有意义，也就是说open的时候不能传入O_NONBLOCK，
     * 如果经过了c_cc[VTIME]这么长时间，缓冲区内有数据，但是还没达到c_cc[VMIN]个
     * 数据，read也会返回。而如果当缓冲区内有了c_cc[VMIN]个数据时，无论等待时间
     * 是否到了c_cc[VTIME]，read都会返回，但返回值可能比c_cc[VMIN]还大。如果将
     * c_cc[VMIN]的值设置为0，那么当经过c_cc[VTIME]时间后read也会返回，返回值
     * 为0。如果将c_cc[VTIME]和c_cc[VMIN]都设置为0，那么程序运行的效果与设置
     * O_NONBLOCK类似，不同的是如果设置了O_NONBLOCK，那么在没有数据时read返回-1，
     * 而如果没有设置O_NONBLOCK，那么在没有数据时read返回的是0。
     */
    termios_uart.c_cc[VTIME] = 1;   /* 设置等待时间，单位1/10秒 */
    termios_uart.c_cc[VMIN]  = 1;    /* 最少读取一个字符 */
    
    tcflush(fd, TCIFLUSH);          /* 清空读缓冲区 */
   
    /* wirte config */
    ret = tcsetattr(fd, TCSANOW, &termios_uart);
    if (ret == -1) {
        printf("tcsetattr failed\n");
    }
    
    return ret;
}

BOAT_RESULT boatUartInit(boatUart *uartRef, BUINT8 port, boatUartConfig config,boatUartRxCallback rxCallback)
{
	if(uartRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart device uartRef is NULL!\r\n");
		return BOAT_ERROR;
	}

	if((config.stopbit > UART_STOPBITS_MAX)	|| (config.stopbit < UART_STOPBITS_MIN)) 
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart config.stopbit is invalid![%d]\r\n",config.stopbit);
		return BOAT_ERROR;
	}
	//! databit : 5\6\7\8
	if((config.databit < UART_DATABITS_MIN)	 || (config.databit > UART_DATABITS_MAX))
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart config.databit is invalid![%d]\r\n",config.databit);
		return BOAT_ERROR;
	}
 	//! parity 	 0: No parity check, 1: Parity check is odd, 2: Parity check is even
	if(config.parity > UART_PARITY)	
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart config.parity is invalid![%d]\r\n",config.parity);
		return BOAT_ERROR;
	}
	//! rxCallback
	if(rxCallback == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart device rxCallback is NULL!\r\n");
		return BOAT_ERROR;
	}

	// port to devname
    /* O_NOCTTY: Prevent the operating system from specifying the open file as 
    			the control terminal of the process. If this flag is not 
    			specified, any input will affect the user's process. */
    /* O_NONBLOCK: Make I/O become non-blocking mode, call read and return - 1 
    			immediately if no data is received, and set errno to EAGAIN。*/
    /* O_NDELAY:  same as O_NONBLOCK */
	if (port >= UART_PORT_MAX)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart port number is invalid [%d],must be less than [%ld]",port,UART_PORT_MAX);
		return BOAT_ERROR;
	}
	
	int fd = open(uartDevName[port], O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd == -1)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart: %s open faild!\r\n",uartDevName[port]);
		return BOAT_ERROR;
	}
	uartRef->uartId = fd;
	uartRef->rdBreak = fd;
	
	// set config
	int retVal = uart_set(fd, config.baudrate, config.databit, config.parity, config.stopbit, 'N');
	if(retVal ==  -1)
	{
		close(fd);
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart: %s setting faild!\r\n","/dev/ttyUSB1");
		return BOAT_ERROR;
	}
	
	// set callback
	uartRef->rdCallback = rxCallback;
	
	// creat receive thread
	
	pthread_t th;
    pthread_attr_t attr;
											
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    int ret = pthread_create(&th, &attr, boatUartDefaultThread, uartRef);
    if (ret != 0)
    {
		close(fd);
		uartRef->rdCallback = NULL;
		uartRef->uartId = 0;
		BoatLog(BOAT_LOG_NORMAL, "ERROR: Create boatUartDefaultThread error\n");
		return BOAT_ERROR;
	}
	else
	{
		BoatLog(BOAT_LOG_NORMAL, "INFO: Create boatUartDefaultThread succ.\n");
		return BOAT_SUCCESS;
	}
}

BOAT_RESULT boatUartWrite(boatUart *uartRef,unsigned char *data, BUINT32 len)
{
	if(uartRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart device uartRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	if(uartRef->uartId == 0)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart device id is 0!\r\n");
		return BOAT_ERROR;
	}	
	if(data == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart write data buf NULL!\r\n");
		return BOAT_ERROR;
	}

	int rtnVal = write(uartRef->uartId, data, len);
	if(rtnVal < 0)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart write failed![%x][%d]\r\n",errno,errno);
		return BOAT_ERROR;
	}
	return BOAT_SUCCESS;
}

BOAT_RESULT boatUartDeinit(boatUart *uartRef)
{
	if(uartRef == NULL)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart device uartRef is NULL!\r\n");
		return BOAT_ERROR;
	}
	if(uartRef->uartId <= 0)
	{
		BoatLog(BOAT_LOG_NORMAL, "[boat][uart ] linux-default uart device id is 0!\r\n");
		return BOAT_ERROR;
	}

	uartRef->rdBreak = 0;
	close(uartRef->uartId);
	uartRef->uartId = 0;
	uartRef->rdCallback = NULL;
	memset(uartRef->rdBuf,0,sizeof(uartRef->rdBuf));
	return BOAT_SUCCESS;	
}

void boatUartInitUartIdNagtive(boatUart *uartRef)
{
	uartRef->uartId = -1;
}


