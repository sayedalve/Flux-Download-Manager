// Function: FUN_0058c7f0
// Entry Point: 0x0058c7f0
// Program: IDMan.exe


/* WARNING: Function: __alloca_probe replaced with injection: alloca_probe */

void __thiscall FUN_0058c7f0(int param_1,char param_2)

{
  char cVar1;
  wchar_t wVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  char *pcVar6;
  void *pvVar7;
  size_t sVar8;
  undefined4 uVar9;
  wchar_t *pwVar10;
  wchar_t *pwVar11;
  errno_t eVar12;
  LSTATUS LVar13;
  undefined2 *puVar14;
  wchar_t *pwVar15;
  char *pcVar16;
  char *pcVar17;
  bool bVar18;
  uint local_5ac0;
  char *local_5a70;
  char *local_5a68;
  char *local_5a64;
  char *local_5a60;
  undefined1 *local_5a5c;
  void *local_5a58;
  undefined1 *puStack_5a54;
  uint local_5a50;
  undefined2 *local_5a4c;
  undefined2 local_5a48 [2];
  undefined4 local_5a44;
  wchar_t *local_5a40;
  wchar_t local_5a3c [2];
  undefined4 local_5a38;
  wchar_t *local_5a34;
  wchar_t local_5a30 [2];
  undefined4 local_5a2c;
  wchar_t *local_5a28;
  wchar_t local_5a24 [4];
  wchar_t *local_5a1c;
  wchar_t local_5a18 [4];
  wchar_t local_5a10 [10240];
  wchar_t local_a10 [256];
  wchar_t local_810 [256];
  wchar_t local_610 [256];
  wchar_t local_410 [256];
  wchar_t local_210;
  undefined1 local_20e [510];
  char *local_10 [2];
  uint local_8;
  
  puStack_5a54 = &LAB_0067f7a4;
  local_5a58 = ExceptionList;
  local_5ac0 = DAT_007839c0 ^ (uint)&local_5a4c;
  local_5a5c = (undefined1 *)&local_5ac0;
  ExceptionList = &local_5a58;
  local_5a50 = 0;
  local_8 = local_5ac0;
  FUN_00403060(*(undefined4 *)(param_1 + 0x3258));
  iVar5 = *(int *)(param_1 + 0x3258);
  iVar4 = FUN_005803a0(iVar5);
  iVar5 = __memicmp((void *)(iVar5 + iVar4),"content-length:",0xf);
  if (iVar5 == 0) {
    if (param_2 == '\0') {
      FID_conflict__sscanf
                ((char *)(iVar4 + 0xf + *(int *)(param_1 + 0x3258)),"%I64d",param_1 + 0x3288);
    }
    else {
      FID_conflict__sscanf
                ((char *)(iVar4 + 0xf + *(int *)(param_1 + 0x3258)),"%I64d",
                 (int *)(param_1 + 0x3220));
      if ((*(int *)(param_1 + 0x3220) == 0) && (*(int *)(param_1 + 0x3224) == 0)) {
        for (pcVar16 = (char *)(iVar4 + 0xf + *(int *)(param_1 + 0x3258)); *pcVar16 == ' ';
            pcVar16 = pcVar16 + 1) {
        }
        if ((*pcVar16 == '0') && (*(int *)(param_1 + 0x3208) != 0)) {
          *(undefined1 *)(*(int *)(param_1 + 0x3208) + 0x889) = 1;
        }
        if ((-1 < *(int *)(param_1 + 0x5c)) &&
           ((0 < *(int *)(param_1 + 0x5c) || (*(int *)(param_1 + 0x58) != 0)))) {
          iVar5 = *(int *)(param_1 + 100);
          if ((-1 < iVar5) &&
             ((((0 < iVar5 || (*(int *)(param_1 + 0x60) != 0)) &&
               (*(int *)(param_1 + 0x60) == *(int *)(param_1 + 0x78))) &&
              ((iVar5 == *(int *)(param_1 + 0x7c) && (*(int *)(param_1 + 0x3208) != 0)))))) {
            *(undefined1 *)(*(int *)(param_1 + 0x3208) + 0x25f) = 1;
          }
        }
      }
      if ((*(int *)(param_1 + 0x3224) < 1) && (*(int *)(param_1 + 0x3224) < 0)) {
        *(undefined4 *)(param_1 + 0x3220) = 0;
        *(undefined4 *)(param_1 + 0x3224) = 0;
      }
    }
    goto LAB_0058c958;
  }
  if ((((*(int *)(param_1 + 0x31fc) == 0) && (param_2 != '\0')) &&
      (iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"accept-ranges:",0xe),
      iVar5 == 0)) && (*(int *)(param_1 + 0xc4) == 0)) {
    iVar5 = iVar4 + 0xe + *(int *)(param_1 + 0x3258);
    iVar4 = FUN_005803a0(iVar5);
    pcVar16 = _strstr((char *)(iVar5 + iVar4),"none");
    if (pcVar16 == (char *)0x0) {
      *(undefined4 *)(param_1 + 0x31fc) = 1;
    }
    else {
      if (*(int **)(param_1 + 0x3208) != (int *)0x0) {
        (**(code **)(**(int **)(param_1 + 0x3208) + 0xc))(2);
      }
      *(undefined4 *)(param_1 + 0x31fc) = 2;
    }
    goto LAB_0058c958;
  }
  iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"Content-Range:",0xe);
  if ((iVar5 == 0) && (param_2 != '\0')) {
    pcVar16 = _strchr((char *)(iVar4 + 0xe + *(int *)(param_1 + 0x3258)),0x2f);
    if (pcVar16 != (char *)0x0) {
      cVar1 = pcVar16[1];
      if ((cVar1 != '\r') && (cVar1 != '\n')) {
        FID_conflict__sscanf(pcVar16 + 1,"%I64d",param_1 + 0x3290);
      }
    }
    pcVar16 = _strchr((char *)(iVar4 + 0xe + *(int *)(param_1 + 0x3258)),0x2d);
    if (pcVar16 != (char *)0x0) {
      cVar1 = pcVar16[1];
      if ((cVar1 != '\r') && (cVar1 != '\n')) {
        iVar5 = FID_conflict__sscanf(pcVar16 + 1,"%I64d",(undefined4 *)(param_1 + 0x3298));
        if (iVar5 < 1) {
          *(undefined4 *)(param_1 + 0x3298) = 0;
          *(undefined4 *)(param_1 + 0x329c) = 0;
        }
      }
    }
    goto LAB_0058c958;
  }
  iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"Transfer-Encoding:",0x12);
  if (iVar5 == 0) {
    pcVar16 = (char *)(iVar4 + 0x12 + *(int *)(param_1 + 0x3258));
    while (pcVar16 != (char *)0x0) {
      for (; *pcVar16 == ' '; pcVar16 = pcVar16 + 1) {
      }
      iVar5 = __memicmp(pcVar16,"chunked",7);
      if (iVar5 == 0) {
        *(undefined1 *)(param_1 + 0xe0) = 1;
        break;
      }
      pcVar16 = _strchr(pcVar16,0x2c);
      if (pcVar16 != (char *)0x0) {
        pcVar16 = pcVar16 + 1;
      }
    }
    goto LAB_0058c958;
  }
  iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"Server: BestHop 2.4.4",0x15);
  if (((iVar5 == 0) && (param_2 != '\0')) ||
     ((iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"Server: MediaFire-HTTP-lrbd",
                         0x1b), iVar5 == 0 && (param_2 != '\0')))) {
    if (*(int *)(param_1 + 0x3208) != 0) {
      *(undefined1 *)(*(int *)(param_1 + 0x3208) + 0x25f) = 1;
    }
    goto LAB_0058c958;
  }
  iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"Content-Encoding:",0x11);
  if ((iVar5 == 0) && (param_2 != '\0')) {
    for (pcVar16 = (char *)(iVar4 + 0x11 + *(int *)(param_1 + 0x3258)); *pcVar16 == ' ';
        pcVar16 = pcVar16 + 1) {
    }
    iVar5 = __memicmp(pcVar16,&DAT_006cf068,4);
    if ((((iVar5 == 0) || (iVar5 = __memicmp(pcVar16,"x-gzip",6), iVar5 == 0)) ||
        (iVar5 = __memicmp(pcVar16,"deflate",7), iVar5 == 0)) ||
       (iVar5 = __memicmp(pcVar16,"x-deflate",9), iVar5 == 0)) {
      *(undefined1 *)(param_1 + 0x21) = 1;
      *(undefined1 *)(*(int *)(param_1 + 200) + 0x363) = 1;
    }
    iVar5 = __memicmp(pcVar16,&DAT_006cf048,3);
    if (((iVar5 == 0) || (iVar5 = __memicmp(pcVar16,&DAT_006cf044,3), iVar5 == 0)) ||
       (iVar5 = __memicmp(pcVar16,"brotli",6), iVar5 == 0)) {
      *(undefined1 *)(param_1 + 0x20) = 1;
      *(undefined1 *)(param_1 + 0x3281) = 1;
    }
    goto LAB_0058c958;
  }
  iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"WWW-Authenticate:",0x11);
  if (iVar5 == 0) {
    for (pcVar16 = (char *)(iVar4 + 0x11 + *(int *)(param_1 + 0x3258));
        (*pcVar16 == ' ' || (pcVar17 = pcVar16, *pcVar16 == '\t')); pcVar16 = pcVar16 + 1) {
    }
    do {
      iVar5 = __memicmp(pcVar17,"realm",5);
      if (iVar5 == 0) {
        pcVar17 = pcVar17 + 5;
        goto LAB_0058ccf0;
      }
      pcVar6 = _strchr(pcVar17,0x20);
    } while (((pcVar6 != (char *)0x0) || (pcVar6 = _strchr(pcVar17,9), pcVar6 != (char *)0x0)) &&
            (pcVar17 = pcVar6 + 1, *pcVar17 != '\0'));
LAB_0058cdbc:
    iVar5 = __memicmp(pcVar16,"Basic",5);
    if (iVar5 != 0) {
      *(undefined1 *)(*(int *)(param_1 + 200) + 0x1be) = 0;
      iVar5 = __memicmp(pcVar16,"Digest",6);
      if ((iVar5 == 0) && (pcVar17 = _strstr(pcVar16,"NTLM"), pcVar17 == (char *)0x0)) {
        pcVar17 = _strstr(pcVar16,"\r\n");
        if (((pcVar17 != (char *)0x0) || (pcVar17 = _strstr(pcVar16,"\n"), pcVar17 != (char *)0x0))
           && (sVar8 = (int)pcVar17 - (int)pcVar16, sVar8 != 0)) {
          *(undefined1 *)(*(int *)(param_1 + 200) + 0x1bf) = 1;
          FUN_004024f0();
          local_5a50._0_1_ = 1;
          pvVar7 = (void *)FUN_00402840(sVar8 + 1);
          _memcpy(pvVar7,pcVar16,sVar8);
          *(undefined1 *)((int)pvVar7 + sVar8) = 0;
          FUN_00583e10(local_5a60,"nonce",*(int *)(param_1 + 200) + 0x2fc);
          FUN_00583e10(local_5a60,"realm",*(int *)(param_1 + 200) + 0x304);
          FUN_00583e10(local_5a60,&DAT_006cfc54,*(int *)(param_1 + 200) + 0x30c);
          FUN_00583e10(local_5a60,"algorithm",*(int *)(param_1 + 200) + 0x308);
          FUN_00583e10(local_5a60,"opaque",*(int *)(param_1 + 200) + 0x310);
          pcVar16 = _strstr(local_5a60,"stale");
          if ((pcVar16 != (char *)0x0) &&
             (pcVar16 = _strchr(pcVar16 + 5,0x3d), pcVar16 != (char *)0x0)) {
            pcVar16 = pcVar16 + 1;
            FUN_004024f0();
            local_5a50 = CONCAT31(local_5a50._1_3_,2);
            pcVar17 = _strchr(pcVar16,0x2c);
            if (pcVar17 == (char *)0x0) {
              FUN_00402f80(pcVar16);
            }
            else {
              sVar8 = (int)pcVar17 - (int)pcVar16;
              local_5a64 = (char *)FUN_00402840(sVar8 + 1);
              _memcpy(local_5a64,pcVar16,sVar8);
              local_5a64[sVar8] = '\0';
            }
            iVar5 = __stricmp(local_10[0],"true");
            if (iVar5 == 0) {
              *(undefined1 *)(param_1 + 0x32d4) = 0;
            }
            local_5a50._0_1_ = 1;
            FUN_00402560();
          }
          *(undefined4 *)(*(int *)(param_1 + 200) + 0x314) = 1;
          __time32((__time32_t *)&local_5a68);
          FUN_00402f20(*(int *)(param_1 + 200) + 0x300,&DAT_0069ca1c,local_5a68);
          local_5a50 = (uint)local_5a50._1_3_ << 8;
          FUN_00402560();
        }
      }
      else if ((*(int *)(param_1 + 0x32e4) == 0) && (*(int *)(param_1 + 0x32e8) == 0)) {
        sVar8 = 0;
        pcVar17 = _strstr(pcVar16,"\r\n");
        if (((((pcVar17 != (char *)0x0) || (pcVar17 = _strstr(pcVar16,"\n"), pcVar17 != (char *)0x0)
              ) && (sVar8 = (int)pcVar17 - (int)pcVar16, 6 < (int)sVar8)) &&
            ((iVar5 = __memicmp(pcVar16,"Bearer ",7), iVar5 == 0 &&
             (pcVar17 = _strstr(pcVar16,"NTLM"), pcVar17 == (char *)0x0)))) &&
           (pcVar17 = _strstr(pcVar16,"Digest"), pcVar17 == (char *)0x0)) {
          sVar8 = 6;
        }
        _memcpy(*(void **)(*(int *)(param_1 + 200) + 0x2f8),pcVar16,sVar8);
        *(undefined1 *)(sVar8 + *(int *)(*(int *)(param_1 + 200) + 0x2f8)) = 0;
      }
      else {
        pcVar16 = _strchr(pcVar16,0x20);
        pcVar17 = (char *)0x0;
        local_10[0] = (char *)0x0;
        local_5a68 = pcVar16;
        if (pcVar16 != (char *)0x0) {
          pcVar16 = pcVar16 + 1;
          local_5a68 = pcVar16;
          pcVar6 = _strstr(pcVar16,"\r\n");
          if ((pcVar6 != (char *)0x0) || (pcVar6 = _strstr(pcVar16,"\n"), pcVar6 != (char *)0x0)) {
            pcVar17 = pcVar6 + -(int)pcVar16;
            local_10[0] = pcVar17;
          }
        }
        if (*(int *)(param_1 + 0x32dc) != 0) {
          local_5a50 = CONCAT31(local_5a50._1_3_,3);
          FUN_00610e74(*(undefined4 *)(param_1 + 0x32dc));
          local_5a50 = 0;
          FUN_0058d12c();
          return;
        }
        if ((pcVar17 == (char *)0x0) && (*(int *)(param_1 + 0xa0) != 0)) {
          if (*(int *)(param_1 + 0x32e4) != 0) {
            FUN_00580b30(param_1 + 0x3324);
            *(undefined4 *)(param_1 + 0x32e4) = 0;
          }
          *(undefined4 *)(param_1 + 0x32e4) = 0;
          *(undefined4 *)(param_1 + 0x32e8) = 0;
        }
        else {
          pvVar7 = (void *)FUN_00610e69(pcVar17 + 1);
          *(void **)(param_1 + 0x32dc) = pvVar7;
          _memcpy(pvVar7,pcVar16,(size_t)pcVar17);
          pcVar17[*(int *)(param_1 + 0x32dc)] = '\0';
        }
      }
    }
    goto LAB_0058c958;
  }
  iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"Proxy-Authenticate:",0x13);
  if (iVar5 == 0) {
    for (pcVar16 = (char *)(iVar4 + 0x13 + *(int *)(param_1 + 0x3258));
        (*pcVar16 == ' ' || (*pcVar16 == '\t')); pcVar16 = pcVar16 + 1) {
    }
    iVar5 = __memicmp(pcVar16,"Basic",5);
    if (iVar5 != 0) {
      iVar5 = __memicmp(pcVar16,"Digest",6);
      if (((iVar5 == 0) && (pcVar17 = _strstr(pcVar16,"NTLM"), pcVar17 == (char *)0x0)) &&
         (*(int *)(*(int *)(param_1 + 0x19c) + 8) != 2)) {
        *(undefined4 *)(*(int *)(param_1 + 0x19c) + 8) = 3;
        pcVar17 = _strstr(pcVar16,"\r\n");
        if (((pcVar17 != (char *)0x0) || (pcVar17 = _strstr(pcVar16,"\n"), pcVar17 != (char *)0x0))
           && (sVar8 = (int)pcVar17 - (int)pcVar16, sVar8 != 0)) {
          FUN_004024f0();
          local_5a50._0_1_ = 5;
          local_5a64 = (char *)FUN_00402840(sVar8 + 1);
          _memcpy(local_5a64,pcVar16,sVar8);
          local_5a64[sVar8] = '\0';
          FUN_004024f0();
          local_5a50._0_1_ = 6;
          uVar9 = FUN_0057f340((*(undefined4 **)(param_1 + 200))[9],**(undefined4 **)(param_1 + 200)
                              );
          FUN_00402f20(local_10,"%s/%s",uVar9);
          FUN_005add50(local_5a60,local_10[0]);
          local_5a50._0_1_ = 5;
          FUN_00402560();
          local_5a50 = (uint)local_5a50._1_3_ << 8;
          FUN_00402560();
        }
      }
      else {
        *(undefined4 *)(*(int *)(param_1 + 0x19c) + 8) = 2;
        if ((*(int *)(param_1 + 0x32f0) == 0) && (*(int *)(param_1 + 0x32f4) == 0)) {
          iVar5 = __stricmp(*(char **)(*(int *)(param_1 + 0x19c) + 0x10c),"NTLM");
          if (iVar5 != 0) {
            sVar8 = 0;
            pcVar17 = _strstr(pcVar16,"\r\n");
            if ((pcVar17 != (char *)0x0) ||
               (pcVar17 = _strstr(pcVar16,"\n"), pcVar17 != (char *)0x0)) {
              sVar8 = (int)pcVar17 - (int)pcVar16;
            }
            _memcpy(*(void **)(*(int *)(param_1 + 0x19c) + 0x10c),pcVar16,sVar8);
            *(undefined1 *)(sVar8 + *(int *)(*(int *)(param_1 + 0x19c) + 0x10c)) = 0;
          }
        }
        else {
          pcVar16 = _strchr(pcVar16,0x20);
          pcVar17 = (char *)0x0;
          local_10[0] = (char *)0x0;
          local_5a68 = pcVar16;
          if (pcVar16 != (char *)0x0) {
            pcVar16 = pcVar16 + 1;
            local_5a68 = pcVar16;
            pcVar6 = _strstr(pcVar16,"\r\n");
            if ((pcVar6 != (char *)0x0) || (pcVar6 = _strstr(pcVar16,"\n"), pcVar6 != (char *)0x0))
            {
              pcVar17 = pcVar6 + -(int)pcVar16;
              local_10[0] = pcVar17;
            }
          }
          if (*(int *)(param_1 + 0x32f8) != 0) {
            local_5a50 = CONCAT31(local_5a50._1_3_,7);
            FUN_00610e74(*(undefined4 *)(param_1 + 0x32f8));
            local_5a50 = 0;
            FUN_0058d43a();
            return;
          }
          if ((pcVar17 == (char *)0x0) && (*(int *)(param_1 + 0xa0) != 0)) {
            if (*(int *)(param_1 + 0x32f0) != 0) {
              FUN_00580b30(param_1 + 0x3300);
              *(undefined4 *)(param_1 + 0x32f0) = 0;
            }
            *(undefined4 *)(param_1 + 0x32f0) = 0;
            *(undefined4 *)(param_1 + 0x32f4) = 0;
          }
          else {
            pvVar7 = (void *)FUN_00610e69(pcVar17 + 1);
            *(void **)(param_1 + 0x32f8) = pvVar7;
            _memcpy(pvVar7,pcVar16,(size_t)pcVar17);
            pcVar17[*(int *)(param_1 + 0x32f8)] = '\0';
          }
        }
      }
    }
    goto LAB_0058c958;
  }
  iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"Location:",9);
  if ((iVar5 == 0) && (param_2 != '\0')) {
    if (*(int *)(param_1 + 0x31f8) != 0) {
      FUN_00610e74(*(int *)(param_1 + 0x31f8));
    }
    iVar5 = iVar4 + 9 + *(int *)(param_1 + 0x3258);
    iVar4 = FUN_005803a0(iVar5);
    pcVar17 = (char *)(iVar5 + iVar4);
    sVar8 = 0;
    pcVar16 = _strstr(pcVar17,"\r\n");
    if ((pcVar16 != (char *)0x0) || (pcVar16 = _strstr(pcVar17,"\n"), pcVar16 != (char *)0x0)) {
      sVar8 = (int)pcVar16 - (int)pcVar17;
    }
    pvVar7 = (void *)FUN_00610e69(sVar8 + 1);
    *(void **)(param_1 + 0x31f8) = pvVar7;
    _memcpy(pvVar7,pcVar17,sVar8);
    *(undefined1 *)(sVar8 + *(int *)(param_1 + 0x31f8)) = 0;
    goto LAB_0058c958;
  }
  iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"Content-Disposition:",0x14);
  if ((iVar5 != 0) || (param_2 == '\0')) {
    iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"Content-Type:",0xd);
    if ((iVar5 == 0) && (param_2 != '\0')) {
      iVar5 = *(int *)(param_1 + 0x3270);
      if (((iVar5 == 200) || (iVar5 == 0xce)) || (iVar5 == 0x130)) {
        iVar5 = *(int *)(*(int *)(param_1 + 200) + 0xbc);
        if (iVar5 != 0) {
          FUN_00610e74(iVar5);
        }
        iVar5 = iVar4 + 0xd + *(int *)(param_1 + 0x3258);
        iVar4 = FUN_005803a0(iVar5);
        pcVar17 = (char *)(iVar5 + iVar4);
        local_5a68 = (char *)0x0;
        pcVar16 = _strstr(pcVar17,"\r\n");
        local_10[0] = _strchr(pcVar17,0x3b);
        if ((((local_10[0] < pcVar16) && (local_10[0] != (char *)0x0)) || (pcVar16 == (char *)0x0))
           && (pcVar16 = local_10[0], local_10[0] == (char *)0x0)) {
          pcVar6 = _strstr(pcVar17,"\n");
          pcVar16 = local_5a68;
          if (pcVar6 != (char *)0x0) {
            pcVar16 = pcVar6 + -(int)pcVar17;
          }
        }
        else {
          local_5a68 = pcVar16 + -(int)pcVar17;
          pcVar16 = local_5a68;
        }
        uVar9 = FUN_00610e69(pcVar16 + 5);
        *(undefined4 *)(*(int *)(param_1 + 200) + 0xbc) = uVar9;
        _memcpy(*(void **)(*(int *)(param_1 + 200) + 0xbc),pcVar17,(size_t)pcVar16);
        pcVar16[*(int *)(*(int *)(param_1 + 200) + 0xbc)] = '\0';
        if ((*(char *)(*(int *)(param_1 + 200) + 0x179) == '\0') ||
           (iVar5 = __memicmp(*(void **)(*(int *)(param_1 + 200) + 0xbc),"text/html",9), iVar5 != 0)
           ) {
          pcVar17 = *(char **)(*(int *)(param_1 + 200) + 0xc4);
          pcVar16 = pcVar17 + 1;
          do {
            cVar1 = *pcVar17;
            pcVar17 = pcVar17 + 1;
          } while (cVar1 != '\0');
          if ((pcVar17 == pcVar16) ||
             (iVar5 = __memicmp(*(void **)(*(int *)(param_1 + 200) + 0xbc),"text/html",9),
             iVar5 != 0)) {
            iVar5 = __stricmp(*(char **)(*(int *)(param_1 + 200) + 0xbc),
                              "application/vnd.lumberjack.manifest");
            if ((iVar5 == 0) && ((*(uint *)(*(int *)(param_1 + 200) + 0x164) & 0x100) != 0)) {
              FUN_00610e74(*(undefined4 *)(*(int *)(param_1 + 200) + 0xbc));
              *(undefined4 *)(*(int *)(param_1 + 200) + 0xbc) = 0;
              *(undefined4 *)(*(int *)(param_1 + 200) + 0x348) = 1;
              *(undefined1 *)(*(int *)(param_1 + 200) + 0x34c) = 1;
            }
            else {
              iVar5 = __memicmp(*(void **)(*(int *)(param_1 + 200) + 0xbc),"multipart/byteranges",
                                0x14);
              if ((((iVar5 == 0) ||
                   (iVar5 = __memicmp(*(void **)(*(int *)(param_1 + 200) + 0xbc),
                                      "multipart/x-byteranges",0x16), iVar5 == 0)) &&
                  (local_10[0] != (char *)0x0)) &&
                 ((pcVar16 = _strstr(local_10[0],"boundary"), pcVar16 != (char *)0x0 &&
                  (pcVar16 = _strchr(pcVar16,0x3d), pcVar16 != (char *)0x0)))) {
                iVar5 = FUN_005803a0(pcVar16 + 1);
                pcVar17 = pcVar16 + 1 + iVar5;
                sVar8 = 0;
                pcVar16 = _strstr(pcVar17,"\r\n");
                if ((pcVar16 != (char *)0x0) ||
                   (pcVar16 = _strstr(pcVar17,"\n"), pcVar16 != (char *)0x0)) {
                  sVar8 = (int)pcVar16 - (int)pcVar17;
                }
                pvVar7 = (void *)FUN_00402840(sVar8);
                _memcpy(pvVar7,pcVar17,sVar8);
                pcVar17 = *(char **)(param_1 + 0x327c);
                pcVar16 = pcVar17 + 1;
                do {
                  cVar1 = *pcVar17;
                  pcVar17 = pcVar17 + 1;
                } while (cVar1 != '\0');
                if (pcVar17 != pcVar16) {
                  *(undefined1 *)(param_1 + 0x3280) = 1;
                }
              }
            }
            goto LAB_0058c958;
          }
        }
        FUN_00610e74(*(undefined4 *)(*(int *)(param_1 + 200) + 0xbc));
        *(undefined4 *)(*(int *)(param_1 + 200) + 0xbc) = 0;
      }
    }
    else {
      iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"Set-Cookie:",0xb);
      if ((iVar5 == 0) && (param_2 != '\0')) {
        FUN_00587d20(iVar4 + 0xb + *(int *)(param_1 + 0x3258));
      }
      else {
        iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"ETag:",5);
        if (iVar5 == 0) {
          iVar5 = iVar4 + 5 + *(int *)(param_1 + 0x3258);
          iVar4 = FUN_005803a0(iVar5);
          pcVar17 = (char *)(iVar5 + iVar4);
          sVar8 = 0;
          pcVar16 = _strstr(pcVar17,"\r\n");
          if ((pcVar16 != (char *)0x0) || (pcVar16 = _strstr(pcVar17,"\n"), pcVar16 != (char *)0x0))
          {
            sVar8 = (int)pcVar16 - (int)pcVar17;
          }
          pvVar7 = (void *)FUN_00402840(sVar8);
          _memcpy(pvVar7,pcVar17,sVar8);
        }
        else {
          iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),&DAT_006cfb38,4);
          if (iVar5 == 0) {
            iVar5 = iVar4 + 4 + *(int *)(param_1 + 0x3258);
            iVar4 = FUN_005803a0(iVar5);
            iVar5 = __memicmp((void *)(iVar5 + iVar4),"policyref=\"http://p3p.yahoo.com/w3c/",0x23);
            if (iVar5 == 0) {
              *(undefined1 *)(param_1 + 0x3282) = 1;
            }
          }
          else {
            iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"Date:",5);
            if ((iVar5 == 0) && (param_2 != '\0')) {
              iVar5 = iVar4 + 5 + *(int *)(param_1 + 0x3258);
              iVar4 = FUN_005803a0(iVar5);
              pcVar17 = (char *)(iVar5 + iVar4);
              sVar8 = 0;
              pcVar16 = _strstr(pcVar17,"\r\n");
              if ((pcVar16 != (char *)0x0) ||
                 (pcVar16 = _strstr(pcVar17,"\n"), pcVar16 != (char *)0x0)) {
                sVar8 = (int)pcVar16 - (int)pcVar17;
              }
              local_5a68 = (char *)FUN_00610e69(sVar8 + 1);
              _memcpy(local_5a68,pcVar17,sVar8);
              pcVar16 = local_5a68;
              local_5a68[sVar8] = '\0';
              FUN_00595500();
              local_5a50._0_1_ = 0x1c;
              uVar9 = FUN_00596440(pcVar16);
              *(undefined4 *)(*(int *)(param_1 + 200) + 0x3bc) = uVar9;
              FUN_00610e74(pcVar16);
              local_5a50 = (uint)local_5a50._1_3_ << 8;
              FUN_00595510();
            }
            else {
              iVar5 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar4),"Last-Modified:",0xe);
              if ((iVar5 == 0) && ((*(int *)(param_1 + 0xc4) == 0 && (param_2 != '\0')))) {
                iVar5 = iVar4 + 0xe + *(int *)(param_1 + 0x3258);
                iVar4 = FUN_005803a0(iVar5);
                pcVar17 = (char *)(iVar5 + iVar4);
                sVar8 = 0;
                pcVar16 = _strstr(pcVar17,"\r\n");
                if ((pcVar16 != (char *)0x0) ||
                   (pcVar16 = _strstr(pcVar17,"\n"), pcVar16 != (char *)0x0)) {
                  sVar8 = (int)pcVar16 - (int)pcVar17;
                }
                local_5a68 = (char *)FUN_00610e69(sVar8 + 1);
                _memcpy(local_5a68,pcVar17,sVar8);
                pcVar16 = local_5a68;
                local_5a68[sVar8] = '\0';
                pvVar7 = *(void **)(*(int *)(param_1 + 200) + 0x20);
                if (pvVar7 == (void *)0x0) {
                  FUN_005d9040(local_5a68,*(undefined4 *)(*(int *)(param_1 + 0x3208) + 0xb8));
                }
                else {
                  iVar5 = __memicmp(local_5a68,pvVar7,sVar8);
                  if ((iVar5 != 0) && (*(int *)(*(int *)(param_1 + 0x3208) + 0x144) == 0)) {
                    FUN_00402f20(param_1 + 0x3240,&DAT_00696f08,pcVar16);
                  }
                }
                FUN_00595500();
                local_5a50._0_1_ = 0x1d;
                uVar9 = FUN_00596440(pcVar16);
                *(undefined4 *)(*(int *)(param_1 + 200) + 0x3c0) = uVar9;
                FUN_00610e74(pcVar16);
                local_5a50 = (uint)local_5a50._1_3_ << 8;
                FUN_00595510();
              }
              local_5a50 = 0xffffffff;
            }
          }
        }
      }
    }
    goto LAB_0058c958;
  }
  if (((*(int *)(param_1 + 0x3270) != 200) && (*(int *)(param_1 + 0x3270) != 0xce)) ||
     (*(int *)(*(int *)(param_1 + 200) + 0x348) != 0)) goto LAB_0058c958;
  bVar18 = false;
  pcVar16 = _strstr((char *)(iVar4 + 0x14 + *(int *)(param_1 + 0x3258)),"attachment");
  if (pcVar16 == (char *)0x0) {
    pcVar16 = _strstr((char *)(iVar4 + 0x14 + *(int *)(param_1 + 0x3258)),"inline;");
    if (pcVar16 != (char *)0x0) {
      *(undefined1 *)(*(int *)(param_1 + 200) + 0x19e) = 1;
    }
  }
  else {
    bVar18 = true;
  }
  *(int *)(param_1 + 0x32bc) = *(int *)(param_1 + 0x32bc) + 1;
  FUN_005810e0(iVar4 + 0x14 + *(int *)(param_1 + 0x3258));
  pcVar16 = _strstr((char *)(iVar4 + 0x14 + *(int *)(param_1 + 0x3258)),"filename");
  if (pcVar16 == (char *)0x0) {
    pcVar16 = (char *)(iVar4 + 0x14 + *(int *)(param_1 + 0x3258));
    do {
      pcVar16 = _strpbrk(pcVar16,"Ff");
      while( true ) {
        if (pcVar16 == (char *)0x0) {
          if (bVar18) {
            FUN_00586460();
          }
          goto LAB_0058c958;
        }
        iVar5 = __memicmp(pcVar16,"filename",8);
        if (iVar5 == 0) goto LAB_0058d682;
        pcVar16 = pcVar16 + 2;
        if (*pcVar16 != '\0') break;
        pcVar16 = (char *)0x0;
      }
    } while( true );
  }
