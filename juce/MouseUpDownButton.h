/*
  ==============================================================================

  This is an automatically generated file created by the Jucer!

  Creation date:  12 Dec 2011 11:15:27pm

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Jucer version: 1.12

  ------------------------------------------------------------------------------

  The Jucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-6 by Raw Material Software ltd.

  ==============================================================================
*/

#ifndef __JUCER_HEADER_CMOUSEUPDOWNBUTTON_MOUSEUPDOWNBUTTON_D90E3098__
#define __JUCER_HEADER_CMOUSEUPDOWNBUTTON_MOUSEUPDOWNBUTTON_D90E3098__

//[Headers]     -- You can add your own extra header files here --
#define MOUSE_STATUS_IN		1
#define MOUSE_STATUS_DOWN	2
#define MOUSE_STATUS_LOCK	4

#include "JuceHeader.h"
//[/Headers]



//==============================================================================
/**
                                                                    //[Comments]
    An auto-generated component, created by the Jucer.

    Describe your class and how it works here!
                                                                    //[/Comments]
*/
class CMouseUpDownButton  : public Component
{
public:
    //==============================================================================
    CMouseUpDownButton ();
    ~CMouseUpDownButton();

    //==============================================================================
    //[UserMethods]     -- You can add your own custom methods in this section.
    //[/UserMethods]

    void paint (Graphics& g);
    void resized();
    void mouseEnter (const MouseEvent& e);
    void mouseExit (const MouseEvent& e);
    void mouseDown (const MouseEvent& e);
    void mouseUp (const MouseEvent& e);


    //==============================================================================
    juce_UseDebuggingNewOperator

private:
    //[UserVariables]   -- You can add your own custom variables in this section.
    //[/UserVariables]

    //==============================================================================


    //==============================================================================
    // (prevent copy constructor and operator= being generated..)
    CMouseUpDownButton (const CMouseUpDownButton&);
    const CMouseUpDownButton& operator= (const CMouseUpDownButton&);
    //[UserVariables]   -- You can add your own custom variables in this section.
protected:
	unsigned int iMouseStatus;
	virtual void buttonDownProc(void) = 0;
	virtual void buttonUpProc(void) = 0;
    //[/UserVariables]
};


#endif   // __JUCER_HEADER_CMOUSEUPDOWNBUTTON_MOUSEUPDOWNBUTTON_D90E3098__
