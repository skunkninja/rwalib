#include "MidiMessage.h"
#include <math.h>
#include <string.h>

CMidiMessage::CMidiMessage()
{
	pSysexData = nullptr;
	m_MidiTrack = TRACK_EMPTY;
	m_pMatchMsg = nullptr;
    sysexLen = 0;
    m_MidiMessageType = MIDI_MSG_USER_NO_DEFINE;
}


CMidiMessage::~CMidiMessage()
{
	ReleaseMemory();
}

void CMidiMessage::ReleaseMemory()
{
	if (pSysexData != nullptr)
	{
		delete (uint8 *)pSysexData;
		pSysexData = nullptr;
        sysexLen = 0;
	}
	m_pMatchMsg = nullptr;//DO NOT release the memory!
}

void CMidiMessage::clearMessage()
{
    ReleaseMemory();
    m_MidiTrack = -1;
    sysexLen = 0;
    m_MidiMessageType = MIDI_MSG_USER_NO_DEFINE;
}

MIDI_MESSAGE_USER CMidiMessage::getMessageType()
{
	return m_MidiMessageType;
}
int CMidiMessage::getMidiTrack()
{
	return m_MidiTrack;
}

void CMidiMessage::setMidiTrack(int trackNum)
{
    m_MidiTrack = trackNum;
}

uint32 CMidiMessage::GetTimeInTick()
{
	return m_TickTime;
}

void CMidiMessage::SetTimeInMs(uint32 msTime)
{
	m_TimeInMillisecond = msTime;
}

uint32 CMidiMessage::GetTimeInMs()
{
	return m_TimeInMillisecond;
}

uint8 tblMidiTypeToFileData[] =
{
	0x80,
	0x90,
	0xA0,
	0xB0,
	0xC0,
	0xD0,
	0xE0,
};

uint8 tblMidiTypeDataLen[] = 
{
	2,
	2,
	2,
	2,
	1,
	1,
	2,
};

