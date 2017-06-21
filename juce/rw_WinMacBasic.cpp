#include "rw_WinMacBasic.h"
#include "string.h"

#if JUCE_WINDOWS
#include <Windows.h>
#endif

#if JUCE_MAC
#include <mach-o/dyld.h>
#endif

using namespace juce;

wchar_t *rw_wcscpy(wchar_t *ptarget, int wcharsize, const wchar_t *source)
{
#if JUCE_WINDOWS
	if (wcscpy_s(ptarget, wcharsize, source) == 0)
	{
		return ptarget;
	}
	else
	{
		return nullptr;
	}
#endif
#if JUCE_MAC
    return wcscpy(ptarget, source);
#endif
}


wchar_t *rw_wcscat(wchar_t *ptarget, int wcharsize, const wchar_t *source)
{
#if JUCE_WINDOWS
	if (wcscat_s(ptarget, wcharsize, source) == 0)
	{
		return ptarget;
	}
	else
	{
		return nullptr;
	}
#endif
#if JUCE_MAC
    return wcscat(ptarget, source);
#endif
}


FILE *rw_fopen(const wchar_t *filename, const wchar_t *mode)
{
#if JUCE_WINDOWS
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
#if JUCE_MAC
	String strname(filename);
	String strmode(mode);

	return fopen(strname.toUTF8(), strmode.toUTF8());
#endif
}

void rw_GetExecutablePath(wchar_t *appname, int maxbuf)
{
#if JUCE_WINDOWS
	GetModuleFileName(NULL, appname, maxbuf);
#endif
#if JUCE_MAC
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