LAB_0058d682:
  pcVar16 = _strchr(pcVar16,0x3d);
  if (pcVar16 == (char *)0x0) goto LAB_0058c958;
  pcVar16 = pcVar16 + 1;
  if ((*pcVar16 == '\r') || (*pcVar16 == '\n')) goto LAB_0058c958;
  iVar5 = FUN_005803a0(pcVar16);
  pcVar16 = pcVar16 + iVar5;
  if (*pcVar16 == '\0') goto LAB_0058c958;
  bVar18 = true;
  if (*pcVar16 == '\"') {
    iVar5 = 0x22;
LAB_0058d6ce:
    pcVar16 = pcVar16 + 1;
    local_5a70 = _strchr(pcVar16,iVar5);
    if (local_5a70 == (char *)0x0) goto LAB_0058d86e;
    bVar18 = false;
LAB_0058d6e6:
    local_10[0] = local_5a70 + -(int)pcVar16;
  }
  else {
    if (*pcVar16 == '\'') {
      iVar5 = 0x27;
      goto LAB_0058d6ce;
    }
LAB_0058d86e:
    local_5a70 = _strstr(pcVar16,"\r\n");
    if ((local_5a70 != (char *)0x0) ||
       (local_5a70 = _strstr(pcVar16,"\n"), local_5a70 != (char *)0x0)) goto LAB_0058d6e6;
    local_10[0] = pcVar16;
    do {
      cVar1 = *local_10[0];
      local_10[0] = local_10[0] + 1;
    } while (cVar1 != '\0');
    local_10[0] = local_10[0] + -(int)(pcVar16 + 1);
    if ((local_10[0] == (char *)0x0) || (0x200 < (int)local_10[0])) goto LAB_0058c958;
  }
  FUN_004024f0();
  local_5a50 = CONCAT31(local_5a50._1_3_,9);
  pcVar17 = (char *)FUN_00402840(local_10[0]);
  _memcpy(pcVar17,pcVar16,(size_t)local_10[0]);
  local_10[0][(int)pcVar17] = '\0';
  if (bVar18) {
    pcVar16 = _strchr(local_5a60,0x3b);
    if ((pcVar16 != (char *)0x0) ||
       ((pcVar16 = _strchr(local_5a60,0x2e), pcVar16 != (char *)0x0 &&
        (pcVar16 = _strchr(pcVar16 + 1,0x2c), pcVar16 != (char *)0x0)))) {
      *pcVar16 = '\0';
      iVar5 = __memicmp(local_5a60,"utf-8\'\'",7);
      if ((iVar5 != 0) &&
         ((pcVar16 = _strstr(pcVar16 + 1,"filename"), pcVar16 != (char *)0x0 &&
          (pcVar16 = _strchr(pcVar16 + 8,0x3d), pcVar16 != (char *)0x0)))) {
        iVar5 = __memicmp(pcVar16 + 1,"utf-8\'\'",7);
        if (iVar5 == 0) {
          FUN_004024f0();
          local_5a50._0_1_ = 10;
          FUN_00402f20(local_10,&DAT_00696f08,pcVar16 + 1);
          pcVar16 = local_10[0];
          do {
            cVar1 = *pcVar16;
            pcVar16 = pcVar16 + 1;
          } while (cVar1 != '\0');
          sVar8 = (int)pcVar16 - (int)(local_10[0] + 1);
          pcVar17 = (char *)FUN_00402840(sVar8);
          _memcpy(pcVar17,local_10[0],sVar8);
          pcVar17[sVar8] = '\0';
          pcVar16 = _strchr(local_5a60,0x3b);
          if (pcVar16 != (char *)0x0) {
            *pcVar16 = '\0';
          }
          goto LAB_0058d99f;
        }
      }
    }
  }
  else {
    pcVar16 = local_5a70 + 1;
    local_5a64 = pcVar16;
    if ((*pcVar16 == '.') &&
       (((local_5a70 = _strstr(pcVar16,"\r\n"), local_5a70 != (char *)0x0 ||
         (local_5a70 = _strstr(pcVar16,"\n"), local_5a70 != (char *)0x0)) &&
        (sVar8 = (int)local_5a70 - (int)local_5a64, sVar8 - 2 < 4)))) {
      FUN_004024f0();
      local_5a50._0_1_ = 0xb;
      pvVar7 = (void *)FUN_00402840(sVar8);
      _memcpy(pvVar7,local_5a64,sVar8);
      *(undefined1 *)((int)pvVar7 + sVar8) = 0;
      FUN_004024f0();
      local_5a50._0_1_ = 0xc;
      FUN_00402f20(local_10,&DAT_00696e40,local_5a60,local_5a68);
      FUN_00402f20(&local_5a60,&DAT_00696f08,local_10[0]);
      pcVar17 = local_5a60;
      local_5a50._0_1_ = 0xb;
      FUN_00402560();
LAB_0058d99f:
      local_5a50 = CONCAT31(local_5a50._1_3_,9);
      FUN_00402560();
    }
  }
  iVar5 = __memicmp(pcVar17,"=?UTF-8?",8);
  if (iVar5 == 0) {
    FUN_005dd800(pcVar17,&local_5a60);
  }
  else {
    iVar5 = __memicmp(pcVar17,"=?UTF8?B?",9);
    if (iVar5 == 0) {
      FUN_005dd800(pcVar17,&local_5a60);
    }
    else {
      iVar5 = __memicmp(pcVar17,"ISO-8859\'\'",10);
      if (iVar5 == 0) {
        FUN_004030a0(local_5a60 + 10);
        local_5a50 = CONCAT31(local_5a50._1_3_,0xd);
        ATL::CSimpleStringT<char,0>::operator=
                  ((CSimpleStringT<char,0> *)&local_5a60,(CSimpleStringT<char,0> *)&local_5a64);
      }
      else {
        iVar5 = __memicmp(pcVar17,"ISO-8859-",9);
        if (iVar5 == 0) {
          FUN_004030a0(local_5a60 + 9);
          local_5a50 = CONCAT31(local_5a50._1_3_,0xe);
        }
        else {
          iVar5 = __memicmp(pcVar17,"windows-125",0xb);
          if (iVar5 != 0) goto LAB_0058dab3;
          FUN_004030a0(local_5a60 + 0xb);
          local_5a50 = CONCAT31(local_5a50._1_3_,0xf);
        }
        pcVar16 = _strstr(local_5a64,"\'\'");
        if ((pcVar16 != (char *)0x0) && (pcVar16[2] != '\0')) {
          FUN_00402f80(pcVar16 + 2);
        }
      }
      local_5a50 = CONCAT31(local_5a50._1_3_,9);
      FUN_00402560();
    }
  }
