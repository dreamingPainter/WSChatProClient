// WSChatClient_MFCDlg.cpp: 实现文件
//
#include "pch.h"
#include "WSChatClient_MFC.h"
#include "WSChatClient_MFCDlg.h"

using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <format>

ofstream logfile;
SOCKET s_u;
SOCKET s_t;
struct sockaddr_in server, client, server_tcp, transmmit_channel;

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
	user_state = ONLINE;//0不在线
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
	DDX_Control(pDX, SERVER_IP_IDC_IPADDRESS2, server_ip);
	DDX_Control(pDX, SERVER_PORT_IDC_EDIT4, server_port);
	//  DDX_Text(pDX, GROUPID_IDC_EDIT3, ugroup_id);
	DDX_Control(pDX, GROUPID_IDC_EDIT3, ugroup_id);
	//  DDX_Control(pDX, FILEPATH_IDC_EDIT2, file_path);
	DDX_Control(pDX, SELECT_FILE_RECV_COMBO4, file_receiver);
	DDX_Control(pDX, STATEBOX, state_of_client);
	DDX_Control(pDX, SHOW_TXT, show_txt_buf);
	DDX_Control(pDX, IDC_EDIT8, sendt_txt_buf);
	DDX_Control(pDX, SELECT_CHAT_RECV_COMBO3, message_receiver);
	DDX_Control(pDX, PRIVATE_PWD_IDC_EDIT3, private_pwd);
	DDX_Control(pDX, MEMBER_IDC_LIST2, member_list_view);
	DDX_Control(pDX, FILE_IDC_LIST1, file_list_view);
	//  DDX_Control(pDX, FILEPATH_IDC_EDIT2, file_PathOrID);
	DDX_Control(pDX, FILEPATH_IDC_EDIT2, file_nameOrId);
	DDX_Control(pDX, IDC_EDIT1, file_state);
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
ON_CBN_SELCHANGE(SELECT_CHAT_RECV_COMBO3, &CWSChatClientMFCDlg::OnCbnSelchangeChatRecvCombo3)
ON_EN_CHANGE(STATEBOX, &CWSChatClientMFCDlg::OnEnChangeStatebox)
ON_MESSAGE(BIN_ARRIVE_MSG, &CWSChatClientMFCDlg::OnBinArriveMsg)
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
	private_pwd.SetPasswordChar('*');


	//宽400 高142 member_list_view
	//宽326 高326 file_list_view
	//消息接收对象初始化
	message_receiver.InsertString(0, L"Server");
	//文件接收对象初始化
	file_receiver.InsertString(0, L"Server");
	//成员列表初始化
	member_list_view.InsertColumn(0, _T("成员ID"), LVCFMT_LEFT, 100);
	member_list_view.InsertColumn(1, _T("成员昵称"), LVCFMT_LEFT, 300);
	member_list_view.InsertItem(0, L"4294967295");
	member_list_view.SetItemText(0, 1, L"Server");
	//文件列表初始化
	file_list_view.InsertColumn(0, _T("文件ID"), LVCFMT_LEFT, 100);
	file_list_view.InsertColumn(1, _T("文件名"), LVCFMT_LEFT, 226);


	//初始化列表测试
	uint32_t id = 0, i = 1;
	char name[256], id_c[128];
	//群成员列表初始化测试
	FILE* fp = fopen("member_list.txt", "r");
	while (!feof(fp))
	{
		fscanf(fp, "%d\t%s", &id, name);
		CString name_C,id_C;
		CStringA name_CA = name;
		CStringA id_CA;
		name_C = name_CA;
		_itoa(id, id_c, 10);
		id_CA = id_c;
		id_C = id_CA;
		message_receiver.InsertString(i,name_C);
		file_receiver.InsertString(i, name_C);
		member_list_view.InsertItem(i, id_C);
		member_list_view.SetItemText(i, 1, name_C);
		i++;
	}
	//文件列表初始化测试
	FILE* fp_f = fopen("file_list.txt", "r");
	i = 0;
	while (!feof(fp_f))
	{
		fscanf(fp_f, "%d\t%s", &id, name);
		CString name_C, id_C;
		CStringA name_CA = name;
		CStringA id_CA;
		name_C = name_CA;
		_itoa(id, id_c, 10);
		id_CA = id_c;
		id_C = id_CA;
		file_list_view.InsertItem(i, id_C);
		file_list_view.SetItemText(i, 1, name_C);
		i++;
	}

	state_of_client.EnableWindow(FALSE);
	last_group_id = 0;
	user_id = 2;
	send_data.Empty();
	memset(send_buf, 0, 1024);
	/*char byte_buf = 0x01;
	send_data = byte_buf;
	send_data = bit16_data_into_buf(0x0203, send_data);

	strcpy(send_buf,send_data);*/



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
	hwnd = FindWindowA(NULL, "MFC_WSChatClient");

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
				type = recv_buf[0] & 0xFF;
				//uint16_t len = ntohs(*((uint16_t*)&recv_buf[1]));
				ptr = (char*)malloc(retval);
				memcpy(ptr,recv_buf,retval);
			
				//用Postmsg需要解决data如何传递的问题（一堆buf），用SendMSG需要考虑阻塞的问题，考虑用指针
				//注意释放
				switch (type) {
				case TYPE_LOGIN:
					SendMessage(LOGIN_MSG,NULL,(LPARAM)(void*)&*ptr);
					break;
				case TYPE_MSG_TXT:
					SendMessage(TXT_MSG,NULL, (LPARAM)(void*)&*ptr);
					break;
				case TYPE_MSG_BIN:
					SendMessage(BIN_ARRIVE_MSG, NULL, (LPARAM)(void*)&*ptr);
					break;
				case TYPE_MSG_BIN_ACK:
					SendMessage(BIN_ACK_MSG, NULL, (LPARAM)(void*)&*ptr);
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
					SendMessage(GRP_LIST_MSG, NULL, (LPARAM)(void*)&*ptr);
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
	uint32_t fromID, toID;
	uint16_t len,msg_len;
	char buf_byte,flag;
	CStringA input_txt_A,recv_id_A;
	CString input_txt,recv_name,find_name;
	send_data.Empty();
	//获取发送对象
	flag = 0;
	toID = 0;
	fromID = user_id;
	if (message_receiver.GetCurSel() < 0)
		MessageBox(L"你要给谁发消息？");
	else
		message_receiver.GetLBText(message_receiver.GetCurSel(), recv_name);


	for (int id_index=0; id_index < member_list_view.GetItemCount(); id_index++)
	{
		
		find_name = member_list_view.GetItemText(id_index, 1);
		if (find_name == recv_name)
		{
			char* ptr;
			recv_id_A = member_list_view.GetItemText(id_index, 0);
			toID = strtoul(recv_id_A,&ptr,10);
			flag = 1;
		}
	}
	if (flag == 0)
	{
		MessageBox(L"查无此人\n可尝试刷新列表");
		return;
	}
	//获取数据和数据长度
	sendt_txt_buf.GetWindowText(input_txt);
	input_txt_A = input_txt;
	msg_len = input_txt_A.GetLength();
	len = 8 + 2 + msg_len;
	//加报头
	buf_byte = 0x02;
	send_buf[0] = buf_byte;
	//0xFFFF 2B len
	*((uint16_t*)&send_buf[1]) = htons(len);
	//0xFFFFFFFF 4B fromid
	*((uint32_t*)&send_buf[3]) = htonl(fromID);
	//4B toid
	*((uint32_t*)&send_buf[7]) = htonl(toID);
	//2B msg_len
	*((uint16_t*)&send_buf[11]) = htons(msg_len);
	strcpy(&send_buf[13], input_txt_A.GetBuffer());
	if (sendto(s_u, send_buf, len+3, 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		logfile << "_LINE_:send error" << endl;
	}
	input_txt = "Me : " + input_txt_A;
	input_txt += "\r\n";
	show_txt_buf.SetSel(show_txt_buf.GetWindowTextLength(), show_txt_buf.GetWindowTextLengthW());
	show_txt_buf.ReplaceSel(input_txt);
	show_txt_buf.LineScroll(show_txt_buf.GetLineCount());
	sendt_txt_buf.SetWindowTextW(L"");
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
	CString x;


	UpdateData(TRUE);
	private_pwd.GetWindowText(input_text);
	if (input_text.IsEmpty())
	{
		MessageBox(_T("密码未填写"));
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
		char send_data_C[256],*ptr;
		int i = 0;
		buf_byte = 0x01;//type
		send_data_C[i++] = buf_byte;//Add Packet Header
		msg_len = username_local.GetLength();
		
		len = 2 + msg_len;//type lB|len 2B|msg 1B|1B
		send_data_C[i++] = HIBYTE(len);
		send_data_C[i++] = LOBYTE(len);
		buf_byte = 0x00;//msg_type

		uint8_t msg_len_s = msg_len;
		send_data_C[i++] = buf_byte;
		send_data_C[i++] = msg_len_s;
		//send_data_C[i++] = HIBYTE(msg_len);
		//send_data_C[i++] = LOBYTE(msg_len);

		long_data = username_local;
		ptr = (char*)long_data.GetBuffer();
		strcpy(&send_data_C[i], ptr);

		if (sendto(s_u, send_data_C, sizeof(send_data_C), 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
	//}
		long_data.ReleaseBuffer();
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
		server_port.SetWindowTextW(L"6666");
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
	CStringA group_id_A;
	uint32_t group_id_u;
	short int len;
	unsigned int msg_len;

	if (s_u == 0 || user_state == 0)
	{
		MessageBox(L"未连接服务器");
		return;
	}
	else
	{
		char* ptr;
		ugroup_id.GetWindowText(group_id);
		group_id_A = group_id;
		group_id_u = strtol(group_id_A, &ptr, 10);
		if (last_group_id == group_id_u)
		{
			MessageBox(L"已在此群");
			return;
		}
		else if (last_group_id != 0)
		{
			MessageBox(L"请先退出当前群聊");
			return;
		}
		
		// 创建报文
		if (!send_data.IsEmpty())
			send_data.Empty();
		memset(send_buf, 0, SEND_BUF_SIZE);

		//填充报文
		*send_buf = 0x05;
		*(uint16_t*)(send_buf + 1) = htons(4);


		*(uint32_t*)(send_buf + 3) = htonl(group_id_u);

		//发送报文
		len = 7;
		if (sendto(s_u, send_buf, len,0, (sockaddr*)&server,sizeof(server))==SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		memset(send_buf, 0, SEND_BUF_SIZE);
	}


}

/*
* @brief 退群
*/
void CWSChatClientMFCDlg::OnBnClickedGroupIdcButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString group_id;
	CStringA group_id_A;
	uint32_t group_id_u;
	short int len;
	unsigned int msg_len;

	if (s_u == 0 || user_state == 0)
	{
		MessageBox(L"未连接服务器");
		return;
	}
	else
	{
		if (last_group_id == 0)
		{
			MessageBox(L"未加入群聊");
			return;
		}
		// 创建报文
		if (!send_data.IsEmpty())
			send_data.Empty();
		memset(send_buf, 0, SEND_BUF_SIZE);

		//填充报文
		*send_buf = 0x06;
		*(uint16_t*)(send_buf + 1) = htons(4);
		*(uint32_t*)(send_buf + 3) = htonl(last_group_id);

		//发送报文
		len = 7;
		if (sendto(s_u, send_buf, len, 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
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
	char *p = send_buf;
	CString input_text;
	int port{};
	short int len;
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

		*p = 0x08;//type
		*(uint16_t*)(p + 1) = htons(5);//type|len|msg_type|data(group_id)
		*(p + 3) = 0x01;
		*(uint32_t*)(p + 4) = htons(last_group_id);

		// 发送报文
		len = 8;
		if (sendto(s_u, send_buf, len, 0, (sockaddr*)&server, sizeof(sockaddr)) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		send_data.Empty();

	}

}

/*
@brief 群成员列表获取
*/
void CWSChatClientMFCDlg::OnBnClickedFriendlistIdcButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	short int len;
	char buf_byte,*p;
	memset(send_buf, 0, sizeof(send_buf));
	p = send_buf;
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

		*p = 0x08;//type
		*(uint16_t*)(p + 1) = htons(5);//type|len|msg_type|data(group_id)
		*(p + 3) = 0x00;
		*(uint32_t*)(p + 4) = htons(last_group_id);

		// 发送报文
		len = 8;
		if (sendto(s_u, send_buf, len, 0,(sockaddr*)&server,sizeof(sockaddr)) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		memset(send_buf, 0, 1024);

	}

}

/*
* @brief 上传文件请求
*/
void CWSChatClientMFCDlg::OnBnClickedIdcButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	uint32_t fromID, toID;
	CString filename,file_recver,find_name;
	CStringA recv_id_A,filename_A;
	short int len;
	char flag;
	FILE *fp;

	//初始化
	crc64_of_file = 0;
	flag = 0;

	if (s_u == 0 || user_state == 0)
	{
		MessageBox(L"未连接,无法上传文件");
		return;
	}
	else
	{	
		//读文件名
		file_nameOrId.GetWindowTextW(filename);
		filename_A = filename;
		if (fopen(filename_A, "r") == NULL)
		{
			MessageBox(L"文件不存在，请检查文件名或者文件位置");
			return;
		}

		//获取传输对象ID : 其实可以自己刷新一下
		if (file_receiver.GetCurSel() < 0)
			MessageBox(L"你要给谁传文件？");
		else
			file_receiver.GetLBText(file_receiver.GetCurSel(), file_recver);
		for (int id_index = 0; id_index < member_list_view.GetItemCount(); id_index++)
		{
			find_name = member_list_view.GetItemText(id_index, 1);
			if (find_name == file_recver)
			{
				char* ptr;
				recv_id_A = member_list_view.GetItemText(id_index, 0);
				toID = strtoul(recv_id_A, &ptr, 10);
				flag = 1;
				break;
			}
		}
		if (flag == 0)
		{
			MessageBox(L"查无此人，可尝试刷新列表");
			return ;
		}

		// 创建上传请求报文
		if (!send_data.IsEmpty())
			send_data.Empty();
		memset(send_buf, 0, SEND_BUF_SIZE);
		/*填充报文*/
		fromID = user_id;
		*send_buf = 0x03;
		*(uint32_t*)(send_buf + 3) = htonl(fromID);
		*(uint32_t*)(send_buf + 7) = htonl(toID);
		*(uint64_t*)(send_buf + 15) = crc64_of_file;
		*(uint16_t*)(send_buf + 23) = htons(filename.GetLength());
		char filename_c[256];
		//strcpy_s(filename_c,filename_A.GetLength(),filename_A);
		memcpy(send_buf + 25, filename_A.GetBuffer(), filename_A.GetLength());
		filename_A.ReleaseBuffer();
		//CRC64
		fp = fopen(filename_A, "rb+");
		fseek(fp, 0, SEEK_END);
		uint16_t length_of_file = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		crc64_of_file = crc64_function(fp, length_of_file, 0x4C11DB74C11DB7);
		*(uint64_t*)(send_buf + 15) = htonll(crc64_of_file);

		// 发送报文
		len = 4 + 4 + 8 + 2 + filename.GetLength();
		*(uint16_t*)(send_buf + 1) = htons(len);
		if (sendto(s_u, send_buf, len+3, 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
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

/*下载文件*/
void CWSChatClientMFCDlg::OnBnClickedIdcButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CStringA filename_A,crc64_id_A;
	uint64_t crc64_id_ul = 0;
	CString filename_C,find_name;
	short int len,flag;

	flag = 0;
	len = 0;

	if (s_u == 0 || user_state == 0)
	{
		MessageBox(L"未连接,无法下载文件");
		return;
	}
	else
	{
		//读取文件名/传输对象
		char* ptr;
		file_nameOrId.GetWindowText(filename_C);
		filename_A = filename_C;
		for (int id_index = 0; id_index < file_list_view.GetItemCount(); id_index++)
		{
			find_name = file_list_view.GetItemText(id_index, 1);
			if (find_name == filename_C)
			{
				char* ptr;
				crc64_id_A = file_list_view.GetItemText(id_index, 0);
				crc64_id_ul = strtoull(crc64_id_A, &ptr, 10);
				flag = 1;
				break;
			}
		}
		if (!flag)
		{
			MessageBox(L"无此文件可下载，请检查文件名或者刷新文件列表");
			return;
		}
		// 创建下载请求报文
		if (!send_data.IsEmpty())
			send_data.Empty();
		memset(send_buf, 0, SEND_BUF_SIZE);
		
		//填充报文
		*send_buf = 0x07;
		*(uint16_t*)(send_buf + 1) = htons(8);
		*(uint64_t*)(send_buf + 3) = htonll(crc64_id_ul);

		// 发送报文
		len = 11;
		if (sendto(s_u, send_buf, len, 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		memset(send_buf, 0, SEND_BUF_SIZE);

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
	client.sin_family = AF_INET;
	client.sin_port = htons(0x1234);
	client.sin_addr.S_un.S_addr = INADDR_ANY;
	WSAAsyncSelect(s_u, hwnd, UM_SOCK, FD_READ);
	WSAAsyncSelect(s_t, hwnd, UM_SOCK, FD_READ);
}

void ReleaseClientResource(HWND hwnd) {
	closesocket(s_t);
	closesocket(s_u);
	WSACleanup();
	return;
}

//
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
	uint32_t sum32,crc32_value,buf_crc;
	uint16_t len;
	CStringA user_key;
	CString input_text;
	unsigned char buf[50];

	
	int i = 0;

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
	case 0x01://challenge type 1B|len 2B|msg 1B|N 1B|bits|numbers 
		sum32 = calculate_chllenge_sum(ptr)|0x00000001;
		private_pwd.GetWindowText(input_text);
		user_key = input_text;
		strcpy((char*)buf,user_key);
		crc32_value = crc32_function(buf, input_text.GetLength(), sum32);

		//0x01 LOGIN 1B
		memset(send_buf, 0, sizeof(send_buf));
		send_buf[i++] = 0x01;
		//len 2B = 5
		len = 1 + 4;
		send_buf[i++] = HIBYTE(len);
		send_buf[i++] = LOBYTE(len);
		//msg_type
		send_buf[i++] = 0x01;
		//32 crc
		crc32_value = htonl(crc32_value);
		ptr = send_buf;
		*((uint32_t*)(ptr + sizeof(unsigned char)*4)) = crc32_value;
		/*for (int count = 1; count < 4; count++)
		{
			buf_crc = crc32_value;
			buf_crc = buf_crc >> (32 - count * 8);
			buf_byte = buf_crc;
			send_buf[i++] = buf_byte;
		}
		send_buf[i++] = crc32_value;*/
		//bit32_data_into_buf(crc32_value,ptr_s);

		
		if (sendto(s_u, send_buf, sizeof(send_buf), 0, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		{
			logfile << "_LINE_:send error" << endl;
		}
		break;
	case 0x03:
		user_id = *(uint32_t*)ptr;
		user_id = ntohl(user_id);
		state_of_client.SetWindowText(L"已上线");
		break;
	case 0x02:
		MessageBox(L"login failed");
		break;
	default:
		break;
	}
	free(ptr_header);
	return 0;
}

//收到短消息
afx_msg LRESULT CWSChatClientMFCDlg::OnTxtMsg(WPARAM wParam, LPARAM lParam)
{
	char* ptr_header, * ptr, show_buf[2048];
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	uint32_t fromID, toID;
	uint16_t msg_len;
	CString show_txt;

	ptr = ptr + 3;

	fromID = ntohl(*(uint32_t*)ptr);
	ptr += 4;
	toID = ntohl(*(uint32_t*)ptr);
	if (toID != user_id)
	{
		return 0;
	}
	ptr += 4;
	msg_len = ntohs(*(uint16_t*)ptr);
	ptr += 2;
	//找是谁发的
	LVFINDINFO info;
	
	CString friend_id;
	friend_id.Format(_T("%lu"), fromID);
	info.flags = LVFI_PARTIAL | LVFI_STRING;
	info.psz = friend_id;
	int id_index = member_list_view.FindItem(&info);
	show_txt = member_list_view.GetItemText(id_index, 1);
	//显示数据
	memcpy(show_buf,ptr,msg_len);
	show_buf[msg_len] = 0x00;

	show_txt += " : ";
	CStringA show_txt_A = show_buf;
	show_txt += show_txt_A;
	show_txt += "\t\r\n";
	show_txt_buf.SetSel(show_txt_buf.GetWindowTextLength(), show_txt_buf.GetWindowTextLengthW());
	show_txt_buf.ReplaceSel(show_txt);
	show_txt_buf.LineScroll(show_txt_buf.GetLineCount());
	free(ptr_header);//https://blog.51cto.com/u_15127700/3727200
	return 0;
}


/*收到上传文件ACK后，开始进行文件传输即上传*/
afx_msg LRESULT CWSChatClientMFCDlg::OnBinAckMsg(WPARAM wParam, LPARAM lParam)
{

	char* ptr_header, * ptr;
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	FILE* fp;
	uint16_t port_server;
	uint64_t retval_file_len,crc_64;
	CStringA filename;
	CString input_text;

	//读取文件名/传输对象
	file_nameOrId.GetWindowText(input_text);
	filename = input_text;//文件名

	crc_64 = ntohll(*(uint64_t*)(ptr_header+3));
	if (crc_64 != crc64_of_file)
	{
		MessageBox(L"服务器返回值错误");
		return 0;
	}
	port_server = ntohs(*(uint16_t*)(ptr_header + 11));

	if (port_server == 0x00)
	{
		MessageBox(_T("不可发送"));
		return 0;
	}
	server_tcp.sin_addr = server.sin_addr;
	server_tcp.sin_port = htons(port_server);
	server_tcp.sin_family = AF_INET;
	if (connect(s_t, (sockaddr*)&server_tcp, sizeof(server_tcp)) == SOCKET_ERROR)
	{
		retval = WSAGetLastError();
		if (retval == 10035)
			goto normal;
		MessageBox(L"文件传输失败，无法连接到服务器");
		return 0;
	}
normal:
	//锁定窗口，减少bug和代码量
	file_nameOrId.EnableWindow(FALSE);
	state_of_client.SetWindowTextW(L"文件发送中，暂时无法进行其他操作");
	fp = fopen(filename, "rb+");

	//添加最外层头部
	memset(send_buf, 0, sizeof(send_buf));
	*(uint64_t*)send_buf = htonll(crc64_of_file);
	//其实可以对文件的状态进行管理，每次判断状态（不想写了，堵着吧先
	char file_buffer[4096];
	do {
		retval_file_len = fread(file_buffer, sizeof(char), sizeof(file_buffer), fp);
		*(uint64_t*)(send_buf + 8) = retval_file_len;
		send(s_t, send_buf, 8+retval_file_len, 0);
	} while (!feof(fp));
	fclose(fp);

	free(ptr_header);
	return 0;
}

//收到加群的消息
afx_msg LRESULT CWSChatClientMFCDlg::OnGrpJoinMsg(WPARAM wParam, LPARAM lParam)
{
	char* ptr_header, * ptr;
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	unsigned char msg_type;
	int count;
	uint32_t group_id;


	ptr += 3;
	group_id = ntohl(*(uint32_t*)ptr);

	msg_type = *(++ptr);
	if (msg_type == 0x00)
	{
		last_group_id = group_id;
		CString group_id_C;
		group_id_C.Format(_T("%u"),last_group_id);
		ugroup_id.SetWindowTextW(group_id_C);
		ugroup_id.EnableWindow(FALSE);
		MessageBox(L"加群成功");
	}
	else {
		MessageBox(L"加群失败");
	}
	free(ptr_header);
	return 0;
}

//收到退群的消息
afx_msg LRESULT CWSChatClientMFCDlg::OnGrpQuitMsg(WPARAM wParam, LPARAM lParam)
{
	char* ptr_header, * ptr;
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	unsigned char msg_type;
	uint32_t group_id;

	ptr = ptr + 3;
	group_id = ntohl(*(uint32_t*)ptr);
	msg_type = *(++ptr);
	if (msg_type == 0x00&&group_id==last_group_id)
	{
		ugroup_id.SetWindowTextW(L"");
		ugroup_id.EnableWindow(TRUE);
		MessageBox(L"已退出群聊");
		last_group_id = 0;
	}
	else {
		MessageBox(L"退群失败");
	}
	free(ptr_header);
	return 0;
}

DWORD ReceiveFile(LPVOID lpParam) {
	recv_file_info* ptr = (recv_file_info*)lpParam;
	int retval;
	char buf[4096];
	retval = connect(s_t, (sockaddr*)&server_tcp, sizeof(server_tcp));
	if (retval != SOCKET_ERROR)
	{
		ptr->fp = fopen(ptr->filename, "wb+");
		while (!feof(ptr->fp))
		{
			retval = recv(s_t, buf, sizeof(buf), 0);
			if (retval == 0)
			{
				shutdown(s_t, SD_BOTH);
				return -1;
			}
			else if (retval < 0)
			{
				closesocket(s_t);
				return -1;
			}
			else {
				uint16_t len = ntohs(*(uint16_t*)(buf + 8));
				uint64_t crc64 = ntohll(*(uint64_t*)buf);
				if(crc64==ptr->crc64)
					fwrite(buf, sizeof(char), retval, ptr->fp);
			}
		}
	}
	shutdown(s_t, SD_RECEIVE);
	fclose(ptr->fp);
	ptr->crc64 = 0;
	return 0;
}

/*收到下载文件的消息*/
afx_msg LRESULT CWSChatClientMFCDlg::OnBinGetMsg(WPARAM wParam, LPARAM lParam)
{	//CRC64 8B|port 2B|
	char* ptr_header, * ptr;
	ptr_header = (char*)(void*)lParam;
	ptr = ptr_header;
	uint64_t crc64;
	uint16_t port;
	CString filename_C;
	CString filename_A;
	FILE* fp;
	recv_file_info hparam;
	DWORD threadID;
	
	crc64 = ntohll(*(uint64_t*)(ptr_header+3));
	port = ntohs(*(uint16_t*)(ptr + 11));
	if (crc64 != recv_crc64_of_file)
	{
		MessageBox(L"文件标识符不一致");
		free(ptr_header);
		return 0;
	}
	if (port == 0)
	{
		MessageBox(L"下载文件请求失败");
		free(ptr_header);
		return 0;
	}
	else {
		server_tcp.sin_addr = server.sin_addr;
		server_tcp.sin_port = htons(port);
		server_tcp.sin_family = AF_INET;
		if (connect(s_t, (sockaddr*)&server_tcp, sizeof(server_tcp)) == SOCKET_ERROR)
		{
			retval = WSAGetLastError();
			if (retval == 10035)
				goto normal;
			MessageBox(L"文件传输失败，无法连接到服务器");
			free(ptr_header);
			return 0;
		}
	normal:
		//锁定窗口，可以少写点
		file_nameOrId.EnableWindow(FALSE);
		file_state.SetWindowTextW(L"等待文件下载");
		file_nameOrId.GetWindowText(filename_C);
		filename_A = filename_C;
		memcpy(hparam.filename, filename_A.GetBuffer(), int(filename_A.GetLength()));
		filename_A.ReleaseBuffer();
		
		//接收
		HANDLE recvThread;
		DWORD recvThreadCode;
		hparam.sock = s_t;
		hparam.crc64 = recv_crc64_of_file;
		hparam.fp = fopen(hparam.filename, "wb+");
		
		if (ReceiveFile(&hparam) == -1)
		{
			file_state.SetWindowTextW(L"下载失败");
			file_nameOrId.EnableWindow(FALSE);
			free(ptr_header);
			return 0;
		}
		else
			file_state.SetWindowTextW(L"下载成功");

	}
	free(ptr_header);
	return 0;
}

//收到群成员列表响应
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

	line = 0;
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
	item_num = ntohs(*(uint16_t*)ptr);
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
				uint16_t len = ntohs(*(uint16_t*)ptr);
				ptr += 2;
				int count = 0;
				for (count = 0; count < len; count++)
				{
					buf[count] = *ptr;
					ptr++;
				}
				buf[count] = 0;
				if (i + 1 != item_num)
					fp << member_id << "\t" << buf << endl;
				else
					fp << member_id << "\t" << buf;
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
				CString s_member_name = item.Right(item.GetLength()-pos-1);
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
				uint16_t len = ntohs(*(uint16_t*)ptr);
				ptr += 2;
				int count = 0;
				for (count = 0; count < len; count++)
				{
					buf[count] = *ptr;
					ptr++;
				}
				buf[count] = 0;
				if (i + 1 != item_num)
					fp << file_id << "\t" << buf << endl;
				else
					fp << file_id << "\t" << buf;
				memset(buf, 0, sizeof(buf));
			}
			fp.close();

			file_list_view.DeleteAllItems();
			fp_in.open("file_list.txt", ios::in);
			while (fp_in.getline(buf, 200))
			{
				item = buf;
				pos = item.ReverseFind('\t');
				CString s_file_id = item.Left(pos);
				CString s_file_name = item.Right(item.GetLength()-pos);
				file_list_view.InsertItem(line,s_file_id);
				file_list_view.SetItemText(line, 1, s_file_name);
				memset(buf, 0, 256);
				line++;
			}
			fp_in.close();
			break;
		//在线用户列表获取
		case 0x02:
			fp.open("member_list.txt", ios::trunc | ios::out | ios::in);
			for (int i = 0; i < item_num; i++)
			{
				uint64_t member_id = ntohll(*(uint64_t*)ptr);
				ptr += 8;
				uint16_t len = ntohs(*(uint16_t*)ptr);
				ptr += 2;
				int count = 0;
				for (count = 0; count < len; count++)
				{
					buf[count] = *ptr;
					ptr++;
				}
				buf[count] = 0;
				if (i + 1 != item_num)
					fp << member_id << "\t" << buf << endl;
				else
					fp << member_id << "\t" << buf;
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
				CString s_member_name = item.Right(item.GetLength() - pos - 1);
				member_list_view.InsertItem(line, s_member_id);
				member_list_view.SetItemText(line, 1, s_member_name);
				memset(buf, 0, 256);
				line++;
			}
			fp_in.close();
			break;
		default:
			break;
		}
	}
	else MessageBox(L"Get List Fail!");
	free(ptr_header);
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

void CWSChatClientMFCDlg::OnCbnSelchangeChatRecvCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CWSChatClientMFCDlg::OnEnChangeStatebox()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

//处理错误按键导致程序退出问题
BOOL CWSChatClientMFCDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return TRUE;
	else
	return CDialogEx::PreTranslateMessage(pMsg);
}

//收到有下载文件的通知
afx_msg LRESULT CWSChatClientMFCDlg::OnBinArriveMsg(WPARAM wParam, LPARAM lParam)
{
	char* ptr_header = (char*)(void*)lParam;
	char name[256];
	CString filename_C;
	CStringA filename_A;
	uint16_t len;
	uint32_t fromID,toID;
	uint64_t crc64;

	
	fromID = ntohl(*(uint32_t*)(ptr_header+3));
	toID = ntohl(*(uint32_t*)(ptr_header + 7));
	recv_crc64_of_file = ntohll(*(uint64_t*)(ptr_header + 11));
	len = ntohs(*(uint16_t*)(ptr_header + 19));
	strcpy_s(name, len, ptr_header + 21);
	name[len] = 0x00;
	filename_A = name;
	filename_C = filename_A;
	file_nameOrId.SetWindowText(filename_C);
	file_nameOrId.EnableWindow(FALSE);

	return 0;
}
