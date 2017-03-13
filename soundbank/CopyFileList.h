/*
  ==============================================================================

    CCopyFileList.h
    Created: 9 Jul 2016 4:50:06pm
    Author:  ZhouFeng

  ==============================================================================
*/

#ifndef COPYFILELIST_H_INCLUDED
#define COPYFILELIST_H_INCLUDED

#include "JuceHeader.h"

enum
{
    FILE_NOT_SAME,
    FILE_SAME_PATH_NAME,
    FILE_SAME_NAME_DIFFERENT_PATH,
};

typedef struct _FILE_NAME_NODE
{
    String fullName;
    String shortName;
}FILE_NAME_NODE;

class CCopyFileList :
	public std::vector <FILE_NAME_NODE>
{
public:
	CCopyFileList(void);
	~CCopyFileList(void);

    int AddFile(String fullName,String shortName = String::empty);
protected:
    int isSameNode(FILE_NAME_NODE &testNode,FILE_NAME_NODE &addNode);
    void ReleaseNodeMem(FILE_NAME_NODE *pNodeToRelease);
};


#endif  // CCOPYFILELIST_H_INCLUDED
