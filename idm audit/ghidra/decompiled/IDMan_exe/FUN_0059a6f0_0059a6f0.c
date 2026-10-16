// Function: FUN_0059a6f0
// Entry Point: 0x0059a6f0
// Program: IDMan.exe


/* WARNING: Removing unreachable block (ram,0x0059b858) */
/* WARNING: Removing unreachable block (ram,0x0059b245) */
/* WARNING: Removing unreachable block (ram,0x0059b80e) */
/* WARNING: Removing unreachable block (ram,0x0059b8a3) */

void __fastcall FUN_0059a6f0(int param_1)

{
  uint *puVar1;
  fd_set *exceptfds;
  byte bVar2;
  int iVar3;
  LPCWSTR lpFileName;
  undefined4 *puVar4;
  uint uVar5;
  char cVar6;
  char cVar7;
  int iVar8;
  char *pcVar9;
  char *pcVar10;
  byte *pbVar11;
  byte *pbVar12;
  wchar_t *pwVar13;
  HANDLE hFile;
  DWORD DVar14;
  undefined4 uVar15;
  BOOL BVar16;
  byte *pbVar17;
  int *piVar18;
  uint uVar19;
  int iVar20;
  bool bVar21;
  float10 fVar22;
  longlong lVar23;
  longlong lVar24;
  undefined8 uVar25;
  uint uStack_140;
  int local_130 [3];
  undefined4 auStack_124 [2];
  undefined4 uStack_11c;
  undefined4 uStack_118;
  undefined4 uStack_114;
  int iStack_10c;
  int iStack_108;
  int iStack_104;
  undefined4 uStack_100;
  undefined4 uStack_fc;
  undefined4 uStack_f4;
  undefined4 uStack_f0;
  undefined4 uStack_ec;
  undefined4 uStack_e8;
  undefined4 uStack_e4;
  undefined4 local_dc;
  undefined4 local_d8;
  int iStack_d4;
  ushort uStack_d0;
  int iStack_c8;
  ushort uStack_c4;
  uint local_bc;
  uint local_b8;
  uint local_b4;
  uint local_b0;
  int local_ac;
  ushort uStack_a8;
  uint local_a0;
  uint local_9c;
  int iStack_98;
  ushort uStack_94;
  undefined1 *local_8c;
  void *local_88;
  undefined1 *puStack_84;
  uint local_80;
  char *local_7c;
  int iStack_78;
  timeval local_74;
  int iStack_6c;
  int iStack_68;
  timeval tStack_64;
  uint local_5c;
  int local_58;
  HANDLE pvStack_54;
  uint local_50;
  int local_4c;
  int local_48;
  undefined8 uStack_44;
  uint uStack_3c;
  int iStack_38;
  char *pcStack_34;
  int iStack_30;
  WPARAM local_2c;
  int local_28;
  uint local_24;
  int local_20;
  int iStack_1c;
  int iStack_18;
  char local_13;
  char cStack_12;
  char local_11;
  char *local_10;
  undefined4 local_c;
  char cStack_5;
  
  puStack_84 = &LAB_0067fee8;
  local_88 = ExceptionList;
  uStack_140 = DAT_007839c0 ^ (uint)&local_7c;
  local_8c = (undefined1 *)&uStack_140;
  ExceptionList = &local_88;
  local_80 = 0;
  iVar8 = *(int *)(param_1 + 0xc);
  iVar20 = *(int *)(iVar8 + 0x348);
  local_48 = param_1;
  if (((iVar20 == 0x80) || (*(int *)(iVar8 + 0x344) == 1)) ||
     ((iVar20 == 8 && ((*(int *)(iVar8 + 0x33c) != 8 && (*(char *)(iVar8 + 0x19d) != '\0')))))) {
    *(undefined4 *)(param_1 + 0x28) = 0;
    puVar1 = &uStack_140;
  }
  else {
    iVar3 = *(int *)(iVar8 + 0x33c);
    if (((iVar3 == 4) ||
        ((((iVar20 == 0x40 && (*(char *)(iVar8 + 0x19f) != '\0')) || (iVar3 == 8)) ||
         (iVar20 == 0x800)))) || ((iVar3 == 0x40 && (*(char *)(iVar8 + 0x19f) != '\0')))) {
      *(undefined4 *)(param_1 + 0x28) = 0;
      puVar1 = &uStack_140;
      if ((*(int *)(iVar8 + 0x330) != 0) && (puVar1 = &uStack_140, *(int *)(param_1 + 0x240) != 0))
      {
        puVar1 = (uint *)(*(int *)(iVar8 + 0x330) + 0x184);
        *puVar1 = *puVar1 | 0x100;
        iVar8 = *(int *)(*(int *)(param_1 + 0xc) + 0x330);
        puVar1 = &uStack_140;
        if ((*(int *)(iVar8 + 0x96c) == 3) &&
           ((lpFileName = *(LPCWSTR *)(iVar8 + 0x50), puVar1 = &uStack_140,
            lpFileName == (LPCWSTR)0x0 ||
            (DVar14 = GetFileAttributesW(lpFileName), puVar1 = (uint *)local_8c,
            DVar14 == 0xffffffff)))) {
          local_8c = (undefined1 *)puVar1;
          *(undefined4 *)(*(int *)(*(int *)(param_1 + 0xc) + 0x330) + 0x96c) = 0;
          puVar1 = (uint *)local_8c;
        }
        local_8c = (undefined1 *)puVar1;
        iVar8 = *(int *)(*(int *)(param_1 + 0xc) + 0x330);
        iVar20 = *(int *)(iVar8 + 0x96c);
        puVar1 = (uint *)local_8c;
        if ((((iVar20 == 0) || (iVar20 == 2)) || (iVar20 == 4)) && (*(int *)(iVar8 + 0x970) == 0)) {
          FUN_005bacf0("Starting audio thread\r\n");
          if (*(int *)(param_1 + 0x260) != 0) {
            *(undefined1 *)(*(int *)(*(int *)(param_1 + 0xc) + 0x330) + 0xb17) =
                 *(undefined1 *)(*(int *)(param_1 + 0x260) + 0xb17);
            *(undefined4 *)(*(int *)(*(int *)(param_1 + 0xc) + 0x330) + 8) =
                 *(undefined4 *)(*(int *)(param_1 + 0x260) + 8);
          }
          pcVar9 = *(char **)((*(undefined4 **)(param_1 + 0xc))[0xcc] + 0x20);
          if ((pcVar9 != (char *)0x0) &&
             (iVar8 = __stricmp((char *)**(undefined4 **)(param_1 + 0xc),pcVar9), iVar8 == 0)) {
            *(undefined1 *)(*(int *)(*(int *)(param_1 + 0xc) + 0x330) + 0x390) =
                 *(undefined1 *)(*(int *)(param_1 + 0xc) + 0x370);
          }
          FUN_005cefd0(1);
          puVar1 = (uint *)local_8c;
        }
      }
    }
    else {
      local_8c = (undefined1 *)&uStack_140;
      FUN_004024f0();
      local_80 = CONCAT31(local_80._1_3_,1);
      FUN_00467f20(**(undefined4 **)(param_1 + 0xc),&local_10);
      iVar8 = __stricmp(local_10,"googleusercontent.com");
      if ((((*(int *)(param_1 + 0x1c0) == 0 && *(int *)(param_1 + 0x1c4) == 0) &&
           (*(char *)(param_1 + 0x888) == '\0')) &&
          ((1 < *(int *)(param_1 + 0x1ac) &&
           ((*(char *)(*(int *)(param_1 + 0xc) + 0x17b) != '\0' &&
            (*(int *)(*(int *)(param_1 + 0xc) + 0x348) == 0)))))) &&
         ((*(char *)(*(int *)(param_1 + 0x274) + 0xe0) == '\0' || (iVar8 == 0)))) {
        *(undefined4 *)(*(int *)(param_1 + 0x274) + 0x35a4) = 1;
        iVar8 = *(int *)(param_1 + 0x274);
        *(undefined4 *)(iVar8 + 0x3588) = 0;
        *(undefined4 *)(iVar8 + 0x358c) = 0;
        iVar8 = *(int *)(param_1 + 0x274);
        *(undefined4 *)(iVar8 + 0x3568) = 0;
        *(undefined4 *)(iVar8 + 0x356c) = 0;
        *(undefined1 *)(*(int *)(param_1 + 0x274) + 0x6718) = 1;
        (**(code **)(*(int *)(*(int *)(param_1 + 0x274) + 0x3508) + 0x18))
                  (*(int *)(param_1 + 0x274) + 0x6708);
        for (local_bc = 0;
            (local_bc < *(uint *)(param_1 + 0x47c) &&
            (*(int *)(param_1 + 0x480 + local_bc * 4) !=
             *(int *)(*(int *)(param_1 + 0x274) + 0x6708))); local_bc = local_bc + 1) {
        }
        if ((local_bc == *(uint *)(param_1 + 0x47c)) && (*(uint *)(param_1 + 0x47c) < 0x80)) {
          *(undefined4 *)(param_1 + 0x480 + local_bc * 4) =
               *(undefined4 *)(*(int *)(param_1 + 0x274) + 0x6708);
          *(int *)(param_1 + 0x47c) = *(int *)(param_1 + 0x47c) + 1;
        }
        for (local_9c = 0;
            (local_9c < *(uint *)(param_1 + 0x680) &&
            (*(int *)(param_1 + 0x684 + local_9c * 4) !=
             *(int *)(*(int *)(param_1 + 0x274) + 0x6708))); local_9c = local_9c + 1) {
        }
        if ((local_9c == *(uint *)(param_1 + 0x680)) && (*(uint *)(param_1 + 0x680) < 0x80)) {
          *(undefined4 *)(param_1 + 0x684 + local_9c * 4) =
               *(undefined4 *)(*(int *)(param_1 + 0x274) + 0x6708);
          *(int *)(param_1 + 0x680) = *(int *)(param_1 + 0x680) + 1;
        }
        *(undefined4 *)(param_1 + 0x1b8) = 2;
        *(undefined4 *)(param_1 + 0x1b0) = 2;
      }
      local_80 = local_80 & 0xffffff00;
      FUN_00402560();
      puVar1 = (uint *)local_8c;
    }
  }
  local_8c = (undefined1 *)puVar1;
  iVar8 = *(int *)(param_1 + 0xc);
  if ((*(int *)(iVar8 + 0x348) != 0) && (*(char *)(iVar8 + 0x34c) != '\0')) {
    *(undefined4 *)(iVar8 + 0xb0) = 1;
  }
  local_11 = '\0';
  pcVar9 = _strstr((char *)**(undefined4 **)(param_1 + 0xc),".googlevideo.com");
  if (((pcVar9 != (char *)0x0) ||
      (pcVar9 = _strstr((char *)**(undefined4 **)(param_1 + 0xc),".drive.google.com"),
      pcVar9 != (char *)0x0)) &&
     (piVar18 = *(int **)(*(int *)(param_1 + 0xc) + 8), piVar18 != (int *)0x0)) {
    pcVar9 = "videoplayback?";
    for (uVar19 = 0xe; 3 < uVar19; uVar19 = uVar19 - 4) {
      if (*piVar18 != *(int *)pcVar9) goto LAB_0059ae06;
      pcVar9 = (char *)((int)pcVar9 + 4);
      piVar18 = piVar18 + 1;
    }
    if (((char)*(int *)pcVar9 == (char)*piVar18) &&
       (*(char *)((int)pcVar9 + 1) == *(char *)((int)piVar18 + 1))) {
      *(undefined1 *)(param_1 + 600) = 1;
      iVar8 = *(int *)(param_1 + 0xc);
      if ((*(int *)(iVar8 + 0x33c) == 4) &&
         ((*(int *)(param_1 + 0x40) == 0 && (*(char *)(iVar8 + 0x34c) == '\0')))) {
        if ((-1 < *(int *)(param_1 + 0x1c4)) &&
           ((0 < *(int *)(param_1 + 0x1c4) || (0x1000000 < *(uint *)(param_1 + 0x1c0))))) {
          if (*(int *)(iVar8 + 0x108) == 0) {
            pwVar13 = (wchar_t *)(iVar8 + 0x10c);
          }
          else {
            pwVar13 = *(wchar_t **)(iVar8 + 0x108);
          }
          pwVar13 = _wcsrchr(pwVar13,L'.');
          iVar8 = __stricmp(*(char **)(*(int *)(param_1 + 0xc) + 0xa4),".mp4");
          if (((iVar8 == 0) ||
              ((pwVar13 != (wchar_t *)0x0 && (iVar8 = __wcsicmp(pwVar13,L".mp4"), iVar8 == 0)))) ||
             ((pcVar9 = *(char **)(*(int *)(param_1 + 0xc) + 0xbc), pcVar9 != (char *)0x0 &&
              ((iVar8 = __stricmp(pcVar9,"video/mp4"), iVar8 == 0 ||
               (iVar8 = __stricmp(*(char **)(*(int *)(param_1 + 0xc) + 0xbc),"audio/mp4"),
               iVar8 == 0)))))) {
            local_11 = '\x01';
          }
          else {
            iVar8 = *(int *)(*(int *)(param_1 + 0xc) + 8);
            if (iVar8 != 0) {
              uVar15 = FUN_00457940(iVar8);
              iVar8 = *(int *)(param_1 + 0x274);
              iVar8 = FUN_0055a070(uVar15,0,*(uint *)(iVar8 + 0x80) + *(uint *)(iVar8 + 0x60),
                                   *(int *)(iVar8 + 0x84) + *(int *)(iVar8 + 100) +
                                   (uint)CARRY4(*(uint *)(iVar8 + 0x80),*(uint *)(iVar8 + 0x60)),
                                   *(undefined4 *)(param_1 + 0x1c0),*(undefined4 *)(param_1 + 0x1c4)
                                   ,iVar8 + 0x78);
              if (iVar8 != 0) {
                iVar8 = *(int *)(param_1 + 0x274);
                if (-1 < *(int *)(iVar8 + 0x6c)) {
                  iVar20 = *(int *)(iVar8 + 0x6c) - (uint)(*(uint *)(iVar8 + 0x68) < 0x40);
                  if ((iVar20 < *(int *)(iVar8 + 0x7c)) ||
                     ((iVar20 <= *(int *)(iVar8 + 0x7c) &&
                      (*(uint *)(iVar8 + 0x68) - 0x40 <= *(uint *)(iVar8 + 0x78)))))
                  goto LAB_0059ae06;
                }
                FUN_00582fd0();
                iVar8 = *(int *)(param_1 + 0x274);
                puVar1 = (uint *)(iVar8 + 0x60);
                uVar19 = *puVar1;
                *puVar1 = *puVar1 + *(uint *)(iVar8 + 0x80);
                *(int *)(iVar8 + 100) =
                     *(int *)(iVar8 + 100) + *(int *)(iVar8 + 0x84) +
                     (uint)CARRY4(uVar19,*(uint *)(iVar8 + 0x80));
                iVar8 = *(int *)(param_1 + 0x274);
                *(undefined4 *)(iVar8 + 0x80) = 0;
                *(undefined4 *)(iVar8 + 0x84) = 0;
                (**(code **)(**(int **)(param_1 + 0x274) + 0x18))
                          (*(int **)(param_1 + 0x274) + 0xc80);
                *(undefined4 *)(param_1 + 0x278) = 0;
                *(undefined4 *)(param_1 + 0x47c) = 0;
                *(undefined4 *)(param_1 + 0x680) = 0;
                for (local_b0 = 0;
                    (local_b0 < *(uint *)(param_1 + 0x47c) &&
                    (*(int *)(param_1 + 0x480 + local_b0 * 4) !=
                     *(int *)(*(int *)(param_1 + 0x274) + 0x3200))); local_b0 = local_b0 + 1) {
                }
                if ((local_b0 == *(uint *)(param_1 + 0x47c)) && (*(uint *)(param_1 + 0x47c) < 0x80))
                {
                  *(undefined4 *)(param_1 + 0x480 + local_b0 * 4) =
                       *(undefined4 *)(*(int *)(param_1 + 0x274) + 0x3200);
                  *(int *)(param_1 + 0x47c) = *(int *)(param_1 + 0x47c) + 1;
                }
                for (local_a0 = 0;
                    (local_a0 < *(uint *)(param_1 + 0x680) &&
                    (*(int *)(param_1 + 0x684 + local_a0 * 4) !=
                     *(int *)(*(int *)(param_1 + 0x274) + 0x3200))); local_a0 = local_a0 + 1) {
                }
                if ((local_a0 == *(uint *)(param_1 + 0x680)) && (*(uint *)(param_1 + 0x680) < 0x80))
                {
                  *(undefined4 *)(param_1 + 0x684 + local_a0 * 4) =
                       *(undefined4 *)(*(int *)(param_1 + 0x274) + 0x3200);
                  *(int *)(param_1 + 0x680) = *(int *)(param_1 + 0x680) + 1;
                }
                iVar8 = *(int *)(param_1 + 0x884);
                if (iVar8 != -1) {
                  for (local_b4 = 0;
                      (local_b4 < *(uint *)(param_1 + 0x278) &&
                      (*(int *)(param_1 + 0x27c + local_b4 * 4) != iVar8)); local_b4 = local_b4 + 1)
                  {
                  }
                  if ((local_b4 == *(uint *)(param_1 + 0x278)) &&
                     (*(uint *)(param_1 + 0x278) < 0x80)) {
                    *(int *)(param_1 + 0x27c + local_b4 * 4) = iVar8;
                    *(int *)(param_1 + 0x278) = *(int *)(param_1 + 0x278) + 1;
                  }
                  for (local_b8 = 0;
                      (local_b8 < *(uint *)(param_1 + 0x680) &&
                      (*(int *)(param_1 + 0x684 + local_b8 * 4) != *(int *)(param_1 + 0x884)));
                      local_b8 = local_b8 + 1) {
                  }
                  if ((local_b8 == *(uint *)(param_1 + 0x680)) &&
                     (*(uint *)(param_1 + 0x680) < 0x80)) {
                    *(undefined4 *)(param_1 + 0x684 + local_b8 * 4) =
                         *(undefined4 *)(param_1 + 0x884);
                    *(int *)(param_1 + 0x680) = *(int *)(param_1 + 0x680) + 1;
                  }
                }
              }
            }
          }
        }
      }
    }
  }
LAB_0059ae06:
  FUN_005bbe30(DAT_00789d54,1);
  if (*(int *)(param_1 + 0x260) != 0) {
    FUN_005befd0();
  }
  FUN_005baea0();
  local_2c = 3000;
  FUN_005bb420();
  *(int *)(param_1 + 0x1bc) = *(int *)(param_1 + 0x1bc) + -5;
  FUN_004024f0();
  local_80 = CONCAT31(local_80._1_3_,2);
  local_24 = 0;
  local_20 = 0;
  local_50 = *(uint *)(param_1 + 0x1c8);
  local_4c = *(int *)(param_1 + 0x1cc);
  FUN_005bb560();
  local_74.tv_sec = (*(char *)(param_1 + 600) == '\0') + 2;
  local_74.tv_usec = 0;
  FUN_00640553(local_130);
  FUN_00640553(&local_ac);
  local_5c = 0;
  local_58 = 0;
  local_2c = 0x14db;
  local_13 = '\x01';
  local_28 = 0;
  iVar8 = 0;
  local_dc = 0;
  local_d8 = 0;
  for (iVar20 = 0; iVar20 < *(int *)(param_1 + 0x1b0); iVar20 = iVar20 + 1) {
    if (*(int *)(*(int *)(param_1 + 0x274) + iVar20 * 0x3508 + 0x9c) != 0) {
      iVar8 = iVar8 + 1;
      cVar6 = FUN_005964b0();
      if (cVar6 != '\0') {
        local_28 = local_28 + 1;
      }
    }
  }
  if ((local_28 == 1) && (iVar8 == 1)) {
    local_13 = 0;
    local_74.tv_sec = 1;
    FUN_0059af00();
    return;
  }
  do {
    if (*(int *)(param_1 + 0x1c) != 0) {
      FUN_0055e8b0();
      iVar8 = *(int *)(param_1 + 0xc);
      if (*(int *)(iVar8 + 0x348) == 0x20) {
        uStack_f4 = *(undefined4 *)(iVar8 + 0xf0);
        uStack_f0 = *(undefined4 *)(iVar8 + 0xf4);
      }
      else {
        uStack_f4 = *(undefined4 *)(param_1 + 0x1c0);
        uStack_f0 = *(undefined4 *)(param_1 + 0x1c4);
      }
      uStack_ec = *(undefined4 *)(param_1 + 0x1c8);
      uStack_e8 = *(undefined4 *)(param_1 + 0x1cc);
      uStack_e4 = 0;
      SendMessageA((HWND)**(undefined4 **)(param_1 + 0x10),0x111,0x1471,(LPARAM)&uStack_f4);
LAB_0059af6d:
      local_80 = local_80 & 0xffffff00;
      FUN_00402560();
      FUN_0059c706();
      return;
    }
    cVar6 = FUN_005baa90();
    iStack_1c = CONCAT31(iStack_1c._1_3_,cVar6);
    if (cVar6 != '\0') {
      FUN_00640553(&iStack_c8);
      local_5c = *(uint *)(param_1 + 0x1c8);
      local_58 = *(int *)(param_1 + 0x1cc);
    }
    iVar8 = 0;
    for (iVar20 = 0; iVar20 < *(int *)(param_1 + 0x1b0); iVar20 = iVar20 + 1) {
      if ((*(int *)(iVar20 * 0x3508 + *(int *)(param_1 + 0x274) + 0x9c) != 0) &&
         (cVar6 = FUN_0057f540(), cVar6 != '\0')) {
        iVar8 = 1;
        break;
      }
    }
    cStack_5 = '\0';
    if (local_13 == '\0') {
      iVar8 = 1;
      local_74.tv_sec = (*(char *)(param_1 + 600) == '\0') + 2;
      local_13 = '\x01';
    }
    else if (iVar8 == 0) {
      iVar8 = select(0,(fd_set *)(param_1 + 0x278),(fd_set *)(param_1 + 0x47c),
                     (fd_set *)(param_1 + 0x680),&local_74);
      cStack_5 = '\x01';
    }
    if (*(int *)(param_1 + 0x1c) != 0) {
LAB_0059b03c:
      FUN_0055e8b0();
      goto LAB_0059af6d;
    }
    if ((*(int *)(param_1 + 0x1c8) == 0 && *(int *)(param_1 + 0x1cc) == 0) &&
       (FUN_00640553(&local_ac), 300 < local_ac - local_130[0])) {
      local_80 = local_80 & 0xffffff00;
      FUN_00402560();
      FUN_0059c706();
      return;
    }
    uStack_3c = *(uint *)(param_1 + 0x1c8);
    iStack_38 = *(int *)(param_1 + 0x1cc);
    if ((iVar8 == -1) || (iVar8 == 0)) {
      FUN_00596e20();
    }
    else {
      for (iVar8 = 0; iVar8 < *(int *)(param_1 + 0x1b0); iVar8 = iVar8 + 1) {
        if (*(int *)(*(int *)(param_1 + 0x274) + iVar8 * 0x3508 + 0x9c) != 0) {
          if (*(int *)(param_1 + 0x1c) != 0) goto LAB_0059b03c;
          if ((cStack_5 != '\0') || (cVar6 = FUN_0057f540(), cVar6 != '\0')) {
            FUN_0058fdf0();
          }
        }
      }
    }
    FUN_005bb600(0,iStack_1c);
    FUN_00640553(&iStack_98);
    if (0xf9 < (int)(((iStack_98 - local_ac) * 1000 - (uint)uStack_a8) + (uint)uStack_94)) {
      FUN_00640553(&local_ac);
      for (iVar8 = 0; iVar8 < *(int *)(param_1 + 0x1b0); iVar8 = iVar8 + 1) {
        if (*(int *)(iVar8 * 0x3508 + *(int *)(param_1 + 0x274) + 0x9c) != 0) {
          FUN_0057c100();
        }
      }
    }
    if ((*(char *)(*(int *)(param_1 + 0xc) + 0x17c) != '\0') &&
       (*(int *)(*(int *)(param_1 + 0x274) + 0x88) != 0 ||
        *(int *)(*(int *)(param_1 + 0x274) + 0x8c) != 0)) {
      *(undefined1 *)(*(int *)(param_1 + 0xc) + 0x17c) = 0;
    }
    if (*(int *)(param_1 + 0x24) == 0) {
      iVar8 = *(int *)(param_1 + 0x1cc);
      if ((iVar8 < local_4c) ||
         ((((iVar8 <= local_4c && (*(uint *)(param_1 + 0x1c8) <= local_50)) ||
           (uStack_3c != *(uint *)(param_1 + 0x1c8))) || (iStack_38 != iVar8)))) goto LAB_0059b2f3;
      if (local_24 == 0 && local_20 == 0) {
        local_24 = iStack_98 * 1000 + (uint)uStack_94;
        local_20 = (int)local_24 >> 0x1f;
      }
      else {
        uVar19 = iStack_98 * 1000 + (uint)uStack_94;
        fVar22 = (float10)FUN_0057c0c0();
        uStack_44 = (double)fVar22;
        FUN_0057c080();
        lVar23 = FUN_0063ecb6();
        if (lVar23 < CONCAT44((((int)uVar19 >> 0x1f) - local_20) - (uint)(uVar19 < local_24),
                              uVar19 - local_24)) {
          local_24 = 0;
          local_20 = 0;
          iVar8 = FUN_005d0440();
          if (iVar8 != 0) {
            if (*(int *)(param_1 + 0x1c) != 0) goto LAB_0059af6d;
            FUN_005bacf0("Restart type s018\n");
            FUN_00582fd0();
            FUN_00580b80();
            iVar8 = (**(code **)(**(int **)(param_1 + 0x274) + 0x18))
                              (*(int **)(param_1 + 0x274) + 0xc80);
            if ((iVar8 != 0) || (iVar8 = FUN_00592d50(0), iVar8 != 0)) goto LAB_0059b2bc;
            local_50 = *(uint *)(param_1 + 0x1c8);
            local_4c = *(int *)(param_1 + 0x1cc);
            FUN_005bbe30(DAT_00789d54,1);
          }
        }
      }
    }
    else {
LAB_0059b2f3:
      local_24 = 0;
      local_20 = 0;
    }
    puVar4 = *(undefined4 **)(param_1 + 0xc);
    if (puVar4[0x2c] == 2) {
      bVar21 = true;
      cStack_5 = '\x01';
      uVar19 = *(uint *)(param_1 + 0x1c0);
      iVar8 = *(int *)(param_1 + 0x1c4);
      if ((iVar8 < 0) || ((iVar8 < 1 && (uVar19 < 0x33)))) {
LAB_0059b341:
        if ((iVar8 < 1) && ((iVar8 < 0 || (uVar19 < 0x33)))) {
          iVar20 = *(int *)(param_1 + 0x274);
          if ((*(int *)(iVar20 + 0x8c) <= iVar8) &&
             ((*(int *)(iVar20 + 0x8c) < iVar8 || (*(uint *)(iVar20 + 0x88) < uVar19))))
          goto LAB_0059b36a;
          bVar21 = true;
        }
      }
      else {
        iVar20 = *(int *)(param_1 + 0x274);
        if ((-1 < *(int *)(iVar20 + 0x8c)) &&
           ((0 < *(int *)(iVar20 + 0x8c) || (0x31 < *(uint *)(iVar20 + 0x88))))) goto LAB_0059b341;
LAB_0059b36a:
        bVar21 = false;
        if (((*(char *)(iVar20 + 0xe0) != '\0') &&
            (*(int *)(iVar20 + 0x58) == 0 && *(int *)(iVar20 + 0x5c) == 0)) &&
           (*(char *)(iVar20 + 0xe1) != '\0')) {
          bVar21 = true;
        }
      }
      if ((puVar4[0x59] & 0x100) != 0) {
        if ((((*(int *)(param_1 + 0x1cc) < 1) &&
             ((*(int *)(param_1 + 0x1cc) < 0 || (*(uint *)(param_1 + 0x1c8) < 0x7d1)))) &&
            ((pcVar9 = _strstr((char *)*puVar4,".googlevideo.com"), pcVar9 != (char *)0x0 ||
             (pcVar9 = _strstr((char *)**(undefined4 **)(param_1 + 0xc),".drive.google.com"),
             pcVar9 != (char *)0x0)))) &&
           (piVar18 = *(int **)(*(int *)(param_1 + 0xc) + 8), piVar18 != (int *)0x0)) {
          pcVar9 = "videoplayback?";
          for (uVar19 = 0xe; 3 < uVar19; uVar19 = uVar19 - 4) {
            if (*piVar18 != *(int *)pcVar9) goto LAB_0059b44d;
            pcVar9 = (char *)((int)pcVar9 + 4);
            piVar18 = piVar18 + 1;
          }
          if (((char)*(int *)pcVar9 == (char)*piVar18) &&
             (*(char *)((int)pcVar9 + 1) == *(char *)((int)piVar18 + 1))) {
            iVar8 = *(int *)(param_1 + 0x1c4);
            if ((iVar8 < 0) ||
               (((iVar8 < 1 && (*(uint *)(param_1 + 0x1c0) < 0x7d1)) ||
                (*(uint *)(param_1 + 0x1c0) == 0 && iVar8 == 0)))) goto LAB_0059b764;
          }
        }
      }
LAB_0059b44d:
      if (bVar21) {
        iStack_18 = FUN_005daba0(*(undefined1 *)(param_1 + 0x25e));
        iVar8 = *(int *)(param_1 + 0xc);
        if ((((*(char *)(iVar8 + 0x36a) != '\0') && (cVar6 = FUN_0057c210(), cVar6 != '\0')) ||
            (*(char *)(param_1 + 0x888) != '\0')) &&
           ((*(int *)(iVar8 + 0xb0) != 2 || (iStack_18 != 0)))) {
          FUN_00582fd0();
          FUN_0055f780();
          *(undefined4 *)(param_1 + 200) = 0;
          *(undefined4 *)(param_1 + 0xcc) = 0;
          *(undefined4 *)(param_1 + 0xd0) = 0;
          *(undefined4 *)(param_1 + 0xd4) = 0;
          *(undefined4 *)(param_1 + 0x1c8) = 0;
          *(undefined4 *)(param_1 + 0x1cc) = 0;
          if ((*(char *)(*(int *)(param_1 + 0xc) + 0x36a) == '\0') ||
             (cVar6 = FUN_0057c210(), cVar6 == '\0')) goto LAB_0059ba89;
          goto LAB_0059af6d;
        }
        if (0 < *(int *)(param_1 + 0xb8)) {
          pcVar9 = *(char **)(iVar8 + 0xcc);
          pcVar10 = pcVar9;
          do {
            cVar6 = *pcVar10;
            pcVar10 = pcVar10 + 1;
          } while (cVar6 != '\0');
          if (pcVar10 != pcVar9 + 1) {
            if (*(void **)(iVar8 + 0xbc) != (void *)0x0) {
              pcVar10 = pcVar9;
              do {
                cVar6 = *pcVar10;
                pcVar10 = pcVar10 + 1;
              } while (cVar6 != '\0');
              iVar8 = __memicmp(pcVar9,*(void **)(iVar8 + 0xbc),(int)pcVar10 - (int)(pcVar9 + 1));
              if (iVar8 == 0) goto LAB_0059b58a;
            }
            if (**(int **)(param_1 + 0x14) != 0) {
              SendMessageA((HWND)**(int **)(param_1 + 0x14),0x111,local_2c,
                           *(LPARAM *)(param_1 + 0xb8));
            }
            if (**(int **)(param_1 + 0x18) != 0) {
              SendMessageA((HWND)**(int **)(param_1 + 0x18),0x111,local_2c,
                           *(LPARAM *)(param_1 + 0xb8));
            }
          }
        }
LAB_0059b58a:
        if (iStack_18 == 0) goto LAB_0059b764;
        if (*(int *)(param_1 + 0x148) == 0) {
          if (DAT_007893e7 == '\0') {
            cStack_5 = '\0';
            pbVar17 = *(byte **)(*(int *)(param_1 + 0xc) + 0xbc);
            if (pbVar17 != (byte *)0x0) {
              pbVar12 = *(byte **)(*(int *)(param_1 + 0xc) + 0xe4);
              pbVar11 = pbVar12;
              do {
                bVar2 = *pbVar11;
                pbVar11 = pbVar11 + 1;
              } while (bVar2 != 0);
              if (pbVar11 != pbVar12 + 1) {
                do {
                  bVar2 = *pbVar12;
                  bVar21 = bVar2 < *pbVar17;
                  if (bVar2 != *pbVar17) {
LAB_0059b600:
                    iVar8 = (1 - (uint)bVar21) - (uint)(bVar21 != 0);
                    goto LAB_0059b605;
                  }
                  if (bVar2 == 0) break;
                  bVar2 = pbVar12[1];
                  bVar21 = bVar2 < pbVar17[1];
                  if (bVar2 != pbVar17[1]) goto LAB_0059b600;
                  pbVar12 = pbVar12 + 2;
                  pbVar17 = pbVar17 + 2;
                } while (bVar2 != 0);
                iVar8 = 0;
LAB_0059b605:
                if (iVar8 != 0) {
                  cStack_5 = '\x01';
                }
              }
            }
            iVar8 = *(int *)(param_1 + 0x1c4);
            if ((iVar8 < 0) || ((iVar8 < 1 && (*(int *)(param_1 + 0x1c0) == 0)))) {
LAB_0059b644:
              if (cStack_5 == '\0') goto LAB_0059b695;
            }
            else {
              iVar20 = *(int *)(*(int *)(param_1 + 0xc) + 0xe8);
              iVar3 = *(int *)(*(int *)(param_1 + 0xc) + 0xec);
              if (((iVar3 < 0) || ((iVar3 < 1 && (iVar20 == 0)))) ||
                 ((iVar20 == *(int *)(param_1 + 0x1c0) && (iVar3 == iVar8)))) goto LAB_0059b644;
            }
            FUN_00402f80(&DAT_006979dc);
            iVar8 = *(int *)(param_1 + 0xc);
            *(undefined4 *)(iVar8 + 0xe8) = 0;
            *(undefined4 *)(iVar8 + 0xec) = 0;
            FUN_00474ee0(0,DAT_0078a554,*(undefined4 *)(*(int *)(param_1 + 0xc) + 8),0x50040);
            goto LAB_0059af6d;
          }
LAB_0059b695:
          iVar8 = *(int *)(param_1 + 0xc);
          if (*(int *)(iVar8 + 0x3c) != 0) {
            if (*(int *)(iVar8 + 0x34) == 0) {
              pwVar13 = (wchar_t *)(iVar8 + 0x38);
            }
            else {
              pwVar13 = *(wchar_t **)(iVar8 + 0x34);
            }
            pwVar13 = _wcsrchr(pwVar13,L'.');
            if ((((pwVar13 != (wchar_t *)0x0) && (iVar8 = __wcsicmp(pwVar13,L".html"), iVar8 != 0))
                && (iVar8 = __wcsicmp(pwVar13,L".htm"), iVar8 != 0)) &&
               (iVar8 = FUN_00474ee0(0,DAT_00789dc8,*(undefined4 *)(*(int *)(param_1 + 0xc) + 8),
                                     0x50004), iVar8 == 7)) goto LAB_0059af6d;
          }
        }
        iVar8 = *(int *)(param_1 + 0xc);
        if (*(int *)(iVar8 + 0x3c) != 0) {
          if (*(int *)(iVar8 + 0x34) == 0) {
            pwVar13 = (wchar_t *)(iVar8 + 0x38);
          }
          else {
            pwVar13 = *(wchar_t **)(iVar8 + 0x34);
          }
          pwVar13 = _wcsrchr(pwVar13,L'.');
          if (((pwVar13 != (wchar_t *)0x0) && (iVar8 = __wcsicmp(pwVar13,L".html"), iVar8 != 0)) &&
             ((iVar8 = __wcsicmp(pwVar13,L".htm"), iVar8 != 0 &&
              (iVar8 = FUN_005d5f90(), iVar8 != 0)))) {
LAB_0059b2bc:
            local_80 = local_80 & 0xffffff00;
            FUN_00402560();
            FUN_0059c706();
            return;
          }
        }
      }
    }
LAB_0059b764:
    if ((char)iStack_1c != '\0') {
      if ((-1 < *(int *)(param_1 + 0x1cc)) &&
         ((0 < *(int *)(param_1 + 0x1cc) || (0x400 < *(uint *)(param_1 + 0x1c8))))) {
        lVar23 = FUN_005baa60();
        FUN_00640553(&iStack_d4);
        local_7c = (char *)(*(uint *)(param_1 + 0x1c8) - local_5c);
        uVar19 = (uint)(*(uint *)(param_1 + 0x1c8) < local_5c);
        iVar8 = *(int *)(param_1 + 0x1cc) - local_58;
        iStack_78 = iVar8 - uVar19;
        if ((-1 < iStack_78) &&
           ((iStack_78 != 0 &&
             SBORROW4(*(int *)(param_1 + 0x1cc),local_58) == SBORROW4(iVar8,uVar19) ||
            (local_7c != (char *)0x0)))) {
          local_10 = (char *)(((iStack_d4 - iStack_c8) * 1000 - (uint)uStack_c4) + (uint)uStack_d0);
          if (local_10 == (char *)0x0) {
            local_10 = (char *)0x1;
          }
          lVar24 = FUN_0063ecb6();
          if (lVar23 < lVar24) {
            iStack_30 = (int)((ulonglong)(lVar24 - lVar23) >> 0x20);
            uVar25 = __allmul(lVar24 - lVar23,local_10,(int)local_10 >> 0x1f);
            local_10 = (char *)__alldiv(uVar25,lVar23);
            iStack_6c = *(int *)(param_1 + 0x1d0);
            iStack_68 = iStack_6c >> 0x1f;
            lVar24 = __alldiv(lVar23,2,0);
            if (CONCAT44(iStack_68,iStack_6c) < lVar24) {
              local_10 = (char *)((int)local_10 / 0x14);
            }
            else {
              uVar25 = __allmul(lVar23,9,0);
              lVar23 = __alldiv(uVar25,10,0);
              if (CONCAT44(iStack_68,iStack_6c) < lVar23) {
                local_10 = (char *)((int)local_10 / 2);
              }
            }
            if ((int)local_10 < 0x2ee1) {
              pcVar9 = local_10;
              if ((int)local_10 < 1) goto LAB_0059b99c;
            }
            else {
              pcVar9 = (char *)0x2ee0;
            }
            iVar8 = *(int *)(param_1 + 0x884);
            if (iVar8 == -1) {
              Sleep((DWORD)pcVar9);
            }
            else {
              *(undefined4 *)(param_1 + 0x278) = 0;
              *(undefined4 *)(param_1 + 0x47c) = 0;
              *(undefined4 *)(param_1 + 0x680) = 0;
              for (uVar19 = 0;
                  (uVar19 < *(uint *)(param_1 + 0x278) &&
                  (*(int *)(param_1 + 0x27c + uVar19 * 4) != iVar8)); uVar19 = uVar19 + 1) {
              }
              if ((uVar19 == *(uint *)(param_1 + 0x278)) && (*(uint *)(param_1 + 0x278) < 0x80)) {
                *(int *)(param_1 + 0x27c + uVar19 * 4) = iVar8;
                *(int *)(param_1 + 0x278) = *(int *)(param_1 + 0x278) + 1;
              }
              for (uVar19 = 0;
                  (uVar19 < *(uint *)(param_1 + 0x680) &&
                  (*(int *)(param_1 + 0x684 + uVar19 * 4) != *(int *)(param_1 + 0x884)));
                  uVar19 = uVar19 + 1) {
              }
              exceptfds = (fd_set *)(param_1 + 0x680);
              if ((uVar19 == exceptfds->fd_count) && (exceptfds->fd_count < 0x80)) {
                *(undefined4 *)(param_1 + 0x684 + uVar19 * 4) = *(undefined4 *)(param_1 + 0x884);
                exceptfds->fd_count = exceptfds->fd_count + 1;
              }
              tStack_64.tv_sec = (int)pcVar9 / 1000;
              tStack_64.tv_usec = ((int)pcVar9 % 1000) * 1000;
              select(0,(fd_set *)(param_1 + 0x278),(fd_set *)(param_1 + 0x47c),exceptfds,&tStack_64)
              ;
            }
          }
        }
      }
    }
LAB_0059b99c:
    iStack_18 = FUN_00596a20();
    if (iStack_18 != 0) {
      cStack_12 = '\x01';
      if ((iStack_18 == 1) && (*(int *)(param_1 + 0x40) == 0)) {
        puVar4 = *(undefined4 **)(param_1 + 0xc);
        if ((puVar4[0xd2] == 0) || (*(char *)(puVar4 + 0xd3) == '\0')) {
          pcVar9 = _strstr((char *)*puVar4,".googlevideo.com");
          if (((pcVar9 != (char *)0x0) ||
              (pcVar9 = _strstr((char *)**(undefined4 **)(param_1 + 0xc),".drive.google.com"),
              pcVar9 != (char *)0x0)) &&
             (piVar18 = *(int **)(*(int *)(param_1 + 0xc) + 8), piVar18 != (int *)0x0)) {
            pcVar9 = "videoplayback?";
            for (uVar19 = 0xe; 3 < uVar19; uVar19 = uVar19 - 4) {
              if (*piVar18 != *(int *)pcVar9) goto LAB_0059bc8d;
              pcVar9 = (char *)((int)pcVar9 + 4);
              piVar18 = piVar18 + 1;
            }
            if (((char)*(int *)pcVar9 == (char)*piVar18) &&
               (*(char *)((int)pcVar9 + 1) == *(char *)((int)piVar18 + 1))) {
              if ((((*(int *)(param_1 + 0x1cc) < 1) &&
                   ((*(int *)(param_1 + 0x1cc) < 0 || (*(uint *)(param_1 + 0x1c8) < 0x1f400)))) &&
                  (pcVar9 = *(char **)(*(int *)(param_1 + 0xc) + 8), pcVar9 != (char *)0x0)) &&
                 ((((((pcVar9 = _strstr(pcVar9,"&dur=0.0"), pcVar9 != (char *)0x0 ||
                      (pcVar9 = _strstr(*(char **)(*(int *)(param_1 + 0xc) + 8),"?dur=0.0"),
                      pcVar9 != (char *)0x0)) &&
                     ((pcVar9 = _strstr(*(char **)(*(int *)(param_1 + 0xc) + 8),"&source=yt_otf"),
                      pcVar9 != (char *)0x0 ||
                      (pcVar9 = _strstr(*(char **)(*(int *)(param_1 + 0xc) + 8),"?source=yt_otf"),
                      pcVar9 != (char *)0x0)))) &&
                    ((pcVar9 = _strstr(*(char **)(*(int *)(param_1 + 0xc) + 8),"&otf=1"),
                     pcVar9 != (char *)0x0 ||
                     (pcVar9 = _strstr(*(char **)(*(int *)(param_1 + 0xc) + 8),"?otf=1"),
                     pcVar9 != (char *)0x0)))) ||
                   (((pcVar9 = _strstr(*(char **)(*(int *)(param_1 + 0xc) + 200),"&source=yt_otf"),
                     pcVar9 != (char *)0x0 ||
                     (pcVar9 = _strstr(*(char **)(*(int *)(param_1 + 0xc) + 200),"?source=yt_otf"),
                     pcVar9 != (char *)0x0)) &&
                    ((pcVar9 = _strstr(*(char **)(*(int *)(param_1 + 0xc) + 200),"&otf=1"),
                     pcVar9 != (char *)0x0 ||
                     (pcVar9 = _strstr(*(char **)(*(int *)(param_1 + 0xc) + 200),"?otf=1"),
                     pcVar9 != (char *)0x0)))))) && (iVar8 = FUN_0059a0b0(&cStack_12), iVar8 != 0)))
                 ) {
                local_80 = local_80 & 0xffffff00;
                FUN_00402560();
                FUN_0059c706();
                return;
              }
            }
LAB_0059bc8d:
            if (cStack_12 == '\0') goto LAB_0059c474;
          }
        }
        else {
          FUN_0056fd80();
          iVar8 = FUN_005996d0();
          if (iVar8 != 0) {
            cStack_12 = '\0';
            *(undefined1 *)(*(int *)(param_1 + 0xc) + 0x363) = 0;
            *(undefined1 *)(*(int *)(param_1 + 0xc) + 0x364) = 1;
            *(undefined1 *)(*(int *)(param_1 + 0xc) + 0x352) = 0;
            *(undefined8 *)(*(int *)(param_1 + 0xc) + 0x3d0) =
                 *(undefined8 *)(*(int *)(param_1 + 0x40) + 0x18);
            FUN_005bb070();
            cVar6 = *(char *)(*(int *)(param_1 + 0xc) + 0x36a);
            if (((cVar6 == '\0') || (cVar7 = FUN_0057c210(), cVar7 == '\0')) &&
               (*(char *)(param_1 + 0x888) == '\0')) goto LAB_0059c474;
            if ((cVar6 == '\0') || (cVar6 = FUN_0057c210(), cVar6 == '\0')) {
              FUN_0055f780();
              *(undefined4 *)(param_1 + 0x1c8) = 0;
              *(undefined4 *)(param_1 + 0x1cc) = 0;
LAB_0059ba89:
              local_80 = local_80 & 0xffffff00;
              FUN_00402560();
              FUN_0059c706();
              return;
            }
            goto LAB_0059af6d;
          }
          *(undefined1 *)(*(int *)(param_1 + 0xc) + 0x363) = 0;
          *(undefined1 *)(*(int *)(param_1 + 0xc) + 0x364) = 1;
          *(undefined1 *)(*(int *)(param_1 + 0xc) + 0x352) = 0;
          iStack_18 = 0x753c;
        }
      }
      if (*(int *)(param_1 + 0x1c) != 0) goto LAB_0059b03c;
      FUN_005bacf0("http:rcd, dwnl finished, s1(t7)\r\n");
      if (((*(undefined4 **)(param_1 + 0xc))[0x59] & 0x100) == 0) goto LAB_0059c146;
      if ((((0 < *(int *)(param_1 + 0x1cc)) ||
           ((-1 < *(int *)(param_1 + 0x1cc) && (2000 < *(uint *)(param_1 + 0x1c8))))) ||
          ((pcVar9 = _strstr((char *)**(undefined4 **)(param_1 + 0xc),".googlevideo.com"),
           pcVar9 == (char *)0x0 &&
           (pcVar9 = _strstr((char *)**(undefined4 **)(param_1 + 0xc),".drive.google.com"),
           pcVar9 == (char *)0x0)))) ||
         (piVar18 = *(int **)(*(int *)(param_1 + 0xc) + 8), piVar18 == (int *)0x0))
      goto LAB_0059c146;
      uVar19 = 0xe;
      pcVar9 = "videoplayback?";
      break;
    }
LAB_0059c474:
    if (local_11 == '\0') {
LAB_0059c6d6:
      FUN_005970d0();
    }
    else if (*(int *)(param_1 + 0xb0) != 0) {
      FUN_0056fd80();
      iVar8 = *(int *)(param_1 + 0x274);
      iVar8 = FUN_0055a070(0,*(undefined4 *)(*(int *)(param_1 + 0xb0) + 0x10),
                           *(uint *)(iVar8 + 0x80) + *(uint *)(iVar8 + 0x60),
                           *(int *)(iVar8 + 0x84) + *(int *)(iVar8 + 100) +
                           (uint)CARRY4(*(uint *)(iVar8 + 0x80),*(uint *)(iVar8 + 0x60)),
                           *(undefined4 *)(param_1 + 0x1c0),*(undefined4 *)(param_1 + 0x1c4),
                           iVar8 + 0x78);
      if (iVar8 == 0) {
        iVar8 = *(int *)(param_1 + 0x274);
        if ((-1 < *(int *)(iVar8 + 0x84)) &&
           ((0 < *(int *)(iVar8 + 0x84) || (0x7d000 < *(uint *)(iVar8 + 0x80))))) {
          local_11 = '\0';
        }
      }
      else {
        local_11 = '\0';
        iVar8 = *(int *)(param_1 + 0x274);
        if (*(int *)(iVar8 + 0x6c) < 0) {
LAB_0059c516:
          FUN_00582fd0();
          iVar8 = *(int *)(param_1 + 0x274);
          puVar1 = (uint *)(iVar8 + 0x60);
          uVar19 = *puVar1;
          *puVar1 = *puVar1 + *(uint *)(iVar8 + 0x80);
          *(int *)(iVar8 + 100) =
               *(int *)(iVar8 + 100) + *(int *)(iVar8 + 0x84) +
               (uint)CARRY4(uVar19,*(uint *)(iVar8 + 0x80));
          iVar8 = *(int *)(param_1 + 0x274);
          *(undefined4 *)(iVar8 + 0x80) = 0;
          *(undefined4 *)(iVar8 + 0x84) = 0;
          (**(code **)(**(int **)(param_1 + 0x274) + 0x18))(*(int **)(param_1 + 0x274) + 0xc80);
          *(undefined4 *)(param_1 + 0x278) = 0;
          *(undefined4 *)(param_1 + 0x47c) = 0;
          *(undefined4 *)(param_1 + 0x680) = 0;
          for (uVar19 = 0;
              (uVar19 < *(uint *)(param_1 + 0x47c) &&
              (*(int *)(param_1 + 0x480 + uVar19 * 4) !=
               *(int *)(*(int *)(param_1 + 0x274) + 0x3200))); uVar19 = uVar19 + 1) {
          }
          if ((uVar19 == *(uint *)(param_1 + 0x47c)) && (*(uint *)(param_1 + 0x47c) < 0x80)) {
            *(undefined4 *)(param_1 + 0x480 + uVar19 * 4) =
                 *(undefined4 *)(*(int *)(param_1 + 0x274) + 0x3200);
            *(int *)(param_1 + 0x47c) = *(int *)(param_1 + 0x47c) + 1;
          }
          for (uVar19 = 0;
              (uVar19 < *(uint *)(param_1 + 0x680) &&
              (*(int *)(param_1 + 0x684 + uVar19 * 4) !=
               *(int *)(*(int *)(param_1 + 0x274) + 0x3200))); uVar19 = uVar19 + 1) {
          }
          if ((uVar19 == *(uint *)(param_1 + 0x680)) && (*(uint *)(param_1 + 0x680) < 0x80)) {
            *(undefined4 *)(param_1 + 0x684 + uVar19 * 4) =
                 *(undefined4 *)(*(int *)(param_1 + 0x274) + 0x3200);
            *(int *)(param_1 + 0x680) = *(int *)(param_1 + 0x680) + 1;
          }
          iVar8 = *(int *)(param_1 + 0x884);
          if (iVar8 != -1) {
            for (uVar19 = 0;
                (uVar19 < *(uint *)(param_1 + 0x278) &&
                (*(int *)(param_1 + 0x27c + uVar19 * 4) != iVar8)); uVar19 = uVar19 + 1) {
            }
            if ((uVar19 == *(uint *)(param_1 + 0x278)) && (*(uint *)(param_1 + 0x278) < 0x80)) {
              *(int *)(param_1 + 0x27c + uVar19 * 4) = iVar8;
              *(int *)(param_1 + 0x278) = *(int *)(param_1 + 0x278) + 1;
            }
            for (uVar19 = 0;
                (uVar19 < *(uint *)(param_1 + 0x680) &&
                (*(int *)(param_1 + 0x684 + uVar19 * 4) != *(int *)(param_1 + 0x884)));
                uVar19 = uVar19 + 1) {
            }
            if ((uVar19 == *(uint *)(param_1 + 0x680)) && (*(uint *)(param_1 + 0x680) < 0x80)) {
              *(undefined4 *)(param_1 + 0x684 + uVar19 * 4) = *(undefined4 *)(param_1 + 0x884);
              *(int *)(param_1 + 0x680) = *(int *)(param_1 + 0x680) + 1;
            }
          }
        }
        else {
          iVar20 = *(int *)(iVar8 + 0x6c) - (uint)(*(uint *)(iVar8 + 0x68) < 0x40);
          if ((*(int *)(iVar8 + 0x7c) <= iVar20) &&
             ((*(int *)(iVar8 + 0x7c) < iVar20 ||
              (*(uint *)(iVar8 + 0x78) < *(uint *)(iVar8 + 0x68) - 0x40)))) goto LAB_0059c516;
        }
        iVar8 = *(int *)(param_1 + 0x274);
      }
      FUN_00570620(*(undefined4 *)(*(int *)(iVar8 + 0xa8) + 0x10));
      if (local_11 == '\0') goto LAB_0059c6d6;
    }
    FUN_005bd1d0();
  } while( true );
LAB_0059bd26:
  if (uVar19 < 4) goto LAB_0059bd40;
  if (*piVar18 != *(int *)pcVar9) goto LAB_0059c146;
  uVar19 = uVar19 - 4;
  pcVar9 = (char *)((int)pcVar9 + 4);
  piVar18 = piVar18 + 1;
  goto LAB_0059bd26;
LAB_0059bd40:
  if (((char)*(int *)pcVar9 == (char)*piVar18) &&
     (*(char *)((int)pcVar9 + 1) == *(char *)((int)piVar18 + 1))) {
    FUN_005bacf0("http:rcd, Check for redirect, s1\r\n");
    iStack_1c = 0;
    if (((*(int *)(param_1 + 0x1c8) == *(int *)(param_1 + 0x1c0)) &&
        (*(int *)(param_1 + 0x1cc) == *(int *)(param_1 + 0x1c4))) ||
       (*(int *)(param_1 + 0x1c0) == 0 && *(int *)(param_1 + 0x1c4) == 0)) {
      FUN_0056fd80();
      hFile = CreateFileW(*(LPCWSTR *)(*(int *)(param_1 + 0xb0) + 0x10),0x80000000,1,
                          (LPSECURITY_ATTRIBUTES)0x0,3,0,(HANDLE)0x0);
      pvStack_54 = hFile;
      if (hFile == (HANDLE)0xffffffff) {
        DVar14 = GetLastError();
        *(DWORD *)(param_1 + 0x14c) = DVar14;
        if ((DVar14 == 0x20) || (DVar14 == 5)) {
          FUN_004024f0();
          local_80._0_1_ = 3;
          FUN_00402f20(&local_10,"Check for redirect, cf failed, lastErr=%ld Sleep 1000\r\n",
                       *(undefined4 *)(param_1 + 0x14c));
          FUN_005bacf0(local_10);
          Sleep(1000);
          hFile = CreateFileW(*(LPCWSTR *)(*(int *)(param_1 + 0xb0) + 0x10),0x80000000,1,
                              (LPSECURITY_ATTRIBUTES)0x0,3,0,(HANDLE)0x0);
          local_80 = CONCAT31(local_80._1_3_,2);
          pvStack_54 = hFile;
          FUN_00402560();
          if (hFile != (HANDLE)0xffffffff) goto LAB_0059be55;
        }
        FUN_004024f0();
        local_80 = CONCAT31(local_80._1_3_,7);
        FUN_00402f20(&iStack_18,"Check for redirect, cf failed, lastErr=%ld\r\n",
                     *(undefined4 *)(param_1 + 0x14c));
        FUN_005bacf0(iStack_18);
LAB_0059c0f4:
        local_80._0_1_ = 2;
        FUN_00402560();
        local_80 = (uint)local_80._1_3_ << 8;
        FUN_00402560();
        FUN_0059c706();
        return;
      }
LAB_0059be55:
      pcStack_34 = *(char **)(param_1 + 0x1c8);
      iStack_30 = *(undefined4 *)(param_1 + 0x1cc);
      local_10 = pcStack_34;
      if (((*(int *)(param_1 + 0x1c0) == 0 && *(int *)(param_1 + 0x1c4) == 0) ||
          (*(char *)(*(int *)(param_1 + 0xc) + 0x363) != '\0')) &&
         (local_10 = (char *)GetFileSize(hFile,(LPDWORD)0x0), local_10 == (char *)0x0)) {
        local_7c = *(char **)(param_1 + 0x1c8);
        iStack_78 = *(int *)(param_1 + 0x1cc);
        local_10 = local_7c;
      }
      pcVar9 = local_10;
      if (*(int *)(*(int *)(param_1 + 0x274) + 0x31f8) != 0) {
        local_80 = CONCAT31(local_80._1_3_,4);
        FUN_00610e74(*(undefined4 *)(*(int *)(param_1 + 0x274) + 0x31f8));
        local_80 = 2;
        FUN_0059bef4();
        return;
      }
      uVar15 = FUN_00610e69(local_10 + 1);
      *(undefined4 *)(*(int *)(param_1 + 0x274) + 0x31f8) = uVar15;
      if (*(int *)(*(int *)(param_1 + 0x274) + 0x31f8) != 0) {
        FUN_005bacf0("Check for redirect, s2\r\n");
        local_10 = (char *)0x0;
        BVar16 = ReadFile(hFile,*(LPVOID *)(*(int *)(param_1 + 0x274) + 0x31f8),(DWORD)pcVar9,
                          (LPDWORD)&local_10,(LPOVERLAPPED)0x0);
        uStack_44 = (double)CONCAT44(BVar16,(undefined4)uStack_44);
        if ((BVar16 == 0) || (local_10 != pcVar9)) {
          FUN_004024f0();
          local_80 = CONCAT31(local_80._1_3_,6);
          pcVar10 = local_10;
          DVar14 = GetLastError();
          FUN_00402f20(&iStack_18,
                       "Check for redirect, rf_res=%ld, lastErr=%ld, buff=%ld, read=%ld\r\n",
                       uStack_44._4_4_,DVar14,pcVar9,pcVar10);
          FUN_005bacf0(iStack_18);
          CloseHandle(hFile);
          goto LAB_0059c0f4;
        }
        pcVar9[*(int *)(*(int *)(param_1 + 0x274) + 0x31f8)] = '\0';
        FUN_005bacf0("Check for redirect, s3, fc:\r\n");
        FUN_005bacf0(*(undefined4 *)(*(int *)(param_1 + 0x274) + 0x31f8));
        FUN_005bacf0(&DAT_00699144);
        iVar8 = __memicmp(*(void **)(*(int *)(param_1 + 0x274) + 0x31f8),&DAT_00697070,7);
        if ((iVar8 == 0) ||
           (iVar8 = __memicmp(*(void **)(*(int *)(param_1 + 0x274) + 0x31f8),&DAT_00697050,8),
           iVar8 == 0)) {
          FUN_005885d0();
          iStack_1c = 1;
        }
        else {
          FUN_00610e74(*(undefined4 *)(*(int *)(param_1 + 0x274) + 0x31f8));
          *(undefined4 *)(*(int *)(param_1 + 0x274) + 0x31f8) = 0;
        }
      }
      CloseHandle(hFile);
      if (iStack_1c != 0) {
        FUN_005bacf0("http:rcd, Redirect found\r\n");
        *(undefined4 *)(param_1 + 0x1c8) = 0;
        *(undefined4 *)(param_1 + 0x1cc) = 0;
        *(undefined8 *)(param_1 + 400) = 0;
        FUN_0055e8b0();
        FUN_0055f780();
        *(undefined4 *)(param_1 + 200) = 0;
        *(undefined4 *)(param_1 + 0xcc) = 0;
        *(undefined4 *)(param_1 + 0xd0) = 0;
        *(undefined4 *)(param_1 + 0xd4) = 0;
        iVar8 = *(int *)(param_1 + 0x274);
        *(undefined4 *)(iVar8 + 0x60) = 0;
        *(undefined4 *)(iVar8 + 100) = 0;
        FUN_00580b80();
        FUN_005bc210(0,0,1);
        local_80 = local_80 & 0xffffff00;
        FUN_00402560();
        FUN_0059c706();
        return;
      }
    }
  }
LAB_0059c146:
  if ((iStack_18 != 0x753c) && (iStack_18 != -1)) {
LAB_0059c1e7:
    iStack_30 = *(int *)(param_1 + 0x1cc);
    iVar8 = *(int *)(param_1 + 0x1c0);
    iVar20 = *(int *)(param_1 + 0x1c4);
    if (((*(int *)(param_1 + 0x1c8) == iVar8) && (iStack_30 == iVar20)) &&
       (iVar8 != 0 || iVar20 != 0)) {
      if (**(int **)(param_1 + 0x10) != 0) {
        uStack_100 = *(undefined4 *)(param_1 + 0x1cc);
        uStack_fc = *(undefined4 *)(param_1 + 0x1d0);
        iStack_10c = iVar8;
        iStack_108 = iVar20;
        iStack_104 = *(int *)(param_1 + 0x1c8);
        SendMessageA((HWND)**(int **)(param_1 + 0x10),0x111,0x1471,(LPARAM)&iStack_10c);
      }
      if (**(int **)(param_1 + 0x14) != 0) {
        auStack_124[0] = *(undefined4 *)(param_1 + 0x240);
        uStack_11c = *(undefined4 *)(param_1 + 0x1c8);
        uStack_118 = *(undefined4 *)(param_1 + 0x1cc);
        uStack_114 = *(undefined4 *)(param_1 + 0x1d0);
        SendMessageA((HWND)**(int **)(param_1 + 0x14),0x111,0x1394,(LPARAM)auStack_124);
      }
    }
    FUN_00402f20(&local_c,DAT_00789d50,*(undefined4 *)(param_1 + 0x1c8),
                 *(undefined4 *)(param_1 + 0x1cc));
    FUN_005bbe30(local_c,1);
    uVar19 = *(uint *)(param_1 + 0x1c8);
    iStack_30 = *(int *)(param_1 + 0x1cc);
    uVar5 = *(uint *)(param_1 + 0x1c0);
    iVar8 = *(int *)(param_1 + 0x1c4);
    if (((uVar19 != uVar5) || (iStack_30 != iVar8)) && (uVar5 != 0 || iVar8 != 0)) {
      iStack_38 = *(int *)(param_1 + 0x1cc);
      tStack_64.tv_sec = *(uint *)(*(int *)(param_1 + 0xc) + 0x388);
      uStack_3c = uVar19;
      if ((uVar19 != uVar5 + tStack_64.tv_sec) ||
         (iStack_38 != iVar8 + (uint)CARRY4(uVar5,tStack_64.tv_sec))) {
        iVar8 = iVar8 + (uint)CARRY4(uVar5,tStack_64.tv_sec);
        if ((iVar8 <= *(int *)(param_1 + 0x1cc)) &&
           ((iVar8 < *(int *)(param_1 + 0x1cc) ||
            (uVar5 + tStack_64.tv_sec < *(uint *)(param_1 + 0x1c8))))) {
          FUN_0055f110();
          local_80 = local_80 & 0xffffff00;
          FUN_00402560();
          FUN_0059c706();
          return;
        }
        local_80 = local_80 & 0xffffff00;
        FUN_00402560();
        FUN_0059c706();
        return;
      }
    }
    iVar8 = *(int *)(param_1 + 0xc);
    if ((((*(int *)(iVar8 + 0x348) != 0) && (*(int *)(param_1 + 0x40) != 0)) &&
        (iVar20 = *(int *)(*(int *)(param_1 + 0x40) + 0x40), iVar20 != 0)) &&
       ((iVar20 = *(int *)(iVar20 + 0x48), iVar20 != 0 && (*(int *)(iVar8 + 0x33c) == 8)))) {
      FUN_005985a0(iVar20,iVar8 + 0x41c);
    }
    for (iVar8 = 0; iVar8 < *(int *)(param_1 + 0x1b0); iVar8 = iVar8 + 1) {
      if (*(int *)(iVar8 * 0x3508 + *(int *)(param_1 + 0x274) + 0x9c) != 0) {
        FUN_0057c100();
      }
    }
    FUN_005bd1d0();
    local_80 = local_80 & 0xffffff00;
    FUN_00402560();
    FUN_0059c706();
    return;
  }
  if (((*(uint *)(*(int *)(param_1 + 0xc) + 0x164) & 0x100) != 0) &&
     ((*(int *)(*(int *)(param_1 + 0xc) + 0x30) != 0 &&
      (*(int *)(param_1 + 0x1c8) != 0 || *(int *)(param_1 + 0x1cc) != 0)))) {
    if ((-1 < *(int *)(param_1 + 0x1c4)) &&
       ((0 < *(int *)(param_1 + 0x1c4) || (0x40000000 < *(uint *)(param_1 + 0x1c0))))) {
      FUN_005bacf0("Checking flv file struct.\n");
      iVar8 = FUN_00598a20();
      if (iVar8 != 0) {
        FUN_005bc210(*(undefined4 *)(param_1 + 0x1c8),*(undefined4 *)(param_1 + 0x1cc),1);
        FUN_005bacf0("Duration==lastTimeStamp, seems the flv file is comlete.\n");
        goto LAB_0059c1e7;
      }
    }
  }
  FUN_00402f20(&local_c,DAT_00789d5c,*(undefined4 *)(param_1 + 0x1c8),
               *(undefined4 *)(param_1 + 0x1cc));
  FUN_005bbe30(local_c,1);
  local_80 = local_80 & 0xffffff00;
  FUN_00402560();
  FUN_0059c706();
  return;
}


