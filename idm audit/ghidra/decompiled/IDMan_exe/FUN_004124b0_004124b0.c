// Function: FUN_004124b0
// Entry Point: 0x004124b0
// Program: IDMan.exe


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


