// Function: FUN_004cb100
// Entry Point: 0x004cb100
// Program: IDMan.exe


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
        }
        iVar7 = iVar7 + -1;
      } while (iVar7 != 0);
    }
    piVar4 = piVar4 + 1;
    iVar1 = iVar1 + 1;
    local_258 = local_258 + -1;
  } while (local_258 != 0);
  LVar3 = RegCreateKeyExA((HKEY)&DAT_80000001,"Software\\DownloadManager\\ListSettings",0,"",0,
                          0xf003f,(LPSECURITY_ATTRIBUTES)0x0,&local_25c,local_224);
  if (LVar3 == 0) {
    local_268 = 8;
    LVar3 = RegQueryValueExA(local_25c,"FileName",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_264,
                             &local_268);
    if (LVar3 == 0) {
      param_1[0x48] = local_264;
      param_1[0x49] = local_260;
    }
    local_268 = 8;
    LVar3 = RegQueryValueExA(local_25c,"DateAdded",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_264,
                             &local_268);
    if (LVar3 == 0) {
      param_1[0x4a] = local_264;
      param_1[0x4b] = local_260;
    }
    local_268 = 8;
    LVar3 = RegQueryValueExA(local_25c,"Queue",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_264,
                             &local_268);
    if (LVar3 == 0) {
      param_1[0x4c] = local_264;
      param_1[0x4d] = local_260;
    }
    local_268 = 8;
    LVar3 = RegQueryValueExA(local_25c,"Size",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_264,
                             &local_268);
    if (LVar3 == 0) {
      param_1[0x4e] = local_264;
      param_1[0x4f] = local_260;
    }
    local_268 = 8;
    LVar3 = RegQueryValueExA(local_25c,"Status",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_264,
                             &local_268);
    if (LVar3 == 0) {
      param_1[0x50] = local_264;
      param_1[0x51] = local_260;
    }
    local_268 = 8;
    LVar3 = RegQueryValueExA(local_25c,"Timeleft",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_264,
                             &local_268);
    if (LVar3 == 0) {
      param_1[0x52] = local_264;
      param_1[0x53] = local_260;
    }
    local_268 = 8;
    LVar3 = RegQueryValueExA(local_25c,"TransferRate",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_264,
                             &local_268);
    if (LVar3 == 0) {
      param_1[0x54] = local_264;
      param_1[0x55] = local_260;
    }
    local_268 = 8;
    LVar3 = RegQueryValueExA(local_25c,"LastTry",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_264,
                             &local_268);
    if (LVar3 == 0) {
      param_1[0x56] = local_264;
      param_1[0x57] = local_260;
    }
    local_268 = 8;
    LVar3 = RegQueryValueExA(local_25c,"Description",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_264,
                             &local_268);
    if (LVar3 == 0) {
      param_1[0x58] = local_264;
      param_1[0x59] = local_260;
    }
    local_268 = 8;
    LVar3 = RegQueryValueExA(local_25c,"SaveTo",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_264,
                             &local_268);
    if (LVar3 == 0) {
      param_1[0x5a] = local_264;
      param_1[0x5b] = local_260;
    }
    local_268 = 8;
    LVar3 = RegQueryValueExA(local_25c,"Referer",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_264,
                             &local_268);
    if (LVar3 == 0) {
      param_1[0x5c] = local_264;
      param_1[0x5d] = local_260;
    }
    local_268 = 8;
    LVar3 = RegQueryValueExA(local_25c,"Parent_wp",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_264,
                             &local_268);
    if (LVar3 == 0) {
      param_1[0x5e] = local_264;
      param_1[0x5f] = local_260;
    }
    local_268 = 0x30;
    LVar3 = RegQueryValueExA(local_25c,"Order639",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)(param_1 + 0x60)
                             ,&local_268);
    if (LVar3 != 0) {
      local_268 = 0x2c;
      LVar3 = RegQueryValueExA(local_25c,"Order",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_254,
                               &local_268);
      if (LVar3 == 0) {
        param_1[0x60] = local_254;
        param_1[0x61] = local_250;
        param_1[0x62] = local_24c;
        param_1[99] = local_248;
        param_1[100] = local_244;
        param_1[0x65] = local_240;
        param_1[0x66] = local_23c;
        param_1[0x67] = local_238;
        param_1[0x68] = local_234;
        param_1[0x69] = local_230;
        param_1[0x6a] = local_22c;
      }
    }
    iVar7 = 0;
    piVar4 = param_1 + 0x48;
    iVar1 = 0xc;
    do {
      if (piVar4[1] == 0) {
        if (*piVar4 == 0) {
          *piVar4 = 10;
        }
      }
      else {
        iVar7 = iVar7 + *piVar4;
      }
      piVar4 = piVar4 + 2;
      iVar1 = iVar1 + -1;
    } while (iVar1 != 0);
    if (iVar7 == 0) {
      FUN_004cac30();
    }
    iVar1 = 0x60;
    hdc = GetDC((HWND)0x0);
    if (hdc != (HDC)0x0) {
      iVar1 = GetDeviceCaps(hdc,0x58);
      GetDeviceCaps(hdc,0x5a);
      ReleaseDC((HWND)0x0,hdc);
    }
    if (iVar1 != 0x60) {
      piVar4 = param_1 + 0x48;
      iVar7 = 0xc;
      do {
        iVar5 = MulDiv(*piVar4,iVar1,0x60);
        *piVar4 = iVar5;
        piVar4 = piVar4 + 2;
        iVar7 = iVar7 + -1;
      } while (iVar7 != 0);
    }
    if (DAT_00788ef4 != 0) {
      puVar8 = param_1 + 0x48;
      *(undefined8 *)(param_1 + 0x46) = _DAT_0077bd58;
      iVar1 = 0xc;
      do {
        uVar6 = FUN_0063ec80();
        *puVar8 = uVar6;
        puVar8 = puVar8 + 2;
        iVar1 = iVar1 + -1;
      } while (iVar1 != 0);
    }
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
          }
          iVar7 = iVar7 + -1;
        } while (iVar7 != 0);
      }
      piVar4 = piVar4 + 1;
      iVar1 = iVar1 + 1;
      local_258 = local_258 + -1;
    } while (local_258 != 0);
    RegCloseKey(local_25c);
  }
  else {
    FUN_004c4c30();
    local_4._0_1_ = 5;
    FUN_004c4da0("Cannot open(/create) registry listsettings subkey.\n",1);
    local_4 = CONCAT31(local_4._1_3_,4);
    FUN_004c4d90();
  }
  local_4 = 0xffffffff;
  ExceptionList = local_c;
  __security_check_cookie(local_10 ^ (uint)&local_268);
  return;
}


