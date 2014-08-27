#ifndef stdafx_h__
#define stdafx_h__

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#include <stdio.h>
#include <tchar.h>

#include <cpprest/basic_types.h>
#ifdef WIN32
#define NOMINMAX
#include <Windows.h>
#else
# include <sys/time.h>
#endif

#include <cpprest/http_listener.h>

#define TRACE					ucout 
#define TRACE_ACTION(a, k, v)	ucout << a << L" (" << k << L", " << v << L")\n"

#endif // stdafx_h__
