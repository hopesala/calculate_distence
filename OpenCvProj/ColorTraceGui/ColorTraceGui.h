
// ColorTraceGui.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CColorTraceGuiApp:
// �йش����ʵ�֣������ ColorTraceGui.cpp
//

class CColorTraceGuiApp : public CWinApp
{
public:
	CColorTraceGuiApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CColorTraceGuiApp theApp;