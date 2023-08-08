#include <stdio.h>
#include "bsltestentry.h"
#include "boatlog.h"

int main(int argc, char *argv[])
{
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
	Suite *suitemakeSsltest = makessltestSuite();
	Suite *suitemakeStoragetest = makestoragetestSuite();

    sr = srunner_create(suitemakeTasktest);
	srunner_add_suite(sr, suitemakeTimertest);
	srunner_add_suite(sr, suitemakeSemtest);
	srunner_add_suite(sr, suitemakeQueuetest);
	srunner_add_suite(sr, suitemakeUarttest);
	srunner_add_suite(sr, sutiemakeMutextest);
	srunner_add_suite(sr, suitemakeKeypairtest);
	srunner_add_suite(sr, suitemakeKeystoreSofttest);
	srunner_add_suite(sr, suitemakeKeystoreIntftest);
	srunner_add_suite(sr, suitemakeSsltest);
	srunner_add_suite(sr, suitemakeStoragetest);

    srunner_run_all(sr,CK_NORMAL);


    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed_number;


}

