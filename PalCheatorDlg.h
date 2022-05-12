// PalCheatorDlg.h : header file
//

#if !defined(AFX_PALCHEATORDLG_H__A309F9FA_0A57_4D53_B1DB_8FB3F637C16B__INCLUDED_)
#define AFX_PALCHEATORDLG_H__A309F9FA_0A57_4D53_B1DB_8FB3F637C16B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CoolControlsManager.h"
#include "MyTabCtrl.h"


/////////////////////////////////////////////////////////////////////////////
// CPalCheatorDlg dialog

class CPalCheatorDlg : public CDialog
{
// Construction
public:
	CPalCheatorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPalCheatorDlg)
	enum { IDD = IDD_PALCHEATOR_DIALOG };
	CMyTabCtrl	m_TabCtrl;
	int		m_GameSaveNumber;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPalCheatorDlg)
	protected:
	virtual void OnOK( ); 

	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPalCheatorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonExit();
	afx_msg void OnButtonRead();
	afx_msg void OnButtonAbout();
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALCHEATORDLG_H__A309F9FA_0A57_4D53_B1DB_8FB3F637C16B__INCLUDED_)
