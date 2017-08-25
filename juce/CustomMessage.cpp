/*
  ==============================================================================

    CustomMessage.cpp
    Created: 17 Jun 2016 5:12:03pm
    Author:  ZhouFeng

  ==============================================================================
*/

#include "CustomMessage.h"


CCustomMessage::CCustomMessage(int type, int arg1, int arg2, int arg3, void *pointerArg)
{
    m_iType = type;
    
    m_iArg1 = arg1;
    m_iArg2 = arg2;
    m_iArg3 = arg3;
	m_pArg = pointerArg;
}
CCustomMessage::~CCustomMessage()
{
}