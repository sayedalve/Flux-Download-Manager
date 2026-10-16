// Function: FUN_0042a180
// Entry Point: 0x0042a180
// Program: IDMan.exe


/* WARNING: Function: __alloca_probe replaced with injection: alloca_probe */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0042a180(LPCWSTR param_1,LPCWSTR param_2,LPCWSTR param_3,BSTR param_4,LPCWSTR param_5,
                 LPCWSTR param_6,short *param_7,short *param_8,int param_9,LPCWSTR param_10,
                 short *param_11,short *param_12,wchar_t *param_13,LPCWSTR param_14,
                 wchar_t *param_15,int param_16,int param_17,LPCWSTR param_18,LPCWSTR param_19,
                 LPCWSTR param_20,LPCWSTR param_21,LPCWSTR param_22,LPCWSTR param_23,
                 LPCWSTR param_24,LPCWSTR param_25,LPCWSTR param_26,LPCWSTR param_27,
                 undefined4 param_28,LPCWSTR param_29,int param_30,UINT param_31,DWORD param_32,
                 DWORD param_33,int param_34)

{
  byte bVar1;
  WCHAR WVar2;
  OLECHAR OVar3;
  short sVar4;
  wchar_t wVar5;
  uint uVar6;
  char cVar7;
  undefined1 uVar8;
  int *lParam;
  HRESULT HVar9;
  int iVar10;
  LPSTR pCVar11;
  DWORD DVar12;
  char *pcVar13;
  char *pcVar14;
  int *piVar15;
  int iVar16;
  BSTR pOVar17;
  BSTR pOVar18;
  wchar_t *pwVar19;
  short *psVar20;
  char *pcVar21;
  wchar_t *pwVar22;
  LPCWSTR pWVar23;
  LPCWSTR pWVar24;
  byte *pbVar25;
  HWND pHVar26;
  LSTATUS LVar27;
  WCHAR *pWVar28;
  BYTE *_Str2;
  UINT UVar29;
  bool bVar30;
  bool bVar31;
  uint uStack_7dc8;
  int local_7d74;
  DWORD local_7d70;
  LPCWSTR local_7d6c;
  BSTR local_7d68;
  LPCWSTR local_7d64;
  LPCWSTR local_7d60;
  undefined1 *local_7d5c;
  void *local_7d58;
  undefined1 *puStack_7d54;
  undefined4 local_7d50;
  LPCWSTR local_7d4c;
  short *local_7d48;
  LPCWSTR local_7d44;
  LPCWSTR local_7d40;
  undefined1 local_7d3b;
  undefined1 local_7d3a;
  undefined1 local_7d39;
  LPCWSTR local_7d38;
  LPCWSTR local_7d34;
  LPCWSTR local_7d30;
  char *local_7d2c;
  undefined1 local_7d27;
  char local_7d26;
  char local_7d25;
  LPCWSTR local_7d24;
  LPCWSTR local_7d20;
  LPCWSTR local_7d1c;
  UINT local_7d18;
  DWORD local_7d14;
  char local_7d0e;
  char local_7d0d;
  LPCWSTR local_7d0c;
  HKEY local_7d08;
  wchar_t *local_7d04;
  char *local_7d00;
  undefined4 local_7cfc;
  UINT local_7cf8;
  int *local_7cf4 [2577];
  byte local_54b0 [21232];
  UINT local_1c0;
  UINT local_1bc;
  wchar_t *local_154;
  undefined4 local_150;
  short *local_14c;
  WCHAR *local_148;
  WCHAR local_144 [2];
  undefined4 local_140;
  BYTE local_13c;
  BYTE local_13b [259];
  char local_38 [8];
  undefined1 local_30;
  undefined1 local_28;
  undefined1 local_27;
  undefined1 local_26;
  undefined1 local_25;
  undefined1 local_24;
  undefined1 local_23;
  undefined1 local_22;
  undefined1 local_21;
  undefined1 local_20;
  undefined1 local_1f;
  undefined1 local_1e;
  undefined1 local_1d;
  undefined1 local_1c;
  undefined1 local_1b;
  undefined1 local_1a;
  undefined1 local_19;
  undefined1 local_18;
  undefined1 local_17;
  undefined1 local_16;
  undefined1 local_15;
  undefined1 local_14;
  undefined1 local_13;
  undefined1 local_12;
  undefined1 local_11;
  undefined1 local_10;
  undefined1 local_f;
  undefined1 local_e;
  undefined1 local_d;
  undefined1 local_c;
  uint local_8;
  
  puStack_7d54 = &LAB_006622f7;
  local_7d58 = ExceptionList;
  uStack_7dc8 = DAT_007839c0 ^ (uint)&local_7d4c;
  local_7d5c = (undefined1 *)&uStack_7dc8;
  ExceptionList = &local_7d58;
  local_7d6c = param_1;
  local_7d4c = param_3;
  local_7d68 = param_4;
  local_7d1c = param_5;
  local_7d64 = param_6;
  local_7d60 = param_10;
  local_14c = param_11;
  local_7d48 = param_12;
  local_7d04 = param_13;
  local_7d30 = param_14;
  local_7d24 = param_18;
  local_7d44 = param_22;
  local_7d40 = param_23;
  local_7d38 = param_24;
  local_7d34 = param_25;
  local_7d20 = param_26;
  local_7d0c = param_29;
  local_7d70 = param_32;
  local_7d14 = param_33;
  local_7d50 = 0;
  local_8 = uStack_7dc8;
  local_7d08 = (HKEY)FUN_00610e3a(0xa0);
  local_7d50._0_1_ = 1;
  if (local_7d08 == (HKEY)0x0) {
    lParam = (int *)0x0;
  }
  else {
    lParam = (int *)FUN_004267e0();
  }
  local_7d50._0_1_ = 0;
  uVar8 = (undefined1)local_7d50;
  local_7d50._0_1_ = 0;
  if (lParam == (int *)0x0) goto LAB_0042a4c6;
  FUN_004030a0(s_fkbkxacbiy<5_0077b76c);
  local_7d50._0_1_ = 2;
  lParam[0x24] = 0;
  lParam[0x25] = 0;
  lParam[0x23] = param_34;
  lParam[0x21] = param_9;
  lParam[0x22] = param_9;
  FUN_004030a0(s_fkbkxacbiy<5_0077b76c);
  local_7d50._0_1_ = 3;
  uVar8 = (undefined1)local_7d50;
  local_7d50._0_1_ = 3;
  if (lParam[0x21] == 0) {
    lParam[0x21] = 0x100;
  }
  if ((lParam[0x21] & 0x4000000U) != 0) {
    lParam[0x21] = lParam[0x21] & 0xffffffef;
    lParam[0x21] = lParam[0x21] | 8;
  }
  UVar29 = 0;
  local_7cf8 = 0;
  local_7d18 = 0;
  lParam[0x20] = 0;
  local_7d0d = '\0';
  if (param_30 == 0) {
    if (param_31 != 0) {
      local_7d18 = param_31;
      uVar8 = (undefined1)local_7d50;
    }
LAB_0042a442:
    local_7d50._0_1_ = uVar8;
    pWVar24 = local_7d6c;
    UVar29 = WideCharToMultiByte(local_7d18,0,local_7d6c,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
    local_7cf8 = UVar29;
    if (UVar29 == 0) {
      if (param_31 != 0) {
        local_7d18 = UVar29;
        UVar29 = WideCharToMultiByte(0,0,pWVar24,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
        local_7cf8 = UVar29;
      }
      goto LAB_0042a49c;
    }
LAB_0042a4ed:
    pCVar11 = (LPSTR)FUN_00402840(UVar29);
    iVar10 = WideCharToMultiByte(local_7d18,0,pWVar24,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
    if (iVar10 != 0) {
      local_148 = (WCHAR *)0x0;
      local_140 = 0;
      local_144[0] = L'\0';
      local_7d50._0_1_ = 7;
      FUN_00401970(*lParam,local_7d18);
      pWVar23 = pWVar24;
      pWVar28 = local_148;
      if (local_148 == (WCHAR *)0x0) {
        pWVar28 = local_144;
      }
      do {
        WVar2 = *pWVar23;
        bVar30 = (ushort)WVar2 < (ushort)*pWVar28;
        if (WVar2 != *pWVar28) {
LAB_0042a5e6:
          iVar10 = (1 - (uint)bVar30) - (uint)(bVar30 != 0);
          goto LAB_0042a5eb;
        }
        if (WVar2 == L'\0') break;
        WVar2 = pWVar23[1];
        bVar30 = (ushort)WVar2 < (ushort)pWVar28[1];
        if (WVar2 != pWVar28[1]) goto LAB_0042a5e6;
        pWVar23 = pWVar23 + 2;
        pWVar28 = pWVar28 + 2;
      } while (WVar2 != L'\0');
      iVar10 = 0;
LAB_0042a5eb:
      if (iVar10 != 0) {
        local_7d18 = 0xfde9;
        UVar29 = WideCharToMultiByte(0xfde9,0,pWVar24,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
        local_7cf8 = UVar29;
        if (UVar29 != 0) {
          FUN_004024f0();
          local_7d50 = CONCAT31(local_7d50._1_3_,8);
          pCVar11 = (LPSTR)FUN_00402840(UVar29);
          iVar10 = WideCharToMultiByte(0xfde9,0,pWVar24,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
          if ((iVar10 != 0) && (iVar10 = FUN_004b2450(local_7cf4[0]), iVar10 != 0)) {
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            FUN_00610e74(iVar10);
          }
          local_7d50._0_1_ = 7;
          FUN_00402560();
        }
      }
      lParam[0x20] = local_7d18;
      lParam[0x21] = lParam[0x21] | 0x80000;
      iVar10 = __strnicmp("ssidm://",(char *)*lParam,8);
      if (iVar10 == 0) {
        FUN_004024f0();
        local_7d50._0_1_ = 9;
        FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 8);
        FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
        local_7d50._0_1_ = 7;
        FUN_00402560();
      }
      iVar10 = __strnicmp("idmfc",(char *)*lParam,5);
      if (iVar10 == 0) {
        FUN_004024f0();
        local_7d50._0_1_ = 10;
        FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 5);
        FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
        local_7d50._0_1_ = 7;
        FUN_00402560();
      }
      iVar10 = __strnicmp("idm0x",(char *)*lParam,5);
      if (iVar10 == 0) {
        pcVar14 = (char *)(*lParam + 5);
        pcVar13 = pcVar14;
        do {
          cVar7 = *pcVar13;
          pcVar13 = pcVar13 + 1;
        } while (cVar7 != '\0');
        if (0xe < (uint)((int)pcVar13 - (*lParam + 6))) {
          _strncpy(local_38,pcVar14,8);
          local_30 = 0;
          for (pcVar14 = local_38; *pcVar14 != '\0'; pcVar14 = pcVar14 + 1) {
            if (*pcVar14 != '0') {
              if (*pcVar14 != '\0') {
                local_7d08 = (HKEY)0x0;
                iVar10 = FID_conflict__sscanf(pcVar14,"%x",&local_7d08);
                if (((iVar10 == 1) && (local_7d08 != (HKEY)0x0)) && (local_7d08 < (HKEY)0x2)) {
                  FUN_004024f0();
                  local_7d50._0_1_ = 0xb;
                  FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 0xd);
                  FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
                  local_7d50._0_1_ = 7;
                  FUN_00402560();
                  goto LAB_0042a825;
                }
              }
              break;
            }
          }
        }
      }
      else {
LAB_0042a825:
        FUN_005d2ca0(local_7cfc);
        FUN_00402c00(&DAT_0069cbbc,&DAT_0069765c);
        for (iVar10 = 4; iVar10 < 10; iVar10 = iVar10 + 1) {
          FUN_004024f0();
          local_7d50._0_1_ = 0xc;
          FUN_00402f20(local_7cf4,local_7cfc,iVar10);
          piVar15 = local_7cf4[0];
          do {
            iVar16 = *piVar15;
            piVar15 = (int *)((int)piVar15 + 1);
          } while ((char)iVar16 != '\0');
          iVar16 = __strnicmp((char *)local_7cf4[0],(char *)*lParam,
                              (int)piVar15 - ((int)local_7cf4[0] + 1));
          if (iVar16 == 0) {
            local_7d50._0_1_ = 7;
            FUN_00402560();
            goto LAB_0042a8a6;
          }
          local_7d50._0_1_ = 7;
          FUN_00402560();
        }
        iVar10 = __stricmp("idmcmd://startnetmon/",(char *)*lParam);
        if ((iVar10 == 0) ||
           (iVar10 = __stricmp("idmcmd://listennetmon/",(char *)*lParam), iVar10 == 0)) {
          _DAT_00788d18 = 1;
          while (*(int *)(DAT_007890a4 + 0x20) == 0) {
            FUN_00425ed0(0x32);
          }
          iVar10 = __stricmp("idmcmd://startnetmon/",(char *)*lParam);
          PostMessageA(*(HWND *)(DAT_007890a4 + 0x20),0x2b49,(uint)(iVar10 == 0),0);
        }
        else {
          FUN_005d2ca0(local_7d00);
          iVar10 = __strnicmp(local_7d00,(char *)*lParam,0xe);
          if (iVar10 == 0) {
            FUN_004024f0();
            local_7d50._0_1_ = 0xd;
            FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 0xe);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp("dhstoidm://",(char *)*lParam,0xb);
          if ((iVar10 == 0) ||
             (iVar10 = __strnicmp("dhs2toidm://",(char *)*lParam,0xc), iVar10 == 0)) {
            FUN_004024f0();
            local_7d50 = CONCAT31(local_7d50._1_3_,0xe);
            iVar10 = __strnicmp("dhs2toidm://",(char *)*lParam,0xc);
            if (iVar10 == 0) {
              FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 0xc);
              lParam[0x21] = lParam[0x21] | 0x40000;
            }
            else {
              FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 0xb);
            }
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            lParam[0x24] = 2;
            lParam[0x21] = lParam[0x21] | 0x8000;
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp("ytatoidm://",(char *)*lParam,0xb);
          if (iVar10 == 0) {
            FUN_004024f0();
            local_7d50._0_1_ = 0xf;
            FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 0xb);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            lParam[0x24] = 4;
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp("yta2toidm://",(char *)*lParam,0xc);
          if ((iVar10 == 0) ||
             (iVar10 = __strnicmp("yta3toidm://",(char *)*lParam,0xc), iVar10 == 0)) {
            iVar10 = __strnicmp("yta3toidm://",(char *)*lParam,0xc);
            if (iVar10 == 0) {
              lParam[0x21] = lParam[0x21] | 0x20000;
            }
            FUN_004024f0();
            local_7d50._0_1_ = 0x10;
            FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 0xc);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            lParam[0x24] = 4;
            lParam[0x21] = lParam[0x21] | 0x10000;
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp("yta5toidm://",(char *)*lParam,0xc);
          if (iVar10 == 0) {
            FUN_004024f0();
            local_7d50._0_1_ = 0x11;
            FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 0xc);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            lParam[0x24] = 0x10;
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp("idmhlstomp4://",(char *)*lParam,0xe);
          if (iVar10 == 0) {
            FUN_004024f0();
            local_7d50._0_1_ = 0x12;
            FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 0xe);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            lParam[0x24] = 8;
            lParam[0x25] = 1;
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp("idmhlstomp4v2://",(char *)*lParam,0x10);
          if (iVar10 == 0) {
            FUN_004024f0();
            local_7d50._0_1_ = 0x13;
            FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 0x10);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            lParam[0x24] = 8;
            lParam[0x25] = 1;
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp("hlstoidm://",(char *)*lParam,0xb);
          if (iVar10 == 0) {
            FUN_004024f0();
            local_7d50._0_1_ = 0x14;
            FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 0xb);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            lParam[0x24] = 8;
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp("hls2toidm://",(char *)*lParam,0xc);
          if (iVar10 == 0) {
            FUN_004024f0();
            local_7d50._0_1_ = 0x15;
            FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 0xc);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            lParam[0x24] = 0x100;
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp("hls2toidmtomp4://",(char *)*lParam,0x11);
          if (iVar10 == 0) {
            FUN_004024f0();
            local_7d50._0_1_ = 0x16;
            FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 0x11);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            lParam[0x24] = 0x100;
            lParam[0x25] = 1;
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp("vimjson://",(char *)*lParam,10);
          if (iVar10 == 0) {
            FUN_004024f0();
            local_7d50._0_1_ = 0x17;
            FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 10);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            lParam[0x24] = 0x800;
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp("idmfchttp://",(char *)*lParam,0xc);
          if (((iVar10 == 0) ||
              (iVar10 = __strnicmp("idmfchttps://",(char *)*lParam,0xd), iVar10 == 0)) ||
             (iVar10 = __strnicmp("idmfcftp://",(char *)*lParam,0xb), iVar10 == 0)) {
            FUN_004024f0();
            local_7d50._0_1_ = 0x18;
            FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 5);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp("mpdtoidm://",(char *)*lParam,0xb);
          if (iVar10 == 0) {
            FUN_004024f0();
            local_7d50._0_1_ = 0x19;
            iVar10 = __strnicmp("mpdtoidm:///",(char *)*lParam,0xc);
            if (iVar10 == 0) {
              pcVar14 = "http://idm%s";
            }
            else {
              pcVar14 = "%s";
            }
            FUN_00402f20(local_7cf4,pcVar14,*lParam + 0xb);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            lParam[0x24] = 0x40;
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp("mpdatoidm://",(char *)*lParam,0xc);
          if (iVar10 == 0) {
            FUN_004024f0();
            local_7d50._0_1_ = 0x1a;
            iVar10 = __strnicmp("mpdatoidm:///",(char *)*lParam,0xd);
            if (iVar10 == 0) {
              pcVar14 = "http://idm%s";
            }
            else {
              pcVar14 = "%s";
            }
            FUN_00402f20(local_7cf4,pcVar14,*lParam + 0xc);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            lParam[0x24] = 0x80;
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          iVar10 = __strnicmp(local_7d00,(char *)*lParam,0xe);
          if (iVar10 == 0) {
            FUN_004024f0();
            local_7d50._0_1_ = 0x1b;
            FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam + 0xe);
            FUN_00402f20(lParam,&DAT_00696f08,local_7cf4[0]);
            local_7d50._0_1_ = 7;
            FUN_00402560();
          }
          pcVar14 = (char *)*lParam;
          local_7d2c = pcVar14;
          iVar10 = __strnicmp("rtmpe://",pcVar14,8);
          if ((iVar10 == 0) ||
             ((iVar10 = __strnicmp("rtmp://",pcVar14,7), iVar10 == 0 &&
              ((lParam[0x21] & 0x400U) != 0)))) {
            FUN_00426bf0(1);
            AfxBeginThread(FUN_004264a0,(void *)0x0,0,0,0,(_SECURITY_ATTRIBUTES *)0x0);
          }
          else {
            iVar10 = FUN_00406b40(pcVar14);
            if ((iVar10 == 0) || (iVar10 = __strnicmp("idmreg://",pcVar14,9), iVar10 == 0)) {
              local_7d26 = '\0';
              local_7d0e = '\0';
              local_7d25 = '\0';
              pcVar13 = _strstr(pcVar14,".youtube.com");
              if (((pcVar13 != (char *)0x0) ||
                  ((pcVar13 = _strstr(pcVar14,".googlevideo.com"), pcVar13 != (char *)0x0 ||
                   (pcVar13 = _strstr(pcVar14,".drive.google.com"), pcVar13 != (char *)0x0)))) &&
                 (pcVar13 = _strstr(pcVar14,"videoplayback"), pcVar13 != (char *)0x0)) {
                local_7d25 = '\x01';
                pcVar13 = _strstr(pcVar14,"range=");
                if (((((pcVar13 != (char *)0x0) ||
                      (pcVar13 = _strstr(pcVar14,"begin="), pcVar13 != (char *)0x0)) ||
                     (pcVar13 = _strstr(pcVar14,"&ump="), pcVar13 != (char *)0x0)) ||
                    ((pcVar13 = _strstr(pcVar14,"&len="), pcVar13 != (char *)0x0 ||
                     (pcVar13 = _strstr(pcVar14,"?ump="), pcVar13 != (char *)0x0)))) ||
                   (pcVar13 = _strstr(pcVar14,"?len="), pcVar13 != (char *)0x0)) {
                  FUN_004024f0();
                  local_7d50 = CONCAT31(local_7d50._1_3_,0x1c);
                  FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam);
                  pcVar13 = _strstr(pcVar14,"range=");
                  if (pcVar13 != (char *)0x0) {
                    FUN_00426c10(local_7cf4[0],"&range=","?range=",lParam);
                    FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam);
                  }
                  pcVar13 = _strstr(pcVar14,"begin=");
                  if (pcVar13 != (char *)0x0) {
                    FUN_00426c10(local_7cf4[0],"&begin=","?begin=",lParam);
                    FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam);
                  }
                  pcVar13 = _strstr(pcVar14,"?len=");
                  if ((pcVar13 != (char *)0x0) ||
                     (pcVar13 = _strstr(pcVar14,"&len="), pcVar13 != (char *)0x0)) {
                    FUN_00426c10(local_7cf4[0],"&len=","?len=",lParam);
                    FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam);
                  }
                  pcVar13 = _strstr(pcVar14,"&ump=");
                  if ((pcVar13 != (char *)0x0) ||
                     (pcVar14 = _strstr(pcVar14,"?ump="), pcVar14 != (char *)0x0)) {
                    FUN_00426c10(local_7cf4[0],"&ump=","?ump=",lParam);
                    local_7d0e = '\x01';
                  }
                  pcVar14 = (char *)*lParam;
                  param_16 = 0;
                  param_17 = 0;
                  local_7d26 = '\x01';
                  local_7d50._0_1_ = 7;
                  local_7d2c = pcVar14;
                  FUN_00402560();
                }
                pcVar13 = _strstr(pcVar14,"sig=");
                if ((pcVar13 != (char *)0x0) &&
                   (pcVar13 = _strstr(pcVar14,"signature="), pcVar13 == (char *)0x0)) {
                  FUN_004024f0();
                  local_7d50 = CONCAT31(local_7d50._1_3_,0x1d);
                  cVar7 = FUN_00426da0(*lParam,&DAT_0069c85c,local_7cf4);
                  if (cVar7 == '\0') {
                    cVar7 = FUN_00426da0(*lParam,"signature",local_7cf4);
                    if (cVar7 != '\0') {
                      piVar15 = local_7cf4[0];
                      do {
                        iVar10 = *piVar15;
                        piVar15 = (int *)((int)piVar15 + 1);
                      } while ((char)iVar10 != '\0');
                      local_7d08 = (HKEY)((int)piVar15 - (int)((int)local_7cf4[0] + 1));
                      for (iVar10 = 0; iVar10 < (int)local_7d08; iVar10 = iVar10 + 1) {
                        uVar8 = FUN_004149b0(iVar10);
                        iVar16 = FUN_00425fb0(uVar8);
                        if ((iVar16 == 0) && (cVar7 = FUN_004149b0(iVar10), cVar7 != '.')) {
                          FUN_00402c00("?signature=","?sig=");
                          pcVar13 = "&sig=";
                          pcVar14 = "&signature=";
                          goto LAB_0042b3a0;
                        }
                      }
                    }
                  }
                  else {
                    piVar15 = local_7cf4[0];
                    do {
                      iVar10 = *piVar15;
                      piVar15 = (int *)((int)piVar15 + 1);
                    } while ((char)iVar10 != '\0');
                    local_7d08 = (HKEY)((int)piVar15 - (int)((int)local_7cf4[0] + 1));
                    for (iVar10 = 0; iVar10 < (int)local_7d08; iVar10 = iVar10 + 1) {
                      uVar8 = FUN_004149b0(iVar10);
                      iVar16 = FUN_00425fb0(uVar8);
                      if ((iVar16 == 0) && (cVar7 = FUN_004149b0(iVar10), cVar7 != '.'))
                      goto LAB_0042b3ac;
                    }
                    FUN_00402c00("?sig=","?signature=");
                    pcVar13 = "&signature=";
                    pcVar14 = "&sig=";
LAB_0042b3a0:
                    FUN_00402c00(pcVar14,pcVar13);
                    pcVar14 = (char *)*lParam;
                    local_7d2c = pcVar14;
                  }
LAB_0042b3ac:
                  local_7d50._0_1_ = 7;
                  FUN_00402560();
                }
              }
              if (((DAT_00789338 == 0) ||
                  ((((lParam[0x21] & 4U) != 0 && ((lParam[0x21] & 0x800U) == 0)) ||
                   (iVar10 = FUN_00422aa0(*lParam), iVar10 == 0)))) &&
                 ((((DAT_007890a4 == 0 || (*(int *)(DAT_007890a4 + 0x20) == 0)) ||
                   (*(int *)(DAT_007890a4 + 0x370) < 1)) ||
                  (((*(byte *)(lParam + 0x21) & 8) == 0 ||
                   (iVar10 = __stricmp(*(char **)(DAT_007890a4 + 0x374),pcVar14), iVar10 != 0))))))
              {
                if (param_2 != (LPCWSTR)0x0) {
                  pWVar24 = param_2;
                  do {
                    WVar2 = *pWVar24;
                    pWVar24 = pWVar24 + 1;
                  } while (WVar2 != L'\0');
                  if ((((int)pWVar24 - (int)(param_2 + 1) >> 1 != 0) &&
                      (UVar29 = WideCharToMultiByte(0,0,param_2,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                    (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0))
                     && ((int)UVar29 < 5000)) {
                    pCVar11 = (LPSTR)FUN_00402840(UVar29);
                    WideCharToMultiByte(0,0,param_2,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                  }
                }
                if (local_7d4c != (LPCWSTR)0x0) {
                  pWVar24 = local_7d4c;
                  do {
                    WVar2 = *pWVar24;
                    pWVar24 = pWVar24 + 1;
                  } while (WVar2 != L'\0');
                  if ((((int)pWVar24 - (int)(local_7d4c + 1) >> 1 != 0) &&
                      (UVar29 = WideCharToMultiByte(0,0,local_7d4c,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                    (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0))
                     && ((int)UVar29 < 10000)) {
                    pCVar11 = (LPSTR)FUN_00402840(UVar29);
                    WideCharToMultiByte(0,0,local_7d4c,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                  }
                }
                if (local_7d60 != (LPCWSTR)0x0) {
                  pWVar24 = local_7d60;
                  do {
                    WVar2 = *pWVar24;
                    pWVar24 = pWVar24 + 1;
                  } while (WVar2 != L'\0');
                  if ((((int)pWVar24 - (int)(local_7d60 + 1) >> 1 != 0) &&
                      (UVar29 = WideCharToMultiByte(0,0,local_7d60,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                    (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0))
                     && ((int)UVar29 < 2000)) {
                    pCVar11 = (LPSTR)FUN_00402840(UVar29);
                    WideCharToMultiByte(0,0,local_7d60,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                  }
                }
                pOVar17 = local_7d68;
                if ((local_7d68 != (BSTR)0x0) && (UVar29 = SysStringLen(local_7d68), UVar29 != 0)) {
                  if (local_7d25 != '\0') {
                    pOVar18 = pOVar17;
                    do {
                      OVar3 = *pOVar18;
                      pOVar18 = pOVar18 + 1;
                    } while (OVar3 != L'\0');
                    if ((int)pOVar18 - (int)(pOVar17 + 1) >> 1 == 1) {
                      local_7d0e = '\x01';
                      lParam[0x21] = lParam[0x21] & 0xfffffdff;
                      goto LAB_0042b5e4;
                    }
                  }
                  if (local_7d0e == '\0') {
                    UVar29 = WideCharToMultiByte(0,0,pOVar17,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0
                                                );
                    local_7cf8 = UVar29;
                    if ((UVar29 != 0) && ((int)UVar29 < 500000)) {
                      pCVar11 = (LPSTR)FUN_00402840(UVar29);
                      WideCharToMultiByte(0,0,local_7d68,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                    }
                  }
                  else {
                    lParam[0x21] = lParam[0x21] & 0xfffffdff;
                  }
                }
LAB_0042b5e4:
                if (local_7d1c != (LPCWSTR)0x0) {
                  pWVar24 = local_7d1c;
                  do {
                    WVar2 = *pWVar24;
                    pWVar24 = pWVar24 + 1;
                  } while (WVar2 != L'\0');
                  if ((((int)pWVar24 - (int)(local_7d1c + 1) >> 1 != 0) &&
                      (UVar29 = WideCharToMultiByte(0,0,local_7d1c,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                    (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0))
                     && ((int)UVar29 < 99)) {
                    pCVar11 = (LPSTR)FUN_00402840(UVar29);
                    WideCharToMultiByte(0,0,local_7d1c,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                  }
                }
                if (local_7d64 != (LPCWSTR)0x0) {
                  pWVar24 = local_7d64;
                  do {
                    WVar2 = *pWVar24;
                    pWVar24 = pWVar24 + 1;
                  } while (WVar2 != L'\0');
                  if ((((int)pWVar24 - (int)(local_7d64 + 1) >> 1 != 0) &&
                      (UVar29 = WideCharToMultiByte(0,0,local_7d64,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                    (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0))
                     && ((int)UVar29 < 99)) {
                    pCVar11 = (LPSTR)FUN_00402840(UVar29);
                    WideCharToMultiByte(0,0,local_7d64,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                  }
                }
                if (param_7 != (short *)0x0) {
                  psVar20 = param_7;
                  do {
                    sVar4 = *psVar20;
                    psVar20 = psVar20 + 1;
                  } while (sVar4 != 0);
                  if ((int)psVar20 - (int)(param_7 + 1) >> 1 != 0) {
                    FUN_00410c20(param_7);
                  }
                }
                if (param_8 != (short *)0x0) {
                  psVar20 = param_8;
                  do {
                    sVar4 = *psVar20;
                    psVar20 = psVar20 + 1;
                  } while (sVar4 != 0);
                  if (((int)psVar20 - (int)(param_8 + 1) >> 1 != 0) &&
                     (((lParam[0x21] & 0x800U) == 0 || ((lParam[0x21] & 0x40U) != 0)))) {
                    FUN_00410c20(param_8);
                    if (local_7d04 != (wchar_t *)0x0) {
                      pwVar19 = local_7d04;
                      do {
                        wVar5 = *pwVar19;
                        pwVar19 = pwVar19 + 1;
                      } while (wVar5 != L'\0');
                      if ((int)pwVar19 - (int)(local_7d04 + 1) >> 1 != 0) {
                        local_7d3a = 0;
                        pwVar19 = _wcsrchr((wchar_t *)lParam[8],L'.');
                        if (pwVar19 != (wchar_t *)0x0) {
                          pwVar19 = pwVar19 + 1;
                          iVar10 = __wcsicmp(local_7d04,pwVar19);
                          if (iVar10 == 0) {
                            local_7d3a = 1;
                            goto LAB_0042b821;
                          }
                        }
                        FUN_00430560();
                        local_7d50._0_1_ = 0x1e;
                        FUN_00417910(pwVar19);
                        local_7d50._0_1_ = 0x1f;
                        FUN_00417910(local_7d04);
                        local_7d50._0_1_ = 0x20;
                        FUN_004024f0();
                        local_7d50 = CONCAT31(local_7d50._1_3_,0x21);
                        FUN_00402f20(&local_7d1c,"idmcheckedtype/%s",local_7d08);
                        iVar10 = FUN_00432cd0(local_7d1c,local_7d4c);
                        if (iVar10 == 0) {
                          FUN_004263c0(&DAT_00699b54);
                          FUN_004263c0(local_7d04);
                        }
                        local_7d50._0_1_ = 0x20;
                        FUN_00402560();
                        local_7d50._0_1_ = 0x1f;
                        FUN_00402560();
                        local_7d50._0_1_ = 0x1e;
                        FUN_00402560();
                        local_7d50._0_1_ = 7;
                        FUN_00430280();
                      }
                    }
                  }
                }
LAB_0042b821:
                if (local_14c != (short *)0x0) {
                  psVar20 = local_14c;
                  do {
                    sVar4 = *psVar20;
                    psVar20 = psVar20 + 1;
                  } while (sVar4 != 0);
                  if ((int)psVar20 - (int)(local_14c + 1) >> 1 != 0) {
                    FUN_00410c20(local_14c);
                  }
                }
                if (local_7d48 != (short *)0x0) {
                  psVar20 = local_7d48;
                  do {
                    sVar4 = *psVar20;
                    psVar20 = psVar20 + 1;
                  } while (sVar4 != 0);
                  if ((int)psVar20 - (int)(local_7d48 + 1) >> 1 != 0) {
                    FUN_00410c20(local_7d48);
                  }
                }
                pwVar19 = local_7d04;
                if (local_7d04 != (wchar_t *)0x0) {
                  pwVar22 = local_7d04;
                  do {
                    wVar5 = *pwVar22;
                    pwVar22 = pwVar22 + 1;
                  } while (wVar5 != L'\0');
                  if ((int)pwVar22 - (int)(local_7d04 + 1) >> 1 != 0) {
                    if (((lParam[0x21] & 0x1000U) != 0) &&
                       (iVar10 = __wcsicmp(local_7d04,L"BIN"), iVar10 == 0)) goto LAB_0042c910;
                    UVar29 = WideCharToMultiByte(0,0,pwVar19,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0
                                                );
                    local_7cf8 = UVar29;
                    if (UVar29 != 0) {
                      pCVar11 = (LPSTR)FUN_00402840(UVar29);
                      WideCharToMultiByte(0,0,local_7d04,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                      if (lParam[0x25] == 1) {
                        FUN_00402f80(&DAT_0069cb3c);
                      }
                    }
                  }
                }
                if (local_7d30 != (LPCWSTR)0x0) {
                  pWVar24 = local_7d30;
                  do {
                    WVar2 = *pWVar24;
                    pWVar24 = pWVar24 + 1;
                  } while (WVar2 != L'\0');
                  if (((int)pWVar24 - (int)(local_7d30 + 1) >> 1 != 0) &&
                     (UVar29 = WideCharToMultiByte(0xfde9,0,local_7d30,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                   (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0))
                  {
                    if (lParam[0x24] == 4) {
                      FUN_004024f0();
                      local_7d50 = CONCAT31(local_7d50._1_3_,0x22);
                      pcVar13 = (char *)FUN_00402840(UVar29);
                      WideCharToMultiByte(0,0,local_7d30,-1,pcVar13,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                      iVar10 = _strncmp("pdmt=",pcVar13,5);
                      pcVar14 = pcVar13;
                      if ((iVar10 == 0) && (pcVar21 = _strchr(pcVar13,0x2c), pcVar21 != (char *)0x0)
                         ) {
                        *pcVar21 = '\0';
                        pcVar14 = pcVar21 + 1;
                        FUN_0057f8f0(&local_150);
                        local_7d48 = (short *)0x0;
                        iVar10 = FUN_00580790(pcVar13 + 5,&local_150,&local_7d48);
                        if (iVar10 != 0) {
                          FUN_00402f20(lParam + 0xc,&DAT_00696f08,local_150);
                        }
                        FUN_0057f910(&local_150);
                      }
                      FUN_00402f20(lParam + 0x19,&DAT_00696f08,pcVar14);
                      local_7d50._0_1_ = 7;
                      FUN_00402560();
                    }
                    else {
                      pCVar11 = (LPSTR)FUN_00402840(UVar29);
                      WideCharToMultiByte(0xfde9,0,local_7d30,-1,pCVar11,UVar29,(LPCSTR)0x0,
                                          (LPBOOL)0x0);
                    }
                  }
                }
                pcVar14 = local_7d2c;
                if (param_15 != (wchar_t *)0x0) {
                  pwVar19 = param_15;
                  do {
                    wVar5 = *pwVar19;
                    pwVar19 = pwVar19 + 1;
                  } while (wVar5 != L'\0');
                  if ((((int)pwVar19 - (int)(param_15 + 1) >> 1 != 0) && (lParam[0x24] != 0x10)) &&
                     ((((pcVar13 = _strstr(local_7d2c,".youtube.com"), pcVar13 == (char *)0x0 &&
                        (pcVar13 = _strstr(pcVar14,".googlevideo.com"), pcVar13 == (char *)0x0)) ||
                       ((pcVar14 = _strstr(pcVar14,"videoplayback"), pcVar14 == (char *)0x0 ||
                        (iVar10 = __wcsicmp(param_15,L"text/plain"), iVar10 != 0)))) &&
                      (UVar29 = WideCharToMultiByte(0,0,param_15,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                    (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0))
                     )) {
                    pCVar11 = (LPSTR)FUN_00402840(UVar29);
                    WideCharToMultiByte(0,0,param_15,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                    local_154 = (wchar_t *)0x0;
                    local_14c = (short *)0x0;
                    local_150 = (uint)local_150._2_2_ << 0x10;
                    local_7d50._0_1_ = 0x23;
                    FUN_00401970(lParam[0xd],local_7d18);
                    pwVar22 = param_15;
                    pwVar19 = local_154;
                    if (local_154 == (wchar_t *)0x0) {
                      pwVar19 = (wchar_t *)&local_150;
                    }
                    do {
                      wVar5 = *pwVar22;
                      bVar30 = (ushort)wVar5 < (ushort)*pwVar19;
                      if (wVar5 != *pwVar19) {
LAB_0042bb8d:
                        iVar10 = (1 - (uint)bVar30) - (uint)(bVar30 != 0);
                        goto LAB_0042bb92;
                      }
                      if (wVar5 == L'\0') break;
                      wVar5 = pwVar22[1];
                      bVar30 = (ushort)wVar5 < (ushort)pwVar19[1];
                      if (wVar5 != pwVar19[1]) goto LAB_0042bb8d;
                      pwVar22 = pwVar22 + 2;
                      pwVar19 = pwVar19 + 2;
                    } while (wVar5 != L'\0');
                    iVar10 = 0;
LAB_0042bb92:
                    if (iVar10 != 0) {
                      local_7d18 = 0xfde9;
                      UVar29 = WideCharToMultiByte(0xfde9,0,param_15,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                   (LPBOOL)0x0);
                      local_7cf8 = UVar29;
                      if (UVar29 != 0) {
                        FUN_004024f0();
                        local_7d50 = CONCAT31(local_7d50._1_3_,0x24);
                        pCVar11 = (LPSTR)FUN_00402840(UVar29);
                        iVar10 = WideCharToMultiByte(0xfde9,0,param_15,-1,pCVar11,UVar29,(LPCSTR)0x0
                                                     ,(LPBOOL)0x0);
                        if (iVar10 != 0) {
                          FUN_00402f20(lParam + 0xd,&DAT_00696f08,local_7d1c);
                        }
                        local_7d50._0_1_ = 0x23;
                        FUN_00402560();
                      }
                    }
                    local_7d50._0_1_ = 7;
                    FUN_004011f0();
                  }
                }
                pWVar24 = local_7d24;
                lParam[0x1e] = param_16;
                lParam[0x1f] = param_17;
                if (local_7d24 != (LPCWSTR)0x0) {
                  pWVar23 = local_7d24;
                  do {
                    WVar2 = *pWVar23;
                    pWVar23 = pWVar23 + 1;
                  } while (WVar2 != L'\0');
                  if (((int)pWVar23 - (int)(local_7d24 + 1) >> 1 != 0) &&
                     (UVar29 = WideCharToMultiByte(0,0,local_7d24,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                   (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0))
                  {
                    pCVar11 = (LPSTR)FUN_00402840(UVar29);
                    WideCharToMultiByte(0,0,pWVar24,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                  }
                }
                if (param_19 != (LPCWSTR)0x0) {
                  pWVar24 = param_19;
                  do {
                    WVar2 = *pWVar24;
                    pWVar24 = pWVar24 + 1;
                  } while (WVar2 != L'\0');
                  if (((int)pWVar24 - (int)(param_19 + 1) >> 1 != 0) &&
                     (UVar29 = WideCharToMultiByte(0,0,param_19,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                   (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0))
                  {
                    pCVar11 = (LPSTR)FUN_00402840(UVar29);
                    WideCharToMultiByte(0,0,param_19,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                  }
                }
                if (param_20 != (LPCWSTR)0x0) {
                  pWVar24 = param_20;
                  do {
                    WVar2 = *pWVar24;
                    pWVar24 = pWVar24 + 1;
                  } while (WVar2 != L'\0');
                  if (((int)pWVar24 - (int)(param_20 + 1) >> 1 != 0) &&
                     (UVar29 = WideCharToMultiByte(0,0,param_20,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                   (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0))
                  {
                    pCVar11 = (LPSTR)FUN_00402840(UVar29);
                    WideCharToMultiByte(0,0,param_20,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                  }
                }
                if (param_21 != (LPCWSTR)0x0) {
                  pWVar24 = param_21;
                  do {
                    WVar2 = *pWVar24;
                    pWVar24 = pWVar24 + 1;
                  } while (WVar2 != L'\0');
                  if (((int)pWVar24 - (int)(param_21 + 1) >> 1 != 0) &&
                     (UVar29 = WideCharToMultiByte(0,0,param_21,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                   (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0))
                  {
                    pCVar11 = (LPSTR)FUN_00402840(UVar29);
                    WideCharToMultiByte(0,0,param_21,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                  }
                }
                if (local_7d44 != (LPCWSTR)0x0) {
                  pWVar24 = local_7d44;
                  do {
                    WVar2 = *pWVar24;
                    pWVar24 = pWVar24 + 1;
                  } while (WVar2 != L'\0');
                  if (((int)pWVar24 - (int)(local_7d44 + 1) >> 1 != 0) &&
                     (UVar29 = WideCharToMultiByte(0,0,local_7d44,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                   (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0))
                  {
                    piVar15 = lParam + 0x12;
                    pCVar11 = (LPSTR)FUN_00402840(UVar29);
                    WideCharToMultiByte(0,0,local_7d44,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                    pcVar14 = (char *)*piVar15;
                    pcVar13 = pcVar14;
                    do {
                      cVar7 = *pcVar13;
                      pcVar13 = pcVar13 + 1;
                    } while (cVar7 != '\0');
                    if (((pcVar13 != pcVar14 + 1) &&
                        (((pcVar14 = _strstr(pcVar14,".youtube.com"), pcVar14 != (char *)0x0 ||
                          (pcVar14 = _strstr((char *)*piVar15,".googlevideo.com"),
                          pcVar14 != (char *)0x0)) &&
                         (pcVar14 = _strstr((char *)*piVar15,"videoplayback"),
                         pcVar14 != (char *)0x0)))) &&
                       (((((pcVar14 = _strstr((char *)*piVar15,"range="), pcVar14 != (char *)0x0 ||
                           (pcVar14 = _strstr((char *)*piVar15,"begin="), pcVar14 != (char *)0x0))
                          || (pcVar14 = _strstr((char *)*piVar15,"&ump="), pcVar14 != (char *)0x0))
                         || ((pcVar14 = _strstr((char *)*piVar15,"&len="), pcVar14 != (char *)0x0 ||
                             (pcVar14 = _strstr((char *)*piVar15,"?ump="), pcVar14 != (char *)0x0)))
                         ) || (pcVar14 = _strstr((char *)*piVar15,"?len="), pcVar14 != (char *)0x0))
                       )) {
                      FUN_004024f0();
                      local_7d50 = CONCAT31(local_7d50._1_3_,0x25);
                      FUN_00402f20(&local_7d04,&DAT_00696f08,*piVar15);
                      pcVar14 = _strstr((char *)*piVar15,"range=");
                      if (pcVar14 != (char *)0x0) {
                        FUN_00426c10(*piVar15,"&range=","?range=",piVar15);
                        FUN_00402f20(&local_7d04,&DAT_00696f08,*piVar15);
                      }
                      pcVar14 = _strstr((char *)*piVar15,"begin=");
                      if (pcVar14 != (char *)0x0) {
                        FUN_00426c10(local_7d04,"&begin=","?begin=",piVar15);
                        FUN_00402f20(&local_7d04,&DAT_00696f08,*piVar15);
                      }
                      pcVar14 = _strstr((char *)*piVar15,"&len=");
                      if ((pcVar14 != (char *)0x0) ||
                         (pcVar14 = _strstr((char *)*piVar15,"?len="), pcVar14 != (char *)0x0)) {
                        FUN_00426c10(local_7d04,"&len=","?len=",piVar15);
                        FUN_00402f20(&local_7d04,&DAT_00696f08,*piVar15);
                      }
                      pcVar14 = _strstr((char *)*piVar15,"&ump=");
                      if ((pcVar14 != (char *)0x0) ||
                         (pcVar14 = _strstr((char *)*piVar15,"?ump="), pcVar14 != (char *)0x0)) {
                        FUN_00426c10(local_7d04,"&ump=","?ump=",piVar15);
                        lParam[0x21] = lParam[0x21] & 0xfffffdff;
                        FUN_00402f80(&DAT_006979dc);
                      }
                      if (local_7d26 == '\0') {
                        FUN_004024f0();
                        local_7d50 = CONCAT31(local_7d50._1_3_,0x26);
                        FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam);
                        pcVar14 = local_7d2c;
                        pcVar13 = _strstr(local_7d2c,"range=");
                        if (pcVar13 != (char *)0x0) {
                          FUN_00426c10(local_7cf4[0],"&range=","?range=",lParam);
                          FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam);
                        }
                        pcVar13 = _strstr(pcVar14,"begin=");
                        if (pcVar13 != (char *)0x0) {
                          FUN_00426c10(local_7cf4[0],"&begin=","?begin=",lParam);
                          FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam);
                        }
                        pcVar13 = _strstr(pcVar14,"&len=");
                        if ((pcVar13 != (char *)0x0) ||
                           (pcVar13 = _strstr(pcVar14,"?len="), pcVar13 != (char *)0x0)) {
                          FUN_00426c10(local_7cf4[0],"&len=","?len=",lParam);
                          FUN_00402f20(local_7cf4,&DAT_00696f08,*lParam);
                        }
                        pcVar13 = _strstr(pcVar14,"&ump=");
                        if ((pcVar13 != (char *)0x0) ||
                           (pcVar14 = _strstr(pcVar14,"?ump="), pcVar14 != (char *)0x0)) {
                          FUN_00426c10(local_7cf4[0],"&ump=","?ump=",lParam);
                          lParam[0x21] = lParam[0x21] & 0xfffffdff;
                          FUN_00402f80(&DAT_006979dc);
                        }
                        local_7d2c = (char *)*lParam;
                        param_16 = 0;
                        param_17 = 0;
                        lParam[0x1e] = 0;
                        lParam[0x1f] = 0;
                        local_7d50 = CONCAT31(local_7d50._1_3_,0x25);
                        FUN_00402560();
                      }
                      local_7d50._0_1_ = 7;
                      FUN_00402560();
                    }
                  }
                  if (local_7d40 != (LPCWSTR)0x0) {
                    pWVar24 = local_7d40;
                    do {
                      WVar2 = *pWVar24;
                      pWVar24 = pWVar24 + 1;
                    } while (WVar2 != L'\0');
                    if (((int)pWVar24 - (int)(local_7d40 + 1) >> 1 != 0) &&
                       (UVar29 = WideCharToMultiByte(0,0,local_7d40,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                     (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0)
                       ) {
                      pCVar11 = (LPSTR)FUN_00402840(UVar29);
                      WideCharToMultiByte(0,0,local_7d40,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                    }
                  }
                  if (local_7d38 != (LPCWSTR)0x0) {
                    pWVar24 = local_7d38;
                    do {
                      WVar2 = *pWVar24;
                      pWVar24 = pWVar24 + 1;
                    } while (WVar2 != L'\0');
                    if (((int)pWVar24 - (int)(local_7d38 + 1) >> 1 != 0) &&
                       (UVar29 = WideCharToMultiByte(0,0,local_7d38,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                     (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0)
                       ) {
                      pCVar11 = (LPSTR)FUN_00402840(UVar29);
                      WideCharToMultiByte(0,0,local_7d38,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                    }
                  }
                  if (local_7d34 != (LPCWSTR)0x0) {
                    pWVar24 = local_7d34;
                    do {
                      WVar2 = *pWVar24;
                      pWVar24 = pWVar24 + 1;
                    } while (WVar2 != L'\0');
                    if (((int)pWVar24 - (int)(local_7d34 + 1) >> 1 != 0) &&
                       (UVar29 = WideCharToMultiByte(0,0,local_7d34,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                     (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0)
                       ) {
                      pCVar11 = (LPSTR)FUN_00402840(UVar29);
                      WideCharToMultiByte(0,0,local_7d34,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                    }
                  }
                  if (local_7d20 != (LPCWSTR)0x0) {
                    pWVar24 = local_7d20;
                    do {
                      WVar2 = *pWVar24;
                      pWVar24 = pWVar24 + 1;
                    } while (WVar2 != L'\0');
                    if (((int)pWVar24 - (int)(local_7d20 + 1) >> 1 != 0) &&
                       (UVar29 = WideCharToMultiByte(0,0,local_7d20,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                     (LPBOOL)0x0), local_7cf8 = UVar29, UVar29 != 0)
                       ) {
                      pCVar11 = (LPSTR)FUN_00402840(UVar29);
                      WideCharToMultiByte(0,0,local_7d20,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                    }
                  }
                  if (param_27 != (LPCWSTR)0x0) {
                    pWVar24 = param_27;
                    do {
                      WVar2 = *pWVar24;
                      pWVar24 = pWVar24 + 1;
                    } while (WVar2 != L'\0');
                    if (((int)pWVar24 - (int)(param_27 + 1) >> 1 != 0) &&
                       (local_7cf8 = WideCharToMultiByte(0,0,param_27,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                                         (LPBOOL)0x0), local_7cf8 != 0)) {
                      pCVar11 = (LPSTR)FUN_00402840(local_7cf8);
                      WideCharToMultiByte(0,0,param_27,-1,pCVar11,local_7cf8,(LPCSTR)0x0,(LPBOOL)0x0
                                         );
                    }
                  }
                }
                FUN_00426b90(param_28,lParam + 0x1a);
                FUN_00426b90(local_7d70,lParam + 0x1b);
                FUN_00426b90(local_7d14,lParam + 0x1c);
                if (local_7d0c == (LPCWSTR)0x0) {
                  bVar1 = *(byte *)(lParam + 0x21);
joined_r0x0042c424:
                  if ((bVar1 & 0x20) != 0) {
                    FUN_00402f20(lParam + 0x1d,"IDwnlMgr");
                  }
                }
                else {
                  pWVar24 = local_7d0c;
                  do {
                    WVar2 = *pWVar24;
                    pWVar24 = pWVar24 + 1;
                  } while (WVar2 != L'\0');
                  if ((int)pWVar24 - (int)(local_7d0c + 1) >> 1 == 0) {
                    bVar1 = *(byte *)(lParam + 0x21);
                    goto joined_r0x0042c424;
                  }
                  UVar29 = WideCharToMultiByte(0,0,local_7d0c,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                               (LPBOOL)0x0);
                  local_7cf8 = UVar29;
                  if (UVar29 != 0) {
                    piVar15 = lParam + 0x1d;
                    pCVar11 = (LPSTR)FUN_00402840(UVar29);
                    WideCharToMultiByte(0,0,local_7d0c,-1,pCVar11,UVar29,(LPCSTR)0x0,(LPBOOL)0x0);
                    pcVar14 = _strstr((char *)*piVar15,"firefox.exe|");
                    if (((pcVar14 != (char *)0x0) ||
                        (pcVar14 = _strstr((char *)*piVar15,"Firefox.exe|"), pcVar14 != (char *)0x0)
                        ) && (pcVar14 = _strrchr((char *)*piVar15,0x7c), pcVar14 != (char *)0x0)) {
                      *pcVar14 = '\0';
                    }
                  }
                }
                if ((*(byte *)(lParam + 0x21) & 4) == 0) {
                  pcVar13 = (char *)lParam[3];
                  pcVar14 = pcVar13 + 1;
                  do {
                    cVar7 = *pcVar13;
                    pcVar13 = pcVar13 + 1;
                  } while (cVar7 != '\0');
                  iVar10 = FUN_004fa4c0(*lParam,0,lParam[5],lParam[6],lParam[1],lParam[2],
                                        (int)pcVar13 - (int)pcVar14,0,0,0);
                  if (iVar10 != 0) goto LAB_0042c910;
                  pcVar13 = (char *)lParam[0x12];
                  pcVar14 = pcVar13 + 1;
                  do {
                    cVar7 = *pcVar13;
                    pcVar13 = pcVar13 + 1;
                  } while (cVar7 != '\0');
                  if (pcVar13 != pcVar14) {
                    pcVar13 = (char *)lParam[0x13];
                    pcVar14 = pcVar13 + 1;
                    do {
                      cVar7 = *pcVar13;
                      pcVar13 = pcVar13 + 1;
                    } while (cVar7 != '\0');
                    iVar10 = FUN_004fa4c0(lParam[0x12],0,lParam[0x15],lParam[0x16],lParam[1],
                                          lParam[0x14],(int)pcVar13 - (int)pcVar14,0,0,0);
                    if (iVar10 != 0) goto LAB_0042c910;
                  }
                  pcVar13 = (char *)lParam[3];
                  pcVar14 = pcVar13 + 1;
                  do {
                    cVar7 = *pcVar13;
                    pcVar13 = pcVar13 + 1;
                  } while (cVar7 != '\0');
                  pWVar24 = (LPCWSTR)FUN_004fa4c0(*lParam,lParam[0x12],lParam[5],lParam[6],lParam[1]
                                                  ,lParam[2],(int)pcVar13 - (int)pcVar14,param_16,
                                                  param_17,1);
                  if (pWVar24 == (LPCWSTR)0x0) {
                    if ((*(byte *)(lParam + 0x21) & 0x20) == 0) {
                      local_7d0c = pWVar24;
                      iVar10 = FUN_004fa160(*lParam,lParam[1],&local_7d0c);
                      if (iVar10 != 0) goto LAB_0042c910;
                      if ((local_7d0c != (LPCWSTR)0x0) && ((*(byte *)(lParam + 0x21) & 0x10) != 0))
                      {
                        lParam[0x21] = 8;
                      }
                    }
                    goto LAB_0042c592;
                  }
LAB_0042c856:
                  FUN_00426bf0(1);
                }
                else {
                  FUN_004f9cc0(&DAT_006979dc,&DAT_006979dc,&DAT_006979dc,&DAT_006979dc,&DAT_006979dc
                               ,&DAT_006979dc,1,0,0,0);
LAB_0042c592:
                  pcVar14 = local_7d2c;
                  local_7d74 = 1;
                  bVar30 = false;
                  iVar10 = __strnicmp("idmreg://",local_7d2c,9);
                  if (iVar10 == 0) {
                    bVar30 = true;
                  }
                  else {
                    pcVar13 = _strstr(pcVar14,".com/autoreg.html?d=");
                    if ((pcVar13 != (char *)0x0) ||
                       (pcVar13 = _strstr(pcVar14,".com/fillregform.html?d="),
                       pcVar13 != (char *)0x0)) {
                      FUN_005509c0();
                      local_7d50._0_1_ = 0x27;
                      iVar10 = FUN_00552250(pcVar14,local_7d18);
                      if (iVar10 != 0) {
                        local_28 = 0x21;
                        local_27 = 0x66;
                        local_26 = 0x61;
                        local_25 = 0x7b;
                        local_24 = 0x6a;
                        local_23 = 0x7d;
                        local_22 = 0x61;
                        local_21 = 0x6a;
                        local_20 = 0x7b;
                        local_1f = 0x6b;
                        local_1e = 0x60;
                        local_1d = 0x78;
                        local_1c = 0x61;
                        local_1b = 99;
                        local_1a = 0x60;
                        local_19 = 0x6e;
                        local_18 = 0x6b;
                        local_17 = 0x62;
                        local_16 = 0x6e;
                        local_15 = 0x61;
                        local_14 = 0x6e;
                        local_13 = 0x68;
                        local_12 = 0x6a;
                        local_11 = 0x7d;
                        local_10 = 0x21;
                        local_f = 0x6c;
                        local_e = 0x60;
                        local_d = 0x62;
                        local_c = 0;
                        FUN_005d2ca0(&local_28);
                        FUN_004024f0();
                        local_7d50._0_1_ = 0x28;
                        FUN_004024f0();
                        local_7d50._0_1_ = 0x29;
                        FUN_00402f20(&local_7d20,"www%s",&local_28);
                        FUN_00402f20(&local_7d0c,"secure%s",&local_28);
                        pbVar25 = local_54b0;
                        pWVar24 = local_7d20;
                        do {
                          bVar1 = *pbVar25;
                          bVar31 = bVar1 < (byte)*pWVar24;
                          if (bVar1 != (byte)*pWVar24) {
LAB_0042c750:
                            iVar10 = (1 - (uint)bVar31) - (uint)(bVar31 != 0);
                            goto LAB_0042c755;
                          }
                          if (bVar1 == 0) break;
                          bVar1 = pbVar25[1];
                          bVar31 = bVar1 < *(byte *)((int)pWVar24 + 1);
                          if (bVar1 != *(byte *)((int)pWVar24 + 1)) goto LAB_0042c750;
                          pbVar25 = pbVar25 + 2;
                          pWVar24 = pWVar24 + 1;
                        } while (bVar1 != 0);
                        iVar10 = 0;
LAB_0042c755:
                        if (iVar10 == 0) {
LAB_0042c78b:
                          bVar30 = true;
                        }
                        else {
                          pbVar25 = local_54b0;
                          pWVar24 = local_7d0c;
                          do {
                            bVar1 = *pbVar25;
                            bVar31 = bVar1 < (byte)*pWVar24;
                            if (bVar1 != (byte)*pWVar24) {
LAB_0042c782:
                              iVar10 = (1 - (uint)bVar31) - (uint)(bVar31 != 0);
                              goto LAB_0042c787;
                            }
                            if (bVar1 == 0) break;
                            bVar1 = pbVar25[1];
                            bVar31 = bVar1 < *(byte *)((int)pWVar24 + 1);
                            if (bVar1 != *(byte *)((int)pWVar24 + 1)) goto LAB_0042c782;
                            pbVar25 = pbVar25 + 2;
                            pWVar24 = pWVar24 + 1;
                          } while (bVar1 != 0);
                          iVar10 = 0;
LAB_0042c787:
                          if (iVar10 == 0) goto LAB_0042c78b;
                        }
                        local_7d50._0_1_ = 0x28;
                        FUN_00402560();
                        local_7d50._0_1_ = 0x27;
                        FUN_00402560();
                      }
                      local_7d50._0_1_ = 7;
                      FUN_00550a00();
                      pcVar14 = local_7d2c;
                    }
                  }
                  if ((*(int *)(DAT_007890a4 + 0x20) == 0) && (DAT_00788d00 != 0)) {
                    if (bVar30) {
                      FUN_00402f20(&DAT_00788d5c,&DAT_00696f08,pcVar14);
                      pHVar26 = DAT_0078988c;
                      if ((DAT_0078988c != (HWND)0x0) ||
                         (pHVar26 = DAT_007898a0, DAT_007898a0 != (HWND)0x0)) {
                        PostMessageA(pHVar26,0x2b45,0,0);
                        FUN_00426bf0(1);
                        goto LAB_0042a8a6;
                      }
                      pHVar26 = FindWindowA((LPCSTR)0x0,"Internet Download Manager");
                      if (pHVar26 != (HWND)0x0) {
                        GetWindowThreadProcessId(pHVar26,&local_7d14);
                        DVar12 = GetCurrentProcessId();
                        if (local_7d14 == DVar12) {
                          SendMessageA(pHVar26,0x10,0,0);
                        }
                      }
                    }
                    goto LAB_0042c856;
                  }
                  while (*(int *)(DAT_007890a4 + 0x20) == 0) {
                    FUN_00425ed0(0x32);
                  }
                  if (bVar30) {
                    if (DAT_007898a0 != (HWND)0x0) {
                      PostMessageA(DAT_007898a0,0x2b45,0,0);
                    }
                    PostMessageA(*(HWND *)(DAT_007890a4 + 0x20),0x2b44,0,(LPARAM)lParam);
                  }
                  else {
                    if ((lParam[0x21] & 0x8000000U) != 0) {
                      pcVar13 = (char *)lParam[0xd];
                      pcVar14 = pcVar13 + 1;
                      do {
                        cVar7 = *pcVar13;
                        pcVar13 = pcVar13 + 1;
                      } while (cVar7 != '\0');
                      if (((pcVar13 != pcVar14) &&
                          (UVar29 = SysStringLen((BSTR)lParam[8]), UVar29 == 0)) &&
                         ((iVar10 = __strnicmp((char *)lParam[0xd],"text/html",9), iVar10 == 0 ||
                          (iVar10 = __strnicmp((char *)lParam[0xd],"application/json",0x10),
                          iVar10 == 0)))) goto LAB_0042c910;
                    }
                    if ((*(byte *)(lParam + 0x21) & 0x10) != 0) {
                      pcVar13 = (char *)lParam[0xd];
                      pcVar14 = pcVar13 + 1;
                      do {
                        cVar7 = *pcVar13;
                        pcVar13 = pcVar13 + 1;
                      } while (cVar7 != '\0');
                      if (((pcVar13 != pcVar14) &&
                          (UVar29 = SysStringLen((BSTR)lParam[8]), UVar29 == 0)) &&
                         (iVar10 = __strnicmp((char *)lParam[0xd],"text/html",9), iVar10 == 0))
                      goto LAB_0042c910;
                    }
                    if ((lParam[0x21] & 0x800010U) != 0) {
                      pcVar13 = (char *)lParam[0xd];
                      pcVar14 = pcVar13 + 1;
                      do {
                        cVar7 = *pcVar13;
                        pcVar13 = pcVar13 + 1;
                      } while (cVar7 != '\0');
                      if ((pcVar13 != pcVar14) &&
                         (UVar29 = SysStringLen((BSTR)lParam[8]), UVar29 == 0)) {
                        pcVar13 = (char *)lParam[0xb];
                        pcVar14 = pcVar13 + 1;
                        do {
                          cVar7 = *pcVar13;
                          pcVar13 = pcVar13 + 1;
                        } while (cVar7 != '\0');
                        if (pcVar13 != pcVar14) {
                          FUN_004024f0();
                          local_7d50._0_1_ = 0x2a;
                          FUN_00402f20(&local_7d04,&DAT_00696f08,lParam[0xd]);
                          pcVar14 = _strchr((char *)local_7d04,0x3b);
                          if (pcVar14 != (char *)0x0) {
                            *pcVar14 = '\0';
                          }
                          bVar30 = false;
                          local_7d27 = 0;
                          iVar10 = __stricmp((char *)local_7d04,"application/octet-stream");
                          if ((iVar10 == 0) ||
                             (iVar10 = __stricmp((char *)local_7d04,"binary/octet-stream"),
                             iVar10 == 0)) {
LAB_0042cae4:
                            lParam[0x21] = lParam[0x21] & 0xffffffef;
                            uVar6 = lParam[0x21];
                            lParam[0x21] = uVar6 | 8;
                            if ((uVar6 & 0x8000000) != 0) {
                              lParam[0x21] = lParam[0x21] & 0xf7ffffff;
                            }
                          }
                          else {
                            iVar10 = FUN_00432cd0(local_7d04,lParam[0xb]);
                            if (iVar10 == 0) {
                              FUN_004024f0();
                              local_7d50 = CONCAT31(local_7d50._1_3_,0x2b);
                              FUN_00402f20(&local_7d0c,"MIME\\Database\\Content Type\\%s",local_7d04
                                          );
                              LVar27 = RegOpenKeyExA((HKEY)0x80000000,(LPCSTR)local_7d0c,0,1,
                                                     &local_7d08);
                              if (LVar27 == 0) {
                                local_7d14 = 0x104;
                                LVar27 = RegQueryValueExA(local_7d08,"Extension",(LPDWORD)0x0,
                                                          (LPDWORD)0x0,&local_13c,&local_7d14);
                                if (LVar27 == 0) {
                                  _Str2 = &local_13c;
                                  if (local_13c == '.') {
                                    _Str2 = local_13b;
                                  }
                                  iVar10 = __stricmp((char *)lParam[0xb],(char *)_Str2);
                                  if (iVar10 == 0) {
                                    bVar30 = true;
                                    local_7d27 = 1;
                                  }
                                }
                                RegCloseKey(local_7d08);
                              }
                              local_7d50._0_1_ = 0x2a;
                              FUN_00402560();
                              if (!bVar30) goto LAB_0042cae4;
                            }
                            else {
                              local_7d27 = 1;
                            }
                          }
                          local_7d50._0_1_ = 7;
                          FUN_00402560();
                        }
                      }
                    }
                    pcVar14 = (char *)lParam[0xb];
                    pcVar13 = pcVar14;
                    do {
                      cVar7 = *pcVar13;
                      pcVar13 = pcVar13 + 1;
                    } while (cVar7 != '\0');
                    if (pcVar13 != pcVar14 + 1) {
                      iVar10 = __stricmp(pcVar14,"PDF");
                      if (iVar10 == 0) {
                        if ((*(byte *)(lParam + 0x21) & 0x10) != 0) {
                          lParam[0x21] = lParam[0x21] & 0xffffffef;
                          uVar6 = lParam[0x21];
                          lParam[0x21] = uVar6 | 8;
                          lParam[0x21] = uVar6 & 0xf7ffffff | 8;
                        }
                      }
                      else {
                        lParam[0x21] = lParam[0x21] & 0xf7ffffff;
                      }
                    }
                    pcVar14 = local_7d2c;
                    uVar6 = lParam[0x21];
                    if ((((((uVar6 & 8) == 0) && ((uVar6 & 0x8000000) == 0)) ||
                         ((uVar6 & 0x1000000) != 0)) && ((uVar6 & 0x20) == 0)) &&
                       (((uVar6 & 0x800) == 0 || (DAT_00789418 == 0)))) {
                      if ((uVar6 & 0x1000000) == 0) {
                        PostMessageA(*(HWND *)(DAT_007890a4 + 0x20),0x2b3e,0,(LPARAM)lParam);
                      }
                      else {
                        local_7d74 = SendMessageA(*(HWND *)(DAT_007890a4 + 0x20),0x2b3e,0,
                                                  (LPARAM)lParam);
                      }
                    }
                    else {
                      local_7d3b = 1;
                      if (((*(int *)(DAT_007890a4 + 0x20) != 0) &&
                          ((*(int *)(DAT_007890a4 + 0x23c) != 0 &&
                           (iVar10 = __strnicmp("rtmp://",local_7d2c,7), iVar10 != 0)))) &&
                         (iVar10 = __strnicmp("rtmpt://",pcVar14,8), iVar10 != 0)) {
                        local_7d39 = 1;
                        local_7d70 = 4;
                        LVar27 = RegQueryValueExA(DAT_00788d08,"RememberDuplLinksA",(LPDWORD)0x0,
                                                  (LPDWORD)0x0,(LPBYTE)&local_7d14,&local_7d70);
                        if ((LVar27 == 0) && (local_7d14 == 1)) {
                          local_7d39 = 0;
                        }
                        else {
                          iVar10 = FUN_0049f390(pcVar14,lParam[1],lParam[0x20]);
                          if (-1 < iVar10) {
                            PostMessageA(*(HWND *)(DAT_007890a4 + 0x20),0x2b3e,0,(LPARAM)lParam);
                            local_7d3b = 0;
                            goto LAB_0042cdd6;
                          }
                          if (iVar10 == -0x19) goto LAB_0042ce54;
                        }
                      }
                      FUN_004027b0(lParam);
                      local_7d50._0_1_ = 0x2c;
                      FUN_004027b0(lParam + 5);
                      local_7d50._0_1_ = 0x2d;
                      FUN_004027b0(lParam + 6);
                      local_7d50._0_1_ = 0x2e;
                      FUN_004027b0(lParam + 1);
                      local_7d50._0_1_ = 0x2f;
                      FUN_004027b0(lParam + 2);
                      local_7d50._0_1_ = 0x30;
                      FUN_004027b0(lParam + 0x12);
                      local_7d50._0_1_ = 0x31;
                      pcVar13 = (char *)lParam[3];
                      pcVar14 = pcVar13 + 1;
                      do {
                        cVar7 = *pcVar13;
                        pcVar13 = pcVar13 + 1;
                      } while (cVar7 != '\0');
                      local_7d74 = SendMessageA(*(HWND *)(DAT_007890a4 + 0x20),0x2b3e,0,
                                                (LPARAM)lParam);
                      if (local_7d74 == 0) {
                        param_17 = 0;
                        param_16 = 0;
                      }
                      FUN_004f9cc0(local_7d44,local_7d0c,local_7d40,local_7d38,local_7d34,local_7d20
                                   ,(int)pcVar13 - (int)pcVar14,param_16,param_17,local_7d74 != 0);
                      local_7d50._0_1_ = 0x30;
                      FUN_00402560();
                      local_7d50._0_1_ = 0x2f;
                      FUN_00402560();
                      local_7d50._0_1_ = 0x2e;
                      FUN_00402560();
                      local_7d50._0_1_ = 0x2d;
                      FUN_00402560();
                      local_7d50._0_1_ = 0x2c;
                      FUN_00402560();
                      local_7d50._0_1_ = 7;
                      FUN_00402560();
                    }
LAB_0042cdd6:
                    if (local_7d74 == 0) goto LAB_0042ce54;
                  }
                }
LAB_0042a8a6:
                local_7d50._0_1_ = 3;
                FUN_004011f0();
                local_7d50._0_1_ = 2;
                FUN_00402560();
                local_7d50._0_1_ = 0;
                FUN_00402560();
                uVar8 = (undefined1)local_7d50;
                goto LAB_0042a4c6;
              }
LAB_0042c910:
              FUN_00426bf0(1);
            }
            else {
              FUN_00426bf0(1);
              if ((*(byte *)(lParam + 0x21) & 0x20) == 0) {
                pOVar17 = SysAllocString(local_7d6c);
                AfxBeginThread(FUN_00426670,pOVar17,0,0,0,(_SECURITY_ATTRIBUTES *)0x0);
              }
            }
          }
        }
      }
LAB_0042ce54:
      local_7d50._0_1_ = 3;
      FUN_004011f0();
      local_7d50._0_1_ = 2;
      FUN_00402560();
      local_7d50._0_1_ = 0;
      FUN_00402560();
      uVar8 = (undefined1)local_7d50;
      goto LAB_0042a4c6;
    }
    FUN_004024f0();
    local_7d50._0_1_ = 5;
    DVar12 = GetLastError();
    FUN_00402f20(&local_7d30,
                 "WideCharToMultiByte() has returned with error %ld in SendLinkToIDM()\r\n",DVar12);
    FUN_004c4c30();
    local_7d50._0_1_ = 6;
    FUN_004c4da0(local_7d30,1);
    FUN_00426bf0(1);
    local_7d50._0_1_ = 5;
    FUN_004c4d90();
    local_7d50._0_1_ = 3;
    FUN_00402560();
  }
  else {
    FUN_00536830();
    local_7d50 = CONCAT31(local_7d50._1_3_,4);
    HVar9 = CoCreateInstance((IID *)&DAT_00733e0c,(LPUNKNOWN)0x0,0x17,(IID *)&DAT_0069c6a8,
                             local_7cf4);
    if ((-1 < HVar9) &&
       (iVar10 = (**(code **)(*local_7cf4[0] + 0x1c))(local_7cf4[0],param_30,&local_1c0),
       pWVar24 = local_7d6c, -1 < iVar10)) {
      local_7d18 = local_1bc;
      UVar29 = WideCharToMultiByte(local_1bc,0,local_7d6c,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
      local_7cf8 = UVar29;
      if ((int)UVar29 < 1) {
        local_7d18 = local_1c0;
        UVar29 = WideCharToMultiByte(local_1c0,0,pWVar24,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
        local_7cf8 = UVar29;
        if ((int)UVar29 < 1) {
          local_7d18 = 0;
        }
        else {
          local_7d0d = '\x01';
        }
      }
      else {
        local_7d0d = '\x01';
      }
    }
    local_7d50._0_1_ = 3;
    FUN_00521bb0();
    pWVar24 = local_7d6c;
    uVar8 = (undefined1)local_7d50;
    if (local_7d0d == '\0') goto LAB_0042a442;
LAB_0042a49c:
    if (UVar29 != 0) goto LAB_0042a4ed;
    FUN_00426bf0(1);
  }
  local_7d50._0_1_ = 2;
  FUN_00402560();
  local_7d50._0_1_ = 0;
  FUN_00402560();
  uVar8 = (undefined1)local_7d50;
LAB_0042a4c6:
  local_7d50._0_1_ = uVar8;
  ExceptionList = local_7d58;
  __security_check_cookie(local_8 ^ (uint)&local_7d4c);
  return;
}


