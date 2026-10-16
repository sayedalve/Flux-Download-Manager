// Function: FUN_00592d50
// Entry Point: 0x00592d50
// Program: IDMan.exe


void __thiscall FUN_00592d50(int *param_1,int param_2)

{
  short *psVar1;
  byte bVar2;
  short sVar3;
  int iVar4;
  uint uVar5;
  char cVar6;
  undefined4 uVar7;
  int iVar8;
  char *pcVar9;
  undefined4 uVar10;
  byte *pbVar11;
  byte *pbVar12;
  undefined4 *puVar13;
  char *pcVar14;
  undefined1 *puVar15;
  char *pcVar16;
  int iVar17;
  short *psVar18;
  int *piVar19;
  code *pcVar20;
  uint uVar21;
  int iVar22;
  bool bVar23;
  uint uStack_c8;
  undefined4 local_b8;
  ushort local_b4;
  int local_ac;
  int *local_a8;
  int local_a4;
  undefined4 local_a0;
  uint local_9c;
  WPARAM local_98;
  undefined4 local_94;
  undefined4 local_90;
  CTime local_8c [4];
  char *local_88;
  char local_81;
  undefined4 local_80;
  char local_79;
  WPARAM local_78;
  char *local_74;
  char local_6d;
  undefined4 local_6c;
  char *local_68;
  undefined1 *local_64;
  void *local_60;
  undefined1 *puStack_5c;
  int local_58;
  short *local_54;
  short local_50 [2];
  undefined4 local_4c;
  char local_48 [64];
  uint local_8;
  
  puStack_5c = &LAB_0067fb09;
  local_60 = ExceptionList;
  uStack_c8 = DAT_007839c0 ^ (uint)&local_54;
  local_64 = (undefined1 *)&uStack_c8;
  ExceptionList = &local_60;
  local_9c = 0;
  local_58 = 0;
  local_78 = 3000;
  local_98 = 5000;
  if ((*(char *)(param_1[0x32] + 0x19b) == '\0') ||
     (uVar7 = 5, *(char *)(param_1[0x32] + 0x36f) == '\0')) {
    uVar7 = 0xf;
  }
  piVar19 = param_1 + 0xc80;
  iVar17 = *piVar19;
  local_a8 = param_1;
  local_8 = uStack_c8;
  local_68 = (char *)(**(code **)(*param_1 + 0x1c))(iVar17,uVar7);
  if (*(int *)(param_1[0xc82] + 0x1c) != 0) goto LAB_00592fd1;
  if (((local_68 == (char *)0x2751) || (local_68 == (char *)0x2743)) &&
     ((*(uint *)(param_1[0x32] + 0x164) & 0x2400000) == 0)) {
    *(char **)(param_1[0x32] + 0x1cc) = local_68;
    *(bool *)(param_1[0x32] + 0x19b) = *(char *)(param_1[0x32] + 0x19b) == '\0';
    local_74 = local_68;
    (**(code **)(*param_1 + 0x18))(piVar19);
    local_68 = (char *)(**(code **)(*param_1 + 0x1c))(*piVar19,0x1e);
    if (*(int *)(param_1[0xc82] + 0x1c) != 0) goto LAB_00592fd1;
    if (local_74 == local_68) {
      *(bool *)(param_1[0x32] + 0x19b) = *(char *)(param_1[0x32] + 0x19b) == '\0';
    }
LAB_00592e7b:
    if (local_68 != (char *)0x0) goto LAB_00592e83;
  }
  else {
    if (local_68 != (char *)0x274c) goto LAB_00592e7b;
    iVar8 = param_1[0x32];
    if ((*(char *)(iVar8 + 0x19b) != '\0') && ((*(uint *)(iVar8 + 0x164) & 0x400000) == 0)) {
      *(undefined4 *)(iVar8 + 0x1cc) = 0x274c;
      *(undefined1 *)(param_1[0x32] + 0x19b) = 0;
      (**(code **)(*param_1 + 0x18))(piVar19);
      local_68 = (char *)(**(code **)(*param_1 + 0x1c))(*piVar19,0x1e);
      if (*(int *)(param_1[0xc82] + 0x1c) != 0) goto LAB_00592fd1;
      if (local_68 != (char *)0x274c) goto LAB_00592e7b;
      *(undefined1 *)(param_1[0x32] + 0x19b) = 1;
    }
LAB_00592e83:
    pcVar9 = local_68;
    iVar8 = FUN_00566ce0(local_68);
    if ((iVar8 == 0) || (iVar8 = FUN_00566c60(pcVar9), iVar8 != 0)) {
      do {
        iVar8 = FUN_00567aa0();
        if (iVar8 == 0) goto LAB_00592f80;
        (**(code **)(*param_1 + 0x18))(piVar19);
        pcVar9 = (char *)(**(code **)(*param_1 + 0x1c))(*piVar19,0x1e);
        local_68 = pcVar9;
        if (*(int *)(param_1[0xc82] + 0x1c) != 0) goto LAB_00592fd1;
        if (pcVar9 == (char *)0x0) goto LAB_00592feb;
        iVar8 = FUN_00566ce0(pcVar9);
      } while (iVar8 == 0);
      goto LAB_00592ef0;
    }
LAB_00592f80:
    if (pcVar9 != (char *)0x0) {
      local_ac = 0;
      do {
        iVar8 = FUN_00566ce0(pcVar9);
        if ((iVar8 != 0) || (local_ac = local_ac + 1, 5 < local_ac)) {
          if (pcVar9 != (char *)0x0) goto LAB_00592ef0;
          goto LAB_00592feb;
        }
        (**(code **)(*param_1 + 0x18))(piVar19);
        pcVar9 = (char *)(**(code **)(*param_1 + 0x1c))(*piVar19,0x1e);
        local_68 = pcVar9;
      } while (*(int *)(param_1[0xc82] + 0x1c) == 0);
LAB_00592fd1:
      FUN_0055e8b0();
      FUN_005954d0(uVar7);
      return;
    }
  }
LAB_00592feb:
  param_1[0xc9a] = 1;
  local_78 = local_78 + 2000;
  iVar8 = param_1[0x67];
  if (iVar8 != 0) {
    local_79 = *(char *)(iVar8 + 0x15c);
    if (local_79 == '\0') goto LAB_0059306f;
    local_68 = (char *)FUN_00580e60();
    if (local_68 == (char *)0x0) goto LAB_00593208;
    if (local_68 != (char *)0x7585) {
      iVar8 = param_1[0x67];
      if ((((*(char *)(iVar8 + 0x15d) == '\0') && (local_68 == (char *)0x7559)) &&
          (*(int *)(iVar8 + 0x154) == 5)) &&
         ((*(char *)(iVar8 + 0x15e) == '\0' && (*(char *)(iVar8 + 0x15f) == '\0')))) {
        *(undefined1 *)(iVar8 + 0x15e) = 1;
        *(undefined1 *)(param_1[0x67] + 0x15f) = 1;
        *(undefined4 *)(param_1[0x67] + 0x154) = 4;
        pcVar9 = "Restart type 003\n";
        goto LAB_00593038;
      }
      if (((*(int *)(iVar8 + 0x154) != 5) || (local_68 != (char *)0x7580)) ||
         (*(char *)(iVar8 + 0x15e) != '\0')) {
        if ((((*(char *)(iVar8 + 0x15d) == '\0') && (*(int *)(iVar8 + 0x154) == 4)) &&
            (*(char *)(iVar8 + 0x15e) != '\0')) && (*(char *)(iVar8 + 0x15f) != '\0')) {
          *(undefined4 *)(iVar8 + 0x154) = 4;
          local_68 = (char *)0x7559;
        }
        FUN_005954d0(uVar7);
        return;
      }
      *(undefined1 *)(iVar8 + 0x15e) = 1;
      *(undefined4 *)(param_1[0x67] + 0x154) = 4;
      pcVar9 = "Restart type 004\n";
      local_68 = (char *)0x7580;
      goto LAB_00593195;
    }
    pcVar9 = "Restart type 002\n";
LAB_00593038:
    FUN_00566b20(pcVar9);
    FUN_00582fd0();
LAB_00593047:
    pcVar20 = *(code **)(*param_1 + 0x18);
    goto LAB_0059304c;
  }
  local_79 = '\0';
LAB_0059306f:
  if (((iVar8 != 0) && (*(char *)(iVar8 + 0x137) != '\0')) && (iVar8 = param_1[0xd40], iVar8 != 0))
  {
    local_68 = (char *)FUN_005f2660(*(undefined4 *)(iVar8 + 0x33a0),iVar8 + 0x334c);
    if (local_68 != (char *)0x0) goto code_r0x005954d0;
    iVar8 = param_1[0xd40];
    local_68 = (char *)FUN_005f7a10(*piVar19,iVar8 + 0x334c,*(undefined4 *)param_1[0x67],
                                    iVar8 + 0x3354,iVar8 + 0x335c,1);
    if (local_68 == (char *)0x0) {
      FUN_005f2d70(param_1[0xd40] + 0x3354);
      goto LAB_00593208;
    }
    if (local_68 != (char *)0x757e) goto code_r0x005954d0;
    pcVar9 = "Restart type 005\n";
    local_68 = (char *)0x757e;
LAB_00593195:
    FUN_00566b20(pcVar9);
    FUN_00582fd0();
    goto LAB_005931a4;
  }
LAB_00593208:
  local_78 = local_78 + 0x153;
  local_81 = '\x01';
  if (param_1[0x33] == 0) {
    iVar8 = *(int *)(param_1[0x32] + 0x24);
  }
  else {
    iVar8 = *(int *)(param_1[0x33] + 0x10);
  }
  if (iVar8 == 2) {
    local_81 = '\0';
    if (*(int *)param_1[0x67] != 0) {
      local_68 = (char *)FUN_0058f410(1);
      if (local_68 != (char *)0x0) goto code_r0x005954d0;
      if (param_1[0xc9c] != 200) {
        FUN_0056fc90(param_1[0xc96]);
        FUN_005954d0(uVar7);
        return;
      }
      param_1[0x29] = 0x10;
      local_68 = (char *)0x0;
    }
    pcVar9 = (char *)FUN_005f2660(param_1[0xce8],param_1 + 0xcd3);
    local_68 = pcVar9;
    if (pcVar9 == (char *)0x0) {
      if ((undefined4 *)param_1[0x33] == (undefined4 *)0x0) {
        uVar10 = *(undefined4 *)param_1[0x32];
      }
      else {
        uVar10 = *(undefined4 *)param_1[0x33];
      }
      pcVar9 = (char *)FUN_005f7a10(param_1[0xc80],param_1 + 0xcd3,uVar10,param_1 + 0xcd5,
                                    param_1 + 0xcd7,1);
    }
    else if (((param_1[0x67] == 0) || (*(char *)(param_1[0x67] + 0x137) == '\0')) ||
            ((param_1[0xd40] == 0 || (pcVar9 != (char *)0x757e)))) {
      FUN_005954d0(uVar7);
      return;
    }
    local_68 = pcVar9;
    if (pcVar9 == (char *)0x0) {
      FUN_005f2d70(param_1 + 0xcd5);
      goto LAB_005933e9;
    }
    FUN_00582fd0();
    if (((*(char *)((int)param_1 + 0xe3) == '\0') && (*(int *)(param_1[0x32] + 0x188) == 5)) &&
       (pcVar9 == (char *)0x80090304)) {
      iVar8 = FUN_00567aa0();
      if (iVar8 == 0) {
        if (*(int *)(param_1[0x32] + 0x180) == 0) {
          FUN_005954d0(uVar7);
          return;
        }
        FUN_00566b20("Cancel HTTP2\n");
        *(undefined4 *)(param_1[0x32] + 0x180) = 0;
      }
      else {
        FUN_00566b20("Try next server\n");
        *(undefined4 *)(param_1[0x32] + 0x188) = 0;
        *(undefined4 *)(param_1[0x32] + 0x180) = 0;
        *(undefined4 *)(param_1[0x32] + 0x184) = 0;
      }
    }
    else if (pcVar9 != (char *)0x757e) {
      FUN_005954d0(uVar7);
      return;
    }
    FUN_00566b20("Restart type 006\n");
    local_68 = (char *)(**(code **)(*param_1 + 0x18))(param_1 + 0xc80);
  }
  else {
LAB_005933e9:
    if (param_2 == 0) {
      local_68 = (char *)(**(code **)(*param_1 + 0x20))(0);
    }
    else {
      local_68 = (char *)FUN_00580390();
    }
    if (local_68 != (char *)0x0) goto code_r0x005954d0;
    pcVar9 = (char *)FUN_0058f050(param_2);
    local_68 = pcVar9;
    if ((pcVar9 + -0x2714 < (char *)0x6d) && (iVar8 = FUN_005ef720(), iVar8 != 0)) {
      FUN_00582fd0();
      FUN_00566b20("Restart type s025, l3634\n");
LAB_00593443:
      local_68 = (char *)(**(code **)(*param_1 + 0x18))(param_1 + 0xc80);
      if (local_68 == (char *)0x0) {
        FUN_00592d50(param_2);
        FUN_005954d0(iVar17);
        return;
      }
      goto code_r0x005954d0;
    }
    local_98 = local_98 + 0x194;
    if (pcVar9 == (char *)0x757e) {
      if (param_1[0x33] == 0) {
        iVar8 = *(int *)(param_1[0x32] + 0x24);
      }
      else {
        iVar8 = *(int *)(param_1[0x33] + 0x10);
      }
      if (iVar8 != 2) goto LAB_005935c0;
      pbVar12 = (byte *)param_1[0xc92];
      pbVar11 = pbVar12;
      do {
        bVar2 = *pbVar11;
        pbVar11 = pbVar11 + 1;
      } while (bVar2 != 0);
      if (pbVar11 == pbVar12 + 1) {
LAB_005934ee:
        if (local_81 == '\0') goto LAB_005935c0;
      }
      else {
        pbVar11 = *(byte **)param_1[0x32];
        do {
          bVar2 = *pbVar12;
          bVar23 = bVar2 < *pbVar11;
          if (bVar2 != *pbVar11) {
LAB_005934e5:
            iVar8 = (1 - (uint)bVar23) - (uint)(bVar23 != 0);
            goto LAB_005934ea;
          }
          if (bVar2 == 0) break;
          bVar2 = pbVar12[1];
          bVar23 = bVar2 < pbVar11[1];
          if (bVar2 != pbVar11[1]) goto LAB_005934e5;
          pbVar12 = pbVar12 + 2;
          pbVar11 = pbVar11 + 2;
        } while (bVar2 != 0);
        iVar8 = 0;
LAB_005934ea:
        if (iVar8 == 0) goto LAB_005934ee;
      }
      FUN_00566b20("Restart type 007\n");
      FUN_00582fd0();
      goto LAB_005931a4;
    }
    if (pcVar9 == (char *)0x7586) {
      if (param_1[0x33] == 0) {
        iVar8 = *(int *)(param_1[0x32] + 0x24);
      }
      else {
        iVar8 = *(int *)(param_1[0x33] + 0x10);
      }
      if (iVar8 == 2) {
        FUN_00566b20("Restart type 007b\n");
        FUN_00582fd0();
        goto LAB_00593443;
      }
LAB_005935c0:
      bVar23 = false;
    }
    else {
      if ((pcVar9 != (char *)0x0) || (param_1[0xc9c] != 0xce)) goto LAB_005935c0;
      pcVar16 = (char *)param_1[0xc9f];
      pcVar14 = pcVar16 + 1;
      do {
        cVar6 = *pcVar16;
        pcVar16 = pcVar16 + 1;
      } while (cVar6 != '\0');
      if (pcVar16 == pcVar14) goto LAB_005935c0;
      puVar13 = (undefined4 *)param_1[0x33];
      if (puVar13 == (undefined4 *)0x0) {
        puVar13 = (undefined4 *)FUN_004030a0(*(undefined4 *)param_1[0x32]);
        local_58 = CONCAT31(local_58._1_3_,1);
        local_9c = local_9c | 1;
      }
      pcVar14 = _strstr((char *)*puVar13,"sciencedirect.com");
      if ((pcVar14 == (char *)0x0) || (param_1[0xc7f] == 2)) goto LAB_005935c0;
      bVar23 = true;
    }
    local_58 = 0;
    if ((local_9c & 1) != 0) {
      local_9c = local_9c & 0xfffffffe;
      FUN_00402560();
    }
    if (bVar23) {
      if ((int *)param_1[0xc82] != (int *)0x0) {
        (**(code **)(*(int *)param_1[0xc82] + 0xc))(2);
      }
      param_1[0xc7f] = 2;
      FUN_00582fd0();
      FUN_00566b20("Restart type 11\n");
      goto LAB_005931a4;
    }
    if ((((param_1[0xc9c] != 200) || (param_1[0x16] != 0 || param_1[0x17] != 0)) ||
        (0 < param_1[0x19])) ||
       (((-1 < param_1[0x19] && (param_1[0x18] != 0)) || (*(char *)((int)param_1 + 0x320d) != '\0'))
       )) {
LAB_0059376f:
      if (pcVar9 != (char *)0x0) {
        if (pcVar9 == (char *)0x7563) {
          FUN_005954d0(uVar7);
          return;
        }
        if (pcVar9 == (char *)0x7581) {
          FUN_005954d0(uVar7);
          return;
        }
        goto LAB_00593693;
      }
LAB_005940a7:
      if (param_1[0xc8d] != 0) {
        if (*(char **)(param_1[0x32] + 0xbc) == (char *)0x0) {
          if (pcVar9 == (char *)0x0) goto LAB_005949f7;
          bVar23 = false;
        }
        else {
          iVar8 = __stricmp(*(char **)(param_1[0x32] + 0xbc),"text/html");
          bVar23 = iVar8 == 0;
        }
        cVar6 = FUN_005b0600();
        if ((((cVar6 == '\0') && (iVar8 = param_1[0x32], *(char *)(iVar8 + 0x367) != '\0')) &&
            (((param_1[0xc82] == 0 || (*(int *)(param_1[0xc82] + 0x188) == 0)) &&
             ((*(char *)(iVar8 + 0x368) == '\0' && (*(char *)(iVar8 + 0x369) == '\0')))))) &&
           ((bVar23 || (pcVar9 != (char *)0x0)))) {
          FUN_004024f0();
          local_58._0_1_ = 8;
          FUN_004024f0();
          local_58._0_1_ = 9;
          FUN_004024f0();
          local_58._0_1_ = 10;
          FUN_004024f0();
          local_58._0_1_ = 0xb;
          FUN_004024f0();
          local_54 = (short *)0x0;
          local_4c = 0;
          local_50[0] = 0;
          local_58._0_1_ = 0xd;
          cVar6 = FUN_005b07c0(&local_80,&local_90,&local_94,&local_88,&local_a0,&local_74,&local_6c
                               ,&local_54);
          if (cVar6 != '\0') {
            *(undefined1 *)(param_1[0x32] + 0x368) = 1;
            *(undefined1 *)(param_1[0x32] + 0x366) = 1;
            *(undefined1 *)(param_1[0x32] + 0x369) = 1;
            FUN_005b07b0();
            *(undefined4 *)(param_1[0x32] + 0x24) = local_a0;
            *(char **)(param_1[0x32] + 0x14) = local_88;
            iVar17 = FUN_004546b0(local_80);
            if (((iVar17 == 0) && (iVar17 = FUN_00454720(local_90), iVar17 == 0)) &&
               (iVar17 = FUN_00425d10(local_94,0), iVar17 == 0)) {
              FUN_00454630(local_6c);
              FUN_00402f80(&DAT_006979dc);
              *(undefined1 *)(param_1[0x32] + 0x355) = 0;
              FUN_004545b0(&DAT_006979dc);
              if ((param_1[0xc82] != 0) && (iVar17 = *(int *)(param_1[0xc82] + 0xb8), iVar17 != 0))
              {
                if (*(int *)(param_1[0x32] + 0x4a4) == 0) {
                  FUN_005d6890(iVar17);
                }
                if (*(HKEY *)(param_1[0x32] + 0x4a4) != (HKEY)0x0) {
                  RegDeleteValueA(*(HKEY *)(param_1[0x32] + 0x4a4),"Referer");
                }
              }
              psVar18 = local_54;
              if (local_54 == (short *)0x0) {
                psVar18 = local_50;
              }
              psVar1 = psVar18 + 1;
              do {
                sVar3 = *psVar18;
                psVar18 = psVar18 + 1;
              } while (sVar3 != 0);
              pcVar20 = SendMessageA_exref;
              if ((int)psVar18 - (int)psVar1 >> 1 != 0) {
                psVar18 = local_54;
                if (local_54 == (short *)0x0) {
                  psVar18 = local_50;
                }
                FUN_005d5870(psVar18);
                pcVar20 = SendMessageA_exref;
                if (param_1[0xc82] != 0) {
                  iVar17 = *(int *)(param_1[0xc82] + 0xb8);
                  if (iVar17 != 0) {
                    FUN_005d75a0(iVar17);
                  }
                  pcVar20 = SendMessageA_exref;
                  puVar13 = *(undefined4 **)(param_1[0xc82] + 0x14);
                  if (puVar13 != (undefined4 *)0x0) {
                    SendMessageA((HWND)*puVar13,0x111,0x13c9,*(LPARAM *)(param_1[0xc82] + 0xb8));
                  }
                }
              }
              *(undefined1 *)(param_1[0x32] + 0x1be) = 1;
              FUN_00402f80(&DAT_006979dc);
              *(undefined1 *)(param_1[0x32] + 0x1a5) = 0;
              *(undefined1 *)(param_1[0x32] + 0x35b) = 0;
              *(undefined1 *)(param_1 + 0x38) = 0;
              FUN_00514a50(param_1[0x32]);
              FUN_00583340();
              if (param_1[0x31] == 0) {
                if (**(int **)param_1[0x2e] != 0) {
                  (*pcVar20)(**(int **)param_1[0x2e],0x111,0x1478,0);
                }
                if ((param_1[0xc82] != 0) && (iVar17 = *(int *)(param_1[0xc82] + 0xb8), iVar17 != 0)
                   ) {
                  FUN_005d9c60(iVar17,0,0);
                  puVar13 = *(undefined4 **)(param_1[0xc82] + 0x14);
                  if (puVar13 != (undefined4 *)0x0) {
                    (*pcVar20)(*puVar13,0x111,0x1395,*(undefined4 *)(param_1[0xc82] + 0xb8));
                  }
                  puVar13 = *(undefined4 **)(param_1[0xc82] + 0x18);
                  if (puVar13 != (undefined4 *)0x0) {
                    (*pcVar20)(*puVar13,0x111,0x1395,*(undefined4 *)(param_1[0xc82] + 0xb8));
                  }
                }
              }
              if (param_1[0xc7e] != 0) {
                local_58 = CONCAT31(local_58._1_3_,0xe);
                FUN_00610e74(param_1[0xc7e]);
                local_58 = 0xd;
                FUN_00594893();
                return;
              }
              param_1[0xc7e] = 0;
              FUN_005d30c0();
              FUN_00582fd0();
              FUN_00566b20("Restart type 17\n");
              iVar17 = (**(code **)(*param_1 + 0x18))(param_1 + 0xc80);
              if (((((iVar17 == 0) && (iVar17 = FUN_00592d50(param_2), iVar17 == 0)) &&
                   (*(char **)(param_1[0x32] + 0xbc) != (char *)0x0)) &&
                  ((iVar17 = __stricmp(*(char **)(param_1[0x32] + 0xbc),"text/html"), iVar17 != 0 &&
                   (param_1[0xc82] != 0)))) &&
                 (iVar17 = *(int *)(param_1[0xc82] + 0xb8), iVar17 != 0)) {
                if (*(int *)(param_1[0x32] + 0x4a4) == 0) {
                  FUN_005d6890(iVar17);
                }
                if (*(int *)(param_1[0x32] + 0x4a4) != 0) {
                  local_98 = 1;
                  RegSetValueExA(*(HKEY *)(param_1[0x32] + 0x4a4),"noSyntRef",0,4,(BYTE *)&local_98,
                                 4);
                }
              }
            }
            local_58._0_1_ = 0xc;
            FUN_004011f0();
            local_58._0_1_ = 0xb;
            FUN_00402560();
            local_58._0_1_ = 10;
            FUN_00402560();
            local_58._0_1_ = 9;
            FUN_00402560();
            local_58._0_1_ = 8;
            FUN_00402560();
            local_58 = (uint)local_58._1_3_ << 8;
            FUN_00402560();
            FUN_005954d0(uVar7);
            return;
          }
          local_58._0_1_ = 0xc;
          FUN_004011f0();
          local_58._0_1_ = 0xb;
          FUN_00402560();
          local_58._0_1_ = 10;
          FUN_00402560();
          local_58._0_1_ = 9;
          FUN_00402560();
          local_58._0_1_ = 8;
          FUN_00402560();
          local_58 = (uint)local_58._1_3_ << 8;
          FUN_00402560();
        }
      }
LAB_005949f7:
      if ((param_1[0xc9c] == 200) && (param_1[0x16] == 0 && param_1[0x17] == 0)) {
        param_1[0x16] = param_1[0xc88];
        param_1[0x17] = param_1[0xc89];
      }
      if (param_1[0xc9c] == 0xce) {
        if (((param_1[0x16] == 0 && param_1[0x17] == 0) && (param_1[0x19] < 1)) &&
           ((param_1[0x19] < 0 || (param_1[0x18] == 0)))) {
          if ((param_1[0xc88] != 0 || param_1[0xc89] != 0) &&
             (param_1[0xca4] == 0 && param_1[0xca5] == 0)) {
            param_1[0x16] = param_1[0xc88];
            param_1[0x17] = param_1[0xc89];
          }
        }
        if (param_1[0x16] == 0 && param_1[0x17] == 0) {
          iVar8 = param_1[0x19];
          if (iVar8 < 1) {
            if ((iVar8 < 0) || (param_1[0x18] == 0)) {
              iVar22 = param_1[0xca5];
              if (param_1[0xca4] != 0 || iVar22 != 0) {
                iVar4 = param_1[0xc89];
                if ((iVar22 <= iVar4) &&
                   ((iVar22 < iVar4 || ((uint)param_1[0xca4] < (uint)param_1[0xc88])))) {
                  param_1[0x16] = param_1[0xc88];
                  param_1[0x17] = iVar4;
                  goto LAB_00594b13;
                }
              }
            }
            if ((iVar8 < 1) && ((iVar8 < 0 || (param_1[0x18] == 0)))) {
              uVar21 = param_1[0xc88];
              iVar8 = param_1[0xc89];
              if ((uVar21 != 0 || iVar8 != 0) &&
                 ((param_1[0xca4] == uVar21 + 1 &&
                  (param_1[0xca5] == iVar8 + (uint)(0xfffffffe < uVar21))))) {
                param_1[0x16] = uVar21;
                param_1[0x17] = iVar8;
                goto LAB_00594b13;
              }
            }
          }
          param_1[0x16] = param_1[0xca4];
          param_1[0x17] = param_1[0xca5];
        }
      }
LAB_00594b13:
      iVar8 = param_1[0xc9c];
      if (((iVar8 == 200) || (iVar8 == 0xce)) && (param_1[0x16] == 0 && param_1[0x17] == 0)) {
        iVar22 = param_1[0xcab];
        if ((-1 < iVar22) && ((0 < iVar22 || (param_1[0xcaa] != 0)))) {
          param_1[0x16] = param_1[0xcaa];
          param_1[0x17] = iVar22;
        }
      }
      iVar22 = param_1[0x32];
      if (((*(int *)(iVar22 + 0x348) != 0) && (param_1[0xc82] != 0)) &&
         (*(int *)(param_1[0xc82] + 0x40) != 0)) {
        if (iVar8 == 200) {
          iVar8 = param_1[0xc88];
          iVar4 = param_1[0xc89];
          if ((-1 < iVar4) && ((0 < iVar4 || (iVar8 != 0)))) {
            param_1[0x1e] = iVar8 + -1;
            param_1[0x1f] = iVar4 - (uint)(iVar8 == 0);
          }
        }
        param_1[0x16] = 0;
        param_1[0x17] = 0;
      }
      if (param_1[0x16] != 0 || param_1[0x17] != 0) {
        iVar8 = param_1[0xc82];
        iVar22 = *(int *)(iVar8 + 0xd0);
        iVar4 = *(int *)(iVar8 + 0xd4);
        if (((iVar22 == 0 && iVar4 == 0) ||
            (((iVar22 == param_1[0x16] && (iVar4 == param_1[0x17])) ||
             (*(int *)(iVar8 + 200) == 0 && *(int *)(iVar8 + 0xcc) == 0)))) ||
           ((char)param_1[0xc9e] != '\0')) {
LAB_00594c83:
          iVar22 = param_1[0x32];
        }
        else {
          if (param_1[0xc9c] == 0xce) {
            if (((param_1[0xca4] == iVar22 + -1) && (param_1[0xca5] == iVar4 - (uint)(iVar22 == 0)))
               && (param_1[0xca6] == 0 && param_1[0xca7] == 0)) {
              param_1[0xca6] = param_1[0xca4];
              param_1[0xca7] = param_1[0xca5];
            }
          }
          iVar22 = param_1[0x32];
          if (param_1[0xca6] != 0 || param_1[0xca7] != 0) {
            iVar22 = *(int *)(iVar8 + 0xd0);
            local_88 = *(char **)(iVar8 + 0xd4);
            if ((param_1[0xca6] == iVar22 + -1) &&
               (param_1[0xca7] == (int)local_88 - (uint)(iVar22 == 0))) {
              param_1[0x16] = iVar22;
              param_1[0x17] = (int)local_88;
              goto LAB_00594c83;
            }
            iVar22 = param_1[0x32];
          }
          if ((param_1[0xc9c] == 0xce) &&
             (uVar21 = param_1[0xcac], uVar21 != 0 || param_1[0xcad] != 0)) {
            uVar5 = *(uint *)(iVar8 + 0xd0);
            local_88 = *(char **)(iVar8 + 0xd4);
            if ((param_1[0xc88] == uVar5 - uVar21) &&
               ((char *)param_1[0xc89] == local_88 + (-(uint)(uVar5 < uVar21) - param_1[0xcad]))) {
              param_1[0x16] = uVar5;
              param_1[0x17] = (int)local_88;
              goto LAB_00594c83;
            }
            iVar22 = param_1[0x32];
          }
          if ((*(int *)(iVar8 + 0x148) == 0) ||
             ((*(char *)(iVar22 + 0x36b) == '\0' && (*(char *)(iVar22 + 0x19a) == '\0')))) {
            if (*(char *)((int)param_1 + 0x3286) != '\0') {
              iVar4 = *(int *)(iVar8 + 0xd4);
              if (((param_1[0xca4] == *(int *)(iVar8 + 0xd0)) && (param_1[0xca5] == iVar4)) &&
                 (param_1[0xc9c] == 0xce)) {
                param_1[0x16] = *(int *)(iVar8 + 0xd0);
                param_1[0x17] = iVar4;
                goto LAB_00594c89;
              }
            }
            iVar8 = param_1[0x19];
            if (iVar8 < 1) {
              if (((iVar8 < 0) || (param_1[0x18] == 0)) &&
                 ((*(char *)((int)param_1 + 0x320d) == '\0' &&
                  ((*(char *)(iVar22 + 0x366) != '\0' && (*(char *)(iVar22 + 0x368) == '\0')))))) {
                *(undefined1 *)(param_1 + 0xc83) = 1;
                *(undefined1 *)((int)param_1 + 0x320d) = 1;
                if (*(char *)(iVar22 + 0x366) != '\0') {
                  *(undefined1 *)(iVar22 + 0x368) = 1;
                }
                FUN_00566b20("Restart type 21a\n");
                FUN_00582fd0();
                goto LAB_00593047;
              }
              if ((iVar8 < 0) || ((iVar8 < 1 && (param_1[0x18] == 0)))) goto LAB_005950b4;
            }
            if (((char)param_1[0xc86] != '\0') || (*(char *)((int)param_1 + 0x3286) != '\0'))
            goto LAB_005950b4;
            *(undefined1 *)((int)param_1 + 0x3286) = 1;
            *(undefined4 *)(iVar22 + 800) = 1;
            if (*(char *)(param_1[0x32] + 0x366) != '\0') {
              *(undefined1 *)(param_1[0x32] + 0x368) = 1;
            }
            FUN_00566b20("Restart type 21b\n");
            FUN_00582fd0();
            pcVar20 = *(code **)(*param_1 + 0x18);
LAB_0059304c:
            local_68 = (char *)(*pcVar20)(param_1 + 0xc80);
            if (local_68 == (char *)0x0) {
              FUN_00592d50(param_2);
              FUN_005954d0(iVar17);
              return;
            }
            goto code_r0x005954d0;
          }
          *(undefined1 *)(param_1 + 0xc9e) = 1;
        }
      }
LAB_00594c89:
      if ((char)param_1[0xc9e] == '\0') {
        pcVar9 = *(char **)(iVar22 + 0xbc);
        if (pcVar9 != (char *)0x0) {
          local_88 = *(char **)(iVar22 + 0x120);
          pcVar14 = local_88;
          do {
            cVar6 = *pcVar14;
            pcVar14 = pcVar14 + 1;
          } while (cVar6 != '\0');
          if (pcVar14 != local_88 + 1) {
            pcVar14 = pcVar9 + 1;
            do {
              cVar6 = *pcVar9;
              pcVar9 = pcVar9 + 1;
            } while (cVar6 != '\0');
            if (pcVar9 != pcVar14) {
              local_a4 = 1;
              pcVar9 = _strchr(local_88,0x3b);
              if ((pcVar9 != (char *)0x0) &&
                 (pcVar9 = _strchr(*(char **)(param_1[0x32] + 0xbc),0x3b), pcVar9 == (char *)0x0)) {
                pcVar9 = *(char **)(param_1[0x32] + 0xbc);
                local_88 = pcVar9 + 1;
                pcVar14 = pcVar9;
                do {
                  cVar6 = *pcVar14;
                  pcVar14 = pcVar14 + 1;
                } while (cVar6 != '\0');
                iVar17 = __strnicmp(*(char **)(param_1[0x32] + 0x120),pcVar9,
                                    (int)pcVar14 - (int)local_88);
                if (iVar17 == 0) {
                  local_a4 = 0;
                }
              }
              pcVar9 = *(char **)(param_1[0x32] + 0x120);
              local_88 = pcVar9 + 1;
              pcVar14 = pcVar9;
              do {
                cVar6 = *pcVar14;
                pcVar14 = pcVar14 + 1;
              } while (cVar6 != '\0');
              iVar17 = __strnicmp(pcVar9,*(char **)(param_1[0x32] + 0xbc),
                                  (int)pcVar14 - (int)local_88);
              if ((iVar17 != 0) && (local_a4 != 0)) {
                local_6d = '\x01';
                iVar17 = __memicmp(*(void **)(param_1[0x32] + 0x120),"text/html",9);
                if ((iVar17 == 0) &&
                   (iVar17 = __memicmp(*(void **)(param_1[0x32] + 0xbc),"text/html",9), iVar17 != 0)
                   ) {
                  local_6d = '\0';
                }
                iVar17 = __memicmp(*(void **)(param_1[0x32] + 0x120),"text/plain",10);
                if ((iVar17 == 0) &&
                   (iVar17 = __memicmp(*(void **)(param_1[0x32] + 0xbc),"text/html",9), iVar17 != 0)
                   ) {
                  local_6d = '\0';
                }
                iVar17 = __strnicmp(*(char **)(param_1[0x32] + 0x120),"application/x-mpegURL",0x15);
                if ((iVar17 == 0) &&
                   ((iVar17 = __strnicmp(*(char **)(param_1[0x32] + 0xbc),
                                         "application/vnd.apple.mpegurl",0x1d), iVar17 == 0 ||
                    (iVar17 = __stricmp(*(char **)(param_1[0x32] + 0xbc),"video/MP2T"), iVar17 == 0)
                    ))) {
                  local_6d = '\0';
                  FUN_00402f80(&DAT_006979dc);
                }
                iVar17 = __memicmp(*(void **)(param_1[0x32] + 0xbc),"text/html",9);
                if ((iVar17 != 0) &&
                   (((pcVar9 = _strstr(*(char **)param_1[0x32],".googlevideo.com"),
                     pcVar9 != (char *)0x0 ||
                     (pcVar9 = _strstr(*(char **)param_1[0x32],".drive.google.com"),
                     pcVar9 != (char *)0x0)) &&
                    (piVar19 = *(int **)(param_1[0x32] + 8), piVar19 != (int *)0x0)))) {
                  pcVar9 = "videoplayback?";
                  for (uVar21 = 0xe; 3 < uVar21; uVar21 = uVar21 - 4) {
                    if (*piVar19 != *(int *)pcVar9) goto LAB_00595154;
                    pcVar9 = (char *)((int)pcVar9 + 4);
                    piVar19 = piVar19 + 1;
                  }
                  if ((((char)*(int *)pcVar9 == (char)*piVar19) &&
                      (*(char *)((int)pcVar9 + 1) == *(char *)((int)piVar19 + 1))) &&
                     ((iVar17 = __memicmp(*(void **)(param_1[0x32] + 0xbc),"video/",6), iVar17 != 0
                      || ((iVar17 = __memicmp(*(void **)(param_1[0x32] + 0xbc),"audio/",6),
                          iVar17 != 0 ||
                          (iVar17 = __stricmp(*(char **)(param_1[0x32] + 0xbc),
                                              "application/octet-stream"), iVar17 == 0)))))) {
                    local_6d = '\0';
                    FUN_00402f80(&DAT_006979dc);
                    goto LAB_005951f1;
                  }
                }
LAB_00595154:
                if (local_6d != '\0') {
                  iVar17 = param_1[0xc82];
                  if ((*(int *)(iVar17 + 0x148) == 0) ||
                     ((*(char *)(param_1[0x32] + 0x36b) == '\0' &&
                      (*(char *)(param_1[0x32] + 0x19a) == '\0')))) {
                    iVar17 = __memicmp(*(void **)(param_1[0x32] + 0xbc),"text/html",9);
                    if (iVar17 == 0) {
LAB_005950b4:
                      FUN_00582fd0();
                      FUN_005954d0(uVar7);
                      return;
                    }
                    if (((param_1[0x16] == 0 && param_1[0x17] == 0) ||
                        (param_1[0x16] != *(int *)(param_1[0x32] + 0x140))) ||
                       (param_1[0x17] != *(int *)(param_1[0x32] + 0x144))) goto LAB_005950b4;
                    local_6d = '\0';
                  }
                  else if (*(int *)(iVar17 + 200) != 0 || *(int *)(iVar17 + 0xcc) != 0) {
                    *(undefined1 *)(param_1 + 0xc9e) = 1;
                  }
                }
              }
            }
          }
        }
LAB_005951f1:
        if ((char)param_1[0xc9e] == '\0') {
          pcVar9 = (char *)param_1[0xc90];
          pcVar14 = pcVar9;
          do {
            cVar6 = *pcVar14;
            pcVar14 = pcVar14 + 1;
          } while (cVar6 != '\0');
          if (pcVar14 != pcVar9 + 1) {
            FUN_005811d0(pcVar9);
          }
        }
      }
      FUN_00402f80(&DAT_006979dc);
      if ((char)param_1[0xc9e] == '\0') {
        FUN_00640553(&local_b8);
        _sprintf(local_48,"Time: %ld.%ld\r\n",local_b8,(uint)local_b4);
        FUN_00566b20(local_48);
        param_1[0x29] = 6;
        param_1[0x30] = 0;
        FUN_00567380(DAT_00789c74,1);
        if (param_1[0x31] == 0) {
          puVar13 = *(undefined4 **)(param_1[0xc82] + 0x18);
          if (puVar13 != (undefined4 *)0x0) {
            SendMessageA((HWND)*puVar13,0x111,local_78,*(LPARAM *)(param_1[0xc82] + 0xb8));
          }
        }
        if (0 < *(int *)(param_1[0x32] + 0x3bc)) {
          FUN_0043b730(local_8c);
          iVar17 = ATL::CTime::GetYear(local_8c);
          iVar8 = ATL::CTime::GetMonth(local_8c);
          if ((*(int *)(param_1[0x32] + 0x3bc) <= (iVar17 + -0x76c) * 0xc + -1 + iVar8) &&
             (DAT_007893d0 != (HWND)0x0)) {
            PostMessageA(DAT_007893d0,0x111,local_98,*(int *)(param_1[0x32] + 0x3bc));
          }
        }
        if (((local_79 != '\0') && (*(int *)(param_1[0x67] + 0x154) == 4)) &&
           (*(char *)(param_1[0x67] + 0x15e) != '\0')) {
          local_88 = (char *)0x4;
          RegSetValueExA(DAT_00788d08,"SocksType",0,4,(BYTE *)&local_88,4);
        }
        FUN_005954d0(uVar7);
        return;
      }
      *(undefined1 *)(param_1 + 0xc9e) = 0;
      FUN_0056fd80();
      local_79 = *(int *)(param_1[0xc82] + 0xb0) != 0;
      FUN_0055f780();
      iVar17 = param_1[0xc82];
      *(undefined4 *)(iVar17 + 200) = 0;
      *(undefined4 *)(iVar17 + 0xcc) = 0;
      iVar17 = param_1[0xc82];
      *(undefined4 *)(iVar17 + 0xd0) = 0;
      *(undefined4 *)(iVar17 + 0xd4) = 0;
      iVar17 = param_1[0xc82];
      *(undefined4 *)(iVar17 + 0x1c8) = 0;
      *(undefined4 *)(iVar17 + 0x1cc) = 0;
      param_1[0xc9d] = 0;
      param_1[0x2a] = 0;
      param_1[0x18] = 0;
      param_1[0x19] = 0;
      param_1[0x1e] = 0;
      param_1[0x1f] = 0;
      param_1[0x20] = 0;
      param_1[0x21] = 0;
      param_1[0x22] = 0;
      param_1[0x23] = 0;
      param_1[0x1c] = 0;
      param_1[0x1d] = 0;
      param_1[0xc88] = 0;
      param_1[0xc89] = 0;
      param_1[0xca2] = 0;
      param_1[0xca3] = 0;
      param_1[0xca4] = 0;
      param_1[0xca5] = 0;
      param_1[0xca6] = 0;
      param_1[0xca7] = 0;
      param_1[0x16] = 0;
      param_1[0x17] = 0;
      param_1[0xcaa] = 0;
      param_1[0xcab] = 0;
      if (param_1[0xc82] != 0) {
        *(undefined1 *)(param_1[0xc82] + 0x889) = 0;
      }
      FUN_00566b20("Restart type 008\n");
      FUN_00582fd0();
      local_68 = (char *)(**(code **)(*param_1 + 0x18))(param_1 + 0xc80);
      if (local_68 == (char *)0x0) {
        local_68 = (char *)FUN_00592d50(param_2);
        if (local_68 == (char *)0x0) {
          pcVar9 = (char *)0;
          if (local_79 != '\0') {
            pcVar9 = (char *)FUN_0055f910(param_1,*(undefined4 *)(param_1[0x32] + 0x30));
          }
          local_68 = pcVar9;
          FUN_00566e60();
          FUN_005954d0(uVar7);
          return;
        }
LAB_00592ef0:
        FUN_005954d0(uVar7);
        return;
      }
      goto code_r0x005954d0;
    }
    uVar21 = param_1[0xca4];
    if (((uVar21 == 0 && param_1[0xca5] == 0) || (uVar21 + 1 != param_1[0xc88])) ||
       (param_1[0xca5] + (uint)(0xfffffffe < uVar21) != param_1[0xc89])) goto LAB_0059376f;
    local_68 = (char *)0x7542;
LAB_00593693:
    puVar13 = (undefined4 *)param_1[0x32];
    if ((((puVar13[0xcf] == 0x40) || (puVar13[0xcf] == 0x80)) &&
        (iVar8 = param_1[0xc82], iVar8 != 0)) &&
       ((*(char *)(iVar8 + 0x259) != '\0' && (*(char *)(iVar8 + 0x25a) == '\0')))) {
      *(undefined1 *)(iVar8 + 0x25a) = 1;
      pcVar9 = _strstr(*(char **)(param_1[0x32] + 8),"-tmp_dashinit");
      if (pcVar9 != (char *)0x0) {
        FUN_004024f0();
        local_58._0_1_ = 2;
        FUN_00402f20(&local_78,&DAT_00696f08,*(undefined4 *)(param_1[0x32] + 8));
        FUN_00402c00("-tmp_dashinit",&DAT_006979dc);
        FUN_00425d10(local_78,1);
        local_58 = (uint)local_58._1_3_ << 8;
        FUN_00402560();
      }
      FUN_00582fd0();
      FUN_00566b20("Restart type s020s1\n");
    }
    else if ((((*(char *)(puVar13 + 0x68) == '\0') && (-1 < (int)puVar13[199])) &&
             ((0 < (int)puVar13[199] || (puVar13[0xc6] != 0)))) && ((puVar13[0x59] & 0x100) != 0)) {
      *(undefined1 *)(puVar13 + 0x68) = 1;
      FUN_00582fd0();
      FUN_00566b20("Restart type s021s2\n");
    }
    else {
      if (((*(char *)((int)puVar13 + 0x1c2) == '\0') || (*(char *)((int)puVar13 + 0x1c1) != '\0'))
         || ((puVar13[0x59] & 0x100) == 0)) {
        if ((((puVar13[0xd2] == 0) && ((puVar13[0x59] & 0x100) != 0)) &&
            ((local_68 != (char *)0x0 &&
             ((pcVar9 = _strstr((char *)*puVar13,".googlevideo.com"), pcVar9 != (char *)0x0 ||
              (pcVar9 = _strstr(*(char **)param_1[0x32],".drive.google.com"), pcVar9 != (char *)0x0)
              ))))) && (piVar19 = *(int **)(param_1[0x32] + 8), piVar19 != (int *)0x0)) {
          pcVar9 = "videoplayback?";
          for (uVar21 = 0xe; 3 < uVar21; uVar21 = uVar21 - 4) {
            if (*piVar19 != *(int *)pcVar9) goto LAB_00593a56;
            pcVar9 = (char *)((int)pcVar9 + 4);
            piVar19 = piVar19 + 1;
          }
          if ((((char)*(int *)pcVar9 == (char)*piVar19) &&
              (*(char *)((int)pcVar9 + 1) == *(char *)((int)piVar19 + 1))) &&
             (((pcVar9 = _strstr(*(char **)(param_1[0x32] + 8),"&source=yt_otf"),
               pcVar9 != (char *)0x0 ||
               (pcVar9 = _strstr(*(char **)(param_1[0x32] + 8),"?source=yt_otf"),
               pcVar9 != (char *)0x0)) &&
              ((((pcVar9 = _strstr(*(char **)(param_1[0x32] + 8),"&otf=1"), pcVar9 != (char *)0x0 ||
                 (pcVar9 = _strstr(*(char **)(param_1[0x32] + 8),"?otf=1"), pcVar9 != (char *)0x0))
                && (pcVar9 = _strstr(*(char **)(param_1[0x32] + 8),"&sq="), pcVar9 == (char *)0x0))
               && (pcVar9 = _strstr(*(char **)(param_1[0x32] + 8),"?sq="), pcVar9 == (char *)0x0))))
             )) {
            FUN_00582fd0();
            FUN_00566b20("Restart type s020y\n");
            *(undefined4 *)(param_1[0x32] + 0x348) = 0x200;
            *(undefined1 *)(param_1[0x32] + 0x34c) = 1;
            if ((param_1[0xc82] != 0) && (iVar8 = *(int *)(param_1[0xc82] + 0xb8), iVar8 != 0)) {
              FUN_005d75d0(iVar8);
            }
            FUN_004024f0();
            local_58._0_1_ = 3;
            FUN_00402f20(&local_78,"%s&sq=0",*(undefined4 *)(param_1[0x32] + 8));
            FUN_00425d10(local_78,0);
            piVar19 = param_1 + 0xc80;
            local_68 = (char *)(**(code **)(*param_1 + 0x18))(piVar19);
            if (local_68 == (char *)0x0) {
              FUN_00592d50(param_2);
              local_58 = (uint)local_58._1_3_ << 8;
              FUN_00402560();
              FUN_005954d0(piVar19);
              return;
            }
            local_58 = (uint)local_58._1_3_ << 8;
            FUN_00402560();
            FUN_005954d0(iVar17);
            return;
          }
        }
LAB_00593a56:
        if ((local_68 == (char *)0x7566) &&
           ((pcVar9 = _strstr(*(char **)param_1[0x32],".dailymotion.com"), pcVar9 != (char *)0x0 ||
            (pcVar9 = _strstr(*(char **)param_1[0x32],".dmcdn.net"), pcVar9 != (char *)0x0)))) {
          pcVar9 = *(char **)(param_1[0x32] + 8);
          if ((pcVar9 != (char *)0x0) &&
             ((((*(uint *)(param_1[0x32] + 0x164) & 0x100) != 0 &&
               (pcVar9 = _strrchr(pcVar9,0x2e), pcVar9 != (char *)0x0)) &&
              (iVar8 = __stricmp(pcVar9,".flv"), iVar8 == 0)))) {
            FUN_004024f0();
            local_58._0_1_ = 4;
            FUN_00402f20(&local_74,&DAT_00696f08,*(undefined4 *)(param_1[0x32] + 8));
            pcVar9 = _strrchr(local_74,0x2e);
            if (pcVar9 != (char *)0x0) {
              *pcVar9 = '\0';
              FUN_004024f0();
              local_58._0_1_ = 5;
              FUN_00402f20(&local_78,"%s.mnft",local_74);
              FUN_00425d10(local_78,0);
              FUN_00582fd0();
              FUN_00566b20("Restart type 12\n");
              piVar19 = param_1 + 0xc80;
              local_68 = (char *)(**(code **)(*param_1 + 0x18))(piVar19);
              if (local_68 == (char *)0x0) {
                FUN_00592d50(param_2);
                local_58._0_1_ = 4;
                FUN_00402560();
                local_58 = (uint)local_58._1_3_ << 8;
                FUN_00402560();
                FUN_005954d0(piVar19);
                return;
              }
              local_58._0_1_ = 4;
              FUN_00402560();
              local_58 = (uint)local_58._1_3_ << 8;
              FUN_00402560();
              FUN_005954d0(iVar17);
              return;
            }
            local_58 = (uint)local_58._1_3_ << 8;
            FUN_00402560();
          }
        }
        if ((param_1[0x19] < 1) && ((param_1[0x19] < 0 || (param_1[0x18] == 0)))) {
          iVar8 = param_1[0x32];
          pcVar14 = *(char **)(iVar8 + 0x3a4);
          pcVar9 = pcVar14 + 1;
          do {
            cVar6 = *pcVar14;
            pcVar14 = pcVar14 + 1;
          } while (cVar6 != '\0');
          if (((pcVar14 == pcVar9) || (*(char *)(iVar8 + 0x36d) != '\0')) ||
             ((local_68 != (char *)0x7565 &&
              ((((local_68 != (char *)0x7566 && (local_68 != (char *)0x7542)) &&
                (local_68 != (char *)0x756e)) && (local_68 != (char *)0x755e))))))
          goto LAB_00593d1d;
          *(undefined1 *)(iVar8 + 0x36d) = 1;
          *(undefined1 *)(param_1[0x32] + 0x375) = 1;
          if (param_1[0x33] == 0) {
            iVar8 = *(int *)(param_1[0x32] + 0x24);
          }
          else {
            iVar8 = *(int *)(param_1[0x33] + 0x10);
          }
          if (((iVar8 == 2) && (*(int *)(param_1[0x32] + 0x188) != 5)) &&
             ((local_68 == (char *)0x7566 || (local_68 == (char *)0x7542)))) {
            *(undefined4 *)(param_1[0x32] + 0x188) = 5;
            iVar8 = param_1[0x32];
            if ((*(char *)(iVar8 + 0x1b8) == '\0') || (*(int *)(iVar8 + 0x1b0) == 0)) {
              if (param_1[0x33] == 0) {
                cVar6 = *(char *)(iVar8 + 0x1a5);
              }
              else {
                cVar6 = *(char *)(param_1[0x33] + 0x14);
              }
              if (((cVar6 == '\0') && (*(int *)(iVar8 + 0x184) == 0)) &&
                 (*(char *)(iVar8 + 0x1a1) == '\0')) {
                *(undefined4 *)(iVar8 + 0x180) = 1;
                *(undefined4 *)(param_1[0x32] + 0x184) = 1;
              }
            }
          }
          FUN_00582fd0();
          FUN_00566b20("Restart, type 18\n");
          pcVar20 = *(code **)(*param_1 + 0x18);
          goto LAB_0059304c;
        }
LAB_00593d1d:
        FUN_004024f0();
        local_58._0_1_ = 6;
        FUN_00598c40(&local_6c,0);
        cVar6 = FUN_004b2420(local_6c);
        pcVar9 = local_68;
        if ((local_68 != (char *)0x7565) && (local_68 != (char *)0x7542)) goto LAB_00593ee7;
        if ((*(char *)(param_1[0x32] + 0x34f) == '\0') && (cVar6 != '\0')) {
          FUN_00582fd0();
          FUN_00566b20("Restart type 14\n");
          if (param_1[0x33] == 0) {
            iVar8 = *(int *)(param_1[0x32] + 0x24);
          }
          else {
            iVar8 = *(int *)(param_1[0x33] + 0x10);
          }
          if (((iVar8 == 2) && (*(int *)(param_1[0x32] + 0x188) != 5)) &&
             ((pcVar9 == (char *)0x7566 || (pcVar9 == (char *)0x7542)))) {
            *(undefined4 *)(param_1[0x32] + 0x188) = 5;
            iVar8 = param_1[0x32];
            if ((*(char *)(iVar8 + 0x1b8) == '\0') || (*(int *)(iVar8 + 0x1b0) == 0)) {
              if (param_1[0x33] == 0) {
                cVar6 = *(char *)(iVar8 + 0x1a5);
              }
              else {
                cVar6 = *(char *)(param_1[0x33] + 0x14);
              }
              if (((cVar6 == '\0') && (*(int *)(iVar8 + 0x184) == 0)) &&
                 (*(char *)(iVar8 + 0x1a1) == '\0')) {
                *(undefined4 *)(iVar8 + 0x180) = 1;
                *(undefined4 *)(param_1[0x32] + 0x184) = 1;
              }
            }
          }
          local_68 = (char *)(**(code **)(*param_1 + 0x18))(param_1 + 0xc80);
          if (local_68 == (char *)0x0) {
            *(undefined1 *)(param_1[0x32] + 0x34f) = 1;
            local_68 = (char *)0x0;
            goto LAB_00593e5c;
          }
          goto LAB_005944d2;
        }
        if ((local_68 == (char *)0x7542) && (param_2 == 0)) {
          if (-1 < param_1[0x19]) {
            if (((0 < param_1[0x19]) || (param_1[0x18] != 0)) &&
               (*(int *)(param_1[0x32] + 800) != 0)) {
              FUN_005d69f0(0,*(undefined4 *)(param_1[0xc82] + 0xb8));
              FUN_00582fd0();
              FUN_00566b20("Restart type 15\n");
              local_68 = (char *)(**(code **)(*param_1 + 0x18))(param_1 + 0xc80);
              if (local_68 == (char *)0x0) {
                param_2 = 0;
                local_68 = (char *)0x0;
                goto LAB_00593e5c;
              }
              goto LAB_005944d2;
            }
            goto LAB_00593ee7;
          }
LAB_00593efd:
          if (((*(char *)((int)param_1 + 0x320d) == '\0') &&
              ((iVar8 = param_1[0x32], *(char *)(iVar8 + 0x366) != '\0' &&
               (*(char *)(iVar8 + 0x368) == '\0')))) &&
             ((local_68 == (char *)0x7565 ||
              ((((local_68 == (char *)0x7566 || (local_68 == (char *)0x7542)) ||
                (local_68 == (char *)0x756e)) || (local_68 == (char *)0x755e)))))) {
            *(undefined1 *)(param_1 + 0xc83) = 1;
            *(undefined1 *)((int)param_1 + 0x320d) = 1;
            if (*(char *)(iVar8 + 0x366) != '\0') {
              *(undefined1 *)(iVar8 + 0x368) = 1;
            }
            FUN_00582fd0();
            FUN_00566b20("Restart type 16\n");
            if (param_1[0x33] == 0) {
              iVar8 = *(int *)(param_1[0x32] + 0x24);
            }
            else {
              iVar8 = *(int *)(param_1[0x33] + 0x10);
            }
            if (((iVar8 == 2) && (*(int *)(param_1[0x32] + 0x188) != 5)) &&
               ((pcVar9 == (char *)0x7566 || (pcVar9 == (char *)0x7542)))) {
              *(undefined4 *)(param_1[0x32] + 0x188) = 5;
              iVar8 = param_1[0x32];
              if ((*(char *)(iVar8 + 0x1b8) == '\0') || (*(int *)(iVar8 + 0x1b0) == 0)) {
                if (param_1[0x33] == 0) {
                  cVar6 = *(char *)(iVar8 + 0x1a5);
                }
                else {
                  cVar6 = *(char *)(param_1[0x33] + 0x14);
                }
                if (((cVar6 == '\0') && (*(int *)(iVar8 + 0x184) == 0)) &&
                   (*(char *)(iVar8 + 0x1a1) == '\0')) {
                  *(undefined4 *)(iVar8 + 0x180) = 1;
                  *(undefined4 *)(param_1[0x32] + 0x184) = 1;
                }
              }
            }
            local_68 = (char *)(**(code **)(*param_1 + 0x18))(param_1 + 0xc80);
            if (local_68 == (char *)0x0) goto LAB_00593e5c;
            goto LAB_005944d2;
          }
        }
        else {
LAB_00593ee7:
          if ((param_1[0x19] < 1) && ((param_1[0x19] < 0 || (param_1[0x18] == 0))))
          goto LAB_00593efd;
        }
        if (param_1[0xc8d] != 0) {
          cVar6 = FUN_005b0600();
          if (((cVar6 != '\0') || (iVar8 = param_1[0x32], *(char *)(iVar8 + 0x367) == '\0')) ||
             ((*(char *)(iVar8 + 0x368) != '\0' || (*(char *)(iVar8 + 0x369) != '\0'))))
          goto LAB_005944d2;
          local_58 = (uint)local_58._1_3_ << 8;
          FUN_00402560();
          pcVar9 = local_68;
          goto LAB_005940a7;
        }
        iVar8 = param_1[0x32];
        pcVar14 = *(char **)(iVar8 + 0x420);
        pcVar9 = pcVar14 + 1;
        do {
          cVar6 = *pcVar14;
          pcVar14 = pcVar14 + 1;
        } while (cVar6 != '\0');
        if ((pcVar14 != pcVar9) && (*(char *)(iVar8 + 0x375) == '\0')) {
          *(undefined1 *)(iVar8 + 0x375) = 1;
          if (param_1[0x33] == 0) {
            iVar8 = *(int *)(param_1[0x32] + 0x24);
          }
          else {
            iVar8 = *(int *)(param_1[0x33] + 0x10);
          }
          if (((iVar8 == 2) && (*(int *)(param_1[0x32] + 0x188) != 5)) &&
             ((local_68 == (char *)0x7566 || (local_68 == (char *)0x7542)))) {
            *(undefined4 *)(param_1[0x32] + 0x188) = 5;
            iVar8 = param_1[0x32];
            if ((*(char *)(iVar8 + 0x1b8) == '\0') || (*(int *)(iVar8 + 0x1b0) == 0)) {
              if (param_1[0x33] == 0) {
                cVar6 = *(char *)(iVar8 + 0x1a5);
              }
              else {
                cVar6 = *(char *)(param_1[0x33] + 0x14);
              }
              if (((cVar6 == '\0') && (*(int *)(iVar8 + 0x184) == 0)) &&
                 (*(char *)(iVar8 + 0x1a1) == '\0')) {
                *(undefined4 *)(iVar8 + 0x180) = 1;
                *(undefined4 *)(param_1[0x32] + 0x184) = 1;
              }
            }
          }
          FUN_00582fd0();
          FUN_00566b20("Restart type 26\n");
          local_68 = (char *)(**(code **)(*param_1 + 0x18))(param_1 + 0xc80);
          if (local_68 != (char *)0x0) {
            local_58 = (uint)local_58._1_3_ << 8;
            FUN_00402560();
            FUN_005954d0(uVar7);
            return;
          }
LAB_00593e5c:
          FUN_00592d50(param_2);
          local_58 = (uint)local_58._1_3_ << 8;
          FUN_00402560();
          FUN_005954d0(iVar17);
          return;
        }
        if (((*(uint *)(iVar8 + 0x164) & 0x80000) != 0) &&
           ((pcVar9 = _strstr(*(char **)(iVar8 + 0x1ac),"Firefox/"), pcVar9 != (char *)0x0 ||
            (pcVar9 = _strstr(*(char **)(param_1[0x32] + 0x1ac),"Chrome/"), pcVar9 != (char *)0x0)))
           ) {
          ATL::CSimpleStringT<char,0>::operator=
                    ((CSimpleStringT<char,0> *)(param_1[0x32] + 0x1ac),
                     (CSimpleStringT<char,0> *)&DAT_00788b2c);
          *(undefined1 *)(param_1[0x32] + 0x36d) = 0;
          *(undefined1 *)(param_1[0x32] + 0x373) = 1;
          FUN_00582fd0();
          FUN_00566b20("Restart type 25\n");
          local_68 = (char *)(**(code **)(*param_1 + 0x18))(param_1 + 0xc80);
          if (local_68 != (char *)0x0) {
            local_58 = (uint)local_58._1_3_ << 8;
            FUN_00402560();
            FUN_005954d0(uVar7);
            return;
          }
          goto LAB_00593e5c;
        }
        puVar15 = (undefined1 *)param_1[0xc8f];
        if (puVar15 == (undefined1 *)0x0) {
          puVar15 = &DAT_006979dc;
        }
        FUN_004030a0(puVar15);
        local_58 = CONCAT31(local_58._1_3_,7);
        FUN_00454630(&DAT_006979dc);
        if (param_1[0xc82] != 0) {
          FUN_00402f80(&DAT_006979dc);
        }
        *(undefined1 *)(param_1[0x32] + 0x1c4) = 0;
        FUN_00586630();
        iVar8 = param_1[0xc82];
        if (((iVar8 == 0) || (*(char *)(iVar8 + 0x25b) != '\0')) ||
           (pcVar9 = (char *)param_1[0xc8f], pcVar9 == (char *)0x0)) {
LAB_005943e3:
          if (param_1[0x33] == 0) {
            iVar8 = *(int *)(param_1[0x32] + 0x24);
          }
          else {
            iVar8 = *(int *)(param_1[0x33] + 0x10);
          }
          if (((iVar8 == 2) && (*(int *)(param_1[0x32] + 0x188) != 5)) &&
             ((local_68 == (char *)0x7566 || (local_68 == (char *)0x7542)))) {
            *(undefined4 *)(param_1[0x32] + 0x188) = 5;
            iVar8 = param_1[0x32];
            if ((*(char *)(iVar8 + 0x1b8) == '\0') || (*(int *)(iVar8 + 0x1b0) == 0)) {
              if (param_1[0x33] == 0) {
                cVar6 = *(char *)(iVar8 + 0x1a5);
              }
              else {
                cVar6 = *(char *)(param_1[0x33] + 0x14);
              }
              if (((cVar6 == '\0') && (*(int *)(iVar8 + 0x184) == 0)) &&
                 (*(char *)(iVar8 + 0x1a1) == '\0')) {
                *(undefined4 *)(iVar8 + 0x180) = 1;
                *(undefined4 *)(param_1[0x32] + 0x184) = 1;
              }
            }
            FUN_00582fd0();
            FUN_00566b20("Restart type 28\n");
            local_68 = (char *)(**(code **)(*param_1 + 0x18))(param_1 + 0xc80);
            goto joined_r0x005944bb;
          }
        }
        else {
          pcVar14 = pcVar9;
          do {
            cVar6 = *pcVar14;
            pcVar14 = pcVar14 + 1;
          } while (cVar6 != '\0');
          if (pcVar14 == pcVar9 + 1) goto LAB_005943e3;
          pcVar14 = pcVar9 + 1;
          do {
            cVar6 = *pcVar9;
            pcVar9 = pcVar9 + 1;
          } while (cVar6 != '\0');
          pcVar16 = local_74;
          do {
            cVar6 = *pcVar16;
            pcVar16 = pcVar16 + 1;
          } while (cVar6 != '\0');
          if ((int)pcVar9 - (int)pcVar14 == (int)pcVar16 - (int)(local_74 + 1)) goto LAB_005943e3;
          *(undefined1 *)(iVar8 + 0x25b) = 1;
          FUN_00582fd0();
          FUN_00566b20("Restart type 23\n");
          local_68 = (char *)(**(code **)(*param_1 + 0x18))(param_1 + 0xc80);
joined_r0x005944bb:
          if (local_68 == (char *)0x0) {
            FUN_00592d50(param_2);
            local_58._0_1_ = 6;
            FUN_00402560();
            local_58 = (uint)local_58._1_3_ << 8;
            FUN_00402560();
            FUN_005954d0(iVar17);
            return;
          }
        }
        local_58._0_1_ = 6;
        FUN_00402560();
LAB_005944d2:
        local_58 = (uint)local_58._1_3_ << 8;
        FUN_00402560();
        FUN_005954d0(uVar7);
        return;
      }
      *(undefined1 *)((int)puVar13 + 0x1c1) = 1;
      FUN_00582fd0();
      FUN_00566b20("Restart type s021s3\n");
    }
LAB_005931a4:
    local_68 = (char *)(**(code **)(*param_1 + 0x18))(param_1 + 0xc80);
  }
  if (local_68 == (char *)0x0) {
    FUN_00592d50(param_2);
    FUN_005954d0(iVar17);
    return;
  }
code_r0x005954d0:
  ExceptionList = local_60;
  __security_check_cookie(local_8 ^ (uint)&local_54);
  return;
}


