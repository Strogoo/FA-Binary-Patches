#include "CObject.h"
#include "magic_classes.h"
#include "moho.h"
#include "utility.h"
#include <cmath>

#define NON_GENERAL_REG(var_) [var_] "g"(var_)

const char *texturePtr = "111111111111";
const char *path = "/textures/engine/newFogOfWar.dds";


void LoadFxFieldValue()
{
    asm(
        //"call 0x00491280;"

        "push eax;"

        "mov eax, %[texturePtr];"
        "cmp dword ptr[eax], 0x31313131;"
        "jne texPtrExists;"
        
        "push 0x1;"  //allowCreate
        "push 0x0;"
        "push %[path];"
        "push %[texturePtr];"
        
        "call 0x00441370;"
        
        "pop ebx;"
        "pop ecx;"  //clear stack
        "push eax;"
        "mov eax, ebx;"
        
        "call 0x00438140;"
        "jmp 0x007F60A5;"
        
        "texPtrExists:;"
        "mov ebx, eax;"
        "pop eax;"
        "pop ecx;" //clear stack
        "push ebx;"
        
        "call 0x00438140;"
        "jmp 0x007F60A5;"
        
        
        :
        :[texturePtr]"m"(texturePtr),
         [path]"m"(path)
        :
    );
}