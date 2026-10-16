// Function: FUN_0041b0d0
// Entry Point: 0x0041b0d0
// Program: IDMan.exe


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


