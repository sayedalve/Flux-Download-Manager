// Function: FUN_10034b40
// Entry Point: 0x10034b40
// Program: IDMNetMon.dll


void __thiscall FUN_10034b40(int param_1,int *param_2)

{
  int *piVar1;
  void *pvVar2;
  size_t sVar3;
  u_short uVar4;
  uint uVar5;
  int iVar6;
  undefined4 uVar7;
  DWORD DVar8;
  uint *puVar9;
  int iVar10;
  undefined4 *puVar11;
  byte *pbVar12;
  uint *_Dst;
  undefined1 *puVar13;
  undefined4 *local_10;
  size_t local_c;
  undefined4 local_8;
  undefined2 local_4;
  
  if (((char)param_2[7] == '\0') || (param_2[8] == 0)) {
    iVar6 = *(int *)(param_1 + 0x318);
    *(int *)(param_1 + 0x318) = iVar6 + 1;
    if ((char)param_2[7] == '\0') {
      if (iVar6 != 0) {
        param_2[8] = iVar6;
      }
      uVar5 = param_2[0xd] * 0x15 + 0x31;
      if ((uint)param_2[2] < uVar5) {
        FUN_10005810(uVar5);
      }
      FUN_10004e40(param_2,"MSG#%u#%u#%u#%u",0,param_2[8],param_2[9],param_2[10],param_2[0xb]);
      uVar5 = 0;
      if (param_2[0xd] != 0) {
        pbVar12 = (byte *)(param_2 + 0xe);
        do {
          if ((param_2[1] + 2U <= (uint)param_2[2]) ||
             (iVar6 = FUN_10005810(param_2[1] + 2U), iVar6 != 0)) {
            *(undefined1 *)(param_2[1] + *param_2) = 0x3a;
            iVar6 = param_2[1];
            param_2[1] = iVar6 + 1;
            *(undefined1 *)(iVar6 + 1 + *param_2) = 0;
          }
          FUN_10026110(param_2,&DAT_10064e18 + (uint)*pbVar12 * 5,0x14,*(undefined4 *)(pbVar12 + 8),
                       *(undefined4 *)(pbVar12 + 0xc));
          uVar5 = uVar5 + 1;
          pbVar12 = pbVar12 + 0x10;
        } while (uVar5 < (uint)param_2[0xd]);
      }
      iVar6 = param_2[5];
      uVar5 = iVar6 + 1;
      if ((uVar5 <= (uint)param_2[6]) ||
         ((iVar6 = FUN_10003720(uVar5), iVar6 != 0 && (iVar6 = param_2[5], param_2[4] + iVar6 != 0))
         )) {
        *(undefined1 *)(iVar6 + param_2[4]) = 0x3b;
        param_2[5] = uVar5;
      }
      *(undefined1 *)(param_2 + 7) = 1;
    }
  }
  uVar5 = param_2[5] + param_2[1];
  if (*(char *)(param_1 + 0x30c) == '\x04') {
    local_10 = &DAT_10074bf4;
    local_c = 0;
    local_8 = 0;
    local_4 = 0;
    FUN_10005810(0xd2);
    uVar7 = 200;
    if ((*(byte *)(param_2 + 0xb) & 4) != 0) {
      uVar7 = 300;
    }
    FUN_10004e40(&local_10,"HTTP/1.0 %u IDM\r\n",0,uVar7);
    if (param_2[9] == 1) {
      FUN_10026110(&local_10,"X-IDM-Status: %llu\r\n",0,param_2[0x14],param_2[0x15]);
    }
    FUN_10026110(&local_10,
                 "Content-Type: text/plain\r\nContent-Length: %u\r\nAccess-Control-Allow-Origin: *\r\nCache-Control: no-cache\r\nPragma: no-cache\r\nConnection: close\r\n\r\n"
                 ,0x98,uVar5);
    piVar1 = (int *)(param_1 + 0x32c);
    FUN_10003720(*(int *)(param_1 + 0x330) + local_c + uVar5);
    sVar3 = local_c;
    puVar11 = local_10;
    iVar6 = *(int *)(param_1 + 0x330);
    uVar5 = iVar6 + local_c;
    if ((uVar5 <= *(uint *)(param_1 + 0x334)) ||
       ((iVar6 = FUN_10003720(uVar5), iVar6 != 0 &&
        (iVar6 = *(int *)(param_1 + 0x330), *piVar1 + iVar6 != 0)))) {
      memcpy((void *)(*piVar1 + iVar6),puVar11,sVar3);
      *(uint *)(param_1 + 0x330) = uVar5;
    }
    pvVar2 = (void *)*param_2;
    sVar3 = param_2[1];
    iVar6 = *(int *)(param_1 + 0x330);
    uVar5 = iVar6 + sVar3;
    if ((uVar5 <= *(uint *)(param_1 + 0x334)) ||
       ((iVar6 = FUN_10003720(uVar5), iVar6 != 0 &&
        (iVar6 = *(int *)(param_1 + 0x330), *piVar1 + iVar6 != 0)))) {
      memcpy((void *)(*piVar1 + iVar6),pvVar2,sVar3);
      *(uint *)(param_1 + 0x330) = uVar5;
    }
    sVar3 = param_2[5];
    if (sVar3 != 0) {
      iVar6 = *(int *)(param_1 + 0x330);
      pvVar2 = (void *)param_2[4];
      uVar5 = iVar6 + sVar3;
      if ((uVar5 <= *(uint *)(param_1 + 0x334)) ||
         ((iVar6 = FUN_10003720(uVar5), iVar6 != 0 &&
          (iVar6 = *(int *)(param_1 + 0x330), *(int *)(param_1 + 0x32c) + iVar6 != 0)))) {
        memcpy((void *)(*(int *)(param_1 + 0x32c) + iVar6),pvVar2,sVar3);
        *(uint *)(param_1 + 0x330) = uVar5;
      }
    }
    FUN_100337b0();
    *(undefined1 *)(param_1 + 0x30c) = 5;
    if (local_10 != &DAT_10074bf4) {
      free(local_10);
    }
  }
  else if (*(int *)(param_1 + 0x394) == -1) {
    if (*(int *)(param_1 + 0x340) != -1) {
      if (uVar5 < 0x7e) {
        iVar6 = 2;
      }
      else {
        iVar6 = (-(uint)(uVar5 < 0x10000) & 0xfffffffa) + 10;
      }
      iVar10 = FUN_10003720(*(int *)(param_1 + 0x330) + iVar6 + uVar5);
      if ((iVar10 == 0) ||
         (puVar13 = (undefined1 *)(*(int *)(param_1 + 0x330) + *(int *)(param_1 + 0x32c)),
         puVar13 == (undefined1 *)0x0)) goto LAB_10034f8b;
      puVar11 = (undefined4 *)(puVar13 + 2);
      *(int *)(param_1 + 0x330) = *(int *)(param_1 + 0x330) + iVar6 + uVar5;
      *puVar13 = 0x82;
      if (uVar5 < 0x7e) {
        puVar13[1] = (byte)uVar5 & 0x7f;
      }
      else if (uVar5 < 0x10000) {
        puVar13[1] = 0x7e;
        uVar4 = htons((u_short)uVar5);
        *(u_short *)(puVar13 + 2) = uVar4;
        puVar11 = (undefined4 *)(puVar13 + 4);
      }
      else {
        puVar13[1] = 0x7f;
        *puVar11 = 0;
        *(uint *)(puVar13 + 6) =
             uVar5 >> 0x18 | (uVar5 & 0xff0000) >> 8 | (uVar5 & 0xff00) << 8 | uVar5 * 0x1000000;
        puVar11 = (undefined4 *)(puVar13 + 10);
      }
      memcpy(puVar11,(void *)*param_2,param_2[1]);
      memcpy((void *)(param_2[1] + (int)puVar11),(void *)param_2[4],param_2[5]);
      FUN_100337b0();
    }
  }
  else {
    if (*(char *)(param_1 + 0x314) != '\0') {
      uVar5 = uVar5 + 2;
    }
    iVar6 = FUN_10003720(*(int *)(param_1 + 0x330) + uVar5 + 4);
    if ((iVar6 == 0) ||
       (puVar9 = (uint *)(*(int *)(param_1 + 0x32c) + *(int *)(param_1 + 0x330)),
       puVar9 == (uint *)0x0)) {
LAB_10034f8b:
      FUN_10033870(1);
      FUN_100334a0();
      return;
    }
    *(int *)(param_1 + 0x330) = *(int *)(param_1 + 0x330) + uVar5 + 4;
    _Dst = puVar9 + 1;
    *puVar9 = uVar5;
    if (*(char *)(param_1 + 0x314) != '\0') {
      *(undefined1 *)((uVar5 - 1) + (int)_Dst) = 0x22;
      *(undefined1 *)_Dst = 0x22;
      _Dst = (uint *)((int)puVar9 + 5);
    }
    memcpy(_Dst,(void *)*param_2,param_2[1]);
    memcpy((undefined1 *)(param_2[1] + (int)_Dst),(void *)param_2[4],param_2[5]);
    FUN_100333a0();
  }
  if (*(int *)(param_1 + 0x290) != 0) {
    iVar6 = *(int *)(param_1 + 0x290);
    DVar8 = GetTickCount();
    *(DWORD *)(param_1 + 0x294) = DVar8 + iVar6;
  }
  return;
}


