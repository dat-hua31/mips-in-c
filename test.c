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

bool run_shift_left_test(Wire32 x, Wire shamt0, Wire shamt1, Wire shamt2, Wire shamt3, Wire shamt4) {
  uint32_t shift_amt = (uint32_t)shamt0 | 
                       ((uint32_t)shamt1 << 1) | 
                       ((uint32_t)shamt2 << 2) | 
                       ((uint32_t)shamt3 << 3) | 
                       ((uint32_t)shamt4 << 4);
  
  uint32_t expected = (uint32_t)x << shift_amt;

  Wire32 result = shift_left32(x, shamt0, shamt1, shamt2, shamt3, shamt4);
  bool match = (expected == (uint32_t)result);

  printf("[%s] shift_left32(x=0x%08X, shamt=%2u) [bits: %u %u %u %u %u] -> result=0x%08X, expected=0x%08X\n", 
         match ? "PASS" : "FAIL", (uint32_t)x, shift_amt, shamt4, shamt3, shamt2, shamt1, shamt0, (uint32_t)result, expected);
  return match;
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

void run_shift_left_test_suite() {
  int passed = 0, total = 0;

  #define RUN_SHIFT_TEST(x, s4, s3, s2, s1, s0) do { \
    total++; \
    if (run_shift_left_test(x, s0, s1, s2, s3, s4)) passed++; \
  } while(0)

  RUN_SHIFT_TEST(0x12345678, 0, 0, 0, 0, 0);
  RUN_SHIFT_TEST(0xFFFFFFFF, 0, 0, 0, 0, 0);
  RUN_SHIFT_TEST(0x00000001, 0, 0, 0, 0, 1);
  RUN_SHIFT_TEST(0x00000001, 0, 0, 0, 1, 0);
  RUN_SHIFT_TEST(0x00000001, 0, 0, 1, 0, 0);
  RUN_SHIFT_TEST(0x00000001, 0, 1, 0, 0, 0);
  RUN_SHIFT_TEST(0x00000001, 1, 0, 0, 0, 0);
  RUN_SHIFT_TEST(0x00000001, 0, 0, 0, 1, 1);
  RUN_SHIFT_TEST(0x00000001, 0, 0, 1, 1, 1);
  RUN_SHIFT_TEST(0x00000001, 0, 1, 1, 1, 1);
  RUN_SHIFT_TEST(0x00000001, 1, 1, 1, 1, 1);
  RUN_SHIFT_TEST(0xDEADBEEF, 0, 0, 0, 1, 0);
  RUN_SHIFT_TEST(0x55555555, 0, 0, 1, 0, 0);
  RUN_SHIFT_TEST(0x80000000, 0, 0, 0, 0, 1);
  RUN_SHIFT_TEST(0x0000FFFF, 0, 1, 0, 0, 0);

  #undef RUN_SHIFT_TEST

  printf("--- SHIFT LEFT SUMMARY: %d / %d PASSED ---\n\n", passed, total);
}

int main(int argc, char *argv[]) {
  bool run_rca = false;
  bool run_sbt = false;
  bool run_shift = false;

  if (argc < 2) {
    run_rca = true;
    run_sbt = true;
    run_shift = true;
  } else {
    for (int i = 1; i < argc; i++) {
      if (strcmp(argv[i], "-rca") == 0) run_rca = true;
      if (strcmp(argv[i], "-sbt") == 0) run_sbt = true;
      if (strcmp(argv[i], "-shift") == 0) run_shift = true;
    }
  }

  if (run_rca) run_rca_test_suite();
  if (run_sbt) run_sbt_test_suite();
  if (run_shift) run_shift_left_test_suite();

  return 0;
}