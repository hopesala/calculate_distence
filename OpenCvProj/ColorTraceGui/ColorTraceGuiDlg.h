
// ColorTraceGuiDlg.h : 头文件
//

#pragma once

#define WND_W	640
#define WND_H	480
#define pi 3.14159265
//#define Unfind 9212
//#define Forward 9213
//#define Acrossleft 9214
//#define Acrossright 9215

// CColorTraceGuiDlg 对话框
class CColorTraceGuiDlg : public CDialogEx
{
	///////////////////////////////////////////dll中的函数///////////////////////////////////////////////////
	typedef bool (*_CT_Init)(HDC hdcSrc,HDC hdcOut,HDC hdcY,HDC hdcCr,HDC hdcCb,UINT uTimer,char* chCamName);
	typedef bool (*_CT_GetWrong)();
	typedef void (*_CT_MouseEvent)(int nWndId,int event,int x,int y);
	typedef void (*_CT_GetData)(int &x,int &y);
	typedef void (*_CT_Stop)();
	typedef int (*_RobotRules)(int x,int y,int count[]);
	//dllApp函数句柄
	HINSTANCE hInstApp;
	HINSTANCE hInstRules;

public:
	//dll类中的函数
	_CT_Init CT_Init;
	_CT_GetWrong CT_GetWrong;
	_CT_MouseEvent CT_MouseEvent;
	_CT_GetData CT_GetData;
	_CT_Stop CT_Stop;
	_RobotRules RobotRules;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 构造
public:
	CColorTraceGuiDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_COLORTRACEGUI_DIALOG };

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
	afx_msg void OnBnClickedButtonBegin();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	void MouseEvent(int event,CPoint point);
	bool m_bRun;
	bool m_bbOk;
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonRun();
	afx_msg void OnBnClickedStopfButton1();
};
