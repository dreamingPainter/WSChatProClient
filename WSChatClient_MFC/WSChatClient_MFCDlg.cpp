// WSChatClient_MFCDlg.cpp: 实现文件
//
#include "pch.h"
#include "WSChatClient_MFC.h"
#include "WSChatClient_MFCDlg.h"

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ofstream logfile;
SOCKET s_u;
SOCKET s_t;

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
	user_id = 0;


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
	DDX_Control(pDX, FILEPATH_IDC_EDIT2, file_path);
	DDX_Control(pDX, SELECT_FILE_RECV_COMBO4, file_receiver);
	DDX_Control(pDX, STATEBOX, state_of_client);
	DDX_Control(pDX, SHOW_TXT, show_txt_buf);
	DDX_Control(pDX, IDC_EDIT8, sendt_txt_buf);
	DDX_Control(pDX, SELECT_CHAT_RECV_COMBO3, message_receiver);
	DDX_Control(pDX, PRIVATE_PWD_IDC_EDIT3, private_pwd);
	DDX_Control(pDX, MEMBER_IDC_LIST2, member_list_view);
	DDX_Control(pDX, FILE_IDC_LIST1, file_list_view);
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
ON_CBN_SELCHANGE(SELECT_FILE_RECV_COMBO4, &CWSChatClientMFCDlg::OnCbnSelchangeFileRecvCombo4)
ON_EN_CHANGE(FILEPATH_IDC_EDIT2, &CWSChatClientMFCDlg::OnEnChangeIdcEdit2)
ON_BN_CLICKED(DOWNLOAD_IDC_BUTTON6, &CWSChatClientMFCDlg::OnBnClickedIdcButton6)
ON_MESSAGE(UM_SOCK, &CWSChatClientMFCDlg::OnUmSock)
ON_MESSAGE(LOGIN_MSG, &CWSChatClientMFCDlg::OnLoginMsg)
ON_MESSAGE(TXT_MSG, &CWSChatClientMFCDlg::OnTxtMsg)
ON_MESSAGE(BIN_ACK_MSG, &CWSChatClientMFCDlg::OnBinAckMsg)
ON_MESSAGE(GRP_JOIN_MSG, &CWSChatClientMFCDlg::OnGrpJoinMsg)
ON_MESSAGE(GRP_QUIT_MSG, &CWSChatClientMFCDlg::OnGrpQuitMsg)
ON_MESSAGE(BIN_GET_MSG, &CWSChatClientMFCDlg::OnBinGetMsg)
ON_MESSAGE(GRP_LIST_MSG, &CWSChatClientMFCDlg::OnGrpListMsg)
ON_MESSAGE(LOGIN_CHALLENGE_ACK, &CWSChatClientMFCDlg::OnLoginChallengeAck)
ON_EN_CHANGE(IDC_EDIT7, &CWSChatClientMFCDlg::OnEnChangeEdit7)
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
	private_pwd.SetPasswordChar('*');
	//宽400 高142 member_list_view
	//宽326 高326 file_list_view
	member_list_view.InsertColumn(0, _T("成员ID"), LVCFMT_LEFT, 100);
	member_list_view.InsertColumn(1, _T("成员昵称"), LVCFMT_LEFT, 300);
	file_list_view.InsertColumn(0, _T("文件ID"), LVCFMT_LEFT, 100);
	file_list_view.InsertColumn(1, _T("文件名"), LVCFMT_LEFT, 226);
	state_of_client.EnableWindow(FALSE);


	logfile.open("log.txt", ios::out | ios::ate | ios::trunc);
	logfile << "Init Dialog Success" << endl;

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
	int s,addr_len;
	unsigned int type;
	HWND hwnd;
	hwnd = FindWindowA("CWSChatClientMFCDlg", NULL);

	switch (message)
	{
	//处理资源启动和释放等
	case WM_CREATE:
		InitResourceOfClient(hwnd);
		break;
	case WM_DESTROY:
		ReleaseClientResource(hwnd);
		PostQuitMessage(0);
		break;
	case UM_SOCK:
		s = (SOCKET)wParam;
		wmEvent = LOWORD(lParam);

		switch (wmEvent)
		{
		case FD_READ:
			if (s == s_u)
			{	
				char *ptr;
				addr_len = sizeof(server);
				retval = recvfrom(s_u, recv_buf, sizeof(recv_buf), 0, (sockaddr*)&server, &addr_len);
				type = recv_buf[0] & 0x01;
				ptr = (char*)malloc(strlen(recv_buf) + 1);
				strcpy(ptr,recv_buf);
				//用Postmsg需要解决data如何传递的问题（一堆buf），用SendMSG需要考虑阻塞的问题，考虑用指针
				//注意释放
				switch (type) {
				case TYPE_LOGIN:
					PostMessage(LOGIN_MSG,NULL,(LPARAM)(void*)&*ptr);
					break;
				case TYPE_MSG_TXT:
					PostMessage(TXT_MSG,NULL, (LPARAM)(void*)&*ptr);
					break;
				case TYPE_MSG_BIN_ACK:
					PostMessage(BIN_ACK_MSG, NULL, (LPARAM)(void*)&*ptr);
					break;
				case TYPE_GRP_JOIN:
					PostMessage(GRP_JOIN_MSG,NULL, (LPARAM)(void*)&*ptr);
					break;
				case TYPE_GRP_QUIT:
					PostMessage(GRP_QUIT_MSG, NULL, (LPARAM)(void*)&*ptr);
					break;
				case TYPE_BIN_GET:
					PostMessage(BIN_GET_MSG, NULL, (LPARAM)(void*)&*ptr);
					break;
				case TYPE_GRP_LST:
					PostMessage(GRP_LIST_MSG, NULL, (LPARAM)(void*)&*ptr);
					break;
				default:
					free(ptr);
					memset(recv_buf,0,sizeof(recv_buf));
					break;
				}

				//LIST_OPER * 需要一个接收的消息处理函数：接收文件和好友列表 TYPE_GRP_LIST 8
				//DOWNLOAD_FILE_OPER * 需要一个接收的消息处理函数：接收下载请求的返回报文和文件 7
				//GROUP_QUIT_OPERA * 需要一个接收的消息处理函数：退群 6
				//GROUP_JOIN_OPERA * 需要一个接收的消息处理函数：加群 5
				//PULL_FILE * 需要一个接收的消息处理函数：确定上传 4
				//MESSAGE_OPER * 需要一个接收的消息处理函数：接收短消息 TYPE_MSG_TXT 2
				//LOGIN_OPER * 需要一个接收的消息处理函数：接收登录消息 TYPE_LOGIN 1	
			}
			else if (s == s_t)
			{//写这算了
				uint64_t crc64 = 0;
				uint64_t file_len = 0;
				FILE *file;
				char *ptr;
				char file_crc64;
				//只能拿crc64的值当文件名了，，或者随便取一个，，，者只能下载一个文件，下完要移走改名✔，，或者在维护的文件列表里查
				//?如何知道文件到底发没发完，这个len是整个原始文件的字节长度，还是报文里数据的字节数目✔，还是比特数目
 				retval = recv(s_t, recv_buf, sizeof(recv_buf), 0);
				ptr = recv_buf+3;
				file = fopen("Download", "ab+");
				crc64 = 0x0000000000000000;//8 byte 16F


				for (int count = 0; count < 8; count++)
				{
					crc64 = (crc64 | (*ptr)) << 8;
					ptr++;
				}
				for (int count = 0; count < 8; count++)
				{
					file_len = (file_len | (*ptr)) << 8;
					ptr++;
				}
				fwrite(ptr, sizeof(char), file_len, file);
			}
			memset(recv_buf, 0, sizeof(recv_buf));
			break;
		case FD_CLOSE:
			break;

		default:
			break;
		}

	break;

	default:
		break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}

