////////////////////////////////////////////////////////////
//
// SECTION 0: TYPE CONVENTIONS
//
// Software C features are used here for topological mapping.
//
////////////////////////////////////////////////////////////

// Wire (0 - 1 signal)
#include <stdbool.h>
typedef bool Wire;


// Wire32 (32-bit bus)
#include <stdint.h>
typedef uint32_t Wire32;


// Getting a wire out of 32 wires
Wire _wire32_get(Wire32 w, int i) {
  return w >> i & 1;
}


// Setting a wire out of 32 wires
void _wire32_set(Wire32* w, int i, Wire val) {
  uint32_t clear_mask = ~(1U << i);
  uint32_t bit_val = ((uint32_t)val) << i;
  *w = (*w & clear_mask) | bit_val;
}


// Splitting a wire into 32 wires
Wire32 _broadcast(Wire w) {
  return -(Wire32)w;
}

////////////////////////////////////////////////////////////
//
// SECTION 1: PRIMITIVE & COMBINATIONAL FOUNDATION
//
////////////////////////////////////////////////////////////

// NAND Gate: Where we start
Wire nand_gate(Wire a, Wire b) {
  return !(a & b);
}
Wire32 nand_gate32(Wire32 a, Wire32 b) {
  return ~(a & b);
}


// NOT Gate
Wire not_gate(Wire a) {
  return nand_gate(a, a);
}
Wire32 not_gate32(Wire32 a) {
  return nand_gate32(a, a);
}


// AND Gate
Wire and_gate(Wire a, Wire b) {
  return not_gate(nand_gate(a, b));
}
Wire32 and_gate32(Wire32 a, Wire32 b) {
  return not_gate32(nand_gate32(a, b));
}


// OR Gate
Wire or_gate(Wire a, Wire b) {
  return nand_gate(not_gate(a), not_gate(b));
}
Wire32 or_gate32(Wire32 a, Wire32 b) {
  return nand_gate32(not_gate32(a), not_gate32(b));
}


// XOR Gate
Wire xor_gate(Wire a, Wire b) {
  return or_gate(
           and_gate(a, not_gate(b)),
           and_gate(not_gate(a), b));
}
Wire32 xor_gate32(Wire32 a, Wire32 b) {
  return or_gate32(
           and_gate32(a, not_gate32(b)),
           and_gate32(not_gate32(a), b));
}


// XNOR Gate
Wire xnor_gate(Wire a, Wire b){
  return not_gate(xor_gate(a, b));
}
Wire32 xnor_gate32(Wire32 a, Wire32 b){
  return not_gate32(xor_gate32(a, b));
}


// 2-to-1 MUX
Wire mux2(Wire d0, Wire d1, Wire s) {
  return or_gate(
           and_gate(d0, not_gate(s)),
           and_gate(d1, s));
}
Wire32 mux2_32(Wire32 d0, Wire32 d1, Wire s) {
  Wire32 s_broadcast = _broadcast(s);
  return or_gate32(
           and_gate32(d0, not_gate32(s_broadcast)),
           and_gate32(d1, s_broadcast));
}


// 4-to-1 MUX
Wire mux4(Wire d0, Wire d1, Wire d2, Wire d3, Wire s0, Wire s1) {
  return mux2(mux2(d0, d1, s0), mux2(d2, d3, s0), s1);
}
Wire32 mux4_32(Wire32 d0, Wire32 d1, Wire32 d2, Wire32 d3, Wire s0, Wire s1) {
  return mux2_32(mux2_32(d0, d1, s0), mux2_32(d2, d3, s0), s1);
}
Wire32 mux16_32(Wire32 d0, Wire32 d1, Wire32 d2, Wire32 d3,
                Wire32 d4, Wire32 d5, Wire32 d6, Wire32 d7,
                Wire32 d8, Wire32 d9, Wire32 d10, Wire32 d11,
                Wire32 d12, Wire32 d13, Wire32 d14, Wire32 d15,
                Wire s0, Wire s1, Wire s2, Wire s3)
{
  Wire32 b0 = mux4_32(d0,  d1,  d2,  d3,  s0, s1);
  Wire32 b1 = mux4_32(d4,  d5,  d6,  d7,  s0, s1);
  Wire32 b2 = mux4_32(d8,  d9,  d10, d11, s0, s1);
  Wire32 b3 = mux4_32(d12, d13, d14, d15, s0, s1);
  return mux4_32(b0, b1, b2, b3, s2, s3);
}


// Full Adder
typedef struct {
  Wire sum;
  Wire cout;
} FAResult;

FAResult full_adder(Wire a, Wire b, Wire cin) {
  Wire a_xor_b = xor_gate(a, b);
  Wire sum = xor_gate(cin, a_xor_b);
  Wire cout = or_gate(and_gate(a, b), and_gate(cin, a_xor_b));
  return (FAResult){sum, cout};
}

////////////////////////////////////////////////////////////
//
// SECTION 2: 32-BIT ALU & ARITHMETIC ENGINE
//
////////////////////////////////////////////////////////////

// 32-Bit Ripple-Carry Adder
typedef struct {
  Wire32 sum;
  Wire cout;
} RCAResult;

RCAResult ripple_carry_adder(Wire32 a, Wire32 b, Wire cin) {
  Wire32 sum;
  Wire carry = cin;
  for (int i = 0; i < 32; i++) {
    Wire bit_a = _wire32_get(a, i);
    Wire bit_b = _wire32_get(b, i);
    FAResult fa = full_adder(bit_a, bit_b, carry);
    _wire32_set(&sum, i, fa.sum);
    carry = fa.cout;
  }
  return (RCAResult){sum, carry};
}


// 32-Bit Subtractor 
typedef struct {
  Wire32 diff;
  Wire cout;
} SBTResult;

SBTResult subtractor(Wire32 a, Wire32 b) {
  Wire32 inv_b = not_gate32(b);
  RCAResult rca = ripple_carry_adder(a, inv_b, 1);
  return (SBTResult){rca.sum, rca.cout};
}


// ALU 


////////////////////////////////////////////////////////////
//
// TESTING
//
////////////////////////////////////////////////////////////

#include <stdio.h>

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

void run_sbt_test_suite() {
  int passed = 0;
  int total = 0;

  #define RUN_SBT_TEST(a, b) do { \
    total++; \
    if (run_sbt_test(a, b)) { \
      passed++; \
    } \
  } while(0)

  // Basic cases
  RUN_SBT_TEST(0, 0);
  RUN_SBT_TEST(5, 3);
  RUN_SBT_TEST(3, 5);          // Tests borrow (cout = 0)
  RUN_SBT_TEST(10, 10);

  // Boundary cases
  RUN_SBT_TEST(0xFFFFFFFF, 0);
  RUN_SBT_TEST(0, 0xFFFFFFFF); // Tests borrow
  RUN_SBT_TEST(0xFFFFFFFF, 1);
  RUN_SBT_TEST(0x00000001, 0xFFFFFFFF); // Tests borrow

  // Bit pattern & Hex edge cases
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
  //run_rca_test_suite();
  //run_sbt_test_suite();
  return 0;
}
