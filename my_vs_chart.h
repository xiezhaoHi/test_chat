
// my_vs_chart.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cmy_vs_chartApp: 
// �йش����ʵ�֣������ my_vs_chart.cpp
//

class Cmy_vs_chartApp : public CWinApp
{
public:
	Cmy_vs_chartApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cmy_vs_chartApp theApp;