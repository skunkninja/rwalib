#include "DataBas.h"

#define SWAP(A,B,t) (t)=(A);(A)=(B);(B)=(t);

void LSB_MSB(uint32 &lData)
{
	DWORD_UNION t;
	uint8 swapByte;
	t.dwordData = lData;

	SWAP(t.byteData[0],t.byteData[3],swapByte);
	SWAP(t.byteData[1],t.byteData[2],swapByte);

	lData = t.dwordData;
}
void LSB_MSB(uint16 &sData)
{
	WORD_UNION t;
	uint8 swapByte;
	t.dwordData = sData;

	SWAP(t.byteData[0],t.byteData[1],swapByte);

	sData = t.dwordData;
}