LAB_0058dab3:
  iVar5 = __memicmp(local_5a60,"utf-8\'",6);
  if ((iVar5 != 0) && (local_5a70 != (char *)0x0)) {
    pcVar16 = _strstr(local_5a70,"filename");
    if (pcVar16 == (char *)0x0) {
      do {
        pcVar16 = _strpbrk(local_5a70,"Ff");
        while( true ) {
          if (pcVar16 == (char *)0x0) goto LAB_0058dd09;
          iVar5 = __memicmp(pcVar16,"filename",8);
          if (iVar5 == 0) goto LAB_0058db33;
          local_5a70 = pcVar16 + 2;
          if (*local_5a70 != '\0') break;
          pcVar16 = (char *)0x0;
        }
      } while( true );
    }
LAB_0058db33:
    pcVar16 = _strchr(pcVar16,0x3d);
    if (pcVar16 != (char *)0x0) {
      pcVar16 = pcVar16 + 1;
      if ((*pcVar16 != '\r') && (*pcVar16 != '\n')) {
        iVar5 = FUN_005803a0(pcVar16);
        pcVar17 = pcVar16 + iVar5;
        if (*pcVar17 != '\0') {
          bVar18 = true;
          if (*pcVar17 == '\"') {
            pcVar17 = pcVar17 + 1;
            local_5a68 = _strchr(pcVar17,0x22);
            if (local_5a68 == (char *)0x0) goto LAB_0058dc1d;
            bVar18 = false;
LAB_0058dba2:
            local_5a68 = local_5a68 + -(int)pcVar17;
          }
          else {
LAB_0058dc1d:
            local_5a68 = _strstr(pcVar17,"\r\n");
            if ((local_5a68 != (char *)0x0) ||
               (local_5a68 = _strstr(pcVar17,"\n"), local_5a68 != (char *)0x0)) goto LAB_0058dba2;
            local_5a68 = pcVar17;
            do {
              cVar1 = *local_5a68;
              local_5a68 = local_5a68 + 1;
            } while (cVar1 != '\0');
            local_5a68 = local_5a68 + -(int)(pcVar17 + 1);
            if ((local_5a68 == (char *)0x0) || (0x100 < (int)local_5a68)) {
              local_5a50 = local_5a50 & 0xffffff00;
              FUN_00402560();
              goto LAB_0058c958;
            }
          }
          pcVar16 = (char *)FUN_00402840(local_5a68);
          _memcpy(pcVar16,pcVar17,(size_t)local_5a68);
          if ((bVar18) && (pcVar17 = _strchr(local_5a60,0x3b), pcVar17 != (char *)0x0)) {
            *pcVar17 = '\0';
          }
        }
      }
    }
    iVar5 = __memicmp(pcVar16,"ISO-8859\'\'",10);
    if (iVar5 == 0) {
      FUN_004030a0(local_5a60 + 10);
      local_5a50 = CONCAT31(local_5a50._1_3_,0x10);
      ATL::CSimpleStringT<char,0>::operator=
                ((CSimpleStringT<char,0> *)&local_5a60,(CSimpleStringT<char,0> *)&local_5a64);
    }
    else {
      iVar5 = __memicmp(pcVar16,"ISO-8859-",9);
      if (iVar5 == 0) {
        FUN_004030a0(local_5a60 + 9);
        local_5a50 = CONCAT31(local_5a50._1_3_,0x11);
      }
      else {
        iVar5 = __memicmp(pcVar16,"windows-125",0xb);
        if (iVar5 != 0) goto LAB_0058dd09;
        FUN_004030a0(local_5a60 + 0xb);
        local_5a50 = CONCAT31(local_5a50._1_3_,0x12);
      }
      pcVar16 = _strstr(local_5a64,"\'\'");
      if ((pcVar16 != (char *)0x0) && (pcVar16[2] != '\0')) {
        FUN_00402f80(pcVar16 + 2);
      }
    }
    local_5a50 = CONCAT31(local_5a50._1_3_,9);
    FUN_00402560();
  }
