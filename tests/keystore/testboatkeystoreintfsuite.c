#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


#include "boatlog.h"
#include "check.h"
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatkeypair.h"
#include "boat_keystore_intf.h"

BOAT_RESULT BoatPort_keyQuery(const BoatKeypairPriKeyCtx_config *config, BoatKeypairPriKeyCtx *pkCtx);
BOAT_RESULT BoatPort_keyDelete(BoatKeypairPriKeyCtx *pkCtx);

START_TEST(test_BoAT_keystore_02intf_test_0010_BoAT_Keystore_Write_Binary_secure_parameter_inputs)
{

	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_02intf_test_0010_BoAT_Keystore_Write_Binary_secure_parameter_inputs\r\n");

	
	rtnVal = BoAT_Keystore_Write_Binary_secure(NULL, 0, 0);

	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_02intf_test_0010_BoAT_Keystore_Write_Binary_secure_parameter_inputs finished\r\n");

}
END_TEST

START_TEST(test_BoAT_keystore_02intf_test_0011_BoAT_Keystore_read_Binary_secure_parameter_inputs)
{

	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_02intf_test_0011_BoAT_Keystore_read_Binary_secure_parameter_inputs\r\n");

	
	rtnVal = BoAT_Keystore_read_Binary_secure(NULL, 0, 0);

	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_02intf_test_0011_BoAT_Keystore_read_Binary_secure_parameter_inputs finished\r\n");

}
END_TEST

START_TEST(test_BoAT_keystore_02intf_test_0012_BoAT_recover_pubkey_parameter_inputs)
{

	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_02intf_test_0012_BoAT_recover_pubkey_parameter_inputs\r\n");
	BUINT8 *pubkey = NULL;
	BUINT8 *digest = NULL;
	const BUINT8 *signature = NULL;
	BUINT8 Prefix = 0;
	
	rtnVal = BoAT_recover_pubkey(BOAT_KEYPAIR_PRIKEY_TYPE_UNKNOWN, BOAT_KEYPAIR_PRIKEY_FORMAT_UNKNOWN, pubkey, digest, signature, Prefix);

	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_02intf_test_0012_BoAT_recover_pubkey_parameter_inputs finished\r\n");

}
END_TEST

START_TEST(test_BoAT_keystore_02intf_test_0013_BoatPort_keyQuery_parameter_inputs)
{

	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_02intf_test_0013_BoatPort_keyQuery_parameter_inputs\r\n");
	
	rtnVal = BoatPort_keyQuery(NULL, NULL);

	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_02intf_test_0013_BoatPort_keyQuery_parameter_inputs finished\r\n");

}
END_TEST

START_TEST(test_BoAT_keystore_02intf_test_0014_BoatPort_keyDelete_parameter_inputs)
{

	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_02intf_test_0014_BoatPort_keyDelete_parameter_inputs\r\n");
	
	rtnVal = BoatPort_keyDelete(NULL);

	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keystore_02intf_test_0014_BoatPort_keyDelete_parameter_inputs finished\r\n");

}
END_TEST



Suite *makeKeystoreIntftest_suite(void)
{
    /* Create Suite */
    Suite *sKeystoreIntftest = suite_create("keystore_soft");

    /* Create test cases */
    TCase *sKeystoreIntftest_api = tcase_create("keystore_soft_api");

	tcase_set_timeout(sKeystoreIntftest_api,100);

    /* Add a test case to the Suite */
    suite_add_tcase(sKeystoreIntftest,sKeystoreIntftest_api);

    /* Test cases are added to the test set */

	tcase_add_test(sKeystoreIntftest_api,test_BoAT_keystore_02intf_test_0010_BoAT_Keystore_Write_Binary_secure_parameter_inputs);
	tcase_add_test(sKeystoreIntftest_api,test_BoAT_keystore_02intf_test_0011_BoAT_Keystore_read_Binary_secure_parameter_inputs);
	tcase_add_test(sKeystoreIntftest_api,test_BoAT_keystore_02intf_test_0012_BoAT_recover_pubkey_parameter_inputs);
	tcase_add_test(sKeystoreIntftest_api,test_BoAT_keystore_02intf_test_0013_BoatPort_keyQuery_parameter_inputs);
	tcase_add_test(sKeystoreIntftest_api,test_BoAT_keystore_02intf_test_0014_BoatPort_keyDelete_parameter_inputs);

    return sKeystoreIntftest;

}
