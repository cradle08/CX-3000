// ProcessScope.h

#ifndef    __UPDATE_H__
#define    __UPDATE_H__

#include  "MyType.h"
#include "KernelHeader.h" 
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


// cmd exec
#define    CMD_QUERY_EDTION       		0x00000003
#define    CMD_MCU_UPDATE				0x00000A01
// cmd status
#define    CMD_STATUS_EDTION      		0x00000003
#define	   CMD_UPDATE_DATA_REQUEST		0x01000801
// cmd data
#define	  CMD_UPDATE_DATA_PACKET		0x30000601
#define  COLLECT_RET_SUCESS            	0x0000 

// query data flag
#define    CMD_CTRL_UPDATE_FLAG 			0x00000900 	//define at update.h
#define    CMD_STATUS_SET_UPDATE_FLAG 		0x01000900 //define at update.h
#define    CMD_QUERY_UPDATE_FLAG			0x20000200	//define at update.h
#define    CMD_STATUS_QUARY_UPDATE_FLAG		0x01000200	//define at update.h

#define UPDATE_PACKET_DATA_LEN			512




typedef struct{
	UINT16 nPacket_Total;
	UINT16 nPacket_No;
	UINT16 nPacket_Len;
	UINT16 nCrc;
	UINT32 nData_Len;
	UINT32 pWriteAddr;
	UINT8  Version[4];
	  

}UPDATE_TypeDef;




UINT16 Crc16_Add(UINT8 nData, UINT16 nAcc);
UINT16 Crc16_Data(UINT8 *pData, UINT16 nLen, UINT16 nAcc);
UINT8 Update_Start_Msg(UINT8* pMsg);
UINT8 Update_Packet_Handler(UINT8* pMsg);

















#endif





