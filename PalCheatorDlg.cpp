// PalCheatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PalCheator.h"
#include "PalCheatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int          g_CurrentSaveFileID;          //当前是游戏的哪个存盘文件
extern GameSaveInfo g_SaveInfo;                   //游戏存盘的数据

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPalCheatorDlg dialog

CPalCheatorDlg::CPalCheatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPalCheatorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPalCheatorDlg)
	m_GameSaveNumber = -1;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
	GetCtrlManager().InstallHook();
}

void CPalCheatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPalCheatorDlg)
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	DDX_Radio(pDX, IDC_RADIO1, m_GameSaveNumber);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPalCheatorDlg, CDialog)
	//{{AFX_MSG_MAP(CPalCheatorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_READ, OnButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, OnButtonAbout)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPalCheatorDlg message handlers

BOOL CPalCheatorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here 
	m_TabCtrl.InsertItem(0, "角色");
	m_TabCtrl.InsertItem(1, "物品");
	m_TabCtrl.InsertItem(2, "说明");
	
	m_TabCtrl.Init();

	m_GameSaveNumber = g_CurrentSaveFileID - 1;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPalCheatorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPalCheatorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPalCheatorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CPalCheatorDlg::OnOK( )
{
}

void CPalCheatorDlg::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
	OnCancel();	
}

void CPalCheatorDlg::OnButtonRead() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	FILE *f;
	char FileName[MAX_PATH];
	sprintf(FileName, "%d.rpg", m_GameSaveNumber + 1);
	f = fopen(FileName, "rb");
	if (!f)
	{
		MessageBox("无法打开仙剑奇侠传DOS版的存盘文件!\r\n请在游戏目录下运行本程序!",
			         "错误信息", MB_OK | MB_ICONERROR);
		return;
	}
	
	fread(&g_SaveInfo, sizeof(g_SaveInfo), 1, f);
	fclose(f);
	
	g_CurrentSaveFileID = m_GameSaveNumber + 1;

	m_TabCtrl.MemoryDataToDialog();
}

void CPalCheatorDlg::OnButtonAbout() 
{
	// TODO: Add your control notification handler code here
	CAboutDlg dlg;
	dlg.DoModal();	
}

void CPalCheatorDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if (g_CurrentSaveFileID != m_GameSaveNumber + 1)
	{
		m_GameSaveNumber = g_CurrentSaveFileID - 1;
		UpdateData(FALSE);
	}

	m_TabCtrl.DialogDataToMemory();
	
	char FileName[MAX_PATH];
	sprintf(FileName, "%d.rpg", g_CurrentSaveFileID);
	FILE *f = fopen(FileName, "rb+");
	if (!f)
	{
		MessageBox("无法打开仙剑奇侠传DOS版的存盘文件!", "错误信息", MB_OK | MB_ICONERROR);
		return;
	}
	
	fwrite(&g_SaveInfo, sizeof(g_SaveInfo), 1, f);
	fclose(f);
}
