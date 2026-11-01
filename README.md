# Overview

This project implements a 32-bit MIPS32 architecture using a hardware-first philosophy. All code adheres to the following laws:

* **No Temporal Control Flow:** Runtime `if`, `else`, `switch`, or loops on active data/control paths are prohibited. Selection and operations must use structural gate logic or complex modules built from those gates.
* **Hierarchical Composition:** Complex modules must be built by cascading smaller, verified primitives (e.g., Gates $\rightarrow$ 2-to-1 MUX $\rightarrow$ 4-to-1 MUX $\rightarrow$ 16-to-1 MUX).

This is where we start:
```C
Wire nand_gate(Wire a, Wire b) {
  return !(a & b);
}
Wire32 nand_gate32(Wire32 a, Wire32 b) {
  return ~(a & b);
}
```

*(Note: Static topology configuration, such as structural bitfield mapping, is permitted as board-level trace routing).*
