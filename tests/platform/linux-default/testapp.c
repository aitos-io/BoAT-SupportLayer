#include <stdio.h>
#include "bsltestentry.h"

typedef int (* testfun)(void);

static void runtest(char *name,testfun p)
{
    if(p == NULL)
    {
        printf("=== test Error test function is NULL ===\n");
	return;
    }
    if(p == NULL)
    {
        printf("=== test Error test name is NULL ===\n");
	return;
    }	
    printf("=== start run boat %s tests ===\n",name);

    int failed_number = p();

    printf("boat %s tests run completed,failed number:%d\n",name,failed_number);

    printf("=== complete boat %s tests ===\n\n",name);
}


int main(int argc, char *argv[])
{
	printf("===== linux-default tests =====\n");
	//runtest("Queue", runQueueTests);
	//runtest("Timer", runTimerTests);
	//runtest("Uart", runUartTests);
	//runtest("sem", runSemTests);
    //runtest("mutex",runMutexTests);
    runtest("Task/Thread", runTaskTests);
}

