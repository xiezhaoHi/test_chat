
// my_vs_chartDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "my_vs_chart.h"
#include "my_vs_chartDlg.h"
#include "afxdialogex.h"
#include "em/stdemdef.h"

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


// Cmy_vs_chartDlg 对话框



Cmy_vs_chartDlg::Cmy_vs_chartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MY_VS_CHART_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSendChat = nullptr;
	m_bVoiceChat = false;
}

Cmy_vs_chartDlg::~Cmy_vs_chartDlg()
{
	if (m_pSendChat != nullptr)
	{
		m_pSendChat->DestroyWindow();
		delete m_pSendChat;
	}
}

void Cmy_vs_chartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSG_SHOW, m_RichEditShow);
}


//消息映射
BEGIN_MESSAGE_MAP(Cmy_vs_chartDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &Cmy_vs_chartDlg::OnBnClickedOk)
	ON_MESSAGE(WM_REQUESTVOICE, &Cmy_vs_chartDlg::EM_Voice_Income)
	ON_MESSAGE(WM_ACCEPTCHAT, EM_Voice_DestAcceptChat)
	ON_MESSAGE(WM_EMCANCELREQUEST, Voice_CancelRequest)
	ON_MESSAGE(WM_EMDONOTCHAT, Voice_OnDoNotChat)
	ON_MESSAGE(WM_CANCELREQUESTVOICE, EM_Voice_CancelIncome)
	ON_MESSAGE(WM_DONOTACCEPTCHAT, EM_Voice_DoNotAcceptChat)
	ON_MESSAGE(WM_SRCACCEPTCHAT, Voice_SrcAcceptChat)
END_MESSAGE_MAP()


// Cmy_vs_chartDlg 消息处理程序

BOOL Cmy_vs_chartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 启动 语音通话 服务
	// 消息接收线程启动
	m_MSGrecv.Run(this);
	

	// 启动语音对话处理线程
	// 启动录音接受线程
	m_cw = (CEIMChatingWaiter*)
		AfxBeginThread(RUNTIME_CLASS(CEIMChatingWaiter), THREAD_PRIORITY_HIGHEST);



	GetDlgItem(IDC_EDIT_IP)->SetWindowText("127.0.0.1");
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cmy_vs_chartDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cmy_vs_chartDlg::OnPaint()
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
HCURSOR Cmy_vs_chartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//发送语音请求
void Cmy_vs_chartDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(0);
	m_pSendChat = new EM_SendChat;
	if (!IsWindow(m_pSendChat->GetSafeHwnd()))
	{
		m_pSendChat->Create(IDD_VOICE2);
		m_pSendChat->SetParent(this);
		m_pSendChat->ShowWindow(SW_SHOW);

		// 调整 Msg_Show 窗口大小
		CRect rtRich;
		m_RichEditShow.GetWindowRect(&rtRich);
		ScreenToClient(&rtRich);

		CRect rtChat;
		m_pSendChat->GetWindowRect(&rtChat);
		ScreenToClient(&rtChat);

		rtRich.bottom -= rtChat.bottom;
		m_RichEditShow.MoveWindow(&rtRich);

		rtChat.top = rtRich.bottom;
		rtChat.bottom = rtChat.top + rtChat.bottom;
		m_pSendChat->MoveWindow(&rtChat);
		//	m_pSendChat->UpdateWindow();
	}

	// 使语音请求按钮变灰
	//GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_EDIT_IP)->GetWindowText(m_strUserIP);
	// 向对方发送语音对话的请求
	EM_DATA data;
	data.msg = EM_REQUESTVOICE;
	data.buf = NULL;
	data.len = 0;

	//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
	m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
	SetTimer(33, 1888, NULL);
	// end 向对方发送语音对话的请求

	// 添加信息到 Msg_Show
	Voice_AddText(CString("-- 等待对方接受请求"));
}


// 对方向你发送语音对话请求
afx_msg LRESULT Cmy_vs_chartDlg::EM_Voice_Income(WPARAM wParam, LPARAM lParam)
{
	char *szIP = (char*)lParam;
	m_strUserIP = szIP;
	//HTREEITEM hItem = _User_GetUserItem(szIP);
	//if (NULL != hItem)
	{
		//EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);

		// 一次只能一个语音对话
		if (m_bVoiceChat)
		{
			Voice_DoNotAcceptChat(1);
		}
		else
		{
			Voice_Income();
		}

		// 为什么不直接 m_Chatdlg[index]->ShowWindow(SW_NORMAL);
		// 因为直接那样显示，Chat窗口的焦点会失去，焦点回到Main窗口.
		// 可能是TreeCtrl 的问题
		//PostMessage(WM_EMDBLCLICKTREE, (WPARAM)this, (LPARAM)2);
	}

	//	delete pBuf;
	return 0;

}// End 对方向你发送语音对话请求

 // 对方拒绝接受对话请求
