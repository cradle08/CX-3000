// InterruptBufferCircle.h

#ifndef  __INTERRUPT_BUFFER_CIRCLE_H__
#define  __INTERRUPT_BUFFER_CIRCLE_H__


#include "Mytype.h"



//-----------------------------------------------------------------------------------------
// definition

// the length the buffer-circle
// attention: the max valid length is "BUFFER_CIRCLE_LENGTH - 1", and one data
//            will be used to make a distinction between "buffer-full" and 
//            "buffer-empty".
#define  INTERRUPT_BUFFER_CIRCLE_LENGTH      200 


//-----------------------------------------------------------------------------------------
// data type

#if 0 
// status of the buffer-circle
struct tBufferCircle				  
{
    UINT16 	              nGet;       // counter, the offset address to get a char
    UINT16                nPut;       // counter, the offset address to put a char
	// enum eFlag           eBusy;       // whether the buffer-circle is busy

    // attention: 1. the buffer-circle is showing following.
    //               1). empty:  nPut = nGet + 1,
    //               2). full:   nPut = nGet,
    //               3). part:   nPut > nGet + 1  or  nPut < nGet.
    //            2. the order to handle ".nGet" and ".nPut".
    //               1). first to put a char in the buffer, then ".nPut" increase 1,
    //               2). first ".nGet" increase 1, then to get a char from the buffer.
};
#endif


//-----------------------------------------------------------------------------------------
// variable declaration





//-----------------------------------------------------------------------------------------
// function declaration


// store the commands which are fetched from the flash

_EXT_ UINT8  INT_BUF_Init(void);

//
_EXT_ UINT8  INT_BUF_PutChar(UINT8 ch);

//
_EXT_ UINT8  INT_BUF_GetChar(UINT8* pch);

//
_EXT_ UINT8  INT_BUF_PutNChar(UINT8* pchBuf, UINT16 nLen);

//
_EXT_ UINT8  INT_BUF_GetNChar(UINT8* pchBuf, UINT16 nLen);

// the length of the buffer remained to put chars
_EXT_ UINT16 INT_BUF_GetLenRemain(void);             // extern

// the length of the buffer consumed to get chars
_EXT_ UINT16 INT_BUF_GetLenConsume(void);            // extern







#endif







