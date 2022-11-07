
// WSChatClient_MFCDlg.h: 头文件
//

#pragma once


// CWSChatClientMFCDlg 对话框
class CWSChatClientMFCDlg : public CDialogEx
{
// 构造
public:
	CWSChatClientMFCDlg(CWnd* pParent = nullptr);	// 标准构造函数

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
};
