/*
  ==============================================================================

    CCopyFileList.cpp
    Created: 9 Jul 2016 4:50:06pm
    Author:  ZhouFeng

  ==============================================================================
*/

#include "CopyFileList.h"

CCopyFileList::CCopyFileList(void)
{
}


CCopyFileList::~CCopyFileList(void)
{
}


int CCopyFileList::isSameNode(FILE_NAME_NODE &testNode, FILE_NAME_NODE &addNode)
{
    if((testNode.fullName).compareIgnoreCase(addNode.fullName) == 0)
    {
        return FILE_SAME_PATH_NAME;
    }

    //full name is not same.

	if ((testNode.shortName).compareIgnoreCase(addNode.shortName) != 0)
    {
        return FILE_NOT_SAME;
    }

    //full name not same, but short name is same.
    return FILE_SAME_NAME_DIFFERENT_PATH;
}

int CCopyFileList::AddFile(String fullName, String shortName)
{
    FILE_NAME_NODE tmpNode;

	tmpNode.fullName = fullName;

    if(shortName.isEmpty())
    {
        File tmpFile(fullName);
		(tmpNode.shortName) = tmpFile.getFileName();
    }
    else
    {
		(tmpNode.shortName) = shortName;
    }

	int bHasSameName = FILE_NOT_SAME;

	for (iterator it=begin(); it != end(); it++)
	{
		bHasSameName = isSameNode((*it), tmpNode);
		if (bHasSameName != FILE_NOT_SAME)//Can't add to Link List.
		{
			return bHasSameName;
		}
	}
	push_back(tmpNode);
    return bHasSameName;
}