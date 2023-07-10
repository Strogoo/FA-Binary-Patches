#include "../define.h"
asm(
  ".section h0; .set h0,0x6CE3B2;"
  "jmp "QU(SimGetCommandQueue)";"
  "nop;"
  "nop;"
  "nop;"
);