
// WSChatClient_MFCDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "WSChatClient_MFC.h"
#include "WSChatClient_MFCDlg.h"
#include "afxdialogex.h"
#include <iostream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


ofstream logfile;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWSChatClientMFCDlg 对话框



CWSChatClientMFCDlg::CWSChatClientMFCDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WSCHATCLIENT_MFC_DIALOG, pParent)
	, username_local(_T(""))
{
	/*init class value*/
	s_u = 0;//UDP socket
	s_t = 0;//TCP socket
	retval = 0;
	user_state = OFFLINE;//0不在线
	//ugroup_id.SetWindowText(0);
	send_data = _T("");


	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWSChatClientMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, USERNAME_IDC_EDIT1, username_local);
	//  DDX_Control(pDX, PUBLIC_PWD_IDC_EDIT2, pubilc_key_value);
	//  DDX_Text(pDX, PUBLIC_PWD_IDC_EDIT2, public_key_value);
	//  DDX_Text(pDX, PUBLIC_PWD_IDC_EDIT2, public_key_value);
	DDX_Text(pDX, USERNAME_IDC_EDIT1, username_local);
	DDX_Control(pDX, USER_IP_IDC_IPADDRESS1, user_ip_local);
	DDX_Control(pDX, PUBLIC_PWD_IDC_EDIT2, public_key_value);
	DDX_Control(pDX, SERVER_IP_IDC_IPADDRESS2, server_ip);
	DDX_Control(pDX, SERVER_PORT_IDC_EDIT4, server_port);
	//  DDX_Text(pDX, GROUPID_IDC_EDIT3, ugroup_id);
	DDX_Control(pDX, GROUPID_IDC_EDIT3, ugroup_id);
}

