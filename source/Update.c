
#include "KernelHeader.h"

const UINT8 softver_edtion[] = "0.0.1";

_STA_ IO_ UINT8 XRAM_ s_AppRecvBuf[1500] = {0};
_STA_ IO_ UPDATE_TypeDef stUpdate;

IO_ UINT32  g_Udp_Count, g_Frame_Count, g_Send_Fail;
IO_ UINT8 g_AirLight_Flag;

IO_ UINT8 g_Test_Mode = 0xFF;




void Add_Udp_Count(void)
{
	g_Udp_Count++;
}

UINT32 Get_Udp_Count(void)
{
	return g_Udp_Count;
}

void Reset_Udp_Count(UINT32 nVal)
{
	g_Udp_Count   = nVal;
	g_Frame_Count = nVal;
	g_Send_Fail   = nVal;
}

    
/* 采集报告处理 */
void collect_return_hdl(UINT16 stat)
{
    UINT8  dat[16], n;
    UINT16 tim;
	UINT32 udp_count;

    n = 0;
    tim = IT_LIST_GetTicks();
    dat[n++] = tim >> 8;
	//the time value change to large-end, data still use little-end ,yaolan_20190418
    dat[n++] = tim;  /* 为了保持与数据包一致，采用小段模式 */
    tim = IT_ADC_GetTicks();
    dat[n++] = tim >> 8;
    dat[n++] = tim;  /* 为了保持与数据包一致，采用小段模式 */

    dat[n++] = stat >> 8;
    dat[n++] = stat;      /* 大小端模式转换，该处采用大端模式 */

	udp_count = Get_Udp_Count();
	dat[n++]  = udp_count >> 24;
	dat[n++]  = udp_count >> 16;
	dat[n++]  = udp_count >> 8;
	dat[n++]  = udp_count;
	
    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, 0x100000, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, 0x100000, dat, n);  /* 重发一次数据包 */
    while (((UINT16)(IT_ADC_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
    PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, 0x100000, dat, n);  /* 重发一次数据包 */

}

void Msg_Return_Handle_0(EN_MSG_TYPE eType, UINT32 nCmd)
{
    UINT8  dat[2] = {0}, n;
    UINT32 tim;

    n = 0;
 //   dat[n++] = nResult;      /* 大小端模式转换，该处采用大端模式 */
	
	tim = IT_SYS_GetTicks();
	if(eType == e_Msg_Status)
	{
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
	}else if(eType == e_Msg_Data){
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
	}
}

void Msg_Return_Handle_8(EN_MSG_TYPE eType, UINT32 nCmd, INT8 nResult)
{
    UINT8  dat[2] = {0}, n;
    UINT32 tim;

    n = 0;
    dat[n++] = nResult;      /* 大小端模式转换，该处采用大端模式 */
	
	tim = IT_SYS_GetTicks();
	if(eType == e_Msg_Status)
	{
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
	}else if(eType == e_Msg_Data){
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
	}
}

void Msg_Return_Handle_16(EN_MSG_TYPE eType, UINT32 nCmd, INT16 nResult)
{ 
    UINT8  dat[3], n;
    UINT32 tim;

    n = 0;
    dat[n++] = nResult >> 8;
    dat[n++] = nResult;      /* 大小端模式转换，该处采用大端模式 */
	
	tim = IT_SYS_GetTicks();
	if(eType == e_Msg_Status)
	{
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
	}else if(eType == e_Msg_Data){
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
	}
}

void Msg_Return_Handle_32(EN_MSG_TYPE eType, UINT32 nCmd, INT32 nResult)
{
    UINT8  dat[5]={0}, n;
    UINT32 tim;

    n = 0;
    dat[n++] = nResult >> 24;
    dat[n++] = nResult >> 16;      /* 大小端模式转换，该处采用大端模式 */
	dat[n++] = nResult >> 8;
	dat[n++] = nResult;
	
	tim = IT_SYS_GetTicks();
	if(eType == e_Msg_Status)
	{
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
	}else if(eType == e_Msg_Data){
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
	}
}

void Msg_Return_Handle_String(EN_MSG_TYPE eType, UINT32 nCmd, UINT8 *pRst, UINT8 nLen)
{
    UINT32 tim;
	UINT8 n = nLen, *dat = pRst;
		
	tim = IT_SYS_GetTicks();
	if(eType == e_Msg_Status)
	{
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
//		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
//		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
//		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
//		PL_NET_SendResult(PROTOCOL_HEAD_SEND_STATUS, nCmd, dat, n);  /* 重发一次数据包 */
	}else if(eType == e_Msg_Data){
		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 50); /* 延时50毫秒后再发送结果 */
		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 44 53 57 53 - 00 10 00 00 */
//		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 60); /* 延时10毫秒后再发送结果 */
//		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
//		while (((UINT16)(IT_SYS_GetTicks() - tim)) < 70); /* 延时10毫秒后再发送结果 */
//		PL_NET_SendResult(PROTOCOL_HEAD_SEND_DATA, nCmd, dat, n);  /* 重发一次数据包 */
	}
}

