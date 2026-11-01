#include "main.h"

////////////////////////////////////////////////////////////
Wire _wire32_get(Wire32 w, int i) {
  return w >> i & 1;
}


////////////////////////////////////////////////////////////
void _wire32_set(Wire32* w, int i, Wire val) {
  uint32_t clear_mask = ~(1U << i);
  uint32_t bit_val = ((uint32_t)val) << i;
  *w = (*w & clear_mask) | bit_val;
}


////////////////////////////////////////////////////////////
Wire32 _broadcast(Wire w) {
  return -(Wire32)w;
}


////////////////////////////////////////////////////////////
Wire nand_gate(Wire a, Wire b) {
  return !(a & b);
}
Wire32 nand_gate32(Wire32 a, Wire32 b) {
  return ~(a & b);
}


////////////////////////////////////////////////////////////
Wire not_gate(Wire a) {
  return nand_gate(a, a);
}
Wire32 not_gate32(Wire32 a) {
  return nand_gate32(a, a);
}


////////////////////////////////////////////////////////////
Wire and_gate(Wire a, Wire b) {
  return not_gate(nand_gate(a, b));
}
Wire32 and_gate32(Wire32 a, Wire32 b) {
  return not_gate32(nand_gate32(a, b));
}


////////////////////////////////////////////////////////////
Wire or_gate(Wire a, Wire b) {
  return nand_gate(not_gate(a), not_gate(b));
}
Wire32 or_gate32(Wire32 a, Wire32 b) {
  return nand_gate32(not_gate32(a), not_gate32(b));
}


////////////////////////////////////////////////////////////
Wire nor_gate(Wire a, Wire b) {
  return not_gate(or_gate(a, b));
}
Wire32 nor_gate32(Wire32 a, Wire32 b) {
  return not_gate32(or_gate32(a, b));
}


////////////////////////////////////////////////////////////
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


////////////////////////////////////////////////////////////
Wire xnor_gate(Wire a, Wire b){
  return not_gate(xor_gate(a, b));
}
Wire32 xnor_gate32(Wire32 a, Wire32 b){
  return not_gate32(xor_gate32(a, b));
}


////////////////////////////////////////////////////////////
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


////////////////////////////////////////////////////////////
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


////////////////////////////////////////////////////////////
FAResult full_adder(Wire a, Wire b, Wire cin) {
  Wire a_xor_b = xor_gate(a, b);
  Wire sum = xor_gate(cin, a_xor_b);
  Wire cout = or_gate(and_gate(a, b), and_gate(cin, a_xor_b));
  return (FAResult){sum, cout};
}


////////////////////////////////////////////////////////////
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


////////////////////////////////////////////////////////////
SBTResult subtractor(Wire32 a, Wire32 b) {
  Wire32 inv_b = not_gate32(b);
  RCAResult rca = ripple_carry_adder(a, inv_b, 1);
  return (SBTResult){rca.sum, rca.cout};
}


////////////////////////////////////////////////////////////
ALUResult alu(Wire32 a, Wire32 b, Wire op0, Wire op1, Wire op2, Wire op3);