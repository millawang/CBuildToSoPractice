//
//  ZipPassUtil.c
//  CTest
//
//  Created by 王怡婷 on 2015/9/14.
//  Copyright (c) 2015年 milla. All rights reserved.
//

#include <jni.h>
#include <stdio.h>
#include <string.h>
#include "ZipPassUtil.h"
#include <android/log.h>

#define MAX_HEX_NUM 16
#define OUT_DATA_LEN sizeof(int)*2  // 2个16进制数表示1个字节

#define BITS_OF_INT   sizeof(int)*8  // int的位数
#define  LOG_TAG    "libZipPassUtil"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
static char Hex_Char_Table[MAX_HEX_NUM] = {
    '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
};

JNIEXPORT jstring Java_com_example_millawang_myapplication_MainActivity_getZipPass(JNIEnv* env,jclass c, jstring javaString){

    //char *id = "0101343685368356";
    char *id = (*env)->GetStringUTFChars(env, javaString, 0);
    //(*env)->ReleaseStringUTFChars(env, javaString, id);
    char output;
    char *to = (char*) malloc(1);
    char *last = strncpy(to, id+strlen(id)-1, 1);
    int ascII = last[0];
    int data =ascII;
    char hexCode[OUT_DATA_LEN+1];
    int i,index;
    printf("getZipPass id(1) = %s \n", id);
    hexCode[OUT_DATA_LEN] = '\0';
    for( i=OUT_DATA_LEN-1; i>=0; i-- )
    {
        index = data & 0xf;
        hexCode[i] = Hex_Char_Table[index];
        data = data>>4;
    }
   
    char s2[] = "00";
    int start = 0;
    int s2len = strlen(s2);
    while (1) {
        // 尋找s2在s1的位置, 回傳-1代表沒找到
        start = indexOf(hexCode, s2, start);
        if (start == -1) {
            break;
        }

        // 從s1移除 從start開始後面s2len個字元
        removeString(hexCode, start, s2len);
    }

    char finalCode[strlen(id)];
    strncpy(finalCode, id, strlen(id));
    finalCode[strlen(id)]='\0';

    int j;
    char value[10];
    if(strlen(hexCode) > 0){
       for(j=0;j<strlen(hexCode);j++){
           strcpy(value, hexCode+j);
           value[1] = '\0';
           chngChar(&finalCode, &value, "");
       }
    }
    (*env)->ReleaseStringUTFChars(env, javaString, id);
    return (*env)->NewStringUTF(env, finalCode);
}

int indexOf(char *source, char *target, int start) {
    int targetLen = strlen(target);
    int len = strlen(source) - targetLen;
    char *ptr;
    int i;
    
    for (i = start, ptr = (source + start); i <= len; ++i) {
        if (strncmp(ptr++, target, targetLen) == 0){
            return i;
        }
    }
    return -1;
}

void removeString(char *str, int start, int len) {
    char *ptr1 = str + start;
    char *ptr2 = ptr1 + len;
    while (*ptr2) {
        *ptr1++ = *ptr2++;
    }
    *ptr1 = '\0';
}

void chngChar (char *pchrsource, char*pchrfind, char *pchrrep) {
    int intrep;// 替換文字的長度
    int intfind;// 搜尋文字的長度
    int intlength;// 結果文字的長度
    int intgap=0;// 偏移量
    char pchrresult[30];
    char* pchrformer;
    char* pchrlocation;

    intfind=strlen(pchrfind);// 搜尋文字的長度
    intrep=strlen(pchrrep);// 替換文字的長度
    intlength=strlen(pchrsource)+1;// 結果文字的長度

    strcpy(pchrresult, pchrsource);//複製文字
    pchrformer=pchrsource;
    pchrlocation= strstr(pchrformer, pchrfind);//搜尋文字出現的起始位址指標

    while(pchrlocation!=NULL){

        intgap+=(pchrlocation - pchrformer);//定位偏移量
        pchrresult[intgap]='\0';// 將結束符號定在搜尋到的位址上

        intlength+=(intrep-intfind);//計算新的長度
        //pchrresult = (char*)realloc(pchrresult, intlength * sizeof(char));// 變更記憶體空間
        strcat(pchrresult, pchrrep);//串接在結果後面
        intgap+=intrep;//更新偏移量
        pchrformer=pchrlocation+intfind;//更新尚未被取代的字串的位址

        strcat(pchrresult, pchrformer);//將尚未被取代的文字串接在結果後面
        pchrlocation= strstr(pchrformer, pchrfind);//搜尋文字出現的起始位址指標
    }

    pchrresult[strlen(pchrresult)]='\0';
    strcpy(pchrsource, pchrresult);
}

