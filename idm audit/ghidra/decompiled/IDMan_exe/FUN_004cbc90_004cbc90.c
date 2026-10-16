// Function: FUN_004cbc90
// Entry Point: 0x004cbc90
// Program: IDMan.exe


void __fastcall FUN_004cbc90(CListCtrl *param_1)

{
  uint uVar1;
  LSTATUS LVar2;
  HDC pHVar3;
  LRESULT LVar4;
  int iVar5;
  int iVar6;
  CListCtrl *pCVar7;
  WPARAM wParam;
  undefined1 auStack_254 [7];
  char local_24d;
  HKEY local_24c;
  CListCtrl *local_248;
  char local_244;
  int local_240;
  int local_23c;
  LRESULT local_238;
  DWORD local_234 [134];
  uint local_1c;
  void *local_14;
  undefined1 *puStack_10;
  undefined4 uStack_c;
  
  uStack_c = 0xffffffff;
  puStack_10 = &LAB_0066ea5b;
  local_14 = ExceptionList;
  local_1c = DAT_007839c0 ^ (uint)auStack_254;
  uVar1 = DAT_007839c0 ^ (uint)&stack0xfffffda0;
  ExceptionList = &local_14;
  LVar2 = RegCreateKeyExA((HKEY)&DAT_80000001,"Software\\DownloadManager\\ListSettings",0,"",0,
                          0xf003f,(LPSECURITY_ATTRIBUTES)0x0,&local_24c,local_234);
  if (LVar2 == 0) {
    iVar6 = 0x60;
    local_24d = '\0';
    local_240 = 0x60;
    local_23c = 0x60;
    pHVar3 = GetDC((HWND)0x0);
    if (pHVar3 != (HDC)0x0) {
      iVar6 = GetDeviceCaps(pHVar3,0x58);
      local_240 = iVar6;
      local_23c = GetDeviceCaps(pHVar3,0x5a);
      ReleaseDC((HWND)0x0,pHVar3);
    }
    local_244 = '\x01';
    if (iVar6 != 0x60) {
      local_24d = '\x01';
    }
    pCVar7 = param_1 + 0x120;
    wParam = 0;
    local_248 = pCVar7;
    do {
      if (*(int *)(pCVar7 + 4) == 0) {
        if (local_24d != '\0') {
          LVar4 = *(LRESULT *)pCVar7;
          goto LAB_004cbdd5;
        }
      }
      else {
        if (local_24d == '\0') {
          LVar4 = SendMessageA(*(HWND *)(param_1 + 0x20),0x101d,wParam,0);
        }
        else {
          LVar4 = SendMessageA(*(HWND *)(param_1 + 0x20),0x101d,wParam,0);
LAB_004cbdd5:
          LVar4 = FUN_004a9b10(LVar4);
        }
        *(LRESULT *)pCVar7 = LVar4;
      }
      if (*(double *)(param_1 + 0x118) != 0.0) {
        LVar4 = FUN_0063ec80();
        *(LRESULT *)pCVar7 = LVar4;
      }
      wParam = wParam + 1;
      pCVar7 = pCVar7 + 8;
    } while ((int)wParam < 0xc);
    RegSetValueExA(local_24c,"FileName",0,0,(BYTE *)local_248,8);
    RegSetValueExA(local_24c,"DateAdded",0,0,(BYTE *)(param_1 + 0x128),8);
    RegSetValueExA(local_24c,"Queue",0,0,(BYTE *)(param_1 + 0x130),8);
    RegSetValueExA(local_24c,"Size",0,0,(BYTE *)(param_1 + 0x138),8);
    RegSetValueExA(local_24c,"Status",0,0,(BYTE *)(param_1 + 0x140),8);
    RegSetValueExA(local_24c,"Timeleft",0,0,(BYTE *)(param_1 + 0x148),8);
    RegSetValueExA(local_24c,"TransferRate",0,0,(BYTE *)(param_1 + 0x150),8);
    RegSetValueExA(local_24c,"LastTry",0,0,(BYTE *)(param_1 + 0x158),8);
    RegSetValueExA(local_24c,"Description",0,0,(BYTE *)(param_1 + 0x160),8);
    RegSetValueExA(local_24c,"SaveTo",0,0,(BYTE *)(param_1 + 0x168),8);
    RegSetValueExA(local_24c,"Referer",0,0,(BYTE *)(param_1 + 0x170),8);
    RegSetValueExA(local_24c,"Parent_wp",0,0,(BYTE *)(param_1 + 0x178),8);
    iVar6 = CListCtrl::GetColumnOrderArray(param_1,(int *)(param_1 + 0x180),0xc);
    if (iVar6 != 0) {
      RegSetValueExA(local_24c,"Order639",0,0,(BYTE *)(param_1 + 0x180),0x30);
    }
    pCVar7 = local_248;
    if (local_24d != '\0') {
      local_248 = (CListCtrl *)0xc;
      iVar6 = local_240;
      do {
        local_238 = *(LRESULT *)pCVar7;
        if (local_244 == '\0') {
          pHVar3 = GetDC((HWND)0x0);
          if (pHVar3 != (HDC)0x0) {
            iVar6 = GetDeviceCaps(pHVar3,0x58);
            GetDeviceCaps(pHVar3,0x5a);
            ReleaseDC((HWND)0x0,pHVar3);
          }
          local_244 = '\x01';
        }
        iVar5 = MulDiv(local_238,iVar6,0x60);
        *(int *)pCVar7 = iVar5;
        pCVar7 = pCVar7 + 8;
        local_248 = local_248 + -1;
      } while (local_248 != (CListCtrl *)0x0);
    }
    RegCloseKey(local_24c);
  }
  else {
    FUN_004c4c30(uVar1);
    uStack_c = 0;
    FUN_004c4da0("Cannot open(/create) registry listsettings subkey.\n",1);
    uStack_c = 0xffffffff;
    FUN_004c4d90();
  }
  ExceptionList = local_14;
  __security_check_cookie(local_1c ^ (uint)auStack_254);
  return;
}


