// Function: FUN_005c5b20
// Entry Point: 0x005c5b20
// Program: IDMan.exe


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
  
  puStack_c0 = &LAB_00682b24;
  local_c4 = ExceptionList;
  local_1bc = DAT_007839c0 ^ (uint)&local_b8;
  local_c8 = (undefined1 *)&local_1bc;
  ExceptionList = &local_c4;
  local_e4 = param_4;
  local_bc = 0;
  puVar6 = &local_1bc;
  local_8 = local_1bc;
  if (*(int *)(param_1 + 0x958) != 0) {
    FUN_005d91c0(*(undefined4 *)(param_1 + 0x510),0,0);
    *(undefined4 *)(param_1 + 0x958) = 0;
    puVar6 = (uint *)local_c8;
  }
  local_c8 = (undefined1 *)puVar6;
  if (*(int *)(param_1 + 0xaf4) != 0) {
    *(undefined4 *)(param_1 + 0xae4) = 0x7540;
  }
  if (*(int *)(param_1 + 0xae4) == 0x7573) {
    FUN_005bdcf0(3);
    FUN_0056f990();
    goto LAB_005c63e3;
  }
  if (*(int *)(param_1 + 4) == 0) {
    FUN_005bdcf0(0);
  }
  if (*(int *)(param_1 + 0xaf4) != 0) {
    FUN_005bdcf0(0);
    FUN_005d2d20(*(undefined4 *)(param_1 + 0xae4),DAT_00789da8,*(undefined4 *)(param_1 + 0x510),1);
    FUN_0056f5a0("Cancel by user.\n");
    goto LAB_005c63e3;
  }
  iVar7 = *(int *)(param_1 + 0xae4);
  if (iVar7 == 0x7540) {
    FUN_0056f5a0("Cancel by user.\n");
    FUN_005bdcf0(2);
    goto LAB_005c63e3;
  }
  if (iVar7 == 0x758b) goto LAB_005c63e3;
  local_da = '\0';
  local_cd = '\0';
  local_dc = 1;
  if (iVar7 < 0x753c) {
    if (iVar7 != 0x753b) {
      if (iVar7 == -0x7ff6fcfc) goto switchD_005c5d05_caseD_7542;
      if ((iVar7 != 0x17) && (iVar7 != 0x70)) goto switchD_005c5d05_caseD_7543;
    }
    goto switchD_005c5d05_caseD_7558;
  }
  switch(iVar7) {
  case 0x7542:
  case 0x7565:
  case 0x7566:
  case 0x7578:
  case 0x757a:
switchD_005c5d05_caseD_7542:
    if (*(char *)(param_1 + 0xb17) != '\0') goto LAB_005c5e73;
    if (((DAT_0078990d == '\0') || (*(int *)(param_1 + 4) != 0)) ||
       ((*(uint *)(param_1 + 0x184) & 0x80000) == 0)) goto LAB_005c5e6b;
    FUN_004024f0();
    local_bc._0_1_ = 1;
    FUN_004024f0();
    local_bc = CONCAT31(local_bc._1_3_,2);
    if (*(char *)(param_1 + 0xad0) == '\0') {
      puVar25 = *(undefined1 **)(param_1 + 0x974);
      if (puVar25 == (undefined1 *)0x0) {
        puVar25 = &DAT_006979dc;
      }
      uVar22 = *(undefined4 *)(param_1 + 0x978);
    }
    else {
      uVar22 = *(undefined4 *)(param_1 + 0xab8);
      puVar25 = *(undefined1 **)(param_1 + 0xab4);
    }
    FUN_00402f20(&local_cc,"%s:%ld",puVar25,uVar22);
    *(undefined1 *)(param_1 + 0xb17) = 1;
    FUN_004b6d80(param_1);
    FUN_004ba8d0(param_1);
    if (*(char *)(param_1 + 0xad0) == '\0') {
      puVar25 = *(undefined1 **)(param_1 + 0x974);
      if (puVar25 == (undefined1 *)0x0) {
        puVar25 = &DAT_006979dc;
      }
      uVar22 = *(undefined4 *)(param_1 + 0x978);
    }
    else {
      uVar22 = *(undefined4 *)(param_1 + 0xab8);
      puVar25 = *(undefined1 **)(param_1 + 0xab4);
    }
    FUN_00402f20(&local_d8,"%s:%ld",puVar25,uVar22);
    iVar7 = __stricmp(local_d8,(char *)local_cc);
    if (iVar7 != 0) {
      FUN_0056f5a0("Try via the same proxy as in browser request.\n");
      *(undefined4 *)(param_1 + 0xb1c) = *(undefined4 *)(param_1 + 0xae4);
      if (*(int *)(param_1 + 0x544) != 0) {
        FUN_00402f20(param_1 + 0xb20,&DAT_00696f08,*(int *)(param_1 + 0x544));
      }
      if ((*(HWND *)(param_1 + 0x14) == (HWND)0x0) || (*(int *)(param_1 + 0x510) == 0)) {
        *(undefined4 *)(param_1 + 0x970) = 0;
        FUN_005cefd0(0);
      }
      else {
        PostMessageA(*(HWND *)(param_1 + 0x14),0x111,0x1431,*(int *)(param_1 + 0x510));
      }
      local_bc._0_1_ = 1;
      FUN_00402560();
      local_bc = (uint)local_bc._1_3_ << 8;
      FUN_00402560();
      goto LAB_005c63e3;
    }
    *(undefined1 *)(param_1 + 0xb17) = 0;
    if (*(char *)(param_1 + 0xb18) != '\0') {
      FUN_0056f5a0("Cannot detect proxy in browser proxy script :(\n");
      local_cd = '\x01';
    }
    local_bc._0_1_ = 1;
    FUN_00402560();
    goto LAB_005c5fea;
  default:
switchD_005c5d05_caseD_7543:
    if (*(char *)(param_1 + 0x372) == '\0') goto switchD_005c5d05_caseD_7542;
  case 0x7558:
  case 0x7560:
  case 0x7570:
  case 0x7573:
  case 0x7582:
    break;
  }
switchD_005c5d05_caseD_7558:
  local_dc = 0;
