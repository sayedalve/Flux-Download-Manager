// Function: FUN_1000b56a
// Entry Point: 0x1000b56a
// Program: idmcchandler7.dll


void FUN_1000b56a(void)

{
  char cVar1;
  ushort uVar2;
  short sVar3;
  int iVar4;
  undefined4 *puVar5;
  undefined2 *puVar6;
  bool bVar7;
  int iVar8;
  void *pvVar9;
  ushort *puVar10;
  ushort *puVar11;
  short *psVar12;
  undefined4 uVar13;
  undefined4 uVar14;
  char *pcVar15;
  char *pcVar16;
  char *pcVar17;
  char *pcVar18;
  undefined3 extraout_var;
  OLECHAR *pOVar19;
  DWORD DVar20;
  wchar_t *pwVar21;
  undefined2 *puVar22;
  undefined4 uVar23;
  undefined4 uVar24;
  undefined4 uVar25;
  undefined4 uVar26;
  undefined4 uVar27;
  undefined4 uVar28;
  undefined4 uVar29;
  undefined4 uVar30;
  BSTR pOVar31;
  LPCSTR pCVar32;
  int *piVar33;
  short *psVar34;
  int *piVar35;
  uint uVar36;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 extraout_EDX_01;
  int *piVar37;
  undefined4 extraout_EDX_02;
  undefined4 extraout_EDX_03;
  undefined4 extraout_EDX_04;
  undefined4 extraout_EDX_05;
  undefined4 extraout_EDX_06;
  undefined4 extraout_EDX_07;
  undefined4 extraout_EDX_08;
  undefined4 extraout_EDX_09;
  undefined4 extraout_EDX_10;
  undefined4 extraout_EDX_11;
  undefined4 extraout_EDX_12;
  undefined4 extraout_EDX_13;
  undefined4 extraout_EDX_14;
  undefined4 extraout_EDX_15;
  undefined4 extraout_EDX_16;
  undefined4 extraout_EDX_17;
  undefined4 extraout_EDX_18;
  undefined4 extraout_EDX_19;
  undefined4 extraout_EDX_20;
  undefined4 extraout_EDX_21;
  undefined4 extraout_EDX_22;
  undefined4 extraout_EDX_23;
  undefined4 extraout_EDX_24;
  undefined4 extraout_EDX_25;
  undefined4 extraout_EDX_26;
  undefined4 extraout_EDX_27;
  int unaff_EBX;
  uint unaff_EBP;
  HWND hWnd;
  code *pcVar38;
  char *pcVar39;
  int iVar40;
  undefined4 uStack0000000c;
  undefined4 uVar41;
  undefined4 uVar42;
  undefined4 uVar43;
  undefined *puVar44;
  undefined4 uVar45;
  undefined4 uVar46;
  undefined4 uVar47;
  undefined4 uVar48;
  undefined4 uVar49;
  undefined4 uVar50;
  UINT Msg;
  undefined4 uVar51;
  wchar_t *pwVar52;
  undefined4 uVar53;
  size_t sVar54;
  undefined4 uVar55;
  undefined4 uVar56;
  
  ReleaseMutex(DAT_1005ac5c);
  if (*(int *)(unaff_EBP - 0x120) == 0) {
    if (*(int *)(unaff_EBP - 0x34) != 0) {
      *(int *)(unaff_EBX + 0xc) = *(int *)(unaff_EBP - 0x34);
      if ((*(int *)(unaff_EBP - 0x4c) == 0) || (*(int *)(unaff_EBP - 0x44) != 0)) {
LAB_1000bb4b:
        pwVar52 = L"TS";
        pwVar21 = (wchar_t *)FUN_100038b0(unaff_EBX + 0xd8);
        iVar8 = _wcsicmp(pwVar21,pwVar52);
        if (iVar8 == 0) {
          (*DAT_1005abe4)();
          *(undefined1 *)(unaff_EBP - 4) = 0x49;
          (*DAT_1005ac0c)();
          piVar33 = (int *)FUN_10003910(unaff_EBP - 0x2dc);
          *(int **)(unaff_EBP - 0x228) = piVar33;
          pcVar16 = "hlstoidm://";
          piVar37 = piVar33;
          for (uVar36 = 0xb; 3 < uVar36; uVar36 = uVar36 - 4) {
            if (*piVar37 != *(int *)pcVar16) goto LAB_1000bc0e;
            pcVar16 = (char *)((int)pcVar16 + 4);
            piVar37 = piVar37 + 1;
          }
          if ((((char)*(int *)pcVar16 == (char)*piVar37) &&
              (*(char *)((int)pcVar16 + 1) == *(char *)((int)piVar37 + 1))) &&
             (*(char *)((int)pcVar16 + 2) == *(char *)((int)piVar37 + 2))) {
            piVar33 = (int *)((int)piVar33 + 0xb);
            *(int **)(unaff_EBP - 0x228) = piVar33;
            iVar8 = FUN_100025c0(&DAT_1005ac14,*(int *)(unaff_EBX + 0xc));
            if (iVar8 != 2) goto LAB_1000bc0e;
          }
          else {
LAB_1000bc0e:
            piVar35 = &DAT_1004ce1c;
            piVar37 = piVar33;
            for (uVar36 = 0xc; 3 < uVar36; uVar36 = uVar36 - 4) {
              if (*piVar37 != *piVar35) goto LAB_1000bc3f;
              piVar35 = piVar35 + 1;
              piVar37 = piVar37 + 1;
            }
            piVar33 = piVar33 + 3;
            *(int **)(unaff_EBP - 0x228) = piVar33;
LAB_1000bc3f:
            iVar8 = FUN_100026d0(&DAT_1005ac14,*(int *)(unaff_EBX + 0xc),(char *)piVar33,
                                 (undefined4 *)(unaff_EBP - 0x238));
            if ((iVar8 == 0) || (*(int *)(unaff_EBP - 0x238) != 0)) {
              *(undefined1 *)(unaff_EBP - 4) = 10;
              (*DAT_1005abec)();
              goto LAB_1000bc76;
            }
          }
          *(undefined1 *)(unaff_EBP - 4) = 10;
          (*DAT_1005abec)();
        }
        else {
LAB_1000bc76:
          iVar8 = unaff_EBX + 0x48;
          sVar54 = 0xb;
          pwVar52 = L"mpdtoidm://";
          pwVar21 = (wchar_t *)FUN_100038b0(iVar8);
          iVar40 = wcsncmp(pwVar21,pwVar52,sVar54);
          if (iVar40 == 0) {
LAB_1000bcc6:
            FUN_10002600(&DAT_1005ac14,*(int *)(unaff_EBX + 0xc));
          }
          else {
            sVar54 = 0xc;
            pwVar52 = L"mpdatoidm://";
            pwVar21 = (wchar_t *)FUN_100038b0(iVar8);
            iVar40 = wcsncmp(pwVar21,pwVar52,sVar54);
            if (iVar40 == 0) goto LAB_1000bcc6;
            iVar40 = FUN_10002580(&DAT_1005ac14,*(int *)(unaff_EBX + 0xc));
            if (iVar40 != 0) goto LAB_10009049;
          }
          if (DAT_1005ac34 == (code *)0x0) {
            if (DAT_1005ac30 == (code *)0x0) {
              uVar14 = FUN_100038b0(iVar8);
              *(undefined4 *)(unaff_EBP - 0x5bc) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x58);
              *(undefined4 *)(unaff_EBP - 0x5b8) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x68);
              *(undefined4 *)(unaff_EBP - 0x5b4) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x78);
              *(undefined4 *)(unaff_EBP - 0x5b0) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x88);
              *(undefined4 *)(unaff_EBP - 0x5ac) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x98);
              *(undefined4 *)(unaff_EBP - 0x5a8) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0xa8);
              *(undefined4 *)(unaff_EBP - 0x5a4) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0xb8);
              *(undefined4 *)(unaff_EBP - 0x5a0) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 200);
              *(undefined4 *)(unaff_EBP - 0x59c) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0xd8);
              *(undefined4 *)(unaff_EBP - 0x598) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0xe8);
              *(undefined4 *)(unaff_EBP - 0x594) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x17c);
              *(undefined4 *)(unaff_EBP - 0x590) = uVar14;
              *(undefined4 *)(unaff_EBP - 0x58c) = *(undefined4 *)(unaff_EBX + 0x178);
              *(undefined4 *)(unaff_EBP - 0x584) = *(undefined4 *)(unaff_EBX + 0x2c8);
              *(undefined4 *)(unaff_EBP - 0x580) = *(undefined4 *)(unaff_EBX + 0x2cc);
              *(int *)(unaff_EBP - 0x57c) = unaff_EBX + 0x1ac;
              iVar8 = unaff_EBP - 0x5bc;
              Msg = 0x40c;
            }
            else {
              uVar14 = FUN_100038b0(iVar8);
              *(undefined4 *)(unaff_EBP - 0x540) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x58);
              *(undefined4 *)(unaff_EBP - 0x53c) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x68);
              *(undefined4 *)(unaff_EBP - 0x538) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x78);
              *(undefined4 *)(unaff_EBP - 0x534) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x88);
              *(undefined4 *)(unaff_EBP - 0x530) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x98);
              *(undefined4 *)(unaff_EBP - 0x52c) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0xa8);
              *(undefined4 *)(unaff_EBP - 0x528) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0xb8);
              *(undefined4 *)(unaff_EBP - 0x524) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 200);
              *(undefined4 *)(unaff_EBP - 0x520) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0xd8);
              *(undefined4 *)(unaff_EBP - 0x51c) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0xe8);
              *(undefined4 *)(unaff_EBP - 0x518) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x17c);
              *(undefined4 *)(unaff_EBP - 0x514) = uVar14;
              *(undefined4 *)(unaff_EBP - 0x510) = *(undefined4 *)(unaff_EBX + 0x178);
              *(undefined4 *)(unaff_EBP - 0x508) = *(undefined4 *)(unaff_EBX + 0x2c8);
              *(undefined4 *)(unaff_EBP - 0x504) = *(undefined4 *)(unaff_EBX + 0x2cc);
              *(int *)(unaff_EBP - 0x500) = unaff_EBX + 0x1ac;
              uVar14 = FUN_100038b0(unaff_EBX + 0x168);
              *(undefined4 *)(unaff_EBP - 0x4fc) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0xf8);
              *(undefined4 *)(unaff_EBP - 0x4f8) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x108);
              *(undefined4 *)(unaff_EBP - 0x4f4) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x128);
              *(undefined4 *)(unaff_EBP - 0x4f0) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x118);
              *(undefined4 *)(unaff_EBP - 0x4ec) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x148);
              *(undefined4 *)(unaff_EBP - 0x4e8) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x158);
              *(undefined4 *)(unaff_EBP - 0x4e4) = uVar14;
              uVar14 = FUN_100038b0(unaff_EBX + 0x138);
              *(undefined4 *)(unaff_EBP - 0x4e0) = uVar14;
              *(undefined4 *)(unaff_EBP - 0x4dc) = 0;
              iVar8 = unaff_EBP - 0x540;
              Msg = 0x40d;
            }
            hWnd = *(HWND *)(unaff_EBP - 0x34);
            SendMessageA(hWnd,Msg,0,iVar8);
          }
          else {
            *(undefined4 *)(unaff_EBP - 500) = 0;
            uVar14 = FUN_10002680(&DAT_1005ac14,*(int *)(unaff_EBP - 0x34),
                                  (undefined4 *)(unaff_EBP - 500));
            *(undefined4 *)(unaff_EBP + 0xa0) = 0;
            *(undefined4 *)(unaff_EBP + 0xa8) = 0;
            *(undefined2 *)(unaff_EBP + 0xa4) = 0;
            *(undefined1 *)(unaff_EBP - 4) = 0x4a;
            if ((LPCSTR)(unaff_EBX + 0x1ac) != (LPCSTR)0x0) {
              FUN_10005490((void *)(unaff_EBP + 0xa0),(LPCSTR)(unaff_EBX + 0x1ac),0xfde9);
            }
            FUN_100038b0(unaff_EBX + 0xe8);
            FUN_100038b0(unaff_EBX + 0x138);
            FUN_100038b0(unaff_EBX + 0x158);
            FUN_100038b0(unaff_EBX + 0x148);
            FUN_100038b0(unaff_EBX + 0x118);
            FUN_100038b0(unaff_EBX + 0x128);
            FUN_100038b0(unaff_EBX + 0x108);
            FUN_100038b0(unaff_EBX + 0xf8);
            FUN_100038b0(unaff_EBX + 0x168);
            FUN_100038b0(unaff_EBX + 0x17c);
            FUN_100038b0(unaff_EBX + 0xe8);
            FUN_100038b0(unaff_EBX + 0xd8);
            FUN_100038b0(unaff_EBX + 200);
            FUN_100038b0(unaff_EBX + 0xb8);
            FUN_100038b0(unaff_EBX + 0xa8);
            FUN_100038b0(unaff_EBX + 0x98);
            uVar13 = FUN_100038b0(unaff_EBX + 0x88);
            uVar23 = FUN_100038b0(unaff_EBX + 0x78);
            uVar24 = FUN_100038b0(unaff_EBX + 0x68);
            uVar25 = FUN_100038b0(unaff_EBX + 0x58);
            uVar26 = FUN_100038b0(unaff_EBX + 0x48);
            (*DAT_1005ac34)(uVar14,0,uVar14,*(undefined4 *)(unaff_EBP - 500),0,0,0,0,uVar26,uVar25,
                            uVar24,uVar23,uVar13);
            *(undefined1 *)(unaff_EBP - 4) = 10;
            FUN_10013730((int *)(unaff_EBP + 0xa0));
            hWnd = *(HWND *)(unaff_EBP - 0x34);
          }
          if ((*(int *)(unaff_EBP - 0x30) == 0) || (*(int *)(unaff_EBP - 0x1b8) != 0)) {
            if (((*(int *)(unaff_EBX + 700) != 0) &&
                ((hWnd != (HWND)0x0 && (*(int *)(unaff_EBP - 0x30) == 0)))) &&
               ((*(int *)(unaff_EBP - 0x44) != 0 || (*(int *)(unaff_EBP - 0xb4) != 0)))) {
              PostMessageA(hWnd,0x40f,0,0);
            }
          }
          else {
            iVar8 = *(int *)(unaff_EBX + 0x2ec);
            if (iVar8 == 0) {
              iVar8 = unaff_EBP - 0x1d8;
            }
            *(int *)(unaff_EBP - 0xd0) = iVar8;
            for (iVar40 = 0; *(int *)(unaff_EBP - 0x7c) = iVar40, iVar40 < *(int *)(iVar8 + 0x10);
                iVar40 = iVar40 + 1) {
              *(undefined4 *)(unaff_EBP - 0xb0) = 0;
              *(undefined1 *)(unaff_EBP - 4) = 0x4b;
              (*DAT_1005abe4)();
              *(undefined1 *)(unaff_EBP - 4) = 0x4c;
              (*DAT_1005abe4)();
              *(undefined1 *)(unaff_EBP - 4) = 0x4d;
              uVar14 = FUN_10004600(0,*(int *)(*(int *)(*(int *)(iVar8 + 0x18) + iVar40 * 4) + 4),
                                    *(int *)(unaff_EBP - 0x88),*(int *)(unaff_EBP - 0x9c),
                                    (undefined4 *)(unaff_EBP - 0xb0),(void *)(unaff_EBP - 0x3c4),
                                    (void *)(unaff_EBP - 0x324));
              if ((DAT_1005ac6c == 0) && ((char)uVar14 == '\0')) {
                iVar4 = *(int *)(*(int *)(iVar8 + 0x18) + iVar40 * 4);
                if ((*(int *)(iVar4 + 0x10) == 0) && (*(int *)(iVar4 + 0xc) != 0)) {
                  FUN_10003910(unaff_EBP - 0x324);
                  FUN_10007d90(&DAT_1005acc0,extraout_EDX_04);
                  FUN_10007a90(&DAT_1005acc0,extraout_EDX_05);
                  FUN_10003910(unaff_EBP - 0x3c4);
                  _snprintf((char *)(unaff_EBX + 0x1ac),0x100,"%s %s, %s %s");
                  pOVar31 = SysAllocString(L"");
                  *(BSTR *)(unaff_EBP - 0x74) = pOVar31;
                  *(undefined1 *)(unaff_EBP - 4) = 0x4e;
                  psVar34 = (short *)FUN_100038b0(unaff_EBX + 0x168);
                  psVar12 = psVar34 + 1;
                  do {
                    sVar3 = *psVar34;
                    psVar34 = psVar34 + 1;
                  } while (sVar3 != 0);
                  pcVar38 = SysFreeString_exref;
                  if ((int)psVar34 - (int)psVar12 >> 1 != 0) {
                    FUN_10001a10((void *)(unaff_EBP - 0x74),L"pdmt=");
                    (*DAT_1005abc0)();
                    *(undefined1 *)(unaff_EBP - 4) = 0x4f;
                    *(undefined1 **)(unaff_EBP - 0x94) = &stack0xffffffec;
                    FUN_100055b0(&stack0xffffffec,unaff_EBX + 0x168);
                    *(undefined1 *)(unaff_EBP - 4) = 0x50;
                    *(undefined1 *)(unaff_EBP - 4) = 0x4f;
                    FUN_100059f0();
                    psVar34 = (short *)FUN_100038b0(unaff_EBP - 0x354);
                    psVar12 = psVar34 + 1;
                    do {
                      sVar3 = *psVar34;
                      psVar34 = psVar34 + 1;
                    } while (sVar3 != 0);
                    pcVar38 = SysFreeString_exref;
                    if ((int)psVar34 - (int)psVar12 >> 1 != 0) {
                      pOVar19 = (OLECHAR *)FUN_100038b0(unaff_EBP - 0x354);
                      FUN_10003b60((void *)(unaff_EBP - 0x1ec),pOVar19);
                      *(undefined1 *)(unaff_EBP - 4) = 0x51;
                      FUN_10005660((void *)(unaff_EBP - 0x74),(undefined4 *)(unaff_EBP - 0x1ec));
                      *(undefined1 *)(unaff_EBP - 4) = 0x4f;
                      pcVar38 = SysFreeString_exref;
                      SysFreeString(*(BSTR *)(unaff_EBP - 0x1ec));
                    }
                    pOVar31 = SysAllocString(L",");
                    *(BSTR *)(unaff_EBP - 0x22c) = pOVar31;
                    *(undefined1 *)(unaff_EBP - 4) = 0x52;
                    FUN_10005660((void *)(unaff_EBP - 0x74),(undefined4 *)(unaff_EBP - 0x22c));
                    *(undefined1 *)(unaff_EBP - 4) = 0x4f;
                    (*pcVar38)();
                    *(undefined1 *)(unaff_EBP - 4) = 0x4e;
                    (*DAT_1005abc8)();
                  }
                  pOVar31 = SysAllocString(L"au=");
                  *(BSTR *)(unaff_EBP - 0x240) = pOVar31;
                  *(undefined1 *)(unaff_EBP - 4) = 0x53;
                  FUN_10005660((void *)(unaff_EBP - 0x74),(undefined4 *)(unaff_EBP - 0x240));
                  *(undefined1 *)(unaff_EBP - 4) = 0x4e;
                  (*pcVar38)();
                  (*DAT_1005abc0)();
                  *(undefined1 *)(unaff_EBP - 4) = 0x54;
                  (*DAT_1005abc0)();
                  *(undefined1 *)(unaff_EBP - 4) = 0x55;
                  *(undefined4 *)(unaff_EBP - 300) = 0;
                  *(undefined1 *)(unaff_EBP - 4) = 0x56;
                  pvVar9 = *(void **)(unaff_EBP - 0xd0);
                  iVar8 = FUN_10041d40(pvVar9,*(int *)(*(int *)(*(int *)((int)pvVar9 + 0x18) +
                                                               *(int *)(unaff_EBP - 0x7c) * 4) + 4),
                                       (undefined4 *)(unaff_EBP - 0x298),
                                       (undefined4 *)(unaff_EBP - 300));
                  if (iVar8 != 0) {
                    *(undefined4 *)(unaff_EBP - 0x90) = 0;
                    *(undefined1 *)(unaff_EBP - 4) = 0x57;
                    iVar8 = FUN_100050c0(*(undefined4 *)
                                          (*(int *)(*(int *)((int)pvVar9 + 0x18) +
                                                   *(int *)(unaff_EBP - 0x7c) * 4) + 4));
                    if (iVar8 == 0) {
                      pwVar21 = L"ytatoidm://";
                    }
                    else {
                      FUN_10001a10((void *)(unaff_EBP - 0xb0),L"MKV");
                      iVar8 = FUN_100050c0(*(undefined4 *)(unaff_EBP - 0x298));
                      if (iVar8 == 0) {
                        pwVar21 = L"yta2toidm://";
                      }
                      else {
                        pwVar21 = L"yta3toidm://";
                      }
                    }
                    FUN_10001a10((void *)(unaff_EBP - 0x90),pwVar21);
                    FUN_10003b60((void *)(unaff_EBP - 0x178),
                                 (OLECHAR *)
                                 **(undefined4 **)
                                   (*(int *)((int)pvVar9 + 0x18) + *(int *)(unaff_EBP - 0x7c) * 4));
                    *(undefined1 *)(unaff_EBP - 4) = 0x58;
                    FUN_10005660((void *)(unaff_EBP - 0x90),(undefined4 *)(unaff_EBP - 0x178));
                    *(undefined1 *)(unaff_EBP - 4) = 0x57;
                    (*pcVar38)();
                    FUN_100038f0((void *)(unaff_EBP - 0x4d8),*(undefined4 *)(unaff_EBP - 300));
                    *(undefined1 **)(unaff_EBP - 0x94) = &stack0xffffffe4;
                    FUN_100055b0(&stack0xffffffe4,unaff_EBP - 0x4d8);
                    *(undefined1 *)(unaff_EBP - 4) = 0x59;
                    *(undefined1 *)(unaff_EBP - 4) = 0x57;
                    FUN_100059f0();
                    psVar34 = (short *)FUN_100038b0(unaff_EBP - 900);
                    psVar12 = psVar34 + 1;
                    do {
                      sVar3 = *psVar34;
                      psVar34 = psVar34 + 1;
                    } while (sVar3 != 0);
                    if ((int)psVar34 - (int)psVar12 >> 1 != 0) {
                      pOVar19 = (OLECHAR *)FUN_100038b0(unaff_EBP - 900);
                      FUN_10003b60((void *)(unaff_EBP - 0x20c),pOVar19);
                      *(undefined1 *)(unaff_EBP - 4) = 0x5a;
                      FUN_10005660((void *)(unaff_EBP - 0x74),(undefined4 *)(unaff_EBP - 0x20c));
                      *(undefined1 *)(unaff_EBP - 4) = 0x57;
                      (*pcVar38)();
                      uVar55 = 0;
                      uVar53 = 0;
                      uVar51 = 0;
                      uVar50 = 0;
                      uVar49 = 0;
                      uVar48 = 0;
                      uVar24 = FUN_100038b0(unaff_EBX + 0xf8);
                      uVar14 = *(undefined4 *)(unaff_EBP - 0x74);
                      pCVar32 = (LPCSTR)(unaff_EBX + 0x1ac);
                      uVar47 = 0;
                      uVar46 = 0;
                      uVar13 = *(undefined4 *)(unaff_EBX + 0x178);
                      uVar25 = FUN_100038b0(unaff_EBX + 0x17c);
                      uVar26 = FUN_100038b0(unaff_EBX + 0xe8);
                      uVar23 = *(undefined4 *)(unaff_EBP - 0xb0);
                      uVar45 = 0;
                      uVar27 = FUN_100038b0(unaff_EBX + 0xb8);
                      uVar43 = 0;
                      uVar28 = FUN_100038b0(unaff_EBX + 0x98);
                      uVar29 = FUN_100038b0(unaff_EBX + 0x88);
                      uVar42 = 0;
                      uVar30 = FUN_100038b0(unaff_EBX + 0x68);
                      uVar41 = FUN_100038b0(unaff_EBX + 0x58);
                      FUN_100085b0((uint)(DAT_1005ac30 != (code *)0x0),*(HWND *)(unaff_EBP - 0x34),
                                   *(undefined4 *)(unaff_EBP - 0x90),uVar41,uVar30,uVar42,uVar29,
                                   uVar28,uVar43,uVar27,uVar45,uVar23,uVar26,uVar25,uVar13,uVar46,
                                   uVar47,pCVar32,uVar14,uVar24,uVar48,uVar49,uVar50,uVar51,uVar53,
                                   uVar55);
                    }
                    *(undefined1 *)(unaff_EBP - 4) = 0x56;
                    (*pcVar38)();
                  }
                  *(undefined1 *)(unaff_EBP - 4) = 0x55;
                  (*pcVar38)();
                  *(undefined1 *)(unaff_EBP - 4) = 0x54;
                  (*DAT_1005abc8)();
                  *(undefined1 *)(unaff_EBP - 4) = 0x4e;
                  (*DAT_1005abc8)();
                  *(undefined1 *)(unaff_EBP - 4) = 0x4d;
                  (*pcVar38)();
LAB_1000c750:
                  iVar8 = *(int *)(unaff_EBP - 0xd0);
                  iVar40 = *(int *)(unaff_EBP - 0x7c);
                }
              }
              else if (*(int *)(*(int *)(*(int *)(iVar8 + 0x18) + iVar40 * 4) + 8) == 0) {
                pcVar16 = (char *)(unaff_EBX + 0x1ac);
                FUN_10003910(unaff_EBP - 0x324);
                FUN_10007d90(&DAT_1005acc0,extraout_EDX_02);
                FUN_10007a90(&DAT_1005acc0,extraout_EDX_03);
                FUN_10003910(unaff_EBP - 0x3c4);
                _snprintf(pcVar16,0x100,"%s %s, %s %s");
                uVar14 = **(undefined4 **)
                           (*(int *)(*(int *)(unaff_EBP - 0xd0) + 0x18) +
                           *(int *)(unaff_EBP - 0x7c) * 4);
                uVar56 = 0;
                uVar55 = 0;
                uVar53 = 0;
                uVar51 = 0;
                uVar50 = 0;
                uVar49 = 0;
                uVar24 = FUN_100038b0(unaff_EBX + 0xf8);
                uVar48 = 0;
                uVar47 = 0;
                uVar46 = 0;
                uVar13 = *(undefined4 *)(unaff_EBX + 0x178);
                uVar25 = FUN_100038b0(unaff_EBX + 0x17c);
                uVar26 = FUN_100038b0(unaff_EBX + 0xe8);
                uVar23 = *(undefined4 *)(unaff_EBP - 0xb0);
                uVar45 = 0;
                uVar27 = FUN_100038b0(unaff_EBX + 0xb8);
                uVar43 = 0;
                uVar28 = FUN_100038b0(unaff_EBX + 0x98);
                uVar29 = FUN_100038b0(unaff_EBX + 0x88);
                uVar42 = 0;
                uVar30 = FUN_100038b0(unaff_EBX + 0x68);
                uVar41 = FUN_100038b0(unaff_EBX + 0x58);
                FUN_100085b0((uint)(DAT_1005ac30 != (code *)0x0),*(HWND *)(unaff_EBP - 0x34),uVar14,
                             uVar41,uVar30,uVar42,uVar29,uVar28,uVar43,uVar27,uVar45,uVar23,uVar26,
                             uVar25,uVar13,uVar46,uVar47,pcVar16,uVar48,uVar24,uVar49,uVar50,uVar51,
                             uVar53,uVar55,uVar56);
                goto LAB_1000c750;
              }
              *(undefined1 *)(unaff_EBP - 4) = 0x4c;
              (*DAT_1005abec)();
              *(undefined1 *)(unaff_EBP - 4) = 0x4b;
              (*DAT_1005abec)();
              *(undefined1 *)(unaff_EBP - 4) = 10;
              SysFreeString(*(BSTR *)(unaff_EBP - 0xb0));
            }
          }
        }
      }
      else if (*(int *)(unaff_EBX + 0x2ec) == 0) {
        psVar12 = (short *)FUN_100038b0(unaff_EBX + 0x48);
        uVar14 = FUN_10042b20(*(void **)(unaff_EBP - 0x4c),psVar12);
        if ((char)uVar14 == '\0') {
          iVar8 = unaff_EBX + 0x108;
          iVar40 = FUN_100038b0(iVar8);
          if (iVar40 != 0) {
            psVar34 = (short *)FUN_100038b0(iVar8);
            psVar12 = psVar34 + 1;
            do {
              sVar3 = *psVar34;
              psVar34 = psVar34 + 1;
            } while (sVar3 != 0);
            if ((int)psVar34 - (int)psVar12 >> 1 != 0) {
              puVar11 = (ushort *)FUN_100038b0(unaff_EBX + 0x48);
              puVar10 = (ushort *)FUN_100038b0(iVar8);
              do {
                uVar2 = *puVar10;
                bVar7 = uVar2 < *puVar11;
                if (uVar2 != *puVar11) {
LAB_1000bb17:
                  iVar40 = (1 - (uint)bVar7) - (uint)(bVar7 != 0);
                  goto LAB_1000bb1c;
                }
                if (uVar2 == 0) break;
                uVar2 = puVar10[1];
                bVar7 = uVar2 < puVar11[1];
                if (uVar2 != puVar11[1]) goto LAB_1000bb17;
                puVar10 = puVar10 + 2;
                puVar11 = puVar11 + 2;
              } while (uVar2 != 0);
              iVar40 = 0;
LAB_1000bb1c:
              if (iVar40 != 0) {
                psVar12 = (short *)FUN_100038b0(iVar8);
                uVar14 = FUN_10042b20(*(void **)(unaff_EBP - 0x4c),psVar12);
                if ((char)uVar14 != '\0') goto LAB_10009049;
              }
            }
          }
          if ((*(int *)(unaff_EBP - 0x1c) < 0x79) || (DAT_1005ac6c != 0)) goto LAB_1000bb4b;
        }
      }
      else {
        *(undefined1 *)(unaff_EBP - 0x26) = 0;
        iVar8 = unaff_EBX + 0xe8;
        iVar40 = FUN_10003870(iVar8);
        if (iVar40 != 0) {
          iVar40 = FUN_10001a40((undefined4 *)(*(int *)(unaff_EBP - 0x4c) + 4));
          if (iVar40 != 0) {
            puVar10 = (ushort *)FUN_100038b0(iVar8);
            puVar11 = *(ushort **)(*(int *)(unaff_EBP - 0x4c) + 4);
            do {
              uVar2 = *puVar11;
              bVar7 = uVar2 < *puVar10;
              if (uVar2 != *puVar10) {
LAB_1000b6bb:
                iVar40 = (1 - (uint)bVar7) - (uint)(bVar7 != 0);
                goto LAB_1000b6c0;
              }
              if (uVar2 == 0) break;
              uVar2 = puVar11[1];
              bVar7 = uVar2 < puVar10[1];
              if (uVar2 != puVar10[1]) goto LAB_1000b6bb;
              puVar11 = puVar11 + 2;
              puVar10 = puVar10 + 2;
            } while (uVar2 != 0);
            iVar40 = 0;
LAB_1000b6c0:
            if (iVar40 == 0) goto LAB_1000b6db;
          }
          *(undefined1 *)(unaff_EBP - 0x26) = 1;
          pOVar19 = (OLECHAR *)FUN_100038b0(iVar8);
          FUN_10001a10((void *)(*(int *)(unaff_EBP - 0x4c) + 4),pOVar19);
        }
LAB_1000b6db:
        iVar8 = 0;
        while( true ) {
          *(int *)(unaff_EBP - 200) = iVar8;
          if (*(int *)(*(int *)(unaff_EBX + 0x2ec) + 0x10) <= iVar8) break;
          uVar36 = FUN_10041f40(*(void **)(unaff_EBP - 0x4c),
                                (ushort *)
                                **(undefined4 **)
                                  (*(int *)(*(int *)(unaff_EBX + 0x2ec) + 0x18) + iVar8 * 4),0,0,
                                (undefined4 *)(unaff_EBP - 0x94),(undefined4 *)(unaff_EBP - 0x564));
          if ((char)uVar36 == '\0') {
            puVar5 = *(undefined4 **)(*(int *)(*(int *)(unaff_EBX + 0x2ec) + 0x18) + iVar8 * 4);
            FUN_10002900(*(void **)(unaff_EBP - 0x4c),(OLECHAR *)*puVar5,puVar5[1],puVar5[3]);
            *(undefined4 *)(unaff_EBP - 0x124) = 0;
            *(undefined1 *)(unaff_EBP - 4) = 0x46;
            (*DAT_1005abe4)();
            *(undefined1 *)(unaff_EBP - 4) = 0x47;
            (*DAT_1005abe4)();
            *(undefined1 *)(unaff_EBP - 4) = 0x48;
            uVar14 = FUN_10004600(0,*(int *)(*(int *)(*(int *)(*(int *)(unaff_EBX + 0x2ec) + 0x18) +
                                                     iVar8 * 4) + 4),*(int *)(unaff_EBP - 0x88),
                                  *(int *)(unaff_EBP - 0x9c),(undefined4 *)(unaff_EBP - 0x124),
                                  (void *)(unaff_EBP - 0x574),(void *)(unaff_EBP - 0x560));
            if ((DAT_1005ac6c != 0) || ((char)uVar14 != '\0')) {
              pcVar16 = (char *)(unaff_EBX + 0x1ac);
              FUN_10003910(unaff_EBP - 0x560);
              FUN_10007d90(&DAT_1005acc0,extraout_EDX_00);
              FUN_10007a90(&DAT_1005acc0,extraout_EDX_01);
              FUN_10003910(unaff_EBP - 0x574);
              _snprintf(pcVar16,0x100,"%s %s, %s %s");
              uVar14 = **(undefined4 **)
                         (*(int *)(*(int *)(unaff_EBX + 0x2ec) + 0x18) +
                         *(int *)(unaff_EBP - 200) * 4);
              uVar56 = 0;
              uVar55 = 0;
              uVar53 = 0;
              uVar51 = 0;
              uVar50 = 0;
              uVar49 = 0;
              uVar24 = FUN_100038b0(unaff_EBX + 0xf8);
              uVar48 = 0;
              uVar47 = 0;
              uVar46 = 0;
              uVar13 = *(undefined4 *)(unaff_EBX + 0x178);
              uVar25 = FUN_100038b0(unaff_EBX + 0x17c);
              uVar26 = FUN_100038b0(unaff_EBX + 0xe8);
              uVar23 = *(undefined4 *)(unaff_EBP - 0x124);
              uVar45 = 0;
              uVar27 = FUN_100038b0(unaff_EBX + 0xb8);
              uVar43 = 0;
              uVar28 = FUN_100038b0(unaff_EBX + 0x98);
              uVar29 = FUN_100038b0(unaff_EBX + 0x88);
              uVar42 = 0;
              uVar30 = FUN_100038b0(unaff_EBX + 0x68);
              uVar41 = FUN_100038b0(unaff_EBX + 0x58);
              FUN_100085b0((uint)(DAT_1005ac30 != (code *)0x0),*(HWND *)(unaff_EBP - 0x34),uVar14,
                           uVar41,uVar30,uVar42,uVar29,uVar28,uVar43,uVar27,uVar45,uVar23,uVar26,
                           uVar25,uVar13,uVar46,uVar47,pcVar16,uVar48,uVar24,uVar49,uVar50,uVar51,
                           uVar53,uVar55,uVar56);
              iVar8 = *(int *)(unaff_EBP - 200);
            }
            *(undefined1 *)(unaff_EBP - 4) = 0x47;
            (*DAT_1005abec)();
            *(undefined1 *)(unaff_EBP - 4) = 0x46;
            (*DAT_1005abec)();
            *(undefined1 *)(unaff_EBP - 4) = 10;
            SysFreeString(*(BSTR *)(unaff_EBP - 0x124));
LAB_1000ba8a:
            iVar8 = iVar8 + 1;
          }
          else {
            if (*(char *)(unaff_EBP - 0x26) == '\0') goto LAB_1000ba8a;
            if (DAT_1005ac3c == (code *)0x0) {
              *(undefined4 *)(unaff_EBP - 0x5d4) =
                   **(undefined4 **)(*(int *)(*(int *)(unaff_EBX + 0x2ec) + 0x18) + iVar8 * 4);
              *(undefined4 *)(unaff_EBP - 0x5cc) = 0;
              *(undefined4 *)(unaff_EBP - 0x5c8) = 0;
              (*DAT_1005abe4)();
              *(undefined1 *)(unaff_EBP - 4) = 0x44;
              (*DAT_1005ac0c)();
              (*DAT_1005abc0)();
              *(undefined1 *)(unaff_EBP - 4) = 0x45;
              (*DAT_1005ac08)();
              iVar40 = FUN_10003870(unaff_EBP - 0x314);
              if (iVar40 != 0) {
                puVar11 = (ushort *)FUN_100038b0(unaff_EBX + 0xe8);
                puVar10 = (ushort *)FUN_100038b0(unaff_EBP - 0x314);
                do {
                  uVar2 = *puVar10;
                  bVar7 = uVar2 < *puVar11;
                  if (uVar2 != *puVar11) {
LAB_1000b856:
                    iVar40 = (1 - (uint)bVar7) - (uint)(bVar7 != 0);
                    goto LAB_1000b85b;
                  }
                  if (uVar2 == 0) break;
                  uVar2 = puVar10[1];
                  bVar7 = uVar2 < puVar11[1];
                  if (uVar2 != puVar11[1]) goto LAB_1000b856;
                  puVar10 = puVar10 + 2;
                  puVar11 = puVar11 + 2;
                } while (uVar2 != 0);
                iVar40 = 0;
LAB_1000b85b:
                if (iVar40 == 0) {
                  uVar14 = FUN_10003910(unaff_EBP - 0x334);
                  *(undefined4 *)(unaff_EBP - 0x5c4) = uVar14;
                  SendMessageA(*(HWND *)(unaff_EBP - 0x34),0x40e,0,unaff_EBP - 0x5d4);
                }
              }
              *(undefined1 *)(unaff_EBP - 4) = 0x44;
              (*DAT_1005abc8)();
              *(undefined1 *)(unaff_EBP - 4) = 10;
              (*DAT_1005abec)();
              iVar8 = iVar8 + 1;
            }
            else {
              *(undefined4 *)(unaff_EBP - 0x220) = 0;
              iVar40 = FUN_10002680(&DAT_1005ac14,*(int *)(unaff_EBP - 0x34),
                                    (undefined4 *)(unaff_EBP - 0x220));
              if (iVar40 == 0) goto LAB_1000ba8a;
              FUN_100038b0(unaff_EBX + 0xe8);
              (*DAT_1005ac3c)(iVar40);
              iVar8 = *(int *)(unaff_EBP - 200) + 1;
            }
          }
        }
      }
