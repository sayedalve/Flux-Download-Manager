// Function: FUN_00610540
// Entry Point: 0x00610540
// Program: IDMan.exe


void FUN_00610540(char *param_1,uint param_2)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  size_t sVar4;
  char *pcVar5;
  char *pcVar6;
  char *pcVar7;
  uint local_10;
  undefined4 local_c;
  int local_8;
  undefined1 local_4 [4];
  
  uVar2 = param_2;
  pcVar6 = param_1;
  local_10 = 0;
  if ((0xb < param_2) &&
     (iVar3 = FID_conflict__sscanf
                        (param_1,"MSG#%u#%u#%u#%u%n",&param_1,local_4,&local_c,&param_2,&local_8),
     iVar3 == 4)) {
    pcVar6 = pcVar6 + local_8;
    for (pcVar5 = pcVar6;
        (pcVar7 = pcVar6, pcVar5 < pcVar6 + (uVar2 - local_8) && (*pcVar5 != '\0'));
        pcVar5 = pcVar5 + 1) {
      if (*pcVar5 == ',') goto LAB_006105d9;
    }
    while ((pcVar5 = pcVar7, pcVar5 < pcVar6 + (uVar2 - local_8) && (*pcVar5 != '\0'))) {
      if (*pcVar5 == ';') goto LAB_006105d9;
      pcVar7 = pcVar5 + 1;
    }
  }
LAB_006105c6:
  FUN_0060e130();
  return;
LAB_006105d9:
  if (pcVar6 < pcVar5) {
    cVar1 = *pcVar6;
    pcVar7 = pcVar6;
    while (cVar1 == ':') {
      sVar4 = _strspn(pcVar7 + 1,"0123456789.-");
      local_10 = local_10 + 1;
      pcVar7 = pcVar7 + 1 + sVar4;
      cVar1 = *pcVar7;
    }
    if (pcVar7 != pcVar5) goto LAB_006105c6;
  }
  pcVar6 = pcVar6 + 1;
  pcVar5 = pcVar5 + 1;
  pcVar7 = pcVar6 + (uVar2 - local_8) + (-1 - (int)pcVar5);
  switch(local_4[0]) {
  case 1:
    if (local_10 != 0) {
      FUN_00610020(param_1,local_c,param_2,pcVar6,pcVar5,pcVar7);
      return;
    }
    break;
  default:
    if ((param_2 & 1) == 0) {
      return;
    }
    FUN_0060ecf0(param_1,0);
    return;
  case 0x97:
    if (4 < local_10) {
      FUN_0060e920(param_1,local_c,param_2,pcVar6,pcVar5,pcVar7);
      return;
    }
    break;
  case 0x9a:
    FUN_00610110(param_1,local_c,param_2,pcVar6,pcVar5,pcVar7);
    return;
  case 0x9c:
    FUN_0060f7a0(param_1,local_c,param_2,pcVar6,pcVar5,pcVar7);
    return;
  }
  goto LAB_006105c6;
}


