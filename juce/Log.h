#ifndef LOG_BASIC_H
#define LOG_BASIC_H


//产生log的类,可以指定log到文件,标准输出(Dos屏幕),编辑器,理论上可以同时.
#define LOG_FILE	1
#define LOG_STD_OUT	0

//使用编辑器类时,编辑器类要有一个Public函数AddLog(wchar_t *log).
#define LOG_EDITOR	1

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#if LOG_EDITOR
#include "LogEditor.h"
#endif

#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif

class CLog
{
public:
	CLog(void);
	~CLog(void);
	//增加一个标准的log串
	int AddLogString(wchar_t *newLog);
	//增加一个unicode格式串
	int AddFormatString(const wchar_t* _Format, ...);
	//增加一个ansi格式串
	int AddFormatString(const char* _Format, ...);
	//清除log(对文件log暂时无效果)
	void ClearLog();
#if LOG_EDITOR
	//当log记录为编辑框时,指定一个log编辑框类
	int SetEditor(CLogEditor *editor);
    int SetFontBold();
#endif

#if LOG_FILE
	//当log记录为文件时，指定一个log文件。
	int SetLogFileName(const wchar_t *filename,bool clearold);
	void CloseLogFile();
#endif
/////////////////////////////////////////////////////////////////
protected:
#if LOG_FILE
	//log文件名
	wchar_t FileName[_MAX_PATH];
	FILE *fpt;
#endif
#if LOG_EDITOR
	//log编辑器
	CLogEditor *localEditor;
#endif
};

#endif
