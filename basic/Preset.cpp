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

//�趨ini�ļ�����
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

//���ļ�ȡ��һ��Ԥ��
//����:
//    presetIndex:Ԥ���±�
//    presetSave:���ڱ���ڵ����ݵĽڵ�ָ��
void CPreset::GetOneNode(int presetIndex,PRESET_NODE* presetSave)
{
	wchar_t localStr[50];
	swprintf_s(localStr,50,L"Preset%d",presetIndex);
	GetOneNode(localStr,presetSave);
}

//����һ��Ԥ�赽�ļ�
//����:
//    presetIndex:Ԥ���±�
//    presetSave:Ԥ��ڵ�����
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

//��������Ԥ����Ŀ
int CPreset::GetTotalPreset()
{
	return iLocalPreset;
}

//����һ��Ԥ��
//�Խڵ��е����ֶ���������
//����ڵ�����Ϊ��,�򱣴浽LastSave�ڵ�,�����һ��ʹ�õ�ֵ
//�������Ѿ��еĽڵ���Ѱ��,������ظ���,�򸲸�,û���ظ���,������һ��Ԥ��
//����ֵ:
//<0 :����
//>=0:���ӵĽڵ�λ��(ע��:����ڵ�����Ϊ��,���浽LastSave,�򷵻�ֵҲΪ0,��������Ӧ�����ֵ���.)
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

//ɾ��һ��Ԥ��
//����ֵ:
// 0  - �ɹ�
// -1 - ʧ��
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

//��ȡԤ��
//����ֵΪԤ��ڵ�ָ��
//NULL   - ʧ��
//��NULL - Ԥ��ڵ�
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