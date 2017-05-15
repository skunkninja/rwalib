#include "SgiLoad.h"
#include "CopyFileList.h"
#include "FileNameBasic.h"

#include "JuceHeader.h"

#if RINGWAY_GM1
#define SGI_FILE_MAJOR_VERSION  1 //
#define SGI_FILE_MINOR_VERSION  2 //
#else
#define SGI_FILE_MAJOR_VERSION  1 //
#define SGI_FILE_MINOR_VERSION  4 //
#endif

#define SGI_FILE_WARNING_MAJOR  1//<
#define SGI_FILE_WARNING_MINOR  2//

#define SGI_FILE_EG_HOLD_CONVERT_MAJOR  1//<=
#define SGI_FILE_EG_HOLD_CONVERT_MINOR  2//<=

#define SGI_FILE_EG23_CONVERT_MAJOR  1 //<=
#define SGI_FILE_EG23_CONVERT_MINOR  3 //<=

void initSplitInfo(SPLIT_INFO &splitInfo)
{
	//Basic info
	splitInfo.BasicInfo.bSelect = false;
	splitInfo.BasicInfo.iStartKey = 0;
	splitInfo.BasicInfo.iEndKey = 0;
	splitInfo.BasicInfo.iEndKeyMin = 0;
	splitInfo.BasicInfo.iEndKeyMax = 0;
	splitInfo.BasicInfo.iStartKeyMin = 0;
	splitInfo.BasicInfo.iStartKeyMax = 0;

	splitInfo.BasicInfo.vStartVel.setValue(1);
	splitInfo.BasicInfo.vStartVel.setRange(1, 127);

	splitInfo.BasicInfo.vEndVel.setValue(127);
	splitInfo.BasicInfo.vEndVel.setRange(1, 127);

	//EG1
	splitInfo.EG1Info.vEG1Level.setValue(255);
	splitInfo.EG1Info.vEG1Level.setRange(0, 255);

	splitInfo.EG1Info.vEG1Rate.setValue(127);

	splitInfo.EG1Info.vEG2Level.setValue(255);
	splitInfo.EG1Info.vEG2Level.setRange(0, 255);

	splitInfo.EG1Info.vEG2Rate.setValue(120);

	splitInfo.EG1Info.vEG3Level.setValue(255);
	splitInfo.EG1Info.vEG3Level.setRange(0, 255);

	splitInfo.EG1Info.vEG3Rate.setValue(50);

	splitInfo.EG1Info.vEG4Level.setValue(0);
	splitInfo.EG1Info.vEG4Level.setRange(0, 255);

	splitInfo.EG1Info.vEG4Rate.setValue(110);

	splitInfo.EG1Info.vEG4SLevel.setValue(0);
	splitInfo.EG1Info.vEG4SLevel.setRange(0, 255);

	splitInfo.EG1Info.vEG4SRate.setValue(60);

	splitInfo.EG1Info.vInitVal.setValue(255);
	splitInfo.EG1Info.vInitVal.setRange(0, 255);

	splitInfo.EG1Info.iKeyOffType = KEY_OFF_PEDAL_HOLD;

	//EG2
	splitInfo.EG2Info.vEG1Level.setValue(0);
	splitInfo.EG2Info.vEG1Level.setRange(0, 255);

	splitInfo.EG2Info.vEG1Rate.setValue(0);

	splitInfo.EG2Info.vEG2Level.setValue(0);
	splitInfo.EG2Info.vEG2Level.setRange(0, 255);

	splitInfo.EG2Info.vEG2Rate.setValue(0);

	splitInfo.EG2Info.vEG3Level.setValue(0);
	splitInfo.EG2Info.vEG3Level.setRange(0, 255);

	splitInfo.EG2Info.vEG3Rate.setValue(0);

	splitInfo.EG2Info.vEG4Level.setValue(0);
	splitInfo.EG2Info.vEG4Level.setRange(0, 255);

	splitInfo.EG2Info.vEG4Rate.setValue(0);

	splitInfo.EG2Info.vEG4SLevel.setValue(0);
	splitInfo.EG2Info.vEG4SLevel.setRange(0, 255);

	splitInfo.EG2Info.vEG4SRate.setValue(0);

	splitInfo.EG2Info.vInitVal.setValue(0);
	splitInfo.EG2Info.vInitVal.setRange(0, 255);

	//EG3
	splitInfo.EG3Info.vEG1Level.setValue(128);
	splitInfo.EG3Info.vEG1Level.setRange(0, 255);

	splitInfo.EG3Info.vEG1Rate.setValue(90);

	splitInfo.EG3Info.vEG2Level.setValue(128);
	splitInfo.EG3Info.vEG2Level.setRange(0, 255);

	splitInfo.EG3Info.vEG2Rate.setValue(127);

	splitInfo.EG3Info.vEG3Level.setValue(128);
	splitInfo.EG3Info.vEG3Level.setRange(0, 255);

	splitInfo.EG3Info.vEG3Rate.setValue(127);

	splitInfo.EG3Info.vEG4Level.setValue(128);
	splitInfo.EG3Info.vEG4Level.setRange(0, 255);

	splitInfo.EG3Info.vEG4Rate.setValue(127);

	splitInfo.EG3Info.vEG4SLevel.setValue(128);
	splitInfo.EG3Info.vEG4SLevel.setRange(0, 255);

	splitInfo.EG3Info.vEG4SRate.setValue(127);

	splitInfo.EG3Info.vInitVal.setValue(0);
	splitInfo.EG3Info.vInitVal.setRange(0, 255);

	splitInfo.KBDInfo1.iActiveNode = 0;
	splitInfo.KBDInfo1.allNodeList.push_back({ 0, 0 });
	splitInfo.KBDInfo1.allNodeList.push_back({ 127, 0 });

	splitInfo.KBDInfo2.iActiveNode = 0;
	splitInfo.KBDInfo2.allNodeList.push_back({ 0, 0 });
	splitInfo.KBDInfo2.allNodeList.push_back({ 127, 0 });

	splitInfo.LFO1Info.vAttack.setValue(0);
	splitInfo.LFO1Info.vDelay.setValue(0);
	splitInfo.LFO1Info.vFreq.setValue(0);
	splitInfo.LFO1Info.iType = 1;
	splitInfo.LFO2Info.vAttack.setValue(0);
	splitInfo.LFO2Info.vDelay.setValue(0);
	splitInfo.LFO2Info.vFreq.setValue(0);
	splitInfo.LFO2Info.iType = 1;
	//splitInfo.waveGen.cWaveName =NULL;
	splitInfo.waveGen.bEg3Enable = 0;
	splitInfo.waveGen.vUserCTune.setValue(0);
	splitInfo.waveGen.vUserCTune.setRange(-127, 127);

	splitInfo.waveGen.vEg3Amount.setValue(0);
	splitInfo.waveGen.vEg3Amount.setRange(0, 31);

	splitInfo.waveGen.vUserFTune.setValue(0);
	splitInfo.waveGen.vUserFTune.setRange(0, 255);

	splitInfo.waveGen.iKbdType = 0;
	splitInfo.waveGen.vLfoAmount.setValue(0);
	splitInfo.waveGen.iLoopEnd = 0;
	splitInfo.waveGen.iLoopLength = 0;
	splitInfo.waveGen.iLoopmode = 0;
	splitInfo.waveGen.iLoopStart = 0;
	splitInfo.waveGen.iSampleBit = 0;
	splitInfo.waveGen.iSampleLen = 0;
	splitInfo.waveGen.iWaveStart = 0;

	splitInfo.filter.bKbdTable = false;
	splitInfo.filter.iKbdTableNum = 0;
	splitInfo.filter.bEg2 = false;
	splitInfo.filter.bLfo2 = false;

	splitInfo.filter.bOffset = false;
	splitInfo.filter.bFcDynamic = false;
	splitInfo.filter.bQDynamic = false;
	splitInfo.filter.iFilterType = 0;
	splitInfo.filter.iFilterSlope = 0;

	splitInfo.filter.vFcDynOffset.setValue(64);
	splitInfo.filter.vFcDynOffset.setRange(-127, 127);

	splitInfo.filter.vFcDynSlope.setValue(0);
	splitInfo.filter.vFcDynSlope.setRange(-2.0f, 1.75f);

	splitInfo.filter.vFc.setValue(127);

	splitInfo.filter.vQAmpulitude.setValue(0);

	splitInfo.filter.iFilterType = 1;
	splitInfo.filter.iFilterSlope = 0;

	splitInfo.filter.vQDynOffset.setValue(0);
	splitInfo.filter.vQDynOffset.setRange(-127, 127);

	splitInfo.filter.vQDynSlope.setValue(0);
	splitInfo.filter.vQDynSlope.setRange(-2.0f, 1.75f);

	splitInfo.outamp.iVolume = 0;
	splitInfo.outamp.bKbdTable = false;
	splitInfo.outamp.iKbdTableNum = 0;
	splitInfo.outamp.bEg1 = false;
	splitInfo.outamp.bLfo2 = false;
	splitInfo.outamp.bOffset = false;
	splitInfo.outamp.iOffset = 0;
	splitInfo.outamp.bDynamic = false;

	splitInfo.outamp.vMixL.setValue(100);
	splitInfo.outamp.vMixR.setValue(100);

	splitInfo.outamp.vFX1Vol.setValue(127);
	splitInfo.outamp.vFX2Vol.setValue(127);
	splitInfo.outamp.vFX3Vol.setValue(127);
	splitInfo.outamp.vFX4Vol.setValue(127);

	splitInfo.outamp.vDynOffset.setValue(0);
	splitInfo.outamp.vDynOffset.setRange(-127, 127);

	splitInfo.outamp.vDynSlope.setValue(1);
	splitInfo.outamp.vDynSlope.setRange(-2.0f, 1.75f);

	splitInfo.sampleAmp.bKbdTable = false;
	splitInfo.sampleAmp.iKbdTableNum = 0;
	splitInfo.sampleAmp.bDynamic = false;

	splitInfo.sampleAmp.vAmplitude.setValue(255);
	splitInfo.sampleAmp.vAmplitude.setRange(0, 255);

	splitInfo.sampleAmp.vDynOffset.setValue(64);
	splitInfo.sampleAmp.vDynOffset.setRange(-127, 127);

	splitInfo.sampleAmp.vDynSlope.setValue(0);
	splitInfo.sampleAmp.vDynSlope.setRange(-2.0f, 1.75f);

	splitInfo.bReadOnly = false;

	splitInfo.iSaveOffset = 0;
}



