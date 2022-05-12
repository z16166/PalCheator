#if !defined(AFX_RESOURCEPAGE_H__1C53DCFD_0C6A_4ED6_8703_9BD823C8AE97__INCLUDED_)
#define AFX_RESOURCEPAGE_H__1C53DCFD_0C6A_4ED6_8703_9BD823C8AE97__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResourcePage.h : header file
//

#include "resource.h"
#include "GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// ResourcePage dialog

class ResourcePage : public CDialog
{
// Construction
public:
	void DialogDataToMemory(void);
	void MemoryDataToDialog(void);
	ResourcePage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ResourcePage)
	enum { IDD = IDD_DIALOG_RESOURCE };
	CGridCtrl m_Grid;
	int		m_TotalResource;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ResourcePage)
	protected:
	virtual void OnOK( ); 
	virtual void OnCancel( ); 
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ResourcePage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEliteResource();
	afx_msg void OnButtonFullResource();
	afx_msg void OnButtonResetResource();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESOURCEPAGE_H__1C53DCFD_0C6A_4ED6_8703_9BD823C8AE97__INCLUDED_)
