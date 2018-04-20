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
// Date: 2006-8-23 13:33:08
// 
// $Id: hello.cpp,v 1.3 2006/08/23 05:35:11 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include "resource.h"

// -------------------------------------------------------------------------
// class CMyView

class CMyView : public winx::Window<CMyView>
{
	WINX_CLASS("MyView");

	WINX_ACCEL(IDR_ACCEL_MYVIEW);
	
	WINX_CMDS_BEGIN()
		WINX_CMD(ID_FILE_OPEN, OnOpen)
	WINX_CMDS_END();

public:
	VOID OnLButtonDown(HWND hWnd, UINT nFlags, winx::CPoint pt)
	{		
		::SetFocus(hWnd); //focus to support accelerators
	}
	VOID OnOpen(HWND hWnd)
	{
		winx::MsgBox(hWnd, _T("CMyView::Open"), _T("Information"));
	}
	VOID OnPaint(HWND hWnd)
	{
		winx::PaintDC dc(hWnd);
		dc.TextOut(1, 9, _T("The above control is RichEdit, and I am MyView."));
		dc.TextOut(1, 33, _T("Please set focus to me, and hit Ctrl+O."));
		dc.TextOut(1, 57, _T("For more information, hit F1."));
	}
};

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
	WINX_ACCELFRAME();
	WINX_ACCEL(IDR_ACCEL);

	WINX_CMDS_BEGIN_EX()
		WINX_CMD(ID_EDIT_UNDO, OnCmdUndo)
		WINX_CMD(ID_EDIT_REDO, OnCmdRedo)
		WINX_CMD(ID_HELP_ABOUT, OnCmdAbout)
	WINX_CMDS_END_EX();

public:
	BOOL TestTranslateAccel(HWND hWnd, LPMSG lpMsg)
	{
		if (::GetDlgCtrlID(::GetFocus()) == IDC_RICHEDIT1)
		{
			//�����ǰ��Edit�ؼ�����ֹת������Z����صĿ�ݼ�������Edit�Ϳ���֧��Ctrl+Z�ˡ�
			//���ǣ�TestTranslateAccelֻ�ܴ����ݼ������ڲ˵��е�Undo���������Ϊ���ˡ�
			if (lpMsg->message == WM_KEYDOWN && lpMsg->wParam == 'Z')
				return CannotTranslateAccel;
		}
		return CanTranslateAccel;
	}
	VOID OnCmdAbout(HWND hWnd)
	{
		winx::SimpleDialog dlg;
		dlg.DoModal(hWnd, IDD_ABOUT);
	}
	VOID OnCmdUndo(HWND hWnd)
	{
		//Undo��������壬����ͨ��TestTranslateAccel�����
		//�������ǲ������ķ��������ڲ˵��е�Undo���������Ϊ���ˡ�
		winx::MsgBox(hWnd, _T("CHelloDlg::Undo"), _T("Information"));
	}
	VOID OnCmdRedo(HWND hWnd)
	{
		//Redo�������ǲ���ForwardCommand��ʽ��
		//���ǱȽ������Ľ����������Ȼ���ӡ���Ϊ����Ҫ���þ���ؼ��ĺ�����
		winx::RichEditHandle edit = ::GetFocus();
		if (::GetDlgCtrlID(edit) == IDC_RICHEDIT1)
			edit.Redo();
		else
			winx::MsgBox(hWnd, _T("CHelloDlg::Redo"), _T("Information"));
	}
};

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CMyView::RegisterClass();
	InitCommonControls();
	InitRichEditControl();
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.3  2006/08/23 05:35:11  xushiwei
// tutorial-005: AMAZED Focus Message
//
