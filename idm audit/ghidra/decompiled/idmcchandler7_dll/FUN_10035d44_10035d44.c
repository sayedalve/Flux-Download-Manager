// Function: FUN_10035d44
// Entry Point: 0x10035d44
// Program: idmcchandler7.dll


void FUN_10035d44(void)

{
  byte bVar1;
  undefined4 *puVar2;
  char cVar3;
  DWORD DVar4;
  byte *pbVar5;
  int *piVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  char *pcVar10;
  undefined4 uVar11;
  void *pvVar12;
  char *pcVar13;
  char *pcVar14;
  undefined1 *puVar15;
  wchar_t *pwVar16;
  byte *pbVar17;
  BSTR pOVar18;
  char *pcVar19;
  char *pcVar20;
  char **ppcVar21;
  byte **ppbVar22;
  int **ppiVar23;
  int iVar24;
  char ***pppcVar25;
  int iVar26;
  BSTR pOVar27;
  int *piVar28;
  undefined4 extraout_ECX;
  undefined4 extraout_EDX;
  undefined4 extraout_EDX_00;
  char *extraout_EDX_01;
  undefined4 extraout_EDX_02;
  undefined4 extraout_EDX_03;
  undefined1 unaff_BL;
  code *pcVar29;
  int *unaff_EBP;
  code *pcVar30;
  bool bVar31;
  undefined4 uStack0000000c;
  int iVar32;
  int iVar33;
  int iVar34;
  int iVar35;
  int iVar36;
  int iVar37;
  int iVar38;
  LPCWSTR pWVar39;
  undefined4 uVar40;
  undefined4 uVar41;
  BSTR pOVar42;
  int iVar43;
  short *psVar44;
  int iVar45;
  SAFEARRAY *pSVar46;
  WCHAR *pWVar47;
  int iVar48;
  int iVar49;
  int iVar50;
  uint uVar51;
  undefined4 uVar52;
  short *psVar53;
  undefined4 uVar54;
  int iVar55;
  OLECHAR *pOVar56;
  char *pcVar57;
  int iVar58;
  int iVar59;
  undefined4 uVar60;
  BSTR pOVar61;
  char *pcVar62;
  undefined4 uVar63;
  undefined4 *puVar64;
  int iVar65;
  int *piVar66;
  uint *puVar67;
  undefined4 uVar68;
  size_t sVar69;
  wchar_t *pwVar70;
  undefined *puVar71;
  
  ReleaseMutex(DAT_1005ac5c);
  iVar7 = unaff_EBP[-0x1a];
  *(undefined1 *)(unaff_EBP + -1) = unaff_BL;
  (*DAT_1005abec)(unaff_EBP + 10);
  *(undefined1 *)(unaff_EBP + -1) = 6;
  (*DAT_1005abec)(unaff_EBP + 0x16);
  iVar8 = unaff_EBP[-0x44];
  if ((iVar8 == 200) || (iVar8 == 0xce)) {
    iVar8 = unaff_EBP[3];
    puVar64 = (undefined4 *)(iVar8 + -4);
    FUN_10016e40((int)puVar64);
    *(undefined1 *)(iVar8 + 0x10) = 0;
    FUN_10018fa0(unaff_EBP + -0x23);
    *(undefined1 *)(unaff_EBP + -1) = 0xe;
    unaff_EBP[-0x43] = (int)(unaff_EBP + -0x23);
    puVar2 = (undefined4 *)*puVar64;
    piVar6 = FUN_1001c6e0(unaff_EBP + -0x43);
    (*(code *)*puVar2)(puVar64,&DAT_1004ca50,piVar6);
    if (unaff_EBP[-0x23] != 0) {
      iVar7 = (**(code **)(*(int *)unaff_EBP[4] + 0x104))
                        ((int *)unaff_EBP[4],iVar7,unaff_EBP[-0x23]);
      unaff_EBP[2] = iVar7;
      if (iVar7 < 0) {
        *(undefined1 *)(unaff_EBP + -1) = 6;
        FUN_10017f20(unaff_EBP + -0x23);
        *(undefined1 *)(unaff_EBP + -1) = 5;
        FUN_10017f20(unaff_EBP + 1);
        *(undefined1 *)(unaff_EBP + -1) = 4;
        goto LAB_10035ea3;
      }
      iVar7 = unaff_EBP[3];
      if (*(char *)(iVar7 + 0x15) != '\0') {
        *(undefined4 *)(iVar7 + 0xdc) = 0;
        *(undefined4 *)(iVar7 + 0xe0) = 0;
      }
      unaff_EBP[-0x41] = 0;
      *(undefined1 *)((int)unaff_EBP + 0x17) = 0;
      iVar8 = FUN_10003890(iVar7 + 0x78);
      if (iVar8 != 0) {
        iVar8 = unaff_EBP[3];
        iVar9 = FUN_10003890((void *)(iVar8 + 0x68));
        if (iVar9 == 0) {
          FUN_100148e0((void *)(iVar8 + 0x68),iVar7 + 0x78);
        }
      }
      iVar8 = unaff_EBP[3];
      iVar7 = iVar8 + 0x68;
      iVar9 = FUN_10003890(iVar7);
      if (iVar9 == 0) {
        pcVar29 = strchr_exref;
        if (*(char *)(iVar8 + 0x11) != '\0') {
          iVar7 = 0x3f;
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          pcVar29 = strchr_exref;
          pcVar10 = strchr(pcVar10,iVar7);
          if (pcVar10 != (char *)0x0) {
            iVar7 = FUN_10003890(unaff_EBP + -8);
            pcVar13 = operator_new(iVar7 + 1);
            pcVar14 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar10 = pcVar13;
            do {
              cVar3 = *pcVar14;
              *pcVar10 = cVar3;
              pcVar14 = pcVar14 + 1;
              pcVar10 = pcVar10 + 1;
            } while (cVar3 != '\0');
            pcVar10 = strchr(pcVar13,0x3f);
            if (pcVar10 != (char *)0x0) {
              *pcVar10 = '\0';
              cVar3 = FUN_10030aa0(pcVar13,0);
              if (cVar3 != '\0') {
                *(undefined1 *)((int)unaff_EBP + 0x17) = 1;
              }
            }
            operator_delete(pcVar13);
          }
        }
      }
      else {
        iVar8 = 1;
        pcVar10 = (char *)FUN_10003910(iVar7);
        cVar3 = FUN_10030aa0(pcVar10,iVar8);
        pcVar29 = strchr_exref;
        if (cVar3 != '\0') {
          *(undefined1 *)((int)unaff_EBP + 0x17) = 1;
          pcVar29 = strchr_exref;
        }
      }
      iVar8 = unaff_EBP[3];
      iVar7 = iVar8 + 0x88;
      iVar9 = FUN_10003890(iVar7);
      if ((iVar9 != 0) && (iVar9 = FUN_10003890(iVar8 + 0x68), iVar9 == 0)) {
        (*DAT_1005abe4)(unaff_EBP + 0x16);
        *(undefined1 *)(unaff_EBP + -1) = 0xf;
        uVar68 = 0x3f;
        uVar11 = FUN_10003910(unaff_EBP + -8);
        iVar9 = (*pcVar29)(uVar11,uVar68);
        if (iVar9 == 0) {
LAB_100361b5:
          pcVar13 = "text/plain";
          pcVar10 = (char *)FUN_10003910(iVar7);
          iVar7 = _stricmp(pcVar10,pcVar13);
          if (iVar7 == 0) {
            uVar68 = 0x3f;
            uVar11 = FUN_10003910(unaff_EBP + -8);
            iVar7 = (*pcVar29)(uVar11,uVar68);
            if (((iVar7 != 0) && (-1 < *(int *)(iVar8 + 0xe0))) &&
               ((0 < *(int *)(iVar8 + 0xe0) || (0x3e800 < *(uint *)(iVar8 + 0xdc))))) {
              iVar7 = FUN_10003890(unaff_EBP + -8);
              pcVar13 = operator_new(iVar7 + 1);
              pcVar14 = (char *)FUN_10003910(unaff_EBP + -8);
              pcVar10 = pcVar13;
              do {
                cVar3 = *pcVar14;
                *pcVar10 = cVar3;
                pcVar14 = pcVar14 + 1;
                pcVar10 = pcVar10 + 1;
              } while (cVar3 != '\0');
              puVar15 = (undefined1 *)(*pcVar29)(pcVar13,0x3f);
              if (puVar15 != (undefined1 *)0x0) {
                *puVar15 = 0;
                cVar3 = FUN_10030aa0(pcVar13,0);
                if (cVar3 != '\0') {
                  *(undefined1 *)((int)unaff_EBP + 0x17) = 1;
                }
              }
              operator_delete(pcVar13);
            }
          }
        }
        else {
          piVar6 = unaff_EBP + 0x16;
          pvVar12 = (void *)FUN_10003910(iVar7);
          uVar11 = FUN_10016f10(pvVar12,piVar6);
          if ((char)uVar11 == '\0') goto LAB_100361b5;
          iVar7 = FUN_10003890(unaff_EBP + -8);
          pcVar13 = operator_new(iVar7 + 1);
          pcVar14 = (char *)FUN_10003910(unaff_EBP + -8);
          pcVar10 = pcVar13;
          do {
            cVar3 = *pcVar14;
            *pcVar10 = cVar3;
            pcVar14 = pcVar14 + 1;
            pcVar10 = pcVar10 + 1;
          } while (cVar3 != '\0');
          pcVar10 = strchr(pcVar13,0x3f);
          if (pcVar10 != (char *)0x0) {
            *pcVar10 = '\0';
            cVar3 = FUN_10030aa0(pcVar13,0);
            if (cVar3 == '\0') {
              pbVar17 = &DAT_1004cafc;
              pbVar5 = (byte *)FUN_10003910(unaff_EBP + 0x16);
              do {
                bVar1 = *pbVar5;
                bVar31 = bVar1 < *pbVar17;
                if (bVar1 != *pbVar17) {
LAB_10036181:
                  iVar7 = (1 - (uint)bVar31) - (uint)(bVar31 != 0);
                  goto LAB_10036186;
                }
                if (bVar1 == 0) break;
                bVar1 = pbVar5[1];
                bVar31 = bVar1 < pbVar17[1];
                if (bVar1 != pbVar17[1]) goto LAB_10036181;
                pbVar5 = pbVar5 + 2;
                pbVar17 = pbVar17 + 2;
              } while (bVar1 != 0);
              iVar7 = 0;
LAB_10036186:
              if ((iVar7 == 0) && (cVar3 = FUN_10030aa0(".FLV",0), cVar3 != '\0'))
              goto LAB_1003619d;
            }
            else {
              pcVar10 = strrchr(pcVar13,0x2e);
              if (pcVar10 != (char *)0x0) {
                pcVar10 = pcVar10 + 1;
                pcVar14 = (char *)FUN_10003910(unaff_EBP + 0x16);
                iVar7 = _stricmp(pcVar14,pcVar10);
                if (iVar7 != 0) {
                  iVar8 = unaff_EBP[3] + 0x88;
                  pcVar14 = "application/octet-stream";
                  pcVar10 = (char *)FUN_10003910(iVar8);
                  iVar7 = _stricmp(pcVar10,pcVar14);
                  if (iVar7 != 0) {
                    pcVar14 = "application/x-sdlc";
                    pcVar10 = (char *)FUN_10003910(iVar8);
                    iVar7 = _stricmp(pcVar10,pcVar14);
                    if (iVar7 != 0) {
                      pcVar14 = "binary/octet-stream";
                      pcVar10 = (char *)FUN_10003910(iVar8);
                      iVar7 = _stricmp(pcVar10,pcVar14);
                      if (iVar7 != 0) goto LAB_100361a1;
                    }
                  }
                }
LAB_1003619d:
                *(undefined1 *)((int)unaff_EBP + 0x17) = 1;
              }
            }
          }
LAB_100361a1:
          operator_delete(pcVar13);
          iVar8 = unaff_EBP[3];
          pcVar29 = strchr_exref;
        }
        *(undefined1 *)(unaff_EBP + -1) = 0xe;
        (*DAT_1005abec)(unaff_EBP + 0x16);
      }
      (*DAT_1005abe4)(unaff_EBP + -0x15);
      *(undefined1 *)(unaff_EBP + -1) = 0x10;
      iVar7 = FUN_10003890(iVar8 + 0x68);
      if (iVar7 == 0) {
        unaff_EBP[6] = (int)MyStrClass::vftable;
        unaff_EBP[7] = 0;
        unaff_EBP[8] = 0;
        *(undefined1 *)(unaff_EBP + 9) = 0;
        *(undefined1 *)(unaff_EBP + -1) = 0x11;
        unaff_EBP[0x12] = (int)MyStrClass::vftable;
        unaff_EBP[0x13] = 0;
        unaff_EBP[0x14] = 0;
        *(undefined1 *)(unaff_EBP + 0x15) = 0;
        *(undefined1 *)(unaff_EBP + -1) = 0x12;
        (*DAT_1005abe4)(unaff_EBP + 0xe);
        *(undefined1 *)(unaff_EBP + -1) = 0x13;
        pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
        FUN_10001950(unaff_EBP + 6,pcVar10);
        piVar6 = (int *)unaff_EBP[7];
        if ((piVar6 == (int *)0x0) || (unaff_EBP[8] == 0)) {
          piVar6 = unaff_EBP + 9;
        }
        puVar15 = (undefined1 *)(*pcVar29)(piVar6,0x3f);
        if (puVar15 != (undefined1 *)0x0) {
          *puVar15 = 0;
        }
        piVar6 = (int *)unaff_EBP[7];
        if ((piVar6 == (int *)0x0) || (unaff_EBP[8] == 0)) {
          piVar6 = unaff_EBP + 9;
        }
        puVar15 = (undefined1 *)(*pcVar29)(piVar6,0x23);
        if (puVar15 != (undefined1 *)0x0) {
          *puVar15 = 0;
        }
        piVar6 = (int *)unaff_EBP[7];
        if ((piVar6 == (int *)0x0) || (unaff_EBP[8] == 0)) {
          piVar6 = unaff_EBP + 9;
        }
        pcVar10 = strrchr((char *)piVar6,0x2e);
        if ((pcVar10 != (char *)0x0) && (iVar7 = (*pcVar29)(pcVar10,0x2f), iVar7 == 0)) {
          FUN_10001950(unaff_EBP + 0x12,pcVar10 + 1);
        }
        piVar6 = (int *)unaff_EBP[0x13];
        if ((piVar6 == (int *)0x0) || (unaff_EBP[0x14] == 0)) {
          piVar6 = unaff_EBP + 0x15;
        }
        iVar7 = _stricmp((char *)piVar6,"f4m");
        if (((iVar7 == 0) && (*(int *)(iVar8 + 0xe0) < 1)) &&
           ((*(int *)(iVar8 + 0xe0) < 0 || (*(uint *)(iVar8 + 0xdc) < 0x186a1)))) {
          unaff_EBP[-0x41] = 1;
        }
        iVar7 = FUN_10003890(iVar8 + 0x88);
        if (iVar7 != 0) {
          piVar6 = unaff_EBP + 0xe;
          pvVar12 = (void *)FUN_10003910(iVar8 + 0x88);
          FUN_10016f10(pvVar12,piVar6);
        }
        piVar6 = (int *)unaff_EBP[0x13];
        if ((piVar6 == (int *)0x0) || (unaff_EBP[0x14] == 0)) {
          piVar6 = unaff_EBP + 0x15;
        }
        pcVar10 = (char *)FUN_10003910(unaff_EBP + 0xe);
        iVar7 = _stricmp(pcVar10,(char *)piVar6);
        if (iVar7 == 0) {
          piVar6 = (int *)FUN_10003910(unaff_EBP + 0xe);
LAB_100366bc:
          FUN_10001090(unaff_EBP + -0x15,piVar6);
        }
        else {
          *(undefined1 *)((int)unaff_EBP + -0x2a) = 0;
          iVar7 = FUN_10003890(unaff_EBP + 0xe);
          if (iVar7 == 0) {
LAB_10036447:
            iVar7 = 0x2e;
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar10 = strrchr(pcVar10,iVar7);
            iVar7 = 0x3f;
            pcVar13 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar13 = strrchr(pcVar13,iVar7);
            pcVar19 = (char *)0x0;
            pcVar20 = "://";
            pcVar14 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar14 = strstr(pcVar14,pcVar20);
            if (pcVar14 != (char *)0x0) {
              pcVar19 = strrchr(pcVar14 + 3,0x2f);
            }
            if ((pcVar13 != (char *)0x0) || (pcVar10 < pcVar19)) {
              iVar7 = FUN_10003890(unaff_EBP + 0xe);
              if (iVar7 == 0) {
                if (pcVar13 == (char *)0x0) goto LAB_100364c1;
                goto LAB_100365a2;
              }
              goto LAB_1003642d;
            }
LAB_100364c1:
            if (((pcVar10 <= pcVar19) || (pcVar19 == (char *)0x0)) || (unaff_EBP[0x14] == 0)) {
LAB_100365a2:
              iVar7 = FUN_10003890(unaff_EBP + 0xe);
              if (iVar7 != 0) {
                pcVar14 = "application/octet-stream";
                iVar8 = unaff_EBP[3] + 0x88;
                pcVar13 = (char *)FUN_10003910(iVar8);
                iVar7 = _stricmp(pcVar13,pcVar14);
                if (iVar7 != 0) {
                  pcVar14 = "binary/octet-stream";
                  pcVar13 = (char *)FUN_10003910(iVar8);
                  iVar7 = _stricmp(pcVar13,pcVar14);
                  if (iVar7 != 0) {
                    pcVar14 = "application/x-sdlc";
                    pcVar13 = (char *)FUN_10003910(iVar8);
                    iVar7 = _stricmp(pcVar13,pcVar14);
                    if (iVar7 != 0) {
                      pcVar14 = "text/plain";
                      pcVar13 = (char *)FUN_10003910(iVar8);
                      iVar7 = _stricmp(pcVar13,pcVar14);
                      if (iVar7 != 0) goto LAB_1003642d;
                    }
                  }
                }
              }
              if (((pcVar19 < pcVar10) && (pcVar19 != (char *)0x0)) && (unaff_EBP[0x14] != 0)) {
                pcVar29 = _stricmp_exref;
                if (*(char *)((int)unaff_EBP + 0x17) == '\0') {
LAB_1003666e:
                  piVar6 = (int *)unaff_EBP[0x13];
                  if ((piVar6 == (int *)0x0) || (unaff_EBP[0x14] == 0)) {
                    piVar6 = unaff_EBP + 0x15;
                  }
                  iVar7 = (*pcVar29)(piVar6,&DAT_1004e724);
                  if (iVar7 != 0) {
                    piVar6 = (int *)unaff_EBP[0x13];
                    if ((piVar6 == (int *)0x0) || (unaff_EBP[0x14] == 0)) {
                      piVar6 = unaff_EBP + 0x15;
                    }
                    iVar7 = (*pcVar29)(piVar6,&DAT_1004e6d4);
                    if (iVar7 != 0) goto LAB_100366c5;
                  }
                }
                else {
                  pcVar13 = "text/plain";
                  pcVar10 = (char *)FUN_10003910(unaff_EBP[3] + 0x88);
                  pcVar29 = _stricmp_exref;
                  iVar7 = _stricmp(pcVar10,pcVar13);
                  if (iVar7 != 0) goto LAB_1003666e;
                }
                piVar6 = (int *)unaff_EBP[0x13];
                if ((piVar6 == (int *)0x0) || (unaff_EBP[0x14] == 0)) {
                  piVar6 = unaff_EBP + 0x15;
                }
                goto LAB_100366bc;
              }
              goto LAB_100366c5;
            }
            piVar6 = (int *)unaff_EBP[0x13];
            if (piVar6 == (int *)0x0) {
              piVar6 = unaff_EBP + 0x15;
            }
            iVar7 = _stricmp((char *)piVar6,"asp");
            if (iVar7 == 0) goto LAB_100365a2;
            piVar6 = (int *)unaff_EBP[0x13];
            if ((piVar6 == (int *)0x0) || (unaff_EBP[0x14] == 0)) {
              piVar6 = unaff_EBP + 0x15;
            }
            iVar7 = _stricmp((char *)piVar6,"jsp");
            if (iVar7 == 0) goto LAB_100365a2;
            piVar6 = (int *)unaff_EBP[0x13];
            if ((piVar6 == (int *)0x0) || (unaff_EBP[0x14] == 0)) {
              piVar6 = unaff_EBP + 0x15;
            }
            iVar7 = _stricmp((char *)piVar6,"php");
            if (iVar7 == 0) goto LAB_100365a2;
            piVar6 = (int *)unaff_EBP[0x13];
            if ((piVar6 == (int *)0x0) || (unaff_EBP[0x14] == 0)) {
              piVar6 = unaff_EBP + 0x15;
            }
            iVar7 = _stricmp((char *)piVar6,"cgi");
            if (iVar7 == 0) goto LAB_100365a2;
            piVar6 = (int *)unaff_EBP[0x13];
            if ((piVar6 == (int *)0x0) || (unaff_EBP[0x14] == 0)) {
              piVar6 = unaff_EBP + 0x15;
            }
            iVar7 = _stricmp((char *)piVar6,"pl");
            if (iVar7 == 0) goto LAB_100365a2;
            iVar7 = unaff_EBP[0x13];
            if ((iVar7 == 0) || (unaff_EBP[0x14] == 0)) {
              FUN_10001090(unaff_EBP + -0x15,unaff_EBP + 0x15);
              *(undefined1 *)((int)unaff_EBP + -0x2a) = 1;
              goto LAB_100366c5;
            }
          }
          else {
            pcVar13 = "flv";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + 0xe);
            iVar7 = _stricmp(pcVar10,pcVar13);
            if (iVar7 != 0) goto LAB_10036447;
LAB_1003642d:
            iVar7 = FUN_10003910(unaff_EBP + 0xe);
          }
          FUN_10001090(unaff_EBP + -0x15,iVar7);
          *(undefined1 *)((int)unaff_EBP + -0x2a) = 1;
        }
LAB_100366c5:
        *(undefined1 *)(unaff_EBP + -1) = 0x12;
        (*DAT_1005abec)(unaff_EBP + 0xe);
        *(undefined1 *)(unaff_EBP + -1) = 0x11;
        FUN_10001910(unaff_EBP + 0x12);
        *(undefined1 *)(unaff_EBP + -1) = 0x10;
        FUN_10001910(unaff_EBP + 6);
        pcVar29 = strchr_exref;
      }
      else {
        iVar7 = 0x2e;
        pcVar10 = (char *)FUN_10003910(iVar8 + 0x68);
        pcVar10 = strrchr(pcVar10,iVar7);
        if (pcVar10 != (char *)0x0) {
          FUN_10001090(unaff_EBP + -0x15,pcVar10 + 1);
        }
      }
      iVar9 = 0;
      unaff_EBP[-0xb6] = 0;
      iVar7 = 0;
      unaff_EBP[-0xd] = 0;
      iVar8 = FUN_10003890(unaff_EBP + -0x15);
      if (iVar8 != 0) {
        pcVar13 = "swf";
        pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
        iVar8 = _stricmp(pcVar10,pcVar13);
        if (iVar8 != 0) {
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
          iVar8 = FUN_10016370(pcVar10);
          if (iVar8 == 0) {
            iVar9 = 1;
            unaff_EBP[-0xb6] = 1;
          }
        }
        pcVar13 = "mp4";
        pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
        iVar8 = _stricmp(pcVar10,pcVar13);
        if ((iVar8 == 0) && (iVar8 = unaff_EBP[3], *(char *)(iVar8 + 0x11) == '\0')) {
          iVar65 = *(int *)(iVar8 + 0xe0);
          if (((*(uint *)(iVar8 + 0xdc) != 0 || iVar65 != 0) && (iVar65 < 1)) &&
             ((iVar65 < 0 || (*(uint *)(iVar8 + 0xdc) < 0x3e800)))) {
            iVar7 = 1;
            unaff_EBP[-0xd] = 1;
          }
        }
      }
      (*DAT_1005abe4)(unaff_EBP + -0x6d);
      *(undefined1 *)(unaff_EBP + -1) = 0x14;
      uVar68 = 0x25;
      uVar11 = FUN_10003910(unaff_EBP + -8);
      iVar8 = (*pcVar29)(uVar11,uVar68);
      if (iVar8 != 0) {
        FUN_10017d50((int *)unaff_EBP[4],unaff_EBP[-0x1a],unaff_EBP[1],unaff_EBP + -0x6d);
      }
      unaff_EBP[-0x69] = 0;
      unaff_EBP[-0x68] = 0;
      FUN_1000eab0(unaff_EBP + -0xa5);
      *(undefined1 *)(unaff_EBP + -1) = 0x15;
      unaff_EBP[-0x93] = 0;
      if ((DAT_1005ac5c != (HANDLE)0x0) &&
         (DVar4 = WaitForSingleObject(DAT_1005ac5c,1000), DVar4 == 0)) {
        *(undefined1 *)(unaff_EBP + -1) = 0x16;
        piVar6 = unaff_EBP + -0xa5;
        iVar65 = 0;
        iVar8 = 1;
        pbVar5 = (byte *)FUN_10003910(unaff_EBP + -8);
        iVar8 = FUN_1003c410(&DAT_1005c160,pbVar5,iVar8,iVar65,(int)piVar6);
        if (iVar8 == 0) {
          piVar6 = unaff_EBP + -0xa5;
          iVar8 = 1;
          pbVar5 = (byte *)FUN_10003910(unaff_EBP + -8);
          iVar8 = FUN_1003c410(&DAT_1005c170,pbVar5,iVar9,iVar8,(int)piVar6);
          if (iVar8 != 0) {
            unaff_EBP[-0xd] = 1;
            goto LAB_10036908;
          }
          if (iVar7 != 0) goto LAB_10036908;
          iVar7 = FUN_10003890(unaff_EBP + -0x6d);
          if (iVar7 != 0) {
            pbVar5 = (byte *)FUN_10003910(unaff_EBP + -8);
            pbVar17 = (byte *)FUN_10003910(unaff_EBP + -0x6d);
            do {
              bVar1 = *pbVar17;
              bVar31 = bVar1 < *pbVar5;
              if (bVar1 != *pbVar5) {
LAB_100368d0:
                iVar7 = (1 - (uint)bVar31) - (uint)(bVar31 != 0);
                goto LAB_100368d5;
              }
              if (bVar1 == 0) break;
              bVar1 = pbVar17[1];
              bVar31 = bVar1 < pbVar5[1];
              if (bVar1 != pbVar5[1]) goto LAB_100368d0;
              pbVar17 = pbVar17 + 2;
              pbVar5 = pbVar5 + 2;
            } while (bVar1 != 0);
            iVar7 = 0;
LAB_100368d5:
            if (iVar7 != 0) {
              piVar6 = unaff_EBP + -0xa5;
              iVar8 = 0;
              iVar7 = 1;
              pbVar5 = (byte *)FUN_10003910(unaff_EBP + -0x6d);
              iVar7 = FUN_1003c680(&DAT_1005c160,pbVar5,iVar7,iVar8,(int)piVar6);
              unaff_EBP[-0xd] = iVar7;
              if (iVar7 != 0) goto LAB_10036908;
            }
          }
        }
        else {
          unaff_EBP[-0xd] = 1;
LAB_10036908:
          pcVar13 = "youtube.com/watch?";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          pcVar10 = strstr(pcVar10,pcVar13);
          if (pcVar10 == (char *)0x0) {
            pcVar13 = "googlevideo.com/watch?";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar10 = strstr(pcVar10,pcVar13);
            if (pcVar10 == (char *)0x0) goto LAB_100369f2;
          }
          pcVar13 = "?v=";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          pcVar10 = strstr(pcVar10,pcVar13);
          if (pcVar10 == (char *)0x0) {
            pcVar13 = "&spf=";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar10 = strstr(pcVar10,pcVar13);
            if (pcVar10 == (char *)0x0) {
              pcVar13 = "&v=";
              pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
              pcVar10 = strstr(pcVar10,pcVar13);
              if (pcVar10 == (char *)0x0) goto LAB_100369f2;
            }
          }
          iVar7 = unaff_EBP[3];
          iVar8 = FUN_10003890(iVar7 + 0x88);
          if (iVar8 != 0) {
            sVar69 = 0x10;
            pcVar13 = "application/json";
            pcVar10 = (char *)FUN_10003910(iVar7 + 0x88);
            iVar7 = _strnicmp(pcVar10,pcVar13,sVar69);
            if (iVar7 == 0) {
              pwVar70 = L"youtube.com/";
              pwVar16 = (wchar_t *)FUN_100038b0(unaff_EBP + -0x97);
              pwVar16 = wcsstr(pwVar16,pwVar70);
              if (pwVar16 != (wchar_t *)0x0) {
                FUN_1003ba50(&DAT_1005c170,unaff_EBP[-0x8d],unaff_EBP[-0x8b],unaff_EBP[-0x8a]);
              }
            }
          }
        }
LAB_100369f2:
        unaff_EBP[-1] = 0x15;
        ReleaseMutex(DAT_1005ac5c);
        iVar7 = unaff_EBP[-0xd];
      }
      if (((iVar7 != 0) && (*(char *)(unaff_EBP[3] + 0x11) != '\0')) && (unaff_EBP[-0x89] == 10)) {
        iVar7 = 0;
        unaff_EBP[-0xd] = 0;
      }
      *(undefined1 *)((int)unaff_EBP + -0x87) = 0;
      pcVar13 = "youtube.com/videoplayback?";
      pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
      pcVar10 = strstr(pcVar10,pcVar13);
      if (pcVar10 == (char *)0x0) {
        pcVar13 = "googlevideo.com/videoplayback?";
        pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
        pcVar10 = strstr(pcVar10,pcVar13);
        if (pcVar10 != (char *)0x0) goto LAB_10036a65;
LAB_10036b27:
        if ((iVar7 != 0) && (unaff_EBP[-0x93] == 0)) {
          piVar28 = (int *)unaff_EBP[4];
          piVar6 = unaff_EBP + -0xa5;
          piVar66 = (int *)unaff_EBP[1];
          iVar8 = unaff_EBP[-0x1a];
          pbVar5 = (byte *)FUN_10003910(unaff_EBP + -8);
          FUN_10030bd0(piVar28,pbVar5,iVar8,piVar66,(int)piVar6);
        }
      }
      else {
LAB_10036a65:
        *(undefined1 *)((int)unaff_EBP + -0x87) = 1;
        if (iVar7 != 0) {
          iVar9 = unaff_EBP[3] + 0x88;
          iVar8 = FUN_10003890(iVar9);
          if (iVar8 != 0) {
            sVar69 = 0x18;
            pcVar10 = "application/octet-stream";
            pvVar12 = (void *)FUN_10003910(iVar9);
            iVar8 = _memicmp(pvVar12,pcVar10,sVar69);
            if (iVar8 != 0) {
              sVar69 = 10;
              pcVar10 = "text/plain";
              pvVar12 = (void *)FUN_10003910(iVar9);
              iVar8 = _memicmp(pvVar12,pcVar10,sVar69);
              if (iVar8 != 0) goto LAB_10036b27;
            }
            pcVar13 = "&itag=";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar10 = strstr(pcVar10,pcVar13);
            if (pcVar10 == (char *)0x0) {
              pcVar13 = "?itag=";
              pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
              pcVar10 = strstr(pcVar10,pcVar13);
              if (pcVar10 == (char *)0x0) goto LAB_10036b27;
            }
            iVar8 = sscanf(pcVar10 + 6,"%ld",unaff_EBP + -0x43);
            if (iVar8 == 1) {
              FUN_100043f0(unaff_EBP[-0x43],unaff_EBP + -0x15);
            }
          }
          goto LAB_10036b27;
        }
      }
      unaff_EBP[-0x2b] = 0;
      unaff_EBP[-0x24] = 0;
      pcVar29 = _stricmp_exref;
      if (iVar7 != 0) {
        pcVar30 = strstr_exref;
        if (DAT_1005a678 != 0) {
          pcVar13 = "vimeocdn.com";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          pcVar30 = strstr_exref;
          pcVar10 = strstr(pcVar10,pcVar13);
          if (pcVar10 == (char *)0x0) {
            pcVar13 = "akamaihd.net";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar10 = strstr(pcVar10,pcVar13);
            if (pcVar10 == (char *)0x0) {
              pcVar13 = "vimeo.com";
              pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
              pcVar10 = strstr(pcVar10,pcVar13);
              if (pcVar10 == (char *)0x0) goto LAB_10036bfc;
            }
          }
          FUN_10010450(&PTR_vftable_1005a674,unaff_EBP[-0x8b],unaff_EBP[-0x8a],unaff_EBP[-0x8d]);
        }
LAB_10036bfc:
        pcVar29 = _stricmp_exref;
        if (DAT_1005a670 != 0) {
          pcVar10 = "_Seg1-Frag";
          uVar11 = FUN_10003910(unaff_EBP + -8);
          iVar7 = (*pcVar30)(uVar11,pcVar10);
          pcVar29 = _stricmp_exref;
          if (iVar7 != 0) {
            unaff_EBP[10] = (int)MyStrClass::vftable;
            unaff_EBP[0xb] = 0;
            unaff_EBP[0xc] = 0;
            *(undefined1 *)(unaff_EBP + 0xd) = 0;
            *(undefined1 *)(unaff_EBP + -1) = 0x18;
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
            FUN_10001950(unaff_EBP + 10,pcVar10);
            FUN_10018cd0(unaff_EBP + 10,"&als=","?als=");
            FUN_10018cd0(unaff_EBP + 10,"&hdcore=","?hdcore=");
            *(undefined1 *)((int)unaff_EBP + -0x85) = 0;
            *(undefined1 *)((int)unaff_EBP + -0x55) = 0;
            for (iVar7 = DAT_1005a670; iVar7 != 0; iVar7 = *(int *)(iVar7 + 0x4c)) {
              iVar8 = *(int *)(iVar7 + 0x48);
LAB_10036c90:
              if (iVar8 != 0) {
                unaff_EBP[0xe] = (int)MyStrClass::vftable;
                unaff_EBP[0xf] = 0;
                unaff_EBP[0x10] = 0;
                *(undefined1 *)(unaff_EBP + 0x11) = 0;
                *(undefined1 *)(unaff_EBP + -1) = 0x19;
                piVar6 = unaff_EBP + 0xe;
                iVar9 = 0;
                pcVar10 = (char *)FUN_10001930(iVar8);
                pcVar13 = (char *)FUN_10001930(iVar7 + 8);
                cVar3 = FUN_1001af50(pcVar13,pcVar10,iVar9,piVar6);
                if (cVar3 == '\0') goto LAB_10036d92;
                piVar6 = (int *)unaff_EBP[0xb];
                if ((piVar6 == (int *)0x0) || (unaff_EBP[0xc] == 0)) {
                  piVar6 = unaff_EBP + 0xd;
                }
                piVar28 = (int *)unaff_EBP[0xf];
                if ((piVar28 == (int *)0x0) || (unaff_EBP[0x10] == 0)) {
                  piVar28 = unaff_EBP + 0x11;
                }
                iVar9 = strncmp((char *)piVar28,(char *)piVar6,unaff_EBP[0x10]);
                if (iVar9 != 0) goto LAB_10036d92;
                piVar6 = (int *)unaff_EBP[0xb];
                if ((piVar6 == (int *)0x0) || (unaff_EBP[0xc] == 0)) {
                  piVar6 = unaff_EBP + 0xd;
                }
                iVar9 = strncmp((char *)(unaff_EBP[0x10] + (int)piVar6),"Seg1-Frag",9);
                if (iVar9 != 0) goto LAB_10036d92;
                *(undefined1 *)((int)unaff_EBP + -0x85) = 1;
                unaff_EBP[-0x24] = iVar7;
                piVar6 = (int *)unaff_EBP[0xb];
                if ((piVar6 == (int *)0x0) || (unaff_EBP[0xc] == 0)) {
                  piVar6 = unaff_EBP + 0xd;
                }
                pcVar10 = (char *)(unaff_EBP[0x10] + 10 + (int)piVar6);
                while( true ) {
                  unaff_EBP[-0xb3] = (int)pcVar10;
                  iVar8 = isdigit((int)*pcVar10);
                  if (iVar8 == 0) break;
                  pcVar10 = pcVar10 + 1;
                }
                unaff_EBP[-0x67] = (int)pcVar10;
                *(undefined1 *)(unaff_EBP + -1) = 0x18;
                FUN_10001910(unaff_EBP + 0xe);
              }
              if (*(char *)((int)unaff_EBP + -0x85) != '\0') {
                pcVar13 = "_Seg1-Frag1";
                pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
                pcVar10 = strstr(pcVar10,pcVar13);
                iVar7 = unaff_EBP[-0x24];
                if (pcVar10 != (char *)0x0) {
                  FUN_10010a60(iVar7);
                  piVar6 = (int *)unaff_EBP[0xb];
                  if ((piVar6 == (int *)0x0) || (unaff_EBP[0xc] == 0)) {
                    piVar6 = unaff_EBP + 0xd;
                  }
                  FUN_10001950((void *)(iVar7 + 0x34),(char *)piVar6);
                  unaff_EBP[-0x2b] = iVar7;
                  goto LAB_10036e8d;
                }
                if (*(int *)(iVar7 + 0x30) == 0) goto LAB_10036e8d;
                if (*(int *)(iVar7 + 0x28) != 0) {
LAB_10036e52:
                  if (*(char *)((int)unaff_EBP + -0x55) != '\0') goto LAB_10036e93;
                  puVar64 = (undefined4 *)unaff_EBP[-0x24];
                  FUN_10015240(&PTR_vftable_1005a66c,(int)puVar64);
                  FUN_10010a70(puVar64);
                  goto LAB_10036e70;
                }
                pcVar10 = (char *)FUN_10001930(iVar7 + 0x34);
                pcVar10 = strstr(pcVar10,"_Seg1-Frag1");
                if (pcVar10 == (char *)0x0) goto LAB_10036e52;
                pbVar17 = (byte *)(pcVar10 + 0xb);
                pbVar5 = (byte *)unaff_EBP[-0x67];
                goto LAB_10036e23;
              }
            }
            iVar7 = unaff_EBP[-0x24];
LAB_10036e8d:
            if (*(char *)((int)unaff_EBP + -0x55) == '\0') {
LAB_10036e70:
              pcVar29 = _stricmp_exref;
              *(undefined1 *)(unaff_EBP + -1) = 0x15;
              FUN_10001910(unaff_EBP + 10);
            }
            else {
LAB_10036e93:
              FUN_10015240(&PTR_vftable_1005a66c,iVar7);
              unaff_EBP[-0x42] = 0;
              piVar6 = unaff_EBP + -0x42;
              puVar67 = (uint *)(unaff_EBP + -0x30);
              pcVar13 = "flv";
              pcVar10 = (char *)FUN_10001930(iVar7 + 8);
              iVar7 = FUN_100164f0(pcVar10,pcVar13,puVar67,piVar6);
              pcVar29 = _stricmp_exref;
              if (iVar7 == 0) {
                FUN_10010a70((undefined4 *)unaff_EBP[-0x24]);
                *(undefined1 *)(unaff_EBP + -1) = 0x15;
                FUN_10001910(unaff_EBP + 10);
              }
              else {
                FUN_10003c00(unaff_EBP + -10,(LPCSTR)unaff_EBP[-0x67]);
                *(undefined1 *)(unaff_EBP + -1) = 0x1a;
                iVar7 = *(int *)(unaff_EBP[-0x24] + 0x48);
                unaff_EBP[-0xf] = iVar7;
                while (pcVar29 = _stricmp_exref, iVar7 != 0) {
                  unaff_EBP[0x16] = (int)MyStrClass::vftable;
                  unaff_EBP[0x17] = 0;
                  unaff_EBP[0x18] = 0;
                  *(undefined1 *)(unaff_EBP + 0x19) = 0;
                  *(undefined1 *)(unaff_EBP + -1) = 0x1b;
                  piVar6 = unaff_EBP + 0x16;
                  iVar8 = 0;
                  pcVar10 = (char *)FUN_10001930(iVar7);
                  pcVar13 = (char *)FUN_10001930(unaff_EBP[-0x24] + 8);
                  cVar3 = FUN_1001af50(pcVar13,pcVar10,iVar8,piVar6);
                  if (cVar3 != '\0') {
                    (*DAT_1005abe4)(unaff_EBP + -0x28);
                    *(undefined1 *)(unaff_EBP + -1) = 0x1c;
                    (*DAT_1005abe4)(unaff_EBP + 0xe);
                    *(undefined1 *)(unaff_EBP + -1) = 0x1d;
                    iVar8 = FUN_10001930(unaff_EBP[-0x24] + 0x18);
                    FUN_10001090(unaff_EBP + -0x28,iVar8);
                    iVar8 = FUN_10001930(iVar7 + 0x40);
                    FUN_10001090(unaff_EBP + 0xe,iVar8);
                    if ((*(int *)(iVar7 + 0x60) != 0) && (*(int *)(iVar7 + 0x58) != 0)) {
                      FUN_10001070(unaff_EBP + 0xe," bitrate ");
                      iVar8 = FUN_10001930(iVar7 + 0x50);
                      FUN_10001070(unaff_EBP + 0xe,iVar8);
                    }
                    FUN_10003c00(unaff_EBP + -0x10,"dhs2toidm://");
                    *(undefined1 *)(unaff_EBP + -1) = 0x1e;
                    piVar6 = (int *)unaff_EBP[0x17];
                    if ((piVar6 == (int *)0x0) || (unaff_EBP[0x18] == 0)) {
                      piVar6 = unaff_EBP + 0x19;
                    }
                    FUN_10003c00(unaff_EBP + -0xc,(LPCSTR)piVar6);
                    *(undefined1 *)(unaff_EBP + -1) = 0x1f;
                    FUN_10005660(unaff_EBP + -0x10,unaff_EBP + -0xc);
                    pOVar18 = SysAllocString(L"Seg%ld-Frag%ld");
                    unaff_EBP[-0xa6] = (int)pOVar18;
                    *(undefined1 *)(unaff_EBP + -1) = 0x20;
                    FUN_10005660(unaff_EBP + -0x10,unaff_EBP + -0xa6);
                    *(undefined1 *)(unaff_EBP + -1) = 0x1f;
                    SysFreeString(pOVar18);
                    FUN_10005660(unaff_EBP + -0x10,unaff_EBP + -10);
                    unaff_EBP[-0x46] = 0;
                    unaff_EBP[-0xaa] = 0;
                    unaff_EBP[-0x18] = 0;
                    unaff_EBP[-0x19] = unaff_EBP[3] + -4;
                    piVar66 = (int *)unaff_EBP[-0x1a];
                    FUN_10020060((int *)unaff_EBP[4],piVar66,(int *)unaff_EBP[1],
                                 (uint *)(unaff_EBP + -0x46),unaff_EBP + -0xaa,unaff_EBP + -0x18);
                    (*DAT_1005abe4)(unaff_EBP + -0x3c);
                    *(undefined1 *)(unaff_EBP + -1) = 0x21;
                    (*DAT_1005abe4)(unaff_EBP + -0x37);
                    *(undefined1 *)(unaff_EBP + -1) = 0x22;
                    (*DAT_1005abe4)(unaff_EBP + -0x40);
                    *(undefined1 *)(unaff_EBP + -1) = 0x23;
                    iVar8 = (**(code **)(*(int *)unaff_EBP[4] + 0xc4))
                                      ((int *)unaff_EBP[4],piVar66,unaff_EBP + -0x3c);
                    unaff_EBP[2] = iVar8;
                    FUN_10005620(unaff_EBP + -0x2f,"User-Agent",0xffffffff);
                    *(undefined1 *)(unaff_EBP + -1) = 0x24;
                    iVar8 = (**(code **)(*(int *)unaff_EBP[4] + 0xe8))
                                      ((int *)unaff_EBP[4],piVar66,unaff_EBP + -0x2f,
                                       unaff_EBP + -0x40);
                    unaff_EBP[2] = iVar8;
                    FUN_10005620(unaff_EBP + -0x81,"Cookie",0xffffffff);
                    *(undefined1 *)(unaff_EBP + -1) = 0x25;
                    iVar8 = (**(code **)(*(int *)unaff_EBP[4] + 0xe8))
                                      ((int *)unaff_EBP[4],piVar66,unaff_EBP + -0x81,
                                       unaff_EBP + -0x37);
                    unaff_EBP[2] = iVar8;
                    (*DAT_1005abe4)(unaff_EBP + -0x52);
                    *(undefined1 *)(unaff_EBP + -1) = 0x26;
                    (*DAT_1005abe4)(unaff_EBP + -0x4e);
                    *(undefined1 *)(unaff_EBP + -1) = 0x27;
                    piVar6 = unaff_EBP + -0x4e;
                    piVar28 = unaff_EBP + -0x52;
                    pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
                    FUN_1003b030(&DAT_1005b918,pcVar10,piVar28,piVar6);
                    (*DAT_1005abc0)(unaff_EBP + -0x56);
                    *(undefined1 *)(unaff_EBP + -1) = 0x28;
                    (*DAT_1005abc0)(unaff_EBP + -0x79);
                    *(undefined1 *)(unaff_EBP + -1) = 0x29;
                    (*DAT_1005abc0)(unaff_EBP + -0x71);
                    *(undefined1 *)(unaff_EBP + -1) = 0x2a;
                    (*DAT_1005abc0)(unaff_EBP + -0x85);
                    *(undefined1 *)(unaff_EBP + -1) = 0x2b;
                    (*DAT_1005abc0)(unaff_EBP + -0x62);
                    *(undefined1 *)(unaff_EBP + -1) = 0x2c;
                    (*DAT_1005abc0)(unaff_EBP + -0xb1);
                    *(undefined1 *)(unaff_EBP + -1) = 0x2d;
                    (*DAT_1005abc0)(unaff_EBP + -0x5e);
                    *(undefined1 *)(unaff_EBP + -1) = 0x2e;
                    (*DAT_1005abc0)(unaff_EBP + -0x66);
                    *(undefined1 *)(unaff_EBP + -1) = 0x2f;
                    iVar8 = FUN_10003890(unaff_EBP + -0x37);
                    if (iVar8 != 0) {
                      (*DAT_1005ac08)(unaff_EBP + -0x37,0,unaff_EBP + -0x79);
                    }
                    iVar8 = FUN_10003890(unaff_EBP + -0x40);
                    if (iVar8 != 0) {
                      (*DAT_1005ac08)(unaff_EBP + -0x40,0,unaff_EBP + -0x71);
                    }
                    iVar8 = FUN_10003890(unaff_EBP + -0x3c);
                    if (iVar8 != 0) {
                      (*DAT_1005ac08)(unaff_EBP + -0x3c,0,unaff_EBP + -0x56);
                    }
                    iVar8 = FUN_10003890(unaff_EBP + -0x52);
                    if (iVar8 != 0) {
                      (*DAT_1005ac08)(unaff_EBP + -0x52,0,unaff_EBP + -0x85);
                    }
                    iVar8 = FUN_10003890(unaff_EBP + -0x4e);
                    if (iVar8 != 0) {
                      (*DAT_1005ac08)(unaff_EBP + -0x4e,0,unaff_EBP + -0x62);
                    }
                    iVar8 = unaff_EBP[3];
                    iVar9 = FUN_10003890(iVar8 + 0x68);
                    if (iVar9 != 0) {
                      (*DAT_1005ac08)(iVar8 + 0x68,0,unaff_EBP + -0xb1);
                    }
                    if (unaff_EBP[-0x18] != 0) {
                      FUN_10005620(unaff_EBP + -0x5a,unaff_EBP[-0x18],0xffffffff);
                      *(undefined1 *)(unaff_EBP + -1) = 0x30;
                      (*DAT_1005ac08)(unaff_EBP + -0x5a,0,unaff_EBP + -0x66);
                      *(undefined1 *)(unaff_EBP + -1) = 0x2f;
                      (*DAT_1005abec)(unaff_EBP + -0x5a);
                    }
                    iVar8 = unaff_EBP[3];
                    iVar9 = FUN_10003890(iVar8 + 0x88);
                    if (iVar9 != 0) {
                      (*DAT_1005ac08)(iVar8 + 0x88,0,unaff_EBP + -0x5e);
                    }
                    (*DAT_1005abc0)(unaff_EBP + 6);
                    *(undefined1 *)(unaff_EBP + -1) = 0x31;
                    iVar8 = FUN_10003870(unaff_EBP + -0x97);
                    if (iVar8 == 0) {
                      (*DAT_1005abe4)(unaff_EBP + 0x12);
                      *(undefined1 *)(unaff_EBP + -1) = 0x32;
                      iVar8 = FUN_10019e90((int *)unaff_EBP[4],piVar66,unaff_EBP[1],unaff_EBP + 0x12
                                          );
                      if ((iVar8 != 0) && (iVar8 = FUN_10003890(unaff_EBP + 0x12), iVar8 != 0)) {
                        (*DAT_1005ac08)(unaff_EBP + 0x12,0,unaff_EBP + 6);
                      }
                      *(undefined1 *)(unaff_EBP + -1) = 0x31;
                      (*DAT_1005abec)(unaff_EBP + 0x12);
                    }
                    else {
                      FUN_100038d0(unaff_EBP + 6,unaff_EBP + -0x97);
                    }
                    pcVar13 = (char *)FUN_10001930(iVar7 + 0x20);
                    pcVar10 = pcVar13 + 1;
                    do {
                      cVar3 = *pcVar13;
                      pcVar13 = pcVar13 + 1;
                    } while (cVar3 != '\0');
                    pcVar19 = (char *)FUN_10001930(iVar7 + 0x10);
                    pcVar14 = pcVar19 + 1;
                    do {
                      cVar3 = *pcVar19;
                      pcVar19 = pcVar19 + 1;
                    } while (cVar3 != '\0');
                    pcVar20 = operator_new((uint)(pcVar13 +
                                                 (int)(pcVar19 +
                                                      ((0x80 - (int)pcVar14) - (int)pcVar10))));
                    unaff_EBP[-0x42] = (int)pcVar20;
                    if (pcVar20 != (char *)0x0) {
                      iVar7 = FUN_10001930(iVar7 + 0x10);
                      iVar8 = FUN_10001930(unaff_EBP[-0xf] + 0x20);
                      _snprintf(pcVar20,(size_t)(pcVar13 +
                                                (int)(pcVar19 +
                                                     ((0x80 - (int)pcVar14) - (int)pcVar10))),
                                "<bootstrapInfo>%s</bootstrapInfo><metadata>%s</metadata>",iVar8,
                                iVar7);
                      FUN_10003c00(unaff_EBP + -0xe,pcVar20);
                      *(undefined1 *)(unaff_EBP + -1) = 0x33;
                      pvVar12 = operator_new(0x2f8);
                      unaff_EBP[-0x30] = (int)pvVar12;
                      *(undefined1 *)(unaff_EBP + -1) = 0x34;
                      if (pvVar12 == (void *)0x0) {
                        piVar6 = (int *)0x0;
                      }
                      else {
                        iVar7 = FUN_10003890(unaff_EBP + 0xe);
                        if (iVar7 == 0) {
                          unaff_EBP[-0x17] = 0;
                        }
                        else {
                          iVar7 = FUN_10003910(unaff_EBP + 0xe);
                          unaff_EBP[-0x17] = iVar7;
                        }
                        iVar8 = 0;
                        iVar7 = FUN_10003890(unaff_EBP + -0x28);
                        if (iVar7 == 0) {
                          unaff_EBP[-0x21] = 0;
                        }
                        else {
                          iVar7 = FUN_10003910(unaff_EBP + -0x28);
                          unaff_EBP[-0x21] = iVar7;
                        }
                        iVar7 = FUN_10003870(unaff_EBP + 6);
                        if (iVar7 == 0) {
                          unaff_EBP[-0x1f] = 0;
                        }
                        else {
                          iVar7 = FUN_100038b0(unaff_EBP + 6);
                          unaff_EBP[-0x1f] = iVar7;
                        }
                        iVar7 = FUN_10003870(unaff_EBP + -0x9b);
                        if (iVar7 == 0) {
                          unaff_EBP[-0x20] = 0;
                        }
                        else {
                          iVar7 = FUN_100038b0(unaff_EBP + -0x9b);
                          unaff_EBP[-0x20] = iVar7;
                        }
                        iVar7 = FUN_10003870(unaff_EBP + -0x5e);
                        if (iVar7 == 0) {
                          unaff_EBP[-0x11] = 0;
                        }
                        else {
                          iVar7 = FUN_100038b0(unaff_EBP + -0x5e);
                          unaff_EBP[-0x11] = iVar7;
                        }
                        iVar7 = FUN_10003870(unaff_EBP + -0x71);
                        if (iVar7 == 0) {
                          unaff_EBP[-0x1c] = 0;
                        }
                        else {
                          iVar7 = FUN_100038b0(unaff_EBP + -0x71);
                          unaff_EBP[-0x1c] = iVar7;
                        }
                        iVar7 = FUN_10003870(unaff_EBP + -0xb1);
                        if (iVar7 == 0) {
                          unaff_EBP[-0x1d] = 0;
                        }
                        else {
                          iVar7 = FUN_100038b0(unaff_EBP + -0xb1);
                          unaff_EBP[-0x1d] = iVar7;
                        }
                        iVar7 = FUN_10003870(unaff_EBP + -0x62);
                        if (iVar7 == 0) {
                          unaff_EBP[-0x1b] = 0;
                        }
                        else {
                          iVar7 = FUN_100038b0(unaff_EBP + -0x62);
                          unaff_EBP[-0x1b] = iVar7;
                        }
                        iVar7 = FUN_10003870(unaff_EBP + -0x85);
                        if (iVar7 == 0) {
                          unaff_EBP[-0x1e] = 0;
                        }
                        else {
                          iVar7 = FUN_100038b0(unaff_EBP + -0x85);
                          unaff_EBP[-0x1e] = iVar7;
                        }
                        iVar7 = FUN_10003870(unaff_EBP + -0x66);
                        if (iVar7 != 0) {
                          iVar8 = FUN_100038b0(unaff_EBP + -0x66);
                        }
                        iVar7 = FUN_10003870(unaff_EBP + -0x79);
                        if (iVar7 == 0) {
                          iVar7 = 0;
                        }
                        else {
                          iVar7 = FUN_100038b0(unaff_EBP + -0x79);
                        }
                        iVar9 = FUN_10003870(unaff_EBP + -0x56);
                        if (iVar9 == 0) {
                          iVar9 = 0;
                        }
                        else {
                          iVar9 = FUN_100038b0(unaff_EBP + -0x56);
                        }
                        piVar6 = FUN_1000e0e0(pvVar12,(uint *)unaff_EBP[-0x91],
                                              (int **)unaff_EBP[-0x90],unaff_EBP[-0x87],
                                              (wchar_t *)unaff_EBP[-0x8e],unaff_EBP[-0x93],
                                              (wchar_t *)unaff_EBP[-0x10],iVar9,iVar7,iVar8,
                                              unaff_EBP[-0x1e],unaff_EBP[-0x1b],unaff_EBP[-0x1d],
                                              unaff_EBP[-0x1c],unaff_EBP[-0x11],0x1004cc00,
                                              (LPCWSTR)unaff_EBP[-0x20],0,1,1,0,0,unaff_EBP[-0xe],
                                              unaff_EBP[-0x1f],(wchar_t *)0x0,0,0,0,0,0,
                                              unaff_EBP[-0x8b],unaff_EBP[-0x8a],unaff_EBP[-0x8d],0,0
                                              ,unaff_EBP[-0x21],unaff_EBP[-0x17],0,0);
                        pcVar20 = (char *)unaff_EBP[-0x42];
                      }
                      *(undefined1 *)(unaff_EBP + -1) = 0x33;
                      if (piVar6 != (int *)0x0) {
                        if (piVar6[2] == 0) {
                          iVar7 = piVar6[3];
                        }
                        else {
                          iVar7 = piVar6[2];
                        }
                        *(int *)(unaff_EBP[3] + 0xec) = iVar7;
                        if (piVar6[2] == 0) {
                          (**(code **)(*piVar6 + 0x18))(1);
                        }
                      }
                      operator_delete(pcVar20);
                      *(undefined1 *)(unaff_EBP + -1) = 0x31;
                      SysFreeString((BSTR)unaff_EBP[-0xe]);
                    }
                    *(undefined1 *)(unaff_EBP + -1) = 0x2f;
                    (*DAT_1005abc8)(unaff_EBP + 6);
                    *(undefined1 *)(unaff_EBP + -1) = 0x2e;
                    (*DAT_1005abc8)(unaff_EBP + -0x66);
                    *(undefined1 *)(unaff_EBP + -1) = 0x2d;
                    (*DAT_1005abc8)(unaff_EBP + -0x5e);
                    *(undefined1 *)(unaff_EBP + -1) = 0x2c;
                    (*DAT_1005abc8)(unaff_EBP + -0xb1);
                    *(undefined1 *)(unaff_EBP + -1) = 0x2b;
                    (*DAT_1005abc8)(unaff_EBP + -0x62);
                    *(undefined1 *)(unaff_EBP + -1) = 0x2a;
                    (*DAT_1005abc8)(unaff_EBP + -0x85);
                    *(undefined1 *)(unaff_EBP + -1) = 0x29;
                    (*DAT_1005abc8)(unaff_EBP + -0x71);
                    *(undefined1 *)(unaff_EBP + -1) = 0x28;
                    (*DAT_1005abc8)(unaff_EBP + -0x79);
                    *(undefined1 *)(unaff_EBP + -1) = 0x27;
                    (*DAT_1005abc8)(unaff_EBP + -0x56);
                    *(undefined1 *)(unaff_EBP + -1) = 0x26;
                    (*DAT_1005abec)(unaff_EBP + -0x4e);
                    *(undefined1 *)(unaff_EBP + -1) = 0x25;
                    (*DAT_1005abec)(unaff_EBP + -0x52);
                    *(undefined1 *)(unaff_EBP + -1) = 0x24;
                    (*DAT_1005abec)(unaff_EBP + -0x81);
                    *(undefined1 *)(unaff_EBP + -1) = 0x23;
                    (*DAT_1005abec)(unaff_EBP + -0x2f);
                    *(undefined1 *)(unaff_EBP + -1) = 0x22;
                    (*DAT_1005abec)(unaff_EBP + -0x40);
                    *(undefined1 *)(unaff_EBP + -1) = 0x21;
                    (*DAT_1005abec)(unaff_EBP + -0x37);
                    *(undefined1 *)(unaff_EBP + -1) = 0x1f;
                    (*DAT_1005abec)(unaff_EBP + -0x3c);
                    *(undefined1 *)(unaff_EBP + -1) = 0x1e;
                    SysFreeString((BSTR)unaff_EBP[-0xc]);
                    *(undefined1 *)(unaff_EBP + -1) = 0x1d;
                    SysFreeString((BSTR)unaff_EBP[-0x10]);
                    *(undefined1 *)(unaff_EBP + -1) = 0x1c;
                    (*DAT_1005abec)(unaff_EBP + 0xe);
                    *(undefined1 *)(unaff_EBP + -1) = 0x1b;
                    (*DAT_1005abec)(unaff_EBP + -0x28);
                    iVar7 = unaff_EBP[-0xf];
                  }
                  iVar7 = *(int *)(iVar7 + 100);
                  unaff_EBP[-0xf] = iVar7;
                  *(undefined1 *)(unaff_EBP + -1) = 0x1a;
                  FUN_10001910(unaff_EBP + 0x16);
                }
                *(undefined1 *)(unaff_EBP + -1) = 0x18;
                SysFreeString((BSTR)unaff_EBP[-10]);
                FUN_10010a70((undefined4 *)unaff_EBP[-0x24]);
                *(undefined1 *)(unaff_EBP + -1) = 0x15;
                FUN_10001910(unaff_EBP + 10);
              }
            }
          }
        }
      }
      iVar7 = FUN_10003890(unaff_EBP + -0x15);
      if (iVar7 != 0) {
        iVar7 = unaff_EBP[3];
        if (*(int *)(iVar7 + 0xf0) != 0) {
          (*DAT_1005abe4)(unaff_EBP + -0x28);
          *(undefined1 *)(unaff_EBP + -1) = 0x35;
          FUN_10001090(unaff_EBP + -0x28,"name.");
          uVar11 = FUN_10003910(unaff_EBP + -0x15);
          FUN_10001070(unaff_EBP + -0x28,uVar11);
          iVar8 = 0;
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x28);
          cVar3 = FUN_10030aa0(pcVar10,iVar8);
          if (cVar3 != '\0') {
            *(undefined1 *)((int)unaff_EBP + 0x17) = 1;
          }
          *(undefined1 *)(unaff_EBP + -1) = 0x15;
          (*DAT_1005abec)(unaff_EBP + -0x28);
        }
        if (*(char *)((int)unaff_EBP + 0x17) != '\0') {
          if ((*(int *)(iVar7 + 0xe0) < 1) &&
             ((*(int *)(iVar7 + 0xe0) < 0 || (*(uint *)(iVar7 + 0xdc) < 0x400000)))) {
            puVar71 = &DAT_1004fb50;
            uVar11 = FUN_10003910(unaff_EBP + -0x15);
            iVar8 = (*pcVar29)(uVar11,puVar71);
            if (iVar8 == 0) {
              *(undefined1 *)((int)unaff_EBP + 0x17) = 0;
            }
          }
          iVar8 = *(int *)(iVar7 + 0xe0);
          if (((*(uint *)(iVar7 + 0xdc) != 0 || iVar8 != 0) && (iVar8 < 1)) &&
             ((iVar8 < 0 || (*(uint *)(iVar7 + 0xdc) < 0x400000)))) {
            puVar71 = &DAT_1004fe14;
            uVar11 = FUN_10003910(unaff_EBP + -0x15);
            iVar8 = (*pcVar29)(uVar11,puVar71);
            if (iVar8 == 0) {
              *(undefined1 *)((int)unaff_EBP + 0x17) = 0;
            }
          }
          iVar8 = FUN_10003890(iVar7 + 0x88);
          if (iVar8 != 0) {
            sVar69 = 0x10;
            pcVar13 = "application/json";
            pcVar10 = (char *)FUN_10003910(iVar7 + 0x88);
            iVar7 = _strnicmp(pcVar10,pcVar13,sVar69);
            if (iVar7 == 0) {
              puVar71 = &DAT_1004fb50;
              uVar11 = FUN_10003910(unaff_EBP + -0x15);
              iVar7 = (*pcVar29)(uVar11,puVar71);
              if (iVar7 != 0) {
                puVar71 = &DAT_1004fb64;
                uVar11 = FUN_10003910(unaff_EBP + -0x15);
                iVar7 = (*pcVar29)(uVar11,puVar71);
                if (iVar7 != 0) {
                  puVar71 = &DAT_1004ed20;
                  uVar11 = FUN_10003910(unaff_EBP + -0x15);
                  iVar7 = (*pcVar29)(uVar11,puVar71);
                  if (iVar7 != 0) {
                    puVar71 = &DAT_1004fb60;
                    uVar11 = FUN_10003910(unaff_EBP + -0x15);
                    iVar7 = (*pcVar29)(uVar11,puVar71);
                    if (iVar7 != 0) {
                      puVar71 = &DAT_1004fb5c;
                      uVar11 = FUN_10003910(unaff_EBP + -0x15);
                      iVar7 = (*pcVar29)(uVar11,puVar71);
                      if (iVar7 != 0) {
                        puVar71 = &DAT_1004fb58;
                        uVar11 = FUN_10003910(unaff_EBP + -0x15);
                        iVar7 = (*pcVar29)(uVar11,puVar71);
                        if (iVar7 != 0) {
                          puVar71 = &DAT_1004fb54;
                          uVar11 = FUN_10003910(unaff_EBP + -0x15);
                          iVar7 = (*pcVar29)(uVar11,puVar71);
                          if (iVar7 != 0) goto LAB_10037abf;
                        }
                      }
                    }
                  }
                }
              }
              *(undefined1 *)((int)unaff_EBP + 0x17) = 0;
            }
          }
        }
LAB_10037abf:
        puVar71 = &DAT_1004e230;
        uVar11 = FUN_10003910(unaff_EBP + -0x15);
        iVar7 = (*pcVar29)(uVar11,puVar71);
        if (iVar7 == 0) {
          unaff_EBP[-0x69] = 1;
          if (*(char *)((int)unaff_EBP + 0x17) != '\0') {
            unaff_EBP[-0x45] = 0;
            piVar6 = unaff_EBP + -0x45;
            puVar67 = (uint *)(unaff_EBP + -0x46);
            pcVar13 = "FLV";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
            iVar7 = FUN_100164f0(pcVar10,pcVar13,puVar67,piVar6);
            if ((iVar7 != 0) || ((unaff_EBP[-0x46] != 0 && (unaff_EBP[-0xd] != 0)))) {
              *(undefined1 *)((int)unaff_EBP + 0x17) = 0;
            }
          }
          unaff_EBP[-0x21] = 0;
          unaff_EBP[-0x1c] = 0;
          unaff_EBP[-0x1f] = 0;
          piVar66 = (int *)unaff_EBP[-0x1a];
          FUN_10020060((int *)unaff_EBP[4],piVar66,(int *)unaff_EBP[1],(uint *)(unaff_EBP + -0x21),
                       unaff_EBP + -0x1c,unaff_EBP + -0x1f);
          (*DAT_1005abe4)(unaff_EBP + -0x56);
          *(undefined1 *)(unaff_EBP + -1) = 0x36;
          (*DAT_1005abe4)(unaff_EBP + -0x4e);
          *(undefined1 *)(unaff_EBP + -1) = 0x37;
          (*DAT_1005abe4)(unaff_EBP + -0x52);
          *(undefined1 *)(unaff_EBP + -1) = 0x38;
          iVar7 = (**(code **)(*(int *)unaff_EBP[4] + 0xc4))
                            ((int *)unaff_EBP[4],piVar66,unaff_EBP + -0x56);
          unaff_EBP[2] = iVar7;
          FUN_10005620(unaff_EBP + -0x81,"User-Agent",0xffffffff);
          *(undefined1 *)(unaff_EBP + -1) = 0x39;
          iVar7 = (**(code **)(*(int *)unaff_EBP[4] + 0xe8))
                            ((int *)unaff_EBP[4],piVar66,unaff_EBP + -0x81,unaff_EBP + -0x52);
          unaff_EBP[2] = iVar7;
          FUN_10005620(unaff_EBP + -0x5a,"Cookie",0xffffffff);
          *(undefined1 *)(unaff_EBP + -1) = 0x3a;
          iVar7 = (**(code **)(*(int *)unaff_EBP[4] + 0xe8))
                            ((int *)unaff_EBP[4],piVar66,unaff_EBP + -0x5a,unaff_EBP + -0x4e);
          unaff_EBP[2] = iVar7;
          (*DAT_1005abe4)(unaff_EBP + -0x40);
          *(undefined1 *)(unaff_EBP + -1) = 0x3b;
          (*DAT_1005abe4)(unaff_EBP + -0x37);
          *(undefined1 *)(unaff_EBP + -1) = 0x3c;
          piVar6 = unaff_EBP + -0x37;
          piVar28 = unaff_EBP + -0x40;
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          FUN_1003b030(&DAT_1005b918,pcVar10,piVar28,piVar6);
          (*DAT_1005abe4)(unaff_EBP + -0x3c);
          *(undefined1 *)(unaff_EBP + -1) = 0x3d;
          if (unaff_EBP[-0xd] != 0) {
            FUN_10016110(unaff_EBP + -0x9b,unaff_EBP + -0x3c);
          }
          (*DAT_1005abc0)(unaff_EBP + 0xe);
          *(undefined1 *)(unaff_EBP + -1) = 0x3e;
          if ((unaff_EBP[-0xd] == 0) || (iVar7 = FUN_10003870(unaff_EBP + -0x97), iVar7 == 0)) {
            (*DAT_1005abe4)(unaff_EBP + -0x28);
            *(undefined1 *)(unaff_EBP + -1) = 0x3f;
            iVar7 = FUN_10019e90((int *)unaff_EBP[4],piVar66,unaff_EBP[1],unaff_EBP + -0x28);
            if ((iVar7 != 0) && (iVar7 = FUN_10003890(unaff_EBP + -0x28), iVar7 != 0)) {
              (*DAT_1005ac08)(unaff_EBP + -0x28,0,unaff_EBP + 0xe);
            }
            *(undefined1 *)(unaff_EBP + -1) = 0x3e;
            (*DAT_1005abec)(unaff_EBP + -0x28);
          }
          else {
            FUN_100038d0(unaff_EBP + 0xe,unaff_EBP + -0x97);
          }
          (*DAT_1005abe4)(unaff_EBP + 0x16);
          *(undefined1 *)(unaff_EBP + -1) = 0x40;
          if (unaff_EBP[-0xd] == 0) {
            FUN_10017d50((int *)unaff_EBP[4],(int)piVar66,unaff_EBP[1],unaff_EBP + 0x16);
          }
          else {
            uVar11 = FUN_10003910(unaff_EBP + -0x9f);
            FUN_10001090(unaff_EBP + 0x16,uVar11);
          }
          iVar7 = FUN_10003890(unaff_EBP + 0x16);
          if (iVar7 == 0) {
LAB_10037dad:
            unaff_EBP[-10] = 0;
          }
          else {
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar13 = (char *)FUN_10003910(unaff_EBP + 0x16);
            iVar7 = _stricmp(pcVar13,pcVar10);
            if (iVar7 == 0) goto LAB_10037dad;
            iVar7 = FUN_10003910(unaff_EBP + 0x16);
            unaff_EBP[-10] = iVar7;
          }
          iVar7 = FUN_10003870(unaff_EBP + 0xe);
          if (iVar7 == 0) {
            unaff_EBP[-0x18] = 0;
          }
          else {
            iVar7 = FUN_100038b0(unaff_EBP + 0xe);
            unaff_EBP[-0x18] = iVar7;
          }
          iVar7 = unaff_EBP[3];
          iVar8 = iVar7 + 0x98;
          iVar9 = FUN_10003890(iVar8);
          if (iVar9 == 0) {
            unaff_EBP[-0x17] = 0;
          }
          else {
            iVar8 = FUN_10003910(iVar8);
            unaff_EBP[-0x17] = iVar8;
          }
          iVar8 = FUN_10003890(unaff_EBP + -0x3c);
          if (iVar8 == 0) {
            unaff_EBP[-0x20] = 0;
          }
          else {
            iVar8 = FUN_10003910(unaff_EBP + -0x3c);
            unaff_EBP[-0x20] = iVar8;
          }
          iVar7 = iVar7 + 0x68;
          iVar8 = FUN_10003890(iVar7);
          if (iVar8 == 0) {
            unaff_EBP[-0xf] = 0;
          }
          else {
            iVar7 = FUN_10003910(iVar7);
            unaff_EBP[-0xf] = iVar7;
          }
          iVar7 = FUN_10003890(unaff_EBP + -0x52);
          if (iVar7 == 0) {
            unaff_EBP[-0x11] = 0;
          }
          else {
            iVar7 = FUN_10003910(unaff_EBP + -0x52);
            unaff_EBP[-0x11] = iVar7;
          }
          iVar7 = FUN_10003890(unaff_EBP + -0x56);
          if (iVar7 == 0) {
            pcVar10 = (char *)0x0;
          }
          else {
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x56);
          }
          iVar7 = FUN_10003890(unaff_EBP + -0x37);
          if (iVar7 == 0) {
            ppcVar21 = (char **)0x0;
          }
          else {
            ppcVar21 = (char **)FUN_10003910(unaff_EBP + -0x37);
          }
          iVar7 = FUN_10003890(unaff_EBP + -0x40);
          if (iVar7 == 0) {
            ppbVar22 = (byte **)0x0;
          }
          else {
            ppbVar22 = (byte **)FUN_10003910(unaff_EBP + -0x40);
          }
          iVar7 = FUN_10003890(unaff_EBP + -0x4e);
          if (iVar7 == 0) {
            ppiVar23 = (int **)0x0;
          }
          else {
            ppiVar23 = (int **)FUN_10003910(unaff_EBP + -0x4e);
          }
          pcVar62 = (char *)0x0;
          uVar11 = 0;
          pcVar57 = (char *)0x0;
          pcVar13 = (char *)unaff_EBP[-10];
          psVar53 = (short *)unaff_EBP[-0x18];
          pwVar16 = (wchar_t *)unaff_EBP[-0x17];
          iVar7 = *(int *)(unaff_EBP[3] + 0xe0);
          pcVar14 = *(char **)(unaff_EBP[3] + 0xdc);
          cVar3 = '\0';
          pcVar20 = (char *)0x0;
          iVar8 = unaff_EBP[-0x21];
          pWVar47 = (WCHAR *)unaff_EBP[-0x1f];
          psVar44 = (short *)unaff_EBP[-0x20];
          piVar6 = (int *)unaff_EBP[-0xf];
          pcVar19 = (char *)unaff_EBP[-0x11];
          pbVar5 = (byte *)FUN_10003910(unaff_EBP + -8);
          FUN_100270e0(-(uint)(unaff_EBP[-0xd] != 0) & (uint)(unaff_EBP + -0xa5),
                       (int *)(unaff_EBP[3] + 0xec),pbVar5,ppiVar23,ppbVar22,ppcVar21,pcVar10,
                       pcVar19,piVar6,psVar44,pWVar47,iVar8,pcVar20,cVar3,pcVar14,iVar7,pwVar16,
                       psVar53,pcVar13,pcVar57,uVar11,pcVar62);
          if ((void *)unaff_EBP[-0x1c] != (void *)0x0) {
            operator_delete((void *)unaff_EBP[-0x1c]);
          }
          *(undefined1 *)(unaff_EBP + -1) = 0x3e;
          (*DAT_1005abec)(unaff_EBP + 0x16);
          *(undefined1 *)(unaff_EBP + -1) = 0x3d;
          (*DAT_1005abc8)(unaff_EBP + 0xe);
          *(undefined1 *)(unaff_EBP + -1) = 0x3c;
          (*DAT_1005abec)(unaff_EBP + -0x3c);
          *(undefined1 *)(unaff_EBP + -1) = 0x3b;
          (*DAT_1005abec)(unaff_EBP + -0x37);
          *(undefined1 *)(unaff_EBP + -1) = 0x3a;
          (*DAT_1005abec)(unaff_EBP + -0x40);
          *(undefined1 *)(unaff_EBP + -1) = 0x39;
          (*DAT_1005abec)(unaff_EBP + -0x5a);
          *(undefined1 *)(unaff_EBP + -1) = 0x38;
          (*DAT_1005abec)(unaff_EBP + -0x81);
          *(undefined1 *)(unaff_EBP + -1) = 0x37;
          (*DAT_1005abec)(unaff_EBP + -0x52);
          *(undefined1 *)(unaff_EBP + -1) = 0x36;
          (*DAT_1005abec)(unaff_EBP + -0x4e);
          *(undefined1 *)(unaff_EBP + -1) = 0x15;
          (*DAT_1005abec)(unaff_EBP + -0x56);
        }
      }
      *(undefined1 *)((int)unaff_EBP + -0x86) = 0;
      *(undefined1 *)(unaff_EBP + -1) = 0x41;
      unaff_EBP[-0xb4] = 0;
      iVar7 = FUN_10003890(unaff_EBP + -0x15);
      if (((iVar7 != 0) && (unaff_EBP[-0x69] == 0)) && (unaff_EBP[-0xd] != 0)) {
        unaff_EBP[-0xb4] = 1;
        unaff_EBP[-10] = 0;
        piVar6 = unaff_EBP + -10;
        puVar67 = (uint *)(unaff_EBP + -0x45);
        pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
        pcVar13 = (char *)FUN_10003910(unaff_EBP + -8);
        iVar7 = FUN_100164f0(pcVar13,pcVar10,puVar67,piVar6);
        unaff_EBP[-0x38] = iVar7;
        if (unaff_EBP[-0x45] != 0) {
          *(undefined1 *)((int)unaff_EBP + 0x17) = 0;
        }
        pcVar13 = "asf";
        pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
        iVar8 = _stricmp(pcVar10,pcVar13);
        iVar7 = unaff_EBP[3];
        if (iVar8 == 0) {
          iVar8 = *(int *)(iVar7 + 0xe0);
          if (((*(uint *)(iVar7 + 0xdc) != 0 || iVar8 != 0) && (iVar8 < 1)) &&
             ((iVar8 < 0 || (*(uint *)(iVar7 + 0xdc) < 0x19000)))) {
            unaff_EBP[-0x38] = 0;
            *(undefined1 *)((int)unaff_EBP + 0x17) = 0;
          }
        }
        if (*(char *)((int)unaff_EBP + -0x87) == '\0') {
          pcVar13 = "mp4";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
          iVar8 = _stricmp(pcVar10,pcVar13);
          if (iVar8 == 0) {
            iVar8 = *(int *)(iVar7 + 0xe0);
            if (((*(uint *)(iVar7 + 0xdc) != 0 || iVar8 != 0) && (iVar8 < 1)) &&
               ((iVar8 < 0 || (*(uint *)(iVar7 + 0xdc) < 0x3e800)))) {
              pcVar13 = "fbcdn.net";
              pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
              pcVar10 = strstr(pcVar10,pcVar13);
              if (pcVar10 != (char *)0x0) {
                pcVar13 = "&byteend=";
                pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
                pcVar10 = strstr(pcVar10,pcVar13);
                if (pcVar10 != (char *)0x0) goto LAB_1003815f;
              }
              unaff_EBP[-0x38] = 0;
              *(undefined1 *)((int)unaff_EBP + 0x17) = 0;
              goto LAB_10038f40;
            }
          }
        }
LAB_1003815f:
        if ((*(char *)((int)unaff_EBP + 0x17) != '\0') && (unaff_EBP[-0x89] != 10)) {
          pcVar13 = "gz";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
          iVar8 = _stricmp(pcVar10,pcVar13);
          if (iVar8 != 0) {
            pcVar13 = "z";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
            iVar8 = _stricmp(pcVar10,pcVar13);
            if (iVar8 != 0) {
              pcVar13 = "gzip";
              pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
              iVar8 = _stricmp(pcVar10,pcVar13);
              if (iVar8 != 0) {
                pcVar13 = "zip";
                pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
                iVar8 = _stricmp(pcVar10,pcVar13);
                if (iVar8 != 0) {
                  pcVar13 = "bz2";
                  pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
                  iVar8 = _stricmp(pcVar10,pcVar13);
                  if (iVar8 != 0) {
                    pcVar13 = "lzh";
                    pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
                    iVar8 = _stricmp(pcVar10,pcVar13);
                    if (iVar8 != 0) {
                      pcVar13 = "exe";
                      pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
                      iVar8 = _stricmp(pcVar10,pcVar13);
                      if (iVar8 != 0) {
                        pcVar13 = "bin";
                        pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
                        iVar8 = _stricmp(pcVar10,pcVar13);
                        if (iVar8 != 0) {
                          *(undefined1 *)((int)unaff_EBP + -0x86) = 1;
                          goto LAB_10038247;
                        }
                      }
                    }
                  }
                }
              }
            }
          }
          *(undefined1 *)((int)unaff_EBP + 0x17) = 0;
        }
LAB_10038247:
        if (unaff_EBP[-0x38] != 0) {
          *(undefined1 *)((int)unaff_EBP + 0x17) = 0;
          if (unaff_EBP[-10] != 0) {
            iVar8 = *(int *)(iVar7 + 0xe0);
            if (((*(uint *)(iVar7 + 0xdc) != 0 || iVar8 != 0) && (iVar8 < 1)) &&
               ((iVar8 < 0 || (*(uint *)(iVar7 + 0xdc) < (uint)unaff_EBP[-10])))) {
              unaff_EBP[-0x38] = 0;
              goto LAB_10038f40;
            }
          }
          unaff_EBP[-0x68] = 1;
          unaff_EBP[-0x11] = 1;
          pcVar13 = "mp3";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
          iVar8 = _stricmp(pcVar10,pcVar13);
          if (iVar8 == 0) {
LAB_1003834b:
            unaff_EBP[-0x11] = 2;
          }
          else {
            pcVar13 = "wav";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
            iVar8 = _stricmp(pcVar10,pcVar13);
            if (iVar8 == 0) goto LAB_1003834b;
            pcVar13 = "m4a";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
            iVar8 = _stricmp(pcVar10,pcVar13);
            if (iVar8 == 0) goto LAB_1003834b;
            pcVar13 = "aac";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
            iVar8 = _stricmp(pcVar10,pcVar13);
            if (iVar8 == 0) goto LAB_1003834b;
            pcVar13 = "ra";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
            iVar8 = _stricmp(pcVar10,pcVar13);
            if (iVar8 == 0) goto LAB_1003834b;
            pcVar13 = "rm";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
            iVar8 = _stricmp(pcVar10,pcVar13);
            if (iVar8 == 0) goto LAB_1003834b;
            pcVar13 = "wma";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
            iVar8 = _stricmp(pcVar10,pcVar13);
            if ((iVar8 == 0) || (unaff_EBP[-0x89] == 4)) goto LAB_1003834b;
          }
          unaff_EBP[-0xab] = 0;
          unaff_EBP[-0x10] = 0;
          unaff_EBP[-0x17] = 0;
          piVar66 = (int *)unaff_EBP[-0x1a];
          FUN_10020060((int *)unaff_EBP[4],piVar66,(int *)unaff_EBP[1],(uint *)(unaff_EBP + -0xab),
                       unaff_EBP + -0x10,unaff_EBP + -0x17);
          (*DAT_1005abe4)(unaff_EBP + -0x2f);
          *(undefined1 *)(unaff_EBP + -1) = 0x42;
          (*DAT_1005abe4)(unaff_EBP + -0x4a);
          *(undefined1 *)(unaff_EBP + -1) = 0x43;
          (*DAT_1005abe4)(unaff_EBP + -0x75);
          *(undefined1 *)(unaff_EBP + -1) = 0x44;
          iVar8 = (**(code **)(*(int *)unaff_EBP[4] + 0xc4))
                            ((int *)unaff_EBP[4],piVar66,unaff_EBP + -0x2f);
          unaff_EBP[2] = iVar8;
          FUN_10005620(unaff_EBP + -0xbe,"User-Agent",0xffffffff);
          *(undefined1 *)(unaff_EBP + -1) = 0x45;
          iVar8 = (**(code **)(*(int *)unaff_EBP[4] + 0xe8))
                            ((int *)unaff_EBP[4],piVar66,unaff_EBP + -0xbe,unaff_EBP + -0x75);
          unaff_EBP[2] = iVar8;
          FUN_10005620(unaff_EBP + -0xba,"Cookie",0xffffffff);
          *(undefined1 *)(unaff_EBP + -1) = 0x46;
          iVar8 = (**(code **)(*(int *)unaff_EBP[4] + 0xe8))
                            ((int *)unaff_EBP[4],piVar66,unaff_EBP + -0xba,unaff_EBP + -0x4a);
          unaff_EBP[2] = iVar8;
          (*DAT_1005abe4)(unaff_EBP + -0x7d);
          *(undefined1 *)(unaff_EBP + -1) = 0x47;
          (*DAT_1005abe4)(unaff_EBP + -0x33);
          *(undefined1 *)(unaff_EBP + -1) = 0x48;
          piVar6 = unaff_EBP + -0x33;
          piVar28 = unaff_EBP + -0x7d;
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          FUN_1003b030(&DAT_1005b918,pcVar10,piVar28,piVar6);
          (*DAT_1005abc0)(unaff_EBP + -0x5a);
          *(undefined1 *)(unaff_EBP + -1) = 0x49;
          (*DAT_1005abc0)(unaff_EBP + -0x81);
          *(undefined1 *)(unaff_EBP + -1) = 0x4a;
          (*DAT_1005abc0)(unaff_EBP + -0x62);
          *(undefined1 *)(unaff_EBP + -1) = 0x4b;
          (*DAT_1005abc0)(unaff_EBP + -0x5e);
          *(undefined1 *)(unaff_EBP + -1) = 0x4c;
          (*DAT_1005abc0)(unaff_EBP + -0x66);
          *(undefined1 *)(unaff_EBP + -1) = 0x4d;
          (*DAT_1005abc0)(unaff_EBP + 0x12);
          *(undefined1 *)(unaff_EBP + -1) = 0x4e;
          (*DAT_1005abc0)(unaff_EBP + 10);
          *(undefined1 *)(unaff_EBP + -1) = 0x4f;
          (*DAT_1005abc0)(unaff_EBP + 6);
          *(undefined1 *)(unaff_EBP + -1) = 0x50;
          (*DAT_1005abc0)(unaff_EBP + -0x85);
          *(undefined1 *)(unaff_EBP + -1) = 0x51;
          (*DAT_1005abc0)(unaff_EBP + -0x71);
          *(undefined1 *)(unaff_EBP + -1) = 0x52;
          (*DAT_1005ac08)(unaff_EBP + -8,0,unaff_EBP + -0x5a);
          (*DAT_1005ac08)(unaff_EBP + -0x15,0,unaff_EBP + -0x81);
          iVar8 = FUN_10003890(unaff_EBP + -0x4a);
          if (iVar8 != 0) {
            (*DAT_1005ac08)(unaff_EBP + -0x4a,0,unaff_EBP + -0x5e);
          }
          iVar8 = FUN_10003890(unaff_EBP + -0x75);
          if (iVar8 != 0) {
            (*DAT_1005ac08)(unaff_EBP + -0x75,0,unaff_EBP + -0x66);
          }
          iVar8 = FUN_10003890(unaff_EBP + -0x2f);
          if (iVar8 != 0) {
            (*DAT_1005ac08)(unaff_EBP + -0x2f,0,unaff_EBP + -0x62);
          }
          iVar8 = FUN_10003890(unaff_EBP + -0x7d);
          if (iVar8 != 0) {
            (*DAT_1005ac08)(unaff_EBP + -0x7d,0,unaff_EBP + 0x12);
          }
          iVar8 = FUN_10003890(unaff_EBP + -0x33);
          if (iVar8 != 0) {
            (*DAT_1005ac08)(unaff_EBP + -0x33,0,unaff_EBP + 10);
          }
          iVar8 = FUN_10003890(iVar7 + 0x68);
          if (iVar8 != 0) {
            (*DAT_1005ac08)(iVar7 + 0x68,0,unaff_EBP + 6);
          }
          if (unaff_EBP[-0x17] != 0) {
            FUN_10005620(unaff_EBP + -0x28,unaff_EBP[-0x17],0xffffffff);
            *(undefined1 *)(unaff_EBP + -1) = 0x53;
            (*DAT_1005ac08)(unaff_EBP + -0x28,0,unaff_EBP + -0x71);
            *(undefined1 *)(unaff_EBP + -1) = 0x52;
            (*DAT_1005abec)(unaff_EBP + -0x28);
          }
          iVar8 = FUN_10003890(iVar7 + 0x88);
          if (iVar8 != 0) {
            (*DAT_1005ac08)(iVar7 + 0x88,0,unaff_EBP + -0x85);
          }
          (*DAT_1005abc0)(unaff_EBP + 0xe);
          *(undefined1 *)(unaff_EBP + -1) = 0x54;
          iVar8 = FUN_10003870(unaff_EBP + -0x97);
          if (iVar8 == 0) {
            (*DAT_1005abe4)(unaff_EBP + -0x28);
            *(undefined1 *)(unaff_EBP + -1) = 0x55;
            iVar8 = FUN_10019e90((int *)unaff_EBP[4],piVar66,unaff_EBP[1],unaff_EBP + -0x28);
            if ((iVar8 != 0) && (iVar8 = FUN_10003890(unaff_EBP + -0x28), iVar8 != 0)) {
              (*DAT_1005ac08)(unaff_EBP + -0x28,0,unaff_EBP + 0xe);
            }
            *(undefined1 *)(unaff_EBP + -1) = 0x54;
            (*DAT_1005abec)(unaff_EBP + -0x28);
          }
          else {
            FUN_100038d0(unaff_EBP + 0xe,unaff_EBP + -0x97);
          }
          (*DAT_1005abe4)(unaff_EBP + 0x16);
          *(undefined1 *)(unaff_EBP + -1) = 0x56;
          uVar11 = FUN_10003910(unaff_EBP + -0x9f);
          FUN_10001090(unaff_EBP + 0x16,uVar11);
          (*DAT_1005abc0)(unaff_EBP + -0x79);
          *(undefined1 *)(unaff_EBP + -1) = 0x57;
          (*DAT_1005abc0)(unaff_EBP + -0x56);
          *(undefined1 *)(unaff_EBP + -1) = 0x58;
          (*DAT_1005abc0)(unaff_EBP + -0x4e);
          *(undefined1 *)(unaff_EBP + -1) = 0x59;
          (*DAT_1005abc0)(unaff_EBP + -0x52);
          *(undefined1 *)(unaff_EBP + -1) = 0x5a;
          iVar8 = FUN_10003890(unaff_EBP + 0x16);
          if (iVar8 != 0) {
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar13 = (char *)FUN_10003910(unaff_EBP + 0x16);
            iVar8 = _stricmp(pcVar13,pcVar10);
            if (iVar8 != 0) {
              (*DAT_1005ac08)(unaff_EBP + 0x16,0,unaff_EBP + -0x79);
              (*DAT_1005abe4)(unaff_EBP + -0x37);
              *(undefined1 *)(unaff_EBP + -1) = 0x5b;
              (*DAT_1005abe4)(unaff_EBP + -0x3c);
              *(undefined1 *)(unaff_EBP + -1) = 0x5c;
              piVar6 = unaff_EBP + -0x3c;
              piVar28 = unaff_EBP + -0x37;
              pcVar10 = (char *)FUN_10003910(unaff_EBP + 0x16);
              FUN_1003b030(&DAT_1005b918,pcVar10,piVar28,piVar6);
              iVar8 = FUN_10003890(unaff_EBP + -0x37);
              if (iVar8 != 0) {
                (*DAT_1005ac08)(unaff_EBP + -0x37,0,unaff_EBP + -0x4e);
              }
              iVar8 = FUN_10003890(unaff_EBP + -0x3c);
              if (iVar8 != 0) {
                (*DAT_1005ac08)(unaff_EBP + -0x3c,0,unaff_EBP + -0x52);
              }
              FUN_10018fa0(unaff_EBP + -0xc);
              *(undefined1 *)(unaff_EBP + -1) = 0x5d;
              FUN_10018fa0(unaff_EBP + -0xe);
              *(undefined1 *)(unaff_EBP + -1) = 0x5e;
              unaff_EBP[-0xa7] = (int)(unaff_EBP + -0xc);
              piVar6 = FUN_1001c6e0(unaff_EBP + -0xa7);
              iVar8 = (*DAT_1005abb4)(piVar6);
              unaff_EBP[-0xf] = iVar8;
              if ((-1 < iVar8) && (unaff_EBP[-0xc] != 0)) {
                unaff_EBP[-0xaf] = unaff_EBP[-0xc];
                unaff_EBP[-0xae] = (int)(unaff_EBP + -0xf);
                FUN_10005b20(unaff_EBP + -10,unaff_EBP + -0xaf);
                *(undefined1 *)(unaff_EBP + -1) = 0x5f;
                if (((unaff_EBP[-0xf] & 0x80000000U) == 0) && (unaff_EBP[-10] != 0)) {
                  piVar6 = (int *)unaff_EBP[-0xc];
                  unaff_EBP[-0x29] = (int)(unaff_EBP + -0xe);
                  iVar7 = *piVar6;
                  piVar28 = FUN_1001c6e0(unaff_EBP + -0x29);
                  iVar7 = (**(code **)(iVar7 + 0x10))
                                    (piVar6,"@mozilla.org/cookiemanager;1",&DAT_1004d6b8,piVar28);
                  unaff_EBP[-0xf] = iVar7;
                  if ((-1 < iVar7) && (unaff_EBP[-0xe] != 0)) {
                    (*DAT_1005abe4)(unaff_EBP + -0x28);
                    *(undefined1 *)(unaff_EBP + -1) = 0x60;
                    piVar28 = (int *)unaff_EBP[-0xe];
                    piVar6 = unaff_EBP + -0x28;
                    pcVar10 = (char *)FUN_10003910(unaff_EBP + 0x16);
                    iVar7 = FUN_100251a0(piVar28,pcVar10,piVar6);
                    if ((iVar7 != 0) && (iVar7 = FUN_10003890(unaff_EBP + -0x28), iVar7 != 0)) {
                      (*DAT_1005ac08)(unaff_EBP + -0x28,0,unaff_EBP + -0x56);
                    }
                    *(undefined1 *)(unaff_EBP + -1) = 0x5f;
                    (*DAT_1005abec)(unaff_EBP + -0x28);
                  }
                }
                *(undefined1 *)(unaff_EBP + -1) = 0x5e;
                FUN_10017f20(unaff_EBP + -10);
                iVar7 = unaff_EBP[3];
              }
              *(undefined1 *)(unaff_EBP + -1) = 0x5d;
              FUN_10017f20(unaff_EBP + -0xe);
              *(undefined1 *)(unaff_EBP + -1) = 0x5c;
              FUN_10017f20(unaff_EBP + -0xc);
              *(undefined1 *)(unaff_EBP + -1) = 0x5b;
              (*DAT_1005abec)(unaff_EBP + -0x3c);
              *(undefined1 *)(unaff_EBP + -1) = 0x5a;
              (*DAT_1005abec)(unaff_EBP + -0x37);
            }
          }
          (*DAT_1005abc0)(unaff_EBP + -0x40);
          *(undefined1 *)(unaff_EBP + -1) = 0x61;
          iVar8 = FUN_10003890(iVar7 + 0x98);
          if (iVar8 != 0) {
            (*DAT_1005ac08)(iVar7 + 0x98,0,unaff_EBP + -0x40);
          }
          pvVar12 = operator_new(0x2f8);
          unaff_EBP[-0x29] = (int)pvVar12;
          *(undefined1 *)(unaff_EBP + -1) = 0x62;
          if (pvVar12 == (void *)0x0) {
            piVar6 = (int *)0x0;
          }
          else {
            iVar7 = FUN_10003870(unaff_EBP + -0x52);
            if (iVar7 == 0) {
              unaff_EBP[-10] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + -0x52);
              unaff_EBP[-10] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + -0x4e);
            if (iVar7 == 0) {
              unaff_EBP[-0x18] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + -0x4e);
              unaff_EBP[-0x18] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + -0x56);
            if (iVar7 == 0) {
              unaff_EBP[-0x17] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + -0x56);
              unaff_EBP[-0x17] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + -0x79);
            if (iVar7 == 0) {
              unaff_EBP[-0x21] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + -0x79);
              unaff_EBP[-0x21] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + 0xe);
            if (iVar7 == 0) {
              unaff_EBP[-0x1f] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + 0xe);
              unaff_EBP[-0x1f] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + -0x40);
            if (iVar7 == 0) {
              unaff_EBP[-0x20] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + -0x40);
              unaff_EBP[-0x20] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + -0x9b);
            if (iVar7 == 0) {
              unaff_EBP[-0xf] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + -0x9b);
              unaff_EBP[-0xf] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + -0x85);
            if (iVar7 == 0) {
              unaff_EBP[-0x1c] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + -0x85);
              unaff_EBP[-0x1c] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + -0x66);
            if (iVar7 == 0) {
              unaff_EBP[-0x1d] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + -0x66);
              unaff_EBP[-0x1d] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + 6);
            if (iVar7 == 0) {
              unaff_EBP[-0x1b] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + 6);
              unaff_EBP[-0x1b] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + 10);
            if (iVar7 == 0) {
              unaff_EBP[-0x1e] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + 10);
              unaff_EBP[-0x1e] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + 0x12);
            if (iVar7 == 0) {
              unaff_EBP[-0x19] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + 0x12);
              unaff_EBP[-0x19] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + -0x71);
            if (iVar7 == 0) {
              unaff_EBP[-0xe] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + -0x71);
              unaff_EBP[-0xe] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + -0x5e);
            if (iVar7 == 0) {
              unaff_EBP[-0xc] = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + -0x5e);
              unaff_EBP[-0xc] = iVar7;
            }
            iVar7 = FUN_10003870(unaff_EBP + -0x62);
            if (iVar7 == 0) {
              iVar7 = 0;
            }
            else {
              iVar7 = FUN_100038b0(unaff_EBP + -0x62);
            }
            ppiVar23 = (int **)unaff_EBP[-0x90];
            puVar67 = (uint *)unaff_EBP[-0x91];
            uVar63 = 0;
            uVar60 = 0;
            iVar58 = 0;
            iVar55 = 0;
            uVar54 = 0;
            uVar52 = 0;
            uVar51 = unaff_EBP[-0x8d];
            iVar8 = unaff_EBP[-0x8a];
            iVar9 = unaff_EBP[-0x8b];
            iVar65 = unaff_EBP[-10];
            iVar50 = unaff_EBP[-0x18];
            iVar48 = 0;
            iVar45 = 0;
            iVar43 = unaff_EBP[-0x17];
            pwVar16 = (wchar_t *)unaff_EBP[-0x21];
            iVar26 = unaff_EBP[-0x1f];
            iVar49 = unaff_EBP[-0x20];
            uVar11 = *(undefined4 *)(unaff_EBP[3] + 0xe0);
            uVar68 = *(undefined4 *)(unaff_EBP[3] + 0xdc);
            iVar59 = unaff_EBP[-0x11];
            uVar41 = 1;
            uVar40 = 0;
            pWVar39 = (LPCWSTR)unaff_EBP[-0xf];
            iVar24 = FUN_100038b0(unaff_EBP + -0x81);
            iVar38 = unaff_EBP[-0x1c];
            iVar37 = unaff_EBP[-0x1d];
            iVar36 = unaff_EBP[-0x1b];
            iVar35 = unaff_EBP[-0x1e];
            iVar34 = unaff_EBP[-0x19];
            iVar33 = unaff_EBP[-0xe];
            iVar32 = unaff_EBP[-0xc];
            pwVar70 = (wchar_t *)FUN_100038b0(unaff_EBP + -0x5a);
            piVar6 = FUN_1000e0e0((void *)unaff_EBP[-0x29],puVar67,ppiVar23,unaff_EBP[-0x87],
                                  (wchar_t *)unaff_EBP[-0x8e],unaff_EBP[-0x93],pwVar70,iVar7,iVar32,
                                  iVar33,iVar34,iVar35,iVar36,iVar37,iVar38,iVar24,pWVar39,uVar40,
                                  uVar41,iVar59,uVar68,uVar11,iVar49,iVar26,pwVar16,iVar43,iVar45,
                                  iVar48,iVar50,iVar65,iVar9,iVar8,uVar51,uVar52,uVar54,iVar55,
                                  iVar58,uVar60,uVar63);
            iVar7 = unaff_EBP[3];
          }
          *(undefined1 *)(unaff_EBP + -1) = 0x61;
          if (piVar6 != (int *)0x0) {
            if (piVar6[2] == 0) {
              iVar8 = piVar6[3];
            }
            else {
              iVar8 = piVar6[2];
            }
            *(int *)(iVar7 + 0xec) = iVar8;
            if (piVar6[2] == 0) {
              (**(code **)(*piVar6 + 0x18))(1);
            }
          }
          if ((void *)unaff_EBP[-0x10] != (void *)0x0) {
            operator_delete((void *)unaff_EBP[-0x10]);
          }
          *(undefined1 *)(unaff_EBP + -1) = 0x5a;
          (*DAT_1005abc8)(unaff_EBP + -0x40);
          *(undefined1 *)(unaff_EBP + -1) = 0x59;
          (*DAT_1005abc8)(unaff_EBP + -0x52);
          *(undefined1 *)(unaff_EBP + -1) = 0x58;
          (*DAT_1005abc8)(unaff_EBP + -0x4e);
          *(undefined1 *)(unaff_EBP + -1) = 0x57;
          (*DAT_1005abc8)(unaff_EBP + -0x56);
          *(undefined1 *)(unaff_EBP + -1) = 0x56;
          (*DAT_1005abc8)(unaff_EBP + -0x79);
          *(undefined1 *)(unaff_EBP + -1) = 0x54;
          (*DAT_1005abec)(unaff_EBP + 0x16);
          *(undefined1 *)(unaff_EBP + -1) = 0x52;
          (*DAT_1005abc8)(unaff_EBP + 0xe);
          *(undefined1 *)(unaff_EBP + -1) = 0x51;
          (*DAT_1005abc8)(unaff_EBP + -0x71);
          *(undefined1 *)(unaff_EBP + -1) = 0x50;
          (*DAT_1005abc8)(unaff_EBP + -0x85);
          *(undefined1 *)(unaff_EBP + -1) = 0x4f;
          (*DAT_1005abc8)(unaff_EBP + 6);
          *(undefined1 *)(unaff_EBP + -1) = 0x4e;
          (*DAT_1005abc8)(unaff_EBP + 10);
          *(undefined1 *)(unaff_EBP + -1) = 0x4d;
          (*DAT_1005abc8)(unaff_EBP + 0x12);
          *(undefined1 *)(unaff_EBP + -1) = 0x4c;
          (*DAT_1005abc8)(unaff_EBP + -0x66);
          *(undefined1 *)(unaff_EBP + -1) = 0x4b;
          (*DAT_1005abc8)(unaff_EBP + -0x5e);
          *(undefined1 *)(unaff_EBP + -1) = 0x4a;
          (*DAT_1005abc8)(unaff_EBP + -0x62);
          *(undefined1 *)(unaff_EBP + -1) = 0x49;
          (*DAT_1005abc8)(unaff_EBP + -0x81);
          *(undefined1 *)(unaff_EBP + -1) = 0x48;
          (*DAT_1005abc8)(unaff_EBP + -0x5a);
          *(undefined1 *)(unaff_EBP + -1) = 0x47;
          (*DAT_1005abec)(unaff_EBP + -0x33);
          *(undefined1 *)(unaff_EBP + -1) = 0x46;
          (*DAT_1005abec)(unaff_EBP + -0x7d);
          *(undefined1 *)(unaff_EBP + -1) = 0x45;
          (*DAT_1005abec)(unaff_EBP + -0xba);
          *(undefined1 *)(unaff_EBP + -1) = 0x44;
          (*DAT_1005abec)(unaff_EBP + -0xbe);
          *(undefined1 *)(unaff_EBP + -1) = 0x43;
          (*DAT_1005abec)(unaff_EBP + -0x75);
          *(undefined1 *)(unaff_EBP + -1) = 0x42;
          (*DAT_1005abec)(unaff_EBP + -0x4a);
          *(undefined1 *)(unaff_EBP + -1) = 0x41;
          (*DAT_1005abec)(unaff_EBP + -0x2f);
        }
      }
