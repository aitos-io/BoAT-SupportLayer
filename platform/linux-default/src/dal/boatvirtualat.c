#include "boattypes.h"
#include "boaterrcode.h"
#include "boatdal.h"


BOAT_RESULT boatVirtualAtOpen(boatVirtualAtRxCallback rxCallback)
{
	return BOAT_ERROR;
}
BOAT_RESULT boatVirtualAtSend(char *cmd, BUINT16 len)
{
	return BOAT_ERROR;
}
BOAT_RESULT boatVirtualAtTimedSend(char *cmd, BUINT16 len, BUINT16 timeout)
{
	return BOAT_ERROR;
}
BOAT_RESULT boatVirtualAtClose(void)
{
	return BOAT_ERROR;
}

