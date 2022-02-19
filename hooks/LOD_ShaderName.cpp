//HOOK LOD ROffset = 0x2FD68

#include <stdlib.h>
#include "../preprocessor/define.h"
#include "../preprocessor/macro.h"

__asm__
(
    ".equ by_pass_address,"QU(lodShaderName)"-0x0042FD68 \n"
);

__asm__ volatile
(
    "jmp . + by_pass_address \n"
    "nop \n"
    ".align 128, 0x0 \n"
);