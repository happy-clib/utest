#include "./hc_unit.h"

void test_unit() {
  ut_suite("test ut_truthy", {
    ut_test("should work", {
      ut_truthy(1);
      ut_truthy(!0);
      ut_truthy("");
    })
  })

  ut_suite("test ut_falsy", {
    ut_test("should work", {
      ut_falsy(!1);
      ut_falsy(0);
      ut_falsy(!"");
    })
  })

  ut_suite("test ut_equal", {
    ut_test("should work", {
      ut_equal(1, 1);
      ut_equal(0, 0);
    })
  })
}

