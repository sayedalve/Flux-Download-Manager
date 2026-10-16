// Function: FUN_005d9c60
// Entry Point: 0x005d9c60
// Program: IDMan.exe


void __thiscall FUN_005d9c60(undefined4 *param_1,int param_2,int param_3,int param_4)

{
  BYTE BVar1;
  int iVar2;
  uint *puVar3;
  char cVar4;
  BYTE *pBVar5;
  char *pcVar6;
  char *pcVar7;
  BYTE *pBVar8;
  uint uStack_40;
  BYTE local_2c [8];
  int local_24;
  int local_20;
  BYTE *local_1c;
  uint local_18;
  undefined1 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  puStack_c = &LAB_00684178;
  local_10 = ExceptionList;
  uStack_40 = DAT_007839c0 ^ (uint)&stack0xfffffffc;
  local_14 = (undefined1 *)&uStack_40;
  ExceptionList = &local_10;
  local_8 = 0;
  if ((param_2 != 0) &&
     ((puVar3 = &uStack_40, param_1[0x129] != 0 ||
      (cVar4 = FUN_005d6890(param_2), puVar3 = (uint *)local_14, cVar4 != '\0')))) {
    local_14 = (undefined1 *)puVar3;
    pBVar8 = (BYTE *)*param_1;
    pBVar5 = pBVar8;
    do {
      BVar1 = *pBVar5;
      pBVar5 = pBVar5 + 1;
    } while (BVar1 != '\0');
    RegSetValueExA((HKEY)param_1[0x129],"Host",0,1,pBVar8,(DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))))
    ;
    pBVar8 = (BYTE *)param_1[1];
    if (pBVar8 != (BYTE *)0x0) {
      pBVar5 = pBVar8;
      do {
        BVar1 = *pBVar5;
        pBVar5 = pBVar5 + 1;
      } while (BVar1 != '\0');
      RegSetValueExA((HKEY)param_1[0x129],"Path",0,1,pBVar8,
                     (DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))));
    }
    pcVar6 = _strstr((char *)*param_1,"googlevideo.com");
    if (pcVar6 == (char *)0x0) {
      pBVar8 = (BYTE *)param_1[2];
      pBVar5 = pBVar8;
      do {
        BVar1 = *pBVar5;
        pBVar5 = pBVar5 + 1;
      } while (BVar1 != '\0');
      RegSetValueExA((HKEY)param_1[0x129],"FileName",0,1,pBVar8,
                     (DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))));
    }
    else {
      FUN_005d5770(param_1[0x129],param_1[2],&DAT_0069c668,"FileName");
    }
    pBVar8 = (BYTE *)param_1[3];
    pBVar5 = pBVar8;
    do {
      BVar1 = *pBVar5;
      pBVar5 = pBVar5 + 1;
    } while (BVar1 != '\0');
    RegSetValueExA((HKEY)param_1[0x129],"User",0,1,pBVar8,(DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))))
    ;
    FUN_005c11b0();
    if (param_1[0xf] != 0) {
      FUN_005d3960();
    }
    pBVar8 = (BYTE *)param_1[0x10];
    if (pBVar8 != (BYTE *)0x0) {
      pBVar5 = pBVar8;
      do {
        BVar1 = *pBVar5;
        pBVar5 = pBVar5 + 1;
      } while (BVar1 != '\0');
      pcVar6 = "WDescription";
      if (*(char *)((int)param_1 + 0x356) == '\0') {
        pcVar6 = "Description";
      }
      RegSetValueExA((HKEY)param_1[0x129],pcVar6,0,1,pBVar8,
                     (DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))));
    }
    pBVar8 = (BYTE *)param_1[8];
    if (pBVar8 != (BYTE *)0x0) {
      pBVar5 = pBVar8;
      do {
        BVar1 = *pBVar5;
        pBVar5 = pBVar5 + 1;
      } while (BVar1 != '\0');
      RegSetValueExA((HKEY)param_1[0x129],"LastModified",0,1,pBVar8,
                     (DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))));
    }
    iVar2 = *(int *)((BYTE *)param_1[0x6b] + -0xc);
    if (iVar2 == 0) {
      RegDeleteValueA((HKEY)param_1[0x129],"UA");
    }
    else {
      RegSetValueExA((HKEY)param_1[0x129],"UA",0,1,(BYTE *)param_1[0x6b],iVar2 + 1);
    }
    pBVar8 = (BYTE *)param_1[0x30];
    if (*(int *)(pBVar8 + -0xc) != 0) {
      pBVar5 = pBVar8;
      do {
        BVar1 = *pBVar5;
        pBVar5 = pBVar5 + 1;
      } while (BVar1 != '\0');
      RegSetValueExA((HKEY)param_1[0x129],"mdtm",0,1,pBVar8,
                     (DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))));
    }
    pBVar8 = (BYTE *)param_1[6];
    if (pBVar8 != (BYTE *)0x0) {
      pBVar5 = pBVar8;
      do {
        BVar1 = *pBVar5;
        pBVar5 = pBVar5 + 1;
      } while (BVar1 != '\0');
      RegSetValueExA((HKEY)param_1[0x129],"Referer",0,1,pBVar8,
                     (DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))));
    }
    pBVar8 = (BYTE *)param_1[7];
    if (pBVar8 != (BYTE *)0x0) {
      pBVar5 = pBVar8;
      do {
        BVar1 = *pBVar5;
        pBVar5 = pBVar5 + 1;
      } while (BVar1 != '\0');
      RegSetValueExA((HKEY)param_1[0x129],"Cookie",0,1,pBVar8,
                     (DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))));
    }
    pBVar8 = (BYTE *)param_1[0x6a];
    iVar2 = *(int *)(pBVar8 + -0xc);
    if (iVar2 == 0) {
      if ((*(char *)((int)param_1 + 0x1a5) == '\0') || (*(char *)((int)param_1 + 0x35b) == '\0')) {
        RegDeleteValueA((HKEY)param_1[0x129],"PD");
      }
      else {
        FUN_00402f80(&DAT_006979dc);
        RegSetValueExA((HKEY)param_1[0x129],"PD",0,1,(BYTE *)param_1[0x6a],1);
      }
    }
    else {
      RegSetValueExA((HKEY)param_1[0x129],"PD",0,1,pBVar8,iVar2 + 1);
    }
    iVar2 = *(int *)((BYTE *)param_1[0x34] + -0xc);
    if (iVar2 == 0) {
      RegDeleteValueA((HKEY)param_1[0x129],"PDCType");
    }
    else {
      RegSetValueExA((HKEY)param_1[0x129],"PDCType",0,1,(BYTE *)param_1[0x34],iVar2 + 1);
    }
    RegSetValueExA((HKEY)param_1[0x129],"TPswitch",0,4,(BYTE *)(param_1 + 9),4);
    RegSetValueExA((HKEY)param_1[0x129],"Port",0,4,(BYTE *)(param_1 + 5),4);
    RegSetValueExA((HKEY)param_1[0x129],"categoryID",0,4,(BYTE *)(param_1 + 0xc9),4);
    local_18 = (uint)*(byte *)((int)param_1 + 0x35f);
    RegSetValueExA((HKEY)param_1[0x129],"CISBU",0,4,(BYTE *)&local_18,4);
    if (*(char *)(param_1 + 0xd6) != '\0') {
      local_18 = 1;
      RegSetValueExA((HKEY)param_1[0x129],"bPHSBU",0,4,(BYTE *)&local_18,4);
    }
    if (*(char *)((int)param_1 + 0x359) != '\0') {
      local_18 = 1;
      RegSetValueExA((HKEY)param_1[0x129],"bFLSBU",0,4,(BYTE *)&local_18,4);
    }
    if (*(char *)((int)param_1 + 0x1a3) != '\0') {
      local_18 = 1;
      RegSetValueExA((HKEY)param_1[0x129],"bRPCHK",0,4,(BYTE *)&local_18,4);
    }
    if (*(char *)((int)param_1 + 0x19d) != '\0') {
      local_18 = 1;
      RegSetValueExA((HKEY)param_1[0x129],"bMUIMPV",0,4,(BYTE *)&local_18,4);
    }
    if (*(char *)(param_1 + 0x70) != '\0') {
      local_18 = 1;
      RegSetValueExA((HKEY)param_1[0x129],"bUNRA",0,4,(BYTE *)&local_18,4);
    }
    RegSetValueExA((HKEY)param_1[0x129],"cFlags",0,4,(BYTE *)(param_1 + 0x59),4);
    if ((-1 < param_4) && ((0 < param_4 || (param_3 != 0)))) {
      RegSetValueExA((HKEY)param_1[0x129],"FileSize",0,0,(BYTE *)&param_3,8);
    }
    pcVar7 = (char *)param_1[0x4d];
    pcVar6 = pcVar7 + 1;
    do {
      cVar4 = *pcVar7;
      pcVar7 = pcVar7 + 1;
    } while (cVar4 != '\0');
    if (pcVar7 != pcVar6) {
      if (param_1[0x5c] == 0) {
        FUN_00426430();
      }
      pcVar6 = _strrchr((char *)param_1[0x4d],0x5c);
      if (pcVar6 == (char *)0x0) {
        pBVar8 = (BYTE *)param_1[0x4d];
      }
      else {
        pBVar8 = (BYTE *)(pcVar6 + 1);
      }
      pBVar5 = pBVar8;
      do {
        BVar1 = *pBVar5;
        pBVar5 = pBVar5 + 1;
      } while (BVar1 != '\0');
      RegSetValueExA((HKEY)param_1[0x129],"cFromDll",0,1,pBVar8,
                     (DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))));
    }
    pBVar8 = (BYTE *)param_1[0x4e];
    pBVar5 = pBVar8;
    do {
      BVar1 = *pBVar5;
      pBVar5 = pBVar5 + 1;
    } while (BVar1 != '\0');
    if (pBVar5 != pBVar8 + 1) {
      pBVar5 = pBVar8;
      do {
        BVar1 = *pBVar5;
        pBVar5 = pBVar5 + 1;
      } while (BVar1 != '\0');
      RegSetValueExA((HKEY)param_1[0x129],"cFPBrExe",0,1,pBVar8,
                     (DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))));
    }
    if (param_1[0xd2] != 0) {
      local_24 = param_1[0xd2];
      RegSetValueExA((HKEY)param_1[0x129],"nAggrV",0,4,(BYTE *)&local_24,4);
    }
    if (param_1[0xd1] != 0) {
      local_20 = param_1[0xd1];
      RegSetValueExA((HKEY)param_1[0x129],"nAddTF",0,4,(BYTE *)&local_20,4);
    }
    pBVar8 = (BYTE *)param_1[0xca];
    pBVar5 = pBVar8;
    do {
      BVar1 = *pBVar5;
      pBVar5 = pBVar5 + 1;
    } while (BVar1 != '\0');
    if (pBVar5 != pBVar8 + 1) {
      pBVar5 = pBVar8;
      do {
        BVar1 = *pBVar5;
        pBVar5 = pBVar5 + 1;
      } while (BVar1 != '\0');
      RegSetValueExA((HKEY)param_1[0x129],"bstrInfo",0,1,pBVar8,
                     (DWORD)(pBVar5 + (1 - (int)(pBVar8 + 1))));
    }
    if ((BYTE *)param_1[0xcf] != (BYTE *)0x0) {
      local_1c = (BYTE *)param_1[0xcf];
      RegSetValueExA((HKEY)param_1[0x129],"nAggr2",0,4,(BYTE *)&local_1c,4);
    }
    if (((param_1[0xcf] != 0) || (param_1[0xd2] == 0x40)) || (param_1[0xd2] == 0x800)) {
      if (*(char *)((int)param_1 + 0x34e) != '\0') {
        local_2c[4] = '\x01';
        local_2c[5] = '\0';
        local_2c[6] = '\0';
        local_2c[7] = '\0';
        RegSetValueExA((HKEY)param_1[0x129],"bPFAV",0,4,local_2c + 4,4);
      }
      if (param_1[0xcd] != 0) {
        RegSetValueExA((HKEY)param_1[0x129],"nAATDID",0,4,(BYTE *)(param_1 + 0xcd),4);
      }
    }
    if (*(char *)((int)param_1 + 0x179) != '\0') {
      local_2c[0] = '\x01';
      local_2c[1] = '\0';
      local_2c[2] = '\0';
      local_2c[3] = '\0';
      RegSetValueExA((HKEY)param_1[0x129],"bChCnt",0,4,local_2c,4);
    }
    if (((param_1[9] == 4) || (param_1[9] == 5)) && (*(char *)((int)param_1 + 0x35e) != '\0')) {
      FUN_004024f0();
      local_8._0_1_ = 1;
      pcVar7 = (char *)param_1[0x105];
      pcVar6 = pcVar7 + 1;
      do {
        cVar4 = *pcVar7;
        pcVar7 = pcVar7 + 1;
      } while (cVar4 != '\0');
      if ((pcVar7 != pcVar6) && (param_1[0x104] != 0)) {
        FUN_00402f20(&local_24,"&userArgs=%s",param_1[0x105]);
      }
      FUN_004024f0();
      local_8._0_1_ = 2;
      pcVar7 = (char *)param_1[0xf3];
      pcVar6 = pcVar7 + 1;
      do {
        cVar4 = *pcVar7;
        pcVar7 = pcVar7 + 1;
      } while (cVar4 != '\0');
      if (pcVar7 != pcVar6) {
        FUN_00402f20(&local_20,"&tcUrl=%s",param_1[0x106]);
      }
      FUN_004024f0();
      local_8._0_1_ = 3;
      FUN_00402f20(&local_1c,
                   "capabilities=%f&audioCodecs=%f&videoCodecs=%f&videoFunction=%f&duration=%I64d%s%s"
                   ,*(undefined8 *)(param_1 + 0xf8),*(undefined8 *)(param_1 + 0xfa),
                   *(undefined8 *)(param_1 + 0xfc),*(undefined8 *)(param_1 + 0xfe),param_1[0x100],
                   param_1[0x101],local_24,local_20);
      pBVar8 = local_1c;
      do {
        BVar1 = *pBVar8;
        pBVar8 = pBVar8 + 1;
      } while (BVar1 != '\0');
      RegSetValueExA((HKEY)param_1[0x129],"Cookie",0,1,local_1c,
                     (DWORD)(pBVar8 + (1 - (int)(local_1c + 1))));
      local_8._0_1_ = 2;
      FUN_00402560();
      local_8._0_1_ = 1;
      FUN_00402560();
      local_8 = (uint)local_8._1_3_ << 8;
      FUN_00402560();
    }
    FUN_005d79f0(param_2);
    ExceptionList = local_10;
    return;
  }
  ExceptionList = local_10;
  return;
}


