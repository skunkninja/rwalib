/*
  ==============================================================================

    MidiFile.cpp
    Created: 8 Aug 2014 9:14:12am
    Author:  ZhouFeng

  ==============================================================================
*/

#include "rwaMidiFile.h"
#include "DataBas.h"

//#include <QuartzCore/CABase.h>
#include "sys/time.h"
#include "stdlib.h"

//This buf should like midi file's format
//All time should be 0.
uint8 pAllSoundOffMsgBuf[]=
{
    0,0xb0,0x78,00,
    0,0xb1,0x78,00,
    0,0xb2,0x78,00,
    0,0xb3,0x78,00,
    0,0xb4,0x78,00,
    0,0xb5,0x78,00,
    0,0xb6,0x78,00,
    0,0xb7,0x78,00,
    0,0xb8,0x78,00,
    0,0xb9,0x78,00,
    0,0xba,0x78,00,
    0,0xbb,0x78,00,
    0,0xbc,0x78,00,
    0,0xbd,0x78,00,
    0,0xbe,0x78,00,
    0,0xbf,0x78,00,
};

#define DEFAULT_TEMPO       100
#define DEFAULT_BEAT_TYPE   0x30004

RWAMidiFile::RWAMidiFile()
{
    bPlayFinished = true;
    bReadyForPlay = false;
    
    m_pAdditionalMsgAfter = new CMidiFileTrack;
    m_pAdditionalMsgBefore = new CMidiFileTrack;
    
    memcpy(midiHead.midiHeadID, "MThd", 4);
    midiHead.midiHeadLen = 6;
    midiHead.midiHeadDiv = 96;
    
    controlTrack = NULL;
}

RWAMidiFile::~RWAMidiFile()
{
	DeleteAllTracks();
}

void RWAMidiFile::DeleteAllTracks()
{
	for (std::vector <CMidiFileTrack *>::iterator it = allTracks.begin(); it != allTracks.end(); it++)
	{
		if (NULL != *it)
		{
			delete *it;
			*it = NULL;
		}
	}
    
    if (controlTrack != NULL)
    {
        delete controlTrack;
        controlTrack = NULL;
    }

	allTracks.clear();
}

/*MIDI_FILE_ERROR RWAMidiFile::LoadMidiFile(String fileName)
{
	int64 fileSize;
	File midifile(fileName);
	FileInputStream readStream(midifile);

	if (readStream.failedToOpen())
	{
		return MIDI_FILE_ERROR_FAIL_TO_OPEN;
	}

	fileSize = readStream.getTotalLength();
	midiFileBuffer = new uint8[int32(fileSize)];
	readStream.read(midiFileBuffer, int32(fileSize));
	
	MIDI_FILE_ERROR returnVal = LoadMidiFileFromBuffer(midiFileBuffer, int32(fileSize));

	delete[] midiFileBuffer;
	return returnVal;
}*/

#define READ_FROM_BUF(TARGET,SOURCE,SOURCE_OFFSET_IN_BYTE,LENGTH_IN_BYTE) \
{memcpy(TARGET, SOURCE + SOURCE_OFFSET_IN_BYTE, LENGTH_IN_BYTE);  \
	SOURCE_OFFSET_IN_BYTE += LENGTH_IN_BYTE;					\
}

void RWAMidiFile::UnloadMidiFile()
{
    m_FileTimeInmS = 0;
    m_FileTimeInTick = 0;
    
    m_loopStartTick = -1;
    m_loopEndTick = -1;
    
    m_speedRate = SPEED_NORMAL;
    m_rateModifyRemainder = 0;
    
    bPlayFinished = true;
    bReadyForPlay = false;
    
    DeleteAllTracks();
}

