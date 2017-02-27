#ifndef STRING_BASIC_H
#define STRING_BASIC_H

#include "../JuceLibraryCode/JuceHeader.h"

#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif

wchar_t *rw_wcscpy(wchar_t *ptarget, int maxbuf, const wchar_t *source);
wchar_t *rw_wcscat(wchar_t *ptarget, int maxbuf, const wchar_t *source);
FILE *rw_fopen(const wchar_t *filename, const wchar_t *mode);

void rw_GetExecutablePath(wchar_t *appname, int maxbuf);
#endif
