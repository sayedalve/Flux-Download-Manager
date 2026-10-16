# Ghidra Analysis Report: IDMNetMon.dll

- **Program**: `IDMNetMon.dll`
- **Executable Path**: `/C:/Users/sayed/Downloads/IDM_RE_Investigation/binaries/IDMNetMon.dll`
- **Language**: `x86:LE:32:default`
- **Compiler**: `windows`
- **Image Base**: `10000000`

## Targeted String Occurrences and Cross-References

### Pattern: `mpdatoidm` (1 occurrence(s))

- **Found at Address**: `0x1006d700`
  - **Direct References** (1):
    - from `0x1003facf` (Type: READ) in Function `[FUN_1003f260](file:///0x1003f260)`

### Pattern: `hlstoidm` (1 occurrence(s))

- **Found at Address**: `0x1006d40c`
  - **Direct References** (1):
    - from `0x1003d050` (Type: DATA) in Function `[FUN_1003ced0](file:///0x1003ced0)`

### Pattern: `hls2toidm` (3 occurrence(s))

- **Found at Address**: `0x1006a720`
  - **Direct References** (3):
    - from `0x10011324` (Type: DATA) in Function `[FUN_10010480](file:///0x10010480)`
    - from `0x1001135f` (Type: DATA) in Function `[FUN_10010480](file:///0x10010480)`
    - from `0x10011308` (Type: DATA) in Function `[FUN_10010480](file:///0x10010480)`
- **Found at Address**: `0x1006a740`
  - **Direct References** (1):
    - from `0x100113c7` (Type: READ) in Function `[FUN_10010480](file:///0x10010480)`
- **Found at Address**: `0x1006d418`
  - **Direct References** (1):
    - from `0x1003d074` (Type: DATA) in Function `[FUN_1003ced0](file:///0x1003ced0)`

### Pattern: `idmbrbtn` (1 occurrence(s))

- **Found at Address**: `0x1006b6d4`
  - **Direct References** (1):
    - from `0x1001979d` (Type: READ) in Function `[FUN_10019660](file:///0x10019660)`

### Pattern: `CreateIDMButton` (1 occurrence(s))

- **Found at Address**: `0x1006cd10`
  - **Direct References** (1):
    - from `0x100325b7` (Type: DATA) in Function `[FUN_100324f0](file:///0x100324f0)`

### Pattern: `CreateIDMButton3` (1 occurrence(s))

- **Found at Address**: `0x1006cd10`
  - **Direct References** (1):
    - from `0x100325b7` (Type: DATA) in Function `[FUN_100324f0](file:///0x100324f0)`

### Pattern: `CreateDownlSelWtIDMButton` (1 occurrence(s))

- **Found at Address**: `0x1006cd24`
  - **Direct References** (1):
    - from `0x100325cd` (Type: DATA) in Function `[FUN_100324f0](file:///0x100324f0)`

### Pattern: `MSG#` (2 occurrence(s))

- **Found at Address**: `0x10034ffb`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.
- **Found at Address**: `0x1006d3a4`
  - **Direct References** (1):
    - from `0x10034b9f` (Type: DATA) in Function `[FUN_10034b40](file:///0x10034b40)`

### Pattern: `\\.\pipe\` (1 occurrence(s))

- **Found at Address**: `0x1006ac88`
  - **Direct References** (1):
    - from `0x10013d68` (Type: DATA) in Function `[FUN_10013d30](file:///0x10013d30)`

## Decompiled Key Functions

Total unique functions identified for decompilation: 7

### Function `[FUN_1003ced0](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMNetMon_dll/FUN_1003ced0_1003ced0.c)`

- **Entry Point**: `0x1003ced0`
- **Signature**: `undefined FUN_1003ced0(void)`
- **Callers** (5): `FUN_1002bfc0` (0x1002bfc0), `FUN_10037770` (0x10037770), `FUN_1001c4d0` (0x1001c4d0), `FUN_100398f0` (0x100398f0), `FUN_10019190` (0x10019190)
- **Callees** (1): `_strnicmp` (0xEXTERNAL:00000032)

