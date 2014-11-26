
// ColorTraceGuiDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ColorTraceGui.h"
#include "ColorTraceGuiDlg.h"
#include "afxdialogex.h"
#include <cmath>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

short int nsign=0;
short int rsign=0;
short int lsign=0;
short int rflag=0;
int count[10]={0,0,0,0,0,0,0,0,0,0};
short int lflag=0;
short int nPos=0;
//int nc=0,ns=0,n0=0;
int D[5][481];
double t;

CColorTraceGuiDlg::CColorTraceGuiDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CColorTraceGuiDlg::IDD, pParent)
	, m_bRun(false)
	, m_bbOk(false)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CColorTraceGuiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CColorTraceGuiDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BEGIN, &CColorTraceGuiDlg::OnBnClickedButtonBegin)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CColorTraceGuiDlg::OnBnClickedButtonStop)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_RUN, &CColorTraceGuiDlg::OnBnClickedButtonRun)
	ON_BN_CLICKED(IDC_STOPF_BUTTON1, &CColorTraceGuiDlg::OnBnClickedStopfButton1)
END_MESSAGE_MAP()


// CColorTraceGuiDlg ��Ϣ�������

BOOL CColorTraceGuiDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//����dll
	hInstApp=LoadLibrary("ColorTraceApp.dll");
	hInstRules=LoadLibrary("RobotRules.dll");

	CT_Init = (_CT_Init)GetProcAddress(hInstApp,"CT_Init");
	CT_GetWrong = (_CT_GetWrong)GetProcAddress(hInstApp,"CT_GetWrong");
	CT_MouseEvent = (_CT_MouseEvent)GetProcAddress(hInstApp,"CT_MouseEvent");
	CT_GetData = (_CT_GetData)GetProcAddress(hInstApp,"CT_GetData");
	CT_Stop = (_CT_Stop)GetProcAddress(hInstApp,"CT_Stop");
	RobotRules = (_RobotRules)GetProcAddress(hInstRules,"RobotRules");

	GetDlgItem(IDC_STATIC_SRC)->MoveWindow(20,30,WND_W,WND_H);
	GetDlgItem(IDC_STATIC_OUT)->MoveWindow(20+WND_W+25,30,WND_W,WND_H);
	GetDlgItem(IDC_STATIC_Y)->MoveWindow(20,550,256,100);
	GetDlgItem(IDC_STATIC_CR)->MoveWindow(286,550,256,100);
	GetDlgItem(IDC_STATIC_CB)->MoveWindow(286+256+10,550,256,100);

	GetDlgItem(IDC_BUTTON_STOP)->ShowWindow(SW_HIDE);
	//OnBnClickedButtonBegin();

	memset(D, 0, sizeof(D));
	//double L = 57.0;
	double H = 24.5;
	double a = 12.37;
	//����ͷˮƽ����ƫ��
	double b = 23.26;
	//����ͷˮƽ����ƫ��
	//t = tan(a*3.14 / 180);
	t = (double)tan(a / 180 * pi) / (tan(a / 180 * pi) + (double)tan(b / 180 * pi));
	for (int i = (int)WND_H*t + 1; i <= WND_H + 1; i++) {	
		D[0][i] = (1 - t) / ((double) i / WND_H - t) * H / tan(b / 180 * pi);//*L
		//printf("%.0f %.1f\n", i, D);
	}
	

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CColorTraceGuiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CColorTraceGuiDlg::OnPaint()
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
HCURSOR CColorTraceGuiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
char chCamName[256];
HDC hdcSrc;
HDC hdcOut;
HDC hdcY;
HDC hdcCr;	
HDC hdcCb;
void CColorTraceGuiDlg::OnBnClickedButtonBegin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	hdcSrc=GetDlgItem(IDC_STATIC_SRC)->GetDC()->m_hDC;
	hdcOut=GetDlgItem(IDC_STATIC_OUT)->GetDC()->m_hDC;
	hdcY=GetDlgItem(IDC_STATIC_Y)->GetDC()->m_hDC;
	hdcCr=GetDlgItem(IDC_STATIC_CR)->GetDC()->m_hDC;	
	hdcCb=GetDlgItem(IDC_STATIC_CB)->GetDC()->m_hDC;

	chCamName[256];
	GetDlgItemText(IDC_EDIT_CAMN_AME,chCamName,256);

	short int nId = GetDlgItemInt(IDC_EDIT_ID,0,TRUE);
	if (nId == NULL)
	{
		MessageBox("�����������ID��");
		return;
	}
	else if ((nId<=0)||(nId>255))
	{
		MessageBox("�������������1-255֮�����ݣ�");
		return;
	}

	if (CT_Init(hdcSrc,hdcOut,hdcY,hdcCr,hdcCb,100,chCamName))
	{
		m_bbOk=true;
		SetTimer(1,100,NULL);
	}
}

