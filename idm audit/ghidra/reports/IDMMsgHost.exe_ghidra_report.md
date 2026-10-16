# Ghidra Analysis Report: IDMMsgHost.exe

- **Program**: `IDMMsgHost.exe`
- **Executable Path**: `/C:/Users/sayed/Downloads/IDM_RE_Investigation/binaries/IDMMsgHost.exe`
- **Language**: `x86:LE:32:default`
- **Compiler**: `windows`
- **Image Base**: `00400000`

## Targeted String Occurrences and Cross-References

### Pattern: `IDMMsgHost` (5 occurrence(s))

- **Found at Address**: `0x00404a6f`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.
- **Found at Address**: `0x00404a88`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.
- **Found at Address**: `0x00404ed0`
  - **Direct References** (1):
    - from `0x00404e8c` (Type: DATA) (No enclosing function)
- **Found at Address**: `0x004072b4`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.
- **Found at Address**: `0x004073bc`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.

### Pattern: `GetNamedPipeServerProcessId` (1 occurrence(s))

- **Found at Address**: `0x004046b4`
  - **Direct References** (1):
    - from `0x00402878` (Type: DATA) in Function `[FUN_00402830](file:///0x00402830)`

### Pattern: `GetNamedPipeClientProcessId` (1 occurrence(s))

- **Found at Address**: `0x004046d0`
  - **Direct References** (1):
    - from `0x00402880` (Type: DATA) in Function `[FUN_00402830](file:///0x00402830)`

### Pattern: `MSG#` (1 occurrence(s))

- **Found at Address**: `0x00404924`
  - **Direct References** (1):
    - from `0x004036a4` (Type: READ) in Function `[FUN_00403530](file:///0x00403530)`

### Pattern: `\\.\pipe\` (1 occurrence(s))

- **Found at Address**: `0x0040471c`
  - **Direct References** (2):
    - from `0x004028d9` (Type: READ) in Function `[FUN_00402830](file:///0x00402830)`
    - from `0x004028cc` (Type: DATA) in Function `[FUN_00402830](file:///0x00402830)`

## Decompiled Key Functions

Total unique functions identified for decompilation: 2

### Function `[FUN_00402830](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMMsgHost_exe/FUN_00402830_00402830.c)`

- **Entry Point**: `0x00402830`
- **Signature**: `undefined __stdcall FUN_00402830(void)`
- **Callers** (1): `___tmainCRTStartup` (0x00401487)
- **Callees** (29): `CreateSemaphoreW` (0xEXTERNAL:00000033), `LoadLibraryW` (0xEXTERNAL:00000038), `memset` (0x0040386a), `OpenMutexW` (0xEXTERNAL:00000032), `CloseHandle` (0xEXTERNAL:00000047), `GetLastError` (0xEXTERNAL:00000024), `RegCloseKey` (0xEXTERNAL:0000004f), `ProcessIdToSessionId` (0xEXTERNAL:0000003b), `GetStdHandle` (0xEXTERNAL:00000042), `CreateThread` (0xEXTERNAL:00000037)... [19 more]

```c

void FUN_00402830(void)

{
  uint uVar1;
  int *piVar2;
  HKEY pHVar3;
  HMODULE pHVar4;
  HANDLE pvVar5;
  DWORD DVar6;
  LSTATUS LVar7;
  HRESULT HVar8;
  BOOL BVar9;
  HANDLE pvVar10;
  DWORD DVar11;
  int iVar12;
  LPCVOID pvVar13;
  int iVar14;
  uint uVar15;
  HKEY pHVar16;
  HKEY pHVar17;
  wchar_t *pwVar18;
  HANDLE *ppvVar19;
  WCHAR *pWVar20;
  uint uVar21;
  int iVar22;
  uint *this;
  HKEY *ppHVar23;
  _OVERLAPPED *p_Var24;
  DWORD *pSessionId;
  uint uStack_22c;
  undefined1 local_218 [8];
  undefined1 local_210 [8];
  undefined1 local_208 [8];
  undefined1 local_200 [8];
  undefined1 local_1f8 [8];
  undefined1 local_1f0 [8];
  undefined1 local_1e8 [8];
  undefined1 local_1e0 [8];
  undefined1 local_1d8 [8];
  undefined1 local_1d0 [8];
  undefined1 local_1c8 [8];
  undefined1 local_1c0 [8];
  int local_1b8 [6];
  HANDLE local_1a0;
  uint local_19c;
  _OVERLAPPED local_198;
  undefined4 local_184;
  HANDLE local_180;
  HANDLE local_17c;
  undefined4 local_178;
  char local_174 [4];
  HANDLE local_170;
  uint local_16c;
  _OVERLAPPED local_168;
  undefined4 local_154;
  undefined4 local_150;
  undefined4 local_14c;
  undefined4 local_148;
  char local_144 [4];
  int *local_140;
  HANDLE local_13c;
  undefined4 local_138;
  undefined4 local_134;
  undefined4 local_130;
  undefined4 local_12c;
  undefined4 local_128;
  HANDLE local_124;
  undefined4 local_120;
  undefined4 local_11c;
  undefined4 local_118;
  undefined4 local_114;
  undefined2 local_110;
  HANDLE local_10c;
  undefined4 local_108;
  undefined4 local_104;
  undefined4 local_100;
  undefined4 local_fc;
  undefined4 local_f8;
  HANDLE local_f4;
// ... [Truncated in summary - Full code in FUN_00402830_00402830.c (487 lines)]
```