MIDI_FILE_ERROR RWAMidiFile::LoadMidiFileFromBuffer(uint8 *buf, int32 length)
{
	int32 offset = 0;
	uint8 *pTrackData = nullptr;
    
    UnloadMidiFile();

	if (length < (sizeof(MIDI_FILE_HEAD)+8))
	{
		return MIDI_FILE_ERROR_SIZE;
	}

	READ_FROM_BUF(&midiHead, buf, offset, sizeof(midiHead));

	LSB_MSB(midiHead.midiHeadLen);
	LSB_MSB(midiHead.midiHeadType);
	LSB_MSB(midiHead.midiHeadTracks);
	LSB_MSB(midiHead.midiHeadDiv);

	if (memcmp(midiHead.midiHeadID, "MThd", 4) != 0)
	{
		return MIDI_FILE_ERROR_HEAD;
	}
	if (midiHead.midiHeadLen != 6)
	{
		return MIDI_FILE_ERROR_HEAD;
	}
	if (midiHead.midiHeadType > 1)
	{
		return MIDI_FILE_ERROR_HEAD;
	}

	for (int trk = 0; trk < midiHead.midiHeadTracks; trk++)
	{
		MIDI_TRACK_HEAD trkHead;
		if ((length - offset) < sizeof(MIDI_TRACK_HEAD))
		{
			return MIDI_FILE_ERROR_TRACK_HEAD;
		}
		READ_FROM_BUF(&trkHead, buf, offset, sizeof(MIDI_TRACK_HEAD));

		if (memcmp(trkHead.midiTrkID, "MTrk", 4) != 0)//
		{
			return MIDI_FILE_ERROR_TRACK_HEAD;
		}

		LSB_MSB(trkHead.midiTrkLen);
		pTrackData = new uint8[trkHead.midiTrkLen];//

		if (pTrackData != nullptr)
		{
			if ((length - offset) < int(trkHead.midiTrkLen))
			{
				delete[] pTrackData;
				pTrackData = nullptr;
				return MIDI_FILE_ERROR_DATA;
			}
			READ_FROM_BUF(pTrackData, buf, offset, trkHead.midiTrkLen);
			//--
			
			
			if ((midiHead.midiHeadType == 0) && (midiHead.midiHeadTracks == 1))
			{
				//midi 0 file, need special code to split the one track data to 16 tracks.
                CMidiFileTrack *tempTrack = new CMidiFileTrack;
				tempTrack->AddFromTrackBuf(pTrackData, trkHead.midiTrkLen);
                
                if (tempTrack->GetMidiTrackNum() == TRACK_MIX)//if the midi track has no control track, the midi file is not correct.
                {
                    CMidiFileTrack *addTrack = nullptr;
                    controlTrack = new CMidiFileTrack;
                    const std::vector <CMidiMessage *> *pAllMessage = tempTrack->GetAllMessages();
                    
                    for (std::vector <CMidiMessage *>::const_iterator itMsg = pAllMessage->begin(); itMsg != pAllMessage->end(); itMsg++)
                    {
                        int track = (*itMsg)->getMidiTrack();
                        
                        if (track == TRACK_CONTROL)
                        {
                            controlTrack->AddMidiMessage(*itMsg);
                        }
                        else
                        {
                            bool found = false;
                            for (std::vector <CMidiFileTrack *>::iterator itTrk = allTracks.begin(); itTrk != allTracks.end(); itTrk++)
                            {
                                if (track == (*itTrk)->GetMidiTrackNum()) {
                                    found = true;
                                    addTrack = (*itTrk);
                                    break;
                                }
                            }
                            if (!found)
                            {
                                addTrack = new CMidiFileTrack;
                                allTracks.push_back(addTrack);
                            }
                            
                            if (addTrack != nullptr)
                            {
                                addTrack->AddMidiMessage(*itMsg);
                            }
                        }
                    }
                }
                
                for (std::vector <CMidiFileTrack *>::iterator itTrk = allTracks.begin(); itTrk != allTracks.end(); itTrk++)
                {
                    (*itTrk)->ScanMatchNoteMessage();
                }
                
                delete tempTrack;
			}
			else
			{
                if (trk == 0)
                {
                    controlTrack = new CMidiFileTrack;
                    controlTrack->AddFromTrackBuf(pTrackData,  trkHead.midiTrkLen);
                    controlTrack->SetMidiTrackNum(TRACK_CONTROL);
                }
                else
                {
                    CMidiFileTrack *tempTrack = new CMidiFileTrack;
                    tempTrack->AddFromTrackBuf(pTrackData, trkHead.midiTrkLen);
                    tempTrack->ScanMatchNoteMessage();
                    allTracks.push_back(tempTrack);
                }
			}

			delete [] pTrackData;
			pTrackData = nullptr;
		}
		else
		{
			return MIDI_FILE_ERROR_MEMORY;
		}
	}
	if (pTrackData != nullptr)
	{
		delete[] pTrackData;
	}
	return MIDI_CLASS_OK;
}