```c

undefined4 __fastcall FUN_1003ced0(undefined4 *param_1)

{
  int iVar1;
  
  if ((((uint)param_1[1] < 5) || (iVar1 = _strnicmp((char *)*param_1,"http:",5), iVar1 != 0)) &&
     (((uint)param_1[1] < 6 || (iVar1 = _strnicmp((char *)*param_1,"https:",6), iVar1 != 0)))) {
    if (((3 < (uint)param_1[1]) && (iVar1 = _strnicmp((char *)*param_1,"ftp:",4), iVar1 == 0)) ||
       ((4 < (uint)param_1[1] && (iVar1 = _strnicmp((char *)*param_1,"ftps:",5), iVar1 == 0)))) {
      return 2;
    }
    if ((((4 < (uint)param_1[1]) && (iVar1 = _strnicmp((char *)*param_1,"rtmp:",5), iVar1 == 0)) ||
        ((5 < (uint)param_1[1] && (iVar1 = _strnicmp((char *)*param_1,"rtmpt:",6), iVar1 == 0)))) ||
       ((5 < (uint)param_1[1] && (iVar1 = _strnicmp((char *)*param_1,"rtmpe:",6), iVar1 == 0)))) {
      return 3;
    }
    if ((6 < (uint)param_1[1]) && (iVar1 = _strnicmp((char *)*param_1,"idmreg:",7), iVar1 == 0)) {
      return 4;
    }
    if (((((((uint)param_1[1] < 9) ||
           (iVar1 = _strnicmp((char *)*param_1,"dhstoidm:",9), iVar1 != 0)) &&
          (((uint)param_1[1] < 10 ||
           (iVar1 = _strnicmp((char *)*param_1,"dhs2toidm:",10), iVar1 != 0)))) &&
         ((((uint)param_1[1] < 9 || (iVar1 = _strnicmp((char *)*param_1,"hlstoidm:",9), iVar1 != 0))
          && (((uint)param_1[1] < 10 ||
              (iVar1 = _strnicmp((char *)*param_1,"hls2toidm:",10), iVar1 != 0)))))) &&
        ((((uint)param_1[1] < 9 || (iVar1 = _strnicmp((char *)*param_1,"ytatoidm:",9), iVar1 != 0))
         && (((uint)param_1[1] < 10 ||
             (iVar1 = _strnicmp((char *)*param_1,"yta2toidm:",10), iVar1 != 0)))))) &&
       (((uint)param_1[1] < 10 || (iVar1 = _strnicmp((char *)*param_1,"yta3toidm:",10), iVar1 != 0))
       )) {
      return 0;
    }
  }
  return 1;
}
```

### Function `[FUN_10019660](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMNetMon_dll/FUN_10019660_10019660.c)`

- **Entry Point**: `0x10019660`
- **Signature**: `undefined FUN_10019660(void)`
- **Callers** (6): `FUN_10024b20` (0x10024b20), `FUN_10021450` (0x10021450), `FUN_10037770` (0x10037770), `FUN_100215b0` (0x100215b0), `FUN_10024f40` (0x10024f40), `FUN_10024c50` (0x10024c50)
- **Callees** (6): `_wcsnicmp` (0xEXTERNAL:0000001b), `FUN_10006cf0` (0x10006cf0), `memcpy` (0x1005e36d), `malloc` (0xEXTERNAL:00000021), `__RTDynamicCast` (0x1005e3af), `FUN_10010080` (0x10010080)

