#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "cunit_fann.h"

void AddTests(void);

// Suite cleanup functions
static int suite_success_init(void) { return 0; }
static int suite_success_clean(void) { return 0; }

/* All tests which will be run. */
static CU_TestInfo suite_fann_basic[] = {
  { "simple_test", cunit_simple_test },
  { "simple_train", cunit_simple_train },
  { "steepness_train", cunit_steepness_train },
  { "xor_test", cunit_xor_test },
  { "xor_train", cunit_xor_train },
	CU_TEST_INFO_NULL,
};


/* All suites which will be run. */
static CU_SuiteInfo suites[] = {
  { "suite_fann_basic",  suite_success_init, suite_success_clean, suite_fann_basic },
	CU_SUITE_INFO_NULL,
};


int main(void)
{
	if (CU_initialize_registry()) 
	{
		printf("\nInitialization of Test Registry failed.");
	}
	else 
	{
		AddTests();

		/* also, NORMAL and SILENT */
		CU_basic_set_mode(CU_BRM_VERBOSE);
		
		/* also, FAIL and ABORT */
		CU_set_error_action(CUEA_IGNORE);

		/*CU_set_output_filename("TestAutomated");
		CU_list_tests_to_file();
		CU_automated_run_tests();*/

		printf("\nTests completed with return value %d.\n", CU_basic_run_tests());
		CU_cleanup_registry();
	}

	return 0;
}


void AddTests(void)
{
	assert(NULL != CU_get_registry());
	assert(!CU_is_test_running());

	/* Register suites. */
	if (CU_register_suites(suites) != CUE_SUCCESS) {
		fprintf(stderr, "suite registration failed - %s\n", CU_get_error_msg());
		exit(EXIT_FAILURE);
	}
}
