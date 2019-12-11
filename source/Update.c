
#include "KernelHeader.h"



_STA_ IO_ UINT8 XRAM_ s_AppRecvBuf[1500] = {0};
_STA_ IO_ UPDATE_TypeDef stUpdate;



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








