// Function: FUN_10007170
// Entry Point: 0x10007170
// Program: idmcchandler7.dll


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
  local_4f4 = this;
  local_8 = uStack_530;
  (*DAT_1005abc0)((short)(void *)((int)this + 0x48));
  local_4b0._0_1_ = 4;
  (*DAT_1005abc0)((int)this + 0x58);
  local_4b0._0_1_ = 5;
  (*DAT_1005abc0)((int)this + 0x68);
  local_4b0._0_1_ = 6;
  (*DAT_1005abc0)((int)this + 0x78);
  local_4b0._0_1_ = 7;
  (*DAT_1005abc0)((int)this + 0x88);
  local_4b0._0_1_ = 8;
  (*DAT_1005abc0)((int)this + 0x98);
  local_4b0._0_1_ = 9;
  (*DAT_1005abc0)((int)this + 0xa8);
  local_4b0._0_1_ = 10;
  (*DAT_1005abc0)((int)this + 0xb8);
  local_4b0._0_1_ = 0xb;
  (*DAT_1005abc0)((int)this + 200);
  local_4b0._0_1_ = 0xc;
  (*DAT_1005abc0)((int)this + 0xd8);
  local_4b0._0_1_ = 0xd;
  (*DAT_1005abc0)((int)this + 0xe8);
  local_4b0._0_1_ = 0xe;
  (*DAT_1005abc0)((int)this + 0xf8);
  local_4b0._0_1_ = 0xf;
  (*DAT_1005abc0)((int)this + 0x108);
  local_4b0._0_1_ = 0x10;
  (*DAT_1005abc0)((int)this + 0x118);
  local_4b0._0_1_ = 0x11;
  (*DAT_1005abc0)((int)this + 0x128);
  local_4b0._0_1_ = 0x12;
  (*DAT_1005abc0)((int)this + 0x138);
  local_4b0._0_1_ = 0x13;
  (*DAT_1005abc0)((short)((int)this + 0x148));
  local_4b0._0_1_ = 0x14;
  (*DAT_1005abc0)((int)this + 0x158);
  local_4b0._0_1_ = 0x15;
  (*DAT_1005abc0)((int)this + 0x168);
  local_4b0._0_1_ = 0x16;
  (*DAT_1005abc0)((int)this + 0x17c);
  local_4b0._0_1_ = 0x17;
  (*DAT_1005abe4)((int)this + 0x18c);
  local_4b0._0_1_ = 0x18;
  (*DAT_1005abe4)((int)this + 0x19c);
  local_4e4 = (uint *)((int)this + 0x2d8);
  *local_4e4 = 0;
  local_4b0 = CONCAT31(local_4b0._1_3_,0x1c);
  *(undefined4 *)((int)this + 0x2d0) = 0;
  *(undefined4 *)((int)this + 0x2e8) = 0;
  *(undefined4 *)((int)this + 4) = param_1;
  *(bool *)((int)this + 0x3d) = param_2 != 0;
  *(undefined4 *)((int)this + 0x2dc) = 0;
  *(undefined4 *)((int)this + 8) = 0;
  *(undefined4 *)((int)this + 0x2d4) = 0;
  *(undefined4 *)((int)this + 0x2c0) = 0;
  *(undefined1 *)((int)this + 0x38) = 0;
  *(undefined1 *)((int)this + 0x39) = 0;
  *(undefined1 *)((int)this + 0x3b) = 0;
  *(undefined4 *)((int)this + 0xc) = 0;
  FUN_10003b40(local_4e4,param_3);
  *(undefined1 *)((int)this + 0x3c) = 1;
  *(undefined1 *)((int)this + 0x3e) = 0;
  *(undefined1 *)((int)this + 0x3f) = 0;
  *(undefined1 *)((int)this + 0x40) = 0;
  *(undefined4 *)((int)this + 0x2ec) = 0;
  *(undefined4 *)((int)this + 0x2f0) = 0;
  *(undefined4 *)((int)this + 700) = 0;
  *(undefined4 *)((int)this + 0x2f4) = 0;
  *(undefined4 *)((int)this + 0x2e0) = 0;
  *(undefined4 *)((int)this + 0x2e4) = 0;
  FUN_100038f0((void *)((int)this + 0x48),L"Download selected files");
  uVar10 = extraout_ECX;
  uVar11 = extraout_EDX;
  if ((*(int *)((int)this + 4) == 0) || (*(int *)((int)this + 0x2d8) == 0)) goto LAB_100073f9;
  if (DAT_1005ac38 != (FARPROC)0x0) goto LAB_1000757e;
  if (DAT_1005ac28 == (HMODULE)0x0) {
    local_500 = 1;
    local_4ac.dwOSVersionInfoSize = 0x94;
    BVar4 = GetVersionExA(&local_4ac);
    if ((BVar4 == 0) || (1 < local_4ac.dwPlatformId)) {
      local_4c8 = (int *)0x208;
      LVar5 = RegQueryValueExW(DAT_1005ac60,L"ExePath",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)local_210,
                               (LPDWORD)&local_4c8);
      uVar10 = extraout_ECX_03;
      uVar11 = extraout_EDX_03;
      if ((LVar5 == 0) &&
         (pwVar7 = wcsrchr(local_210,L'\\'), uVar10 = extraout_ECX_04, uVar11 = extraout_EDX_04,
         pwVar7 != (wchar_t *)0x0)) {
        *pwVar7 = L'\0';
        _snwprintf(local_418,0x104,L"%s\\idmbrbtn.dll",(short)local_210);
        DAT_1005ac28 = LoadLibraryW(local_418);
        uVar10 = extraout_ECX_05;
        uVar11 = extraout_EDX_05;
      }
    }
    else {
      local_500 = 0;
      local_4c4 = (int *)0x104;
      LVar5 = RegQueryValueExA(DAT_1005ac60,"ExePath",(LPDWORD)0x0,(LPDWORD)0x0,local_314,
                               (LPDWORD)&local_4c4);
      uVar10 = extraout_ECX_00;
      uVar11 = extraout_EDX_00;
      if ((LVar5 == 0) &&
         (pcVar6 = strrchr((char *)local_314,0x5c), uVar10 = extraout_ECX_01,
         uVar11 = extraout_EDX_01, pcVar6 != (char *)0x0)) {
        *pcVar6 = '\0';
        _snprintf(local_10c,0x104,"%s\\idmbrbtn.dll",(short)local_314);
        DAT_1005ac28 = LoadLibraryA(local_10c);
        uVar10 = extraout_ECX_02;
        uVar11 = extraout_EDX_02;
      }
    }
    if (DAT_1005ac28 != (HMODULE)0x0) goto LAB_10007560;
  }
  else {
LAB_10007560:
    DAT_1005ac38 = GetProcAddress(DAT_1005ac28,"CreateDownlSelWtIDMButton");
    uVar10 = extraout_ECX_06;
    uVar11 = extraout_EDX_06;
  }
  if (DAT_1005ac38 != (FARPROC)0x0) {
LAB_1000757e:
    FUN_10018fa0(&local_4cc);
    local_4b0._0_1_ = 0x1d;
    FUN_10018fa0(&local_4d8);
    local_4b0._0_1_ = 0x1e;
    local_4d0 = (int ***)&local_4d8;
    piVar8 = FUN_1001c6e0(&local_4d0);
    local_4c0 = (*DAT_1005abb4)((short)piVar8);
    if (-1 < (int)local_4c0) {
      if ((int ****)local_4d8 != (int ****)0x0) {
        local_4e8 = local_4d8;
        local_4e4 = &local_4c0;
        FUN_10005b20(&local_4c8,&local_4e8);
        local_4b0._0_1_ = 0x1f;
        if (((local_4c0 & 0x80000000) == 0) && (local_4c8 != (int *)0x0)) {
          local_4d0 = local_4d8;
          pppiVar2 = (int ***)*local_4d8;
          local_4d4 = piVar1;
          piVar8 = FUN_1001c6e0(&local_4d4);
          local_4c0 = (*(code *)pppiVar2[4])
                                (local_4d0,"@idm/mozilla_cc_helper5;1",&DAT_1004ca70,(short)piVar8);
        }
        local_4b0._0_1_ = 0x1e;
        FUN_10017f20((int *)&local_4c8);
      }
      if ((-1 < (int)local_4c0) && (local_4c4 = (int *)*piVar1, local_4c4 != (int *)0x0)) {
        local_4e0 = &local_4cc;
        local_4d4 = *(int **)((int)this + 0x2d8);
        iVar9 = *local_4c4;
        piVar8 = FUN_1001c6e0(&local_4e0);
        local_4c0 = (**(code **)(iVar9 + 0x38))(local_4c4,local_4d4,(short)piVar8);
        if ((-1 < (int)local_4c0) && (local_4cc != (int **)0x0)) {
          local_4e4 = operator_new(0xc);
          local_4b0._0_1_ = 0x20;
          if (local_4e4 == (uint *)0x0) {
            iVar9 = 0;
          }
          else {
            iVar9 = FUN_100057c0(local_4e4,this);
          }
          local_4b0._0_1_ = 0x1e;
          *(int *)((int)this + 0x2e8) = iVar9;
          if (iVar9 != 0) {
            FUN_10018fa0(&local_4c4);
            local_4b0._0_1_ = 0x21;
            local_4dc = &local_4c4;
            puVar3 = (undefined4 *)**(undefined4 **)((int)this + 0x2e8);
            piVar8 = FUN_1001c6e0(&local_4dc);
            (*(code *)*puVar3)(*(undefined4 *)((int)this + 0x2e8),&DAT_1004ca50,(short)piVar8);
            if ((local_4c4 != (int *)0x0) &&
               (local_4c0 = (**(code **)(*(int *)*piVar1 + 0x110))
                                      ((int *)*piVar1,local_4cc,(short)local_4c4),
               -1 < (int)local_4c0)) {
              FUN_100055d0(local_520,L"unload",0xffffffff);
              local_4b0._0_1_ = 0x22;
              local_4c0 = (**(code **)(*(int *)*piVar1 + 0x118))((int *)*piVar1,local_520,1);
              if (-1 < (int)local_4c0) {
                *(undefined1 *)((int)this + 0x38) = 1;
              }
              FUN_100055d0(local_510,L"scroll",0xffffffff);
              local_4b0._0_1_ = 0x23;
              local_4c0 = (**(code **)(*(int *)*piVar1 + 0x118))((int *)*piVar1,local_510,0);
              FUN_100055d0(local_4f0,L"resize",0xffffffff);
              local_4b0._0_1_ = 0x24;
              local_4c0 = (**(code **)(*(int *)*piVar1 + 0x118))((int *)*piVar1,local_4f0,0);
              local_4b0._0_1_ = 0x23;
              (*DAT_1005abc8)(local_4f0);
              local_4b0._0_1_ = 0x22;
              (*DAT_1005abc8)(local_510);
              local_4b0._0_1_ = 0x21;
              (*DAT_1005abc8)(local_520);
            }
            local_4b0._0_1_ = 0x1e;
            FUN_10017f20((int *)&local_4c4);
          }
        }
      }
    }
    lpRect = (RECT *)((int)this + 0x20);
    GetClientRect(*(HWND *)((int)this + 4),lpRect);
    if ((*(char *)((int)this + 0x3d) != '\0') && (*piVar1 != 0)) {
      FUN_10018fa0(&local_4c8);
      local_4b0._0_1_ = 0x25;
      if (local_4cc != (int **)0x0) {
        local_4c4 = (int *)*piVar1;
        local_4f8 = &local_4c8;
        local_4dc = local_4cc;
        local_4e0 = (int ***)(*local_4c4 + 0x6c);
        piVar8 = FUN_1001c6e0(&local_4f8);
        (*(code *)*local_4e0)(local_4c4,local_4dc,(short)piVar8);
      }
      iVar9 = FUN_100211b0((int *)*piVar1,local_4c8,&local_4d4,&local_4fc,&local_4d0,&local_4e4);
      if (iVar9 != 0) {
        lpRect->left = (LONG)local_4d4;
        *(int *)((int)this + 0x28) = (int)local_4d4 + (int)local_4d0;
        *(int *)((int)this + 0x24) = local_4fc;
        *(int *)((int)this + 0x2c) = local_4fc + (int)local_4e4;
      }
      local_4b0._0_1_ = 0x1e;
      FUN_10017f20((int *)&local_4c8);
    }
    lpRect->left = lpRect->left + 2;
    *(int *)((int)this + 0x24) = *(int *)((int)this + 0x24) + 2;
    iVar9 = GetSystemMetrics(2);
    *(int *)((int)this + 0x28) = *(int *)((int)this + 0x28) + (-2 - iVar9);
    iVar9 = GetSystemMetrics(3);
    *(int *)((int)this + 0x2c) = *(int *)((int)this + 0x2c) + (-2 - iVar9);
    ScreenToClient(*(HWND *)((int)this + 4),(LPPOINT)&param_4);
    pt.y._0_2_ = (short)param_5;
    pt.x = param_4;
    pt.y._2_2_ = (short)((uint)param_5 >> 0x10);
    BVar4 = PtInRect(lpRect,pt);
    if (BVar4 == 0) {
      pt_00.y._0_2_ = (short)param_7;
      pt_00.x = param_6;
      pt_00.y._2_2_ = (short)((uint)param_7 >> 0x10);
      BVar4 = PtInRect(lpRect,pt_00);
      if (BVar4 == 0) {
        *(LONG *)((int)this + 0x10) = lpRect->left;
        *(undefined4 *)((int)this + 0x14) = *(undefined4 *)((int)this + 0x24);
        *(undefined4 *)((int)this + 0x18) = *(undefined4 *)((int)this + 0x28);
        iVar9 = *(int *)((int)this + 0x2c);
      }
      else {
        ScreenToClient(*(HWND *)((int)this + 4),(LPPOINT)&param_6);
        *(int *)((int)this + 0x10) = param_6 + -1;
        *(int *)((int)this + 0x18) = param_6 + 1;
        *(int *)((int)this + 0x14) = param_7 + -1;
        iVar9 = param_7 + 1;
      }
    }
    else {
      *(int *)((int)this + 0x10) = param_4 + -1;
      *(int *)((int)this + 0x18) = param_4 + 1;
      *(int *)((int)this + 0x14) = param_5 + -1;
      iVar9 = param_5 + 1;
    }
    *(int *)((int)this + 0x1c) = iVar9;
    iVar9 = (*DAT_1005ac38)(*(undefined4 *)((int)this + 4),*(undefined4 *)((int)this + 0x10),
                            *(undefined4 *)((int)this + 0x14),*(undefined4 *)((int)this + 0x18),
                            *(undefined4 *)((int)this + 0x1c),lpRect->left,
                            *(undefined4 *)((int)this + 0x24),*(undefined4 *)((int)this + 0x28),
                            *(undefined4 *)((int)this + 0x2c),this,0x10);
    *(int *)((int)this + 8) = iVar9;
    if (iVar9 != 0) {
      *(undefined1 *)((int)this + 0x3f) = 1;
    }
    local_4b0._0_1_ = 0x1d;
    FUN_10017f20((int *)&local_4d8);
    local_4b0 = CONCAT31(local_4b0._1_3_,0x1c);
    FUN_10017f20((int *)&local_4cc);
    local_4b0 = 0x1b;
    FUN_100079c2(extraout_ECX_07,extraout_EDX_07);
    return;
  }
LAB_100073f9:
  local_4b0 = CONCAT31(local_4b0._1_3_,0x1b);
  FUN_100079c2(uVar10,uVar11);
  return;
}


