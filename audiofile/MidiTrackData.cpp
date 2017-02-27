 #include "MidiTrackData.h"

#include <math.h>

bool mycompare(CMidiMessage *v1, CMidiMessage *v2)
{
    return (v1->GetTimeInTick() < v2->GetTimeInTick());
}

CMidiFileTrack::CMidiFileTrack()
{
    m_midiTrack = TRACK_EMPTY;
    playedMessageSet = false;
}


CMidiFileTrack::~CMidiFileTrack()
{
	ClearMessage();
}

void CMidiFileTrack::SetMidiTrackNum(int newtracknum)
{
	m_midiTrack = newtracknum;
}

int CMidiFileTrack::GetMidiTrackNum()
{
	return m_midiTrack;
}

void CMidiFileTrack::ClearMessage()
{
	for (std::vector <CMidiMessage *>::iterator it = m_allMessages.begin(); it != m_allMessages.end(); it++)
	if (NULL != *it)
	{
		delete *it;
		*it = NULL;
	}
	m_allMessages.clear();
}

//for midi edit.
//insert a midi message to the track.
//now only add to the message list.
bool CMidiFileTrack::AddMidiMessage(CMidiMessage *msg)
{
    if (msg == NULL)
    {
        return false;
    }
    CMidiMessage *tmpMidiMessage = new CMidiMessage;
    if (tmpMidiMessage == NULL)
    {
        return false;
    }
    *tmpMidiMessage = *msg;
    m_allMessages.push_back(tmpMidiMessage);
    
    if (m_midiTrack == TRACK_EMPTY)
    {
        m_midiTrack = tmpMidiMessage->getMidiTrack();
    }
    else if (m_midiTrack != TRACK_MIX)
    {
        if (m_midiTrack != tmpMidiMessage->getMidiTrack())
        {
            m_midiTrack = TRACK_MIX;
        }
    }
    
    return true;
}


void CMidiFileTrack::sortMidiMessage()
{
    std::sort(m_allMessages.begin(), m_allMessages.end(), mycompare);
}


bool CMidiFileTrack::AddFromTrackBuf(uint8 *buf, int bufLen)
{
	int offset = 0;
	int currentTickTime = 0;
	CMidiMessage *tmpMidiMessage;

	uint8 preEventByte = 0;

	for (;;)
	{
		tmpMidiMessage = new CMidiMessage;
		offset += tmpMidiMessage->loadFromRawData(buf + offset, currentTickTime, preEventByte);
		if (offset > bufLen)
		{
			delete tmpMidiMessage;
			return true;
		}
		currentTickTime = tmpMidiMessage->GetTimeInTick();
		
		if (tmpMidiMessage->getMessageType() == MIDI_MSG_USER_TRACK_END)
		{
            m_allMessages.push_back(tmpMidiMessage);
			break;
		}
        
        if (m_midiTrack == TRACK_EMPTY)
        {
            m_midiTrack = tmpMidiMessage->getMidiTrack();
        }
        else if (m_midiTrack != TRACK_MIX)
        {
            if (tmpMidiMessage->getMessageType() != MIDI_MSG_USER_NO_DEFINE)
            {
                if (m_midiTrack != tmpMidiMessage->getMidiTrack())
                {
                    m_midiTrack = TRACK_MIX;
                }
            }
            
        }
        
        if (tmpMidiMessage->getMessageType() != MIDI_MSG_USER_NO_DEFINE)
		{
			m_allMessages.push_back(tmpMidiMessage);
		}
		else
		{
			delete tmpMidiMessage;
		}
	}
	return false;
}


void CMidiFileTrack::ScanMatchNoteMessage()
{
    for (std::vector <CMidiMessage *>::iterator it1 = m_allMessages.begin(); it1 != m_allMessages.end(); it1++)
    {
        (*it1)->SetMatchMessage(NULL);
    }
    
    for (std::vector <CMidiMessage *>::iterator it1 = m_allMessages.begin(); it1 != m_allMessages.end(); it1++)
    {
        if ((*it1)->getMessageType() == MIDI_MSG_USER_NOTE_ON)
        {
            int noteVal = (*it1)->getNote();
            int track = (*it1)->getMidiTrack();
            
            for (std::vector <CMidiMessage *>::iterator it2 = it1+1; it2 != m_allMessages.end(); it2++)
            {
                if (((*it2)->getMessageType() == MIDI_MSG_USER_NOTE_OFF)
                    && ((*it2)->getMidiTrack() == track)
                    && ((*it2)->getNote() == noteVal))
                {
                    (*it1)->SetMatchMessage((*it2));
                    (*it2)->SetMatchMessage((*it1));
                    break;
                }
            }
        }
    }
}