```c

int * __thiscall
FUN_10019660(int param_1,int *param_2,int param_3,int param_4,uint param_5,char param_6)

{
  uint uVar1;
  uint uVar2;
  void *_Src;
  undefined4 *puVar3;
  bool bVar4;
  void *pvVar5;
  int iVar6;
  void *_Dst;
  
  uVar2 = *(uint *)(param_1 + 0x116c);
  iVar6 = *(int *)(param_3 + 0x230);
  _Src = *(void **)(param_3 + 0x22c);
  *(undefined2 *)(param_2 + 3) = 0x1ff;
  if (iVar6 < 1) {
LAB_100196cd:
    *param_2 = (int)&DAT_10074bf4;
    param_2[2] = 0;
    param_2[1] = 0;
  }
  else {
    param_2[2] = iVar6 * 2 + 0x20;
    pvVar5 = malloc(iVar6 * 2 + 0x24);
    _Dst = (void *)((int)pvVar5 + 4);
    if (pvVar5 == (void *)0x0) {
      _Dst = (void *)0x0;
    }
    *param_2 = (int)_Dst;
    if (_Dst == (void *)0x0) goto LAB_100196cd;
    param_2[1] = iVar6;
    *(undefined2 *)(iVar6 * 2 + (int)_Dst) = 0;
    memcpy(_Dst,_Src,iVar6 * 2);
  }
  if ((param_5 == 0) && (param_4 != 0)) {
    param_5 = *(uint *)(param_4 + 0x94);
  }
  iVar6 = __RTDynamicCast(param_3,0,&BasicClient::RTTI_Type_Descriptor,
                          &PluginClient::RTTI_Type_Descriptor,0);
  if ((((iVar6 != 0) && (-1 < param_2[1] + -10)) &&
      (iVar6 = _wcsnicmp((wchar_t *)(*param_2 + (param_2[1] + -10) * 2),L"chrome.exe",10),
      iVar6 == 0)) && (uVar1 = param_2[1] - 4, uVar1 < (uint)param_2[1])) {
    *(undefined2 *)(*param_2 + uVar1 * 2) = 0x2a;
  }
  if (0x6290fff < uVar2) goto LAB_100197f7;
  if ((param_6 != '\0') || ((param_5 & 0x100) != 0)) {
    if (((uint)param_2[2] < 0x1a) && (iVar6 = FUN_10006cf0(0x1a), iVar6 == 0)) {
      param_2[1] = 0;
      *(undefined2 *)*param_2 = 0;
    }
    else {
      puVar3 = (undefined4 *)*param_2;
      param_2[1] = 0xc;
      *(undefined2 *)(puVar3 + 6) = 0;
      *puVar3 = DAT_1006b6d4;
      puVar3[1] = DAT_1006b6d8;
      puVar3[2] = DAT_1006b6dc;
      puVar3[3] = DAT_1006b6e0;
      puVar3[4] = DAT_1006b6e4;
      puVar3[5] = DAT_1006b6e8;
    }
  }
  if (param_2[1] + -0xb < 0) {
LAB_100197ee:
    bVar4 = true;
  }
  else {
    iVar6 = _wcsnicmp((wchar_t *)(*param_2 + (param_2[1] + -0xb) * 2),L"firefox.exe",0xb);
    bVar4 = false;
    if (iVar6 != 0) goto LAB_100197ee;
  }
  if (!bVar4) {
    return param_2;
  }
LAB_100197f7:
  FUN_10010080(param_2,L"|%hu:%hu:%hu:%hu:%u:%u",0x2d,*(undefined2 *)(DAT_10074c4c + 0x1b30),
               *(undefined2 *)(DAT_10074c4c + 0x1b32),*(undefined2 *)(param_3 + 0x16),0,
// ... [Truncated in summary - Full code in FUN_10019660_10019660.c (83 lines)]
```

### Function `[FUN_10010480](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMNetMon_dll/FUN_10010480_10010480.c)`

- **Entry Point**: `0x10010480`
- **Signature**: `undefined FUN_10010480(void)`
- **Callers** (1): `FUN_10010390` (0x10010390)
- **Callees** (30): `FUN_100100b0` (0x100100b0), `FUN_10007720` (0x10007720), `__alldiv` (0x10002230), `_strnicmp` (0xEXTERNAL:00000032), `FUN_10042fb0` (0x10042fb0), `malloc` (0xEXTERNAL:00000021), `FUN_10006d50` (0x10006d50), `FUN_10006420` (0x10006420), `FUN_10006950` (0x10006950), `FUN_1002e380` (0x1002e380)... [20 more]

