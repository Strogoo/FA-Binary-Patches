//HOOK LOD ROffset = 0x2D667

#include <stdlib.h>
#include "../preprocessor/define.h"
#include "../preprocessor/macro.h"

__asm__
(
    ".equ by_pass_address,"QU(GetIntegerAnnotation)"-0x0042D667 \n"
);

__asm__ volatile
(
    "jmp . + by_pass_address \n"
    "nop \n"
    "nop \n"
    "nop \n"
    "nop \n"
    ".align 128, 0x0 \n"
);