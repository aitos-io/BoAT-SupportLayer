#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>


#include "boatlog.h"
#include "check.h"
#include "boattypes.h"
#include "boaterrcode.h"
#include "boatkeypair.h"




START_TEST(test_BoAT_keypaire_01keypair_test_0011_BoATKeypair_GetKeypairByIndex_Failed_filenotexist)
{

	BOAT_RESULT rtnVal;
	BoatKeypairPriKeyCtx priKeyCtx;
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keypaire_01keypair_test_0011_BoATKeypair_GetKeypairByIndex_Failed_filenotexist\r\n");
	remove( "./boat_wallet_file");
	rtnVal = BoATKeypair_GetKeypairByIndex(&priKeyCtx, 1);

	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keypaire_01keypair_test_0011_BoATKeypair_GetKeypairByIndex_Failed_filenotexist finished\r\n");

}
END_TEST

START_TEST(test_BoAT_keypaire_01keypair_test_0012_BoATKeypair_GetKeypairByIndex_Failed_Ctx_NULL)
{

	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keypaire_01keypair_test_0012_BoATKeypair_GetKeypairByIndex_Failed_Ctx_NULL\r\n");
	rtnVal = BoATKeypair_GetKeypairByIndex(NULL, 1);

	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keypaire_01keypair_test_0012_BoATKeypair_GetKeypairByIndex_Failed_Ctx_NULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_keypaire_01keypair_test_0013_BoATKeypair_GetKeypairByIndex_Failed_index_over_MAX)
{

	BOAT_RESULT rtnVal;
	BoatKeypairPriKeyCtx priKeyCtx;
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keypaire_01keypair_test_0013_BoATKeypair_GetKeypairByIndex_Failed_index_over_MAX\r\n");
	rtnVal = BoATKeypair_GetKeypairByIndex(&priKeyCtx, BOAT_MAX_KEYPAIR_NUM + 1);

	ck_assert_int_eq(rtnVal,BOAT_ERROR_KEYPAIR_KEY_INDEX_EXCEED);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keypaire_01keypair_test_0013_BoATKeypair_GetKeypairByIndex_Failed_index_over_MAX finished\r\n");

}
END_TEST

START_TEST(test_BoAT_keypaire_01keypair_test_0014_BoATKeypair_GetKeypairList_Failed_Ctx_NULL)
{

	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keypaire_01keypair_test_0014_BoATKeypair_GetKeypairList_Failed_Ctx_NULL\r\n");
	rtnVal = BoATKeypair_GetKeypairList(NULL);

	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keypaire_01keypair_test_0014_BoATKeypair_GetKeypairList_Failed_Ctx_NULL finished\r\n");

}
END_TEST

START_TEST(test_BoAT_keypaire_01keypair_test_0015_BoATKeypair_GetKeypairList_Failed_filenoexist)
{

	BOAT_RESULT rtnVal;
	BoatIotKeypairContext keypairList;
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keypaire_01keypair_test_0015_BoATKeypair_GetKeypairList_Failed_filenoexist\r\n");
	remove( "./boat_wallet_file");
	rtnVal = BoATKeypair_GetKeypairList(&keypairList);

	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	
	BoatLog(BOAT_LOG_NORMAL, "[boat][keystroe soft] Testing test_BoAT_keypaire_01keypair_test_0015_BoATKeypair_GetKeypairList_Failed_filenoexist finished\r\n");

}
END_TEST

Suite *makeKeypairtest_suite(void)
{
    /* Create Suite */
    Suite *sKeypairtest = suite_create("keypair");

    /* Create test cases */
    TCase *sKeypairtest_api = tcase_create("keypair_api");

	tcase_set_timeout(sKeypairtest_api,100);

    /* Add a test case to the Suite */
    suite_add_tcase(sKeypairtest,sKeypairtest_api);

    /* Test cases are added to the test set */

	tcase_add_test(sKeypairtest_api,test_BoAT_keypaire_01keypair_test_0011_BoATKeypair_GetKeypairByIndex_Failed_filenotexist);
	tcase_add_test(sKeypairtest_api,test_BoAT_keypaire_01keypair_test_0012_BoATKeypair_GetKeypairByIndex_Failed_Ctx_NULL);
	tcase_add_test(sKeypairtest_api,test_BoAT_keypaire_01keypair_test_0013_BoATKeypair_GetKeypairByIndex_Failed_index_over_MAX);
	tcase_add_test(sKeypairtest_api,test_BoAT_keypaire_01keypair_test_0014_BoATKeypair_GetKeypairList_Failed_Ctx_NULL);
	tcase_add_test(sKeypairtest_api,test_BoAT_keypaire_01keypair_test_0015_BoATKeypair_GetKeypairList_Failed_filenoexist);

    return sKeypairtest;

}
