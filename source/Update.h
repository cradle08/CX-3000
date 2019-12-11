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


//UINT32 PL_UnionFourBytes(UINT8 chByteHH, UINT8 chByteHL, UINT8 chByteLH, UINT8 chByteLL);
_EXT_ IO_ UINT32 g_Udp_Count, g_Frame_Count, g_Send_Fail;
_EXT_ IO_ UINT8 g_AirLight_Flag;
_EXT_ const UINT8 softver_edtion[];

UINT32 Get_Udp_Count(void);
void Reset_Udp_Count(UINT32 nVal);
void Add_Udp_Count(void);


typedef enum{
	e_Msg_Ctrol  = 0,
	e_Msg_Status = 1,
	e_Msg_Query  = 2,
	e_Msg_Data   = 3,
	e_Msg_End    = 4
}EN_MSG_TYPE;

typedef struct{
	UINT16 nPacket_Total;
	UINT16 nPacket_No;
	UINT16 nPacket_Len;
	UINT16 nCrc;
	UINT32 nData_Len;
	UINT32 pWriteAddr;
	UINT8  Version[4];
	  

}UPDATE_TypeDef;


void collect_return_hdl(UINT16 stat);
void Msg_Return_Handle_0(EN_MSG_TYPE eType, UINT32 nCmd);
void Msg_Return_Handle_8(EN_MSG_TYPE eType, UINT32 nCmd, INT8 nResult);
void Msg_Return_Handle_16(EN_MSG_TYPE eType, UINT32 nCmd, INT16 nResult);
void Msg_Return_Handle_32(EN_MSG_TYPE eType, UINT32 nCmd, INT32 nResult);
void Msg_Return_Handle_String(EN_MSG_TYPE eType, UINT32 nCmd, UINT8 *pRst, UINT8 nLen);

UINT8 MSG_Handling(UINT8 * pchCmdBuf, UINT8 * pchFbkBuf);

UINT16 Crc16_Add(UINT8 nData, UINT16 nAcc);
UINT16 Crc16_Data(UINT8 *pData, UINT16 nLen, UINT16 nAcc);
UINT8 MT_RESET_Software(void);
UINT8  HW_LWIP_MainLine(void);

















#endif





