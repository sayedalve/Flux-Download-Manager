// Function: FUN_0045f0c0
// Entry Point: 0x0045f0c0
// Program: IDMan.exe


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
  FUN_00402310(&DAT_00696e40,puVar1,L"IDMMsgHost.exe",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"idmindex.dll",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"IDMGrHlp.exe",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"libcrypto.dll",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"IDMShellExt.dll",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"IDMVMPrs.dll",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"idmbrbtn.dll",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"idmvs.dll",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"idmvconv.dll",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"idmnmcl.dll",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"IDMGCExt.crx",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  puVar1 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar1 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar1,L"IDMNetMon.dll",0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
  if (DAT_00788ce0 == 0) {
    iVar2 = FUN_00447030();
    if (iVar2 == 0) goto LAB_0045f66a;
    puVar1 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar1 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar1,L"IDMShellExt64.dll",0);
    pWVar3 = local_1c;
    if (local_1c == (LPCWSTR)0x0) {
      pWVar3 = local_18;
    }
    GetFileAttributesW(pWVar3);
    puVar1 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar1 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar1,L"IDMNetMon64.dll",0);
    pWVar3 = local_1c;
    if (local_1c == (LPCWSTR)0x0) {
      pWVar3 = local_18;
    }
    GetFileAttributesW(pWVar3);
    puVar1 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar1 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar1,L"IDMVMPrs64.dll",0);
    pWVar3 = local_1c;
    if (local_1c == (LPCWSTR)0x0) {
      pWVar3 = local_18;
    }
    GetFileAttributesW(pWVar3);
    puVar1 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar1 = &DAT_00788d84;
    }
    pwVar4 = L"idmbrbtn64.dll";
  }
  else {
    puVar1 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar1 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar1,L"IDMShellExtAA.dll",0);
    pWVar3 = local_1c;
    if (local_1c == (LPCWSTR)0x0) {
      pWVar3 = local_18;
    }
    GetFileAttributesW(pWVar3);
    puVar1 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar1 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar1,L"IDMNetMonAA.dll",0);
    pWVar3 = local_1c;
    if (local_1c == (LPCWSTR)0x0) {
      pWVar3 = local_18;
    }
    GetFileAttributesW(pWVar3);
    puVar1 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar1 = &DAT_00788d84;
    }
    pwVar4 = L"idmbrbtnAA.dll";
  }
  FUN_00402310(&DAT_00696e40,puVar1,pwVar4,0);
  pWVar3 = local_1c;
  if (local_1c == (LPCWSTR)0x0) {
    pWVar3 = local_18;
  }
  GetFileAttributesW(pWVar3);
LAB_0045f66a:
  local_4 = 0xffffffff;
  FUN_004011f0();
  ExceptionList = local_c;
  __security_check_cookie(local_10 ^ (uint)&local_1c);
  return;
}


