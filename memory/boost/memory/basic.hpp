//
//  boost/memory/basic.hpp
//
//  Copyright (c) 2004 - 2008 xushiwei (xushiweizh@gmail.com)
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/libs/memory/index.htm for documentation.
//
#ifndef BOOST_MEMORY_BASIC_HPP
#define BOOST_MEMORY_BASIC_HPP

// -------------------------------------------------------------------------

#if !defined(_NEW_) && !defined(_NEW)
#include <new>	// new
#endif

#if !defined(_CSTDIO_) && !defined(_CSTDIO)
#include <cstdio>
#endif

#if !defined(_INC_MALLOC) && !defined(_MALLOC_H)
#include <malloc.h>	// _alloca
#endif

#ifndef BOOST_DETAIL_DEBUG_HPP
#include <boost/detail/debug.hpp>
#endif

#pragma pack() // default pack
#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif
// warning: identifier was truncated to '255' characters in the debug information

// =========================================================================

#ifndef BOOST_MEMORY_CALL
#define BOOST_MEMORY_CALL
#endif

// -------------------------------------------------------------------------

#ifndef NS_BOOST_MEMORY_BEGIN
#define NS_BOOST_MEMORY_BEGIN	namespace boost { namespace memory {
#define NS_BOOST_MEMORY_END		} }
#define NS_BOOST_MEMORY			boost::memory
#endif

// -------------------------------------------------------------------------

#ifndef BOOST_MEMORY_ASSERT
#define BOOST_MEMORY_ASSERT(e)	BOOST_DETAIL_ASSERT(e)
#endif

#ifndef BOOST_MEMORY_STATIC_ASSERT
#if defined(BOOST_STATIC_ASSERT)
#define BOOST_MEMORY_STATIC_ASSERT(e)	BOOST_STATIC_ASSERT(e)
#else
#define BOOST_MEMORY_STATIC_ASSERT(e)	BOOST_DETAIL_ASSERT(e)
#endif
#endif

// -------------------------------------------------------------------------

#if defined(BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION)
	#define BOOST_MEMORY_NO_PARTIAL_SPECIALIZATION
#elif defined(_MSC_VER)
	#if (_MSC_VER <= 1200)
	#define BOOST_MEMORY_NO_PARTIAL_SPECIALIZATION
	#endif
#endif

#if defined(BOOST_NO_FUNCTION_TYPE_SPECIALIZATIONS)
	#define BOOST_MEMORY_NO_FUNCTION_TYPE_SPECIALIZATIONS
#elif defined(_MSC_VER)
	#if (_MSC_VER <= 1200)
	#define BOOST_MEMORY_NO_FUNCTION_TYPE_SPECIALIZATIONS
	#endif
#endif

// -------------------------------------------------------------------------

#if !defined(_MSC_VER)
#ifndef __forceinline
#define __forceinline inline
#endif
#ifndef __stdcall
#define __stdcall
#endif
#endif

// =========================================================================
// constructor_traits, destructor_traits

#ifndef BOOST_MEMORY_TYPE_TRAITS_HPP
#include "type_traits.hpp"
#endif

// =========================================================================
// NEW, NEW_ARRAY, ALLOC, ALLOC_ARRAY

NS_BOOST_MEMORY_BEGIN

class unmanaged_
{
public:
	template <class Type>
	__forceinline Type* BOOST_MEMORY_CALL operator->*(Type* p) const
	{
		return p;
	}
};

template <class AllocT>
class managed_
{
private:
	AllocT& m_alloc;

public:
	explicit managed_(AllocT& alloc) : m_alloc(alloc) {}

	template <class Type>
	__forceinline Type* BOOST_MEMORY_CALL operator->*(Type* p) const
	{
		m_alloc.manage(p, destructor_traits<Type>::destruct);
		return p;
	}
};

template <class AllocT>
__forceinline unmanaged_ BOOST_MEMORY_CALL get_managed__(AllocT& alloc, int fnZero)
{
	return unmanaged_();
}

template <class AllocT>
__forceinline 
managed_<AllocT> BOOST_MEMORY_CALL get_managed__(AllocT& alloc, destructor_t fn)
{
	return managed_<AllocT>(alloc);
}

NS_BOOST_MEMORY_END

#define BOOST_MEMORY_ALLOC(alloc, Type)					((Type*)(alloc).allocate(sizeof(Type)))
#define BOOST_MEMORY_ALLOC_ARRAY(alloc, Type, count)	((Type*)(alloc).allocate(sizeof(Type)*(count)))

#define BOOST_MEMORY_NEW_ARRAY(alloc, Type, count) 		NS_BOOST_MEMORY::array_factory<Type>::create(alloc, count)
#define BOOST_MEMORY_DESTRUCTOR(Type)					NS_BOOST_MEMORY::destructor_traits<Type>::destruct

#if defined(BOOST_MEMORY_NO_STRICT_EXCEPTION_SEMANTICS)
//
// Backward options:
// 	not strict in accord with normal C++ semantics but a bit faster
//
#define BOOST_MEMORY_NEW(alloc, Type)					\
	::new((alloc).allocate(sizeof(Type), BOOST_MEMORY_DESTRUCTOR(Type))) Type

#else

#define BOOST_MEMORY_UNMANAGED_ALLOC_(alloc, Type)		\
	::new((alloc).unmanaged_alloc(sizeof(Type), BOOST_MEMORY_DESTRUCTOR(Type))) Type

#define BOOST_MEMORY_GET_MANAGED_(alloc, Type)			\
	NS_BOOST_MEMORY::get_managed__(alloc, BOOST_MEMORY_DESTRUCTOR(Type))

#define BOOST_MEMORY_NEW(alloc, Type)					\
	BOOST_MEMORY_GET_MANAGED_(alloc, Type) ->* BOOST_MEMORY_UNMANAGED_ALLOC_(alloc, Type)

#endif

#define BOOST_MEMORY_UNMANAGED_NEW(alloc, Type)			\
	::new((alloc).allocate(sizeof(Type))) Type

// =========================================================================
// function enableMemoryLeakCheck

NS_BOOST_MEMORY_BEGIN

inline void BOOST_MEMORY_CALL enableMemoryLeakCheck()
{
#if defined(_MSC_VER)
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
#endif
}

NS_BOOST_MEMORY_END

// -------------------------------------------------------------------------
// function swap_object

NS_BOOST_MEMORY_BEGIN

inline void swap(void* a, void* b, size_t cb)
{
	void* t = _alloca(cb);
	memcpy(t, a, cb);
	memcpy(a, b, cb);
	memcpy(b, t, cb);
}

template <class Type>
void swap_object(Type* a, Type* b)
{
	swap(a, b, sizeof(Type));
}

NS_BOOST_MEMORY_END

// =========================================================================
// $Log: basic.hpp,v $

#endif /* BOOST_MEMORY_BASIC_HPP */
