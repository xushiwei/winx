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
// Module: stdext/archive/ReadArchive.h
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-11-29 19:27:08
// 
// $Id: ReadArchive.h,v 1.4 2006/12/14 09:15:04 xushiwei Exp $
// -----------------------------------------------------------------------*/
#ifndef STDEXT_ARCHIVE_READARCHIVE_H
#define STDEXT_ARCHIVE_READARCHIVE_H

#ifndef STDEXT_ARCHIVE_BASIC_H
#include "Basic.h"
#endif

#ifndef STDEXT_ARCHIVE_TEXT_H
#include "Text.h"
#endif

NS_STDEXT_BEGIN

// -------------------------------------------------------------------------
// class ReadArchive

template <class Handle, class StreamHandle, class CacheT = ArchiveCache>
class ReadArchive
{
private:
	typedef typename CacheT::allocator_type AllocT;

public:
	enum { endch = StreamHandle::endch };
	enum { cacheSize = CacheT::cacheSize };
	enum { roundSize = CacheT::roundSize };

	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef typename CacheT::allocator_type allocator_type;

   	typedef typename StreamHandle::int_type	int_type;
	typedef typename StreamHandle::char_type char_type;
	typedef typename StreamHandle::uchar_type uchar_type;

	typedef typename StreamHandle::pos_type	pos_type;
	typedef typename StreamHandle::off_type	off_type;
	
protected:
	char_type*	m_lpBufStart;
	size_type	m_nBufSize;
	char_type*	m_lpBufCur;
	char_type*	m_lpBufMax;

	StreamHandle m_handle;
	AllocT m_alloc;

private:
	ReadArchive(const ReadArchive&);
	void operator=(const ReadArchive&);
	
public:
	explicit ReadArchive(AllocT alloc = AllocT())
		: m_alloc(alloc)
	{
		m_lpBufStart = STD_ALLOC_ARRAY(alloc, char_type, cacheSize);
		m_nBufSize	 = cacheSize;
		m_lpBufCur	 = m_lpBufMax = m_lpBufStart;
		// ��״̬��m_lpBufMax - m_lpBufCur Ϊ�Ѿ�����Ԥ�������ݣ�
	}

	explicit ReadArchive(Handle hFile, pos_type pos = 0, AllocT alloc = AllocT())
		: m_alloc(alloc)
	{
		m_lpBufStart = STD_ALLOC_ARRAY(alloc, char_type, cacheSize);
		m_nBufSize	 = cacheSize;
		m_lpBufCur	 = m_lpBufMax = m_lpBufStart;
		m_handle.open_handle(hFile, pos);
	}

	explicit ReadArchive(LPCWSTR szFile, AllocT alloc = AllocT())
		: m_alloc(alloc)
	{
		m_lpBufStart = STD_ALLOC_ARRAY(alloc, char_type, cacheSize);
		m_nBufSize	 = cacheSize;
		m_lpBufCur	 = m_lpBufMax = m_lpBufStart;
		m_handle.open_to_read(szFile);
	}

	explicit ReadArchive(LPCSTR szFile, AllocT alloc = AllocT())
		: m_alloc(alloc)
	{
		m_lpBufStart = STD_ALLOC_ARRAY(alloc, char_type, cacheSize);
		m_nBufSize	 = cacheSize;
		m_lpBufCur	 = m_lpBufMax = m_lpBufStart;
		m_handle.open_to_read(szFile);
	}

	ReadArchive(const ReadArchive& rhs, bool fClone, AllocT alloc = AllocT())
		: m_alloc(alloc)
	{
		m_lpBufStart = STD_ALLOC_ARRAY(alloc, char_type, cacheSize);
		m_nBufSize	 = cacheSize;
		m_lpBufCur	 = m_lpBufMax = m_lpBufStart;
		m_handle.copy(rhs.m_handle);
	}

	~ReadArchive()
	{
		m_alloc.deallocate(m_lpBufStart, m_nBufSize);
	}

public:
	void winx_call clear_cache()
	{
		m_lpBufCur = m_lpBufMax = m_lpBufStart;
	}

	pos_type winx_call tell() const
	{
		return m_handle.tell() - (INT32)(m_lpBufMax - m_lpBufCur);
	}

	void winx_call seek(pos_type offset)
	{
		// ��״̬��m_lpBufMax - m_lpBufStart����m_nBufSize��������Ч���ݷ�Χ��
		// ??			--- m_lpBufStart
		// position		--- m_lpBufMax
		// offset		--- m_lpBufCur
		pos_type position = m_handle.tell();
		pos_type delta = position - offset;
		if (delta <= (pos_type)(INT)(m_lpBufMax - m_lpBufStart))
		{
			m_lpBufCur = m_lpBufMax - toUInt(delta);
		}
		else
		{
			m_handle.seek(static_cast<off_type>(offset), SEEK_SET);
			m_lpBufCur = m_lpBufMax = m_lpBufStart;
		}
	}

