// Function: FUN_100230a0
// Entry Point: 0x100230a0
// Program: idmcchandler7.dll


void __fastcall FUN_100230a0(int param_1,int param_2)

{
  char cVar1;
  void *pvVar2;
  int iVar3;
  LSTATUS LVar4;
  wchar_t *pwVar5;
  LPCSTR _Dst;
  undefined4 *puVar6;
  UINT UVar7;
  BSTR pOVar8;
  int iVar9;
  int *piVar10;
  int extraout_EDX;
  int extraout_EDX_00;
  int extraout_EDX_01;
  int extraout_EDX_02;
  int extraout_EDX_03;
  int extraout_EDX_04;
  int extraout_EDX_05;
  int extraout_EDX_06;
  int extraout_EDX_07;
  BSTR bstrString;
  int iVar11;
  char *pcVar12;
  undefined4 *puVar13;
  bool bVar14;
  char *pcVar15;
  void **ppvVar16;
  BSTR *ppOVar17;
  BSTR local_4c8;
  int *local_4c4;
  BSTR pOStack_4c0;
  uint uStack_4bc;
  undefined4 *puStack_4b8;
  int iStack_4b4;
  BSTR pOStack_4b0;
  int iStack_4ac;
  int iStack_4a8;
  int iStack_4a4;
  int iStack_4a0;
  void *pvStack_49c;
  int iStack_498;
  int iStack_494;
  BSTR pOStack_490;
  int iStack_48c;
  int iStack_488;
  LPCWSTR pWStack_484;
  int iStack_480;
  wchar_t *pwStack_47c;
  int iStack_478;
  LPCSTR pCStack_474;
  LPCSTR local_470;
  undefined1 auStack_46c [16];
  undefined1 auStack_45c [16];
  undefined1 auStack_44c [4];
  BSTR pOStack_448;
  undefined1 auStack_444 [3];
  undefined4 uStack_441;
  wchar_t local_420 [260];
  wchar_t local_218 [260];
  uint local_10;
  void *local_c;
  undefined1 *puStack_8;
  int iStack_4;
  
  iStack_4 = 0xffffffff;
  puStack_8 = &LAB_10048c0b;
  local_c = ExceptionList;
  local_10 = DAT_1005a88c ^ (uint)&local_4c8;
  ExceptionList = &local_c;
  if (9 < *(int *)(param_1 + 0x22c)) {
    pvVar2 = memchr(*(void **)(param_1 + 0x228),0x3c,*(int *)(param_1 + 0x22c) - 5);
    param_2 = extraout_EDX;
    while (pvVar2 != (void *)0x0) {
      pcVar12 = (char *)((int)pvVar2 + 1);
      iVar11 = (int)pcVar12 - *(int *)(param_1 + 0x228);
      if (*pcVar12 == '/') {
        cVar1 = *(char *)((int)pvVar2 + 2);
        pcVar12 = (char *)((int)pvVar2 + 2);
        while ((iVar11 = iVar11 + 1, cVar1 != '\0' &&
               ((((iVar3 = isspace((int)cVar1), iVar3 != 0 || (*pcVar12 == '\n')) ||
                 (*pcVar12 == '\r')) && (iVar11 < *(int *)(param_1 + 0x22c) + -4))))) {
          cVar1 = pcVar12[1];
          pcVar12 = pcVar12 + 1;
        }
        param_2 = *(int *)(param_1 + 0x22c) + -3;
        if (param_2 <= iVar11) break;
        iVar3 = _memicmp(pcVar12,&DAT_1004f454,3);
        if ((iVar3 == 0) ||
           (iVar3 = _memicmp(pcVar12,&DAT_1004f4b8,3), param_2 = extraout_EDX_00, iVar3 == 0)) {
          *(undefined4 *)(param_1 + 0x184) = 1;
          if (DAT_1005acac == (FARPROC)0x0) {
            if (DAT_1005aca8 == (HMODULE)0x0) {
              local_4c8 = (BSTR)0x104;
              LVar4 = RegQueryValueExW(DAT_1005ac60,L"ExePath",(LPDWORD)0x0,(LPDWORD)0x0,
                                       (LPBYTE)local_420,(LPDWORD)&local_4c8);
              param_2 = extraout_EDX_02;
              if ((LVar4 == 0) &&
                 (pwVar5 = wcsrchr(local_420,L'\\'), param_2 = extraout_EDX_03,
                 pwVar5 != (wchar_t *)0x0)) {
                *pwVar5 = L'\0';
                _snwprintf(local_218,0x104,L"%s\\idmvmprs.dll",local_420);
                DAT_1005aca8 = LoadLibraryW(local_218);
                GetLastError();
                param_2 = extraout_EDX_04;
              }
              if (DAT_1005aca8 != (HMODULE)0x0) goto LAB_1002327d;
            }
            else {
LAB_1002327d:
              DAT_1005acac = GetProcAddress(DAT_1005aca8,"Parse1");
              param_2 = extraout_EDX_05;
            }
            if (DAT_1005acac == (FARPROC)0x0) break;
          }
          local_4c4 = (int *)0x0;
          _Dst = operator_new(*(int *)(param_1 + 0x22c) + 1);
          param_2 = extraout_EDX_06;
          local_470 = _Dst;
          if (_Dst != (LPCSTR)0x0) {
            memcpy(_Dst,*(void **)(param_1 + 0x228),*(size_t *)(param_1 + 0x22c));
            _Dst[*(int *)(param_1 + 0x22c)] = '\0';
            (*DAT_1005abf0)(param_1 + 0x18,&local_4c8,0);
            iVar11 = (*DAT_1005acac)(local_4c8,_Dst,0,0,&local_4c4);
            if ((iVar11 != 0) && (local_4c4 != (int *)0x0)) {
              ppOVar17 = &local_4c8;
              ppvVar16 = &pvStack_49c;
              pcVar15 = "mp4";
              local_4c8 = (BSTR)0x0;
              pcVar12 = (char *)FUN_10003910(param_1 + 0x18);
              iVar11 = FUN_100164f0(pcVar12,pcVar15,(uint *)ppvVar16,ppOVar17);
              if (iVar11 != 0) {
                puVar6 = (undefined4 *)(**(code **)*local_4c4)();
                uStack_4bc = 0;
                bVar14 = puVar6 == (undefined4 *)0x0;
                if (bVar14) {
                  puStack_4b8 = puVar6;
                  puVar6 = (undefined4 *)(**(code **)(*local_4c4 + 4))();
                }
                uStack_4bc = (uint)bVar14;
                puStack_4b8 = puVar6;
                FUN_10003c00(&local_4c8,_Dst);
                iStack_4 = 0;
                while (puVar6 != (undefined4 *)0x0) {
                  (*DAT_1005abe4)(auStack_45c);
                  iStack_4._0_1_ = 1;
                  (*DAT_1005abe4)(auStack_46c);
                  iStack_4 = CONCAT31(iStack_4._1_3_,2);
                  if (puVar6[4] != 0 || puVar6[5] != 0) {
                    sprintf((char *)((int)&uStack_441 + 1),"%I64d",puVar6[4],puVar6[5]);
                    (*DAT_1005abfc)(auStack_45c,(int)&uStack_441 + 1,0xffffffff);
                  }
                  if ((puVar6[1] != 0) && (uStack_4bc == 0)) {
                    sprintf((char *)((int)&uStack_441 + 1),"%ldp",puVar6[1]);
                    if (0x2cf < (uint)puVar6[1]) {
                      puVar13 = &uStack_441;
                      do {
                        pcVar12 = (char *)((int)puVar13 + 1);
                        puVar13 = (undefined4 *)((int)puVar13 + 1);
                      } while (*pcVar12 != '\0');
                      *puVar13 = DAT_1004ceac;
                    }
                    FUN_10001090(auStack_46c,(int)&uStack_441 + 1);
                  }
                  if (puVar6[2] != 0) {
                    FUN_10001070(auStack_46c," bitrate ");
                    sprintf((char *)((int)&uStack_441 + 1),"%ldkbps",(uint)puVar6[2] >> 10);
                    FUN_10001070(auStack_46c,(int)&uStack_441 + 1);
                  }
                  pwVar5 = L"mpdatoidm://";
                  if (uStack_4bc == 0) {
                    pwVar5 = L"mpdtoidm://";
                  }
                  pOStack_4c0 = SysAllocString(pwVar5);
                  iStack_4._0_1_ = 3;
                  (*DAT_1005abf0)(param_1 + 0x18,&pCStack_474,0);
                  FUN_10003c00(&pOStack_4b0,pCStack_474);
                  bstrString = pOStack_4b0;
                  iStack_4._0_1_ = 4;
                  UVar7 = SysStringLen(pOStack_4b0);
                  FUN_10003b80(&pOStack_4c0,bstrString,UVar7);
                  pOVar8 = SysAllocString(L"&selected_id=");
                  iStack_4._0_1_ = 5;
                  pOStack_448 = pOVar8;
                  UVar7 = SysStringLen(pOVar8);
                  FUN_10003b80(&pOStack_4c0,pOVar8,UVar7);
                  iStack_4._0_1_ = 4;
                  SysFreeString(pOVar8);
                  FUN_10003c00(&pOStack_490,(LPCSTR)*puVar6);
                  pOVar8 = pOStack_490;
                  iStack_4._0_1_ = 6;
                  UVar7 = SysStringLen(pOStack_490);
                  FUN_10003b80(&pOStack_4c0,pOVar8,UVar7);
                  pvStack_49c = operator_new(0x2f8);
                  iStack_4._0_1_ = 7;
                  if (pvStack_49c == (void *)0x0) {
                    piVar10 = (int *)0x0;
                  }
                  else {
                    iVar11 = (*DAT_1005abf0)(auStack_46c,auStack_44c,0);
                    if (iVar11 == 0) {
                      iStack_4a0 = 0;
                    }
                    else {
                      iStack_4a0 = FUN_10003910(auStack_46c);
                    }
                    iVar11 = (*DAT_1005abf0)(auStack_45c,auStack_444,0);
                    if (iVar11 == 0) {
                      iStack_4a8 = 0;
                    }
                    else {
                      iStack_4a8 = FUN_10003910(auStack_45c);
                    }
                    iVar11 = FUN_10003870(param_1 + 0x118);
                    if (iVar11 == 0) {
                      iStack_4ac = 0;
                    }
                    else {
                      iStack_4ac = FUN_100038b0(param_1 + 0x118);
                    }
                    iVar11 = FUN_10003870(param_1 + 0x108);
                    if (iVar11 == 0) {
                      iStack_488 = 0;
                    }
                    else {
                      iStack_488 = FUN_100038b0(param_1 + 0x108);
                    }
                    iVar11 = FUN_10003870(param_1 + 0xf8);
                    if (iVar11 == 0) {
                      iStack_4a4 = 0;
                    }
                    else {
                      iStack_4a4 = FUN_100038b0(param_1 + 0xf8);
                    }
                    iVar11 = FUN_10003870(param_1 + 0xe8);
                    if (iVar11 == 0) {
                      iStack_480 = 0;
                    }
                    else {
                      iStack_480 = FUN_100038b0(param_1 + 0xe8);
                    }
                    iVar11 = FUN_10003870(param_1 + 0xd8);
                    if (iVar11 == 0) {
                      iStack_478 = 0;
                    }
                    else {
                      iStack_478 = FUN_100038b0(param_1 + 0xd8);
                    }
                    iVar11 = FUN_10003870(param_1 + 200);
                    if (iVar11 == 0) {
                      pwStack_47c = (wchar_t *)0x0;
                    }
                    else {
                      pwStack_47c = (wchar_t *)FUN_100038b0(param_1 + 200);
                    }
                    iVar11 = FUN_10003870(param_1 + 0xb8);
                    if (iVar11 == 0) {
                      iStack_494 = 0;
                    }
                    else {
                      iStack_494 = FUN_100038b0(param_1 + 0xb8);
                    }
                    iVar11 = FUN_10003870(param_1 + 0xa8);
                    if (iVar11 == 0) {
                      pWStack_484 = (LPCWSTR)0x0;
                    }
                    else {
                      pWStack_484 = (LPCWSTR)FUN_100038b0(param_1 + 0xa8);
                    }
                    iVar11 = FUN_10003870(param_1 + 0x48);
                    if (iVar11 == 0) {
                      iStack_48c = 0;
                    }
                    else {
                      iStack_48c = FUN_100038b0(param_1 + 0x48);
                    }
                    iVar11 = FUN_10003870(param_1 + 0x68);
                    if (iVar11 == 0) {
                      iStack_4b4 = 0;
                    }
                    else {
                      iStack_4b4 = FUN_100038b0(param_1 + 0x68);
                    }
                    iVar11 = FUN_10003870(param_1 + 0x58);
                    if (iVar11 == 0) {
                      iStack_498 = 0;
                    }
                    else {
                      iStack_498 = FUN_100038b0(param_1 + 0x58);
                    }
                    iVar11 = FUN_10003870(param_1 + 0x98);
                    if (iVar11 == 0) {
                      iVar11 = 0;
                    }
                    else {
                      iVar11 = FUN_100038b0(param_1 + 0x98);
                    }
                    iVar3 = FUN_10003870(param_1 + 0x38);
                    if (iVar3 == 0) {
                      iVar3 = 0;
                    }
                    else {
                      iVar3 = FUN_100038b0(param_1 + 0x38);
                    }
                    iVar9 = FUN_10003870(param_1 + 0x28);
                    if (iVar9 == 0) {
                      iVar9 = 0;
                    }
                    else {
                      iVar9 = FUN_100038b0(param_1 + 0x28);
                    }
                    piVar10 = FUN_1000e0e0(pvStack_49c,*(uint **)(param_1 + 0x150),
                                           *(int ***)(param_1 + 0x154),*(int *)(param_1 + 0x1a4),
                                           *(wchar_t **)(param_1 + 0x13c),*(uint *)(param_1 + 0x140)
                                           ,pOStack_4c0,iVar9,iVar3,iVar11,iStack_498,iStack_4b4,0,
                                           iStack_48c,0,0x1004cbf0,pWStack_484,0,1,
                                           (uStack_4bc != 0) + 1,0,0,(int)local_4c8,iStack_494,
                                           pwStack_47c,iStack_478,iStack_480,iStack_4a4,iStack_488,
                                           iStack_4ac,*(undefined4 *)(param_1 + 0x198),
                                           *(undefined4 *)(param_1 + 0x19c),
                                           *(uint *)(param_1 + 0x1a0),0,0,iStack_4a8,iStack_4a0,0,0)
                    ;
                    bstrString = pOStack_4b0;
                    puVar6 = puStack_4b8;
                    pOVar8 = pOStack_490;
                  }
                  iStack_4._0_1_ = 6;
                  if (piVar10 != (int *)0x0) {
                    iVar11 = piVar10[2];
                    if (iVar11 == 0) {
                      iVar11 = piVar10[3];
                    }
                    *(int *)(param_1 + 0x138) = iVar11;
                    if (piVar10[2] == 0) {
                      (**(code **)(*piVar10 + 0x18))(1);
                    }
                  }
                  puVar6 = (undefined4 *)puVar6[7];
                  iStack_4._0_1_ = 4;
                  puStack_4b8 = puVar6;
                  SysFreeString(pOVar8);
                  iStack_4._0_1_ = 3;
                  SysFreeString(bstrString);
                  iStack_4._0_1_ = 2;
                  SysFreeString(pOStack_4c0);
                  iStack_4._0_1_ = 1;
                  (*DAT_1005abec)(auStack_46c);
                  iStack_4 = (uint)iStack_4._1_3_ << 8;
                  (*DAT_1005abec)(auStack_45c);
                }
                iStack_4 = 0xffffffff;
                SysFreeString(local_4c8);
                _Dst = local_470;
              }
              (**(code **)(*local_4c4 + 0x10))();
            }
            operator_delete(_Dst);
            param_2 = extraout_EDX_07;
          }
          break;
        }
      }
      if (*(int *)(param_1 + 0x22c) + -5 <= iVar11) break;
      pvVar2 = memchr(pcVar12,0x3c,(*(int *)(param_1 + 0x22c) - iVar11) - 5);
      param_2 = extraout_EDX_01;
    }
  }
  ExceptionList = local_c;
  FUN_100447e0(local_10 ^ (uint)&local_4c8,param_2);
  return;
}


