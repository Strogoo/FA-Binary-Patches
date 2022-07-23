//HOOK selectionPriority ROffset = 0x4730D6

#include <stdlib.h>
#include "../preprocessor/define.h"
#include "../preprocessor/macro.h"

__asm__
(
    ".equ by_pass_address,"QU(ProjectAsFloat)"-0x008730D6 \n"
);

__asm__ volatile
(
    "jmp . + by_pass_address \n"
	"nop \n"
    ".align 128, 0x0 \n"
);