UINT16 Crc16_Add(UINT8 nData, UINT16 nAcc)
{
  nAcc ^= nData;
  nAcc  = (nAcc >> 8) | (nAcc << 8);
  nAcc ^= (nAcc & 0xff00) << 4;
  nAcc ^= (nAcc >> 8) >> 4;
  nAcc ^= (nAcc & 0xff00) >> 5;
  return nAcc;
}


UINT16 Crc16_Data(UINT8 *pData, UINT16 nLen, UINT16 nAcc)
{
  UINT16 i;

  for(i = 0; i < nLen; ++i) {
    nAcc = Crc16_Add(*pData, nAcc);
    ++pData;
  }
  return nAcc;

}


UINT8 Update_Start_Msg(UINT8* pMsg)
{
	stUpdate.nPacket_Total = PL_UnionTwoBytes(*(pMsg + 8), *(pMsg + 9));
	memset((void*)&stUpdate, 0, sizeof(UPDATE_TypeDef));

	stUpdate.nData_Len 	= PL_UnionFourBytes(*(pMsg + 10), *(pMsg + 11), *(pMsg + 12), *(pMsg + 13));
	stUpdate.Version[0] 	= *(pMsg + 14); 
	stUpdate.Version[1] 	= *(pMsg + 15);
	stUpdate.Version[2] 	= *(pMsg + 16); 
	stUpdate.Version[3] 	= *(pMsg + 17);
	stUpdate.pWriteAddr 	= FLASH_FIREWARE_START_ADDR;
	// erase fireware flash sector
	Erase_Specify_Sector(FLASH_FIREWARE_START_ADDR, FLASH_FIREWARE_LEN);
	printf("fireware sector erase finshed\r\n");

	// send request first packet data msg
	stUpdate.nPacket_No = 0;
	Msg_Return_Handle_16(e_Msg_Status ,CMD_UPDATE_DATA_REQUEST, stUpdate.nPacket_No);
	return 0;
}



UINT8 Update_Packet_Handler(UINT8* pMsg)
{
	UINT16 nNo = 0, nLen = 0, nCrc = 0;
	
	nNo = PL_UnionTwoBytes(*(pMsg + 8), *(pMsg + 9));
	nLen = PL_UnionTwoBytes(*(pMsg + 10), *(pMsg + 11));
	nCrc = PL_UnionTwoBytes(*(pMsg + 12), *(pMsg + 13));
	
	if(nNo != stUpdate.nPacket_No)
	{
		Msg_Return_Handle_16(e_Msg_Status ,CMD_UPDATE_DATA_REQUEST, stUpdate.nPacket_No);
		return 0;
	}

	// check and get packet data 
	if(nNo < stUpdate.nPacket_Total){
		if(UPDATE_PACKET_DATA_LEN != nLen){
			printf("packet data len error\r\n");
			Msg_Return_Handle_16(e_Msg_Status ,CMD_UPDATE_DATA_REQUEST, stUpdate.nPacket_No);
			return 0;
		}
	}else if(nNo == stUpdate.nPacket_Total){
		if(nLen > UPDATE_PACKET_DATA_LEN){
			printf("last packet data len more than 512\r\n");
			Msg_Return_Handle_16(e_Msg_Status ,CMD_UPDATE_DATA_REQUEST, stUpdate.nPacket_No);
			return 0;
		}
	}
	memcpy((void*)s_AppRecvBuf, pMsg, UPDATE_PACKET_DATA_LEN);

	// crc check
	//stUpdate.nCrc = 
	if(nCrc != stUpdate.nCrc)
	{
		printf("Crc checksum is error\r\n");
		Msg_Return_Handle_16(e_Msg_Status ,CMD_UPDATE_DATA_REQUEST, stUpdate.nPacket_No);
		return 0;
	}

	// save to flash
	stUpdate.pWriteAddr = Flash_Write_Data(stUpdate.pWriteAddr, (UINT8*)s_AppRecvBuf, nLen);

	// finished or get next packet data recv
	if(nNo == stUpdate.nPacket_Total)
	{
		printf("update finished\r\n");
		collect_return_hdl(COLLECT_RET_SUCESS);
		g_Record_Param.nUpdate_Flag = UPDATE_RESTART;
	}else if(nNo < stUpdate.nPacket_Total){
		
		stUpdate.nPacket_No++;
		Msg_Return_Handle_16(e_Msg_Status ,CMD_UPDATE_DATA_REQUEST, stUpdate.nPacket_No);
	}
	return 0;
}