/*发送短消息*/
void CWSChatClientMFCDlg::OnBnClickedIdcButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	uint32_t fromID, toID, ID_buf;
	uint16_t len,msg_len;
	char buf_byte;
	CStringA input_txt_A,recv_id_A;
	CString input_txt,recv_id;
	send_data.Empty();
	//获取发送对象
	fromID = user_id;
	message_receiver.GetLBText(message_receiver.GetCurSel(), recv_id);
	recv_id = recv_id_A;
	toID = atoi(recv_id_A);
	//获取数据和数据长度
	sendt_txt_buf.GetWindowText(input_txt);
	input_txt_A = input_txt;
	msg_len = input_txt_A.GetLength();
	len = 8 + 2 + msg_len;
	//加报头
	buf_byte = 0x02;
	send_data = buf_byte;
	//0xFFFF 2B len
	bit16_data_into_buf(len, &send_data);
	//0xFFFFFFFF 4B fromid
	bit32_data_into_buf(toID, &send_data);
	//4B toid
	bit32_data_into_buf(toID,&send_data);
	//2B msg_len
	bit16_data_into_buf(msg_len, &send_data);

	send_data = send_data + input_txt_A;
	if (sendto(s_u, send_data, sizeof(send_data), 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		logfile << "_LINE_:send error" << endl;
	}

	sendt_txt_buf.Clear();
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
	char bh,bl;


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

	server.sin_family = AF_INET;
	server_ip.GetAddress(server.sin_addr.S_un.S_un_b.s_b1, server.sin_addr.S_un.S_un_b.s_b2,
		server.sin_addr.S_un.S_un_b.s_b3, server.sin_addr.S_un.S_un_b.s_b4);
	server_port.GetWindowText(input_text);
	port = _ttoi(input_text);
	server.sin_port = htons(port);
	/*if (connect(s_t, (sockaddr*)&server, sizeof(server))<0) {
		retval = WSAGetLastError();
		logfile << "Network Error" << retval << endl;
	}*/
	//else {
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
		send_data = send_data + bh + bl + buf_byte + long_data;//MAKEWORD(b1,bn)
		if (sendto(s_u, send_data, sizeof(send_data), 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
	//}
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
//}de

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
	CString group_id;
	char buf_byte;
	CString input_text;
	int port{};
	uint32_t group_id_uint;
	short int len;
	unsigned int msg_len;

	if (s_u == 0 || user_state == 0)
	{
		MessageBox(L"未连接服务器");
		return;
	}
	else
	{
		if (last_group_id != 0)
			MessageBox(L"已在群,请先退出");
		
		// 创建报文
		if (!send_data.IsEmpty())
			send_data.Empty();
		ugroup_id.GetWindowText(group_id);

		if (!send_data.IsEmpty())
			send_data.Empty();

		//高地址整数高位，低地址整数低位
		buf_byte = 0x05;//type
		send_data = buf_byte;//Add Packet Header
		len = 4 ;//type|len|data(group_id)
		bit16_data_into_buf(len, &send_data);
		group_id_uint = atoi(CStringA(group_id));
		bit32_data_into_buf(group_id_uint, &send_data);

		sendto(s_u, send_data, sizeof(send_data), 0, (sockaddr*)&server, sizeof(server));

		// 发送报文
		len = send_data.GetLength();
		if (sendto(s_u, send_data, len,0, (sockaddr*)&server,sizeof(server))==SOCKET_ERROR)
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
	CString group_id;

	CString input_text;
	int port{};
	short int len;
	unsigned int msg_len;
	char buf_byte;
	CStringA long_data;

	if (s_u == 0 || user_state == 0)
	{
		MessageBox(L"未连接,退群失败");
		return;
	}
	else
	{
		ugroup_id.GetWindowText(group_id);
		// 创建报文
		if (!send_data.IsEmpty())
			send_data.Empty();
		
		//高地址整数高位，低地址整数低位
		buf_byte = 0x06;//type
		send_data = buf_byte;//Add Packet Header
		len = 4;//type|len|data(group_id)
		

		// 发送报文
		len = send_data.GetLength();
		if (sendto(s_u, send_data, len, 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
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
	CString group_id;

	CString input_text;
	int port{};
	short int len;
	unsigned int msg_len;
	char buf_byte;
	CStringA long_data;
	char bh, bl;
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

		buf_byte = 0x08;//type
		send_data = buf_byte;//Add Packet Header
		len = 4;//type|len|data(group_id)
		bh = HIBYTE(len);
		bl = LOBYTE(len);
		long_data = group_id;
		send_data = send_data + bh + bl + long_data;//MAKEWORD(b1,bn)


		// 发送报文
		len = send_data.GetLength();
		if (send(s_u, send_data, len, 0) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		send_data.Empty();

	}

}

/*群成员列表*/
void CWSChatClientMFCDlg::OnBnClickedFriendlistIdcButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CString group_id;

	CString input_text;
	int port{};
	short int len;
	unsigned int msg_len;
	char buf_byte;
	CStringA long_data;
	char bh, bl;
	if (s_u == 0 || user_state == 0)
	{
		MessageBox(L"未连接,无法获取好友列表");
		return;
	}
	else
	{
		// 创建报文
		if (!send_data.IsEmpty())
			send_data.Empty();

		buf_byte = 0x08;//type
		send_data = buf_byte;//Add Packet Header
		len = 4;//type|len|data(group_id)
		bh = HIBYTE(len);
		bl = LOBYTE(len);
		long_data = group_id;
		send_data = send_data + bh + bl + long_data;//MAKEWORD(b1,bn)

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
	int pos;
	CStringA filename;

	CString input_text;
	int port{};
	short int len;
	char buf_byte;
	CStringA long_data;
	char bh, bl;
	short int file_receiver_id;

	crc64_of_file = 0;

	if (s_t == 0 || user_state == 0)
	{
		MessageBox(L"未连接,无法上传文件");
		return;
	}
	else
	{	
		//读取文件名/传输对象
		file_path.GetWindowText(input_text);
		pos = input_text.ReverseFind('\\');
		filename = input_text.Right(input_text.GetLength()-pos+1);//文件名
		file_receiver.GetWindowText(input_text);
		long_data = input_text;
		file_receiver_id = atoi(long_data);//传输对象ID

		// 创建上传请求报文
		if (!send_data.IsEmpty())
			send_data.Empty();

		//高地址整数高位，低地址整数低位
		buf_byte = 0x03;//type
		send_data = buf_byte;//Add Packet Header

		/*懒得封装成函数了*/
		len = 4+4+8+2+filename.GetLength();//type|len|from ID 4B|to ID 4B|crc 64 8B|len 2B|file name|
		bh = HIBYTE(len);
		bl = LOBYTE(len);
		send_data = send_data + bh + bl ;//MAKEWORD(b1,bn)
		bh = HIBYTE(user_id);
		bl = LOBYTE(user_id);
		send_data = send_data + bh + bl;
		bh = HIBYTE(file_receiver_id);
		bl = LOBYTE(file_receiver_id);
		send_data = send_data + bh + bl;
		/*CRC*/
		len = filename.GetLength();
		bh = HIBYTE(len);
		bl = LOBYTE(len);
		send_data = send_data + bh + bl + filename;//MAKEWORD(b1,bn)


		// 发送报文
		len = send_data.GetLength();
		if (sendto(s_u, send_data, len, 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		send_data.Empty();

	}
}


void CWSChatClientMFCDlg::OnCbnSelchangeFileRecvCombo4()
{
	// TODO: 在此添加控件通知处理程序代码


}


void CWSChatClientMFCDlg::OnEnChangeIdcEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CWSChatClientMFCDlg::OnBnClickedIdcButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	int pos;
	CStringA filename;

	CString input_text;
	int port{};
	short int len;
	char buf_byte;
	CStringA long_data;
	char bh, bl;
	short int file_receiver_id;

	if (s_t == 0 || user_state == 0)
	{
		MessageBox(L"未连接,无法上传文件");
		return;
	}
	else
	{
		//读取文件名/传输对象
		file_path.GetWindowText(input_text);
		pos = input_text.ReverseFind('\\');
		filename = input_text.Right(input_text.GetLength() - pos + 1);//文件名
		file_receiver.GetWindowText(input_text);
		long_data = input_text;
		file_receiver_id = atoi(long_data);//传输对象ID

		// 创建上传请求报文
		if (!send_data.IsEmpty())
			send_data.Empty();

		//高地址整数高位，低地址整数低位
		buf_byte = 0x03;//type
		send_data = buf_byte;//Add Packet Header

		/*懒得封装成函数了*/
		len = 4 + 4 + 8 + 2 + filename.GetLength();//type|len|from ID 4B|to ID 4B|crc 64 8B|len 2B|file name|
		bh = HIBYTE(len);
		bl = LOBYTE(len);
		send_data = send_data + bh + bl;//MAKEWORD(b1,bn)
		bh = HIBYTE(user_id);
		bl = LOBYTE(user_id);
		send_data = send_data + bh + bl;
		bh = HIBYTE(file_receiver_id);
		bl = LOBYTE(file_receiver_id);
		send_data = send_data + bh + bl;
		/*CRC*/
		len = filename.GetLength();
		bh = HIBYTE(len);
		bl = LOBYTE(len);
		send_data = send_data + bh + bl + filename;//MAKEWORD(b1,bn)


		// 发送报文
		len = send_data.GetLength();
		if (sendto(s_u, send_data, len, 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		send_data.Empty();

	}
}


afx_msg LRESULT CWSChatClientMFCDlg::OnUmSock(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

void InitResourceOfClient(HWND hwnd) {
	WSAData wsa;
	if(WSAStartup(0x101, &wsa)!=0)
		logfile << "_LINE_:Init error" << endl;
	if ((s_u = socket(AF_INET, SOCK_DGRAM, 0)) == SOCKET_ERROR)
	{
		logfile << "_LINE_:Init error" << endl;
		MessageBox(hwnd, _T("sock UDP socket() error"),L"client",MB_OK);
	}
	if ((s_t = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
		logfile << "_LINE_:Init error" << endl;
		MessageBox(hwnd, _T("sock TCP socket() error"),L"client",MB_OK);
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


afx_msg LRESULT CWSChatClientMFCDlg::OnLoginChallengeAck(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加控件通知处理程序代码
	CString group_id;
	CString input_text;
	int port{};
	short int len;
	char buf_byte;
	CStringA long_data;
	char bh, bl;
	if (s_u == 0 || user_state == 0)
	{
		MessageBox(L"未连接,无法获取好友列表");
		return 0;
	}
	else
	{
		// 创建报文
		if (!send_data.IsEmpty())
			send_data.Empty();

		buf_byte = 0x08;//type
		send_data = buf_byte;//Add Packet Header
		len = 4;//type|len|data(group_id)
		bh = HIBYTE(len);
		bl = LOBYTE(len);
		long_data = group_id;
		send_data = send_data + bh + bl + long_data;//MAKEWORD(b1,bn)

		// 发送报文
		len = send_data.GetLength();
		if (send(s_u, send_data, len, 0) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		send_data.Empty();

	}
	return 0;
}


afx_msg LRESULT CWSChatClientMFCDlg::OnLoginMsg(WPARAM wParam, LPARAM lParam)
{
	char* ptr_header, * ptr, buf_byte;
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
		crc32_value = crc32_function(buf, input_text.GetLength(), sum32);

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
	char* ptr_header, * ptr, show_buf[2048];
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	uint32_t fromID, toID;
	uint16_t len;
	CString show_txt;

	ptr = ptr + 3;

	for (int i = 0; i < 4; i++)
	{
		fromID = *ptr | 0x00;
		fromID = fromID << 8;
		ptr++;
	}
	for (int i = 0; i < 4; i++)
	{
		toID = *ptr | 0x00;
		toID = toID << 8;
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
			retval_file_len = retval_file_len >> 8;
			msg_header = msg_header + buf_byte;
		};
		len = retval + 8;
		for (int i = 0; i < 8; i++)//数据长度
		{
			buf_byte = retval_file_len | 0x00;
			retval_file_len = retval_file_len >> 8;
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
	char* ptr_header, * ptr;
	char buf[256];
	ofstream fp;
	ifstream fp_in;
	unsigned char type;
	uint32_t group_id,line;
	uint16_t item_num,pos;
	CString item;

	line = 1;
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	ptr += 3;
	//type
	type = *(ptr);
	ptr++;
	//group_id
	group_id = ntohl(*(uint32_t*)ptr);
	//item_len
	ptr += 4;
	item_num = *(uint16_t*)ptr;
	ptr += 2;
	//report：https://blog.csdn.net/to_Baidu/article/details/61428276
	if (group_id == last_group_id)
	{
		switch (type)
		{
		case 0x00://member_list
			fp.open("member_list.txt", ios::trunc | ios::out | ios::in);
			for (int i = 0; i < item_num; i++)
			{	
				uint64_t member_id = ntohll(*(uint64_t*)ptr);
				ptr += 8;
				uint8_t len = *(uint8_t*)ptr;
				ptr++;
				for (int count = 0; count < len; count++)
				{
					buf[count] = *ptr;
					ptr++;
				}
				fp << member_id << "\t\t" << buf << endl;
				memset(buf, 0, sizeof(buf));
			}
			fp.close();

			member_list_view.DeleteAllItems();
			fp_in.open("member_list.txt", ios::in);
			while (fp_in.getline(buf, 26))
			{
				item = buf;
				pos = item.ReverseFind('\t');
				CString s_member_id = item.Left(pos);
				CString s_member_name = item.Right(pos);
				member_list_view.InsertItem(line,s_member_id);
				member_list_view.SetItemText(line, 1, s_member_name);
				memset(buf, 0, 256);
				line++;
			}
			fp_in.close();
			break;
		case 0x01://file_list
			fp.open("file_list.txt", ios::trunc | ios::out| ios::in);
			for (int i = 0; i < item_num; i++)
			{
				uint64_t file_id = ntohll(*(uint64_t*)ptr);
				ptr += 8;
				uint8_t len = *(uint8_t*)ptr;
				ptr++;
				for (int count = 0; count < len; count++)
				{
					buf[count] = *ptr;
					ptr++;
				}
				fp << file_id << "\t\t" << buf << endl;
				memset(buf, 0, sizeof(buf));
			}
			file_list_view.DeleteAllItems();
			fp.close();

			file_list_view.DeleteAllItems();
			fp_in.open("file_list.txt", ios::in);
			while (fp_in.getline(buf, 26))
			{
				item = buf;
				pos = item.ReverseFind('\t');
				CString s_file_id = item.Left(pos);
				CString s_file_name = item.Right(pos);
				file_list_view.InsertItem(line,s_file_id);
				file_list_view.SetItemText(line, 1, s_file_name);
				memset(buf, 0, 256);
				line++;
			}
			fp_in.close();
			break;
		default:
			break;
		}
	}
	else MessageBox(L"Get wrong List!");
	free(ptr);
	return 0;
}



void CWSChatClientMFCDlg::OnEnChangeEdit7()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}