void CColorTraceGuiDlg::OnBnClickedButtonRun()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_bRun=true;

	char ProgrammePath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH,ProgrammePath);
	ShellExecute(NULL,NULL,_T("usbhidioc.exe"),NULL,ProgrammePath,SW_SHOWNORMAL);

	GetDlgItem(IDC_BUTTON_RUN)->ShowWindow(SW_HIDE); 
	GetDlgItem(IDC_BUTTON_STOP)->ShowWindow(SW_SHOW); 
}


void CColorTraceGuiDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_bRun)
	{
		KillTimer(1);
		Sleep(100);
		CT_Stop();
		m_bRun=false;
		m_bbOk=false;
		GetDlgItem(IDC_BUTTON_STOP)->ShowWindow(SW_HIDE); 
		GetDlgItem(IDC_BUTTON_RUN)->ShowWindow(SW_SHOW); 
		HWND hwReceiveApp;
		hwReceiveApp = ::FindWindow( NULL, "2401Control" );
		::SendMessage(hwReceiveApp,WM_CLOSE,0,0);
	}
}

void CColorTraceGuiDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bbOk)
	{
		MouseEvent(1,point);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CColorTraceGuiDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bbOk)
	{
		MouseEvent(2,point);
	}
	CDialogEx::OnRButtonDown(nFlags, point);
}

//����¼�
void CColorTraceGuiDlg::MouseEvent(int event,CPoint point)
{
	CRect RectParent;
	GetWindowRect(&RectParent);

	CPoint pointScreen;
	pointScreen.x=point.x+RectParent.left+GetSystemMetrics(SM_CXDLGFRAME);
	pointScreen.y=point.y+RectParent.top+GetSystemMetrics(SM_CYDLGFRAME)+GetSystemMetrics(SM_CYCAPTION);

	CRect Rect;
	int nWndId=0;

	GetDlgItem(IDC_STATIC_SRC)->GetWindowRect(&Rect);
	if (Rect.PtInRect(pointScreen))
	{
		nWndId=1;
		TRACE("SRC");
	}
	else
	{
		GetDlgItem(IDC_STATIC_Y)->GetWindowRect(&Rect);
		if (Rect.PtInRect(pointScreen))
		{
			nWndId=2;
			TRACE("Y");
		}
		else
		{
			GetDlgItem(IDC_STATIC_CR)->GetWindowRect(&Rect);
			if (Rect.PtInRect(pointScreen))
			{
				nWndId=3;
				TRACE("CR");
			}
			else
			{
				GetDlgItem(IDC_STATIC_CB)->GetWindowRect(&Rect);
				if (Rect.PtInRect(pointScreen))
				{
					nWndId=4;
					TRACE("CB");
				}
				else
					return;
			}
		}
	}

	CPoint pointClient;
	pointClient.x=pointScreen.x-Rect.left;
	pointClient.y=pointScreen.y-Rect.top;

	CT_MouseEvent(nWndId,event,pointClient.x,pointClient.y);
}

void CColorTraceGuiDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (m_bRun)
	{
		OnBnClickedButtonStop();
	}
	else if(m_bbOk)
	{
		KillTimer(1);
		Sleep(500);
		CT_Stop();
	}
	CDialogEx::OnClose();
}
int prex=0,prey=0;
void CColorTraceGuiDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	int x,y;
	CT_GetData(x,y);
	SetDlgItemInt(IDC_DES_EDIT,D[0][y],true);
	//�������λ��640*480
	if(m_bRun)
	{
		nPos = RobotRules(x,y,count);
		/*if(CT_GetWrong() == true)
		{
			CT_Stop();
			CT_Init(hdcSrc,hdcOut,hdcY,hdcCr,hdcCb,100,chCamName);
			nPos = 0;
		}*/
		short int nId = GetDlgItemInt(IDC_EDIT_ID,0,TRUE);
		char chCode[2];
		chCode[0]=nId;
		chCode[1]=nPos;

		//ͨ����Ϣ�����ݷ��͸��������
		COPYDATASTRUCT MyCDS;
		MyCDS.dwData = 0;							// function identifier
		MyCDS.cbData = 2;			// size of data
		MyCDS.lpData = (void*)chCode;      // data structure

		HWND hwReceiveApp;
		hwReceiveApp = ::FindWindow( NULL, "2401Control" );
		::SendMessage(hwReceiveApp,WM_COPYDATA,(WPARAM)(HWND) m_hWnd,(LPARAM) (LPVOID) &MyCDS);

		//�ڴ�������ʾ���ٵ�����
		CString str,strTemp;
		//strTemp.Format("%d/%d     ",x,y);
		strTemp.Format("%d %d    ",nId,nPos);
		GetDlgItemText(IDC_EDIT_MSG,str);
		str+=strTemp;
		SetDlgItemText(IDC_EDIT_MSG,str);
		GetDlgItem(IDC_EDIT_MSG)->PostMessage(WM_VSCROLL, SB_BOTTOM, 0);

		CDialogEx::OnTimer(nIDEvent);
	}
}


void CColorTraceGuiDlg::OnBnClickedStopfButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for(int i=0;i<10;i++)
	{
		count[i] = 1;
	}
}
