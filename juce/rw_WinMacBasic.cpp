#include "rw_WinMacBasic.h"
#include "string.h"

using namespace juce;

wchar_t *rw_wcscpy(wchar_t *ptarget, int maxbuf, const wchar_t *source)
{
    return wcscpy(ptarget, source);
}


wchar_t *rw_wcscat(wchar_t *ptarget, int maxbuf, const wchar_t *source)
{
    return wcscat(ptarget, source);
}


FILE *rw_fopen(const wchar_t *filename, const wchar_t *mode)
{
	FILE *fp;
#if JUCE_WINDOWS
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