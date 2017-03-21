//
//  FindFile.cpp
//  StaffProc
//
//  Created by zhoufeng on 15/5/25.
//  Copyright (c) 2015 RWA. All rights reserved.
//

#include "FindFile.h"
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#if JUCE_MAC
#include <dirent.h>
#endif
CRWAFindFile::CRWAFindFile(CRWAFindFileCallBack *callback)
:m_pCallBack(callback),m_IsSearchFile(true)
{

}

CRWAFindFile::~CRWAFindFile()
{
    
}

void CRWAFindFile::setFindPath(const char *newPath)
{
    //wchar_t tmpPath[PATH_MAX];
    //mbstowcs(tmpPath, newPath, strlen(newPath));
    strcpy(m_FindPath, newPath);
}


void CRWAFindFile::addFileExtFilter(const char *newFileExt, bool clearOthersFirst)
{
    fileFilter tmpFilter;
    strcpy(tmpFilter.fileExt, newFileExt);
    
    if(clearOthersFirst)
    {
        fileExtList.clear();
    }
    
    fileExtList.push_back(tmpFilter);
}

void CRWAFindFile::SearchFolder(bool folderOnly)
{
    m_IsSearchFile = !folderOnly;
}

int CRWAFindFile::FindFile()
{
#if JUCE_MAC
    //scandir(const char *dirname, struct dirent ***namelist, int (*select)(const struct dirent *),
    //        int (*compar)(const struct dirent **, const struct dirent **));
    
    DIR *dirP = opendir(m_FindPath);
    dirent *dp;
    if (dirP == nullptr)
    {
        return -1;
    }
    
    while ((dp = readdir(dirP)) != NULL)
    {
        if (m_IsSearchFile)
        {
            if(dp->d_type == DT_REG)
            {
                unsigned long filterNumber = fileExtList.size();
                bool needprocess = false;
                if (filterNumber != 0) //need test file extension
                {
                    int i;
                    for (i = 0; i < filterNumber; i++) {
                        if(strcasecmp(dp->d_name+dp->d_namlen-strlen(fileExtList[i].fileExt),fileExtList[i].fileExt) == 0)
                        {
                            needprocess = true;
                            break;
                        }
                    }
                }
                else
                {
                    needprocess = true;
                }
                
                if (needprocess)
                {
                    char tmpFileName[PATH_MAX];
                    strcpy(tmpFileName, m_FindPath);
                    strcat(tmpFileName, dp->d_name);
                    m_pCallBack->processFindFile(tmpFileName, dp->d_name);
                }
                
            }
        }
        else
        {
            if(dp->d_type == DT_DIR) //folder not process any filter.
            {
                if (strcmp(dp->d_name,".") == 0)
                {
                    continue;
                }
                if (strcmp(dp->d_name,"..") == 0)
                {
                    continue;
                }
                char tmpFileName[PATH_MAX];
                strcpy(tmpFileName, m_FindPath);
                strcat(tmpFileName, dp->d_name);
                strcat(tmpFileName, "/");
                m_pCallBack->processFindFile(tmpFileName, dp->d_name);
            }
        }
    }
#endif    
    return 0;
}