CSgiLoad::CSgiLoad(void)
{
}


CSgiLoad::~CSgiLoad(void)
{
}

//加载一个sgi文件
int CSgiLoad::LoadFile(strSgiFileInfo* pSaveSgiFileInfo)
{
    //--------------
    //use JUCE xml
    //--------------
	File openfile(pSaveSgiFileInfo->sSgiName);
    String strSaveFilePath;
    String strCurrentPath;

    if(!openfile.exists())
    {
        //		plog->AddFormatString("%s file is not exists.\n"),FileName);
        return SGI_LOAD_ERROR_FILE_NOT_EXIST;
    }

    XmlDocument myDocument (openfile);
    XmlElement* myElement = myDocument.getDocumentElement();
    //plog->AddFormatString("%s ...\n"),FileName);
    if(myElement == NULL)
    {
        //		plog->AddFormatString("%s file is not correct.\n"),FileName);
        return SGI_LOAD_ERROR_FILE_NOT_CORRECT;
    }

    strSaveFilePath = myElement->getStringAttribute("PATH");
    FileNameToPath(&strSaveFilePath);
	strCurrentPath = pSaveSgiFileInfo->sSgiName;
    FileNameToPath(&strCurrentPath);

	int iFileMajorVersion = myElement->getIntAttribute("FileMajorVer");
	int iFileMinorVersion = myElement->getIntAttribute("FileMinorVer");

	int iFileVersion = (iFileMajorVersion << 16) + iFileMinorVersion;

#if RINGWAY_GM1
#else
	if (iFileVersion > ((SGI_FILE_MAJOR_VERSION << 16) + SGI_FILE_MINOR_VERSION))
	{
		pSaveSgiFileInfo->iMajorVersion = iFileMajorVersion;
		pSaveSgiFileInfo->iMinorVersion = iFileMinorVersion;
		delete myElement;
		return SGI_LOAD_ERROR_FILE_VERSION_HIGH;
	}
#endif

    int SampleCount=myElement->getIntAttribute("SampleCount");
    if(!SampleCount)
    {
        //		plog->AddFormatString("must 1 sample wav at least!"));
        delete myElement;
        return SGI_LOAD_ERROR_NO_SAMPLE;
    }

	pSaveSgiFileInfo->iRepeatMode = myElement->getIntAttribute("Repeat", 1);

    String SampleFileName,newSampleFileName;

	pSaveSgiFileInfo->iTotalSample = SampleCount;

	for (int i = 0; i < SampleCount; i++)
	{
		SampleFileName = myElement->getStringAttribute(L"SampleList" + String(i));
		(pSaveSgiFileInfo->allSampleList).add(SampleFileName);
	}

	SPLIT_INFO tmpSplitInfo;

    int splitIndex = 0;
    int iCurrentLayer = 0;
    if (myElement->hasTagName ("RAW_Instrument"))
    {
        // now we'll iterate its sub-elements looking for 'giraffe' elements..
        forEachXmlChildElement (*myElement,Layer)
        {
            if (Layer->hasTagName ("Layer"))
            {
				bool hasSplitInfo = false;;
				initSplitInfo(tmpSplitInfo);
                // found a giraffe, so use some of its attributes..
                forEachXmlChildElement(*Layer, Split)
                {
                    if(Split->hasTagName("Split"))
                    {
						hasSplitInfo = true;
						tmpSplitInfo.iLayerIndex = iCurrentLayer;

						tmpSplitInfo.BasicInfo.iStartKey = Split->getIntAttribute("StartNote");
						tmpSplitInfo.BasicInfo.iEndKey  = Split->getIntAttribute("EndNote");

						tmpSplitInfo.BasicInfo.vStartVel.setValue(Split->getIntAttribute("StartVel"));
						tmpSplitInfo.BasicInfo.vEndVel.setValue(Split->getIntAttribute("EndVel"));
                        //----------------------------------------------load WaveGenertor----------------------------------------
                        forEachXmlChildElement(*Split,WaveGenertor)
                        {
                            if(WaveGenertor->hasTagName("WaveGenertor"))
                            {
                                String strSampleFile = WaveGenertor->getStringAttribute("WaveName");
                                ConvertFilePath(&strSampleFile,strSaveFilePath,strCurrentPath);

								tmpSplitInfo.waveGen.cWaveName = strSampleFile;
								tmpSplitInfo.waveGen.iChannel  = WaveGenertor->getIntAttribute("Channel");
								tmpSplitInfo.waveGen.iWaveStart = WaveGenertor->getIntAttribute("WaveStart");
								tmpSplitInfo.waveGen.iLoopStart = WaveGenertor->getIntAttribute("LoopStart");
								tmpSplitInfo.waveGen.iLoopEnd = WaveGenertor->getIntAttribute("LoopEnd");
								tmpSplitInfo.waveGen.iLoopLength = WaveGenertor->getIntAttribute("LoopLen");
								tmpSplitInfo.waveGen.iLoopmode = WaveGenertor->getIntAttribute("LoopMode");
								if (tmpSplitInfo.waveGen.iLoopmode > 2)
                                {
									tmpSplitInfo.waveGen.iLoopmode = 0;
                                }
								tmpSplitInfo.waveGen.iSampleBit = WaveGenertor->getIntAttribute("SampleBit");
								tmpSplitInfo.waveGen.iKbdType = WaveGenertor->getIntAttribute("KbdType");
								tmpSplitInfo.waveGen.iMCUCTune = WaveGenertor->getIntAttribute("IcTune");
								tmpSplitInfo.waveGen.iMCUFTune = WaveGenertor->getIntAttribute("IfTune");
								int tmpCTune = WaveGenertor->getIntAttribute("IUsercTune", 0xffff);
								if (tmpCTune == 0xffff)
								{
									tmpSplitInfo.waveGen.vUserCTune.setRange(65535, 65535);
								}
								tmpSplitInfo.waveGen.vUserCTune.setValue(tmpCTune);
								tmpSplitInfo.waveGen.vUserFTune.setValue(WaveGenertor->getIntAttribute("IUserfTune",0xffff));
								tmpSplitInfo.waveGen.bEg3Enable = WaveGenertor->getIntAttribute("Eg3Enable") ? true : false;
								tmpSplitInfo.waveGen.vEg3Amount.setValue(WaveGenertor->getIntAttribute("Eg3Amount"));
								tmpSplitInfo.waveGen.vLfoAmount.setValue(WaveGenertor->getIntAttribute("LfoAmount"));

                            }
                        }
                        //----------------------------------------------load SampleAmplitude--------------------------------------
                        forEachXmlChildElement(*Split,SampleAmplitude)
                        {
                            if(SampleAmplitude->hasTagName("SampleAmplitude"))
                            {
                                /*								layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->sampleAmp.iAmplitude=SampleAmplitude->getIntAttribute("Amplitude"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->sampleAmp.bKbdTable=SampleAmplitude->getIntAttribute("KbdTable"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->sampleAmp.iKbdTableNum=SampleAmplitude->getIntAttribute("KbdTableNum"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->sampleAmp.bDynamic=SampleAmplitude->getIntAttribute("Dynamic"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->sampleAmp.iDynOffset=SampleAmplitude->getIntAttribute("DynOffset"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->sampleAmp.iDynSlope=SampleAmplitude->getIntAttribute("DynSlope"))/4.;

                                int iAmplitude;
                                bool bKbdTable;
                                int iKbdTableNum;

                                bool bDynamic;
                                int iDynOffset;
                                int iDynSlope;*/
								tmpSplitInfo.sampleAmp.vAmplitude.setValue(SampleAmplitude->getIntAttribute("Amplitude"));
                                //SavesgiFileInfo->pSampleAmp[splitIndex].bKbdTable = (SampleAmplitude->getIntAttribute("KbdTable")) != 0)?true:false;
								tmpSplitInfo.sampleAmp.iKbdTableNum = SampleAmplitude->getIntAttribute("KbdTableNum");

								if (tmpSplitInfo.sampleAmp.iKbdTableNum != 0)
                                {
									tmpSplitInfo.sampleAmp.bKbdTable = true;
                                }
                                else
                                {
									tmpSplitInfo.sampleAmp.bKbdTable = false;
                                }
								tmpSplitInfo.sampleAmp.bDynamic = (SampleAmplitude->getIntAttribute("Dynamic") != 0) ? true : false;
								tmpSplitInfo.sampleAmp.vDynOffset.setValue(SampleAmplitude->getIntAttribute("DynOffset"));
								tmpSplitInfo.sampleAmp.vDynSlope.setValue(float(SampleAmplitude->getDoubleAttribute("DynSlope")/4.0));
                            }
                        }
                        //----------------------------------------------load Filter-----------------------------------------------
                        forEachXmlChildElement(*Split,Filter)
                        {
                            /*
                            if(Filter->hasTagName("Filter")))
                            {
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.iFc=Filter->getIntAttribute("Fc"));
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.bKbdTable=Filter->getIntAttribute("KbdTable"));
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.iKbdTableNum=Filter->getIntAttribute("KbdTableNum"));
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.bEg2=Filter->getIntAttribute("Eg2"));
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.bLfo2=Filter->getIntAttribute("Lfo2"));
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.iLfo2Amount=Filter->getIntAttribute("Lfo2Amount"));
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.bOffset=Filter->getIntAttribute("Boffset"));
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.iOffset=Filter->getIntAttribute("Ioffset"));
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.bFcDynamic=Filter->getIntAttribute("FcDynamic"));
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.iFcDynOffset=Filter->getIntAttribute("FcDynOffset"));
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.iFcDynSlope=Filter->getIntAttribute("FcDynSlope"))/4.;
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.iQDynSlope=Filter->getIntAttribute("QDynSlope"))/4.;
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.bQDynamic=Filter->getIntAttribute("QDynamic"));
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.iQDynOffset=Filter->getIntAttribute("QDynOffset"));
                            layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->filter.iQAmpulitude=Filter->getIntAttribute("QAmpulitude"));
                            }*/
                            if(Filter->hasTagName("Filter"))
                            {
								tmpSplitInfo.filter.vFc.setValue(Filter->getIntAttribute("Fc"));
                                //SavesgiFileInfo->pFilter[splitIndex].bKbdTable = Filter->getIntAttribute("KbdTable"))!=0?true:false;
								tmpSplitInfo.filter.iKbdTableNum = Filter->getIntAttribute("KbdTableNum");
								if (tmpSplitInfo.filter.iKbdTableNum != 0)
                                {
									tmpSplitInfo.filter.bKbdTable = true;
                                }
                                else
                                {
									tmpSplitInfo.filter.bKbdTable = false;
                                }
								tmpSplitInfo.filter.bEg2 = Filter->getIntAttribute("Eg2") != 0 ? true : false;
								tmpSplitInfo.filter.bLfo2 = Filter->getIntAttribute("Lfo2") != 0 ? true : false;
								tmpSplitInfo.filter.vLfo2Amount.setValue(Filter->getIntAttribute("Lfo2Amount"));
                                tmpSplitInfo.filter.bOffset = Filter->getIntAttribute("Boffset")!=0?true:false;
                                tmpSplitInfo.filter.vOffset.setValue(Filter->getIntAttribute("Ioffset"));
                                tmpSplitInfo.filter.bFcDynamic = Filter->getIntAttribute("FcDynamic")!=0?true:false;
                                tmpSplitInfo.filter.vFcDynOffset.setValue(Filter->getIntAttribute("FcDynOffset"));
                                tmpSplitInfo.filter.vFcDynSlope.setValue(float(Filter->getDoubleAttribute("FcDynSlope")/4.0));
                                tmpSplitInfo.filter.vQAmpulitude.setValue(Filter->getIntAttribute("QAmpulitude"));
                                tmpSplitInfo.filter.bQDynamic = Filter->getIntAttribute("QDynamic")!=0?true:false;
                                tmpSplitInfo.filter.vQDynOffset.setValue(Filter->getIntAttribute("QDynOffset"));
								tmpSplitInfo.filter.vQDynSlope.setValue(float(Filter->getDoubleAttribute("QaAmpulitude")/4.0));

                                tmpSplitInfo.filter.iFilterType = Filter->getIntAttribute("Type");
                                tmpSplitInfo.filter.iFilterSlope = Filter->getIntAttribute("Slope");
                            }
                        }
                        //----------------------------------------------load OutAmpLitude-------------------------------------
                        forEachXmlChildElement(*Split,OutAmpLitude)
                        {
                            if(OutAmpLitude->hasTagName("OutAmpLitude"))
                            {
                                /*
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.iVolume=OutAmpLitude->getIntAttribute("Volume"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.bKbdTable=OutAmpLitude->getIntAttribute("KbdTable"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.iKbdTableNum=OutAmpLitude->getIntAttribute("KbdTableNum"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.bEg1=OutAmpLitude->getIntAttribute("Eg1"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.bLfo2=OutAmpLitude->getIntAttribute("Lfo2"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.iLfo2Amount=OutAmpLitude->getIntAttribute("Lfo2Amount"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.bOffset=OutAmpLitude->getIntAttribute("Boffset"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.iOffset=OutAmpLitude->getIntAttribute("Ioffset"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.bDynamic=OutAmpLitude->getIntAttribute("Dynamic"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.iDynOffset=OutAmpLitude->getIntAttribute("DynOffset"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.iDynSlope=OutAmpLitude->getIntAttribute("DynSlope"))/4.;
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.iMixL=OutAmpLitude->getIntAttribute("MixL"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.iMixR=OutAmpLitude->getIntAttribute("MixR"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.iFxMixL=OutAmpLitude->getIntAttribute("FxMixL"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->outamp.iFxMixR=OutAmpLitude->getIntAttribute("FxMixR"));]
                                **/
								tmpSplitInfo.outamp.iVolume = OutAmpLitude->getIntAttribute("Volume");
                                //tmpSplitInfo.outAmp.bKbdTable = OutAmpLitude->getIntAttribute("KbdTable"))!=0?true:false;
								tmpSplitInfo.outamp.iKbdTableNum = OutAmpLitude->getIntAttribute("KbdTableNum");
								if (tmpSplitInfo.outamp.iKbdTableNum != 0)
                                {
									tmpSplitInfo.outamp.bKbdTable = true;
                                }
                                else
                                {
									tmpSplitInfo.outamp.bKbdTable = false;
                                }
								tmpSplitInfo.outamp.bEg1 = OutAmpLitude->getIntAttribute("Eg1") != 0 ? true : false;
								tmpSplitInfo.outamp.bLfo2 = OutAmpLitude->getIntAttribute("Lfo2") != 0 ? true : false;

								tmpSplitInfo.outamp.vLfo2Amount.setValue(OutAmpLitude->getIntAttribute("Lfo2Amount"));
								tmpSplitInfo.outamp.bOffset = OutAmpLitude->getIntAttribute("Boffset") != 0 ? true : false;
								tmpSplitInfo.outamp.iOffset = OutAmpLitude->getIntAttribute("Ioffset");
								tmpSplitInfo.outamp.bDynamic = OutAmpLitude->getIntAttribute("Dynamic") != 0 ? true : false;
								tmpSplitInfo.outamp.vDynOffset.setValue(OutAmpLitude->getIntAttribute("DynOffset"));

								tmpSplitInfo.outamp.vDynSlope.setValue(float(OutAmpLitude->getDoubleAttribute("DynSlope")/4.0));
								tmpSplitInfo.outamp.vMixL.setValue(OutAmpLitude->getIntAttribute("MixL"));
								tmpSplitInfo.outamp.vMixR.setValue(OutAmpLitude->getIntAttribute("MixR"));
								tmpSplitInfo.outamp.vFX1Vol.setValue(OutAmpLitude->getIntAttribute("Fx1Mix", 127));
								tmpSplitInfo.outamp.vFX2Vol.setValue(OutAmpLitude->getIntAttribute("Fx2Mix", 127));
								tmpSplitInfo.outamp.vFX3Vol.setValue(OutAmpLitude->getIntAttribute("Fx3Mix", 127));
								tmpSplitInfo.outamp.vFX4Vol.setValue(OutAmpLitude->getIntAttribute("Fx4Mix", 127));
                            }
                        }
                        //----------------------------------------------load keyboard1--------------------------------------------
                        forEachXmlChildElement(*Split, Keyboard1)
                        {
                            if(Keyboard1->hasTagName("Keyboard1"))
                            {

								tmpSplitInfo.KBDInfo1.iActiveNode  = 0;

								tmpSplitInfo.KBDInfo1.allNodeList.clear();
                                forEachXmlChildElement(*Keyboard1, Segment)
                                {
                                    if(Segment->hasTagName("Segment"))
                                    {
										KBD_NODE tmpKeyNode;
										tmpKeyNode.iKey = Segment->getIntAttribute("Key");
										tmpKeyNode.iVal = Segment->getIntAttribute("Value");
										tmpSplitInfo.KBDInfo1.allNodeList.push_back(tmpKeyNode);
                                    }
                                }
                            }
                        }
                        //----------------------------------------------LOAD keyboard 2----------------------------------------------	
                        forEachXmlChildElement(*Split, Keyboard2)
                        {
                            if(Keyboard2->hasTagName("Keyboard2"))
                            {
								tmpSplitInfo.KBDInfo2.iActiveNode = 0;

								tmpSplitInfo.KBDInfo2.allNodeList.clear();
                                forEachXmlChildElement(*Keyboard2, Segment)
                                {
                                    if(Segment->hasTagName("Segment"))
                                    {
										KBD_NODE tmpKeyNode;
										tmpKeyNode.iKey = Segment->getIntAttribute("Key");
										tmpKeyNode.iVal = Segment->getIntAttribute("Value");
										tmpSplitInfo.KBDInfo2.allNodeList.push_back(tmpKeyNode);
                                    }
                                }
                            }
                        }
                        //-----------------------------load LFO1----------------------------------------------------------------------------	
                        forEachXmlChildElement(*Split, LFO1)
                        {
                            if(LFO1->hasTagName("LFO1"))
                            {
                                /*
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->LFO1Info.iType=LFO1->getIntAttribute("LFO1Type"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->LFO1Info.iFreq=LFO1->getIntAttribute("LFO1Freq"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->LFO1Info.iDelay=LFO1->getIntAttribute("LFO1Delay"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->LFO1Info.iAttack=LFO1->getIntAttribute("LFO1Attack"));
                                int iFreq;
                                int iDelay;
                                int iAttack;
                                int iType;
                                */
								tmpSplitInfo.LFO1Info.vFreq.setValue(LFO1->getIntAttribute("LFO1Freq"));
								tmpSplitInfo.LFO1Info.vDelay.setValue(LFO1->getIntAttribute("LFO1Delay"));
								tmpSplitInfo.LFO1Info.vAttack.setValue(LFO1->getIntAttribute("LFO1Attack"));
								tmpSplitInfo.LFO1Info.iType = LFO1->getIntAttribute("LFO1Type");
                            }
                        }
                        //---------------------------------------------------------------------------------------------------------
                        //-----------------------------load LFO2----------------------------------------------------------------------------	
                        forEachXmlChildElement(*Split, LFO2)
                        {
                            if(LFO2->hasTagName("LFO2"))
                            {
                                /*
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->LFO2Info.iType=LFO2->getIntAttribute("LFO2Type"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->LFO2Info.iFreq=LFO2->getIntAttribute("LFO2Freq"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->LFO2Info.iDelay=LFO2->getIntAttribute("LFO2Delay"));
                                layerEdit[iLayerNum]->GetSplitInfo((layerEdit[iLayerNum]->iSplitNum))->LFO2Info.iAttack=LFO2->getIntAttribute("LFO2Attack"));
                                */
								tmpSplitInfo.LFO2Info.vFreq.setValue(LFO2->getIntAttribute("LFO2Freq"));
								tmpSplitInfo.LFO2Info.vDelay.setValue(LFO2->getIntAttribute("LFO2Delay"));
								tmpSplitInfo.LFO2Info.vAttack.setValue(LFO2->getIntAttribute("LFO2Attack"));
								tmpSplitInfo.LFO2Info.iType = LFO2->getIntAttribute("LFO2Type");
                            }
                        }
                        //---------------------------------------------------------------------------------------------------------
                        //--------------------------------load EG1---------------------------------
                        forEachXmlChildElement(*Split, EG1)
                        {
                            if(EG1->hasTagName("EG1"))
                            {
								tmpSplitInfo.EG1Info.vInitVal.setValue(EG1->getIntAttribute("EG1_EG1InitVal"));//int iInitVal;

								tmpSplitInfo.EG1Info.vEG1Level.setValue(EG1->getIntAttribute("EG1_EG1Level"));//int iInitVal;
								tmpSplitInfo.EG1Info.vEG1Rate.setValue(EG1->getIntAttribute("EG1_EG1Rate"));//int iInitVal;

								tmpSplitInfo.EG1Info.vEG2Level.setValue(EG1->getIntAttribute("EG1_EG2Level"));//int iInitVal;
								tmpSplitInfo.EG1Info.vEG2Rate.setValue(EG1->getIntAttribute("EG1_EG2Rate"));//int iInitVal;

								tmpSplitInfo.EG1Info.vEG3Level.setValue(EG1->getIntAttribute("EG1_EG3Level"));//int iInitVal;
								int iTmp;
								iTmp = EG1->getIntAttribute("EG1_EG3Rate");
								tmpSplitInfo.EG1Info.vEG3Rate.setValue(iTmp & 0x7f);//int iInitVal;

								tmpSplitInfo.EG1Info.iKeyOffType = 0;
								if (iTmp & 0x80)
								{
									tmpSplitInfo.EG1Info.iKeyOffType |= KEY_OFF_PEDAL_HOLD;
								}

								iTmp = EG1->getIntAttribute("EG1_EG4Rate");//int iEG3Rate;
								tmpSplitInfo.EG1Info.vEG4Rate.setValue(iTmp & 0x7f);//int iInitVal;

								if (iTmp & 0x80)
								{
									tmpSplitInfo.EG1Info.iKeyOffType |= KEY_OFF_ONE_SHOT;
								}

								tmpSplitInfo.EG1Info.vEG4SRate.setValue(EG1->getIntAttribute("EG1_EG4SRate"));//int iInitVal;
                                
								tmpSplitInfo.EG1Info.vEG4Level.setValue(0);//int iInitVal;
								tmpSplitInfo.EG1Info.vEG4SLevel.setValue(0);//int iInitVal;


								if (iFileVersion <= ((SGI_FILE_EG_HOLD_CONVERT_MAJOR << 16) + SGI_FILE_EG_HOLD_CONVERT_MINOR))
								{
									if (tmpSplitInfo.EG1Info.vEG3Level.getValue() == 0)
									{
										tmpSplitInfo.EG1Info.iKeyOffType &= ~KEY_OFF_PEDAL_HOLD;
										tmpSplitInfo.EG1Info.vEG4SRate.setValue(tmpSplitInfo.EG1Info.vEG4Rate.getValue());
									}
								}
                            }
                        }
                        //-----------------------------------------------------------------
                        //--------------------------------load EG2---------------------------------
                        forEachXmlChildElement(*Split, EG2)
                        {
                            if(EG2->hasTagName("EG2"))
                            {
								tmpSplitInfo.EG2Info.vInitVal.setValue(EG2->getIntAttribute("EG2_EG1InitVal"));//int iInitVal;

								tmpSplitInfo.EG2Info.vEG1Level.setValue(EG2->getIntAttribute("EG2_EG1Level"));//int iInitVal;
								tmpSplitInfo.EG2Info.vEG1Rate.setValue(EG2->getIntAttribute("EG2_EG1Rate"));//int iInitVal;

								tmpSplitInfo.EG2Info.vEG2Level.setValue(EG2->getIntAttribute("EG2_EG2Level"));//int iInitVal;
								tmpSplitInfo.EG2Info.vEG2Rate.setValue(EG2->getIntAttribute("EG2_EG2Rate"));//int iInitVal;

								tmpSplitInfo.EG2Info.vEG3Level.setValue(EG2->getIntAttribute("EG2_EG3Level"));//int iInitVal;
								int iTmp;
								iTmp = EG2->getIntAttribute("EG2_EG3Rate");
								tmpSplitInfo.EG2Info.vEG3Rate.setValue(iTmp & 0x7f);//int iInitVal;

								tmpSplitInfo.EG2Info.iKeyOffType = 0;
								//if version less than 1.04, PEG/FEG should use same hold flag as AEG
								if (iFileVersion <= ((SGI_FILE_EG23_CONVERT_MAJOR << 16) + SGI_FILE_EG23_CONVERT_MINOR))
								{
									if (tmpSplitInfo.EG1Info.iKeyOffType & KEY_OFF_PEDAL_HOLD)
									{
										iTmp |= 0x80;
									}
									else
									{
										iTmp &= 0x7f;
									}
								}

								if (iTmp & 0x80)
								{
									tmpSplitInfo.EG2Info.iKeyOffType |= KEY_OFF_PEDAL_HOLD;
								}

								tmpSplitInfo.EG2Info.vEG4Level.setValue(EG2->getIntAttribute("EG2_EG4Level"));//int iInitVal;
								iTmp = EG2->getIntAttribute("EG2_EG4Rate");//int iEG3Rate;
								tmpSplitInfo.EG2Info.vEG4Rate.setValue(iTmp & 0x7f);//int iInitVal;

								if (iTmp & 0x80)
								{
									tmpSplitInfo.EG2Info.iKeyOffType |= KEY_OFF_ONE_SHOT;
								}

								tmpSplitInfo.EG2Info.vEG4SLevel.setValue(tmpSplitInfo.EG2Info.vEG4Level.getValue());//int iInitVal;17Jun2013-use release level.
								tmpSplitInfo.EG2Info.vEG4SRate.setValue(EG2->getIntAttribute("EG2_EG4SRate"));//int iInitVal;
                            }
                        }
                        //-----------------------------------------------------------------
                        //--------------------------------load EG3---------------------------------
                        forEachXmlChildElement(*Split, EG3)
                        {
                            if(EG3->hasTagName("EG3"))
                            {
								tmpSplitInfo.EG3Info.vInitVal.setValue(EG3->getIntAttribute("EG3_EG1InitVal"));//int iInitVal;

								tmpSplitInfo.EG3Info.vEG1Level.setValue(EG3->getIntAttribute("EG3_EG1Level"));//int iInitVal;
								tmpSplitInfo.EG3Info.vEG1Rate.setValue(EG3->getIntAttribute("EG3_EG1Rate"));//int iInitVal;

								tmpSplitInfo.EG3Info.vEG2Level.setValue(EG3->getIntAttribute("EG3_EG2Level"));//int iInitVal;
								tmpSplitInfo.EG3Info.vEG2Rate.setValue(EG3->getIntAttribute("EG3_EG2Rate"));//int iInitVal;

								tmpSplitInfo.EG3Info.vEG3Level.setValue(EG3->getIntAttribute("EG3_EG3Level"));//int iInitVal;
								int iTmp;
								iTmp = EG3->getIntAttribute("EG3_EG3Rate");//int iEG3Rate;
								tmpSplitInfo.EG3Info.vEG3Rate.setValue(iTmp & 0x7f);//int iInitVal;

								tmpSplitInfo.EG3Info.iKeyOffType = 0;

								//if version less than 1.04, PEG/FEG should use same hold flag as AEG
								if (iFileVersion <= ((SGI_FILE_EG23_CONVERT_MAJOR << 16) + SGI_FILE_EG23_CONVERT_MINOR))
								{
									if (tmpSplitInfo.EG1Info.iKeyOffType & KEY_OFF_PEDAL_HOLD)
									{
										iTmp |= 0x80;
									}
									else
									{
										iTmp &= 0x7f;
									}
								}
								if (iTmp & 0x80)
								{
									tmpSplitInfo.EG3Info.iKeyOffType |= KEY_OFF_PEDAL_HOLD;
								}

								tmpSplitInfo.EG3Info.vEG4Level.setValue(EG3->getIntAttribute("EG3_EG4Level"));//int iInitVal;
								iTmp = EG3->getIntAttribute("EG3_EG4Rate");//int iEG3Rate;
								tmpSplitInfo.EG3Info.vEG4Rate.setValue(iTmp & 0x7f);//int iInitVal;
								if (iTmp & 0x80)
								{
									tmpSplitInfo.EG3Info.iKeyOffType |= KEY_OFF_ONE_SHOT;
								}

								tmpSplitInfo.EG3Info.vEG4SLevel.setValue(tmpSplitInfo.EG3Info.vEG4Level.getValue());//int iInitVal;17Jun2013-use release level.
								tmpSplitInfo.EG3Info.vEG4SRate.setValue(EG3->getIntAttribute("EG3_EG4SRate"));//int iInitVal;
                            }
                        }
                        //-----------------------------------------------------------------
                        //-----------------------------------------------------------------
                        //						(layerEdit[iLayerNum]->iSplitNum)++;
						tmpSplitInfo.BasicInfo.bMute = false;
						tmpSplitInfo.BasicInfo.bSelect = false;
                        splitIndex++;
						(pSaveSgiFileInfo->allSplitInfo).push_back(tmpSplitInfo);
                    }
                }
				if (hasSplitInfo)
				{
					iCurrentLayer++;
				}
            }
        }
    }

    /**/	
    delete myElement;
    return 0;
}


