//
//  FindFile.h
//  StaffProc
//
//  Created by zhoufeng on 15/5/25.
//  Copyright (c) 2015 RWA. All rights reserved.
//

#ifndef FIND_FILE_H
#define FIND_FILE_H

#include <stdio.h>
#include <vector>
#ifndef PATH_MAX
#define PATH_MAX  260
#endif

typedef struct
{
    char fileExt[PATH_MAX];
}fileFilter;

typedef struct
{
    char fileName[PATH_MAX];
}fileInfo;


//------
//class for call back
class CRWAFindFileCallBack
{
public:
    CRWAFindFileCallBack() {};
    virtual ~CRWAFindFileCallBack() {};
    friend class CRWAFindFile;
private:
    virtual void processFindFile(const char *fullName, const char *rawName) = 0;
};


class CRWAFindFile
{
public:
    CRWAFindFile(CRWAFindFileCallBack *callback);
    ~CRWAFindFile();
    void setFindPath(const char *newPath);
    //void setFindPath(wchar_t *newPath);
    
    void addFileExtFilter(const char *newFileExt, bool clearOthersFirst = false);
    int FindFile();
    void SearchFolder(bool folderOnly);
protected:
    char m_FindPath[PATH_MAX];
    CRWAFindFileCallBack *m_pCallBack;
    std::vector<fileFilter> fileExtList;
    bool m_IsSearchFile;//true  - search file
                      //false - search folder
};



#endif /* defined(__StaffProc__FindFile__) */
