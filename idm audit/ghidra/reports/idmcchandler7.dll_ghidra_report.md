# Ghidra Analysis Report: idmcchandler7.dll

- **Program**: `idmcchandler7.dll`
- **Executable Path**: `/C:/Users/sayed/Downloads/IDM_RE_Investigation/binaries/idmcchandler7.dll`
- **Language**: `x86:LE:32:default`
- **Compiler**: `windows`
- **Image Base**: `10000000`

## Targeted String Occurrences and Cross-References

### Pattern: `video/mp4` (1 occurrence(s))

- **Found at Address**: `0x1004d438`
  - **Direct References** (2):
    - from `0x10017232` (Type: DATA) in Function `[FUN_10016f10](file:///0x10016f10)`
    - from `0x1001174a` (Type: DATA) in Function `[FUN_10011550](file:///0x10011550)`

### Pattern: `video/webm` (1 occurrence(s))

- **Found at Address**: `0x1004e740`
  - **Direct References** (1):
    - from `0x100174d9` (Type: DATA) in Function `[FUN_10016f10](file:///0x10016f10)`

### Pattern: `application/dash+xml` (1 occurrence(s))

- **Found at Address**: `0x1004fd3c`
  - **Direct References** (1):
    - from `0x100398cd` (Type: DATA) in Function `[FUN_10035d44](file:///0x10035d44)`

### Pattern: `application/vnd.apple.mpegurl` (1 occurrence(s))

- **Found at Address**: `0x1004e6dc`
  - **Direct References** (2):
    - from `0x10017564` (Type: DATA) in Function `[FUN_10016f10](file:///0x10016f10)`
    - from `0x100399ba` (Type: DATA) in Function `[FUN_10035d44](file:///0x10035d44)`

### Pattern: `mpdatoidm` (1 occurrence(s))

- **Found at Address**: `0x1004cde8`
  - **Direct References** (4):
    - from `0x100234a6` (Type: DATA) in Function `[FUN_100230a0](file:///0x100230a0)`
    - from `0x1000cf89` (Type: DATA) in Function `[FUN_1000b56a](file:///0x1000b56a)`
    - from `0x1000bc99` (Type: DATA) in Function `[FUN_1000b56a](file:///0x1000b56a)`
    - from `0x10023494` (Type: DATA) in Function `[FUN_100230a0](file:///0x100230a0)`

### Pattern: `mpdtoidm` (1 occurrence(s))

- **Found at Address**: `0x1004ce04`
  - **Direct References** (4):
    - from `0x100234a6` (Type: DATA) in Function `[FUN_100230a0](file:///0x100230a0)`
    - from `0x1000cf6b` (Type: DATA) in Function `[FUN_1000b56a](file:///0x1000b56a)`
    - from `0x1000bc7b` (Type: DATA) in Function `[FUN_1000b56a](file:///0x1000b56a)`
    - from `0x1002349b` (Type: DATA) in Function `[FUN_100230a0](file:///0x100230a0)`

### Pattern: `hlstoidm` (1 occurrence(s))

- **Found at Address**: `0x1004ce2c`
  - **Direct References** (3):
    - from `0x1002a6db` (Type: DATA) in Function `[FUN_1002a250](file:///0x1002a250)`
    - from `0x1000bbbd` (Type: READ) in Function `[FUN_1000b56a](file:///0x1000b56a)`
    - from `0x1000bbaf` (Type: DATA) in Function `[FUN_1000b56a](file:///0x1000b56a)`

### Pattern: `hls2toidm` (1 occurrence(s))

- **Found at Address**: `0x1004ce1c`
  - **Direct References** (3):
    - from `0x1002a70a` (Type: DATA) in Function `[FUN_1002a250](file:///0x1002a250)`
    - from `0x1000bc27` (Type: READ) in Function `[FUN_1000b56a](file:///0x1000b56a)`
    - from `0x1000bc13` (Type: DATA) in Function `[FUN_1000b56a](file:///0x1000b56a)`

### Pattern: `IDM_Download_Panel_controller2_class` (1 occurrence(s))

- **Found at Address**: `0x1004d304`
  - **Direct References** (2):
    - from `0x1000e9fd` (Type: DATA) in Function `[FUN_1000e990](file:///0x1000e990)`
    - from `0x1000f7e7` (Type: DATA) in Function `[FUN_1000f780](file:///0x1000f780)`

### Pattern: `idmbrbtn` (2 occurrence(s))

- **Found at Address**: `0x1004cd5b`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.
- **Found at Address**: `0x1004cd3e`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.