LAB_10009049:
      *(undefined1 *)(unaff_EBP - 4) = 8;
      FUN_10017f20((int *)(unaff_EBP - 0x14));
      *(undefined1 *)(unaff_EBP - 4) = 7;
      FUN_10007dc0((undefined4 *)(unaff_EBP - 0xac));
      *(undefined1 *)(unaff_EBP - 4) = 6;
      FUN_10003580((undefined4 *)(unaff_EBP - 0x1d8));
      *(undefined1 *)(unaff_EBP - 4) = 1;
      FUN_10017f20((int *)(unaff_EBP - 0x20));
      *(undefined1 *)(unaff_EBP - 4) = 0;
      FUN_10017f20((int *)(unaff_EBP - 0x18));
      uVar14 = extraout_EDX;
      goto LAB_10009056;
    }
  }
  else {
    if ((DAT_1005ac74 != 0) && (*(int *)(DAT_1005ac74 + 0xc) != 0)) {
      *(undefined4 *)(DAT_1005ac74 + 0xc) = 0;
    }
    if (*(HWND *)(unaff_EBP - 0x34) != (HWND)0x0) {
      SendMessageA(*(HWND *)(unaff_EBP - 0x34),0x10,0,0);
    }
  }
  if (*(int *)(unaff_EBP - 0x14) != 0) {
    FUN_10018fa0((undefined4 *)(unaff_EBP - 0xfc));
    *(undefined1 *)(unaff_EBP - 4) = 0x5b;
    piVar37 = *(int **)(unaff_EBX + 0x34);
    *(uint *)(unaff_EBP - 0x248) = unaff_EBP - 0xfc;
    *(undefined4 *)(unaff_EBP - 0x48) = *(undefined4 *)(unaff_EBP - 0x14);
    iVar8 = *piVar37;
    FUN_1001c6e0((undefined4 *)(unaff_EBP - 0x248));
    iVar8 = (**(code **)(iVar8 + 0x3c))();
    *(int *)(unaff_EBP - 0x2c) = iVar8;
    if ((-1 < iVar8) && (*(int *)(unaff_EBP - 0xfc) != 0)) {
      pvVar9 = operator_new(0xc);
      *(void **)(unaff_EBP - 100) = pvVar9;
      *(undefined1 *)(unaff_EBP - 4) = 0x5c;
      if (pvVar9 == (void *)0x0) {
        iVar8 = 0;
      }
      else {
        iVar8 = FUN_100057c0(pvVar9,unaff_EBX);
      }
      *(undefined1 *)(unaff_EBP - 4) = 0x5b;
      *(int *)(unaff_EBX + 0x2e8) = iVar8;
      if (iVar8 != 0) {
        FUN_10018fa0((undefined4 *)(unaff_EBP - 0x134));
        *(undefined1 *)(unaff_EBP - 4) = 0x5d;
        *(uint *)(unaff_EBP - 0x250) = unaff_EBP - 0x134;
        puVar5 = (undefined4 *)**(undefined4 **)(unaff_EBX + 0x2e8);
        FUN_1001c6e0((undefined4 *)(unaff_EBP - 0x250));
        (*(code *)*puVar5)();
        if (*(int *)(unaff_EBP - 0x134) != 0) {
          iVar8 = (**(code **)(**(int **)(unaff_EBX + 0x34) + 0x110))();
          *(int *)(unaff_EBP - 0x2c) = iVar8;
          if (-1 < iVar8) {
            FUN_100055d0((void *)(unaff_EBP - 0x614),L"unload",0xffffffff);
            *(undefined1 *)(unaff_EBP - 4) = 0x5e;
            iVar8 = (**(code **)(**(int **)(unaff_EBX + 0x34) + 0x118))();
            *(int *)(unaff_EBP - 0x2c) = iVar8;
            if (-1 < iVar8) {
              *(undefined1 *)(unaff_EBX + 0x38) = 1;
            }
            FUN_100055d0((void *)(unaff_EBP - 0x624),L"scroll",0xffffffff);
            *(undefined1 *)(unaff_EBP - 4) = 0x5f;
            uVar14 = (**(code **)(**(int **)(unaff_EBX + 0x34) + 0x118))();
            *(undefined4 *)(unaff_EBP - 0x2c) = uVar14;
            FUN_100055d0((void *)(unaff_EBP - 0x5f4),L"resize",0xffffffff);
            *(undefined1 *)(unaff_EBP - 4) = 0x60;
            uVar14 = (**(code **)(**(int **)(unaff_EBX + 0x34) + 0x118))
                               (*(int **)(unaff_EBX + 0x34),unaff_EBP - 0x5f4);
            *(undefined4 *)(unaff_EBP - 0x2c) = uVar14;
            *(undefined1 *)(unaff_EBP - 4) = 0x5f;
            (*DAT_1005abc8)(unaff_EBP - 0x5f4);
            *(undefined1 *)(unaff_EBP - 4) = 0x5e;
            (*DAT_1005abc8)(unaff_EBP - 0x624);
            *(undefined1 *)(unaff_EBP - 4) = 0x5d;
            (*DAT_1005abc8)(unaff_EBP - 0x614);
          }
        }
        *(undefined1 *)(unaff_EBP - 4) = 0x5b;
        FUN_10017f20((int *)(unaff_EBP - 0x134));
      }
    }
    *(undefined1 *)(unaff_EBP - 4) = 10;
    FUN_10017f20((int *)(unaff_EBP - 0xfc));
  }
  if (DAT_1005ac6c != 0) {
    *(undefined1 *)(unaff_EBX + 0x40) = 1;
  }
  FUN_100069d0(unaff_EBX);
  pcVar15 = (char *)FUN_10007b20(&DAT_1005acc0,extraout_EDX_06);
  iVar8 = FUN_10007b50(&DAT_1005acc0,extraout_EDX_07);
  *(int *)(unaff_EBP - 0x180) = iVar8;
  pcVar16 = pcVar15;
  do {
    cVar1 = *pcVar16;
    pcVar16 = pcVar16 + 1;
  } while (cVar1 != '\0');
  pcVar17 = *(char **)(unaff_EBP - 0x68);
  pcVar39 = pcVar17 + 1;
  do {
    cVar1 = *pcVar17;
    pcVar17 = pcVar17 + 1;
  } while (cVar1 != '\0');
  pcVar18 = operator_new((uint)(pcVar16 + (int)(pcVar17 + ((3 - (int)pcVar39) - (int)(pcVar15 + 1)))
                               ));
  *(char **)(unaff_EBP - 0x108) = pcVar18;
  if (pcVar18 != (char *)0x0) {
    _snprintf(pcVar18,(size_t)(pcVar16 + (int)(pcVar17 + ((3 - (int)pcVar39) - (int)(pcVar15 + 1))))
              ,pcVar15);
  }
  *(undefined4 *)(unaff_EBP - 0x16c) = 0;
  pcVar15 = *(char **)(unaff_EBP - 0x180);
  pcVar16 = pcVar15 + 1;
  do {
    cVar1 = *pcVar15;
    pcVar15 = pcVar15 + 1;
  } while (cVar1 != '\0');
  pcVar17 = *(char **)(unaff_EBP - 0x68);
  pcVar39 = pcVar17 + 1;
  do {
    cVar1 = *pcVar17;
    pcVar17 = pcVar17 + 1;
  } while (cVar1 != '\0');
  pcVar18 = operator_new((uint)(pcVar15 + (int)(pcVar17 + ((3 - (int)pcVar39) - (int)pcVar16))));
  *(char **)(unaff_EBP - 0x114) = pcVar18;
  if (pcVar18 != (char *)0x0) {
    _snprintf(pcVar18,(size_t)(pcVar15 + (int)(pcVar17 + ((3 - (int)pcVar39) - (int)pcVar16))),
              *(char **)(unaff_EBP - 0x180));
  }
  bVar7 = FUN_100162f0();
  *(bool *)(unaff_EBP - 0x3d) = CONCAT31(extraout_var,bVar7) != 0;
  if (DAT_1005ac34 == (code *)0x0) {
    if (DAT_1005ac30 == (code *)0x0) {
      FUN_100038b0(unaff_EBX + 0x17c);
      FUN_100038b0(unaff_EBX + 0xe8);
      FUN_100038b0(unaff_EBX + 0xd8);
      FUN_100038b0(unaff_EBX + 200);
      FUN_100038b0(unaff_EBX + 0xb8);
      FUN_100038b0(unaff_EBX + 0xa8);
      FUN_100038b0(unaff_EBX + 0x98);
      FUN_100038b0(unaff_EBX + 0x88);
      FUN_100038b0(unaff_EBX + 0x78);
      FUN_100038b0(unaff_EBX + 0x68);
      FUN_100038b0(unaff_EBX + 0x58);
      FUN_100038b0(unaff_EBX + 0x48);
      FUN_10007c10(&DAT_1005acc0,extraout_EDX_14);
      FUN_10007be0(&DAT_1005acc0,extraout_EDX_15);
      FUN_10007bb0(&DAT_1005acc0,extraout_EDX_16);
      FUN_10007b80(&DAT_1005acc0,extraout_EDX_17);
      FUN_10007a60(&DAT_1005acc0,extraout_EDX_18);
      FUN_10007a30(&DAT_1005acc0,extraout_EDX_19);
      uVar14 = (*DAT_1005ac2c)(*(undefined4 *)(unaff_EBX + 4),*(undefined4 *)(unaff_EBX + 0x10),
                               *(undefined4 *)(unaff_EBX + 0x14),*(undefined4 *)(unaff_EBX + 0x18),
                               *(undefined4 *)(unaff_EBX + 0x1c),*(undefined4 *)(unaff_EBX + 0x20),
                               *(undefined4 *)(unaff_EBX + 0x24),*(undefined4 *)(unaff_EBX + 0x28),
                               *(undefined4 *)(unaff_EBX + 0x2c));
    }
    else {
      FUN_100038b0(unaff_EBX + 0x138);
      FUN_100038b0(unaff_EBX + 0x158);
      FUN_100038b0(unaff_EBX + 0x148);
      FUN_100038b0(unaff_EBX + 0x118);
      FUN_100038b0(unaff_EBX + 0x128);
      FUN_100038b0(unaff_EBX + 0x108);
      FUN_100038b0(unaff_EBX + 0xf8);
      FUN_100038b0(unaff_EBX + 0x168);
      FUN_100038b0(unaff_EBX + 0x17c);
      FUN_100038b0(unaff_EBX + 0xe8);
      FUN_100038b0(unaff_EBX + 0xd8);
      FUN_100038b0(unaff_EBX + 200);
      FUN_100038b0(unaff_EBX + 0xb8);
      FUN_100038b0(unaff_EBX + 0xa8);
      FUN_100038b0(unaff_EBX + 0x98);
      FUN_100038b0(unaff_EBX + 0x88);
      FUN_100038b0(unaff_EBX + 0x78);
      FUN_100038b0(unaff_EBX + 0x68);
      FUN_100038b0(unaff_EBX + 0x58);
      FUN_100038b0(unaff_EBX + 0x48);
      FUN_10007c10(&DAT_1005acc0,extraout_EDX_08);
      FUN_10007be0(&DAT_1005acc0,extraout_EDX_09);
      FUN_10007bb0(&DAT_1005acc0,extraout_EDX_10);
      FUN_10007b80(&DAT_1005acc0,extraout_EDX_11);
      FUN_10007a60(&DAT_1005acc0,extraout_EDX_12);
      FUN_10007a30(&DAT_1005acc0,extraout_EDX_13);
      uVar14 = (*DAT_1005ac30)(*(undefined4 *)(unaff_EBX + 4),*(undefined4 *)(unaff_EBX + 0x10),
                               *(undefined4 *)(unaff_EBX + 0x14),*(undefined4 *)(unaff_EBX + 0x18),
                               *(undefined4 *)(unaff_EBX + 0x1c),*(undefined4 *)(unaff_EBX + 0x20),
                               *(undefined4 *)(unaff_EBX + 0x24),*(undefined4 *)(unaff_EBX + 0x28),
                               *(undefined4 *)(unaff_EBX + 0x2c));
    }
    *(undefined4 *)(unaff_EBX + 8) = uVar14;
  }
  else {
    *(undefined4 *)(unaff_EBP + 0x94) = 0;
    *(undefined4 *)(unaff_EBP + 0x9c) = 0;
    *(undefined2 *)(unaff_EBP + 0x98) = 0;
    *(undefined1 *)(unaff_EBP - 4) = 0x61;
    if ((LPCSTR)(unaff_EBX + 0x1ac) != (LPCSTR)0x0) {
      FUN_10005490((void *)(unaff_EBP + 0x94),(LPCSTR)(unaff_EBX + 0x1ac),0xfde9);
    }
    FUN_100038b0(unaff_EBX + 0xe8);
    FUN_100038b0(unaff_EBX + 0x138);
    FUN_100038b0(unaff_EBX + 0x158);
    FUN_100038b0(unaff_EBX + 0x148);
    FUN_100038b0(unaff_EBX + 0x118);
    FUN_100038b0(unaff_EBX + 0x128);
    FUN_100038b0(unaff_EBX + 0x108);
    FUN_100038b0(unaff_EBX + 0xf8);
    FUN_100038b0(unaff_EBX + 0x168);
    FUN_100038b0(unaff_EBX + 0x17c);
    FUN_100038b0(unaff_EBX + 0xe8);
    FUN_100038b0(unaff_EBX + 0xd8);
    FUN_100038b0(unaff_EBX + 200);
    FUN_100038b0(unaff_EBX + 0xb8);
    FUN_100038b0(unaff_EBX + 0xa8);
    FUN_100038b0(unaff_EBX + 0x98);
    FUN_100038b0(unaff_EBX + 0x88);
    FUN_100038b0(unaff_EBX + 0x78);
    FUN_100038b0(unaff_EBX + 0x68);
    FUN_100038b0(unaff_EBX + 0x58);
    FUN_100038b0(unaff_EBX + 0x48);
    uVar14 = (*DAT_1005ac34)();
    *(undefined4 *)(unaff_EBX + 8) = uVar14;
    *(undefined1 *)(unaff_EBP - 4) = 10;
    FUN_10013730((int *)(unaff_EBP + 0x94));
  }
  if (*(int *)(unaff_EBX + 8) != 0) {
    *(undefined1 *)(unaff_EBX + 0x3f) = 1;
    if (*(char *)(unaff_EBX + 0x3d) != '\0') {
      FUN_1001e9e0();
    }
    if ((*(int *)(unaff_EBX + 0x2ec) != 0) && (iVar8 = FUN_10003870(unaff_EBX + 0xe8), iVar8 != 0))
    {
      pOVar19 = (OLECHAR *)FUN_100038b0(unaff_EBX + 0xe8);
      FUN_10001a10((void *)(*(int *)(unaff_EBX + 0x2ec) + 4),pOVar19);
    }
    if ((*(int *)(unaff_EBP - 0x44) != 0) && (*(int *)(unaff_EBP - 0x1a0) != 0)) {
      *(undefined4 *)(unaff_EBX + 0x2d0) = 1;
    }
    if ((DAT_1005ac5c != (HANDLE)0x0) &&
       (DVar20 = WaitForSingleObject(DAT_1005ac5c,1000), DVar20 == 0)) {
      *(undefined1 *)(unaff_EBP - 4) = 0x62;
      FUN_10002cc0(&DAT_1005ac14,unaff_EBX,*(undefined4 *)(unaff_EBX + 4),
                   *(undefined4 *)(unaff_EBP - 0x20),*(undefined4 *)(unaff_EBX + 8),
                   *(undefined4 *)(unaff_EBX + 0x2e0),*(undefined4 *)(unaff_EBX + 0x2e4),
                   *(undefined4 *)(unaff_EBX + 0x2dc),*(undefined4 *)(unaff_EBP - 0x30),
                   *(int *)(unaff_EBX + 0x2ec));
      sVar54 = 0xb;
      pwVar52 = L"mpdtoidm://";
      pwVar21 = (wchar_t *)FUN_100038b0(unaff_EBX + 0x48);
      iVar8 = wcsncmp(pwVar21,pwVar52,sVar54);
      if (iVar8 != 0) {
        sVar54 = 0xc;
        pwVar52 = L"mpdatoidm://";
        pwVar21 = (wchar_t *)FUN_100038b0(unaff_EBX + 0x48);
        iVar8 = wcsncmp(pwVar21,pwVar52,sVar54);
        if (iVar8 != 0) goto code_r0x1000cfae;
      }
      FUN_10002600(&DAT_1005ac14,*(int *)(unaff_EBX + 8));
code_r0x1000cfae:
      *(undefined4 *)(unaff_EBP - 4) = 10;
      FUN_1000cfca();
      return;
    }
  }
  if (((*(int *)(unaff_EBP - 0x104) != 0) && (*(int *)(unaff_EBX + 8) != 0)) &&
     (1 < *(int *)(unaff_EBP - 0xa8))) {
    *(undefined4 *)(unaff_EBP - 0x70) = 1;
    while (*(int *)(unaff_EBP - 0x70) < *(int *)(unaff_EBP - 0xa8)) {
      FUN_10007a90(&DAT_1005acc0,*(int *)(unaff_EBP - 0xa8));
      _snprintf((char *)(unaff_EBX + 0x1ac),0x100,"MP4 %s, ");
      puVar6 = (undefined2 *)(unaff_EBX + 0x1ab);
      do {
        puVar22 = puVar6;
        puVar6 = (undefined2 *)((int)puVar22 + 1);
      } while (*(char *)((int)puVar22 + 1) != '\0');
      *(undefined2 *)((int)puVar22 + 1) = DAT_1004ceb0;
      uVar14 = CONCAT31((int3)((uint)extraout_EDX_20 >> 8),DAT_1004ceb2);
      *(undefined1 *)((int)puVar22 + 3) = DAT_1004ceb2;
      pcVar15 = (char *)FUN_10007d90(&DAT_1005acc0,uVar14);
      pcVar16 = pcVar15;
      do {
        cVar1 = *pcVar16;
        pcVar16 = pcVar16 + 1;
      } while (cVar1 != '\0');
      pcVar39 = (char *)(unaff_EBX + 0x1ab);
      do {
        pcVar17 = pcVar39 + 1;
        pcVar39 = pcVar39 + 1;
      } while (*pcVar17 != '\0');
      pcVar17 = pcVar15;
      for (uVar36 = (uint)((int)pcVar16 - (int)pcVar15) >> 2; uVar36 != 0; uVar36 = uVar36 - 1) {
        *(undefined4 *)pcVar39 = *(undefined4 *)pcVar17;
        pcVar17 = pcVar17 + 4;
        pcVar39 = pcVar39 + 4;
      }
      for (uVar36 = (int)pcVar16 - (int)pcVar15 & 3; uVar36 != 0; uVar36 = uVar36 - 1) {
        *pcVar39 = *pcVar17;
        pcVar17 = pcVar17 + 1;
        pcVar39 = pcVar39 + 1;
      }
      iVar8 = *(int *)(unaff_EBP - 0xa0);
      pcVar15 = " HD";
      pcVar16 = pcVar15;
      if (*(int *)(*(int *)(iVar8 + *(int *)(unaff_EBP - 0x70) * 4) + 0x10) == 0) {
        pcVar15 = " SD";
        pcVar16 = pcVar15;
      }
      do {
        cVar1 = *pcVar15;
        pcVar15 = pcVar15 + 1;
      } while (cVar1 != '\0');
      pcVar39 = (char *)(unaff_EBX + 0x1ab);
      do {
        pcVar17 = pcVar39 + 1;
        pcVar39 = pcVar39 + 1;
      } while (*pcVar17 != '\0');
      pcVar17 = pcVar16;
      for (uVar36 = (uint)((int)pcVar15 - (int)pcVar16) >> 2; uVar36 != 0; uVar36 = uVar36 - 1) {
        *(undefined4 *)pcVar39 = *(undefined4 *)pcVar17;
        pcVar17 = pcVar17 + 4;
        pcVar39 = pcVar39 + 4;
      }
      for (uVar36 = (int)pcVar15 - (int)pcVar16 & 3; uVar36 != 0; uVar36 = uVar36 - 1) {
        *pcVar39 = *pcVar17;
        pcVar17 = pcVar17 + 1;
        pcVar39 = pcVar39 + 1;
      }
      iVar40 = *(int *)(iVar8 + *(int *)(unaff_EBP - 0x70) * 4);
      if (*(int *)(iVar40 + 8) != 0 || *(int *)(iVar40 + 0xc) != 0) {
        iVar8 = *(int *)(iVar8 + *(int *)(unaff_EBP - 0x70) * 4);
        FUN_100084a0(*(uint *)(iVar8 + 8),*(uint *)(iVar8 + 0xc),(char *)(unaff_EBP + 0x6c4),0x40);
        puVar6 = (undefined2 *)(unaff_EBX + 0x1ab);
        do {
          puVar22 = puVar6;
          puVar6 = (undefined2 *)((int)puVar22 + 1);
        } while (*(char *)((int)puVar22 + 1) != '\0');
        *(undefined2 *)((int)puVar22 + 1) = DAT_1004ceb0;
        *(undefined1 *)((int)puVar22 + 3) = DAT_1004ceb2;
        pcVar16 = (char *)(unaff_EBP + 0x6c4);
        pcVar15 = pcVar16;
        do {
          cVar1 = *pcVar15;
          pcVar15 = pcVar15 + 1;
        } while (cVar1 != '\0');
        pcVar39 = (char *)(unaff_EBX + 0x1ab);
        do {
          pcVar17 = pcVar39 + 1;
          pcVar39 = pcVar39 + 1;
        } while (*pcVar17 != '\0');
        pcVar17 = pcVar16;
        for (uVar36 = (uint)((int)pcVar15 - (int)pcVar16) >> 2; uVar36 != 0; uVar36 = uVar36 - 1) {
          *(undefined4 *)pcVar39 = *(undefined4 *)pcVar17;
          pcVar17 = pcVar17 + 4;
          pcVar39 = pcVar39 + 4;
        }
        for (uVar36 = (int)pcVar15 - (int)pcVar16 & 3; uVar36 != 0; uVar36 = uVar36 - 1) {
          *pcVar39 = *pcVar17;
          pcVar17 = pcVar17 + 1;
          pcVar39 = pcVar39 + 1;
        }
        iVar8 = *(int *)(unaff_EBP - 0xa0);
      }
      uVar14 = **(undefined4 **)(iVar8 + *(int *)(unaff_EBP - 0x70) * 4);
      uVar55 = 0;
      uVar53 = 0;
      uVar51 = 0;
      uVar50 = 0;
      uVar49 = 0;
      uVar48 = 0;
      uVar23 = FUN_100038b0(unaff_EBX + 0xf8);
      uVar47 = 0;
      pCVar32 = (LPCSTR)(unaff_EBX + 0x1ac);
      uVar46 = 0;
      uVar45 = 0;
      uVar13 = *(undefined4 *)(unaff_EBX + 0x178);
      uVar24 = FUN_100038b0(unaff_EBX + 0x17c);
      uVar25 = FUN_100038b0(unaff_EBX + 0xe8);
      puVar44 = &DAT_1004d1a4;
      uVar43 = 0;
      uVar26 = FUN_100038b0(unaff_EBX + 0xb8);
      uVar42 = 0;
      uVar27 = FUN_100038b0(unaff_EBX + 0x98);
      uVar28 = FUN_100038b0(unaff_EBX + 0x88);
      uVar41 = 0;
      uVar29 = FUN_100038b0(unaff_EBX + 0x68);
      uVar30 = FUN_100038b0(unaff_EBX + 0x58);
      FUN_100085b0((uint)(DAT_1005ac30 != (code *)0x0),*(HWND *)(unaff_EBX + 8),uVar14,uVar30,uVar29
                   ,uVar41,uVar28,uVar27,uVar42,uVar26,uVar43,puVar44,uVar25,uVar24,uVar13,uVar45,
                   uVar46,pCVar32,uVar47,uVar23,uVar48,uVar49,uVar50,uVar51,uVar53,uVar55);
      *(int *)(unaff_EBP - 0x70) = *(int *)(unaff_EBP - 0x70) + 1;
    }
  }
  if ((*(int *)(unaff_EBP - 0x30) == 0) || (*(int *)(unaff_EBX + 8) == 0)) {
    if (((*(int *)(unaff_EBX + 700) != 0) && (*(HWND *)(unaff_EBX + 8) != (HWND)0x0)) &&
       ((*(int *)(unaff_EBP - 0x44) != 0 || (*(int *)(unaff_EBP - 0xb4) != 0)))) {
      PostMessageA(*(HWND *)(unaff_EBX + 8),0x40f,0,0);
    }
  }
  else if (*(int *)(unaff_EBX + 0x2f0) == 0) {
    iVar8 = *(int *)(unaff_EBX + 0x2ec);
    if (iVar8 == 0) {
      iVar8 = unaff_EBP - 0x1d8;
      *(int *)(unaff_EBP - 0xcc) = iVar8;
    }
    else {
      *(int *)(unaff_EBP - 0xcc) = iVar8;
    }
    for (iVar40 = 0; *(int *)(unaff_EBP - 0x6c) = iVar40, iVar40 < *(int *)(iVar8 + 0x10);
        iVar40 = iVar40 + 1) {
      *(undefined4 *)(unaff_EBP - 0xbc) = 0;
      *(undefined1 *)(unaff_EBP - 4) = 0x7b;
      (*DAT_1005abe4)();
      *(undefined1 *)(unaff_EBP - 4) = 0x7c;
      (*DAT_1005abe4)();
      *(undefined1 *)(unaff_EBP - 4) = 0x7d;
      uVar14 = FUN_10004600(0,*(int *)(*(int *)(*(int *)(iVar8 + 0x18) + iVar40 * 4) + 4),
                            *(int *)(unaff_EBP - 0x88),*(int *)(unaff_EBP - 0x9c),
                            (undefined4 *)(unaff_EBP - 0xbc),(void *)(unaff_EBP - 0x3e4),
                            (void *)(unaff_EBP - 0x2f4));
      pcVar38 = SysFreeString_exref;
      if ((DAT_1005ac6c == 0) && ((char)uVar14 == '\0')) {
        iVar8 = *(int *)(*(int *)(iVar8 + 0x18) + iVar40 * 4);
        if ((*(int *)(iVar8 + 0x10) == 0) && (*(int *)(iVar8 + 0xc) != 0)) {
          FUN_10003910(unaff_EBP - 0x2f4);
          FUN_10007d90(&DAT_1005acc0,extraout_EDX_25);
          FUN_10007a90(&DAT_1005acc0,extraout_EDX_26);
          FUN_10003910(unaff_EBP - 0x3e4);
          _snprintf((char *)(unaff_EBX + 0x1ac),0x100,"%s %s, %s %s");
          pOVar31 = SysAllocString(L"");
          *(BSTR *)(unaff_EBP - 0x78) = pOVar31;
          *(undefined1 *)(unaff_EBP - 4) = 0x7e;
          psVar34 = (short *)FUN_100038b0(unaff_EBX + 0x168);
          psVar12 = psVar34 + 1;
          do {
            sVar3 = *psVar34;
            psVar34 = psVar34 + 1;
          } while (sVar3 != 0);
          pcVar38 = SysFreeString_exref;
          if ((int)psVar34 - (int)psVar12 >> 1 != 0) {
            FUN_10001a10((void *)(unaff_EBP - 0x78),L"pdmt=");
            (*DAT_1005abc0)();
            *(undefined1 *)(unaff_EBP - 4) = 0x7f;
            *(undefined1 **)(unaff_EBP - 100) = &stack0xffffffec;
            FUN_100055b0(&stack0xffffffec,unaff_EBX + 0x168);
            *(undefined1 *)(unaff_EBP - 4) = 0x80;
            *(undefined1 *)(unaff_EBP - 4) = 0x7f;
            FUN_100059f0();
            psVar34 = (short *)FUN_100038b0(unaff_EBP - 0x344);
            psVar12 = psVar34 + 1;
            do {
              sVar3 = *psVar34;
              psVar34 = psVar34 + 1;
            } while (sVar3 != 0);
            pcVar38 = SysFreeString_exref;
            if ((int)psVar34 - (int)psVar12 >> 1 != 0) {
              pOVar19 = (OLECHAR *)FUN_100038b0(unaff_EBP - 0x344);
              FUN_10003b60((void *)(unaff_EBP - 0x1a8),pOVar19);
              *(undefined1 *)(unaff_EBP - 4) = 0x81;
              FUN_10005660((void *)(unaff_EBP - 0x78),(undefined4 *)(unaff_EBP - 0x1a8));
              *(undefined1 *)(unaff_EBP - 4) = 0x7f;
              pcVar38 = SysFreeString_exref;
              SysFreeString(*(BSTR *)(unaff_EBP - 0x1a8));
            }
            pOVar31 = SysAllocString(L",");
            *(BSTR *)(unaff_EBP - 600) = pOVar31;
            *(undefined1 *)(unaff_EBP - 4) = 0x82;
            FUN_10005660((void *)(unaff_EBP - 0x78),(undefined4 *)(unaff_EBP - 600));
            *(undefined1 *)(unaff_EBP - 4) = 0x7f;
            (*pcVar38)();
            *(undefined1 *)(unaff_EBP - 4) = 0x7e;
            (*DAT_1005abc8)();
          }
          pOVar31 = SysAllocString(L"au=");
          *(BSTR *)(unaff_EBP - 0x288) = pOVar31;
          *(undefined1 *)(unaff_EBP - 4) = 0x83;
          FUN_10005660((void *)(unaff_EBP - 0x78),(undefined4 *)(unaff_EBP - 0x288));
          *(undefined1 *)(unaff_EBP - 4) = 0x7e;
          (*pcVar38)();
          (*DAT_1005abc0)();
          *(undefined1 *)(unaff_EBP - 4) = 0x84;
          (*DAT_1005abc0)();
          *(undefined1 *)(unaff_EBP - 4) = 0x85;
          *(undefined4 *)(unaff_EBP - 0x11c) = 0;
          *(undefined1 *)(unaff_EBP - 4) = 0x86;
          pvVar9 = *(void **)(unaff_EBP - 0xcc);
          iVar8 = FUN_10041d40(pvVar9,*(int *)(*(int *)(*(int *)((int)pvVar9 + 0x18) +
                                                       *(int *)(unaff_EBP - 0x6c) * 4) + 4),
                               (undefined4 *)(unaff_EBP - 0x260),(undefined4 *)(unaff_EBP - 0x11c));
          if (iVar8 != 0) {
            *(undefined4 *)(unaff_EBP - 0x8c) = 0;
            *(undefined1 *)(unaff_EBP - 4) = 0x87;
            iVar8 = FUN_100050c0(*(undefined4 *)
                                  (*(int *)(*(int *)((int)pvVar9 + 0x18) +
                                           *(int *)(unaff_EBP - 0x6c) * 4) + 4));
            if (iVar8 == 0) {
              pwVar21 = L"ytatoidm://";
            }
            else {
              FUN_10001a10((void *)(unaff_EBP - 0xbc),L"MKV");
              iVar8 = FUN_100050c0(*(undefined4 *)(unaff_EBP - 0x260));
              if (iVar8 == 0) {
                pwVar21 = L"yta2toidm://";
              }
              else {
                pwVar21 = L"yta3toidm://";
              }
            }
            FUN_10001a10((void *)(unaff_EBP - 0x8c),pwVar21);
            FUN_10003b60((void *)(unaff_EBP - 0x21c),
                         (OLECHAR *)
                         **(undefined4 **)
                           (*(int *)((int)pvVar9 + 0x18) + *(int *)(unaff_EBP - 0x6c) * 4));
            *(undefined1 *)(unaff_EBP - 4) = 0x88;
            FUN_10005660((void *)(unaff_EBP - 0x8c),(undefined4 *)(unaff_EBP - 0x21c));
            *(undefined1 *)(unaff_EBP - 4) = 0x87;
            (*pcVar38)();
            FUN_100038f0((void *)(unaff_EBP - 0x4c8),*(undefined4 *)(unaff_EBP - 0x11c));
            *(undefined1 **)(unaff_EBP - 100) = &stack0xffffffe4;
            FUN_100055b0(&stack0xffffffe4,unaff_EBP - 0x4c8);
            *(undefined1 *)(unaff_EBP - 4) = 0x89;
            *(undefined1 *)(unaff_EBP - 4) = 0x87;
            FUN_100059f0();
            psVar34 = (short *)FUN_100038b0(unaff_EBP - 0x3a4);
            psVar12 = psVar34 + 1;
            do {
              sVar3 = *psVar34;
              psVar34 = psVar34 + 1;
            } while (sVar3 != 0);
            if ((int)psVar34 - (int)psVar12 >> 1 != 0) {
              pOVar19 = (OLECHAR *)FUN_100038b0(unaff_EBP - 0x3a4);
              FUN_10003b60((void *)(unaff_EBP - 0x1b0),pOVar19);
              *(undefined1 *)(unaff_EBP - 4) = 0x8a;
              FUN_10005660((void *)(unaff_EBP - 0x78),(undefined4 *)(unaff_EBP - 0x1b0));
              *(undefined1 *)(unaff_EBP - 4) = 0x87;
              (*pcVar38)();
              uVar55 = 0;
              uVar53 = 0;
              uVar51 = 0;
              uVar50 = 0;
              uVar49 = 0;
              uVar48 = 0;
              uVar24 = FUN_100038b0(unaff_EBX + 0xf8);
              uVar14 = *(undefined4 *)(unaff_EBP - 0x78);
              pCVar32 = (LPCSTR)(unaff_EBX + 0x1ac);
              uVar47 = 0;
              uVar46 = 0;
              uVar13 = *(undefined4 *)(unaff_EBX + 0x178);
              uVar25 = FUN_100038b0(unaff_EBX + 0x17c);
              uVar26 = FUN_100038b0(unaff_EBX + 0xe8);
              uVar23 = *(undefined4 *)(unaff_EBP - 0xbc);
              uVar45 = 0;
              uVar27 = FUN_100038b0(unaff_EBX + 0xb8);
              uVar43 = 0;
              uVar28 = FUN_100038b0(unaff_EBX + 0x98);
              uVar29 = FUN_100038b0(unaff_EBX + 0x88);
              uVar42 = 0;
              uVar30 = FUN_100038b0(unaff_EBX + 0x68);
              uVar41 = FUN_100038b0(unaff_EBX + 0x58);
              FUN_100085b0((uint)(DAT_1005ac30 != (code *)0x0),*(HWND *)(unaff_EBX + 8),
                           *(undefined4 *)(unaff_EBP - 0x8c),uVar41,uVar30,uVar42,uVar29,uVar28,
                           uVar43,uVar27,uVar45,uVar23,uVar26,uVar25,uVar13,uVar46,uVar47,pCVar32,
                           uVar14,uVar24,uVar48,uVar49,uVar50,uVar51,uVar53,uVar55);
            }
            *(undefined1 *)(unaff_EBP - 4) = 0x86;
            (*pcVar38)();
          }
          *(undefined1 *)(unaff_EBP - 4) = 0x85;
          (*pcVar38)();
          *(undefined1 *)(unaff_EBP - 4) = 0x84;
          (*DAT_1005abc8)();
          *(undefined1 *)(unaff_EBP - 4) = 0x7e;
          (*DAT_1005abc8)();
          *(undefined1 *)(unaff_EBP - 4) = 0x7d;
          (*pcVar38)();
          iVar40 = *(int *)(unaff_EBP - 0x6c);
        }
      }
      else if (*(int *)(*(int *)(*(int *)(iVar8 + 0x18) + iVar40 * 4) + 8) == 0) {
        pcVar16 = (char *)(unaff_EBX + 0x1ac);
        FUN_10003910(unaff_EBP - 0x2f4);
        FUN_10007d90(&DAT_1005acc0,extraout_EDX_23);
        FUN_10007a90(&DAT_1005acc0,extraout_EDX_24);
        FUN_10003910(unaff_EBP - 0x3e4);
        _snprintf(pcVar16,0x100,"%s %s, %s %s");
        uVar14 = **(undefined4 **)
                   (*(int *)(*(int *)(unaff_EBP - 0xcc) + 0x18) + *(int *)(unaff_EBP - 0x6c) * 4);
        uVar56 = 0;
        uVar55 = 0;
        uVar53 = 0;
        uVar51 = 0;
        uVar50 = 0;
        uVar49 = 0;
        uVar24 = FUN_100038b0(unaff_EBX + 0xf8);
        uVar48 = 0;
        uVar47 = 0;
        uVar46 = 0;
        uVar13 = *(undefined4 *)(unaff_EBX + 0x178);
        uVar25 = FUN_100038b0(unaff_EBX + 0x17c);
        uVar26 = FUN_100038b0(unaff_EBX + 0xe8);
        uVar23 = *(undefined4 *)(unaff_EBP - 0xbc);
        uVar45 = 0;
        uVar27 = FUN_100038b0(unaff_EBX + 0xb8);
        uVar43 = 0;
        uVar28 = FUN_100038b0(unaff_EBX + 0x98);
        uVar29 = FUN_100038b0(unaff_EBX + 0x88);
        uVar42 = 0;
        uVar30 = FUN_100038b0(unaff_EBX + 0x68);
        uVar41 = FUN_100038b0(unaff_EBX + 0x58);
        FUN_100085b0((uint)(DAT_1005ac30 != (code *)0x0),*(HWND *)(unaff_EBX + 8),uVar14,uVar41,
                     uVar30,uVar42,uVar29,uVar28,uVar43,uVar27,uVar45,uVar23,uVar26,uVar25,uVar13,
                     uVar46,uVar47,pcVar16,uVar48,uVar24,uVar49,uVar50,uVar51,uVar53,uVar55,uVar56);
        iVar40 = *(int *)(unaff_EBP - 0x6c);
        pcVar38 = SysFreeString_exref;
      }
      *(undefined1 *)(unaff_EBP - 4) = 0x7c;
      (*DAT_1005abec)();
      *(undefined1 *)(unaff_EBP - 4) = 0x7b;
      (*DAT_1005abec)();
      *(undefined1 *)(unaff_EBP - 4) = 10;
      (*pcVar38)();
      iVar8 = *(int *)(unaff_EBP - 0xcc);
    }
  }
  else {
    piVar37 = *(int **)(*(int *)(unaff_EBX + 0x2f0) + 4);
    while (piVar37 != (int *)0x0) {
      if ((piVar37[10] == 0) && (iVar8 = FUN_10004fd0(*piVar37,0), iVar8 != 0)) {
        *(undefined4 *)(unaff_EBP - 0x148) = 0;
        *(undefined1 *)(unaff_EBP - 4) = 100;
        (*DAT_1005abe4)();
        *(undefined1 *)(unaff_EBP - 4) = 0x65;
        (*DAT_1005abe4)();
        *(undefined1 *)(unaff_EBP - 4) = 0x66;
        uVar14 = FUN_10004600(1,*piVar37,*(int *)(unaff_EBP - 0x88),*(int *)(unaff_EBP - 0x9c),
                              (undefined4 *)(unaff_EBP - 0x148),(void *)(unaff_EBP - 0x478),
                              (void *)(unaff_EBP - 0x458));
        if ((DAT_1005ac6c != 0) || ((char)uVar14 != '\0')) {
          pOVar31 = SysAllocString(L"sl=");
          *(BSTR *)(unaff_EBP - 0x80) = pOVar31;
          *(undefined1 *)(unaff_EBP - 4) = 0x67;
          pOVar31 = SysAllocString(L"yta5toidm://");
          *(BSTR *)(unaff_EBP - 0x138) = pOVar31;
          *(undefined1 *)(unaff_EBP - 4) = 0x68;
          pCVar32 = (LPCSTR)FUN_10001930((int)(piVar37 + 1));
          FUN_10003c00((void *)(unaff_EBP - 0x188),pCVar32);
          *(undefined1 *)(unaff_EBP - 4) = 0x69;
          FUN_10005660((void *)(unaff_EBP - 0x138),(undefined4 *)(unaff_EBP - 0x188));
          *(undefined1 *)(unaff_EBP - 4) = 0x68;
          SysFreeString(*(BSTR *)(unaff_EBP - 0x188));
          (*DAT_1005abc0)();
          *(undefined1 *)(unaff_EBP - 4) = 0x6a;
          (*DAT_1005abc0)();
          *(undefined1 *)(unaff_EBP - 4) = 0x6b;
          (*DAT_1005abc0)();
          *(undefined1 *)(unaff_EBP - 4) = 0x6c;
          (*DAT_1005abc0)();
          *(undefined1 *)(unaff_EBP - 4) = 0x6d;
          (*DAT_1005abc0)();
          *(undefined1 *)(unaff_EBP - 4) = 0x6e;
          (*DAT_1005abc0)();
          *(undefined1 *)(unaff_EBP - 4) = 0x6f;
          *(undefined4 *)(unaff_EBP - 0x130) = 0;
          *(undefined1 *)(unaff_EBP - 4) = 0x70;
          *(undefined4 *)(unaff_EBP - 0x154) = 0;
          *(undefined1 *)(unaff_EBP - 4) = 0x71;
          *(undefined4 *)(unaff_EBP - 0x128) = 0;
          *(undefined1 *)(unaff_EBP - 4) = 0x72;
          pCVar32 = (LPCSTR)FUN_10001930((int)(piVar37 + 5));
          FUN_10003c80((void *)(unaff_EBP - 0x130),pCVar32);
          FUN_100038f0((void *)(unaff_EBP - 0x404),*(undefined4 *)(unaff_EBP - 0x130));
          *(undefined1 **)(unaff_EBP - 100) = &stack0xffffffec;
          FUN_100055b0(&stack0xffffffec,unaff_EBP - 0x404);
          *(undefined1 *)(unaff_EBP - 4) = 0x73;
          *(undefined1 *)(unaff_EBP - 4) = 0x72;
          FUN_100059f0();
          pOVar19 = (OLECHAR *)FUN_100038b0(unaff_EBP - 0x550);
          FUN_10003b60((void *)(unaff_EBP - 0x1a4),pOVar19);
          *(undefined1 *)(unaff_EBP - 4) = 0x74;
          FUN_10005660((void *)(unaff_EBP - 0x80),(undefined4 *)(unaff_EBP - 0x1a4));
          *(undefined1 *)(unaff_EBP - 4) = 0x72;
          SysFreeString(*(BSTR *)(unaff_EBP - 0x1a4));
          pOVar31 = SysAllocString(L",au=");
          *(BSTR *)(unaff_EBP - 0x2a8) = pOVar31;
          *(undefined1 *)(unaff_EBP - 4) = 0x75;
          FUN_10005660((void *)(unaff_EBP - 0x80),(undefined4 *)(unaff_EBP - 0x2a8));
          *(undefined1 *)(unaff_EBP - 4) = 0x72;
          SysFreeString(pOVar31);
          piVar33 = FUN_100105b0(*(void **)(unaff_EBX + 0x2f0),*piVar37);
          if (piVar33 != (int *)0x0) {
            pCVar32 = (LPCSTR)FUN_10001930((int)(piVar33 + 1));
            FUN_10003c80((void *)(unaff_EBP - 0x154),pCVar32);
            FUN_100038f0((void *)(unaff_EBP - 0x428),*(undefined4 *)(unaff_EBP - 0x154));
            *(undefined1 **)(unaff_EBP - 100) = &stack0xffffffec;
            FUN_100055b0(&stack0xffffffec,unaff_EBP - 0x428);
            *(undefined1 *)(unaff_EBP - 4) = 0x76;
            *(undefined1 *)(unaff_EBP - 4) = 0x72;
            FUN_100059f0();
            pOVar19 = (OLECHAR *)FUN_100038b0(unaff_EBP - 0x4b8);
            FUN_10003b60((void *)(unaff_EBP - 0x19c),pOVar19);
            *(undefined1 *)(unaff_EBP - 4) = 0x77;
            FUN_10005660((void *)(unaff_EBP - 0x80),(undefined4 *)(unaff_EBP - 0x19c));
            *(undefined1 *)(unaff_EBP - 4) = 0x72;
            SysFreeString(*(BSTR *)(unaff_EBP - 0x19c));
            pOVar31 = SysAllocString(L",asl=");
            *(BSTR *)(unaff_EBP - 0x1ac) = pOVar31;
            *(undefined1 *)(unaff_EBP - 4) = 0x78;
            FUN_10005660((void *)(unaff_EBP - 0x80),(undefined4 *)(unaff_EBP - 0x1ac));
            *(undefined1 *)(unaff_EBP - 4) = 0x72;
            SysFreeString(*(BSTR *)(unaff_EBP - 0x1ac));
            pCVar32 = (LPCSTR)FUN_10001930((int)(piVar33 + 5));
            FUN_10003c80((void *)(unaff_EBP - 0x128),pCVar32);
            FUN_100038f0((void *)(unaff_EBP - 0x468),*(undefined4 *)(unaff_EBP - 0x128));
            *(undefined1 **)(unaff_EBP - 100) = &stack0xffffffec;
            FUN_100055b0(&stack0xffffffec,unaff_EBP - 0x468);
            *(undefined1 *)(unaff_EBP - 4) = 0x79;
            *(undefined1 *)(unaff_EBP - 4) = 0x72;
            FUN_100059f0();
            pOVar19 = (OLECHAR *)FUN_100038b0(unaff_EBP - 0x498);
            FUN_10003b60((void *)(unaff_EBP - 0x1b4),pOVar19);
            *(undefined1 *)(unaff_EBP - 4) = 0x7a;
            FUN_10005660((void *)(unaff_EBP - 0x80),(undefined4 *)(unaff_EBP - 0x1b4));
            *(undefined1 *)(unaff_EBP - 4) = 0x72;
            SysFreeString(*(BSTR *)(unaff_EBP - 0x1b4));
            *(undefined4 *)(unaff_EBP - 0x10c) = 0;
            iVar8 = FUN_100050c0(*piVar37);
            if (iVar8 != 0) {
              *(undefined4 *)(unaff_EBP - 0x10c) = 1;
            }
            iVar8 = FUN_100050c0(*piVar33);
            if (iVar8 != 0) {
              *(int *)(unaff_EBP - 0x10c) = *(int *)(unaff_EBP - 0x10c) + 1;
            }
            if (*(int *)(unaff_EBP - 0x10c) != 0) {
              FUN_100038f0((void *)(unaff_EBX + 0xd8),&DAT_1004cb74);
            }
            *(undefined4 *)(unaff_EBX + 0x2c8) = 0;
            *(undefined4 *)(unaff_EBX + 0x2cc) = 0;
            pcVar16 = (char *)(unaff_EBX + 0x1ac);
            FUN_10003910(unaff_EBP - 0x458);
            FUN_10007d90(&DAT_1005acc0,extraout_EDX_21);
            FUN_10007a90(&DAT_1005acc0,extraout_EDX_22);
            FUN_10003910(unaff_EBP - 0x478);
            _snprintf(pcVar16,0x100,"%s %s, %s %s");
            uVar55 = 0;
            uVar53 = 0;
            uVar51 = 0;
            uVar50 = 0;
            uVar49 = 0;
            uVar48 = 0;
            uVar24 = FUN_100038b0(unaff_EBX + 0xf8);
            uVar14 = *(undefined4 *)(unaff_EBP - 0x80);
            uVar47 = 0;
            uVar46 = 0;
            uVar13 = *(undefined4 *)(unaff_EBX + 0x178);
            uVar25 = FUN_100038b0(unaff_EBX + 0x17c);
            uVar26 = FUN_100038b0(unaff_EBX + 0xe8);
            uVar23 = *(undefined4 *)(unaff_EBP - 0x148);
            uVar45 = 0;
            uVar27 = FUN_100038b0(unaff_EBX + 0xb8);
            uVar43 = 0;
            uVar28 = FUN_100038b0(unaff_EBX + 0x98);
            uVar29 = FUN_100038b0(unaff_EBX + 0x88);
            uVar42 = 0;
            uVar30 = FUN_100038b0(unaff_EBX + 0x68);
            uVar41 = FUN_100038b0(unaff_EBX + 0x58);
            FUN_100085b0((uint)(DAT_1005ac30 != (code *)0x0),*(HWND *)(unaff_EBX + 8),
                         *(undefined4 *)(unaff_EBP - 0x138),uVar41,uVar30,uVar42,uVar29,uVar28,
                         uVar43,uVar27,uVar45,uVar23,uVar26,uVar25,uVar13,uVar46,uVar47,pcVar16,
                         uVar14,uVar24,uVar48,uVar49,uVar50,uVar51,uVar53,uVar55);
          }
          *(undefined1 *)(unaff_EBP - 4) = 0x71;
          SysFreeString(*(BSTR *)(unaff_EBP - 0x128));
          *(undefined1 *)(unaff_EBP - 4) = 0x70;
          SysFreeString(*(BSTR *)(unaff_EBP - 0x154));
          *(undefined1 *)(unaff_EBP - 4) = 0x6f;
          SysFreeString(*(BSTR *)(unaff_EBP - 0x130));
          *(undefined1 *)(unaff_EBP - 4) = 0x6e;
          (*DAT_1005abc8)();
          *(undefined1 *)(unaff_EBP - 4) = 0x6d;
          (*DAT_1005abc8)();
          *(undefined1 *)(unaff_EBP - 4) = 0x6c;
          (*DAT_1005abc8)();
          *(undefined1 *)(unaff_EBP - 4) = 0x6b;
          (*DAT_1005abc8)();
          *(undefined1 *)(unaff_EBP - 4) = 0x6a;
          (*DAT_1005abc8)();
          *(undefined1 *)(unaff_EBP - 4) = 0x68;
          (*DAT_1005abc8)();
          *(undefined1 *)(unaff_EBP - 4) = 0x67;
          SysFreeString(*(BSTR *)(unaff_EBP - 0x138));
          *(undefined1 *)(unaff_EBP - 4) = 0x66;
          SysFreeString(*(BSTR *)(unaff_EBP - 0x80));
        }
        *(undefined1 *)(unaff_EBP - 4) = 0x65;
        (*DAT_1005abec)();
        *(undefined1 *)(unaff_EBP - 4) = 100;
        (*DAT_1005abec)();
        *(undefined1 *)(unaff_EBP - 4) = 10;
        SysFreeString(*(BSTR *)(unaff_EBP - 0x148));
      }
      piVar37 = (int *)piVar37[9];
      *(int **)(unaff_EBP - 0x408) = piVar37;
    }
  }
  if (*(int *)(unaff_EBP - 0x114) != 0) {
    operator_delete(*(void **)(unaff_EBP - 0x114));
  }
  if (*(int *)(unaff_EBP - 0x16c) != 0) {
    operator_delete(*(void **)(unaff_EBP - 0x16c));
  }
  if (*(int *)(unaff_EBP - 0x108) != 0) {
    operator_delete(*(void **)(unaff_EBP - 0x108));
  }
  *(undefined1 *)(unaff_EBP - 4) = 8;
  FUN_10017f20((int *)(unaff_EBP - 0x14));
  *(undefined1 *)(unaff_EBP - 4) = 7;
  FUN_10007dc0((undefined4 *)(unaff_EBP - 0xac));
  *(undefined1 *)(unaff_EBP - 4) = 6;
  FUN_10003580((undefined4 *)(unaff_EBP - 0x1d8));
  *(undefined1 *)(unaff_EBP - 4) = 1;
  FUN_10017f20((int *)(unaff_EBP - 0x20));
  *(undefined1 *)(unaff_EBP - 4) = 0;
  FUN_10017f20((int *)(unaff_EBP - 0x18));
  *(undefined4 *)(unaff_EBP - 4) = 0xffffffff;
  uVar14 = extraout_EDX_27;
LAB_10009056:
  ExceptionList = *(void **)(unaff_EBP - 0xc);
  uStack0000000c = 0x10009071;
  FUN_100447e0(*(uint *)(unaff_EBP + 0x824) ^ unaff_EBP,uVar14);
  return;
}


