#include "ArgProc.h"
#include <string.h>

CArgProc::CArgProc(void)
{
	workArg.strType = nullptr;
	workArg.strVal = nullptr;
	workArg.bHasValue = false;
	workArg.bIsArgument = false;
}

CArgProc::~CArgProc(void)
{
	if(workArg.strType != nullptr)
	{
		delete [] workArg.strType;
	}
}

void CArgProc::SetArgString(const wchar_t *pstr)
{
	int i;
	int argLength = wcslen(pstr);
	if(workArg.strType != nullptr)
	{
		delete [] workArg.strType;
		workArg.strType = nullptr;
	}

	workArg.strType = new wchar_t [argLength+1];
	wcscpy_s(workArg.strType,argLength+1,pstr);
	
	if(workArg.strType[0] == '-')
	{
		for(i = 0;i < argLength;i++)
		{
			if(workArg.strType[i] == ':')
			{
				workArg.strType[i] = '\0';
				workArg.strVal = workArg.strType+i+1;
				break;
			}
		}
		if(i == argLength)
		{
			workArg.bHasValue = false;
			workArg.strVal = nullptr;
		}
		else
		{
			workArg.bHasValue = true;
		}
		workArg.bIsArgument = true;
	}
	else
	{
		workArg.bHasValue = false;
		workArg.bIsArgument = false;
		workArg.strVal = nullptr;
	}
}

bool CArgProc::IsArgument()
{
	return workArg.bIsArgument;
}
wchar_t *CArgProc::GetArgument()
{
	return workArg.strType;
}
wchar_t *CArgProc::GetValue()
{
	if(workArg.bHasValue)
	{
		return workArg.strVal;
	}
	else
	{
		return nullptr;
	}
}