int CSgiLoad::SaveFile(strSgiFileInfo* saveSgiFileInfo)
{
    //	FileChooser fc ("Choose a file to save..."),
    //				File::getCurrentWorkingDirectory(),
    //				"*.txt"),
    //				1);
    //----------------------------------------

	if (saveSgiFileInfo == NULL)
    {
        return SGI_SAVE_NO_DATA;
    }

	File sgiSave(saveSgiFileInfo->sSgiName);

    int i;
    CCopyFileList tmpFileList;

	std::vector<SPLIT_INFO>::iterator it;

	for (it = saveSgiFileInfo->allSplitInfo.begin(); it != saveSgiFileInfo->allSplitInfo.end(); it++)
	{
		tmpFileList.AddFile((*it).waveGen.cWaveName);
    }

    if(tmpFileList.size() == 0)
    {
        return SGI_SAVE_NO_SAMPLE;
    }

    String FullName,ShortName;
    XmlElement RAW_SAVELIST ("RAW_Instrument");
    //----------------------------------------------------

    RAW_SAVELIST.setAttribute("PATH",sgiSave.getFullPathName().replaceCharacter('\\','/'));
    RAW_SAVELIST.setAttribute("FileMajorVer",SGI_FILE_MAJOR_VERSION);
    RAW_SAVELIST.setAttribute("FileMinorVer",SGI_FILE_MINOR_VERSION);
    RAW_SAVELIST.setAttribute("CurrentLayerID",0);
    RAW_SAVELIST.setAttribute("CurrentSplitID",0);
    
    //

    RAW_SAVELIST.setAttribute("SampleCount", (int)tmpFileList.size());

    RAW_SAVELIST.setAttribute("Repeat",false);

    RAW_SAVELIST.setAttribute("CurSampleNum",0);
    for(i =0;i<int(tmpFileList.size());i++)
    {	
        RAW_SAVELIST.setAttribute("SampleList"+String(i),(tmpFileList[i]).fullName);
    }
    //----------------------------------------------------------------
	it = (saveSgiFileInfo->allSplitInfo).begin();
	int iCurrentLayer = (*it).iLayerIndex;

    XmlElement* Layer = new XmlElement ("Layer");
    Layer->setAttribute ("Trigger", "Note on");
    RAW_SAVELIST.addChildElement (Layer);

	for (; it != (saveSgiFileInfo->allSplitInfo).end(); it++)
    {
        if((*it).iLayerIndex != iCurrentLayer)
        {
			iCurrentLayer = (*it).iLayerIndex;
            Layer = new XmlElement ("Layer");
            Layer->setAttribute ("Trigger", "Note on");
            RAW_SAVELIST.addChildElement (Layer);
        }
        // create an inner element..
        XmlElement* Split = new XmlElement ("Split");
		Split->setAttribute("StartNote", (*it).BasicInfo.iStartKey);
        Split->setAttribute("EndNote", (*it).BasicInfo.iEndKey);

		Split->setAttribute("StartVel", (*it).BasicInfo.vStartVel.getValue());
		Split->setAttribute("EndVel", (*it).BasicInfo.vEndVel.getValue());
        Layer->addChildElement(Split);
        //---------------------------WaveGenertor----------------------
        XmlElement * WaveGenertor = new XmlElement ("WaveGenertor");
        //	String tempstring;
        //	tempstring=String(*((*it).waveGen.cWaveName));
        //	WaveGenertor->setAttribute("WaveName"),(*it).waveGen.cWaveName);
		WaveGenertor->setAttribute("WaveName", (*it).waveGen.cWaveName);
		WaveGenertor->setAttribute("SampleLen", (*it).waveGen.iSampleLen);
		WaveGenertor->setAttribute("WaveStart", (*it).waveGen.iWaveStart);
		WaveGenertor->setAttribute("LoopStart", (*it).waveGen.iLoopStart);
		WaveGenertor->setAttribute("LoopEnd", (*it).waveGen.iLoopEnd);
		WaveGenertor->setAttribute("LoopLen", (*it).waveGen.iLoopLength);
		WaveGenertor->setAttribute("LoopMode", (*it).waveGen.iLoopmode);
        WaveGenertor->setAttribute("SampleBit",(*it).waveGen.iSampleBit);
        WaveGenertor->setAttribute("KbdType",(*it).waveGen.iKbdType);

		WaveGenertor->setAttribute("IcTune", (*it).waveGen.iMCUCTune);
		WaveGenertor->setAttribute("IfTune", (*it).waveGen.iMCUFTune);

        WaveGenertor->setAttribute("IUsercTune",(*it).waveGen.vUserCTune.getValue());
        WaveGenertor->setAttribute("IUserfTune",(*it).waveGen.vUserFTune.getValue());

        WaveGenertor->setAttribute("Eg3Enable",(*it).waveGen.bEg3Enable);
        WaveGenertor->setAttribute("Eg3Amount",(*it).waveGen.vEg3Amount.getValue());
        WaveGenertor->setAttribute("LfoAmount",(*it).waveGen.vLfoAmount.getValue());
        WaveGenertor->setAttribute("Channel",(*it).waveGen.iChannel);
        Split->addChildElement(WaveGenertor);
        //---------------------------SampleAmplitude-------------------
        XmlElement * SampleAmplitude = new XmlElement ("SampleAmplitude");
        SampleAmplitude->setAttribute("Amplitude", (*it).sampleAmp.vAmplitude.getValue());
		SampleAmplitude->setAttribute("KbdTable", (*it).sampleAmp.bKbdTable);
		SampleAmplitude->setAttribute("KbdTableNum", (*it).sampleAmp.iKbdTableNum);
		SampleAmplitude->setAttribute("Dynamic", (*it).sampleAmp.bDynamic);
		SampleAmplitude->setAttribute("DynOffset", (*it).sampleAmp.vDynOffset.getValue());
		SampleAmplitude->setAttribute("DynSlope", (*it).sampleAmp.vDynSlope.getValue()*4);
        Split->addChildElement(SampleAmplitude);
        //---------------------------Filter----------------------------
        XmlElement * Filter = new XmlElement ("Filter");
        Filter->setAttribute("Fc",(*it).filter.vFc.getValue());
        Filter->setAttribute("KbdTable",(*it).filter.bKbdTable);
        Filter->setAttribute("KbdTableNum",(*it).filter.iKbdTableNum);
        Filter->setAttribute("Eg2",(*it).filter.bEg2);
        Filter->setAttribute("Lfo2",(*it).filter.bLfo2);
        Filter->setAttribute("Lfo2Amount",(*it).filter.vLfo2Amount.getValue());
        Filter->setAttribute("Boffset",(*it).filter.bOffset);
        Filter->setAttribute("Ioffset",(*it).filter.vOffset.getValue());
        Filter->setAttribute("FcDynamic",(*it).filter.bFcDynamic);
        Filter->setAttribute("FcDynOffset",(*it).filter.vFcDynOffset.getValue());
        Filter->setAttribute("FcDynSlope",(*it).filter.vFcDynSlope.getValue()*4);
        Filter->setAttribute("QaAmpulitude",(*it).filter.vQDynSlope.getValue()*4);
        Filter->setAttribute("QDynamic",(*it).filter.bQDynamic);
        Filter->setAttribute("QDynOffset",(*it).filter.vQDynOffset.getValue());
        Filter->setAttribute("QAmpulitude",(*it).filter.vQAmpulitude.getValue());
        Filter->setAttribute("Type",(*it).filter.iFilterType);
        Filter->setAttribute("Slope",(*it).filter.iFilterSlope);
        Split->addChildElement(Filter);
        //----------------------------OutAmpLitude-----------------------
        XmlElement * OutAmpLitude = new XmlElement ("OutAmpLitude");
        OutAmpLitude->setAttribute("Volume", (*it).outamp.iVolume);
        OutAmpLitude->setAttribute("KbdTable",(*it).outamp.bKbdTable);
        OutAmpLitude->setAttribute("KbdTableNum",(*it).outamp.iKbdTableNum);
        OutAmpLitude->setAttribute("Eg1",(*it).outamp.bEg1);
        OutAmpLitude->setAttribute("Lfo2",(*it).outamp.bLfo2);
        OutAmpLitude->setAttribute("Lfo2Amount",(*it).outamp.vLfo2Amount.getValue());
        OutAmpLitude->setAttribute("Boffset",(*it).outamp.bOffset);
        OutAmpLitude->setAttribute("Ioffset",(*it).outamp.iOffset);
        OutAmpLitude->setAttribute("Dynamic",(*it).outamp.bDynamic);
        OutAmpLitude->setAttribute("DynOffset",(*it).outamp.vDynOffset.getValue());
        OutAmpLitude->setAttribute("DynSlope",(*it).outamp.vDynSlope.getValue()*4);
        OutAmpLitude->setAttribute("MixL",(*it).outamp.vMixL.getValue());
        OutAmpLitude->setAttribute("MixR",(*it).outamp.vMixR.getValue());

		OutAmpLitude->setAttribute("Fx1Mix", (*it).outamp.vFX1Vol.getValue());
		OutAmpLitude->setAttribute("Fx2Mix", (*it).outamp.vFX2Vol.getValue());
		OutAmpLitude->setAttribute("Fx3Mix", (*it).outamp.vFX3Vol.getValue());
		OutAmpLitude->setAttribute("Fx4Mix", (*it).outamp.vFX4Vol.getValue());

        Split->addChildElement(OutAmpLitude);
        //---------------------------kbd 1-----------------------------
        XmlElement* Keyboard1 = new XmlElement ("Keyboard1");
        Keyboard1->setAttribute("TotalKbd1Nodes",int((*it).KBDInfo1.allNodeList.size()));
        Split->addChildElement(Keyboard1);
		for (std::vector <KBD_NODE>::iterator itkbd = (*it).KBDInfo1.allNodeList.begin();
			itkbd != (*it).KBDInfo1.allNodeList.end(); itkbd++)
        {
            XmlElement* Segment = new XmlElement ("Segment");
            Segment->setAttribute("Key", (*itkbd).iKey);
			Segment->setAttribute("Value", (*itkbd).iVal);
            Keyboard1->addChildElement(Segment);
        }

        //----------------------------kbd 2-------------------------------
        XmlElement* Keyboard2 = new XmlElement ("Keyboard2");
		Keyboard2->setAttribute("TotalKbd2Nodes", int((*it).KBDInfo2.allNodeList.size()));
        Split->addChildElement(Keyboard2);
		for (std::vector <KBD_NODE>::iterator itkbd = (*it).KBDInfo2.allNodeList.begin();
			itkbd != (*it).KBDInfo2.allNodeList.end(); itkbd++)
		{
			XmlElement* Segment = new XmlElement("Segment");
			Segment->setAttribute("Key", (*itkbd).iKey);
			Segment->setAttribute("Value", (*itkbd).iVal);
			Keyboard2->addChildElement(Segment);
		}
        //-------------------------------lfo1----------------------------------
        XmlElement* LFO1 = new XmlElement ("LFO1");
        LFO1->setAttribute("LFO1Type",(*it).LFO1Info.iType);//int iType;
		LFO1->setAttribute("LFO1Freq", (*it).LFO1Info.vFreq.getValue());//int iFreq;
		LFO1->setAttribute("LFO1Delay", (*it).LFO1Info.vDelay.getValue());//int iDelay;
		LFO1->setAttribute("LFO1Attack", (*it).LFO1Info.vAttack.getValue());//int iAttack;
        Split->addChildElement(LFO1);
        //-----------------------------------------------------------------
        //-------------------------------lfo0----------------------------------
        XmlElement* LFO2 = new XmlElement ("LFO2");
		LFO2->setAttribute("LFO2Type", (*it).LFO2Info.iType);//int iType;
		LFO2->setAttribute("LFO2Freq", (*it).LFO2Info.vFreq.getValue());//int iFreq;
		LFO2->setAttribute("LFO2Delay", (*it).LFO2Info.vDelay.getValue());//int iDelay;
		LFO2->setAttribute("LFO2Attack", (*it).LFO2Info.vAttack.getValue());//int iAttack;
        Split->addChildElement(LFO2);
        //--------------------------------EG1---------------------------------
        XmlElement* EG1 = new XmlElement ("EG1");
		EG1->setAttribute("EG1_EG1InitVal", (*it).EG1Info.vInitVal.getValue());//int iInitVal;
		EG1->setAttribute("EG1_EG1Level", (*it).EG1Info.vEG1Level.getValue());//int iEG1Level;
		EG1->setAttribute("EG1_EG1Rate", (*it).EG1Info.vEG1Rate.getValue());//int iEG1Rate;

		EG1->setAttribute("EG1_EG2Level", (*it).EG1Info.vEG2Level.getValue());//int iEG2Level;
		EG1->setAttribute("EG1_EG2Rate", (*it).EG1Info.vEG2Rate.getValue());//int iEG2Rate;

		EG1->setAttribute("EG1_EG3Level", (*it).EG1Info.vEG3Level.getValue());//int iEG3Level;
		if ((*it).EG1Info.iKeyOffType&KEY_OFF_PEDAL_HOLD)
		{
			EG1->setAttribute("EG1_EG3Rate", (*it).EG1Info.vEG3Rate.getValue() | 0x80);//int iEG3Rate;
		}
		else
		{
			EG1->setAttribute("EG1_EG3Rate", (*it).EG1Info.vEG3Rate.getValue());//int iEG3Rate;
		}

		EG1->setAttribute("EG1_EG4Level", (*it).EG1Info.vEG4Level.getValue());//int iEG4Level;
		if ((*it).EG1Info.iKeyOffType&KEY_OFF_ONE_SHOT)
		{
			EG1->setAttribute("EG1_EG4Rate", (*it).EG1Info.vEG4Rate.getValue() | 0x80);//int iEG4Rate;
		}
		else
		{
			EG1->setAttribute("EG1_EG4Rate", (*it).EG1Info.vEG4Rate.getValue());//int iEG4Rate;
		}

		EG1->setAttribute("EG1_EG4SLevel", (*it).EG1Info.vEG4SLevel.getValue());
		EG1->setAttribute("EG1_EG4SRate", (*it).EG1Info.vEG4SRate.getValue());
        Split->addChildElement(EG1);
        //--------------------------------------------------------------------
        //--------------------------------EG2---------------------------------
        XmlElement* EG2 = new XmlElement ("EG2");
		EG2->setAttribute("EG2_EG1InitVal", (*it).EG2Info.vInitVal.getValue());//int iInitVal; 
		EG2->setAttribute("EG2_EG1Level", (*it).EG2Info.vEG1Level.getValue());//int iEG1Level;
		EG2->setAttribute("EG2_EG1Rate", (*it).EG2Info.vEG1Rate.getValue());//int iEG1Rate;

		EG2->setAttribute("EG2_EG2Level", (*it).EG2Info.vEG2Level.getValue());//int iEG2Level;
		EG2->setAttribute("EG2_EG2Rate", (*it).EG2Info.vEG2Rate.getValue());//int iEG2Rate;

		EG2->setAttribute("EG2_EG3Level", (*it).EG2Info.vEG3Level.getValue());//int iEG3Level;
		if ((*it).EG2Info.iKeyOffType & KEY_OFF_PEDAL_HOLD)
		{
			EG2->setAttribute("EG2_EG3Rate", (*it).EG2Info.vEG3Rate.getValue() | 0x80);//int iEG3Rate;
		}
		else
		{
			EG2->setAttribute("EG2_EG3Rate", (*it).EG2Info.vEG3Rate.getValue());//int iEG3Rate;
		}

		EG2->setAttribute("EG2_EG4Level", (*it).EG2Info.vEG4Level.getValue());//int iEG4Level;
		if ((*it).EG2Info.iKeyOffType & KEY_OFF_ONE_SHOT)
		{
			EG2->setAttribute("EG2_EG4Rate", (*it).EG2Info.vEG4Rate.getValue() | 0x80);//int iEG4Rate;
		}
		else
		{
			EG2->setAttribute("EG2_EG4Rate", (*it).EG2Info.vEG4Rate.getValue());//int iEG4Rate;
		}

		EG2->setAttribute("EG2_EG4SLevel", (*it).EG2Info.vEG4SLevel.getValue());
		EG2->setAttribute("EG2_EG4SRate", (*it).EG2Info.vEG4SRate.getValue());
        Split->addChildElement(EG2);
        //--------------------------------------------------------------------
        //--------------------------------EG3---------------------------------
        XmlElement* EG3 = new XmlElement ("EG3");
		EG3->setAttribute("EG3_EG1InitVal", (*it).EG3Info.vInitVal.getValue());//int iInitVal;
		EG3->setAttribute("EG3_EG1Level", (*it).EG3Info.vEG1Level.getValue());//int iEG1Level;
		EG3->setAttribute("EG3_EG1Rate", (*it).EG3Info.vEG1Rate.getValue());//int iEG1Rate;

		EG3->setAttribute("EG3_EG2Level", (*it).EG3Info.vEG2Level.getValue());//int iEG2Level;
		EG3->setAttribute("EG3_EG2Rate", (*it).EG3Info.vEG2Rate.getValue());//int iEG2Rate;

		EG3->setAttribute("EG3_EG3Level", (*it).EG3Info.vEG3Level.getValue());//int iEG3Level;
		if ((*it).EG3Info.iKeyOffType&KEY_OFF_PEDAL_HOLD)
		{
			EG3->setAttribute("EG3_EG3Rate", (*it).EG3Info.vEG3Rate.getValue() | 0x80);//int iEG3Rate;
		}
		else
		{
			EG3->setAttribute("EG3_EG3Rate", (*it).EG3Info.vEG3Rate.getValue());//int iEG3Rate;
		}

		EG3->setAttribute("EG3_EG4Level", (*it).EG3Info.vEG4Level.getValue());//int iEG4Level;
		if ((*it).EG3Info.iKeyOffType&KEY_OFF_ONE_SHOT)
		{
			EG3->setAttribute("EG3_EG4Rate", (*it).EG3Info.vEG4Rate.getValue() | 0x80);//int iEG4Rate;
		}
		else
		{
			EG3->setAttribute("EG3_EG4Rate", (*it).EG3Info.vEG4Rate.getValue());//int iEG4Rate;
		}

		EG3->setAttribute("EG3_EG4SLevel", (*it).EG3Info.vEG4SLevel.getValue());
		EG3->setAttribute("EG3_EG4SRate", (*it).EG3Info.vEG4SRate.getValue());
        Split->addChildElement(EG3);
            //--------------------------------WaveGenertor---------------------------------

            //-----------------------------------------------------------------
        
    }
    // now we can turn the whole thing into a text document..
    //String myXmlDoc = RAW_SAVELIST.createDocument (String::empty);

    //----------------------------------------
    //if(chosenFile.exists())
    {
        RAW_SAVELIST.writeToFile(sgiSave,String::empty);
        FileOutputStream (sgiSave,16384);
    }
    return 0;
}
