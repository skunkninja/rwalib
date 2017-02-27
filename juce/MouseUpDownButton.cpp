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

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "MouseUpDownButton.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
CMouseUpDownButton::CMouseUpDownButton ()
    : Component ("MouseUpAndDownButton")
{

    //[UserPreSize]
    //[/UserPreSize]

    setSize (100, 100);

    //[Constructor] You can add your own custom stuff here..
	iMouseStatus = 0;
    //[/Constructor]
}

CMouseUpDownButton::~CMouseUpDownButton()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]



    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void CMouseUpDownButton::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void CMouseUpDownButton::resized()
{
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}

void CMouseUpDownButton::mouseEnter (const MouseEvent& e)
{
    //[UserCode_mouseEnter] -- Add your code here...
	iMouseStatus |= MOUSE_STATUS_IN;
	repaint();
    //[/UserCode_mouseEnter]
}

void CMouseUpDownButton::mouseExit (const MouseEvent& e)
{
    //[UserCode_mouseExit] -- Add your code here...
	if(iMouseStatus & MOUSE_STATUS_LOCK)
	{
		iMouseStatus &= (~MOUSE_STATUS_IN);
	}
	else
	{
		iMouseStatus &= (~MOUSE_STATUS_IN);
	}
	repaint();
    //[/UserCode_mouseExit]
}

void CMouseUpDownButton::mouseDown (const MouseEvent& e)
{
    //[UserCode_mouseDown] -- Add your code here...
	if(!(iMouseStatus & MOUSE_STATUS_DOWN))
	{
		iMouseStatus |= MOUSE_STATUS_DOWN;
		repaint();
		//--down
		buttonDownProc();
	}
    //[/UserCode_mouseDown]
}

void CMouseUpDownButton::mouseUp (const MouseEvent& e)
{
    //[UserCode_mouseUp] -- Add your code here...
	if(e.mods.isCtrlDown())
	{
		iMouseStatus |= MOUSE_STATUS_LOCK;
	}
	else
	{
		iMouseStatus &= (~MOUSE_STATUS_DOWN);
		repaint();
		//--up
		buttonUpProc();
	}
    //[/UserCode_mouseUp]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Jucer information section --

    This is where the Jucer puts all of its metadata, so don't change anything in here!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="CMouseUpDownButton" componentName="MouseUpAndDownButton"
                 parentClasses="public Component" constructorParams="" variableInitialisers=""
                 snapPixels="8" snapActive="1" snapShown="1" overlayOpacity="0.330000013"
                 fixedSize="1" initialWidth="100" initialHeight="100">
  <METHODS>
    <METHOD name="mouseEnter (const MouseEvent&amp; e)"/>
    <METHOD name="mouseDown (const MouseEvent&amp; e)"/>
    <METHOD name="mouseUp (const MouseEvent&amp; e)"/>
    <METHOD name="mouseExit (const MouseEvent&amp; e)"/>
  </METHODS>
  <BACKGROUND backgroundColour="0"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif
