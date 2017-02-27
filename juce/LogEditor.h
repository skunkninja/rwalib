#ifndef LOG_EDITOR_H
#define LOG_EDITOR_H

#include "JuceHeader.h"
//Juce下的Log编辑器。只需要一个public函数:AddLog
//clear函数juce已自带。

#define MSG_ADD_TEXT 0x4000
#define MSG_SET_BOLD 0x4001

class CLogEditor :
	public TextEditor,
	public MessageListener
{
public:
	CLogEditor(const String& componentName = String::empty,
                         juce_wchar passwordCharacter = 0);
	~CLogEditor(void);
	//增加一个字串到编辑器。
	void setBold();
	int AddLog(wchar_t *str);
protected:
	void handleMessage(const Message& message);
};
#endif