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
// Module: winx/APIHook.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2007-2-9 14:11:46
// 
// $Id: $
// -----------------------------------------------------------------------*/
#ifndef WINX_APIHOOK_H
#define WINX_APIHOOK_H

#ifndef WINX_BASIC_H
#include "Basic.h"
#endif

// -------------------------------------------------------------------------
// Reference Win32 SDK

#if defined(WINX_USE_WINSDK)

#ifndef _INC_TOOLHELP32
#include "../../../winsdk/include/tlhelp32.h"
#endif

#else

#ifndef _INC_TOOLHELP32
#include <tlhelp32.h>
#endif

#endif

EXTERN_C WINBASEAPI BOOL WINAPI IsDebuggerPresent(VOID);

NS_WINX_BEGIN

// -------------------------------------------------------------------------

typedef struct _APIHOOK32_ENTRY
{
	LPCSTR     pszAPIName;			//API����
	LPCSTR     pszCallerModuleName;	//�����õ�ģ����
	PROC       pfnOriginApiAddress;	//ԭʼ�ĺ�����ַ
	PROC       pfnDummyFuncAddress;	//�µĺ�����ַ
	HMODULE    hModCallerModule;		//���õ�ģ����
}APIHOOK32_ENTRY, *PAPIHOOK32_ENTRY;

// -------------------------------------------------------------------------
// _LocationIAT

//���У�hModuleΪ����ģ������szImportModΪ��������ơ�
inline PIMAGE_IMPORT_DESCRIPTOR winx_call _LocationIAT(HMODULE hModule, LPCSTR szImportMod)
{
	PIMAGE_DOS_HEADER pDOSHeader;
	PIMAGE_NT_HEADERS pNTHeader;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc;

	//����Ƿ�ΪDOS�������Ƿ���NULL����DOS����û��IAT��
	pDOSHeader = (PIMAGE_DOS_HEADER) hModule;
	if(pDOSHeader->e_magic != IMAGE_DOS_SIGNATURE)
		return NULL;

	//����Ƿ�ΪNT��־�����򷵻�NULL��
	pNTHeader = (PIMAGE_NT_HEADERS)((DWORD)pDOSHeader+ (DWORD)(pDOSHeader->e_lfanew));
	if(pNTHeader->Signature != IMAGE_NT_SIGNATURE)
		return NULL;

	//û��IAT���򷵻�NULL��
	if(pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress == 0)
		return NULL;

	//��λ��һ��IATλ�á�
	pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD)pDOSHeader + (DWORD)(pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress));
	//�������������ѭ��������е�IAT����ƥ���򷵻ظ�IAT��ַ����������һ��IAT��
	while (pImportDesc->Name)
	{
		//��ȡ��IAT��������������ơ�
		PSTR szCurrMod = (PSTR)((DWORD)pDOSHeader + (DWORD)(pImportDesc->Name));
		if (_stricmp(szCurrMod, szImportMod) == 0)
			return pImportDesc;

		pImportDesc++;
	}
	return NULL;
}

