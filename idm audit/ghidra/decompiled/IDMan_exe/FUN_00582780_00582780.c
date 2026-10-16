// Function: FUN_00582780
// Entry Point: 0x00582780
// Program: IDMan.exe


int __thiscall
FUN_00582780(int param_1,undefined4 param_2,undefined4 param_3,undefined4 *param_4,
            undefined4 param_5)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  char *pcVar4;
  int iVar5;
  void *local_10;
  undefined1 *puStack_c;
  undefined4 local_8;
  
  puStack_c = &LAB_0067eda0;
  local_10 = ExceptionList;
  ExceptionList = &local_10;
  local_8 = 0;
  do {
    iVar2 = FUN_00580d50();
    if (iVar2 != 0) {
      ExceptionList = local_10;
      return iVar2;
    }
    while( true ) {
      pcVar4 = *(char **)(param_1 + 0x3258);
      if (*pcVar4 == '\n') goto LAB_005827f1;
      if ((*pcVar4 == '\r') && (pcVar4[1] == '\n')) break;
      if ((*(char *)(param_1 + 0x3280) != '\0') &&
         (pcVar4 = _strstr(pcVar4,*(char **)(param_1 + 0x327c)), pcVar4 != (char *)0x0)) {
        *(undefined1 *)(param_1 + 0x3280) = 0;
      }
      iVar2 = *(int *)(param_1 + 0x3258);
      iVar5 = FUN_005803a0(iVar2);
      iVar2 = __memicmp((void *)(iVar2 + iVar5),"content-length:",0xf);
      if (iVar2 == 0) {
        FID_conflict__sscanf((char *)(*(int *)(param_1 + 0x3258) + 0xf + iVar5),"%I64d",param_2);
      }
      iVar2 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar5),"Content-Range:",0xe);
      if (iVar2 == 0) {
        pcVar4 = _strstr((char *)(*(int *)(param_1 + 0x3258) + 0xe + iVar5),"bytes");
        if (pcVar4 != (char *)0x0) {
          FUN_005803a0(pcVar4 + 5);
          FID_conflict__sscanf(pcVar4 + iVar5 + 5,"%I64d",param_3);
        }
        pcVar4 = _strchr((char *)(*(int *)(param_1 + 0x3258) + 0xe + iVar5),0x2f);
        if (pcVar4 != (char *)0x0) {
          cVar1 = pcVar4[1];
          if ((cVar1 != '\r') && (cVar1 != '\n')) {
            FID_conflict__sscanf(pcVar4 + 1,"%I64d",param_5);
          }
        }
        pcVar4 = _strchr((char *)(*(int *)(param_1 + 0x3258) + 0xe + iVar5),0x2d);
        if (pcVar4 != (char *)0x0) {
          cVar1 = pcVar4[1];
          if (((cVar1 != '\r') && (cVar1 != '\n')) &&
             (iVar2 = FID_conflict__sscanf(pcVar4 + 1,"%I64d",param_4), iVar2 < 1)) {
            *param_4 = 0;
            param_4[1] = 0;
          }
        }
      }
      FUN_00581c80();
      iVar2 = FUN_00580d50();
      if (iVar2 != 0) {
        ExceptionList = local_10;
        return iVar2;
      }
    }
  } while (*(char *)(param_1 + 0x3280) != '\0');
LAB_005827f1:
  *(undefined4 *)(param_1 + 0x3204) = 0;
  uVar3 = FUN_00566d50();
  FUN_00574b80(uVar3);
  FUN_00566d80();
  *(undefined4 *)(param_1 + 0xa4) = 5;
  ExceptionList = local_10;
  return 0;
}


