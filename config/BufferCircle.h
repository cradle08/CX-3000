// BufferCircle.h

#ifndef  __BUFFER_CIRCLE_H__
#define  __BUFFER_CIRCLE_H__


#include "MyType.h"



//-----------------------------------------------------------------------------------------
// definition

// the length the buffer-circle
// attention: the max valid length is "BUFFER_CIRCLE_LENGTH - 1", and one data
//            will be used to make a distinction between "buffer-full" and 
//            "buffer-empty".
#define  BUFFER_CIRCLE_LENGTH_SEND         2000   // 2015_05_22-changed, 500->5000
// attention: The buffer must be set bigger, or the data will overflow when download the order!
// 2014_11_29-changed
#define  BUFFER_CIRCLE_LENGTH_RECV         500   // attention:  20(small value), for buffer-circle testing



//-----------------------------------------------------------------------------------------
// data type

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



//-----------------------------------------------------------------------------------------
// variable declaration





//-----------------------------------------------------------------------------------------
// function declaration

//------------- buffer circle for sending chars ------------------------
//
_EXT_ UINT8  BUF_Send_Init(void);

//
_EXT_ UINT8  BUF_Send_PutChar(UINT8 ch);

//
_EXT_ UINT8  BUF_Send_GetChar(UINT8* pch);

//
_EXT_ UINT8  BUF_Send_PutNChar(UINT8* pchBuf, UINT16 nLen);

//
_EXT_ UINT8  BUF_Send_GetNChar(UINT8* pchBuf, UINT16 nLen);

// the length of the buffer remained to put chars
_STA_ UINT16 BUF_Send_GetLenRemain(void);

// the length of the buffer consumed to get chars
_STA_ UINT16 BUF_Send_GetLenConsume(void);



//------------- buffer circle for receiving chars ------------------------
//
_EXT_ UINT8  BUF_Recv_Init(void);

//
_EXT_ UINT8  BUF_Recv_PutChar(UINT8 ch);

//
_EXT_ UINT8  BUF_Recv_GetChar(UINT8* pch);

//
_EXT_ UINT8  BUF_Recv_PutNChar(UINT8* pchBuf, UINT16 nLen);

//
_EXT_ UINT8  BUF_Recv_GetNChar(UINT8* pchBuf, UINT16 nLen);

// the length of the buffer remained to put chars
_STA_ UINT16 BUF_Recv_GetLenRemain(void);

// the length of the buffer consumed to get chars
_STA_ UINT16 BUF_Recv_GetLenConsume(void);













#endif





