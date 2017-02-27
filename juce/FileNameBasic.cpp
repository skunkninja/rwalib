#include "FileNameBasic.h"
#include "rw_WinMacBasic.h"
#include "String2Words.h"

void FileNameToPath(String *filename)
{
	wchar_t tmpFileName[512];
#if JUCE_WINDOWS
	filename->copyToUTF16(tmpFileName,sizeof(tmpFileName));
#endif
#if JUCE_MAC
    filename->copyToUTF32(tmpFileName,sizeof(tmpFileName));
#endif
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
		*filename = "";
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
	savepath = savepath.replace("\\", "/", true);
	currentPath = currentPath.replace("\\", "/", true);
	*filename = tmpFileName.replace("\\", "/", true);
	if(savepath != currentPath)
	{
		if(filename->startsWithIgnoreCase(savepath))
		{
			tmpFileName = filename->replace(savepath,currentPath,true);
			*filename = tmpFileName;
		}
	}
    wchar_t *ptmpPathName;
    ptmpPathName = new wchar_t [_MAX_PATH];
    
	rw_wcscpy(ptmpPathName, _MAX_PATH, filename->toWideCharPointer());

    FormatPath(ptmpPathName);
    *filename = ptmpPathName;

    delete [] ptmpPathName;
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

void MixFilePath(wchar_t *target,wchar_t *sourcePath,const wchar_t *newPath)
{
	//if newPath start with '\' or '/' - Absolute path 1.
	//else if newPath start with X:/ or X:\ - Absoulte path 2.
	//else 

	int i;
	if((wcslen(newPath)!=0) && ((newPath[0] == '\\') || newPath[0] == '/'))
	{
		if((wcslen(newPath) > 1) && ((newPath[1] == '\\') || (newPath[1] == '/')))
		{
			return;//not support.
		}
        //disk label(C: D: etc) in source path
        if(wcslen(sourcePath)>1 && (sourcePath[1] == ':'))
        {
		    target[0]=sourcePath[0];
		    target[1]=sourcePath[1];
		    target[2]='\0';
		    rw_wcscat(target,_MAX_PATH,newPath);
        }
        else
        {
			rw_wcscpy(target, _MAX_PATH, newPath);//use new path, use current work path.
        }
	}
	else if(newPath[1] == ':')
	{
		if((newPath[2] == '/') || (newPath[2] == '\\'))
		{
			rw_wcscpy(target,_MAX_PATH,newPath);
		}
	}
	else
	{
		rw_wcscpy(target, _MAX_PATH, sourcePath);
        i = wcslen(target)-1;
        if((sourcePath[i] != '/') && (sourcePath[i] != '\\'))
        {
			rw_wcscat(target, _MAX_PATH, L"/");
        }

		rw_wcscat(target, _MAX_PATH, newPath);
	}
    FormatPath(target);
}

void FormatPath(wchar_t *sourcePath)
{
    wchar_t *workPath;
    int i;
    const wchar_t notAvaiableInPath[] = {L"\\/:?\"<>|"};
    int iMaxLen = _MAX_PATH;

    if((i = wcslen(sourcePath)) == 0)
    {
        return;
    }
    workPath = sourcePath;

    if((sourcePath[0] == '/') || (sourcePath[0] == '\\'))
    {
        if((i > 1) && ((sourcePath[1] == '/') || (sourcePath[1] == '\\')))
        {
            return;//  start with "\\" or "//" not support.
        }
        workPath++; iMaxLen--;
    }
    else if(i > 1)
    {
        if(sourcePath[1] == ':')
        {
            if((i > 2) && ((sourcePath[2] == '/') || (sourcePath[2] == '\\')))
            {
                workPath+=3;//skip 
                iMaxLen -=3;
            }
            else
            {
                workPath+=2;//skip 
                iMaxLen -= 2;
            }
        }
    }

    if(wcschr(notAvaiableInPath,*workPath))
    {
        return;
    }
    //
    //work path now start with avaiable chars in path.
    //
    CString2Words pathWords;
    pathWords.SetString(workPath);
    pathWords.SetCommentCharString(L"");
    pathWords.SetNotInCharString(notAvaiableInPath);

    int iWords = pathWords.SplitWords(0);
    pathWords.SplitWords(iWords);

    int *pNeedCopy = new int[iWords];
    memset(pNeedCopy, 0, sizeof(int)*iWords);

    int iLastCopy = 0;
    for(i = 0;i < iWords;i++)
    {
        if(wcscmp(pathWords.GetWord(i),L".") == 0)
        {
            // . -> current path, do nothing.
        }
        else if(wcscmp(pathWords.GetWord(i),L"..") == 0)
        {
            if(iLastCopy > 0)
            {
                iLastCopy--;
                pNeedCopy[iLastCopy] = i;
            }
            else
            {
                delete [] pNeedCopy;
                return; //error. no change.
            }
        }
        else
        {
            pNeedCopy[iLastCopy] = i;
            iLastCopy++;
        }
    }

    rw_wcscpy(workPath,iMaxLen,L"");
    for(i = 0;i < iLastCopy;i++)
    {
        if(i != 0)
        {
            rw_wcscat(workPath,iMaxLen,L"/");
        }
        rw_wcscat(workPath,iMaxLen,pathWords.GetWord(pNeedCopy[i]));
    }
    delete [] pNeedCopy;
}
