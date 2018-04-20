/* -------------------------------------------------------------------------
// WINX: a C++ template GUI library - MOST SIMPLE BUT EFFECTIVE
// 
// This file is a part of the WINX Library.
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php)
// which can be found in the file CPL.txt at this distribution. By using
// this software in any fashion, you are agreeing to be bound by the terms
// of this license. You must not remove this notice, or any other, from
// this software.
// 
// Module: hello.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-19 18:26:15
// 
// $Id: hello.cpp,v 1.1 2006/08/21 18:51:42 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelessDialog<CHelloDlg, IDD_HELLO>
{
};

// -------------------------------------------------------------------------
// CHelloMainFrame

class CHelloMainFrame : public winx::MainFrame<CHelloMainFrame>
{
	WINX_CLASS("CHelloMainFrame");

public:
	LRESULT OnCreate(HWND hWnd, LPCREATESTRUCT lpCS)
	{
		CHelloDlg* pDlg1 = WINX_NEW(CHelloDlg);
		CHelloDlg* pDlg2 = WINX_NEW(CHelloDlg);
		pDlg1->Create(hWnd);
		pDlg2->Create(hWnd, 200, 200);
		return 0;
	}

	void OnPaint(HWND hWnd)
	{
		winx::PaintDC dc(hWnd);
		dc.TextOut(1, 1, _T("Hello, WINX!"));
	}
};

// -------------------------------------------------------------------------
// WinMain

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	EnableMemoryLeakCheck(); // this is optional
	
	CHelloMainFrame::RegisterClass();
	CHelloMainFrame wndMain;
	wndMain.Create(NULL, _T("Hello"));

	return RunMsgLoop();
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
//