```c

void __fastcall FUN_10010480(uint param_1,char *param_2,char param_3)

{
  byte bVar1;
  byte *pbVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  uint uVar6;
  char cVar7;
  ushort uVar8;
  int iVar9;
  undefined4 uVar10;
  int *piVar11;
  int *piVar12;
  undefined4 uVar13;
  undefined4 *puVar14;
  size_t sVar15;
  void *pvVar16;
  uint uVar17;
  int *piVar18;
  byte *pbVar19;
  undefined *puVar20;
  undefined *puVar21;
  byte bVar22;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 extraout_ECX_02;
  undefined4 extraout_ECX_03;
  int extraout_ECX_04;
  int extraout_ECX_05;
  int extraout_ECX_06;
  int extraout_ECX_07;
  int extraout_ECX_08;
  undefined4 extraout_ECX_09;
  undefined4 extraout_ECX_10;
  undefined4 extraout_ECX_11;
  int extraout_ECX_12;
  int extraout_ECX_13;
  int extraout_ECX_14;
  undefined4 extraout_ECX_15;
  undefined4 extraout_ECX_16;
  undefined4 extraout_ECX_17;
  int extraout_ECX_18;
  int extraout_ECX_19;
  undefined4 extraout_ECX_20;
  undefined4 extraout_ECX_21;
  undefined4 extraout_ECX_22;
  undefined4 extraout_ECX_23;
  undefined4 extraout_ECX_24;
  ushort uVar23;
  undefined3 uVar24;
  uint3 uVar25;
  int iVar26;
  int *piVar27;
  char *pcVar28;
  longlong lVar29;
  undefined8 uVar30;
  longlong lVar31;
  char local_1c5;
  byte local_1c4;
  byte local_1c3;
  uint local_1c0;
  char local_1b5;
  int *local_1b4;
  uint local_1b0;
  int local_1ac;
  int *local_1a8;
  int *local_1a4;
  char *local_1a0;
  int *local_19c;
  uint local_198;
  ushort local_194;
  undefined4 *local_190;
  size_t local_18c;
  size_t local_188;
  undefined2 local_184;
  int local_180;
// ... [Truncated in summary - Full code in FUN_10010480_10010480.c (986 lines)]
```

### Function `[FUN_10013d30](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMNetMon_dll/FUN_10013d30_10013d30.c)`

- **Entry Point**: `0x10013d30`
- **Signature**: `undefined FUN_10013d30(void)`
- **Callers** (1): `FUN_10013280` (0x10013280)
- **Callees** (10): `FUN_10008610` (0x10008610), `FUN_10030850` (0x10030850), `FUN_10017d60` (0x10017d60), `FUN_10012770` (0x10012770), `socket` (0xEXTERNAL:0000000b), `WSACreateEvent` (0xEXTERNAL:0000000c), `WSAStartup` (0xEXTERNAL:0000000d), `CreateEventW` (0xEXTERNAL:00000088), `ProcessIdToSessionId` (0xEXTERNAL:00000089), `GetCurrentProcessId` (0xEXTERNAL:000000a0)... [0 more]

```c

undefined4 __fastcall FUN_10013d30(int param_1)

{
  char cVar1;
  int iVar2;
  DWORD dwProcessId;
  BOOL BVar3;
  HANDLE pvVar4;
  SOCKET SVar5;
  undefined4 extraout_ECX;
  DWORD *pSessionId;
  WSADATA local_194;
  
  iVar2 = FUN_10008610();
  if (iVar2 != 0) {
    pSessionId = (DWORD *)(param_1 + 0x1b3c);
    dwProcessId = GetCurrentProcessId();
    BVar3 = ProcessIdToSessionId(dwProcessId,pSessionId);
    if (BVar3 != 0) {
      FUN_10012770(param_1 + 2,0x40,L"\\\\.\\pipe\\IDMNetworkMonitor.%u",
                   *(DWORD *)(param_1 + 0x1b3c));
      pvVar4 = CreateEventW((LPSECURITY_ATTRIBUTES)0x0,1,0,(LPCWSTR)0x0);
      *(HANDLE *)(param_1 + 0x1be4) = pvVar4;
      if (pvVar4 != (HANDLE)0x0) {
        pvVar4 = CreateEventW((LPSECURITY_ATTRIBUTES)0x0,1,0,(LPCWSTR)0x0);
        *(HANDLE *)(param_1 + 0x1be8) = pvVar4;
        if (pvVar4 != (HANDLE)0x0) {
          pvVar4 = CreateEventW((LPSECURITY_ATTRIBUTES)0x0,1,0,(LPCWSTR)0x0);
          *(HANDLE *)(param_1 + 0x1ccc) = pvVar4;
          if (pvVar4 != (HANDLE)0x0) {
            pvVar4 = CreateEventW((LPSECURITY_ATTRIBUTES)0x0,1,0,(LPCWSTR)0x0);
            *(HANDLE *)(param_1 + 0x1cf8) = pvVar4;
            if (pvVar4 != (HANDLE)0x0) {
              iVar2 = WSAStartup(0x202,&local_194);
              if (iVar2 == 0) {
                *(undefined1 *)(param_1 + 0x1cd4) = 1;
                iVar2 = WSACreateEvent();
                *(int *)(param_1 + 0x1cdc) = iVar2;
                if (iVar2 != 0) {
                  SVar5 = socket(2,1,6);
                  *(SOCKET *)(param_1 + 0x1cd8) = SVar5;
                  if (SVar5 != 0xffffffff) {
                    cVar1 = FUN_10030850();
                    if (cVar1 != '\0') {
                      FUN_10017d60(extraout_ECX);
                      if (DAT_10074c8c != (code *)0x0) {
                        (*DAT_10074c8c)(*(undefined4 *)(param_1 + 0x1ccc),param_1 + 0x1cd0);
                      }
                      return 1;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return 0;
}
```