BEGIN_MESSAGE_MAP(CWSChatClientMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(SEND_IDC_BUTTON7, &CWSChatClientMFCDlg::OnBnClickedIdcButton7)
	ON_EN_CHANGE(USERNAME_IDC_EDIT1, &CWSChatClientMFCDlg::OnEnChangeIdcEdit1)
	ON_BN_CLICKED(USERINFO_IDC_BUTTON1, &CWSChatClientMFCDlg::OnBnClickedIdcButton1)
	ON_NOTIFY(IPN_FIELDCHANGED, USER_IP_IDC_IPADDRESS1, &CWSChatClientMFCDlg::OnIpnFieldchangedIpIdcIpaddress1)
	ON_BN_CLICKED(LOOP_IP_IDC_RADIO1, &CWSChatClientMFCDlg::OnBnClickedIpIdcRadio1)
	ON_EN_CHANGE(PUBLIC_PWD_IDC_EDIT2, &CWSChatClientMFCDlg::OnEnChangePwdIdcEdit2)
	ON_NOTIFY(IPN_FIELDCHANGED, SERVER_IP_IDC_IPADDRESS2, &CWSChatClientMFCDlg::OnIpnFieldchangedIpIdcIpaddress2)
//	ON_MESSAGE(GROUP_OPR, &CWSChatClientMFCDlg::OnGroupOpr)
ON_BN_CLICKED(SERVERINFO_IDC_BUTTON2, &CWSChatClientMFCDlg::OnBnClickedIdcButton2)
ON_BN_CLICKED(JOIN_GROUP_IDC_BUTTON4, &CWSChatClientMFCDlg::OnBnClickedGroupIdcButton4)
ON_BN_CLICKED(QUIT_GROUP_IDC_BUTTON3, &CWSChatClientMFCDlg::OnBnClickedGroupIdcButton3)
ON_EN_CHANGE(GROUPID_IDC_EDIT3, &CWSChatClientMFCDlg::OnEnChangeIdcEdit3)
ON_BN_CLICKED(RENEW_FILELIST_IDC_BUTTON7, &CWSChatClientMFCDlg::OnBnClickedFilelistIdcButton7)
ON_BN_CLICKED(RENEW_FRIENDLIST_IDC_BUTTON6, &CWSChatClientMFCDlg::OnBnClickedFriendlistIdcButton6)
ON_BN_CLICKED(UPLOAD_IDC_BUTTON5, &CWSChatClientMFCDlg::OnBnClickedIdcButton5)
END_MESSAGE_MAP()


// CWSChatClientMFCDlg 消息处理程序

BOOL CWSChatClientMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	public_key_value.SetPasswordChar('*');

	//logfile.open("log.txt", ios::out | ios::ate | ios::trunc);
	//logfile << "Init Dialog Success" << endl;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CWSChatClientMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CWSChatClientMFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CWSChatClientMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CWSChatClientMFCDlg::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CWSChatClientMFCDlg::OnIpnFieldchangedIpaddress1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


void CWSChatClientMFCDlg::OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


//LRESULT CAboutDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//
//	return CDialogEx::WindowProc(message, wParam, lParam);
//}


LRESULT CWSChatClientMFCDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	int wmId, wmEvent;

	switch (message)
	{
	//处理资源启动和释放等
	case WM_COMMAND:
		break;
	//
	case WM_DESTROY:
		break;
	//case UM_SOCK:
	//	break;

	//LIST_OPER * 需要一个接收的消息处理函数：接收文件和好友列表 TYPE_GRP_LIST 8
	//DOWNLOAD_FILE_OPER * 需要一个接收的消息处理函数：接收下载请求的返回报文和文件 7
	//GROUP_QUIT_OPERA * 需要一个接收的消息处理函数：退群 6
	//GROUP_JOIN_OPERA * 需要一个接收的消息处理函数：加群 5
	//PULL_FILE * 需要一个接收的消息处理函数：确定上传 4
	//MESSAGE_OPER * 需要一个接收的消息处理函数：接收短消息 TYPE_MSG_TXT 2
	//LOGIN_OPER * 需要一个接收的消息处理函数：接收登录消息 TYPE_LOGIN 1	

	default:
		break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CWSChatClientMFCDlg::OnBnClickedIdcButton7()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CWSChatClientMFCDlg::OnEnChangeIdcEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

}

/*登录*/
void CWSChatClientMFCDlg::OnBnClickedIdcButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString input_text;
	int port{};
	short int len;
	unsigned int msg_len;
	char buf_byte;
	CStringA long_data;
	char b4, b3, b2, b1,bh,bl;


	UpdateData(TRUE);
	public_key_value.GetWindowText(input_text);
	if (input_text.IsEmpty())
	{
		MessageBox(_T("公钥未填写"));
		return;
	}
	if (username_local.IsEmpty())
	{
		MessageBox(_T("请填写您的用户名！"));
		return;
	}
	if (user_ip_local.IsBlank())
	{
		//本地ip获取
		MessageBox(_T("默认使用本机ip"));
		return;
	}
	if (server_ip.IsBlank())
	{
		MessageBox(_T("未填写服务器地址"));
		return;
	}
	server_port.GetWindowText(input_text);
	if (input_text.IsEmpty())
	{
		MessageBox(_T("未填写服务器端口号"));
		return;
	}
	
	s_u = socket(AF_INET, SOCK_DGRAM, 0);
	s_t = socket(AF_INET, SOCK_STREAM, 0);
	server.sin_family = AF_INET;
	server_ip.GetAddress(server.sin_addr.S_un.S_un_b.s_b1, server.sin_addr.S_un.S_un_b.s_b2,
		server.sin_addr.S_un.S_un_b.s_b3, server.sin_addr.S_un.S_un_b.s_b4);
	server_port.GetWindowText(input_text);
	port = _ttoi(input_text);
	server.sin_port = htons(port);
	if (connect(s_t, (sockaddr*)&server, sizeof(server))<0) {
		retval = WSAGetLastError();
		logfile << "Network Error" << retval << endl;
	}
	else {
		//TYPE_LOGIN的用户的登陆报文：type|length|msg|len|username
		if (!send_data.IsEmpty())
			send_data.Empty();

		//高地址整数高位，低地址整数低位
		buf_byte = 0x01;//type
		send_data = buf_byte;//Add Packet Header
		msg_len = username_local.GetLength();
		len = 1 + 4 + msg_len;//msg_l|msg_len_l|data_l
		bh = HIBYTE(len);
		bl = LOBYTE(len);
		buf_byte = 0x01;//msg_type
		long_data = username_local;
		send_data = send_data + bl + bh + buf_byte + long_data;//MAKEWORD(b1,bn)
		sendto(s_u, send_data, sizeof(send_data), 0, (sockaddr*)&server, sizeof(server));
	}



	UpdateData(FALSE);
	
}


void CWSChatClientMFCDlg::OnIpnFieldchangedIpIdcIpaddress1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

