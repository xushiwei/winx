// MfcTogetherDoc.h : interface of the CMfcTogetherDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MFCTOGETHERDOC_H__E83A5A6D_D2AB_47EF_A6D1_E1942EBEBC9D__INCLUDED_)
#define AFX_MFCTOGETHERDOC_H__E83A5A6D_D2AB_47EF_A6D1_E1942EBEBC9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMfcTogetherDoc : public CDocument
{
protected: // create from serialization only
	CMfcTogetherDoc();
	DECLARE_DYNCREATE(CMfcTogetherDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfcTogetherDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMfcTogetherDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMfcTogetherDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFCTOGETHERDOC_H__E83A5A6D_D2AB_47EF_A6D1_E1942EBEBC9D__INCLUDED_)