### Pattern: `CreateIDMButton` (3 occurrence(s))

- **Found at Address**: `0x1004d2bc`
  - **Direct References** (1):
    - from `0x10008b34` (Type: DATA) in Function `[FUN_100089f0](file:///0x100089f0)`
- **Found at Address**: `0x1004d2cc`
  - **Direct References** (1):
    - from `0x10008b21` (Type: DATA) in Function `[FUN_100089f0](file:///0x100089f0)`
- **Found at Address**: `0x1004d2e0`
  - **Direct References** (1):
    - from `0x10008b0e` (Type: DATA) in Function `[FUN_100089f0](file:///0x100089f0)`

### Pattern: `CreateIDMButton2` (1 occurrence(s))

- **Found at Address**: `0x1004d2cc`
  - **Direct References** (1):
    - from `0x10008b21` (Type: DATA) in Function `[FUN_100089f0](file:///0x100089f0)`

### Pattern: `CreateIDMButton3` (1 occurrence(s))

- **Found at Address**: `0x1004d2e0`
  - **Direct References** (1):
    - from `0x10008b0e` (Type: DATA) in Function `[FUN_100089f0](file:///0x100089f0)`

### Pattern: `SetIDMButtonValue3` (1 occurrence(s))

- **Found at Address**: `0x1004d2a8`
  - **Direct References** (1):
    - from `0x10008b47` (Type: DATA) in Function `[FUN_100089f0](file:///0x100089f0)`

### Pattern: `CreateDownlSelWtIDMButton` (1 occurrence(s))

- **Found at Address**: `0x1004cd1c`
  - **Direct References** (1):
    - from `0x10007560` (Type: DATA) in Function `[FUN_10007170](file:///0x10007170)`

## Decompiled Key Functions

Total unique functions identified for decompilation: 10

### Function `[FUN_100230a0](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/idmcchandler7_dll/FUN_100230a0_100230a0.c)`

- **Entry Point**: `0x100230a0`
- **Signature**: `undefined __fastcall FUN_100230a0(int param_1, int param_2)`
- **Callers** (1): `FUN_1002ba00` (0x1002ba00)
- **Callees** (26): `_memicmp` (0xEXTERNAL:00000036), `operator_new` (0x10044b98), `wcsrchr` (0xEXTERNAL:00000033), `FUN_10001090` (0x10001090), `memcpy` (0x10044b8c), `GetProcAddress` (0xEXTERNAL:00000043), `RegQueryValueExW` (0xEXTERNAL:0000009d), `FUN_10003910` (0x10003910), `LoadLibraryW` (0xEXTERNAL:00000045), `SysAllocString` (0xEXTERNAL:000000af)... [16 more]

```c

void __fastcall FUN_100230a0(int param_1,int param_2)

{
  char cVar1;
  void *pvVar2;
  int iVar3;
  LSTATUS LVar4;
  wchar_t *pwVar5;
  LPCSTR _Dst;
  undefined4 *puVar6;
  UINT UVar7;
  BSTR pOVar8;
  int iVar9;
  int *piVar10;
  int extraout_EDX;
  int extraout_EDX_00;
  int extraout_EDX_01;
  int extraout_EDX_02;
  int extraout_EDX_03;
  int extraout_EDX_04;
  int extraout_EDX_05;
  int extraout_EDX_06;
  int extraout_EDX_07;
  BSTR bstrString;
  int iVar11;
  char *pcVar12;
  undefined4 *puVar13;
  bool bVar14;
  char *pcVar15;
  void **ppvVar16;
  BSTR *ppOVar17;
  BSTR local_4c8;
  int *local_4c4;
  BSTR pOStack_4c0;
  uint uStack_4bc;
  undefined4 *puStack_4b8;
  int iStack_4b4;
  BSTR pOStack_4b0;
  int iStack_4ac;
  int iStack_4a8;
  int iStack_4a4;
  int iStack_4a0;
  void *pvStack_49c;
  int iStack_498;
  int iStack_494;
  BSTR pOStack_490;
  int iStack_48c;
  int iStack_488;
  LPCWSTR pWStack_484;
  int iStack_480;
  wchar_t *pwStack_47c;
  int iStack_478;
  LPCSTR pCStack_474;
  LPCSTR local_470;
  undefined1 auStack_46c [16];
  undefined1 auStack_45c [16];
  undefined1 auStack_44c [4];
  BSTR pOStack_448;
  undefined1 auStack_444 [3];
  undefined4 uStack_441;
  wchar_t local_420 [260];
  wchar_t local_218 [260];
  uint local_10;
  void *local_c;
  undefined1 *puStack_8;
  int iStack_4;
  
  iStack_4 = 0xffffffff;
  puStack_8 = &LAB_10048c0b;
  local_c = ExceptionList;
  local_10 = DAT_1005a88c ^ (uint)&local_4c8;
  ExceptionList = &local_c;
  if (9 < *(int *)(param_1 + 0x22c)) {
    pvVar2 = memchr(*(void **)(param_1 + 0x228),0x3c,*(int *)(param_1 + 0x22c) - 5);
    param_2 = extraout_EDX;
    while (pvVar2 != (void *)0x0) {
      pcVar12 = (char *)((int)pvVar2 + 1);
      iVar11 = (int)pcVar12 - *(int *)(param_1 + 0x228);
      if (*pcVar12 == '/') {
// ... [Truncated in summary - Full code in FUN_100230a0_100230a0.c (374 lines)]
```