LAB_0058dd09:
  FUN_00406f90(local_5a60);
  local_5a50 = CONCAT31(local_5a50._1_3_,0x13);
  ATL::CSimpleStringT<char,0>::operator=
            ((CSimpleStringT<char,0> *)(*(int *)(param_1 + 200) + 0xc4),
             (CSimpleStringT<char,0> *)&local_5a60);
  iVar5 = __memicmp(local_5a60,"utf-8\'\'",7);
  if ((iVar5 == 0) || (iVar5 = __memicmp(local_5a60,"utf-8\' \'",8), iVar5 == 0)) {
    iVar5 = __memicmp(local_5a60,"utf-8\'\'",7);
    FUN_004030a0(local_5a60 + (iVar5 != 0) + 7);
    local_5a50._0_1_ = 0x14;
    FUN_00401970(local_5a68,0);
    ATL::CSimpleStringT<char,0>::operator=
              ((CSimpleStringT<char,0> *)(*(int *)(param_1 + 200) + 0xc4),
               (CSimpleStringT<char,0> *)&local_5a68);
    *(undefined1 *)(*(int *)(param_1 + 200) + 0x355) = 1;
    local_5a50 = CONCAT31(local_5a50._1_3_,0x13);
    FUN_00402560();
  }
  FUN_005c54f0(*(undefined4 *)(*(int *)(param_1 + 200) + 0xc4),
               -(uint)(*(char *)(*(int *)(param_1 + 200) + 0x355) != '\0') & 0xfde9,&local_5a1c,1,1,
               0,1);
  pcVar16 = *(char **)(*(int *)(param_1 + 200) + 0xbc);
  if (pcVar16 != (char *)0x0) {
    pcVar17 = pcVar16;
    do {
      cVar1 = *pcVar17;
      pcVar17 = pcVar17 + 1;
    } while (cVar1 != '\0');
    if ((pcVar17 != pcVar16 + 1) && (iVar5 = __memicmp(pcVar16,"text/html",9), iVar5 == 0)) {
      FUN_00610e74(*(undefined4 *)(*(int *)(param_1 + 200) + 0xbc));
      *(undefined4 *)(*(int *)(param_1 + 200) + 0xbc) = 0;
    }
  }
  if ((((*(int *)(param_1 + 0xc4) == 0) &&
       (iVar5 = *(int *)(param_1 + 200), *(int *)(iVar5 + 0x348) == 0)) &&
      (*(int *)(iVar5 + 0x33c) == 0)) &&
     ((*(char *)(iVar5 + 0x359) == '\0' &&
      (((*(uint *)(iVar5 + 0x164) & 0x100) == 0 || ((*(uint *)(iVar5 + 0x164) & 0x2000) == 0)))))) {
    pwVar10 = local_5a1c;
    if (local_5a1c == (wchar_t *)0x0) {
      pwVar10 = local_5a18;
    }
    pwVar10 = _wcschr(pwVar10,L'%');
    if (pwVar10 != (wchar_t *)0x0) {
      pwVar10 = local_5a1c;
      if (local_5a1c == (wchar_t *)0x0) {
        pwVar10 = local_5a18;
      }
      pwVar11 = pwVar10 + 1;
      do {
        wVar2 = *pwVar10;
        pwVar10 = pwVar10 + 1;
      } while (wVar2 != L'\0');
      if ((uint)((int)pwVar10 - (int)pwVar11 >> 1) < 0x2800) {
        pwVar10 = local_5a1c;
        if (local_5a1c == (wchar_t *)0x0) {
          pwVar10 = local_5a18;
        }
        pwVar11 = local_5a10;
        do {
          wVar2 = *pwVar10;
          *pwVar11 = wVar2;
          pwVar10 = pwVar10 + 1;
          pwVar11 = pwVar11 + 1;
        } while (wVar2 != L'\0');
        FUN_005bf9c0(local_5a10,&local_5a1c);
      }
    }
    pwVar10 = local_5a1c;
    if (local_5a1c == (wchar_t *)0x0) {
      pwVar10 = local_5a18;
    }
    pwVar11 = pwVar10 + 1;
    do {
      wVar2 = *pwVar10;
      pwVar10 = pwVar10 + 1;
    } while (wVar2 != L'\0');
    if (0x100 < (uint)((int)pwVar10 - (int)pwVar11 >> 1)) {
      pwVar10 = local_5a1c;
      if (local_5a1c == (wchar_t *)0x0) {
        pwVar10 = local_5a18;
      }
      pwVar11 = _wcsrchr(pwVar10,L'.');
      pwVar10 = local_5a1c;
      if (local_5a1c == (wchar_t *)0x0) {
        pwVar10 = local_5a18;
      }
      if (pwVar11 != (wchar_t *)0x0) {
        pwVar15 = pwVar11;
        do {
          wVar2 = *pwVar15;
          pwVar15 = pwVar15 + 1;
        } while (wVar2 != L'\0');
        if ((uint)((int)pwVar15 - (int)(pwVar11 + 1) >> 1) < 10) {
          pwVar15 = pwVar11;
          do {
            wVar2 = *pwVar15;
            pwVar15 = pwVar15 + 1;
          } while (wVar2 != L'\0');
          pwVar10[0xfd - ((int)pwVar15 - (int)(pwVar11 + 1) >> 1)] = L'\0';
          local_5a4c = (undefined2 *)0x0;
          local_5a44 = 0;
          local_5a48[0] = 0;
          local_5a50._0_1_ = 0x15;
          FUN_00402310(&DAT_00696e40,pwVar10,pwVar11,0);
          puVar14 = local_5a4c;
          if (local_5a4c == (undefined2 *)0x0) {
            puVar14 = local_5a48;
          }
          FUN_004017e0(puVar14);
          local_5a50 = CONCAT31(local_5a50._1_3_,0x13);
          FUN_004011f0();
          goto LAB_0058dfe9;
        }
      }
      pwVar10[0xfd] = L'\0';
    }
LAB_0058dfe9:
    puVar3 = *(undefined4 **)(*(int *)(param_1 + 0x3208) + 0x18);
    if ((puVar3 == (undefined4 *)0x0) || (*(int *)(*(int *)(param_1 + 200) + 0x3c) != 0)) {
      iVar5 = *(int *)(param_1 + 200);
      if (*(int *)(iVar5 + 0x3c) != 0) {
        if (*(int *)(iVar5 + 0x34) == 0) {
          pwVar10 = (wchar_t *)(iVar5 + 0x38);
        }
        else {
          pwVar10 = *(wchar_t **)(iVar5 + 0x34);
        }
        eVar12 = __wsplitpath_s(pwVar10,(wchar_t *)local_10,3,local_810,0x100,&local_210,0x100,
                                local_610,0x100);
        if (eVar12 == 0) {
          local_5a34 = (wchar_t *)0x0;
          local_5a2c = 0;
          local_5a30[0] = L'\0';
          local_5a50._0_1_ = 0x16;
          pwVar10 = local_5a1c;
          if (local_5a1c == (wchar_t *)0x0) {
            pwVar10 = local_5a18;
          }
          FUN_004110c0("%s%s%s",local_10,local_810,pwVar10,0);
          FUN_00402170(&local_210);
          local_5a50._0_1_ = 0x17;
          FUN_00402170(&local_210);
          local_5a50._0_1_ = 0x18;
          FUN_00405300(local_610);
          pwVar10 = local_5a1c;
          if (local_5a1c == (wchar_t *)0x0) {
            pwVar10 = local_5a18;
          }
          pwVar11 = local_5a28;
          if (local_5a28 == (wchar_t *)0x0) {
            pwVar11 = local_5a24;
          }
          iVar5 = __wcsicmp(pwVar11,pwVar10);
          if (iVar5 != 0) {
            pwVar10 = local_5a1c;
            if (local_5a1c == (wchar_t *)0x0) {
              pwVar10 = local_5a18;
            }
            pwVar10 = _wcschr(pwVar10,L'+');
            if (pwVar10 == (wchar_t *)0x0) {
LAB_0058e1f4:
              pwVar10 = local_5a28;
              if (local_5a28 == (wchar_t *)0x0) {
                pwVar10 = local_5a24;
              }
              pwVar10 = _wcsrchr(pwVar10,L'_');
              if (pwVar10 != (wchar_t *)0x0) {
                FUN_00420590(&local_5a28);
                local_5a50._0_1_ = 0x1a;
                pwVar10 = local_5a40;
                if (local_5a40 == (wchar_t *)0x0) {
                  pwVar10 = local_5a3c;
                }
                pwVar10 = _wcsrchr(pwVar10,L'_');
                if (pwVar10 != (wchar_t *)0x0) {
                  *pwVar10 = L'\0';
                  do {
                    pwVar10 = pwVar10 + 1;
                    wVar2 = *pwVar10;
                    if ((wVar2 == L'\0') || (wVar2 == L'.')) {
                      if (*pwVar10 == L'.') {
                        FUN_00405300(pwVar10);
                        pwVar10 = local_5a1c;
                        if (local_5a1c == (wchar_t *)0x0) {
                          pwVar10 = local_5a18;
                        }
                        pwVar11 = local_5a40;
                        if (local_5a40 == (wchar_t *)0x0) {
                          pwVar11 = local_5a3c;
                        }
                        iVar5 = __wcsicmp(pwVar11,pwVar10);
                        if (iVar5 == 0) goto LAB_0058e19b;
                      }
                      break;
                    }
                    iVar5 = _isdigit((uint)(ushort)wVar2);
                  } while (iVar5 != 0);
                }
                local_5a50._0_1_ = 0x18;
                FUN_004011f0();
              }
              if ((*(uint *)(*(int *)(param_1 + 200) + 0x164) & 0x2000) != 0) {
                pwVar10 = local_5a34;
                if (local_5a34 == (wchar_t *)0x0) {
                  pwVar10 = local_5a30;
                }
                eVar12 = __wsplitpath_s(pwVar10,(wchar_t *)local_10,3,local_810,0x100,&local_210,
                                        0x100,local_410,0x100);
                if ((eVar12 == 0) && (iVar5 = __wcsicmp(local_610,local_410), iVar5 != 0))
                goto LAB_0058e1a7;
              }
              if (*(int *)(*(int *)(param_1 + 0x3208) + 0x148) == 0) {
                pwVar10 = local_5a28;
                if (local_5a28 == (wchar_t *)0x0) {
                  pwVar10 = local_5a24;
                }
                pwVar11 = L"manuallogin.html";
                do {
                  wVar2 = *pwVar10;
                  bVar18 = (ushort)wVar2 < (ushort)*pwVar11;
                  if (wVar2 != *pwVar11) {
LAB_0058e3ea:
                    iVar5 = (1 - (uint)bVar18) - (uint)(bVar18 != 0);
                    goto LAB_0058e3ef;
                  }
                  if (wVar2 == L'\0') break;
                  wVar2 = pwVar10[1];
                  bVar18 = (ushort)wVar2 < (ushort)pwVar11[1];
                  if (wVar2 != pwVar11[1]) goto LAB_0058e3ea;
                  pwVar10 = pwVar10 + 2;
                  pwVar11 = pwVar11 + 2;
                } while (wVar2 != L'\0');
                iVar5 = 0;
LAB_0058e3ef:
                if (iVar5 == 0) {
                  pwVar10 = local_5a34;
                  if (local_5a34 == (wchar_t *)0x0) {
                    pwVar10 = local_5a30;
                  }
                  eVar12 = __wsplitpath_s(pwVar10,(wchar_t *)local_10,3,local_810,0x100,&local_210,
                                          0x100,local_410,0x100);
                  if ((eVar12 == 0) &&
                     ((iVar5 = __wcsicmp(local_410,L".htm"), iVar5 == 0 ||
                      (iVar5 = __wcsicmp(local_410,L".html"), iVar5 == 0)))) goto LAB_0058e1a7;
                }
                pwVar10 = local_5a34;
                if (local_5a34 == (wchar_t *)0x0) {
                  pwVar10 = local_5a30;
                }
                eVar12 = __wsplitpath_s(pwVar10,(wchar_t *)local_10,3,local_810,0x100,&local_210,
                                        0x100,local_a10,0x100);
                if ((eVar12 == 0) && (iVar5 = __wcsicmp(local_610,local_a10), iVar5 == 0)) {
                  if ((*(char *)(*(int *)(param_1 + 200) + 0x1a3) != '\0') ||
                     ((*(byte *)(*(int *)(param_1 + 200) + 0x164) & 0x40) != 0)) goto LAB_0058e1a7;
                  pwVar10 = &local_210;
                  do {
                    wVar2 = *pwVar10;
                    pwVar10 = pwVar10 + 1;
                  } while (wVar2 != L'\0');
                  pwVar11 = local_5a28;
                  if (local_5a28 == (wchar_t *)0x0) {
                    pwVar11 = local_5a24;
                  }
                  iVar5 = _wcsncmp(&local_210,pwVar11,(int)pwVar10 - (int)local_20e >> 1);
                  if (iVar5 == 0) {
                    pwVar10 = &local_210;
                    do {
                      wVar2 = *pwVar10;
                      pwVar10 = pwVar10 + 1;
                    } while (wVar2 != L'\0');
                    pwVar11 = local_5a28;
                    if (local_5a28 == (wchar_t *)0x0) {
                      pwVar11 = local_5a24;
                    }
                    pwVar15 = pwVar11 + 1;
                    do {
                      wVar2 = *pwVar11;
                      pwVar11 = pwVar11 + 1;
                    } while (wVar2 != L'\0');
                    if ((uint)((int)pwVar10 - (int)local_20e >> 1) <
                        (uint)((int)pwVar11 - (int)pwVar15 >> 1)) {
                      pwVar10 = &local_210;
                      do {
                        wVar2 = *pwVar10;
                        pwVar10 = pwVar10 + 1;
                      } while (wVar2 != L'\0');
                      if (local_5a28 == (wchar_t *)0x0) {
                        local_5a28 = local_5a24;
                      }
                      if (local_5a28[(int)pwVar10 - (int)local_20e >> 1] == L'_') goto LAB_0058e1a7;
                    }
                  }
                }
                if (*(char *)(*(int *)(param_1 + 200) + 0x35a) == '\0') {
                  if (1 < *(int *)(param_1 + 0x32bc)) goto LAB_0058e1a7;
                  local_5a64 = (char *)0x4;
                  LVar13 = RegQueryValueExA(DAT_00788d08,"notaskfn2",(LPDWORD)0x0,(LPDWORD)0x0,
                                            (LPBYTE)&local_5a68,(LPDWORD)&local_5a64);
                  if ((LVar13 != 0) || (local_5a68 != (char *)0x1)) {
                    local_5a40 = (wchar_t *)0x0;
                    local_5a38 = 0;
                    local_5a3c[0] = L'\0';
                    local_5a50._0_1_ = 0x1b;
                    if (local_5a1c == (wchar_t *)0x0) {
                      local_5a1c = local_5a18;
                    }
                    puVar14 = local_5a4c;
                    if (local_5a4c == (undefined2 *)0x0) {
                      puVar14 = local_5a48;
                    }
                    FUN_004110c0(DAT_00789e04,puVar14,local_610,local_5a1c,DAT_00788e50);
                    pwVar10 = local_5a40;
                    if (local_5a40 == (wchar_t *)0x0) {
                      pwVar10 = local_5a3c;
                    }
                    iVar5 = FUN_00475070(0,pwVar10,"Internet Download Manager",0x50024);
                    if (iVar5 == 7) goto LAB_0058e19b;
                    local_5a50._0_1_ = 0x18;
                    FUN_004011f0();
                  }
                }
              }
              else {
                pwVar10 = local_5a34;
                if (local_5a34 == (wchar_t *)0x0) {
                  pwVar10 = local_5a30;
                }
                eVar12 = __wsplitpath_s(pwVar10,(wchar_t *)local_10,3,local_810,0x100,&local_210,
                                        0x100,local_410,0x100);
                if ((eVar12 == 0) && (iVar5 = __wcsicmp(local_610,local_410), iVar5 == 0))
                goto LAB_0058e1a7;
              }
              pwVar10 = local_5a34;
              if (local_5a34 == (wchar_t *)0x0) {
                pwVar10 = local_5a30;
              }
              FUN_005d5870(pwVar10);
              iVar5 = *(int *)(*(int *)(param_1 + 0x3208) + 0xb8);
              if (iVar5 != 0) {
                FUN_005d75a0(iVar5);
              }
              puVar3 = *(undefined4 **)(*(int *)(param_1 + 0x3208) + 0x14);
              if (puVar3 != (undefined4 *)0x0) {
                SendMessageA((HWND)*puVar3,0x111,0x13c9,
                             *(LPARAM *)(*(int *)(param_1 + 0x3208) + 0xb8));
              }
            }
            else {
              pwVar10 = local_5a1c;
              if (local_5a1c == (wchar_t *)0x0) {
                pwVar10 = local_5a18;
              }
              FUN_00402170(pwVar10);
              local_5a50._0_1_ = 0x19;
              FUN_0040f3e0(0x2b,0x20);
              pwVar10 = local_5a40;
              if (local_5a40 == (wchar_t *)0x0) {
                pwVar10 = local_5a3c;
              }
              pwVar11 = local_5a28;
              if (local_5a28 == (wchar_t *)0x0) {
                pwVar11 = local_5a24;
              }
              iVar5 = __wcsicmp(pwVar11,pwVar10);
              if (iVar5 != 0) {
                local_5a50._0_1_ = 0x18;
                FUN_004011f0();
                goto LAB_0058e1f4;
              }
LAB_0058e19b:
              local_5a50._0_1_ = 0x18;
              FUN_004011f0();
            }
          }
LAB_0058e1a7:
          local_5a50._0_1_ = 0x17;
          FUN_004011f0();
          local_5a50._0_1_ = 0x16;
          FUN_004011f0();
          local_5a50 = CONCAT31(local_5a50._1_3_,0x13);
          FUN_004011f0();
        }
      }
    }
    else if ((*(uint *)(*(int *)(param_1 + 200) + 0x164) & 0x2000) == 0) {
      if (local_5a1c == (wchar_t *)0x0) {
        local_5a1c = local_5a18;
      }
      SendMessageA((HWND)*puVar3,0x111,0x13bc,(LPARAM)local_5a1c);
    }
    else {
      if (local_5a1c == (wchar_t *)0x0) {
        local_5a1c = local_5a18;
      }
      SendMessageA((HWND)*puVar3,0x111,0x148f,(LPARAM)local_5a1c);
    }
  }
  local_5a50._0_1_ = 9;
  FUN_004011f0();
  local_5a50 = (uint)local_5a50._1_3_ << 8;
  FUN_00402560();
