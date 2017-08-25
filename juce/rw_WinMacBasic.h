#ifndef STRING_BASIC_H
#define STRING_BASIC_H

#if JUCE_WINDOWS || JUCE_MAC
#include "../JuceLibraryCode/JuceHeader.h"
#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif
#else
#include <stdio.h>
#endif

wchar_t *rw_wcscpy(wchar_t *ptarget, int wcharsize, const wchar_t *source);
wchar_t *rw_wcscat(wchar_t *ptarget, int wcharsize, const wchar_t *source);
FILE *rw_fopen(const wchar_t *filename, const wchar_t *mode);

void rw_GetExecutablePath(wchar_t *appname, int maxbuf);
#endif
