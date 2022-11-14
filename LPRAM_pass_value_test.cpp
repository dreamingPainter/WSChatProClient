
#include <stdio.h>
#include <iostream>
#include <windef.h>
    
int main()
{
    char recv_buf[1024];
    strcpy(recv_buf,"Hello");

    char *ptr = recv_buf;
    printf("%0lx %0lx %x\n",&recv_buf[0],recv_buf,(void*)&*ptr);
    
    LPARAM x;
    char *c;
    x = (LPARAM)(void*)&*ptr;
    c = (char*)(void*)x;
    printf("%x %s",x,c);
}