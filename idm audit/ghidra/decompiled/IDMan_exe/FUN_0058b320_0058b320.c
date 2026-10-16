// Function: FUN_0058b320
// Entry Point: 0x0058b320
// Program: IDMan.exe


/* WARNING: Removing unreachable block (ram,0x0058bb9b) */
/* WARNING: Removing unreachable block (ram,0x0058bbab) */

void __thiscall FUN_0058b320(int param_1,int param_2)

{
  undefined4 uVar1;
  uint uVar2;
  longlong lVar3;
  longlong lVar4;
  uint *puVar5;
  char *pcVar6;
  int iVar7;
  undefined1 *puVar8;
  undefined4 *puVar9;
  int *piVar10;
  undefined4 uVar11;
  char *pcVar12;
  undefined2 *puVar13;
  int iVar14;
  char *pcVar15;
  int iVar16;
  char cVar17;
  uint uVar18;
  bool bVar19;
  longlong lVar20;
  uint uStack_21c;
  undefined1 *local_1f8;
  char *local_1ec;
  undefined2 *local_1e8;
  undefined4 local_1e0;
  uint local_1dc;
  undefined4 local_1d8;
  undefined4 local_1d4;
  undefined4 local_1d0;
  undefined4 local_1cc;
  undefined4 local_1c8;
  undefined4 local_1c4;
  char *local_1c0;
  undefined4 local_1bc;
  char *local_1b8;
  undefined4 local_1b4;
  int local_1b0;
  undefined4 local_1ac;
  undefined1 local_1a5;
  undefined4 local_1a4;
  char local_19e;
  char local_19d;
  undefined1 *local_19c [2];
  char *local_194;
  uint local_190;
  undefined1 *local_18c;
  void *local_188;
  undefined1 *puStack_184;
  int local_180;
  CHAR local_17c [96];
  undefined1 local_11c [36];
  undefined1 local_f8 [36];
  char local_d4 [13];
  undefined2 local_c7 [57];
  char local_54 [16];
  CHAR local_44 [12];
  undefined1 local_38;
  undefined4 local_37;
  undefined4 local_33;
  undefined4 local_2f;
  undefined4 local_2b;
  undefined4 local_27;
  undefined4 local_23;
  undefined4 local_1f;
  undefined4 local_1b;
  uint local_14;
  int local_10;
  uint local_8;
  
  puStack_184 = &LAB_0067f69b;
  local_188 = ExceptionList;
  uStack_21c = DAT_007839c0 ^ (uint)local_17c;
  local_18c = (undefined1 *)&uStack_21c;
  ExceptionList = &local_188;
  local_190 = 0;
  local_180 = 0;
  local_19d = '\0';
  local_19e = '\0';
  *(undefined1 *)(param_1 + 0x32d4) = 0;
  iVar7 = *(int *)(param_1 + 200);
  pcVar6 = *(char **)(iVar7 + 0x3a8);
  pcVar15 = pcVar6 + 1;
  do {
    cVar17 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar17 != '\0');
  local_8 = uStack_21c;
  if ((uint)((int)pcVar6 - (int)pcVar15) < 0xd) {
    puVar5 = &uStack_21c;
    if (*(char *)(iVar7 + 0x1be) == '\0') {
      if (*(char *)(iVar7 + 0x1bf) == '\0') {
        if ((*(int *)(param_1 + 0x32dc) == 0) ||
           (puVar5 = &uStack_21c, *(int *)(param_1 + 0x32e0) != 0)) {
          FUN_00588290(iVar7 + 0x1f8,*(int *)(param_1 + 0x32dc),*(undefined4 *)(iVar7 + 0xc),
                       *(undefined4 *)(iVar7 + 0x10),param_1 + 0x32e0,param_1 + 0x32d8,0);
          puVar5 = (uint *)local_18c;
        }
      }
      else {
        pcVar6 = *(char **)(iVar7 + 0x2fc);
        pcVar15 = pcVar6 + 1;
        do {
          cVar17 = *pcVar6;
          pcVar6 = pcVar6 + 1;
        } while (cVar17 != '\0');
        puVar5 = &uStack_21c;
        if (pcVar6 != pcVar15) {
          local_19d = '\x01';
          puVar5 = &uStack_21c;
        }
      }
    }
  }
  else {
    local_19e = '\x01';
    puVar5 = &uStack_21c;
  }
  local_18c = (undefined1 *)puVar5;
  uVar11 = DAT_00789d44;
  if (param_2 != 0) {
    uVar11 = DAT_00789d40;
  }
  FUN_00567380(uVar11,1);
  FUN_00586630();
  pcVar15 = *(char **)(param_1 + 0x323c);
  if (pcVar15 != (char *)0x0) {
    for (; cVar17 = *pcVar15, cVar17 != '\0'; pcVar15 = pcVar15 + 1) {
      if (((((cVar17 < 'a') || ('z' < cVar17)) && ((cVar17 < 'A' || ('Z' < cVar17)))) &&
          ((((((cVar17 < '0' || ('9' < cVar17)) && (cVar17 != ' ')) &&
             ((cVar17 != '!' && (cVar17 != '#')))) && (cVar17 != '$')) &&
           ((cVar17 != '%' && (cVar17 != '&')))))) &&
         (((cVar17 != '\'' &&
           ((((((cVar17 != '*' && (cVar17 != '+')) && (cVar17 != '-')) &&
              (((cVar17 != '.' && (cVar17 != '/')) &&
               ((cVar17 != ';' && ((cVar17 != ':' && (cVar17 != '=')))))))) && (cVar17 != '?')) &&
            (((cVar17 != '@' && (cVar17 != '^')) && (cVar17 != '_')))))) &&
          ((cVar17 != '|' && (cVar17 != '~')))))) {
        *pcVar15 = '_';
      }
    }
  }
  iVar7 = FUN_00587a60();
  if (iVar7 != 0) {
    ExceptionList = local_188;
    __security_check_cookie(local_8 ^ (uint)local_17c);
    return;
  }
  FUN_004024f0();
  local_180._0_1_ = 1;
  if (*(int *)(param_1 + 0xcc) == 0) {
    if (param_2 == 0) {
      if ((*(char *)(*(int *)(param_1 + 200) + 0x1a5) != '\0') ||
         (puVar8 = &DAT_006cfabc, *(char *)(*(int *)(param_1 + 200) + 0x1b8) != '\0')) {
        puVar8 = &DAT_006cfac0;
      }
    }
    else {
      puVar8 = &DAT_006cfab4;
    }
  }
  else {
    puVar8 = &DAT_006cfac0;
    if (*(char *)(*(int *)(param_1 + 0xcc) + 0x14) == '\0') {
      puVar8 = &DAT_006cfabc;
    }
  }
  FUN_00402f80(puVar8);
  if (local_19d != '\0') {
    *(undefined1 *)(param_1 + 0x32d4) = 1;
    FUN_004024f0();
    local_180._0_1_ = 2;
    FUN_00402f20(&local_194,&DAT_006cfaac,*(undefined4 *)(*(int *)(param_1 + 200) + 0x314));
    piVar10 = (int *)(*(int *)(param_1 + 200) + 0x314);
    *piVar10 = *piVar10 + 1;
    local_38 = 0;
    local_37 = 0;
    local_33 = 0;
    local_2f = 0;
    local_2b = 0;
    local_27 = 0;
    local_23 = 0;
    local_1f = 0;
    local_1b = 0;
    iVar7 = *(int *)(param_1 + 200);
    local_1b0 = *(undefined4 *)(iVar7 + 0x308);
    FUN_0056f130(local_1b0,*(undefined4 *)(iVar7 + 0xc),*(undefined4 *)(iVar7 + 0x304),
                 *(undefined4 *)(iVar7 + 0x10),*(undefined4 *)(iVar7 + 0x2fc),
                 *(undefined4 *)(iVar7 + 0x300),local_11c);
    iVar7 = *(int *)(param_1 + 200);
    FUN_0056f2f0(local_11c,*(undefined4 *)(iVar7 + 0x2fc),local_194,*(undefined4 *)(iVar7 + 0x300),
                 *(undefined4 *)(iVar7 + 0x30c),local_1c4,*(undefined4 *)(param_1 + 0x3238),
                 &local_38,local_f8);
    FUN_004024f0();
    local_180._0_1_ = 3;
    FUN_004024f0();
    local_180 = CONCAT31(local_180._1_3_,4);
    iVar7 = *(int *)(param_1 + 200);
    pcVar6 = *(char **)(iVar7 + 0x30c);
    pcVar15 = pcVar6 + 1;
    do {
      cVar17 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar17 != '\0');
    if (pcVar6 != pcVar15) {
      FUN_00402f20(local_19c,"qop=%s, nc=%s, cnonce=\"%s\", ",*(undefined4 *)(iVar7 + 0x30c),
                   local_194,*(undefined4 *)(iVar7 + 0x300));
    }
    pcVar6 = *(char **)(*(int *)(param_1 + 200) + 0x310);
    pcVar15 = pcVar6 + 1;
    do {
      cVar17 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar17 != '\0');
    if (pcVar6 != pcVar15) {
      FUN_00402f20(&local_1b8,", opaque=\"%s\"",*(undefined4 *)(*(int *)(param_1 + 200) + 0x310));
    }
    pcVar15 = local_1b8;
    do {
      cVar17 = *pcVar15;
      pcVar15 = pcVar15 + 1;
    } while (cVar17 != '\0');
    if (pcVar15 == local_1b8 + 1) {
      puVar9 = (undefined4 *)FUN_004030a0(&DAT_006979dc);
      local_180 = CONCAT31(local_180._1_3_,5);
      local_190 = local_190 | 1;
      local_1b8 = (char *)*puVar9;
    }
    pcVar6 = *(char **)(*(int *)(param_1 + 200) + 0x30c);
    pcVar15 = pcVar6 + 1;
    do {
      cVar17 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar17 != '\0');
    puVar8 = local_19c[0];
    if (pcVar6 == pcVar15) {
      piVar10 = (int *)FUN_004030a0(&DAT_006979dc);
      local_180 = 6;
      local_190 = local_190 | 2;
      puVar8 = (undefined1 *)*piVar10;
    }
    iVar7 = *(int *)(param_1 + 200);
    FUN_00402f20(param_1 + 0x32d8,
                 "Authorization: Digest username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", %sresponse=\"%s\"%s\r\n"
                 ,*(undefined4 *)(iVar7 + 0xc),*(undefined4 *)(iVar7 + 0x304),
                 *(undefined4 *)(iVar7 + 0x2fc),*(undefined4 *)(param_1 + 0x3238),puVar8,local_f8,
                 local_1b8);
    local_180 = 5;
    if ((local_190 & 2) != 0) {
      local_190 = local_190 & 0xfffffffd;
      FUN_00402560();
    }
    local_180 = 4;
    if ((local_190 & 1) != 0) {
      local_190 = local_190 & 0xfffffffe;
      FUN_00402560();
    }
    local_180._0_1_ = 3;
    FUN_00402560();
    local_180._0_1_ = 2;
    FUN_00402560();
    local_180._0_1_ = 1;
    FUN_00402560();
  }
  iVar7 = *(int *)(param_1 + 200);
  local_1dc = (uint)(*(char *)(iVar7 + 0x351) != '\0');
  *(undefined4 *)(param_1 + 0x32b0) = 0;
  *(undefined4 *)(param_1 + 0x32b4) = 0;
  *(undefined4 *)(param_1 + 0x68) = 0xffffffff;
  *(undefined4 *)(param_1 + 0x6c) = 0xffffffff;
  if ((*(int *)(iVar7 + 0x348) == 8) && (iVar14 = *(int *)(param_1 + 0xa8), iVar14 != 0)) {
    uVar18 = *(uint *)(iVar14 + 0x50);
    if (uVar18 == 0 && *(int *)(iVar14 + 0x54) == 0) goto LAB_0058b823;
    iVar7 = uVar18 + *(uint *)(iVar14 + 0x48);
    iVar16 = (*(int *)(iVar14 + 0x54) + *(int *)(iVar14 + 0x4c) +
             (uint)CARRY4(uVar18,*(uint *)(iVar14 + 0x48))) - (uint)(iVar7 == 0);
    *(int *)(param_1 + 0x68) = iVar7 + -1;
    *(int *)(param_1 + 0x6c) = iVar16;
    _sprintf(local_d4,"Range: bytes=%I64d-%I64d\r\n",*(undefined4 *)(iVar14 + 0x48),
             *(undefined4 *)(iVar14 + 0x4c),*(undefined4 *)(param_1 + 0x68),iVar16);
  }
  else {
LAB_0058b823:
    if ((((*(int *)(iVar7 + 0x348) == 0x40) && (*(int *)(param_1 + 0x3208) != 0)) &&
        (iVar14 = *(int *)(*(int *)(param_1 + 0x3208) + 0x40), iVar14 != 0)) &&
       ((*(int *)(iVar14 + 0x58) != 0 && (iVar14 = *(int *)(param_1 + 0xa8), iVar14 != 0)))) {
      *(undefined4 *)(param_1 + 0x60) = *(undefined4 *)(iVar14 + 0x38);
      *(undefined4 *)(param_1 + 100) = *(undefined4 *)(iVar14 + 0x3c);
      iVar7 = *(int *)(iVar14 + 0x40);
      iVar14 = *(int *)(iVar14 + 0x44);
      if ((iVar14 < 0) || ((iVar14 < 1 && (iVar7 == 0)))) {
        _sprintf(local_d4,"Range: bytes=%I64d-\r\n",*(undefined4 *)(param_1 + 0x60),
                 *(undefined4 *)(param_1 + 100));
      }
      else {
        *(int *)(param_1 + 0x78) = iVar7;
        *(int *)(param_1 + 0x7c) = iVar14;
        *(int *)(param_1 + 0x68) = iVar7;
        *(undefined4 *)(param_1 + 0x6c) = *(undefined4 *)(param_1 + 0x7c);
        _sprintf(local_d4,"Range: bytes=%I64d-%I64d\r\n",*(undefined4 *)(param_1 + 0x60),
                 *(undefined4 *)(param_1 + 100),iVar7,*(undefined4 *)(param_1 + 0x7c));
      }
    }
    else if (*(char *)(param_1 + 0x3210) == '\0') {
      uVar18 = *(uint *)(param_1 + 0x60);
      iVar14 = *(int *)(param_1 + 100);
      if ((iVar14 < 1) && ((iVar14 < 0 || (uVar18 == 0)))) {
        if ((((*(int *)(iVar7 + 0x33c) == 0x40) || (*(int *)(iVar7 + 0x33c) == 0x80)) &&
            (iVar14 = *(int *)(param_1 + 0x3208), iVar14 != 0)) &&
           (*(char *)(iVar14 + 0x25a) != '\0')) {
          uVar11 = *(undefined4 *)(iVar14 + 0x250);
          *(undefined4 *)(param_1 + 0x78) = uVar11;
          uVar1 = *(undefined4 *)(iVar14 + 0x254);
          *(undefined4 *)(param_1 + 0x7c) = uVar1;
          *(undefined4 *)(param_1 + 0x68) = uVar11;
          *(undefined4 *)(param_1 + 0x6c) = uVar1;
          _sprintf(local_d4,"Range: bytes=%I64d-%I64d\r\n",*(undefined4 *)(iVar14 + 0x248),
                   *(undefined4 *)(iVar14 + 0x24c),*(undefined4 *)(iVar14 + 0x250),
                   *(undefined4 *)(iVar14 + 0x254));
        }
        else if ((*(char *)(param_1 + 0xe2) == '\0') && (*(char *)(iVar7 + 0x1c1) == '\0')) {
          if (((*(char *)(iVar7 + 0x1a0) == '\0') || (*(int *)(iVar7 + 0x31c) < 0)) ||
             ((*(int *)(iVar7 + 0x31c) < 1 && (*(int *)(iVar7 + 0x318) == 0)))) {
            if ((*(int *)(param_1 + 0x3208) != 0) &&
               (*(int *)(*(int *)(param_1 + 0x3208) + 0x44) != 0)) {
              iVar7 = *(int *)(param_1 + 0x78);
              iVar14 = *(int *)(param_1 + 0x7c);
              if ((-1 < iVar14) && ((0 < iVar14 || (iVar7 != 0)))) {
                *(int *)(param_1 + 0x68) = iVar7;
                *(int *)(param_1 + 0x6c) = iVar14;
                _sprintf(local_d4,"Range: bytes=0-%I64d\r\n",iVar7,iVar14);
                goto LAB_0058bc68;
              }
            }
            _sprintf(local_d4,"");
            *(undefined1 *)(param_1 + 0x320c) = 0;
          }
          else {
            uVar11 = *(undefined4 *)(iVar7 + 0x318);
            *(undefined4 *)(param_1 + 0x78) = uVar11;
            uVar1 = *(undefined4 *)(iVar7 + 0x31c);
            *(undefined4 *)(param_1 + 0x7c) = uVar1;
            *(undefined4 *)(param_1 + 0x68) = uVar11;
            *(undefined4 *)(param_1 + 0x6c) = uVar1;
            _sprintf(local_d4,"Range: bytes=0-%I64d\r\n",*(undefined4 *)(iVar7 + 0x318),
                     *(undefined4 *)(iVar7 + 0x31c));
          }
        }
        else {
          _sprintf(local_d4,"Range: bytes=0-\r\n");
        }
      }
      else {
        if (*(char *)(iVar7 + 0x374) == '\0') {
          iVar16 = *(int *)(param_1 + 0x3208);
          if (iVar16 == 0) {
            local_19c[0] = (undefined1 *)0x0;
            iVar16 = 0;
          }
          else {
            local_19c[0] = *(undefined1 **)(iVar16 + 0xb0);
            iVar16 = *(int *)(iVar16 + 0x44);
          }
          if ((((*(int *)(param_1 + 0x58) == 0 && *(int *)(param_1 + 0x5c) == 0) ||
               (local_19c[0] == (undefined1 *)0x0)) || (*(int *)(iVar7 + 800) == 0)) &&
             (iVar16 == 0)) {
            if (*(int *)(iVar7 + 800) == 0) {
              local_1dc = 1;
            }
            if (*(char *)(param_1 + 0xd8) != '\0') {
              bVar19 = 0xffffffcf < uVar18;
              uVar18 = uVar18 + 0x30;
              iVar14 = iVar14 + (uint)bVar19;
            }
            *(uint *)(param_1 + 0x32b0) = uVar18;
            *(int *)(param_1 + 0x32b4) = iVar14;
            if (*(char *)(param_1 + 0xdf) != '\0') {
              uVar2 = *(uint *)(param_1 + 0x194);
              iVar14 = (int)uVar2 >> 0x1f;
              iVar7 = *(int *)(param_1 + 0x32b4);
              if ((iVar7 < iVar14) || ((iVar7 <= iVar14 && (uVar18 < uVar2)))) {
                *(undefined1 *)(param_1 + 0xdf) = 0;
              }
              else {
                *(uint *)(param_1 + 0x32b0) = uVar18 - uVar2;
                *(uint *)(param_1 + 0x32b4) = (iVar7 - iVar14) - (uint)(uVar18 < uVar2);
              }
            }
            _sprintf(local_d4,"Range: bytes=%I64d-\r\n",*(undefined4 *)(param_1 + 0x32b0),
                     *(undefined4 *)(param_1 + 0x32b4));
            *(undefined1 *)(param_1 + 0x3218) = 0;
            goto LAB_0058bc68;
          }
          lVar20 = FUN_005803d0();
          lVar3 = lVar20;
          if (*(int *)(param_1 + 0x3208) != 0) {
            if (*(char *)(*(int *)(param_1 + 0x3208) + 0x25f) == '\0') {
              iVar14 = *(int *)(param_1 + 0x58) + -1;
              iVar7 = *(int *)(param_1 + 0x5c) - (uint)(*(int *)(param_1 + 0x58) == 0);
              lVar4 = CONCAT44(iVar7,iVar14);
              if ((lVar20 < lVar4) && (lVar3 = CONCAT44(iVar7,iVar14), lVar20 + 0x40 < lVar4)) {
                lVar3 = lVar20 + 0x40;
              }
            }
            else {
              lVar3 = lVar20 + 1;
            }
          }
          uVar18 = *(uint *)(param_1 + 0x60);
          iVar7 = *(int *)(param_1 + 100);
          if (*(char *)(param_1 + 0xd8) != '\0') {
            bVar19 = 0xffffffcf < uVar18;
            uVar18 = uVar18 + 0x30;
            iVar7 = iVar7 + (uint)bVar19;
          }
          *(uint *)(param_1 + 0x32b0) = uVar18;
          *(int *)(param_1 + 0x32b4) = iVar7;
          if (*(char *)(param_1 + 0xdf) != '\0') {
            local_14 = *(uint *)(param_1 + 0x194);
            local_10 = (int)local_14 >> 0x1f;
            uVar18 = *(uint *)(param_1 + 0x32b0);
            iVar7 = *(int *)(param_1 + 0x32b4);
            if ((iVar7 < local_10) || ((iVar7 <= local_10 && (uVar18 < local_14)))) {
              *(undefined1 *)(param_1 + 0xdf) = 0;
            }
            else {
              *(uint *)(param_1 + 0x32b0) = uVar18 - local_14;
              *(uint *)(param_1 + 0x32b4) = (iVar7 - local_10) - (uint)(uVar18 < local_14);
            }
          }
          *(longlong *)(param_1 + 0x68) = lVar3;
          iVar14 = *(int *)(param_1 + 0x32b4);
          uVar18 = *(uint *)(param_1 + 0x32b0);
        }
        else {
          lVar3 = *(longlong *)(param_1 + 0x78);
          *(undefined4 *)(param_1 + 0x68) = *(undefined4 *)(param_1 + 0x78);
          *(undefined4 *)(param_1 + 0x6c) = *(undefined4 *)(param_1 + 0x7c);
        }
        _sprintf(local_d4,"Range: bytes=%I64d-%I64d\r\n",uVar18,iVar14,(int)lVar3,
                 (int)((ulonglong)lVar3 >> 0x20));
        *(undefined1 *)(param_1 + 0x3218) = 1;
      }
    }
    else {
      _sprintf(local_d4,"Range: bytes=0-\r\n");
    }
  }
LAB_0058bc68:
  iVar7 = *(int *)(param_1 + 0xcc);
  if (iVar7 == 0) {
    iVar14 = *(int *)(*(int *)(param_1 + 200) + 0x14);
    local_1b0 = *(int *)(*(int *)(param_1 + 200) + 0x24);
  }
  else {
    iVar14 = *(int *)(iVar7 + 0xc);
    local_1b0 = *(int *)(iVar7 + 0x10);
  }
  if (iVar14 == 0x50) {
    if (local_1b0 == 1) goto LAB_0058bd3a;
LAB_0058bca7:
    _sprintf(local_54,":%ld",iVar14);
  }
  else {
    if ((iVar14 != 0x1bb) || (local_1b0 != 2)) goto LAB_0058bca7;
LAB_0058bd3a:
    _sprintf(local_54,"");
  }
  FUN_004024f0();
  local_180 = CONCAT31(local_180._1_3_,7);
  iVar7 = *(int *)(param_1 + 200);
  if ((*(char *)(iVar7 + 0x366) == '\0') || (*(char *)(iVar7 + 0x368) == '\0')) {
    if (*(char **)(iVar7 + 0x18) == (char *)0x0) {
      if (*(char *)(iVar7 + 0x373) != '\0') {
        pcVar6 = *(char **)(iVar7 + 0x124);
        pcVar15 = pcVar6 + 1;
        do {
          cVar17 = *pcVar6;
          pcVar6 = pcVar6 + 1;
        } while (cVar17 != '\0');
        if (pcVar6 != pcVar15) {
          FUN_00402f20(&local_1ac,"Referer: %s\r\n",*(undefined4 *)(iVar7 + 0x124));
          goto LAB_0058bd94;
        }
      }
      goto LAB_0058bd8e;
    }
    iVar7 = __strnicmp(*(char **)(iVar7 + 0x18),"file://",7);
    if (iVar7 == 0) goto LAB_0058bd8e;
    FUN_00402f20(&local_1ac,"Referer: %s\r\n",*(undefined4 *)(*(int *)(param_1 + 200) + 0x18));
  }
  else {
LAB_0058bd8e:
    FUN_00402f80(&DAT_006979dc);
  }
LAB_0058bd94:
  FUN_004030a0(&DAT_006979dc);
  local_180._0_1_ = 8;
  if ((*(char *)(*(int *)(param_1 + 200) + 0x36b) != '\0') &&
     (pcVar15 = *(char **)(*(int *)(param_1 + 200) + 0x20), pcVar15 != (char *)0x0)) {
    pcVar6 = pcVar15 + 1;
    do {
      cVar17 = *pcVar15;
      pcVar15 = pcVar15 + 1;
    } while (cVar17 != '\0');
    for (iVar7 = (int)pcVar15 - (int)pcVar6; 1 < iVar7; iVar7 = iVar7 + -1) {
      pcVar15 = (char *)(*(int *)(*(int *)(param_1 + 200) + 0x20) + -1 + iVar7);
      cVar17 = *pcVar15;
      if ((cVar17 != '\n') && (cVar17 != '\r')) {
        FUN_00402f20(&local_1d4,"If-Modified-Since: %s\r\n",
                     *(undefined4 *)(*(int *)(param_1 + 200) + 0x20));
        break;
      }
      *pcVar15 = '\0';
    }
  }
  FUN_004024f0();
  local_180 = CONCAT31(local_180._1_3_,9);
  iVar7 = *(int *)(param_1 + 0xcc);
  if (iVar7 == 0) {
    pcVar6 = *(char **)(*(int *)(param_1 + 200) + 0xd0);
    pcVar15 = pcVar6 + 1;
    do {
      cVar17 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar17 != '\0');
    if (pcVar6 != pcVar15) {
      uVar11 = *(undefined4 *)(*(int *)(param_1 + 200) + 0xd0);
      goto LAB_0058be61;
    }
LAB_0058be75:
    FUN_00402f20(&local_1a4,"application/x-www-form-urlencoded");
  }
  else {
    pcVar6 = *(char **)(iVar7 + 0x1c);
    pcVar15 = pcVar6 + 1;
    do {
      cVar17 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar17 != '\0');
    if (pcVar6 == pcVar15) goto LAB_0058be75;
    uVar11 = *(undefined4 *)(iVar7 + 0x1c);
LAB_0058be61:
    FUN_00402f20(&local_1a4,&DAT_00696f08,uVar11);
  }
  FUN_004024f0();
  local_180._0_1_ = 10;
  iVar7 = *(int *)(param_1 + 0xcc);
  if (iVar7 == 0) {
    if (*(char *)(*(int *)(param_1 + 200) + 0x1a5) != '\0') {
      FUN_00402f20(&local_1bc,"Content-Type: %s\r\nContent-Length: %d\r\n",local_1a4,
                   *(undefined4 *)(*(int *)(*(int *)(param_1 + 200) + 0x1a8) + -0xc));
    }
    if (*(char *)(*(int *)(param_1 + 200) + 0x1b8) != '\0') {
      uVar11 = *(undefined4 *)(*(int *)(param_1 + 200) + 0x1b4);
      goto LAB_0058beed;
    }
  }
  else if (*(char *)(iVar7 + 0x14) != '\0') {
    uVar11 = *(undefined4 *)(*(int *)(iVar7 + 0x18) + -0xc);
LAB_0058beed:
    FUN_00402f20(&local_1bc,"Content-Type: %s\r\nContent-Length: %d\r\n",local_1a4,uVar11);
  }
  FUN_004024f0();
  local_180 = CONCAT31(local_180._1_3_,0xb);
  pcVar6 = *(char **)(*(int *)(param_1 + 200) + 0x38c);
  pcVar15 = pcVar6 + 1;
  do {
    cVar17 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar17 != '\0');
  if (pcVar6 != pcVar15) {
    FUN_00402f20(&local_1d8,"Cookie2: %s\r\n",*(undefined4 *)(*(int *)(param_1 + 200) + 0x38c));
  }
  pcVar6 = *(char **)(*(int *)(param_1 + 200) + 0x390);
  pcVar15 = pcVar6 + 1;
  do {
    cVar17 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar17 != '\0');
  if (pcVar6 == pcVar15) {
    if ((DAT_00788ccc == (int)pcVar6 - (int)pcVar15) ||
       (iVar7 = GetLocaleInfoA(0x400,0x5c,local_17c,0x5f), iVar7 == 0)) {
      iVar7 = GetLocaleInfoA(0x400,0x59,local_44,9);
      if ((iVar7 == 0) || (iVar7 = GetLocaleInfoA(0x400,0x5a,(LPSTR)&local_14,9), iVar7 == 0)) {
        FUN_00402f20(*(int *)(param_1 + 200) + 0x390,"en-US");
      }
      else {
        FUN_00402f20(*(int *)(param_1 + 200) + 0x390,"%s-%s",local_44,&local_14);
      }
    }
    else {
      FUN_00402f20(*(int *)(param_1 + 200) + 0x390,&DAT_00696f08,local_17c);
    }
    pcVar15 = *(char **)(*(int *)(param_1 + 200) + 0x390);
    pcVar6 = pcVar15;
    do {
      cVar17 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar17 != '\0');
    if (((2 < (uint)((int)pcVar6 - (int)(pcVar15 + 1))) &&
        (pcVar15 = _strchr(pcVar15,0x2d), pcVar15 != (char *)0x0)) &&
       (iVar7 = _isdigit((int)pcVar15[1]), iVar7 != 0)) {
      *pcVar15 = '\0';
    }
  }
  FUN_004024f0();
  local_180._0_1_ = 0xc;
  pcVar6 = *(char **)(*(int *)(param_1 + 200) + 0x390);
  pcVar15 = pcVar6 + 1;
  do {
    cVar17 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar17 != '\0');
  if (pcVar6 != pcVar15) {
    FUN_00402f20(&local_1e0,"Accept-Language: %s\r\n",
                 *(undefined4 *)(*(int *)(param_1 + 200) + 0x390));
  }
  FUN_004024f0();
  local_180 = CONCAT31(local_180._1_3_,0xd);
  pcVar15 = _strrchr((char *)**(undefined4 **)(param_1 + 200),0x2e);
  if ((pcVar15 != (char *)0x0) && (iVar7 = __stricmp(pcVar15,".monster"), iVar7 == 0)) {
    *(undefined1 *)(*(int *)(param_1 + 200) + 0x371) = 1;
  }
  iVar7 = *(int *)(param_1 + 200);
  pcVar6 = *(char **)(iVar7 + 0x398);
  pcVar15 = pcVar6 + 1;
  do {
    cVar17 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar17 != '\0');
  if (pcVar6 == pcVar15) {
    if (*(char *)(iVar7 + 0x371) == '\0') {
      pcVar6 = *(char **)(iVar7 + 0x124);
      pcVar15 = pcVar6 + 1;
      do {
        cVar17 = *pcVar6;
        pcVar6 = pcVar6 + 1;
      } while (cVar17 != '\0');
      if (pcVar6 == pcVar15) {
        if ((*(char **)(iVar7 + 0x18) != (char *)0x0) && (*(char *)(iVar7 + 0x366) == '\0')) {
          local_1a5 = 0;
          pcVar15 = _strrchr(*(char **)(iVar7 + 0x18),0x2e);
          if ((pcVar15 == (char *)0x0) || (iVar7 = __strnicmp(pcVar15,".swf",4), iVar7 != 0)) {
            FUN_004024f0();
            local_180 = CONCAT31(local_180._1_3_,0xf);
            uVar11 = *(undefined4 *)(*(int *)(param_1 + 200) + 0x18);
            goto LAB_0058c1a5;
          }
          local_1a5 = 1;
        }
      }
      else {
        FUN_004024f0();
        local_180 = CONCAT31(local_180._1_3_,0xe);
        uVar11 = *(undefined4 *)(*(int *)(param_1 + 200) + 0x124);
LAB_0058c1a5:
        FUN_00402f20(&local_194,&DAT_00696f08,uVar11);
        pcVar15 = _strstr(local_194,"://");
        if ((pcVar15 != (char *)0x0) &&
           (pcVar15 = _strpbrk(pcVar15 + 3,":/"), pcVar15 != (char *)0x0)) {
          *pcVar15 = '\0';
          FUN_00402f20(&local_1cc,"Origin: %s\r\n",local_194);
        }
        local_180 = CONCAT31(local_180._1_3_,0xd);
        FUN_00402560();
      }
    }
  }
  else {
    FUN_00402f20(&local_1cc,"Origin: %s\r\n",*(undefined4 *)(iVar7 + 0x398));
  }
  FUN_004024f0();
  local_180 = CONCAT31(local_180._1_3_,0x10);
  iVar7 = *(int *)(param_1 + 200);
  pcVar6 = *(char **)(iVar7 + 0x3a0);
  pcVar15 = pcVar6 + 1;
  do {
    cVar17 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar17 != '\0');
  if (pcVar6 == pcVar15) {
    if (*(char *)(iVar7 + 0x372) == '\0') {
      pcVar15 = "Accept-Charset: *\r\n";
    }
    else {
      pcVar15 = "";
    }
    FUN_00402f20(&local_1c8,pcVar15);
  }
  else {
    FUN_00402f20(&local_1c8,"Accept-Charset: %s\r\n",*(undefined4 *)(iVar7 + 0x3a0));
  }
  FUN_004024f0();
  local_180 = CONCAT31(local_180._1_3_,0x11);
  iVar7 = *(int *)(param_1 + 200);
  if (*(char *)(iVar7 + 0x36e) == '\0') {
    if (((*(char *)(iVar7 + 0x361) == '\0') && (*(char *)(iVar7 + 0x362) == '\0')) ||
       (*(char *)(iVar7 + 0x364) != '\0')) {
      pcVar6 = *(char **)(iVar7 + 0x420);
      pcVar15 = pcVar6 + 1;
      do {
        cVar17 = *pcVar6;
        pcVar6 = pcVar6 + 1;
      } while (cVar17 != '\0');
      if ((pcVar6 != pcVar15) && (*(char *)(iVar7 + 0x375) != '\0')) {
        FUN_00402f20(&local_1b4,"Accept-Encoding: %s\r\n",*(undefined4 *)(iVar7 + 0x420));
        goto LAB_0058c2f1;
      }
      pcVar15 = "Accept-Encoding: identity\r\n";
    }
    else {
      pcVar15 = "";
    }
  }
  else {
    pcVar15 = "Accept-Encoding: gzip, deflate\r\n";
  }
  FUN_00402f20(&local_1b4,pcVar15);
LAB_0058c2f1:
  FUN_004024f0();
  local_180._0_1_ = 0x12;
  iVar7 = *(int *)(param_1 + 200);
  if (*(int *)(iVar7 + 0x180) == 0) {
    if (*(int *)(param_1 + 0xcc) == 0) {
      puVar9 = (undefined4 *)(iVar7 + 0x1a8);
    }
    else {
      puVar9 = (undefined4 *)(*(int *)(param_1 + 0xcc) + 0x18);
    }
    local_19c[0] = (undefined1 *)*puVar9;
  }
  else {
    local_19c[0] = &DAT_006979dc;
  }
  pcVar6 = *(char **)(iVar7 + 0x3a4);
  pcVar15 = pcVar6 + 1;
  do {
    cVar17 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar17 != '\0');
  if ((pcVar6 == pcVar15) || (*(char *)(iVar7 + 0x36d) == '\0')) {
    puVar9 = (undefined4 *)FUN_004030a0(&DAT_006979dc);
    local_180._0_1_ = 0x13;
    local_190 = local_190 | 4;
    local_1b8 = (char *)*puVar9;
  }
  else {
    local_1b8 = *(char **)(iVar7 + 0x3a4);
  }
  local_194 = "Cache-Control: no-cache\r\nPragma: no-cache\r\n";
  if (local_1dc == 0) {
    local_194 = "";
  }
  puVar8 = *(undefined1 **)(param_1 + 0x323c);
  local_1e8 = &DAT_00699144;
  local_1f8 = puVar8;
  if (puVar8 == (undefined1 *)0x0) {
    local_1e8 = (undefined2 *)&DAT_006979dc;
    local_1f8 = &DAT_006979dc;
  }
  local_1ec = "Cookie: ";
  if (puVar8 == (undefined1 *)0x0) {
    local_1ec = "";
  }
  iVar7 = *(int *)(param_1 + 200);
  if (local_19e == '\0') {
    if (*(char *)(iVar7 + 0x1be) == '\0') {
      local_1d0 = *(undefined4 *)(param_1 + 0x32d8);
    }
    else {
      local_1d0 = *(undefined4 *)(param_1 + 0x324c);
    }
  }
  else {
    local_1d0 = *(undefined4 *)(iVar7 + 0x3a8);
  }
  if (((*(int *)(*(int *)(param_1 + 0x32d8) + -0xc) == 0) || (*(char *)(iVar7 + 0x1be) != '\0')) ||
     (pcVar15 = "Connection: Keep-Alive\r\n", *(int *)(param_1 + 0x32dc) != 0)) {
    pcVar15 = "";
  }
  pcVar12 = *(char **)(iVar7 + 0x39c);
  pcVar6 = pcVar12 + 1;
  do {
    cVar17 = *pcVar12;
    pcVar12 = pcVar12 + 1;
  } while (cVar17 != '\0');
  if (pcVar12 == pcVar6) {
    puVar9 = (undefined4 *)FUN_004030a0(&DAT_006cf818);
    local_180._0_1_ = 0x14;
    local_180._1_3_ = 0;
    local_190 = local_190 | 8;
    uVar11 = *puVar9;
  }
  else {
    uVar11 = *(undefined4 *)(iVar7 + 0x39c);
  }
  puVar9 = *(undefined4 **)(param_1 + 0xcc);
  if (puVar9 == (undefined4 *)0x0) {
    puVar9 = (undefined4 *)FUN_004030a0(**(undefined4 **)(param_1 + 200));
    local_180._0_1_ = 0x15;
    local_180._1_3_ = 0;
    local_190 = local_190 | 0x10;
  }
  FUN_00402f20(&local_1c0,
               "%s %s HTTP/1.1\r\nHost: %s%s\r\nUser-Agent: %s\r\nAccept: %s\r\n%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\r\n%s"
               ,local_1c4,*(undefined4 *)(param_1 + 0x3238),*puVar9,local_54,
               *(undefined4 *)(*(int *)(param_1 + 200) + 0x1ac),uVar11,local_1b4,local_1e0,local_1c8
               ,local_1cc,pcVar15,local_1d4,local_d4,local_1ac,local_1d0,local_1ec,local_1f8,
               local_1e8,local_1d8,local_1bc,local_194,local_1b8,local_19c[0]);
  local_180 = 0x14;
  uVar18 = local_190;
  if ((local_190 & 0x10) != 0) {
    uVar18 = local_190 & 0xffffffef;
    local_190 = uVar18;
    FUN_00402560();
  }
  local_180 = 0x13;
  if ((uVar18 & 8) != 0) {
    uVar18 = uVar18 & 0xfffffff7;
    local_190 = uVar18;
    FUN_00402560();
  }
  local_180 = 0x12;
  if ((uVar18 & 4) != 0) {
    local_190 = uVar18 & 0xfffffffb;
    FUN_00402560();
  }
  if (((0 < *(int *)(param_1 + 100)) ||
      ((-1 < *(int *)(param_1 + 100) && (*(int *)(param_1 + 0x60) != 0)))) ||
     (*(int *)(param_1 + 0x78) != 0 || *(int *)(param_1 + 0x7c) != 0)) {
    FUN_004024f0();
    local_180._0_1_ = 0x16;
    pcVar15 = local_d4;
    do {
      cVar17 = *pcVar15;
      pcVar15 = pcVar15 + 1;
    } while (cVar17 != '\0');
    puVar13 = local_c7;
    if (pcVar15 == local_d4 + 1) {
      puVar13 = &DAT_00699144;
    }
    FUN_00402f20(local_19c,"%I64d-%I64d, %s",*(undefined4 *)(param_1 + 0x60),
                 *(undefined4 *)(param_1 + 100),*(undefined4 *)(param_1 + 0x78),
                 *(undefined4 *)(param_1 + 0x7c),puVar13);
    FUN_00566b20(local_19c[0]);
    local_180 = CONCAT31(local_180._1_3_,0x12);
    FUN_00402560();
  }
  *(undefined4 *)(param_1 + 0x3270) = 0;
  *(undefined1 *)(param_1 + 0xe0) = 0;
  *(undefined4 *)(param_1 + 0x3204) = 1;
  FUN_00566d80();
  *(undefined4 *)(param_1 + 0xa4) = 4;
  if (local_1b0 == 2) {
    iVar7 = *(int *)(param_1 + 200);
    if (*(int *)(iVar7 + 0x180) == 0) {
      pcVar15 = local_1c0;
      do {
        cVar17 = *pcVar15;
        pcVar15 = pcVar15 + 1;
      } while (cVar17 != '\0');
      FUN_005f3130(local_1c0,(int)pcVar15 - (int)(local_1c0 + 1));
      iVar7 = *(int *)(param_1 + 200);
      if ((*(char *)(iVar7 + 0x1b8) != '\0') && (*(int *)(iVar7 + 0x1b0) != 0)) {
        FUN_005f3130(*(int *)(iVar7 + 0x1b0),*(undefined4 *)(iVar7 + 0x1b4));
      }
    }
    else {
      pcVar15 = (char *)0x0;
      iVar14 = 0;
      if ((*(char *)(iVar7 + 0x1b8) == '\0') || (*(char **)(iVar7 + 0x1b0) == (char *)0x0)) {
        iVar16 = *(int *)(param_1 + 0xcc);
        if (iVar16 == 0) {
          cVar17 = *(char *)(iVar7 + 0x1a5);
        }
        else {
          cVar17 = *(char *)(iVar16 + 0x14);
        }
        if (cVar17 != '\0') {
          if (iVar16 == 0) {
            puVar9 = (undefined4 *)(iVar7 + 0x1a8);
          }
          else {
            puVar9 = (undefined4 *)(iVar16 + 0x18);
          }
          pcVar15 = (char *)*puVar9;
          pcVar6 = pcVar15;
          do {
            cVar17 = *pcVar6;
            pcVar6 = pcVar6 + 1;
          } while (cVar17 != '\0');
          iVar14 = (int)pcVar6 - (int)(pcVar15 + 1);
        }
      }
      else {
        iVar14 = *(int *)(iVar7 + 0x1b4);
        pcVar15 = *(char **)(iVar7 + 0x1b0);
      }
      FUN_005f7680(&local_1c0,pcVar15,iVar14);
    }
  }
  else {
    pcVar15 = "send() HEAD";
    if (param_2 == 0) {
      pcVar15 = "send() GET";
    }
    iVar7 = send(*(SOCKET *)(param_1 + 0x3200),local_1c0,*(int *)(local_1c0 + -0xc),0);
    FUN_00567470(iVar7,pcVar15);
    iVar7 = *(int *)(param_1 + 200);
    if ((*(char *)(iVar7 + 0x1b8) != '\0') && (*(char **)(iVar7 + 0x1b0) != (char *)0x0)) {
      send(*(SOCKET *)(param_1 + 0x3200),*(char **)(iVar7 + 0x1b0),*(int *)(iVar7 + 0x1b4),0);
    }
  }
  local_180._0_1_ = 0x11;
  FUN_00402560();
  local_180._0_1_ = 0x10;
  FUN_00402560();
  local_180._0_1_ = 0xd;
  FUN_00402560();
  local_180._0_1_ = 0xc;
  FUN_00402560();
  local_180._0_1_ = 0xb;
  FUN_00402560();
  local_180._0_1_ = 10;
  FUN_00402560();
  local_180._0_1_ = 9;
  FUN_00402560();
  local_180._0_1_ = 8;
  FUN_00402560();
  local_180._0_1_ = 7;
  FUN_00402560();
  local_180._0_1_ = 1;
  FUN_00402560();
  local_180 = (uint)local_180._1_3_ << 8;
  FUN_00402560();
  FUN_0058c7b4();
  return;
}