int RWAMidiFile::GetTotalTrackNum()
{
	return (int)allTracks.size();
}

CMidiFileTrack* RWAMidiFile::GetTrackInfo(int trackNum)
{
	for (std::vector <CMidiFileTrack *>::iterator it = allTracks.begin(); it != allTracks.end(); it++)
    {
        if (NULL != *it)
        {
            if ((*it)->GetMidiTrackNum() == trackNum)
            {
                return *it;
            }
        }
    }

	return nullptr;
}

int RWAMidiFile::GetDiv()
{
	return midiHead.midiHeadDiv;
}

void RWAMidiFile::SetDiv(int div)
{
    midiHead.midiHeadDiv = div;
}

void RWAMidiFile::PreparePlay()
{
    m_FileTimeInmS = 0;
    m_FileTimeInTick = 0;
    allTickTimeInfo.clear();
    
    m_initTempo = -1;
    m_initBeatType = -1;
    

    controlTrack->PrepareTickTimeArray(allTickTimeInfo, GetDiv(), m_initTempo, m_initBeatType);

    
    if (allTickTimeInfo.size() == 0)
    {
        //some fault.
    }
    else
    {
        controlTrack->ConvertToTimeInMs(allTickTimeInfo, m_FileTimeInmS, m_FileTimeInTick);
        controlTrack->StartPlay();
        for (std::vector <CMidiFileTrack *>::iterator it = allTracks.begin(); it != allTracks.end(); it++)
        {
            if (NULL != (*it))
            {
                //if ((*it)->GetMidiTrackNum() != -1)
                {
                    (*it)->ConvertToTimeInMs(allTickTimeInfo, m_FileTimeInmS, m_FileTimeInTick);
                    (*it)->StartPlay();
                }
            }
        }
    }
   
	if(m_initTempo == -1)
	{
		m_initTempo = DEFAULT_TEMPO;
	}
	
	if(m_initBeatType == -1)
	{
		m_initBeatType = DEFAULT_BEAT_TYPE; //3/4
	}
    
    bReadyForPlay = true;
    bPlayFinished = true;
    bStopFlag = false;
    
    m_iCurrentTimeInMs = 0;
    m_fCurrentTimeInTick = 0.0f;
}

void RWAMidiFile::StartPlay()
{
    if (!bReadyForPlay) {
        return;
    }
    timeval time;
    gettimeofday(&time, NULL);
	m_previousTime = time.tv_sec*1000+time.tv_usec/1000;
    m_pAdditionalMsgAfter->ClearMessage();
    m_pAdditionalMsgBefore->ClearMessage();
    
    bPlayFinished = false;
    bStopFlag = false;
}

void RWAMidiFile::StopPlay()
{
    if(!isReadyForPlay())
    {
        return;
    }
    bStopFlag = true;
    PerpareAdditionalMsg(m_pAdditionalMsgAfter,pAllSoundOffMsgBuf, sizeof(pAllSoundOffMsgBuf));
}


bool RWAMidiFile::PlayPastTime(std::vector<RAW_MIDI_MSG> &allRawMsg)
{
    bool playResult;
    CMidiFileTrack tmpTrackToPlay;
    playResult = PlayPastTime(&tmpTrackToPlay);
    
    ConvertAllPlayedMessages(allRawMsg, tmpTrackToPlay);
    
    return playResult;
}

bool RWAMidiFile::PlayToTickTime(int tickTime, std::vector<RAW_MIDI_MSG> &allRawMsg)
{
    bool playResult;
    CMidiFileTrack tmpTrackToPlay;
    playResult = PlayToTickTime(tickTime, &tmpTrackToPlay);
    
    ConvertAllPlayedMessages(allRawMsg, tmpTrackToPlay);
    
    return playResult;
}

