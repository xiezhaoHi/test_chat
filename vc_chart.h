
// vc_chart.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cvc_chartApp: 
// �йش����ʵ�֣������ vc_chart.cpp
//

class Cvc_chartApp : public CWinApp
{
public:
	Cvc_chartApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cvc_chartApp theApp;