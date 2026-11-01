////////////////////////////////////////////////////////////
//
// SECTION 1: PRIMITIVE & COMBINATIONAL FOUNDATION
//
////////////////////////////////////////////////////////////

#include <stdbool.h>


// NAND Gate
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

int main() {
  return 0;
}
