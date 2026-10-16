// Function: FUN_004b8980
// Entry Point: 0x004b8980
// Program: IDMan.exe


void FUN_004b8980(HWND param_1,UINT param_2,int *param_3,int param_4)

{
  char cVar1;
  char *pcVar2;
  undefined4 *puVar3;
  int iVar4;
  char *pcVar5;
  char *pcVar6;
  DWORD DVar7;
  int iVar8;
  char *pcVar9;
  HGLOBAL pvVar10;
  uint uVar11;
  uint uVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  undefined4 uVar15;
  uint uStack_70;
  undefined4 local_60;
  undefined4 local_5c;
  undefined1 *local_58;
  undefined4 local_54;
  undefined4 local_50;
  undefined4 local_4c;
  undefined4 local_48;
  HGLOBAL local_44;
  HGLOBAL local_40;
  int local_3c;
  int local_38;
  char *local_34;
  undefined1 *local_30;
  undefined1 local_2c [8];
  undefined1 *local_24;
  undefined1 local_20 [8];
  uint local_18;
  undefined1 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  int local_8;
  
  puStack_c = &LAB_0066cf88;
  local_10 = ExceptionList;
  uStack_70 = DAT_007839c0 ^ (uint)&stack0xfffffffc;
  local_14 = (undefined1 *)&uStack_70;
  ExceptionList = &local_10;
  local_8 = 0;
  local_18 = uStack_70;
  if (param_2 == 2) {
    PostQuitMessage(0);
    DAT_0078977c = 0;
    DAT_00789778 = 0;
    FUN_004b8def();
    return;
  }
  if (param_2 != 0x1491) {
    local_8 = 0xffffffff;
    local_14 = (undefined1 *)&uStack_70;
    DefWindowProcA(param_1,param_2,(WPARAM)param_3,param_4);
    FUN_004b8def();
    return;
  }
  local_38 = 0;
  if (param_4 == 0) {
    param_4 = DAT_007898d4;
  }
  uVar15 = 0;
  uVar14 = 0;
  uVar13 = 0;
  local_14 = (undefined1 *)&uStack_70;
  iVar4 = (*DAT_00789768)(0,param_4,0,0,0);
  if (iVar4 == 0) {
    *param_3 = (int)s_error_0077e654;
  }
  else {
    local_34 = (char *)0x0;
    pcVar5 = (char *)param_3[2];
    pcVar9 = pcVar5 + 1;
    do {
      cVar1 = *pcVar5;
      pcVar5 = pcVar5 + 1;
    } while (cVar1 != '\0');
    pcVar2 = (char *)param_3[1];
    pcVar6 = pcVar2;
    do {
      cVar1 = *pcVar6;
      pcVar6 = pcVar6 + 1;
    } while (cVar1 != '\0');
    iVar4 = (*DAT_0078976c)(pcVar2,(int)pcVar6 - (int)(pcVar2 + 1),param_3[2],
                            (int)pcVar5 - (int)pcVar9,param_3,&local_34);
    if ((iVar4 == 0) || (*param_3 == 0)) {
      local_34 = (char *)0x0;
      *param_3 = 0;
      pcVar5 = (char *)param_3[2];
      pcVar9 = pcVar5 + 1;
      do {
        cVar1 = *pcVar5;
        pcVar5 = pcVar5 + 1;
      } while (cVar1 != '\0');
      pcVar2 = (char *)param_3[1];
      pcVar6 = pcVar2;
      do {
        cVar1 = *pcVar6;
        pcVar6 = pcVar6 + 1;
      } while (cVar1 != '\0');
      iVar4 = (*DAT_0078976c)(pcVar2,(int)pcVar6 - (int)(pcVar2 + 1),param_3[2],
                              (int)pcVar5 - (int)pcVar9,param_3,&local_34);
      if ((iVar4 == 0) || (*param_3 == 0)) {
        DVar7 = GetLastError();
        if (DVar7 == 0x3eb) {
          *param_3 = (int)s_error_0077e654;
        }
      }
      else {
        local_38 = 1;
      }
    }
    else {
      local_38 = 1;
    }
    (*DAT_00789770)(0,0);
    if (local_38 != 0) goto LAB_004b8dc1;
  }
  pcVar5 = (char *)param_3[3];
  pcVar9 = pcVar5 + 1;
  do {
    cVar1 = *pcVar5;
    pcVar5 = pcVar5 + 1;
  } while (cVar1 != '\0');
  if (pcVar5 != pcVar9) {
    if ((DAT_00789790 == (HMODULE)0x0) &&
       (DAT_00789790 = (HMODULE)FUN_00446ef0(L"Winhttp.dll"), DAT_00789790 != (HMODULE)0x0)) {
      DAT_00789788 = GetProcAddress(DAT_00789790,"WinHttpGetProxyForUrl");
      DAT_00789784 = GetProcAddress(DAT_00789790,"WinHttpOpen");
      DAT_0078978c = GetProcAddress(DAT_00789790,"WinHttpCloseHandle");
    }
    if (((DAT_00789784 != (FARPROC)0x0) && (DAT_00789788 != (FARPROC)0x0)) &&
       (iVar4 = (*DAT_00789784)(L"Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko)"
                                ,1,0,0,0), local_3c = iVar4, iVar4 != 0)) {
      FUN_00406f90(param_3[3]);
      local_8._0_1_ = 1;
      local_60 = 2;
      if (local_30 == (undefined1 *)0x0) {
        local_30 = local_2c;
      }
      local_5c = 0;
      local_4c = 1;
      local_54 = 0;
      local_50 = 0;
      local_48 = 0;
      local_44 = (HGLOBAL)0x0;
      local_40 = (HGLOBAL)0x0;
      local_58 = local_30;
      FUN_00406f90(param_3[1]);
      local_8._0_1_ = 2;
      if (local_24 == (undefined1 *)0x0) {
        local_24 = local_20;
      }
      iVar8 = (*DAT_00789788)(iVar4,local_24,&local_60,&local_48);
      if (iVar8 == 0) {
        GetLastError();
      }
      else {
        FUN_00417910(local_44);
        local_8 = CONCAT31(local_8._1_3_,3);
        pcVar9 = local_34;
        do {
          cVar1 = *pcVar9;
          pcVar9 = pcVar9 + 1;
        } while (cVar1 != '\0');
        pvVar10 = GlobalAlloc(0,(SIZE_T)(pcVar9 + (8 - (int)(local_34 + 1))));
        *param_3 = (int)pvVar10;
        if (pvVar10 == (HGLOBAL)0x0) {
          *param_3 = (int)s_error_0077e654;
        }
        else {
          iVar4 = __memicmp(local_34,&DAT_00697050,8);
          if (iVar4 == 0) {
            puVar3 = (undefined4 *)*param_3;
            *puVar3 = DAT_006a8c94;
            *(undefined2 *)(puVar3 + 1) = DAT_006a8c98;
            *(undefined1 *)((int)puVar3 + 6) = DAT_006a8c9a;
            pcVar5 = local_34;
          }
          else {
            iVar4 = __memicmp(local_34,&DAT_00697070,7);
            if (iVar4 == 0) {
              puVar3 = (undefined4 *)*param_3;
              *puVar3 = DAT_006a8c8c;
              *(undefined2 *)(puVar3 + 1) = DAT_006a8c90;
              pcVar5 = local_34;
            }
            else {
              iVar4 = __memicmp(local_34,"socks5://",9);
              if (iVar4 == 0) {
                puVar3 = (undefined4 *)*param_3;
                *puVar3 = DAT_006a8c84;
                puVar3[1] = DAT_006a8c88;
                pcVar5 = local_34;
              }
              else {
                iVar4 = __memicmp(local_34,"socks4://",9);
                if (iVar4 == 0) {
                  puVar3 = (undefined4 *)*param_3;
                  *puVar3 = DAT_006a8c7c;
                  puVar3[1] = DAT_006a8c80;
                  pcVar5 = local_34;
                }
                else {
                  iVar4 = __memicmp(local_34,"socks://",8);
                  pcVar9 = (char *)*param_3;
                  pcVar5 = local_34;
                  if (iVar4 == 0) {
                    *(undefined4 *)pcVar9 = s_SOCKS_006a8c68._0_4_;
                    *(undefined2 *)(pcVar9 + 4) = s_SOCKS_006a8c68._4_2_;
                    pcVar9[6] = s_SOCKS_006a8c68[6];
                  }
                  else {
                    *(undefined4 *)pcVar9 = DAT_006a8c60;
                    *(undefined2 *)(pcVar9 + 4) = DAT_006a8c64;
                    pcVar9[6] = DAT_006a8c66;
                  }
                }
              }
            }
          }
          do {
            cVar1 = *pcVar5;
            pcVar5 = pcVar5 + 1;
          } while (cVar1 != '\0');
          uVar11 = (int)pcVar5 - (int)local_34;
          pcVar9 = (char *)(*param_3 + -1);
          do {
            pcVar5 = pcVar9 + 1;
            pcVar9 = pcVar9 + 1;
          } while (*pcVar5 != '\0');
          pcVar5 = local_34;
          for (uVar12 = uVar11 >> 2; uVar12 != 0; uVar12 = uVar12 - 1) {
            *(undefined4 *)pcVar9 = *(undefined4 *)pcVar5;
            pcVar5 = pcVar5 + 4;
            pcVar9 = pcVar9 + 4;
          }
          for (uVar11 = uVar11 & 3; uVar11 != 0; uVar11 = uVar11 - 1) {
            *pcVar9 = *pcVar5;
            pcVar5 = pcVar5 + 1;
            pcVar9 = pcVar9 + 1;
          }
          local_38 = 1;
          iVar4 = local_3c;
        }
        if (local_44 != (HGLOBAL)0x0) {
          GlobalFree(local_44);
        }
        if (local_40 != (HGLOBAL)0x0) {
          GlobalFree(local_40);
        }
        local_8._0_1_ = 2;
        FUN_00402560();
      }
      if (DAT_0078978c != (FARPROC)0x0) {
        (*DAT_0078978c)(iVar4);
      }
      local_8._0_1_ = 1;
      FUN_004011f0();
      local_8 = (uint)local_8._1_3_ << 8;
      FUN_004011f0();
    }
  }
LAB_004b8dc1:
  FUN_004b8def(param_4,uVar13,uVar14,uVar15);
  return;
}


