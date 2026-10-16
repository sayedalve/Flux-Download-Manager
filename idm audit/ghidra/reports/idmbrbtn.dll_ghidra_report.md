# Ghidra Analysis Report: idmbrbtn.dll

- **Program**: `idmbrbtn.dll`
- **Executable Path**: `/C:/Users/sayed/Downloads/IDM_RE_Investigation/binaries/idmbrbtn.dll`
- **Language**: `x86:LE:32:default`
- **Compiler**: `windows`
- **Image Base**: `10000000`

## Targeted String Occurrences and Cross-References

### Pattern: `hlstoidm` (1 occurrence(s))

- **Found at Address**: `0x1000bd3c`
  - **Direct References** (2):
    - from `0x10003cd2` (Type: DATA) in Function `[FUN_10003c30](file:///0x10003c30)`
    - from `0x10003d98` (Type: DATA) in Function `[FUN_10003c30](file:///0x10003c30)`

### Pattern: `hls2toidm` (1 occurrence(s))

- **Found at Address**: `0x1000bd24`
  - **Direct References** (2):
    - from `0x10003cb8` (Type: DATA) in Function `[FUN_10003c30](file:///0x10003c30)`
    - from `0x10003d7a` (Type: DATA) in Function `[FUN_10003c30](file:///0x10003c30)`

### Pattern: `idmbrbtn` (2 occurrence(s))

- **Found at Address**: `0x1000d68b`
  - **Direct References**: None found.
  - **Indirect References**: None found in data sections.
- **Found at Address**: `0x1000da26`
  - **Direct References** (1):
    - from `0x1000d99c` (Type: DATA) (No enclosing function)

### Pattern: `CreateIDMButton` (3 occurrence(s))

- **Found at Address**: `0x1000da4d`
  - **Direct References** (1):
    - from `0x1000d9ec` (Type: DATA) (No enclosing function)
- **Found at Address**: `0x1000da5e`
  - **Direct References** (1):
    - from `0x1000d9f0` (Type: DATA) (No enclosing function)
- **Found at Address**: `0x1000da6f`
  - **Direct References** (1):
    - from `0x1000d9e8` (Type: DATA) (No enclosing function)

### Pattern: `CreateIDMButton2` (1 occurrence(s))

- **Found at Address**: `0x1000da4d`
  - **Direct References** (1):
    - from `0x1000d9ec` (Type: DATA) (No enclosing function)

### Pattern: `CreateIDMButton3` (1 occurrence(s))

- **Found at Address**: `0x1000da5e`
  - **Direct References** (1):
    - from `0x1000d9f0` (Type: DATA) (No enclosing function)

### Pattern: `SetIDMButtonValue3` (1 occurrence(s))

- **Found at Address**: `0x1000da8e`
  - **Direct References** (1):
    - from `0x1000d9f8` (Type: DATA) (No enclosing function)

### Pattern: `CreateDownlSelWtIDMButton` (1 occurrence(s))

- **Found at Address**: `0x1000da33`
  - **Direct References** (1):
    - from `0x1000d9e4` (Type: DATA) (No enclosing function)

## Decompiled Key Functions

Total unique functions identified for decompilation: 1

### Function `[FUN_10003c30](file:///C:/Users/sayed/Downloads/IDM_RE_Investigation/ghidra/decompiled/idmbrbtn_dll/FUN_10003c30_10003c30.c)`

- **Entry Point**: `0x10003c30`
- **Signature**: `uint __thiscall FUN_10003c30(void * this, undefined4 * param_1, char param_2)`
- **Callers** (2): `FUN_100036a0` (0x100036a0), `FUN_100045e0` (0x100045e0)
- **Callees** (10): `FUN_10009090` (0x10009090), `memmove` (0x1000ad52), `FUN_10005a80` (0x10005a80), `FUN_10009fd0` (0x10009fd0), `FUN_100073f0` (0x100073f0), `SetWindowPos` (0xEXTERNAL:00000077), `wcsncmp` (0xEXTERNAL:00000038), `FUN_10008f50` (0x10008f50), `memset` (0x1000ad46), `FUN_10005b60` (0x10005b60)... [0 more]

