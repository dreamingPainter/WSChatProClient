#include "resource.h"
#include "WSChatClient_MFCdlg.h"
#include "crc_funcion.h"


afx_msg LRESULT CWSChatClientMFCDlg::OnLoginMsg(WPARAM wParam, LPARAM lParam)
{
	char* ptr_header, * ptr;
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	char buf[1024];
	unsigned short port;
	unsigned char msg_type;
	char b4, b3, b2, b1, bh, bl;
	uint16_t len_N,bits,sum16;
	uint32_t sum32,crc32;

	ptr += 3;
	msg_type = *(ptr);
	switch (msg_type)
	{
	case 0x00://no such user
		MessageBox(L"no such user");
		break;
	case 0x01://challenge,又忘记了
		len_N = MAKEWORD(*(++ptr), *(++ptr));
		bits = len_N / 8 + !!(len_N % 8);
		if (bits == 16)//随便等于的
		{
			//sum16 = sum_16();
			//crc32 = crc_32();
			
		}
		if (bits == 32)
		{
			//sum32 = sum_32();
			//crc32 = crc32();
		}
		break;
	default:
		break;
	}

	return 0;
}


afx_msg LRESULT CWSChatClientMFCDlg::OnTxtMsg(WPARAM wParam, LPARAM lParam)
{
	char *ptr_header, * ptr, buf_byte,show_buf[2048];
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
	strcpy(show_buf,ptr);
	show_txt = "Me";
	show_txt = show_buf + '\r\n';
	show_txt_buf.SetSel(show_txt_buf.GetWindowTextLength(), show_txt_buf.GetWindowTextLengthW());
	show_txt_buf.ReplaceSel(show_txt);
	show_txt_buf.LineScroll(show_txt_buf.GetLineCount());
	free(ptr_header);//https://blog.51cto.com/u_15127700/3727200
}


/*收到ACK后，开始进行文件传输即上传*/
afx_msg LRESULT CWSChatClientMFCDlg::OnBinAckMsg(WPARAM wParam, LPARAM lParam)
{

	char* ptr_header, * ptr, sendbuf[1024],buf_byte;
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	FILE* fp;
	int pos;
	uint16_t len;
	uint64_t retval_file_len;
	CStringA filename,string_crc64,msg_header;
	CString input_text;
	//读取文件名/传输对象
	file_path.GetWindowText(input_text);
	pos = input_text.ReverseFind('\\');
	filename = input_text.Right(input_text.GetLength() - pos + 1);//文件名


	//锁定窗口，减少bug和代码量
	file_path.EnableWindow(FALSE);
	state_of_client.SetWindowTextW(L"文件上传中，暂时无法进行其他操作");
	fp = fopen(filename,"r");

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
		send_data = send_data + msg_header + sendbuf;
		send(s_t, send_data, strlen(send_data) + 1, 0);
	} while (retval < size(sendbuf));

	
	
	return 0;
}


afx_msg LRESULT CWSChatClientMFCDlg::OnGrpJoinMsg(WPARAM wParam, LPARAM lParam)
{
	char* ptr_header, * ptr;
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	unsigned char group_id[4],msg_type;
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
		ugroup_id.SetWindowTextW(last_group_id);
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
		if (connect(s_t, (sockaddr*)&transmmit_channel, sizeof(transmmit_channel)) == SOCKET_ERROR)
			MessageBox(L"Receive preparation failed");
		
	}
	free(ptr_header);
	return 0;
}

//列表
afx_msg LRESULT CWSChatClientMFCDlg::OnGrpListMsg(WPARAM wParam, LPARAM lParam)
{
	//type 1B|len 2B|msg_type 1B|group id 4B|itemlen 2B|id of item 8B|len 1B|username/filename
	char* ptr_header,*ptr;
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
