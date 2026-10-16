// Function: FUN_00610980
// Entry Point: 0x00610980
// Program: IDMan.exe


void __fastcall FUN_00610980(int *param_1)

{
  DWORD dwProcessId;
  HANDLE pvVar1;
  int iVar2;
  char *pcVar3;
  CHAR *pCVar4;
  DWORD *pSessionId;
  DWORD local_48;
  CHAR local_44 [27];
  char local_29 [37];
  uint local_4;
  
  pSessionId = &local_48;
  local_4 = DAT_007839c0 ^ (uint)&local_48;
  local_48 = 0;
  dwProcessId = GetCurrentProcessId();
  ProcessIdToSessionId(dwProcessId,pSessionId);
  pvVar1 = CreateEventA((LPSECURITY_ATTRIBUTES)0x0,1,0,(LPCSTR)0x0);
  param_1[1] = (int)pvVar1;
  if (pvVar1 != (HANDLE)0x0) {
    pcVar3 = "\\\\.\\pipe\\IDMNetworkMonitor.";
    pCVar4 = local_44;
    for (iVar2 = 7; iVar2 != 0; iVar2 = iVar2 + -1) {
      *(undefined4 *)pCVar4 = *(undefined4 *)pcVar3;
      pcVar3 = pcVar3 + 4;
      pCVar4 = pCVar4 + 4;
    }
    local_29[1] = '\0';
    local_29[2] = '\0';
    local_29[3] = '\0';
    local_29[4] = '\0';
    local_29[5] = '\0';
    local_29[6] = '\0';
    local_29[7] = '\0';
    local_29[8] = '\0';
    local_29[9] = '\0';
    local_29[10] = '\0';
    local_29[0xb] = '\0';
    local_29[0xc] = '\0';
    local_29[0xd] = '\0';
    local_29[0xe] = '\0';
    local_29[0xf] = '\0';
    local_29[0x10] = '\0';
    local_29[0x11] = '\0';
    local_29[0x12] = '\0';
    local_29[0x13] = '\0';
    local_29[0x14] = '\0';
    local_29[0x15] = '\0';
    local_29[0x16] = '\0';
    local_29[0x17] = '\0';
    local_29[0x18] = '\0';
    local_29[0x19] = '\0';
    local_29[0x1a] = '\0';
    local_29[0x1b] = '\0';
    local_29[0x1c] = '\0';
    local_29[0x1d] = '\0';
    local_29[0x1e] = '\0';
    local_29[0x1f] = '\0';
    local_29[0x20] = '\0';
    local_29[0x21] = '\0';
    local_29[0x22] = '\0';
    local_29[0x23] = '\0';
    local_29[0x24] = '\0';
    __itoa(local_48,local_29,10);
    pvVar1 = CreateFileA(local_44,0xc0000000,0,(LPSECURITY_ATTRIBUTES)0x0,3,0x60000000,(HANDLE)0x0);
    *param_1 = (int)pvVar1;
    if (pvVar1 != (HANDLE)0xffffffff) {
      param_1[2] = 0;
      param_1[3] = 0;
      param_1[4] = 0;
      param_1[5] = 0;
      param_1[6] = 0;
      param_1[7] = 0;
      param_1[8] = 0;
      param_1[9] = 0;
      param_1[10] = 0;
      param_1[0xb] = 0;
      param_1[0xb] = param_1[1];
      param_1[6] = param_1[1];
      *(undefined1 *)((int)param_1 + 0x31) = 0;
      *(undefined1 *)(param_1 + 0xc) = 0;
      FUN_00610800();
      if (*param_1 != -1) {
        __security_check_cookie(local_4 ^ (uint)&local_48);
        return;
      }
    }
  }
  FUN_0060f3c0();
  FUN_00610e74(param_1);
  __security_check_cookie(local_4 ^ (uint)&local_48);
  return;
}


