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
#if 0
    BoatLog(BOAT_LOG_NORMAL, "===== linux-default tests =====\n");
    //runtest("Queue", runQueueTests);
    //runtest("Timer", runTimerTests);
    //runtest("Uart", runUartTests);
    //runtest("sem", runSemTests);
    //runtest("mutex",runMutexTests);
    //runtest("Task/Thread", runTaskTests);
    //runtest("KeystoreSoft",runKeystoreSoftTests);
    //runtest("KeystoreIntf",runKeystoreIntfTests);
    //runtest("Keypair",runKeypairTests);
#endif
    SRunner *sr = NULL;
    int failed_number = 0;

	//Suite *suitemakeI2ctest = makeI2ctest_suite();
	//Suite *suitemake_bleService_test = make_bleService_test_suite();
	//Suite *suitemakeVirtualAtTest = makeVirtualAtTestSuite();
	Suite *suitemakeTasktest = makeTasktest_suite();
	Suite *suitemakeTimertest = makeTimertest_suite();
	Suite *suitemakeSemtest = makeSemtest_suite();
	Suite *suitemakeQueuetest = makeQueuetest_suite();
	Suite *suitemakeUarttest = makeUarttest_suite();
	Suite *sutiemakeMutextest = makeMutextest_suite();
	Suite *suitemakeKeypairtest = makeKeypairtest_suite();
	Suite *suitemakeKeystoreSofttest = makeKeystoreSofttest_suite();
	Suite *suitemakeKeystoreIntftest = makeKeystoreIntftest_suite();

    sr = srunner_create(suitemakeTasktest);
	srunner_add_suite(sr, suitemakeTimertest);
	srunner_add_suite(sr, suitemakeSemtest);
	srunner_add_suite(sr, suitemakeQueuetest);
	srunner_add_suite(sr, suitemakeUarttest);
	srunner_add_suite(sr, sutiemakeMutextest);
	srunner_add_suite(sr, suitemakeKeypairtest);
	srunner_add_suite(sr, suitemakeKeystoreSofttest);
	srunner_add_suite(sr, suitemakeKeystoreIntftest);

    srunner_run_all(sr,CK_NORMAL);


    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed_number;


}

