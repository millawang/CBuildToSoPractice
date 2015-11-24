//
//  ZipPassUtil.h
//  CTest
//
//  Created by 王怡婷 on 2015/9/14.
//  Copyright (c) 2015年 milla. All rights reserved.
//

#ifndef __CTest__ZipPassUtil__
#define __CTest__ZipPassUtil__

#include <stdio.h>
char *getZipPass(char *id);
int indexOf(char *source, char *target, int start);

void removeString(char *str, int start, int len);
void chngChar (char *pchrsource, char*pchrfind, char *pchrrep);

#endif /* defined(__CTest__ZipPassUtil__) */
