
// my_vs_chartDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "my_vs_chart.h"
#include "my_vs_chartDlg.h"
#include "afxdialogex.h"
#include "em/stdemdef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// Cmy_vs_chartDlg �Ի���



Cmy_vs_chartDlg::Cmy_vs_chartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MY_VS_CHART_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pSendChat = nullptr;
	m_bVoiceChat = false;
	m_cw = nullptr;
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


//��Ϣӳ��
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


// Cmy_vs_chartDlg ��Ϣ�������

BOOL Cmy_vs_chartDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// ���� ����ͨ�� ����
	// ��Ϣ�����߳�����
	m_MSGrecv.Run(this);
	

	// ���������Ի������߳�
	// ����¼�������߳�
	m_cw = (CEIMChatingWaiter*)
		AfxBeginThread(RUNTIME_CLASS(CEIMChatingWaiter), THREAD_PRIORITY_HIGHEST);




	GetDlgItem(IDC_EDIT_IP)->SetWindowText("127.0.0.1");
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cmy_vs_chartDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cmy_vs_chartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//������������
void Cmy_vs_chartDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	

	
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(0);
	m_pSendChat = new EM_SendChat;
	if (!IsWindow(m_pSendChat->GetSafeHwnd()))
	{
		m_pSendChat->Create(IDD_VOICE2);
		m_pSendChat->SetParent(this);
		m_pSendChat->ShowWindow(SW_SHOW);

		// ���� Msg_Show ���ڴ�С
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

	// ʹ��������ť���
	//GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
	
	GetDlgItem(IDC_EDIT_IP)->GetWindowText(m_strUserIP);
	// ��Է����������Ի�������
	EM_DATA data;
	data.msg = EM_REQUESTVOICE;
	data.buf = NULL;
	data.len = 0;

	//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
	m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
	//SetTimer(33, 1888, NULL);
	// end ��Է����������Ի�������

	// �����Ϣ�� Msg_Show
	Voice_AddText(CString("-- �ȴ��Է���������"));
}


// �Է����㷢�������Ի�����
afx_msg LRESULT Cmy_vs_chartDlg::EM_Voice_Income(WPARAM wParam, LPARAM lParam)
{
	char *szIP = (char*)lParam;
	m_strUserIP = szIP;
	//HTREEITEM hItem = _User_GetUserItem(szIP);
	//if (NULL != hItem)
	{
		//EM_MsgDlg *pDlg = _Msg_GetDialog(hItem);

		// һ��ֻ��һ�������Ի�
		if (m_bVoiceChat)
		{
			Voice_DoNotAcceptChat(1);
		}
		else
		{
			Voice_Income();
		}

		// Ϊʲô��ֱ�� m_Chatdlg[index]->ShowWindow(SW_NORMAL);
		// ��Ϊֱ��������ʾ��Chat���ڵĽ����ʧȥ������ص�Main����.
		// ������TreeCtrl ������
		//PostMessage(WM_EMDBLCLICKTREE, (WPARAM)this, (LPARAM)2);
	}

	//	delete pBuf;
	return 0;

}// End �Է����㷢�������Ի�����

 // �Է��ܾ����ܶԻ�����
void Cmy_vs_chartDlg::Voice_DoNotAcceptChat(int nValue)
{
	if (NULL == m_pSendChat)
		return;
	
	// ɾ����������Ի���
	CRect rtChat;
	m_pSendChat->GetWindowRect(&rtChat);
	if (m_pSendChat->m_pwo)
	{
		m_pSendChat->m_pwo->Stop();
	}
	delete m_pSendChat;
	m_pSendChat = NULL;
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow();
	// end ɾ����������Ի���
	// ���� Msg_Show ��С
	CRect rtRich;
	m_RichEditShow.GetWindowRect(&rtRich);
	ScreenToClient(&rtRich);
	rtRich.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(&rtRich);
	// end ���� Msg_Show ��С

	// �����Ϣ�� Msg_Show
	if (0 == nValue)  //�ֶ��ܾ� �����·�������
	{
		KillTimer(33);
		Voice_AddText(_T("-- �Է������������Ի���"));
	}
	else //1 ��ʾ ���� ���ڽ��е�ͨ��
	{ 
		Voice_AddText(_T("-- �����Ի�������"));
		if (m_cw)
		{
			m_cw->Stop();
		}
	}
	m_bVoiceChat = FALSE;
}
// end �Է��ܾ����ܶԻ�����

