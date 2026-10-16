// Function: FUN_0041cac0
// Entry Point: 0x0041cac0
// Program: IDMan.exe


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
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMGetAll.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"idmmkb.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMan.exe",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDManTypeInfo.tlb",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"Uninstall.exe",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IEExt.htm",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IEGetAll.htm",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"idmmzcc3.xpi",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMShellExt.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMNetMon.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  if (DAT_00788ce0 == 0) {
    puVar2 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar2 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar2,L"IDMNetMon64.dll",0);
    puVar3 = local_21c;
    if (local_21c == (undefined2 *)0x0) {
      puVar3 = local_218;
    }
    FUN_0041bd70(puVar3);
    puVar2 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar2 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar2,L"IDMShellExt64.dll",0);
    puVar3 = local_21c;
    if (local_21c == (undefined2 *)0x0) {
      puVar3 = local_218;
    }
    FUN_0041bd70(puVar3);
    puVar2 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar2 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar2,L"idmbrbtn64.dll",0);
    puVar3 = local_21c;
    if (local_21c == (undefined2 *)0x0) {
      puVar3 = local_218;
    }
    FUN_0041bd70(puVar3);
    puVar2 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar2 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar2,L"IDMVMPrs64.dll",0);
    puVar3 = local_21c;
    if (local_21c == (undefined2 *)0x0) {
      puVar3 = local_218;
    }
    FUN_0041bd70(puVar3);
    puVar2 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar2 = &DAT_00788d84;
    }
    pwVar4 = L"IDMIntegrator64.exe";
  }
  else {
    puVar2 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar2 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar2,L"IDMNetMonAA.dll",0);
    puVar3 = local_21c;
    if (local_21c == (undefined2 *)0x0) {
      puVar3 = local_218;
    }
    FUN_0041bd70(puVar3);
    puVar2 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar2 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar2,L"IDMShellExtAA.dll",0);
    puVar3 = local_21c;
    if (local_21c == (undefined2 *)0x0) {
      puVar3 = local_218;
    }
    FUN_0041bd70(puVar3);
    puVar2 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar2 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar2,L"IDMVMPrsAA.dll",0);
    puVar3 = local_21c;
    if (local_21c == (undefined2 *)0x0) {
      puVar3 = local_218;
    }
    FUN_0041bd70(puVar3);
    puVar2 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar2 = &DAT_00788d84;
    }
    FUN_00402310(&DAT_00696e40,puVar2,L"idmbrbtnAA.dll",0);
    puVar3 = local_21c;
    if (local_21c == (undefined2 *)0x0) {
      puVar3 = local_218;
    }
    FUN_0041bd70(puVar3);
    puVar2 = DAT_00788d80;
    if (DAT_00788d80 == (undefined *)0x0) {
      puVar2 = &DAT_00788d84;
    }
    pwVar4 = L"IDMIntegratorAA.exe";
  }
  FUN_00402310(&DAT_00696e40,puVar2,pwVar4,0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"idmbrbtn.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMGrHlp.exe",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"idmfsa.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IEMonitor.exe",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"idmftype.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMGetAll64.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"downlWithIDM64.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMFType.dat",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMFType64.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMGCExt.crx",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"idmvs.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"idmBroker.exe",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"MediumILStart.exe",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"idmindex.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"idmvconv.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMMsgHost.exe",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMMsgHost.json",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMMsgHostMoz.json",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMOpExt.nex",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMGCExt59.crx",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"idmfc.dat",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMVMPrs.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"libcrypto.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"libssl.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"IDMEdgeExt.crx",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  puVar2 = DAT_00788d80;
  if (DAT_00788d80 == (undefined *)0x0) {
    puVar2 = &DAT_00788d84;
  }
  FUN_00402310(&DAT_00696e40,puVar2,L"oldjsproxy.dll",0);
  puVar3 = local_21c;
  if (local_21c == (undefined2 *)0x0) {
    puVar3 = local_218;
  }
  FUN_0041bd70(puVar3);
  local_22c = local_22c & 0xffffff00;
  FUN_004011f0();
  local_22c = 0xffffffff;
  ExceptionList = local_234;
  __security_check_cookie(local_8 ^ (uint)&local_228);
  return;
}


