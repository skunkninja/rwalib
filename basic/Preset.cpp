#include "Preset.h"
#include "stdio.h"
#include "rw_WinMacBasic.h"

CPreset::CPreset(void)
{
	presetLastSave = new PRESET_NODE;
	AllPreset = new PRESET_NODE* [MAX_PRESET];
    for(int i = 0;i < MAX_PRESET;i++)
    {
        AllPreset[i] = new PRESET_NODE;
    }
	rw_wcscpy(localIniName, _MAX_PATH, L"");
}

CPreset::~CPreset(void)
{
	delete presetLastSave;
    for(int i = 0;i < MAX_PRESET;i++)
    {
        delete AllPreset[i];
    }
	delete []AllPreset;
}

void CPreset::SaveIniFile()
{
	if(wcslen(localIniName) == 0)
	{
		return;
	}
	wchar_t localStr[50];
	int i;
	swprintf_s(localStr,50,L"%d",iLocalPreset);
	//WritePrivateProfileString(L"GlobleInfo",L"TotalPreset",localStr,localIniName);
	SaveOneNode(-1,presetLastSave);

	for(i = 0;i < iLocalPreset;i++)
	{
		SaveOneNode(i,*(AllPreset+i));
	}
}

//设定ini文件名称
int CPreset::SetIniFile(wchar_t *iniFileName)
{
	int i;
	wchar_t localStr[50];
	wcscpy_s(localIniName,_MAX_PATH,iniFileName);

	//GetPrivateProfileString(L"GlobleInfo",L"TotalPreset",L"0",localStr,50,localIniName);
	iLocalPreset = _wtoi(localStr);

	GetOneNode(L"LastSave",presetLastSave);

	iLocalPreset = GetTotalPreset();

	for(i = 0;i < iLocalPreset;i++)
	{
		GetOneNode(i,*(AllPreset+i));
	}
	return 0;
}

//从文件取出一个预设
//输入:
//    presetIndex:预设下标
//    presetSave:用于保存节点数据的节点指针
void CPreset::GetOneNode(int presetIndex,PRESET_NODE* presetSave)
{
	wchar_t localStr[50];
	swprintf_s(localStr,50,L"Preset%d",presetIndex);
	GetOneNode(localStr,presetSave);
}

//保存一个预设到文件
//输入:
//    presetIndex:预设下标
//    presetSave:预设节点数据
void CPreset::SaveOneNode(int presetIndex,PRESET_NODE* presetSave)
{
	wchar_t localStr[50];
	if(presetIndex < 0)
	{
		swprintf_s(localStr,50,L"LastSave");
	}
	else
	{
		swprintf_s(localStr,50,L"Preset%d",presetIndex);
	}
	SaveOneNode(localStr,presetSave);
}

//返回所有预设数目
int CPreset::GetTotalPreset()
{
	return iLocalPreset;
}

//增加一个预设
//以节点中的名字段用作区分
//如果节点名字为空,则保存到LastSave节点,即最后一次使用的值
//否则在已经有的节点中寻找,如果有重复的,则覆盖,没有重复的,则增加一个预设
//返回值:
//<0 :错误
//>=0:增加的节点位置(注意:如果节点名字为空,保存到LastSave,则返回值也为0,但调用者应该区分得清.)
int CPreset::AddPreset(void *voidpreset)
{
	int i;
	PRESET_NODE *presetData = (PRESET_NODE *)voidpreset;
	if(wcslen(presetData->strPresetName) == 0)
	{
		wcscpy_s(presetData->strPresetName,_MAX_PATH,L"LastSave");
		memcpy(presetLastSave,presetData,sizeof(PRESET_NODE));
		return 0;
	}

	for(i = 0;i < iLocalPreset;i++)
	{
		if(_wcsicmp(AllPreset[i]->strPresetName,presetData->strPresetName) == 0)
		{
			break;
		}
	}
	if(i == iLocalPreset)//not find same.
	{
		if(i >= MAX_PRESET)//has to the max preset number.
		{
			return -1;//error.
		}
		iLocalPreset++;

        memcpy(AllPreset[i],presetData,sizeof(PRESET_NODE));
        //--Add a new one, so we need sort all preset by name.
        
        for(i = 0;i < iLocalPreset-1;i++)
        {
            for(int j = i+1;j < iLocalPreset;j++)
            {
                if(_wcsicmp(AllPreset[i]->strPresetName,AllPreset[j]->strPresetName) > 0)
                {
                    PRESET_NODE *tmpSave;
                    tmpSave = AllPreset[i];
                    AllPreset[i] = AllPreset[j];
                    AllPreset[j] = tmpSave;
                }
            }
        }
	    for(i = 0;i < iLocalPreset;i++)
	    {
		    if(_wcsicmp(AllPreset[i]->strPresetName,presetData->strPresetName) == 0)
		    {
			    break;
		    }
	    }
	}
    else
    {
	    memcpy(AllPreset[i],presetData,sizeof(PRESET_NODE));
    }

	return i;//retun the new index the preset.
}

//删除一个预设
//返回值:
// 0  - 成功
// -1 - 失败
int CPreset::DelPreset(const wchar_t *strPresetName)
{
	int i;
	for(i = 0;i < iLocalPreset;i++)
	{
		if(_wcsicmp(AllPreset[i]->strPresetName,strPresetName) == 0)
		{
			break;
		}
	}
	if(i == iLocalPreset)//not find same.
	{
		return -1;//error.
	}
	for(;i < iLocalPreset-1;i++)
	{
		memcpy(AllPreset[i],AllPreset[i+1],sizeof(PRESET_NODE));
	}
	iLocalPreset--;
	return 0;
}

//获取预设
//返回值为预设节点指针
//NULL   - 失败
//非NULL - 预设节点
const void* CPreset::GetPreset(int presetIndex)
{
	if((presetIndex >= 0) && (presetIndex < iLocalPreset))
	{
		return *(AllPreset+presetIndex);
	}
	else if(presetIndex == -1)
	{
		return presetLastSave;
	}
	return NULL;
}