#include <stdio.h>
#include <pthread.h>
#include "boatdal.h"
#include "boatlog.h"
#include "boattypes.h"
#include "boatstorage.h"
#include "boatkeypair.h"
#include "check.h"

__BOATSTATIC BOAT_RESULT createKeypair(BCHAR *keypairName)
{
    BOAT_RESULT result = BOAT_SUCCESS;
    BoatKeypairPriKeyCtx_config keypair_config = {0};

	/* wallet_config value assignment */
        keypair_config.prikey_genMode = BOAT_KEYPAIR_PRIKEY_GENMODE_INTERNAL_GENERATION;
        keypair_config.prikey_type    = BOAT_KEYPAIR_PRIKEY_TYPE_SECP256K1;
	
	/* create venachain keypair */
    result = BoatKeypairCreate(&keypair_config, keypairName,BOAT_STORE_TYPE_FLASH);
    if (result < 0)
	{
        return BOAT_ERROR_WALLET_CREATE_FAIL;
    }
    return BOAT_SUCCESS;
}


START_TEST(test_BoAT_DAL_LinuxDefault_01boatstorage_test_0010_BoatGetStorageSize)
{
	BOAT_RESULT rtnVal;
	BUINT32 size;
	BoatLog(BOAT_LOG_NORMAL, "\r\nTesting 01boatstorage_test_0010\r\n");
	rtnVal = BoatGetStorageSize(NULL,NULL);
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	rtnVal = BoatGetStorageSize(&size,NULL);
	ck_assert_int_eq(rtnVal,BOAT_ERROR_STORAGE_FILE_OPEN_FAIL);
	rtnVal = createKeypair("key0011");
	rtnVal = BoatGetStorageSize(&size,NULL);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	BoatLog(BOAT_LOG_NORMAL, "Testing 01boatstorage_test_0010 finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_LinuxDefault_01boatstorage_test_0011_BoatRemoveFile)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "\r\nTesting 01boatstorage_test_0011\r\n");

	rtnVal = BoatRemoveFile(NULL,NULL);
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	rtnVal = BoatRemoveFile(BOAT_FILE_STOREDATA,NULL);
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	rtnVal = BoatRemoveFile(BOAT_FILE_STOREDATA,NULL);
	ck_assert_int_eq(rtnVal,BOAT_ERROR_STORAGE_FILE_REMOVE_FAIL);
	BoatLog(BOAT_LOG_NORMAL, "Testing 01boatstorage_test_0011 finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_LinuxDefault_01boatstorage_test_0012_BoatWriteStorageFail)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "\r\nTesting 01boatstorage_test_0012\r\n");

	rtnVal = BoatWriteStorage(0, NULL, 0, NULL);
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	BoatLog(BOAT_LOG_NORMAL, "Testing 01boatstorage_test_0012 finished\r\n");
}
END_TEST

START_TEST(test_BoAT_DAL_LinuxDefault_01boatstorage_test_0013_BoatReadStorageFail)
{
	BOAT_RESULT rtnVal;
	BUINT8 rdbuf[1024];
	BoatLog(BOAT_LOG_NORMAL, "\r\nTesting 01boatstorage_test_0013\r\n");

	rtnVal = BoatReadStorage(0, NULL, 0, NULL);
	ck_assert_int_eq(rtnVal,BOAT_ERROR_COMMON_INVALID_ARGUMENT);
	rtnVal = BoatReadStorage(20, rdbuf, 1024, NULL);
	ck_assert_int_eq(rtnVal,BOAT_ERROR_STORAGE_FILE_READ_FAIL);
	BoatLog(BOAT_LOG_NORMAL, "Testing 01boatstorage_test_0013 finished\r\n");
}
END_TEST

Suite *makestoragetestSuite(void)
{
    /* Create Suite */
    Suite *sstoragetest = suite_create("storage_test");

    /* Create test cases */
    TCase *tcstoragetest_api = tcase_create("storage_test_api");

    /* Add a test case to the Suite */
    suite_add_tcase(sstoragetest,tcstoragetest_api);
	tcase_set_timeout(tcstoragetest_api,20);

    /* Test cases are added to the test set */
	
	tcase_add_test(tcstoragetest_api, test_BoAT_DAL_LinuxDefault_01boatstorage_test_0010_BoatGetStorageSize);
	tcase_add_test(tcstoragetest_api, test_BoAT_DAL_LinuxDefault_01boatstorage_test_0012_BoatWriteStorageFail);
	tcase_add_test(tcstoragetest_api, test_BoAT_DAL_LinuxDefault_01boatstorage_test_0013_BoatReadStorageFail);
	tcase_add_test(tcstoragetest_api, test_BoAT_DAL_LinuxDefault_01boatstorage_test_0011_BoatRemoveFile);
    return sstoragetest;

}


