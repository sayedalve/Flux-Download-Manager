// Function: FUN_1003ced0
// Entry Point: 0x1003ced0
// Program: IDMNetMon.dll


undefined4 __fastcall FUN_1003ced0(undefined4 *param_1)

{
  int iVar1;
  
  if ((((uint)param_1[1] < 5) || (iVar1 = _strnicmp((char *)*param_1,"http:",5), iVar1 != 0)) &&
     (((uint)param_1[1] < 6 || (iVar1 = _strnicmp((char *)*param_1,"https:",6), iVar1 != 0)))) {
    if (((3 < (uint)param_1[1]) && (iVar1 = _strnicmp((char *)*param_1,"ftp:",4), iVar1 == 0)) ||
       ((4 < (uint)param_1[1] && (iVar1 = _strnicmp((char *)*param_1,"ftps:",5), iVar1 == 0)))) {
      return 2;
    }
    if ((((4 < (uint)param_1[1]) && (iVar1 = _strnicmp((char *)*param_1,"rtmp:",5), iVar1 == 0)) ||
        ((5 < (uint)param_1[1] && (iVar1 = _strnicmp((char *)*param_1,"rtmpt:",6), iVar1 == 0)))) ||
       ((5 < (uint)param_1[1] && (iVar1 = _strnicmp((char *)*param_1,"rtmpe:",6), iVar1 == 0)))) {
      return 3;
    }
    if ((6 < (uint)param_1[1]) && (iVar1 = _strnicmp((char *)*param_1,"idmreg:",7), iVar1 == 0)) {
      return 4;
    }
    if (((((((uint)param_1[1] < 9) ||
           (iVar1 = _strnicmp((char *)*param_1,"dhstoidm:",9), iVar1 != 0)) &&
          (((uint)param_1[1] < 10 ||
           (iVar1 = _strnicmp((char *)*param_1,"dhs2toidm:",10), iVar1 != 0)))) &&
         ((((uint)param_1[1] < 9 || (iVar1 = _strnicmp((char *)*param_1,"hlstoidm:",9), iVar1 != 0))
          && (((uint)param_1[1] < 10 ||
              (iVar1 = _strnicmp((char *)*param_1,"hls2toidm:",10), iVar1 != 0)))))) &&
        ((((uint)param_1[1] < 9 || (iVar1 = _strnicmp((char *)*param_1,"ytatoidm:",9), iVar1 != 0))
         && (((uint)param_1[1] < 10 ||
             (iVar1 = _strnicmp((char *)*param_1,"yta2toidm:",10), iVar1 != 0)))))) &&
       (((uint)param_1[1] < 10 || (iVar1 = _strnicmp((char *)*param_1,"yta3toidm:",10), iVar1 != 0))
       )) {
      return 0;
    }
  }
  return 1;
}


