// Function: FUN_1002a250
// Entry Point: 0x1002a250
// Program: idmcchandler7.dll


void __thiscall FUN_1002a250(void *this,int param_1)

{
  char cVar1;
  uint uVar2;
  char *pcVar3;
  int iVar4;
  int *piVar5;
  char *pcVar6;
  LPCSTR pCVar7;
  UINT UVar8;
  undefined1 *puVar9;
  int iVar10;
  int iVar11;
  int *piVar12;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 uVar13;
  BSTR bstrString;
  BSTR pOStack_2b0;
  int *piStack_2ac;
  int **local_2a8;
  int *piStack_2a4;
  wchar_t *pwStack_2a0;
  uint uStack_29c;
  BSTR local_298;
  undefined1 *puStack_294;
  int iStack_290;
  int iStack_28c;
  int iStack_288;
  undefined *puStack_284;
  int iStack_280;
  int iStack_27c;
  char *pcStack_278;
  int iStack_274;
  int iStack_270;
  int iStack_26c;
  int iStack_268;
  int iStack_264;
  int iStack_260;
  int iStack_25c;
  wchar_t *pwStack_258;
  uint uStack_254;
  int iStack_250;
  int iStack_24c;
  LPCWSTR pWStack_248;
  void *pvStack_244;
  int iStack_240;
  int iStack_23c;
  char *pcStack_238;
  int iStack_234;
  int iStack_230;
  int iStack_22c;
  int iStack_228;
  int iStack_224;
  int iStack_220;
  int iStack_21c;
  int iStack_218;
  int iStack_214;
  char *pcStack_210;
  int iStack_20c;
  int iStack_208;
  int *piStack_204;
  uint *puStack_200;
  int iStack_1fc;
  LPCWSTR pWStack_1f8;
  int iStack_1f4;
  wchar_t *pwStack_1f0;
  int iStack_1ec;
  undefined1 auStack_1e8 [16];
  undefined1 auStack_1d8 [16];
  undefined1 auStack_1c8 [16];
  undefined1 auStack_1b8 [16];
  undefined1 auStack_1a8 [4];
  undefined1 auStack_1a4 [4];
  undefined1 auStack_1a0 [4];
  undefined1 auStack_19c [4];
  undefined1 auStack_198 [4];
  undefined1 auStack_194 [4];
  undefined1 auStack_190 [4];
  undefined1 auStack_18c [4];
  undefined1 auStack_188 [4];
  undefined1 auStack_184 [4];
  undefined1 auStack_180 [4];
  undefined1 auStack_17c [4];
  undefined1 auStack_178 [4];
  undefined1 auStack_174 [4];
  undefined1 auStack_170 [4];
  undefined1 auStack_16c [4];
  undefined1 auStack_168 [4];
  undefined1 auStack_164 [4];
  undefined1 auStack_160 [4];
  undefined1 auStack_15c [4];
  undefined **ppuStack_158;
  char *pcStack_154;
  int iStack_150;
  char acStack_14c [4];
  undefined **ppuStack_148;
  char *pcStack_144;
  int iStack_140;
  char acStack_13c [4];
  undefined **ppuStack_138;
  char *pcStack_134;
  int iStack_130;
  char acStack_12c [4];
  undefined **ppuStack_128;
  char *pcStack_124;
  int iStack_120;
  char acStack_11c [4];
  undefined **ppuStack_118;
  undefined1 *puStack_114;
  int iStack_110;
  undefined1 auStack_10c [4];
  undefined **ppuStack_108;
  undefined1 *puStack_104;
  int iStack_100;
  undefined1 auStack_fc [4];
  undefined **ppuStack_f8;
  char *pcStack_f4;
  int iStack_f0;
  char acStack_ec [4];
  undefined **ppuStack_e8;
  LPCSTR pCStack_e4;
  int iStack_e0;
  CHAR aCStack_dc [4];
  int local_d8 [38];
  int iStack_40;
  int iStack_38;
  char acStack_30 [32];
  uint local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_1004954f;
  local_c = ExceptionList;
  local_10 = DAT_1005a88c ^ (uint)&pOStack_2b0;
  uVar2 = DAT_1005a88c ^ (uint)&stack0xfffffd40;
  ExceptionList = &local_c;
  if ((*(int *)((int)this + 0x148) == 0 && *(int *)((int)this + 0x14c) == 0) && (param_1 == 0)) {
    FUN_10017f80((undefined4 *)((int)this + 0x228),&DAT_1004f450,1);
    *(int *)((int)this + 0x22c) = *(int *)((int)this + 0x22c) + -1;
    pcVar3 = strstr(*(char **)((int)this + 0x228),"#EXT-X-ENDLIST");
    uVar13 = extraout_EDX;
    if (pcVar3 == (char *)0x0) goto LAB_1002b2c6;
  }
  *(undefined4 *)((int)this + 0x1ac) = 1;
  FUN_1003f4d0(local_d8);
  local_4 = 0;
  iVar4 = FUN_1003f5b0(local_d8,*(undefined4 *)((int)this + 0x22c),*(void **)((int)this + 0x228),
                       *(undefined4 *)((int)this + 0x22c));
  if (iVar4 != 0) {
    local_2a8 = (int **)0x0;
    (*DAT_1005abf0)((int)this + 0x18,&local_298,0,uVar2);
    iVar4 = FUN_100164f0((char *)local_298,"ts",(uint *)&pcStack_278,&local_2a8);
    if (iVar4 != 0) {
      FUN_10018fa0(&piStack_2ac);
      local_4._0_1_ = 1;
      FUN_10018fa0(&piStack_2a4);
      piVar12 = piStack_2ac;
      local_4._0_1_ = 2;
      piStack_2ac = (int *)0x0;
      if (piVar12 != (int *)0x0) {
        (**(code **)(*piVar12 + 8))(piVar12);
      }
      uStack_29c = (*DAT_1005abb4)(&piStack_2ac);
      if ((-1 < (int)uStack_29c) && (piStack_2ac != (int *)0x0)) {
        piStack_204 = piStack_2ac;
        puStack_200 = &uStack_29c;
        FUN_10005b20(&pOStack_2b0,&piStack_204);
        piVar12 = piStack_2ac;
        local_4._0_1_ = 3;
        if (((uStack_29c & 0x80000000) == 0) && (pOStack_2b0 != (BSTR)0x0)) {
          iVar4 = *piStack_2ac;
          local_2a8 = &piStack_2a4;
          piVar5 = FUN_1001c6e0(&local_2a8);
          uStack_29c = (**(code **)(iVar4 + 0x10))
                                 (piVar12,"@mozilla.org/cookiemanager;1",&DAT_1004d6b8,piVar5);
        }
        local_4._0_1_ = 2;
        FUN_10017f20((int *)&pOStack_2b0);
      }
      local_2a8 = (int **)0x1;
      uStack_254 = (uint)(iStack_40 == 1);
      iStack_25c = 1;
      iStack_290 = 0;
      pcStack_f4 = (char *)0x0;
      iStack_f0 = 0;
      acStack_ec[0] = '\0';
      ppuStack_f8 = MyStrClass::vftable;
      ppuStack_148 = MyStrClass::vftable;
      pcStack_144 = (char *)0x0;
      iStack_140 = 0;
      acStack_13c[0] = '\0';
      ppuStack_118 = MyStrClass::vftable;
      puStack_114 = (undefined1 *)0x0;
      iStack_110 = 0;
      auStack_10c[0] = 0;
      ppuStack_108 = MyStrClass::vftable;
      puStack_104 = (undefined1 *)0x0;
      iStack_100 = 0;
      auStack_fc[0] = 0;
      ppuStack_128 = MyStrClass::vftable;
      pcStack_124 = (char *)0x0;
      iStack_120 = 0;
      acStack_11c[0] = '\0';
      ppuStack_138 = MyStrClass::vftable;
      pcStack_134 = (char *)0x0;
      iStack_130 = 0;
      acStack_12c[0] = '\0';
      local_4 = CONCAT31(local_4._1_3_,9);
      iVar4 = FUN_1003eb40(local_d8,&ppuStack_f8,&ppuStack_148,&ppuStack_108,&ppuStack_118,
                           &ppuStack_128);
      while (local_4._1_3_ = (uint3)((uint)local_4 >> 8), iVar4 != 0) {
        ppuStack_158 = MyStrClass::vftable;
        pcStack_154 = (char *)0x0;
        iStack_150 = 0;
        acStack_14c[0] = '\0';
        local_4._0_1_ = 10;
        if ((pcStack_f4 == (char *)0x0) || (pcVar3 = pcStack_f4, iStack_f0 == 0)) {
          pcVar3 = acStack_ec;
        }
        (*DAT_1005abf0)((int)this + 0x18,&pcStack_238,0);
        cVar1 = FUN_1001af50(pcStack_238,pcVar3,1,&ppuStack_158);
        if (cVar1 != '\0') {
          if ((pcStack_154 == (char *)0x0) || (pcVar3 = pcStack_154, iStack_150 == 0)) {
            pcVar3 = acStack_14c;
          }
          iVar4 = strncmp(pcVar3,"https://strm.yandex.ru",0x16);
          if (iVar4 == 0) {
            if ((pcStack_134 == (char *)0x0) || (pcVar3 = pcStack_134, iStack_130 == 0)) {
              pcVar3 = acStack_12c;
            }
            pcVar6 = pcVar3 + 1;
            do {
              cVar1 = *pcVar3;
              pcVar3 = pcVar3 + 1;
            } while (cVar1 != '\0');
            if (pcVar3 != pcVar6) {
              if ((pcStack_134 == (char *)0x0) || (pcVar3 = pcStack_134, iStack_130 == 0)) {
                pcVar3 = acStack_12c;
              }
              if ((pcStack_154 == (char *)0x0) || (pcVar6 = pcStack_154, iStack_150 == 0)) {
                pcVar6 = acStack_14c;
              }
              pcVar3 = strstr(pcVar6,pcVar3);
              if ((pcStack_154 == (char *)0x0) || (pcVar6 = pcStack_154, iStack_150 == 0)) {
                pcVar6 = acStack_14c;
              }
              if (pcVar3 == pcVar6) goto LAB_1002b167;
            }
            if ((pcStack_154 == (char *)0x0) || (pcVar3 = pcStack_154, iStack_150 == 0)) {
              pcVar3 = acStack_14c;
            }
            FUN_10001950(&ppuStack_138,pcVar3);
          }
          FUN_10003c00(&pwStack_2a0,"hlstoidm://");
          pOStack_2b0 = (BSTR)0x0;
          local_4._0_1_ = 0xc;
          if (iStack_140 != 0) {
            iStack_290 = 2;
            FUN_10003c80(&pwStack_2a0,"hls2toidm://");
            ppuStack_e8 = MyStrClass::vftable;
            pCStack_e4 = (LPCSTR)0x0;
            iStack_e0 = 0;
            aCStack_dc[0] = '\0';
            local_4 = CONCAT31(local_4._1_3_,0xd);
            if ((pcStack_144 == (char *)0x0) || (pcVar3 = pcStack_144, iStack_140 == 0)) {
              pcVar3 = acStack_13c;
            }
            (*DAT_1005abf0)((int)this + 0x18,&pcStack_210,0);
            cVar1 = FUN_1001af50(pcStack_210,pcVar3,1,&ppuStack_e8);
            if (cVar1 != '\0') {
              if ((pCStack_e4 == (LPCSTR)0x0) || (pCVar7 = pCStack_e4, iStack_e0 == 0)) {
                pCVar7 = aCStack_dc;
              }
              FUN_10003c80(&pOStack_2b0,pCVar7);
            }
            if ((uStack_254 != 0) && (iStack_25c != 0)) {
              iStack_25c = 0;
            }
            local_4._0_1_ = 0xc;
            ppuStack_e8 = MyStrClass::vftable;
            if (pCStack_e4 != (LPCSTR)0x0) {
              operator_delete(pCStack_e4);
            }
          }
          if ((pcStack_154 == (char *)0x0) || (pCVar7 = pcStack_154, iStack_150 == 0)) {
            pCVar7 = acStack_14c;
          }
          FUN_10003c00(&local_298,pCVar7);
          bstrString = local_298;
          local_4._0_1_ = 0xe;
          UVar8 = SysStringLen(local_298);
          FUN_10003b80(&pwStack_2a0,bstrString,UVar8);
          (*DAT_1005abe4)(auStack_1e8);
          local_4 = CONCAT31(local_4._1_3_,0xf);
          if ((puStack_104 == (undefined1 *)0x0) || (puVar9 = puStack_104, iStack_100 == 0)) {
            puVar9 = auStack_fc;
          }
          (*DAT_1005abfc)(auStack_1e8,puVar9,0xffffffff);
          if (iStack_120 != 0) {
            pcVar3 = pcStack_124;
            if (pcStack_124 == (char *)0x0) {
              pcVar3 = acStack_11c;
            }
            iVar4 = sscanf(pcVar3,"%ld",&iStack_1f4);
            if (iVar4 == 1) {
              _snprintf(acStack_30,0x20," %ld",
                        (int)(iStack_1f4 + (iStack_1f4 >> 0x1f & 0x3ffU)) >> 10);
              (*DAT_1005ac00)(auStack_1e8,0xffffffff,0," bitrate ",0xffffffff);
              (*DAT_1005ac00)(auStack_1e8,0xffffffff,0,acStack_30,0xffffffff);
            }
          }
          if (((uStack_29c & 0x80000000) == 0) && (piStack_2a4 != (int *)0x0)) {
            if (uStack_254 == 0) {
              iVar4 = (*DAT_1005abcc)((int)this + 0x38,auStack_16c,0);
              if (iVar4 != 0) goto LAB_1002aae1;
              (*DAT_1005abe4)(auStack_1d8);
              local_4 = CONCAT31(local_4._1_3_,0x12);
              if ((pcStack_154 == (char *)0x0) || (pcVar3 = pcStack_154, iStack_150 == 0)) {
                pcVar3 = acStack_14c;
              }
              iVar4 = FUN_100251a0(piStack_2a4,pcVar3,auStack_1d8);
              if ((iVar4 != 0) && (iVar4 = (*DAT_1005abf0)(auStack_1d8,auStack_164,0), iVar4 != 0))
              {
                (*DAT_1005ac08)(auStack_1d8,0,(int)this + 0x38);
              }
              puVar9 = auStack_1d8;
            }
            else {
              (*DAT_1005abe4)(auStack_1b8);
              local_4._0_1_ = 0x10;
              if ((pcStack_154 == (char *)0x0) || (pcVar3 = pcStack_154, iStack_150 == 0)) {
                pcVar3 = acStack_14c;
              }
              iVar4 = FUN_100251a0(piStack_2a4,pcVar3,auStack_1b8);
              if ((iVar4 != 0) && (iVar4 = (*DAT_1005abf0)(auStack_1b8,auStack_17c,0), iVar4 != 0))
              {
                (*DAT_1005abc0)(auStack_1c8);
                local_4 = CONCAT31(local_4._1_3_,0x11);
                (*DAT_1005ac08)(auStack_1b8,0,auStack_1c8);
                iVar4 = (*DAT_1005abcc)(auStack_1c8,auStack_1a4,0);
                if (iVar4 != 0) {
                  FUN_100038d0((void *)((int)this + 0x38),auStack_1c8);
                }
                local_4._0_1_ = 0x10;
                (*DAT_1005abc8)(auStack_1c8);
              }
              puVar9 = auStack_1b8;
            }
            local_4 = CONCAT31(local_4._1_3_,0xf);
            (*DAT_1005abec)(puVar9);
          }
LAB_1002aae1:
          pvStack_244 = operator_new(0x2f8);
          local_4._0_1_ = 0x13;
          if (pvStack_244 == (void *)0x0) {
            piVar12 = (int *)0x0;
          }
          else {
            iVar4 = (*DAT_1005abf0)(auStack_1e8,auStack_15c,0);
            if (iVar4 == 0) {
              iStack_24c = 0;
            }
            else {
              (*DAT_1005abf0)(auStack_1e8,&iStack_1ec,0);
              iStack_24c = iStack_1ec;
            }
            if (iStack_110 == 0) {
              puStack_294 = (undefined1 *)0x0;
            }
            else if (puStack_114 == (undefined1 *)0x0) {
              puStack_294 = auStack_10c;
            }
            else {
              puStack_294 = puStack_114;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 0x118,auStack_198,0);
            if (iVar4 == 0) {
              iStack_280 = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0x118,&iStack_234,0);
              iStack_280 = iStack_234;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 0x108,auStack_1a0,0);
            if (iVar4 == 0) {
              iStack_260 = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0x108,&iStack_1fc,0);
              iStack_260 = iStack_1fc;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 0xf8,auStack_1a8,0);
            if (iVar4 == 0) {
              iStack_288 = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0xf8,&iStack_22c,0);
              iStack_288 = iStack_22c;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 0xe8,auStack_174,0);
            if (iVar4 == 0) {
              iStack_250 = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0xe8,&iStack_208,0);
              iStack_250 = iStack_208;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 0xd8,auStack_178,0);
            if (iVar4 == 0) {
              iStack_270 = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0xd8,&iStack_224,0);
              iStack_270 = iStack_224;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 200,auStack_190,0);
            if (iVar4 == 0) {
              pwStack_258 = (wchar_t *)0x0;
            }
            else {
              (*DAT_1005abcc)((int)this + 200,&pwStack_1f0,0);
              pwStack_258 = pwStack_1f0;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 0xb8,auStack_168,0);
            if (iVar4 == 0) {
              iStack_268 = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0xb8,&iStack_21c,0);
              iStack_268 = iStack_21c;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 0xa8,auStack_188,0);
            if (iVar4 == 0) {
              pWStack_248 = (LPCWSTR)0x0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0xa8,&pWStack_1f8,0);
              pWStack_248 = pWStack_1f8;
            }
            puStack_284 = &DAT_1004cbf0;
            if (iStack_38 == 0) {
              puStack_284 = &DAT_1004f9a0;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 0x88,auStack_170,0);
            if ((iVar4 == 0) || (iStack_40 == 1)) {
              iStack_28c = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0x88,&iStack_214,0);
              iStack_28c = iStack_214;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 0x48,auStack_180,0);
            if (iVar4 == 0) {
              iStack_27c = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0x48,&iStack_20c,0);
              iStack_27c = iStack_20c;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 0x78,auStack_160,0);
            if (iVar4 == 0) {
              iStack_274 = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0x78,&iStack_23c,0);
              iStack_274 = iStack_23c;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 0x68,auStack_19c,0);
            if (iVar4 == 0) {
              iStack_26c = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0x68,&iStack_240,0);
              iStack_26c = iStack_240;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 0x58,auStack_194,0);
            if (iVar4 == 0) {
              iStack_264 = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0x58,&iStack_230,0);
              iStack_264 = iStack_230;
            }
            iVar4 = (*DAT_1005abcc)((int)this + 0x98,auStack_18c,0);
            if (iVar4 == 0) {
              iVar4 = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0x98,&iStack_228,0);
              iVar4 = iStack_228;
            }
            iVar10 = (*DAT_1005abcc)((int)this + 0x38,auStack_184,0);
            if (iVar10 == 0) {
              iVar10 = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0x38,&iStack_220,0);
              iVar10 = iStack_220;
            }
            iVar11 = (*DAT_1005abcc)((int)this + 0x28,&piStack_204,0);
            if (iVar11 == 0) {
              iVar11 = 0;
            }
            else {
              (*DAT_1005abcc)((int)this + 0x28,&iStack_218,0);
              iVar11 = iStack_218;
            }
            piVar12 = FUN_1000e0e0(pvStack_244,*(uint **)((int)this + 0x150),
                                   *(int ***)((int)this + 0x154),*(int *)((int)this + 0x1a4),
                                   *(wchar_t **)((int)this + 0x13c),*(uint *)((int)this + 0x140),
                                   pwStack_2a0,iVar11,iVar10,iVar4,iStack_264,iStack_26c,iStack_274,
                                   iStack_27c,iStack_28c,(int)puStack_284,pWStack_248,0xfde9,1,1,0,0
                                   ,-(uint)(iStack_140 != 0) & (uint)pOStack_2b0,iStack_268,
                                   pwStack_258,iStack_270,iStack_250,iStack_288,iStack_260,
                                   iStack_280,*(undefined4 *)((int)this + 0x198),
                                   *(undefined4 *)((int)this + 0x19c),*(uint *)((int)this + 0x1a0),0
                                   ,0,(int)puStack_294,iStack_24c,0,0);
            bstrString = local_298;
          }
          local_4._0_1_ = 0xf;
          if (piVar12 != (int *)0x0) {
            iVar4 = piVar12[2];
            if (iVar4 == 0) {
              iVar4 = piVar12[3];
            }
            *(int *)((int)this + 0x138) = iVar4;
            if ((local_2a8 != (int **)0x0) && (iVar4 != 0)) {
              (*DAT_1005abf0)((int)this + 0x18,&pcStack_278,0);
              FUN_100030c0(&DAT_1005ac14,*(int *)((int)this + 0x138),pcStack_278,local_d8);
              local_2a8 = (int **)0x0;
              if (iStack_290 != 0) {
                FUN_10002640(&DAT_1005ac14,*(int *)((int)this + 0x138),iStack_290);
              }
            }
            if (piVar12[2] == 0) {
              (**(code **)(*piVar12 + 0x18))(1);
            }
          }
          local_4._0_1_ = 0xe;
          (*DAT_1005abec)(auStack_1e8);
          local_4._0_1_ = 0xc;
          SysFreeString(bstrString);
          local_4._0_1_ = 0xb;
          SysFreeString(pOStack_2b0);
          local_4._0_1_ = 10;
          SysFreeString(pwStack_2a0);
        }
