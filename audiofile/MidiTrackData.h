#ifndef MIDI_TRACK_DATA_DEF
#define MIDI_TRACK_DATA_DEF

#include "MidiMessage.h"
#include <vector>
#include <list>

typedef struct _TICK_TIME_INFO
{
	int timeInTick;
	int timeInMS;

	int beatPerPara;
	int beatType;
	int tickTimsInUs;
	int tempo;
    
    int measureNumber;//the measure number of this message
    int tickLeftInMeasure;
}TickTimeInfo;

class CMidiFileTrack
{
public:
	CMidiFileTrack();
	~CMidiFileTrack();

	void SetMidiTrackNum(int newtracknum);
    
	int GetMidiTrackNum();
	void ClearMessage();
	bool AddMidiMessage(CMidiMessage *pMsg);
    //Add midi message in standard midi file format to the track.
    //Not clear old message. Now the message will add the the end of old messages.
	bool AddFromTrackBuf(uint8 *buf, int bufLen);
    
    //get match note on/note off message.
    //all the message must sort by time before call this function.
    void ScanMatchNoteMessage();
    
    void PrepareTickTimeArray(std::vector <TickTimeInfo> &allTickTimeInfo, int div, int &initTempo, int &initBeatType);
	//convert all messages' tick time to mS time.
	//return time of last event.
	void ConvertToTimeInMs(std::vector <TickTimeInfo> allTickTimeInfo, int &maxTimeInmS, int &maxTimeInTick);
	void StartPlay();
	bool PlayToTickTime(uint32 tickTime, CMidiFileTrack *pTrackMixedTo);

	std::vector <CMidiMessage *> *GetAllMessages();
    void sortMidiMessage();
    
    //all the message must sort by time before call this function.
    void saveToFile(FILE *fp);
protected:
	std::vector <CMidiMessage *> m_allMessages;
	std::vector <CMidiMessage *>::iterator playedMessage;
	int m_midiTrack;
    bool playedMessageSet;
};

#endif