void Cmy_vs_chartDlg::Voice_DoNotAcceptChat(int nValue)
{
	if (NULL == m_pSendChat)
		return;
	
	// 删除语音请求对话框
	CRect rtChat;
	m_pSendChat->GetWindowRect(&rtChat);
	delete m_pSendChat;
	m_pSendChat = NULL;
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow();
	// end 删除语音请求对话框
	// 调整 Msg_Show 大小
	CRect rtRich;
	m_RichEditShow.GetWindowRect(&rtRich);
	ScreenToClient(&rtRich);
	rtRich.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(&rtRich);
	// end 调整 Msg_Show 大小

	// 添加信息到 Msg_Show
	if (0 == nValue)  //手动拒绝 不重新发送请求
	{
		KillTimer(33);
		Voice_AddText(_T("-- 对方不接受语音对话！"));
	}
	else
	{ //时间到了 或重复 发送 请求 
		
		Voice_AddText(_T("-- 语音对话结束！"));
	}
}
// end 对方拒绝接受对话请求

// 被请求语音对话
void Cmy_vs_chartDlg::Voice_Income()
{
	// 启动语音请求对话框
	//m_pSendChat->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(0);
	m_bVoiceChat = true;
	m_pRecvChat = new EM_RecvChat;
	if (!IsWindow(m_pRecvChat->GetSafeHwnd()))
	{
		m_pRecvChat->m_strIP = m_strUserIP;
		m_pRecvChat->Create(IDD_VOICE1);
		m_pRecvChat->SetParent(this);
		m_pRecvChat->ShowWindow(SW_SHOW);

		// 调整 Msg_Show 窗口大小
		CRect rtRich;
		m_RichEditShow.GetWindowRect(&rtRich);
		ScreenToClient(&rtRich);

		CRect rtChat;
		m_pRecvChat->GetWindowRect(&rtChat);
		ScreenToClient(&rtChat);

		rtRich.bottom -= rtChat.bottom;
		m_RichEditShow.MoveWindow(&rtRich);

		rtChat.top = rtRich.bottom;
		rtChat.bottom = rtChat.top + rtChat.bottom;
		m_pRecvChat->MoveWindow(&rtChat);
	}
	// end 启动语音请求对话框

	// 使语音请求按钮变灰
	//GetDlgItem(IDC_VOICECHAT)->EnableWindow(FALSE);

	// 添加信息到 Msg_Show
	Voice_AddText("-- 对方请求语音对话");

}
// end 被请求语音对话


void  Cmy_vs_chartDlg::Voice_AddText(CString const& strText)
{
	m_RichEditShow.AddString(strText);
}


// 对方接受语音对话请求
afx_msg LRESULT  Cmy_vs_chartDlg::EM_Voice_DestAcceptChat(WPARAM wParam, LPARAM lParam)
{

		Voice_DestAcceptChat();

		// 为什么不直接 m_Chatdlg[index]->ShowWindow(SW_NORMAL);
		// 因为直接那样显示，Chat窗口的焦点会失去，焦点回到Main窗口.
		// 可能是TreeCtrl 的问题
		return 0;
}
// end 对方接受语音对话请求

// 对方接受对话请求
void Cmy_vs_chartDlg::Voice_DestAcceptChat()
{
	if (NULL == m_pSendChat)
		return;
	KillTimer(33);
	m_pSendChat->DestAcceptChat(m_strUserIP);

	// 添加信息到 Msg_Show
	Voice_AddText(_T("-- 对方同意语音对话，对话进行中！"));

	m_bVoiceChat = TRUE;
}
// end 对方接受对话请求


// 取消向对方请求语音对话
afx_msg LRESULT   Cmy_vs_chartDlg::Voice_CancelRequest(WPARAM wParam, LPARAM lParam)
{
	int nValue = wParam;

	if (NULL == m_pSendChat)
		return -1;
	KillTimer(33);
	// 删除语音请求对话框
	CRect rtChat;
	m_pSendChat->GetWindowRect(&rtChat);
	delete m_pSendChat;
	m_pSendChat = NULL;
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow();
	// end 删除语音请求对话框

	// 调整 Msg_Show 大小
	CRect rtRich;
	m_RichEditShow.GetWindowRect(&rtRich);
	ScreenToClient(&rtRich);
	rtRich.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(&rtRich);
	// end 调整 Msg_Show 大小

	if (2 == nValue)
	{
		// 向对方发送语音对话的请求
		EM_DATA data;
		data.msg = EM_CANCELREQUESTVOICE;
		data.buf = (char*)&nValue;
		data.len = nValue;

		m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end 向对方发送语音对话的请求

		// 添加信息到 Msg_Show
		Voice_AddText(_T("-- 你停止请求语音对话！"));
	}
	else
	{
		// 向对方发送语音对话的请求
		EM_DATA data;
		data.msg = EM_CANCELREQUESTVOICE;
		data.buf = (char*)&nValue;
		data.len = nValue;

		m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end 向对方发送语音对话的请求

		// 添加信息到 Msg_Show
		Voice_AddText(_T("-- 语音对话结束！"));
		m_cw->Stop();
	}
	m_bVoiceChat = FALSE;
	return -1;
}
// end 取消向对方请求语音对话