LAB_1002b167:
        local_4 = CONCAT31(local_4._1_3_,9);
        ppuStack_158 = MyStrClass::vftable;
        if (pcStack_154 != (char *)0x0) {
          operator_delete(pcStack_154);
        }
        iVar4 = FUN_1003eb40(local_d8,&ppuStack_f8,&ppuStack_148,&ppuStack_108,&ppuStack_118,
                             &ppuStack_128);
      }
      local_4._0_1_ = 8;
      ppuStack_138 = MyStrClass::vftable;
      if (pcStack_134 != (char *)0x0) {
        operator_delete(pcStack_134);
      }
      local_4._0_1_ = 7;
      ppuStack_128 = MyStrClass::vftable;
      if (pcStack_124 != (char *)0x0) {
        operator_delete(pcStack_124);
      }
      local_4._0_1_ = 6;
      ppuStack_108 = MyStrClass::vftable;
      if (puStack_104 != (undefined1 *)0x0) {
        operator_delete(puStack_104);
      }
      local_4._0_1_ = 5;
      ppuStack_118 = MyStrClass::vftable;
      if (puStack_114 != (undefined1 *)0x0) {
        operator_delete(puStack_114);
      }
      local_4._0_1_ = 4;
      ppuStack_148 = MyStrClass::vftable;
      if (pcStack_144 != (char *)0x0) {
        operator_delete(pcStack_144);
      }
      local_4._0_1_ = 2;
      ppuStack_f8 = MyStrClass::vftable;
      if (pcStack_f4 != (char *)0x0) {
        operator_delete(pcStack_f4);
      }
      local_4._0_1_ = 1;
      FUN_10017f20((int *)&piStack_2a4);
      local_4 = (uint)local_4._1_3_ << 8;
      FUN_10017f20((int *)&piStack_2ac);
    }
  }
  local_4 = 0xffffffff;
  FUN_1003f550(local_d8);
  uVar13 = extraout_EDX_00;
LAB_1002b2c6:
  ExceptionList = local_c;
  FUN_100447e0(local_10 ^ (uint)&pOStack_2b0,uVar13);
  return;
}


