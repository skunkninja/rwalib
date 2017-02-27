#ifndef PRESET_HEAD
#define PRESET_HEAD
#include <stdlib.h>

//Ԥ����,���ڱ���һЩԤ��,�Ա��´ο���ʹ��.ʹ�õ��Ǳ�׼ini�ļ�������Ԥ�á�

#define MAX_PRESET 40

//һ���������Ҫ��Ϊ1,���Զ���ڵ�ṹ,�������Ҫʹ�������,���ó�0,��ʹ����ͨ��.
#define USER_DEF_PRESET_NODE 1//����δʹ��.

//����ڵ�ṹ���ļ�
#if USER_DEF_PRESET_NODE
#include "PresetNode.h"
#else
typedef struct _PRESET_NODE
{
	wchar_t strPresetName[_MAX_PATH];

	//add any preset you need
}PRESET_NODE;
#endif

/**
�����Ҫʹ�ô���
1.USER_DEF_PRESET_NODE ����Ϊ 1,�����û��Զ���ڵ�ṹ��
2.�ڵ�Ľṹ�б������strPresetNodeName[_MAX_PATH]; ����
typedef struct _PRESET_NODE
{
	wchar_t strPresetName[_MAX_PATH];

	//add any preset you need
}PRESET_NODE;

3.���ش���,��дGetOneNode(wchar_t *preSetGroup,PRESET_NODE* presetSave);
  ��дSaveOneNode(wchar_t *preSetGroup,PRESET_NODE* presetSave);

4.ʹ���������SetIniFile��ini�ļ��м���Ԥ��
  ʹ���������SaveIniFile��Ԥ�豣�浽ini

//------------------------------------------------
//GetOneNode����ʾ��
//����һ��Ԥ�赽�ļ�
//����:
//    preSetGroup:Ԥ��ڵ���(����Ԥ������)
//    presetSave: �ڵ�����
void CLocalPreset::SaveOneNode(wchar_t *preSetGroup,PRESET_NODE* presetSave)
{
	wchar_t localStr[50];
	WritePrivateProfileString(preSetGroup,L"PresetName",presetSave->strPresetName,localIniName);

	WritePrivateProfileString(preSetGroup,L"FlashName",presetSave->strFlashName,localIniName);
	swprintf_s(localStr,50,L"%d",presetSave->iFlashPage);
	WritePrivateProfileString(preSetGroup,L"FlashPage",localStr,localIniName);

	WritePrivateProfileString(preSetGroup,L"NameWrite",presetSave->strFile2Write,localIniName);
	swprintf_s(localStr,50,L"%d",presetSave->iFileOffset);
	WritePrivateProfileString(preSetGroup,L"FileOffset",localStr,localIniName);
	swprintf_s(localStr,50,L"%d",presetSave->iWriteLength);
	WritePrivateProfileString(preSetGroup,L"WriteLength",localStr,localIniName);
	swprintf_s(localStr,50,L"%d",presetSave->iFlashOffset);
	WritePrivateProfileString(preSetGroup,L"FlashOffset",localStr,localIniName);

	WritePrivateProfileString(preSetGroup,L"NameRead",presetSave->strFile2Read,localIniName);
	swprintf_s(localStr,50,L"%d",presetSave->iReadOffset);
	WritePrivateProfileString(preSetGroup,L"ReadOffset",localStr,localIniName);
	swprintf_s(localStr,50,L"%d",presetSave->iReadLength);
	WritePrivateProfileString(preSetGroup,L"ReadLength",localStr,localIniName);
}

//-------------------------------------
//SaveOneNode����ʾ��
//���ļ�ȡ��һ��Ԥ��
//����:
//    preSetGroup:Ԥ��ڵ���(����Ԥ������)
//    presetSave:���ڱ���ڵ����ݵĽڵ�ָ��
void CLocalPreset::GetOneNode(wchar_t *preSetGroup,PRESET_NODE* presetSave)
{
	wchar_t localStr[50];
	GetPrivateProfileString(preSetGroup,L"PresetName",L"",presetSave->strPresetName,_MAX_PATH,localIniName);

	GetPrivateProfileString(preSetGroup,L"FlashName",L"",presetSave->strFlashName,_MAX_PATH,localIniName);
	GetPrivateProfileString(preSetGroup,L"FlashPage",L"0",localStr,50,localIniName);
	presetSave->iFlashPage = _wtoi(localStr);

	GetPrivateProfileString(preSetGroup,L"NameWrite",L"",presetSave->strFile2Write,_MAX_PATH,localIniName);
	GetPrivateProfileString(preSetGroup,L"FileOffset",L"0",localStr,50,localIniName);
	presetSave->iFileOffset = _wtoi(localStr);
	GetPrivateProfileString(preSetGroup,L"WriteLength",L"0",localStr,50,localIniName);
	presetSave->iWriteLength = _wtoi(localStr);
	GetPrivateProfileString(preSetGroup,L"FlashOffset",L"0",localStr,50,localIniName);
	presetSave->iFlashOffset = _wtoi(localStr);

	GetPrivateProfileString(preSetGroup,L"NameRead",L"",presetSave->strFile2Read,_MAX_PATH,localIniName);
	GetPrivateProfileString(preSetGroup,L"ReadOffset",L"0",localStr,50,localIniName);
	presetSave->iReadOffset = _wtoi(localStr);
	GetPrivateProfileString(preSetGroup,L"ReadLength",L"0",localStr,50,localIniName);
	presetSave->iReadLength = _wtoi(localStr);
}
//---
***/

class CPreset
{
public:
	CPreset(void);
	~CPreset(void);
	const void* GetPreset(int presetIndex);
	int AddPreset(void *presetData);
	int DelPreset(const wchar_t *strPresetName);
	int SetIniFile(wchar_t *iniFileName);
	int GetTotalPreset();
	void SaveIniFile();
protected:
	PRESET_NODE *presetLastSave;
	PRESET_NODE **AllPreset;
	virtual void GetOneNode(wchar_t *preSetGroup,PRESET_NODE* presetSave) = 0;
	void GetOneNode(int presetIndex,PRESET_NODE* presetSave);
	virtual void SaveOneNode(wchar_t *preSetGroup,PRESET_NODE* presetSave) = 0;
	void SaveOneNode(int presetIndex,PRESET_NODE* presetSave);


	wchar_t localIniName[_MAX_PATH];
	int iLocalPreset;
};
#endif