### Function `[FUN_1002a250](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/idmcchandler7_dll/FUN_1002a250_1002a250.c)`

- **Entry Point**: `0x1002a250`
- **Signature**: `undefined __thiscall FUN_1002a250(void * this, int param_1)`
- **Callers** (2): `FUN_100356e0` (0x100356e0), `FUN_1002ba00` (0x1002ba00)
- **Callees** (29): `FUN_1003f4d0` (0x1003f4d0), `sscanf` (0xEXTERNAL:0000001b), `strncmp` (0xEXTERNAL:00000007), `SysStringLen` (0xEXTERNAL:000000b1), `FUN_10018fa0` (0x10018fa0), `operator_new` (0x10044b98), `FUN_100030c0` (0x100030c0), `FUN_10017f20` (0x10017f20), `FUN_10017f80` (0x10017f80), `FUN_100251a0` (0x100251a0)... [19 more]

```c

void __thiscall FUN_1002a250(void *this,int param_1)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  int iVar4;
  int *piVar5;
  char *pcVar6;
  LPCSTR pCVar7;
  UINT UVar8;
  undefined1 *puVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 uVar13;
  BSTR bstrString;
  BSTR pOStack_2b0;
  int *piStack_2ac;
  int **local_2a8;
  int *piStack_2a4;
  wchar_t *pwStack_2a0;
  uint uStack_29c;
  BSTR local_298;
  undefined1 *puStack_294;
  int iStack_290;
  int iStack_28c;
  int iStack_288;
  undefined *puStack_284;
  int iStack_280;
  int iStack_27c;
  char *pcStack_278;
  int iStack_274;
  int iStack_270;
  int iStack_26c;
  int iStack_268;
  int iStack_264;
  int iStack_260;
  int iStack_25c;
  wchar_t *pwStack_258;
  uint uStack_254;
  int iStack_250;
  int iStack_24c;
  LPCWSTR pWStack_248;
  void *pvStack_244;
  int iStack_240;
  int iStack_23c;
  char *pcStack_238;
  int iStack_234;
  int iStack_230;
  int iStack_22c;
  int iStack_228;
  int iStack_224;
  int iStack_220;
  int iStack_21c;
  int iStack_218;
  int iStack_214;
  char *pcStack_210;
  int iStack_20c;
  int iStack_208;
  int *piStack_204;
  uint *puStack_200;
  int iStack_1fc;
  LPCWSTR pWStack_1f8;
  int iStack_1f4;
  wchar_t *pwStack_1f0;
  int iStack_1ec;
  undefined1 auStack_1e8 [16];
  undefined1 auStack_1d8 [16];
  undefined1 auStack_1c8 [16];
  undefined1 auStack_1b8 [16];
  undefined1 auStack_1a8 [4];
  undefined1 auStack_1a4 [4];
  undefined1 auStack_1a0 [4];
  undefined1 auStack_19c [4];
  undefined1 auStack_198 [4];
  undefined1 auStack_194 [4];
// ... [Truncated in summary - Full code in FUN_1002a250_1002a250.c (605 lines)]
```

### Function `[FUN_10007170](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/idmcchandler7_dll/FUN_10007170_10007170.c)`