//***************************************************************************************/
//       SetWindowsAPIHook    �ҽ�WindowsAPI����  phk->hModCallerModule != NULL         //
//***************************************************************************************//
inline BOOL winx_call SetWindowsAPIHook(const APIHOOK32_ENTRY* phk)
{
	WINX_ASSERT(
		phk->pszAPIName &&
		phk->pszCallerModuleName &&
		phk->pfnOriginApiAddress &&
		phk->hModCallerModule
		);

	PIMAGE_THUNK_DATA	pThunk;
	PIMAGE_IMPORT_DESCRIPTOR pImportDesc = _LocationIAT(phk->hModCallerModule, phk->pszCallerModuleName);
	if (pImportDesc == NULL)
		return FALSE;

	//Ѱ��������Ҫ�ĺ���
	pThunk = (PIMAGE_THUNK_DATA) ((PBYTE)phk->hModCallerModule+pImportDesc->FirstThunk);//IAT
	for (; pThunk->u1.Function; pThunk++)
	{
		//ppfn��¼����IAT������Ӧ�ĺ����ĵ�ַ
		PROC * ppfn= (PROC *)&pThunk->u1.Function;
		if (*ppfn == phk->pfnOriginApiAddress)
		{
#if (0)
			return ::WriteProcessMemory(
				::GetCurrentProcess(),
				ppfn, &phk->pfnDummyFuncAddress, sizeof(*ppfn), NULL);
#else
            MEMORY_BASIC_INFORMATION mbi;
            ::VirtualQuery(ppfn, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
            ::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect);
			try
			{
				*ppfn = phk->pfnDummyFuncAddress;
			}
			catch (...)
			{
				WINX_REPORT("SetWindowsAPIHook failed.");
				return FALSE;
			}
            ::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);
			return TRUE;
#endif
		}

#ifdef _DEBUG
		{{
			//!!! ע�⣺��Win9X���Ի����£�����ƥ�������Ҫ���⴦��
			//ʵ���ϣ�Release��Ҳ���ܻᱻ���ԣ���������������Ͳ�֧���ˡ�
			//ע�⣺��Win9Xϵͳ�У�_SetApiHookUp()��֧�ֵ��Ի����µ�Release��
			// Get the Windows version.
			DWORD dwVersion = GetVersion();
			DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
			DWORD dwWindowsMinorVersion =  (DWORD)(HIBYTE(LOWORD(dwVersion)));

			if (dwVersion < 0x80000000)              // Windows NT/2000/XP
			{
			}
			else if (dwWindowsMajorVersion < 4)      // Win32s
			{
			}
			else                                     // Windows 95/98/Me
			{
				if (IsDebuggerPresent())
				{
					//push, jmp
					if (memcmp(*ppfn, phk->pfnOriginApiAddress, 6) == 0)
					{
						if ((DWORD)(*ppfn) + *(LPDWORD)((LPBYTE)(*ppfn) + 6) == (DWORD)(phk->pfnOriginApiAddress) + *(LPDWORD)((LPBYTE)(phk->pfnOriginApiAddress) + 6))	// +10
						{
							WriteProcessMemory(
								GetCurrentProcess(),ppfn,
								(void*)&(phk->pfnDummyFuncAddress),sizeof(phk->pfnDummyFuncAddress),NULL);
							return TRUE;
						}
					}
				}
			}
		}}
#endif //_DEBUG

	}
	return FALSE;
}

inline BOOL winx_call UnhookWindowsAPI(PAPIHOOK32_ENTRY lphk)
{
	WINX_ASSERT(
		lphk->hModCallerModule != NULL);

	PROC temp;
	temp = lphk->pfnOriginApiAddress;
	lphk->pfnOriginApiAddress = lphk->pfnDummyFuncAddress;
	lphk->pfnDummyFuncAddress = temp;
	return SetWindowsAPIHook(lphk);
}

//***************************************************************************************/
//       SetWindowsAPIHookEx  �ҽ�WindowsAPI����  ��phk->hModCallerModule == NULL        //
//                                               ��������ϵͳ�ڹҽӺ���                 //
//                            ����SetWindowsHookEx ����                                  //
//***************************************************************************************//
inline BOOL winx_call SetWindowsAPIHookEx(const APIHOOK32_ENTRY* phk)
{
	WINX_ASSERT(
		phk->pszAPIName &&
		phk->pszCallerModuleName &&
		phk->pfnOriginApiAddress
		);

	if (phk->hModCallerModule == NULL)
	{
		HMODULE& hModCallerModule = (HMODULE&)phk->hModCallerModule;
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,0);
		MODULEENTRY32 me = { sizeof(MODULEENTRY32) };
		BOOL bOk = Module32First(hSnapshot,&me);
		while (bOk)
		{
			hModCallerModule = me.hModule;
#if defined(_DEBUG)
			if (SetWindowsAPIHook(phk))
				WINX_TRACE(_T(">>> SetWindowsAPIHookEx - hook %s success.\n"), me.szModule);
#else
			SetWindowsAPIHook(phk);
#endif
			bOk = Module32Next(hSnapshot,&me);
		}
		
		hModCallerModule = NULL;
		::CloseHandle(hSnapshot);
		return TRUE;
	}
	else
	{
		return SetWindowsAPIHook(phk);
	}
}

inline BOOL winx_call UnhookWindowsAPIEx(PAPIHOOK32_ENTRY lphk)
{
	PROC temp;
	temp = lphk->pfnOriginApiAddress;
	lphk->pfnOriginApiAddress = lphk->pfnDummyFuncAddress;
	lphk->pfnDummyFuncAddress = temp;
	return SetWindowsAPIHookEx(lphk);
}

// -------------------------------------------------------------------------
// ValidatePrototype

template <class A, class B>
inline BOOL winx_call ValidatePrototype(A, B)
{
	WINX_ASSERT_DERIVE(A, B);
	return TRUE;
}

// -------------------------------------------------------------------------
// WINX_API_HOOK, WINX_CALL_ORGAPI

