#include <stdio.h>
#include <pthread.h>
#include "boatdal.h"
#include "boatlog.h"
#include "boattypes.h"
#include "boatcheck.h"

START_TEST(test_BoAT_DAL_ML307A_01VirtualAt_test_0010_boatVirtualAtOpen_Failed)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_ML307A_01VirtualAt_test_0010_boatVirtualAtOpen_Failed\r\n");
	rtnVal = boatVirtualAtOpen(NULL);
	ck_assert_int_eq(rtnVal,BOAT_ERROR_DAL_INVALID_ARGUMENT);
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_ML307A_01VirtualAt_test_0010_boatVirtualAtOpen_Failed finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_ML307A_01VirtualAt_test_0011_boatVirtualAtSend_Failed)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_ML307A_01VirtualAt_test_0011_boatVirtualAtSend_Failed\r\n");
	rtnVal = boatVirtualAtSend(NULL, 0);
	ck_assert_int_eq(rtnVal,BOAT_ERROR_DAL_INVALID_ARGUMENT);
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_ML307A_01VirtualAt_test_0011_boatVirtualAtSend_Failed finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_ML307A_01VirtualAt_test_0012_boatVirtualAtTimedSend_Failed)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_ML307A_01VirtualAt_test_0012_boatVirtualAtTimedSend_Failed\r\n");
	rtnVal = boatVirtualAtTimedSend(NULL, 0, 0);
	ck_assert_int_eq(rtnVal,BOAT_ERROR_DAL_VIRTUAL_NOT_SUPPORT);
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_ML307A_01VirtualAt_test_0012_boatVirtualAtTimedSend_Failed finished\r\n");

}
END_TEST

START_TEST(test_BoAT_DAL_ML307A_01VirtualAt_test_0013_boatVirtualAtClose_Failed)
{
	BOAT_RESULT rtnVal;
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_ML307A_01VirtualAt_test_0013_boatVirtualAtClose_Failed\r\n");
	rtnVal = boatVirtualAtClose();
	ck_assert_int_eq(rtnVal,BOAT_SUCCESS);
	BoatLog(BOAT_LOG_NORMAL, "Testing test_BoAT_DAL_ML307A_01VirtualAt_test_0013_boatVirtualAtClose_Failed finished\r\n");

}
END_TEST



Suite *makeVirtualAtTestSuite(void)
{
    /* Create Suite */
    Suite *sVirtualAttest = suite_create("virtualAt_test");

    /* Create test cases */
    TCase *tcVirtualAttest_api = tcase_create("virtualAt_test_api");

    /* Add a test case to the Suite */
    suite_add_tcase(sVirtualAttest,tcVirtualAttest_api);
	//tcase_set_timeout(tcVirtualAttest_api,20);

    /* Test cases are added to the test set */
	
#if 1
	tcase_add_test(tcVirtualAttest_api, test_BoAT_DAL_ML307A_01VirtualAt_test_0010_boatVirtualAtOpen_Failed);
	tcase_add_test(tcVirtualAttest_api, test_BoAT_DAL_ML307A_01VirtualAt_test_0011_boatVirtualAtSend_Failed);
	tcase_add_test(tcVirtualAttest_api, test_BoAT_DAL_ML307A_01VirtualAt_test_0012_boatVirtualAtTimedSend_Failed);
	tcase_add_test(tcVirtualAttest_api, test_BoAT_DAL_ML307A_01VirtualAt_test_0013_boatVirtualAtClose_Failed);
#endif
    return sVirtualAttest;

}

int runVirtualAtTests(void)
{
    SRunner *sr = NULL;
    int failed_number = 0;

    Suite *suiteVirtualAtTest = makeVirtualAtTestSuite();

    sr = srunner_create(suiteVirtualAtTest);

    srunner_run_all(sr,0);


    failed_number = srunner_ntests_failed(sr);

    srunner_free(sr);

    return failed_number;
}