// �����������Ի�
void Cmy_vs_chartDlg::Voice_Income()
{
	// ������������Ի���
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

		// ���� Msg_Show ���ڴ�С
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
	// end ������������Ի���

	// ʹ��������ť���
	//GetDlgItem(IDC_VOICECHAT)->EnableWindow(FALSE);

	// �����Ϣ�� Msg_Show
	Voice_AddText("-- �Է����������Ի�");

}
// end �����������Ի�


void  Cmy_vs_chartDlg::Voice_AddText(CString const& strText)
{
	m_RichEditShow.AddString(strText);
}


// �Է����������Ի�����
afx_msg LRESULT  Cmy_vs_chartDlg::EM_Voice_DestAcceptChat(WPARAM wParam, LPARAM lParam)
{
	m_strUserIP = (LPCTSTR)lParam;
		Voice_DestAcceptChat();

		// Ϊʲô��ֱ�� m_Chatdlg[index]->ShowWindow(SW_NORMAL);
		// ��Ϊֱ��������ʾ��Chat���ڵĽ����ʧȥ������ص�Main����.
		// ������TreeCtrl ������
		return 0;
}
// end �Է����������Ի�����

// �Է����ܶԻ�����
void Cmy_vs_chartDlg::Voice_DestAcceptChat()
{
	if (NULL == m_pSendChat)
		return;
	KillTimer(33);
	m_pSendChat->DestAcceptChat(m_strUserIP);

	// �����Ϣ�� Msg_Show
	Voice_AddText(_T("-- �Է�ͬ�������Ի����Ի������У�"));

	m_bVoiceChat = TRUE;
}
// end �Է����ܶԻ�����


// ȡ����Է����������Ի�
afx_msg LRESULT   Cmy_vs_chartDlg::Voice_CancelRequest(WPARAM wParam, LPARAM lParam)
{
	int nValue = wParam;

	if (NULL == m_pSendChat)
		return -1;
	KillTimer(33);
	// ɾ����������Ի���
	CRect rtChat;
	m_pSendChat->GetWindowRect(&rtChat);
	delete m_pSendChat;
	m_pSendChat = NULL;
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow();
	// end ɾ����������Ի���

	// ���� Msg_Show ��С
	CRect rtRich;
	m_RichEditShow.GetWindowRect(&rtRich);
	ScreenToClient(&rtRich);
	rtRich.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(&rtRich);
	// end ���� Msg_Show ��С

	if (2 == nValue)
	{
		// ��Է����������Ի�������
		EM_DATA data;
		data.msg = EM_CANCELREQUESTVOICE;
		data.buf = (char*)&nValue;
		data.len = nValue;

		m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end ��Է����������Ի�������

		// �����Ϣ�� Msg_Show
		Voice_AddText(_T("-- ��ֹͣ���������Ի���"));
	}
	else
	{
		// ��Է����������Ի�������
		EM_DATA data;
		data.msg = EM_CANCELREQUESTVOICE;
		data.buf = (char*)&nValue;
		data.len = nValue;

		m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end ��Է����������Ի�������

		// �����Ϣ�� Msg_Show
		Voice_AddText(_T("-- �����Ի�������"));
		m_cw->Stop();
	}
	m_bVoiceChat = FALSE;
	return -1;
}
// end ȡ����Է����������Ի�

// �ܾ����ܶԷ������Ի�
afx_msg LRESULT   Cmy_vs_chartDlg::Voice_OnDoNotChat(WPARAM wParam, LPARAM lParam)
{
	if (NULL == m_pRecvChat)
		return -1;

	// ɾ���������ܶԻ���
	CRect rtChat;
	m_pRecvChat->GetWindowRect(&rtChat);
	delete m_pRecvChat;
	m_pRecvChat = NULL;
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow();
	// end ɾ���������ܶԻ���

	// ���� Msg_Show ��С
	CRect rect;
	m_RichEditShow.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(&rect);
	// end ���� Msg_Show ��С

	//m_RichEditShow.HideSelection(FALSE, FALSE);
	// �Թ��������������һ��
	m_RichEditShow.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

	// �����Ϣ�� Msg_Show
	int nRel = (int)wParam;
	if (0 == nRel)
	{
		// ��Է����;ܾ������Ի�����Ϣ
		EM_DATA data;
		data.msg = EM_DONOTCHAT;
		data.buf = (char*)&nRel;
		data.len = nRel;

		m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end ��Է����;ܾ������Ի�����Ϣ
		Voice_AddText(_T("-- �㲻ͬ�������Ի���������ȡ����"));
	}
	if (1 == nRel)
	{
		// ��Է����;ܾ������Ի�����Ϣ
		EM_DATA data;
		data.msg = EM_DONOTCHAT;
		data.buf = (char*)&nRel;
		data.len = nRel;

		m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
		// end ��Է����;ܾ������Ի�����Ϣ
		Voice_AddText(_T("-- �����Ի�������"));
		//	m_pSendChat->m_cw->Stop();
		//	m_pSendChat->m_pwo->Stop();
		m_cw->Stop();
	}
	m_bVoiceChat = FALSE;
	return 0;
}
// end �ܾ����ܶԷ������Ի�

// �Է�ȡ�����������Ի�
void Cmy_vs_chartDlg::Voice_CancelIncome(int nValue)
{
	if (NULL == m_pRecvChat)
		return;
	KillTimer(33);
	// ɾ���������ܶԻ���
	CRect rtChat;
	m_pRecvChat->GetWindowRect(&rtChat);
	if (m_pRecvChat->m_pwo)
	{
		m_pRecvChat->m_pwo->Stop(); //���� ���ն˵�  �������պͲ���
	}
	delete m_pRecvChat;
	m_pRecvChat = NULL;
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow();
	// end ɾ���������ܶԻ���

	// ���� Msg_Show ��С
	CRect rtRich;
	m_RichEditShow.GetWindowRect(&rtRich);
	ScreenToClient(&rtRich);
	rtRich.bottom += rtChat.Height();
	m_RichEditShow.MoveWindow(&rtRich);
	// end ���� Msg_Show ��С

	// �����Ϣ�� Msg_Show
	if (2 == nValue)
	{
		Voice_AddText(_T("-- �Է�ֹͣ���������Ի���"));
	}
	else
	{
		Voice_AddText(_T("-- �����Ի�������"));
		if (m_cw)
		{
			m_cw->Stop(); //���� ���ն˵�  ¼��
		}
	}
	m_bVoiceChat = FALSE;
}
// end �����������Ի�

afx_msg LRESULT    Cmy_vs_chartDlg::EM_Voice_CancelIncome(WPARAM wParam, LPARAM lParam)
{
	int nValue = (int)wParam;

	char *szIP = (char*)lParam;

	Voice_CancelIncome(nValue);
	return 0;
}

//����������
afx_msg LRESULT    Cmy_vs_chartDlg::EM_Voice_DoNotAcceptChat(WPARAM wParam, LPARAM lParam)
{
	int nValue = (int)wParam;

	char *szIP = (char*)lParam;

	
		Voice_DoNotAcceptChat(nValue);
		return 0;
}

// ���ܶԷ������Ի�����
afx_msg LRESULT    Cmy_vs_chartDlg::Voice_SrcAcceptChat(WPARAM wParam, LPARAM lParam)
{
	// ��Է�����ͬ�������Ի�����Ϣ
	EM_DATA data;
	data.msg = EM_ACCEPTCHAT;
	data.buf = NULL;
	data.len = 0;

	//	EM_USERINFO ui(m_szUserPCName, m_szUserIP);
	m_MSGrecv.SendMsg((LPSTR)(LPCTSTR)m_strUserIP, &data);
	// end ��Է�����ͬ�������Ի�����Ϣ

	// �����Ϣ�� Msg_Show
	Voice_AddText(_T("-- ��ͬ�������Ի����Ի������У�"));
	return 0;
}
// end ���ܶԷ������Ի�����


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