- **Entry Point**: `0x10007170`
- **Signature**: `undefined __thiscall FUN_10007170(void * this, undefined4 param_1, int param_2, undefined4 param_3, int param_4, int param_5, int param_6, int param_7)`
- **Callers** (1): `FUN_1002fff0` (0x1002fff0)
- **Callees** (25): `GetProcAddress` (0xEXTERNAL:00000043), `FUN_10018fa0` (0x10018fa0), `operator_new` (0x10044b98), `FUN_10017f20` (0x10017f20), `_snwprintf` (0xEXTERNAL:00000029), `RegQueryValueExA` (0xEXTERNAL:0000009e), `ScreenToClient` (0xEXTERNAL:00000091), `FUN_10003b40` (0x10003b40), `FUN_10005b20` (0x10005b20), `LoadLibraryW` (0xEXTERNAL:00000045)... [15 more]

```c

void __thiscall
FUN_10007170(void *this,undefined4 param_1,int param_2,undefined4 param_3,int param_4,int param_5,
            int param_6,int param_7)

{
  int *piVar1;
  RECT *lpRect;
  int ***pppiVar2;
  undefined4 *puVar3;
  POINT pt;
  POINT pt_00;
  BOOL BVar4;
  LSTATUS LVar5;
  char *pcVar6;
  wchar_t *pwVar7;
  int *piVar8;
  int iVar9;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 extraout_ECX_02;
  undefined4 extraout_ECX_03;
  undefined4 extraout_ECX_04;
  undefined4 extraout_ECX_05;
  undefined4 uVar10;
  undefined4 extraout_ECX_06;
  undefined4 extraout_ECX_07;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 extraout_EDX_01;
  undefined4 extraout_EDX_02;
  undefined4 extraout_EDX_03;
  undefined4 extraout_EDX_04;
  undefined4 extraout_EDX_05;
  undefined4 uVar11;
  undefined4 extraout_EDX_06;
  undefined4 extraout_EDX_07;
  uint uStack_530;
  undefined1 local_520 [16];
  undefined1 local_510 [16];
  undefined4 local_500;
  int local_4fc;
  int **local_4f8;
  void *local_4f4;
  undefined1 local_4f0 [8];
  int ***local_4e8;
  uint *local_4e4;
  int ***local_4e0;
  int **local_4dc;
  int ***local_4d8;
  int *local_4d4;
  int ***local_4d0;
  int **local_4cc;
  int *local_4c8;
  int *local_4c4;
  uint local_4c0;
  undefined1 *local_4bc;
  void *local_4b8;
  undefined1 *puStack_4b4;
  undefined4 local_4b0;
  _OSVERSIONINFOA local_4ac;
  wchar_t local_418 [130];
  BYTE local_314 [260];
  wchar_t local_210 [130];
  char local_10c [260];
  uint local_8;
  
  puStack_4b4 = &LAB_10045faf;
  local_4b8 = ExceptionList;
  uStack_530 = DAT_1005a88c ^ (uint)&local_4ac;
  local_4bc = (undefined1 *)&uStack_530;
  ExceptionList = &local_4b8;
  *(undefined ***)this = CDownloadMediaBtn::vftable;
  piVar1 = (int *)((int)this + 0x34);
  *piVar1 = 0;
  local_4e4 = (uint *)((int)this + 0x44);
  *local_4e4 = 0;
  local_4b0._0_1_ = 3;
  local_4b0._1_3_ = 0;
// ... [Truncated in summary - Full code in FUN_10007170_10007170.c (362 lines)]
```

### Function `[FUN_10016f10](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/idmcchandler7_dll/FUN_10016f10_10016f10.c)`

- **Entry Point**: `0x10016f10`
- **Signature**: `undefined4 __cdecl FUN_10016f10(void * param_1, void * param_2)`
- **Callers** (3): `FUN_1001b0b0` (0x1001b0b0), `FUN_10035d44` (0x10035d44), `FUN_10031090` (0x10031090)
- **Callees** (2): `FUN_10001090` (0x10001090), `_memicmp` (0xEXTERNAL:00000036)

