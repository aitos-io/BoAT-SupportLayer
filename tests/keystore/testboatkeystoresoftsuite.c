#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


#include "boatlog.h"
#include "check.h"
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatkeypair.h"


#if 1
START_TEST(test_BoAT_keystore_01soft_test_0010_BoAT_recover_pubkey_soft_parameter_inputs)
{

	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_keystore_01soft_test_0010_BoAT_recover_pubkey_soft_parameter_inputs\r\n");
	BUINT8 *pubkey = NULL;
	BUINT8 *digest = NULL;
	const BUINT8 *signature = NULL;
	BUINT8 Prefix = 0;
	
	rtnVal = BoAT_recover_pubkey_soft(BOAT_KEYPAIR_PRIKEY_TYPE_UNKNOWN, BOAT_KEYPAIR_PRIKEY_FORMAT_UNKNOWN, pubkey, digest, signature, Prefix);

	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][sem] Testing test_BoAT_keystore_01soft_test_0010_BoAT_recover_pubkey_soft_parameter_inputs finished\r\n");

}
END_TEST




Suite *makeKeystoreSofttest_suite(void)
{
    /* Create Suite */
    Suite *sKeystorSofttest = suite_create("keystore_soft");

    /* Create test cases */
    TCase *sKeystorSofttest_api = tcase_create("keystore_soft_api");

	tcase_set_timeout(sKeystorSofttest_api,100);

    /* Add a test case to the Suite */
    suite_add_tcase(sKeystorSofttest,sKeystorSofttest_api);

    /* Test cases are added to the test set */

	tcase_add_test(sKeystorSofttest_api,test_BoAT_keystore_01soft_test_0010_BoAT_recover_pubkey_soft_parameter_inputs);


    return sKeystorSofttest;

}
#endif

int runKeystoreSoftTests(void)
{

#if 1
    SRunner *sr = NULL;
    int failed_number = 0;

    Suite *suiteKeystoreSofttest = makeKeystoreSofttest_suite();

    sr = srunner_create(suiteKeystoreSofttest);

    srunner_run_all(sr,0);


    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed_number;
#endif

	
}

