#pragma once
#include "constdef.h"

#ifndef _MAX_PATH
#if JUCE_MAC
#include "sys/syslimits.h"
#define _MAX_PATH PATH_MAX
#endif
#endif

typedef struct _ALLsgi_INFO
{
	int iTotalSample;
	bool bNoteOnRepeat;
	int iVol;

	StringArray allSampleList;
	std::vector<SPLIT_INFO> allSplitInfo;

	int iBank;
	int iProgram;

	bool bDrum;
	int iDrumKey;//Drum only
	int iExcGroup;

	wchar_t sSgiName[_MAX_PATH];//sgi name


	int iRepeatMode;

	int iMajorVersion;
	int iMinorVersion;

	bool isDuplicate;
	int iDuplicateID;

	int iPage;//The page of the sgi data.
	int iOffset;//The offset of the sgi data.
	int iDataWordSize;

	//uint16 *pSgiData;
}strSgiFileInfo;

enum
{
    SGI_LOAD_ERROR_FILE_NOT_EXIST=1,
    SGI_LOAD_ERROR_FILE_NOT_CORRECT,
	SGI_LOAD_ERROR_FILE_VERSION_HIGH,
    SGI_LOAD_ERROR_NO_SAMPLE,

    SGI_SAVE_NO_DATA,
    SGI_SAVE_NO_SAMPLE,
};

class CSgiLoad
{
public:
    CSgiLoad(void);
    ~CSgiLoad(void);
	int LoadFile(strSgiFileInfo* SavesgiFileInfo);
	int SaveFile(strSgiFileInfo* SavesgiFileInfo);
};

