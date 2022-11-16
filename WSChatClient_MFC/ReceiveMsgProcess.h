#define _CRT_SECURE_NO_WARNINGS
#include "resource.h"
#include "WSChatClient_MFCdlg.h"
#include "crc_funcion.h"

void bit32_data_into_buf(uint32_t src, CStringA *dst_data);
void bit16_data_into_buf(uint16_t src, CStringA* dst_data);

uint32_t calculate_chllenge_sum(char *ptr)//传入的ptr指向N
{
	unsigned char N;
	char *challenge_value_pos;
	uint32_t sum,bitset_byte_num,value_32;
	uint16_t value_16;

	N = *ptr;
	bitset_byte_num = (N / 8) + ((N % 8) ? 1 : 0);
	challenge_value_pos = ptr + bitset_byte_num + 1;

	for (int count = 0; count < N; count++)
	{	//对于第i个数，其对应的置位在第几个字节的第几位
		unsigned char byte_buf = *(ptr + count / 8 + ((count % 8) ? 1 : 0));
		if (byte_buf&(0x01<<(count%8)))//4字节
		{
			value_32 = *(uint32_t*)challenge_value_pos;
			sum += ntohl(value_32);
			challenge_value_pos += 4;
		}
		else {//2字节
			value_16 = *(uint16_t*)challenge_value_pos;
			sum += ntohl(value_16);
			challenge_value_pos += 2;
		}
	}
	return sum;
}



