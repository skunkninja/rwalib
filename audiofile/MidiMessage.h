#ifndef MIDI_MESSAGE_DEF
#define MIDI_MESSAGE_DEF

#define TRACK_CONTROL  -1  //sysex, tempo, beat etc.
#define TRACK_MIX      -2  //like a midi 0 file.
#define TRACK_EMPTY    -3  //a empty track will return this value.

#include "DataBas.h"
#include "stdio.h"
#include "MidiDisplayDef.h"

typedef enum
{
	MIDI_FILE_MSG_NOTE_OFF = 0x80,
	MIDI_FILE_MSG_NOTE_ON = 0x90,
	MIDI_FILE_MSG_KEY_PRESSURE = 0xa0,
	MIDI_FILE_MSG_CONTROL = 0xb0,
	MIDI_FILE_MSG_PROGRAM = 0xc0,
	MIDI_FILE_MSG_AFTER_TOUCH = 0xd0,
	MIDI_FILE_MSG_PITCH = 0xe0,
	MIDI_FILE_MSG_GLOBAL = 0xf0,
	//define for global type
	MIDI_FILE_MSG_SYS_EX = 0xf0,
	MIDI_FILE_MSG_MIDI_META_EVENT = 0xff,
}MIDI_FILE_MESSAGE_DEF;

typedef enum
{
	MIDI_FILE_META_TRACK_END = 0x2f,
	MIDI_FILE_META_TEMPO = 0x51,
	MIDI_FILE_META_BEAT_INFO = 0x58,
}MIDI_FILE_META_DATA_DEF;

typedef struct
{
	int beatTime;
}STR_TEMPO_SAVE;

typedef struct
{
    union
    {
        struct
        {
            uint16 beatsPerPara;//1,2,3,4,6,8... etc
            uint16 paraType;//1/4 = 4, 1/8 = 8, 1/2 = 2
        };
        uint32 beatInfo; //.0-.15 beats per para, .16-.31 para type
    };
}STR_BEAT_TYPE_SAVE;

extern uint8 tblMidiTypeToFileData[];
extern uint8 tblMidiTypeDataLen[];

class CMidiMessage
{
public:
	CMidiMessage();
	~CMidiMessage();
	
	MIDI_MESSAGE_USER getMessageType();
	int getMidiTrack();
	
	void setMidiTrack(int newTrack);

	void SetTimeInMs(uint32 msTime);
	//load a Message from RAW midi data.
	//input : pRawData - standard midi message data. The data is from a midi file most of the time.
	//        ticktime - ticktime of previous message.
	//output: bytes of current message in the buffer.
	int loadFromRawData(uint8 *pRawData, int ticktime, uint8 &preEventByte);
    
    int saveToFile(int currentTick, FILE *fp);
   
	uint32 GetTimeInTick();
	uint32 GetTimeInMs();
    
	int getSysexMsgLen();
    const unsigned char *getSysexBuf() {return pSysexData;}

	bool isNoteOn();
	bool isNoteOff();
	uint8 getNote();

	void SetMatchMessage(CMidiMessage *matchMsg);
	CMidiMessage *GetMatchMessage();
    const CMidiMessage& operator= (const CMidiMessage&);
    
    STR_TEMPO_SAVE* getTempoInfo() {return &tempoInfo;}
    STR_BEAT_TYPE_SAVE* getBeatInfo() {return &beatInfo;}
    
    unsigned char getShortData1() {return shortData1;}
    unsigned char getShortData2() {return shortData2;}
    
    //Init message.
    void initTempoMessage(int msgTickTime, int value);
    void initBeatInfoMessage(int msgTickTime, int value);
    void initSysexMessage(int msgTickTime, int msgLen, const unsigned char *data);
    void initShortMessage(MIDI_MESSAGE_USER type, int msgTickTime, int data1);
    void initShortMessage(MIDI_MESSAGE_USER type, int msgTickTime, int data1, int data2);
    void setTrackEndMessage(int msgTickTime);
    
    void clearMessage();
protected:
	MIDI_MESSAGE_USER m_MidiMessageType;

	int m_MidiTrack;
	unsigned int m_TickTime;
	unsigned int m_TimeInMillisecond;
    
    //if note on, this point to the match note off msg.
    //if note off, this point to the match note on msg.
    CMidiMessage *m_pMatchMsg;

	//void CopyData(uint8 *pSource, int &offset, int len);
    
    //save the special info
    STR_TEMPO_SAVE tempoInfo;
    STR_BEAT_TYPE_SAVE beatInfo;
    
    int sysexLen;
    unsigned char *pSysexData;
    //
    unsigned char shortData1, shortData2;

	//Get midi style dynamic byte value.
	//input: data - pointer to midi data.
	//       retValue - reference of the data.
	//output: how many bytes the value used.
	int GetDynamicValBytes(uint8 *data, int &retValue);
    
    void WriteDynamicVal(FILE *fp, int value);
    
	uint8 GetNextByte(uint8 *data, int &offset);
	void ReleaseMemory();
};

#endif
