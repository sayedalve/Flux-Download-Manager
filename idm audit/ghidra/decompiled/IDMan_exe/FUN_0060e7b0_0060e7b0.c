// Function: FUN_0060e7b0
// Entry Point: 0x0060e7b0
// Program: IDMan.exe


void FUN_0060e7b0(int *param_1)

{
  int iVar1;
  int *piVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  uint uVar6;
  byte *pbVar7;
  void *local_14;
  int local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  piVar2 = param_1;
  local_4 = 0xffffffff;
  puStack_8 = &LAB_00685ed8;
  local_c = ExceptionList;
  uVar3 = DAT_007839c0 ^ (uint)&stack0xffffffdc;
  ExceptionList = &local_c;
  FUN_0060d670(param_1[9] * 0x15 + 0x30);
  uVar6 = 0;
  local_4 = 0;
  if ((char)piVar2[3] != '\0') goto LAB_0060e824;
  iVar1 = piVar2[1];
  if ((uint)piVar2[2] < iVar1 + 1U) {
    iVar4 = FUN_0060d620(1);
    if (iVar4 != 0) goto LAB_0060e816;
  }
  else {
LAB_0060e816:
    *(undefined1 *)(piVar2[1] + *piVar2) = 0x3b;
    piVar2[1] = iVar1 + 1U;
  }
  *(undefined1 *)(piVar2 + 3) = 1;
LAB_0060e824:
  FUN_0060de70(&local_14,"MSG#%u#%u#%u#%u",piVar2[4],piVar2[5],piVar2[6],piVar2[7],uVar3);
  if (piVar2[9] != 0) {
    pbVar7 = (byte *)(piVar2 + 10);
    do {
      param_1 = (int *)CONCAT31(param_1._1_3_,0x3a);
      FUN_0060d950(1,&param_1);
      FUN_0060e650(&local_14,&DAT_007297a0 + (uint)*pbVar7 * 5,*(undefined4 *)(pbVar7 + 8),
                   *(undefined4 *)(pbVar7 + 0xc));
      uVar6 = uVar6 + 1;
      pbVar7 = pbVar7 + 0x10;
    } while (uVar6 < (uint)piVar2[9]);
  }
  iVar4 = *(int *)((int)local_14 + -8) + piVar2[1];
  iVar1 = iVar4 + 4;
  piVar5 = (int *)FUN_0060d620(iVar1);
  if (piVar5 == (int *)0x0) {
    FUN_0060e130();
  }
  else {
    *(int *)(local_10 + 0x44) = *(int *)(local_10 + 0x44) + iVar1;
    *piVar5 = iVar4;
    _memcpy(piVar5 + 1,local_14,*(size_t *)((int)local_14 + -8));
    _memcpy((void *)((int)(piVar5 + 1) + *(int *)((int)local_14 + -8)),(void *)*piVar2,piVar2[1]);
    FUN_0060e6f0();
  }
  local_4 = 0xffffffff;
  FUN_0060d6e0();
  ExceptionList = local_c;
  return;
}