	void winx_call seek_end()
	{
		m_handle.seek(0, SEEK_END);
		m_lpBufCur = m_lpBufMax = m_lpBufStart;
	}

public:
	size_type winx_call skip(size_type nMax)
	{
		if (nMax + m_lpBufCur <= m_lpBufMax)
		{
			// 1���ڻ��������㹻ʱ���������ݲ�����
			m_lpBufCur  += nMax;
		}
		else
		{
			// 2���ڻ������ݲ���ʱ
			// ??			--- m_lpBufStart
			// position		--- m_lpBufMax		= �ļ�ָ���Ӧ��
			// offset		--- m_lpBufCur
			m_handle.seek(nMax - (m_lpBufMax - m_lpBufCur), SEEK_CUR);
			m_lpBufCur = m_lpBufMax = m_lpBufStart;
		}
		return nMax;
	}

private:
	const char_type* winx_call do_peek_(size_type nMax)
	{
		const size_type cbRest = m_lpBufMax - m_lpBufCur;
		if (nMax > m_nBufSize)
		{
			const size_type cbBufSize = ROUND(nMax, roundSize);
			char_type* lpNewBuf = STD_ALLOC_ARRAY(m_alloc, char_type, cbBufSize);
			copyMemory(lpNewBuf, m_lpBufCur, cbRest);
			m_alloc.deallocate(m_lpBufStart, m_nBufSize);
			m_lpBufStart = lpNewBuf;
			m_nBufSize = cbBufSize;
		}
		else
		{
			copyMemory(m_lpBufStart, m_lpBufCur, cbRest);
		}

		const size_type cb = m_handle.get(m_lpBufStart + cbRest, m_nBufSize - cbRest) + cbRest;
		
		m_lpBufCur = m_lpBufStart;
		m_lpBufMax = m_lpBufStart + cb;

		return cb < nMax ? NULL : m_lpBufStart;
	}

public:
	const char_type* winx_call peek(size_type nMax)
	{
		if (m_lpBufCur + nMax <= m_lpBufMax)
		{
			return m_lpBufCur;
		}
		else
		{
			return do_peek_(nMax);
		}
	}

	const char_type* winx_call get(size_type nMax)
	{
		if (m_lpBufCur + nMax <= m_lpBufMax)
		{
			const char_type* p = m_lpBufCur;
			m_lpBufCur += nMax;
			return p;
		}
		else
		{
			const char_type* p = do_peek_(nMax);
			if (p) {
				m_lpBufCur += nMax;
				return p;
			}
			return NULL;
		}
	}

	size_type winx_call get(char_type* lpBuf, size_type nMax)
	{
		WINX_ASSERT(lpBuf && m_lpBufStart && m_lpBufCur);
		
		// 1���ڻ��������㹻ʱ���������ݲ�����
		if (nMax + m_lpBufCur <= m_lpBufMax)
		{
			copyMemory(lpBuf, m_lpBufCur, nMax);
			m_lpBufCur += nMax;
			return nMax;
		}
		
		// 2���ڻ������ݲ���ʱ
		
		// nRead	--- ��������ֽ���
		// nMax		--- ʣ���ֽ���
		// nBlkRead --- �ļ������ķ���ֵ
		size_type nBlkRead, nRead;
		
		// a���ȶ��뻺���е�����
		nRead = (size_type)(m_lpBufMax - m_lpBufCur);
		copyMemory(lpBuf, m_lpBufCur, nRead);
		nMax -= nRead;
		m_lpBufCur = m_lpBufMax;
		
		// b����m_nBufSize�����������ݶ���
		size_type nTemp = nMax - nMax % m_nBufSize;
		if (nTemp)
		{
			nBlkRead = m_handle.get(lpBuf + nRead, nTemp);
			nRead += nBlkRead;
			if (nBlkRead < nTemp)
			{
				// �����Ѿ������ļ�ĩ�ˣ�
				return nRead;
			}
			nMax -= nTemp;
		}
		WINX_ASSERT(nMax < m_nBufSize);
		
		// c��ʣ����ȶ��뵽�������У���д��Ŀ���ڴ�
		nBlkRead	= m_handle.get(m_lpBufStart, m_nBufSize);
		m_lpBufMax	= (m_lpBufCur = m_lpBufStart) + nBlkRead;
		if (nBlkRead < nMax)
			nMax = nBlkRead;
		copyMemory(lpBuf + nRead, m_lpBufCur, nMax);
		m_lpBufCur += nMax;
		nRead += nMax;
		
		return nRead;
	}

