// WinEchodDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FiberListener.h"
#include "FiberSleep.h"
#include "FiberConnect.h"
#include "WinEchod.h"
#include "WinEchodDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWinEchodDlg �Ի���


CWinEchodDlg::CWinEchodDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CWinEchodDlg::IDD, pParent)
	, m_dosFp(NULL)
	, m_listenPort(9001)
	, m_listenIP(_T("127.0.0.1"))
	, m_fiberListen(NULL)
	, m_cocurrent(1)
	, m_count(100)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CWinEchodDlg::~CWinEchodDlg()
{
	if (m_dosFp)
	{
		fclose(m_dosFp);
		FreeConsole();
		m_dosFp = NULL;
	}
}

void CWinEchodDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LISTEN_PORT, m_listenPort);
	DDV_MinMaxUInt(pDX, m_listenPort, 0, 65535);
	DDX_Text(pDX, IDC_LISTEN_IP, m_listenIP);
	DDV_MaxChars(pDX, m_listenIP, 64);
	DDX_Text(pDX, IDC_COCURRENT, m_cocurrent);
	DDV_MinMaxUInt(pDX, m_cocurrent, 1, 1000);
	DDX_Text(pDX, IDC_COUNT, m_count);
	DDV_MinMaxUInt(pDX, m_count, 100, 1000);
}

BEGIN_MESSAGE_MAP(CWinEchodDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_OPEN_DOS, &CWinEchodDlg::OnBnClickedOpenDos)
	ON_BN_CLICKED(IDC_LISTEN, &CWinEchodDlg::OnBnClickedListen)
	ON_BN_CLICKED(IDC_START_SCHEDULE, &CWinEchodDlg::OnBnClickedStartSchedule)
	ON_BN_CLICKED(IDC_CREATE_TIMER, &CWinEchodDlg::OnBnClickedCreateTimer)
	ON_BN_CLICKED(IDC_CONNECT, &CWinEchodDlg::OnBnClickedConnect)
END_MESSAGE_MAP()


// CWinEchodDlg ��Ϣ�������

BOOL CWinEchodDlg::OnInitDialog()
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

	//ShowWindow(SW_MAXIMIZE);

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CWinEchodDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CWinEchodDlg::OnPaint()
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
HCURSOR CWinEchodDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CWinEchodDlg::OnBnClickedOpenDos()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_dosFp == NULL)
	{
		//GetDlgItem(IDC_OPEN_DOS)->EnableWindow(FALSE);
		UpdateData();
		AllocConsole();
		m_dosFp = freopen("CONOUT$","w+t",stdout);
		printf("DOS opened now, listen=%s:%d\r\n",
			m_listenIP.GetString(), m_listenPort);
		GetDlgItem(IDC_OPEN_DOS)->SetWindowText("�ر� DOS ����");
	}
	else
	{
		fclose(m_dosFp);
		m_dosFp = NULL;
		FreeConsole();
		GetDlgItem(IDC_OPEN_DOS)->SetWindowText("�� DOS ����");
	}
}


void CWinEchodDlg::OnBnClickedListen()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_fiberListen)
	{
		GetDlgItem(IDC_LISTEN)->EnableWindow(FALSE);
		m_fiberListen->kill();
		printf("listening fiber was killed\r\n");
		m_listen.unbind();
		printf("listening socket was closed\r\n");
		m_fiberListen = NULL;
		printf("fiber schedule stopped!\r\n");
		GetDlgItem(IDC_LISTEN)->SetWindowText("����");
		GetDlgItem(IDC_LISTEN)->EnableWindow(TRUE);
	}
	else
	{
		CString addr;
		addr.Format("%s:%d", m_listenIP.GetString(), m_listenPort);
		if (m_listen.open(addr) == false)
		{
			printf("listen %s error %s\r\n", addr.GetString());
			return;
		}
		GetDlgItem(IDC_LISTEN)->SetWindowText("ֹͣ");

		printf("listen %s ok\r\n", addr.GetString());
		m_fiberListen = new CFiberListener(m_listen);
		m_fiberListen->start();
	}
}


void CWinEchodDlg::OnBnClickedStartSchedule()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	OnBnClickedCreateTimer();
	acl::fiber::schedule(acl::FIBER_EVENT_T_WMSG);
}


void CWinEchodDlg::OnBnClickedCreateTimer()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	acl::fiber* fb = new CFiberSleep;
	fb->start();
}


void CWinEchodDlg::OnBnClickedConnect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData();
	for (UINT i = 0; i < m_cocurrent; i++)
	{
		acl::fiber* fb = new CFiberConnect(m_count);
		fb->start();
	}
}
