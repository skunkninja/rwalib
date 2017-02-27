#include "LogEditor.h"
#include "CustomMessage.h"
CLogEditor::CLogEditor(const String& componentName,
                         juce_wchar passwordCharacter)
						 :TextEditor(componentName,passwordCharacter)
{
}

CLogEditor::~CLogEditor(void)
{
}

//将str增加到事件消息区
int CLogEditor::AddLog(wchar_t *str)
{
	CCustomMessage *pMsg;
	pMsg = new CCustomMessage(MSG_ADD_TEXT, 0, 0, 0, str);
	postMessage(pMsg);
	return 0;
}

void CLogEditor::setBold()
{
	CCustomMessage *pMsg;
	pMsg = new CCustomMessage(MSG_SET_BOLD, 0, 0, 0);
	postMessage(pMsg);
}

//处理消息
void CLogEditor::handleMessage(const Message& message)
{
	wchar_t *str;
	CCustomMessage *tmpMessage = (CCustomMessage *)&message;
	switch (tmpMessage->getType())
	{
	case MSG_ADD_TEXT:
		TextEditor::setCaretPosition(getTotalNumChars());
		str = (wchar_t *)tmpMessage->getPointerArg();
		insertTextAtCaret(str);
		delete [] str;
		break;
	case MSG_SET_BOLD:
		setFont(Font(14, Font::bold));
		break;
	}
}