int CMidiMessage::loadFromRawData(uint8 *pRawData, int _ticktime, uint8 &preEventByte)
{
    //get ticktime
    int addr_offset = 0;
    int tmp_val = 0;
    uint8 tmp_midi_data;
    
    addr_offset += GetDynamicValBytes(pRawData, tmp_val);
    
    m_TickTime = _ticktime + tmp_val;
    
    tmp_midi_data = GetNextByte(pRawData, addr_offset);
    
    if ((tmp_midi_data & 0xf0) < 0x80)
    {
        if (preEventByte == 0)
        {
            //fault. Previous message is not correct.
        }
        tmp_midi_data = preEventByte;
        addr_offset--;
    }
    else
    {
        preEventByte = tmp_midi_data;
    }
    
    switch (tmp_midi_data & 0xf0)
    {
        case MIDI_FILE_MSG_NOTE_OFF:
            //CopyData(pRawData, addr_offset, 2);
            shortData1 = *(pRawData + addr_offset); addr_offset++;
            shortData2 = *(pRawData + addr_offset); addr_offset++;
            m_MidiMessageType = MIDI_MSG_USER_NOTE_OFF;
            m_MidiTrack = tmp_midi_data&0xf;
            break;
        case MIDI_FILE_MSG_NOTE_ON:
            if (*(pRawData + addr_offset+1) == 0)
            {
                m_MidiMessageType = MIDI_MSG_USER_NOTE_OFF;
            }
            else
            {
                m_MidiMessageType = MIDI_MSG_USER_NOTE_ON;
            }
            
            shortData1 = *(pRawData + addr_offset); addr_offset++;
            shortData2 = *(pRawData + addr_offset); addr_offset++;
            m_MidiTrack = tmp_midi_data & 0xf;
            break;
        case MIDI_FILE_MSG_KEY_PRESSURE:
            shortData1 = *(pRawData + addr_offset); addr_offset++;
            shortData2 = *(pRawData + addr_offset); addr_offset++;
            m_MidiMessageType = MIDI_MSG_USER_KEY_PRESSURE;
            m_MidiTrack = tmp_midi_data & 0xf;
            break;
        case MIDI_FILE_MSG_CONTROL:
            shortData1 = *(pRawData + addr_offset); addr_offset++;
            shortData2 = *(pRawData + addr_offset); addr_offset++;
            m_MidiMessageType = MIDI_MSG_USER_CONTROL;
            m_MidiTrack = tmp_midi_data & 0xf;
            break;
        case MIDI_FILE_MSG_PROGRAM:
            shortData1 = *(pRawData + addr_offset); addr_offset++;
            m_MidiMessageType = MIDI_MSG_USER_PROGRAM;
            m_MidiTrack = tmp_midi_data & 0xf;
            break;
        case MIDI_FILE_MSG_AFTER_TOUCH:
            shortData1 = *(pRawData + addr_offset); addr_offset++;
            m_MidiMessageType = MIDI_MSG_USER_AFTER_TOUCH;
            m_MidiTrack = tmp_midi_data & 0xf;
            break;
        case MIDI_FILE_MSG_PITCH:
            shortData1 = *(pRawData + addr_offset); addr_offset++;
            shortData2 = *(pRawData + addr_offset); addr_offset++;
            m_MidiMessageType = MIDI_MSG_USER_PITCH;
            m_MidiTrack = tmp_midi_data & 0xf;
            break;
        case MIDI_FILE_MSG_GLOBAL:
            ReleaseMemory();
            switch (tmp_midi_data)
        {
            case MIDI_FILE_MSG_SYS_EX:
                int len;
                addr_offset += GetDynamicValBytes(pRawData + addr_offset, len);
                
                memcpy(pSysexData, pRawData+addr_offset, len-1);
                
                addr_offset+=len;
                m_MidiMessageType = MIDI_MSG_USER_SYSEX;
                
                m_MidiTrack = TRACK_CONTROL;
                break;
            case MIDI_FILE_MSG_MIDI_META_EVENT:
                //
                tmp_midi_data = GetNextByte(pRawData, addr_offset);
                
                addr_offset += GetDynamicValBytes(pRawData + addr_offset, len);
                
                switch (tmp_midi_data)
            {
                case MIDI_FILE_META_TRACK_END:
                    m_MidiMessageType = MIDI_MSG_USER_TRACK_END;
                    m_MidiTrack = TRACK_CONTROL;
                    break;
                case MIDI_FILE_META_TEMPO:
                    
                    //p_tempo_save = new STR_TEMPO_SAVE;
                    
                    tempoInfo.beatTime = GetNextByte(pRawData, addr_offset) << 16;
                    tempoInfo.beatTime |= GetNextByte(pRawData, addr_offset) << 8;
                    tempoInfo.beatTime |= GetNextByte(pRawData, addr_offset);
                    //float fTempo;
                    //fTempo=60000000./ tempoInfo.beatTime;
                    //tempoInfo.tempo = ceilf( fTempo);
                    
                    m_MidiMessageType = MIDI_MSG_USER_TEMPO;
                    
                    m_MidiTrack = TRACK_CONTROL;
                    break;
                case MIDI_FILE_META_BEAT_INFO:
                    //p_para_save = new STR_BEAT_TYPE_SAVE;
                    beatInfo.beatsPerPara = GetNextByte(pRawData, addr_offset);
                    beatInfo.paraType = int(pow(2.f, GetNextByte(pRawData, addr_offset)) + 0.5);
                    addr_offset += 2;
                    
                    m_MidiMessageType = MIDI_MSG_USER_BEAT_INFO;
                    m_MidiTrack = TRACK_CONTROL;
                    break;
                default:
                    addr_offset += len;
                    break;
            }
                
                break;
        }
            break;
    }
    
    return addr_offset;
}


int CMidiMessage::getSysexMsgLen()
{
    return sysexLen;
}


int CMidiMessage::GetDynamicValBytes(uint8 *data, int &retValue)
{
	uint8 tmpVal;
	int offset = 0;
	retValue = 0;

	do
	{
		tmpVal = GetNextByte(data, offset);
		retValue *= 128;
		retValue += (tmpVal & 0x7f);

	} while (tmpVal & 0x80);
	
	return offset;
}


uint8 CMidiMessage::GetNextByte(uint8 *data, int &offset)
{
	uint8 returnVal = *(data + offset);
	offset++;
	return returnVal;
}


bool CMidiMessage::isNoteOn()
{
	if (m_MidiMessageType == MIDI_MSG_USER_NOTE_ON)
	{
		return true;
	}
	return false;
}
bool CMidiMessage::isNoteOff()
{
	if (m_MidiMessageType == MIDI_MSG_USER_NOTE_OFF)
	{
		return true;
	}
	return false;
}

