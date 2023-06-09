#include "stdio.h"
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatdal.h"
#include "boatlog.h"

char *vatp = "Linux does not support VirtualAt \r\nok\r\n";

BUINT32 (* dalATRxCallBack)(char *content,BUINT32 len) = NULL;


BOAT_RESULT boatVirtualAtOpen(boatVirtualAtRxCallback rxCallback)
{
	BoatLog(BOAT_LOG_VERBOSE, "boatVirtualAtOpen:%p",rxCallback);
	dalATRxCallBack = rxCallback;
	return BOAT_ERROR;
}

BOAT_RESULT boatVirtualAtSend(char *cmd, BUINT16 len)
{
	if ( dalATRxCallBack != NULL)
	{
		//char *p = "Linux does not support VirtualAt \0d\0aERROR\0d\0a";
		BoatLog(BOAT_LOG_VERBOSE, "\nboatVirtualAtSend:%p:%s\n\n",dalATRxCallBack,vatp);
		(dalATRxCallBack)(vatp,(BUINT32) strlen(vatp));
	}
	return BOAT_ERROR;
}
BOAT_RESULT boatVirtualAtTimedSend(char *cmd, BUINT16 len, BUINT16 timeout)
{
	if ( dalATRxCallBack != NULL)
	{
		//char *p = "\n\nLinux does not support VirtualAt \0d\0aERROR\0d\0a";
		BoatLog(BOAT_LOG_VERBOSE, "\nboatVirtualAtTimedSend:%p:%s\n\n",dalATRxCallBack,vatp);
		(dalATRxCallBack)((char *)vatp, (BUINT32)strlen(vatp));
		BoatLog(BOAT_LOG_VERBOSE, "boatVirtualAtOpen: FINISHED");
	}
	return BOAT_ERROR;
}
BOAT_RESULT boatVirtualAtClose(void)
{
	dalATRxCallBack = NULL;
	return BOAT_ERROR;
}