### Function `[FUN_1003f260](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMNetMon_dll/FUN_1003f260_1003f260.c)`

- **Entry Point**: `0x1003f260`
- **Signature**: `undefined FUN_1003f260(void)`
- **Callers** (2): `FUN_1003f190` (0x1003f190), `FUN_1001c4d0` (0x1001c4d0)
- **Callees** (19): `FUN_100065a0` (0x100065a0), `strlen` (0xEXTERNAL:00000022), `FUN_1000cfd0` (0x1000cfd0), `__RTDynamicCast` (0x1005e3af), `FUN_10007720` (0x10007720), `FUN_10010080` (0x10010080), `free` (0xEXTERNAL:00000026), `FUN_1003ff80` (0x1003ff80), `FUN_10042fb0` (0x10042fb0), `_strnicmp` (0xEXTERNAL:00000032)... [9 more]

```c

undefined1 __thiscall
FUN_1003f260(int *param_1,size_t param_2,undefined4 *param_3,char *param_4,char param_5)

{
  int *piVar1;
  char *pcVar2;
  int *piVar3;
  int iVar4;
  byte bVar5;
  char cVar6;
  char cVar7;
  int iVar8;
  uint uVar9;
  int iVar10;
  size_t sVar11;
  void *pvVar12;
  undefined4 *puVar13;
  byte *pbVar14;
  undefined4 *puVar15;
  undefined4 uVar16;
  uint uVar17;
  char *pcVar18;
  char *pcVar19;
  undefined *puVar20;
  char *extraout_ECX;
  char *extraout_ECX_00;
  char *extraout_ECX_01;
  char *extraout_ECX_02;
  char *extraout_ECX_03;
  char *extraout_ECX_04;
  char *extraout_ECX_05;
  char *extraout_ECX_06;
  char *extraout_ECX_07;
  char *extraout_ECX_08;
  char *extraout_ECX_09;
  char *extraout_ECX_10;
  char *extraout_ECX_11;
  char *extraout_ECX_12;
  char *extraout_ECX_13;
  char *extraout_ECX_14;
  undefined *puVar21;
  char *extraout_ECX_15;
  byte bVar22;
  undefined1 uVar23;
  code *pcVar24;
  int unaff_ESI;
  byte *pbVar25;
  bool bVar26;
  bool bVar27;
  byte unaff_retaddr;
  undefined4 *puStack_88;
  char *pcStack_84;
  size_t local_80;
  uint uStack_7c;
  ushort uStack_78;
  int *local_74;
  undefined4 *puStack_70;
  char *pcStack_6c;
  size_t sStack_68;
  size_t sStack_64;
  uint local_60;
  char *pcStack_5c;
  uint uStack_58;
  undefined4 uStack_50;
  uint uStack_4c;
  undefined4 *local_48;
  uint local_44;
  undefined4 *local_40;
  undefined4 local_3c;
  size_t local_38;
  undefined2 uStack_34;
  size_t sStack_30;
  char **ppcStack_2c;
  uint uStack_28;
  undefined4 *puStack_24;
  undefined4 *puStack_c;
  char **ppcStack_4;
  
  local_38 = 0;
// ... [Truncated in summary - Full code in FUN_1003f260_1003f260.c (610 lines)]
```

