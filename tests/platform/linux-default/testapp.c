#include <stdio.h>
#include "bsltestentry.h"
#include "boatlog.h"
typedef int (* testfun)(void);

static void runtest(char *name,testfun p)
{
    if(p == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "=== test Error test function is NULL ===\n");
    return;
    }
    if(p == NULL)
    {
        BoatLog(BOAT_LOG_NORMAL, "=== test Error test name is NULL ===\n");
        return;
    }    
    BoatLog(BOAT_LOG_NORMAL, "=== start run boat %s tests ===\n",name);

    int failed_number = p();

    BoatLog(BOAT_LOG_NORMAL, "boat %s tests run completed,failed number:%d\n",name,failed_number);

    if(failed_number > 0)
    {
        BoatLog(BOAT_LOG_NORMAL, "=== %s test failed === ",name);
        return;
    }
    BoatLog(BOAT_LOG_NORMAL, "=== complete boat %s tests ===\n\n",name);
}


int main(int argc, char *argv[])
{
    BoatLog(BOAT_LOG_NORMAL, "===== linux-default tests =====\n");
    runtest("Queue", runQueueTests);
    runtest("Timer", runTimerTests);
    runtest("Uart", runUartTests);
    runtest("sem", runSemTests);
    runtest("mutex",runMutexTests);
    runtest("Task/Thread", runTaskTests);
    runtest("KeystoreSoft",runKeystoreSoftTests);
    runtest("KeystoreIntf",runKeystoreIntfTests);
    runtest("Keypair",runKeypairTests);
}

