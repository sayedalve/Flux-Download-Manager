# Ghidra Analysis Report: IDMan.exe

- **Program**: `IDMan.exe`
- **Executable Path**: `/C:/Users/sayed/Downloads/IDM_RE_Investigation/binaries/IDMan.exe`
- **Language**: `x86:LE:32:default`
- **Compiler**: `windows`
- **Image Base**: `00400000`

## Targeted String Occurrences and Cross-References

### Pattern: `video/mp4` (1 occurrence(s))

- **Found at Address**: `0x0069daf0`
  - **Direct References** (2):
    - from `0x0043237e` (Type: DATA) in Function `[FUN_00430560](file:///0x00430560)`
    - from `0x0059abb9` (Type: DATA) in Function `[FUN_0059a6f0](file:///0x0059a6f0)`

### Pattern: `video/webm` (1 occurrence(s))

- **Found at Address**: `0x0069db30`
  - **Direct References** (1):
    - from `0x004322de` (Type: DATA) in Function `[FUN_00430560](file:///0x00430560)`

### Pattern: `video/MP2T` (1 occurrence(s))

- **Found at Address**: `0x006d01f8`
  - **Direct References** (1):
    - from `0x00594e4e` (Type: DATA) in Function `[FUN_00592d50](file:///0x00592d50)`

### Pattern: `application/vnd.apple.mpegurl` (1 occurrence(s))

- **Found at Address**: `0x006d0204`
  - **Direct References** (1):
    - from `0x00594e30` (Type: DATA) in Function `[FUN_00592d50](file:///0x00592d50)`

### Pattern: `mpdatoidm` (2 occurrence(s))

- **Found at Address**: `0x0069c92c`
  - **Direct References** (2):
    - from `0x0042af03` (Type: DATA) in Function `[FUN_0042a180](file:///0x0042a180)`
    - from `0x00427a06` (Type: DATA) in Function `[FUN_00426f10](file:///0x00426f10)`
- **Found at Address**: `0x0069c93c`
  - **Direct References** (2):
    - from `0x0042aee1` (Type: DATA) in Function `[FUN_0042a180](file:///0x0042a180)`
    - from `0x004279dc` (Type: DATA) in Function `[FUN_00426f10](file:///0x00426f10)`

### Pattern: `mpdtoidm` (2 occurrence(s))

- **Found at Address**: `0x0069c95c`
  - **Direct References** (2):
    - from `0x0042ae7d` (Type: DATA) in Function `[FUN_0042a180](file:///0x0042a180)`
    - from `0x00427967` (Type: DATA) in Function `[FUN_00426f10](file:///0x00426f10)`
- **Found at Address**: `0x0069c96c`
  - **Direct References** (2):
    - from `0x0042ae5b` (Type: DATA) in Function `[FUN_0042a180](file:///0x0042a180)`
    - from `0x0042793d` (Type: DATA) in Function `[FUN_00426f10](file:///0x00426f10)`

### Pattern: `hlstoidm` (1 occurrence(s))

- **Found at Address**: `0x0069ca04`
  - **Direct References** (2):
    - from `0x0042ac61` (Type: DATA) in Function `[FUN_0042a180](file:///0x0042a180)`
    - from `0x004275a4` (Type: DATA) in Function `[FUN_00426f10](file:///0x00426f10)`

### Pattern: `hls2toidm` (2 occurrence(s))

- **Found at Address**: `0x0069c9bc`
  - **Direct References** (2):
    - from `0x0042ad19` (Type: DATA) in Function `[FUN_0042a180](file:///0x0042a180)`
    - from `0x00427760` (Type: DATA) in Function `[FUN_00426f10](file:///0x00426f10)`
- **Found at Address**: `0x0069c9d0`
  - **Direct References** (2):
    - from `0x0042acbb` (Type: DATA) in Function `[FUN_0042a180](file:///0x0042a180)`
    - from `0x004276f4` (Type: DATA) in Function `[FUN_00426f10](file:///0x00426f10)`

### Pattern: `Download this video` (2 occurrence(s))

- **Found at Address**: `0x006a7370`
  - **Direct References**: None found.
  - **Indirect Pointer References** (1):
    - Pointer at `0x004ac31d`
- **Found at Address**: `0x006af105`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.

### Pattern: `com.tonec.idm` (5 occurrence(s))

- **Found at Address**: `0x0069a370`
  - **Direct References** (1):
    - from `0x004130cc` (Type: DATA) in Function `[FUN_00412e50](file:///0x00412e50)`
- **Found at Address**: `0x0069a436`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.
- **Found at Address**: `0x0069a470`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.
- **Found at Address**: `0x0069b1b1`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.
- **Found at Address**: `0x0069b211`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.

### Pattern: `IDMMsgHost` (3 occurrence(s))

- **Found at Address**: `0x0069a144`
  - **Direct References** (6):
    - from `0x0041b148` (Type: DATA) in Function `[FUN_0041b0d0](file:///0x0041b0d0)`
    - from `0x00412ecb` (Type: DATA) in Function `[FUN_00412e50](file:///0x00412e50)`
    - from `0x00412520` (Type: DATA) in Function `[FUN_004124b0](file:///0x004124b0)`
    - from `0x0045f20b` (Type: DATA) in Function `[FUN_0045f0c0](file:///0x0045f0c0)`
    - from `0x00412eae` (Type: DATA) in Function `[FUN_00412e50](file:///0x00412e50)`
    - from `0x0041d35d` (Type: DATA) in Function `[FUN_0041cac0](file:///0x0041cac0)`
- **Found at Address**: `0x0069a480`
  - **Direct References** (3):
    - from `0x00412ecb` (Type: DATA) in Function `[FUN_00412e50](file:///0x00412e50)`
    - from `0x00412eb7` (Type: DATA) in Function `[FUN_00412e50](file:///0x00412e50)`
    - from `0x0041d391` (Type: DATA) in Function `[FUN_0041cac0](file:///0x0041cac0)`
- **Found at Address**: `0x0069b4dc`
  - **Direct References** (2):
    - from `0x0045f244` (Type: DATA) in Function `[FUN_0045f0c0](file:///0x0045f0c0)`
    - from `0x0041d329` (Type: DATA) in Function `[FUN_0041cac0](file:///0x0041cac0)`

### Pattern: `idmbrbtn` (4 occurrence(s))

- **Found at Address**: `0x0069c78c`
  - **Direct References** (2):
    - from `0x005c7141` (Type: DATA) in Function `[FUN_005c5b20](file:///0x005c5b20)`
    - from `0x00429914` (Type: DATA) in Function `[FUN_00426f10](file:///0x00426f10)`
- **Found at Address**: `0x0069b65c`
  - **Direct References** (2):
    - from `0x0045f39a` (Type: DATA) in Function `[FUN_0045f0c0](file:///0x0045f0c0)`
    - from `0x0041d01d` (Type: DATA) in Function `[FUN_0041cac0](file:///0x0041cac0)`
- **Found at Address**: `0x0069b6c0`
  - **Direct References** (2):
    - from `0x0045f641` (Type: DATA) in Function `[FUN_0045f0c0](file:///0x0045f0c0)`
    - from `0x0041cf81` (Type: DATA) in Function `[FUN_0041cac0](file:///0x0041cac0)`
- **Found at Address**: `0x0069b74c`
  - **Direct References** (2):
    - from `0x0045f56f` (Type: DATA) in Function `[FUN_0045f0c0](file:///0x0045f0c0)`
    - from `0x0041ced1` (Type: DATA) in Function `[FUN_0041cac0](file:///0x0041cac0)`

### Pattern: `CreateIDMButton` (1 occurrence(s))

- **Found at Address**: `0x006a7384`
  - **Direct References**: None found.
  - **Indirect Pointer References** (2):
    - Pointer at `0x004ac244`
    - Pointer at `0x0054cafe`

### Pattern: `idmBroker` (1 occurrence(s))

- **Found at Address**: `0x0069b558`
  - **Direct References** (1):
    - from `0x0041d259` (Type: DATA) in Function `[FUN_0041cac0](file:///0x0041cac0)`

### Pattern: `MSG#` (2 occurrence(s))

- **Found at Address**: `0x007297cc`
  - **Direct References** (1):
    - from `0x0060e838` (Type: DATA) in Function `[FUN_0060e7b0](file:///0x0060e7b0)`
- **Found at Address**: `0x00729824`
  - **Direct References** (1):
    - from `0x00610579` (Type: DATA) in Function `[FUN_00610540](file:///0x00610540)`

### Pattern: `\\.\pipe\` (1 occurrence(s))

- **Found at Address**: `0x00729838`
  - **Direct References** (2):
    - from `0x006109da` (Type: READ) in Function `[FUN_00610980](file:///0x00610980)`
    - from `0x006109d1` (Type: DATA) in Function `[FUN_00610980](file:///0x00610980)`

### Pattern: `Content-Range` (1 occurrence(s))

- **Found at Address**: `0x006cf014`
  - **Direct References** (3):
    - from `0x0058289d` (Type: DATA) in Function `[FUN_00582780](file:///0x00582780)`
    - from `0x00582049` (Type: DATA) in Function `[FUN_00581c80](file:///0x00581c80)`
    - from `0x0058ca0f` (Type: DATA) in Function `[FUN_0058c7f0](file:///0x0058c7f0)`

### Pattern: `WinHttpOpen` (1 occurrence(s))

- **Found at Address**: `0x006a8d4c`
  - **Direct References** (1):
    - from `0x004b8b12` (Type: DATA) in Function `[FUN_004b8980](file:///0x004b8980)`

### Pattern: `Referer` (5 occurrence(s))

- **Found at Address**: `0x006aa2ec`
  - **Direct References** (5):
    - from `0x004cbf16` (Type: DATA) in Function `[FUN_004cbc90](file:///0x004cbc90)`
    - from `0x004cb4c1` (Type: DATA) in Function `[FUN_004cb100](file:///0x004cb100)`
    - from `0x005946db` (Type: DATA) in Function `[FUN_00592d50](file:///0x00592d50)`
    - from `0x005d9eb0` (Type: DATA) in Function `[FUN_005d9c60](file:///0x005d9c60)`
    - from `0x004ee019` (Type: DATA) in Function `[FUN_004ec770](file:///0x004ec770)`
- **Found at Address**: `0x006cf9a4`
  - **Direct References** (4):
    - from `0x005af6cb` (Type: DATA) in Function `[FUN_005ae8e0](file:///0x005ae8e0)`
    - from `0x005af702` (Type: DATA) in Function `[FUN_005ae8e0](file:///0x005ae8e0)`
    - from `0x0058bd15` (Type: DATA) in Function `[FUN_0058b320](file:///0x0058b320)`
    - from `0x0058bd78` (Type: DATA) in Function `[FUN_0058b320](file:///0x0058b320)`
- **Found at Address**: `0x006d5640`
  - **Direct References** (1):
    - from `0x005dcb0c` (Type: DATA) in Function `[FUN_005dc710](file:///0x005dc710)`
- **Found at Address**: `0x00799c16`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.
- **Found at Address**: `0x007ab654`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.

## Decompiled Key Functions

Total unique functions identified for decompilation: 25

### Function `[FUN_0058c7f0](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_0058c7f0_0058c7f0.c)`

- **Entry Point**: `0x0058c7f0`
- **Signature**: `undefined FUN_0058c7f0(void)`
- **Callers** (2): `FUN_0058fb30` (0x0058fb30), `FUN_0058f050` (0x0058f050)
- **Callees** (57): `FUN_004110c0` (0x004110c0), `FUN_005803a0` (0x005803a0), `FUN_00402f80` (0x00402f80), `FUN_005810e0` (0x005810e0), `_strpbrk` (0x0063f240), `FUN_004011f0` (0x004011f0), `FUN_005d5870` (0x005d5870), `__memicmp` (0x006414cd), `FUN_004017e0` (0x004017e0), `FUN_00420590` (0x00420590)... [47 more]

