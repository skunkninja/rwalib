//
//  MidiDisplayDef.h
//  shingledrum
//
//  Created by 周峰 on 14/11/26.
//  Copyright (c) 2014年 huhu. All rights reserved.
//

#ifndef shingledrum_MidiDisplayDef_h
#define shingledrum_MidiDisplayDef_h

#define SPEED_NORMAL 0x10000

//information for note display
typedef struct
{
    int startTimeInmS;
    int endTimeInmS;
    //
    int startTimeInTick;
    int endTimeInTick;
    //
    int keyVal;
}noteDisplayInfo;

typedef enum
{
    DISPLAY_PARA,
    DISPLAY_BEAT,
}PARA_DISPLAY_TYPE;

typedef struct
{
    int timeInmS;
    int timeInTick;
    PARA_DISPLAY_TYPE type;
    int para;
}paraDisplayInfo;

typedef enum
{
    MIDI_MSG_USER_NOTE_OFF,
    MIDI_MSG_USER_NOTE_ON,
    MIDI_MSG_USER_KEY_PRESSURE,
    MIDI_MSG_USER_CONTROL,
    MIDI_MSG_USER_PROGRAM,
    MIDI_MSG_USER_AFTER_TOUCH,
    MIDI_MSG_USER_PITCH,
    MIDI_MSG_USER_SYSEX,
    MIDI_MSG_USER_TRACK_END,
    MIDI_MSG_USER_TEMPO,
    MIDI_MSG_USER_BEAT_INFO,
    MIDI_MSG_USER_NO_DEFINE,
}MIDI_MESSAGE_USER;

typedef struct
{
    MIDI_MESSAGE_USER userMsgType;
    int timeInTick;
    union
    {
        int msgLen;//length of "msgBuf"
        int beatTime; //if msg type is tempo, save the beat time here.
        int beatType;//if msg type is beat type, save the beat type here.
    };
    unsigned char *msgBuf;//the raw message buffer.(except "tempo" and "beat type")
}RAW_MIDI_MSG;



typedef enum
{
    TIME_MODE_TICK,
    TIME_MODE_MS,
}TIME_MODE;

#endif
