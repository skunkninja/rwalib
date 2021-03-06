﻿#include "rw_WinMacBasic.h"
#include "string.h"

#if JUCE_WINDOWS || WIN32
#include <Windows.h>
#endif

#if (JUCE_MAC || JUCE_IOS)
#include <mach-o/dyld.h>
#endif

#if (JUCE_WINDOWS || JUCE_MAC || JUCE_IOS)
using namespace juce;
#endif

wchar_t *rw_wcscpy(wchar_t *ptarget, int wcharsize, const wchar_t *source)
{
#if JUCE_WINDOWS || WIN32
	if (wcscpy_s(ptarget, wcharsize, source) == 0)
	{
		return ptarget;
	}
	else
	{
		return nullptr;
	}
#endif
#if (JUCE_MAC || JUCE_IOS)
    return wcscpy(ptarget, source);
#endif
}


wchar_t *rw_wcscat(wchar_t *ptarget, int wcharsize, const wchar_t *source)
{
#if JUCE_WINDOWS|| WIN32
	if (wcscat_s(ptarget, wcharsize, source) == 0)
	{
		return ptarget;
	}
	else
	{
		return nullptr;
	}
#endif
#if (JUCE_MAC || JUCE_IOS)
    return wcscat(ptarget, source);
#endif
}


FILE *rw_fopen(const wchar_t *filename, const wchar_t *mode)
{
#if JUCE_WINDOWS|| WIN32
    FILE *fp;
	if (_wfopen_s(&fp, filename, mode) == 0)
	{
		return fp;
	}
	else
	{
		return nullptr;
	}
#endif
#if (JUCE_MAC || JUCE_IOS)
	String strname(filename);
	String strmode(mode);

	return fopen(strname.toUTF8(), strmode.toUTF8());
#endif
}

void rw_GetExecutablePath(wchar_t *appname, int maxbuf)
{
#if JUCE_WINDOWS || WIN32
	GetModuleFileName(NULL, appname, maxbuf);
#endif
#if (JUCE_MAC || JUCE_IOS)
	char buf[0];
	uint32_t size = 0;
	int res = _NSGetExecutablePath(buf, &size);

	char* tmppath = (char*)malloc(size + 1);
	tmppath[size] = 0;
	res = _NSGetExecutablePath(tmppath, &size);

	String tmppathstring = String::fromUTF8(tmppath);
	//tmppathstring.copyToWchar_t(workPath, sizeof(workPath));
	rw_wcscpy(appname, maxbuf, tmppathstring.toWideCharPointer());
	delete[] tmppath;
#endif
}