	int_type winx_call get()
	{
		if (m_lpBufCur < m_lpBufMax)
			return (uchar_type)*m_lpBufCur++;

		char_type ch;
		return get(&ch, 1) ? (uchar_type)ch : (int_type)endch;
	}

	void winx_call reget(size_type offset, char_type* lpBuf, size_type cch)
	{
		WINX_ASSERT((int)offset >= 0 && offset >= cch);
		
		if (offset <= (size_type)(UINT)(m_lpBufCur - m_lpBufStart))
		{
			copyMemory(lpBuf, m_lpBufCur - offset, cch);
		}
		else
		{
			pos_type curpos = tell();
			seek(curpos - offset);
			get(lpBuf, cch);
			seek(curpos);
		}
	}

	int winx_call unget()
	{
		if (m_lpBufCur <= m_lpBufStart)
		{
			pos_type delta = m_nBufSize >> 2;
			pos_type current = tell();
			if (current < delta)
				delta = current;
			m_handle.seek(current-delta, SEEK_SET);
			m_lpBufMax = m_lpBufStart + m_handle.get(m_lpBufStart, m_nBufSize);
			m_lpBufCur = m_lpBufStart + toUInt(delta);

			if (m_lpBufCur <= m_lpBufStart)
				return 0;
		}
		--m_lpBufCur;
		return 1;
	}

	void winx_call unget(int_type ch)
	{
		if (ch != endch)
			unget();
	}

	int_type winx_call peek()
	{
		if (m_lpBufCur < m_lpBufMax)
			return (uchar_type)*m_lpBufCur;

		char_type ch;
		if (!get(&ch, 1))
			return endch;

		--m_lpBufCur;
		return (uchar_type)ch;
	}

public:
	HRESULT winx_call open(LPCWSTR szFile)
	{
		if (m_handle.good())
			return E_ACCESSDENIED;
		return m_handle.open_to_read(szFile);
	}

	HRESULT winx_call open(LPCSTR szFile)
	{
		if (m_handle.good())
			return E_ACCESSDENIED;
		return m_handle.open_to_read(szFile);
	}

	void winx_call open_handle(Handle hFile, pos_type pos = 0)
	{
		if (!m_handle.good())
			m_handle.open_handle(hFile, pos);
	}

	void winx_call close()
	{
		clear_cache();
		m_handle.close();
	}

	void winx_call attach(Handle hFile)
	{
		clear_cache();
		m_handle.attach(hFile);
	}

	Handle winx_call detach()
	{
		clear_cache();
		return m_handle.detach();
	}

public:
	pos_type winx_call size() const {
		return m_handle.size();
	}

	bool winx_call operator!() const {
		return m_handle.bad();
	}

	bool winx_call good() const {
		return m_handle.good();
	}

	bool winx_call bad() const {
		return m_handle.bad();
	}
	
public:
	void winx_call skipws() {
		NS_STDEXT_IO_TEXT::skipws(*this);
	}

	int_type winx_call getnws() {
		return NS_STDEXT_IO_TEXT::getnws(*this);
	}

	template <class StringT>
	size_type winx_call getline(StringT& s) {
		return NS_STDEXT_IO_TEXT::getline(*this, s);
	}

	template <class AllocT2>
	size_type winx_call getline(AllocT2& alloc, BasicString<char_type>& s) {
		return NS_STDEXT_IO_TEXT::getline(*this, alloc, s);
	}

public:
	template <class StringT>
	bool winx_call gets(StringT& s) {
		return NS_STDEXT_IO_BINARY::gets(*this, s);
	}
	
	template <class AllocT>
	bool winx_call gets(AllocT& alloc, BasicString<char>& s) {
		return NS_STDEXT_IO_BINARY::gets(*this, alloc, s);
	}
	
	template <class StringT>
	bool winx_call wgets(StringT& s) {
		return NS_STDEXT_IO_BINARY::wgets(*this, s);
	}
	
	template <class AllocT>
	bool winx_call wgets(AllocT& alloc, BasicString<UINT16>& s) {
		return NS_STDEXT_IO_BINARY::wgets(*this, alloc, s);
	}
};

// -------------------------------------------------------------------------
// $Log: ReadArchive.h,v $

NS_STDEXT_END

#endif /* STDEXT_ARCHIVE_READARCHIVE_H */
