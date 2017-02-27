/*
  ==============================================================================

    MidiFile.h
    Created: 8 Aug 2014 9:14:12am
    Author:  ZhouFeng

  ==============================================================================
*/

#ifndef MIDIFILE_H_INCLUDED
#define MIDIFILE_H_INCLUDED

#include "MidiTrackData.h"
#include "MidiDisplayDef.h"

typedef enum
{
	MIDI_CLASS_OK,
	MIDI_FILE_ERROR_FAIL_TO_OPEN,
	MIDI_FILE_ERROR_SIZE,
	MIDI_FILE_ERROR_HEAD,
	MIDI_FILE_ERROR_MEMORY,
	MIDI_FILE_ERROR_TRACK_HEAD,
	MIDI_FILE_ERROR_DATA
}MIDI_FILE_ERROR;

#pragma pack(push,1)//取消编译器对齐优化,否则使用sizeof和RAM拷贝可能得不到正确数据。此编译条件目前仅在VC下测试过。
typedef struct _MIDI_FILE_HEAD
{
	uint8 midiHeadID[4];//4
	uint32 midiHeadLen;//4 - MSB
	uint16 midiHeadType;//2 - MSB
	uint16 midiHeadTracks;//2 - MSB
	uint16 midiHeadDiv;//2 - MSB
}MIDI_FILE_HEAD;
#pragma pack(pop)


typedef struct _MIDI_TRACK_HEAD
{
	uint8 midiTrkID[4];
	uint32 midiTrkLen;
}MIDI_TRACK_HEAD;


class RWAMidiFile
{
public:
    RWAMidiFile();
    ~RWAMidiFile();
//	MIDI_FILE_ERROR LoadMidiFile(String filename);
	MIDI_FILE_ERROR LoadMidiFileFromBuffer(uint8 *buf, int32 length);
    void UnloadMidiFile();
	int GetTotalTrackNum();
    void AddFileTrack(CMidiFileTrack *newTrack);
    
    void SaveToMidi0(char* fileName);
    
	CMidiFileTrack* GetTrackInfo(int trackNum);
    bool isPlayFinished();
    bool isAutoFinished();
    bool isReadyForPlay();

	//
    void PreparePlay();//get all midi information
	void StartPlay();
    void StopPlay();

    //Call this funtion in a timer to play a midi file.
    //pTrackMixedTo - all message played in this function, user should process all the messages.
    //                e.g: Send to a MIDI devices.
    bool PlayPastTime(std::vector<RAW_MIDI_MSG> &allRawMsg);
    bool PlayToTickTime(int tickTime, std::vector<RAW_MIDI_MSG> &allRawMsg);
    
    //Check if loop occured in the function PlayPastTime, the flag will be retained until next time call PlayPastTime.
    bool hasLoopOccured();
    //Get current played time.
    //If time mode == TIME_MODE_MS, use the time has been rated.
    float GetCurrentTime(TIME_MODE timemode=TIME_MODE_TICK);
    int GetCurrentMsTimeNotRated();
    //bool PlayToMsTime(std::vector <CMidiMessage *> &messageNeedPlay, int playedToTimeInmS);
    void SetLoopTime(int iLoopStartTime, int iLoopEndTime, TIME_MODE timemode=TIME_MODE_TICK);
    
    //Set seek time.
    //If time mode == TIME_MODE_MS, use the time has been rated.
    void SeekToTime(int iStartTime, TIME_MODE timemode=TIME_MODE_TICK, CMidiFileTrack *pTrackMixedTo = NULL);
	int GetDiv();
    void SetDiv(int div);

    //  return first beat type info.
    //  Must call this function after call "PreparePlay"
    int GetInitTempo();//return first tempo message
    
    //  return first beat type info.
    //  Must call this function after call "PreparePlay"
    //  bit 0-15  beat type.
    //  bit 15-30 beats per para
    int GetInitBeatType();//return first beat type info.
    
    //  Get total midi file time.
    //If time mode == TIME_MODE_MS, return file time rated.
	int GetFileTime(TIME_MODE timemode=TIME_MODE_TICK);
    //prepare display information
    void PrepareNoteDisplayData(std::vector <noteDisplayInfo> &noteInfo, int track);//track note information.
    void PrepareParaDisplayData(std::vector <paraDisplayInfo> &paraINfo);//para display information.
    //Play speed rate.
    void SetPlaySpeedRate(uint32 newRate);
    
    //Convert ms time to tick time.
    //useConvertedTime - use the time converted or not.
    //                   false: use standard time, same as m_speedRate = SPEED_NORMAL
    //                   true: use time converted = timeInmS * m_speedRate/SPEED_NORMAL;
    float converToTickTime(int timeInmS, bool useConvertedTime = true);
    
    //Convert tick time to ms time.
    int converToMSTime(int timeInTick, bool useConvertedTime = true);
    
    //get current play measure and the ticks in meausre.
    void getCurrentMeasureTicks(int *pMeasure, int *pTicks, int *pTicksPerMeasure, int *pTicksPerBeat);
    
    int getMeasureStartTime(int measure, TIME_MODE timemode = TIME_MODE_TICK);
protected:
	MIDI_FILE_HEAD midiHead;
	uint8 *midiFileBuffer;
    CMidiFileTrack *controlTrack;
	std::vector <CMidiFileTrack *> allTracks;
    
    //when stop/seek ..., use this to add some additional message.
    //m_additionalMsgBefore will played before current period of time, such as "seek"
    //m_additionalMsgAfter will played after current period of time, such as "stop"
    CMidiFileTrack *m_pAdditionalMsgBefore,*m_pAdditionalMsgAfter;
    void PerpareAdditionalMsg(CMidiFileTrack *pTrack, uint8 *buf, int bufLen);
    bool PlayPastTime(CMidiFileTrack *pTrackMixedTo);
    bool PlayToTickTime(int tickTime, CMidiFileTrack *allRawMsg);
    
    void ConvertAllPlayedMessages(std::vector<RAW_MIDI_MSG> &allRawMsg, CMidiFileTrack &tmpTrackToPlay);

    int m_loopStartTick, m_loopEndTick;
    int m_initTempo;
    int m_initBeatType; // 0xaabbbb - aa -> beat per para, bb-> beat type.
                      // 0x030004 -> 3/4
	//------------------------------------------
	//------------play information--------------
	//time in ms has played
	uint64 m_previousTime;
    uint32 m_playedTime;
    
    //play speed rate control
    uint32 m_speedRate; // 0x10000 means 100% speed. 0x8000 = 50% speed etc.
    uint32 m_rateModifyRemainder;
    //
	//midi file length in ms, note changed with speed rate.
	int m_FileTimeInmS;
	//midi file length in tick.
	int m_FileTimeInTick;
    //current file time, changed with speed rate!!!
    int m_iCurrentTimeInMs;
    
    //current tick time.
    float m_fCurrentTimeInTick;
	//save all tick time change info, used for ms <-> tick convert.
	std::vector <TickTimeInfo> allTickTimeInfo;
	void DeleteAllTracks();
    bool bPlayFinished,bReadyForPlay,bStopFlag;
    void PrepareParaDisplayArea(std::vector <paraDisplayInfo> *pTmpParaDisplayInfo,
                                             std::vector <TickTimeInfo>::iterator currentParaInfo,
                                             int endTickTime, int &lastSavedTick, int &beatNum, int &paraNum);
    //After call PlayPastTime to play a period of time, this flag will be set if a loop occured.
    bool hasLooped;
};



#endif  // MIDIFILE_H_INCLUDED
