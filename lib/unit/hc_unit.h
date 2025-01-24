/*
 * @version 0.1.0
 * @copyright 2025 sun0day 
 * @description Unit test utils for C
 */
#ifndef HC_UNIT_H
#define HC_UNIT_H

#define bool unsigned char
#define true 1
#define false 0
#define fna typeof(__FILE__)
#define lno typeof(__LINE__)

/*
 * Run all test handlers
 */
#define ut_run(...) \
  test_handler handlers[] = { __VA_ARGS__ }; \
  _unit_run(handlers, sizeof(handlers) / sizeof(test_handler));

/*
 * Run a test suite and collect its results
 *
 * @param desc - description of this test suite
 * @param suite_block - code block containing a set of test cases
 */
#define ut_suite(desc, suite_block) { \
  FRT current_frt = _create_file_result(); \
  SRT _internal_srt = _create_suite_result(desc, &current_frt); \
  suite_block \
  current_frt.passed = current_frt.passed & _internal_srt.passed; \
}

/*
 * Run a test case and collect its results
 *
 * @param desc - description of this test case
 * @param case_block - code block containing a set of expected tests 
 */
#define ut_test(desc, case_block) { \
  CRT _internal_crt = _create_case_result(desc, &_internal_srt); \
  case_block \
  _internal_srt.passed = _internal_srt.passed & _internal_crt.passed; \
}

/*
 * Determine if an expression can cast to `true` and collect the result
 *
 * @param expr - arbitrary valid C expression
 * @return ExpectResult
 */
#define ut_truthy(expr) { \
  lno line_no = __LINE__; \
  bool passed = (bool) expr; \
  _internal_crt.expected_results[_internal_crt.expected_len++] = \
    (ERT) { passed, line_no }; \
  _internal_crt.passed = _internal_crt.passed & passed; \
}

/*
 * Determine if an expression can cast to `false` and collect the result
 *
 * @param expr - arbitrary valid C expression
 * @return ExpectResult
 */
#define ut_falsy(expr) { \
  lno line_no = __LINE__; \
  bool passed = !(bool) expr; \
  _internal_crt.expected_results[_internal_crt.expected_len++] = \
    (ERT) { passed, line_no }; \
  _internal_crt.passed = _internal_crt.passed & passed; \
}

/*
 * Determine if two variables are equal via `==` and collect the result
 *
 * @param a - arbitrary variable
 * @param b - arbitrary variable
 * @return ExpectResult
 */
#define ut_equal(a, b) { \
  lno line_no = __LINE__; \
  bool passed = a == b; \
  _internal_crt.expected_results[_internal_crt.expected_len++] = \
    (ERT) { passed, line_no }; \
  _internal_crt.passed = _internal_crt.passed & passed; \
}

/*
 * Test expected result
 *
 * @property line_no - the line number where the case result occurs
 * @property passed - indicate whether the expected result is passed or failed
 */
typedef struct ExpectedResult {
  lno line_no;
  bool passed;
} ERT;

/*
 * Test case result
 *
 * @property expected_results - expected result set from `file_name`
 * @property expected_len - expected result set length
 * @property desc - description of this suite result
 * @property passed - indicate whether the case result is passed or failed
 */
typedef struct CaseResult {
  ERT *expected_results;
  unsigned int expected_len;
  char *desc;
  bool passed;
} CRT;

/*
 * Test suite result
 *
 * @property case_results - case result set from `file_name`
 * @property case_len - case result set length
 * @property desc - description of this suite result
 * @property passed - indicate whether the suite result is passed or failed
 */
typedef struct SuiteResult {
  CRT *case_results;
  unsigned int case_len;
  char *desc;
  bool passed;
} SRT;

/*
 * Test results of a source file
 *
 * @property suite_results - suite result set from `file_name`
 * @property suite_len - suite result set length
 * @property file_name - the file where the suite results come from
 * @property passed - indicate whether the file test result is passed or failed
 */
typedef struct FileResult {
  SRT *suite_results;
  unsigned int suite_len;
  fna file_name;
  bool passed;
} FRT;


/*
 * All test results summary
 *
 * @property file_results - all file test results
 * @property file_len - file test results length
 */
typedef struct TestSummary {
  FRT *file_results;
  unsigned int file_len;
} TSY;

typedef void (*test_handler)();

/*
 * Global variable for collecting test summary 
 */
static TSY TEST_SUMMARY = {(FRT[50]){}, 0};

/*
 * Run test handlers and collect test summary
 * 
 * @param handlers - test handlers defined by user
 * @param len - test handlers' number
 */
void _unit_run(test_handler *handlers, int len) {
  for(int i = 0; i < len; i++) {
    (**handlers)();
    handlers++;
  } 
}

/*
 * Create a file test result
 *
 * @return FileResult
 */
FRT _create_file_result() {
  if(TEST_SUMMARY.file_len == 0 || 
    TEST_SUMMARY.file_results[TEST_SUMMARY.file_len - 1].file_name != __FILE__) {
    TEST_SUMMARY.file_results[TEST_SUMMARY.file_len++] = (FRT){(SRT[50]){}, 0, __FILE__, true};
  } 

  return TEST_SUMMARY.file_results[TEST_SUMMARY.file_len - 1];
}

/*
 * Create a test suite result
 *
 * @param desc - description of created suite result
 * @param current_frt - current testing file result
 * @return SuiteResult
 */
SRT _create_suite_result(char* desc, FRT* current_frt) {
  SRT _internal_srt = {(CRT[20]){}, 0, desc, true}; 
  current_frt->suite_results[current_frt->suite_len++] = _internal_srt; 
  return _internal_srt;
}

/*
 * Create a test case result
 *
 * @param desc - description of created case result
 * @param current_srt - current testing suite result
 * @return CaseResult
 */
CRT _create_case_result(char* desc, SRT* current_srt) {
  CRT _internal_crt = {(ERT[20]){}, 0, desc, true}; 
  current_srt->case_results[current_srt->case_len++] = _internal_crt; 
  return _internal_crt;
}

#endif