void CMidiFileTrack::PrepareTickTimeArray(std::vector <TickTimeInfo> &allTickTimeInfo, int div, int &initTempo, int &initBeatType)
{
	if (GetMidiTrackNum() != -1)//only worked on control track.
	{
		return;
	}

	int saveTickTime = 1000000 / div;
	int beatPerPara = 4, beatType = 4; //4/4 and tempo = 60

	//save a new node when: 1.Tempo change  2.beat type change
	TickTimeInfo tempoInfo;

	uint32 timeInTick = 0;
	int timeInMS = 0;
	int timeInuSSave = 0;

	tempoInfo.timeInTick = timeInTick;
	tempoInfo.timeInMS = timeInMS;
	tempoInfo.tickTimsInUs = saveTickTime;
	tempoInfo.beatPerPara = beatPerPara;
	tempoInfo.beatType = beatType;
	tempoInfo.tempo = 60;
    
    int temp_us_past;

	allTickTimeInfo.push_back(tempoInfo);

	const STR_TEMPO_SAVE *msgTempInfo;

	for (std::vector <CMidiMessage *>::iterator it = m_allMessages.begin(); it != m_allMessages.end(); it++)
	{
		switch ((*it)->getMessageType())
		{
		case MIDI_MSG_USER_TEMPO:
            {
                temp_us_past = 0;
                if ((*it)->GetTimeInTick() == timeInTick)
                {
                    allTickTimeInfo.pop_back();
                }
                else
                {
                    temp_us_past = ((*it)->GetTimeInTick() - timeInTick)*saveTickTime + timeInuSSave;
                }
                msgTempInfo = (*it)->getTempoInfo();
                if ((saveTickTime != ((msgTempInfo->beatTime)/div)) || (allTickTimeInfo.size() == 0))
                {
                    timeInTick = (*it)->GetTimeInTick();
                    timeInMS += (temp_us_past / 1000);
                    timeInuSSave = temp_us_past % 1000;
                    
                    saveTickTime = (msgTempInfo->beatTime/div);
                    
                    tempoInfo.timeInTick = timeInTick;
                    tempoInfo.timeInMS = timeInMS;
                    tempoInfo.tickTimsInUs = saveTickTime;
                    float fTempo;
                    fTempo=60000000./ msgTempInfo->beatTime;
                    tempoInfo.tempo = ceilf(fTempo);
                    
                    if (initTempo == -1)
                    {
                        initTempo = tempoInfo.tempo;
                    }
                    
                    allTickTimeInfo.push_back(tempoInfo);
                }
            }
			break;
		case MIDI_MSG_USER_BEAT_INFO:
            {
                temp_us_past = 0;
                if ((*it)->GetTimeInTick() == timeInTick)
                {
                    allTickTimeInfo.pop_back();
                }
                else
                {
                    temp_us_past = ((*it)->GetTimeInTick() - timeInTick)*saveTickTime + timeInuSSave;
                }
                const STR_BEAT_TYPE_SAVE *msgBeatInfo = (*it)->getBeatInfo();
                
                if ((beatPerPara != msgBeatInfo->beatsPerPara) || (beatType != msgBeatInfo->paraType) || (allTickTimeInfo.size() == 0))
                {
                    timeInTick = (*it)->GetTimeInTick();
                    timeInMS += (temp_us_past / 1000);
                    timeInuSSave = temp_us_past % 1000;
                    
                    beatPerPara = msgBeatInfo->beatsPerPara;
                    beatType = msgBeatInfo->paraType;
                    
                    tempoInfo.timeInTick = timeInTick;
                    tempoInfo.timeInMS = timeInMS;
                    
                    tempoInfo.beatPerPara = beatPerPara;
                    tempoInfo.beatType = beatType;
                    
                    if(initBeatType == -1)
                    {
                        initBeatType = tempoInfo.beatType;
                        initBeatType |= ((tempoInfo.beatPerPara) << 16);
                    }
                    
                    allTickTimeInfo.push_back(tempoInfo);
                }
            }
			break;
        default:
            break;
		}
	}
    
    std::vector<TickTimeInfo>::iterator itTickTime;

    allTickTimeInfo[0].measureNumber = 0;
    allTickTimeInfo[0].tickLeftInMeasure = 0;
    
    if (allTickTimeInfo.size() > 1)
    {
        for (itTickTime = allTickTimeInfo.begin()+1; itTickTime != allTickTimeInfo.end(); itTickTime++)
        {
            std::vector<TickTimeInfo>::iterator preTickTime;
            preTickTime = itTickTime-1;
            
            int tickTimePast = (*itTickTime).timeInTick - (*preTickTime).timeInTick;
            int measureTicks = div*4*(*preTickTime).beatPerPara/(*preTickTime).beatType;
            
            (*itTickTime).measureNumber = (itTickTime-1)->measureNumber + (tickTimePast+(*preTickTime).tickLeftInMeasure)/measureTicks;
            (*itTickTime).tickLeftInMeasure = (tickTimePast+(*preTickTime).tickLeftInMeasure)%measureTicks;
        }
    }

}

