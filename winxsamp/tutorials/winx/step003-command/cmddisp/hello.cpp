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
// Date: 2006-8-22 13:52:52
// 
// $Id: hello.cpp,v 1.2 2006/08/22 08:06:59 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include "resource.h"

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
	WINX_CMDS_BEGIN()
		WINX_CMD(IDC_TODO, OnCmdTodo)
		WINX_CMD(ID_HELP_ABOUT, OnCmdAbout)
	WINX_CMDS_END();

public:
	VOID OnCmdAbout(HWND hWnd)
	{
		winx::SimpleDialog dlg;
		dlg.DoModal(hWnd, IDD_ABOUT);
	}

	VOID winx_msg OnCmdTodo(HWND hWnd)
	{
		winx::MsgBox(hWnd, _T("Todo\n"), _T("Information"));
	}
};

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.2  2006/08/22 08:06:59  xushiwei
// tutorial-003: Focus message - command
//
