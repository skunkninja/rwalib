#ifndef _CONSTDEF_HEAD
#define _CONSTDEF_HEAD

#include "JuceHeader.h"
#include "RelativeValue.h"

#if !defined(MIN)
#define MIN(A,B)	((A) < (B) ? (A) : (B))
#endif

#if !defined(MAX)
#define MAX(A,B)	((A) > (B) ? (A) : (B))
#endif

typedef struct _SPLIT_BASIC_INFO
{
	bool bSelect;
    bool bMute;

	int iSampleStatus;

	//split信息
	int iStartKey;//分段的开始
	int iEndKey;//分段的结束
	int iStartKeyMin,iStartKeyMax;//给上层的Slider作为参数
	int iEndKeyMin,iEndKeyMax;//给上层的Slider作为参数

	CRelativeValue <int> vStartVel;
	CRelativeValue <int> vEndVel;

}SPLIT_BASIC_INFO;

#define KEY_OFF_PEDAL_HOLD	1
#define KEY_OFF_ONE_SHOT	2

typedef struct _EG_INFO
{
	CRelativeValue <int>vInitVal;
	CRelativeValue <int>vEG1Level, vEG1Rate;
	CRelativeValue <int>vEG2Level, vEG2Rate;
	CRelativeValue <int>vEG3Level, vEG3Rate;
	CRelativeValue <int>vEG4Level, vEG4Rate;
	CRelativeValue <int>vEG4SLevel, vEG4SRate;
	int iKeyOffType;
}EG_INFO;

typedef struct _LFO_INFO
{
	int iType;
	CRelativeValue <int>vFreq;
	CRelativeValue <int>vDelay;
	CRelativeValue <int>vAttack;
}LFO_INFO;

typedef struct _KEYBOARD_NODE
{
	int iKey;
	int iVal;
}KBD_NODE;

typedef struct _KBD_LIST
{
	int iActiveNode;
	int iSaveOffset;
	std::vector <KBD_NODE> allNodeList;
}KBD_LIST;

typedef struct _WAVE_Genertor
{
	String cWaveName;
	int iChannel;//use left/right channel of wave file.
//	String ShortName;
	int iSampleLen;
	int iWaveStart;
	int iLoopStart;
//	int iLoopStartFloat;
	int iLoopEnd;
	int iLoopLength;
//	int iLoopLengthFloat;
	int iLoopmode;
	int iSampleBit;
	int iSampleRate;
	int iSampleNote;
	int iKbdType;
	int iMCUCTune;
	int iMCUFTune;
	bool b8BitUseHigh;
	CRelativeValue <int> vUserCTune;
	CRelativeValue <int> vUserFTune;
	bool bEg3Enable;
	CRelativeValue <int> vEg3Amount;
	CRelativeValue <int> vLfoAmount;
}WaveGenertor;

typedef struct _SMP_Amplitude
{
	CRelativeValue <int> vAmplitude;
	bool bKbdTable;
	int iKbdTableNum;

	bool bDynamic;
	CRelativeValue <int> vDynOffset;
	CRelativeValue <float> vDynSlope;
}SampleAmplitude;

typedef struct _Filter
{
	CRelativeValue <int> vFc;
	bool bKbdTable;
	int iKbdTableNum;
	bool bEg2;
	bool bLfo2;
	CRelativeValue <int> vLfo2Amount;

	bool bOffset;
	CRelativeValue <int> vOffset;
	bool bFcDynamic;
	CRelativeValue <int> vFcDynOffset;
	CRelativeValue <float> vFcDynSlope;

	CRelativeValue <int> vQAmpulitude;
	bool bQDynamic;
	CRelativeValue <int> vQDynOffset;
	CRelativeValue <float> vQDynSlope;

	int iFilterType;
	int iFilterSlope;
}Filter;

typedef struct _OUT_Amplitude
{
	int iVolume;
	bool bKbdTable;
	int iKbdTableNum;
	bool bEg1;
	bool bLfo2;
	CRelativeValue<int> vLfo2Amount;

	bool bOffset; 
	int iOffset;
	bool bDynamic;
	CRelativeValue<int> vDynOffset;
	CRelativeValue<float> vDynSlope;

	CRelativeValue<int> vMixL;
	CRelativeValue<int> vMixR;

	CRelativeValue<int> vFX1Vol;
	CRelativeValue<int> vFX2Vol;
	CRelativeValue<int> vFX3Vol;
	CRelativeValue<int> vFX4Vol;
}OutAmpLitude;

/////////////////////////////////////////////////
typedef struct _SPLIT_INFO
{
	SPLIT_BASIC_INFO BasicInfo;

	//EG信息
	EG_INFO EG1Info;
	EG_INFO EG2Info;
	EG_INFO EG3Info;

	//LFO信息
	LFO_INFO LFO1Info;
	LFO_INFO LFO2Info;

	KBD_LIST KBDInfo1;
	KBD_LIST KBDInfo2;

	//Oscillator
	WaveGenertor waveGen;
	SampleAmplitude sampleAmp;

	////////////////////////////////
	//Filter
	Filter filter;
	OutAmpLitude outamp;

//	int iSampleLoopStart;
//	int iSampleLoopEnd;
//	int iSampleLoopType;

	int iSampleCTune;
	int iSampleFTune;

    int iRate2XPos;

	//--used in drum set edit.
	String sgiFileName;
	//bool bModified;  //the split is modified. Used in drum set mode only.
	//
	int iSaveOffset; //for save to binary
	//
	int iLayerIndex;//used in copy/paste

	//
	bool bReadOnly;//if set true, this split can't be selected and edit.
}SPLIT_INFO;

enum
{
	KEY_OFF_TYPE_SHORT_VOICE = 0,//00
	KEY_OFF_TYPE_LONG_VOICE = KEY_OFF_PEDAL_HOLD,//01
	KEY_OFF_TYPE_ONE_SHOT = KEY_OFF_ONE_SHOT//10
};

////////////////////////////////////////////////////
#endif