### Function `[FUN_00403530](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMMsgHost_exe/FUN_00403530_00403530.c)`

- **Entry Point**: `0x00403530`
- **Signature**: `undefined __fastcall FUN_00403530(int * param_1, undefined4 param_2)`
- **Callers** (1): `FUN_00402830` (0x00402830)
- **Callees** (9): `OpenProcess` (0xEXTERNAL:00000048), `free` (0xEXTERNAL:0000001a), `FUN_00401fb0` (0x00401fb0), `FUN_004020a0` (0x004020a0), `FUN_00401c70` (0x00401c70), `CloseHandle` (0xEXTERNAL:00000047), `FUN_00401e30` (0x00401e30), `FUN_00402300` (0x00402300), `_itoa` (0xEXTERNAL:00000013)

```c

void __fastcall FUN_00403530(int *param_1,undefined4 param_2)

{
  char cVar1;
  uint uVar2;
  DWORD dwProcessId;
  HANDLE hObject;
  uint uVar3;
  int iVar4;
  int iVar5;
  char *pcVar6;
  char *pcVar7;
  undefined4 *puVar8;
  undefined1 local_44 [20];
  DWORD local_30;
  undefined1 *local_2c;
  int local_28;
  int local_24;
  char local_20;
  undefined1 local_1f;
  undefined1 local_1a [2];
  DWORD local_18;
  DWORD local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_0040396d;
  local_10 = ExceptionList;
  uVar2 = DAT_00406000 ^ (uint)&stack0xfffffffc;
  ExceptionList = &local_10;
  dwProcessId = FUN_00402300();
  if ((DAT_004066ac == (code *)0x0) || (DAT_004066b0 == (code *)0x0)) {
    if ((DAT_004066b4 != (code *)0x0) && (DAT_004066a8 != (code *)0x0)) {
      FUN_00401e30((int *)&local_2c);
      local_1f = 1;
      local_8 = 0;
      hObject = OpenProcess(0x400,0,dwProcessId);
      if (hObject != (HANDLE)0x0) {
        if (local_24 == 0) {
          uVar3 = 0;
        }
        else {
          uVar3 = (local_24 - 1U) / ((local_20 < '\0') + 1);
        }
        uVar2 = (*DAT_004066b4)(hObject,local_2c,uVar3,uVar2);
        FUN_004020a0(&local_2c,uVar2);
        if (-1 < (int)(local_28 - 8U)) {
          uVar2 = FUN_00401fb0(&local_2c,L"\\cmd.exe",8,local_28 - 8U);
          if (uVar2 == 0) {
            iVar4 = (*DAT_004066a8)(hObject,0,local_44,0x18,0);
            if (iVar4 == 0) {
              dwProcessId = local_30;
            }
          }
        }
        CloseHandle(hObject);
      }
      if ((local_2c != local_1a) && (local_2c + -4 != (undefined1 *)0x0)) {
        free(local_2c + -4);
      }
    }
  }
  else {
    local_14 = 0;
    local_18 = 0;
    (*DAT_004066ac)(param_2,&local_18);
    (*DAT_004066b0)(param_2,&local_14);
    if ((dwProcessId == local_14) && (local_18 != 0)) {
      dwProcessId = local_18;
    }
  }
  iVar4 = FUN_00401c70(param_1,4);
  if (iVar4 != 0) {
    param_1[1] = param_1[1] + 4;
  }
  iVar4 = param_1[1];
  if ((uint)param_1[2] < iVar4 + 0xfU) {
// ... [Truncated in summary - Full code in FUN_00403530_00403530.c (120 lines)]
```