```c

undefined4 __cdecl FUN_10016f10(void *param_1,void *param_2)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  
  (*DAT_1005abfc)(param_2,&DAT_1004cdda,0xffffffff);
  iVar1 = _memicmp(param_1,"application/octet-stream",0x18);
  if (iVar1 != 0) {
    iVar1 = _memicmp(param_1,"binary/octet-stream",0x13);
    if (iVar1 != 0) {
      iVar1 = _memicmp(param_1,"application/x-msdos-program",0x1b);
      if (iVar1 != 0) {
        iVar1 = _memicmp(param_1,"application/x-sdlc",0x12);
        if (iVar1 != 0) {
          iVar1 = _memicmp(param_1,"application/x-msi",0x11);
          if (iVar1 == 0) {
            uVar2 = FUN_10001090(param_2,&DAT_1004ead8);
            return CONCAT31((int3)((uint)uVar2 >> 8),1);
          }
          iVar1 = _memicmp(param_1,"application/zip",0xf);
          if (iVar1 != 0) {
            iVar1 = _memicmp(param_1,"application/x-zip-compressed",0x1c);
            if (iVar1 != 0) {
              iVar1 = _memicmp(param_1,"application/x-rar-compressed",0x1c);
              if (iVar1 != 0) {
                iVar1 = _memicmp(param_1,"application/x-rar",0x11);
                if (iVar1 != 0) {
                  iVar1 = _memicmp(param_1,"application/x-gtar",0x12);
                  if (iVar1 != 0) {
                    iVar1 = _memicmp(param_1,"application/x-tar",0x11);
                    if (iVar1 != 0) {
                      iVar1 = _memicmp(param_1,"application/x-compressed",0x18);
                      if (iVar1 == 0) {
                        uVar2 = FUN_10001090(param_2,&DAT_1004ea20);
                        return CONCAT31((int3)((uint)uVar2 >> 8),1);
                      }
                      iVar1 = _memicmp(param_1,"application/x-compress",0x16);
                      if (iVar1 == 0) {
                        uVar2 = FUN_10001090(param_2,&DAT_1004ea04);
                        return CONCAT31((int3)((uint)uVar2 >> 8),1);
                      }
                      iVar1 = _memicmp(param_1,"application/x-gzip",0x12);
                      if (iVar1 != 0) {
                        iVar1 = _memicmp(param_1,"application/gzip",0x10);
                        if (iVar1 != 0) {
                          iVar1 = _memicmp(param_1,"application/msword",0x12);
                          if (iVar1 == 0) {
                            uVar2 = FUN_10001090(param_2,&DAT_1004e9c0);
                            return CONCAT31((int3)((uint)uVar2 >> 8),1);
                          }
                          iVar1 = _memicmp(param_1,"application/x-aim",0x11);
                          if (iVar1 == 0) {
                            uVar2 = FUN_10001090(param_2,&DAT_1004e9a8);
                            return CONCAT31((int3)((uint)uVar2 >> 8),1);
                          }
                          iVar1 = _memicmp(param_1,"video/msvideo",0xd);
                          if (iVar1 != 0) {
                            iVar1 = _memicmp(param_1,"video/x-msvideo",0xf);
                            if (iVar1 != 0) {
                              iVar1 = _memicmp(param_1,"video/avi",9);
                              if (iVar1 != 0) {
                                iVar1 = _memicmp(param_1,"audio/mp3",9);
                                if (iVar1 != 0) {
                                  iVar1 = _memicmp(param_1,"audio/mp4",9);
                                  if (iVar1 == 0) {
                                    uVar2 = FUN_10001090(param_2,&DAT_1004e958);
                                    return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                  }
                                  iVar1 = _memicmp(param_1,"audio/mpeg",10);
                                  if (iVar1 != 0) {
                                    iVar1 = _memicmp(param_1,"audio/x-mpeg",0xc);
                                    if (iVar1 != 0) {
                                      iVar1 = _memicmp(param_1,"video/mpegts",0xc);
                                      if (iVar1 == 0) {
                                        uVar2 = FUN_10001090(param_2,&DAT_1004e928);
                                        return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                      }
// ... [Truncated in summary - Full code in FUN_10016f10_10016f10.c (276 lines)]
```

### Function `[FUN_1000b56a](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/idmcchandler7_dll/FUN_1000b56a_1000b56a.c)`

- **Entry Point**: `0x1000b56a`
- **Signature**: `undefined __stdcall FUN_1000b56a(void)`
- **Callers** (1): `FUN_10009fd4` (0x10009fd4)
- **Callees** (54): `FUN_10018fa0` (0x10018fa0), `operator_new` (0x10044b98), `_wcsicmp` (0xEXTERNAL:00000016), `WaitForSingleObject` (0xEXTERNAL:0000006c), `FUN_10041f40` (0x10041f40), `FUN_100055b0` (0x100055b0), `FUN_100038b0` (0x100038b0), `FUN_10007bb0` (0x10007bb0), `FUN_10007a60` (0x10007a60), `SysAllocString` (0xEXTERNAL:000000af)... [44 more]

