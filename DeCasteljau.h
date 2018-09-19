// Bezier.h : main header file for the Bezier application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBezierApp:
// See Bezier.cpp for the implementation of this class
//

class CBezierApp : public CWinApp
{
public:
	CBezierApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBezierApp theApp;