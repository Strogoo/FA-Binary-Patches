char stackHolder[4];

void SimGetCommandQueue()
{
    const char* commandType = "commandType";
    const char* targetId = "targetId";
    const char* blueprintId = "blueprintId";
    const char* x = "x";
    const char* y = "y";
    const char* z = "z";
    
	asm(
        //default code
        "add eax, 0x20;"
        "push eax;"
        "lea ecx, [esp+0x24];"
        "call 0x909AF0;"
        
        
        //save stack
        "lea ecx, [esp+0x20];"
        "mov eax, [ecx+0x10];"
        "mov %[stackHolder], eax;"
        
        //Type
        "mov eax, [esi];"
        "push [eax+0x94];"
        "push %[commandType];"
        "mov eax, [eax+0x2C];"
        "mov [ecx+0x10], eax;"
        "call 0x9081F0;"
        
        //X
        "lea ecx, [esp+0x20];"
        "mov eax, [esi];"
        "push [eax+0xA0];"
        "push %[x];"
        "mov eax, [eax+0x2C];"
        "mov [ecx+0x10], eax;"
        "call 0x908320;"
        
        //Y
        "lea ecx, [esp+0x20];"
        "mov eax, [esi];"
        "push [eax+0xA4];"
        "push %[y];"
        "mov eax, [eax+0x2C];"
        "mov [ecx+0x10], eax;"
        "call 0x908320;"
        
        //Z
        "lea ecx, [esp+0x20];"
        "mov eax, [esi];"
        "push [eax+0xA8];"
        "push %[z];"
        "mov eax, [eax+0x2C];"
        "mov [ecx+0x10], eax;"
        "call 0x908320;"
        
        //targetId
        "lea ecx, [esp+0x20];"
        "mov eax, [esi];"
        "mov edx, [eax+0x9C];"
        "cmp edx, 0xF0000000;"
        "je bpId;"
        "and edx, 0x1FFFFF;"
        "push edx;"
        "push %[targetId];"
        "mov eax, [eax+0x2C];"
        "mov [ecx+0x10], eax;"
        "call 0x9081F0;"
        
        //blueprintId
        "bpId:;"
        "lea ecx, [esp+0x20];"
        "mov eax, [esi];"
        "mov edx, [eax+0x5C];"
        "cmp edx, 0x0;"
        "je End;"
        "add edx, 0xC;"
        "push edx;"
        "push %[blueprintId];"
        "mov eax, [eax+0x2C];"
        "mov [ecx+0x10], eax;"
        "call 0x908450;"
        
        // restore stack
        "End:;"
        "mov eax, %[stackHolder];"
        "add esp, 0x30;"
        "mov [esp], eax;"
        "sub esp, 0x30;"
        "jmp 0x6CE3BF;"
        
        :
        : [commandType] "i" (commandType), [targetId] "i" (targetId), [blueprintId] "i" (blueprintId), [x] "i" (x), [y] "i" (y), [z] "i" (z), [stackHolder] "m" (stackHolder)
        :
	);
}

/* Commands
1 = Stop
2 = Move
3 = Dive
4 = FormMove
5 = BuildSiloTactical
6 = BuildSiloNuke
7 = BuildFactory
8 = BuildMobile
9 = BuildAssist
10 = Attack
11 = FormAttack
12 = Nuke
13 = Tactical
14 = Teleport
15 = Guard
16 = Patrol
17 = Ferry
18 = FormPatrol
19 = Reclaim
20 = Repair
21 = Capture
22 = TransportLoadUnits
23 = TransportReverseLoadUnits
24 = TransportUnloadUnits
25 = TransportUnloadSpecificUnits
26 = DetachFromTransport
27 = Upgrade
28 = Script
29 = AssistCommander
30 = KillSelf
31 = DestroySelf
32 = Sacrifice
33 = Pause
34 = OverCharge
35 = AggressiveMove
36 = FormAggressiveMove
37 = AssistMove
38 = SpecialAction
39 = Dock
*/