```c

void FUN_1000b56a(void)

{
  char cVar1;
  ushort uVar2;
  short sVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined2 *puVar6;
  bool bVar7;
  int iVar8;
  void *pvVar9;
  ushort *puVar10;
  ushort *puVar11;
  short *psVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  char *pcVar15;
  char *pcVar16;
  char *pcVar17;
  char *pcVar18;
  undefined3 extraout_var;
  OLECHAR *pOVar19;
  DWORD DVar20;
  wchar_t *pwVar21;
  undefined2 *puVar22;
  undefined4 uVar23;
  undefined4 uVar24;
  undefined4 uVar25;
  undefined4 uVar26;
  undefined4 uVar27;
  undefined4 uVar28;
  undefined4 uVar29;
  undefined4 uVar30;
  BSTR pOVar31;
  LPCSTR pCVar32;
  int *piVar33;
  short *psVar34;
  int *piVar35;
  uint uVar36;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 extraout_EDX_01;
  int *piVar37;
  undefined4 extraout_EDX_02;
  undefined4 extraout_EDX_03;
  undefined4 extraout_EDX_04;
  undefined4 extraout_EDX_05;
  undefined4 extraout_EDX_06;
  undefined4 extraout_EDX_07;
  undefined4 extraout_EDX_08;
  undefined4 extraout_EDX_09;
  undefined4 extraout_EDX_10;
  undefined4 extraout_EDX_11;
  undefined4 extraout_EDX_12;
  undefined4 extraout_EDX_13;
  undefined4 extraout_EDX_14;
  undefined4 extraout_EDX_15;
  undefined4 extraout_EDX_16;
  undefined4 extraout_EDX_17;
  undefined4 extraout_EDX_18;
  undefined4 extraout_EDX_19;
  undefined4 extraout_EDX_20;
  undefined4 extraout_EDX_21;
  undefined4 extraout_EDX_22;
  undefined4 extraout_EDX_23;
  undefined4 extraout_EDX_24;
  undefined4 extraout_EDX_25;
  undefined4 extraout_EDX_26;
  undefined4 extraout_EDX_27;
  int unaff_EBX;
  uint unaff_EBP;
  HWND hWnd;
  code *pcVar38;
  char *pcVar39;
  int iVar40;
  undefined4 uStack0000000c;
  undefined4 uVar41;
  undefined4 uVar42;
// ... [Truncated in summary - Full code in FUN_1000b56a_1000b56a.c (1563 lines)]
```

### Function `[FUN_100089f0](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/idmcchandler7_dll/FUN_100089f0_100089f0.c)`

- **Entry Point**: `0x100089f0`
- **Signature**: `undefined FUN_100089f0(void)`
- **Callers** (1): `FUN_1000e0e0` (0x1000e0e0)
- **Callees** (55): `FUN_10018fa0` (0x10018fa0), `_wcsicmp` (0xEXTERNAL:00000016), `FUN_10042e80` (0x10042e80), `WaitForSingleObject` (0xEXTERNAL:0000006c), `FUN_100038b0` (0x100038b0), `wcsstr` (0xEXTERNAL:00000028), `FUN_10003b60` (0x10003b60), `SysAllocString` (0xEXTERNAL:000000af), `_wcslwr` (0xEXTERNAL:0000003c), `FUN_10005b20` (0x10005b20)... [45 more]

```c
// Decompilation failed or timed out.
```

### Function `[FUN_10035d44](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/idmcchandler7_dll/FUN_10035d44_10035d44.c)`

- **Entry Point**: `0x10035d44`
- **Signature**: `undefined __stdcall FUN_10035d44(void)`
- **Callers** (1): `FUN_10035940` (0x10035940)
- **Callees** (75): `sscanf` (0xEXTERNAL:0000001b), `FUN_10019e90` (0x10019e90), `FUN_10010a60` (0x10010a60), `strncmp` (0xEXTERNAL:00000007), `FUN_1001c6c0` (0x1001c6c0), `FUN_10018fa0` (0x10018fa0), `operator_new` (0x10044b98), `WaitForSingleObject` (0xEXTERNAL:0000006c), `isdigit` (0xEXTERNAL:00000009), `FUN_100038b0` (0x100038b0)... [65 more]

