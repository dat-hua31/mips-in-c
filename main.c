////////////////////////////////////////////////////////////
//
// SECTION 0: TYPE CONVENTIONS
//
////////////////////////////////////////////////////////////

#include <stdbool.h>
typedef bool Wire;

#include <stdint.h>
typedef union {
  uint32_t value;
  struct {
    Wire b0 :1, b1:1,  b2:1,  b3:1,  b4:1,  b5:1,  b6:1,  b7:1;
    Wire b8 :1, b9:1,  b10:1, b11:1, b12:1, b13:1, b14:1, b15:1;
    Wire b16:1, b17:1, b18:1, b19:1, b20:1, b21:1, b22:1, b23:1;
    Wire b24:1, b25:1, b26:1, b27:1, b28:1, b29:1, b30:1, b31:1;
  } bits;
} Wire32;

Wire wire32_get(Wire32 w, int i) {
  switch (i) {
    case 0:  return w.bits.b0;  case 1:  return w.bits.b1;
    case 2:  return w.bits.b2;  case 3:  return w.bits.b3;
    case 4:  return w.bits.b4;  case 5:  return w.bits.b5;
    case 6:  return w.bits.b6;  case 7:  return w.bits.b7;
    case 8:  return w.bits.b8;  case 9:  return w.bits.b9;
    case 10: return w.bits.b10; case 11: return w.bits.b11;
    case 12: return w.bits.b12; case 13: return w.bits.b13;
    case 14: return w.bits.b14; case 15: return w.bits.b15;
    case 16: return w.bits.b16; case 17: return w.bits.b17;
    case 18: return w.bits.b18; case 19: return w.bits.b19;
    case 20: return w.bits.b20; case 21: return w.bits.b21;
    case 22: return w.bits.b22; case 23: return w.bits.b23;
    case 24: return w.bits.b24; case 25: return w.bits.b25;
    case 26: return w.bits.b26; case 27: return w.bits.b27;
    case 28: return w.bits.b28; case 29: return w.bits.b29;
    case 30: return w.bits.b30; case 31: return w.bits.b31;
    default: return 0;
  }
}

void wire32_set(Wire32* w, int i, Wire val) {
  if (!w) return;
  switch (i) {
    case 0:  w->bits.b0 = val;  break; case 1: w->bits.b1 = val;   break;
    case 2:  w->bits.b2 = val;  break; case 3: w->bits.b3 = val;   break;
    case 4:  w->bits.b4 = val;  break; case 5: w->bits.b5 = val;   break;
    case 6:  w->bits.b6 = val;  break; case 7: w->bits.b7 = val;   break;
    case 8:  w->bits.b8 = val;  break; case 9: w->bits.b9 = val;   break;
    case 10: w->bits.b10 = val; break; case 11: w->bits.b11 = val; break;
    case 12: w->bits.b12 = val; break; case 13: w->bits.b13 = val; break;
    case 14: w->bits.b14 = val; break; case 15: w->bits.b15 = val; break;
    case 16: w->bits.b16 = val; break; case 17: w->bits.b17 = val; break;
    case 18: w->bits.b18 = val; break; case 19: w->bits.b19 = val; break;
    case 20: w->bits.b20 = val; break; case 21: w->bits.b21 = val; break;
    case 22: w->bits.b22 = val; break; case 23: w->bits.b23 = val; break;
    case 24: w->bits.b24 = val; break; case 25: w->bits.b25 = val; break;
    case 26: w->bits.b26 = val; break; case 27: w->bits.b27 = val; break;
    case 28: w->bits.b28 = val; break; case 29: w->bits.b29 = val; break;
    case 30: w->bits.b30 = val; break; case 31: w->bits.b31 = val; break;
  }
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
  return (Wire32){ .value = ~(a.value & b.value) };
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


// 4-to-1 MUX
Wire mux4(Wire d0, Wire d1, Wire d2, Wire d3, Wire s0, Wire s1) {
  Wire p0 = and_gate(d0, and_gate(not_gate(s1), not_gate(s0)));
  Wire p1 = and_gate(d1, and_gate(not_gate(s1), s0));
  Wire p2 = and_gate(d2, and_gate(s1, not_gate(s0)));
  Wire p3 = and_gate(d3, and_gate(s1, s0));
  return or_gate(or_gate(p0, p1), or_gate(p2, p3));
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
    Wire bit_a = wire32_get(a, i);
    Wire bit_b = wire32_get(b, i);
    FAResult fa = full_adder(bit_a, bit_b, carry);
    wire32_set(&sum, i, fa.sum);
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

bool run_rca_test(uint32_t a, uint32_t b, Wire cin) {
  printf("======= TESTING RIPPLE-CARRY ADDER (%u + %u + Cin:%u) =======\n", a, b, cin);
  uint32_t expected_sum = a + b + cin;
  uint64_t expected_sum_extended = (uint64_t)a + (uint64_t)b + (uint64_t)cin;
  Wire expected_cout = expected_sum_extended > UINT32_MAX ? 1 : 0;
  printf("Expected sum: %u\n", expected_sum);
  printf("Expected carry out: %u\n", expected_cout);

  Wire32 wire32_a = { .value = a };
  Wire32 wire32_b = { .value = b };
  RCAResult result = ripple_carry_adder(wire32_a, wire32_b, cin);
  printf("Sum: %u\n", result.sum.value);
  printf("Carry out: %u\n", result.cout);

  bool match = (expected_sum == result.sum.value) && (expected_cout == result.cout);
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

  Wire32 wire32_a = { .value = a };
  Wire32 wire32_b = { .value = b };
  SBTResult result = subtractor(wire32_a, wire32_b);
  printf("Diff: %u\n", result.diff.value);
  printf("Carry out: %u\n", result.cout);

  bool match = (expected_diff == result.diff.value) && (expected_cout == result.cout);
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
  // run_rca_test_suite();
  // run_sbt_test_suite();
  return 0;
}