LAB_0058c958:
  ExceptionList = local_5a58;
  __security_check_cookie(local_8 ^ (uint)&local_5a4c);
  return;
LAB_0058ccf0:
  if (*pcVar17 == '\0') goto LAB_0058cdbc;
  if (*pcVar17 == '=') {
    if ((*pcVar17 == '\0') || (pcVar17 = pcVar17 + 1, *pcVar17 == '\0')) goto LAB_0058cdbc;
    iVar5 = FUN_005803a0(pcVar17);
    pcVar17 = pcVar17 + iVar5;
    if (*pcVar17 == '\0') goto LAB_0058cdbc;
    if (*pcVar17 == '\"') {
      pcVar17 = pcVar17 + 1;
      if (*pcVar17 == '\0') goto LAB_0058cdbc;
      pcVar6 = _strchr(pcVar17,0x22);
LAB_0058cd60:
      if (pcVar6 == (char *)0x0) goto LAB_0058cdbc;
    }
    else {
      pcVar6 = _strstr(pcVar17,"\r\n");
      if (pcVar6 == (char *)0x0) {
        pcVar6 = _strstr(pcVar17,"\n");
        goto LAB_0058cd60;
      }
    }
    local_5a68 = pcVar6 + -(int)pcVar17;
    if (0 < (int)local_5a68) {
      pvVar7 = (void *)FUN_00402840(local_5a68);
      _memcpy(pvVar7,pcVar17,(size_t)local_5a68);
    }
    goto LAB_0058cdbc;
  }
  pcVar17 = pcVar17 + 1;
  goto LAB_0058ccf0;
}


