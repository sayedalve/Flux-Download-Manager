// Function: FUN_10010480
// Entry Point: 0x10010480
// Program: IDMNetMon.dll


void __fastcall FUN_10010480(uint param_1,char *param_2,char param_3)

{
  byte bVar1;
  byte *pbVar2;
  bool bVar3;
  bool bVar4;
  bool bVar5;
  uint uVar6;
  char cVar7;
  ushort uVar8;
  int iVar9;
  undefined4 uVar10;
  int *piVar11;
  int *piVar12;
  undefined4 uVar13;
  undefined4 *puVar14;
  size_t sVar15;
  void *pvVar16;
  uint uVar17;
  int *piVar18;
  byte *pbVar19;
  undefined *puVar20;
  undefined *puVar21;
  byte bVar22;
  undefined4 extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  undefined4 extraout_ECX_02;
  undefined4 extraout_ECX_03;
  int extraout_ECX_04;
  int extraout_ECX_05;
  int extraout_ECX_06;
  int extraout_ECX_07;
  int extraout_ECX_08;
  undefined4 extraout_ECX_09;
  undefined4 extraout_ECX_10;
  undefined4 extraout_ECX_11;
  int extraout_ECX_12;
  int extraout_ECX_13;
  int extraout_ECX_14;
  undefined4 extraout_ECX_15;
  undefined4 extraout_ECX_16;
  undefined4 extraout_ECX_17;
  int extraout_ECX_18;
  int extraout_ECX_19;
  undefined4 extraout_ECX_20;
  undefined4 extraout_ECX_21;
  undefined4 extraout_ECX_22;
  undefined4 extraout_ECX_23;
  undefined4 extraout_ECX_24;
  ushort uVar23;
  undefined3 uVar24;
  uint3 uVar25;
  int iVar26;
  int *piVar27;
  char *pcVar28;
  longlong lVar29;
  undefined8 uVar30;
  longlong lVar31;
  char local_1c5;
  byte local_1c4;
  byte local_1c3;
  uint local_1c0;
  char local_1b5;
  int *local_1b4;
  uint local_1b0;
  int local_1ac;
  int *local_1a8;
  int *local_1a4;
  char *local_1a0;
  int *local_19c;
  uint local_198;
  ushort local_194;
  undefined4 *local_190;
  size_t local_18c;
  size_t local_188;
  undefined2 local_184;
  int local_180;
  int *local_17c;
  char local_175;
  int *local_174;
  uint local_170;
  int local_16c;
  char *local_168;
  uint local_164;
  uint local_160;
  undefined2 local_15c;
  char *local_158;
  undefined4 *local_154;
  byte *local_150;
  undefined8 local_148;
  undefined4 local_140;
  undefined4 *local_138;
  int local_134;
  undefined4 local_130;
  uint local_12c;
  int *local_128;
  size_t local_124;
  undefined4 local_120;
  undefined2 local_11c;
  int local_118;
  uint local_114;
  byte *local_110;
  int local_10c;
  undefined4 local_108;
  undefined2 local_104;
  undefined8 local_100;
  undefined4 *local_f4;
  int local_f0;
  undefined4 local_ec;
  undefined2 local_e8;
  undefined4 *local_e4;
  int local_e0;
  undefined4 local_dc;
  undefined2 local_d8;
  undefined4 *local_d4;
  int local_d0;
  undefined4 local_cc;
  undefined2 local_c8;
  char *local_c4;
  undefined4 *local_c0;
  int *local_50;
  int local_18;
  
  local_170 = 0;
  local_c4 = param_2;
  FUN_1002e380();
  puVar20 = &DAT_10074be8;
  if (*(undefined **)(param_1 + 0x20) != (undefined *)0x0) {
    puVar20 = *(undefined **)(param_1 + 0x20);
  }
  iVar9 = FUN_10030140(puVar20,*(undefined4 *)(param_1 + 0x24));
  local_148 = CONCAT44(local_148._4_4_,(uint)local_148);
  local_100 = CONCAT44(local_100._4_4_,(uint)local_100);
  if (iVar9 == 0) goto LAB_100118d9;
  iVar9 = *(int *)(param_1 + 0x10);
  local_140 = *(undefined4 *)(iVar9 + 0xc);
  iVar26 = *(int *)(*(int *)(param_1 + 0xc) + 8);
  local_154 = (undefined4 *)(param_1 + 0x5c + iVar26);
  iVar26 = *(int *)(iVar26 + 0xac + param_1);
  if ((iVar26 == 0) || (bVar5 = true, *(short *)(iVar26 + 0x28) == 0)) {
    bVar5 = false;
  }
  local_175 = local_18 == 1;
  local_1c4 = 0;
  local_1a0 = (char *)&DAT_10074bf4;
  local_194 = 0x100;
  local_19c = (int *)0x0;
  local_198 = 0;
  piVar27 = (int *)0x0;
  uVar17 = param_1;
  local_148 = CONCAT44(local_148._4_4_,(uint)local_148);
  local_100 = CONCAT44(local_100._4_4_,(uint)local_100);
  if ((local_18 == 1) ||
     (local_1b4 = local_50, local_148 = CONCAT44(local_148._4_4_,(uint)local_148),
     local_100 = CONCAT44(local_100._4_4_,(uint)local_100), local_50 == (int *)0x0)) {
LAB_10010d46:
    local_1c3 = local_1c4;
    if (piVar27 == (int *)0x0) goto LAB_10010c4d;
    if (piVar27 == (int *)0x2) {
      iVar26 = _strnicmp(local_1a0,"TS",2);
      iVar9 = extraout_ECX_14;
      if (iVar26 != 0) goto LAB_10010ca1;
      uVar17 = CONCAT31((int3)(uVar17 >> 8),1);
    }
    else {
LAB_10010ca1:
      uVar17 = uVar17 & 0xffffff00;
    }
  }
  else {
    *(int *)(iVar9 + 0x10) = *(int *)(iVar9 + 0x10) + 1;
    local_17c = local_50 + 8;
    local_158 = *(char **)(iVar9 + 0x10);
    iVar26 = 0;
    bVar4 = false;
    local_174 = (int *)0x0;
    uVar10 = FUN_10006290(0x3f,0,0xffffffff,0);
    iVar9 = FUN_10006420(0x2f,uVar10,iVar9);
    local_1c4 = 2;
    piVar27 = (int *)(iVar9 + 1);
    if (iVar9 < 0) {
      piVar27 = (int *)0x0;
    }
    piVar11 = (int *)FUN_1000b2c0();
    piVar12 = (int *)FUN_1000ae80(0);
    piVar18 = (int *)piVar12[1];
    local_1a4 = (int *)(*piVar11 + *piVar12);
    local_194 = CONCAT11(local_194._1_1_,(char)piVar11[3]);
    if (((int)piVar18 < 1) ||
       ((local_1a8 = piVar18, (int)piVar18 + 1 != 0 &&
        (iVar9 = FUN_10005810((int)piVar18 + 1), iVar9 == 0)))) {
      local_1a8 = (int *)0x0;
      local_19c = (int *)0x0;
      *local_1a0 = '\0';
    }
    else {
      local_1a0[(int)piVar18] = '\0';
      local_19c = piVar18;
      memcpy(local_1a0,local_1a4,(size_t)piVar18);
    }
    if (local_190 != &DAT_10074bf4) {
      free(local_190);
    }
    lVar29 = CONCAT44(local_100._4_4_,(uint)local_100);
    local_1ac = local_1b4[0x15];
    if (local_1ac != 0) {
      local_16c = local_148._4_4_;
      local_1c0 = (uint)local_148;
      local_1b0 = local_114;
      local_180 = local_118;
      do {
        local_1a4 = *(int **)(local_1ac + 0x24);
        local_100 = lVar29;
        if (local_1a8 == (int *)0x0) {
LAB_10010720:
          bVar3 = false;
        }
        else {
          local_170 = local_170 | 3;
          piVar11 = (int *)FUN_1000b2c0();
          piVar12 = (int *)FUN_1000ae80(0);
          piVar18 = (int *)piVar12[1];
          local_150 = (byte *)(*piVar11 + *piVar12);
          if ((int)piVar18 < 1) {
LAB_100106f8:
            piVar18 = (int *)0x0;
            local_1b4 = &DAT_10074bf4;
          }
          else {
            local_1b4 = piVar18;
            local_1b4 = malloc((size_t)(piVar18 + 4));
            if (local_1b4 == (int *)0x0) goto LAB_100106f8;
            *(char *)((int)local_1b4 + (int)piVar18) = '\0';
            memcpy(local_1b4,local_150,(size_t)piVar18);
          }
          if (piVar18 == local_1a8) {
            iVar9 = _strnicmp((char *)local_1b4,local_1a0,(size_t)piVar18);
            if (iVar9 == 0) goto LAB_10010720;
          }
          bVar3 = true;
        }
        if (((local_170 & 2) != 0) &&
           (local_170 = local_170 & 0xfffffffd, local_1b4 != &DAT_10074bf4)) {
          free(local_1b4);
        }
        if (((local_170 & 1) != 0) &&
           (local_170 = local_170 & 0xfffffffe, local_c0 != &DAT_10074bf4)) {
          free(local_c0);
        }
        piVar18 = local_17c;
        if (bVar3) {
          local_1a8 = (int *)0x0;
          local_19c = (int *)0x0;
          *local_1a0 = '\0';
        }
        lVar29 = local_148;
        if (local_1a4 < piVar27) {
LAB_1001097a:
          lVar31 = CONCAT44(local_16c,local_1c0);
          if (bVar4) {
LAB_10010989:
            uVar17 = local_1b0;
            lVar31 = CONCAT44(local_16c,local_1c0);
            if ((0 < iVar26) &&
               (piVar27 = (int *)((int)piVar27 + iVar26), lVar31 = CONCAT44(local_16c,local_1c0),
               local_180 != 0 || local_1b0 != 0)) {
              do {
                local_16c = (int)((ulonglong)lVar31 >> 0x20);
                if (lVar31 != CONCAT44(uVar17,local_180)) {
                  piVar27 = (int *)((int)piVar27 + -1);
                }
                local_148 = lVar29;
                uVar30 = __alldiv(local_180,uVar17,10,0);
                uVar17 = (uint)((ulonglong)uVar30 >> 0x20);
                local_180 = (int)uVar30;
                local_118 = local_180;
                local_114 = uVar17;
                lVar29 = __alldiv((int)lVar31,local_16c,10,0);
                piVar18 = local_17c;
                lVar31 = lVar29;
              } while (local_180 != 0 || uVar17 != 0);
            }
            local_1b0 = uVar17;
            if (piVar27 < (int *)piVar18[1]) {
              *(char *)((int)piVar27 + *piVar18) = '*';
            }
            pcVar28 = (char *)((int)piVar27 + 1);
            if (pcVar28 < (char *)piVar18[1]) {
              piVar18[1] = (int)pcVar28;
              pcVar28[*piVar18] = '\0';
            }
          }
          local_16c = (int)((ulonglong)lVar31 >> 0x20);
          local_1c0 = (uint)lVar31;
          local_148 = lVar29;
          cVar7 = FUN_1000af60();
          uVar10 = extraout_ECX;
          if (cVar7 != '\0') {
            FUN_10042fb0(param_1,piVar18,local_158,0x200,2,0);
            uVar10 = extraout_ECX_00;
          }
          iVar26 = 0;
          piVar18 = (int *)(local_1ac + 0x20);
          bVar4 = false;
          local_174 = (int *)0x0;
          local_17c = piVar18;
          uVar13 = FUN_10006290(0x3f,0,0xffffffff,0);
          iVar9 = FUN_10006420(0x2f,uVar13,uVar10);
          lVar29 = local_100;
          if (iVar9 < 0) {
            piVar27 = (int *)0x0;
          }
          else {
            piVar27 = (int *)(iVar9 + 1);
          }
        }
        else {
          iVar9 = strncmp((char *)*local_17c,*(char **)(local_1ac + 0x20),(size_t)piVar27);
          lVar29 = local_148;
          if (iVar9 != 0) goto LAB_1001097a;
          uVar10 = FUN_10006290(0x3f,piVar27,0xffffffff,0);
          iVar9 = FUN_10006290(0x2f,piVar27,uVar10,0);
          lVar29 = local_148;
          if (-1 < iVar9) goto LAB_1001097a;
          lVar29 = local_100;
          if (bVar4) {
            if (iVar26 < 1) goto LAB_10010a9f;
            iVar9 = FUN_1000d360(iVar26,(char *)(*(int *)(local_1ac + 0x20) + (int)local_174));
            local_16c = local_148._4_4_;
            lVar29 = local_148;
            if ((iVar9 == 0) || (local_148._4_4_ < local_100._4_4_)) {
              local_1c0 = (uint)local_148;
            }
            else {
              local_1c0 = (uint)local_148;
              if ((local_100._4_4_ < local_148._4_4_) || ((uint)local_100 <= (uint)local_148))
              goto LAB_10010a9f;
            }
            goto LAB_10010989;
          }
          if (piVar27 < local_1a4) {
            do {
              if (piVar27 < (int *)piVar18[1]) {
                cVar7 = *(char *)((int)piVar27 + *piVar18);
              }
              else {
                cVar7 = '\0';
              }
              if (*(char *)((int)piVar27 + *(int *)(local_1ac + 0x20)) != cVar7) goto LAB_10010896;
              piVar27 = (int *)((int)piVar27 + 1);
            } while (piVar27 < local_1a4);
          }
          if ((piVar27 != local_1a4) || (piVar27 != (int *)piVar18[1])) {
LAB_10010896:
            if (piVar27 < (int *)piVar18[1]) {
              bVar22 = *(byte *)((int)piVar27 + *piVar18);
            }
            else {
              bVar22 = 0;
            }
            if ((*(uint *)(&DAT_10062ca8 + (uint)bVar22 * 4) >> 4 & 1) != 0) {
              bVar1 = *(byte *)(*(int *)(local_1ac + 0x20) + (int)piVar27);
              local_150 = (byte *)(*(int *)(local_1ac + 0x20) + (int)piVar27);
              if ((((*(uint *)(&DAT_10062ca8 + (uint)bVar1 * 4) >> 4 & 1) != 0) &&
                  ((char)bVar22 < (char)bVar1)) &&
                 (piVar11 = piVar27, pbVar2 = local_150, piVar27 != (int *)0x0)) {
                do {
                  if ((*(uint *)(&DAT_10062ca8 + (uint)pbVar2[-1] * 4) >> 4 & 1) == 0) break;
                  piVar11 = (int *)((int)piVar11 + -1);
                  pbVar2 = pbVar2 + -1;
                } while (piVar11 != (int *)0x0);
                if (piVar11 < piVar27) {
                  iVar26 = (int)piVar27 - (int)piVar11;
                  local_174 = piVar11;
                  FUN_1000d360(iVar26,piVar11);
                  FUN_1000d360(iVar26);
                  local_1b0 = local_114;
                  local_180 = local_118;
                  piVar27 = piVar11;
                }
              }
            }
            bVar4 = true;
            lVar29 = local_100;
          }
        }
LAB_10010a9f:
        lVar31 = CONCAT44(local_16c,local_1c0);
        local_1ac = *(int *)(local_1ac + 0x54);
      } while (local_1ac != 0);
      local_1ac = 0;
      if (bVar4) {
        if ((0 < iVar26) &&
           (piVar27 = (int *)((int)piVar27 + iVar26), uVar17 = local_1b0,
           local_180 != 0 || local_1b0 != 0)) {
          do {
            if (lVar31 != CONCAT44(uVar17,local_180)) {
              piVar27 = (int *)((int)piVar27 + -1);
            }
            local_100 = lVar29;
            uVar30 = __alldiv(local_180,uVar17,10,0);
            uVar17 = (uint)((ulonglong)uVar30 >> 0x20);
            local_180 = (int)uVar30;
            lVar31 = __alldiv(lVar31,10,0);
            piVar18 = local_17c;
            lVar29 = local_100;
          } while (local_180 != 0 || uVar17 != 0);
        }
        if (piVar27 < (int *)piVar18[1]) {
          *(char *)((int)piVar27 + *piVar18) = '*';
        }
        pcVar28 = (char *)((int)piVar27 + 1);
        if (pcVar28 < (char *)piVar18[1]) {
          piVar18[1] = (int)pcVar28;
          pcVar28[*piVar18] = '\0';
        }
      }
    }
    piVar27 = local_17c;
    local_100 = lVar29;
    cVar7 = FUN_1000af60();
    if (cVar7 != '\0') {
      FUN_10042fb0(param_1,piVar27,local_158,0x200,2,0);
    }
    local_18c = strlen("TS|MP4|M4V|M4S|M4A|AAC|MP3|AC3|EAC3|VTT|IMSC");
    local_184 = 0x100;
    uVar10 = extraout_ECX_01;
    if ((int)local_18c < 1) {
LAB_10010bac:
      puVar14 = &DAT_10074bf4;
      local_188 = 0;
      local_190 = &DAT_10074bf4;
      local_18c = 0;
    }
    else {
      local_188 = local_18c + 0x10;
      puVar14 = malloc(local_188);
      uVar10 = extraout_ECX_02;
      if (puVar14 == (undefined4 *)0x0) goto LAB_10010bac;
      *(undefined1 *)(local_18c + (int)puVar14) = 0;
      local_190 = puVar14;
      memcpy(puVar14,"TS|MP4|M4V|M4S|M4A|AAC|MP3|AC3|EAC3|VTT|IMSC",local_18c);
      uVar10 = extraout_ECX_03;
    }
    cVar7 = FUN_1000d470(uVar10);
    uVar24 = (undefined3)((uint)piVar18 >> 8);
    uVar17 = CONCAT31(uVar24,cVar7);
    iVar9 = extraout_ECX_04;
    if (puVar14 != &DAT_10074bf4) {
      free(puVar14);
      iVar9 = extraout_ECX_05;
    }
    pcVar28 = local_1a0;
    if (cVar7 == '\0') {
      local_19c = (int *)0x0;
      *local_1a0 = '\0';
      piVar27 = (int *)0x0;
      goto LAB_10010d46;
    }
    _strupr(local_1a0);
    if (local_1a8 != (int *)0x3) {
LAB_10010ca8:
      sVar15 = strlen("M4A|AAC|MP3|AC3|EAC3");
      local_184 = 0x100;
      uVar10 = extraout_ECX_09;
      if ((int)sVar15 < 1) {
LAB_10010cf7:
        puVar14 = &DAT_10074bf4;
        local_188 = 0;
        local_190 = &DAT_10074bf4;
        local_18c = 0;
      }
      else {
        local_188 = sVar15 + 0x10;
        puVar14 = malloc(local_188);
        uVar10 = extraout_ECX_10;
        if (puVar14 == (undefined4 *)0x0) goto LAB_10010cf7;
        *(undefined1 *)((int)puVar14 + sVar15) = 0;
        local_190 = puVar14;
        local_18c = sVar15;
        memcpy(puVar14,"M4A|AAC|MP3|AC3|EAC3",sVar15);
        uVar10 = extraout_ECX_11;
      }
      cVar7 = FUN_1000d470(uVar10);
      uVar17 = CONCAT31(uVar24,cVar7);
      iVar9 = extraout_ECX_12;
      if (puVar14 != &DAT_10074bf4) {
        free(puVar14);
        iVar9 = extraout_ECX_13;
      }
      if (cVar7 != '\0') {
        local_1c4 = 1;
        piVar27 = local_1a8;
        goto LAB_10010d46;
      }
      sVar15 = strlen("VTT|IMSC");
      local_184 = 0x100;
      uVar10 = extraout_ECX_15;
      if ((int)sVar15 < 1) {
LAB_10010dcd:
        puVar14 = &DAT_10074bf4;
        local_188 = 0;
        local_190 = &DAT_10074bf4;
        local_18c = 0;
      }
      else {
        local_188 = sVar15 + 0x10;
        puVar14 = malloc(local_188);
        uVar10 = extraout_ECX_16;
        if (puVar14 == (undefined4 *)0x0) goto LAB_10010dcd;
        *(undefined1 *)(sVar15 + (int)puVar14) = 0;
        local_190 = puVar14;
        local_18c = sVar15;
        memcpy(puVar14,"VTT|IMSC",sVar15);
        uVar10 = extraout_ECX_17;
      }
      cVar7 = FUN_1000d470(uVar10);
      uVar17 = CONCAT31(uVar24,cVar7);
      iVar9 = extraout_ECX_18;
      if (puVar14 != &DAT_10074bf4) {
        free(puVar14);
        iVar9 = extraout_ECX_19;
      }
      piVar27 = local_1a8;
      if (cVar7 != '\0') {
        local_1c4 = 3;
      }
      goto LAB_10010d46;
    }
    iVar9 = _strnicmp(pcVar28,"M4S",3);
    if (iVar9 != 0) goto LAB_10010ca8;
    local_194 = local_194 & 0xff00;
    iVar9 = extraout_ECX_06;
    if (3 < local_198) {
LAB_10010c7f:
      pcVar28[3] = '\0';
      *(undefined2 *)pcVar28 = DAT_1006a6c4;
      local_19c = (int *)0x3;
      pcVar28[2] = DAT_1006a6c6;
      local_1c3 = 2;
      goto LAB_10010ca1;
    }
    iVar26 = FUN_10005810(4);
    iVar9 = extraout_ECX_07;
    pcVar28 = local_1a0;
    if (iVar26 != 0) goto LAB_10010c7f;
    *local_1a0 = '\0';
    local_1c3 = 2;
    local_19c = (int *)0x0;
LAB_10010c4d:
    local_194 = local_194 & 0xff00;
    if (local_198 < 3) {
      iVar26 = FUN_10005810(3);
      iVar9 = extraout_ECX_08;
      if (iVar26 != 0) goto LAB_10010e2a;
      *local_1a0 = '\0';
      local_19c = (int *)0x0;
    }
    else {
LAB_10010e2a:
      local_1a0[2] = '\0';
      local_19c = (int *)0x2;
      local_1a0[0] = 'T';
      local_1a0[1] = 'S';
    }
    uVar17 = CONCAT31((int3)(uVar17 >> 8),local_1a0 != (char *)0x0);
  }
  bVar22 = (byte)uVar17;
  if ((param_3 == '\0') || (bVar22 == 0)) {
    cVar7 = FUN_1000d470(iVar9);
    if (cVar7 == '\0') goto LAB_10010e6f;
    uVar23 = CONCAT11(1,bVar22);
  }
  else {
LAB_10010e6f:
    uVar23 = (ushort)bVar22;
  }
  uVar10 = CONCAT22((short)(uVar17 >> 0x10),uVar23);
  if ((char)uVar23 == '\0') {
LAB_10010f1b:
    bVar4 = false;
  }
  else {
    sVar15 = strlen("MP4");
    local_184 = 0x100;
    uVar13 = extraout_ECX_20;
    if ((int)sVar15 < 1) {
LAB_10010ecc:
      puVar14 = &DAT_10074bf4;
      local_188 = 0;
      local_190 = &DAT_10074bf4;
      local_18c = 0;
    }
    else {
      local_188 = sVar15 + 0x10;
      puVar14 = malloc(local_188);
      uVar13 = extraout_ECX_21;
      if (puVar14 == (undefined4 *)0x0) goto LAB_10010ecc;
      *(undefined1 *)((int)puVar14 + sVar15) = 0;
      local_190 = puVar14;
      local_18c = sVar15;
      memcpy(puVar14,&DAT_1006a6c4,sVar15);
      uVar13 = extraout_ECX_22;
    }
    cVar7 = FUN_1000d470(uVar13);
    uVar10 = CONCAT31((int3)((uint)uVar10 >> 8),cVar7);
    if (puVar14 != &DAT_10074bf4) {
      free(puVar14);
    }
    if (cVar7 == '\0') goto LAB_10010f1b;
    bVar4 = true;
  }
  if (((char)((uint)uVar10 >> 8) != '\0') || (bVar4)) {
    local_168 = (char *)&DAT_10074bf4;
    local_164 = 0;
    local_160 = 0;
    iVar9 = *(int *)(param_1 + 0x10);
    local_15c = 0;
    local_128 = &DAT_10074bf4;
    local_124 = 0;
    local_120 = 0;
    local_11c = 0;
    local_138 = &DAT_10074bf4;
    local_134 = 0;
    local_130 = 0;
    local_12c = local_12c & 0xffff0000;
    local_110 = (byte *)&DAT_10074bf4;
    local_10c = 0;
    local_108 = 0;
    local_104 = 0;
    local_d4 = &DAT_10074bf4;
    local_d0 = 0;
    local_cc = 0;
    local_c8 = 0;
    local_e4 = &DAT_10074bf4;
    local_e0 = 0;
    local_dc = 0;
    local_d8 = 0;
    local_f4 = &DAT_10074bf4;
    local_f0 = 0;
    local_ec = 0;
    local_e8 = 0;
    piVar27 = (int *)(iVar9 + 0x10);
    *piVar27 = *piVar27 + 1;
    local_17c = *(int **)(iVar9 + 0x10);
    cVar7 = FUN_1002e4d0(&local_168,&local_128,&local_138,&local_110,&local_d4,&local_e4,&local_f4,
                         &local_1b5);
    local_1b4 = (int *)CONCAT31((int3)((uint)uVar10 >> 8),cVar7);
    while (cVar7 != '\0') {
      if ((char)local_1b4 == -1) {
        local_1b4 = (int *)((uint)local_1b4 & 0xff);
        if (local_1c3 != 0) {
          local_1b4 = (int *)(uint)local_1c3;
        }
      }
      piVar27 = local_1b4;
      if (local_175 == '\0') {
        sVar15 = local_154[1];
        pvVar16 = (void *)*local_154;
        uVar8 = (ushort)local_15c >> 8;
        local_15c = CONCAT11((char)uVar8,*(undefined1 *)(local_154 + 3));
        if ((int)sVar15 < 1) {
LAB_100111d0:
          local_164 = 0;
          *local_168 = '\0';
        }
        else {
          if (local_160 < sVar15 + 1) {
            iVar9 = FUN_10005810(sVar15 + 1);
            if (iVar9 == 0) goto LAB_100111d0;
          }
          local_168[sVar15] = '\0';
          local_164 = sVar15;
          memcpy(local_168,pvVar16,sVar15);
        }
        if (0x16 < local_164) {
          iVar9 = strncmp(local_168,"https://strm.yandex.ru/",0x17);
          if (iVar9 == 0) {
            FUN_1000bb70();
          }
        }
        iVar9 = *(int *)(*(int *)(param_1 + 0xc) + 8);
        if ((local_134 != 0) &&
           ((*(byte *)(iVar9 + 0x9c + param_1) < 0x6f || (*(int *)(iVar9 + 0x90 + param_1) == 0))))
        {
          *(undefined1 *)(iVar9 + 0x9c + param_1) = 0x6e;
          FUN_10006d50(local_138,local_134,local_12c,0);
        }
LAB_10011260:
        uVar8 = 0;
        if (local_1b5 != '\0') {
          uVar8 = 0x100;
        }
        local_174 = (int *)(uint)uVar8;
        uVar25 = (uint3)((uint)piVar27 >> 8);
        if ((((char)piVar27 != '\x02') || (uVar8 != 0)) || (bVar5)) {
          bVar3 = false;
LAB_100112a0:
          piVar27 = (int *)((uint)uVar25 << 8);
        }
        else {
          bVar3 = true;
          if ((!bVar4) || ((char)(uVar23 >> 8) != '\0')) goto LAB_100112a0;
          piVar27 = (int *)CONCAT31(uVar25,1);
        }
        local_1c5 = (char)piVar27;
        if (local_1c5 == '\0') goto LAB_100112be;
        while ((param_3 == '\0' || (local_124 == 0))) {
LAB_100112be:
          pvVar16 = malloc(0x48);
          if (pvVar16 == (void *)0x0) break;
          uVar17 = FUN_100100b0(extraout_ECX_24);
          local_1b0 = uVar17;
          if (uVar17 == 0) break;
          if (*(char *)(uVar17 + 0x1c) < (char)local_1b4) {
            *(char *)(uVar17 + 0x1c) = (char)local_1b4;
          }
          if ((char)piVar27 == '\0') {
            if (local_124 != 0) {
              *(undefined1 *)(uVar17 + 0x18) = 0;
              if (*(uint *)(uVar17 + 0x14) < 10) {
                iVar9 = FUN_10005810(10);
                if (iVar9 == 0) {
                  *(undefined4 *)(uVar17 + 0x10) = 0;
                  **(undefined1 **)(uVar17 + 0xc) = 0;
                  goto LAB_100113de;
                }
              }
              puVar14 = *(undefined4 **)(uVar17 + 0xc);
              *(undefined4 *)(uVar17 + 0x10) = 9;
              *(undefined1 *)((int)puVar14 + 9) = 0;
              *puVar14 = DAT_1006a740;
              puVar14[1] = DAT_1006a744;
              *(undefined1 *)(puVar14 + 2) = DAT_1006a748;
            }
          }
          else {
            if (local_124 == 0) {
              pcVar28 = "idmhlstomp4v2";
              if (param_3 != '\0') {
                pcVar28 = local_c4;
              }
              if (pcVar28 != (char *)0x0) goto LAB_10011324;
              *(undefined1 *)(uVar17 + 0x18) = 0;
            }
            else {
              pcVar28 = "hls2toidmtomp4";
LAB_10011324:
              local_150 = (byte *)strlen(pcVar28);
              *(undefined1 *)(uVar17 + 0x18) = 0;
              if (0 < (int)local_150) {
                if (*(byte **)(uVar17 + 0x14) < local_150 + 1) {
                  iVar9 = FUN_10005810(local_150 + 1);
                  if (iVar9 == 0) goto LAB_1001137a;
                }
                pvVar16 = *(void **)(uVar17 + 0xc);
                *(byte **)(uVar17 + 0x10) = local_150;
                local_150[(int)pvVar16] = 0;
                memcpy(pvVar16,pcVar28,(size_t)local_150);
                goto LAB_100113de;
              }
            }
LAB_1001137a:
            *(undefined4 *)(uVar17 + 0x10) = 0;
            **(undefined1 **)(uVar17 + 0xc) = 0;
          }
LAB_100113de:
          puVar14 = (undefined4 *)(-(uint)(local_1b0 != 0xfffffffc) & local_1b0);
          piVar27 = (int *)FUN_10042fb0(param_1,&local_168,local_17c,local_174,1,puVar14);
          local_1a4 = piVar27;
          if (puVar14 != (undefined4 *)0x0) {
            piVar18 = (int *)(*(int *)(puVar14[1] + 4) + (int)(puVar14 + 1));
            *piVar18 = *piVar18 + -1;
            if (*piVar18 == 0) {
              if (puVar14 + 1 == (undefined4 *)0x0) {
                puVar14 = (undefined4 *)0x0;
              }
              if (puVar14 != (undefined4 *)0x0) {
                (**(code **)*puVar14)(1);
              }
            }
          }
          if (piVar27 == (int *)0x0) break;
          if (local_1c5 == '\0') {
            local_158 = local_1a0;
            if (local_1a0 != (char *)0x0) goto LAB_10011462;
            *(char *)(piVar27 + 0x1e) = '\0';
            piVar18 = piVar27;
LAB_100114ac:
            piVar27[0x1c] = 0;
            *(undefined1 *)piVar27[0x1b] = 0;
          }
          else {
            local_158 = "MP4";
LAB_10011462:
            piVar18 = (int *)strlen(local_158);
            *(char *)(piVar27 + 0x1e) = '\0';
            if ((int)piVar18 < 1) goto LAB_100114ac;
            if ((char *)piVar27[0x1d] < (char *)((int)piVar18 + 1U)) {
              iVar9 = FUN_10005810((char *)((int)piVar18 + 1U));
              if (iVar9 == 0) goto LAB_100114ac;
            }
            pvVar16 = (void *)piVar27[0x1b];
            piVar27[0x1c] = (int)piVar18;
            *(char *)((int)pvVar16 + (int)piVar18) = '\0';
            memcpy(pvVar16,local_158,(size_t)piVar18);
          }
          if ((local_134 != 0) &&
             ((*(byte *)((int)local_1a4 + 0x39) < 0x6e || (local_1a4[0x24] == 0)))) {
            *(char *)((int)local_1a4 + 0x39) = 'n';
            FUN_10006d50(local_138,local_134,local_12c,0);
          }
          sVar15 = local_124;
          piVar11 = local_128;
          uVar17 = local_1b0;
          piVar27 = piVar18;
          if (local_124 != 0) {
            *(undefined1 *)(local_1b0 + 0x38) = (undefined1)local_11c;
            piVar27 = piVar11;
            if ((int)local_124 < 1) {
LAB_10011549:
              *(undefined4 *)(uVar17 + 0x30) = 0;
              **(undefined1 **)(uVar17 + 0x2c) = 0;
            }
            else {
              if (*(uint *)(local_1b0 + 0x34) < local_124 + 1) {
                iVar9 = FUN_10005810(local_124 + 1);
                if (iVar9 == 0) goto LAB_10011549;
              }
              pvVar16 = *(void **)(uVar17 + 0x2c);
              *(size_t *)(uVar17 + 0x30) = sVar15;
              *(undefined1 *)((int)pvVar16 + sVar15) = 0;
              memcpy(pvVar16,piVar11,sVar15);
            }
          }
          uVar17 = local_1b0;
          if (local_e0 != 0) {
            FUN_1000d300(local_e0,10);
          }
          if (local_d0 != 0) {
            FUN_1000d300(local_d0,10);
          }
          if (local_f0 != 0) {
            FUN_1000d300(local_f0,10);
          }
          piVar18 = local_1a4;
          if (((short)local_174 != 0) || (bVar5)) break;
          piVar27 = local_1a4 + 0x1f;
          local_1a4[0x20] = 0;
          *(undefined2 *)*piVar27 = 0;
          if (*(int *)(uVar17 + 0x20) != 0) {
            piVar11 = (int *)FUN_100436f0(local_10c + 0x23);
            if ((undefined4 *)*piVar27 != &DAT_10074bf4) {
              free((undefined4 *)*piVar27 + -1);
            }
            *(char *)(piVar18 + 0x22) = (char)piVar11[3];
            iVar9 = piVar11[2];
            piVar18[0x21] = iVar9;
            if (iVar9 == 0) {
              *piVar27 = (int)&DAT_10074bf4;
              piVar18[0x21] = 0;
              piVar18[0x20] = 0;
            }
            else {
              *piVar27 = *piVar11;
              piVar18[0x20] = piVar11[1];
              *piVar11 = (int)&DAT_10074bf4;
              piVar11[2] = 0;
              piVar11[1] = 0;
            }
            if (local_c0 != &DAT_10074bf4) {
              free(local_c0 + -1);
            }
          }
          if (local_10c != 0) {
            iVar9 = piVar18[0x20];
            pbVar19 = local_110;
            _strupr((char *)local_110);
            pbVar2 = local_110;
            pbVar19 = (byte *)FUN_10006950(pbVar19);
            local_150 = pbVar2;
            if (pbVar19 != pbVar2) {
              local_150 = pbVar19;
            }
            puVar20 = &DAT_1006a48c;
            if (iVar9 == 0) {
              puVar20 = &DAT_10062710;
            }
            FUN_10010080(piVar27,L"%s%s",local_10c + 0x23,puVar20,local_150);
            if ((local_150 != pbVar2) && (local_150 != (byte *)0x0)) {
              free(local_150);
            }
          }
          uVar6 = local_1b0;
          uVar17 = *(uint *)(local_1b0 + 0x24);
          if (uVar17 != 0) {
            uVar10 = FUN_10007720();
            puVar20 = &DAT_1006a484;
            if (uVar17 < 0x2bd) {
              puVar20 = &DAT_10062710;
            }
            puVar21 = &DAT_1006a48c;
            if (piVar18[0x20] == 0) {
              puVar21 = &DAT_10062710;
            }
            FUN_10010080(piVar27,L"%s%s %up%s",0x23,puVar21,uVar10,uVar17,puVar20);
          }
          uVar17 = *(uint *)(uVar6 + 0x28);
          if (uVar17 != 0) {
            puVar20 = &DAT_1006a48c;
            if (piVar18[0x20] == 0) {
              puVar20 = &DAT_10062710;
            }
            FUN_10010080(piVar27,L"%s%u kbps",0xf,puVar20,uVar17 >> 10);
          }
          if (*(char *)(uVar6 + 0x1c) == '\x01') {
            uVar10 = FUN_10007720();
            puVar20 = &DAT_1006a3bc;
            if (piVar18[0x20] == 0) {
              puVar20 = &DAT_10062710;
            }
            FUN_10010080(piVar27,L"%s(%s)",0xf,puVar20,uVar10);
          }
          FUN_10019190(local_1a4);
          if (((!bVar3) || (!bVar4)) || (local_1c5 != '\0')) break;
          piVar27 = (int *)CONCAT31((int3)((uint)piVar27 >> 8),1);
          local_1c5 = '\x01';
        }
      }
      else if (local_164 != 0) {
        cVar7 = FUN_1000af60();
        if (cVar7 != '\0') {
          if (local_124 != 0) {
            cVar7 = FUN_1000af60();
            if (cVar7 == '\0') goto LAB_100117de;
          }
          if (0x16 < local_164) {
            iVar9 = strncmp(local_168,"https://strm.yandex.ru/",0x17);
            if (iVar9 == 0) {
              iVar9 = FUN_10006420(0x26,0xffffffff,extraout_ECX_23);
              if ((-1 < iVar9) && (iVar9 + 0xbU <= local_164)) {
                iVar9 = strncmp(local_168 + iVar9,"&redundant=",0xb);
                if (iVar9 == 0) goto LAB_100117de;
              }
            }
          }
          goto LAB_10011260;
        }
      }
LAB_100117de:
      cVar7 = FUN_1002e4d0(&local_168,&local_128,&local_138,&local_110,&local_d4,&local_e4,&local_f4
                           ,&local_1b5);
      local_1b4 = (int *)CONCAT31((int3)((uint)piVar27 >> 8),cVar7);
    }
    if (local_f4 != &DAT_10074bf4) {
      free(local_f4);
    }
    if (local_e4 != &DAT_10074bf4) {
      free(local_e4);
    }
    if (local_d4 != &DAT_10074bf4) {
      free(local_d4);
    }
    if (local_110 != (byte *)&DAT_10074bf4) {
      free(local_110);
    }
    if (local_138 != &DAT_10074bf4) {
      free(local_138);
    }
    if (local_128 != &DAT_10074bf4) {
      free(local_128);
    }
    if (local_168 != (char *)&DAT_10074bf4) {
      free(local_168);
    }
  }
  if (local_1a0 != (char *)&DAT_10074bf4) {
    free(local_1a0);
  }
LAB_100118d9:
  FUN_1002e480();
  return;
}


