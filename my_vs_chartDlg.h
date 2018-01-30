
// my_vs_chartDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "EM_SendChat.h"
#include "EM_RecvChat.h"
#include "MSGReceiver.h"
#include <afxtempl.h>
// Cmy_vs_chartDlg �Ի���
class Cmy_vs_chartDlg : public CDialogEx
{
// ����
public:
	Cmy_vs_chartDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~Cmy_vs_chartDlg();
// �Ի�������
//#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY_VS_CHART_DIALOG };
//#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��




// ʵ��
protected:
	bool			m_bVoiceChat;
	HICON m_hIcon;
	EM_SendChat*	m_pSendChat;
	EM_RecvChat*	m_pRecvChat;
	CString			m_strUserIP;
	//
	// ��Ϣ�շ�
	EM_MSGReceiver m_MSGrecv;
	CEIMChatingWaiter* m_cw;


	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnBnClickedOk();
	afx_msg LRESULT EM_Voice_Income(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT EM_Voice_DestAcceptChat(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT Voice_CancelRequest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  Voice_OnDoNotChat(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  EM_Voice_CancelIncome(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  EM_Voice_DoNotAcceptChat(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT  Voice_SrcAcceptChat(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_RichEditShow;
	void  Voice_AddText(CString const& strText);
	void Voice_Income();
	void Voice_DoNotAcceptChat(int nValue);
	void Voice_DestAcceptChat();
	void Voice_CancelIncome(int nValue);
protected:
	//afx_msg LRESULT OnText(WPARAM wParam, LPARAM lParam);
};
