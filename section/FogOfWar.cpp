#include "CObject.h"
#include "magic_classes.h"
#include "moho.h"
#include "utility.h"
#include <cmath>

#define NON_GENERAL_REG(var_) [var_] "g"(var_)

const char *texturePtr = "111111111111";
const char *path = "/textures/engine/newFogOfWar.dds";


float worldToProjectionMatrix[16] = {};
const float *wldToProjPtr = worldToProjectionMatrix;

float InverseWorldToProjectionMatrix[16] = {};
const float *InvwWldToProjPtr = InverseWorldToProjectionMatrix;

bool newVarsLoaded = false;
const bool* newVarsLoadedPtr = &newVarsLoaded;

struct shaderVar2
{ // 0x30 bytes
	int *vtable;
    int nothing;
    char fieldName[15];
    int charLen;
    int end;
    int unknown1;
    int unknown2;
    int unknown3;
    int unknown4;
};

struct shaderVar2 wPos = {0, 0, "ProjectionWpos", 14, 15, 0,0,0,0};
shaderVar2* wPosPtr = &wPos;

struct shaderVar
{ // 0x48 bytes
	int empty;
    char varName[16];
    int varNameLen;
    int varNameMaxLen;
    
    int empty2;
    char fxFileName[16];
    int fxNameLen;
    int fxNameMaxLen;
    
    int ptr1;
    int ptr2;
    int ptr3;
    int ptr4;
};

struct shaderVar projWpos = {0, "ProjectionWpos", 14, 15, 0, "frame", 14, 15, 0,0,0,0};
shaderVar* projWposPtr = &projWpos;
const char* wPosName = "ProjectionWpos";

void RegisterNewFrameVar()
{
    asm(
        "push 0xE398CC;"          // "frame"
        "push %[projWposPtr];"
        "mov ecx, %[wPosName];"
        "call 0x00438000;"        // register shaderVar
        
        "call 0x00A8211E;"
        "jmp 0x00BE131E;"
        
        :
        :[wPosName]"m"(wPosName),
         [projWposPtr]"m"(projWposPtr)
        :
    );
}

void SaveMatrices()
{
    asm(
        "lea eax, ds:[ebx+0x9C];"
        
        "mov %[wldToProjPtr], eax;"

        "jmp 0x0049533F;"
        :
        :[wldToProjPtr]"m"(wldToProjPtr)
        :
    );
}

void SetWpos()
{
    asm(
        "fld dword ptr ds:[0x00F57E68];"
        
        "mov eax, %[newVarsLoadedPtr];"
        "cmp dword ptr[eax], 0x0;"
        "jne fieldsLoaded;"
        
        "mov eax, %[wPosPtr];"
        "mov edx, dword ptr [ecx];"
        "mov dword ptr [eax], edx;"
        
        "mov edx, dword ptr [ecx+0x20];"
        "mov [eax+0x20], edx;"
        
        "mov edx, dword ptr [ecx+0x24];"
        "mov [eax+0x24], edx;"
        
        "mov edx, dword ptr [ecx+0x28];"
        "mov [eax+0x28], edx;"
        
        "mov edx, dword ptr [ecx+0x2C];"
        "mov [eax+0x2C], edx;"
        
        "fieldsLoaded:;"
        
        "mov eax, %[wPosPtr];"
        "mov ecx, eax;"
        "mov edx, [ecx];"
        "mov eax, [edx+0x24];"
        "push ecx;"

        "jmp 0x007F60C5;"
        :
        :[wPosPtr]"m"(wPosPtr),
         [newVarsLoadedPtr]"m"(newVarsLoadedPtr)
        :
    );
}

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


