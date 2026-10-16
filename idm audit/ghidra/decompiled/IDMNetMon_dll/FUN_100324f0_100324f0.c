// Function: FUN_100324f0
// Entry Point: 0x100324f0
// Program: IDMNetMon.dll


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