uint8 CMidiMessage::getNote()
{
	if ((m_MidiMessageType == MIDI_MSG_USER_NOTE_ON) || (m_MidiMessageType == MIDI_MSG_USER_NOTE_OFF))
	{
		return shortData1;
	}
	return 0xff;
}

//for message tempo and beat type
void CMidiMessage::initTempoMessage(int msgTickTime, int value)
{
    m_TickTime = msgTickTime;
    m_MidiMessageType = MIDI_MSG_USER_TEMPO;
    tempoInfo.beatTime = value;
}

void CMidiMessage::initBeatInfoMessage(int msgTickTime, int value)
{
    m_TickTime = msgTickTime;
    m_MidiMessageType = MIDI_MSG_USER_BEAT_INFO;
    beatInfo.beatInfo = value;
}

//for all messages.
void CMidiMessage::initSysexMessage(int msgTickTime, int msgLen, const unsigned char *sourceData)
{
    m_TickTime = msgTickTime;
    m_MidiMessageType = MIDI_MSG_USER_SYSEX;
    
    sysexLen = msgLen;
    pSysexData = new unsigned char[msgLen];
    memcpy(pSysexData, sourceData, msgLen);
}

void CMidiMessage::initShortMessage(MIDI_MESSAGE_USER type, int msgTickTime, int data1)
{
    m_TickTime = msgTickTime;
    m_MidiMessageType = type;
    
    shortData1 = data1;
    shortData2 = 0;
}

void CMidiMessage::initShortMessage(MIDI_MESSAGE_USER type, int msgTickTime, int data1, int data2)
{
    m_TickTime = msgTickTime;
    m_MidiMessageType = type;
    
    shortData1 = data1;
    shortData2 = data2;
}

void CMidiMessage::SetMatchMessage(CMidiMessage *matchMsg)
{
	m_pMatchMsg = matchMsg;
}

void CMidiMessage::setTrackEndMessage(int msgTickTime)
{
    clearMessage();
    m_TickTime = msgTickTime;
    m_MidiMessageType = MIDI_MSG_USER_TRACK_END;
}

CMidiMessage *CMidiMessage::GetMatchMessage()
{
	return m_pMatchMsg;
}

const CMidiMessage& CMidiMessage::operator= (const CMidiMessage&other)
{
    clearMessage();
    if(this != &other)
    {
        m_MidiMessageType = other.m_MidiMessageType;
        
        if (other.pSysexData != nullptr)
        {
            pSysexData = new uint8[other.sysexLen];
            memcpy(pSysexData, other.pSysexData, other.sysexLen);
            sysexLen = other.sysexLen;
        }
        
        m_MidiTrack = other.m_MidiTrack;
        m_TickTime = other.m_TickTime;
        m_TimeInMillisecond = other.m_TimeInMillisecond;
        
        tempoInfo = other.tempoInfo;
        beatInfo = other.beatInfo;
        
        shortData1 = other.shortData1;
        shortData2 = other.shortData2;
      
        m_pMatchMsg = nullptr; // can't copy this pointer.
    }
    return *this;
}

void CMidiMessage::WriteDynamicVal(FILE *fp, int value)
{
    unsigned char writeBuf[4];
    memset(writeBuf, 0, sizeof(writeBuf));
    
    int index = 0;
    while (value != 0)
    {
        writeBuf[index] = value&0x7f;
        value >>= 7;
        index++;
    }
    
    if (index == 0) {
        index = 1;
    }
    
    while (index)
    {
        index--;
        
        if (index != 0) {
            writeBuf[index] |= 0x80;
        }
        fwrite(writeBuf+index, 1, 1, fp);
    }
}