```c

/* WARNING: Function: __alloca_probe replaced with injection: alloca_probe */

void __thiscall FUN_0058c7f0(int param_1,char param_2)

{
  char cVar1;
  wchar_t wVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  char *pcVar6;
  void *pvVar7;
  size_t sVar8;
  undefined4 uVar9;
  wchar_t *pwVar10;
  wchar_t *pwVar11;
  errno_t eVar12;
  LSTATUS LVar13;
  undefined2 *puVar14;
  wchar_t *pwVar15;
  char *pcVar16;
  char *pcVar17;
  bool bVar18;
  uint local_5ac0;
  char *local_5a70;
  char *local_5a68;
  char *local_5a64;
  char *local_5a60;
  undefined1 *local_5a5c;
  void *local_5a58;
  undefined1 *puStack_5a54;
  uint local_5a50;
  undefined2 *local_5a4c;
  undefined2 local_5a48 [2];
  undefined4 local_5a44;
  wchar_t *local_5a40;
  wchar_t local_5a3c [2];
  undefined4 local_5a38;
  wchar_t *local_5a34;
  wchar_t local_5a30 [2];
  undefined4 local_5a2c;
  wchar_t *local_5a28;
  wchar_t local_5a24 [4];
  wchar_t *local_5a1c;
  wchar_t local_5a18 [4];
  wchar_t local_5a10 [10240];
  wchar_t local_a10 [256];
  wchar_t local_810 [256];
  wchar_t local_610 [256];
  wchar_t local_410 [256];
  wchar_t local_210;
  undefined1 local_20e [510];
  char *local_10 [2];
  uint local_8;
  
  puStack_5a54 = &LAB_0067f7a4;
  local_5a58 = ExceptionList;
  local_5ac0 = DAT_007839c0 ^ (uint)&local_5a4c;
  local_5a5c = (undefined1 *)&local_5ac0;
  ExceptionList = &local_5a58;
  local_5a50 = 0;
  local_8 = local_5ac0;
  FUN_00403060(*(undefined4 *)(param_1 + 0x3258));
  iVar5 = *(int *)(param_1 + 0x3258);
  iVar4 = FUN_005803a0(iVar5);
  iVar5 = __memicmp((void *)(iVar5 + iVar4),"content-length:",0xf);
  if (iVar5 == 0) {
    if (param_2 == '\0') {
      FID_conflict__sscanf
                ((char *)(iVar4 + 0xf + *(int *)(param_1 + 0x3258)),"%I64d",param_1 + 0x3288);
    }
    else {
      FID_conflict__sscanf
                ((char *)(iVar4 + 0xf + *(int *)(param_1 + 0x3258)),"%I64d",
                 (int *)(param_1 + 0x3220));
      if ((*(int *)(param_1 + 0x3220) == 0) && (*(int *)(param_1 + 0x3224) == 0)) {
        for (pcVar16 = (char *)(iVar4 + 0xf + *(int *)(param_1 + 0x3258)); *pcVar16 == ' ';
            pcVar16 = pcVar16 + 1) {
        }
// ... [Truncated in summary - Full code in FUN_0058c7f0_0058c7f0.c (1289 lines)]
```

### Function `[FUN_00581c80](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_00581c80_00581c80.c)`

- **Entry Point**: `0x00581c80`
- **Signature**: `undefined FUN_00581c80(void)`
- **Callers** (3): `FUN_00582780` (0x00582780), `FUN_00582340` (0x00582340), `FUN_00582460` (0x00582460)
- **Callees** (7): `__memicmp` (0x006414cd), `_strstr` (0x0063e0c0), `FID_conflict:_sscanf` (0x0063e40e), `FUN_005803a0` (0x005803a0), `_strchr` (0x0063e000), `FUN_00402840` (0x00402840), `_memcpy` (0x0063e5c0)

```c

void __fastcall FUN_00581c80(int param_1)

{
  undefined4 *puVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  void *pvVar6;
  char *pcVar7;
  size_t sVar8;
  char *pcVar9;
  char *pcVar10;
  
  iVar4 = *(int *)(param_1 + 0x3258);
  iVar3 = FUN_005803a0(iVar4);
  iVar4 = __memicmp((void *)(iVar4 + iVar3),"Content-Type:",0xd);
  if (iVar4 == 0) {
    iVar4 = *(int *)(param_1 + 0x3258) + 0xd + iVar3;
    iVar5 = FUN_005803a0(iVar4);
    pcVar9 = (char *)(iVar4 + iVar5);
    iVar4 = __memicmp(pcVar9,"multipart/byteranges",0x14);
    if ((((iVar4 == 0) || (iVar4 = __memicmp(pcVar9,"multipart/x-byteranges",0x16), iVar4 == 0)) &&
        (pcVar9 = _strchr(pcVar9,0x3b), pcVar9 != (char *)0x0)) &&
       ((pcVar9 = _strstr(pcVar9,"boundary"), pcVar9 != (char *)0x0 &&
        (pcVar9 = _strchr(pcVar9,0x3d), pcVar9 != (char *)0x0)))) {
      iVar4 = FUN_005803a0(pcVar9 + 1);
      pcVar10 = pcVar9 + 1 + iVar4;
      sVar8 = 0;
      pcVar9 = _strstr(pcVar10,"\r\n");
      if ((pcVar9 != (char *)0x0) || (pcVar9 = _strstr(pcVar10,"\n"), pcVar9 != (char *)0x0)) {
        sVar8 = (int)pcVar9 - (int)pcVar10;
      }
      pvVar6 = (void *)FUN_00402840(sVar8);
      _memcpy(pvVar6,pcVar10,sVar8);
      pcVar10 = *(char **)(param_1 + 0x327c);
      pcVar9 = pcVar10 + 1;
      do {
        cVar2 = *pcVar10;
        pcVar10 = pcVar10 + 1;
      } while (cVar2 != '\0');
      if (pcVar10 != pcVar9) {
        *(undefined1 *)(param_1 + 0x3280) = 1;
      }
    }
  }
  iVar4 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar3),"Transfer-Encoding:",0x12);
  if (iVar4 == 0) {
    pcVar9 = (char *)(*(int *)(param_1 + 0x3258) + 0x12 + iVar3);
    cVar2 = *(char *)(*(int *)(param_1 + 0x3258) + 0x12 + iVar3);
    while (cVar2 == ' ') {
      pcVar9 = pcVar9 + 1;
      cVar2 = *pcVar9;
    }
    iVar4 = __memicmp(pcVar9,"chunked",7);
    if (iVar4 == 0) {
      *(undefined1 *)(param_1 + 0xe0) = 1;
    }
  }
  iVar4 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar3),"Content-Encoding:",0x11);
  if (iVar4 == 0) {
    pcVar9 = (char *)(*(int *)(param_1 + 0x3258) + 0x11 + iVar3);
    cVar2 = *(char *)(*(int *)(param_1 + 0x3258) + 0x11 + iVar3);
    while (cVar2 == ' ') {
      pcVar9 = pcVar9 + 1;
      cVar2 = *pcVar9;
    }
    iVar3 = __memicmp(pcVar9,&DAT_006cf068,4);
    if (((iVar3 == 0) || (iVar3 = __memicmp(pcVar9,"x-gzip",6), iVar3 == 0)) ||
       ((iVar3 = __memicmp(pcVar9,"deflate",7), iVar3 == 0 ||
        (iVar3 = __memicmp(pcVar9,"x-deflate",9), iVar3 == 0)))) {
      *(undefined1 *)(param_1 + 0x21) = 1;
    }
    iVar3 = __memicmp(pcVar9,&DAT_006cf048,3);
    if (((iVar3 == 0) || (iVar3 = __memicmp(pcVar9,&DAT_006cf044,3), iVar3 == 0)) ||
       (iVar3 = __memicmp(pcVar9,"brotli",6), iVar3 == 0)) {
      *(undefined1 *)(param_1 + 0x20) = 1;
      *(undefined1 *)(param_1 + 0x3281) = 1;
      return;
// ... [Truncated in summary - Full code in FUN_00581c80_00581c80.c (184 lines)]
```

### Function `[FUN_0060e7b0](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_0060e7b0_0060e7b0.c)`

- **Entry Point**: `0x0060e7b0`
- **Signature**: `undefined FUN_0060e7b0(void)`
- **Callers** (7): `FUN_0060eee0` (0x0060eee0), `FUN_0060fb50` (0x0060fb50), `FUN_0060eda0` (0x0060eda0), `FUN_0060fd80` (0x0060fd80), `FUN_0060ef80` (0x0060ef80), `FUN_0060ecf0` (0x0060ecf0), `FUN_0060fc50` (0x0060fc50)
- **Callees** (9): `FUN_0060d6e0` (0x0060d6e0), `FUN_0060d950` (0x0060d950), `FUN_0060e130` (0x0060e130), `FUN_0060de70` (0x0060de70), `FUN_0060e6f0` (0x0060e6f0), `FUN_0060e650` (0x0060e650), `FUN_0060d620` (0x0060d620), `FUN_0060d670` (0x0060d670), `_memcpy` (0x0063e5c0)

```c

void FUN_0060e7b0(int *param_1)

{
  int iVar1;
  int *piVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  uint uVar6;
  byte *pbVar7;
  void *local_14;
  int local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  piVar2 = param_1;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_00685ed8;
  local_c = ExceptionList;
  uVar3 = DAT_007839c0 ^ (uint)&stack0xffffffdc;
  ExceptionList = &local_c;
  FUN_0060d670(param_1[9] * 0x15 + 0x30);
  uVar6 = 0;
  local_4 = 0;
  if ((char)piVar2[3] != '\0') goto LAB_0060e824;
  iVar1 = piVar2[1];
  if ((uint)piVar2[2] < iVar1 + 1U) {
    iVar4 = FUN_0060d620(1);
    if (iVar4 != 0) goto LAB_0060e816;
  }
  else {
LAB_0060e816:
    *(undefined1 *)(piVar2[1] + *piVar2) = 0x3b;
    piVar2[1] = iVar1 + 1U;
  }
  *(undefined1 *)(piVar2 + 3) = 1;
LAB_0060e824:
  FUN_0060de70(&local_14,"MSG#%u#%u#%u#%u",piVar2[4],piVar2[5],piVar2[6],piVar2[7],uVar3);
  if (piVar2[9] != 0) {
    pbVar7 = (byte *)(piVar2 + 10);
    do {
      param_1 = (int *)CONCAT31(param_1._1_3_,0x3a);
      FUN_0060d950(1,&param_1);
      FUN_0060e650(&local_14,&DAT_007297a0 + (uint)*pbVar7 * 5,*(undefined4 *)(pbVar7 + 8),
                   *(undefined4 *)(pbVar7 + 0xc));
      uVar6 = uVar6 + 1;
      pbVar7 = pbVar7 + 0x10;
    } while (uVar6 < (uint)piVar2[9]);
  }
  iVar4 = *(int *)((int)local_14 + -8) + piVar2[1];
  iVar1 = iVar4 + 4;
  piVar5 = (int *)FUN_0060d620(iVar1);
  if (piVar5 == (int *)0x0) {
    FUN_0060e130();
  }
  else {
    *(int *)(local_10 + 0x44) = *(int *)(local_10 + 0x44) + iVar1;
    *piVar5 = iVar4;
    _memcpy(piVar5 + 1,local_14,*(size_t *)((int)local_14 + -8));
    _memcpy((void *)((int)(piVar5 + 1) + *(int *)((int)local_14 + -8)),(void *)*piVar2,piVar2[1]);
    FUN_0060e6f0();
  }
  local_4 = 0xffffffff;
  FUN_0060d6e0();
  ExceptionList = local_c;
  return;
}
```

### Function `[FUN_00426f10](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_00426f10_00426f10.c)`

