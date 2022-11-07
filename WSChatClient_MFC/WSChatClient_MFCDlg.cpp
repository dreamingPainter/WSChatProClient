
// WSChatClient_MFCDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "WSChatClient_MFC.h"
#include "WSChatClient_MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
	switch (message)
	{




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


void CWSChatClientMFCDlg::OnBnClickedIdcButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString input_text;

	UpdateData(TRUE);
	public_key_value.GetWindowText(input_text);
	if (input_text.IsEmpty())
	{
		MessageBox(_T("公钥未填写"));
	}
	else if (username_local.IsEmpty())
		MessageBox(_T("请填写您的用户名！"));
	else if (user_ip_local.IsBlank())
	{
		//本地ip获取
		MessageBox(_T("默认使用本机ip"));
	}
	UpdateData(FALSE);
	
}


void CWSChatClientMFCDlg::OnIpnFieldchangedIpIdcIpaddress1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}


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
