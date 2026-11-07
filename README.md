# Overview

This project implements a 32-bit MIPS32 architecture. All code adheres to the following laws:

* Prohibit runtime `if`, `else`, `switch`, or loops on active data/control paths.


* Build complex modules by cascading smaller, verified primitives.


* Permit static topology configuration, such as structural bitfield mapping, as board-level trace routing.

This is where we start:
```C
Wire nand_gate(Wire a, Wire b) {
  return !(a & b);
}
Wire32 nand_gate32(Wire32 a, Wire32 b) {
  return ~(a & b);
}
```