- **Entry Point**: `0x00426f10`
- **Signature**: `undefined FUN_00426f10(void)`
- **Callers** (0): None
- **Callees** (58): `FUN_004149b0` (0x004149b0), `FUN_005dcc70` (0x005dcc70), `WideCharToMultiByte` (0xEXTERNAL:000000cd), `FUN_00402f80` (0x00402f80), `FUN_00425d10` (0x00425d10), `FUN_0057f8f0` (0x0057f8f0), `FUN_004017e0` (0x004017e0), `FUN_0064057f` (0x0064057f), `SafeArrayGetElement` (0xEXTERNAL:00000257), `__stricmp` (0x0063e53e)... [48 more]

```c

/* WARNING: Removing unreachable block (ram,0x00429a1f) */

void FUN_00426f10(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined4 *puVar1;
  WCHAR WVar2;
  OLECHAR OVar3;
  char cVar4;
  undefined1 uVar5;
  HRESULT HVar6;
  int iVar7;
  LPSTR pCVar8;
  char *pcVar9;
  char *pcVar10;
  char *pcVar11;
  int iVar12;
  BSTR pOVar13;
  wchar_t *pwVar14;
  int *piVar15;
  char **ppcVar16;
  undefined4 uVar17;
  LPCWSTR pWVar18;
  char **ppcVar19;
  SAFEARRAY *pSVar20;
  uint uVar21;
  code *pcVar22;
  int iVar23;
  uint uStack_2cc;
  undefined1 local_2bc [4];
  undefined1 local_2b8 [4];
  undefined1 local_2b4 [4];
  undefined1 local_2b0 [4];
  undefined1 local_2ac [4];
  undefined4 local_2a8;
  int local_2a4;
  undefined4 local_2a0 [2];
  int local_298;
  int local_294;
  int local_290;
  int local_28c;
  char *local_288;
  BSTR local_284;
  int local_280;
  BSTR local_27c;
  int local_278;
  uint local_274;
  uint local_270;
  char *local_26c;
  uint local_268;
  BSTR local_264;
  int local_260;
  int local_25c;
  int local_258;
  undefined4 local_254;
  undefined4 local_250;
  undefined4 local_24c;
  undefined8 local_248;
  BSTR local_240;
  undefined4 local_23c;
  undefined4 local_238;
  undefined4 local_234;
  BSTR local_230;
  undefined4 local_22c;
  BSTR local_228;
  undefined4 local_224;
  undefined4 local_220;
  undefined4 local_21c;
  undefined4 local_218;
  undefined4 local_214;
  undefined4 local_210;
  undefined4 local_20c;
  BSTR local_208;
  BSTR local_204;
  BSTR local_200;
  int *local_1fc;
  LPCWSTR local_1f8;
  undefined4 local_1f4;
  undefined4 local_1f0;
// ... [Truncated in summary - Full code in FUN_00426f10_00426f10.c (1802 lines)]
```

### Function `[FUN_004124b0](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_004124b0_004124b0.c)`

- **Entry Point**: `0x004124b0`
- **Signature**: `undefined FUN_004124b0(void)`
- **Callers** (1): `FUN_0041b1f0` (0x0041b1f0)
- **Callees** (8): `FUN_00402310` (0x00402310), `FUN_00447030` (0x00447030), `__security_check_cookie` (0x0063ca21), `RegQueryValueExW` (0xEXTERNAL:000001f2), `RegOpenKeyExA` (0xEXTERNAL:000001fa), `__wcsicmp` (0x00641f67), `RegCloseKey` (0xEXTERNAL:000001e6), `FUN_004011f0` (0x004011f0)

```c

void FUN_004124b0(HKEY param_1,LPCSTR param_2)

{
  bool bVar1;
  uint uVar2;
  undefined *puVar3;
  LSTATUS LVar4;
  int iVar5;
  wchar_t *pwVar6;
  HKEY local_434;
  DWORD local_430;
  wchar_t *local_42c;
  wchar_t local_428 [2];
  undefined4 local_424;
  wchar_t local_420 [520];
  uint local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_006601fb;
  local_c = ExceptionList;
  local_10 = DAT_007839c0 ^ (uint)&local_434;
  uVar2 = DAT_007839c0 ^ (uint)&stack0xfffffbc0;
  ExceptionList = &local_c;
  bVar1 = false;
  local_42c = (wchar_t *)0x0;
  local_424 = 0;
  local_428[0] = L'\0';
  local_4 = 0;
  puVar3 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar3 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar3,L"IDMMsgHost.json",0);
  LVar4 = RegOpenKeyExA(param_1,param_2,0,0x20019,&local_434);
  if (LVar4 == 0) {
    local_430 = 0x410;
    LVar4 = RegQueryValueExW(local_434,L"",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)local_420,&local_430);
    if (LVar4 == 0) {
      pwVar6 = local_42c;
      if (local_42c == (wchar_t *)0x0) {
        pwVar6 = local_428;
      }
      iVar5 = __wcsicmp(local_420,pwVar6);
      if (iVar5 == 0) {
        bVar1 = true;
      }
    }
    RegCloseKey(local_434);
    if (((bVar1) && (iVar5 = FUN_00447030(), iVar5 != 0)) &&
       (LVar4 = RegOpenKeyExA(param_1,param_2,0,0x20119,&local_434), LVar4 == 0)) {
      local_430 = 0x410;
      LVar4 = RegQueryValueExW(local_434,L"",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)local_420,&local_430)
      ;
      if (LVar4 == 0) {
        pwVar6 = local_42c;
        if (local_42c == (wchar_t *)0x0) {
          pwVar6 = local_428;
        }
        __wcsicmp(local_420,pwVar6);
      }
      RegCloseKey(local_434);
    }
  }
  local_4 = 0xffffffff;
  FUN_004011f0(uVar2);
  ExceptionList = local_c;
  __security_check_cookie(local_10 ^ (uint)&local_434);
  return;
}
```

### Function `[FUN_005c5b20](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_005c5b20_005c5b20.c)`

- **Entry Point**: `0x005c5b20`
- **Signature**: `undefined FUN_005c5b20(void)`
- **Callers** (3): `FUN_004d6a70` (0x004d6a70), `FUN_005c7b80` (0x005c7b80), `FUN_005c85b0` (0x005c85b0)
- **Callees** (67): `FUN_005d6890` (0x005d6890), `FUN_005c11b0` (0x005c11b0), `FUN_0054a900` (0x0054a900), `FUN_005d91c0` (0x005d91c0), `FUN_004fd020` (0x004fd020), `operator=` (0x004058f0), `FUN_00474ee0` (0x00474ee0), `FUN_00406f90` (0x00406f90), `FUN_0056f5a0` (0x0056f5a0), `FUN_005d9be0` (0x005d9be0)... [57 more]

```c

void __thiscall FUN_005c5b20(int param_1,char param_2,char param_3,int param_4)

{
  short *psVar1;
  BYTE BVar2;
  byte bVar3;
  char cVar4;
  short sVar5;
  uint *puVar6;
  int iVar7;
  CWnd *pCVar8;
  BYTE *pBVar9;
  BOOL BVar10;
  undefined4 *puVar11;
  undefined2 *puVar12;
  short *psVar13;
  byte *pbVar14;
  byte *pbVar15;
  char *pcVar16;
  char *pcVar17;
  undefined2 *puVar18;
  undefined2 *puVar19;
  uint uVar20;
  LSTATUS LVar21;
  undefined4 uVar22;
  undefined4 uVar23;
  byte *pbVar24;
  undefined1 *puVar25;
  byte *pbVar26;
  bool bVar27;
  uint local_1bc;
  BYTE *local_138;
  undefined4 local_134;
  undefined4 local_e8;
  int local_e4;
  undefined1 local_dd;
  undefined1 local_dc;
  undefined1 local_db;
  char local_da;
  char local_d9;
  char *local_d8;
  HLOCAL local_d4;
  char local_cd;
  CWnd *local_cc;
  undefined1 *local_c8;
  void *local_c4;
  undefined1 *puStack_c0;
  int local_bc;
  undefined2 *local_b8;
  undefined2 local_b4 [2];
  undefined4 local_b0;
  undefined2 *local_ac;
  undefined2 local_a8 [2];
  undefined4 local_a4;
  undefined2 *local_a0;
  undefined2 local_9c [2];
  undefined4 local_98;
  undefined2 *local_94;
  undefined2 local_90 [2];
  undefined4 local_8c;
  undefined2 *local_88;
  undefined2 local_84 [2];
  undefined4 local_80;
  undefined2 *local_7c;
  undefined2 local_78 [2];
  undefined4 local_74;
  undefined2 *local_70;
  undefined2 local_6c [2];
  undefined4 local_68;
  undefined2 *local_64;
  undefined2 local_60 [2];
  undefined4 local_5c;
  char local_58 [64];
  undefined1 local_18 [4];
  undefined2 *local_14;
  undefined2 local_10 [2];
  undefined4 local_c;
  uint local_8;
  
// ... [Truncated in summary - Full code in FUN_005c5b20_005c5b20.c (1314 lines)]
```

### Function `[FUN_004cb100](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_004cb100_004cb100.c)`

- **Entry Point**: `0x004cb100`
- **Signature**: `undefined FUN_004cb100(void)`
- **Callers** (1): `FUN_00462930` (0x00462930)
- **Callees** (17): `FUN_004c4c30` (0x004c4c30), `FUN_004c4d90` (0x004c4d90), `MulDiv` (0xEXTERNAL:000000ca), `RegCreateKeyExA` (0xEXTERNAL:000001f7), `FUN_0063ec80` (0x0063ec80), `GetDC` (0xEXTERNAL:0000018b), `RegCloseKey` (0xEXTERNAL:000001e6), `FUN_00502d90` (0x00502d90), `__security_check_cookie` (0x0063ca21), `FUN_0040cdf0` (0x0040cdf0)... [7 more]

```c

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004cb100(undefined4 *param_1)

{
  int iVar1;
  int *piVar2;
  LSTATUS LVar3;
  int *piVar4;
  HDC hdc;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  undefined4 *puVar8;
  DWORD local_268;
  undefined4 local_264;
  undefined4 local_260;
  HKEY local_25c;
  int local_258;
  undefined4 local_254;
  undefined4 local_250;
  undefined4 local_24c;
  undefined4 local_248;
  undefined4 local_244;
  undefined4 local_240;
  undefined4 local_23c;
  undefined4 local_238;
  undefined4 local_234;
  undefined4 local_230;
  undefined4 local_22c;
  undefined4 *local_228;
  DWORD local_224 [133];
  uint local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_0066e977;
  local_c = ExceptionList;
  local_10 = DAT_007839c0 ^ (uint)&local_268;
  ExceptionList = &local_c;
  local_228 = param_1;
  FUN_0040cdf0(DAT_007839c0 ^ (uint)&stack0xfffffd88);
  local_4 = 0;
  *param_1 = CIDMListCtrl::vftable;
  FUN_004cd930();
  local_4._0_1_ = 1;
  FUN_0061d4c9();
  local_4._0_1_ = 2;
  FUN_00502d90();
  local_4._0_1_ = 3;
  FUN_0061d4c9();
  local_4._0_1_ = 4;
  *(undefined8 *)(param_1 + 0x46) = 0x3ff0000000000000;
  *(undefined1 *)(param_1 + 0x15) = 0;
  *(undefined1 *)(param_1 + 0x6c) = 0;
  param_1[0x33] = 0;
  iVar1 = 0;
  piVar4 = param_1 + 0x60;
  do {
    *piVar4 = iVar1;
    iVar1 = iVar1 + 1;
    piVar4 = piVar4 + 1;
  } while (iVar1 < 0xc);
  FUN_004cac30();
  iVar1 = 1;
  piVar4 = param_1 + 0x60;
  local_258 = 0xb;
  do {
    if (iVar1 < 0xc) {
      iVar7 = 0xc - iVar1;
      piVar2 = piVar4;
      do {
        piVar2 = piVar2 + 1;
        iVar5 = *piVar4;
        if ((param_1[iVar5 * 2 + 0x49] == 0) && (param_1[*piVar2 * 2 + 0x49] != 0)) {
          *piVar4 = *piVar2;
          *piVar2 = iVar5;
// ... [Truncated in summary - Full code in FUN_004cb100_004cb100.c (277 lines)]
```

