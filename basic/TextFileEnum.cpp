#include "TextFileEnum.h"
#include <string.h>
CTextFileEnum::CTextFileEnum()
{
	fpText = NULL;
	iMaxLineLength = DEFAULT_MAX_LINE_LEN;
//	pLineBuf = new char[iMaxLineLength];
}

CTextFileEnum::~CTextFileEnum()
{
	if(fpText != NULL)
	{
		fclose(fpText);
		fpText = NULL;
	}
//	if(pLineBuf != NULL)
//	{
//		delete [] pLineBuf;
//		pLineBuf = NULL;
//	}
}

//return 0 if open file OK.
//or return other.
int CTextFileEnum::LoadTextFile(const wchar_t *unicodeFileName)
{
	if(fpText != NULL)
	{
		fclose(fpText);
		fpText = NULL;
	}
	if(_wfopen_s(&fpText,unicodeFileName,L"rt") != 0)
	{
		return 1;
	}
	return 0;
}

//
void CTextFileEnum::SetMaxLineLen(int newMaxLen)
{
//	if(pLineBuf != NULL)
//	{
//		delete [] pLineBuf;
//		pLineBuf = NULL;
//	}
	iMaxLineLength = newMaxLen;
//	pLineBuf = new char[newMaxLen];
}

int CTextFileEnum::GetNextLine(char **ppLineBuf)
{
	if(*ppLineBuf == NULL)
	{
		*ppLineBuf  = new char[iMaxLineLength];
	}
	if(fgets(*ppLineBuf,iMaxLineLength,fpText)!=NULL)
	{
		if(strrchr(*ppLineBuf,int('\n'))==NULL&&!feof(fpText))
		{
			delete [] *ppLineBuf;
			*ppLineBuf = NULL;

			return ERROR_TEXT_LINE_LONG;
		}
		return 0;
	}
	else
	{
		delete [] *ppLineBuf;
		*ppLineBuf = NULL;

		if(!feof(fpText))
		{
			return ERROR_FILE_READ;
		}
		return ERROR_FILE_END;
	}
}

void CTextFileEnum::ResetPos()
{
	fseek(fpText,0,SEEK_SET);
}