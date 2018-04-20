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
// Module: TestBoostRegEx.cpp
// Creator: xushiwei
// Email: xushiweizh@gmail.com
// Date: 2006-8-18 13:57:28
// 
// $Id: TestBoostRegEx.cpp,v 1.8 2006/12/03 07:52:55 xushiwei Exp $
// -----------------------------------------------------------------------*/

//#define USES_BOOST
#include <stdext/Basic.h>

// -------------------------------------------------------------------------
// testBoost

#if defined(USES_BOOST)

#include <boost/regex.hpp>

void testBoostRegEx()
{
}

#else

void testBoostRegEx()
{
}

#endif

WINX_AUTORUN(testBoostRegEx);

// -------------------------------------------------------------------------
// $Log: TestBoostRegEx.cpp,v $
//