### Function `[FUN_00592d50](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_00592d50_00592d50.c)`

- **Entry Point**: `0x00592d50`
- **Signature**: `undefined FUN_00592d50(void)`
- **Callers** (6): `FUN_0059af00` (0x0059af00), `FUN_00592d50` (0x00592d50), `FUN_00599e20` (0x00599e20), `FUN_0059c720` (0x0059c720), `FUN_00598ea0` (0x00598ea0), `FUN_00594893` (0x00594893)
- **Callees** (67): `FUN_005811d0` (0x005811d0), `FUN_005d6890` (0x005d6890), `FUN_005b07b0` (0x005b07b0), `FUN_00640553` (0x00640553), `FUN_005d75d0` (0x005d75d0), `operator=` (0x004058f0), `FUN_005d75a0` (0x005d75a0), `FUN_00580e60` (0x00580e60), `FUN_004b2420` (0x004b2420), `FUN_00425d10` (0x00425d10)... [57 more]

```c

void __thiscall FUN_00592d50(int *param_1,int param_2)

{
  short *psVar1;
  byte bVar2;
  short sVar3;
  int iVar4;
  uint uVar5;
  char cVar6;
  undefined4 uVar7;
  int iVar8;
  char *pcVar9;
  undefined4 uVar10;
  byte *pbVar11;
  byte *pbVar12;
  undefined4 *puVar13;
  char *pcVar14;
  undefined1 *puVar15;
  char *pcVar16;
  int iVar17;
  short *psVar18;
  int *piVar19;
  code *pcVar20;
  uint uVar21;
  int iVar22;
  bool bVar23;
  uint uStack_c8;
  undefined4 local_b8;
  ushort local_b4;
  int local_ac;
  int *local_a8;
  int local_a4;
  undefined4 local_a0;
  uint local_9c;
  WPARAM local_98;
  undefined4 local_94;
  undefined4 local_90;
  CTime local_8c [4];
  char *local_88;
  char local_81;
  undefined4 local_80;
  char local_79;
  WPARAM local_78;
  char *local_74;
  char local_6d;
  undefined4 local_6c;
  char *local_68;
  undefined1 *local_64;
  void *local_60;
  undefined1 *puStack_5c;
  int local_58;
  short *local_54;
  short local_50 [2];
  undefined4 local_4c;
  char local_48 [64];
  uint local_8;
  
  puStack_5c = &LAB_0067fb09;
  local_60 = ExceptionList;
  uStack_c8 = DAT_007839c0 ^ (uint)&local_54;
  local_64 = (undefined1 *)&uStack_c8;
  ExceptionList = &local_60;
  local_9c = 0;
  local_58 = 0;
  local_78 = 3000;
  local_98 = 5000;
  if ((*(char *)(param_1[0x32] + 0x19b) == '\0') ||
     (uVar7 = 5, *(char *)(param_1[0x32] + 0x36f) == '\0')) {
    uVar7 = 0xf;
  }
  piVar19 = param_1 + 0xc80;
  iVar17 = *piVar19;
  local_a8 = param_1;
  local_8 = uStack_c8;
  local_68 = (char *)(**(code **)(*param_1 + 0x1c))(iVar17,uVar7);
  if (*(int *)(param_1[0xc82] + 0x1c) != 0) goto LAB_00592fd1;
  if (((local_68 == (char *)0x2751) || (local_68 == (char *)0x2743)) &&
     ((*(uint *)(param_1[0x32] + 0x164) & 0x2400000) == 0)) {
    *(char **)(param_1[0x32] + 0x1cc) = local_68;
// ... [Truncated in summary - Full code in FUN_00592d50_00592d50.c (1404 lines)]
```

### Function `[FUN_00430560](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_00430560_00430560.c)`

- **Entry Point**: `0x00430560`
- **Signature**: `undefined FUN_00430560(void)`
- **Callers** (1): `FUN_0042a180` (0x0042a180)
- **Callees** (11): `RegEnumKeyA` (0xEXTERNAL:000001f6), `FUN_005f0970` (0x005f0970), `FUN_005f0910` (0x005f0910), `RegCloseKey` (0xEXTERNAL:000001e6), `RegOpenKeyExA` (0xEXTERNAL:000001fa), `FUN_00402f80` (0x00402f80), `RegQueryValueExA` (0xEXTERNAL:000001d5), `__security_check_cookie` (0x0063ca21), `RegQueryInfoKeyA` (0xEXTERNAL:000001f5), `FUN_0040ed10` (0x0040ed10)... [1 more]

```c

void __fastcall FUN_00430560(undefined4 *param_1)

{
  LSTATUS LVar1;
  DWORD DVar2;
  uint local_15c;
  DWORD local_14c;
  undefined4 *local_148;
  DWORD local_144;
  DWORD local_140;
  DWORD local_13c;
  DWORD local_138;
  DWORD local_134;
  HKEY local_130;
  HKEY local_12c;
  undefined1 local_128 [8];
  undefined4 local_120;
  undefined1 *local_11c;
  void *local_118;
  undefined1 *puStack_114;
  undefined4 local_110;
  BYTE local_10c;
  undefined1 local_10b [259];
  uint local_8;
  
  local_110 = 0xffffffff;
  puStack_114 = &LAB_006628d9;
  local_118 = ExceptionList;
  local_15c = DAT_007839c0 ^ (uint)&local_10c;
  local_11c = (undefined1 *)&local_15c;
  ExceptionList = &local_118;
  *param_1 = CContentTypesDatabase::vftable;
  local_148 = param_1;
  local_8 = local_15c;
  FUN_0042ffb0();
  local_110 = 0;
  FUN_0042ffb0();
  local_110._0_1_ = 1;
  FUN_0042ffb0();
  local_110._0_1_ = 3;
  *(undefined1 *)(param_1 + 0x10) = 0;
  FUN_005f0910();
  local_110 = CONCAT31(local_110._1_3_,4);
  FUN_00402f80(&DAT_0069e0f8);
  FUN_00402f80("application/octet-stream");
  local_120 = 0;
  FUN_0040ed10(local_128);
  FUN_00402f80(&DAT_0069e0f4);
  FUN_00402f80("application/octet-stream");
  local_120 = 0;
  FUN_0040ed10(local_128);
  FUN_00402f80(&DAT_0069e0f0);
  FUN_00402f80("application/zip");
  local_120 = 1;
  FUN_0040ed10(local_128);
  FUN_00402f80(&DAT_0069e0f0);
  FUN_00402f80("application/x-zip-compressed");
  local_120 = 1;
  FUN_0040ed10(local_128);
  FUN_00402f80(&DAT_0069e0bc);
  FUN_00402f80("application/x-rar-compressed");
  local_120 = 1;
  FUN_0040ed10(local_128);
  FUN_00402f80(&DAT_0069e0bc);
  FUN_00402f80("application/x-rar");
  local_120 = 1;
  FUN_0040ed10(local_128);
  FUN_00402f80(&DAT_0069e084);
  local_120 = 0;
  FUN_0040ed10(local_128);
  FUN_00402f80(&DAT_0069e080);
  FUN_00402f80("application/x-rar-compressed");
  local_120 = 0;
  FUN_0040ed10(local_128);
  FUN_00402f80(&DAT_0069e084);
  local_120 = 0;
  FUN_0040ed10(local_128);
  FUN_00402f80(&DAT_0069e07c);
  local_120 = 0;
// ... [Truncated in summary - Full code in FUN_00430560_00430560.c (1064 lines)]
```

### Function `[FUN_0059a6f0](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_0059a6f0_0059a6f0.c)`

- **Entry Point**: `0x0059a6f0`
- **Signature**: `undefined FUN_0059a6f0(void)`
- **Callers** (1): `FUN_0059c720` (0x0059c720)
- **Callees** (21): `FUN_005cefd0` (0x005cefd0), `FUN_0059af00` (0x0059af00), `FUN_0055a070` (0x0055a070), `_strstr` (0x0063e0c0), `FUN_00467f20` (0x00467f20), `FUN_005bb420` (0x005bb420), `FUN_005bbe30` (0x005bbe30), `FUN_00640553` (0x00640553), `FUN_004024f0` (0x004024f0), `GetFileAttributesW` (0xEXTERNAL:000000c6)... [11 more]

```c

/* WARNING: Removing unreachable block (ram,0x0059b858) */
/* WARNING: Removing unreachable block (ram,0x0059b245) */
/* WARNING: Removing unreachable block (ram,0x0059b80e) */
/* WARNING: Removing unreachable block (ram,0x0059b8a3) */

void __fastcall FUN_0059a6f0(int param_1)

{
  uint *puVar1;
  fd_set *exceptfds;
  byte bVar2;
  int iVar3;
  LPCWSTR lpFileName;
  undefined4 *puVar4;
  uint uVar5;
  char cVar6;
  char cVar7;
  int iVar8;
  char *pcVar9;
  char *pcVar10;
  byte *pbVar11;
  byte *pbVar12;
  wchar_t *pwVar13;
  HANDLE hFile;
  DWORD DVar14;
  undefined4 uVar15;
  BOOL BVar16;
  byte *pbVar17;
  int *piVar18;
  uint uVar19;
  int iVar20;
  bool bVar21;
  float10 fVar22;
  longlong lVar23;
  longlong lVar24;
  undefined8 uVar25;
  uint uStack_140;
  int local_130 [3];
  undefined4 auStack_124 [2];
  undefined4 uStack_11c;
  undefined4 uStack_118;
  undefined4 uStack_114;
  int iStack_10c;
  int iStack_108;
  int iStack_104;
  undefined4 uStack_100;
  undefined4 uStack_fc;
  undefined4 uStack_f4;
  undefined4 uStack_f0;
  undefined4 uStack_ec;
  undefined4 uStack_e8;
  undefined4 uStack_e4;
  undefined4 local_dc;
  undefined4 local_d8;
  int iStack_d4;
  ushort uStack_d0;
  int iStack_c8;
  ushort uStack_c4;
  uint local_bc;
  uint local_b8;
  uint local_b4;
  uint local_b0;
  int local_ac;
  ushort uStack_a8;
  uint local_a0;
  uint local_9c;
  int iStack_98;
  ushort uStack_94;
  undefined1 *local_8c;
  void *local_88;
  undefined1 *puStack_84;
  uint local_80;
  char *local_7c;
  int iStack_78;
  timeval local_74;
  int iStack_6c;
  int iStack_68;
  timeval tStack_64;
  uint local_5c;
// ... [Truncated in summary - Full code in FUN_0059a6f0_0059a6f0.c (1194 lines)]
```

### Function `[FUN_004ec770](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_004ec770_004ec770.c)`

- **Entry Point**: `0x004ec770`
- **Signature**: `undefined FUN_004ec770(void)`
- **Callers** (1): `FUN_004f6a70` (0x004f6a70)
- **Callees** (12): `SetString` (0x00402950), `FUN_0052ec00` (0x0052ec00), `FUN_00402f20` (0x00402f20), `FUN_00402f80` (0x00402f80), `FUN_004eb620` (0x004eb620), `FUN_00401970` (0x00401970), `FUN_0052de50` (0x0052de50), `FUN_004024f0` (0x004024f0), `FUN_0052e2e0` (0x0052e2e0), `FUN_00402560` (0x00402560)... [2 more]

