#include "filenamebasic.h"
#include "rw_WinMacBasic.h"

void FileNameToPath(String *filename)
{
	wchar_t tmpFileName[_MAX_PATH];
	//filename->copyToWchar_t(tmpFileName,sizeof(tmpFileName));
	rw_wcscpy(tmpFileName, _MAX_PATH, filename->toWideCharPointer());
	int j;
	int i;
	j = filename->length()-1;

	for(;j>=0;j--)
	{
		if((tmpFileName[j] == '\\') || (tmpFileName[j] == '/'))
		{
			tmpFileName[j] = '\0';
			break;
		}
	}
	for(i = 0;i < j;i++)
	{
		if(tmpFileName[i] == '\\')
		{
			tmpFileName[i] = '/';
		}
	}
	if(j < 0)
	{
		*filename = File::getCurrentWorkingDirectory().getFullPathName();
	}
	else
	{
		*filename = tmpFileName;
	}
}

void ConvertFilePath(String *filename,String savepath,String currentPath)
{
	String tmpFileName;
	tmpFileName = *filename;
	*filename = tmpFileName.replace("\\","/",true);
	if(savepath != currentPath)
	{
		if(filename->startsWithIgnoreCase(savepath))
		{
			tmpFileName = filename->replace(savepath,currentPath,true);
			*filename = tmpFileName;
		}
	}
}

void FileNameAddStr(String *sourceFileName,String nameToAdd)
{
	wchar_t tmpFileName[_MAX_PATH];
	rw_wcscpy(tmpFileName, _MAX_PATH, sourceFileName->toWideCharPointer());

	bool bFindExt = false;
	int j;
	//int i;
	j = sourceFileName->length()-1;

	for(;j>=0;j--)
	{
		if((tmpFileName[j] == '\\') || (tmpFileName[j] == '/'))
		{
			if((tmpFileName[j] == '\\') || (tmpFileName[j] == '/'))
			{
				break;
			}
		}
		if(tmpFileName[j] == '.')
		{
			tmpFileName[j] = '\0';
			bFindExt = true;
			break;
		}
	}

	if(bFindExt)
	{
		*sourceFileName = tmpFileName;
		*sourceFileName += nameToAdd;
		tmpFileName[j] = '.';
		*sourceFileName += (tmpFileName+j);
	}
}