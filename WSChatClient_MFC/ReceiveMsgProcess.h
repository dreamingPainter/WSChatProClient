#include "Resource.h"
#include "WSChatClient_MFCdlg.h"



afx_msg LRESULT CWSChatClientMFCDlg::OnLoginMsg(WPARAM wParam, LPARAM lParam)
{
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
	return 0;
}


afx_msg LRESULT CWSChatClientMFCDlg::OnGrpQuitMsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


afx_msg LRESULT CWSChatClientMFCDlg::OnBinGetMsg(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


afx_msg LRESULT CWSChatClientMFCDlg::OnGrpListMsg(WPARAM wParam, LPARAM lParam)
{
	char* ptr;
	char buf[1024];

	ptr = (char*)(void*)lParam;

	return 0;
}
