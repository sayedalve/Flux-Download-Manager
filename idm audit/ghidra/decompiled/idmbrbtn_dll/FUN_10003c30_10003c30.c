// Function: FUN_10003c30
// Entry Point: 0x10003c30
// Program: idmbrbtn.dll


uint __thiscall FUN_10003c30(void *this,undefined4 *param_1,char param_2)

{
  short *psVar1;
  bool bVar2;
  char cVar3;
  short *in_EAX;
  int iVar4;
  int iVar5;
  int iVar6;
  uint3 uVar7;
  UINT uFlags;
  void *_Dst;
  uint uVar8;
  undefined4 *puVar9;
  bool bVar10;
  byte local_98;
  undefined4 local_94 [37];
  
  local_98 = 0;
  if ((((param_1 == (undefined4 *)0x0) || (*(char *)this != '\0')) ||
      (in_EAX = (short *)*param_1, in_EAX == (short *)0x0)) || (*in_EAX == 0)) goto LAB_10003f71;
  if ((*(char *)((int)this + 3) == '\0') ||
     (in_EAX = (short *)FUN_10009fd0((wchar_t *)param_1[9],L"TS|MP4|M4V|M4A|AAC|MP3|AC3|EAC3"),
     (char)in_EAX == '\0')) goto LAB_10003e07;
  iVar4 = wcsncmp((wchar_t *)*param_1,L"http:",5);
  if ((iVar4 == 0) || (iVar4 = wcsncmp((wchar_t *)*param_1,L"https:",6), iVar4 == 0)) {
    in_EAX = (short *)0x0;
    bVar2 = true;
    bVar10 = false;
LAB_10003cfe:
    cVar3 = '\0';
  }
  else {
    iVar4 = wcsncmp((wchar_t *)*param_1,L"hls2toidm:",10);
    bVar10 = iVar4 == 0;
    iVar4 = wcsncmp((wchar_t *)*param_1,L"hlstoidm:",9);
    in_EAX = (short *)CONCAT31((int3)((uint)iVar4 >> 8),bVar10);
    if (iVar4 != 0) {
      bVar2 = false;
      goto LAB_10003cfe;
    }
    cVar3 = '\x01';
    bVar2 = false;
  }
  if (((char)in_EAX == '\0') ||
     (in_EAX = (short *)CONCAT31((int3)((uint)in_EAX >> 8),2), DAT_1000f715 != '\0')) {
    in_EAX = (short *)CONCAT31((int3)((uint)in_EAX >> 8),cVar3);
  }
  if (((!bVar2) || (*(char *)((int)this + 0x660) == '\0')) &&
     ((cVar3 == '\0' || (*(char *)((int)this + 0x660) < '\x02')))) {
    if ((!bVar2) && (*(char *)((int)this + 0x660) < (char)in_EAX)) {
      uVar8 = 0;
      *(char *)((int)this + 0x660) = (char)in_EAX;
      if (*(int *)((int)this + 0x6f4) != 0) {
        iVar4 = 0;
        do {
          iVar6 = *(int *)((int)this + 0x6fc);
          in_EAX = (short *)FUN_10009fd0(*(wchar_t **)(iVar6 + 0x2c + iVar4),
                                         L"TS|MP4|M4V|M4A|AAC|MP3|AC3|EAC3");
          if ((char)in_EAX != '\0') {
            iVar5 = wcsncmp(*(wchar_t **)(iVar6 + iVar4),L"hls2toidm:",10);
            in_EAX = (short *)0x0;
            if (iVar5 != 0) {
              if (!bVar10) {
                iVar6 = wcsncmp(*(wchar_t **)(iVar6 + iVar4),L"hlstoidm:",9);
                in_EAX = (short *)0x0;
                if (iVar6 == 0) goto LAB_10003ded;
              }
              FUN_10008f50((void *)((int)this + 0x6f0),uVar8);
              *(int *)((int)this + 0x6f4) = *(int *)((int)this + 0x6f4) + -1;
              _Dst = (void *)(*(int *)((int)this + 0x6fc) + iVar4);
              in_EAX = memmove(_Dst,(void *)((int)_Dst + 400),
                               (*(int *)((int)this + 0x6f4) - uVar8) * 400);
              uVar8 = uVar8 - 1;
              iVar4 = iVar4 + -400;
            }
          }
LAB_10003ded:
          uVar8 = uVar8 + 1;
          iVar4 = iVar4 + 400;
        } while (uVar8 < *(uint *)((int)this + 0x6f4));
      }
    }
LAB_10003e07:
    if ((*(char *)((int)this + 0x6e3) != '\0') && ((param_1[0x22] & 0x400) != 0)) {
      *(undefined1 *)((int)this + 0x6e3) = 0;
    }
    if ((*(char *)((int)this + 0x6e5) != '\0') && ((*(byte *)(param_1 + 0x22) & 0x80) == 0)) {
      *(undefined1 *)((int)this + 0x6e5) = 0;
    }
    if (*(uint *)((int)this + 0x6f4) < 0x100) {
      if (*(char *)((int)this + 1) != '\0') {
        memset(local_94,0,0x90);
        puVar9 = local_94;
        for (iVar4 = 0x1a; iVar4 != 0; iVar4 = iVar4 + -1) {
          *puVar9 = *param_1;
          param_1 = param_1 + 1;
          puVar9 = puVar9 + 1;
        }
        param_1 = local_94;
      }
      in_EAX = (short *)FUN_10009090((void *)((int)this + 0x6f0),param_1);
      local_98 = (byte)((uint)in_EAX >> 0x1f) ^ 1;
      if (-1 < (int)in_EAX) {
        if ((*(char *)((int)this + 0x6e4) != '\0') &&
           (psVar1 = *(short **)((int)this + 0x6fc), iVar4 = (int)in_EAX * 200, in_EAX = psVar1,
           *(int *)(psVar1 + iVar4 + 0x28) < 3)) {
          *(undefined1 *)((int)this + 0x6e4) = 0;
        }
        if (param_2 == '\0') {
          if ((*(char *)((int)this + 0x6e2) != '\0') && (*(char *)((int)this + 3) == '\0')) {
            *(undefined1 *)((int)this + 0x6e2) = 0;
          }
          FUN_100073f0(this);
          in_EAX = (short *)FUN_10005b60((int)this);
        }
      }
    }
    uVar7 = (uint3)((uint)in_EAX >> 8);
    if ((((*(char *)((int)this + 0x6e3) == '\0') && (*(char *)((int)this + 0x6e1) == '\0')) &&
        (*(char *)((int)this + 0x6e2) == '\0')) &&
       ((*(char *)((int)this + 0x6e4) == '\0' && (*(char *)((int)this + 0x6e5) == '\0')))) {
      iVar4 = CONCAT31(uVar7,1);
    }
    else {
      iVar4 = (uint)uVar7 << 8;
    }
    cVar3 = (char)iVar4;
    if ((*(char *)((int)this + 7) != cVar3) && (*(char *)((int)this + 7) = cVar3, cVar3 != '\0')) {
      FUN_10005a80(this,'\0');
      uFlags = 0x80;
      if (*(char *)((int)this + 7) != '\0') {
        uFlags = 0x40;
      }
      iVar4 = SetWindowPos(*(HWND *)((int)this + 0x710),(HWND)0x0,*(int *)((int)this + 0x678),
                           *(int *)((int)this + 0x67c),*(int *)((int)this + 0x680),
                           *(int *)((int)this + 0x684),uFlags);
    }
    return CONCAT31((int3)((uint)iVar4 >> 8),local_98);
  }
LAB_10003f71:
  return (uint)in_EAX & 0xffffff00;
}