// 拒绝接受对方语音对话
afx_msg LRESULT   Cmy_vs_chartDlg::Voice_OnDoNotChat(WPARAM wParam, LPARAM lParam)
{
	if (NULL == m_pRecvChat)
		return -1;

	// 删除语音接受对话框
	CRect rtChat;
	m_pRecvChat->GetWindowRect(&rtChat);
	delete m_pRecvChat;
	m_pRecvChat = NULL;
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow();
	// end 删除语音接受对话框

	// 调整 Msg_Show 大小
	CRect rect;
	m_RichEditShow.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(&rect);
	// end 调整 Msg_Show 大小

	//m_RichEditShow.HideSelection(FALSE, FALSE);
	// 试滚动条滚动到最后一行
	m_RichEditShow.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

	// 添加信息到 Msg_Show
	int nRel = (int)wParam;
	if (0 == nRel)
	{
		// 向对方发送拒绝语音对话的消息
		EM_DATA data;
		data.msg = EM_DONOTCHAT;
		data.buf = (char*)&nRel;
		data.len = nRel;

		m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end 向对方发送拒绝语音对话的消息
		Voice_AddText(_T("-- 你不同意语音对话，操作被取消！"));
	}
	if (1 == nRel)
	{
		// 向对方发送拒绝语音对话的消息
		EM_DATA data;
		data.msg = EM_DONOTCHAT;
		data.buf = (char*)&nRel;
		data.len = nRel;

		m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end 向对方发送拒绝语音对话的消息
		Voice_AddText(_T("-- 语音对话结束！"));
		//	m_pSendChat->m_cw->Stop();
		//	m_pSendChat->m_pwo->Stop();
		m_cw->Stop();
	}
	m_bVoiceChat = FALSE;
	return 0;
}
// end 拒绝接受对方语音对话

// 对方取消请求语音对话
void Cmy_vs_chartDlg::Voice_CancelIncome(int nValue)
{
	if (NULL == m_pRecvChat)
		return;
	KillTimer(33);
	// 删除语音接受对话框
	CRect rtChat;
	m_pRecvChat->GetWindowRect(&rtChat);
	delete m_pRecvChat;
	m_pRecvChat = NULL;
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow();
	// end 删除语音接受对话框

	// 调整 Msg_Show 大小
	CRect rtRich;
	m_RichEditShow.GetWindowRect(&rtRich);
	ScreenToClient(&rtRich);
	rtRich.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(&rtRich);
	// end 调整 Msg_Show 大小

	// 添加信息到 Msg_Show
	if (2 == nValue)
	{
		Voice_AddText(_T("-- 对方停止请求语音对话！"));
	}
	else
	{
		Voice_AddText(_T("-- 语音对话结束！"));
	}
}
// end 被请求语音对话

afx_msg LRESULT    Cmy_vs_chartDlg::EM_Voice_CancelIncome(WPARAM wParam, LPARAM lParam)
{
	int nValue = (int)wParam;

	char *szIP = (char*)lParam;

	Voice_CancelIncome(nValue);
	return -1;
}

//不接受请求
afx_msg LRESULT    Cmy_vs_chartDlg::EM_Voice_DoNotAcceptChat(WPARAM wParam, LPARAM lParam)
{
	int nValue = (int)wParam;

	char *szIP = (char*)lParam;

	
		Voice_DoNotAcceptChat(nValue);
		return 0;
}

// 接受对方语音对话请求
afx_msg LRESULT    Cmy_vs_chartDlg::Voice_SrcAcceptChat(WPARAM wParam, LPARAM lParam)
{
	// 向对方发送同意语音对话的消息
	EM_DATA data;
	data.msg = EM_ACCEPTCHAT;
	data.buf = NULL;
	data.len = 0;

	//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
	m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
	// end 向对方发送同意语音对话的消息

	// 添加信息到 Msg_Show
	Voice_AddText(_T("-- 你同意语音对话，对话进行中！"));
	return 0;
}
// end 接受对方语音对话请求


void Cmy_vs_chartDlg::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	// TODO: Add your message handler code here and/or call default
	if (33 == nIDEvent)
	{
		EM_DATA data;
		data.msg = EM_REQUESTVOICE;
		data.buf = NULL;
		data.len = 0;

		//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
		m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
	}
	
	CDialog::OnTimer(nIDEvent);
}
