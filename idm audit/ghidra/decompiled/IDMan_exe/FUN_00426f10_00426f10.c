// Function: FUN_00426f10
// Entry Point: 0x00426f10
// Program: IDMan.exe


/* WARNING: Removing unreachable block (ram,0x00429a1f) */

void FUN_00426f10(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined4 *puVar1;
  WCHAR WVar2;
  OLECHAR OVar3;
  char cVar4;
  undefined1 uVar5;
  HRESULT HVar6;
  int iVar7;
  LPSTR pCVar8;
  char *pcVar9;
  char *pcVar10;
  char *pcVar11;
  int iVar12;
  BSTR pOVar13;
  wchar_t *pwVar14;
  int *piVar15;
  char **ppcVar16;
  undefined4 uVar17;
  LPCWSTR pWVar18;
  char **ppcVar19;
  SAFEARRAY *pSVar20;
  uint uVar21;
  code *pcVar22;
  int iVar23;
  uint uStack_2cc;
  undefined1 local_2bc [4];
  undefined1 local_2b8 [4];
  undefined1 local_2b4 [4];
  undefined1 local_2b0 [4];
  undefined1 local_2ac [4];
  undefined4 local_2a8;
  int local_2a4;
  undefined4 local_2a0 [2];
  int local_298;
  int local_294;
  int local_290;
  int local_28c;
  char *local_288;
  BSTR local_284;
  int local_280;
  BSTR local_27c;
  int local_278;
  uint local_274;
  uint local_270;
  char *local_26c;
  uint local_268;
  BSTR local_264;
  int local_260;
  int local_25c;
  int local_258;
  undefined4 local_254;
  undefined4 local_250;
  undefined4 local_24c;
  undefined8 local_248;
  BSTR local_240;
  undefined4 local_23c;
  undefined4 local_238;
  undefined4 local_234;
  BSTR local_230;
  undefined4 local_22c;
  BSTR local_228;
  undefined4 local_224;
  undefined4 local_220;
  undefined4 local_21c;
  undefined4 local_218;
  undefined4 local_214;
  undefined4 local_210;
  undefined4 local_20c;
  BSTR local_208;
  BSTR local_204;
  BSTR local_200;
  int *local_1fc;
  LPCWSTR local_1f8;
  undefined4 local_1f4;
  undefined4 local_1f0;
  undefined4 local_1ec;
  undefined4 local_1e8;
  undefined4 local_1e4;
  undefined4 local_1e0;
  undefined4 local_1dc;
  undefined4 local_1d8;
  undefined4 local_1d4;
  undefined4 local_1d0;
  BSTR local_1cc;
  BSTR local_1c8;
  undefined4 local_1c4;
  BSTR local_1c0;
  undefined4 local_1bc;
  undefined4 local_1b8;
  BSTR local_1b4;
  BSTR local_1b0;
  BSTR local_1ac;
  BSTR local_1a8;
  undefined4 local_1a4;
  BSTR local_1a0;
  undefined4 local_19c;
  BSTR local_198;
  undefined4 local_194;
  BSTR local_190;
  undefined4 local_18c;
  BSTR local_188;
  undefined4 local_184;
  BSTR local_180;
  BSTR local_17c;
  BSTR local_178;
  BSTR local_174;
  undefined4 local_170;
  uint local_16c;
  int local_168;
  undefined4 local_164;
  LPCWSTR local_160;
  BSTR local_15c;
  undefined4 local_158;
  BSTR local_154;
  undefined4 local_150;
  char *local_14c;
  char *local_148;
  char *local_144;
  char *local_140;
  wchar_t *local_13c;
  int local_138;
  int local_134;
  undefined4 local_130;
  LPCWSTR local_12c;
  undefined4 local_128;
  LPCWSTR local_124;
  undefined4 local_120;
  undefined4 local_11c;
  undefined4 local_118;
  int local_114;
  undefined4 local_110;
  undefined4 local_10c;
  SAFEARRAY *local_108;
  undefined4 local_104;
  undefined4 local_100;
  char *local_fc;
  char *local_f8;
  int local_f4;
  char *local_f0;
  undefined4 local_ec;
  int local_e8;
  char local_e1;
  UINT local_e0;
  undefined1 local_d9;
  char *local_d8;
  char *local_d4;
  char *local_d0;
  int *local_cc;
  char *local_c8;
  char *local_c4;
  undefined4 local_c0;
  undefined4 local_bc;
  char local_b5;
  char *local_b4 [2];
  char local_a9;
  int local_a8;
  char *local_a4;
  LONG local_a0;
  undefined4 local_9c;
  char *local_98;
  undefined1 *local_94;
  void *local_90;
  undefined1 *puStack_8c;
  int local_88;
  UINT local_84;
  UINT local_80;
  char local_18 [8];
  undefined1 local_10;
  uint local_8;
  
  puStack_8c = &LAB_00662128;
  local_90 = ExceptionList;
  uStack_2cc = DAT_007839c0 ^ (uint)&local_84;
  local_94 = (undefined1 *)&uStack_2cc;
  ExceptionList = &local_90;
  local_88 = 0;
  local_e1 = '\0';
  local_8 = uStack_2cc;
  FUN_004030a0(s_fkbkxacbiy<5_0077b76c);
  local_88._0_1_ = 1;
  pSVar20 = *(SAFEARRAY **)(param_3 + 8);
  local_108 = pSVar20;
  SafeArrayGetUBound(pSVar20,1,&local_28c);
  SafeArrayGetLBound(pSVar20,1,&local_260);
  SafeArrayGetUBound(local_108,2,&local_e8);
  SafeArrayGetLBound(local_108,2,&local_f4);
  local_1c0 = (BSTR)0x0;
  local_15c = (BSTR)0x0;
  local_264 = (BSTR)FUN_00610e3a(0x24);
  local_88._0_1_ = 2;
  if (local_264 == (BSTR)0x0) {
    local_cc = (int *)0x0;
  }
  else {
    local_cc = (int *)FUN_00426550();
  }
  piVar15 = local_cc;
  local_88._0_1_ = 1;
  if (local_cc == (int *)0x0) {
    local_88 = (uint)local_88._1_3_ << 8;
    FUN_00402560();
  }
  else {
    FUN_005d2ca0(local_d8);
    FUN_004024f0();
    local_88._0_1_ = 3;
    FUN_004024f0();
    local_88 = CONCAT31(local_88._1_3_,4);
    FUN_0040dbd0(param_2);
    pSVar20 = local_108;
    if (local_e8 - local_f4 < 3) {
      FUN_00402f80(&DAT_006979dc);
    }
    else {
      local_a0 = 0;
      local_9c = 3;
      SafeArrayGetElement(local_108,&local_a0,&local_1c0);
      FUN_0040dbd0(local_1c0);
      if (3 < local_e8 - local_f4) {
        local_a0 = 0;
        local_9c = 4;
        SafeArrayGetElement(pSVar20,&local_a0,&local_15c);
      }
    }
    local_e0 = 0;
    for (local_114 = local_260; local_114 <= local_28c; local_114 = local_114 + 1) {
      FUN_004024f0();
      local_88._0_1_ = 5;
      FUN_004024f0();
      local_88._0_1_ = 6;
      FUN_004024f0();
      local_88._0_1_ = 7;
      FUN_004024f0();
      local_88._0_1_ = 8;
      FUN_004024f0();
      pSVar20 = local_108;
      iVar7 = local_114;
      local_164 = 0;
      local_88._0_1_ = 10;
      local_a0 = local_114;
      local_9c = 0;
      SafeArrayGetElement(local_108,&local_a0,&local_12c);
      if ((iVar7 == local_260) && (local_15c != (BSTR)0x0)) {
        FUN_00536830();
        local_88 = CONCAT31(local_88._1_3_,0xb);
        HVar6 = CoCreateInstance((IID *)&DAT_00733e0c,(LPUNKNOWN)0x0,0x17,(IID *)&DAT_0069c6a8,
                                 &local_1fc);
        if (-1 < HVar6) {
          iVar7 = (**(code **)(*local_1fc + 0x1c))(local_1fc,local_15c,&local_84);
          if (-1 < iVar7) {
            local_e0 = local_80;
            iVar7 = WideCharToMultiByte(local_80,0,local_12c,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0
                                       );
            if (iVar7 < 1) {
              local_e0 = local_84;
              iVar7 = WideCharToMultiByte(local_84,0,local_12c,-1,(LPSTR)0x0,0,(LPCSTR)0x0,
                                          (LPBOOL)0x0);
              if (iVar7 < 1) {
                local_e0 = 0;
              }
            }
          }
        }
        local_88._0_1_ = 10;
        FUN_00521bb0();
      }
      iVar7 = WideCharToMultiByte(local_e0,0,local_12c,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
      if (iVar7 == 0) {
LAB_00427241:
        pcVar22 = SysFreeString_exref;
        SysFreeString(local_12c);
        uVar17 = 0;
      }
      else {
        pCVar8 = (LPSTR)FUN_00402840(iVar7);
        iVar7 = WideCharToMultiByte(local_e0,0,local_12c,-1,pCVar8,iVar7,(LPCSTR)0x0,(LPBOOL)0x0);
        if (iVar7 == 0) goto LAB_00427241;
        iVar7 = __strnicmp("ssidm://",local_98,8);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0xc;
          FUN_00402f20(&local_21c,&DAT_00696f08,local_98 + 8);
          FUN_00402f20(&local_98,&DAT_00696f08,local_21c);
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        iVar7 = __strnicmp("idmfc",local_98,5);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0xd;
          FUN_00402f20(&local_22c,&DAT_00696f08,local_98 + 5);
          FUN_00402f20(&local_98,&DAT_00696f08,local_22c);
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        iVar7 = __strnicmp("idm0x",local_98,5);
        if (iVar7 == 0) {
          local_26c = local_98 + 5;
          pcVar9 = local_26c;
          do {
            cVar4 = *pcVar9;
            pcVar9 = pcVar9 + 1;
          } while (cVar4 != '\0');
          if (0xe < (uint)((int)pcVar9 - (int)(local_98 + 6))) {
            _strncpy(local_18,local_26c,8);
            local_10 = 0;
            local_26c = local_18;
            while( true ) {
              if (*local_26c == '\0') goto LAB_00427394;
              if (*local_26c != '0') break;
              local_26c = local_26c + 1;
            }
            if (*local_26c != '\0') {
              local_270 = 0;
              iVar7 = FID_conflict__sscanf(local_26c,"%x",&local_270);
              if (((iVar7 == 1) && (local_270 != 0)) && (local_270 < 2)) {
                FUN_004024f0();
                local_88._0_1_ = 0xe;
                FUN_00402f20(&local_234,&DAT_00696f08,local_98 + 0xd);
                FUN_00402f20(&local_98,&DAT_00696f08,local_234);
                local_88._0_1_ = 10;
                FUN_00402560();
                goto LAB_004274be;
              }
            }
          }
LAB_00427394:
          local_88._0_1_ = 9;
          SysFreeString((BSTR)0x0);
          local_88._0_1_ = 8;
          FUN_00402560();
          local_88._0_1_ = 7;
          FUN_00402560();
          local_88._0_1_ = 6;
          FUN_00402560();
          local_88._0_1_ = 5;
          FUN_00402560();
          local_88._0_1_ = 4;
          FUN_00402560();
          local_88._0_1_ = 3;
          FUN_00402560();
          local_88._0_1_ = 1;
          FUN_00402560();
          local_88 = (uint)local_88._1_3_ << 8;
          FUN_00402560();
          goto LAB_00427013;
        }
LAB_004274be:
        iVar7 = __strnicmp(local_d8,local_98,0xe);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0xf;
          FUN_00402f20(&local_23c,&DAT_00696f08,local_98 + 0xe);
          FUN_00402f20(&local_98,&DAT_00696f08,local_23c);
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        local_a8 = 0;
        local_158 = 0;
        iVar7 = __strnicmp("dhstoidm://",local_98,0xb);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0x10;
          FUN_00402f20(&local_250,&DAT_00696f08,local_98 + 0xb);
          FUN_00402f20(&local_98,&DAT_00696f08,local_250);
          local_a8 = 2;
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        iVar7 = __strnicmp("hlstoidm://",local_98,0xb);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0x11;
          FUN_00402f20(&local_210,&DAT_00696f08,local_98 + 0xb);
          FUN_00402f20(&local_98,&DAT_00696f08,local_210);
          local_a8 = 8;
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        iVar7 = __strnicmp("idmhlstomp4://",local_98,0xe);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0x12;
          FUN_00402f20(&local_1d4,&DAT_00696f08,local_98 + 0xe);
          FUN_00402f20(&local_98,&DAT_00696f08,local_1d4);
          local_a8 = 8;
          local_158 = 1;
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        iVar7 = __strnicmp("idmhlstomp4v2://",local_98,0x10);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0x13;
          FUN_00402f20(&local_184,&DAT_00696f08,local_98 + 0x10);
          FUN_00402f20(&local_98,&DAT_00696f08,local_184);
          local_a8 = 8;
          local_158 = 1;
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        iVar7 = __strnicmp("hls2toidm://",local_98,0xc);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0x14;
          FUN_00402f20(&local_1dc,&DAT_00696f08,local_98 + 0xc);
          FUN_00402f20(&local_98,&DAT_00696f08,local_1dc);
          local_a8 = 0x100;
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        iVar7 = __strnicmp("hls2toidmtomp4://",local_98,0x11);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0x15;
          FUN_00402f20(&local_18c,&DAT_00696f08,local_98 + 0x11);
          FUN_00402f20(&local_98,&DAT_00696f08,local_18c);
          local_a8 = 0x100;
          local_158 = 1;
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        iVar7 = __strnicmp("vimjson://",local_98,10);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0x16;
          FUN_00402f20(&local_1e4,&DAT_00696f08,local_98 + 10);
          FUN_00402f20(&local_98,&DAT_00696f08,local_1e4);
          local_a8 = 0x800;
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        iVar7 = __strnicmp("ytatoidm://",local_98,0xb);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0x17;
          FUN_00402f20(&local_194,&DAT_00696f08,local_98 + 0xb);
          FUN_00402f20(&local_98,&DAT_00696f08,local_194);
          local_a8 = 4;
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        iVar7 = __strnicmp("idmfchttp://",local_98,0xc);
        if (iVar7 == 0) {
LAB_004278e9:
          FUN_004024f0();
          local_88._0_1_ = 0x18;
          FUN_00402f20(&local_1ec,&DAT_00696f08,local_98 + 5);
          FUN_00402f20(&local_98,&DAT_00696f08,local_1ec);
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        else {
          iVar7 = __strnicmp("idmfchttps://",local_98,0xd);
          if (iVar7 == 0) goto LAB_004278e9;
          iVar7 = __strnicmp("idmfcftp://",local_98,0xb);
          if (iVar7 == 0) goto LAB_004278e9;
        }
        iVar7 = __strnicmp("mpdtoidm://",local_98,0xb);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0x19;
          iVar7 = __strnicmp("mpdtoidm:///",local_98,0xc);
          if (iVar7 == 0) {
            pcVar9 = "http://idm%s";
          }
          else {
            pcVar9 = "%s";
          }
          FUN_00402f20(&local_150,pcVar9,local_98 + 0xb);
          FUN_00402f20(&local_98,&DAT_00696f08,local_150);
          local_a8 = 0x40;
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        iVar7 = __strnicmp("mpdatoidm://",local_98,0xc);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0x1a;
          iVar7 = __strnicmp("mpdatoidm:///",local_98,0xd);
          if (iVar7 == 0) {
            pcVar9 = "http://idm%s";
          }
          else {
            pcVar9 = "%s";
          }
          FUN_00402f20(&local_170,pcVar9,local_98 + 0xc);
          FUN_00402f20(&local_98,&DAT_00696f08,local_170);
          local_a8 = 0x80;
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        local_278 = 0;
        local_280 = 0;
        iVar7 = __strnicmp("yta2toidm://",local_98,0xc);
        if (iVar7 == 0) {
LAB_00427aaf:
          iVar7 = __strnicmp("yta3toidm://",local_98,0xc);
          if (iVar7 == 0) {
            local_280 = 1;
          }
          FUN_004024f0();
          local_88._0_1_ = 0x1b;
          FUN_00402f20(&local_19c,&DAT_00696f08,local_98 + 0xc);
          FUN_00402f20(&local_98,&DAT_00696f08,local_19c);
          local_a8 = 4;
          local_278 = 1;
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        else {
          iVar7 = __strnicmp("yta3toidm://",local_98,0xc);
          if (iVar7 == 0) goto LAB_00427aaf;
        }
        iVar7 = __strnicmp("yta5toidm://",local_98,0xc);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0x1c;
          FUN_00402f20(&local_1f4,&DAT_00696f08,local_98 + 0xc);
          FUN_00402f20(&local_98,&DAT_00696f08,local_1f4);
          local_a8 = 0x10;
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        iVar7 = __strnicmp(local_d8,local_98,0xe);
        if (iVar7 == 0) {
          FUN_004024f0();
          local_88._0_1_ = 0x1d;
          FUN_00402f20(&local_1a4,&DAT_00696f08,local_98 + 0xe);
          FUN_00402f20(&local_98,&DAT_00696f08,local_1a4);
          local_88._0_1_ = 10;
          FUN_00402560();
        }
        FUN_004024f0();
        local_88 = CONCAT31(local_88._1_3_,0x1e);
        if (local_a8 == 0x800) {
          pcVar9 = _strstr(local_98,"##");
          if (pcVar9 != (char *)0x0) {
            while( true ) {
              pcVar11 = pcVar9 + 2;
              pcVar10 = _strstr(pcVar11,"##");
              if (pcVar10 == (char *)0x0) break;
              pcVar9 = _strstr(pcVar11,"##");
            }
            *pcVar9 = '\0';
            FUN_00402f20(&local_1b8,&DAT_00696f08,pcVar11);
            pSVar20 = local_108;
          }
        }
        else {
          pcVar9 = _strchr(local_98,0x23);
          if (pcVar9 != (char *)0x0) {
            *pcVar9 = '\0';
          }
        }
        local_b5 = '\x01';
        local_a9 = '\0';
        if (local_a8 != 0x10) {
          pcVar9 = _strstr(local_98,".youtube.com");
          if (pcVar9 == (char *)0x0) {
            pcVar9 = _strstr(local_98,".googlevideo.com");
            if (pcVar9 != (char *)0x0) goto LAB_00427cb4;
          }
          else {
LAB_00427cb4:
            pcVar9 = _strstr(local_98,"videoplayback");
            if (pcVar9 != (char *)0x0) {
              pcVar9 = _strstr(local_98,"range=");
              if (pcVar9 == (char *)0x0) {
                pcVar9 = _strstr(local_98,"begin=");
                if (pcVar9 == (char *)0x0) {
                  pcVar9 = _strstr(local_98,"&ump=");
                  if (pcVar9 == (char *)0x0) {
                    pcVar9 = _strstr(local_98,"&len=");
                    if (pcVar9 == (char *)0x0) {
                      pcVar9 = _strstr(local_98,"?ump=");
                      if (pcVar9 == (char *)0x0) {
                        pcVar9 = _strstr(local_98,"?len=");
                        if (pcVar9 == (char *)0x0) goto LAB_00427e9d;
                      }
                    }
                  }
                }
              }
              FUN_004024f0();
              local_88 = CONCAT31(local_88._1_3_,0x1f);
              FUN_00402f20(&local_100,&DAT_00696f08,local_98);
              pcVar9 = _strstr(local_98,"range=");
              if (pcVar9 != (char *)0x0) {
                FUN_00426c10(local_98,"&range=","?range=",&local_98);
                FUN_00402f20(&local_100,&DAT_00696f08,local_98);
              }
              pcVar9 = _strstr(local_98,"begin=");
              if (pcVar9 != (char *)0x0) {
                FUN_00426c10(local_100,"&begin=","?begin=",&local_98);
                FUN_00402f20(&local_100,&DAT_00696f08,local_98);
              }
              pcVar9 = _strstr(local_98,"&len=");
              if (pcVar9 == (char *)0x0) {
                pcVar9 = _strstr(local_98,"?len=");
                if (pcVar9 != (char *)0x0) goto LAB_00427e19;
              }
              else {
LAB_00427e19:
                FUN_00426c10(local_100,"&len=","?len=",&local_98);
                FUN_00402f20(&local_100,&DAT_00696f08,local_98);
              }
              pcVar9 = _strstr(local_98,"&ump=");
              if (pcVar9 == (char *)0x0) {
                pcVar9 = _strstr(local_98,"?ump=");
                if (pcVar9 != (char *)0x0) goto LAB_00427e6f;
              }
              else {
LAB_00427e6f:
                FUN_00426c10(local_100,"&ump=","?ump=",&local_98);
              }
              local_b5 = '\0';
              local_a9 = '\x01';
              local_88 = CONCAT31(local_88._1_3_,0x1e);
              FUN_00402560();
            }
          }
LAB_00427e9d:
          pcVar9 = _strstr(local_98,".youtube.com");
          if (pcVar9 == (char *)0x0) {
            pcVar9 = _strstr(local_98,".googlevideo.com");
            if (pcVar9 == (char *)0x0) goto LAB_00428072;
          }
          pcVar9 = _strstr(local_98,"videoplayback");
          if (pcVar9 != (char *)0x0) {
            pcVar9 = _strstr(local_98,"sig=");
            if (pcVar9 != (char *)0x0) {
              pcVar9 = _strstr(local_98,"signature=");
              if (pcVar9 == (char *)0x0) {
                FUN_004024f0();
                local_88 = CONCAT31(local_88._1_3_,0x20);
                cVar4 = FUN_00426da0(local_98,&DAT_0069c85c,&local_f8);
                if (cVar4 == '\0') {
                  cVar4 = FUN_00426da0(local_98,"signature",&local_f8);
                  if (cVar4 != '\0') {
                    pcVar9 = local_f8 + 1;
                    pcVar11 = local_f8;
                    do {
                      cVar4 = *pcVar11;
                      pcVar11 = pcVar11 + 1;
                    } while (cVar4 != '\0');
                    for (iVar7 = 0; local_290 = iVar7, iVar7 < (int)pcVar11 - (int)pcVar9;
                        iVar7 = iVar7 + 1) {
                      uVar5 = FUN_004149b0(iVar7);
                      iVar12 = FUN_00425fb0(uVar5);
                      if (iVar12 == 0) {
                        cVar4 = FUN_004149b0(iVar7);
                        if (cVar4 != '.') {
                          FUN_00402c00("?signature=","?sig=");
                          pcVar11 = "&sig=";
                          pcVar9 = "&signature=";
                          goto LAB_00428053;
                        }
                      }
                    }
                  }
                }
                else {
                  pcVar9 = local_f8 + 1;
                  pcVar11 = local_f8;
                  do {
                    cVar4 = *pcVar11;
                    pcVar11 = pcVar11 + 1;
                  } while (cVar4 != '\0');
                  for (iVar7 = 0; local_294 = iVar7, iVar7 < (int)pcVar11 - (int)pcVar9;
                      iVar7 = iVar7 + 1) {
                    uVar5 = FUN_004149b0(iVar7);
                    iVar12 = FUN_00425fb0(uVar5);
                    if (iVar12 == 0) {
                      cVar4 = FUN_004149b0(iVar7);
                      if (cVar4 != '.') goto LAB_0042805b;
                    }
                  }
                  FUN_00402c00("?sig=","?signature=");
                  pcVar11 = "&signature=";
                  pcVar9 = "&sig=";
LAB_00428053:
                  FUN_00402c00(pcVar9,pcVar11);
                }
LAB_0042805b:
                local_88 = CONCAT31(local_88._1_3_,0x1e);
                FUN_00402560();
                pSVar20 = local_108;
              }
            }
          }
        }
LAB_00428072:
        iVar7 = local_114;
        local_a0 = local_114;
        local_9c = 1;
        SafeArrayGetElement(pSVar20,&local_a0,&local_27c);
        FUN_0040dbd0(local_27c);
        local_a0 = iVar7;
        local_9c = 2;
        SafeArrayGetElement(pSVar20,&local_a0,&local_1f8);
        FUN_00402f80(&DAT_006979dc);
        if (local_1f8 != (LPCWSTR)0x0) {
          pWVar18 = local_1f8;
          do {
            WVar2 = *pWVar18;
            pWVar18 = pWVar18 + 1;
          } while (WVar2 != L'\0');
          if ((int)pWVar18 - (int)(local_1f8 + 1) >> 1 != 0) {
            iVar7 = WideCharToMultiByte(0xfde9,0,local_1f8,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
            if (iVar7 != 0) {
              pCVar8 = (LPSTR)FUN_00402840(iVar7);
              WideCharToMultiByte(0xfde9,0,local_1f8,-1,pCVar8,iVar7,(LPCSTR)0x0,(LPBOOL)0x0);
            }
          }
        }
        iVar7 = local_114;
        local_160 = (LPCWSTR)0x0;
        local_228 = (BSTR)0x0;
        local_124 = (LPCWSTR)0x0;
        if (6 < local_e8 - local_f4) {
          local_a0 = local_114;
          local_9c = 5;
          SafeArrayGetElement(pSVar20,&local_a0,&local_228);
          local_a0 = iVar7;
          local_9c = 6;
          SafeArrayGetElement(pSVar20,&local_a0,&local_124);
          local_a0 = iVar7;
          local_9c = 7;
          SafeArrayGetElement(pSVar20,&local_a0,&local_160);
        }
        pcVar22 = SafeArrayGetElement_exref;
        FUN_00410c20(&DAT_00697cd8);
        FUN_00402f80(&DAT_006979dc);
        FUN_00402f80(&DAT_006979dc);
        if (local_228 != (BSTR)0x0) {
          pOVar13 = local_228;
          do {
            OVar3 = *pOVar13;
            pOVar13 = pOVar13 + 1;
          } while (OVar3 != L'\0');
          pcVar22 = SafeArrayGetElement_exref;
          if ((int)pOVar13 - (int)(local_228 + 1) >> 1 != 0) {
            FUN_00410c20(local_228);
            pcVar22 = SafeArrayGetElement_exref;
          }
        }
        if (local_160 != (LPCWSTR)0x0) {
          pWVar18 = local_160;
          do {
            WVar2 = *pWVar18;
            pWVar18 = pWVar18 + 1;
          } while (WVar2 != L'\0');
          pcVar22 = SafeArrayGetElement_exref;
          if ((int)pWVar18 - (int)(local_160 + 1) >> 1 != 0) {
            iVar7 = WideCharToMultiByte(0,0,local_160,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
            pcVar22 = SafeArrayGetElement_exref;
            if (iVar7 != 0) {
              pCVar8 = (LPSTR)FUN_00402840(iVar7);
              WideCharToMultiByte(0,0,local_160,-1,pCVar8,iVar7,(LPCSTR)0x0,(LPBOOL)0x0);
              pcVar22 = SafeArrayGetElement_exref;
            }
          }
        }
        if (local_124 != (LPCWSTR)0x0) {
          pWVar18 = local_124;
          do {
            WVar2 = *pWVar18;
            pWVar18 = pWVar18 + 1;
          } while (WVar2 != L'\0');
          pcVar22 = SafeArrayGetElement_exref;
          if ((int)pWVar18 - (int)(local_124 + 1) >> 1 != 0) {
            iVar7 = WideCharToMultiByte(0,0,local_124,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0);
            pcVar22 = SafeArrayGetElement_exref;
            if (iVar7 != 0) {
              pCVar8 = (LPSTR)FUN_00402840(iVar7);
              WideCharToMultiByte(0,0,local_124,-1,pCVar8,iVar7,(LPCSTR)0x0,(LPBOOL)0x0);
              pcVar22 = SafeArrayGetElement_exref;
            }
          }
        }
        local_174 = (BSTR)0x0;
        local_154 = (BSTR)0x0;
        local_200 = (BSTR)0x0;
        local_240 = (BSTR)0x0;
        local_208 = (BSTR)0x0;
        local_230 = (BSTR)0x0;
        local_1c8 = (BSTR)0x0;
        local_1cc = (BSTR)0x0;
        local_13c = (wchar_t *)0x0;
        local_17c = (BSTR)0x0;
        local_180 = (BSTR)0x0;
        local_188 = (BSTR)0x0;
        local_190 = (BSTR)0x0;
        local_198 = (BSTR)0x0;
        local_1a0 = (BSTR)0x0;
        local_284 = (BSTR)0x0;
        local_1a8 = (BSTR)0x0;
        local_1b0 = (BSTR)0x0;
        local_248._0_4_ = 0;
        local_248._4_4_ = 0;
        local_248 = 0;
        if (0x14 < local_e8 - local_f4) {
          local_9c = 8;
          (*pcVar22)(pSVar20,&local_a0,&local_174);
          local_9c = 9;
          (*pcVar22)(pSVar20,&local_a0,&local_154);
          if (local_b5 != '\0') {
            local_9c = 10;
            (*pcVar22)(pSVar20,&local_a0,&local_200);
            if (local_200 != (BSTR)0x0) {
              local_248 = FUN_0064057f(local_200);
            }
          }
          local_9c = 0xb;
          (*pcVar22)(pSVar20,&local_a0,&local_240);
          local_9c = 0xc;
          (*pcVar22)(pSVar20,&local_a0,&local_208);
          local_9c = 0xd;
          (*pcVar22)(pSVar20,&local_a0,&local_230);
          local_9c = 0xe;
          (*pcVar22)(pSVar20,&local_a0,&local_1c8);
          local_9c = 0xf;
          (*pcVar22)(pSVar20,&local_a0,&local_1cc);
          local_9c = 0x10;
          (*pcVar22)(pSVar20,&local_a0,&local_13c);
          if (((local_a9 == '\0') && (local_13c != (wchar_t *)0x0)) && (local_a8 != 0x10)) {
            pwVar14 = _wcsstr(local_13c,L".youtube.com");
            if (pwVar14 == (wchar_t *)0x0) {
              pwVar14 = _wcsstr(local_13c,L".googlevideo.com");
              if (pwVar14 == (wchar_t *)0x0) goto LAB_0042869d;
            }
            pwVar14 = _wcsstr(local_13c,L"videoplayback");
            if (pwVar14 != (wchar_t *)0x0) {
              pcVar9 = _strstr(local_98,"range=");
              if (pcVar9 == (char *)0x0) {
                pcVar9 = _strstr(local_98,"begin=");
                if (pcVar9 == (char *)0x0) {
                  pcVar9 = _strstr(local_98,"&ump=");
                  if (pcVar9 == (char *)0x0) {
                    pcVar9 = _strstr(local_98,"&len=");
                    if (pcVar9 == (char *)0x0) {
                      pcVar9 = _strstr(local_98,"?ump=");
                      if (pcVar9 == (char *)0x0) {
                        pcVar9 = _strstr(local_98,"?len=");
                        if (pcVar9 == (char *)0x0) goto LAB_0042869d;
                      }
                    }
                  }
                }
              }
              FUN_004024f0();
              local_88 = CONCAT31(local_88._1_3_,0x21);
              FUN_00402f20(&local_10c,&DAT_00696f08,local_98);
              pcVar9 = _strstr(local_98,"range=");
              if (pcVar9 != (char *)0x0) {
                FUN_00426c10(local_98,"&range=","?range=",&local_98);
                FUN_00402f20(&local_10c,&DAT_00696f08,local_98);
              }
              pcVar9 = _strstr(local_98,"begin=");
              if (pcVar9 != (char *)0x0) {
                FUN_00426c10(local_10c,"&begin=","?begin=",&local_98);
                FUN_00402f20(&local_10c,&DAT_00696f08,local_98);
              }
              pcVar9 = _strstr(local_98,"&len=");
              if (pcVar9 == (char *)0x0) {
                pcVar9 = _strstr(local_98,"?len=");
                if (pcVar9 != (char *)0x0) goto LAB_004285ff;
              }
              else {
LAB_004285ff:
                FUN_00426c10(local_10c,"&len=","?len=",&local_98);
                FUN_00402f20(&local_10c,&DAT_00696f08,local_98);
              }
              pcVar9 = _strstr(local_98,"&ump=");
              if (pcVar9 == (char *)0x0) {
                pcVar9 = _strstr(local_98,"?ump=");
                if (pcVar9 != (char *)0x0) goto LAB_0042865b;
              }
              else {
LAB_0042865b:
                FUN_00426c10(local_10c,"&ump=","?ump=",&local_98);
              }
              local_b5 = '\0';
              local_a9 = '\x01';
              local_248._0_4_ = 0;
              local_248._4_4_ = 0;
              local_88 = CONCAT31(local_88._1_3_,0x1e);
              FUN_00402560();
            }
          }
LAB_0042869d:
          local_9c = 0x11;
          (*pcVar22)(pSVar20,&local_a0,&local_17c);
          local_9c = 0x12;
          (*pcVar22)(pSVar20,&local_a0,&local_180);
          local_9c = 0x13;
          (*pcVar22)(pSVar20,&local_a0,&local_188);
          local_9c = 0x14;
          (*pcVar22)(pSVar20,&local_a0,&local_190);
          local_9c = 0x15;
          (*pcVar22)(pSVar20,&local_a0,&local_198);
        }
        FUN_004024f0();
        local_88._0_1_ = 0x22;
        iVar23 = -1;
        iVar7 = FUN_00403e50(local_98);
        local_138 = iVar7;
        iVar12 = __strnicmp("rtmpe://",local_98,8);
        if (iVar12 == 0) {
          local_e1 = '\x01';
        }
        else if ((iVar7 == 4) || (iVar7 == 5)) {
          local_204 = (BSTR)0x0;
          local_1b4 = (BSTR)0x0;
          FUN_004024f0();
          local_88._0_1_ = 0x23;
          FUN_004024f0();
          local_88._0_1_ = 0x24;
          if (0x17 < local_e8 - local_f4) {
            local_9c = 0x18;
            SafeArrayGetElement(pSVar20,&local_a0,&local_204);
            FUN_00426b90(local_204,&local_1c4);
            local_9c = 3;
            SafeArrayGetElement(pSVar20,&local_a0,&local_1b4);
            FUN_00426b90(local_1b4,&local_214);
          }
          FUN_004024f0();
          local_88._0_1_ = 0x25;
          FUN_004024f0();
          local_88 = CONCAT31(local_88._1_3_,0x26);
          FUN_00426b90(local_174,&local_1bc);
          FUN_00426b90(local_154,&local_20c);
          iVar23 = FUN_004a04c0(local_98,local_1c4,local_214,local_20c,local_1bc,0,local_c0);
          if (local_204 != (BSTR)0x0) {
            SysFreeString(local_204);
          }
          if (local_1b4 != (BSTR)0x0) {
            SysFreeString(local_1b4);
          }
          local_88._0_1_ = 0x25;
          FUN_00402560();
          local_88._0_1_ = 0x24;
          FUN_00402560();
          local_88._0_1_ = 0x23;
          FUN_00402560();
          local_88._0_1_ = 0x22;
          FUN_00402560();
        }
        else {
          local_178 = (BSTR)0x0;
          local_1ac = (BSTR)0x0;
          FUN_004024f0();
          local_88 = CONCAT31(local_88._1_3_,0x27);
          if ((2 < local_e8 - local_f4) && (0 < local_114)) {
            local_9c = 3;
            HVar6 = SafeArrayGetElement(pSVar20,&local_a0,&local_178);
            if (HVar6 == 0) {
              FUN_0040dbd0(local_178);
            }
          }
          if (0x17 < local_e8 - local_f4) {
            local_9c = 0x18;
            SafeArrayGetElement(pSVar20,&local_a0,&local_1ac);
            FUN_00426b90(local_1ac,&local_14c);
            local_9c = 0x16;
            SafeArrayGetElement(pSVar20,&local_a0,&local_1a0);
            local_9c = 0x17;
            SafeArrayGetElement(pSVar20,&local_a0,&local_284);
          }
          pcVar9 = local_b4[0];
          do {
            cVar4 = *pcVar9;
            pcVar9 = pcVar9 + 1;
          } while (cVar4 != '\0');
          pcVar11 = local_d0;
          do {
            cVar4 = *pcVar11;
            pcVar11 = pcVar11 + 1;
          } while (cVar4 != '\0');
          ppcVar16 = &local_d0;
          if (pcVar11 == local_d0 + 1) {
            ppcVar16 = &local_c4;
          }
          pcVar11 = local_14c;
          do {
            cVar4 = *pcVar11;
            pcVar11 = pcVar11 + 1;
          } while (cVar4 != '\0');
          ppcVar19 = &local_14c;
          if (pcVar11 == local_14c + 1) {
            ppcVar19 = local_b4;
          }
          iVar23 = FUN_004a0840(local_98,*ppcVar19,local_c0,0,local_bc,*ppcVar16,local_e0,local_164,
                                local_c8,local_d4,
                                ~-(uint)(pcVar9 != local_b4[0] + 1) & (uint)local_b4[0]);
          if (local_178 != (BSTR)0x0) {
            SysFreeString(local_178);
          }
          if (local_1ac != (BSTR)0x0) {
            SysFreeString(local_1ac);
          }
          local_88._0_1_ = 0x22;
          FUN_00402560();
        }
        if (0x18 < local_e8 - local_f4) {
          local_9c = 0x19;
          SafeArrayGetElement(local_108,&local_a0,&local_1a8);
          if (0x19 < local_e8 - local_f4) {
            local_9c = 0x1a;
            SafeArrayGetElement(local_108,&local_a0,&local_1b0);
          }
        }
        if (-1 < iVar23) {
          FUN_004024f0();
          local_88._0_1_ = 0x28;
          FUN_004024f0();
          local_88._0_1_ = 0x29;
          FUN_004024f0();
          local_88._0_1_ = 0x2a;
          FUN_004024f0();
          local_88._0_1_ = 0x2b;
          FUN_004024f0();
          local_88._0_1_ = 0x2c;
          FUN_004024f0();
          local_88._0_1_ = 0x2d;
          FUN_004024f0();
          local_88._0_1_ = 0x2e;
          FUN_004024f0();
          local_88._0_1_ = 0x2f;
          FUN_004024f0();
          local_88._0_1_ = 0x30;
          FUN_004024f0();
          local_88._0_1_ = 0x31;
          FUN_004024f0();
          local_88._0_1_ = 0x32;
          FUN_004024f0();
          local_88._0_1_ = 0x33;
          FUN_004024f0();
          local_88._0_1_ = 0x34;
          FUN_004024f0();
          local_88._0_1_ = 0x35;
          FUN_004024f0();
          local_88._0_1_ = 0x36;
          FUN_004024f0();
          local_88 = CONCAT31(local_88._1_3_,0x37);
          if ((local_138 != 4) && (local_138 != 5)) {
            FUN_00426b90(local_174,&local_f0);
            FUN_00426b90(local_154,&local_144);
          }
          FUN_00426b90(local_240,&local_11c);
          FUN_00426b90(local_230,&local_128);
          FUN_00426b90(local_1c8,&local_120);
          FUN_00426b90(local_1cc,&local_118);
          FUN_00426b90(local_13c,&local_a4);
          FUN_00426b90(local_1a0,&local_134);
          if (local_a9 != '\0') {
            pcVar9 = local_a4;
            do {
              cVar4 = *pcVar9;
              pcVar9 = pcVar9 + 1;
            } while (cVar4 != '\0');
            if ((pcVar9 != local_a4 + 1) && (local_a8 != 0x10)) {
              pcVar9 = _strstr(local_a4,".youtube.com");
              if (pcVar9 == (char *)0x0) {
                pcVar9 = _strstr(local_a4,".googlevideo.com");
                if (pcVar9 == (char *)0x0) goto LAB_00428e6d;
              }
              pcVar9 = _strstr(local_a4,"videoplayback");
              if (pcVar9 != (char *)0x0) {
                pcVar9 = _strstr(local_a4,"range=");
                if (pcVar9 == (char *)0x0) {
                  pcVar9 = _strstr(local_a4,"begin=");
                  if (pcVar9 == (char *)0x0) {
                    pcVar9 = _strstr(local_a4,"&ump=");
                    if (pcVar9 == (char *)0x0) {
                      pcVar9 = _strstr(local_a4,"&len=");
                      if (pcVar9 == (char *)0x0) {
                        pcVar9 = _strstr(local_a4,"?ump=");
                        if (pcVar9 == (char *)0x0) {
                          pcVar9 = _strstr(local_a4,"?len=");
                          if (pcVar9 == (char *)0x0) goto LAB_00428e6d;
                        }
                      }
                    }
                  }
                }
                FUN_004024f0();
                local_88 = CONCAT31(local_88._1_3_,0x38);
                FUN_00402f20(&local_104,&DAT_00696f08,local_a4);
                pcVar9 = _strstr(local_a4,"range=");
                if (pcVar9 != (char *)0x0) {
                  FUN_00426c10(local_a4,"&range=","?range=",&local_a4);
                  FUN_00402f20(&local_104,&DAT_00696f08,local_a4);
                }
                pcVar9 = _strstr(local_a4,"begin=");
                if (pcVar9 != (char *)0x0) {
                  FUN_00426c10(local_104,"&begin=","?begin=",&local_a4);
                  FUN_00402f20(&local_104,&DAT_00696f08,local_a4);
                }
                pcVar9 = _strstr(local_a4,"&len=");
                if (pcVar9 == (char *)0x0) {
                  pcVar9 = _strstr(local_a4,"?len=");
                  if (pcVar9 != (char *)0x0) goto LAB_00428df1;
                }
                else {
LAB_00428df1:
                  FUN_00426c10(local_104,"&len=","?len=",&local_a4);
                  FUN_00402f20(&local_104,&DAT_00696f08,local_a4);
                }
                pcVar9 = _strstr(local_a4,"&ump=");
                if (pcVar9 == (char *)0x0) {
                  pcVar9 = _strstr(local_a4,"?ump=");
                  if (pcVar9 != (char *)0x0) goto LAB_00428e47;
                }
                else {
LAB_00428e47:
                  FUN_00426c10(local_104,"&ump=","?ump=",&local_a4);
                }
                local_88 = CONCAT31(local_88._1_3_,0x37);
                FUN_00402560();
              }
            }
          }
LAB_00428e6d:
          FUN_00426b90(local_17c,&local_24c);
          FUN_00426b90(local_180,&local_1f0);
          FUN_00426b90(local_188,&local_220);
          FUN_00426b90(local_190,&local_1e8);
          FUN_00426b90(local_198,&local_238);
          FUN_00426b90(local_208,&local_148);
          FUN_00426b90(local_1a8,&local_130);
          FUN_00426b90(local_1b0,&local_110);
          piVar15 = (int *)FUN_00426030(iVar23);
          iVar7 = *piVar15;
          *(uint *)(iVar7 + 0x184) = *(uint *)(iVar7 + 0x184) | 0x80000;
          *(UINT *)(iVar7 + 0x194) = local_e0;
          if ((local_138 == 4) || (local_138 == 5)) {
            if (local_d4 != (char *)0x0) {
              pcVar9 = local_d4;
              do {
                cVar4 = *pcVar9;
                pcVar9 = pcVar9 + 1;
              } while (cVar4 != '\0');
              if (pcVar9 != local_d4 + 1) {
                iVar12 = iVar7 + 0xe4;
                pcVar9 = local_d4;
                goto LAB_0042983d;
              }
            }
          }
          else {
            if (local_a8 == 8) {
              *(undefined4 *)(iVar7 + 0x368) = 8;
            }
            *(undefined4 *)(iVar7 + 0x364) = local_158;
            if (local_a8 == 0x100) {
              *(undefined4 *)(iVar7 + 0x368) = 8;
              *(undefined4 *)(iVar7 + 0x35c) = 8;
              if (local_f0 != (char *)0x0) {
                pcVar9 = local_f0;
                do {
                  cVar4 = *pcVar9;
                  pcVar9 = pcVar9 + 1;
                } while (cVar4 != '\0');
                if (pcVar9 != local_f0 + 1) {
                  pcVar9 = local_d0;
                  do {
                    cVar4 = *pcVar9;
                    pcVar9 = pcVar9 + 1;
                  } while (cVar4 != '\0');
                  ppcVar16 = &local_d0;
                  if (pcVar9 == local_d0 + 1) {
                    ppcVar16 = &local_c4;
                  }
                  iVar12 = FUN_004a0840(local_f0,local_b4[0],local_c0,0,local_bc,*ppcVar16,local_e0,
                                        0,0,0,0);
                  if (-1 < iVar12) {
                    piVar15 = (int *)FUN_00426030(iVar12);
                    iVar12 = *piVar15;
                    if (local_134 != 0) {
                      FUN_005dcc70(local_134);
                    }
                    *(undefined4 *)(iVar12 + 0x3d4) = 1;
                    FUN_00467a90(iVar12,local_11c,local_128,local_120,local_118,0,0,0,0,0,0,
                                 local_130,local_110);
                    *(undefined4 *)(iVar12 + 0x184) = *(undefined4 *)(iVar7 + 0x184);
                    *(undefined4 *)(iVar12 + 0x35c) = 8;
                    *(undefined4 *)(iVar12 + 0x368) = 8;
                    *(undefined1 *)(iVar12 + 0x36e) = 1;
                    goto LAB_004292c0;
                  }
                }
                goto LAB_004292c9;
              }
            }
            else {
              if (local_a8 == 0x800) {
                *(undefined4 *)(iVar7 + 0x368) = 0x800;
                FUN_00402f20(iVar7 + 0x348,&DAT_00696f08,local_1b8);
                pcVar9 = local_d0;
                do {
                  cVar4 = *pcVar9;
                  pcVar9 = pcVar9 + 1;
                } while (cVar4 != '\0');
                ppcVar16 = &local_d0;
                if (pcVar9 == local_d0 + 1) {
                  ppcVar16 = &local_c4;
                }
                iVar12 = FUN_004a0840(local_98,local_b4[0],local_c0,0,local_bc,*ppcVar16,local_e0,0,
                                      0,0,0);
                if (-1 < iVar12) {
                  piVar15 = (int *)FUN_00426030(iVar12);
                  iVar12 = *piVar15;
                  FUN_005dcc70(local_134);
                  *(undefined4 *)(iVar12 + 0x3d4) = 1;
                  FUN_00467a90(iVar12,local_11c,local_128,local_120,local_118,0,0,0,0,0,0,local_130,
                               local_110);
                  uVar21 = *(uint *)(iVar7 + 0x184);
                  *(uint *)(iVar12 + 0x184) = uVar21;
                  *(uint *)(iVar12 + 0x184) = uVar21 | 0x100;
                  *(undefined4 *)(iVar12 + 0x368) = 0x800;
                  *(undefined1 *)(iVar12 + 0x36e) = 1;
                  if (local_f0 != (char *)0x0) {
                    pcVar9 = local_f0;
                    do {
                      cVar4 = *pcVar9;
                      pcVar9 = pcVar9 + 1;
                    } while (cVar4 != '\0');
                    if (pcVar9 != local_f0 + 1) {
LAB_004292b8:
                      FUN_00402f20(iVar12 + 0xf0,&DAT_00696f08,local_f0);
                    }
                  }
LAB_004292c0:
                  *(int *)(iVar7 + 0x350) = iVar12;
                }
              }
              else if (local_a8 == 0x80) {
                *(undefined4 *)(iVar7 + 0x368) = 0x80;
              }
              else if (local_a8 == 0x40) {
                *(undefined4 *)(iVar7 + 0x368) = 0x40;
                pcVar9 = local_d0;
                do {
                  cVar4 = *pcVar9;
                  pcVar9 = pcVar9 + 1;
                } while (cVar4 != '\0');
                ppcVar16 = &local_d0;
                if (pcVar9 == local_d0 + 1) {
                  ppcVar16 = &local_c4;
                }
                iVar12 = FUN_004a0840(local_98,local_b4[0],local_c0,0,local_bc,*ppcVar16,local_e0,0,
                                      0,0,0);
                if (-1 < iVar12) {
                  piVar15 = (int *)FUN_00426030(iVar12);
                  iVar12 = *piVar15;
                  FUN_005dcc70(local_134);
                  *(undefined4 *)(iVar12 + 0x3d4) = 1;
                  FUN_00467a90(iVar12,local_11c,local_128,local_120,local_118,0,0,0,0,0,0,local_130,
                               local_110);
                  uVar21 = *(uint *)(iVar7 + 0x184);
                  *(uint *)(iVar12 + 0x184) = uVar21;
                  *(uint *)(iVar12 + 0x184) = uVar21 | 0x100;
                  *(undefined4 *)(iVar12 + 0x368) = 0x40;
                  *(undefined1 *)(iVar12 + 0x36e) = 1;
                  if (local_f0 != (char *)0x0) {
                    pcVar9 = local_f0;
                    do {
                      cVar4 = *pcVar9;
                      pcVar9 = pcVar9 + 1;
                    } while (cVar4 != '\0');
                    if (pcVar9 != local_f0 + 1) goto LAB_004292b8;
                  }
                  goto LAB_004292c0;
                }
              }
LAB_004292c9:
              pcVar9 = local_f0;
              if (local_f0 != (char *)0x0) {
                pcVar11 = local_f0;
                do {
                  cVar4 = *pcVar11;
                  pcVar11 = pcVar11 + 1;
                } while (cVar4 != '\0');
                if (pcVar11 != local_f0 + 1) {
                  if ((local_a8 == 4) || (local_a8 == 0x10)) {
                    local_288 = local_f0;
                    iVar12 = _strncmp("pdmt=",local_f0,5);
                    pcVar11 = pcVar9;
                    if (iVar12 == 0) {
                      pcVar10 = _strchr(pcVar9,0x2c);
                      if (pcVar10 != (char *)0x0) {
                        *pcVar10 = '\0';
                        pcVar11 = pcVar10 + 1;
                        local_288 = pcVar11;
                        FUN_0057f8f0(local_2a0);
                        local_2a8 = 0;
                        iVar12 = FUN_00580790(pcVar9 + 5,local_2a0,&local_2a8);
                        if (iVar12 != 0) {
                          FUN_00402f20(iVar7 + 0xf0,&DAT_00696f08,local_2a0[0]);
                        }
                        FUN_0057f910(local_2a0);
                      }
                    }
                    FUN_00402f20(iVar7 + 0x34c,&DAT_00696f08,pcVar11);
                    *(undefined4 *)(iVar7 + 0x35c) = 4;
                    if (local_278 != 0) {
                      *(uint *)(iVar7 + 0x184) = *(uint *)(iVar7 + 0x184) | 0x10000;
                    }
                    if (local_280 != 0) {
                      *(uint *)(iVar7 + 0x184) = *(uint *)(iVar7 + 0x184) | 0x20000;
                    }
                    if (local_a8 == 0x10) {
                      FUN_004024f0();
                      local_88._0_1_ = 0x39;
                      FUN_004703a0(pcVar11,&DAT_0069c7b4,&local_224,1);
                      *(undefined4 *)(iVar7 + 0x368) = 0x10;
                      FUN_00402f20(iVar7 + 0x348,&DAT_00696f08,local_224);
                      local_88 = CONCAT31(local_88._1_3_,0x37);
                      FUN_00402560();
                    }
                    FUN_004024f0();
                    local_88._0_1_ = 0x3a;
                    FUN_004703a0(pcVar11,&DAT_0069c7b0,&local_ec,1);
                    FUN_004024f0();
                    local_88 = CONCAT31(local_88._1_3_,0x3b);
                    cVar4 = FUN_00426da0(local_ec,"signature",&local_fc);
                    if (cVar4 == '\0') {
                      cVar4 = FUN_00426da0(local_ec,&DAT_0069c85c,&local_fc);
                      if (cVar4 != '\0') {
                        pcVar9 = local_fc + 1;
                        pcVar11 = local_fc;
                        do {
                          cVar4 = *pcVar11;
                          pcVar11 = pcVar11 + 1;
                        } while (cVar4 != '\0');
                        for (iVar12 = 0; local_298 = iVar12, iVar12 < (int)pcVar11 - (int)pcVar9;
                            iVar12 = iVar12 + 1) {
                          uVar5 = FUN_004149b0(iVar12);
                          iVar23 = FUN_00425fb0(uVar5);
                          if (iVar23 == 0) {
                            cVar4 = FUN_004149b0(iVar12);
                            if (cVar4 != '.') goto LAB_004295ca;
                          }
                        }
                        FUN_00402c00("?sig=","?signature=");
                        pcVar11 = "&signature=";
                        pcVar9 = "&sig=";
LAB_004295c2:
                        FUN_00402c00(pcVar9,pcVar11);
                      }
                    }
                    else {
                      pcVar9 = local_fc + 1;
                      pcVar11 = local_fc;
                      do {
                        cVar4 = *pcVar11;
                        pcVar11 = pcVar11 + 1;
                      } while (cVar4 != '\0');
                      for (iVar12 = 0; local_2a4 = iVar12, iVar12 < (int)pcVar11 - (int)pcVar9;
                          iVar12 = iVar12 + 1) {
                        uVar5 = FUN_004149b0(iVar12);
                        iVar23 = FUN_00425fb0(uVar5);
                        if (iVar23 == 0) {
                          cVar4 = FUN_004149b0(iVar12);
                          if (cVar4 != '.') {
                            FUN_00402c00("?signature=","?sig=");
                            pcVar11 = "&sig=";
                            pcVar9 = "&signature=";
                            goto LAB_004295c2;
                          }
                        }
                      }
                    }
LAB_004295ca:
                    pcVar9 = local_d0;
                    do {
                      cVar4 = *pcVar9;
                      pcVar9 = pcVar9 + 1;
                    } while (cVar4 != '\0');
                    ppcVar16 = &local_d0;
                    if (pcVar9 == local_d0 + 1) {
                      ppcVar16 = &local_c4;
                    }
                    iVar12 = FUN_004a0840(local_ec,local_b4[0],local_c0,0,local_bc,*ppcVar16,
                                          local_e0,0,0,0,0);
                    if (-1 < iVar12) {
                      piVar15 = (int *)FUN_00426030(iVar12);
                      iVar12 = *piVar15;
                      FUN_005dcc70(local_134);
                      *(undefined4 *)(iVar12 + 0x3d4) = 1;
                      FUN_00467a90(iVar12,local_11c,local_128,local_120,local_118,0,0,0,0,0,0,
                                   local_130,local_110);
                      uVar21 = *(uint *)(iVar7 + 0x184);
                      *(uint *)(iVar12 + 0x184) = uVar21;
                      *(uint *)(iVar12 + 0x184) = uVar21 | 0x100;
                      *(undefined4 *)(iVar12 + 0x35c) = 4;
                      *(undefined1 *)(iVar12 + 0x36e) = 1;
                      if (local_a8 == 0x10) {
                        FUN_004024f0();
                        local_88._0_1_ = 0x3c;
                        FUN_004703a0(local_288,&DAT_0069c7a8,&local_1d0,1);
                        *(undefined4 *)(iVar12 + 0x368) = 0x10;
                        FUN_00402f20(iVar12 + 0x348,&DAT_00696f08,local_1d0);
                        local_274 = 0;
                        uVar17 = FUN_00457940(local_98);
                        iVar23 = FUN_004579a0(uVar17);
                        if (iVar23 != 0) {
                          local_274 = 1;
                        }
                        uVar21 = (uint)(iVar23 != 0);
                        uVar17 = FUN_00457940(local_ec);
                        iVar23 = FUN_004579a0(uVar17);
                        if (iVar23 != 0) {
                          uVar21 = uVar21 + 1;
                          local_274 = uVar21;
                        }
                        if (uVar21 != 0) {
                          *(uint *)(iVar7 + 0x184) = *(uint *)(iVar7 + 0x184) | 0x10000;
                        }
                        if (uVar21 == 2) {
                          *(uint *)(iVar7 + 0x184) = *(uint *)(iVar7 + 0x184) | 0x20000;
                        }
                        local_88 = CONCAT31(local_88._1_3_,0x3b);
                        FUN_00402560();
                      }
                      *(int *)(iVar7 + 0x350) = iVar12;
                    }
                    local_88._0_1_ = 0x3a;
                    FUN_00402560();
                    local_88 = CONCAT31(local_88._1_3_,0x37);
                    FUN_00402560();
                  }
                  else if (local_a8 == 2) {
                    FUN_00402f20(iVar7 + 0x348,&DAT_00696f08,local_f0);
                    *(undefined4 *)(iVar7 + 0x368) = 2;
                    *(uint *)(iVar7 + 0x184) = *(uint *)(iVar7 + 0x184) | 0x8000;
                  }
                  else if (local_a8 != 0x100) {
                    FUN_00402f20(iVar7 + 0xf0,&DAT_00696f08,local_f0);
                  }
                }
              }
            }
            if (local_144 != (char *)0x0) {
              pcVar9 = local_144;
              do {
                cVar4 = *pcVar9;
                pcVar9 = pcVar9 + 1;
              } while (cVar4 != '\0');
              if ((pcVar9 != local_144 + 1) && (local_a8 != 0x10)) {
                pcVar9 = _strstr(local_98,".youtube.com");
                if (pcVar9 == (char *)0x0) {
                  pcVar9 = _strstr(local_98,".googlevideo.com");
                  if (pcVar9 != (char *)0x0) goto LAB_004297d7;
                }
                else {
LAB_004297d7:
                  pcVar9 = _strstr(local_98,"videoplayback");
                  if (pcVar9 != (char *)0x0) {
                    iVar12 = __stricmp(local_144,"text/plain");
                    if (iVar12 == 0) goto LAB_00429845;
                  }
                }
                iVar12 = iVar7 + 0x140;
                pcVar9 = local_144;
LAB_0042983d:
                FUN_00402f20(iVar12,&DAT_00696f08,pcVar9);
              }
            }
          }
LAB_00429845:
          FUN_005dcc70(local_134);
          *(undefined4 *)(iVar7 + 0x3d4) = 1;
          FUN_00467a90(iVar7,local_11c,local_128,local_120,local_118,local_a4,local_24c,local_1f0,
                       local_220,local_1e8,local_238,local_130,local_110);
          if (local_148 != (char *)0x0) {
            pcVar9 = local_148;
            do {
              cVar4 = *pcVar9;
              pcVar9 = pcVar9 + 1;
            } while (cVar4 != '\0');
            if (pcVar9 != local_148 + 1) {
              puVar1 = (undefined4 *)(iVar7 + 0x154);
              FUN_00402f20(puVar1,&DAT_00696f08,local_148);
              FUN_00402f20(iVar7 + 0x158,&DAT_00696f08,local_148);
              FUN_00426430();
              iVar12 = __stricmp(local_148,"idmbrbtn.dll");
              if (iVar12 == 0) {
                *(uint *)(iVar7 + 0x184) = *(uint *)(iVar7 + 0x184) | 0x100;
                *(undefined1 *)(iVar7 + 0x199) = 1;
              }
              else {
                pcVar9 = _strrchr((char *)*puVar1,0x7c);
                if (pcVar9 != (char *)0x0) {
                  iVar12 = FID_conflict__sscanf
                                     (pcVar9 + 1,"%ld:%ld:%ld:%ld:%ld:%ld",local_2b8,local_2bc,
                                      local_2b4,local_2b0,local_2ac,&local_268);
                  if (iVar12 == 6) {
                    if ((local_268 & 0x100) != 0) {
                      *(uint *)(iVar7 + 0x184) = *(uint *)(iVar7 + 0x184) | 0x100;
                      *(uint *)(iVar7 + 0x184) = *(uint *)(iVar7 + 0x184) | 4;
                      *(undefined1 *)(iVar7 + 0x199) = 1;
                    }
                    if ((local_268 & 0x100000) != 0) {
                      *(uint *)(iVar7 + 0x184) = *(uint *)(iVar7 + 0x184) | 0x100000;
                    }
                  }
                  pcVar11 = _strstr((char *)*puVar1,"firefox.exe|");
                  if (pcVar11 == (char *)0x0) {
                    pcVar11 = _strstr((char *)*puVar1,"Firefox.exe|");
                    if (pcVar11 == (char *)0x0) goto LAB_004299ff;
                  }
                  *pcVar9 = '\0';
                }
              }
            }
          }
LAB_004299ff:
          *(longlong *)(iVar7 + 0x160) = local_248;
          if (0 < local_248) {
            *(undefined1 *)(iVar7 + 0x370) = 0;
          }
          if ((*(uint *)(iVar7 + 0x184) & 0x100) != 0) {
            pcVar9 = _strstr(*(char **)(iVar7 + 0x28),"&bytes=");
            if (pcVar9 != (char *)0x0) {
              FUN_004024f0();
              local_88 = CONCAT31(local_88._1_3_,0x3d);
              FUN_00467f20(*(undefined4 *)(iVar7 + 0x20),&local_140);
              local_d9 = 0;
              iVar12 = __stricmp(local_140,"mycdn.me");
              if (iVar12 == 0) {
LAB_00429ac4:
                local_d9 = 1;
                pcVar9 = _strstr(*(char **)(iVar7 + 0x28),"&bytes=");
                if (pcVar9 != (char *)0x0) {
                  iVar12 = FID_conflict__sscanf(pcVar9 + 7,"%I64d-%I64d",&local_25c,&local_16c);
                  if (iVar12 == 2) {
                    if (((local_25c != 0 || local_258 != 0) ||
                        ((-1 < local_168 && ((0 < local_168 || (0x200000 < local_16c)))))) &&
                       (*(int *)(iVar7 + 0x28) != 0)) {
                      FUN_004024f0();
                      local_88._0_1_ = 0x3e;
                      FUN_00402f20(&local_218,"&bytes=%I64d-%I64d",local_25c,local_258,local_16c,
                                   local_168);
                      FUN_004024f0();
                      local_88._0_1_ = 0x3f;
                      local_25c = 0;
                      local_258 = 0;
                      if ((-1 < local_168) && ((0 < local_168 || (0x200000 < local_16c)))) {
                        local_16c = 0x200000;
                        local_168 = 0;
                      }
                      FUN_00402f20(&local_1d8,"&bytes=%I64d-%I64d",0,0,local_16c,local_168);
                      FUN_004030a0(*(undefined4 *)(iVar7 + 0x28));
                      local_88._0_1_ = 0x40;
                      FUN_00402c00(local_218,local_1d8);
                      FUN_00425d10(local_254,0);
                      local_88._0_1_ = 0x3f;
                      FUN_00402560();
                      local_88._0_1_ = 0x3e;
                      FUN_00402560();
                      local_88 = CONCAT31(local_88._1_3_,0x3d);
                      FUN_00402560();
                    }
                    *(undefined4 *)(iVar7 + 0x368) = 0x20;
                    *(undefined4 *)(iVar7 + 0x160) = 0;
                    *(undefined4 *)(iVar7 + 0x164) = 0;
                  }
                }
              }
              else if (local_b4[0] != (char *)0x0) {
                FUN_00467f20(local_b4[0],&local_140);
                iVar12 = __stricmp(local_140,"mycdn.me");
                if (iVar12 == 0) goto LAB_00429ac4;
              }
              local_88 = CONCAT31(local_88._1_3_,0x37);
              FUN_00402560();
            }
          }
          if (local_124 == (LPCWSTR)0x0) {
LAB_00429d41:
            if ((*(uint *)(iVar7 + 0x184) & 0x100) != 0) {
              pcVar9 = local_c8;
              do {
                cVar4 = *pcVar9;
                pcVar9 = pcVar9 + 1;
              } while (cVar4 != '\0');
              if (pcVar9 != local_c8 + 1) {
                FUN_004263f0(local_c8);
                local_88._0_1_ = 0x42;
                cVar4 = FUN_00455bc0(local_c8);
                pOVar13 = local_264;
                pcVar9 = "audio.%s";
                if (cVar4 == '\0') {
                  pcVar9 = "video.%s";
                }
                FUN_004021a0(pcVar9,local_264,0);
                cVar4 = FUN_00455bc0(local_c8);
                pcVar9 = "audio.%s";
                if (cVar4 == '\0') {
                  pcVar9 = "video.%s";
                }
                FUN_00402f20(iVar7 + 0xe4,pcVar9,local_c8);
                *(uint *)(iVar7 + 0x184) = *(uint *)(iVar7 + 0x184) | 0x2000;
                *(uint *)(iVar7 + 0x184) = *(uint *)(iVar7 + 0x184) | 0x40;
                local_88 = CONCAT31(local_88._1_3_,0x37);
                SysFreeString(pOVar13);
              }
            }
          }
          else {
            pWVar18 = local_124;
            do {
              WVar2 = *pWVar18;
              pWVar18 = pWVar18 + 1;
            } while (WVar2 != L'\0');
            if ((int)pWVar18 - (int)(local_124 + 1) >> 1 == 0) goto LAB_00429d41;
            FUN_004017e0(local_124);
            *(uint *)(iVar7 + 0x184) = *(uint *)(iVar7 + 0x184) | 0x40;
            iVar12 = WideCharToMultiByte(0xfde9,0,local_124,-1,(LPSTR)0x0,0,(LPCSTR)0x0,(LPBOOL)0x0)
            ;
            if (iVar12 != 0) {
              FUN_004024f0();
              local_88 = CONCAT31(local_88._1_3_,0x41);
              pCVar8 = (LPSTR)FUN_00402840(iVar12);
              iVar12 = WideCharToMultiByte(0xfde9,0,local_124,-1,pCVar8,iVar12,(LPCSTR)0x0,
                                           (LPBOOL)0x0);
              if (iVar12 != 0) {
                FUN_00402f20(iVar7 + 0xe4,&DAT_00696f08,local_1e0);
                *(undefined1 *)(iVar7 + 0x375) = 1;
              }
              local_88 = CONCAT31(local_88._1_3_,0x37);
              FUN_00402560();
            }
          }
          local_88._0_1_ = 0x36;
          FUN_00402560();
          local_88._0_1_ = 0x35;
          FUN_00402560();
          local_88._0_1_ = 0x34;
          FUN_00402560();
          local_88._0_1_ = 0x33;
          FUN_00402560();
          local_88._0_1_ = 0x32;
          FUN_00402560();
          local_88._0_1_ = 0x31;
          FUN_00402560();
          local_88._0_1_ = 0x30;
          FUN_00402560();
          local_88._0_1_ = 0x2f;
          FUN_00402560();
          local_88._0_1_ = 0x2e;
          FUN_00402560();
          local_88._0_1_ = 0x2d;
          FUN_00402560();
          local_88._0_1_ = 0x2c;
          FUN_00402560();
          local_88._0_1_ = 0x2b;
          FUN_00402560();
          local_88._0_1_ = 0x2a;
          FUN_00402560();
          local_88._0_1_ = 0x29;
          FUN_00402560();
          local_88._0_1_ = 0x28;
          FUN_00402560();
          local_88._0_1_ = 0x22;
          FUN_00402560();
        }
        pcVar22 = SysFreeString_exref;
        SysFreeString(local_12c);
        if (local_27c != (BSTR)0x0) {
          SysFreeString(local_27c);
        }
        if (local_1f8 != (BSTR)0x0) {
          SysFreeString(local_1f8);
        }
        if (local_160 != (BSTR)0x0) {
          SysFreeString(local_160);
        }
        if (local_124 != (BSTR)0x0) {
          SysFreeString(local_124);
        }
        if (local_228 != (BSTR)0x0) {
          SysFreeString(local_228);
        }
        if (local_174 != (BSTR)0x0) {
          SysFreeString(local_174);
        }
        if (local_154 != (BSTR)0x0) {
          SysFreeString(local_154);
        }
        if (local_200 != (BSTR)0x0) {
          SysFreeString(local_200);
        }
        if (local_240 != (BSTR)0x0) {
          SysFreeString(local_240);
        }
        if (local_208 != (BSTR)0x0) {
          SysFreeString(local_208);
        }
        if (local_230 != (BSTR)0x0) {
          SysFreeString(local_230);
        }
        if (local_1c8 != (BSTR)0x0) {
          SysFreeString(local_1c8);
        }
        if (local_1cc != (BSTR)0x0) {
          SysFreeString(local_1cc);
        }
        if (local_13c != (BSTR)0x0) {
          SysFreeString(local_13c);
        }
        if (local_17c != (BSTR)0x0) {
          SysFreeString(local_17c);
        }
        if (local_180 != (BSTR)0x0) {
          SysFreeString(local_180);
        }
        if (local_188 != (BSTR)0x0) {
          SysFreeString(local_188);
        }
        if (local_190 != (BSTR)0x0) {
          SysFreeString(local_190);
        }
        if (local_198 != (BSTR)0x0) {
          SysFreeString(local_198);
        }
        if (local_1a0 != (BSTR)0x0) {
          SysFreeString(local_1a0);
        }
        if (local_284 != (BSTR)0x0) {
          SysFreeString(local_284);
        }
        if (local_1a8 != (BSTR)0x0) {
          SysFreeString(local_1a8);
        }
        if (local_1b0 != (BSTR)0x0) {
          SysFreeString(local_1b0);
        }
        local_88._0_1_ = 0x1e;
        FUN_00402560();
        local_88._0_1_ = 10;
        FUN_00402560();
        uVar17 = local_164;
      }
      local_88._0_1_ = 9;
      (*pcVar22)(uVar17);
      local_88._0_1_ = 8;
      FUN_00402560();
      local_88._0_1_ = 7;
      FUN_00402560();
      local_88._0_1_ = 6;
      FUN_00402560();
      local_88._0_1_ = 5;
      FUN_00402560();
      local_88 = CONCAT31(local_88._1_3_,4);
      FUN_00402560();
      piVar15 = local_cc;
    }
    if (local_1c0 != (BSTR)0x0) {
      SysFreeString(local_1c0);
    }
    if (local_15c != (BSTR)0x0) {
      SysFreeString(local_15c);
    }
    if (*(int *)(DAT_007890a4 + 0x20) == 0) {
      while (DAT_00788d00 == 0) {
        if (*(int *)(DAT_007890a4 + 0x20) != 0) goto LAB_0042a0f0;
        FUN_00425ed0(0x32);
      }
    }
    else {
LAB_0042a0f0:
      if (piVar15[2] == 0) {
        (**(code **)(*piVar15 + 4))(1);
        if (local_e1 != '\0') {
          AfxBeginThread(FUN_004264a0,(void *)0x0,0,0,0,(_SECURITY_ATTRIBUTES *)0x0);
        }
      }
      else {
        PostMessageA(*(HWND *)(DAT_007890a4 + 0x20),0x111,0x13f5,(LPARAM)piVar15);
      }
    }
    local_88._0_1_ = 3;
    FUN_00402560();
    local_88._0_1_ = 1;
    FUN_00402560();
    local_88 = (uint)local_88._1_3_ << 8;
    FUN_00402560();
  }
LAB_00427013:
  ExceptionList = local_90;
  __security_check_cookie(local_8 ^ (uint)&local_84);
  return;
}