```c

void FUN_10035d44(void)

{
  byte bVar1;
  undefined4 *puVar2;
  char cVar3;
  DWORD DVar4;
  byte *pbVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  char *pcVar10;
  undefined4 uVar11;
  void *pvVar12;
  char *pcVar13;
  char *pcVar14;
  undefined1 *puVar15;
  wchar_t *pwVar16;
  byte *pbVar17;
  BSTR pOVar18;
  char *pcVar19;
  char *pcVar20;
  char **ppcVar21;
  byte **ppbVar22;
  int **ppiVar23;
  int iVar24;
  char ***pppcVar25;
  int iVar26;
  BSTR pOVar27;
  int *piVar28;
  undefined4 extraout_ECX;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  char *extraout_EDX_01;
  undefined4 extraout_EDX_02;
  undefined4 extraout_EDX_03;
  undefined1 unaff_BL;
  code *pcVar29;
  int *unaff_EBP;
  code *pcVar30;
  bool bVar31;
  undefined4 uStack0000000c;
  int iVar32;
  int iVar33;
  int iVar34;
  int iVar35;
  int iVar36;
  int iVar37;
  int iVar38;
  LPCWSTR pWVar39;
  undefined4 uVar40;
  undefined4 uVar41;
  BSTR pOVar42;
  int iVar43;
  short *psVar44;
  int iVar45;
  SAFEARRAY *pSVar46;
  WCHAR *pWVar47;
  int iVar48;
  int iVar49;
  int iVar50;
  uint uVar51;
  undefined4 uVar52;
  short *psVar53;
  undefined4 uVar54;
  int iVar55;
  OLECHAR *pOVar56;
  char *pcVar57;
  int iVar58;
  int iVar59;
  undefined4 uVar60;
  BSTR pOVar61;
  char *pcVar62;
  undefined4 uVar63;
  undefined4 *puVar64;
  int iVar65;
  int *piVar66;
  uint *puVar67;
// ... [Truncated in summary - Full code in FUN_10035d44_10035d44.c (3017 lines)]
```

### Function `[FUN_1000f780](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/idmcchandler7_dll/FUN_1000f780_1000f780.c)`

- **Entry Point**: `0x1000f780`
- **Signature**: `undefined4 __stdcall FUN_1000f780(void)`
- **Callers** (1): `FUN_1000f820` (0x1000f820)
- **Callees** (5): `RegisterClassA` (0xEXTERNAL:0000008b), `GetStockObject` (0xEXTERNAL:00000096), `LoadIconA` (0xEXTERNAL:00000089), `LoadCursorA` (0xEXTERNAL:0000007c), `GetLastError` (0xEXTERNAL:00000069)

```c

undefined4 FUN_1000f780(void)

{
  ATOM AVar1;
  DWORD DVar2;
  WNDCLASSA local_28;
  
  if (DAT_1005ac45 != '\0') {
    return 1;
  }
  local_28.style = 3;
  local_28.lpfnWndProc = (WNDPROC)&LAB_1000ec70;
  local_28.cbClsExtra = 0;
  local_28.cbWndExtra = 0;
  local_28.hInstance = (HINSTANCE)0x0;
  local_28.hIcon = LoadIconA((HINSTANCE)0x0,(LPCSTR)0x7f00);
  local_28.hCursor = LoadCursorA((HINSTANCE)0x0,(LPCSTR)0x7f00);
  local_28.hbrBackground = GetStockObject(2);
  local_28.lpszMenuName = (LPCSTR)0x0;
  local_28.lpszClassName = "IDM_Download_Panel_controller2_class";
  AVar1 = RegisterClassA(&local_28);
  if (AVar1 == 0) {
    DVar2 = GetLastError();
    if (DVar2 != 0x582) {
      return 0;
    }
  }
  DAT_1005ac45 = 1;
  return 1;
}
```

### Function `[FUN_10011550](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/idmcchandler7_dll/FUN_10011550_10011550.c)`

- **Entry Point**: `0x10011550`
- **Signature**: `undefined __fastcall FUN_10011550(void * param_1, char * param_2, char * param_3)`
- **Callers** (1): `FUN_100117f0` (0x100117f0)
- **Callees** (6): `FUN_10011370` (0x10011370), `_stricmp` (0xEXTERNAL:00000026), `FUN_10010380` (0x10010380), `FUN_10001950` (0x10001950), `FUN_100447e0` (0x100447e0), `operator_delete` (0x10044b92)

