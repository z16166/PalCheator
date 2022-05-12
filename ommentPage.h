#if !defined(AFX_OMMENTPAGE_H__1E33CE63_B33A_4F75_B4BF_AC841338FBD0__INCLUDED_)
#define AFX_OMMENTPAGE_H__1E33CE63_B33A_4F75_B4BF_AC841338FBD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ommentPage.h : header file
//
#include "GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CommentPage dialog

class CommentPage : public CDialog
{
// Construction
public:
	CommentPage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CommentPage)
	enum { IDD = IDD_DIALOG_COMMENT };
		// NOTE: the ClassWizard will add data members here
		CGridCtrl m_Grid;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CommentPage)
	protected:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CommentPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OMMENTPAGE_H__1E33CE63_B33A_4F75_B4BF_AC841338FBD0__INCLUDED_)
