#include "Log.h"
#if LOG_STD_OUT
#include "stdio.h"
#endif
#if (defined (_WIN32) || defined (_WIN64))
#include "windows.h"
#endif

#include "rw_WinMacBasic.h"

CLog::CLog(void)
{
#if LOG_FILE
	fpt = NULL;
#endif

#if LOG_EDITOR
	localEditor = NULL;
#endif
}

CLog::~CLog(void)
{
#if LOG_FILE
	if(fpt != NULL)
	{
		fclose(fpt);
	}
#endif
}

#if LOG_EDITOR
int CLog::SetEditor(CLogEditor *editor)
{
	localEditor = editor;
	return 0;
}
#endif

#if LOG_FILE
int CLog::SetLogFileName(const wchar_t *filename,bool clearold)
{
	if(fpt != NULL)
	{
		fclose(fpt);
		fpt = NULL;
	}
	rw_wcscpy(FileName,numElementsInArray(FileName),filename);
	if((fpt = rw_fopen(FileName,L"wt")) == NULL)
	{
		return 1;
	}
	return 0;
}

void CLog::CloseLogFile()
{
	if(fpt != NULL)
	{
		fclose(fpt);
		fpt = NULL;
	}
}
#endif

int CLog::AddLogString(wchar_t *newLog)
{
#if LOG_STD_OUT
	wprintf("%s",newLog);
#endif
#if LOG_FILE
	if(fpt != NULL)
	{
		fwprintf(fpt,L"%s",newLog);
	}
#endif
#if LOG_EDITOR
	if(localEditor != NULL)
	{
		int len = wcslen(newLog);
		wchar_t *pLog = new wchar_t[len+1];
		rw_wcscpy(pLog,len+1,newLog);
		localEditor->AddLog(pLog);
	}
#endif
	return 0;
}

int CLog::AddFormatString(const wchar_t* _Format, ...)
{
	wchar_t buff[1024];
	va_list vl;
	va_start (vl, _Format);
#if JUCE_WINDOWS
	_vsnwprintf_s(buff, numElementsInArray(buff),numElementsInArray(buff)-1, _Format, vl);
#endif
#if JUCE_MAC
    vswprintf(buff, numElementsInArray(buff), _Format, vl);
#endif
	return AddLogString(buff);
}

#if LOG_EDITOR
int CLog::SetFontBold()
{
	localEditor->setBold();
    return 0;
}
#endif

int CLog::AddFormatString(const char* _Format, ...)
{
	char buff[1024];
	wchar_t unicodebuf[1024];
	va_list vl;
	va_start (vl, _Format);

#if JUCE_WINDOWS
	_vsnprintf_s(buff, numElementsInArray(buff),numElementsInArray(buff)-1, _Format, vl);

	MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED,
		buff,
		strlen(buff)+1,
		unicodebuf,
		numElementsInArray(unicodebuf));
#endif
#if JUCE_MAC
    vsnprintf(buff, numElementsInArray(buff), _Format, vl);
    
    String tmpString = String::fromUTF8(buff);
    rw_wcscpy(unicodebuf, numElementsInArray(unicodebuf), tmpString.toWideCharPointer());
#endif
	return AddLogString(unicodebuf);
}

void CLog::ClearLog()
{
#if LOG_EDITOR
	if(localEditor != NULL)
	{
		localEditor->clear();
        localEditor->setFont(Font(14,0));
	}
#endif
}
