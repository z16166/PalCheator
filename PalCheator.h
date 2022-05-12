// PalCheator.h : main header file for the PALCHEATOR application
//

#if !defined(AFX_PALCHEATOR_H__0160FE1C_0918_4CDB_91D6_5CD9C3AF9C70__INCLUDED_)
#define AFX_PALCHEATOR_H__0160FE1C_0918_4CDB_91D6_5CD9C3AF9C70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPalCheatorApp:
// See PalCheator.cpp for the implementation of this class
//

class CPalCheatorApp : public CWinApp
{
public:
	CPalCheatorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPalCheatorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPalCheatorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PALCHEATOR_H__0160FE1C_0918_4CDB_91D6_5CD9C3AF9C70__INCLUDED_)