bool InverseMat4x4()
{
    const float *m = wldToProjPtr;
    
    float inv[16];
    float det;
    int i;

    inv[0]  =  m[5] * m[10] * m[15] - m[5] * m[11] * m[14] - m[9] * m[6] * m[15] + m[9] * m[7] * m[14] + m[13] * m[6] * m[11] - m[13] * m[7] * m[10];
    inv[4]  = -m[4] * m[10] * m[15] + m[4] * m[11] * m[14] + m[8] * m[6] * m[15] - m[8] * m[7] * m[14] - m[12] * m[6] * m[11] + m[12] * m[7] * m[10];
    inv[8]  =  m[4] * m[9]  * m[15] - m[4] * m[11] * m[13] - m[8] * m[5] * m[15] + m[8] * m[7] * m[13] + m[12] * m[5] * m[11] - m[12] * m[7] * m[9];
    inv[12] = -m[4] * m[9]  * m[14] + m[4] * m[10] * m[13] + m[8] * m[5] * m[14] - m[8] * m[6] * m[13] - m[12] * m[5] * m[10] + m[12] * m[6] * m[9];
    inv[1]  = -m[1] * m[10] * m[15] + m[1] * m[11] * m[14] + m[9] * m[2] * m[15] - m[9] * m[3] * m[14] - m[13] * m[2] * m[11] + m[13] * m[3] * m[10];
    inv[5]  =  m[0] * m[10] * m[15] - m[0] * m[11] * m[14] - m[8] * m[2] * m[15] + m[8] * m[3] * m[14] + m[12] * m[2] * m[11] - m[12] * m[3] * m[10];
    inv[9]  = -m[0] * m[9]  * m[15] + m[0] * m[11] * m[13] + m[8] * m[1] * m[15] - m[8] * m[3] * m[13] - m[12] * m[1] * m[11] + m[12] * m[3] * m[9];
    inv[13] =  m[0] * m[9]  * m[14] - m[0] * m[10] * m[13] - m[8] * m[1] * m[14] + m[8] * m[2] * m[13] + m[12] * m[1] * m[10] - m[12] * m[2] * m[9];
    inv[2]  =  m[1] * m[6]  * m[15] - m[1] * m[7]  * m[14] - m[5] * m[2] * m[15] + m[5] * m[3] * m[14] + m[13] * m[2] * m[7]  - m[13] * m[3] * m[6];
    inv[6]  = -m[0] * m[6]  * m[15] + m[0] * m[7]  * m[14] + m[4] * m[2] * m[15] - m[4] * m[3] * m[14] - m[12] * m[2] * m[7]  + m[12] * m[3] * m[6];
    inv[10] =  m[0] * m[5]  * m[15] - m[0] * m[7]  * m[13] - m[4] * m[1] * m[15] + m[4] * m[3] * m[13] + m[12] * m[1] * m[7]  - m[12] * m[3] * m[5];
    inv[14] = -m[0] * m[5]  * m[14] + m[0] * m[6]  * m[13] + m[4] * m[1] * m[14] - m[4] * m[2] * m[13] - m[12] * m[1] * m[6]  + m[12] * m[2] * m[5];
    inv[3]  = -m[1] * m[6]  * m[11] + m[1] * m[7]  * m[10] + m[5] * m[2] * m[11] - m[5] * m[3] * m[10] - m[9]  * m[2] * m[7]  + m[9]  * m[3] * m[6];
    inv[7]  =  m[0] * m[6]  * m[11] - m[0] * m[7]  * m[10] - m[4] * m[2] * m[11] + m[4] * m[3] * m[10] + m[8]  * m[2] * m[7]  - m[8]  * m[3] * m[6];
    inv[11] = -m[0] * m[5]  * m[11] + m[0] * m[7]  * m[9]  + m[4] * m[1] * m[11] - m[4] * m[3] * m[9]  - m[8]  * m[1] * m[7]  + m[8]  * m[3] * m[5];
    inv[15] =  m[0] * m[5]  * m[10] - m[0] * m[6]  * m[9]  - m[4] * m[1] * m[10] + m[4] * m[2] * m[9]  + m[8]  * m[1] * m[6]  - m[8]  * m[2] * m[5];

    det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];
    if (det == 0) return false;
    det = 1.0 / det;

    for (i = 0; i < 16; i++)
        InverseWorldToProjectionMatrix[i] = inv[i] * det;
    
    return true;
}