```c

void FUN_004ec770(void)

{
  int iVar1;
  uint uStack_30;
  undefined4 local_1c;
  undefined4 local_18;
  undefined1 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  local_8 = 0xffffffff;
  puStack_c = &LAB_00671b50;
  local_10 = ExceptionList;
  uStack_30 = DAT_007839c0 ^ (uint)&stack0xfffffffc;
  local_14 = (undefined1 *)&uStack_30;
  ExceptionList = &local_10;
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b18,
             "File %s download complete.\n\nDownloaded %s (%I64d Bytes)\nFile saved as %s",0x48);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b1c,
             "This feature is available in registered version only.",0x35);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b20,
             "Cannot rename downloaded file from temp folder (%s) to %s. The file has been saved in temp folder."
             ,0x62);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b24,
             "Password verification failed! Please try again.",0x2f);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b28,"Password field has not been filled!",0x23);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b2c,
             "User name field has not been filled! Please try again.",0x36);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b30,
             "Site/path field have not been filled! Please try again",0x36);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b34,
             "Are you sure you want to delete login information for this site?",0x40);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b38,
             "Your download list has the file that will be downloaded from the ",0x41);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b3c,
             ".\nApply these username and password to this download?",0x35);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b40,
             ".\nChange username and password of this download to \"anonymous\" ?",0x40);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b44,
             "The login information for this site/path already exists in your password list.\nThe new login information hasn\'t been added. You may want to edit the existing one."
             ,0xa2);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b48,
             "You have changed the \"site/path field.\" The login information for this new site/path already exists in your password list.\nThe login information hasn\'t been changed. You may want to edit the existing one."
             ,0xcc);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b4c,"This site is temporarily unavailable!",0x25);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b50,
             "Authorization is required for this site/path. You can set login information for this download by selecting properties item in a right click context menu. Or add login information to \"Options/Sites passwords\" to use these login/password every time you are downloading files from this site."
             ,0x120);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b54,
             "Proxy authorization is required for this proxy server. You can change proxy username and password in \"Options/Proxy\""
             ,0x74);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b60,"Cannot download this file.",0x1a);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b64,"Cannot download this file.",0x1a);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b68,"Not enough memory to resume download.",0x25);
  ATL::CSimpleStringT<char,0>::SetString
            ((CSimpleStringT<char,0> *)&DAT_00789b6c,
             "No connection could be made because the target machine actively refused it. \nProbably the service is inactive on the server or you have a firewall installed that blocks Internet Download Manager. If you have a firewall, please ensure that its settings allow IDM downloading from the Internet."
             ,0x124);
// ... [Truncated in summary - Full code in FUN_004ec770_004ec770.c (1605 lines)]
```

### Function `[FUN_004b8980](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_004b8980_004b8980.c)`

- **Entry Point**: `0x004b8980`
- **Signature**: `undefined FUN_004b8980(void)`
- **Callers** (0): None
- **Callees** (13): `DefWindowProcA` (0xEXTERNAL:00000119), `GetLastError` (0xEXTERNAL:000000c7), `GetProcAddress` (0xEXTERNAL:000000c2), `__memicmp` (0x006414cd), `FUN_00406f90` (0x00406f90), `FUN_00402560` (0x00402560), `PostQuitMessage` (0xEXTERNAL:0000011a), `GlobalFree` (0xEXTERNAL:000000b1), `FUN_004011f0` (0x004011f0), `FUN_004b8def` (0x004b8def)... [3 more]

```c

void FUN_004b8980(HWND param_1,UINT param_2,int *param_3,int param_4)

{
  char cVar1;
  char *pcVar2;
  undefined4 *puVar3;
  int iVar4;
  char *pcVar5;
  char *pcVar6;
  DWORD DVar7;
  int iVar8;
  char *pcVar9;
  HGLOBAL pvVar10;
  uint uVar11;
  uint uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  uint uStack_70;
  undefined4 local_60;
  undefined4 local_5c;
  undefined1 *local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  HGLOBAL local_44;
  HGLOBAL local_40;
  int local_3c;
  int local_38;
  char *local_34;
  undefined1 *local_30;
  undefined1 local_2c [8];
  undefined1 *local_24;
  undefined1 local_20 [8];
  uint local_18;
  undefined1 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  puStack_c = &LAB_0066cf88;
  local_10 = ExceptionList;
  uStack_70 = DAT_007839c0 ^ (uint)&stack0xfffffffc;
  local_14 = (undefined1 *)&uStack_70;
  ExceptionList = &local_10;
  local_8 = 0;
  local_18 = uStack_70;
  if (param_2 == 2) {
    PostQuitMessage(0);
    DAT_0078977c = 0;
    DAT_00789778 = 0;
    FUN_004b8def();
    return;
  }
  if (param_2 != 0x1491) {
    local_8 = 0xffffffff;
    local_14 = (undefined1 *)&uStack_70;
    DefWindowProcA(param_1,param_2,(WPARAM)param_3,param_4);
    FUN_004b8def();
    return;
  }
  local_38 = 0;
  if (param_4 == 0) {
    param_4 = DAT_007898d4;
  }
  uVar15 = 0;
  uVar14 = 0;
  uVar13 = 0;
  local_14 = (undefined1 *)&uStack_70;
  iVar4 = (*DAT_00789768)(0,param_4,0,0,0);
  if (iVar4 == 0) {
    *param_3 = (int)s_error_0077e654;
  }
  else {
    local_34 = (char *)0x0;
    pcVar5 = (char *)param_3[2];
    pcVar9 = pcVar5 + 1;
    do {
// ... [Truncated in summary - Full code in FUN_004b8980_004b8980.c (273 lines)]
```

### Function `[FUN_0058b320](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_0058b320_0058b320.c)`

- **Entry Point**: `0x0058b320`
- **Signature**: `undefined FUN_0058b320(void)`
- **Callers** (1): `FUN_005ae8e0` (0x005ae8e0)
- **Callees** (28): `FUN_0056f2f0` (0x0056f2f0), `_strchr` (0x0063e000), `FUN_00587a60` (0x00587a60), `send` (0x0065d503), `FUN_004030a0` (0x004030a0), `__strnicmp` (0x0064419f), `FUN_005803d0` (0x005803d0), `_strstr` (0x0063e0c0), `FUN_00566b20` (0x00566b20), `FUN_004024f0` (0x004024f0)... [18 more]

```c

/* WARNING: Removing unreachable block (ram,0x0058bb9b) */
/* WARNING: Removing unreachable block (ram,0x0058bbab) */

void __thiscall FUN_0058b320(int param_1,int param_2)

{
  undefined4 uVar1;
  uint uVar2;
  longlong lVar3;
  longlong lVar4;
  uint *puVar5;
  char *pcVar6;
  int iVar7;
  undefined1 *puVar8;
  undefined4 *puVar9;
  int *piVar10;
  undefined4 uVar11;
  char *pcVar12;
  undefined2 *puVar13;
  int iVar14;
  char *pcVar15;
  int iVar16;
  char cVar17;
  uint uVar18;
  bool bVar19;
  longlong lVar20;
  uint uStack_21c;
  undefined1 *local_1f8;
  char *local_1ec;
  undefined2 *local_1e8;
  undefined4 local_1e0;
  uint local_1dc;
  undefined4 local_1d8;
  undefined4 local_1d4;
  undefined4 local_1d0;
  undefined4 local_1cc;
  undefined4 local_1c8;
  undefined4 local_1c4;
  char *local_1c0;
  undefined4 local_1bc;
  char *local_1b8;
  undefined4 local_1b4;
  int local_1b0;
  undefined4 local_1ac;
  undefined1 local_1a5;
  undefined4 local_1a4;
  char local_19e;
  char local_19d;
  undefined1 *local_19c [2];
  char *local_194;
  uint local_190;
  undefined1 *local_18c;
  void *local_188;
  undefined1 *puStack_184;
  int local_180;
  CHAR local_17c [96];
  undefined1 local_11c [36];
  undefined1 local_f8 [36];
  char local_d4 [13];
  undefined2 local_c7 [57];
  char local_54 [16];
  CHAR local_44 [12];
  undefined1 local_38;
  undefined4 local_37;
  undefined4 local_33;
  undefined4 local_2f;
  undefined4 local_2b;
  undefined4 local_27;
  undefined4 local_23;
  undefined4 local_1f;
  undefined4 local_1b;
  uint local_14;
  int local_10;
  uint local_8;
  
  puStack_184 = &LAB_0067f69b;
  local_188 = ExceptionList;
  uStack_21c = DAT_007839c0 ^ (uint)local_17c;
  local_18c = (undefined1 *)&uStack_21c;
// ... [Truncated in summary - Full code in FUN_0058b320_0058b320.c (945 lines)]
```

### Function `[FUN_004cbc90](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_004cbc90_004cbc90.c)`

- **Entry Point**: `0x004cbc90`
- **Signature**: `undefined FUN_004cbc90(void)`
- **Callers** (3): `FUN_004cc000` (0x004cc000), `FUN_004aa050` (0x004aa050), `FUN_00455ea0` (0x00455ea0)
- **Callees** (15): `__security_check_cookie` (0x0063ca21), `RegCreateKeyExA` (0xEXTERNAL:000001f7), `FUN_004c4da0` (0x004c4da0), `GetDeviceCaps` (0xEXTERNAL:000001c6), `FUN_004c4c30` (0x004c4c30), `RegCloseKey` (0xEXTERNAL:000001e6), `FUN_0063ec80` (0x0063ec80), `FUN_004c4d90` (0x004c4d90), `SendMessageA` (0xEXTERNAL:0000018c), `ReleaseDC` (0xEXTERNAL:00000176)... [5 more]

```c

void __fastcall FUN_004cbc90(CListCtrl *param_1)

{
  uint uVar1;
  LSTATUS LVar2;
  HDC pHVar3;
  LRESULT LVar4;
  int iVar5;
  int iVar6;
  CListCtrl *pCVar7;
  WPARAM wParam;
  undefined1 auStack_254 [7];
  char local_24d;
  HKEY local_24c;
  CListCtrl *local_248;
  char local_244;
  int local_240;
  int local_23c;
  LRESULT local_238;
  DWORD local_234 [134];
  uint local_1c;
  void *local_14;
  undefined1 *puStack_10;
  undefined4 uStack_c;
  
  uStack_c = 0xffffffff;
  puStack_10 = &LAB_0066ea5b;
  local_14 = ExceptionList;
  local_1c = DAT_007839c0 ^ (uint)auStack_254;
  uVar1 = DAT_007839c0 ^ (uint)&stack0xfffffda0;
  ExceptionList = &local_14;
  LVar2 = RegCreateKeyExA((HKEY)&DAT_80000001,"Software\\DownloadManager\\ListSettings",0,"",0,
                          0xf003f,(LPSECURITY_ATTRIBUTES)0x0,&local_24c,local_234);
  if (LVar2 == 0) {
    iVar6 = 0x60;
    local_24d = '\0';
    local_240 = 0x60;
    local_23c = 0x60;
    pHVar3 = GetDC((HWND)0x0);
    if (pHVar3 != (HDC)0x0) {
      iVar6 = GetDeviceCaps(pHVar3,0x58);
      local_240 = iVar6;
      local_23c = GetDeviceCaps(pHVar3,0x5a);
      ReleaseDC((HWND)0x0,pHVar3);
    }
    local_244 = '\x01';
    if (iVar6 != 0x60) {
      local_24d = '\x01';
    }
    pCVar7 = param_1 + 0x120;
    wParam = 0;
    local_248 = pCVar7;
    do {
      if (*(int *)(pCVar7 + 4) == 0) {
        if (local_24d != '\0') {
          LVar4 = *(LRESULT *)pCVar7;
          goto LAB_004cbdd5;
        }
      }
      else {
        if (local_24d == '\0') {
          LVar4 = SendMessageA(*(HWND *)(param_1 + 0x20),0x101d,wParam,0);
        }
        else {
          LVar4 = SendMessageA(*(HWND *)(param_1 + 0x20),0x101d,wParam,0);
LAB_004cbdd5:
          LVar4 = FUN_004a9b10(LVar4);
        }
        *(LRESULT *)pCVar7 = LVar4;
      }
      if (*(double *)(param_1 + 0x118) != 0.0) {
        LVar4 = FUN_0063ec80();
        *(LRESULT *)pCVar7 = LVar4;
      }
      wParam = wParam + 1;
      pCVar7 = pCVar7 + 8;
    } while ((int)wParam < 0xc);
    RegSetValueExA(local_24c,"FileName",0,0,(BYTE *)local_248,8);
    RegSetValueExA(local_24c,"DateAdded",0,0,(BYTE *)(param_1 + 0x128),8);
// ... [Truncated in summary - Full code in FUN_004cbc90_004cbc90.c (128 lines)]
```