LAB_005c5e6b:
  if (*(char *)(param_1 + 0xb17) == '\0') {
    if (((DAT_0078990d == '\0') && (*(int *)(param_1 + 4) == 0)) &&
       ((*(uint *)(param_1 + 0x184) & 0x80000) != 0)) {
      FUN_004024f0();
      local_bc._0_1_ = 3;
      FUN_004024f0();
      local_bc._0_1_ = 4;
      if (*(char *)(param_1 + 0xad0) == '\0') {
        puVar25 = *(undefined1 **)(param_1 + 0x974);
        if (puVar25 == (undefined1 *)0x0) {
          puVar25 = &DAT_006979dc;
        }
        uVar22 = *(undefined4 *)(param_1 + 0x978);
      }
      else {
        uVar22 = *(undefined4 *)(param_1 + 0xab8);
        puVar25 = *(undefined1 **)(param_1 + 0xab4);
      }
      FUN_00402f20(&local_d8,"%s:%ld",puVar25,uVar22);
      *(undefined1 *)(param_1 + 0xb17) = 1;
      FUN_004b6d80(param_1);
      FUN_004ba8d0(param_1);
      if (*(char *)(param_1 + 0xad0) == '\0') {
        puVar25 = *(undefined1 **)(param_1 + 0x974);
        if (puVar25 == (undefined1 *)0x0) {
          puVar25 = &DAT_006979dc;
        }
        uVar22 = *(undefined4 *)(param_1 + 0x978);
      }
      else {
        uVar22 = *(undefined4 *)(param_1 + 0xab8);
        puVar25 = *(undefined1 **)(param_1 + 0xab4);
      }
      FUN_00402f20(&local_cc,"%s:%ld",puVar25,uVar22);
      iVar7 = __stricmp((char *)local_cc,local_d8);
      if (iVar7 != 0) {
        local_cd = '\x01';
      }
      *(undefined1 *)(param_1 + 0xb17) = 0;
      local_bc._0_1_ = 3;
      FUN_00402560();
LAB_005c5fea:
      local_bc = (uint)local_bc._1_3_ << 8;
      FUN_00402560();
    }
  }
  else {
LAB_005c5e73:
    if ((iVar7 == 0x756f) || (iVar7 == 0x7559)) {
      if (*(int *)(param_1 + 0xb1c) != 0) {
        *(int *)(param_1 + 0xae4) = *(int *)(param_1 + 0xb1c);
      }
      pcVar17 = *(char **)(param_1 + 0xb20);
      pcVar16 = pcVar17;
      do {
        cVar4 = *pcVar16;
        pcVar16 = pcVar16 + 1;
      } while (cVar4 != '\0');
      if (pcVar16 != pcVar17 + 1) {
        FUN_0056fc90(pcVar17);
        local_cd = '\x01';
        *(undefined1 *)(param_1 + 0xb17) = 0;
        goto LAB_005c5fef;
      }
      FUN_0056fa30();
      local_cd = '\x01';
    }
    *(undefined1 *)(param_1 + 0xb17) = 0;
  }
