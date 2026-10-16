// Function: FUN_1000f780
// Entry Point: 0x1000f780
// Program: idmcchandler7.dll


undefined4 FUN_1000f780(void)

{
  ATOM AVar1;
  DWORD DVar2;
  WNDCLASSA local_28;
  
  if (DAT_1005ac45 != '\0') {
    return 1;
  }
  local_28.style = 3;
  local_28.lpfnWndProc = (WNDPROC)&LAB_1000ec70;
  local_28.cbClsExtra = 0;
  local_28.cbWndExtra = 0;
  local_28.hInstance = (HINSTANCE)0x0;
  local_28.hIcon = LoadIconA((HINSTANCE)0x0,(LPCSTR)0x7f00);
  local_28.hCursor = LoadCursorA((HINSTANCE)0x0,(LPCSTR)0x7f00);
  local_28.hbrBackground = GetStockObject(2);
  local_28.lpszMenuName = (LPCSTR)0x0;
  local_28.lpszClassName = "IDM_Download_Panel_controller2_class";
  AVar1 = RegisterClassA(&local_28);
  if (AVar1 == 0) {
    DVar2 = GetLastError();
    if (DVar2 != 0x582) {
      return 0;
    }
  }
  DAT_1005ac45 = 1;
  return 1;
}


