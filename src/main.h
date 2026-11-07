#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <stdint.h>

////////////////////////////////////////////////////////////
//
// Structures Definitions
//
////////////////////////////////////////////////////////////

// Wire
typedef bool Wire;

// 32-Wire
typedef uint32_t Wire32;

// Full Adder Result
typedef struct {
  Wire sum;       // Sum bit
  Wire cout;      // Carry-out bit
} FAResult;

// 32-Bit Ripple-Carry Adder Result
typedef struct {
  Wire32 sum;     // 32-bit sum
  Wire cout;      // Carry-out bit
} RCAResult;

// 32-Bit Subtractor Result
typedef struct {
  Wire32 diff;    // 32-bit difference
  Wire cout;      // Carry-out bit
} SBTResult;

// ALU Result
typedef struct {
  Wire32 result;  // 32-bit result
  Wire overflow;  // Indicate overflow in signed calculations
  Wire zero;      // Zero flag
} ALUResult;

////////////////////////////////////////////////////////////
//
// Topological Utilities
//
////////////////////////////////////////////////////////////

Wire _wire32_get(Wire32 w, int i);              // Get a wire out of 32 wires
void _wire32_set(Wire32* w, int i, Wire val);   // Set a wire to val in a 32-wire
Wire32 _broadcast(Wire w);                      // Broadcast a wire into a 32-wire

////////////////////////////////////////////////////////////
//
// Logic Gates
//
////////////////////////////////////////////////////////////

// 1-bit Logic Gates
Wire nand_gate(Wire a, Wire b);           // Return a NAND b
Wire not_gate(Wire a);                    // Return a NOT b
Wire and_gate(Wire a, Wire b);            // Return a AND b
Wire or_gate(Wire a, Wire b);             // Return a OR b
Wire nor_gate(Wire a, Wire b);            // Return a NOR b
Wire xor_gate(Wire a, Wire b);            // Return a XOR b
Wire xnor_gate(Wire a, Wire b);           // Return a XNOR b

// 32-bit Logic Gates
Wire32 nand_gate32(Wire32 a, Wire32 b);   // Return a NAND b (32-bit)
Wire32 not_gate32(Wire32 a);              // Return a NOT b (32-bit)
Wire32 and_gate32(Wire32 a, Wire32 b);    // Return a AND b (32-bit)
Wire32 or_gate32(Wire32 a, Wire32 b);     // Return a OR b (32-bit)
Wire32 nor_gate32(Wire32 a, Wire32 b);    // Return a NOR b (32-bit)
Wire32 xor_gate32(Wire32 a, Wire32 b);    // Return a XOR b (32-bit)
Wire32 xnor_gate32(Wire32 a, Wire32 b);   // Return a XNOR b (32-bit)

////////////////////////////////////////////////////////////
//
// Combinational Circuits
//
////////////////////////////////////////////////////////////

// 1-bit Multiplexers
Wire mux2(Wire d0, Wire d1, Wire s);                              // Return 2-to-1 multiplexer result
Wire mux4(Wire d0, Wire d1, Wire d2, Wire d3, Wire s0, Wire s1);  // Return 4-to-1 multiplexer result

// 32-bit Multiplexers
Wire32 mux2_32(Wire32 d0, Wire32 d1, Wire s);                                   // Return 2-to-1 multiplexer result
Wire32 mux4_32(Wire32 d0, Wire32 d1, Wire32 d2, Wire32 d3, Wire s0, Wire s1);   // Return 4-to-1 multiplexer result
Wire32 mux16_32(Wire32 d0, Wire32 d1, Wire32 d2, Wire32 d3,
                Wire32 d4, Wire32 d5, Wire32 d6, Wire32 d7,
                Wire32 d8, Wire32 d9, Wire32 d10, Wire32 d11,
                Wire32 d12, Wire32 d13, Wire32 d14, Wire32 d15,
                Wire s0, Wire s1, Wire s2, Wire s3);        // Return 16-to-1 multiplexer result

// Adders
FAResult  full_adder(Wire a, Wire b, Wire cin);             // Return full adder result
RCAResult ripple_carry_adder(Wire32 a, Wire32 b, Wire cin); // Return 32-bit ripple-carry adder result
SBTResult subtractor(Wire32 a, Wire32 b);                   // Return 32-bit subtractor result

// Shifters
Wire32 shift_left32(Wire32 x, Wire shamt0, Wire shamt1, Wire shamt2, Wire shamt3, Wire shamt4);  // Return 32-bit left shift result
Wire32 shift_right32(Wire32 x, Wire shamt0, Wire shamt1, Wire shamt2, Wire shamt3, Wire shamt4); // Return 32-bit right shift result

// ALU
ALUResult alu(
  Wire32 a, Wire32 b, 
  Wire shamt0, Wire shamt1, Wire shamt2, Wire shamt3, Wire shamt4, 
  Wire op0, Wire op1, Wire op2, Wire op3);                  // Return 32-bit ALU result

#endif // MAIN_H