LAB_005c5fef:
  local_d9 = '\0';
  local_64 = (undefined2 *)0x0;
  local_5c = 0;
  local_60[0] = 0;
  local_bc._0_1_ = 5;
  uVar20 = *(uint *)(param_1 + 0xae4);
  uVar22 = DAT_00789b64;
  if ((int)uVar20 < 0x7533) {
    if (uVar20 == 0x7532) {
switchD_005c6259_caseD_274c:
      if (((*(uint *)(param_1 + 0x184) & 8) != 0) || ((*(uint *)(param_1 + 0x184) & 0x100) != 0)) {
        *(undefined1 *)(param_1 + 0x385) = 1;
      }
      goto switchD_005c63b6_caseD_7533;
    }
    if ((int)uVar20 < 0x271e) {
      uVar23 = DAT_00789b70;
      if (uVar20 == 0x271d) goto LAB_005c6266;
      if (uVar20 < 0x80000000) {
        uVar22 = DAT_0078a198;
        if ((uVar20 != 5) && (uVar22 = DAT_0078a608, uVar20 != 0x17)) {
          if (uVar20 == 0x70) {
            FUN_00460040(DAT_0078a080,DAT_00788e50);
            local_a0 = (undefined2 *)0x0;
            local_98 = 0;
            local_9c[0] = 0;
            local_bc._0_1_ = 0x14;
            FormatMessageW(0x1100,(LPCVOID)0x0,0x70,0x400,(LPWSTR)&local_d4,0,(va_list *)0x0);
            puVar19 = local_70;
            if (local_70 == (undefined2 *)0x0) {
              puVar19 = local_6c;
            }
            FUN_00402310("\r\n\r\n%s%s",puVar19,local_d4,0);
            LocalFree(local_d4);
            FUN_00401970(DAT_00789b64,DAT_00788e50);
            puVar19 = local_a0;
            if (local_a0 == (undefined2 *)0x0) {
              puVar19 = local_9c;
            }
            FUN_00405300(puVar19);
            local_bc._0_1_ = 0x13;
            FUN_004011f0();
            local_bc._0_1_ = 5;
            FUN_004011f0();
            goto LAB_005c6c4b;
          }
          goto switchD_005c6259_caseD_2727;
        }
      }
      else {
        if (uVar20 != 0xffffffff) {
          if (((uVar20 == 0x80090304) || (uVar20 == 0x8009030e)) || (uVar20 == 0xfffffffc))
          goto switchD_005c63b6_caseD_7539;
          goto switchD_005c6259_caseD_2727;
        }
        if ((*(int *)(param_1 + 0x44) != 4) && (*(int *)(param_1 + 0x44) != 5)) {
          FUN_00402170(param_1 + 0x548);
          local_bc._0_1_ = 0x17;
          FUN_00402170(param_1 + 0xb3c);
          local_bc._0_1_ = 0x18;
          puVar19 = local_70;
          if (local_70 == (undefined2 *)0x0) {
            puVar19 = local_6c;
          }
          puVar18 = local_7c;
          if (local_7c == (undefined2 *)0x0) {
            puVar18 = local_78;
          }
          FUN_00402310(DAT_00789b58,puVar18,puVar19,DAT_00788e50);
          local_bc._0_1_ = 0x17;
          FUN_004011f0();
          local_bc._0_1_ = 5;
          FUN_004011f0();
          goto LAB_005c6c4b;
        }
      }
      goto LAB_005c6c43;
    }
    if (0x2af9 < (int)uVar20) {
      if (uVar20 == 0x2afa) goto switchD_005c63b6_caseD_7539;
      if (uVar20 == 0x2afc) {
        FUN_004024f0();
        local_bc._0_1_ = 0xe;
        uVar22 = DAT_00789d84;
        goto LAB_005c6292;
      }
switchD_005c6259_caseD_2727:
      uVar22 = DAT_00789b64;
      if ((*(int *)(param_1 + 0x44) != 4) && (*(int *)(param_1 + 0x44) != 5)) {
        FUN_00402170(param_1 + 0x548);
        local_bc._0_1_ = 0x19;
        puVar19 = local_70;
        if (local_70 == (undefined2 *)0x0) {
          puVar19 = local_6c;
        }
        FUN_004021a0(DAT_00789b5c,puVar19,DAT_00788e50);
        local_bc._0_1_ = 5;
        FUN_004011f0();
        goto LAB_005c6c4b;
      }
      goto LAB_005c6c43;
    }
    uVar23 = DAT_00789b80;
    if (uVar20 != 0x2af9) {
      switch(uVar20) {
      case 0x2726:
        FUN_004024f0();
        local_bc._0_1_ = 0xd;
        uVar22 = DAT_00789b64;
LAB_005c6292:
        FUN_00402f20(&local_cc,"%s\r\n\r\n%s",uVar22,DAT_0078a294);
        FUN_00401970(local_cc,DAT_00788e50);
        *(undefined1 *)(param_1 + 0x385) = 1;
        local_bc._0_1_ = 5;
        FUN_00402560();
        goto LAB_005c6c4b;
      default:
        goto switchD_005c6259_caseD_2727;
      case 0x2728:
        uVar22 = DAT_00789b90;
        break;
      case 0x2741:
        uVar22 = DAT_00789b74;
        break;
      case 0x2742:
        uVar22 = DAT_00789b8c;
        break;
      case 0x2743:
      case 0x2751:
        uVar22 = DAT_00789b84;
        break;
      case 0x2744:
        uVar22 = DAT_00789b88;
        break;
      case 0x2746:
        uVar22 = DAT_00789b78;
        break;
      case 0x274c:
        goto switchD_005c6259_caseD_274c;
      case 0x274d:
        uVar23 = DAT_00789b6c;
        goto LAB_005c6266;
      case 0x2750:
        uVar22 = DAT_00789b7c;
      }
      goto LAB_005c6c43;
    }
LAB_005c6266:
    FUN_00401970(uVar23,DAT_00788e50);
    *(undefined1 *)(param_1 + 0x385) = 1;
    goto LAB_005c6c4b;
  }
  switch(uVar20) {
  case 0x7533:
switchD_005c63b6_caseD_7533:
    uVar22 = DAT_00789dc4;
    break;
  default:
    goto switchD_005c6259_caseD_2727;
  case 0x7539:
  case 0x753f:
  case 0x7550:
  case 0x7552:
  case 0x7554:
  case 0x7556:
  case 0x7557:
  case 0x755a:
  case 0x755d:
  case 0x756c:
  case 0x756e:
switchD_005c63b6_caseD_7539:
    break;
  case 0x753b:
    uVar22 = DAT_00789b68;
    break;
  case 0x7542:
    uVar22 = DAT_00789b60;
    break;
  case 0x7548:
    uVar22 = DAT_00789b94;
    break;
  case 0x754a:
  case 0x754b:
    uVar22 = DAT_00789bfc;
    break;
  case 0x754c:
    uVar22 = DAT_00789c00;
    break;
  case 0x754d:
    uVar22 = DAT_00789c04;
    break;
  case 0x7553:
    uVar22 = DAT_00789e00;
    break;
  case 0x7558:
    FUN_005bdcf0(0);
    uVar22 = DAT_00789b50;
    if (((*(int *)(param_1 + 8) == 0) && (uVar22 = DAT_00789b50, *(int *)(param_1 + 4) == 0)) &&
       (uVar22 = DAT_00789b50, param_2 != '\0')) {
      if ((*(int *)(param_1 + 0x540) == 0) ||
         (BVar10 = IsWindow(*(HWND *)(*(int *)(param_1 + 0x540) + 0x20)), BVar10 == 0)) {
        pCVar8 = (CWnd *)0x0;
      }
      else {
        pCVar8 = CWnd::FromHandle(*(HWND__ **)(*(int *)(param_1 + 0x540) + 0x20));
      }
      uVar22 = FUN_0049f340(*(undefined4 *)(param_1 + 0x44));
      FUN_004030a0(uVar22);
      local_bc._0_1_ = 6;
      FUN_004fd020(local_d8,*(undefined4 *)(param_1 + 0x20),*(undefined4 *)(param_1 + 0x1f0),0,0,
                   pCVar8,0);
      local_bc._0_1_ = 7;
      iVar7 = FUN_00612dbf();
      if (iVar7 == 1) {
        if (*(HWND *)(param_1 + 0x10) != (HWND)0x0) {
          SendMessageA(*(HWND *)(param_1 + 0x10),0x111,0x1472,0);
        }
        *(undefined4 *)(param_1 + 0x10) = 0;
        FUN_005be010(local_138,local_134);
        if (*(int *)(param_1 + 0x510) != 0) {
          if (*(int *)(param_1 + 0x4c4) == 0) {
            FUN_005d6890(*(int *)(param_1 + 0x510));
          }
          if (*(HKEY *)(param_1 + 0x4c4) != (HKEY)0x0) {
            pBVar9 = local_138;
            do {
              BVar2 = *pBVar9;
              pBVar9 = pBVar9 + 1;
            } while (BVar2 != '\0');
            RegSetValueExA(*(HKEY *)(param_1 + 0x4c4),"User",0,1,local_138,
                           (DWORD)(pBVar9 + (1 - (int)(local_138 + 1))));
            if (*(char *)(param_1 + 0x1e0) != '\0') {
              local_e4 = 1;
              RegSetValueExA(*(HKEY *)(param_1 + 0x4c4),"bUNRA",0,4,(BYTE *)&local_e4,4);
            }
            FUN_005c11b0(*(undefined4 *)(param_1 + 0x4c4),"Password","EncPassword",local_134,0);
          }
        }
        FUN_004024f0();
        local_bc = CONCAT31(local_bc._1_3_,8);
        FUN_00402f20(&local_cc,"\r\n%s\r\ndownlResult = %ldsetting new login\r\n",DAT_00789b50,
                     *(undefined4 *)(param_1 + 0xae4));
        FUN_0056f5a0(local_cc);
        if (((param_3 != '\0') && (*(HWND *)(param_1 + 0x14) != (HWND)0x0)) &&
           (*(int *)(param_1 + 0x510) != 0)) {
          PostMessageA(*(HWND *)(param_1 + 0x14),0x111,0x1431,*(int *)(param_1 + 0x510));
        }
        local_bc._0_1_ = 7;
        FUN_00402560();
        local_bc._0_1_ = 6;
        FUN_004bdfe0();
        local_bc._0_1_ = 5;
        FUN_00402560();
        goto LAB_005c63d7;
      }
      local_bc._0_1_ = 6;
      FUN_004bdfe0();
      local_bc._0_1_ = 5;
      FUN_00402560();
      uVar22 = DAT_00789b50;
    }
    break;
  case 0x7559:
  case 0x756f:
    FUN_00402f80(&DAT_006979dc);
    if (((*(int *)(param_1 + 8) == 0) && (*(int *)(param_1 + 4) == 0)) && (param_2 != '\0')) {
      FUN_005bdcf0(0);
      if ((*(int *)(param_1 + 0x540) == 0) ||
         (BVar10 = IsWindow(*(HWND *)(*(int *)(param_1 + 0x540) + 0x20)), BVar10 == 0)) {
        local_cc = (CWnd *)0x0;
      }
      else {
        local_cc = CWnd::FromHandle(*(HWND__ **)(*(int *)(param_1 + 0x540) + 0x20));
      }
      uVar22 = FUN_0049f340(*(undefined4 *)(param_1 + 0x44));
      FUN_004030a0(uVar22);
      local_bc._0_1_ = 9;
      pcVar16 = *(char **)(param_1 + 0x3cc);
      pcVar17 = pcVar16 + 1;
      do {
        cVar4 = *pcVar16;
        pcVar16 = pcVar16 + 1;
      } while (cVar4 != '\0');
      FUN_004024f0();
      local_bc = CONCAT31(local_bc._1_3_,10);
      if (((pcVar16 != pcVar17) || (*(char *)(param_1 + 0xaaa) != '\0')) ||
         ((DAT_00789904 == 2 && ((DAT_00789918 & 0x10) != 0)))) {
        iVar7 = *(int *)(param_1 + 0x974);
        if (iVar7 == 0) {
          uVar22 = *(undefined4 *)(param_1 + 0xab8);
          iVar7 = *(int *)(param_1 + 0xab4);
        }
        else {
          uVar22 = *(undefined4 *)(param_1 + 0x978);
        }
        FUN_00402f20(&local_d8,"%s:%ld",iVar7,uVar22);
      }
      else if (*(int *)(param_1 + 0x974) == 0) {
        ATL::CSimpleStringT<char,0>::operator=
                  ((CSimpleStringT<char,0> *)&local_d8,(CSimpleStringT<char,0> *)(param_1 + 0xab4));
      }
      else {
        FUN_00402f80(*(int *)(param_1 + 0x974));
      }
      FUN_004fd020(local_d4,local_d8,*(undefined4 *)(param_1 + 0xa94),1,
                   *(undefined1 *)(param_1 + 0xaaa),local_cc,pcVar16 != pcVar17);
      local_bc = CONCAT31(local_bc._1_3_,0xb);
      iVar7 = FUN_00612dbf();
      if (iVar7 == 1) {
        *(undefined1 *)(param_1 + 0xaad) = 1;
        if (*(HWND *)(param_1 + 0x10) != (HWND)0x0) {
          SendMessageA(*(HWND *)(param_1 + 0x10),0x111,0x1472,0);
        }
        *(undefined4 *)(param_1 + 0x10) = 0;
        local_dd = 1;
        if (*(int *)(param_1 + 0x974) == 0) {
          *(undefined1 *)(param_1 + 0xad1) = 1;
          FUN_00402f20(param_1 + 0xabc,&DAT_00696f08,local_138);
          FUN_00402f20(param_1 + 0xac0,&DAT_00696f08,local_134);
        }
        else {
          iVar7 = FUN_004bb640(local_138);
          if ((iVar7 != 0) || (iVar7 = FUN_004bb6b0(local_134), iVar7 != 0)) {
            local_dd = 0;
            goto LAB_005c67ea;
          }
        }
        FUN_004024f0();
        local_bc = CONCAT31(local_bc._1_3_,0xc);
        uVar22 = DAT_0078a7c4;
        if (*(char *)(param_1 + 0xaaa) == '\0') {
          uVar22 = DAT_00789b54;
        }
        FUN_00402f20(&local_cc,"\r\n%s\r\ndownlResult = %ldsetting new login\r\n",uVar22,
                     *(undefined4 *)(param_1 + 0xae4));
        FUN_0056f5a0(local_cc);
        if (((param_3 != '\0') && (*(HWND *)(param_1 + 0x14) != (HWND)0x0)) &&
           (*(int *)(param_1 + 0x510) != 0)) {
          PostMessageA(*(HWND *)(param_1 + 0x14),0x111,0x1431,*(int *)(param_1 + 0x510));
        }
        local_bc._0_1_ = 0xb;
        FUN_00402560();
        local_bc._0_1_ = 10;
        FUN_004bdfe0();
        local_bc._0_1_ = 9;
        FUN_00402560();
        local_bc._0_1_ = 5;
        FUN_00402560();
        goto LAB_005c63d7;
      }
LAB_005c67ea:
      local_bc._0_1_ = 10;
      FUN_004bdfe0();
      local_bc._0_1_ = 9;
      FUN_00402560();
      local_bc._0_1_ = 5;
      FUN_00402560();
    }
    puVar11 = &DAT_0078a7c4;
    if (*(char *)(param_1 + 0xaaa) == '\0') {
      puVar11 = &DAT_00789b54;
    }
    uVar22 = *puVar11;
    break;
  case 0x755e:
    uVar22 = DAT_00789b4c;
    break;
  case 0x755f:
  case 0x7564:
    uVar22 = DAT_00789d68;
    break;
  case 0x7560:
    FUN_005be690(*(undefined4 *)(param_1 + 0x520),*(undefined4 *)(param_1 + 0x524),local_18);
    local_ac = (undefined2 *)0x0;
    local_a4 = 0;
    local_a8[0] = 0;
    local_bc._0_1_ = 0xf;
    FUN_005bfa90(&local_ac);
    FUN_00406f90(local_18);
    local_bc._0_1_ = 0x10;
    FUN_004024f0();
    local_bc._0_1_ = 0x11;
    FUN_00402f20(&local_d4,"%I64d",*(undefined4 *)(param_1 + 0x520),*(undefined4 *)(param_1 + 0x524)
                );
    FUN_00406f90(local_d4);
    local_bc = CONCAT31(local_bc._1_3_,0x12);
    pcVar17 = _strstr(DAT_00789d88,"%I64d");
    if (pcVar17 == (char *)0x0) {
      pcVar17 = _strstr(DAT_00789d88,"%ld");
      if (pcVar17 != (char *)0x0) {
        pcVar17 = "%ld";
        goto LAB_005c6aa9;
      }
    }
    else {
      pcVar17 = "%I64d";
LAB_005c6aa9:
      FUN_00402c00(pcVar17,&DAT_00696f08);
    }
    if (*(int *)(param_1 + 0x54) == 0) {
      iVar7 = param_1 + 0x58;
    }
    else {
      iVar7 = *(int *)(param_1 + 0x54);
    }
    puVar19 = local_70;
    if (local_70 == (undefined2 *)0x0) {
      puVar19 = local_6c;
    }
    puVar18 = local_7c;
    if (local_7c == (undefined2 *)0x0) {
      puVar18 = local_78;
    }
    puVar12 = local_ac;
    if (local_ac == (undefined2 *)0x0) {
      puVar12 = local_a8;
    }
    FUN_0043b1c0(DAT_00789d88,puVar12,puVar18,puVar19,iVar7,DAT_00788e50);
    local_bc._0_1_ = 0x11;
    FUN_004011f0();
    local_bc._0_1_ = 0x10;
    FUN_00402560();
    local_bc._0_1_ = 0xf;
    FUN_004011f0();
    local_bc._0_1_ = 5;
    FUN_004011f0();
    goto LAB_005c6c4b;
  case 0x7561:
    uVar22 = DAT_00789d9c;
    break;
  case 0x7565:
  case 0x757d:
    uVar22 = DAT_00789db0;
    break;
  case 0x7566:
    uVar22 = DAT_00789db8;
    break;
  case 0x7567:
  case 0x7569:
    uVar22 = DAT_00789dc0;
    break;
  case 0x756d:
    uVar22 = DAT_00789e48;
    break;
  case 0x7570:
    FUN_00402170(param_1 + 0x548);
    local_bc._0_1_ = 0x15;
    puVar19 = local_70;
    if (local_70 == (undefined2 *)0x0) {
      puVar19 = local_6c;
    }
    FUN_004021a0(DAT_0078a2a0,puVar19,DAT_00788e50);
    local_bc._0_1_ = 5;
    FUN_004011f0();
    goto LAB_005c6c4b;
  case 0x7573:
    FUN_005bdcf0(0);
    goto LAB_005c63d7;
  case 0x7577:
    uVar22 = DAT_0078a5ac;
    break;
  case 0x7578:
    uVar22 = DAT_00789c3c;
    break;
  case 0x757a:
    uVar22 = DAT_0078a658;
    break;
  case 0x7582:
    FUN_004024f0();
    local_bc._0_1_ = 0x16;
    FUN_00402f20(&local_cc,&DAT_00696f08,DAT_0078a774);
    pcVar17 = _strpbrk((char *)local_cc,"\r\n");
    if (pcVar17 != (char *)0x0) {
      *pcVar17 = '\0';
    }
    FUN_00401970(local_cc,DAT_00788e50);
    local_bc._0_1_ = 5;
    FUN_00402560();
    goto LAB_005c6c4b;
  case 0x7588:
    uVar22 = DAT_0078a914;
    break;
  case 0x7589:
    uVar22 = DAT_0078a918;
    break;
  case 0x758a:
    uVar22 = DAT_0078a91c;
  }