int LuaMousePosXYZ(lua_State *l)
{
    float mX = luaL_checknumber(l, 1);
    float mY = luaL_checknumber(l, 2);
    float mZ = luaL_checknumber(l, 3);
    float mW = 1.0f;
    
    float log = luaL_checknumber(l, 4);

    float line1_1 = wldToProjPtr[0];
    float line1_2 = wldToProjPtr[4];
    float line1_3 = wldToProjPtr[8];
    float line1_4 = wldToProjPtr[12];
    
    float line2_1 = wldToProjPtr[1];
    float line2_2 = wldToProjPtr[5];
    float line2_3 = wldToProjPtr[9];
    float line2_4 = wldToProjPtr[13];
    
    float line3_1 = wldToProjPtr[2];
    float line3_2 = wldToProjPtr[6];
    float line3_3 = wldToProjPtr[10];
    float line3_4 = wldToProjPtr[14];
    
    float line4_1 = wldToProjPtr[3];
    float line4_2 = wldToProjPtr[7];
    float line4_3 = wldToProjPtr[11];
    float line4_4 = wldToProjPtr[15];
    
    float viewX = line1_1*mX + line1_2*mY + line1_3*mZ + line1_4*mW;
    float viewY = line2_1*mX + line2_2*mY + line2_3*mZ + line2_4*mW;
    float viewZ = line3_1*mX + line3_2*mY + line3_3*mZ + line3_4*mW;
    float viewW = line4_1*mX + line4_2*mY + line4_3*mZ + line4_4*mW;
    //float viewW = 0*mX + 0*mY + 0*mZ + 1*mW;
    
    InverseMat4x4();
    
    float inViewX = InvwWldToProjPtr[0]*viewX + InvwWldToProjPtr[4]*viewY + InvwWldToProjPtr[8]*viewZ + InvwWldToProjPtr[12]*viewW;
    float inViewY = InvwWldToProjPtr[1]*viewX + InvwWldToProjPtr[5]*viewY + InvwWldToProjPtr[9]*viewZ + InvwWldToProjPtr[13]*viewW;
    float inViewZ = InvwWldToProjPtr[2]*viewX + InvwWldToProjPtr[6]*viewY + InvwWldToProjPtr[10]*viewZ + InvwWldToProjPtr[14]*viewW;
    float inViewW = InvwWldToProjPtr[3]*viewX + InvwWldToProjPtr[7]*viewY + InvwWldToProjPtr[11]*viewZ + InvwWldToProjPtr[15]*viewW;
    
    char buf[16] = {};
    
    LogF("--------MOUSE_WORLD_POS-------------");
    LogF("-");
    sprintf_s(buf, sizeof(buf), "%f", mX);
    LogF(buf);
    sprintf_s(buf, sizeof(buf), "%f", mY);
    LogF(buf);
    sprintf_s(buf, sizeof(buf), "%f", mZ);
    LogF(buf);
    LogF("-");
    
    LogF("-----MOUSE_Pos * WorldToProjection4x4-----");
    LogF("-");
    sprintf_s(buf, sizeof(buf), "%f", viewX);
    LogF(buf);
    sprintf_s(buf, sizeof(buf), "%f", viewY);
    LogF(buf);
    sprintf_s(buf, sizeof(buf), "%f", viewZ);
    LogF(buf);
    sprintf_s(buf, sizeof(buf), "%f", viewW);
    LogF(buf);
    LogF("-");
    
    LogF("-----MOUSE_Pos * INVERSE_WorldToProjection4x4-----");
    LogF("-");
    sprintf_s(buf, sizeof(buf), "%f", inViewX);
    LogF(buf);
    sprintf_s(buf, sizeof(buf), "%f", inViewY);
    LogF(buf);
    sprintf_s(buf, sizeof(buf), "%f", inViewZ);
    LogF(buf);
    sprintf_s(buf, sizeof(buf), "%f", inViewW);
    LogF(buf);
    LogF("-");
    
    
    

    if (log > 0)
    {
        LogF("----------WorldToProj4x4---------------");
        LogF("-");
        LogF("--ROW 1--");
        sprintf_s(buf, sizeof(buf), "%f", line1_1);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", line1_2);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", line1_3);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", line1_4);
        LogF(buf);
        LogF("-");
        
        LogF("--ROW 2--");
        sprintf_s(buf, sizeof(buf), "%f", line2_1);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", line2_2);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", line2_3);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", line2_4);
        LogF(buf);
        LogF("-");
        
        LogF("--ROW 3--");
        sprintf_s(buf, sizeof(buf), "%f", line3_1);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", line3_2);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", line3_3);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", line3_4);
        LogF(buf);
        LogF("-");
        
        LogF("--ROW 4--");
        sprintf_s(buf, sizeof(buf), "%f", line4_1);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", line4_2);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", line4_3);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", line4_4);
        LogF(buf);
        LogF("-");
        
        
        LogF("----------InverseWorldToProj4x4---------------");
        LogF("-");
        LogF("--ROW 1--");
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[0]);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[4]);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[8]);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[12]);
        LogF(buf);
        LogF("-");
        
        LogF("--ROW 2--");
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[1]);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[5]);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[9]);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[13]);
        LogF(buf);
        LogF("-");
        
        LogF("--ROW 3--");
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[2]);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[6]);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[10]);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[14]);
        LogF(buf);
        LogF("-");
        
        LogF("--ROW 4--");
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[3]);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[7]);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[11]);
        LogF(buf);
        sprintf_s(buf, sizeof(buf), "%f", InverseWorldToProjectionMatrix[15]);
        LogF(buf);
        LogF("-");
    }

     return 0;
}

UIRegFunc MousePosXYZReg{"UI_MousePosXYZ", "UI_MousePosXYZ(posX:float, posY:float, PosZ:float, log:float)", LuaMousePosXYZ};


// void AddNewVarToFrameFx()
// {
    // asm(
        // "call 0x00A8211E;"
        
        
        // "jmp 
        
        // :
        // :[texturePtr]"m"(texturePtr),
         // [path]"m"(path)
        // :
    // );
// }