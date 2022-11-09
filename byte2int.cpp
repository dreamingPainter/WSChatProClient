#include <stdio.h>
#include <iostream>
#include <windef.h>
int main(){
    char b4,b3,b2,b1;
    int len = 80, relen;
    short int hword,lword;
    hword = HIWORD(len);
    lword = LOWORD(len);
    relen = MAKEWORD(lword,hword);
    printf("%x,%x,%x,%x\n",hword,lword,len,relen);
    printf("%x,%x,%x,%x\n",&hword,&lword,len,relen);
}