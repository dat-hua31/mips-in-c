////////////////////////////////////////////////////////////
//
// SECTION 1: PRIMITIVE & COMBINATIONAL FOUNDATION
//
////////////////////////////////////////////////////////////

#include <stdbool.h>


// NAND Gate: Where we start
typedef bool Wire;

Wire nand_gate(Wire a, Wire b) {
  return !(a & b);
}


// NOT Gate
Wire not_gate(Wire a) {
  return nand_gate(a, a);
}


// AND Gate
Wire and_gate(Wire a, Wire b) {
  return not_gate(nand_gate(a, b));
}


// OR Gate
Wire or_gate(Wire a, Wire b) {
  return nand_gate(not_gate(a), not_gate(b));
}


// XOR Gate
Wire xor_gate(Wire a, Wire b) {
  return or_gate(
           and_gate(a, not_gate(b)),
           and_gate(not_gate(a), b));
}


// XNOR Gate
Wire xnor_gate(Wire a, Wire b){
  return not_gate(xor_gate(a, b));
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

#include <stdint.h>


// 32-Bit Ripple-Carry Adder
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


//

////////////////////////////////////////////////////////////
//
// TESTING
//
////////////////////////////////////////////////////////////

#include <stdio.h>

void run_ripple_carry_adder_test(uint32_t a, uint32_t b, Wire cin) {
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
  printf("=======================================================\n");
}

int main() {
  run_ripple_carry_adder_test(12323, 622331, 0);
  run_ripple_carry_adder_test(0xFFFFFFFF, 1, 0);
  run_ripple_carry_adder_test(0x12345678, 0x57654321, 1);
  return 0;
}
