// Function: FUN_00412e50
// Entry Point: 0x00412e50
// Program: IDMan.exe


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
  pcVar9 = RegCloseKey_exref;
  if (LVar5 != 0) {
    if (param_1 == 0) {
      LVar5 = RegOpenKeyExA((HKEY)&DAT_80000001,"SOFTWARE\\Mozilla",0,samDesired,&local_438);
      pcVar9 = RegCloseKey_exref;
      if (LVar5 != 0) goto LAB_00413131;
      RegCloseKey(local_438);
      pcVar8 = "SOFTWARE\\Mozilla\\NativeMessagingHosts";
      pHVar4 = (HKEY)&DAT_80000001;
    }
    else {
      pHVar4 = (HKEY)((param_3 != 0) + 0x80000001);
      LVar5 = RegCreateKeyExA(pHVar4,"Software\\Google",0,"",0,samDesired,(LPSECURITY_ATTRIBUTES)0x0
                              ,&local_438,(LPDWORD)0x0);
      pcVar9 = RegCloseKey_exref;
      if (LVar5 != 0) goto LAB_00413131;
      RegCloseKey(local_438);
      LVar5 = RegCreateKeyExA(pHVar4,"Software\\Google\\Chrome",0,"",0,samDesired,
                              (LPSECURITY_ATTRIBUTES)0x0,&local_438,(LPDWORD)0x0);
      if (LVar5 != 0) goto LAB_00413131;
      RegCloseKey(local_438);
      pcVar8 = "SOFTWARE\\Google\\Chrome\\NativeMessagingHosts";
    }
    LVar5 = RegCreateKeyExA(pHVar4,pcVar8,0,"",0,samDesired,(LPSECURITY_ATTRIBUTES)0x0,&local_438,
                            (LPDWORD)0x0);
    if (LVar5 != 0) goto LAB_00413131;
  }
  LVar5 = RegCreateKeyExA(local_438,"com.tonec.idm",0,"",0,samDesired,(LPSECURITY_ATTRIBUTES)0x0,
                          &local_434,(LPDWORD)0x0);
  if (LVar5 == 0) {
    pwVar7 = local_42c;
    if (local_42c == (wchar_t *)0x0) {
      pwVar7 = local_428;
    }
    pwVar1 = pwVar7 + 1;
    do {
      wVar2 = *pwVar7;
      pwVar7 = pwVar7 + 1;
    } while (wVar2 != L'\0');
    lpData = local_42c;
    if (local_42c == (wchar_t *)0x0) {
      lpData = local_428;
    }
    RegSetValueExW(local_434,L"",0,1,(BYTE *)lpData,((int)pwVar7 - (int)pwVar1 >> 1) * 2 + 2);
    (*pcVar9)(local_434);
  }
  (*pcVar9)(local_438);
LAB_00413131:
  local_4 = 0xffffffff;
  FUN_004011f0();
  ExceptionList = local_c;
  __security_check_cookie(local_10 ^ (uint)&local_438);
  return;
}


