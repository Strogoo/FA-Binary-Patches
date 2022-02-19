#include <stdlib.h>

void lodShaderName()
{
	__asm__
	(
    "test ecx, ecx \n"
    "je 0x0042FD79 \n"
    "mov eax, dword ptr [esp+0x4] \n"
    "cmp byte ptr [eax+0x1], 0x3F \n"
    "jne EXIT \n"
    "add eax, 0x2 \n"
    "EXIT: \n"
    "jmp 0x0042FD6E \n"
	);
}

void GetIntegerAnnotation()
{
	__asm__
	(
    "push eax \n"
    "push ebp \n"
    "add ebp, 0xC \n"
    "mov eax, dword ptr [ebp] \n"
    "cmp dword ptr [ebp+0x4], 0x0 \n"
    "pop ebp \n"
    "pop eax \n"
    "xor esi,esi \n"
    "cmp dword ptr [edi+0x48], esi \n"
    "mov dword ptr [esp+0x10], esi \n"
    "jmp 0x0042D670 \n"
	);
}