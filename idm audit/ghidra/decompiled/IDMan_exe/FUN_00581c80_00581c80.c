// Function: FUN_00581c80
// Entry Point: 0x00581c80
// Program: IDMan.exe


void __fastcall FUN_00581c80(int param_1)

{
  undefined4 *puVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  void *pvVar6;
  char *pcVar7;
  size_t sVar8;
  char *pcVar9;
  char *pcVar10;
  
  iVar4 = *(int *)(param_1 + 0x3258);
  iVar3 = FUN_005803a0(iVar4);
  iVar4 = __memicmp((void *)(iVar4 + iVar3),"Content-Type:",0xd);
  if (iVar4 == 0) {
    iVar4 = *(int *)(param_1 + 0x3258) + 0xd + iVar3;
    iVar5 = FUN_005803a0(iVar4);
    pcVar9 = (char *)(iVar4 + iVar5);
    iVar4 = __memicmp(pcVar9,"multipart/byteranges",0x14);
    if ((((iVar4 == 0) || (iVar4 = __memicmp(pcVar9,"multipart/x-byteranges",0x16), iVar4 == 0)) &&
        (pcVar9 = _strchr(pcVar9,0x3b), pcVar9 != (char *)0x0)) &&
       ((pcVar9 = _strstr(pcVar9,"boundary"), pcVar9 != (char *)0x0 &&
        (pcVar9 = _strchr(pcVar9,0x3d), pcVar9 != (char *)0x0)))) {
      iVar4 = FUN_005803a0(pcVar9 + 1);
      pcVar10 = pcVar9 + 1 + iVar4;
      sVar8 = 0;
      pcVar9 = _strstr(pcVar10,"\r\n");
      if ((pcVar9 != (char *)0x0) || (pcVar9 = _strstr(pcVar10,"\n"), pcVar9 != (char *)0x0)) {
        sVar8 = (int)pcVar9 - (int)pcVar10;
      }
      pvVar6 = (void *)FUN_00402840(sVar8);
      _memcpy(pvVar6,pcVar10,sVar8);
      pcVar10 = *(char **)(param_1 + 0x327c);
      pcVar9 = pcVar10 + 1;
      do {
        cVar2 = *pcVar10;
        pcVar10 = pcVar10 + 1;
      } while (cVar2 != '\0');
      if (pcVar10 != pcVar9) {
        *(undefined1 *)(param_1 + 0x3280) = 1;
      }
    }
  }
  iVar4 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar3),"Transfer-Encoding:",0x12);
  if (iVar4 == 0) {
    pcVar9 = (char *)(*(int *)(param_1 + 0x3258) + 0x12 + iVar3);
    cVar2 = *(char *)(*(int *)(param_1 + 0x3258) + 0x12 + iVar3);
    while (cVar2 == ' ') {
      pcVar9 = pcVar9 + 1;
      cVar2 = *pcVar9;
    }
    iVar4 = __memicmp(pcVar9,"chunked",7);
    if (iVar4 == 0) {
      *(undefined1 *)(param_1 + 0xe0) = 1;
    }
  }
  iVar4 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar3),"Content-Encoding:",0x11);
  if (iVar4 == 0) {
    pcVar9 = (char *)(*(int *)(param_1 + 0x3258) + 0x11 + iVar3);
    cVar2 = *(char *)(*(int *)(param_1 + 0x3258) + 0x11 + iVar3);
    while (cVar2 == ' ') {
      pcVar9 = pcVar9 + 1;
      cVar2 = *pcVar9;
    }
    iVar3 = __memicmp(pcVar9,&DAT_006cf068,4);
    if (((iVar3 == 0) || (iVar3 = __memicmp(pcVar9,"x-gzip",6), iVar3 == 0)) ||
       ((iVar3 = __memicmp(pcVar9,"deflate",7), iVar3 == 0 ||
        (iVar3 = __memicmp(pcVar9,"x-deflate",9), iVar3 == 0)))) {
      *(undefined1 *)(param_1 + 0x21) = 1;
    }
    iVar3 = __memicmp(pcVar9,&DAT_006cf048,3);
    if (((iVar3 == 0) || (iVar3 = __memicmp(pcVar9,&DAT_006cf044,3), iVar3 == 0)) ||
       (iVar3 = __memicmp(pcVar9,"brotli",6), iVar3 == 0)) {
      *(undefined1 *)(param_1 + 0x20) = 1;
      *(undefined1 *)(param_1 + 0x3281) = 1;
      return;
    }
  }
  else {
    iVar4 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar3),"content-length:",0xf);
    if (iVar4 != 0) {
      if ((*(char *)(param_1 + 0x3210) == '\0') ||
         (iVar4 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar3),"Content-Type:",0xd),
         iVar4 != 0)) {
        iVar4 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar3),"ETag:",5);
        if (iVar4 != 0) {
          iVar4 = __memicmp((void *)(*(int *)(param_1 + 0x3258) + iVar3),"Content-Range:",0xe);
          if (iVar4 != 0) {
            return;
          }
          pcVar9 = _strstr((char *)(*(int *)(param_1 + 0x3258) + 0xe + iVar3),"bytes ");
          if (pcVar9 == (char *)0x0) {
            return;
          }
          cVar2 = pcVar9[6];
          if (((cVar2 != '\r') && (cVar2 != '\n')) && (cVar2 != '\0')) {
            puVar1 = (undefined4 *)(param_1 + 0x32a0);
            iVar4 = FID_conflict__sscanf(pcVar9 + 6,"%I64d",puVar1);
            if (iVar4 < 1) {
              *puVar1 = 0xffffffff;
              *(undefined4 *)(param_1 + 0x32a4) = 0xffffffff;
              if ((pcVar9[6] == '(') &&
                 (iVar4 = FID_conflict__sscanf(pcVar9 + 7,"%I64d",puVar1), iVar4 < 1)) {
                *puVar1 = 0xffffffff;
                *(undefined4 *)(param_1 + 0x32a4) = 0xffffffff;
              }
            }
          }
          pcVar9 = _strchr((char *)(*(int *)(param_1 + 0x3258) + 0xe + iVar3),0x2d);
          if (pcVar9 != (char *)0x0) {
            if ((pcVar9[1] != '\r') && (pcVar9[1] != '\n')) {
              iVar4 = FID_conflict__sscanf(pcVar9 + 1,"%I64d",(undefined4 *)(param_1 + 0x3298));
              if (iVar4 < 1) {
                *(undefined4 *)(param_1 + 0x3298) = 0;
                *(undefined4 *)(param_1 + 0x329c) = 0;
              }
            }
          }
          pcVar9 = _strchr((char *)(*(int *)(param_1 + 0x3258) + 0xe + iVar3),0x2f);
          if (pcVar9 == (char *)0x0) {
            return;
          }
          if (pcVar9[1] == '\r') {
            return;
          }
          if (pcVar9[1] == '\n') {
            return;
          }
          iVar3 = FID_conflict__sscanf(pcVar9 + 1,"%I64d",(undefined4 *)(param_1 + 0x3290));
          if (0 < iVar3) {
            return;
          }
          *(undefined4 *)(param_1 + 0x3290) = 0;
          *(undefined4 *)(param_1 + 0x3294) = 0;
          return;
        }
        iVar3 = *(int *)(param_1 + 0x3258) + 5 + iVar3;
        iVar4 = FUN_005803a0(iVar3);
        pcVar10 = (char *)(iVar3 + iVar4);
        sVar8 = 0;
        pcVar9 = _strstr(pcVar10,"\r\n");
        if ((pcVar9 != (char *)0x0) || (pcVar9 = _strstr(pcVar10,"\n"), pcVar9 != (char *)0x0)) {
          sVar8 = (int)pcVar9 - (int)pcVar10;
        }
      }
      else {
        iVar3 = *(int *)(param_1 + 0x3258) + 0xd + iVar3;
        iVar4 = FUN_005803a0(iVar3);
        pcVar10 = (char *)(iVar3 + iVar4);
        sVar8 = 0;
        pcVar9 = _strstr(pcVar10,"\r\n");
        pcVar7 = _strchr(pcVar10,0x3b);
        if ((((pcVar7 < pcVar9) && (pcVar7 != (char *)0x0)) || (pcVar9 == (char *)0x0)) &&
           (pcVar9 = pcVar7, pcVar7 == (char *)0x0)) {
          pcVar9 = _strstr(pcVar10,"\n");
          if (pcVar9 != (char *)0x0) {
            sVar8 = (int)pcVar9 - (int)pcVar10;
          }
        }
        else {
          sVar8 = (int)pcVar9 - (int)pcVar10;
        }
      }
      pvVar6 = (void *)FUN_00402840(sVar8);
      _memcpy(pvVar6,pcVar10,sVar8);
      return;
    }
    if ((*(char *)(param_1 + 0x3210) != '\0') || (*(int *)(*(int *)(param_1 + 200) + 0x348) != 0)) {
      FID_conflict__sscanf
                ((char *)(*(int *)(param_1 + 0x3258) + 0xf + iVar3),"%I64d",
                 (undefined4 *)(param_1 + 0x3220));
      if ((*(int *)(param_1 + 0x3224) < 1) && (*(int *)(param_1 + 0x3224) < 0)) {
        *(undefined4 *)(param_1 + 0x3220) = 0;
        *(undefined4 *)(param_1 + 0x3224) = 0;
        return;
      }
    }
  }
  return;
}


