// SamplingPort.h

#ifndef  __SAMPLING_PORT_H__
#define  __SAMPLING_PORT_H__


#include "MyType.h"


//-----------------------------------------------------------------------------------------
// definition
#define SAMPLE_PORT_OUTPUT	    0
//----------------------------------------------
// frame start:    1 byte 
// control:        1 byte
// length of data: 1 byte 
// data region:    N bytes(command, parameters) 
// check:          1 byte  
// frame end:      1 byte 
//----------------------------------------------

// communication
#define SAMPLE_PORT              COM_SAMPLING // config the port of communication    
//
#define SAMPLE_RECV_BUF_LENGTH   80		 // length of received chars
#define SAMPLE_SEND_BUF_LENGTH   80		 // length of transmitted chars
//
//
#define SAMPLE_HEAD_CHAR         0x7E        // header
#define SAMPLE_TAIL_CHAR         0xE7        // tailer
//
#define SAMPLE_TIMEOUT           200       // timeout of the same state, status-automatic-changed



//-----------------------------------------------------------------------------------------

//
enum eSampleRecvState                   
{
    E_SP_RECV_STATE_IDLE      = 0,   // idle                          
    E_SP_RECV_STATE_HEAD      = 1,   // head 
    E_SP_RECV_STATE_DATA1     = 2,   // data1
    E_SP_RECV_STATE_DATA2     = 3,   // data2
    E_SP_RECV_STATE_TAIL      = 4,   // tail
};

struct tSampleBufferStatus          
{
   // attention: alignment of the struct is two bytes,not one char, to save memory, 
   //            so put the buffer of receiving chars out of the struct.
  
   IO_ enum eProtocolBufAble     eAble;           // enable
   IO_ enum eSampleRecvState     eRecvState;      // state of receiving
   IO_ UINT16                    nDataLength;     // length of the data

};

//-----------------------------------------------------------------------------------------
// global variables declaration

// buffer
_EXT_ IO_ UINT8 XRAM_ g_SPG_achCmdRvBuf[SAMPLE_RECV_BUF_LENGTH];    //  received buffer
// _EXT_ IO_ UINT8 XRAM_ g_SPG_achFbkSdBuf[SAMPLE_SEND_BUF_LENGTH];    //  transmitted buffer


//-----------------------------------------------------------------------------------------
// function declaration: status

_EXT_ UINT8  SPG_InitProtocol(void);

_EXT_ UINT8  SPG_ResetRecvComand(void);                         // reset the buffer

_EXT_ UINT8  SPG_GetDataLength(UINT16* nLen);

_EXT_ enum eProtocolBufAble  SPG_IsRecvCommandValid(void);

_EXT_ UINT8  SPG_CommandCheck(UINT8* pStrCommand, UINT16 nLen, UINT8* pResult);

_EXT_ UINT16 SPG_UnionTwoBytes(UINT8 chByteH, UINT8 chByteL);

// attention: the MPU which bus-band-width (BBW) is less than 32bits shouldn't use this function.
_EXT_ UINT32 SPG_UnionFourBytes(UINT8 chByteHH, UINT8 chByteHL, UINT8 chByteLH, UINT8 chByteLL);

//-----------------------------------------------------------------------------------------
// function declaration: transmit-receive
_EXT_ UINT8  SPG_TriggerSend(void);

_EXT_ UINT8  SPG_SendChar(UINT8 ch);

_EXT_ UINT8  SPG_SendNChar(UINT8* pStr, UINT16 nNum);

_EXT_ UINT8  SPG_SendString(UINT8* pStr);

//-----------------------------------------------------------------------------------------
// function declaration: system
_EXT_ void   SPG_WaitingFrame(void);  // waiting a frame

_EXT_ void   SPG_IsrHandling(void);  // interrupt of com

_EXT_ void   SPG_IsrTime(void);      // called by the sysclk, 1 ms one time 

_EXT_ UINT8  SPG_Communicate(UINT8* pchBufIn, UINT16 nLenIn, UINT8* pchBufOut, UINT16* pnLenOut);










#endif