afx_msg LRESULT CWSChatClientMFCDlg::OnLoginMsg(WPARAM wParam, LPARAM lParam)
{
	char* ptr_header, *ptr, buf_byte;
	unsigned char msg_type;
	uint32_t sum32, crc32_value;
	CStringA user_key;
	CString input_text;
	unsigned char buf[50];

	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	ptr += 3;
	msg_type = *(ptr);
	ptr++;
	switch (msg_type)
	{
	case 0x00://no such user
		MessageBox(L"no such user");
		break;
	case 0x01://challenge,又忘记了
		sum32 = calculate_chllenge_sum(ptr);
		private_pwd.GetWindowText(input_text);
		memcpy(buf, input_text.GetBuffer(input_text.GetLength()), input_text.GetLength());
		crc32_value = crc32_function(buf,input_text.GetLength(),sum32);
		
		send_data.Empty();
		//0x01 LOGIN 1B
		buf_byte = 0x01;
		//len 2B
		bit16_data_into_buf(0x03, &send_data);
		//msg_type
		bit16_data_into_buf(0x01, &send_data);
		//32 crc
		bit32_data_into_buf(crc32_value, &send_data);
		if (sendto(s_u, send_data, sizeof(send_data), 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		break;
	case 0x02:
		user_id = *(uint32_t*)ptr;
		user_id = ntohl(user_id);
		break;
	case 0x03:
		MessageBox(L"login failed");
		break;
	default:
		break;
	}
	free(ptr_header);
	return 0;
}


afx_msg LRESULT CWSChatClientMFCDlg::OnTxtMsg(WPARAM wParam, LPARAM lParam)
{
	char* ptr_header, * ptr, buf_byte, show_buf[2048];
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	uint32_t fromID, toID;
	uint16_t len, msg_len;
	CString show_txt;

	ptr = ptr + 3;

	for (int i = 0; i < 4; i++)
	{
		fromID = *ptr | 0x00;
		fromID << 8;
		ptr++;
	}
	for (int i = 0; i < 4; i++)
	{
		toID = *ptr | 0x00;
		toID << 8;
		ptr++;
	}
	//strcpy(show_buf,ptr);
	show_txt = "Me";
	show_txt = show_buf + '\r\n';
	show_txt_buf.SetSel(show_txt_buf.GetWindowTextLength(), show_txt_buf.GetWindowTextLengthW());
	show_txt_buf.ReplaceSel(show_txt);
	show_txt_buf.LineScroll(show_txt_buf.GetLineCount());
	free(ptr_header);//https://blog.51cto.com/u_15127700/3727200
	return 0;
}


/*收到ACK后，开始进行文件传输即上传*/
afx_msg LRESULT CWSChatClientMFCDlg::OnBinAckMsg(WPARAM wParam, LPARAM lParam)
{

	char* ptr_header, * ptr, sendbuf[1024], buf_byte;
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	FILE* fp;
	int pos;
	uint16_t len;
	uint64_t retval_file_len;
	CStringA filename, string_crc64, msg_header;
	CString input_text;
	//读取文件名/传输对象
	file_path.GetWindowText(input_text);
	pos = input_text.ReverseFind('\\');
	filename = input_text.Right(input_text.GetLength() - pos + 1);//文件名


	//锁定窗口，减少bug和代码量
	file_path.EnableWindow(FALSE);
	state_of_client.SetWindowTextW(L"文件上传中，暂时无法进行其他操作");
	fp = fopen(filename, "r");

	//添加最外层头部
	send_data.Empty();
	buf_byte = 0x04;
	send_data = buf_byte;
	for (int i = 0; i < 8; i++)//crc64
	{
		buf_byte = crc64_of_file | 0x00;
		crc64_of_file >> 8;
		msg_header = msg_header + buf_byte;
	}
	//其实可以对文件的状态进行管理，每次判断状态（不想写了，堵着吧先
	do {
		retval_file_len = fread(sendbuf, sizeof(char), sizeof(sendbuf), fp);
		retval = retval_file_len;
		for (int i = 0; i < 8; i++)//数据长度
		{
			buf_byte = retval_file_len | 0x00;
			retval_file_len >> 8;
			msg_header = msg_header + buf_byte;
		};
		len = retval + 8;
		for (int i = 0; i < 8; i++)//数据长度
		{
			buf_byte = retval_file_len | 0x00;
			retval_file_len >> 8;
			send_data = send_data + buf_byte;
		};
		sendbuf[strlen(sendbuf) + 1] = '\0';
		send_data = send_data + msg_header + sendbuf;
		//send(s_t, send_data, strlen(send_data) + 1, 0);
	} while (retval < sizeof(sendbuf));



	return 0;
}


afx_msg LRESULT CWSChatClientMFCDlg::OnGrpJoinMsg(WPARAM wParam, LPARAM lParam)
{
	char* ptr_header, * ptr;
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	unsigned char group_id[4], msg_type;
	int count;

	ptr += 3;
	for (count = 0; count < 4; count++)
	{
		last_group_id += *ptr;
		ptr++;
	}
	msg_type = *ptr;
	if (msg_type == 0x00)
	{
		//ugroup_id.SetWindowTextW(last_group_id);
		ugroup_id.EnableWindow(FALSE);
	}
	else {
		MessageBox(L"Join group Failed");
	}

	return 0;
}


afx_msg LRESULT CWSChatClientMFCDlg::OnGrpQuitMsg(WPARAM wParam, LPARAM lParam)
{
	char* ptr_header, * ptr;
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	unsigned char group_id[4], msg_type;
	int count;

	ptr += 3;
	for (count = 0; count < 4; count++)
	{
		last_group_id += *ptr;
		ptr++;
	}
	msg_type = *ptr;
	if (msg_type == 0x00)
	{
		ugroup_id.Clear();
		ugroup_id.EnableWindow(TRUE);
	}
	else {
		MessageBox(L"Quit group Failed");
	}

	return 0;
}

/*收到下载文件的消息*/
afx_msg LRESULT CWSChatClientMFCDlg::OnBinGetMsg(WPARAM wParam, LPARAM lParam)
{	//CRC64 8B|port 2B|
	char* ptr_header, * ptr;
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	ptr += 3;
	uint64_t crc64;
	uint16_t port;
	crc64 = 0x0000000000000000;//8 byte 16F
	for (int count = 0; count < 8; count++)
	{
		crc64 = (crc64 | (*ptr)) << 8;
		ptr++;
	}
	port = 0x00000000 | (*ptr);
	port = (port << 8) | *(++ptr);
	if (port == 0)
		MessageBox(L"Get file failed");
	else {
		transmmit_channel.sin_family = AF_INET;
		transmmit_channel.sin_port = port;
		transmmit_channel.sin_addr = server.sin_addr;
		//if (connect(s_t, (sockaddr*)&transmmit_channel, sizeof(transmmit_channel)) == SOCKET_ERROR)
		//	MessageBox(L"Receive preparation failed");

	}
	free(ptr_header);
	return 0;
}

//列表
afx_msg LRESULT CWSChatClientMFCDlg::OnGrpListMsg(WPARAM wParam, LPARAM lParam)
{
	//type 1B|len 2B|msg_type 1B|group id 4B|itemlen 2B|id of item 8B|len 1B|username/filename
	char* ptr_header, * ptr;
	char buf[1024];
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	unsigned char type;
	char b4, b3, b2, b1, bh, bl;
	CString group_id;
	uint16_t itemlen;

	//好麻烦,用结构体实现会不会好一点
	ptr += 3;
	type = *(ptr);
	for (int count = 0; count < 4; count++)
		group_id = group_id + *(++ptr);
	if (group_id == last_group_id)
	{
		itemlen = MAKEWORD(*(++ptr), *(++ptr));
		switch (type)
		{
		case 0x00:

			break;
		case 0x01:

			break;
		default:
			break;
		}
	}
	else MessageBox(L"Get wrong List!");
	free(ptr);
	return 0;
}

void bit32_data_into_buf(uint32_t src, CStringA* dst_data)
{
	char buf_byte;
	uint32_t src_buf;
	for (int i = 1; i < 4; i++)
	{
		src_buf = src;
		src_buf >> (32 - i * 8);
		buf_byte = src_buf;
		dst_data = dst_data + buf_byte;
	}
	return;
}

void bit16_data_into_buf(uint16_t src, CStringA* dst_data)
{
	char buf_byte;
	buf_byte = HIBYTE(src);
	dst_data = dst_data + buf_byte;
	buf_byte = LOBYTE(src);
	dst_data = dst_data + buf_byte;
}

