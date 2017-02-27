#ifndef _DATA_BASIC_HEAD_FILE
#define _DATA_BASIC_HEAD_FILE

/** A platform-independent 8-bit signed integer type. */
typedef signed char                 int8;
/** A platform-independent 8-bit unsigned integer type. */
typedef unsigned char               uint8;
/** A platform-independent 16-bit signed integer type. */
typedef signed short                int16;
/** A platform-independent 16-bit unsigned integer type. */
typedef unsigned short              uint16;
/** A platform-independent 32-bit signed integer type. */
typedef signed int                  int32;
/** A platform-independent 32-bit unsigned integer type. */
typedef unsigned int                uint32;
/** A platform-independent 64-bit integer type. */
typedef long long                 int64;
/** A platform-independent 64-bit unsigned integer type. */
typedef unsigned long long        uint64;

//MSB <-> LSB
typedef union _DWORD_UNION
{
	uint32 dwordData;
	uint8 byteData[4];
}DWORD_UNION;

typedef union _WORD_UNION
{
	uint16 dwordData;
	uint8 byteData[2];
}WORD_UNION;

void LSB_MSB(uint32 &lData);
void LSB_MSB(uint16 &sData);

#endif