LAB_005c6c43:
  FUN_00401970(uVar22,DAT_00788e50);
LAB_005c6c4b:
  if (*(HWND *)(param_1 + 0x14) != (HWND)0x0) {
    SendMessageA(*(HWND *)(param_1 + 0x14),0x111,0x13c7,*(LPARAM *)(param_1 + 0x510));
  }
  switch(*(undefined4 *)(param_1 + 0xae4)) {
  case 0x7542:
  case 0x7565:
  case 0x7566:
  case 0x7578:
    if (*(int *)(param_1 + 0x160) != 0 || *(int *)(param_1 + 0x164) != 0) {
      FUN_00405300(&DAT_006979d8);
      FUN_00460040(DAT_0078a658,DAT_00788e50);
      local_bc._0_1_ = 0x1a;
      puVar19 = local_70;
      if (local_70 == (undefined2 *)0x0) {
        puVar19 = local_6c;
      }
      FUN_00405300(puVar19);
      local_bc._0_1_ = 5;
      FUN_004011f0();
      goto switchD_005c6c85_caseD_757a;
    }
    break;
  case 0x757a:
switchD_005c6c85_caseD_757a:
    if (((*(char *)(param_1 + 0x1b5) == '\0') &&
        ((*(int *)(param_1 + 0x168) < 1 ||
         (0x3b < *(int *)(param_1 + 0x16c) - *(int *)(param_1 + 0x168))))) ||
       ((*(uint *)(param_1 + 0x184) & 0x100) != 0)) {
      if ((((DAT_007893e7 == '\0') && (*(int *)(param_1 + 0x108) == *(int *)(param_1 + 0x160))) &&
          (*(int *)(param_1 + 0x10c) == *(int *)(param_1 + 0x164))) &&
         (*(int *)(param_1 + 0xdc) != 0)) {
        pbVar24 = *(byte **)(param_1 + 0x140);
        pbVar14 = pbVar24;
        do {
          bVar3 = *pbVar14;
          pbVar14 = pbVar14 + 1;
        } while (bVar3 != 0);
        if (pbVar14 != pbVar24 + 1) {
          pbVar14 = *(byte **)(param_1 + 0x104);
          pbVar15 = pbVar14;
          do {
            bVar3 = *pbVar15;
            pbVar15 = pbVar15 + 1;
          } while (bVar3 != 0);
          pbVar26 = pbVar24;
          if (pbVar15 != pbVar14 + 1) {
            do {
              bVar3 = *pbVar14;
              bVar27 = bVar3 < *pbVar26;
              if (bVar3 != *pbVar26) {
LAB_005c6f53:
                iVar7 = (1 - (uint)bVar27) - (uint)(bVar27 != 0);
                goto LAB_005c6f58;
              }
              if (bVar3 == 0) break;
              bVar3 = pbVar14[1];
              bVar27 = bVar3 < pbVar26[1];
              if (bVar3 != pbVar26[1]) goto LAB_005c6f53;
              pbVar14 = pbVar14 + 2;
              pbVar26 = pbVar26 + 2;
            } while (bVar3 != 0);
            iVar7 = 0;
LAB_005c6f58:
            if (iVar7 == 0) {
              pbVar14 = *(byte **)(param_1 + 0xdc);
              do {
                bVar3 = *pbVar14;
                bVar27 = bVar3 < *pbVar24;
                if (bVar3 != *pbVar24) {
LAB_005c6f84:
                  iVar7 = (1 - (uint)bVar27) - (uint)(bVar27 != 0);
                  goto LAB_005c6f89;
                }
                if (bVar3 == 0) break;
                bVar3 = pbVar14[1];
                bVar27 = bVar3 < pbVar24[1];
                if (bVar3 != pbVar24[1]) goto LAB_005c6f84;
                pbVar14 = pbVar14 + 2;
                pbVar24 = pbVar24 + 2;
              } while (bVar3 != 0);
              iVar7 = 0;
LAB_005c6f89:
              if (iVar7 != 0) {
                local_da = '\x01';
                FUN_00401970(DAT_0078a554,DAT_00788e50);
                break;
              }
            }
          }
        }
      }
      FUN_00405300(L"\r\n\r\n");
      FUN_00460040(DAT_0078a678,DAT_00788e50);
      local_bc._0_1_ = 0x1c;
      puVar19 = local_70;
      if (local_70 == (undefined2 *)0x0) {
        puVar19 = local_6c;
      }
      FUN_00405300(puVar19);
      local_bc._0_1_ = 5;
      FUN_004011f0();
    }
    else {
      if (*(char *)(param_1 + 0x1b5) == '\0') {
        FUN_005d8f90(*(undefined4 *)(param_1 + 0x510),1);
      }
      if ((*(int *)(param_1 + 0x524) < 0) ||
         ((*(int *)(param_1 + 0x524) < 1 && (*(int *)(param_1 + 0x520) == 0)))) {
        bVar3 = *(byte *)(param_1 + 0x184);
        local_b8 = (undefined2 *)0x0;
        local_b0 = 0;
        local_b4[0] = 0;
        local_bc = CONCAT31(local_bc._1_3_,0x1b);
        FUN_005bfa90(&local_b8);
        if ((bVar3 & 0x70) == 0) {
          uVar22 = DAT_0078a67c;
          if ((*(char *)(param_1 + 0x1b3) != '\0') && (DAT_007893e7 == '\0')) {
            uVar22 = DAT_0078a554;
          }
          FUN_00401970(uVar22,DAT_00788e50);
        }
        else {
          pcVar17 = *(char **)(param_1 + 0xe8);
          pcVar16 = pcVar17;
          do {
            cVar4 = *pcVar16;
            pcVar16 = pcVar16 + 1;
          } while (cVar4 != '\0');
          if (pcVar16 == pcVar17 + 1) {
            FUN_0054a900(*(undefined4 *)(param_1 + 0x20));
          }
          else {
            FUN_0054aa20(pcVar17);
          }
          if (*(int *)(param_1 + 0x128) == 0) {
            psVar13 = (short *)(param_1 + 300);
          }
          else {
            psVar13 = *(short **)(param_1 + 0x128);
          }
          psVar1 = psVar13 + 1;
          do {
            sVar5 = *psVar13;
            psVar13 = psVar13 + 1;
          } while (sVar5 != 0);
          if ((int)psVar13 - (int)psVar1 >> 1 == 0) {
            puVar19 = local_b8;
            if (local_b8 == (undefined2 *)0x0) {
              puVar19 = local_b4;
            }
          }
          else if (*(int *)(param_1 + 0x128) == 0) {
            puVar19 = (undefined2 *)(param_1 + 300);
          }
          else {
            puVar19 = *(undefined2 **)(param_1 + 0x128);
          }
          FUN_004021a0(DAT_0078a558,puVar19,DAT_00788e50);
          if (((((*(uint *)(param_1 + 0x184) & 0x100) == 0) && (0 < *(int *)(param_1 + 0x168))) &&
              (*(int *)(param_1 + 0x16c) - *(int *)(param_1 + 0x168) < 0x3c)) && (local_cd == '\0'))
          {
            local_d9 = '\x01';
            local_bc._0_1_ = 5;
            FUN_004011f0();
            break;
          }
        }
        local_bc._0_1_ = 5;
        FUN_004011f0();
      }
      else {
        FUN_00401970(DAT_0078a67c,DAT_00788e50);
      }
    }
  }
  if ((*(int *)(param_1 + 0xae4) != 0x7582) && (*(int *)(param_1 + 0x544) != 0)) {
    FUN_00405300(L"\r\n\r\n");
    local_14 = (undefined2 *)0x0;
    local_c = 0;
    local_10[0] = 0;
    local_bc._0_1_ = 0x1d;
    FUN_00401970(DAT_0078a080,DAT_00788e50);
    puVar19 = local_14;
    if (local_14 == (undefined2 *)0x0) {
      puVar19 = local_10;
    }
    FUN_00405300(puVar19);
    FUN_00460040(*(undefined4 *)(param_1 + 0x544),DAT_00788e50);
    local_bc._0_1_ = 0x1e;
    puVar19 = local_70;
    if (local_70 == (undefined2 *)0x0) {
      puVar19 = local_6c;
    }
    FUN_00405300(puVar19);
    local_bc._0_1_ = 0x1d;
    FUN_004011f0();
    local_bc._0_1_ = 5;
    FUN_004011f0();
  }
  if (local_cd != '\0') {
    FUN_00405300(L"\r\n\r\n");
    FUN_00460040(DAT_0078a8e8,DAT_00788e50);
    local_bc = CONCAT31(local_bc._1_3_,0x1f);
    puVar19 = local_7c;
    if (local_7c == (undefined2 *)0x0) {
      puVar19 = local_78;
    }
    FUN_00405300(puVar19);
    local_db = 0;
    pcVar17 = *(char **)(param_1 + 0x154);
    pcVar16 = pcVar17;
    do {
      cVar4 = *pcVar16;
      pcVar16 = pcVar16 + 1;
    } while (cVar4 != '\0');
    if (pcVar16 != pcVar17 + 1) {
      pcVar17 = _strrchr(pcVar17,0x5c);
      if (pcVar17 == (char *)0x0) {
        pcVar17 = *(char **)(param_1 + 0x154);
      }
      else {
        pcVar17 = pcVar17 + 1;
      }
      iVar7 = __memicmp(pcVar17,"chrome*exe",10);
      if ((iVar7 == 0) ||
         (((iVar7 = __stricmp(pcVar17,"idmbrbtn.dll"), iVar7 == 0 &&
           (pcVar17 = _strstr(*(char **)(param_1 + 0x1cc),"Chrome"), pcVar17 != (char *)0x0)) &&
          (pcVar17 = _strstr(*(char **)(param_1 + 0x1cc),"Firefox"), pcVar17 == (char *)0x0)))) {
        local_db = 1;
        FUN_00405300(&DAT_006a3eec);
        FUN_00460040(DAT_0078a8ec,DAT_00788e50);
        local_bc._0_1_ = 0x20;
        puVar19 = local_70;
        if (local_70 == (undefined2 *)0x0) {
          puVar19 = local_6c;
        }
        FUN_00405300(puVar19);
        local_bc = CONCAT31(local_bc._1_3_,0x1f);
        FUN_004011f0();
      }
    }
    local_bc._0_1_ = 5;
    FUN_004011f0();
  }
  puVar19 = local_64;
  if (local_64 == (undefined2 *)0x0) {
    puVar19 = local_60;
  }
  FUN_00402a60(&local_e8,puVar19,DAT_00788e50);
  local_bc._0_1_ = 0x21;
  FUN_005d2d20(*(undefined4 *)(param_1 + 0xae4),local_e8,*(undefined4 *)(param_1 + 0x510),0);
  FUN_0056f5a0(local_e8);
  _sprintf(local_58,"\r\ndownlResult = %ld\r\n",*(undefined4 *)(param_1 + 0xae4));
  FUN_0056f5a0(local_58);
  switch(*(undefined4 *)(param_1 + 0xae4)) {
  case 0x7542:
  case 0x7565:
  case 0x7566:
  case 0x7578:
    if (*(int *)(param_1 + 0x160) != 0 || *(int *)(param_1 + 0x164) != 0)
    goto switchD_005c7266_caseD_757a;
    break;
  case 0x757a:
switchD_005c7266_caseD_757a:
    if ((((*(int *)(param_1 + 8) == 0) && (*(int *)(param_1 + 4) == 0)) && (param_2 != '\0')) &&
       (((*(int *)(param_1 + 0x14) != 0 && (*(int *)(param_1 + 0x510) != 0)) &&
        ((*(char *)(param_1 + 0x1b5) == '\0' && (local_da == '\0')))))) {
      FUN_005bdcf0(0);
      PostMessageA(*(HWND *)(param_1 + 0x14),0x111,0x1463,*(LPARAM *)(param_1 + 0x510));
      *(undefined1 *)(param_1 + 0x385) = 0;
      local_bc._0_1_ = 5;
      FUN_00402560();
LAB_005c63d7:
      local_bc = (uint)local_bc._1_3_ << 8;
      FUN_004011f0();
      goto LAB_005c63e3;
    }
  }
  local_94 = (undefined2 *)0x0;
  local_8c = 0;
  local_90[0] = 0;
  local_bc._0_1_ = 0x22;
  FUN_005bfa90(&local_94);
  if (((*(int *)(param_1 + 8) == 0) && (*(int *)(param_1 + 4) == 0)) &&
     ((param_2 != '\0' &&
      (((*(int *)(param_1 + 0x510) != 0 && (*(int *)(param_1 + 0x53c) == 0)) &&
       (*(int *)(param_1 + 0xae4) != 0x7540)))))) {
    if (*(int *)(param_1 + 0xae4) == 0x7553) {
      puVar19 = local_94;
      if (local_94 == (undefined2 *)0x0) {
        puVar19 = local_90;
      }
      puVar18 = local_64;
      if (local_64 == (undefined2 *)0x0) {
        puVar18 = local_60;
      }
      iVar7 = FUN_00475340(0,puVar18,puVar19,0x10024);
      if ((iVar7 == 6) && (*(HWND *)(param_1 + 0x14) != (HWND)0x0)) {
        SendMessageA(*(HWND *)(param_1 + 0x14),0x111,0x13bb,*(LPARAM *)(param_1 + 0x510));
      }
    }
    else {
      local_88 = (undefined2 *)0x0;
      local_80 = 0;
      local_84[0] = 0;
      local_bc = CONCAT31(local_bc._1_3_,0x23);
      if (local_e4 == 0) {
        puVar19 = local_94;
        if (local_94 == (undefined2 *)0x0) {
          puVar19 = local_90;
        }
        FUN_004017e0(puVar19);
      }
      else {
        FUN_00401970(local_e4,DAT_00788e50);
      }
      if ((*(int *)(param_1 + 0xae4) == -0x7ff6fcda) && (DAT_00788ccc == 0)) {
        local_7c = (undefined2 *)0x0;
        local_74 = 0;
        local_78[0] = 0;
        local_bc._0_1_ = 0x24;
        FUN_00460040(DAT_0078a87c,DAT_00788e50);
        local_bc = CONCAT31(local_bc._1_3_,0x25);
        puVar19 = local_70;
        if (local_70 == (undefined2 *)0x0) {
          puVar19 = local_6c;
        }
        puVar18 = local_64;
        if (local_64 == (undefined2 *)0x0) {
          puVar18 = local_60;
        }
        FUN_00402310("%s\r\n\r\n\r\n%s",puVar18,puVar19,0);
        puVar19 = local_88;
        if (local_88 == (undefined2 *)0x0) {
          puVar19 = local_84;
        }
        puVar18 = local_7c;
        if (local_7c == (undefined2 *)0x0) {
          puVar18 = local_78;
        }
        iVar7 = FUN_00475340(0,puVar18,puVar19,0x50034);
        if (iVar7 == 6) {
          FUN_004b76a0(0,"https://www.internetdownloadmanager.com/support/xp-https-problems.html");
        }
        local_bc._0_1_ = 0x24;
        FUN_004011f0();
LAB_005c77ab:
        local_bc = CONCAT31(local_bc._1_3_,0x23);
        FUN_004011f0();
      }
      else {
        if (*(char *)(param_1 + 0x385) == '\0') {
          puVar19 = local_64;
          if (local_64 == (undefined2 *)0x0) {
            puVar19 = local_60;
          }
          FUN_00402170(puVar19);
          local_bc = CONCAT31(local_bc._1_3_,0x2a);
          if (local_d9 != '\0') {
            if (local_da == '\0') {
              if ((*(uint *)(param_1 + 0x184) & 0x100000) == 0) {
                pcVar17 = *(char **)(param_1 + 0x154);
                pcVar16 = pcVar17;
                do {
                  cVar4 = *pcVar16;
                  pcVar16 = pcVar16 + 1;
                } while (cVar4 != '\0');
                if (pcVar16 != pcVar17 + 1) {
                  pcVar17 = _strrchr(pcVar17,0x5c);
                  if (pcVar17 == (char *)0x0) {
                    pcVar17 = *(char **)(param_1 + 0x154);
                  }
                  else {
                    pcVar17 = pcVar17 + 1;
                  }
                  iVar7 = __strnicmp(pcVar17,"firefox.exe",0xb);
                  if (((iVar7 == 0) || (iVar7 = __memicmp(pcVar17,"chrome.exe",10), iVar7 == 0)) ||
                     ((iVar7 = __memicmp(pcVar17,"msedge.exe",10), iVar7 == 0 ||
                      (iVar7 = __memicmp(pcVar17,"MicrosoftEdgeCP.exe",0x13), iVar7 == 0)))) {
                    FUN_00460040(DAT_0078a8e0,DAT_00788e50);
                    local_bc._0_1_ = 0x2b;
                    FUN_00405300(L"\r\n\r\n");
                    puVar19 = local_7c;
                    if (local_7c == (undefined2 *)0x0) {
                      puVar19 = local_78;
                    }
                    FUN_00405300(puVar19);
                    local_bc = CONCAT31(local_bc._1_3_,0x2a);
                    FUN_004011f0();
                  }
                }
              }
            }
            else {
              local_d9 = '\0';
            }
          }
          if ((*(int *)(param_1 + 0xae4) == -1) ||
             (uVar20 = 0x30, *(int *)(param_1 + 0xae4) == 0x753b)) {
            uVar20 = 0x10;
          }
          puVar19 = local_88;
          if (local_88 == (undefined2 *)0x0) {
            puVar19 = local_84;
          }
          puVar18 = local_70;
          if (local_70 == (undefined2 *)0x0) {
            puVar18 = local_6c;
          }
          FUN_00475340(0,puVar18,puVar19,uVar20 | 0x50000);
          if (local_d9 != '\0') {
            if (*(char *)(param_1 + 0x393) != '\0') {
              if (*(int *)(param_1 + 0x4c4) == 0) {
                FUN_005d6890(*(undefined4 *)(param_1 + 0x510));
              }
              FUN_005d9be0();
            }
            PostMessageA(*(HWND *)(param_1 + 0x14),0x111,0x14cd,*(LPARAM *)(param_1 + 0x510));
          }
          if (*(int *)(param_1 + 0xae4) == 0x757a) {
            FUN_005d4be0();
          }
          goto LAB_005c77ab;
        }
        local_70 = (undefined2 *)0x0;
        local_68 = 0;
        local_6c[0] = 0;
        local_bc._0_1_ = 0x26;
        FUN_00460040(DAT_0078a7d8,DAT_00788e50);
        local_bc._0_1_ = 0x27;
        puVar19 = local_7c;
        if (local_7c == (undefined2 *)0x0) {
          puVar19 = local_78;
        }
        puVar18 = local_64;
        if (local_64 == (undefined2 *)0x0) {
          puVar18 = local_60;
        }
        FUN_00402310("%s\r\n\r\n\r\n%s",puVar18,puVar19,0);
        puVar19 = local_88;
        if (local_88 == (undefined2 *)0x0) {
          puVar19 = local_84;
        }
        puVar18 = local_70;
        if (local_70 == (undefined2 *)0x0) {
          puVar18 = local_6c;
        }
        iVar7 = FUN_00475340(0,puVar18,puVar19,0x50034);
        if (iVar7 == 6) {
          FUN_004024f0();
          local_bc._0_1_ = 0x28;
          FUN_00598c40(&local_cc,0);
          FUN_004024f0();
          local_bc._0_1_ = 0x29;
          puVar25 = *(undefined1 **)(param_1 + 0x20);
          if (puVar25 == (undefined1 *)0x0) {
            puVar25 = &DAT_006979dc;
          }
          if ((*(char *)(param_1 + 0xad0) == '\0') && (*(int *)(param_1 + 0x974) == 0)) {
            uVar22 = 0;
          }
          else {
            uVar22 = 1;
          }
          FUN_00402f20(&local_d4,"%s?err=%ld&ocr=%ld&v6=%ld&proxy=%ld&srv=%s",
                       "https://www.internetdownloadmanager.com/register/new_faq/How_to_configure_Firewalls_for_IDM.html"
                       ,*(undefined4 *)(param_1 + 0xae4),*(undefined4 *)(param_1 + 0x1ec),
                       *(char *)(param_1 + 0x1bb) != '\0',uVar22,puVar25);
          FUN_004b76a0(0,local_d4);
          local_bc._0_1_ = 0x28;
          FUN_00402560();
          local_bc._0_1_ = 0x27;
          FUN_00402560();
        }
        local_bc._0_1_ = 0x26;
        FUN_004011f0();
        local_bc = CONCAT31(local_bc._1_3_,0x23);
        FUN_004011f0();
      }
      local_bc._0_1_ = 0x22;
      FUN_004011f0();
    }
  }
  pCVar8 = *(CWnd **)(param_1 + 4);
  if (pCVar8 == (CWnd *)0x0) {
    *(undefined1 *)(param_1 + 0x385) = 0;
  }
  else if (((pCVar8 == (CWnd *)0x3) && (*(int *)(param_1 + 0xae4) == 0x756e)) || (param_2 == '\0'))
  {
    if ((pCVar8 == (CWnd *)0x4) && (*(char *)(param_1 + 0x385) != '\0')) {
      *(undefined1 *)(param_1 + 0x385) = 0;
      local_cc = pCVar8;
      LVar21 = RegQueryValueExA(DAT_00788d08,"bHDIShwd",(LPDWORD)0x0,(LPDWORD)0x0,(LPBYTE)&local_d4,
                                (LPDWORD)&local_cc);
      if (LVar21 != 0) {
        local_d4 = (HLOCAL)0x1;
        RegSetValueExA(DAT_00788d08,"bHDIShwd",0,4,(BYTE *)&local_d4,4);
        uVar22 = FUN_00410b20();
        FUN_00530580(uVar22);
        local_bc._0_1_ = 0x2d;
        FUN_00612dbf();
        local_bc._0_1_ = 0x22;
        FUN_00530600();
      }
    }
    FUN_005bdcf0(0);
  }
  else if (*(int *)(param_1 + 0xae4) == -1) {
    FUN_00474ee0(0,DAT_00789dac,"Internet Download Manager",0x50010);
    FUN_005bdcf0(0);
  }
  else {
    FUN_00406f90("Internet Download Manager");
    local_bc._0_1_ = 0x2c;
    if ((*(int *)(param_1 + 0xae4) == -1) || (uVar20 = 0x30, *(int *)(param_1 + 0xae4) == 0x753b)) {
      uVar20 = 0x10;
    }
    puVar19 = local_70;
    if (local_70 == (undefined2 *)0x0) {
      puVar19 = local_6c;
    }
    puVar18 = local_64;
    if (local_64 == (undefined2 *)0x0) {
      puVar18 = local_60;
    }
    FUN_00475340(0,puVar18,puVar19,uVar20 | 0x50000);
    local_bc._0_1_ = 0x22;
    FUN_004011f0();
    FUN_005bdcf0(0);
  }
  local_bc._0_1_ = 0x21;
  FUN_004011f0();
  local_bc._0_1_ = 5;
  FUN_00402560();
  local_bc = (uint)local_bc._1_3_ << 8;
  FUN_004011f0();
  local_bc = -1;
LAB_005c63e3:
  ExceptionList = local_c4;
  __security_check_cookie(local_8 ^ (uint)&local_b8);
  return;
}