UINT8 MSG_Handling(UINT8 * pchCmdBuf, UINT8 * pchFbkBuf)
{
    UINT8  chType              = 0;
    UINT32 nCommand            = 0;
    UINT16 nParaLen            = 0, nNo = 0, nLen = 0, nCrc;
	UINT8 nReturn			   = 0;
	UINT8 nStatus			   = 0;
    //
    enum eFlag  bSendBack      = e_False;
    //
//    UINT16 nShort  = 0;
//    UINT32 nWord   = 0;
//	INT16 nAdd     = 0;
//    UINT32 nWord_p = 0;
    //
    chType   = *(pchCmdBuf + 3);
    nCommand = PL_UnionFourBytes(*(pchCmdBuf + 4),
                                 *(pchCmdBuf + 5),
                                 *(pchCmdBuf + 6),
                                 *(pchCmdBuf + 7));

    //===================================================
    // action
	printf("\r\nMSG Handling:%02X%02X%02X%02X ", pchCmdBuf[0],pchCmdBuf[1],pchCmdBuf[2],pchCmdBuf[3]);
    printf("%02X%02X%02X%02X ", pchCmdBuf[4],pchCmdBuf[5],pchCmdBuf[6],pchCmdBuf[7]);
	printf("%02X%02X%02X%02X   ", pchCmdBuf[8],pchCmdBuf[9],pchCmdBuf[10],pchCmdBuf[11]);
	
	// 1.
    if (PROTOCOL_HEAD_RECV_WR == chType) // cntrol cmd
    {
        switch (nCommand)
        {
            case CMD_QUERY_EDTION:
            {
                nCommand  = CMD_STATUS_EDTION;
                bSendBack = e_True;
                //
                *(pchFbkBuf + 0) = '2';        /* FPGA版本 V1.0 ASCII格式 */
                *(pchFbkBuf + 1) = '0';        /* FPGA版本 V1.0 ASCII格式 */
                memset((pchFbkBuf + 2), 0, 32);
                memcpy((pchFbkBuf + 2), softver_edtion, (sizeof(softver_edtion)));/* MCU版本 ASCII格式 32个字节 */
                nParaLen         = (2 + 32);           
            }
			case CMD_MCU_UPDATE:
			{
				printf("%02X%02X%02X%02X ", pchCmdBuf[12],pchCmdBuf[13],pchCmdBuf[14],pchCmdBuf[15]);
				printf("%02X%02X%02X%02X\r\n", pchCmdBuf[16],pchCmdBuf[17],pchCmdBuf[18],pchCmdBuf[19]);
				
				Update_Start_Msg(&pchCmdBuf[8]);
			}
			break;
			case CMD_UPDATE_DATA_PACKET:
			{
				
				Update_Packet_Handler(&pchCmdBuf[8]);
			}
			break;
			case CMD_QUERY_UPDATE_FLAG:
			{
				printf("update flag=%d\r\n",  g_Record_Param.nUpdate_Flag);
				Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_QUARY_UPDATE_FLAG, g_Record_Param.nUpdate_Flag);
			}
			break;
			case CMD_CTRL_UPDATE_FLAG:
			{
				
				printf("set updata flag value=%d\r\n",  pchCmdBuf[8]);
				g_Record_Param.nUpdate_Flag = pchCmdBuf[8];
				nStatus = Flash_Write_Param(&g_Record_Param, RECORD_PARAM_LEN);
				if(nStatus == e_Feedback_Fail)
				{
					Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_SET_UPDATE_FLAG, e_Feedback_Fail);
					printf("set updata flag value=%d error\r\n",  pchCmdBuf[8]);
				}else{
					Msg_Return_Handle_8(e_Msg_Status, CMD_STATUS_SET_UPDATE_FLAG, e_Feedback_Success);
					printf("set updata flag value=%d, successe\r\n",  pchCmdBuf[8]);
				}
			
			}
			break;
			default:break;
		}
	
	}
	
	return nReturn;
}





UINT8  HW_LWIP_MainLine(void)
{
//    UINT8 chReturn;
    //=====================================================
    // 1. lwip handing
    // 1) period task
    // ---check if any packet received
	//Micro_Switch_Check();
    if (ETH_CheckFrameReceived())
    {
        // for debug, the "arp frame"
        // printf("-");
        // process received ethernet packet
        LwIP_Pkt_Handle();
        // udp_echoserver_senddata("abcd", 4);
        //--------------------------------------------
        // 2) message handling
        PL_NET_CheckingFrame((UINT8 *)g_NET_achCmdRvBuf);
        if (E_PL_BUF_ENABLE == PL_NET_IsRecvCommandValid())
        {
            MSG_Handling((UINT8 *)g_NET_achCmdRvBuf, (UINT8 *)g_achFbkSdBuf);
            //
            PL_NET_ResetRecvComand();
        }
    }
    // ---handle periodic timers for LwIP
    LwIP_Periodic_Handle(IT_SYS_GetTicks());

    return e_Feedback_Success;
}


//-----------------------------------------------------------------------------------------
// reset by software
UINT8 MT_RESET_Software(void)
{
    // attention: no return !
//    FPGA_ResetHardware();

    NVIC_SystemReset();

    return e_Feedback_Success;
}






