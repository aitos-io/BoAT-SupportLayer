#include <stdio.h>
#include <pthread.h>
#include "boatdal.h"
#include "boatlog.h"
#include "boattypes.h"
#include "check.h"
const BCHAR cert[] = "Welcome to the official implementation and documents of BoAT-X Framework";


START_TEST(test_BoAT_DAL_LinuxDefault_01boatssl_test_0010_boat_find_subject_common_name_Failed)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing 01boatssl_test_0010\r\n");
	rtnVal = boat_find_subject_common_name(cert,0,"BoAT-X",6);
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	rtnVal = boat_find_subject_common_name(cert,strlen(cert),"BoAT-X",6);
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	BoatLog(BOAT_LOG_NORMAL, "Testing 01boatssl_test_0010 finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_LinuxDefault_01boatssl_test_0011_BoatSend_Failed)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing 01boatssl_test_0011\r\n");
	rtnVal = BoatSend(0, NULL, NULL, 0, NULL);
	ck_assert_int_eq(rtnVal,BOAT_ERROR);
	BoatLog(BOAT_LOG_NORMAL, "Testing 01boatssl_test_0011 finished\r\n");
}
END_TEST


Suite *makessltestSuite(void)
{
    /* Create Suite */
    Suite *sssltest = suite_create("ssl_test");

    /* Create test cases */
    TCase *tcssltest_api = tcase_create("ssl_test_api");

    /* Add a test case to the Suite */
    suite_add_tcase(sssltest,tcssltest_api);
	tcase_set_timeout(tcssltest_api,20);

    /* Test cases are added to the test set */
	
	tcase_add_test(tcssltest_api, test_BoAT_DAL_LinuxDefault_01boatssl_test_0010_boat_find_subject_common_name_Failed);
	tcase_add_test(tcssltest_api, test_BoAT_DAL_LinuxDefault_01boatssl_test_0011_BoatSend_Failed);
    return sssltest;

}