int CMidiMessage::saveToFile(int currentTick, FILE *fp)
{
    int writeTime = m_TickTime - currentTick;
    unsigned char writeBuf[4];
    
    WriteDynamicVal(fp, writeTime);
    
    switch (m_MidiMessageType) {
        case MIDI_MSG_USER_NOTE_OFF:
            writeBuf[0] = 0x80;
            writeBuf[0] |= m_MidiTrack;
            writeBuf[1] = shortData1;
            writeBuf[2] = shortData2;
            fwrite(writeBuf, 3, 1, fp);
            break;
        case MIDI_MSG_USER_NOTE_ON:
            writeBuf[0] = 0x90;
            writeBuf[0] |= m_MidiTrack;
            writeBuf[1] = shortData1;
            writeBuf[2] = shortData2;
            fwrite(writeBuf, 3, 1, fp);
            break;
        case MIDI_MSG_USER_KEY_PRESSURE:
            writeBuf[0] = 0xa0;
            writeBuf[0] |= m_MidiTrack;
            writeBuf[1] = shortData1;
            writeBuf[2] = shortData2;
            fwrite(writeBuf, 3, 1, fp);
            break;
        case MIDI_MSG_USER_CONTROL:
            writeBuf[0] = 0xb0;
            writeBuf[0] |= m_MidiTrack;
            writeBuf[1] = shortData1;
            writeBuf[2] = shortData2;
            fwrite(writeBuf, 3, 1, fp);
            break;
        case MIDI_MSG_USER_PITCH:
            writeBuf[0] = 0xe0;
            writeBuf[0] |= m_MidiTrack;
            writeBuf[1] = shortData1;
            writeBuf[2] = shortData2;
            fwrite(writeBuf, 3, 1, fp);
            break;
        case MIDI_MSG_USER_PROGRAM:
            writeBuf[0] = 0xc0;
            writeBuf[0] |= m_MidiTrack;
            writeBuf[1] = shortData1;
            fwrite(writeBuf, 2, 1, fp);
            break;
        case MIDI_MSG_USER_AFTER_TOUCH:
            writeBuf[0] = 0xd0;
            writeBuf[0] |= m_MidiTrack;
            writeBuf[1] = shortData1;
            fwrite(writeBuf, 2, 1, fp);
            break;
        case MIDI_MSG_USER_SYSEX:
            writeBuf[0] = MIDI_FILE_MSG_SYS_EX;
            fwrite(writeBuf, 1, 1, fp);
            
            WriteDynamicVal(fp, sysexLen+1);
            fwrite(pSysexData, sysexLen, 1, fp);
            writeBuf[0] = 0xf7;
            fwrite(writeBuf, 1, 1, fp);
            break;
        case MIDI_MSG_USER_TRACK_END:
            writeBuf[0] = MIDI_FILE_MSG_MIDI_META_EVENT;
            writeBuf[1] = MIDI_FILE_META_TRACK_END;
            writeBuf[2] = 0x00;
            fwrite(writeBuf, 3, 1, fp);
            break;
        case MIDI_MSG_USER_TEMPO:
            writeBuf[0] = MIDI_FILE_MSG_MIDI_META_EVENT;
            writeBuf[1] = MIDI_FILE_META_TEMPO;
            writeBuf[2] = 0x3;
            fwrite(writeBuf, 3, 1, fp);
            
            memcpy(writeBuf, &tempoInfo.beatTime, 4);
            fwrite(writeBuf+2, 1, 1, fp);
            fwrite(writeBuf+1, 1, 1, fp);
            fwrite(writeBuf+0, 1, 1, fp);
            break;
        case MIDI_MSG_USER_BEAT_INFO:
            writeBuf[0] = MIDI_FILE_MSG_MIDI_META_EVENT;
            writeBuf[1] = MIDI_FILE_META_BEAT_INFO;
            writeBuf[2] = 0x4;
            fwrite(writeBuf, 3, 1, fp);
            
            writeBuf[0] = beatInfo.beatsPerPara;
            if (beatInfo.paraType == 1) {
                writeBuf[1] = 0;
            }
            else if(beatInfo.paraType == 2)
            {
                writeBuf[1] = 1;
            }
            else if(beatInfo.paraType == 4)
            {
                writeBuf[1] = 2;
            }
            else if(beatInfo.paraType == 8)
            {
                writeBuf[1] = 3;
            }
            else if(beatInfo.paraType == 16)
            {
                writeBuf[1] = 4;
            }
            else if(beatInfo.paraType == 32)
            {
                writeBuf[1] = 5;
            }
            writeBuf[2] = writeBuf[3] = 0;
            fwrite(writeBuf, 4, 1, fp);
            break;
        default:
            break;

    }
    
    return m_TickTime;
}
