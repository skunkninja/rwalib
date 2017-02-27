#ifndef PRESET_HEAD
#define PRESET_HEAD
#include <stdlib.h>

//预置类,用于保存一些预置,以便下次快速使用.使用的是标准ini文件来保存预置。

#define MAX_PRESET 40

//一般情况下需要置为1,以自定义节点结构,如果不需要使用这个类,则置成0,以使编译通过.
#define USER_DEF_PRESET_NODE 1//此类未使用.

//定义节点结构的文件
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
如果需要使用此类
1.USER_DEF_PRESET_NODE 定义为 1,以能用户自定义节点结构。
2.节点的结构中必须包含strPresetNodeName[_MAX_PATH]; 这个项。
typedef struct _PRESET_NODE
{
	wchar_t strPresetName[_MAX_PATH];

	//add any preset you need
}PRESET_NODE;

3.重载此类,重写GetOneNode(wchar_t *preSetGroup,PRESET_NODE* presetSave);
  重写SaveOneNode(wchar_t *preSetGroup,PRESET_NODE* presetSave);

4.使用重载类的SetIniFile从ini文件中加载预设
  使用重载类的SaveIniFile将预设保存到ini

//------------------------------------------------
//GetOneNode函数示例
//保存一个预设到文件
//输入:
//    preSetGroup:预设节点名(不是预设名称)
//    presetSave: 节点数据
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
//SaveOneNode函数示例
//从文件取出一个预设
//输入:
//    preSetGroup:预设节点名(不是预设名称)
//    presetSave:用于保存节点数据的节点指针
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