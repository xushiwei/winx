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
// Date: 2006-8-22 16:44:16
// 
// $Id: hello.cpp,v 1.1 2006/08/22 09:02:17 xushiwei Exp $
// -----------------------------------------------------------------------*/

#include <winx.h>
#include "resource.h"

// -------------------------------------------------------------------------
// class CMyEdit - ʹ�����໯(Subclass)����

class CMyEdit : public winx::Edit<CMyEdit>
{
public:
	VOID OnContextMenu(HWND hWnd, winx::CPoint pt)
	{
		//��ֹ���Ҽ��˵�...
	}
};

// -------------------------------------------------------------------------
// class CMyEdit2 - ʹ�ó��໯(Superclass)����

//ע��:
//	ʵ����һ�������ͬʱ���ǳ��࣬Ҳ�����࣬��ȫû�б�Ҫ��CMyEdit2��
//	��������Ϊ��չʾ���໯�����ͳ��໯�����Ĳ�ͬ�㣬ʹ���������ࡣ

class CMyEdit2 : public winx::Edit<CMyEdit2>
{
	WINX_CLASS("MyEdit");
public:
	VOID OnContextMenu(HWND hWnd, winx::CPoint pt)
	{
		//��ֹ���Ҽ��˵�...
	}
};

// -------------------------------------------------------------------------
// CHelloDlg

class CHelloDlg : public winx::ModelDialog<CHelloDlg, IDD_HELLO>
{
public:
	BOOL OnInitDialog(HWND hDlg, HWND hWndDefaultFocus)
	{
		CMyEdit::DoSubclassDlgItem(hDlg, IDC_EDIT1);
		return TRUE;
	}
};

// -------------------------------------------------------------------------

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
	CMyEdit2::RegisterClass();
	CHelloDlg dlg;
	dlg.DoModal();
	return 0;
}

// -------------------------------------------------------------------------
// $Log: hello.cpp,v $
// Revision 1.1  2006/08/22 09:02:17  xushiwei
// tutorial-004: Demonstrate how to implement user-defined controls
//
