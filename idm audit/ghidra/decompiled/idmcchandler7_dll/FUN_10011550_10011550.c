// Function: FUN_10011550
// Entry Point: 0x10011550
// Program: idmcchandler7.dll


void __fastcall FUN_10011550(void *param_1,char *param_2,char *param_3)

{
  char cVar1;
  char *pcVar2;
  int iVar3;
  char *pcVar4;
  undefined ***this;
  char *extraout_EDX;
  char *extraout_EDX_00;
  char *extraout_EDX_01;
  char *extraout_EDX_02;
  char *extraout_EDX_03;
  char *extraout_EDX_04;
  char *extraout_EDX_05;
  char *extraout_EDX_06;
  char *extraout_EDX_07;
  void *local_64;
  undefined **local_60;
  char *local_5c;
  int local_58;
  char local_54 [4];
  undefined **local_50;
  char *local_4c;
  int local_48;
  char local_44 [4];
  undefined **local_40;
  char *local_3c;
  int local_38;
  char local_34 [4];
  undefined **local_30;
  char *local_2c;
  int local_28;
  char local_24 [4];
  undefined **local_20;
  char *local_1c;
  int local_18;
  char local_14 [4];
  uint local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  puStack_8 = &LAB_10046fb8;
  local_c = ExceptionList;
  local_10 = DAT_1005a88c ^ (uint)&local_64;
  ExceptionList = &local_c;
  local_20 = MyStrClass::vftable;
  local_1c = (char *)0x0;
  local_18 = 0;
  local_14[0] = '\0';
  local_30 = MyStrClass::vftable;
  local_2c = (char *)0x0;
  local_28 = 0;
  local_24[0] = '\0';
  local_40 = MyStrClass::vftable;
  local_3c = (char *)0x0;
  local_38 = 0;
  local_34[0] = '\0';
  local_4._1_3_ = 0;
  local_4._0_1_ = 2;
  local_64 = param_1;
  if (param_3 != (char *)0x0) {
    do {
      param_2 = param_3 + 1;
      pcVar2 = param_3;
      do {
        cVar1 = *pcVar2;
        pcVar2 = pcVar2 + 1;
      } while (cVar1 != '\0');
      if ((uint)((int)pcVar2 - (int)param_2) < 2) break;
      local_60 = MyStrClass::vftable;
      local_5c = (char *)0x0;
      local_58 = 0;
      local_54[0] = '\0';
      local_50 = MyStrClass::vftable;
      local_4c = (char *)0x0;
      local_48 = 0;
      local_44[0] = '\0';
      local_4 = CONCAT31(local_4._1_3_,4);
      param_3 = FUN_10010380(param_3,&local_60,&local_50);
      if ((local_5c == (char *)0x0) || (pcVar2 = local_5c, local_58 == 0)) {
        pcVar2 = local_54;
      }
      iVar3 = _stricmp(pcVar2,"url");
      if (iVar3 == 0) {
        if ((local_4c == (char *)0x0) || (pcVar2 = local_4c, local_48 == 0)) {
          pcVar2 = local_44;
        }
        this = &local_20;
LAB_100116e0:
        FUN_10001950(this,pcVar2);
        param_2 = extraout_EDX_00;
      }
      else {
        if ((local_5c == (char *)0x0) || (pcVar2 = local_5c, local_58 == 0)) {
          pcVar2 = local_54;
        }
        iVar3 = _stricmp(pcVar2,"quality");
        if (iVar3 == 0) {
          if ((local_4c == (char *)0x0) || (pcVar2 = local_4c, local_48 == 0)) {
            pcVar2 = local_44;
          }
          this = &local_30;
          goto LAB_100116e0;
        }
        if ((local_5c == (char *)0x0) || (pcVar2 = local_5c, local_58 == 0)) {
          pcVar2 = local_54;
        }
        iVar3 = _stricmp(pcVar2,"mime");
        param_2 = extraout_EDX;
        if (iVar3 == 0) {
          if ((local_4c == (char *)0x0) || (pcVar2 = local_4c, local_48 == 0)) {
            pcVar2 = local_44;
          }
          this = &local_40;
          goto LAB_100116e0;
        }
      }
      if ((param_3 != (char *)0x0) && (*param_3 == ',')) {
        param_3 = param_3 + 1;
      }
      local_4._0_1_ = 3;
      local_50 = MyStrClass::vftable;
      if (local_4c != (char *)0x0) {
        operator_delete(local_4c);
        param_2 = extraout_EDX_01;
      }
      local_4._0_1_ = 2;
      local_60 = MyStrClass::vftable;
      if (local_5c != (char *)0x0) {
        operator_delete(local_5c);
        param_2 = extraout_EDX_02;
      }
    } while (param_3 != (char *)0x0);
    if (((local_18 != 0) && (local_28 != 0)) && (local_38 != 0)) {
      pcVar2 = local_3c;
      if (local_3c == (char *)0x0) {
        pcVar2 = local_34;
      }
      iVar3 = _stricmp(pcVar2,"video/mp4");
      param_2 = extraout_EDX_03;
      if (iVar3 == 0) {
        pcVar2 = local_2c;
        if (local_2c == (char *)0x0) {
          pcVar2 = local_24;
        }
        pcVar4 = local_1c;
        if (local_1c == (char *)0x0) {
          pcVar4 = local_14;
        }
        FUN_10011370(local_64,pcVar4,pcVar2);
        param_2 = extraout_EDX_04;
      }
    }
  }
  local_4._0_1_ = 1;
  local_40 = MyStrClass::vftable;
  if (local_3c != (char *)0x0) {
    operator_delete(local_3c);
    param_2 = extraout_EDX_05;
  }
  local_4 = (uint)local_4._1_3_ << 8;
  local_30 = MyStrClass::vftable;
  if (local_2c != (char *)0x0) {
    operator_delete(local_2c);
    param_2 = extraout_EDX_06;
  }
  local_4 = 0xffffffff;
  local_20 = MyStrClass::vftable;
  if (local_1c != (char *)0x0) {
    operator_delete(local_1c);
    param_2 = extraout_EDX_07;
  }
  ExceptionList = local_c;
  FUN_100447e0(local_10 ^ (uint)&local_64,param_2);
  return;
}


