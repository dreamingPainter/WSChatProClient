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
//下载确认
typedef struct bin_get_ack {
	char type;
	uint16_t len;
	uint64_t crc64;
	uint16_t port;
}bin_get_ack_msg;
//短消息
typedef struct msg_txt_head {
	char type;
	uint16_t len;
	uint32_t fromID;
	uint32_t toID;
	uint16_t msg_len;
	char buf[4];
}msg_txt_frame;

//列表消息
typedef struct group_list_head {
	char type;
	uint16_t len;
	char msg_type;
	uint32_t group_id;
	uint16_t item_num;
	char ptr[2];
}group_list_frame;
//列表成员
typedef struct item {
	uint64_t id;
	char len;
	char ptr[2];
}item;

//加退群帧
typedef struct group_in_quit {
	char type;
	uint16_t len;
	uint32_t group_id;
	char msg_type;
}group_op_frame;
#pragma pack()

//上传响应消息
typedef struct msg_bin_ack_msg{
	char type;
	uint16_t len;
	uint64_t crc64;
	uint16_t port;
}msg_bin_ack_msg;




uint64_t htonll(uint64_t val)
{
	return (((uint64_t)htonl(val)) << 32) + htonl(val >> 32);
}

uint64_t ntohll(uint64_t val)
{
	return (((uint64_t)ntohl(val)) << 32) + ntohl(val >> 32);
}
//下载响应
void bin_get_ack(int len) {
	char send_buf[128];
	bin_get_ack_msg* x = (bin_get_ack_msg*)send_buf;
	x->type = 0x07;
	x->len = 10;
	x->crc64 = 0x1112131415161718;
	x->port = 0x1234;
	sendto(s, send_buf, sizeof(send_buf), 0, (sockaddr*)&remote, len);
}
//上传响应
void msg_bin_ack(int len) {
	char send_buf[128];
	msg_bin_ack_msg* x = (msg_bin_ack_msg*)send_buf;
	x->type = 0x04;
	x->len = 10;
	x->crc64 = 0x1112131415161718;
	x->port = 0x1234;
	sendto(s, send_buf, sizeof(send_buf), 0, (sockaddr*)&remote, len);
}

//加群
void join_group_ack(int len){
	char send_buf[128];
	group_op_frame* x=(group_op_frame*)send_buf;
	x->type = 5;
	x->len = htons(5);
	x->group_id = htonl(16);
	x->msg_type = 0;//0成功，1失败
	sendto(s, send_buf, sizeof(send_buf), 0, (sockaddr*)&remote, len);
}
//退群
void quit_group_ack(int len) {
	char send_buf[128];
	group_op_frame* x = (group_op_frame*)send_buf;
	x->type = 6;
	x->len = htons(5);
	x->group_id = htonl(16);
	x->msg_type = 0;//0成功，1失败
	sendto(s, send_buf, sizeof(send_buf), 0, (sockaddr*)&remote, len);
}
//短消息
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
//列表请求响应
void member_list(int len) {
	char send_buf[128] = { 0 };
	char username[50] = { 0 };
	group_list_frame *x = (group_list_frame*)send_buf;
	item* y;
	x->type = 0x08;
	x->msg_type = 0x00;
	x->group_id = htonl(8);
	x->item_num = htons(2);
	char *p = x->ptr;
	//hello
	y = (item*)p;
	y->id = htonll(uint64_t(11111));
	strcpy_s(username, "hello");
	y->len = strlen(username);
	memcpy(y->ptr, username,strlen(username));
	x->len = 12 + y->len;
	//happy
	p += 9 + y->len;
	y = (item*)p;
	y->id = htonll(uint64_t(333));
	memcpy(username, "goodday",7);
	y->len = 7;
	x->len += 9 + y->len;
	x->len = htons(x->len);
	memcpy(y->ptr, username,strlen(username));
	sendto(s, send_buf, sizeof(send_buf), 0, (sockaddr*)&remote, len);
}

void receive_file(int len) {

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

		switch (*buf) {
		case 0x02://发一个短消息
			msg_txt(buf, len);
			break;
		case 0x03://文件上传响应
			msg_bin_ack(len);
		case 0x04://文件接收
			receive_file(len);
			break;
		case 0x05://加群响应
			join_group_ack(len);
			break;
		case 0x06://退群响应
			quit_group_ack(len);
			break;
		case 0x08://好友列表响应
			member_list(len);
			break;
		}
	}
	closesocket(s);
	WSACleanup();
	return 0;
}

