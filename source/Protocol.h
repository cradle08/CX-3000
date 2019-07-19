// Protocol.h

#ifndef  __PROTOCOL_H__
#define  __PROTOCOL_H__


#include "MyType.h"


//-----------------------------------------------------------------------------------------
// to enable the channel of the message 
#define   PL_ENABLE_UART       1 
#define   PL_ENABLE_NET        1  

//-----------------------------------------------------------------------------------------
// definition

//----------------------------------------------
// frame start:    1 byte 
// control:        1 byte
// length of data: 1 byte 
// data region:    N bytes(command, parameters) 
// check:          1 byte  
// frame end:      1 byte 
//----------------------------------------------

// communication
#define PROTOCOL_PORT              COM_MAIN  // config the port of communication    
//
#define PROTOCOL_RECV_BUF_LENGTH   256		 // length of received chars
#define PROTOCOL_SEND_BUF_LENGTH   1500		 // length of transmitted chars

//
#define PROTOCOL_TIMEOUT           100       // timeout of the same state, status-automatic-changed

//---
#define PROTOCOL_HEAD_RECV_01      0x43  // 'C'
#define PROTOCOL_HEAD_RECV_02      0x42  // 'B'
#define PROTOCOL_HEAD_RECV_03      0x57  // 'W'
#define PROTOCOL_HEAD_RECV_WR      0x57  // 'W' 
#define PROTOCOL_HEAD_RECV_RD      0x52  // 'R'
//---
#define PROTOCOL_HEAD_SEND_01      0x44  // 'D'
#define PROTOCOL_HEAD_SEND_02      0x53  // 'S'
#define PROTOCOL_HEAD_SEND_03      0x57  // 'W'
#define PROTOCOL_HEAD_SEND_STATUS  0x53  // 'S'
#define PROTOCOL_HEAD_SEND_DATA    0x44  // 'D'


//-----------------------------------------------------------------------------------------
// enumeration

// status of buffer
enum eProtocolBufAble         
{
    E_PL_BUF_DISABLE  = 0,           // disable
    E_PL_BUF_ENABLE   = 1,	         // enable
  
};


// record the status, when receiving chars
struct tProtocolBufferStatus          
{
   IO_ enum eProtocolBufAble     eAble;           // enable
};


//-----------------------------------------------------------------------------------------
// global variables declaration

// buffer
_EXT_ IO_ UINT8 XRAM_ g_NET_achCmdRvBuf[PROTOCOL_RECV_BUF_LENGTH];    //  received buffer
//
_EXT_ IO_ UINT8 XRAM_ g_achFbkSdBuf[PROTOCOL_SEND_BUF_LENGTH];    //  transmitted buffer


//-----------------------------------------------------------------------------------------
// function declaration: status
_EXT_ UINT8  PL_InitProtocol(void);
_EXT_ UINT8  PL_CommandCheck(UINT8* pStrCommand, UINT16 nLen, UINT8* pResult);
_EXT_ UINT16 PL_UnionTwoBytes(UINT8 chByteH, UINT8 chByteL);
INT16 PL_UnionTwoBytes_2(UINT8 chByteH, UINT8 chByteL);
// attention: the MPU which bus-band-width (BBW) is less than 32bits shouldn't use this function.
_EXT_ UINT32 PL_UnionFourBytes(UINT8 chByteHH, UINT8 chByteHL, UINT8 chByteLH, UINT8 chByteLL);

//-----------------------------------------------------------------------------------------
// function declaration: for the uart port 
_EXT_ UINT8  PL_COM_TriggerSend(void);
_EXT_ UINT8  PL_COM_SendChar(UINT8 ch);
_EXT_ UINT8  PL_COM_SendNChar(UINT8* pStr, UINT16 nNum);
_EXT_ UINT8  PL_COM_SendString(UINT8* pStr);
_EXT_ UINT8  PL_COM_SendValue(UINT32 nValue);
//
_EXT_ void   PL_COM_IsrHandling(void);  // interrupt of com

//-----------------------------------------------------------------------------------------
// function declaration: for the net port 
_EXT_ UINT8  PL_NET_SendChar(UINT8 ch);
_EXT_ UINT8  PL_NET_SendNChar(UINT8* pStr, UINT16 nNum);
_EXT_ UINT8  PL_NET_SendString(UINT8* pStr);
_EXT_ UINT8  PL_NET_SendResult(UINT8 chType, UINT32 nCommand, UINT8* pchData, UINT16 nDataLen);  // feedback

//
void Debug_Send_String(UINT8 *pStr, UINT16 nLen);
void Debug_Send_32(UINT32 nValue);
_EXT_ void   PL_NET_CheckingFrame(UINT8* pchBuf);
_EXT_ enum eFlag  PL_NET_IsFrameTransmitting(void);  // to avoid the nestification of the frames which are transmitted
//
_EXT_ UINT8  PL_NET_ResetRecvComand(void);                         // reset the buffer
_EXT_ enum eProtocolBufAble  PL_NET_IsRecvCommandValid(void);










#endif