### Function `[FUN_100324f0](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMNetMon_dll/FUN_100324f0_100324f0.c)`

- **Entry Point**: `0x100324f0`
- **Signature**: `undefined FUN_100324f0(void)`
- **Callers** (2): `FUN_10019190` (0x10019190), `FUN_10032250` (0x10032250)
- **Callees** (7): `GetModuleFileNameW` (0xEXTERNAL:0000009f), `LoadLibraryExW` (0xEXTERNAL:00000083), `FreeLibrary` (0xEXTERNAL:00000082), `GetModuleHandleW` (0xEXTERNAL:0000006a), `wcsrchr` (0xEXTERNAL:00000034), `GetLastError` (0xEXTERNAL:00000066), `GetProcAddress` (0xEXTERNAL:0000006b)

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool FUN_100324f0(void)

{
  HMODULE hModule;
  DWORD DVar1;
  wchar_t *pwVar2;
  wchar_t *pwVar3;
  FARPROC pFVar4;
  int iVar5;
  undefined4 extraout_EDX;
  WCHAR local_224 [274];
  
  if (DAT_10074c64 != (FARPROC)0x0) {
    return true;
  }
  hModule = DAT_10074c48;
  if (DAT_10074c48 == (HMODULE)0x0) {
    hModule = GetModuleHandleW((LPCWSTR)0x0);
  }
  pwVar3 = local_224;
  DVar1 = GetModuleFileNameW(hModule,local_224,0x104);
  if ((DVar1 != 0) && (DVar1 = GetLastError(), DVar1 != 0x7a)) {
    pwVar2 = wcsrchr(local_224,L'\\');
    if (pwVar2 != (wchar_t *)0x0) {
      pwVar3 = pwVar2 + 1;
    }
    pwVar2 = L"IDMBRBTN.DLL";
    for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
      *(undefined4 *)pwVar3 = *(undefined4 *)pwVar2;
      pwVar2 = pwVar2 + 2;
      pwVar3 = pwVar3 + 2;
    }
    *pwVar3 = *pwVar2;
    DAT_10074c74 = LoadLibraryExW(local_224,(HANDLE)0x0,DAT_10074c40);
    if (DAT_10074c74 != (HMODULE)0x0) {
      pFVar4 = GetProcAddress(DAT_10074c74,"GetVersionInfo");
      if (pFVar4 == (FARPROC)0x0) {
        _DAT_10074c68 = CONCAT22(DAT_10074c68_2,0x30);
      }
      else {
        _DAT_10074c68 = (*pFVar4)(0);
        _DAT_10074c6c = extraout_EDX;
      }
      DAT_10074c64 = GetProcAddress(DAT_10074c74,"CreateIDMButton3");
      if (DAT_10074c64 != (FARPROC)0x0) {
        DAT_10074c70 = GetProcAddress(DAT_10074c74,"CreateDownlSelWtIDMButton");
        return DAT_10074c64 != (FARPROC)0x0;
      }
      FreeLibrary(DAT_10074c74);
      DAT_10074c74 = (HMODULE)0x0;
    }
  }
  return DAT_10074c64 != (FARPROC)0x0;
}
```

### Function `[FUN_10034b40](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMNetMon_dll/FUN_10034b40_10034b40.c)`

- **Entry Point**: `0x10034b40`
- **Signature**: `undefined FUN_10034b40(void)`
- **Callers** (19): `FUN_1003b650` (0x1003b650), `FUN_1003b320` (0x1003b320), `FUN_1003b6c0` (0x1003b6c0), `FUN_10035cc0` (0x10035cc0), `FUN_1003abe0` (0x1003abe0), `FUN_10035b90` (0x10035b90), `FUN_1003b230` (0x1003b230), `FUN_1003a930` (0x1003a930), `FUN_1003b790` (0x1003b790), `FUN_1003b390` (0x1003b390)... [9 more]
- **Callees** (12): `FUN_10026110` (0x10026110), `memcpy` (0x1005e36d), `FUN_10004e40` (0x10004e40), `FUN_10033870` (0x10033870), `FUN_10003720` (0x10003720), `free` (0xEXTERNAL:00000026), `FUN_100334a0` (0x100334a0), `FUN_100333a0` (0x100333a0), `FUN_10005810` (0x10005810), `GetTickCount` (0xEXTERNAL:00000094)... [2 more]

```c

