// PalCheator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PalCheator.h"
#include "PalCheatorDlg.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPalCheatorApp

BEGIN_MESSAGE_MAP(CPalCheatorApp, CWinApp)
	//{{AFX_MSG_MAP(CPalCheatorApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPalCheatorApp construction

CPalCheatorApp::CPalCheatorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPalCheatorApp object

CPalCheatorApp theApp;

int          g_CurrentSaveFileID = 1;          //当前是游戏的哪个存盘文件
GameSaveInfo g_SaveInfo;                       //游戏存盘的数据

/////////////////////////////////////////////////////////////////////////////
// CPalCheatorApp initialization

BOOL CPalCheatorApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	FILE *f;
	for(g_CurrentSaveFileID = 1; g_CurrentSaveFileID <= 5; g_CurrentSaveFileID++)
	{
		char FileName[MAX_PATH];
		sprintf(FileName, "%d.rpg", g_CurrentSaveFileID);
		f = fopen(FileName, "rb");
		if (f) break;
	}

	if (!f)
	{
		::MessageBox(NULL, "无法打开仙剑奇侠传DOS版的存盘文件!\r\n请在游戏目录下运行本程序!",
			         "错误信息", MB_OK | MB_ICONERROR);
		return false;
	}
	
	fread(&g_SaveInfo, sizeof(g_SaveInfo), 1, f);
	fclose(f);

	CPalCheatorDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