/*用户端IP为环回地址*/
void CWSChatClientMFCDlg::OnBnClickedIpIdcRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString ip_addr;
	
	if (user_ip_local.IsBlank())
	{
		user_ip_local.SetAddress(127, 0, 0, 1);
		user_ip_local.EnableWindow(FALSE);
	}
	else
	{
		((CIPAddressCtrl*)GetDlgItem(USER_IP_IDC_IPADDRESS1))->ClearAddress();
		((CButton*)GetDlgItem(LOOP_IP_IDC_RADIO1))->SetCheck(BST_UNCHECKED);
		user_ip_local.EnableWindow(TRUE);
	}

	

}


void CWSChatClientMFCDlg::OnEnChangePwdIdcEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CWSChatClientMFCDlg::OnIpnFieldchangedIpIdcIpaddress2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


//afx_msg LRESULT CWSChatClientMFCDlg::OnGroupOpr(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}

/*服务器设置为环回地址*/
void CWSChatClientMFCDlg::OnBnClickedIdcButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (server_ip.IsBlank())
	{
		server_ip.SetAddress(127, 0, 0, 1);
		server_port.SetWindowTextW(L"55555");
		server_ip.EnableWindow(FALSE);
		server_port.EnableWindow(FALSE);
	}
	else 
	{
		server_ip.ClearAddress();
		server_port.SetWindowTextW(L"");
		server_ip.EnableWindow(TRUE);
		server_port.EnableWindow(TRUE);
	}

}


/*
* @brief 加群：有逻辑漏洞：只能点加入，或者退出，但是不想写了，现在是不退上一个就不能加新群
*/
void CWSChatClientMFCDlg::OnBnClickedGroupIdcButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	int len;
	CString group_id;
	if (s_u == 0 || user_state == 0)
	{
		MessageBox(L"未连接服务器");
		return;
	}
	else
	{
		if (last_group_id != "0")
			MessageBox(L"已在群%s,请先退出", last_group_id);
		
		// 创建报文
		if (!send_data.IsEmpty())
			send_data.Empty();
		ugroup_id.GetWindowText(group_id);
		/* 未完成：加群报文填写
		
		
		
		
		*/

		// 发送报文
		len = send_data.GetLength();
		if (send(s_u, send_data, len, 0)==SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		send_data.Empty();

	}


}

/*
* @brief 退群
*/
void CWSChatClientMFCDlg::OnBnClickedGroupIdcButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	int len;
	if (s_u == 0 || user_state == 0)
	{
		MessageBox(L"未连接,退群失败");
		return;
	}
	else
	{
		ugroup_id.SetWindowTextW(L"0");
		// 创建报文
		if (!send_data.IsEmpty())
			send_data.Empty();
		/*未完成：退群报文填写
		
		*/

		// 发送报文
		len = send_data.GetLength();
		if (send(s_u, send_data, len, 0) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		send_data.Empty();

	}
}


void CWSChatClientMFCDlg::OnEnChangeIdcEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

/*
* @brief 文件列表获取
*/
void CWSChatClientMFCDlg::OnBnClickedFilelistIdcButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	int len;
	if (s_u == 0 || user_state == 0)
	{
		MessageBox(L"未连接,无法获取文件列表");
		return;
	}
	else
	{
		// 创建报文
		if (!send_data.IsEmpty())
			send_data.Empty();
		/*未完成：获取文件列表报文填写

		*/

		// 发送报文
		len = send_data.GetLength();
		if (send(s_u, send_data, len, 0) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		send_data.Empty();

	}

}


void CWSChatClientMFCDlg::OnBnClickedFriendlistIdcButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	int len;
	if (s_u == 0 || user_state == 0)
	{
		MessageBox(L"未连接,无法获取好友列表");
		return;
	}
	else
	{
		// 创建报文
		if (~send_data.IsEmpty())
			send_data.Empty();
		/*未完成：获取文件列表报文填写

		*/

		// 发送报文
		len = send_data.GetLength();
		if (send(s_u, send_data, len, 0) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		send_data.Empty();

	}

}

/*
* @brief 上传文件请求
*/
void CWSChatClientMFCDlg::OnBnClickedIdcButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	int len;
	CString path;
	if (s_t == 0 || user_state == 0)
	{
		MessageBox(L"未连接,无法获取好友列表");
		return;
	}
	else
	{	
		// 创建上传请求报文
		if (~send_data.IsEmpty())
			send_data.Empty();
		/*未完成：发送文件
			
		*/

		// 发送报文
		len = send_data.GetLength();
		if (send(s_u, send_data, len, 0) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		send_data.Empty();

	}
}