LAB_10038f40:
      unaff_EBP[-1] = 0x15;
      iVar7 = unaff_EBP[3];
      if (*(char *)((int)unaff_EBP + 0x17) == '\0') {
LAB_100395bf:
        unaff_EBP[-0x1b] = 0;
        unaff_EBP[-0xe] = 0;
        unaff_EBP[-0x1f] = 0;
        if ((unaff_EBP[-0xd] != 0) && (iVar8 = FUN_10003890(unaff_EBP + -0x15), iVar8 != 0)) {
          pcVar13 = "asx";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
          iVar8 = _stricmp(pcVar10,pcVar13);
          if (iVar8 == 0) {
LAB_10039628:
            unaff_EBP[-0x1b] = 1;
            pcVar13 = "asf";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
            iVar8 = _stricmp(pcVar10,pcVar13);
            if (((iVar8 == 0) && (-1 < *(int *)(iVar7 + 0xe0))) &&
               ((0 < *(int *)(iVar7 + 0xe0) || (0x5000 < *(uint *)(iVar7 + 0xdc))))) {
              unaff_EBP[-0x1b] = 0;
            }
          }
          else {
            pcVar13 = "asf";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
            iVar8 = _stricmp(pcVar10,pcVar13);
            if (iVar8 == 0) goto LAB_10039628;
            pcVar13 = "wvx";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
            iVar8 = _stricmp(pcVar10,pcVar13);
            if (iVar8 == 0) goto LAB_10039628;
          }
          pcVar13 = "ram";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
          iVar8 = _stricmp(pcVar10,pcVar13);
          if (((iVar8 == 0) && (*(int *)(iVar7 + 0xe0) < 1)) &&
             ((*(int *)(iVar7 + 0xe0) < 0 || (*(uint *)(iVar7 + 0xdc) < 0x800)))) {
            unaff_EBP[-0x1f] = 1;
          }
          pcVar13 = "f4m";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
          iVar8 = _stricmp(pcVar10,pcVar13);
          if (((iVar8 == 0) && (*(int *)(iVar7 + 0xe0) < 1)) &&
             ((*(int *)(iVar7 + 0xe0) < 0 || (*(uint *)(iVar7 + 0xdc) < 0x186a1)))) {
            unaff_EBP[-0x41] = 1;
          }
          pcVar13 = "mpd";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
          iVar8 = _stricmp(pcVar10,pcVar13);
          if (((iVar8 == 0) && (*(int *)(iVar7 + 0xe0) < 1)) &&
             ((*(int *)(iVar7 + 0xe0) < 0 || (*(uint *)(iVar7 + 0xdc) < 0x2dc6c1)))) {
            unaff_EBP[-0xe] = 1;
          }
        }
        unaff_EBP[-0xc] = 0;
        pcVar13 = "youtube.com/get_video_info?";
        pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
        pcVar10 = strstr(pcVar10,pcVar13);
        if (pcVar10 == (char *)0x0) {
          pcVar13 = "youtube.com/api_video_info?";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          pcVar10 = strstr(pcVar10,pcVar13);
          if (pcVar10 != (char *)0x0) goto LAB_10039767;
          pcVar13 = "googlevideo.com/get_video_info?";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          pcVar10 = strstr(pcVar10,pcVar13);
          if (pcVar10 != (char *)0x0) goto LAB_10039767;
          pcVar13 = "googlevideo.com/api_video_info?";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          pcVar10 = strstr(pcVar10,pcVar13);
          if (pcVar10 != (char *)0x0) goto LAB_10039767;
        }
        else {
LAB_10039767:
          unaff_EBP[-0xc] = 1;
          pcVar13 = "youtube.com/get_video_info?";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          pcVar10 = strstr(pcVar10,pcVar13);
          if (pcVar10 == (char *)0x0) {
            pcVar13 = "googlevideo.com/get_video_info?";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar10 = strstr(pcVar10,pcVar13);
            if (pcVar10 == (char *)0x0) goto LAB_100397be;
          }
          pcVar14 = "adunit";
          pcVar13 = "el";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          iVar8 = FUN_10018bd0(pcVar10,pcVar13,pcVar14);
          if (iVar8 != 0) {
            unaff_EBP[-0xc] = 0;
          }
        }
LAB_100397be:
        unaff_EBP[-0x1d] = 0;
        pcVar13 = "youtube.com/watch?";
        pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
        pcVar10 = strstr(pcVar10,pcVar13);
        if (pcVar10 == (char *)0x0) {
          pcVar13 = "googlevideo.com/watch?";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          pcVar10 = strstr(pcVar10,pcVar13);
          if (pcVar10 != (char *)0x0) goto LAB_100397f3;
        }
        else {
LAB_100397f3:
          pcVar13 = "?v=";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          pcVar10 = strstr(pcVar10,pcVar13);
          if (pcVar10 == (char *)0x0) {
            pcVar13 = "&spf=";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar10 = strstr(pcVar10,pcVar13);
            if (pcVar10 == (char *)0x0) {
              pcVar13 = "&v=";
              pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
              pcVar10 = strstr(pcVar10,pcVar13);
              if (pcVar10 == (char *)0x0) goto LAB_1003986c;
            }
          }
          iVar8 = FUN_10003890(iVar7 + 0x88);
          if (iVar8 != 0) {
            sVar69 = 0x10;
            pcVar13 = "application/json";
            pcVar10 = (char *)FUN_10003910(iVar7 + 0x88);
            iVar8 = _strnicmp(pcVar10,pcVar13,sVar69);
            if (iVar8 == 0) {
              unaff_EBP[-0x1d] = 1;
            }
          }
        }
LAB_1003986c:
        unaff_EBP[-0x21] = 0;
        piVar6 = (int *)(iVar7 + 0x88);
        unaff_EBP[-0x29] = (int)piVar6;
        iVar8 = FUN_10003890(piVar6);
        if (iVar8 != 0) {
          sVar69 = 0x23;
          pcVar13 = "application/vnd.lumberjack.manifest";
          pcVar10 = (char *)FUN_10003910(piVar6);
          iVar8 = _strnicmp(pcVar10,pcVar13,sVar69);
          if (((iVar8 == 0) && (*(int *)(iVar7 + 0xe0) < 1)) &&
             ((*(int *)(iVar7 + 0xe0) < 0 || (*(uint *)(iVar7 + 0xdc) < 0x186a1)))) {
            unaff_EBP[-0x21] = 1;
          }
          sVar69 = 0x23;
          pcVar13 = "application/dash+xml";
          pcVar10 = (char *)FUN_10003910(piVar6);
          iVar8 = _strnicmp(pcVar10,pcVar13,sVar69);
          if (((iVar8 == 0) && (*(int *)(iVar7 + 0xe0) < 1)) &&
             ((*(int *)(iVar7 + 0xe0) < 0 || (*(uint *)(iVar7 + 0xdc) < 0x2dc6c1)))) {
            unaff_EBP[-0xe] = 1;
          }
        }
        bVar31 = false;
        unaff_EBP[-0x17] = 0;
        unaff_EBP[-0x18] = 0;
        if (((unaff_EBP[-0xd] != 0) && (*(int *)(iVar7 + 0xe0) < 1)) &&
           ((*(int *)(iVar7 + 0xe0) < 0 || (*(uint *)(iVar7 + 0xdc) < 2000000)))) {
          iVar8 = FUN_10003890(unaff_EBP + -0x15);
          if (iVar8 == 0) {
LAB_10039975:
            iVar8 = FUN_10003890(piVar6);
            if (iVar8 != 0) {
              sVar69 = 0xd;
              pcVar13 = "audio/mpegurl";
              pcVar10 = (char *)FUN_10003910(piVar6);
              iVar8 = _strnicmp(pcVar10,pcVar13,sVar69);
              if (iVar8 != 0) {
                sVar69 = 0x15;
                pcVar13 = "application/x-mpegURL";
                pcVar10 = (char *)FUN_10003910(piVar6);
                iVar8 = _strnicmp(pcVar10,pcVar13,sVar69);
                if (iVar8 != 0) {
                  sVar69 = 0x1d;
                  pcVar13 = "application/vnd.apple.mpegurl";
                  pcVar10 = (char *)FUN_10003910(piVar6);
                  iVar8 = _strnicmp(pcVar10,pcVar13,sVar69);
                  if (iVar8 != 0) goto LAB_100399dc;
                }
              }
              goto LAB_100399d4;
            }
          }
          else {
            pcVar13 = "m3u";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
            iVar8 = _stricmp(pcVar10,pcVar13);
            if (iVar8 != 0) {
              pcVar13 = "m3u8";
              pcVar10 = (char *)FUN_10003910(unaff_EBP + -0x15);
              iVar8 = _stricmp(pcVar10,pcVar13);
              if (iVar8 != 0) goto LAB_10039975;
            }
LAB_100399d4:
            bVar31 = true;
            unaff_EBP[-0x17] = 1;
          }
LAB_100399dc:
          pcVar13 = "player.vimeo.com/video";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          pcVar10 = strstr(pcVar10,pcVar13);
          if (pcVar10 == (char *)0x0) {
            pcVar13 = ".m3u8";
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar10 = strstr(pcVar10,pcVar13);
            if ((pcVar10 != (char *)0x0) && (iVar8 = FUN_10003890(piVar6), iVar8 != 0)) {
              sVar69 = 10;
              pcVar13 = "text/plain";
              pcVar10 = (char *)FUN_10003910(piVar6);
              iVar8 = _strnicmp(pcVar10,pcVar13,sVar69);
              if (iVar8 == 0) {
                bVar31 = true;
                unaff_EBP[-0x17] = 1;
              }
            }
          }
          else {
            unaff_EBP[-0x18] = 1;
          }
        }
        unaff_EBP[-0x1c] = 0;
        if (unaff_EBP[-0xd] != 0) {
          pcVar13 = "facebook.com/ajax/";
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          pcVar10 = strstr(pcVar10,pcVar13);
          if (pcVar10 != (char *)0x0) {
            unaff_EBP[-0x1c] = 1;
          }
        }
        pcVar13 = "manifest.googlevideo.com/api/manifest/";
        pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
        pcVar10 = strstr(pcVar10,pcVar13);
        if (pcVar10 == (char *)0x0) {
          if (((unaff_EBP[-0xd] != 0) || (unaff_EBP[-0xc] != 0)) || (unaff_EBP[-0x1d] != 0))
          goto LAB_10039ab3;
        }
        else {
          unaff_EBP[-0xd] = 1;
LAB_10039ab3:
          unaff_EBP[-0x10] = 0;
          if (((((unaff_EBP[-0x68] != 0) || (unaff_EBP[-0x1f] != 0)) ||
               ((unaff_EBP[-0x1b] != 0 || ((unaff_EBP[-0xc] != 0 || (unaff_EBP[-0x1d] != 0)))))) ||
              (unaff_EBP[-0x21] != 0)) ||
             (((unaff_EBP[-0x41] != 0 || (bVar31)) || (unaff_EBP[-0x18] != 0)))) goto LAB_10039d3a;
          if ((unaff_EBP[-0xe] != 0) || (unaff_EBP[-0x1c] != 0)) {
LAB_10039d21:
            if (((unaff_EBP[-0x2b] != 0) || (unaff_EBP[-0xe] != 0)) || (unaff_EBP[-0x1c] != 0))
            goto LAB_10039d3a;
            goto LAB_1003a65f;
          }
          iVar8 = FUN_10003890(piVar6);
          if (iVar8 == 0) {
LAB_10039b37:
            unaff_EBP[-0x10] = 1;
          }
          else {
            sVar69 = 0x18;
            pcVar10 = "application/octet-stream";
            pvVar12 = (void *)FUN_10003910(piVar6);
            iVar8 = _memicmp(pvVar12,pcVar10,sVar69);
            if (iVar8 == 0) goto LAB_10039b37;
            sVar69 = 10;
            pcVar10 = "text/plain";
            pvVar12 = (void *)FUN_10003910(piVar6);
            iVar8 = _memicmp(pvVar12,pcVar10,sVar69);
            if (iVar8 == 0) goto LAB_10039b37;
            sVar69 = 0x13;
            pcVar10 = "binary/octet-stream";
            pvVar12 = (void *)FUN_10003910(piVar6);
            iVar8 = _memicmp(pvVar12,pcVar10,sVar69);
            if (iVar8 == 0) goto LAB_10039b37;
            sVar69 = 0x12;
            pcVar10 = "application/x-sdlc";
            pvVar12 = (void *)FUN_10003910(piVar6);
            iVar8 = _memicmp(pvVar12,pcVar10,sVar69);
            if (iVar8 == 0) goto LAB_10039b37;
            sVar69 = 9;
            pcVar10 = "text/html";
            pvVar12 = (void *)FUN_10003910(piVar6);
            iVar8 = _memicmp(pvVar12,pcVar10,sVar69);
            if (iVar8 == 0) goto LAB_10039b37;
            pcVar13 = "flv-";
            pcVar10 = (char *)FUN_10003910(piVar6);
            pcVar10 = strstr(pcVar10,pcVar13);
            if (pcVar10 != (char *)0x0) goto LAB_10039b37;
            pcVar13 = "-flv";
            pcVar10 = (char *)FUN_10003910(piVar6);
            pcVar10 = strstr(pcVar10,pcVar13);
            if (pcVar10 != (char *)0x0) goto LAB_10039b37;
            sVar69 = 6;
            pcVar10 = "video/";
            pvVar12 = (void *)FUN_10003910(piVar6);
            iVar8 = _memicmp(pvVar12,pcVar10,sVar69);
            pcVar29 = strstr_exref;
            if (iVar8 == 0) {
              pcVar13 = "flash";
              pcVar10 = (char *)FUN_10003910(piVar6);
              pcVar29 = strstr_exref;
              pcVar10 = strstr(pcVar10,pcVar13);
              if (pcVar10 != (char *)0x0) goto LAB_10039b37;
            }
            iVar8 = *(int *)(iVar7 + 0xe0);
            if (((0 < iVar8) || ((-1 < iVar8 && (0x79d38 < *(uint *)(iVar7 + 0xdc))))) ||
               (*(uint *)(iVar7 + 0xdc) == 0 && iVar8 == 0)) goto LAB_10039b37;
            iVar8 = FUN_10003890(unaff_EBP + -0x15);
            if (iVar8 == 0) {
              unaff_EBP[-0x10] = 1;
            }
            else {
              iVar8 = FUN_10003890(piVar6);
              if (iVar8 == 0) {
                sVar69 = 0x17;
                pcVar13 = "video/vnd.mpeg.dash.mpd";
                pcVar10 = (char *)FUN_10003910(piVar6);
                iVar7 = _strnicmp(pcVar10,pcVar13,sVar69);
                if (iVar7 != 0) {
                  pcVar10 = "manifest.googlevideo.com/api/manifest/";
                  piVar6 = unaff_EBP + -8;
LAB_10039ce5:
                  uVar11 = FUN_10003910(piVar6);
                  iVar7 = (*pcVar29)(uVar11,pcVar10);
                  if (iVar7 == 0) goto LAB_10039d21;
                }
              }
              else {
                sVar69 = 8;
                pcVar13 = "text/xml";
                pcVar10 = (char *)FUN_10003910(piVar6);
                iVar8 = _strnicmp(pcVar10,pcVar13,sVar69);
                if (((iVar8 == 0) && (*(int *)(iVar7 + 0xe0) < 1)) &&
                   ((*(int *)(iVar7 + 0xe0) < 0 || (*(uint *)(iVar7 + 0xdc) < 0x186a1)))) {
                  unaff_EBP[-0x10] = 1;
                  goto LAB_10039d3a;
                }
                puVar71 = &DAT_1004fccc;
                uVar11 = FUN_10003910(piVar6);
                iVar7 = (*pcVar29)(uVar11,puVar71);
                if (iVar7 == 0) {
                  pcVar10 = "+xml";
                  goto LAB_10039ce5;
                }
              }
              unaff_EBP[-0x10] = 1;
            }
          }
LAB_10039d3a:
          FUN_10018fa0(unaff_EBP + -0xf);
          *(undefined1 *)(unaff_EBP + -1) = 0x6b;
          FUN_10018fa0(unaff_EBP + -0x11);
          *(undefined1 *)(unaff_EBP + -1) = 0x6c;
          unaff_EBP[-0xa8] = (int)(unaff_EBP + -0x11);
          piVar6 = FUN_1001c6e0(unaff_EBP + -0xa8);
          iVar7 = (*DAT_1005abb4)(piVar6);
          unaff_EBP[2] = iVar7;
          if (-1 < iVar7) {
            if (unaff_EBP[-0x11] != 0) {
              unaff_EBP[-0x5c] = unaff_EBP[-0x11];
              unaff_EBP[-0x5b] = (int)(unaff_EBP + 2);
              FUN_10005b20(unaff_EBP + -10,unaff_EBP + -0x5c);
              *(undefined1 *)(unaff_EBP + -1) = 0x6d;
              if (((unaff_EBP[2] & 0x80000000U) == 0) && (unaff_EBP[-10] != 0)) {
                piVar6 = (int *)unaff_EBP[-0x11];
                unaff_EBP[-0xac] = (int)(unaff_EBP + -0xf);
                iVar7 = *piVar6;
                piVar28 = FUN_1001c6e0(unaff_EBP + -0xac);
                iVar7 = (**(code **)(iVar7 + 0x10))
                                  (piVar6,"@idm/mozilla_cc_helper9;1",&DAT_1004e160,piVar28);
                unaff_EBP[2] = iVar7;
              }
              *(undefined1 *)(unaff_EBP + -1) = 0x6c;
              FUN_10017f20(unaff_EBP + -10);
              iVar7 = unaff_EBP[2];
            }
            if ((-1 < iVar7) && (unaff_EBP[-0xf] != 0)) {
              puVar64 = operator_new(0x2a0);
              unaff_EBP[-0x30] = (int)puVar64;
              *(undefined1 *)(unaff_EBP + -1) = 0x6e;
              if (puVar64 == (undefined4 *)0x0) {
                puVar64 = (undefined4 *)0x0;
              }
              else {
                puVar64 = FUN_1001abb0(puVar64);
              }
              *(undefined1 *)(unaff_EBP + -1) = 0x6c;
              if (puVar64 != (undefined4 *)0x0) {
                unaff_EBP[-0xad] = 0;
                unaff_EBP[-0xa9] = 0;
                unaff_EBP[-0x1e] = 0;
                unaff_EBP[-0x19] = unaff_EBP[3] + -4;
                piVar66 = (int *)unaff_EBP[-0x1a];
                FUN_10020060((int *)unaff_EBP[4],piVar66,(int *)unaff_EBP[1],
                             (uint *)(unaff_EBP + -0xad),unaff_EBP + -0xa9,unaff_EBP + -0x1e);
                (*DAT_1005abe4)(unaff_EBP + -0x40);
                *(undefined1 *)(unaff_EBP + -1) = 0x6f;
                (*DAT_1005abe4)(unaff_EBP + -0x37);
                *(undefined1 *)(unaff_EBP + -1) = 0x70;
                (*DAT_1005abe4)(unaff_EBP + -0x3c);
                *(undefined1 *)(unaff_EBP + -1) = 0x71;
                iVar7 = (**(code **)(*(int *)unaff_EBP[4] + 0xc4))
                                  ((int *)unaff_EBP[4],piVar66,unaff_EBP + -0x40);
                unaff_EBP[2] = iVar7;
                FUN_10005620(unaff_EBP + -0x5a,"User-Agent",0xffffffff);
                *(undefined1 *)(unaff_EBP + -1) = 0x72;
                iVar7 = (**(code **)(*(int *)unaff_EBP[4] + 0xe8))
                                  ((int *)unaff_EBP[4],piVar66,unaff_EBP + -0x5a,unaff_EBP + -0x3c);
                unaff_EBP[2] = iVar7;
                FUN_10005620(unaff_EBP + -0xba,"Cookie",0xffffffff);
                *(undefined1 *)(unaff_EBP + -1) = 0x73;
                iVar7 = (**(code **)(*(int *)unaff_EBP[4] + 0xe8))
                                  ((int *)unaff_EBP[4],piVar66,unaff_EBP + -0xba,unaff_EBP + -0x37);
                unaff_EBP[2] = iVar7;
                (*DAT_1005abe4)(unaff_EBP + -0x28);
                *(undefined1 *)(unaff_EBP + -1) = 0x74;
                (*DAT_1005abe4)(unaff_EBP + -0x7d);
                *(undefined1 *)(unaff_EBP + -1) = 0x75;
                piVar6 = unaff_EBP + -0x7d;
                piVar28 = unaff_EBP + -0x28;
                pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
                FUN_1003b030(&DAT_1005b918,pcVar10,piVar28,piVar6);
                (*DAT_1005ac08)(unaff_EBP + -8,0,puVar64 + 2);
                uVar11 = FUN_10003910(unaff_EBP + -8);
                FUN_10001090(puVar64 + 6,uVar11);
                iVar7 = FUN_10003890(unaff_EBP + -0x37);
                if (iVar7 != 0) {
                  (*DAT_1005ac08)(unaff_EBP + -0x37,0,puVar64 + 0xe);
                }
                iVar7 = FUN_10003890(unaff_EBP + -0x3c);
                if (iVar7 != 0) {
                  (*DAT_1005ac08)(unaff_EBP + -0x3c,0,puVar64 + 0x12);
                }
                iVar7 = FUN_10003890(unaff_EBP + -0x40);
                if (iVar7 != 0) {
                  (*DAT_1005ac08)(unaff_EBP + -0x40,0,puVar64 + 10);
                }
                iVar7 = FUN_10003890(unaff_EBP + -0x28);
                if (iVar7 != 0) {
                  (*DAT_1005ac08)(unaff_EBP + -0x28,0,puVar64 + 0x16);
                }
                iVar7 = FUN_10003890(unaff_EBP + -0x7d);
                if (iVar7 != 0) {
                  (*DAT_1005ac08)(unaff_EBP + -0x7d,0,puVar64 + 0x1a);
                }
                iVar7 = unaff_EBP[3];
                iVar8 = FUN_10003890(iVar7 + 0x68);
                if (iVar8 != 0) {
                  (*DAT_1005ac08)(iVar7 + 0x68,0,puVar64 + 0x1e);
                }
                if (unaff_EBP[-0x1e] != 0) {
                  FUN_10005620(unaff_EBP + -0xbe,unaff_EBP[-0x1e],0xffffffff);
                  *(undefined1 *)(unaff_EBP + -1) = 0x76;
                  (*DAT_1005ac08)(unaff_EBP + -0xbe,0,puVar64 + 0x26);
                  *(undefined1 *)(unaff_EBP + -1) = 0x75;
                  (*DAT_1005abec)(unaff_EBP + -0xbe);
                }
                iVar7 = unaff_EBP[-0x29];
                iVar8 = FUN_10003890(iVar7);
                if (iVar8 != 0) {
                  (*DAT_1005ac08)(iVar7,0,puVar64 + 0x22);
                }
                iVar7 = unaff_EBP[-0xd];
                if ((iVar7 == 0) || (iVar8 = FUN_10003870(unaff_EBP + -0x97), iVar8 == 0)) {
                  (*DAT_1005abe4)(unaff_EBP + -0x2f);
                  *(undefined1 *)(unaff_EBP + -1) = 0x77;
                  iVar8 = FUN_10019e90((int *)unaff_EBP[4],piVar66,unaff_EBP[1],unaff_EBP + -0x2f);
                  if ((iVar8 != 0) && (iVar8 = FUN_10003890(unaff_EBP + -0x2f), iVar8 != 0)) {
                    (*DAT_1005ac08)(unaff_EBP + -0x2f,0,puVar64 + 0x2e);
                  }
                  *(undefined1 *)(unaff_EBP + -1) = 0x75;
                  (*DAT_1005abec)(unaff_EBP + -0x2f);
                }
                else {
                  FUN_100038d0(puVar64 + 0x2e,unaff_EBP + -0x97);
                }
                (*DAT_1005abe4)(unaff_EBP + 0x16);
                *(undefined1 *)(unaff_EBP + -1) = 0x78;
                if (iVar7 == 0) {
                  FUN_10017d50((int *)unaff_EBP[4],(int)piVar66,unaff_EBP[1],unaff_EBP + 0x16);
                }
                else {
                  uVar11 = FUN_10003910(unaff_EBP + -0x9f);
                  FUN_10001090(unaff_EBP + 0x16,uVar11);
                }
                iVar7 = FUN_10003890(unaff_EBP + 0x16);
                if (iVar7 == 0) {
LAB_1003a418:
                  iVar7 = unaff_EBP[-0x2b];
                }
                else {
                  pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
                  pcVar13 = (char *)FUN_10003910(unaff_EBP + 0x16);
                  iVar7 = _stricmp(pcVar13,pcVar10);
                  if (iVar7 == 0) goto LAB_1003a418;
                  (*DAT_1005ac08)(unaff_EBP + 0x16,0,puVar64 + 0x32);
                  (*DAT_1005abe4)(unaff_EBP + -0x75);
                  *(undefined1 *)(unaff_EBP + -1) = 0x79;
                  (*DAT_1005abe4)(unaff_EBP + -0x4a);
                  *(undefined1 *)(unaff_EBP + -1) = 0x7a;
                  piVar6 = unaff_EBP + -0x4a;
                  piVar28 = unaff_EBP + -0x75;
                  pcVar10 = (char *)FUN_10003910(unaff_EBP + 0x16);
                  FUN_1003b030(&DAT_1005b918,pcVar10,piVar28,piVar6);
                  iVar7 = FUN_10003890(unaff_EBP + -0x75);
                  if (iVar7 != 0) {
                    (*DAT_1005ac08)(unaff_EBP + -0x75,0,puVar64 + 0x42);
                  }
                  iVar7 = FUN_10003890(unaff_EBP + -0x4a);
                  if (iVar7 != 0) {
                    (*DAT_1005ac08)(unaff_EBP + -0x4a,0,puVar64 + 0x46);
                  }
                  FUN_10018fa0(unaff_EBP + -0x1e);
                  *(undefined1 *)(unaff_EBP + -1) = 0x7b;
                  FUN_10018fa0(unaff_EBP + -0x19);
                  *(undefined1 *)(unaff_EBP + -1) = 0x7c;
                  unaff_EBP[-0xb2] = (int)(unaff_EBP + -0x1e);
                  piVar6 = FUN_1001c6e0(unaff_EBP + -0xb2);
                  iVar7 = (*DAT_1005abb4)(piVar6);
                  unaff_EBP[-0x20] = iVar7;
                  if ((-1 < iVar7) && (unaff_EBP[-0x1e] != 0)) {
                    unaff_EBP[-100] = unaff_EBP[-0x1e];
                    unaff_EBP[-99] = (int)(unaff_EBP + -0x20);
                    FUN_10005b20(unaff_EBP + -10,unaff_EBP + -100);
                    *(undefined1 *)(unaff_EBP + -1) = 0x7d;
                    if (((unaff_EBP[-0x20] & 0x80000000U) == 0) && (unaff_EBP[-10] != 0)) {
                      piVar6 = (int *)unaff_EBP[-0x1e];
                      unaff_EBP[-0x30] = (int)(unaff_EBP + -0x19);
                      iVar7 = *piVar6;
                      piVar28 = FUN_1001c6e0(unaff_EBP + -0x30);
                      iVar7 = (**(code **)(iVar7 + 0x10))
                                        (piVar6,"@mozilla.org/cookiemanager;1",&DAT_1004d6b8,piVar28
                                        );
                      unaff_EBP[-0x20] = iVar7;
                      if ((-1 < iVar7) && (unaff_EBP[-0x19] != 0)) {
                        (*DAT_1005abe4)(unaff_EBP + -0x2f);
                        *(undefined1 *)(unaff_EBP + -1) = 0x7e;
                        piVar28 = (int *)unaff_EBP[-0x19];
                        piVar6 = unaff_EBP + -0x2f;
                        pcVar10 = (char *)FUN_10003910(unaff_EBP + 0x16);
                        iVar7 = FUN_100251a0(piVar28,pcVar10,piVar6);
                        if ((iVar7 != 0) && (iVar7 = FUN_10003890(unaff_EBP + -0x2f), iVar7 != 0)) {
                          (*DAT_1005ac08)(unaff_EBP + -0x2f,0,puVar64 + 0x36);
                        }
                        *(undefined1 *)(unaff_EBP + -1) = 0x7d;
                        (*DAT_1005abec)(unaff_EBP + -0x2f);
                      }
                    }
                    *(undefined1 *)(unaff_EBP + -1) = 0x7c;
                    FUN_10017f20(unaff_EBP + -10);
                  }
                  iVar7 = unaff_EBP[-0x2b];
                  *(undefined1 *)(unaff_EBP + -1) = 0x7b;
                  FUN_10017f20(unaff_EBP + -0x19);
                  *(undefined1 *)(unaff_EBP + -1) = 0x7a;
                  FUN_10017f20(unaff_EBP + -0x1e);
                  *(undefined1 *)(unaff_EBP + -1) = 0x79;
                  (*DAT_1005abec)(unaff_EBP + -0x4a);
                  *(undefined1 *)(unaff_EBP + -1) = 0x78;
                  (*DAT_1005abec)(unaff_EBP + -0x75);
                }
                iVar8 = unaff_EBP[3];
                iVar9 = FUN_10003890(iVar8 + 0x98);
                if (iVar9 != 0) {
                  (*DAT_1005ac08)(iVar8 + 0x98,0,puVar64 + 0x4a);
                }
                puVar64[0x50] = unaff_EBP[-0x93];
                puVar64[0x69] = unaff_EBP[-0x87];
                puVar64[0x4f] = unaff_EBP[-0x8e];
                uVar11 = FUN_100038b0(unaff_EBP + -0x9b);
                FUN_100038f0(puVar64 + 0x2a,uVar11);
                iVar8 = unaff_EBP[3];
                puVar64[0x52] = *(undefined4 *)(iVar8 + 0xdc);
                puVar64[0x53] = *(undefined4 *)(iVar8 + 0xe0);
                if (unaff_EBP[-0x91] == 0) {
                  FUN_1003fde0(puVar64 + 0x54,(int *)0x0);
                }
                else {
                  FUN_1001c6c0(puVar64 + 0x54,unaff_EBP[-0x91]);
                }
                if (unaff_EBP[-0x90] == 0) {
                  FUN_1003fde0(puVar64 + 0x55,(int *)0x0);
                }
                else {
                  FUN_10003b40(puVar64 + 0x55,unaff_EBP[-0x90]);
                }
                puVar64[0x56] = unaff_EBP[-0x1b];
                puVar64[0x57] = unaff_EBP[-0x1f];
                puVar64[0x59] = unaff_EBP[-0xc];
                puVar64[0x5a] = unaff_EBP[-0x1d];
                puVar64[0x5b] = unaff_EBP[-0x21];
                puVar64[0x5c] = unaff_EBP[-0x17];
                puVar64[0x5d] = unaff_EBP[-0x18];
                puVar64[0x5e] = unaff_EBP[-0x41];
                puVar64[0x5f] = unaff_EBP[-0xe];
                puVar64[100] = unaff_EBP[-0x1c];
                if (iVar7 != 0) {
                  *(int *)(iVar7 + 0x50) = unaff_EBP[-0x8d];
                  *(int *)(iVar7 + 0x58) = unaff_EBP[-0x8b];
                  *(int *)(iVar7 + 0x5c) = unaff_EBP[-0x8a];
                }
                puVar64[0x6c] = iVar7;
                puVar64[0x4e] = *(undefined4 *)(iVar8 + 0xec);
                puVar64[0x58] = unaff_EBP[-0x10];
                puVar64[0x68] = unaff_EBP[-0x8d];
                puVar64[0x66] = unaff_EBP[-0x8b];
                puVar64[0x67] = unaff_EBP[-0x8a];
                iVar7 = (**(code **)(*(int *)unaff_EBP[-0xf] + 0xc))
                                  ((int *)unaff_EBP[-0xf],puVar64,unaff_EBP[-0x1a]);
                unaff_EBP[2] = iVar7;
                *(undefined1 *)(unaff_EBP + -1) = 0x75;
                (*DAT_1005abec)(unaff_EBP + 0x16);
                *(undefined1 *)(unaff_EBP + -1) = 0x74;
                (*DAT_1005abec)(unaff_EBP + -0x7d);
                *(undefined1 *)(unaff_EBP + -1) = 0x73;
                (*DAT_1005abec)(unaff_EBP + -0x28);
                *(undefined1 *)(unaff_EBP + -1) = 0x72;
                (*DAT_1005abec)(unaff_EBP + -0xba);
                *(undefined1 *)(unaff_EBP + -1) = 0x71;
                (*DAT_1005abec)(unaff_EBP + -0x5a);
                *(undefined1 *)(unaff_EBP + -1) = 0x70;
                (*DAT_1005abec)(unaff_EBP + -0x3c);
                *(undefined1 *)(unaff_EBP + -1) = 0x6f;
                (*DAT_1005abec)(unaff_EBP + -0x37);
                *(undefined1 *)(unaff_EBP + -1) = 0x6c;
                (*DAT_1005abec)(unaff_EBP + -0x40);
              }
            }
          }
          *(undefined1 *)(unaff_EBP + -1) = 0x6b;
          FUN_10017f20(unaff_EBP + -0x11);
          *(undefined1 *)(unaff_EBP + -1) = 0x15;
          FUN_10017f20(unaff_EBP + -0xf);
        }
LAB_1003a65f:
        *(undefined1 *)(unaff_EBP + -1) = 0x14;
        FUN_1000eb70(unaff_EBP + -0xa5);
        *(undefined1 *)(unaff_EBP + -1) = 0x10;
        (*DAT_1005abec)(unaff_EBP + -0x6d);
        *(undefined1 *)(unaff_EBP + -1) = 0xe;
        (*DAT_1005abec)(unaff_EBP + -0x15);
        *(undefined1 *)(unaff_EBP + -1) = 6;
        FUN_10017f20(unaff_EBP + -0x23);
        *(undefined1 *)(unaff_EBP + -1) = 5;
        FUN_10017f20(unaff_EBP + 1);
        *(undefined1 *)(unaff_EBP + -1) = 4;
        (*DAT_1005abec)(unaff_EBP + -8);
        *(undefined1 *)(unaff_EBP + -1) = 2;
        FUN_10017f20(unaff_EBP + -9);
        *(undefined1 *)(unaff_EBP + -1) = 1;
        FUN_10017f20(unaff_EBP);
        *(undefined1 *)(unaff_EBP + -1) = 0;
        FUN_10017f20(unaff_EBP + 4);
        unaff_EBP[-1] = -1;
        uVar11 = extraout_EDX_03;
        goto LAB_10035ed2;
      }
      uVar11 = FUN_10016760();
      if (((char)uVar11 != '\0') && (iVar8 = FUN_1001e270(extraout_ECX,extraout_EDX_00), iVar8 == 0)
         ) {
        FUN_1000fc80(unaff_EBP + -0x31,"ExceptionServers");
        *(undefined1 *)(unaff_EBP + -1) = 100;
        pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
        iVar8 = FUN_1000fec0((int)(unaff_EBP + -0x31),extraout_EDX_01,pcVar10);
        if (iVar8 == 0) {
          unaff_EBP[-0x11] = 0;
          unaff_EBP[-0x1b] = 0;
          unaff_EBP[-0x1d] = 0;
          piVar6 = (int *)unaff_EBP[-0x1a];
          FUN_10020060((int *)unaff_EBP[4],piVar6,(int *)unaff_EBP[1],(uint *)(unaff_EBP + -0x11),
                       unaff_EBP + -0x1b,unaff_EBP + -0x1d);
          iVar8 = iVar7 + 0x28;
          iVar9 = (**(code **)(*(int *)unaff_EBP[4] + 0xc4))((int *)unaff_EBP[4],piVar6,iVar8);
          unaff_EBP[2] = iVar9;
          FUN_10005620(unaff_EBP + -0x4a,"User-Agent",0xffffffff);
          *(undefined1 *)(unaff_EBP + -1) = 0x65;
          iVar9 = iVar7 + 0x58;
          iVar65 = (**(code **)(*(int *)unaff_EBP[4] + 0xe8))
                             ((int *)unaff_EBP[4],unaff_EBP[-0x1a],unaff_EBP + -0x4a,iVar9);
          unaff_EBP[2] = iVar65;
          FUN_10005620(unaff_EBP + -0x2f,"Cookie",0xffffffff);
          *(undefined1 *)(unaff_EBP + -1) = 0x66;
          iVar65 = (**(code **)(*(int *)unaff_EBP[4] + 0xe8))
                             ((int *)unaff_EBP[4],unaff_EBP[-0x1a],unaff_EBP + -0x2f,iVar7 + 0x38);
          unaff_EBP[2] = iVar65;
          (*DAT_1005abe4)(unaff_EBP + 10);
          *(undefined1 *)(unaff_EBP + -1) = 0x67;
          (*DAT_1005abe4)(unaff_EBP + 6);
          *(undefined1 *)(unaff_EBP + -1) = 0x68;
          piVar6 = unaff_EBP + 6;
          piVar28 = unaff_EBP + 10;
          pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
          FUN_1003b030(&DAT_1005b918,pcVar10,piVar28,piVar6);
          *(undefined1 *)((int)unaff_EBP + -0x29) = 0;
          iVar65 = FUN_10003890(iVar7 + 0x68);
          if (iVar65 == 0) {
            iVar65 = unaff_EBP[3] + 0x88;
            iVar7 = FUN_10003890(iVar65);
            if (iVar7 == 0) {
LAB_1003919c:
              *(undefined1 *)((int)unaff_EBP + -0x29) = 1;
            }
            else {
              sVar69 = 0x18;
              pcVar10 = "application/octet-stream";
              pvVar12 = (void *)FUN_10003910(iVar65);
              iVar7 = _memicmp(pvVar12,pcVar10,sVar69);
              if (iVar7 == 0) goto LAB_1003919c;
              sVar69 = 0x13;
              pcVar10 = "binary/octet-stream";
              pvVar12 = (void *)FUN_10003910(iVar65);
              iVar7 = _memicmp(pvVar12,pcVar10,sVar69);
              if (iVar7 == 0) goto LAB_1003919c;
              sVar69 = 0x12;
              pcVar10 = "application/x-sdlc";
              pvVar12 = (void *)FUN_10003910(iVar65);
              iVar7 = _memicmp(pvVar12,pcVar10,sVar69);
              if (iVar7 == 0) goto LAB_1003919c;
              sVar69 = 10;
              pcVar10 = "text/plain";
              pvVar12 = (void *)FUN_10003910(iVar65);
              iVar7 = _memicmp(pvVar12,pcVar10,sVar69);
              if (iVar7 == 0) goto LAB_1003919c;
            }
            iVar7 = unaff_EBP[3];
          }
          else if (*(char *)(iVar7 + 0x13) != '\0') {
            *(undefined1 *)((int)unaff_EBP + -0x29) = 1;
          }
          unaff_EBP[-0x2a] = (uint)(*(char *)((int)unaff_EBP + -0x29) == '\0') * 8 + 8;
          if ((-1 < *(int *)(iVar7 + 0xe0)) &&
             ((0 < *(int *)(iVar7 + 0xe0) || (*(int *)(iVar7 + 0xdc) != 0)))) {
            unaff_EBP[-0x2a] = unaff_EBP[-0x2a] | 0x80;
          }
          if (*(char *)((int)unaff_EBP + -0x86) != '\0') {
            unaff_EBP[-0x2a] = unaff_EBP[-0x2a] | 0x1000;
          }
          (*DAT_1005abe4)(unaff_EBP + 0x16);
          *(undefined1 *)(unaff_EBP + -1) = 0x69;
          (*DAT_1005abe4)(unaff_EBP + 0xe);
          *(undefined1 *)(unaff_EBP + -1) = 0x6a;
          FUN_10017d50((int *)unaff_EBP[4],unaff_EBP[-0x1a],unaff_EBP[1],unaff_EBP + 0x16);
          FUN_10019e90((int *)unaff_EBP[4],unaff_EBP[-0x1a],unaff_EBP[1],unaff_EBP + 0xe);
          iVar65 = FUN_10003890(unaff_EBP + 0x16);
          if (iVar65 == 0) {
LAB_10039283:
            pOVar18 = (BSTR)0x0;
          }
          else {
            pcVar10 = (char *)FUN_10003910(unaff_EBP + -8);
            pcVar13 = (char *)FUN_10003910(unaff_EBP + 0x16);
            iVar65 = _stricmp(pcVar13,pcVar10);
            if (iVar65 == 0) goto LAB_10039283;
            pOVar18 = (BSTR)FUN_10003910(unaff_EBP + 0x16);
          }
          puVar64 = (undefined4 *)0x0;
          pOVar61 = (BSTR)0x0;
          iVar59 = 0;
          pOVar56 = (OLECHAR *)0x0;
          pcVar10 = (char *)FUN_10003910(unaff_EBP + 0xe);
          pppcVar25 = (char ***)FUN_10003910(iVar7 + 0x88);
          uVar51 = unaff_EBP[-0x2a];
          uVar11 = 0;
          iVar65 = *(int *)(iVar7 + 0xe0);
          iVar50 = *(int *)(iVar7 + 0xdc);
          iVar49 = 0;
          pSVar46 = (SAFEARRAY *)0x0;
          piVar6 = (int *)FUN_10003910(unaff_EBP + -0x15);
          pcVar13 = (char *)FUN_10003910(iVar7 + 0x98);
          iVar43 = unaff_EBP[-0x11];
          pOVar42 = (BSTR)unaff_EBP[-0x1d];
          DVar4 = FUN_10003910(iVar7 + 0x68);
          ppcVar21 = (char **)FUN_10003910(iVar9);
          iVar8 = FUN_10003910(iVar8);
          iVar9 = FUN_10003910(unaff_EBP + 6);
          iVar26 = FUN_10003910(unaff_EBP + 10);
          pOVar27 = (BSTR)FUN_10003910(iVar7 + 0x38);
          pcVar14 = (char *)FUN_10003910(unaff_EBP + -8);
          cVar3 = FUN_10028f50(pcVar14,pOVar27,iVar26,iVar9,iVar8,ppcVar21,DVar4,pOVar42,iVar43,
                               pcVar13,piVar6,pSVar46,iVar49,iVar50,iVar65,uVar11,uVar51,pppcVar25,
                               pcVar10,pOVar18,pOVar56,iVar59,pOVar61,puVar64);
          if ((void *)unaff_EBP[-0x1b] != (void *)0x0) {
            operator_delete((void *)unaff_EBP[-0x1b]);
          }
          if (cVar3 != '\0') {
            iVar7 = (**(code **)(*(int *)unaff_EBP[4] + 0xf4))
                              ((int *)unaff_EBP[4],unaff_EBP[-0x1a],0xffffffff);
            unaff_EBP[2] = iVar7;
            if (iVar7 < 0) {
              *(undefined1 *)(unaff_EBP + -1) = 0x69;
              (*DAT_1005abec)(unaff_EBP + 0xe);
              *(undefined1 *)(unaff_EBP + -1) = 0x68;
              (*DAT_1005abec)(unaff_EBP + 0x16);
              *(undefined1 *)(unaff_EBP + -1) = 0x67;
              (*DAT_1005abec)(unaff_EBP + 6);
              *(undefined1 *)(unaff_EBP + -1) = 0x66;
              (*DAT_1005abec)(unaff_EBP + 10);
              *(undefined1 *)(unaff_EBP + -1) = 0x65;
              (*DAT_1005abec)(unaff_EBP + -0x2f);
              *(undefined1 *)(unaff_EBP + -1) = 100;
              (*DAT_1005abec)(unaff_EBP + -0x4a);
              *(undefined1 *)(unaff_EBP + -1) = 0x15;
              FUN_1000fd50(unaff_EBP + -0x31);
              *(undefined1 *)(unaff_EBP + -1) = 0x14;
              FUN_1000eb70(unaff_EBP + -0xa5);
              *(undefined1 *)(unaff_EBP + -1) = 0x10;
              (*DAT_1005abec)(unaff_EBP + -0x6d);
              *(undefined1 *)(unaff_EBP + -1) = 0xe;
              (*DAT_1005abec)(unaff_EBP + -0x15);
              *(undefined1 *)(unaff_EBP + -1) = 6;
              FUN_10017f20(unaff_EBP + -0x23);
              *(undefined1 *)(unaff_EBP + -1) = 5;
              FUN_10017f20(unaff_EBP + 1);
              *(undefined1 *)(unaff_EBP + -1) = 4;
              (*DAT_1005abec)(unaff_EBP + -8);
              *(undefined1 *)(unaff_EBP + -1) = 2;
              FUN_10017f20(unaff_EBP + -9);
              *(undefined1 *)(unaff_EBP + -1) = 1;
              FUN_10017f20(unaff_EBP);
              *(undefined1 *)(unaff_EBP + -1) = 0;
              FUN_10017f20(unaff_EBP + 4);
              uVar11 = extraout_EDX_02;
              goto LAB_10035ed2;
            }
            *(undefined1 *)(unaff_EBP + -1) = 0x69;
            (*DAT_1005abec)(unaff_EBP + 0xe);
            *(undefined1 *)(unaff_EBP + -1) = 0x68;
            (*DAT_1005abec)(unaff_EBP + 0x16);
            *(undefined1 *)(unaff_EBP + -1) = 0x67;
            (*DAT_1005abec)(unaff_EBP + 6);
            *(undefined1 *)(unaff_EBP + -1) = 0x66;
            (*DAT_1005abec)(unaff_EBP + 10);
            *(undefined1 *)(unaff_EBP + -1) = 0x65;
            (*DAT_1005abec)(unaff_EBP + -0x2f);
            *(undefined1 *)(unaff_EBP + -1) = 100;
            (*DAT_1005abec)(unaff_EBP + -0x4a);
            *(undefined1 *)(unaff_EBP + -1) = 0x15;
            FUN_1000fd50(unaff_EBP + -0x31);
            *(undefined1 *)(unaff_EBP + -1) = 0x14;
            FUN_1000eb70(unaff_EBP + -0xa5);
            *(undefined1 *)(unaff_EBP + -1) = 0x10;
            (*DAT_1005abec)(unaff_EBP + -0x6d);
            *(undefined1 *)(unaff_EBP + -1) = 0xe;
            (*DAT_1005abec)(unaff_EBP + -0x15);
            *(undefined1 *)(unaff_EBP + -1) = 6;
            FUN_10017f20(unaff_EBP + -0x23);
            *(undefined1 *)(unaff_EBP + -1) = 5;
            FUN_10017f20(unaff_EBP + 1);
            *(undefined1 *)(unaff_EBP + -1) = 4;
            goto LAB_10035ea3;
          }
          *(undefined1 *)(unaff_EBP + -1) = 0x69;
          (*DAT_1005abec)(unaff_EBP + 0xe);
          *(undefined1 *)(unaff_EBP + -1) = 0x68;
          (*DAT_1005abec)(unaff_EBP + 0x16);
          *(undefined1 *)(unaff_EBP + -1) = 0x67;
          (*DAT_1005abec)(unaff_EBP + 6);
          *(undefined1 *)(unaff_EBP + -1) = 0x66;
          (*DAT_1005abec)(unaff_EBP + 10);
          *(undefined1 *)(unaff_EBP + -1) = 0x65;
          (*DAT_1005abec)(unaff_EBP + -0x2f);
          *(undefined1 *)(unaff_EBP + -1) = 100;
          (*DAT_1005abec)(unaff_EBP + -0x4a);
          *(undefined1 *)(unaff_EBP + -1) = 0x15;
          FUN_1000fd50(unaff_EBP + -0x31);
          goto LAB_100395bf;
        }
        *(undefined1 *)(unaff_EBP + -1) = 0x15;
        FUN_1000fd50(unaff_EBP + -0x31);
      }
      *(undefined1 *)(unaff_EBP + -1) = 0x14;
      FUN_1000eb70(unaff_EBP + -0xa5);
      *(undefined1 *)(unaff_EBP + -1) = 0x10;
      (*DAT_1005abec)(unaff_EBP + -0x6d);
      *(undefined1 *)(unaff_EBP + -1) = 0xe;
      (*DAT_1005abec)(unaff_EBP + -0x15);
    }
    *(undefined1 *)(unaff_EBP + -1) = 6;
    FUN_10017f20(unaff_EBP + -0x23);
    *(undefined1 *)(unaff_EBP + -1) = 5;
    FUN_10017f20(unaff_EBP + 1);
    *(undefined1 *)(unaff_EBP + -1) = 4;
  }
  else {
    if (((((iVar8 != 0x12d) && (iVar8 != 0x12e)) && (iVar8 != 0x12f)) &&
        ((iVar8 != 0x133 && (DAT_1005ac5c != (HANDLE)0x0)))) &&
       (DVar4 = WaitForSingleObject(DAT_1005ac5c,1000), DVar4 == 0)) {
      *(undefined1 *)(unaff_EBP + -1) = 0xc;
      pbVar5 = (byte *)FUN_10003910(unaff_EBP + -8);
      FUN_1003b360(&DAT_1005c160,pbVar5);
      unaff_EBP[-1] = 6;
      ReleaseMutex(DAT_1005ac5c);
    }
    *(undefined1 *)(unaff_EBP + -1) = 5;
    FUN_10017f20(unaff_EBP + 1);
    *(undefined1 *)(unaff_EBP + -1) = 4;
  }
