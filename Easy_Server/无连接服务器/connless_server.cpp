// connless.cpp : 从客户机收到数据后，回传一个ACK字符串.
//
#pragma once
#include <winsock.h>
#include <stdio.h>
#include <stdint.h>
#pragma comment(lib,"wsock32.lib")
SOCKET s;
sockaddr_in local;
sockaddr_in remote;
WSAData wsa;

#pragma pack(1)
typedef struct msg_txt_head {
	char type;
	uint16_t len;
	uint32_t fromID;
	uint32_t toID;
	uint16_t msg_len;
	char buf[4];
}msg_txt_frame;
#pragma pack()
void msg_txt(char *buf,int len) {
	char send_buf[128] = {0};
	msg_txt_frame *x = (msg_txt_frame*)send_buf;
	x->type = 0x02;
	x->len = htons(14);
	x->fromID = htonl(11111);
	x->toID = htonl(2);
	x->msg_len = htons(3);
	strcpy_s(x->buf,"hhh");
	sendto(s, send_buf, sizeof(send_buf), 0, (sockaddr*)&remote, len);
}

int main()
{
	printf("Hello,this is a static server");
	int len;
	char buf[128];

	WSAStartup(0x101,&wsa);
	s = socket(AF_INET,SOCK_DGRAM,0);
	
	local.sin_family = AF_INET;
	//local.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	local.sin_addr.S_un.S_un_b.s_b1 = 127;
	local.sin_addr.S_un.S_un_b.s_b2 = 0;
	local.sin_addr.S_un.S_un_b.s_b3 = 0;
	local.sin_addr.S_un.S_un_b.s_b4 = 1;
	local.sin_port = htons(6666);

	bind(s,(sockaddr*)&local,sizeof(local));
	while(1){
		len = sizeof(remote);
		recvfrom(s,buf,sizeof(buf),0,(sockaddr*)&remote,&len);
		printf("%s\n",buf);
		len = sizeof(remote);

		if (*buf == 0x02)
			msg_txt(buf, len);
		else continue;

	}
	closesocket(s);
	WSACleanup();
	return 0;
}

