#include <stdlib.h>

char ThirdArgument[4];

//Tweaking CUIWorldView:Project(self, VECTOR3) so when it's used with third argument (Bool or whatever)
//it returns x,y floats instead of table for better performance

void ProjectCheckArg()
{
	__asm__
	(
        "add esp, 0x4 \n"
        "cmp eax, 0x3 \n"
        "jne NoThirdArg \n"
        "mov %[ThirdArgument], eax \n"
        "jmp 0x00872EEC \n"
        "NoThirdArg: \n"
        "cmp eax, 0x2 \n"
        "jmp 0x00872ED8 \n"
        :
        : [ThirdArgument] "m" (ThirdArgument)
        :
	);
}

void ProjectAsFloat()
{
     char const * arg3 = ThirdArgument;
	__asm__
	(
        "push eax \n"
        "mov eax, %[arg3] \n"
        "cmp dword ptr [eax], 0x0 \n"
        "jne returnFloats \n"
        "pop eax \n"
        "movss dword ptr [esp+0x1C], xmm1 \n"
        "jmp 0x008730DC \n"
        
        
        "returnFloats: \n"
        "mov dword ptr [eax], 0x0 \n"
        "pop eax \n"
        "sub esp, 0x8 \n"
        "mov eax, dword ptr [esp+0xC] \n"
        "mov dword ptr [esp], eax \n"
        "movss xmm3, xmm0 \n"
        "movss dword ptr [esp+0x4], xmm1 \n"
        "call 0x0090CD40 \n"
        
        "movss dword ptr [esp+0x4], xmm3 \n"
        "call 0x0090CD40 \n"

        "mov eax, 0x2 \n"
        "add esp, 0x8 \n"
        "mov ecx, dword ptr ss:[esp+0x84] \n"
        "mov dword ptr fs:[0x00000000], ecx \n"
        "pop edi \n"
        "pop esi \n"
        "pop ebx \n"
        "mov esp, ebp \n"
        "pop ebp \n"
        "ret \n"
        :
        : [arg3] "i" (arg3)
        :
	);
}