void __thiscall FUN_10034b40(int param_1,int *param_2)

{
  int *piVar1;
  void *pvVar2;
  size_t sVar3;
  u_short uVar4;
  uint uVar5;
  int iVar6;
  undefined4 uVar7;
  DWORD DVar8;
  uint *puVar9;
  int iVar10;
  undefined4 *puVar11;
  byte *pbVar12;
  uint *_Dst;
  undefined1 *puVar13;
  undefined4 *local_10;
  size_t local_c;
  undefined4 local_8;
  undefined2 local_4;
  
  if (((char)param_2[7] == '\0') || (param_2[8] == 0)) {
    iVar6 = *(int *)(param_1 + 0x318);
    *(int *)(param_1 + 0x318) = iVar6 + 1;
    if ((char)param_2[7] == '\0') {
      if (iVar6 != 0) {
        param_2[8] = iVar6;
      }
      uVar5 = param_2[0xd] * 0x15 + 0x31;
      if ((uint)param_2[2] < uVar5) {
        FUN_10005810(uVar5);
      }
      FUN_10004e40(param_2,"MSG#%u#%u#%u#%u",0,param_2[8],param_2[9],param_2[10],param_2[0xb]);
      uVar5 = 0;
      if (param_2[0xd] != 0) {
        pbVar12 = (byte *)(param_2 + 0xe);
        do {
          if ((param_2[1] + 2U <= (uint)param_2[2]) ||
             (iVar6 = FUN_10005810(param_2[1] + 2U), iVar6 != 0)) {
            *(undefined1 *)(param_2[1] + *param_2) = 0x3a;
            iVar6 = param_2[1];
            param_2[1] = iVar6 + 1;
            *(undefined1 *)(iVar6 + 1 + *param_2) = 0;
          }
          FUN_10026110(param_2,&DAT_10064e18 + (uint)*pbVar12 * 5,0x14,*(undefined4 *)(pbVar12 + 8),
                       *(undefined4 *)(pbVar12 + 0xc));
          uVar5 = uVar5 + 1;
          pbVar12 = pbVar12 + 0x10;
        } while (uVar5 < (uint)param_2[0xd]);
      }
      iVar6 = param_2[5];
      uVar5 = iVar6 + 1;
      if ((uVar5 <= (uint)param_2[6]) ||
         ((iVar6 = FUN_10003720(uVar5), iVar6 != 0 && (iVar6 = param_2[5], param_2[4] + iVar6 != 0))
         )) {
        *(undefined1 *)(iVar6 + param_2[4]) = 0x3b;
        param_2[5] = uVar5;
      }
      *(undefined1 *)(param_2 + 7) = 1;
    }
  }
  uVar5 = param_2[5] + param_2[1];
  if (*(char *)(param_1 + 0x30c) == '\x04') {
    local_10 = &DAT_10074bf4;
    local_c = 0;
    local_8 = 0;
    local_4 = 0;
    FUN_10005810(0xd2);
    uVar7 = 200;
    if ((*(byte *)(param_2 + 0xb) & 4) != 0) {
      uVar7 = 300;
    }
    FUN_10004e40(&local_10,"HTTP/1.0 %u IDM\r\n",0,uVar7);
    if (param_2[9] == 1) {
      FUN_10026110(&local_10,"X-IDM-Status: %llu\r\n",0,param_2[0x14],param_2[0x15]);
    }
    FUN_10026110(&local_10,
                 "Content-Type: text/plain\r\nContent-Length: %u\r\nAccess-Control-Allow-Origin: *\r\nCache-Control: no-cache\r\nPragma: no-cache\r\nConnection: close\r\n\r\n"
// ... [Truncated in summary - Full code in FUN_10034b40_10034b40.c (189 lines)]
```

