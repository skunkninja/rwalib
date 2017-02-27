#ifndef _DATA_BASIC_HEAD_FILE
#define _DATA_BASIC_HEAD_FILE

typedef unsigned char BYTE;
typedef unsigned int UINT;
typedef unsigned char UCHAR;
typedef unsigned long DWORD;
typedef unsigned short WORD;

typedef union _DWORD_UNION
{
	unsigned long dwordData;
	unsigned char byteData[4];
}DWORD_UNION;

typedef union _WORD_UNION
{
	unsigned short dwordData;
	unsigned char byteData[2];
}WORD_UNION;

void LSB_MSB(unsigned long &lData);
void LSB_MSB(unsigned short &sData);

#endif
