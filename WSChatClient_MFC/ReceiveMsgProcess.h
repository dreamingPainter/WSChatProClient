#include "Resource.h"
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
		SendMessage(LOGIN_CHALLENGE_ACK,NULL,crc32);
		break;
	default:
		break;
	}

	return 0;
}


afx_msg LRESULT CWSChatClientMFCDlg::OnTxtMsg(WPARAM wParam, LPARAM lParam)
{

	return 0;
}


afx_msg LRESULT CWSChatClientMFCDlg::OnBinAckMsg(WPARAM wParam, LPARAM lParam)
{
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


afx_msg LRESULT CWSChatClientMFCDlg::OnBinGetMsg(WPARAM wParam, LPARAM lParam)
{
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