```c

uint __thiscall FUN_10003c30(void *this,undefined4 *param_1,char param_2)

{
  short *psVar1;
  bool bVar2;
  char cVar3;
  short *in_EAX;
  int iVar4;
  int iVar5;
  int iVar6;
  uint3 uVar7;
  UINT uFlags;
  void *_Dst;
  uint uVar8;
  undefined4 *puVar9;
  bool bVar10;
  byte local_98;
  undefined4 local_94 [37];
  
  local_98 = 0;
  if ((((param_1 == (undefined4 *)0x0) || (*(char *)this != '\0')) ||
      (in_EAX = (short *)*param_1, in_EAX == (short *)0x0)) || (*in_EAX == 0)) goto LAB_10003f71;
  if ((*(char *)((int)this + 3) == '\0') ||
     (in_EAX = (short *)FUN_10009fd0((wchar_t *)param_1[9],L"TS|MP4|M4V|M4A|AAC|MP3|AC3|EAC3"),
     (char)in_EAX == '\0')) goto LAB_10003e07;
  iVar4 = wcsncmp((wchar_t *)*param_1,L"http:",5);
  if ((iVar4 == 0) || (iVar4 = wcsncmp((wchar_t *)*param_1,L"https:",6), iVar4 == 0)) {
    in_EAX = (short *)0x0;
    bVar2 = true;
    bVar10 = false;
LAB_10003cfe:
    cVar3 = '\0';
  }
  else {
    iVar4 = wcsncmp((wchar_t *)*param_1,L"hls2toidm:",10);
    bVar10 = iVar4 == 0;
    iVar4 = wcsncmp((wchar_t *)*param_1,L"hlstoidm:",9);
    in_EAX = (short *)CONCAT31((int3)((uint)iVar4 >> 8),bVar10);
    if (iVar4 != 0) {
      bVar2 = false;
      goto LAB_10003cfe;
    }
    cVar3 = '\x01';
    bVar2 = false;
  }
  if (((char)in_EAX == '\0') ||
     (in_EAX = (short *)CONCAT31((int3)((uint)in_EAX >> 8),2), DAT_1000f715 != '\0')) {
    in_EAX = (short *)CONCAT31((int3)((uint)in_EAX >> 8),cVar3);
  }
  if (((!bVar2) || (*(char *)((int)this + 0x660) == '\0')) &&
     ((cVar3 == '\0' || (*(char *)((int)this + 0x660) < '\x02')))) {
    if ((!bVar2) && (*(char *)((int)this + 0x660) < (char)in_EAX)) {
      uVar8 = 0;
      *(char *)((int)this + 0x660) = (char)in_EAX;
      if (*(int *)((int)this + 0x6f4) != 0) {
        iVar4 = 0;
        do {
          iVar6 = *(int *)((int)this + 0x6fc);
          in_EAX = (short *)FUN_10009fd0(*(wchar_t **)(iVar6 + 0x2c + iVar4),
                                         L"TS|MP4|M4V|M4A|AAC|MP3|AC3|EAC3");
          if ((char)in_EAX != '\0') {
            iVar5 = wcsncmp(*(wchar_t **)(iVar6 + iVar4),L"hls2toidm:",10);
            in_EAX = (short *)0x0;
            if (iVar5 != 0) {
              if (!bVar10) {
                iVar6 = wcsncmp(*(wchar_t **)(iVar6 + iVar4),L"hlstoidm:",9);
                in_EAX = (short *)0x0;
                if (iVar6 == 0) goto LAB_10003ded;
              }
              FUN_10008f50((void *)((int)this + 0x6f0),uVar8);
              *(int *)((int)this + 0x6f4) = *(int *)((int)this + 0x6f4) + -1;
              _Dst = (void *)(*(int *)((int)this + 0x6fc) + iVar4);
              in_EAX = memmove(_Dst,(void *)((int)_Dst + 400),
                               (*(int *)((int)this + 0x6f4) - uVar8) * 400);
              uVar8 = uVar8 - 1;
              iVar4 = iVar4 + -400;
            }
          }
LAB_10003ded:
// ... [Truncated in summary - Full code in FUN_10003c30_10003c30.c (145 lines)]
```