LAB_10035ea3:
  (*DAT_1005abec)(unaff_EBP + -8);
  *(undefined1 *)(unaff_EBP + -1) = 2;
  FUN_10017f20(unaff_EBP + -9);
  *(undefined1 *)(unaff_EBP + -1) = 1;
  FUN_10017f20(unaff_EBP);
  *(undefined1 *)(unaff_EBP + -1) = 0;
  FUN_10017f20(unaff_EBP + 4);
  uVar11 = extraout_EDX;
LAB_10035ed2:
  ExceptionList = (void *)unaff_EBP[-3];
  uStack0000000c = 0x10035eea;
  FUN_100447e0(unaff_EBP[0x1a] ^ (uint)unaff_EBP,uVar11);
  return;
LAB_10036d92:
  iVar8 = *(int *)(iVar8 + 100);
  *(undefined1 *)(unaff_EBP + -1) = 0x18;
  FUN_10001910(unaff_EBP + 0xe);
  goto LAB_10036c90;
  while( true ) {
    bVar1 = pbVar5[1];
    bVar31 = bVar1 < pbVar17[1];
    if (bVar1 != pbVar17[1]) goto LAB_10036e43;
    pbVar5 = pbVar5 + 2;
    pbVar17 = pbVar17 + 2;
    if (bVar1 == 0) break;
LAB_10036e23:
    bVar1 = *pbVar5;
    bVar31 = bVar1 < *pbVar17;
    if (bVar1 != *pbVar17) {
LAB_10036e43:
      iVar8 = (1 - (uint)bVar31) - (uint)(bVar31 != 0);
      goto LAB_10036e48;
    }
    if (bVar1 == 0) break;
  }
  iVar8 = 0;
LAB_10036e48:
  if (iVar8 != 0) goto LAB_10036e52;
  *(undefined1 *)((int)unaff_EBP + -0x55) = 1;
  goto LAB_10036e93;
}


