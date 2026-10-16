// Function: FUN_10013d30
// Entry Point: 0x10013d30
// Program: IDMNetMon.dll


undefined4 __fastcall FUN_10013d30(int param_1)

{
  char cVar1;
  int iVar2;
  DWORD dwProcessId;
  BOOL BVar3;
  HANDLE pvVar4;
  SOCKET SVar5;
  undefined4 extraout_ECX;
  DWORD *pSessionId;
  WSADATA local_194;
  
  iVar2 = FUN_10008610();
  if (iVar2 != 0) {
    pSessionId = (DWORD *)(param_1 + 0x1b3c);
    dwProcessId = GetCurrentProcessId();
    BVar3 = ProcessIdToSessionId(dwProcessId,pSessionId);
    if (BVar3 != 0) {
      FUN_10012770(param_1 + 2,0x40,L"\\\\.\\pipe\\IDMNetworkMonitor.%u",
                   *(DWORD *)(param_1 + 0x1b3c));
      pvVar4 = CreateEventW((LPSECURITY_ATTRIBUTES)0x0,1,0,(LPCWSTR)0x0);
      *(HANDLE *)(param_1 + 0x1be4) = pvVar4;
      if (pvVar4 != (HANDLE)0x0) {
        pvVar4 = CreateEventW((LPSECURITY_ATTRIBUTES)0x0,1,0,(LPCWSTR)0x0);
        *(HANDLE *)(param_1 + 0x1be8) = pvVar4;
        if (pvVar4 != (HANDLE)0x0) {
          pvVar4 = CreateEventW((LPSECURITY_ATTRIBUTES)0x0,1,0,(LPCWSTR)0x0);
          *(HANDLE *)(param_1 + 0x1ccc) = pvVar4;
          if (pvVar4 != (HANDLE)0x0) {
            pvVar4 = CreateEventW((LPSECURITY_ATTRIBUTES)0x0,1,0,(LPCWSTR)0x0);
            *(HANDLE *)(param_1 + 0x1cf8) = pvVar4;
            if (pvVar4 != (HANDLE)0x0) {
              iVar2 = WSAStartup(0x202,&local_194);
              if (iVar2 == 0) {
                *(undefined1 *)(param_1 + 0x1cd4) = 1;
                iVar2 = WSACreateEvent();
                *(int *)(param_1 + 0x1cdc) = iVar2;
                if (iVar2 != 0) {
                  SVar5 = socket(2,1,6);
                  *(SOCKET *)(param_1 + 0x1cd8) = SVar5;
                  if (SVar5 != 0xffffffff) {
                    cVar1 = FUN_10030850();
                    if (cVar1 != '\0') {
                      FUN_10017d60(extraout_ECX);
                      if (DAT_10074c8c != (code *)0x0) {
                        (*DAT_10074c8c)(*(undefined4 *)(param_1 + 0x1ccc),param_1 + 0x1cd0);
                      }
                      return 1;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  }
  return 0;
}