void RWAMidiFile::ConvertAllPlayedMessages(std::vector<RAW_MIDI_MSG> &allRawMsg, CMidiFileTrack &tmpTrackToPlay)
{
    const std::vector <CMidiMessage *> *pMessagesToPlay = tmpTrackToPlay.GetAllMessages();
    
    for (std::vector <CMidiMessage *>::const_iterator it = pMessagesToPlay->begin(); it != pMessagesToPlay->end(); it++)
    {
        RAW_MIDI_MSG tmpRawMsg;
        if ((*it)->getMessageType() > MIDI_MSG_USER_PITCH)
        {
            if ((*it)->getMessageType() == MIDI_MSG_USER_TEMPO)
            {
                //Thread::sleep(1);
                //int tempo = ((STR_TEMPO_SAVE *)(*it)->GetMessageData())->tempo;
                
                tmpRawMsg.userMsgType = MIDI_MSG_USER_TEMPO;
                tmpRawMsg.beatTime = ((*it)->getTempoInfo())->beatTime;
                tmpRawMsg.msgBuf = NULL;
                
                allRawMsg.push_back(tmpRawMsg);
                
            }
            else if((*it)->getMessageType() == MIDI_MSG_USER_BEAT_INFO)
            {
                const STR_BEAT_TYPE_SAVE *tmpBeatInfo = (*it)->getBeatInfo();
                
                tmpRawMsg.userMsgType = MIDI_MSG_USER_BEAT_INFO;
                tmpRawMsg.beatType = tmpBeatInfo->beatInfo;
                
                tmpRawMsg.msgBuf = NULL;
                
                allRawMsg.push_back(tmpRawMsg);
            }
            else if((*it)->getMessageType() == MIDI_MSG_USER_SYSEX)
            {
                
                uint8 *pMidiData = (uint8 *)((*it)->getSysexBuf());
                int dataLen = (*it)->getSysexMsgLen();
                
                tmpRawMsg.userMsgType = MIDI_MSG_USER_SYSEX;
                tmpRawMsg.msgLen = dataLen+2;
                tmpRawMsg.msgBuf = (uint8 *)malloc(dataLen+2);
                
                tmpRawMsg.msgBuf[0] = 0xf0;
                memcpy(tmpRawMsg.msgBuf+1, pMidiData, dataLen);
                tmpRawMsg.msgBuf[dataLen+1] = 0xf7;
                
                allRawMsg.push_back(tmpRawMsg);
            }
        }
        else
        {
            tmpRawMsg.userMsgType = (*it)->getMessageType();
            uint8 FirstByte = tblMidiTypeToFileData[(*it)->getMessageType()];
            FirstByte |= (*it)->getMidiTrack();
            int dataLen = tblMidiTypeDataLen[(*it)->getMessageType()];
            
            tmpRawMsg.msgLen = dataLen+1;
            tmpRawMsg.msgBuf = (uint8 *)malloc(dataLen+1);
            tmpRawMsg.msgBuf[0] = FirstByte;
            tmpRawMsg.msgBuf[1] = (*it)->getShortData1();
            if (dataLen == 2) {
                tmpRawMsg.msgBuf[2] = (*it)->getShortData2();
            }
            
            allRawMsg.push_back(tmpRawMsg);
        }
    }
}

bool RWAMidiFile::PlayPastTime(CMidiFileTrack *pTrackMixedTo)
{
    uint64 currentTime;
    timeval time;
    gettimeofday(&time, NULL);
    currentTime = time.tv_sec*1000+time.tv_usec/1000;
    
	uint32 timePast = uint32(currentTime - m_previousTime);
    m_previousTime = currentTime;
    
    if(timePast > 50)
    {
        timePast = 50;
    }
    
    timePast *= m_speedRate;
    timePast += m_rateModifyRemainder;
    
    m_rateModifyRemainder = timePast%SPEED_NORMAL;
    timePast /= SPEED_NORMAL;
    
    m_iCurrentTimeInMs += timePast;
    
    m_fCurrentTimeInTick = converToTickTime(m_iCurrentTimeInMs, false);
    
    
    return PlayToTickTime(m_fCurrentTimeInTick, pTrackMixedTo);
}


