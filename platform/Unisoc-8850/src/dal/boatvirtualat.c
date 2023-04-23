#include "boattypes.h"
#include "boaterrcode.h"
#include "boatdal.h"
#include "boatlog.h"

#include "osi_api.h"
#include "osi_log.h"
#include "osi_pipe.h"
#include "at_engine.h"
#include <string.h>

static BUINT32 (* dalVirtualAtCallback)(char *content,BUINT32 len);

static osiPipe_t *g_at_rx_pipe = NULL;
static osiPipe_t *g_at_tx_pipe = NULL;
static atDevice_t *g_device = NULL;
static atDispatch_t *g_dispatch = NULL;

static void prvVirtAtRespCallback(void *param, unsigned event)
{
    osiPipe_t *pipe = (osiPipe_t *)param;
    char buf[256];
    for (;;)
    {
        int bytes = osiPipeRead(pipe, buf, 255);
        if (bytes <= 0)
            break;

		if(bytes >= 256)
		{
			BoatLog(BOAT_LOG_CRITICAL,"data buffer is too small!");
			return;
		}
        buf[bytes] = '\0';
        BoatLog(BOAT_LOG_NORMAL, "VAT1 <--(%d): %s", bytes, buf);
		dalVirtualAtCallback(buf,bytes+1);
    }
}


BOAT_RESULT boatVirtualAtOpen(boatVirtualAtRxCallback rxCallback)
{
	g_at_rx_pipe = osiPipeCreate(1024);
    g_at_tx_pipe = osiPipeCreate(1024);
    osiPipeSetReaderCallback(g_at_tx_pipe, OSI_PIPE_EVENT_RX_ARRIVED,
                             prvVirtAtRespCallback, g_at_tx_pipe);

    atDeviceVirtConfig_t cfg = {
        .name = OSI_MAKE_TAG('V', 'A', 'T', '1'),
        .rx_pipe = g_at_rx_pipe,
        .tx_pipe = g_at_tx_pipe,
    };
    g_device = atDeviceVirtCreate(&cfg);
    g_dispatch = atDispatchCreate(g_device);
    atDeviceSetDispatch(g_device, g_dispatch);

	dalVirtualAtCallback = rxCallback;

	return atDeviceOpen(g_device);
}
BOAT_RESULT boatVirtualAtSend(char *cmd, BUINT16 len)
{
	if(g_at_rx_pipe == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL,"Need open virtualAT first!");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	int write_len = osiPipeWriteAll(g_at_rx_pipe,cmd,len,OSI_WAIT_FOREVER);
	if(write_len != len)
	{
		BoatLog(BOAT_LOG_CRITICAL,"VirtualAT write failed!");
		return BOAT_ERROR;
	}
	return BOAT_SUCCESS;
}
BOAT_RESULT boatVirtualAtTimedSend(char *cmd, BUINT16 len, BUINT16 timeout)
{
	if(g_at_rx_pipe == NULL)
	{
		BoatLog(BOAT_LOG_CRITICAL,"Need open virtualAT first!");
		return BOAT_ERROR_COMMON_INVALID_ARGUMENT;
	}
	int write_len = osiPipeWriteAll(g_at_rx_pipe,cmd,len,timeout);
	if(write_len != len)
	{
		BoatLog(BOAT_LOG_CRITICAL,"VirtualAT write failed!");
		return BOAT_ERROR;
	}
	return BOAT_SUCCESS;
}
BOAT_RESULT boatVirtualAtClose(void)
{
	osiPipeStop(g_at_rx_pipe);
	osiPipeStop(g_at_tx_pipe);

	osiPipeDelete(g_at_rx_pipe);
	osiPipeDelete(g_at_tx_pipe);

	g_at_rx_pipe = NULL;
	g_at_tx_pipe = NULL;

	atDeviceDelete(g_device);
	atDispatchDelete(g_dispatch);

	g_device = NULL;
	g_dispatch = NULL;
	
	return BOAT_SUCCESS;
}

