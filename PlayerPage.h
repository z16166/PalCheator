#if !defined(AFX_PLAYERPAGE_H__BC5DA554_EA1C_4885_BEB0_80F5AEED6C91__INCLUDED_)
#define AFX_PLAYERPAGE_H__BC5DA554_EA1C_4885_BEB0_80F5AEED6C91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayerPage.h : header file
//

#include "resource.h"
#include "GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// PlayerPage dialog

class PlayerPage : public CDialog
{
// Construction
public:
	void DialogDataToMemory(void);
	void MemoryDataToDialog(void);
	PlayerPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(PlayerPage)
	enum { IDD = IDD_DIALOG_PLAYER };
	long	m_Money;
	CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PlayerPage)
	protected:
	virtual void OnOK( ); 
	virtual void OnCancel( ); 
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PlayerPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRecommend();
	afx_msg void OnButtonRestore();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERPAGE_H__BC5DA554_EA1C_4885_BEB0_80F5AEED6C91__INCLUDED_)
