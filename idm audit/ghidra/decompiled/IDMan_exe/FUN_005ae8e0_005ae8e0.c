// Function: FUN_005ae8e0
// Entry Point: 0x005ae8e0
// Program: IDMan.exe


/* WARNING: Removing unreachable block (ram,0x005af41f) */
/* WARNING: Removing unreachable block (ram,0x005af42f) */

void __thiscall FUN_005ae8e0(int *param_1,int param_2)

{
  uint uVar1;
  longlong lVar2;
  longlong lVar3;
  uint *puVar4;
  char cVar5;
  char *pcVar6;
  int iVar7;
  undefined1 *puVar8;
  undefined4 *puVar9;
  char *pcVar10;
  undefined2 *puVar11;
  int iVar12;
  char *pcVar13;
  undefined4 uVar14;
  int iVar15;
  uint uVar16;
  bool bVar17;
  undefined8 uVar18;
  longlong lVar19;
  uint uStack_21c;
  undefined4 local_1ec;
  uint local_1e8;
  undefined4 local_1e4;
  undefined4 local_1e0;
  undefined4 local_1dc;
  undefined4 local_1d8;
  undefined4 local_1d4;
  undefined4 local_1d0;
  undefined4 local_1cc;
  char *local_1c8;
  int local_1c4;
  char *local_1c0;
  undefined4 local_1bc;
  undefined4 local_1b8;
  char *local_1b4;
  undefined1 local_1ad;
  undefined4 local_1ac;
  undefined4 local_1a8;
  undefined2 *local_1a4;
  char local_19d;
  char *local_19c;
  char local_195;
  char *local_194;
  char *local_18c;
  uint local_188;
  undefined1 *local_184;
  void *local_180;
  undefined1 *puStack_17c;
  undefined4 local_178;
  CHAR local_174 [96];
  char local_114 [13];
  undefined2 local_107 [57];
  undefined1 local_94 [36];
  CHAR local_70 [12];
  undefined1 local_64;
  undefined4 local_63;
  undefined4 local_5f;
  undefined4 local_5b;
  undefined4 local_57;
  undefined4 local_53;
  undefined4 local_4f;
  undefined4 local_4b;
  undefined4 local_47;
  undefined1 local_40;
  undefined4 local_3f;
  undefined4 local_3b;
  undefined4 local_37;
  undefined4 local_33;
  undefined4 local_2f;
  undefined4 local_2b;
  undefined4 local_27;
  undefined4 local_23;
  char local_1c [8];
  uint local_14;
  int local_10;
  uint local_8;
  
  local_178 = 0xffffffff;
  puStack_17c = &LAB_00681088;
  local_180 = ExceptionList;
  uStack_21c = DAT_007839c0 ^ (uint)local_174;
  local_184 = (undefined1 *)&uStack_21c;
  ExceptionList = &local_180;
  local_188 = 0;
  if (param_1[0x33] == 0) {
    iVar7 = *(int *)(param_1[0x32] + 0x24);
  }
  else {
    iVar7 = *(int *)(param_1[0x33] + 0x10);
  }
  local_8 = uStack_21c;
  if (iVar7 == 2) {
    local_184 = (undefined1 *)&uStack_21c;
    FUN_0058b320(param_2);
    FUN_005b0164();
    return;
  }
  local_195 = '\0';
  local_19d = '\0';
  *(undefined1 *)(param_1 + 0xcb5) = 0;
  iVar7 = param_1[0x32];
  pcVar6 = *(char **)(iVar7 + 0x3a8);
  pcVar13 = pcVar6 + 1;
  do {
    cVar5 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar5 != '\0');
  if ((uint)((int)pcVar6 - (int)pcVar13) < 0xd) {
    puVar4 = &uStack_21c;
    if (*(char *)(iVar7 + 0x1be) == '\0') {
      if (*(char *)(iVar7 + 0x1bf) == '\0') {
        if ((param_1[0xcb7] == 0) || (puVar4 = &uStack_21c, param_1[0xcb8] != 0)) {
          FUN_00588290(iVar7 + 0x1f8,param_1[0xcb7],*(undefined4 *)(iVar7 + 0xc),
                       *(undefined4 *)(iVar7 + 0x10),param_1 + 0xcb8,param_1 + 0xcb6,0);
          puVar4 = (uint *)local_184;
        }
      }
      else {
        pcVar6 = *(char **)(iVar7 + 0x2fc);
        pcVar13 = pcVar6 + 1;
        do {
          cVar5 = *pcVar6;
          pcVar6 = pcVar6 + 1;
        } while (cVar5 != '\0');
        puVar4 = &uStack_21c;
        if (pcVar6 != pcVar13) {
          local_195 = '\x01';
          puVar4 = &uStack_21c;
        }
      }
    }
  }
  else {
    local_19d = '\x01';
    puVar4 = &uStack_21c;
  }
  local_184 = (undefined1 *)puVar4;
  iVar7 = param_1[0x67];
  if ((*(int *)(iVar7 + 8) == 2) && ((param_1[0xcbe] == 0 || (param_1[0xcbf] != 0)))) {
    FUN_00588290(iVar7 + 0xc,param_1[0xcbe],*(undefined4 *)(iVar7 + 0x168),
                 *(undefined4 *)(iVar7 + 0x16c),param_1 + 0xcbf,param_1 + 0xcbb,1);
  }
  FUN_004024f0();
  local_178._0_1_ = 1;
  local_178._1_3_ = 0;
  uVar14 = DAT_00789d44;
  if (param_2 != 0) {
    uVar14 = DAT_00789d40;
  }
  FUN_00567380(uVar14,1);
  FUN_00586630();
  pcVar13 = (char *)param_1[0xc8f];
  if (pcVar13 != (char *)0x0) {
    for (; cVar5 = *pcVar13, cVar5 != '\0'; pcVar13 = pcVar13 + 1) {
      if (((((cVar5 < 'a') || ('z' < cVar5)) && ((cVar5 < 'A' || ('Z' < cVar5)))) &&
          ((((((cVar5 < '0' || ('9' < cVar5)) && (cVar5 != ' ')) &&
             ((cVar5 != '!' && (cVar5 != '#')))) && (cVar5 != '$')) &&
           ((cVar5 != '%' && (cVar5 != '&')))))) &&
         (((cVar5 != '\'' &&
           ((((((cVar5 != '*' && (cVar5 != '+')) && (cVar5 != '-')) &&
              (((cVar5 != '.' && (cVar5 != '/')) &&
               ((cVar5 != ';' && ((cVar5 != ':' && (cVar5 != '=')))))))) && (cVar5 != '?')) &&
            (((cVar5 != '@' && (cVar5 != '^')) && (cVar5 != '_')))))) &&
          ((cVar5 != '|' && (cVar5 != '~')))))) {
        *pcVar13 = '_';
      }
    }
  }
  FUN_005adff0();
  iVar7 = (**(code **)(*param_1 + 0x28))();
  if (iVar7 != 0) goto LAB_005b0122;
  FUN_004024f0();
  local_178._0_1_ = 2;
  if (param_1[0x33] == 0) {
    if (param_2 == 0) {
      if ((*(char *)(param_1[0x32] + 0x1a5) != '\0') ||
         (puVar8 = &DAT_006cfabc, *(char *)(param_1[0x32] + 0x1b8) != '\0')) {
        puVar8 = &DAT_006cfac0;
      }
    }
    else {
      puVar8 = &DAT_006cfab4;
    }
  }
  else {
    puVar8 = &DAT_006cfac0;
    if (*(char *)(param_1[0x33] + 0x14) == '\0') {
      puVar8 = &DAT_006cfabc;
    }
  }
  FUN_00402f80(puVar8);
  if (local_195 != '\0') {
    *(undefined1 *)(param_1 + 0xcb5) = 1;
    FUN_004024f0();
    local_178._0_1_ = 3;
    FUN_00402f20(&local_18c,&DAT_006cfaac,*(undefined4 *)(param_1[0x32] + 0x314));
    *(int *)(param_1[0x32] + 0x314) = *(int *)(param_1[0x32] + 0x314) + 1;
    local_64 = 0;
    local_63 = 0;
    local_5f = 0;
    local_5b = 0;
    local_57 = 0;
    local_53 = 0;
    local_4f = 0;
    local_4b = 0;
    local_47 = 0;
    iVar7 = param_1[0x32];
    local_194 = *(char **)(iVar7 + 0x308);
    FUN_0056f130(local_194,*(undefined4 *)(iVar7 + 0xc),*(undefined4 *)(iVar7 + 0x304),
                 *(undefined4 *)(iVar7 + 0x10),*(undefined4 *)(iVar7 + 0x2fc),
                 *(undefined4 *)(iVar7 + 0x300),&local_40);
    iVar7 = param_1[0x32];
    FUN_0056f2f0(&local_40,*(undefined4 *)(iVar7 + 0x2fc),local_18c,*(undefined4 *)(iVar7 + 0x300),
                 *(undefined4 *)(iVar7 + 0x30c),local_1cc,param_1[0xc8e],&local_64,local_94);
    FUN_004024f0();
    local_178._0_1_ = 4;
    FUN_004024f0();
    local_178 = CONCAT31(local_178._1_3_,5);
    iVar7 = param_1[0x32];
    pcVar6 = *(char **)(iVar7 + 0x30c);
    pcVar13 = pcVar6 + 1;
    do {
      cVar5 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar5 != '\0');
    if (pcVar6 != pcVar13) {
      FUN_00402f20(&local_1a4,"qop=%s, nc=%s, cnonce=\"%s\", ",*(undefined4 *)(iVar7 + 0x30c),
                   local_18c,*(undefined4 *)(iVar7 + 0x300));
    }
    pcVar13 = local_19c;
    do {
      cVar5 = *pcVar13;
      pcVar13 = pcVar13 + 1;
    } while (cVar5 != '\0');
    if (pcVar13 != local_19c + 1) {
      FUN_00402f20(&local_19c,", opaque=\"%s\"",local_19c);
    }
    pcVar13 = local_19c;
    do {
      cVar5 = *pcVar13;
      pcVar13 = pcVar13 + 1;
    } while (cVar5 != '\0');
    pcVar6 = local_19c;
    if (pcVar13 == local_19c + 1) {
      puVar9 = (undefined4 *)FUN_004030a0(&DAT_006979dc);
      local_178 = CONCAT31(local_178._1_3_,6);
      local_188 = local_188 | 1;
      pcVar6 = (char *)*puVar9;
    }
    pcVar10 = *(char **)(param_1[0x32] + 0x30c);
    pcVar13 = pcVar10 + 1;
    do {
      cVar5 = *pcVar10;
      pcVar10 = pcVar10 + 1;
    } while (cVar5 != '\0');
    puVar11 = local_1a4;
    if (pcVar10 == pcVar13) {
      puVar9 = (undefined4 *)FUN_004030a0(&DAT_006979dc);
      local_178 = 7;
      local_188 = local_188 | 2;
      puVar11 = (undefined2 *)*puVar9;
    }
    iVar7 = param_1[0x32];
    FUN_00402f20(param_1 + 0xcb6,
                 "Authorization: Digest username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", %sresponse=\"%s\"%s\r\n"
                 ,*(undefined4 *)(iVar7 + 0xc),*(undefined4 *)(iVar7 + 0x304),
                 *(undefined4 *)(iVar7 + 0x2fc),param_1[0xc8e],puVar11,local_94,pcVar6);
    local_178 = 6;
    if ((local_188 & 2) != 0) {
      local_188 = local_188 & 0xfffffffd;
      FUN_00402560();
    }
    local_178 = 5;
    if ((local_188 & 1) != 0) {
      local_188 = local_188 & 0xfffffffe;
      FUN_00402560();
    }
    local_178._0_1_ = 4;
    FUN_00402560();
    local_178._0_1_ = 3;
    FUN_00402560();
    local_178._0_1_ = 2;
    FUN_00402560();
  }
  *(undefined1 *)(param_1[0x67] + 0x134) = 0;
  iVar7 = param_1[0x67];
  if (*(int *)(iVar7 + 8) == 3) {
    pcVar6 = *(char **)(iVar7 + 0x118);
    pcVar13 = pcVar6 + 1;
    do {
      cVar5 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar5 != '\0');
    if (pcVar6 != pcVar13) {
      *(undefined1 *)(iVar7 + 0x134) = 1;
      FUN_004024f0();
      local_178._0_1_ = 8;
      FUN_00402f20(&local_18c,&DAT_006cfaac,*(undefined4 *)(param_1[0x67] + 0x130));
      *(int *)(param_1[0x67] + 0x130) = *(int *)(param_1[0x67] + 0x130) + 1;
      local_40 = 0;
      local_3f = 0;
      local_3b = 0;
      local_37 = 0;
      local_33 = 0;
      local_2f = 0;
      local_2b = 0;
      local_27 = 0;
      local_23 = 0;
      iVar7 = param_1[0x67];
      local_194 = *(char **)(iVar7 + 0x124);
      FUN_0056f130(local_194,*(undefined4 *)(iVar7 + 0x168),*(undefined4 *)(iVar7 + 0x120),
                   *(undefined4 *)(iVar7 + 0x16c),*(undefined4 *)(iVar7 + 0x118),
                   *(undefined4 *)(iVar7 + 0x11c),local_94);
      if ((*(char *)(param_1[0x32] + 0x1a5) != '\0') ||
         (puVar8 = &DAT_006cfabc, *(char *)(param_1[0x32] + 0x1b8) != '\0')) {
        puVar8 = &DAT_006cfac0;
      }
      iVar7 = param_1[0x67];
      FUN_0056f2f0(local_94,*(undefined4 *)(iVar7 + 0x118),local_18c,*(undefined4 *)(iVar7 + 0x11c),
                   *(undefined4 *)(iVar7 + 0x128),puVar8,param_1[0xc8e],&local_40,&local_64);
      FUN_004024f0();
      local_178._0_1_ = 9;
      FUN_004024f0();
      local_178._0_1_ = 10;
      FUN_004024f0();
      local_178 = CONCAT31(local_178._1_3_,0xb);
      iVar7 = param_1[0x67];
      pcVar6 = *(char **)(iVar7 + 0x128);
      pcVar13 = pcVar6 + 1;
      do {
        cVar5 = *pcVar6;
        pcVar6 = pcVar6 + 1;
      } while (cVar5 != '\0');
      if (pcVar6 != pcVar13) {
        FUN_00402f20(&local_1a8,"qop=%s, nc=%s, cnonce=\"%s\", ",*(undefined4 *)(iVar7 + 0x128),
                     local_18c,*(undefined4 *)(iVar7 + 0x11c));
      }
      iVar7 = param_1[0x67];
      if (*(char *)(iVar7 + 0x135) != '\0') {
        FUN_00402f20(&local_19c,
                     ", charset=utf-8, hashed-dirs=\"service-name,channel-binding\", service-name=\"%s\", channel-binding=\"%s\""
                     ,*(undefined4 *)(iVar7 + 0x110),*(undefined4 *)(iVar7 + 0x114));
      }
      pcVar6 = *(char **)(param_1[0x67] + 300);
      pcVar13 = pcVar6 + 1;
      do {
        cVar5 = *pcVar6;
        pcVar6 = pcVar6 + 1;
      } while (cVar5 != '\0');
      if (pcVar6 != pcVar13) {
        FUN_00402f20(&local_1c0,", opaque=\"%s\"",*(undefined4 *)(param_1[0x67] + 300));
      }
      FUN_004024f0();
      local_178._0_1_ = 0xc;
      FUN_00402f20(&local_1a4,&DAT_00696f08,*(undefined4 *)(param_1[0x67] + 0x168));
      if (*(char *)(param_1[0x67] + 0x135) == '\0') {
        puVar9 = (undefined4 *)FUN_004030a0(&DAT_006979dc);
        local_178._0_1_ = 0xd;
        local_188 = local_188 | 4;
        local_1c8 = (char *)*puVar9;
      }
      else {
        local_1c8 = local_19c;
      }
      uVar16 = local_188;
      pcVar6 = *(char **)(param_1[0x67] + 300);
      pcVar13 = pcVar6 + 1;
      do {
        cVar5 = *pcVar6;
        pcVar6 = pcVar6 + 1;
      } while (cVar5 != '\0');
      if (pcVar6 == pcVar13) {
        puVar9 = (undefined4 *)FUN_004030a0(&DAT_006979dc);
        local_178._0_1_ = 0xe;
        local_178._1_3_ = 0;
        local_188 = uVar16 | 8;
        local_1c0 = (char *)*puVar9;
      }
      uVar16 = local_188;
      pcVar6 = *(char **)(param_1[0x67] + 0x128);
      pcVar13 = pcVar6 + 1;
      do {
        cVar5 = *pcVar6;
        pcVar6 = pcVar6 + 1;
      } while (cVar5 != '\0');
      if (pcVar6 == pcVar13) {
        puVar9 = (undefined4 *)FUN_004030a0(&DAT_006979dc);
        local_178._0_1_ = 0xf;
        local_178._1_3_ = 0;
        local_188 = uVar16 | 0x10;
        local_1a8 = *puVar9;
      }
      uVar16 = local_188;
      FUN_00402f20(param_1 + 0xcbb,
                   "Proxy-Authorization: Digest username=\"%s\", realm=\"%s\", nonce=\"%s\", uri=\"%s\", %sresponse=\"%s\"%s%s\r\n"
                   ,local_1a4,*(undefined4 *)(param_1[0x67] + 0x120),
                   *(undefined4 *)(param_1[0x67] + 0x118),param_1[0xc8e],local_1a8,&local_64,
                   local_1c0,local_1c8);
      local_178 = 0xe;
      if ((uVar16 & 0x10) != 0) {
        uVar16 = uVar16 & 0xffffffef;
        local_188 = uVar16;
        FUN_00402560();
      }
      local_178 = 0xd;
      if ((uVar16 & 8) != 0) {
        uVar16 = uVar16 & 0xfffffff7;
        local_188 = uVar16;
        FUN_00402560();
      }
      local_178 = 0xc;
      if ((uVar16 & 4) != 0) {
        local_188 = uVar16 & 0xfffffffb;
        FUN_00402560();
      }
      local_178._0_1_ = 0xb;
      FUN_00402560();
      local_178._0_1_ = 10;
      FUN_00402560();
      local_178._0_1_ = 9;
      FUN_00402560();
      local_178._0_1_ = 8;
      FUN_00402560();
      local_178._0_1_ = 2;
      FUN_00402560();
    }
  }
  iVar7 = param_1[0x32];
  local_1e8 = (uint)(*(char *)(iVar7 + 0x351) != '\0');
  param_1[0xcac] = 0;
  param_1[0xcad] = 0;
  param_1[0x1a] = -1;
  param_1[0x1b] = -1;
  if ((*(int *)(iVar7 + 0x348) == 8) && (iVar12 = param_1[0x2a], iVar12 != 0)) {
    uVar16 = *(uint *)(iVar12 + 0x50);
    if (uVar16 == 0 && *(int *)(iVar12 + 0x54) == 0) goto LAB_005af22b;
    iVar7 = uVar16 + *(uint *)(iVar12 + 0x48);
    iVar15 = (*(int *)(iVar12 + 0x54) + *(int *)(iVar12 + 0x4c) +
             (uint)CARRY4(uVar16,*(uint *)(iVar12 + 0x48))) - (uint)(iVar7 == 0);
    param_1[0x1a] = iVar7 + -1;
    param_1[0x1b] = iVar15;
    _sprintf(local_114,"Range: bytes=%I64d-%I64d\r\n",*(undefined4 *)(iVar12 + 0x48),
             *(undefined4 *)(iVar12 + 0x4c),param_1[0x1a],iVar15);
  }
  else {
LAB_005af22b:
    if ((char)param_1[0xc84] == '\0') {
      iVar12 = param_1[0x19];
      if ((iVar12 < 1) && ((iVar12 < 0 || (param_1[0x18] == 0)))) {
        if (((*(int *)(iVar7 + 0x33c) == 0x40) || (*(int *)(iVar7 + 0x33c) == 0x80)) &&
           (cVar5 = FUN_00580580(), cVar5 != '\0')) {
          uVar18 = FUN_005805c0();
          *(undefined8 *)(param_1 + 0x1e) = uVar18;
          param_1[0x1a] = (int)uVar18;
          param_1[0x1b] = param_1[0x1f];
          uVar18 = FUN_005805c0();
          uVar18 = FUN_005805a0(uVar18);
          _sprintf(local_114,"Range: bytes=%I64d-%I64d\r\n",(int)uVar18,
                   (int)((ulonglong)uVar18 >> 0x20));
        }
        else {
          if ((*(char *)((int)param_1 + 0xe2) != '\0') ||
             (iVar7 = param_1[0x32], *(char *)(iVar7 + 0x1c1) != '\0')) goto LAB_005af234;
          if (((*(char *)(iVar7 + 0x1a0) == '\0') || (*(int *)(iVar7 + 0x31c) < 0)) ||
             ((*(int *)(iVar7 + 0x31c) < 1 && (*(int *)(iVar7 + 0x318) == 0)))) {
            iVar7 = FUN_005805e0();
            if (iVar7 == 0) {
              _sprintf(local_114,"");
              *(undefined1 *)(param_1 + 0xc83) = 0;
            }
            else {
              param_1[0x1a] = param_1[0x1e];
              param_1[0x1b] = param_1[0x1f];
              _sprintf(local_114,"Range: bytes=0-%I64d\r\n",param_1[0x1e],param_1[0x1f]);
            }
          }
          else {
            iVar12 = *(int *)(iVar7 + 0x318);
            param_1[0x1e] = iVar12;
            iVar15 = *(int *)(iVar7 + 0x31c);
            param_1[0x1f] = iVar15;
            param_1[0x1a] = iVar12;
            param_1[0x1b] = iVar15;
            _sprintf(local_114,"Range: bytes=0-%I64d\r\n",*(undefined4 *)(iVar7 + 0x318),
                     *(undefined4 *)(iVar7 + 0x31c));
          }
        }
      }
      else if (*(char *)(iVar7 + 0x374) == '\0') {
        if (((param_1[0x16] == 0 && param_1[0x17] == 0) || (cVar5 = FUN_00580560(), cVar5 == '\0'))
           || (*(int *)(param_1[0x32] + 800) == 0)) {
          if (*(int *)(param_1[0x32] + 800) == 0) {
            local_1e8 = 1;
          }
          uVar16 = param_1[0x18];
          iVar7 = param_1[0x19];
          if ((iVar7 < 0) || ((iVar7 < 1 && (uVar16 == 0)))) {
            uVar16 = 0;
            iVar7 = 0;
          }
          else if ((char)param_1[0x36] != '\0') {
            bVar17 = 0xffffffcf < uVar16;
            uVar16 = uVar16 + 0x30;
            iVar7 = iVar7 + (uint)bVar17;
          }
          param_1[0xcac] = uVar16;
          param_1[0xcad] = iVar7;
          if (*(char *)((int)param_1 + 0xdf) != '\0') {
            uVar1 = param_1[0x65];
            iVar12 = (int)uVar1 >> 0x1f;
            iVar7 = param_1[0xcad];
            if ((iVar7 < iVar12) || ((iVar7 <= iVar12 && (uVar16 < uVar1)))) {
              *(undefined1 *)((int)param_1 + 0xdf) = 0;
            }
            else {
              param_1[0xcac] = uVar16 - uVar1;
              param_1[0xcad] = (iVar7 - iVar12) - (uint)(uVar16 < uVar1);
            }
          }
          _sprintf(local_114,"Range: bytes=%I64d-\r\n",param_1[0xcac],param_1[0xcad]);
          *(undefined1 *)(param_1 + 0xc86) = 0;
        }
        else {
          lVar19 = FUN_005803d0();
          cVar5 = FUN_00580540();
          if (cVar5 == '\0') {
            iVar12 = param_1[0x16] + -1;
            iVar7 = param_1[0x17] - (uint)(param_1[0x16] == 0);
            lVar3 = CONCAT44(iVar7,iVar12);
            lVar2 = lVar19;
            if ((lVar19 < lVar3) && (lVar2 = CONCAT44(iVar7,iVar12), lVar19 + 0x40 < lVar3)) {
              lVar2 = lVar19 + 0x40;
            }
          }
          else {
            lVar2 = lVar19 + 1;
          }
          uVar16 = param_1[0x18];
          iVar7 = param_1[0x19];
          if ((char)param_1[0x36] != '\0') {
            bVar17 = 0xffffffcf < uVar16;
            uVar16 = uVar16 + 0x30;
            iVar7 = iVar7 + (uint)bVar17;
          }
          param_1[0xcac] = uVar16;
          param_1[0xcad] = iVar7;
          if (*(char *)((int)param_1 + 0xdf) != '\0') {
            local_14 = param_1[0x65];
            local_10 = (int)local_14 >> 0x1f;
            uVar16 = param_1[0xcac];
            if ((iVar7 < local_10) || ((iVar7 <= local_10 && (uVar16 < local_14)))) {
              *(undefined1 *)((int)param_1 + 0xdf) = 0;
            }
            else {
              param_1[0xcac] = uVar16 - local_14;
              param_1[0xcad] = (iVar7 - local_10) - (uint)(uVar16 < local_14);
            }
          }
          *(longlong *)(param_1 + 0x1a) = lVar2;
          _sprintf(local_114,"Range: bytes=%I64d-%I64d\r\n",param_1[0xcac],param_1[0xcad],(int)lVar2
                   ,(int)((ulonglong)lVar2 >> 0x20));
          *(undefined1 *)(param_1 + 0xc86) = 1;
        }
      }
      else {
        param_1[0x1a] = param_1[0x1e];
        param_1[0x1b] = param_1[0x1f];
        _sprintf(local_114,"Range: bytes=%I64d-%I64d\r\n",param_1[0x18],iVar12,param_1[0x1e],
                 param_1[0x1f]);
        *(undefined1 *)(param_1 + 0xc86) = 1;
      }
    }
    else {
LAB_005af234:
      _sprintf(local_114,"Range: bytes=0-\r\n");
    }
  }
  if (param_1[0x33] == 0) {
    iVar7 = *(int *)(param_1[0x32] + 0x14);
  }
  else {
    iVar7 = *(int *)(param_1[0x33] + 0xc);
  }
  if (iVar7 == 0x50) {
    _sprintf(local_1c,"");
  }
  else {
    _sprintf(local_1c,":%ld",iVar7);
  }
  FUN_004030a0(&DAT_006979dc);
  local_178 = CONCAT31(local_178._1_3_,0x10);
  if ((*(int *)(param_1[0x67] + 8) == 1) && (param_1[0xc94] != 0)) {
    FUN_00402f20(&local_1b8,&DAT_00696f08,param_1[0xc94]);
  }
  pcVar6 = *(char **)(param_1[0x32] + 0x3b8);
  pcVar13 = pcVar6 + 1;
  do {
    cVar5 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar5 != '\0');
  if ((pcVar6 != pcVar13) && (*(char *)(param_1[0x67] + 0x13a) != '\0')) {
    FUN_00402f20(&local_1b8,"Proxy-Authorization: Basic %s\r\n",
                 *(undefined4 *)(param_1[0x32] + 0x3b8));
  }
  if ((*(int *)(param_1[0x67] + 8) == 2) || (*(int *)(param_1[0x67] + 8) == 3)) {
    FUN_00402f20(&local_1b8,&DAT_00696f08,param_1[0xcbb]);
  }
  FUN_004024f0();
  local_178 = CONCAT31(local_178._1_3_,0x11);
  iVar7 = param_1[0x32];
  if ((*(char *)(iVar7 + 0x366) == '\0') || (*(char *)(iVar7 + 0x368) == '\0')) {
    if (*(char **)(iVar7 + 0x18) == (char *)0x0) {
      if (*(char *)(iVar7 + 0x373) != '\0') {
        pcVar6 = *(char **)(iVar7 + 0x124);
        pcVar13 = pcVar6 + 1;
        do {
          cVar5 = *pcVar6;
          pcVar6 = pcVar6 + 1;
        } while (cVar5 != '\0');
        if (pcVar6 != pcVar13) {
          FUN_00402f20(&local_1bc,"Referer: %s\r\n",*(undefined4 *)(iVar7 + 0x124));
          goto LAB_005af722;
        }
      }
      goto LAB_005af715;
    }
    iVar7 = __strnicmp(*(char **)(iVar7 + 0x18),"file://",7);
    if (iVar7 == 0) goto LAB_005af715;
    FUN_00402f20(&local_1bc,"Referer: %s\r\n",*(undefined4 *)(param_1[0x32] + 0x18));
  }
  else {
LAB_005af715:
    FUN_00402f80(&DAT_006979dc);
  }
LAB_005af722:
  FUN_004030a0(&DAT_006979dc);
  local_178._0_1_ = 0x12;
  if ((*(char *)(param_1[0x32] + 0x36b) != '\0') &&
     (pcVar13 = *(char **)(param_1[0x32] + 0x20), pcVar13 != (char *)0x0)) {
    pcVar6 = pcVar13 + 1;
    do {
      cVar5 = *pcVar13;
      pcVar13 = pcVar13 + 1;
    } while (cVar5 != '\0');
    for (iVar7 = (int)pcVar13 - (int)pcVar6; 1 < iVar7; iVar7 = iVar7 + -1) {
      pcVar13 = (char *)(*(int *)(param_1[0x32] + 0x20) + -1 + iVar7);
      cVar5 = *pcVar13;
      if ((cVar5 != '\n') && (cVar5 != '\r')) {
        FUN_00402f20(&local_1ec,"If-Modified-Since: %s\r\n",*(undefined4 *)(param_1[0x32] + 0x20));
        break;
      }
      *pcVar13 = '\0';
    }
  }
  FUN_004024f0();
  local_178 = CONCAT31(local_178._1_3_,0x13);
  iVar7 = param_1[0x33];
  if (iVar7 == 0) {
    pcVar6 = *(char **)(param_1[0x32] + 0xd0);
    pcVar13 = pcVar6 + 1;
    do {
      cVar5 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar5 != '\0');
    if (pcVar6 == pcVar13) goto LAB_005af805;
    uVar14 = *(undefined4 *)(param_1[0x32] + 0xd0);
LAB_005af7f1:
    FUN_00402f20(&local_1ac,&DAT_00696f08,uVar14);
  }
  else {
    pcVar6 = *(char **)(iVar7 + 0x1c);
    pcVar13 = pcVar6 + 1;
    do {
      cVar5 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar5 != '\0');
    if (pcVar6 != pcVar13) {
      uVar14 = *(undefined4 *)(iVar7 + 0x1c);
      goto LAB_005af7f1;
    }
LAB_005af805:
    FUN_00402f20(&local_1ac,"application/x-www-form-urlencoded");
  }
  FUN_004024f0();
  local_178._0_1_ = 0x14;
  iVar7 = param_1[0x33];
  if (iVar7 == 0) {
    if (*(char *)(param_1[0x32] + 0x1a5) != '\0') {
      FUN_00402f20(&local_1d8,"Content-Type: %s\r\nContent-Length: %d\r\n",local_1ac,
                   *(undefined4 *)(*(int *)(param_1[0x32] + 0x1a8) + -0xc));
    }
    if (*(char *)(param_1[0x32] + 0x1b8) != '\0') {
      uVar14 = *(undefined4 *)(param_1[0x32] + 0x1b4);
      goto LAB_005af87f;
    }
  }
  else if (*(char *)(iVar7 + 0x14) != '\0') {
    uVar14 = *(undefined4 *)(*(int *)(iVar7 + 0x18) + -0xc);
LAB_005af87f:
    FUN_00402f20(&local_1d8,"Content-Type: %s\r\nContent-Length: %d\r\n",local_1ac,uVar14);
  }
  FUN_004024f0();
  local_178 = CONCAT31(local_178._1_3_,0x15);
  pcVar6 = *(char **)(param_1[0x32] + 0x38c);
  pcVar13 = pcVar6 + 1;
  do {
    cVar5 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar5 != '\0');
  if (pcVar6 != pcVar13) {
    FUN_00402f20(&local_1e4,"Cookie2: %s\r\n",*(undefined4 *)(param_1[0x32] + 0x38c));
  }
  pcVar6 = *(char **)(param_1[0x32] + 0x390);
  pcVar13 = pcVar6 + 1;
  do {
    cVar5 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar5 != '\0');
  if (pcVar6 == pcVar13) {
    if ((DAT_00788ccc == (int)pcVar6 - (int)pcVar13) ||
       (iVar7 = GetLocaleInfoA(0x400,0x5c,local_174,0x5f), iVar7 == 0)) {
      iVar7 = GetLocaleInfoA(0x400,0x59,local_70,9);
      if ((iVar7 == 0) || (iVar7 = GetLocaleInfoA(0x400,0x5a,(LPSTR)&local_14,9), iVar7 == 0)) {
        FUN_00402f20(param_1[0x32] + 0x390,"en-US");
      }
      else {
        FUN_00402f20(param_1[0x32] + 0x390,"%s-%s",local_70,&local_14);
      }
    }
    else {
      FUN_00402f20(param_1[0x32] + 0x390,&DAT_00696f08,local_174);
    }
  }
  FUN_004024f0();
  local_178._0_1_ = 0x16;
  pcVar6 = *(char **)(param_1[0x32] + 0x390);
  pcVar13 = pcVar6 + 1;
  do {
    cVar5 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar5 != '\0');
  if (pcVar6 != pcVar13) {
    FUN_00402f20(&local_1e0,"Accept-Language: %s\r\n",*(undefined4 *)(param_1[0x32] + 0x390));
  }
  FUN_004024f0();
  local_178 = CONCAT31(local_178._1_3_,0x17);
  pcVar13 = _strrchr(*(char **)param_1[0x32],0x2e);
  if ((pcVar13 != (char *)0x0) && (iVar7 = __stricmp(pcVar13,".monster"), iVar7 == 0)) {
    *(undefined1 *)(param_1[0x32] + 0x371) = 1;
  }
  iVar7 = param_1[0x32];
  pcVar6 = *(char **)(iVar7 + 0x398);
  pcVar13 = pcVar6 + 1;
  do {
    cVar5 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar5 != '\0');
  if (pcVar6 == pcVar13) {
    if (*(char *)(iVar7 + 0x371) == '\0') {
      pcVar6 = *(char **)(iVar7 + 0x124);
      pcVar13 = pcVar6 + 1;
      do {
        cVar5 = *pcVar6;
        pcVar6 = pcVar6 + 1;
      } while (cVar5 != '\0');
      if (pcVar6 == pcVar13) {
        if ((*(char **)(iVar7 + 0x18) != (char *)0x0) && (*(char *)(iVar7 + 0x366) == '\0')) {
          local_1ad = 0;
          pcVar13 = _strrchr(*(char **)(iVar7 + 0x18),0x2e);
          if ((pcVar13 == (char *)0x0) || (iVar7 = __strnicmp(pcVar13,".swf",4), iVar7 != 0)) {
            FUN_004024f0();
            local_178 = CONCAT31(local_178._1_3_,0x19);
            uVar14 = *(undefined4 *)(param_1[0x32] + 0x18);
            goto LAB_005afaf1;
          }
          local_1ad = 1;
        }
      }
      else {
        FUN_004024f0();
        local_178 = CONCAT31(local_178._1_3_,0x18);
        uVar14 = *(undefined4 *)(param_1[0x32] + 0x124);
LAB_005afaf1:
        FUN_00402f20(&local_18c,&DAT_00696f08,uVar14);
        pcVar13 = _strstr(local_18c,"://");
        if ((pcVar13 != (char *)0x0) &&
           (pcVar13 = _strpbrk(pcVar13 + 3,":/"), pcVar13 != (char *)0x0)) {
          *pcVar13 = '\0';
          FUN_00402f20(&local_1d0,"Origin: %s\r\n",local_18c);
        }
        local_178 = CONCAT31(local_178._1_3_,0x17);
        FUN_00402560();
      }
    }
  }
  else {
    FUN_00402f20(&local_1d0,"Origin: %s\r\n",*(undefined4 *)(iVar7 + 0x398));
  }
  FUN_004024f0();
  local_178 = CONCAT31(local_178._1_3_,0x1a);
  iVar7 = param_1[0x32];
  pcVar6 = *(char **)(iVar7 + 0x3a0);
  pcVar13 = pcVar6 + 1;
  do {
    cVar5 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar5 != '\0');
  if (pcVar6 == pcVar13) {
    if (*(char *)(iVar7 + 0x372) == '\0') {
      pcVar13 = "Accept-Charset: *\r\n";
    }
    else {
      pcVar13 = "";
    }
    FUN_00402f20(&local_1dc,pcVar13);
  }
  else {
    FUN_00402f20(&local_1dc,"Accept-Charset: %s\r\n",*(undefined4 *)(iVar7 + 0x3a0));
  }
  FUN_004024f0();
  local_178._0_1_ = 0x1b;
  iVar7 = param_1[0x32];
  if (*(char *)(iVar7 + 0x36e) == '\0') {
    if (((*(char *)(iVar7 + 0x361) == '\0') && (*(char *)(iVar7 + 0x362) == '\0')) ||
       (*(char *)(iVar7 + 0x364) != '\0')) {
      pcVar13 = "Accept-Encoding: identity\r\n";
    }
    else {
      pcVar13 = "";
    }
  }
  else {
    pcVar13 = "Accept-Encoding: gzip, deflate\r\n";
  }
  FUN_00402f20(&local_1d4,pcVar13);
  if (param_1[0x33] == 0) {
    local_1c8 = *(char **)(param_1[0x32] + 0x1a8);
  }
  else {
    local_1c8 = *(char **)(param_1[0x33] + 0x18);
  }
  iVar7 = param_1[0x32];
  pcVar6 = *(char **)(iVar7 + 0x3a4);
  pcVar13 = pcVar6 + 1;
  do {
    cVar5 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar5 != '\0');
  if ((pcVar6 == pcVar13) || (*(char *)(iVar7 + 0x36d) == '\0')) {
    puVar9 = (undefined4 *)FUN_004030a0(&DAT_006979dc);
    local_178._0_1_ = 0x1c;
    local_188 = local_188 | 0x20;
    local_1a8 = *puVar9;
  }
  else {
    local_1a8 = *(undefined4 *)(iVar7 + 0x3a4);
  }
  uVar16 = local_188;
  local_1c0 = "Cache-Control: no-cache\r\nPragma: no-cache\r\n";
  if (local_1e8 == 0) {
    local_1c0 = "";
  }
  pcVar13 = (char *)param_1[0xc8f];
  local_1a4 = &DAT_00699144;
  local_18c = pcVar13;
  if (pcVar13 == (char *)0x0) {
    local_1a4 = (undefined2 *)&DAT_006979dc;
    local_18c = "";
  }
  local_19c = "Cookie: ";
  if (pcVar13 == (char *)0x0) {
    local_19c = "";
  }
  iVar7 = param_1[0x32];
  if (local_19d == '\0') {
    if (*(char *)(iVar7 + 0x1be) == '\0') {
      local_1c4 = param_1[0xcb6];
    }
    else {
      local_1c4 = param_1[0xc93];
    }
  }
  else {
    local_1c4 = *(int *)(iVar7 + 0x3a8);
  }
  if ((((*(int *)(param_1[0xcb6] + -0xc) == 0) || (*(char *)(iVar7 + 0x1be) != '\0')) ||
      (param_1[0xcb7] != 0)) &&
     (((*(int *)(param_1[0xcbb] + -0xc) == 0 || (*(int *)(param_1[0x67] + 8) != 2)) ||
      (param_1[0xcbe] != 0)))) {
    local_194 = "";
  }
  else {
    local_194 = "Connection: Keep-Alive\r\n";
  }
  pcVar6 = *(char **)(iVar7 + 0x39c);
  pcVar13 = pcVar6 + 1;
  do {
    cVar5 = *pcVar6;
    pcVar6 = pcVar6 + 1;
  } while (cVar5 != '\0');
  if (pcVar6 == pcVar13) {
    puVar9 = (undefined4 *)FUN_004030a0(&DAT_006cf818);
    local_178._0_1_ = 0x1d;
    local_178._1_3_ = 0;
    local_188 = uVar16 | 0x40;
    uVar14 = *puVar9;
  }
  else {
    uVar14 = *(undefined4 *)(iVar7 + 0x39c);
  }
  uVar16 = local_188;
  puVar9 = (undefined4 *)param_1[0x33];
  if (puVar9 == (undefined4 *)0x0) {
    puVar9 = (undefined4 *)FUN_004030a0(*(undefined4 *)param_1[0x32]);
    local_178._0_1_ = 0x1e;
    local_178._1_3_ = 0;
    local_188 = uVar16 | 0x80;
  }
  uVar16 = local_188;
  FUN_00402f20(&local_1b4,
               "%s %s HTTP/1.1\r\nUser-Agent: %s\r\nHost: %s%s\r\nAccept: %s\r\n%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s\r\n%s"
               ,local_1cc,param_1[0xc8e],*(undefined4 *)(param_1[0x32] + 0x1ac),*puVar9,local_1c,
               uVar14,local_1d4,local_1e0,local_1dc,local_1d0,local_194,local_1ec,local_114,
               local_1bc,local_1c4,local_1b8,local_19c,local_18c,local_1a4,local_1e4,local_1d8,
               local_1c0,local_1a8,local_1c8);
  local_178 = 0x1d;
  if ((char)uVar16 < '\0') {
    uVar16 = uVar16 & 0xffffff7f;
    local_188 = uVar16;
    FUN_00402560();
  }
  local_178 = 0x1c;
  if ((uVar16 & 0x40) != 0) {
    uVar16 = uVar16 & 0xffffffbf;
    local_188 = uVar16;
    FUN_00402560();
  }
  local_178 = 0x1b;
  if ((uVar16 & 0x20) != 0) {
    local_188 = uVar16 & 0xffffffdf;
    FUN_00402560();
  }
  if (((param_1[0x19] < 1) && ((param_1[0x19] < 0 || (param_1[0x18] == 0)))) &&
     (param_1[0x1e] == 0 && param_1[0x1f] == 0)) {
    FUN_00566b20(local_1b8);
  }
  else {
    FUN_004024f0();
    local_178._0_1_ = 0x1f;
    pcVar13 = local_114;
    do {
      cVar5 = *pcVar13;
      pcVar13 = pcVar13 + 1;
    } while (cVar5 != '\0');
    puVar11 = local_107;
    if (pcVar13 == local_114 + 1) {
      puVar11 = &DAT_00699144;
    }
    FUN_00402f20(&local_1a8,"%I64d-%I64d, %s%s",param_1[0x18],param_1[0x19],param_1[0x1e],
                 param_1[0x1f],puVar11,local_1b8);
    FUN_00566b20(local_1a8);
    local_178 = CONCAT31(local_178._1_3_,0x1b);
    FUN_00402560();
  }
  param_1[0xc81] = 1;
  *(undefined1 *)(param_1 + 0x38) = 0;
  FUN_00566d80();
  param_1[0x29] = 4;
  param_1[0xc9c] = 0;
  if (((param_1[0x67] == 0) || (*(char *)(param_1[0x67] + 0x137) == '\0')) ||
     (local_194 = (char *)param_1[0xd40], local_194 == (char *)0x0)) {
    pcVar13 = "send() HEAD";
    if (param_2 == 0) {
      pcVar13 = "send() GET";
    }
    pcVar6 = local_1b4;
    do {
      cVar5 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar5 != '\0');
    iVar7 = send(param_1[0xc80],local_1b4,(int)pcVar6 - (int)(local_1b4 + 1),0);
    FUN_00567470(iVar7,pcVar13);
    iVar7 = param_1[0x32];
    if ((*(char *)(iVar7 + 0x1b8) != '\0') && (*(char **)(iVar7 + 0x1b0) != (char *)0x0)) {
      send(param_1[0xc80],*(char **)(iVar7 + 0x1b0),*(int *)(iVar7 + 0x1b4),0);
    }
  }
  else {
    iVar7 = param_1[0x32];
    if (*(int *)(iVar7 + 0x180) == 0) {
      pcVar13 = local_1b4;
      do {
        cVar5 = *pcVar13;
        pcVar13 = pcVar13 + 1;
      } while (cVar5 != '\0');
      FUN_005f3130(local_1b4,(int)pcVar13 - (int)(local_1b4 + 1));
      iVar7 = param_1[0x32];
      if ((*(char *)(iVar7 + 0x1b8) != '\0') && (*(int *)(iVar7 + 0x1b0) != 0)) {
        FUN_005f3130(*(int *)(iVar7 + 0x1b0),*(undefined4 *)(iVar7 + 0x1b4));
      }
    }
    else {
      pcVar13 = (char *)0x0;
      iVar12 = 0;
      if ((*(char *)(iVar7 + 0x1b8) == '\0') || (*(char **)(iVar7 + 0x1b0) == (char *)0x0)) {
        iVar15 = param_1[0x33];
        if (iVar15 == 0) {
          cVar5 = *(char *)(iVar7 + 0x1a5);
        }
        else {
          cVar5 = *(char *)(iVar15 + 0x14);
        }
        if (cVar5 != '\0') {
          puVar9 = (undefined4 *)(iVar15 + 0x18);
          if (iVar15 == 0) {
            puVar9 = (undefined4 *)(iVar7 + 0x1a8);
          }
          pcVar13 = (char *)*puVar9;
          pcVar6 = pcVar13;
          do {
            cVar5 = *pcVar6;
            pcVar6 = pcVar6 + 1;
          } while (cVar5 != '\0');
          iVar12 = (int)pcVar6 - (int)(pcVar13 + 1);
        }
      }
      else {
        iVar12 = *(int *)(iVar7 + 0x1b4);
        pcVar13 = *(char **)(iVar7 + 0x1b0);
      }
      FUN_005f7680(&local_1b4,pcVar13,iVar12);
    }
  }
  local_178._0_1_ = 0x1a;
  FUN_00402560();
  local_178._0_1_ = 0x17;
  FUN_00402560();
  local_178._0_1_ = 0x16;
  FUN_00402560();
  local_178._0_1_ = 0x15;
  FUN_00402560();
  local_178._0_1_ = 0x14;
  FUN_00402560();
  local_178._0_1_ = 0x13;
  FUN_00402560();
  local_178._0_1_ = 0x12;
  FUN_00402560();
  local_178._0_1_ = 0x11;
  FUN_00402560();
  local_178._0_1_ = 0x10;
  FUN_00402560();
  local_178._0_1_ = 2;
  FUN_00402560();
  local_178 = CONCAT31(local_178._1_3_,1);
  FUN_00402560();
LAB_005b0122:
  local_178 = 0xffffffff;
  FUN_00402560();
  FUN_005b0164();
  return;
}


