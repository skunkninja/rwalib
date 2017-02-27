#if JUCE_WINDOWS
#include "windows.h"
#endif
#include "DataBas.h"

#define SWAP(A,B,t) (t)=(A);(A)=(B);(B)=(t);

void LSB_MSB(DWORD &lData)
{
	DWORD_UNION t;
	BYTE swapByte;
	t.dwordData = lData;

	SWAP(t.byteData[0],t.byteData[3],swapByte);
	SWAP(t.byteData[1],t.byteData[2],swapByte);

	lData = t.dwordData;
}
void LSB_MSB(WORD &sData)
{
	WORD_UNION t;
	BYTE swapByte;
	t.dwordData = sData;

	SWAP(t.byteData[0],t.byteData[1],swapByte);

	sData = t.dwordData;
}