bool RWAMidiFile::PlayToTickTime(int playToTick, CMidiFileTrack *pTrackMixedTo)
{
    bPlayFinished = true;
    m_pAdditionalMsgBefore->PlayToTickTime(1, pTrackMixedTo);
    m_pAdditionalMsgBefore->ClearMessage();//remove all message. Play once only!
    
    hasLooped = false;
    if(controlTrack->PlayToTickTime(playToTick, pTrackMixedTo) == false)
    {
        bPlayFinished = false;
    }
    for (std::vector <CMidiFileTrack *>::iterator it = allTracks.begin(); it != allTracks.end(); it++)
    {
        if (NULL != *it)
        {
            if ((*it)->PlayToTickTime(playToTick, pTrackMixedTo) == false)
            {
                bPlayFinished = false;
            }
        }
    }
    if ((m_loopEndTick != -1) && (m_loopStartTick != -1) && (m_loopEndTick > m_loopStartTick))
    {
        if (m_fCurrentTimeInTick > m_loopEndTick)
        {
            m_iCurrentTimeInMs = converToMSTime(m_loopStartTick, false);
            m_fCurrentTimeInTick = m_loopStartTick;
            
            SeekToTime(m_fCurrentTimeInTick);
            
            hasLooped = true;
        }
    }
    
    m_pAdditionalMsgAfter->PlayToTickTime(1, pTrackMixedTo);
    m_pAdditionalMsgAfter->ClearMessage();//remove all message. Play once only!
    
    if (bStopFlag)
    {
        bStopFlag = false;
        bPlayFinished = true;
    }
    
    return bPlayFinished;
}

float RWAMidiFile::GetCurrentTime(TIME_MODE timemode)
{
    if (timemode == TIME_MODE_MS)
    {
        int64 tmpVal = m_iCurrentTimeInMs;
        tmpVal*=SPEED_NORMAL;
        tmpVal/=m_speedRate;
        return float(tmpVal);
    }
    else
    {
        return m_fCurrentTimeInTick;
    }
}

int RWAMidiFile::GetCurrentMsTimeNotRated()
{
    return m_iCurrentTimeInMs;
}

float RWAMidiFile::converToTickTime(int timeInmS, bool useConvertedTime)
{
    TickTimeInfo lastTickTimeInfo;
    bool bFound = false;
    std::vector <TickTimeInfo>::iterator it;
    
    if (useConvertedTime)
    {
        int64 tmpTime = timeInmS;
        tmpTime *= m_speedRate;
        tmpTime /= SPEED_NORMAL;
        timeInmS = int(tmpTime);
    }
    
    
    for (it = allTickTimeInfo.begin(); it != allTickTimeInfo.end(); it++)
    {
        if (timeInmS < (*it).timeInMS)
        {
            if (it != allTickTimeInfo.begin()) {
                memcpy(&lastTickTimeInfo, &(*(it-1)), sizeof(lastTickTimeInfo));
                bFound = true;
            }
            break;
        }
    }
    
    if ((!bFound) && (allTickTimeInfo.size()!=0))
    {
        it = allTickTimeInfo.end()-1;
        if ((*it).timeInMS < timeInmS) {
            memcpy(&lastTickTimeInfo, &(*it), sizeof(lastTickTimeInfo));
            bFound = true;
        }
    }
    
    if (bFound)
    {
        int msOffset = timeInmS-lastTickTimeInfo.timeInMS;
        float tickOffset = float(msOffset)*1000./float(lastTickTimeInfo.tickTimsInUs);
        return float(lastTickTimeInfo.timeInTick)+tickOffset;
    }
    else
    {
        return 0.f;
    }
}

int RWAMidiFile::converToMSTime(int timeInTick, bool useConvertTime)
{
    TickTimeInfo lastTickTimeInfo;
    bool bFound = false;
    std::vector <TickTimeInfo>::iterator it;
    for (it = allTickTimeInfo.begin(); it != allTickTimeInfo.end(); it++)
    {
        if (timeInTick < (*it).timeInTick)
        {
            if (it != allTickTimeInfo.begin()) {
                memcpy(&lastTickTimeInfo, &(*(it-1)), sizeof(lastTickTimeInfo));
                bFound = true;
            }
            break;
        }
    }
    
    if ((!bFound) && (allTickTimeInfo.size()!=0))
    {
        it = allTickTimeInfo.end()-1;
        if ((*it).timeInTick <= timeInTick) {
            memcpy(&lastTickTimeInfo, &(*it), sizeof(lastTickTimeInfo));
            bFound = true;
        }
    }
    
    if (bFound)
    {
        int tickOffset = timeInTick-lastTickTimeInfo.timeInTick;
        int msOffset = lastTickTimeInfo.tickTimsInUs*tickOffset/1000;
        int64 tmpValue = lastTickTimeInfo.timeInMS+msOffset;
        if (useConvertTime) {
            tmpValue *= SPEED_NORMAL;
            tmpValue /= m_speedRate;
        }
        return int(tmpValue);
    }
    else
    {
        return 0;
    }
}

