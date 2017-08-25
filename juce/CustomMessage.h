/*
  ==============================================================================

    CustomMessage.h
    Created: 17 Jun 2016 5:12:03pm
    Author:  ZhouFeng

  ==============================================================================
*/

#ifndef CUSTOMMESSAGE_H_INCLUDED
#define CUSTOMMESSAGE_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class CCustomMessage: public Message
{
public:
    //simple message with type, three int arguments and one pointer argument.
    //
    CCustomMessage(int type, int arg1, int arg2, int arg3, void *pointerArg = nullptr);
    ~CCustomMessage();

	int getType() { return m_iType; }
	int getArg1() { return m_iArg1; }
	int getArg2() { return m_iArg2; }
	int getArg3() { return m_iArg3; }
	void *getPointerArg() { return m_pArg; }
private:
    int m_iType;
    int m_iArg1, m_iArg2, m_iArg3;
    void *m_pArg;

};


#endif  // CUSTOMMESSAGE_H_INCLUDED
