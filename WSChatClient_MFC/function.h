# include "resource.h"
# include "WSChatClient_MFCDlg.h"
void InitResourceOfClient(HWND hwnd) {
	WSAData wsa;
	if (WSAStartup(0x101, &wsa) != 0)
		logfile << "_LINE_:Init error" << endl;
	if ((s_u = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
	{
		logfile << "_LINE_:Init error" << endl;
		MessageBox(hwnd, _T("sock UDP socket() error"), L"client", MB_OK);
	}
	if ((s_t = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		logfile << "_LINE_:Init error" << endl;
		MessageBox(hwnd, _T("sock TCP socket() error"), L"client", MB_OK);
	}
	WSAAsyncSelect(s_u, hwnd, UM_SOCK, FD_READ);
	WSAAsyncSelect(s_t, hwnd, UM_SOCK, FD_READ);



}

void ReleaseClientResource(HWND hwnd) {
	closesocket(s_t);
	closesocket(s_u);
	WSACleanup();
	return;
}