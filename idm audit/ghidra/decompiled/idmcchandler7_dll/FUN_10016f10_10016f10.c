// Function: FUN_10016f10
// Entry Point: 0x10016f10
// Program: idmcchandler7.dll


undefined4 __cdecl FUN_10016f10(void *param_1,void *param_2)

{
  int iVar1;
  undefined4 uVar2;
  uint uVar3;
  
  (*DAT_1005abfc)(param_2,&DAT_1004cdda,0xffffffff);
  iVar1 = _memicmp(param_1,"application/octet-stream",0x18);
  if (iVar1 != 0) {
    iVar1 = _memicmp(param_1,"binary/octet-stream",0x13);
    if (iVar1 != 0) {
      iVar1 = _memicmp(param_1,"application/x-msdos-program",0x1b);
      if (iVar1 != 0) {
        iVar1 = _memicmp(param_1,"application/x-sdlc",0x12);
        if (iVar1 != 0) {
          iVar1 = _memicmp(param_1,"application/x-msi",0x11);
          if (iVar1 == 0) {
            uVar2 = FUN_10001090(param_2,&DAT_1004ead8);
            return CONCAT31((int3)((uint)uVar2 >> 8),1);
          }
          iVar1 = _memicmp(param_1,"application/zip",0xf);
          if (iVar1 != 0) {
            iVar1 = _memicmp(param_1,"application/x-zip-compressed",0x1c);
            if (iVar1 != 0) {
              iVar1 = _memicmp(param_1,"application/x-rar-compressed",0x1c);
              if (iVar1 != 0) {
                iVar1 = _memicmp(param_1,"application/x-rar",0x11);
                if (iVar1 != 0) {
                  iVar1 = _memicmp(param_1,"application/x-gtar",0x12);
                  if (iVar1 != 0) {
                    iVar1 = _memicmp(param_1,"application/x-tar",0x11);
                    if (iVar1 != 0) {
                      iVar1 = _memicmp(param_1,"application/x-compressed",0x18);
                      if (iVar1 == 0) {
                        uVar2 = FUN_10001090(param_2,&DAT_1004ea20);
                        return CONCAT31((int3)((uint)uVar2 >> 8),1);
                      }
                      iVar1 = _memicmp(param_1,"application/x-compress",0x16);
                      if (iVar1 == 0) {
                        uVar2 = FUN_10001090(param_2,&DAT_1004ea04);
                        return CONCAT31((int3)((uint)uVar2 >> 8),1);
                      }
                      iVar1 = _memicmp(param_1,"application/x-gzip",0x12);
                      if (iVar1 != 0) {
                        iVar1 = _memicmp(param_1,"application/gzip",0x10);
                        if (iVar1 != 0) {
                          iVar1 = _memicmp(param_1,"application/msword",0x12);
                          if (iVar1 == 0) {
                            uVar2 = FUN_10001090(param_2,&DAT_1004e9c0);
                            return CONCAT31((int3)((uint)uVar2 >> 8),1);
                          }
                          iVar1 = _memicmp(param_1,"application/x-aim",0x11);
                          if (iVar1 == 0) {
                            uVar2 = FUN_10001090(param_2,&DAT_1004e9a8);
                            return CONCAT31((int3)((uint)uVar2 >> 8),1);
                          }
                          iVar1 = _memicmp(param_1,"video/msvideo",0xd);
                          if (iVar1 != 0) {
                            iVar1 = _memicmp(param_1,"video/x-msvideo",0xf);
                            if (iVar1 != 0) {
                              iVar1 = _memicmp(param_1,"video/avi",9);
                              if (iVar1 != 0) {
                                iVar1 = _memicmp(param_1,"audio/mp3",9);
                                if (iVar1 != 0) {
                                  iVar1 = _memicmp(param_1,"audio/mp4",9);
                                  if (iVar1 == 0) {
                                    uVar2 = FUN_10001090(param_2,&DAT_1004e958);
                                    return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                  }
                                  iVar1 = _memicmp(param_1,"audio/mpeg",10);
                                  if (iVar1 != 0) {
                                    iVar1 = _memicmp(param_1,"audio/x-mpeg",0xc);
                                    if (iVar1 != 0) {
                                      iVar1 = _memicmp(param_1,"video/mpegts",0xc);
                                      if (iVar1 == 0) {
                                        uVar2 = FUN_10001090(param_2,&DAT_1004e928);
                                        return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                      }
                                      iVar1 = _memicmp(param_1,"video/mpeg",10);
                                      if (iVar1 == 0) {
                                        uVar2 = FUN_10001090(param_2,&DAT_1004e918);
                                        return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                      }
                                      iVar1 = _memicmp(param_1,"video/mpg4",10);
                                      if (iVar1 != 0) {
                                        iVar1 = _memicmp(param_1,"video/x-mpg4",0xc);
                                        if (iVar1 != 0) {
                                          iVar1 = _memicmp(param_1,"video/mp4",9);
                                          if (iVar1 != 0) {
                                            iVar1 = _memicmp(param_1,"video/x-mp4",0xb);
                                            if (iVar1 != 0) {
                                              iVar1 = _memicmp(param_1,"video/quicktime",0xf);
                                              if (iVar1 == 0) {
                                                uVar2 = FUN_10001090(param_2,&DAT_1004e8dc);
                                                return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                              }
                                              iVar1 = _memicmp(param_1,"video/x-ms-asf",0xe);
                                              if (iVar1 == 0) {
                                                uVar2 = FUN_10001090(param_2,&DAT_1004e8c8);
                                                return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                              }
                                              iVar1 = _memicmp(param_1,"video/x-ms-wmv",0xe);
                                              if (iVar1 == 0) {
                                                uVar2 = FUN_10001090(param_2,&DAT_1004e8b4);
                                                return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                              }
                                              iVar1 = _memicmp(param_1,"image/tiff",10);
                                              if (iVar1 == 0) {
                                                uVar2 = FUN_10001090(param_2,&DAT_1004e8a4);
                                                return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                              }
                                              iVar1 = _memicmp(param_1,"application/pdf",0xf);
                                              if (iVar1 == 0) {
                                                uVar2 = FUN_10001090(param_2,&DAT_1004e890);
                                                return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                              }
                                              iVar1 = _memicmp(param_1,"audio/wav",9);
                                              if (iVar1 != 0) {
                                                iVar1 = _memicmp(param_1,"audio/x-wav",0xb);
                                                if (iVar1 != 0) {
                                                  iVar1 = _memicmp(param_1,"audio/x-ms-wma",0xe);
                                                  if (iVar1 == 0) {
                                                    uVar2 = FUN_10001090(param_2,&DAT_1004e860);
                                                    return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                  }
                                                  iVar1 = _memicmp(param_1,
                                                  "application/x-msdownload",0x18);
                                                  if (iVar1 == 0) {
                                                    uVar2 = FUN_10001090(param_2,&DAT_1004eb34);
                                                    return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                  }
                                                  iVar1 = _memicmp(param_1,"video/x-flv",0xb);
                                                  if (iVar1 != 0) {
                                                    iVar1 = _memicmp(param_1,"video/flv",9);
                                                    if (iVar1 != 0) {
                                                      iVar1 = _memicmp(param_1,"video/x-flash-video"
                                                                       ,0x13);
                                                      if (iVar1 != 0) {
                                                        iVar1 = _memicmp(param_1,
                                                  "flv-application/octet-stream",0x1c);
                                                  if (iVar1 != 0) {
                                                    iVar1 = _memicmp(param_1,"text/html",9);
                                                    if (iVar1 == 0) {
                                                      uVar2 = FUN_10001090(param_2,&DAT_1004e7e4);
                                                      return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                    }
                                                    iVar1 = _memicmp(param_1,"text/javascript",0xf);
                                                    if (iVar1 != 0) {
                                                      iVar1 = _memicmp(param_1,
                                                  "application/x-javascript",0x18);
                                                  if (iVar1 != 0) {
                                                    iVar1 = _memicmp(param_1,"image/gif",9);
                                                    if (iVar1 == 0) {
                                                      uVar2 = FUN_10001090(param_2,&DAT_1004e7a4);
                                                      return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                    }
                                                    iVar1 = _memicmp(param_1,"image/jpeg",10);
                                                    if (iVar1 == 0) {
                                                      uVar2 = FUN_10001090(param_2,&DAT_1004e790);
                                                      return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                    }
                                                    iVar1 = _memicmp(param_1,"image/jpg",9);
                                                    if (iVar1 == 0) {
                                                      uVar2 = FUN_10001090(param_2,&DAT_1004e780);
                                                      return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                    }
                                                    iVar1 = _memicmp(param_1,"text/css",8);
                                                    if (iVar1 == 0) {
                                                      uVar2 = FUN_10001090(param_2,&DAT_1004e770);
                                                      return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                    }
                                                    iVar1 = _memicmp(param_1,
                                                  "application/x-shockwave-flash",0x1d);
                                                  if (iVar1 == 0) {
                                                    uVar2 = FUN_10001090(param_2,&DAT_1004e74c);
                                                    return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                  }
                                                  iVar1 = _memicmp(param_1,"video/webm",10);
                                                  if (iVar1 != 0) {
                                                    iVar1 = _memicmp(param_1,"audio/webm",10);
                                                    if (iVar1 != 0) {
                                                      iVar1 = _memicmp(param_1,"video/f4m",9);
                                                      if (iVar1 != 0) {
                                                        iVar1 = _memicmp(param_1,
                                                  "application/f4m+xml",0x13);
                                                  if (iVar1 != 0) {
                                                    iVar1 = _memicmp(param_1,"audio/mpegurl",0xd);
                                                    if (iVar1 == 0) {
                                                      uVar2 = FUN_10001090(param_2,&DAT_1004e6fc);
                                                      return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                    }
                                                    iVar1 = _memicmp(param_1,
                                                  "application/vnd.apple.mpegurl",0x1d);
                                                  if (iVar1 != 0) {
                                                    iVar1 = _memicmp(param_1,"application/x-mpegURL"
                                                                     ,0x15);
                                                    if (iVar1 != 0) {
                                                      iVar1 = _memicmp(param_1,
                                                  "application/x-mobipocket-ebook",0x1e);
                                                  if (iVar1 == 0) {
                                                    uVar2 = FUN_10001090(param_2,&DAT_1004e694);
                                                    return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                  }
                                                  uVar3 = _memicmp(param_1,"application/epub",0x10);
                                                  if (uVar3 == 0) {
                                                    uVar2 = FUN_10001090(param_2,&DAT_1004e678);
                                                    return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                  }
                                                  return uVar3 & 0xffffff00;
                                                  }
                                                  }
                                                  uVar2 = FUN_10001090(param_2,&DAT_1004e6d4);
                                                  return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                  }
                                                  }
                                                  uVar2 = FUN_10001090(param_2,&DAT_1004e724);
                                                  return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                  }
                                                  }
                                                  uVar2 = FUN_10001090(param_2,&DAT_1004e238);
                                                  return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                  }
                                                  }
                                                  uVar2 = FUN_10001090(param_2,&DAT_1004e7d0);
                                                  return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                  }
                                                  }
                                                  }
                                                  }
                                                  uVar2 = FUN_10001090(param_2,&DAT_1004cafc);
                                                  return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                                }
                                              }
                                              uVar2 = FUN_10001090(param_2,&DAT_1004e880);
                                              return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                            }
                                          }
                                        }
                                      }
                                      uVar2 = FUN_10001090(param_2,&DAT_1004caec);
                                      return CONCAT31((int3)((uint)uVar2 >> 8),1);
                                    }
                                  }
                                }
                                uVar2 = FUN_10001090(param_2,&DAT_1004e968);
                                return CONCAT31((int3)((uint)uVar2 >> 8),1);
                              }
                            }
                          }
                          uVar2 = FUN_10001090(param_2,&DAT_1004e994);
                          return CONCAT31((int3)((uint)uVar2 >> 8),1);
                        }
                      }
                      uVar2 = FUN_10001090(param_2,&DAT_1004e9ec);
                      return CONCAT31((int3)((uint)uVar2 >> 8),1);
                    }
                  }
                  uVar2 = FUN_10001090(param_2,&DAT_1004ea54);
                  return CONCAT31((int3)((uint)uVar2 >> 8),1);
                }
              }
              uVar2 = FUN_10001090(param_2,&DAT_1004ea80);
              return CONCAT31((int3)((uint)uVar2 >> 8),1);
            }
          }
          uVar2 = FUN_10001090(param_2,&DAT_1004eac4);
          return CONCAT31((int3)((uint)uVar2 >> 8),1);
        }
      }
    }
  }
  uVar2 = (*DAT_1005abfc)(param_2,&DAT_1004eb34,0xffffffff);
  return CONCAT31((int3)((uint)uVar2 >> 8),1);
}


