#ifndef TEXT_FILE_ENUM_HEAD
#define TEXT_FILE_ENUM_HEAD

#include <stdio.h>

#define DEFAULT_MAX_LINE_LEN  512//max chars in one line.

enum
{
	ERROR_TEXT_LINE_LONG = 1,
	ERROR_FILE_READ,
	ERROR_FILE_END,
};

class CTextFileEnum
{
public:
	CTextFileEnum();
	~CTextFileEnum();

	int LoadTextFile(const wchar_t *unicodeFileName);
	void SetMaxLineLen(int newMaxLen);
	int GetNextLine(char **ppLineBuf);
	void ResetPos();
protected:
	int iMaxLineLength;
	FILE *fpText;
};


#endif