#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


#include "boatlog.h"
#include "check.h"
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatkeypair.h"
#include "boat_keystore_soft.h"

START_TEST(test_BoAT_keystore_01soft_test_0010_BoAT_recover_pubkey_soft_parameter_inputs)
{

	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_01soft_test_0010_BoAT_recover_pubkey_soft_parameter_inputs\r\n");
	BUINT8 *pubkey = NULL;
	BUINT8 *digest = NULL;
	const BUINT8 *signature = NULL;
	BUINT8 Prefix = 0;
	
	rtnVal = BoAT_recover_pubkey_soft(BOAT_KEYPAIR_PRIKEY_TYPE_UNKNOWN, BOAT_KEYPAIR_PRIKEY_FORMAT_UNKNOWN, pubkey, digest, signature, Prefix);

	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_01soft_test_0010_BoAT_recover_pubkey_soft_parameter_inputs finished\r\n");

}
END_TEST

START_TEST(test_BoAT_keystore_01soft_test_0011_BoAT_DeletePrikeyByIndex_soft_parameter_inputs)
{

	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_01soft_test_0011_BoAT_DeletePrikeyByIndex_soft_parameter_inputs\r\n");

	for(int i = 0; i < (BOAT_MAX_KEYPAIR_NUM  + 2); i++)
	{
		rtnVal = BoAT_DeletePrikeyByIndex_soft(i);
		if (i == 0)
		{
			ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
		}
		else if(i < BOAT_MAX_KEYPAIR_NUM + 1)
		{
			ck_assert_int_eq(rtnVal,BOAT_ERROR_STORAGE_FILE_OPEN_FAIL);
		}
		else
		{
			ck_assert_int_eq(rtnVal,BOAT_ERROR);
		}
	}
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_01soft_test_0011_BoAT_DeletePrikeyByIndex_soft_parameter_inputs finished\r\n");

}
END_TEST

Suite *makeKeystoreSofttest_suite(void)
{
    /* Create Suite */
    Suite *sKeystoreSofttest = suite_create("keystore_soft");

    /* Create test cases */
    TCase *sKeystoreSofttest_api = tcase_create("keystore_soft_api");

	tcase_set_timeout(sKeystoreSofttest_api,100);

    /* Add a test case to the Suite */
    suite_add_tcase(sKeystoreSofttest,sKeystoreSofttest_api);

    /* Test cases are added to the test set */

	tcase_add_test(sKeystoreSofttest_api,test_BoAT_keystore_01soft_test_0010_BoAT_recover_pubkey_soft_parameter_inputs);
	tcase_add_test(sKeystoreSofttest_api,test_BoAT_keystore_01soft_test_0011_BoAT_DeletePrikeyByIndex_soft_parameter_inputs);


    return sKeystoreSofttest;

}