void CMidiFileTrack::ConvertToTimeInMs(std::vector <TickTimeInfo> allTickTimeInfo, int &maxTimeInmS, int &maxTimeInTick)
{
	std::vector <TickTimeInfo>::iterator itTickTimeInfo = allTickTimeInfo.begin();

	int currentTimeInTick = (*itTickTimeInfo).timeInTick;
	int currentTimeInMs = (*itTickTimeInfo).timeInMS;
	int currentTickTimeInuS = (*itTickTimeInfo).tickTimsInUs;

	int nextTimeInTick = (*itTickTimeInfo).timeInTick;
	int nextTimeInMs = (*itTickTimeInfo).timeInMS;
	int nextTickTimeInuS = (*itTickTimeInfo).tickTimsInUs;

	itTickTimeInfo++;
	if (itTickTimeInfo != allTickTimeInfo.end())
	{
		nextTimeInTick = (*itTickTimeInfo).timeInTick;
		nextTimeInMs = (*itTickTimeInfo).timeInMS;
		nextTickTimeInuS = (*itTickTimeInfo).tickTimsInUs;
	}
	else
	{
		nextTimeInTick = 0xfffffff;
	}

	bool bEventFound;
	for (std::vector <CMidiMessage *>::iterator it = m_allMessages.begin(); it != m_allMessages.end(); it++)
	{
		int msgTickTime = (*it)->GetTimeInTick();

		bEventFound = false;

		if (msgTickTime >= nextTimeInTick)
		{
			for (; itTickTimeInfo != allTickTimeInfo.end(); itTickTimeInfo++)
			{
				if ((*itTickTimeInfo).timeInTick > msgTickTime)
				{
					currentTimeInTick = (*(itTickTimeInfo-1)).timeInTick;
					currentTimeInMs = (*(itTickTimeInfo - 1)).timeInMS;
					currentTickTimeInuS = (*(itTickTimeInfo - 1)).tickTimsInUs;

					nextTimeInTick = (*itTickTimeInfo).timeInTick;
					nextTimeInMs = (*itTickTimeInfo).timeInMS;
					nextTickTimeInuS = (*itTickTimeInfo).tickTimsInUs;

					bEventFound = true;
					break;
				}
			}
			if (!bEventFound)
			{
				itTickTimeInfo = allTickTimeInfo.end();
				itTickTimeInfo--;
				currentTimeInTick = (*itTickTimeInfo).timeInTick;
				currentTimeInMs = (*itTickTimeInfo).timeInMS;
				currentTickTimeInuS = (*itTickTimeInfo).tickTimsInUs;

				nextTimeInTick = 0xfffffff;
			}
		}

		int msTime = currentTimeInMs + (msgTickTime - currentTimeInTick)*currentTickTimeInuS / 1000;
		(*it)->SetTimeInMs(msTime);
		if (msTime > maxTimeInmS)
		{
			maxTimeInmS = msTime;
		}
		if (msgTickTime > maxTimeInTick)
		{
			maxTimeInTick = msgTickTime;
		}
	}

}

void CMidiFileTrack::StartPlay()
{
	playedMessage = m_allMessages.begin();
    playedMessageSet = true;
}


bool CMidiFileTrack::PlayToTickTime(uint32 tickTime, CMidiFileTrack *pTrackMixedTo)
{
    if (!playedMessageSet)
    {
        return false;//这种情况不能表示歌曲播放已经完成
    }
    if (m_allMessages.size() == 0)
    {
        return true;//歌曲播放已经完成
    }
	if (playedMessage == m_allMessages.end())
	{
		return true;//歌曲播放已经完成
	}
	for (;;)
	{
		if ((*playedMessage)->GetTimeInTick() >= tickTime)
		{
			break;
		}

        if (pTrackMixedTo != nullptr)
        {
            pTrackMixedTo->AddMidiMessage(*playedMessage);
        }
		
		if (playedMessage != m_allMessages.end())
		{
			playedMessage++;
		}
		if (playedMessage == m_allMessages.end())
		{
			return true;//歌曲播放完成
		}
	}
	return false;
}

std::vector <CMidiMessage *> *CMidiFileTrack::GetAllMessages()
{
	return &m_allMessages;
}

void CMidiFileTrack::saveToFile(FILE *fp)
{
    int currentTick = 0;
    for (std::vector <CMidiMessage *>::iterator it = m_allMessages.begin(); it != m_allMessages.end(); it++)
    {
        currentTick = (*it)->saveToFile(currentTick, fp);
    }
}
