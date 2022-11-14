
// WSChatClient_MFCDlg.h: 头文件
//
#include <fstream>
#define OFFLINE 0
#define ONLINE 1

enum PacketEnum
{
	TYPE_UNDEF,
	TYPE_LOGIN,
	TYPE_MSG_TXT,
	TYPE_MSG_BIN,
	TYPE_MSG_BIN_ACK,
	TYPE_GRP_JOIN,
	TYPE_GRP_QUIT,
	TYPE_BIN_GET,
	TYPE_GRP_LST
};

#pragma once


// CWSChatClientMFCDlg 对话框
class CWSChatClientMFCDlg : public CDialogEx
{
// 构造
public:
	CWSChatClientMFCDlg(CWnd* pParent = nullptr);	// 标准构造函数
	struct sockaddr_in server,client;
	int retval ;
	int user_state ;
	CStringA send_data;
	CString last_group_id;
	char recv_buf[1024];
	short int user_id;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WSCHATCLIENT_MFC_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnIpnFieldchangedIpaddress1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedIdcButton7();
	// 记录本地用户的用户名
//	CString username_local;
	afx_msg void OnEnChangeIdcEdit1();
//	CString pubilc_key_value;
	afx_msg void OnBnClickedIdcButton1();
	// 公钥
//	CString public_key_value;
	CString username_local;
	afx_msg void OnIpnFieldchangedIpIdcIpaddress1(NMHDR* pNMHDR, LRESULT* pResult);
	// 客户机本地IP地址信息
	CIPAddressCtrl user_ip_local;
	afx_msg void OnBnClickedIpIdcRadio1();
	afx_msg void OnEnChangePwdIdcEdit2();
	// 公共密钥类
	CEdit public_key_value;
	afx_msg void OnIpnFieldchangedIpIdcIpaddress2(NMHDR* pNMHDR, LRESULT* pResult);
	// FTP服务器的IP地址
	CIPAddressCtrl server_ip;
	// 服务器的端口号
	CEdit server_port;
protected:
//	afx_msg LRESULT OnGroupOpr(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedIdcButton2();
	afx_msg void OnBnClickedGroupIdcButton4();
	afx_msg void OnBnClickedGroupIdcButton3();
	afx_msg void OnEnChangeIdcEdit3();
	CEdit ugroup_id;
	afx_msg void OnBnClickedFilelistIdcButton7();
	afx_msg void OnBnClickedFriendlistIdcButton6();
	afx_msg void OnBnClickedIdcButton5();
	afx_msg void OnCbnSelchangeFileRecvCombo4();
	afx_msg void OnEnChangeIdcEdit2();
	CEdit file_path;
	// 文件接收的对象
	CComboBox file_receiver;
	afx_msg void OnBnClickedIdcButton6();
protected:
	afx_msg LRESULT OnUmSock(WPARAM wParam, LPARAM lParam);
public:
//	virtual HRESULT accDoDefaultAction(VARIANT varChild);
protected:
	afx_msg LRESULT OnLoginMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnTxtMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBinAckMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGrpJoinMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGrpQuitMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnBinGetMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGrpListMsg(WPARAM wParam, LPARAM lParam);
};

void InitResourceOfClient(HWND hwnd);
void ReleaseClientResource(HWND hwnd);