### Function `[FUN_0042a180](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_0042a180_0042a180.c)`

- **Entry Point**: `0x0042a180`
- **Signature**: `undefined FUN_0042a180(void)`
- **Callers** (2): `FUN_0042cea0` (0x0042cea0), `FUN_0042cf20` (0x0042cf20)
- **Callees** (71): `FUN_00536830` (0x00536830), `FUN_004263c0` (0x004263c0), `FindWindowA` (0xEXTERNAL:00000168), `CoCreateInstance` (0xEXTERNAL:00000252), `FUN_0057f910` (0x0057f910), `GetWindowThreadProcessId` (0xEXTERNAL:00000169), `FUN_004030a0` (0x004030a0), `FUN_004c4c30` (0x004c4c30), `RegCloseKey` (0xEXTERNAL:000001e6), `FUN_004c4d90` (0x004c4d90)... [61 more]

```c

/* WARNING: Function: __alloca_probe replaced with injection: alloca_probe */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0042a180(LPCWSTR param_1,LPCWSTR param_2,LPCWSTR param_3,BSTR param_4,LPCWSTR param_5,
                 LPCWSTR param_6,short *param_7,short *param_8,int param_9,LPCWSTR param_10,
                 short *param_11,short *param_12,wchar_t *param_13,LPCWSTR param_14,
                 wchar_t *param_15,int param_16,int param_17,LPCWSTR param_18,LPCWSTR param_19,
                 LPCWSTR param_20,LPCWSTR param_21,LPCWSTR param_22,LPCWSTR param_23,
                 LPCWSTR param_24,LPCWSTR param_25,LPCWSTR param_26,LPCWSTR param_27,
                 undefined4 param_28,LPCWSTR param_29,int param_30,UINT param_31,DWORD param_32,
                 DWORD param_33,int param_34)

{
  byte bVar1;
  WCHAR WVar2;
  OLECHAR OVar3;
  short sVar4;
  wchar_t wVar5;
  uint uVar6;
  char cVar7;
  undefined1 uVar8;
  int *lParam;
  HRESULT HVar9;
  int iVar10;
  LPSTR pCVar11;
  DWORD DVar12;
  char *pcVar13;
  char *pcVar14;
  int *piVar15;
  int iVar16;
  BSTR pOVar17;
  BSTR pOVar18;
  wchar_t *pwVar19;
  short *psVar20;
  char *pcVar21;
  wchar_t *pwVar22;
  LPCWSTR pWVar23;
  LPCWSTR pWVar24;
  byte *pbVar25;
  HWND pHVar26;
  LSTATUS LVar27;
  WCHAR *pWVar28;
  BYTE *_Str2;
  UINT UVar29;
  bool bVar30;
  bool bVar31;
  uint uStack_7dc8;
  int local_7d74;
  DWORD local_7d70;
  LPCWSTR local_7d6c;
  BSTR local_7d68;
  LPCWSTR local_7d64;
  LPCWSTR local_7d60;
  undefined1 *local_7d5c;
  void *local_7d58;
  undefined1 *puStack_7d54;
  undefined4 local_7d50;
  LPCWSTR local_7d4c;
  short *local_7d48;
  LPCWSTR local_7d44;
  LPCWSTR local_7d40;
  undefined1 local_7d3b;
  undefined1 local_7d3a;
  undefined1 local_7d39;
  LPCWSTR local_7d38;
  LPCWSTR local_7d34;
  LPCWSTR local_7d30;
  char *local_7d2c;
  undefined1 local_7d27;
  char local_7d26;
  char local_7d25;
  LPCWSTR local_7d24;
  LPCWSTR local_7d20;
  LPCWSTR local_7d1c;
  UINT local_7d18;
  DWORD local_7d14;
  char local_7d0e;
  char local_7d0d;
  LPCWSTR local_7d0c;
// ... [Truncated in summary - Full code in FUN_0042a180_0042a180.c (1704 lines)]
```

### Function `[FUN_0045f0c0](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_0045f0c0_0045f0c0.c)`

- **Entry Point**: `0x0045f0c0`
- **Signature**: `undefined FUN_0045f0c0(void)`
- **Callers** (0): None
- **Callees** (5): `__security_check_cookie` (0x0063ca21), `FUN_00402310` (0x00402310), `GetFileAttributesW` (0xEXTERNAL:000000c6), `FUN_004011f0` (0x004011f0), `FUN_00447030` (0x00447030)

```c

void FUN_0045f0c0(void)

{
  undefined *puVar1;
  int iVar2;
  LPCWSTR pWVar3;
  wchar_t *pwVar4;
  LPCWSTR local_1c;
  WCHAR local_18 [2];
  undefined4 local_14;
  uint local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_00661858;
  local_c = ExceptionList;
  local_10 = DAT_007839c0 ^ (uint)&local_1c;
  ExceptionList = &local_c;
  local_1c = (LPCWSTR)0x0;
  local_14 = 0;
  local_18[0] = L'\0';
  local_4 = 0;
  if (DAT_00788cdc != 0) {
    puVar1 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar1 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar1,L"oldjsproxy.dll",0);
    pWVar3 = local_1c;
    if (local_1c == (LPCWSTR)0x0) {
      pWVar3 = local_18;
    }
    GetFileAttributesW(pWVar3);
  }
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"Uninstall.exe",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"IDManTypeInfo.tlb",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"libssl.dll",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"IDMMsgHost.json",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
// ... [Truncated in summary - Full code in FUN_0045f0c0_0045f0c0.c (275 lines)]
```

### Function `[FUN_005dc710](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_005dc710_005dc710.c)`

- **Entry Point**: `0x005dc710`
- **Signature**: `undefined FUN_005dc710(void)`
- **Callers** (1): `FUN_005dcc70` (0x005dcc70)
- **Callees** (9): `FUN_005d34e0` (0x005d34e0), `_strncmp` (0x0063e17b), `FID_conflict:_sscanf` (0x0063e40e), `FUN_00403060` (0x00403060), `FUN_004024f0` (0x004024f0), `__memicmp` (0x006414cd), `FUN_005db1e0` (0x005db1e0), `FUN_00402560` (0x00402560), `FUN_00402f20` (0x00402f20)

```c

void __thiscall FUN_005dc710(int param_1,byte *param_2)

{
  byte bVar1;
  int iVar2;
  byte *pbVar3;
  int iVar4;
  byte *pbVar5;
  int iVar6;
  bool bVar7;
  int local_14;
  int local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  pbVar3 = param_2;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_00684380;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  for (iVar6 = 0; ((bVar1 = param_2[iVar6], bVar1 == 0x20 || (bVar1 == 9)) || (bVar1 == 0xd));
      iVar6 = iVar6 + 1) {
  }
  pbVar5 = param_2 + iVar6;
  iVar2 = __memicmp(pbVar5,"Cookie2:",8);
  if (iVar2 == 0) {
    FUN_005db1e0(pbVar5 + 8,param_1 + 0x38c);
  }
  else {
    iVar2 = __memicmp(pbVar5,"Cookie:",7);
    if (iVar2 == 0) {
      FUN_005db1e0(pbVar5 + 7,param_1 + 0x394);
    }
    else {
      iVar2 = __memicmp(pbVar5,"Accept-Language:",0x10);
      if (iVar2 == 0) {
        FUN_005db1e0(pbVar5 + 0x10,param_1 + 0x390);
      }
      else {
        iVar2 = __memicmp(pbVar5,"Accept-Charset:",0xf);
        if (iVar2 == 0) {
          FUN_005db1e0(pbVar5 + 0xf,param_1 + 0x3a0);
        }
        else {
          iVar2 = __memicmp(pbVar5,"Origin:",7);
          if (iVar2 == 0) {
            FUN_005db1e0(pbVar5 + 7,param_1 + 0x398);
          }
          else {
            iVar2 = __memicmp(pbVar5,"Accept:",7);
            if (iVar2 == 0) {
              FUN_005db1e0(pbVar5 + 7,param_1 + 0x39c);
            }
            else {
              iVar2 = __memicmp(pbVar5,"Accept-Encoding:",0x10);
              if (iVar2 == 0) {
                FUN_005db1e0(pbVar5 + 0x10,param_1 + 0x420);
              }
              else {
                iVar2 = __memicmp(pbVar5,"Content-Type:",0xd);
                if (iVar2 == 0) {
                  if (*(char *)(param_1 + 0x1a5) != '\0') {
                    FUN_005db1e0(pbVar5 + 0xd,param_1 + 0xd0);
                  }
                }
                else {
                  iVar2 = __memicmp(pbVar5,"Range:",6);
                  if (iVar2 == 0) {
                    FUN_004024f0();
                    local_4 = 0;
                    FUN_005db1e0(pbVar5 + 6,&param_2);
                    pbVar5 = &DAT_006d56ac;
                    pbVar3 = param_2;
                    do {
                      bVar1 = *pbVar3;
                      bVar7 = bVar1 < *pbVar5;
                      if (bVar1 != *pbVar5) {
LAB_005dc920:
// ... [Truncated in summary - Full code in FUN_005dc710_005dc710.c (251 lines)]
```

### Function `[FUN_0041cac0](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_0041cac0_0041cac0.c)`

- **Entry Point**: `0x0041cac0`
- **Signature**: `undefined FUN_0041cac0(void)`
- **Callers** (1): `FUN_0041f230` (0x0041f230)
- **Callees** (6): `GetSystemDirectoryW` (0xEXTERNAL:00000093), `FUN_004021a0` (0x004021a0), `__security_check_cookie` (0x0063ca21), `FUN_00402310` (0x00402310), `FUN_0041bd70` (0x0041bd70), `FUN_004011f0` (0x004011f0)

```c

void FUN_0041cac0(void)

{
  UINT UVar1;
  undefined *puVar2;
  undefined2 *puVar3;
  wchar_t *pwVar4;
  uint uStack_254;
  undefined2 local_240 [2];
  undefined4 local_23c;
  undefined1 *local_238;
  void *local_234;
  undefined1 *puStack_230;
  uint local_22c;
  undefined2 *local_228;
  undefined2 local_224 [2];
  undefined4 local_220;
  undefined2 *local_21c;
  undefined2 local_218 [2];
  undefined4 local_214;
  WCHAR local_210 [260];
  uint local_8;
  
  puStack_230 = &LAB_00661018;
  local_234 = ExceptionList;
  uStack_254 = DAT_007839c0 ^ (uint)&local_228;
  local_238 = (undefined1 *)&uStack_254;
  ExceptionList = &local_234;
  local_22c = 0;
  local_8 = uStack_254;
  UVar1 = GetSystemDirectoryW(local_210,0x104);
  if (UVar1 != 0) {
    local_23c = 0;
    local_240[0] = 0;
    local_22c._0_1_ = 1;
    FUN_004021a0("%s\\drivers\\idmtdi.sys",local_210,0);
    FUN_0041bd70(local_240);
    local_228 = (undefined2 *)0x0;
    local_220 = 0;
    local_224[0] = 0;
    local_22c._0_1_ = 2;
    FUN_004021a0("%s\\drivers\\idmwfp.sys",local_210,0);
    puVar3 = local_228;
    if (local_228 == (undefined2 *)0x0) {
      puVar3 = local_224;
    }
    FUN_0041bd70(puVar3);
    local_22c._0_1_ = 1;
    FUN_004011f0();
    local_22c = (uint)local_22c._1_3_ << 8;
    FUN_004011f0();
  }
  local_21c = (undefined2 *)0x0;
  local_214 = 0;
  local_218[0] = 0;
  local_22c = CONCAT31(local_22c._1_3_,3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMIECC.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMIECC64.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
// ... [Truncated in summary - Full code in FUN_0041cac0_0041cac0.c (544 lines)]
```

### Function `[FUN_005d9c60](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_005d9c60_005d9c60.c)`

