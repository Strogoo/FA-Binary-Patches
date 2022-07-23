//HOOK selectionPriority ROffset = 0x472ED2

#include <stdlib.h>
#include "../preprocessor/define.h"
#include "../preprocessor/macro.h"

__asm__
(
    ".equ by_pass_address,"QU(ProjectCheckArg)"-0x00872ED2 \n"
);

__asm__ volatile
(
    "jmp . + by_pass_address \n"
	"nop \n"
    ".align 128, 0x0 \n"
);
