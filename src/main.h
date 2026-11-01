#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <stdint.h>

// Wire (0/1 Signal)
typedef bool Wire;

// 32-Wire (32-bit bus)
typedef uint32_t Wire32;

// Full Adder Result
typedef struct {
  Wire sum;
  Wire cout;
} FAResult;

// 32-Bit Ripple-Carry Adder Result
typedef struct {
  Wire32 sum;
  Wire cout;
} RCAResult;

// 32-Bit Subtractor Result
typedef struct {
  Wire32 diff;
  Wire cout;
} SBTResult;

// ALU Result
typedef struct {
  Wire32 result;
  Wire zero;
  Wire overflow;
  Wire cout;
} ALUResult;

// Wire Utilities
Wire _wire32_get(Wire32 w, int i);
void _wire32_set(Wire32* w, int i, Wire val);
Wire32 _broadcast(Wire w);

// Logic Gates
Wire nand_gate(Wire a, Wire b);
Wire not_gate(Wire a);
Wire and_gate(Wire a, Wire b);
Wire or_gate(Wire a, Wire b);
Wire nor_gate(Wire a, Wire b);
Wire xor_gate(Wire a, Wire b);
Wire xnor_gate(Wire a, Wire b);
Wire32 nand_gate32(Wire32 a, Wire32 b);
Wire32 not_gate32(Wire32 a);
Wire32 and_gate32(Wire32 a, Wire32 b);
Wire32 or_gate32(Wire32 a, Wire32 b);
Wire32 nor_gate32(Wire32 a, Wire32 b);
Wire32 xor_gate32(Wire32 a, Wire32 b);
Wire32 xnor_gate32(Wire32 a, Wire32 b);

// MUXs
Wire mux2(Wire d0, Wire d1, Wire s);
Wire mux4(Wire d0, Wire d1, Wire d2, Wire d3, Wire s0, Wire s1);
Wire32 mux2_32(Wire32 d0, Wire32 d1, Wire s);
Wire32 mux4_32(Wire32 d0, Wire32 d1, Wire32 d2, Wire32 d3, Wire s0, Wire s1);
Wire32 mux16_32(Wire32 d0, Wire32 d1, Wire32 d2, Wire32 d3,
                Wire32 d4, Wire32 d5, Wire32 d6, Wire32 d7,
                Wire32 d8, Wire32 d9, Wire32 d10, Wire32 d11,
                Wire32 d12, Wire32 d13, Wire32 d14, Wire32 d15,
                Wire s0, Wire s1, Wire s2, Wire s3);

// Adders
FAResult full_adder(Wire a, Wire b, Wire cin);
RCAResult ripple_carry_adder(Wire32 a, Wire32 b, Wire cin);
SBTResult subtractor(Wire32 a, Wire32 b);

// ALU
ALUResult alu(Wire32 a, Wire32 b, Wire op0, Wire op1, Wire op2, Wire op3);

#endif