- **Entry Point**: `0x005d9c60`
- **Signature**: `undefined FUN_005d9c60(void)`
- **Callers** (6): `FUN_00589514` (0x00589514), `FUN_00592d50` (0x00592d50), `FUN_0059c720` (0x0059c720), `FUN_005146e0` (0x005146e0), `FUN_005be140` (0x005be140), `FUN_00514370` (0x00514370)
- **Callees** (14): `FUN_005d79f0` (0x005d79f0), `FUN_005c11b0` (0x005c11b0), `RegDeleteValueA` (0xEXTERNAL:000001f9), `FUN_00426430` (0x00426430), `FUN_00402f20` (0x00402f20), `FUN_00402560` (0x00402560), `FUN_005d5770` (0x005d5770), `FUN_005d3960` (0x005d3960), `FUN_00402f80` (0x00402f80), `_strrchr` (0x0063f280)... [4 more]

```c

void __thiscall FUN_005d9c60(undefined4 *param_1,int param_2,int param_3,int param_4)

{
  BYTE BVar1;
  int iVar2;
  uint *puVar3;
  char cVar4;
  BYTE *pBVar5;
  char *pcVar6;
  char *pcVar7;
  BYTE *pBVar8;
  uint uStack_40;
  BYTE local_2c [8];
  int local_24;
  int local_20;
  BYTE *local_1c;
  uint local_18;
  undefined1 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  puStack_c = &LAB_00684178;
  local_10 = ExceptionList;
  uStack_40 = DAT_007839c0 ^ (uint)&stack0xfffffffc;
  local_14 = (undefined1 *)&uStack_40;
  ExceptionList = &local_10;
  local_8 = 0;
  if ((param_2 != 0) &&
     ((puVar3 = &uStack_40, param_1[0x129] != 0 ||
      (cVar4 = FUN_005d6890(param_2), puVar3 = (uint *)local_14, cVar4 != '\0')))) {
    local_14 = (undefined1 *)puVar3;
    pBVar8 = (BYTE *)*param_1;
    pBVar5 = pBVar8;
    do {
      BVar1 = *pBVar5;
      pBVar5 = pBVar5 + 1;
    } while (BVar1 != '\0');
    RegSetValueExA((HKEY)param_1[0x129],"Host",0,1,pBVar8,(DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))))
    ;
    pBVar8 = (BYTE *)param_1[1];
    if (pBVar8 != (BYTE *)0x0) {
      pBVar5 = pBVar8;
      do {
        BVar1 = *pBVar5;
        pBVar5 = pBVar5 + 1;
      } while (BVar1 != '\0');
      RegSetValueExA((HKEY)param_1[0x129],"Path",0,1,pBVar8,
                     (DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))));
    }
    pcVar6 = _strstr((char *)*param_1,"googlevideo.com");
    if (pcVar6 == (char *)0x0) {
      pBVar8 = (BYTE *)param_1[2];
      pBVar5 = pBVar8;
      do {
        BVar1 = *pBVar5;
        pBVar5 = pBVar5 + 1;
      } while (BVar1 != '\0');
      RegSetValueExA((HKEY)param_1[0x129],"FileName",0,1,pBVar8,
                     (DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))));
    }
    else {
      FUN_005d5770(param_1[0x129],param_1[2],&DAT_0069c668,"FileName");
    }
    pBVar8 = (BYTE *)param_1[3];
    pBVar5 = pBVar8;
    do {
      BVar1 = *pBVar5;
      pBVar5 = pBVar5 + 1;
    } while (BVar1 != '\0');
    RegSetValueExA((HKEY)param_1[0x129],"User",0,1,pBVar8,(DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))))
    ;
    FUN_005c11b0();
    if (param_1[0xf] != 0) {
      FUN_005d3960();
    }
    pBVar8 = (BYTE *)param_1[0x10];
    if (pBVar8 != (BYTE *)0x0) {
      pBVar5 = pBVar8;
// ... [Truncated in summary - Full code in FUN_005d9c60_005d9c60.c (325 lines)]
```

### Function `[FUN_00412e50](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_00412e50_00412e50.c)`

- **Entry Point**: `0x00412e50`
- **Signature**: `undefined FUN_00412e50(void)`
- **Callers** (1): `FUN_00413170` (0x00413170)
- **Callees** (9): `RegOpenKeyExA` (0xEXTERNAL:000001fa), `__security_check_cookie` (0x0063ca21), `FUN_00402310` (0x00402310), `__wcsicmp` (0x00641f67), `RegCreateKeyExA` (0xEXTERNAL:000001f7), `RegSetValueExW` (0xEXTERNAL:000001ed), `FUN_004011f0` (0x004011f0), `RegCloseKey` (0xEXTERNAL:000001e6), `RegQueryValueExW` (0xEXTERNAL:000001f2)

```c

void FUN_00412e50(int param_1,int param_2,int param_3)

{
  wchar_t *pwVar1;
  wchar_t wVar2;
  undefined *puVar3;
  HKEY pHVar4;
  LSTATUS LVar5;
  int iVar6;
  wchar_t *pwVar7;
  char *pcVar8;
  wchar_t *lpData;
  code *pcVar9;
  REGSAM samDesired;
  HKEY local_438;
  HKEY local_434;
  DWORD local_430;
  wchar_t *local_42c;
  wchar_t local_428 [2];
  undefined4 local_424;
  wchar_t local_420 [520];
  uint local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_006602cb;
  local_c = ExceptionList;
  local_10 = DAT_007839c0 ^ (uint)&local_438;
  ExceptionList = &local_c;
  local_434 = (HKEY)0x0;
  local_42c = (wchar_t *)0x0;
  local_424 = 0;
  local_428[0] = L'\0';
  local_4 = 0;
  pwVar7 = L"IDMMsgHost.json";
  if (param_1 == 0) {
    pwVar7 = L"IDMMsgHostMoz.json";
  }
  puVar3 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar3 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar3,pwVar7,0);
  if (param_1 == 0) {
    pcVar8 = "SOFTWARE\\Mozilla\\NativeMessagingHosts\\com.tonec.idm";
    pHVar4 = (HKEY)&DAT_80000001;
  }
  else {
    pcVar8 = "SOFTWARE\\Google\\Chrome\\NativeMessagingHosts\\com.tonec.idm";
    pHVar4 = (HKEY)((param_3 != 0) + 0x80000001);
  }
  LVar5 = RegOpenKeyExA(pHVar4,pcVar8,0,(-(uint)(param_2 != 0) & 0x100) + 0x20019,&local_438);
  if (LVar5 == 0) {
    local_430 = 0x410;
    LVar5 = RegQueryValueExW(local_438,L"",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)local_420,&local_430);
    if (LVar5 == 0) {
      pwVar7 = local_42c;
      if (local_42c == (wchar_t *)0x0) {
        pwVar7 = local_428;
      }
      iVar6 = __wcsicmp(local_420,pwVar7);
      if (iVar6 == 0) {
        local_434 = (HKEY)0x1;
      }
    }
    RegCloseKey(local_438);
    if (local_434 != (HKEY)0x0) goto LAB_00413131;
  }
  if (param_1 == 0) {
    pcVar8 = "SOFTWARE\\Mozilla\\NativeMessagingHosts";
    pHVar4 = (HKEY)&DAT_80000001;
  }
  else {
    pcVar8 = "SOFTWARE\\Google\\Chrome\\NativeMessagingHosts";
    pHVar4 = (HKEY)((param_3 != 0) + 0x80000001);
  }
  samDesired = (-(uint)(param_2 != 0) & 0x100) + 0xf003f;
  LVar5 = RegOpenKeyExA(pHVar4,pcVar8,0,samDesired,&local_438);
// ... [Truncated in summary - Full code in FUN_00412e50_00412e50.c (134 lines)]
```

### Function `[FUN_0041b0d0](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_0041b0d0_0041b0d0.c)`

- **Entry Point**: `0x0041b0d0`
- **Signature**: `undefined FUN_0041b0d0(void)`
- **Callers** (1): `FUN_0041b1f0` (0x0041b1f0)
- **Callees** (6): `__security_check_cookie` (0x0063ca21), `FUN_00402310` (0x00402310), `RegSetValueExW` (0xEXTERNAL:000001ed), `FUN_004011f0` (0x004011f0), `RegCloseKey` (0xEXTERNAL:000001e6), `FUN_0041ad20` (0x0041ad20)

```c

void FUN_0041b0d0(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  short *psVar1;
  short sVar2;
  int iVar3;
  undefined *puVar4;
  short *psVar5;
  short *lpData;
  HKEY local_20;
  short *local_1c;
  short local_18 [2];
  undefined4 local_14;
  uint local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_00660e38;
  local_c = ExceptionList;
  local_10 = DAT_007839c0 ^ (uint)&local_20;
  ExceptionList = &local_c;
  iVar3 = FUN_0041ad20(param_1,param_2,param_3,&local_20,DAT_007839c0 ^ (uint)&stack0xffffffd8);
  if (iVar3 != 0) {
    local_1c = (short *)0x0;
    local_14 = 0;
    local_18[0] = 0;
    local_4 = 0;
    puVar4 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar4 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar4,L"IDMMsgHost.json",0);
    psVar5 = local_1c;
    if (local_1c == (short *)0x0) {
      psVar5 = local_18;
    }
    psVar1 = psVar5 + 1;
    do {
      sVar2 = *psVar5;
      psVar5 = psVar5 + 1;
    } while (sVar2 != 0);
    lpData = local_1c;
    if (local_1c == (short *)0x0) {
      lpData = local_18;
    }
    RegSetValueExW(local_20,L"",0,1,(BYTE *)lpData,((int)psVar5 - (int)psVar1 >> 1) * 2 + 2);
    RegCloseKey(local_20);
    local_4 = 0xffffffff;
    FUN_004011f0();
  }
  ExceptionList = local_c;
  __security_check_cookie(local_10 ^ (uint)&local_20);
  return;
}
```

### Function `[FUN_005ae8e0](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_005ae8e0_005ae8e0.c)`

- **Entry Point**: `0x005ae8e0`
- **Signature**: `undefined FUN_005ae8e0(void)`
- **Callers** (0): None
- **Callees** (33): `FUN_0056f2f0` (0x0056f2f0), `FUN_005805e0` (0x005805e0), `FUN_005b0164` (0x005b0164), `FUN_005805a0` (0x005805a0), `send` (0x0065d503), `FUN_004030a0` (0x004030a0), `FUN_005805c0` (0x005805c0), `__strnicmp` (0x0064419f), `FUN_00580560` (0x00580560), `FUN_005adff0` (0x005adff0)... [23 more]

```c

/* WARNING: Removing unreachable block (ram,0x005af41f) */
/* WARNING: Removing unreachable block (ram,0x005af42f) */

void __thiscall FUN_005ae8e0(int *param_1,int param_2)

{
  uint uVar1;
  longlong lVar2;
  longlong lVar3;
  uint *puVar4;
  char cVar5;
  char *pcVar6;
  int iVar7;
  undefined1 *puVar8;
  undefined4 *puVar9;
  char *pcVar10;
  undefined2 *puVar11;
  int iVar12;
  char *pcVar13;
  undefined4 uVar14;
  int iVar15;
  uint uVar16;
  bool bVar17;
  undefined8 uVar18;
  longlong lVar19;
  uint uStack_21c;
  undefined4 local_1ec;
  uint local_1e8;
  undefined4 local_1e4;
  undefined4 local_1e0;
  undefined4 local_1dc;
  undefined4 local_1d8;
  undefined4 local_1d4;
  undefined4 local_1d0;
  undefined4 local_1cc;
  char *local_1c8;
  int local_1c4;
  char *local_1c0;
  undefined4 local_1bc;
  undefined4 local_1b8;
  char *local_1b4;
  undefined1 local_1ad;
  undefined4 local_1ac;
  undefined4 local_1a8;
  undefined2 *local_1a4;
  char local_19d;
  char *local_19c;
  char local_195;
  char *local_194;
  char *local_18c;
  uint local_188;
  undefined1 *local_184;
  void *local_180;
  undefined1 *puStack_17c;
  undefined4 local_178;
  CHAR local_174 [96];
  char local_114 [13];
  undefined2 local_107 [57];
  undefined1 local_94 [36];
  CHAR local_70 [12];
  undefined1 local_64;
  undefined4 local_63;
  undefined4 local_5f;
  undefined4 local_5b;
  undefined4 local_57;
  undefined4 local_53;
  undefined4 local_4f;
  undefined4 local_4b;
  undefined4 local_47;
  undefined1 local_40;
  undefined4 local_3f;
  undefined4 local_3b;
  undefined4 local_37;
  undefined4 local_33;
  undefined4 local_2f;
  undefined4 local_2b;
  undefined4 local_27;
  undefined4 local_23;
  char local_1c [8];
// ... [Truncated in summary - Full code in FUN_005ae8e0_005ae8e0.c (1068 lines)]
```