```c

void __fastcall FUN_10011550(void *param_1,char *param_2,char *param_3)

{
  char cVar1;
  char *pcVar2;
  int iVar3;
  char *pcVar4;
  undefined ***this;
  char *extraout_EDX;
  char *extraout_EDX_00;
  char *extraout_EDX_01;
  char *extraout_EDX_02;
  char *extraout_EDX_03;
  char *extraout_EDX_04;
  char *extraout_EDX_05;
  char *extraout_EDX_06;
  char *extraout_EDX_07;
  void *local_64;
  undefined **local_60;
  char *local_5c;
  int local_58;
  char local_54 [4];
  undefined **local_50;
  char *local_4c;
  int local_48;
  char local_44 [4];
  undefined **local_40;
  char *local_3c;
  int local_38;
  char local_34 [4];
  undefined **local_30;
  char *local_2c;
  int local_28;
  char local_24 [4];
  undefined **local_20;
  char *local_1c;
  int local_18;
  char local_14 [4];
  uint local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  puStack_8 = &LAB_10046fb8;
  local_c = ExceptionList;
  local_10 = DAT_1005a88c ^ (uint)&local_64;
  ExceptionList = &local_c;
  local_20 = MyStrClass::vftable;
  local_1c = (char *)0x0;
  local_18 = 0;
  local_14[0] = '\0';
  local_30 = MyStrClass::vftable;
  local_2c = (char *)0x0;
  local_28 = 0;
  local_24[0] = '\0';
  local_40 = MyStrClass::vftable;
  local_3c = (char *)0x0;
  local_38 = 0;
  local_34[0] = '\0';
  local_4._1_3_ = 0;
  local_4._0_1_ = 2;
  local_64 = param_1;
  if (param_3 != (char *)0x0) {
    do {
      param_2 = param_3 + 1;
      pcVar2 = param_3;
      do {
        cVar1 = *pcVar2;
        pcVar2 = pcVar2 + 1;
      } while (cVar1 != '\0');
      if ((uint)((int)pcVar2 - (int)param_2) < 2) break;
      local_60 = MyStrClass::vftable;
      local_5c = (char *)0x0;
      local_58 = 0;
      local_54[0] = '\0';
      local_50 = MyStrClass::vftable;
      local_4c = (char *)0x0;
      local_48 = 0;
      local_44[0] = '\0';
// ... [Truncated in summary - Full code in FUN_10011550_10011550.c (179 lines)]
```

### Function `[FUN_1000e990](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/idmcchandler7_dll/FUN_1000e990_1000e990.c)`

- **Entry Point**: `0x1000e990`
- **Signature**: `undefined __stdcall FUN_1000e990(void)`
- **Callers** (0): None
- **Callees** (10): `CoUninitialize` (0xEXTERNAL:000000ad), `CreateWindowExA` (0xEXTERNAL:0000008d), `DispatchMessageA` (0xEXTERNAL:00000090), `GetCurrentProcessId` (0xEXTERNAL:0000006a), `CoInitialize` (0xEXTERNAL:000000ac), `GetMessageA` (0xEXTERNAL:0000008e), `_snprintf` (0xEXTERNAL:00000017), `GetModuleHandleA` (0xEXTERNAL:00000078), `TranslateMessage` (0xEXTERNAL:0000008f), `FUN_100447e0` (0x100447e0)... [0 more]

```c

void FUN_1000e990(void)

{
  DWORD DVar1;
  HMODULE hInstance;
  BOOL BVar2;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 extraout_EDX_01;
  undefined4 uVar3;
  LPVOID lpParam;
  uint uStack_64;
  tagMSG local_54;
  char local_38 [32];
  uint local_18;
  undefined1 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_10046a20;
  local_10 = ExceptionList;
  uStack_64 = DAT_1005a88c ^ (uint)&stack0xfffffffc;
  local_14 = (undefined1 *)&uStack_64;
  ExceptionList = &local_10;
  local_8 = 0;
  local_18 = uStack_64;
  DVar1 = GetCurrentProcessId();
  _snprintf(local_38,0x20,"%ld",DVar1);
  lpParam = (LPVOID)0x0;
  hInstance = GetModuleHandleA((LPCSTR)0x0);
  DAT_1005ac40 = CreateWindowExA(0,"IDM_Download_Panel_controller2_class",local_38,0,0,0,1,1,
                                 (HWND)0xfffffffd,(HMENU)0x0,hInstance,lpParam);
  uVar3 = extraout_EDX;
  if (DAT_1005ac40 != (HWND)0x0) {
    local_8 = CONCAT31(local_8._1_3_,1);
    CoInitialize((LPVOID)0x0);
    while( true ) {
      BVar2 = GetMessageA(&local_54,(HWND)0x0,0,0);
      if (BVar2 == 0) break;
      uVar3 = extraout_EDX_00;
      if (BVar2 == -1) goto LAB_1000ea68;
      TranslateMessage(&local_54);
      DispatchMessageA(&local_54);
    }
    CoUninitialize();
    local_8 = 0xffffffff;
    uVar3 = extraout_EDX_01;
  }
LAB_1000ea68:
  ExceptionList = local_10;
  FUN_100447e0(local_18 ^ (uint)&stack0xfffffffc,uVar3);
  return;
}
```