/*
bool RWAMidiFile::PlayToMsTime(std::vector <CMidiMessage *> &messageNeedPlay, int playedToTimeInmS)
{
    for (std::vector <CMidiFileTrack *>::iterator it = allTracks.begin(); it != allTracks.end(); it++)
    {
        if (NULL != *it)
        {
            if ((*it)->GetMidiTrackNum())
            {
                if ((*it)->PlayToMsTime(playedToTimeInmS, messageNeedPlay) == false)
                {
                    bPlayFinished = false;
                }
            }
        }
    }
    return bPlayFinished;
}*/

int RWAMidiFile::GetFileTime(TIME_MODE timemode)
{
    if (timemode==TIME_MODE_MS)
    {
        int64 tmpVal = m_FileTimeInmS;
        tmpVal*=SPEED_NORMAL;
        tmpVal/=m_speedRate;
        return int(tmpVal);
    }
    else
    {
        return m_FileTimeInTick;
    }
}

bool RWAMidiFile::isPlayFinished()
{
    return bPlayFinished;
}

bool RWAMidiFile::isAutoFinished()
{
    if (!bPlayFinished) {
        return false;
    }
    else if(bStopFlag)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void RWAMidiFile::PrepareParaDisplayData(std::vector <paraDisplayInfo> &paraInfo)
{
    int lastSavedTick = 0;
    int beatNum = 0, paraNum = 0;
    
    for (std::vector <TickTimeInfo>::iterator it = allTickTimeInfo.begin(); it != allTickTimeInfo.end(); it++)
    {
        if ((it + 1) != allTickTimeInfo.end())
        {
            PrepareParaDisplayArea(&paraInfo, it, (*(it + 1)).timeInTick, lastSavedTick, beatNum, paraNum);
        }
        else
        {
            PrepareParaDisplayArea(&paraInfo, it, m_FileTimeInTick, lastSavedTick, beatNum, paraNum);
        }
    }
}


void RWAMidiFile::PrepareParaDisplayArea(std::vector <paraDisplayInfo> *pTmpParaDisplayInfo,
                                         std::vector <TickTimeInfo>::iterator currentParaInfo,
                                         int endTickTime, int &lastSavedTick, int &beatNum, int &paraNum)
{
    paraDisplayInfo tempDisplayInfo;
    
    int beatTick = GetDiv() * 4 / (*currentParaInfo).beatType;
    
    
    for (; lastSavedTick < endTickTime; lastSavedTick += beatTick)
    {
        tempDisplayInfo.para = paraNum;
        tempDisplayInfo.timeInmS = (lastSavedTick - (*currentParaInfo).timeInTick)*((*currentParaInfo).tickTimsInUs) / 1000 + (*currentParaInfo).timeInMS;
        tempDisplayInfo.timeInTick = lastSavedTick;
        
        if ((beatNum % (*currentParaInfo).beatPerPara) == 0)
        {
            tempDisplayInfo.type = DISPLAY_PARA;
        }
        else
        {
            tempDisplayInfo.type = DISPLAY_BEAT;
        }
        
        pTmpParaDisplayInfo->push_back(tempDisplayInfo);
        
        beatNum++;
        if ((beatNum % (*currentParaInfo).beatPerPara) == 0)
        {
            beatNum = 0;
            paraNum++;
        }
    }
}

void RWAMidiFile::PrepareNoteDisplayData(std::vector <noteDisplayInfo> &noteInfo, int track)
{
    noteDisplayInfo tempNoteInfo;
    CMidiFileTrack *tempMidiTrack = GetTrackInfo(track);
    
    if (tempMidiTrack != nullptr) {
        const std::vector <CMidiMessage *> *pTrackMessages;
        pTrackMessages = tempMidiTrack->GetAllMessages();
        
        for (std::vector <CMidiMessage *>::const_iterator it = pTrackMessages->begin(); it != pTrackMessages->end(); it++)
        {
            if ((*it)->getMessageType() == MIDI_MSG_USER_NOTE_ON)
            {
                tempNoteInfo.startTimeInmS = (*it)->GetTimeInMs();
                tempNoteInfo.startTimeInTick = (*it)->GetTimeInTick();
                CMidiMessage *pMatchMsg;
                pMatchMsg = (*it)->GetMatchMessage();
                if (pMatchMsg != nullptr)
                {
                    tempNoteInfo.endTimeInmS = pMatchMsg->GetTimeInMs();
                    tempNoteInfo.endTimeInTick = pMatchMsg->GetTimeInTick();
                    tempNoteInfo.keyVal = (*it)->getNote();
                    noteInfo.push_back(tempNoteInfo);
                }
            }
        }
    }
}

void RWAMidiFile::PerpareAdditionalMsg(CMidiFileTrack *pTrack, uint8 *buf, int bufLen)
{
    //not clear message in "pTrack".
    pTrack->AddFromTrackBuf(buf, bufLen);
    pTrack->ConvertToTimeInMs(allTickTimeInfo, m_FileTimeInmS, m_FileTimeInTick);
    pTrack->StartPlay();
}

int RWAMidiFile::GetInitTempo()
{
    if (bReadyForPlay)
    {
        return m_initTempo;
    }
    else
    {
        return DEFAULT_TEMPO;
    }
}

//  bit 0-15  beat type.
//  bit 15-30 beats per para
int RWAMidiFile::GetInitBeatType()
{
    if (bReadyForPlay)
    {
        return m_initBeatType;
    }
    else
    {
        return DEFAULT_BEAT_TYPE;//4/4
    }
}

bool RWAMidiFile::isReadyForPlay()
{
    return bReadyForPlay;
}

void RWAMidiFile::SetLoopTime(int iLoopStartTime, int iLoopEndTime, TIME_MODE timemode)
{
    if (timemode == TIME_MODE_MS) {
        iLoopStartTime = converToTickTime(iLoopStartTime);
        iLoopEndTime = converToTickTime(iLoopEndTime);
    }
    
    m_loopStartTick = iLoopStartTime;
    m_loopEndTick = iLoopEndTime;
}

void RWAMidiFile::SeekToTime(int iStartTime, TIME_MODE timemode, CMidiFileTrack *pTrackMixedTo)
{
    if (!bReadyForPlay)
    {
        return;
    }
    int timeSave = iStartTime;
    
    if (timemode == TIME_MODE_MS) {
        iStartTime = converToTickTime(iStartTime);
    }
    
    //int64 tmpVal = iStartTime;
    //tmpVal*=m_speedRate;
    //tmpVal/=SPEED_NORMAL;
    
    //iStartTime = int(tmpVal);
    
    controlTrack->StartPlay();
    controlTrack->PlayToTickTime(iStartTime, pTrackMixedTo);//目前的程序,控制轨包含的系统信息中有音色事件,所以跳转时需要处理。
    for (std::vector <CMidiFileTrack *>::iterator it = allTracks.begin(); it != allTracks.end(); it++)
    {
        if (NULL != (*it))
        {
            (*it)->StartPlay();
            (*it)->PlayToTickTime(iStartTime, NULL);//目前普通轨中的事件在跳转时不需要播放。
        }
    }
    if (timemode == TIME_MODE_MS)
    {
        m_iCurrentTimeInMs = timeSave;
    }
    else
    {
        m_iCurrentTimeInMs = converToMSTime(timeSave, false);
    }
    
    if (timemode == TIME_MODE_TICK)
    {
        m_fCurrentTimeInTick = timeSave;
    }
    else
    {
        m_fCurrentTimeInTick = converToTickTime(timeSave, false);
    }
    m_rateModifyRemainder = 0;
    PerpareAdditionalMsg(m_pAdditionalMsgAfter,pAllSoundOffMsgBuf, sizeof(pAllSoundOffMsgBuf));
}

void RWAMidiFile::SetPlaySpeedRate(uint32 newRate)
{
    if (newRate > SPEED_NORMAL*4) {
        m_speedRate = SPEED_NORMAL*4;
    }
    else if (newRate < SPEED_NORMAL/4)
    {
        m_speedRate = SPEED_NORMAL/4;
    }
    else
    {
        m_speedRate = newRate;
    }
}

bool RWAMidiFile::hasLoopOccured()
{
    return hasLooped;
}

void RWAMidiFile::getCurrentMeasureTicks(int *pMeasure, int *pTicks, int *pTicksPerMeasure, int *pTicksPerBeat)
{
    if (!isReadyForPlay())
    {
        return;
    }
    int currentTickTime = (int)m_fCurrentTimeInTick;
    //currentTickTime = 125760;
    std::vector <TickTimeInfo>::reverse_iterator itTickTime;

    for (itTickTime = allTickTimeInfo.rbegin(); itTickTime != allTickTimeInfo.rend(); itTickTime++)
    {
        if (currentTickTime >= (*itTickTime).timeInTick)
        {
            break;
        }
    }
    
    int measureTicks = GetDiv()*4*(*itTickTime).beatPerPara/(*itTickTime).beatType;
    
    *pMeasure = ((currentTickTime + (*itTickTime).tickLeftInMeasure - (*itTickTime).timeInTick)/measureTicks)+(*itTickTime).measureNumber;
    *pTicks = (currentTickTime + (*itTickTime).tickLeftInMeasure -(*itTickTime).timeInTick)%measureTicks;
    *pTicksPerMeasure = measureTicks;
    *pTicksPerBeat = GetDiv()*4/(*itTickTime).beatType;
}

int RWAMidiFile::getMeasureStartTime(int measure, TIME_MODE timemode)
{
    if (!isReadyForPlay())
    {
        return -1;
    }
    
    std::vector <TickTimeInfo>::reverse_iterator itTickTime;
    
    for (itTickTime = allTickTimeInfo.rbegin(); itTickTime != allTickTimeInfo.rend(); itTickTime++)
    {
        if (measure >= (*itTickTime).measureNumber)
        {
            int startTick = (*itTickTime).timeInTick - (*itTickTime).tickLeftInMeasure;
            
            int measureTicks = GetDiv()*4*(*itTickTime).beatPerPara/(*itTickTime).beatType;
            
            startTick += (measure - (*itTickTime).measureNumber)*measureTicks;
            return startTick;
            break;
        }
    }
    return -1;
}

void RWAMidiFile::AddFileTrack(CMidiFileTrack *newTrack)
{
    newTrack->StartPlay();
    allTracks.push_back(newTrack);
}

void RWAMidiFile::SaveToMidi0(char* fileName)
{
    FILE *fp;
    
    if (allTracks.size() == 0) {
        return;
    }
    
    if ((fp = fopen(fileName, "wb")) == NULL) {
        return;
    }
    
    MIDI_FILE_HEAD tmpHead;
    memcpy(&tmpHead, &midiHead, sizeof(MIDI_FILE_HEAD));
    tmpHead.midiHeadType = 0;
    tmpHead.midiHeadTracks = 1;
    
    LSB_MSB(tmpHead.midiHeadLen);
    LSB_MSB(tmpHead.midiHeadType);
    LSB_MSB(tmpHead.midiHeadTracks);
    LSB_MSB(tmpHead.midiHeadDiv);
    
    fwrite(&tmpHead, sizeof(MIDI_FILE_HEAD), 1, fp);
    
    MIDI_TRACK_HEAD tmpTrackHead;
    memcpy(&tmpTrackHead.midiTrkID, "MTrk", sizeof(MIDI_TRACK_HEAD));
    tmpTrackHead.midiTrkLen = 0;
    fwrite(&tmpTrackHead, sizeof(MIDI_TRACK_HEAD), 1, fp);
    
    uint32 trackStart = ftell(fp);
    
    //should add mix all midi track to one track here. Now we only support one track for test.
    CMidiFileTrack *tmpTrack = allTracks[0];
    
    tmpTrack->saveToFile(fp);
    
    uint32 dataLen = ftell(fp)-trackStart;
    
    LSB_MSB(dataLen);
    fseek(fp, trackStart-4, SEEK_SET);
    fwrite(&dataLen, 4, 1, fp);
    fclose(fp);
}