### Function `[FUN_00610980](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_00610980_00610980.c)`

- **Entry Point**: `0x00610980`
- **Signature**: `undefined FUN_00610980(void)`
- **Callers** (1): `FUN_00610ab0` (0x00610ab0)
- **Callees** (9): `CreateEventA` (0xEXTERNAL:000000a9), `FUN_0060f3c0` (0x0060f3c0), `GetCurrentProcessId` (0xEXTERNAL:00000090), `ProcessIdToSessionId` (0xEXTERNAL:0000007a), `__security_check_cookie` (0x0063ca21), `__itoa` (0x00644d86), `FUN_00610e74` (0x00610e74), `CreateFileA` (0xEXTERNAL:000000b8), `FUN_00610800` (0x00610800)

```c

void __fastcall FUN_00610980(int *param_1)

{
  DWORD dwProcessId;
  HANDLE pvVar1;
  int iVar2;
  char *pcVar3;
  CHAR *pCVar4;
  DWORD *pSessionId;
  DWORD local_48;
  CHAR local_44 [27];
  char local_29 [37];
  uint local_4;
  
  pSessionId = &local_48;
  local_4 = DAT_007839c0 ^ (uint)&local_48;
  local_48 = 0;
  dwProcessId = GetCurrentProcessId();
  ProcessIdToSessionId(dwProcessId,pSessionId);
  pvVar1 = CreateEventA((LPSECURITY_ATTRIBUTES)0x0,1,0,(LPCSTR)0x0);
  param_1[1] = (int)pvVar1;
  if (pvVar1 != (HANDLE)0x0) {
    pcVar3 = "\\\\.\\pipe\\IDMNetworkMonitor.";
    pCVar4 = local_44;
    for (iVar2 = 7; iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pcVar3;
      pcVar3 = pcVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
    local_29[1] = '\0';
    local_29[2] = '\0';
    local_29[3] = '\0';
    local_29[4] = '\0';
    local_29[5] = '\0';
    local_29[6] = '\0';
    local_29[7] = '\0';
    local_29[8] = '\0';
    local_29[9] = '\0';
    local_29[10] = '\0';
    local_29[0xb] = '\0';
    local_29[0xc] = '\0';
    local_29[0xd] = '\0';
    local_29[0xe] = '\0';
    local_29[0xf] = '\0';
    local_29[0x10] = '\0';
    local_29[0x11] = '\0';
    local_29[0x12] = '\0';
    local_29[0x13] = '\0';
    local_29[0x14] = '\0';
    local_29[0x15] = '\0';
    local_29[0x16] = '\0';
    local_29[0x17] = '\0';
    local_29[0x18] = '\0';
    local_29[0x19] = '\0';
    local_29[0x1a] = '\0';
    local_29[0x1b] = '\0';
    local_29[0x1c] = '\0';
    local_29[0x1d] = '\0';
    local_29[0x1e] = '\0';
    local_29[0x1f] = '\0';
    local_29[0x20] = '\0';
    local_29[0x21] = '\0';
    local_29[0x22] = '\0';
    local_29[0x23] = '\0';
    local_29[0x24] = '\0';
    __itoa(local_48,local_29,10);
    pvVar1 = CreateFileA(local_44,0xc0000000,0,(LPSECURITY_ATTRIBUTES)0x0,3,0x60000000,(HANDLE)0x0);
    *param_1 = (int)pvVar1;
    if (pvVar1 != (HANDLE)0xffffffff) {
      param_1[2] = 0;
      param_1[3] = 0;
      param_1[4] = 0;
      param_1[5] = 0;
      param_1[6] = 0;
      param_1[7] = 0;
      param_1[8] = 0;
      param_1[9] = 0;
      param_1[10] = 0;
      param_1[0xb] = 0;
// ... [Truncated in summary - Full code in FUN_00610980_00610980.c (96 lines)]
```

### Function `[FUN_00582780](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_00582780_00582780.c)`

- **Entry Point**: `0x00582780`
- **Signature**: `undefined FUN_00582780(void)`
- **Callers** (1): `FUN_00584c40` (0x00584c40)
- **Callees** (10): `_strchr` (0x0063e000), `FUN_00580d50` (0x00580d50), `_strstr` (0x0063e0c0), `FUN_00566d80` (0x00566d80), `FUN_005803a0` (0x005803a0), `FUN_00581c80` (0x00581c80), `FUN_00574b80` (0x00574b80), `FID_conflict:_sscanf` (0x0063e40e), `FUN_00566d50` (0x00566d50), `__memicmp` (0x006414cd)... [0 more]

```c

int __thiscall
FUN_00582780(int param_1,undefined4 param_2,undefined4 param_3,undefined4 *param_4,
            undefined4 param_5)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  char *pcVar4;
  int iVar5;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_0067eda0;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  local_8 = 0;
  do {
    iVar2 = FUN_00580d50();
    if (iVar2 != 0) {
      ExceptionList = local_10;
      return iVar2;
    }
    while( true ) {
      pcVar4 = *(char **)(param_1 + 0x3258);
      if (*pcVar4 == '\n') goto LAB_005827f1;
      if ((*pcVar4 == '\r') && (pcVar4[1] == '\n')) break;
      if ((*(char *)(param_1 + 0x3280) != '\0') &&
         (pcVar4 = _strstr(pcVar4,*(char **)(param_1 + 0x327c)), pcVar4 != (char *)0x0)) {
        *(undefined1 *)(param_1 + 0x3280) = 0;
      }
      iVar2 = *(int *)(param_1 + 0x3258);
      iVar5 = FUN_005803a0(iVar2);
      iVar2 = __memicmp((void *)(iVar2 + iVar5),"content-length:",0xf);
      if (iVar2 == 0) {
        FID_conflict__sscanf((char *)(*(int *)(param_1 + 0x3258) + 0xf + iVar5),"%I64d",param_2);
      }
      iVar2 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar5),"Content-Range:",0xe);
      if (iVar2 == 0) {
        pcVar4 = _strstr((char *)(*(int *)(param_1 + 0x3258) + 0xe + iVar5),"bytes");
        if (pcVar4 != (char *)0x0) {
          FUN_005803a0(pcVar4 + 5);
          FID_conflict__sscanf(pcVar4 + iVar5 + 5,"%I64d",param_3);
        }
        pcVar4 = _strchr((char *)(*(int *)(param_1 + 0x3258) + 0xe + iVar5),0x2f);
        if (pcVar4 != (char *)0x0) {
          cVar1 = pcVar4[1];
          if ((cVar1 != '\r') && (cVar1 != '\n')) {
            FID_conflict__sscanf(pcVar4 + 1,"%I64d",param_5);
          }
        }
        pcVar4 = _strchr((char *)(*(int *)(param_1 + 0x3258) + 0xe + iVar5),0x2d);
        if (pcVar4 != (char *)0x0) {
          cVar1 = pcVar4[1];
          if (((cVar1 != '\r') && (cVar1 != '\n')) &&
             (iVar2 = FID_conflict__sscanf(pcVar4 + 1,"%I64d",param_4), iVar2 < 1)) {
            *param_4 = 0;
            param_4[1] = 0;
          }
        }
      }
      FUN_00581c80();
      iVar2 = FUN_00580d50();
      if (iVar2 != 0) {
        ExceptionList = local_10;
        return iVar2;
      }
    }
  } while (*(char *)(param_1 + 0x3280) != '\0');
LAB_005827f1:
  *(undefined4 *)(param_1 + 0x3204) = 0;
  uVar3 = FUN_00566d50();
  FUN_00574b80(uVar3);
  FUN_00566d80();
  *(undefined4 *)(param_1 + 0xa4) = 5;
  ExceptionList = local_10;
  return 0;
}
```

### Function `[FUN_00610540](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/IDMan_exe/FUN_00610540_00610540.c)`

- **Entry Point**: `0x00610540`
- **Signature**: `undefined FUN_00610540(void)`
- **Callers** (1): `FUN_006107b0` (0x006107b0)
- **Callees** (8): `FUN_0060e130` (0x0060e130), `FUN_00610110` (0x00610110), `FUN_0060ecf0` (0x0060ecf0), `FID_conflict:_sscanf` (0x0063e40e), `FUN_0060e920` (0x0060e920), `_strspn` (0x006458e0), `FUN_00610020` (0x00610020), `FUN_0060f7a0` (0x0060f7a0)

```c

void FUN_00610540(char *param_1,uint param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  size_t sVar4;
  char *pcVar5;
  char *pcVar6;
  char *pcVar7;
  uint local_10;
  undefined4 local_c;
  int local_8;
  undefined1 local_4 [4];
  
  uVar2 = param_2;
  pcVar6 = param_1;
  local_10 = 0;
  if ((0xb < param_2) &&
     (iVar3 = FID_conflict__sscanf
                        (param_1,"MSG#%u#%u#%u#%u%n",&param_1,local_4,&local_c,&param_2,&local_8),
     iVar3 == 4)) {
    pcVar6 = pcVar6 + local_8;
    for (pcVar5 = pcVar6;
        (pcVar7 = pcVar6, pcVar5 < pcVar6 + (uVar2 - local_8) && (*pcVar5 != '\0'));
        pcVar5 = pcVar5 + 1) {
      if (*pcVar5 == ',') goto LAB_006105d9;
    }
    while ((pcVar5 = pcVar7, pcVar5 < pcVar6 + (uVar2 - local_8) && (*pcVar5 != '\0'))) {
      if (*pcVar5 == ';') goto LAB_006105d9;
      pcVar7 = pcVar5 + 1;
    }
  }
LAB_006105c6:
  FUN_0060e130();
  return;
LAB_006105d9:
  if (pcVar6 < pcVar5) {
    cVar1 = *pcVar6;
    pcVar7 = pcVar6;
    while (cVar1 == ':') {
      sVar4 = _strspn(pcVar7 + 1,"0123456789.-");
      local_10 = local_10 + 1;
      pcVar7 = pcVar7 + 1 + sVar4;
      cVar1 = *pcVar7;
    }
    if (pcVar7 != pcVar5) goto LAB_006105c6;
  }
  pcVar6 = pcVar6 + 1;
  pcVar5 = pcVar5 + 1;
  pcVar7 = pcVar6 + (uVar2 - local_8) + (-1 - (int)pcVar5);
  switch(local_4[0]) {
  case 1:
    if (local_10 != 0) {
      FUN_00610020(param_1,local_c,param_2,pcVar6,pcVar5,pcVar7);
      return;
    }
    break;
  default:
    if ((param_2 & 1) == 0) {
      return;
    }
    FUN_0060ecf0(param_1,0);
    return;
  case 0x97:
    if (4 < local_10) {
      FUN_0060e920(param_1,local_c,param_2,pcVar6,pcVar5,pcVar7);
      return;
    }
    break;
  case 0x9a:
    FUN_00610110(param_1,local_c,param_2,pcVar6,pcVar5,pcVar7);
    return;
  case 0x9c:
    FUN_0060f7a0(param_1,local_c,param_2,pcVar6,pcVar5,pcVar7);
    return;
  }
  goto LAB_006105c6;
}
```

