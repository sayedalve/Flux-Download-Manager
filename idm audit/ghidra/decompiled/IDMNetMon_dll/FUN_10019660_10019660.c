// Function: FUN_10019660
// Entry Point: 0x10019660
// Program: IDMNetMon.dll


int * __thiscall
FUN_10019660(int param_1,int *param_2,int param_3,int param_4,uint param_5,char param_6)

{
  uint uVar1;
  uint uVar2;
  void *_Src;
  undefined4 *puVar3;
  bool bVar4;
  void *pvVar5;
  int iVar6;
  void *_Dst;
  
  uVar2 = *(uint *)(param_1 + 0x116c);
  iVar6 = *(int *)(param_3 + 0x230);
  _Src = *(void **)(param_3 + 0x22c);
  *(undefined2 *)(param_2 + 3) = 0x1ff;
  if (iVar6 < 1) {
LAB_100196cd:
    *param_2 = (int)&DAT_10074bf4;
    param_2[2] = 0;
    param_2[1] = 0;
  }
  else {
    param_2[2] = iVar6 * 2 + 0x20;
    pvVar5 = malloc(iVar6 * 2 + 0x24);
    _Dst = (void *)((int)pvVar5 + 4);
    if (pvVar5 == (void *)0x0) {
      _Dst = (void *)0x0;
    }
    *param_2 = (int)_Dst;
    if (_Dst == (void *)0x0) goto LAB_100196cd;
    param_2[1] = iVar6;
    *(undefined2 *)(iVar6 * 2 + (int)_Dst) = 0;
    memcpy(_Dst,_Src,iVar6 * 2);
  }
  if ((param_5 == 0) && (param_4 != 0)) {
    param_5 = *(uint *)(param_4 + 0x94);
  }
  iVar6 = __RTDynamicCast(param_3,0,&BasicClient::RTTI_Type_Descriptor,
                          &PluginClient::RTTI_Type_Descriptor,0);
  if ((((iVar6 != 0) && (-1 < param_2[1] + -10)) &&
      (iVar6 = _wcsnicmp((wchar_t *)(*param_2 + (param_2[1] + -10) * 2),L"chrome.exe",10),
      iVar6 == 0)) && (uVar1 = param_2[1] - 4, uVar1 < (uint)param_2[1])) {
    *(undefined2 *)(*param_2 + uVar1 * 2) = 0x2a;
  }
  if (0x6290fff < uVar2) goto LAB_100197f7;
  if ((param_6 != '\0') || ((param_5 & 0x100) != 0)) {
    if (((uint)param_2[2] < 0x1a) && (iVar6 = FUN_10006cf0(0x1a), iVar6 == 0)) {
      param_2[1] = 0;
      *(undefined2 *)*param_2 = 0;
    }
    else {
      puVar3 = (undefined4 *)*param_2;
      param_2[1] = 0xc;
      *(undefined2 *)(puVar3 + 6) = 0;
      *puVar3 = DAT_1006b6d4;
      puVar3[1] = DAT_1006b6d8;
      puVar3[2] = DAT_1006b6dc;
      puVar3[3] = DAT_1006b6e0;
      puVar3[4] = DAT_1006b6e4;
      puVar3[5] = DAT_1006b6e8;
    }
  }
  if (param_2[1] + -0xb < 0) {
LAB_100197ee:
    bVar4 = true;
  }
  else {
    iVar6 = _wcsnicmp((wchar_t *)(*param_2 + (param_2[1] + -0xb) * 2),L"firefox.exe",0xb);
    bVar4 = false;
    if (iVar6 != 0) goto LAB_100197ee;
  }
  if (!bVar4) {
    return param_2;
  }
LAB_100197f7:
  FUN_10010080(param_2,L"|%hu:%hu:%hu:%hu:%u:%u",0x2d,*(undefined2 *)(DAT_10074c4c + 0x1b30),
               *(undefined2 *)(DAT_10074c4c + 0x1b32),*(undefined2 *)(param_3 + 0x16),0,
               *(undefined4 *)(param_3 + 0x24),param_5);
  return param_2;
}