#ifndef WINX_API_HOOK
#define WINX_API_HOOK(Module, APIName)										\
public:																		\
	static PROC winx_call GetAPI() { return (PROC)::APIName; }				\
	static LPCSTR winx_call GetAPIName() { return #APIName; }				\
	static LPCSTR winx_call GetModuleName()	{ return Module; }				\
	static BOOL winx_call Validate() {										\
		return winx::ValidatePrototype(::APIName, Process);					\
	}																		\
	template <class Prototype>												\
	static Prototype winx_call GetOrgAPI(Prototype) {						\
		return (Prototype)pe.pfnOriginApiAddress;							\
	}																		\
private:
#endif

#ifndef WINX_CALL_ORGAPI
#define WINX_CALL_ORGAPI		GetOrgAPI(Process)
#endif

// -------------------------------------------------------------------------
// class APIHook

template <class HookClass>
class APIHook
{
public:
	static APIHOOK32_ENTRY pe;

public:
	static BOOL winx_call Hook(HINSTANCE hInst)
	{
		WINX_ASSERT(hInst != NULL);
		WINX_ASSERT(
			HookClass::Validate()
			);
		pe.pszAPIName = HookClass::GetAPIName();
		pe.pszCallerModuleName = HookClass::GetModuleName();
		pe.pfnOriginApiAddress = (PROC)HookClass::GetAPI();
		pe.pfnDummyFuncAddress = (PROC)HookClass::Process;
		pe.hModCallerModule = hInst;
		return SetWindowsAPIHook(&pe);
	}

	static VOID winx_call Unhook()
	{
		UnhookWindowsAPI(&pe);
	}

	static BOOL winx_call HookGlobal()
	{
		WINX_ASSERT(
			HookClass::Validate()
			);
		pe.pszAPIName = HookClass::GetAPIName();
		pe.pszCallerModuleName = HookClass::GetModuleName();
		pe.pfnOriginApiAddress = (PROC)HookClass::GetAPI();
		pe.pfnDummyFuncAddress = (PROC)HookClass::Process;
		pe.hModCallerModule = NULL;
		return SetWindowsAPIHookEx(&pe);
	}

	static VOID winx_call UnhookGlobal()
	{
		UnhookWindowsAPIEx(&pe);
	}
};

template <class HookClass>
APIHOOK32_ENTRY APIHook<HookClass>::pe;

// -------------------------------------------------------------------------
// class APIHookEx

#if (0) //@@todo

template <class HookClass>
class APIHookEx : public APIHook<HookClass>
{
public:
	class LoadLibraryAHook : public APIHook<LoadLibraryAHook>
	{
		WINX_API_HOOK("kernel32.dll", LoadLibraryA)
	public:
		static HMODULE WINAPI Process(LPCSTR lpLibFileName)
		{
			HMODULE hMod = WINX_CALL_ORGAPI(lpLibFileName);
			APIHookEx<HookClass>::HookGlobalEx();
			return hMod;
		}
	};

	class LoadLibraryWHook : public APIHook<LoadLibraryWHook>
	{
		WINX_API_HOOK("kernel32.dll", LoadLibraryW)
	public:
		static HMODULE WINAPI Process(LPCWSTR lpLibFileName)
		{
			HMODULE hMod = WINX_CALL_ORGAPI(lpLibFileName);
			APIHookEx<HookClass>::HookGlobalEx();
			return hMod;
		}
	};

public:
	static BOOL winx_call HookGlobalEx()
	{
		LoadLibraryAHook::HookGlobal();
		LoadLibraryWHook::HookGlobal();
		return HookGlobal();
	}

	static VOID winx_call UnhookGlobalEx()
	{
		LoadLibraryAHook::UnhookGlobal();
		LoadLibraryWHook::UnhookGlobal();
		UnhookGlobal();
	}
};

#endif

// -------------------------------------------------------------------------
// class AutoAPIHook

template <class HookClass>
class AutoAPIHook : public APIHook<HookClass>
{
public:
	AutoAPIHook(HINSTANCE hInst = GetThisModule())
		{ Hook(hInst); }
	~AutoAPIHook()
		{ Unhook(); }
};

// -------------------------------------------------------------------------
// class AutoAPIHookGlobal

template <class HookClass>
class AutoAPIHookGlobal : public APIHook<HookClass>
{
public:
	AutoAPIHookGlobal()
		{ HookGlobal(); }
	~AutoAPIHookGlobal()
		{ UnhookGlobal(); }
};

// -------------------------------------------------------------------------
// $Log: $

NS_WINX_END

#endif /* WINX_APIHOOK_H */
