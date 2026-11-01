#include <stdio.h>
#include "main.h"

bool run_rca_test(Wire32 a, Wire32 b, Wire cin) {
  printf("======= TESTING RIPPLE-CARRY ADDER (%u + %u + Cin:%u) =======\n", a, b, cin);
  uint32_t expected_sum = a + b + cin;
  uint64_t expected_sum_extended = (uint64_t)a + (uint64_t)b + (uint64_t)cin;
  Wire expected_cout = expected_sum_extended > UINT32_MAX ? 1 : 0;
  printf("Expected sum: %u\n", expected_sum);
  printf("Expected carry out: %u\n", expected_cout);

  RCAResult result = ripple_carry_adder(a, b, cin);
  printf("Sum: %u\n", result.sum);
  printf("Carry out: %u\n", result.cout);

  bool match = (expected_sum == result.sum) && (expected_cout == result.cout);
  printf("Match: %s\n", match ? "YES" : "NO");
  printf("\n");
  return match;
}

bool run_sbt_test(uint32_t a, uint32_t b) {
  printf("======= TESTING SUBTRACTOR (%u - %u) =======\n", a, b);
  uint32_t expected_diff = a - b;
  Wire expected_cout = (a >= b) ? 1 : 0;
  printf("Expected diff: %u\n", expected_diff);
  printf("Expected carry out: %u\n", expected_cout);

  SBTResult result = subtractor(a, b);
  printf("Diff: %u\n", result.diff);
  printf("Carry out: %u\n", result.cout);

  bool match = (expected_diff == result.diff) && (expected_cout == result.cout);
  printf("Match: %s\n", match ? "YES" : "NO");
  printf("\n");
  return match;
}

void run_rca_test_suite() {
  int passed = 0;
  int total = 0;

  #define RUN_RCA_TEST(a, b, cin) do { \
    total++; \
    if (run_rca_test(a, b, cin)) { \
      passed++; \
    } \
  } while(0)

  RUN_RCA_TEST(0, 0, 0);
  RUN_RCA_TEST(0, 0, 1);
  RUN_RCA_TEST(1, 1, 0);
  RUN_RCA_TEST(5, 10, 1);
  RUN_RCA_TEST(12323, 622331, 0);
  RUN_RCA_TEST(0xFFFFFFFF, 0, 0);
  RUN_RCA_TEST(0xFFFFFFFF, 1, 0); 
  RUN_RCA_TEST(0xFFFFFFFF, 0, 1);
  RUN_RCA_TEST(0xFFFFFFFF, 0xFFFFFFFF, 1);
  RUN_RCA_TEST(0x12345678, 0x57654321, 1);
  RUN_RCA_TEST(0x55555555, 0xAAAAAAAA, 0); 
  RUN_RCA_TEST(0x55555555, 0xAAAAAAAA, 1);
  RUN_RCA_TEST(0x80000000, 0x80000000, 0);
  RUN_RCA_TEST(0x7FFFFFFF, 0x00000001, 0);
  RUN_RCA_TEST(0xDEADBEEF, 0xCAFEBABE, 0);
  RUN_RCA_TEST(0xDEADBEEF, 0xCAFEBABE, 1);

  #undef RUN_RCA_TEST

  printf("==================================================\n");
  printf("TEST SUITE SUMMARY: %d / %d TESTS PASSED\n", passed, total);
  printf("==================================================\n");
}

void run_sbt_test_suite() {
  int passed = 0;
  int total = 0;

  #define RUN_SBT_TEST(a, b) do { \
    total++; \
    if (run_sbt_test(a, b)) { \
      passed++; \
    } \
  } while(0)

  RUN_SBT_TEST(0, 0);
  RUN_SBT_TEST(5, 3);
  RUN_SBT_TEST(3, 5);          // Tests borrow
  RUN_SBT_TEST(10, 10);
  RUN_SBT_TEST(0xFFFFFFFF, 0);
  RUN_SBT_TEST(0, 0xFFFFFFFF); // Tests borrow
  RUN_SBT_TEST(0xFFFFFFFF, 1);
  RUN_SBT_TEST(0x00000001, 0xFFFFFFFF); // Tests borrow
  RUN_SBT_TEST(0x12345678, 0x00001234);
  RUN_SBT_TEST(0x55555555, 0xAAAAAAAA);
  RUN_SBT_TEST(0xAAAAAAAA, 0x55555555);
  RUN_SBT_TEST(0x80000000, 0x00000001);
  RUN_SBT_TEST(0x00000000, 0x80000000);
  RUN_SBT_TEST(0xDEADBEEF, 0xCAFEBABE);
  RUN_SBT_TEST(0xCAFEBABE, 0xDEADBEEF);

  #undef RUN_SBT_TEST

  printf("==================================================\n");
  printf("TEST SUITE SUMMARY: %d / %d TESTS PASSED\n", passed, total);
  printf("==================================================\n");
}


int main() {
  run_rca_test_suite();
  // run_sbt_test_suite();
  return 0;
}