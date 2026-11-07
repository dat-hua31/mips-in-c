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


////////////////////////////////////////////////////////////
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
  #define RCA_STAGE(i, carry_in) \
    FAResult fa_##i = full_adder(_wire32_get(a, i), _wire32_get(b, i), carry_in); \
    _wire32_set(&sum, i, fa_##i.sum);

  Wire32 sum;
  
  RCA_STAGE(0, cin);         RCA_STAGE(1, fa_0.cout);   RCA_STAGE(2, fa_1.cout);   RCA_STAGE(3, fa_2.cout);
  RCA_STAGE(4, fa_3.cout);   RCA_STAGE(5, fa_4.cout);   RCA_STAGE(6, fa_5.cout);   RCA_STAGE(7, fa_6.cout);
  RCA_STAGE(8, fa_7.cout);   RCA_STAGE(9, fa_8.cout);   RCA_STAGE(10, fa_9.cout);  RCA_STAGE(11, fa_10.cout);
  RCA_STAGE(12, fa_11.cout); RCA_STAGE(13, fa_12.cout); RCA_STAGE(14, fa_13.cout); RCA_STAGE(15, fa_14.cout);
  RCA_STAGE(16, fa_15.cout); RCA_STAGE(17, fa_16.cout); RCA_STAGE(18, fa_17.cout); RCA_STAGE(19, fa_18.cout);
  RCA_STAGE(20, fa_19.cout); RCA_STAGE(21, fa_20.cout); RCA_STAGE(22, fa_21.cout); RCA_STAGE(23, fa_22.cout);
  RCA_STAGE(24, fa_23.cout); RCA_STAGE(25, fa_24.cout); RCA_STAGE(26, fa_25.cout); RCA_STAGE(27, fa_26.cout);
  RCA_STAGE(28, fa_27.cout); RCA_STAGE(29, fa_28.cout); RCA_STAGE(30, fa_29.cout); RCA_STAGE(31, fa_30.cout);

  #undef RCA_STAGE

  return (RCAResult){sum, fa_31.cout};
}


////////////////////////////////////////////////////////////
SBTResult subtractor(Wire32 a, Wire32 b) {
  RCAResult rca = ripple_carry_adder(a, not_gate32(b), 1);
  return (SBTResult){rca.sum, rca.cout};
}


////////////////////////////////////////////////////////////
Wire32 shift_left32(Wire32 x, Wire shamt0, Wire shamt1, Wire shamt2, Wire shamt3, Wire shamt4)
{
  #define CONCAT(a, b) a##b

  #define MUX2(out, i, prev, shamt, control) \
    _wire32_set(&out, i, mux2(_wire32_get(prev, i), (i < shamt) ? 0 : _wire32_get(prev, (i - shamt)), control))

  #define PHASE(out, prev, shamt, control) \
    Wire32 out = 0; \
    MUX2(out, 0,  prev, shamt, control); MUX2(out, 1,  prev, shamt, control); MUX2(out, 2,  prev, shamt, control); MUX2(out, 3,  prev, shamt, control); \
    MUX2(out, 4,  prev, shamt, control); MUX2(out, 5,  prev, shamt, control); MUX2(out, 6,  prev, shamt, control); MUX2(out, 7,  prev, shamt, control); \
    MUX2(out, 8,  prev, shamt, control); MUX2(out, 9,  prev, shamt, control); MUX2(out, 10, prev, shamt, control); MUX2(out, 11, prev, shamt, control); \
    MUX2(out, 12, prev, shamt, control); MUX2(out, 13, prev, shamt, control); MUX2(out, 14, prev, shamt, control); MUX2(out, 15, prev, shamt, control); \
    MUX2(out, 16, prev, shamt, control); MUX2(out, 17, prev, shamt, control); MUX2(out, 18, prev, shamt, control); MUX2(out, 19, prev, shamt, control); \
    MUX2(out, 20, prev, shamt, control); MUX2(out, 21, prev, shamt, control); MUX2(out, 22, prev, shamt, control); MUX2(out, 23, prev, shamt, control); \
    MUX2(out, 24, prev, shamt, control); MUX2(out, 25, prev, shamt, control); MUX2(out, 26, prev, shamt, control); MUX2(out, 27, prev, shamt, control); \
    MUX2(out, 28, prev, shamt, control); MUX2(out, 29, prev, shamt, control); MUX2(out, 30, prev, shamt, control); MUX2(out, 31, prev, shamt, control);
    
  PHASE(p1, x, 1, shamt0);
  PHASE(p2, p1, 2, shamt1);
  PHASE(p3, p2, 4, shamt2);
  PHASE(p4, p3, 8, shamt3);
  PHASE(p5, p4, 16, shamt4);

  #undef PHASE
  #undef MUX2
  #undef CONCAT

  return p5;
}


////////////////////////////////////////////////////////////
// Opcode Encoding op3 op2 op1 op0
// 0000 add    0100 and    1000 slt    1100 sra
// 0001 addu   0101 or     1001 sltu   1101 lui
// 0010 sub    0110 xor    1010 sll    1110 *not used*
// 0011 subu   0111 nor    1011 srl    1111 *not used*
ALUResult alu(
  Wire32 a, Wire32 b, 
  Wire shamt0, Wire shamt1, Wire shamt2, Wire shamt3, Wire shamt4, 
  Wire op0, Wire op1, Wire op2, Wire op3) 
{
  // General computation
  RCAResult rca = ripple_carry_adder(a, b, 0);
  SBTResult sbt = subtractor(a, b);
  Wire32 and = and_gate32(a, b);
  Wire32 or = or_gate32(a, b);
  Wire32 xor = xor_gate32(a, b);
  Wire32 nor = nor_gate32(a, b);

  // Extract sign bits
  Wire a_sign_bit = _wire32_get(a, 31);
  Wire b_sign_bit = _wire32_get(b, 31);
  Wire sum_sign_bit = _wire32_get(rca.sum, 31);
  Wire diff_sign_bit = _wire32_get(sbt.diff, 31);

  // Overflow for add, sub
  // add: Operands share a sign, but result's sign differs.
  // sub: Operands differ in sign, result's sign differs from a.
  Wire add_overflow = and_gate(xnor_gate(a_sign_bit, b_sign_bit), xor_gate(a_sign_bit, sum_sign_bit));
  Wire sub_overflow = and_gate(xor_gate(a_sign_bit, b_sign_bit), xor_gate(a_sign_bit, diff_sign_bit));

  // Set-on-less-than
  // slt (signed): check the difference sign bit, but use XOR to invert when overflow occurs.
  // slt (unsigned): just use the carry-out bit.
  Wire less_than_signed = xor_gate(diff_sign_bit, sub_overflow);
  Wire less_than_unsigned = not_gate(sbt.cout);
  Wire32 slt = (Wire32)less_than_signed;
  Wire32 sltu = (Wire32)less_than_unsigned;

  Wire32 sll = shift_left32(b, shamt0, shamt1, shamt2, shamt3, shamt4);
}