#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "main.h"

bool run_rca_test(Wire32 a, Wire32 b, Wire cin) {
  uint64_t expected_ext = (uint64_t)a + (uint64_t)b + (uint64_t)cin;
  uint32_t expected_sum = (uint32_t)expected_ext;
  Wire expected_cout = expected_ext > UINT32_MAX ? 1 : 0;

  RCAResult result = ripple_carry_adder(a, b, cin);
  bool match = (expected_sum == result.sum) && (expected_cout == result.cout);

  printf("[%s] RCA(a=%u, b=%u, cin=%u)\n", match ? "PASS" : "FAIL", a, b, cin);
  return match;
}

bool run_sbt_test(Wire32 a, Wire32 b) {
  uint32_t expected_diff = (uint32_t)a - (uint32_t)b;
  Wire expected_cout = (a >= b) ? 1 : 0;

  SBTResult result = subtractor(a, b);
  bool match = (expected_diff == result.diff) && (expected_cout == result.cout);

  printf("[%s] SBT(a=%u, b=%u)\n", match ? "PASS" : "FAIL", a, b);
  return match;
}

void run_rca_test_suite() {
  int passed = 0, total = 0;

  #define RUN_RCA_TEST(a, b, cin) do { \
    total++; \
    if (run_rca_test(a, b, cin)) passed++; \
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

  printf("--- RCA SUMMARY: %d / %d PASSED ---\n\n", passed, total);
}

void run_sbt_test_suite() {
  int passed = 0, total = 0;

  #define RUN_SBT_TEST(a, b) do { \
    total++; \
    if (run_sbt_test(a, b)) passed++; \
  } while(0)

  RUN_SBT_TEST(0, 0);
  RUN_SBT_TEST(5, 3);
  RUN_SBT_TEST(3, 5);
  RUN_SBT_TEST(10, 10);
  RUN_SBT_TEST(0xFFFFFFFF, 0);
  RUN_SBT_TEST(0, 0xFFFFFFFF);
  RUN_SBT_TEST(0xFFFFFFFF, 1);
  RUN_SBT_TEST(0x00000001, 0xFFFFFFFF);
  RUN_SBT_TEST(0x12345678, 0x00001234);
  RUN_SBT_TEST(0x55555555, 0xAAAAAAAA);
  RUN_SBT_TEST(0xAAAAAAAA, 0x55555555);
  RUN_SBT_TEST(0x80000000, 0x00000001);
  RUN_SBT_TEST(0x00000000, 0x80000000);
  RUN_SBT_TEST(0xDEADBEEF, 0xCAFEBABE);
  RUN_SBT_TEST(0xCAFEBABE, 0xDEADBEEF);

  #undef RUN_SBT_TEST

  printf("--- SBT SUMMARY: %d / %d PASSED ---\n\n", passed, total);
}

int main(int argc, char *argv[]) {
  bool run_rca = false;
  bool run_sbt = false;
  if (argc < 2) {
    run_rca = true;
    run_sbt = true;
  } else {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-rca") == 0) run_rca = true;
      if (strcmp(argv[i], "-sbt") == 0) run_sbt = true;
    }
  }
  if (run_rca) run_rca_test_suite();
  if (run_sbt) run_sbt_test_suite();
  return 0;
}