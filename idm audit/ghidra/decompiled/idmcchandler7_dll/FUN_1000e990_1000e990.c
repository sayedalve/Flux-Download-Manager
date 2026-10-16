// Function: FUN_1000e990
// Entry Point: 0x1000e990
// Program: idmcchandler7.dll


void FUN_1000e990(void)

{
  DWORD DVar1;
  HMODULE hInstance;
  BOOL BVar2;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  undefined4 extraout_EDX_01;
  undefined4 uVar3;
  LPVOID lpParam;
  uint uStack_64;
  tagMSG local_54;
  char local_38 [32];
  uint local_18;
  undefined1 *local_14;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_10046a20;
  local_10 = ExceptionList;
  uStack_64 = DAT_1005a88c ^ (uint)&stack0xfffffffc;
  local_14 = (undefined1 *)&uStack_64;
  ExceptionList = &local_10;
  local_8 = 0;
  local_18 = uStack_64;
  DVar1 = GetCurrentProcessId();
  _snprintf(local_38,0x20,"%ld",DVar1);
  lpParam = (LPVOID)0x0;
  hInstance = GetModuleHandleA((LPCSTR)0x0);
  DAT_1005ac40 = CreateWindowExA(0,"IDM_Download_Panel_controller2_class",local_38,0,0,0,1,1,
                                 (HWND)0xfffffffd,(HMENU)0x0,hInstance,lpParam);
  uVar3 = extraout_EDX;
  if (DAT_1005ac40 != (HWND)0x0) {
    local_8 = CONCAT31(local_8._1_3_,1);
    CoInitialize((LPVOID)0x0);
    while( true ) {
      BVar2 = GetMessageA(&local_54,(HWND)0x0,0,0);
      if (BVar2 == 0) break;
      uVar3 = extraout_EDX_00;
      if (BVar2 == -1) goto LAB_1000ea68;
      TranslateMessage(&local_54);
      DispatchMessageA(&local_54);
    }
    CoUninitialize();
    local_8 = 0xffffffff;
    uVar3 = extraout_EDX_01;
  }
LAB_1000ea68:
  ExceptionList = local_10;
  FUN_100447e0(local_18 ^ (uint)&stack0xfffffffc,uVar3);
  return;
}


