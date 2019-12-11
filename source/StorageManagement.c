// StorageManagement.c

#ifndef  __STORAGE_MANAGEMENT_C__
#define  __STORAGE_MANAGEMENT_C__


#include "KernelHeader.h"

RECORD_PARAM g_Record_Param;






//UINT8 XRAM_ g_const_ach[4] = 
//{
//    0x12,
//	0x34,
//	0x56,
//	0x78
//};

UINT16 STMFLASH_GetFlashSector(UINT32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_Sector_10; 
	return FLASH_Sector_11;	
}

void Set_Default_Param(RECORD_PARAM *pParam)
{
	pParam->nFlag    		   = FLASH_INIT_FLAG;
	pParam->nRegister_WBC      = 128; //wbc garter value
	pParam->nRegister_RBC      = 128; //rbc garter value
	pParam->nRegister_PLT      = 128; //plt garter value
	pParam->nRegister_RBC_PLT  = 128; //rbc plt garter value
	pParam->nRegister_HGB      = 128; //hgb garter value
	pParam->nRegister_CRP      = 128; //crp garter value
	pParam->nXAddStep 		   = 0;  // add step for moto x go home
	pParam->nAddPress 		   = 0;     
	// CRP
	pParam->nTime 	           = 120;
	pParam->nHZ   	           = 10;
	pParam->nTotal_Num         = 1200;
	// CRC
	pParam->nCrc               = 1;  // this value need to compute
}

//void Iint_SysParam(void)
//{
//	memset((void*)&g_CRP_Data, 0, sizeof(struct CRP_DataType));
//	g_CRP_Data.eEnable = e_False;

//}

UINT8 Flash_Read_Param(RECORD_PARAM *pParam, UINT32 nLen)
{
	UINT32 i;
	UINT8 *pAddr = (UINT8 *)FLASH_PARAM_SAVE_ADDR;
	UINT8 *pData = (UINT8*)pParam;
	
	for(i = 0; i < nLen; i++)
	{
		*(pData) =  *(pAddr);
		pAddr++;
		pData++;
	}
	return e_Feedback_Success;
}

void Set_Default_Param_Machine(RECORD_PARAM *pParam)
{
	pParam->nFlag    		   = FLASH_INIT_FLAG;
	pParam->nRegister_WBC      = 128; //wbc garter value
	pParam->nRegister_RBC      = 128; //rbc garter value
	pParam->nRegister_PLT      = 128; //plt garter value
	pParam->nRegister_RBC_PLT  = 128; //rbc plt garter value
	pParam->nRegister_HGB      = 128; //hgb garter value
	pParam->nRegister_CRP      = 128; //crp garter value
	pParam->nXAddStep 		   = 0;   // add step for moto x go home
	pParam->nAddPress 		   = 0;      
	// update flag
	pParam->nUpdate_Flag	   = UPDATE_WAITING;
	// CRC
	pParam->nCrc      = 1;  // this value need to compute
}


UINT8 Flash_Write_Param(RECORD_PARAM *pParam, UINT32 nLen)
{
	UINT8 *pAddr = (UINT8*)FLASH_PARAM_SAVE_ADDR;
	UINT8 *pData = (UINT8*)pParam;
	UINT16 index;
	
	FLASH_Status status = FLASH_COMPLETE;
	FLASH_Unlock();
	FLASH_DataCacheCmd(DISABLE);
	
	status = FLASH_EraseSector(STMFLASH_GetFlashSector(FLASH_PARAM_SAVE_ADDR), VoltageRange_3);
	if(status != FLASH_COMPLETE) return e_Feedback_Fail;
	if(status==FLASH_COMPLETE)
	{
		for(index = 0; index < nLen; index++)
		{
			if(FLASH_ProgramByte((uint32_t)pAddr,*pData)!=FLASH_COMPLETE)
			{
				break;
			}
			pData++;
		    pAddr++;
		}			
	}
	FLASH_DataCacheCmd(ENABLE);
	FLASH_Lock();
	return e_Feedback_Success;
}

UINT32 Get_Next_Sector_Space(UINT32 addr)
{
	if(addr<ADDR_FLASH_SECTOR_1)return FLASH_SECTOR_16K_LEN;//FLASH_Sector_0;
	else if(addr<ADDR_FLASH_SECTOR_2)return FLASH_SECTOR_16K_LEN;//FLASH_Sector_1;
	else if(addr<ADDR_FLASH_SECTOR_3)return FLASH_SECTOR_16K_LEN;//FLASH_Sector_2;
	else if(addr<ADDR_FLASH_SECTOR_4)return FLASH_SECTOR_64K_LEN;//FLASH_Sector_3;
	else if(addr<ADDR_FLASH_SECTOR_5)return FLASH_SECTOR_128K_LEN;//FLASH_Sector_4;
	else if(addr<ADDR_FLASH_SECTOR_6)return FLASH_SECTOR_128K_LEN;//FLASH_Sector_5;
	else if(addr<ADDR_FLASH_SECTOR_7)return FLASH_SECTOR_128K_LEN;//FLASH_Sector_6;
	else if(addr<ADDR_FLASH_SECTOR_8)return FLASH_SECTOR_128K_LEN;//FLASH_Sector_7;
	else if(addr<ADDR_FLASH_SECTOR_9)return FLASH_SECTOR_128K_LEN;//FLASH_Sector_8;
	else if(addr<ADDR_FLASH_SECTOR_10)return FLASH_SECTOR_128K_LEN;//FLASH_Sector_9;
	else if(addr<ADDR_FLASH_SECTOR_11)return FLASH_SECTOR_128K_LEN;//FLASH_Sector_10; 
	return FLASH_SECTOR_128K_LEN;//FLASH_Sector_11;	
}


UINT8 Erase_Specify_Sector(UINT32 nStartAddr, UINT32 nLen)
{
	UINT8 status;
	UINT16 nSector = 0;
	UINT32 nAddr = 0;
	UINT32 nSpace = 0;
	
	for(nAddr = nStartAddr; nAddr < (nStartAddr + nLen); nAddr += nSpace)
	{
		nSector = STMFLASH_GetFlashSector(nAddr);
		status = FLASH_EraseSector(nSector, VoltageRange_3);
		if(status != FLASH_COMPLETE)
		{
			printf("erase sector %d fail\r\n", nSector);
			return e_Feedback_Fail;
		}else{
			printf("erase sector %d success\r\n", nSector);
		}
		nSpace = Get_Next_Sector_Space(nAddr);
	}
	return e_Feedback_Success;
}



//UINT8 Erase_FireWare_Sector(void)
//{
//	UINT8 status;
//	UINT16 nSector = 0;
//	UINT32 nAddr = 0;
//	UINT32 nSpace = 0;
//	
//	for(nAddr = FLASH_FIREWARE_START_ADDR; nAddr < (FLASH_FIREWARE_START_ADDR + FLASH_FIREWARE_LEN); nAddr += nSpace)
//	{
//		nSector = STMFLASH_GetFlashSector(nAddr);
//		status = FLASH_EraseSector(nSector, VoltageRange_3);
//		if(status != FLASH_COMPLETE)
//		{
//			printf("erase sector %d fail\r\n", nSector);
//			return e_Feedback_Fail;
//		}else{
//			printf("erase sector %d success\r\n", nSector);
//		}
//		nSpace = Get_Next_Sector_Space(nAddr);
//	}
//}


//UINT8 Erase_App_Sector(void)
//{
//	UINT8 status;
//	UINT16 nSector = 0;
//	UINT32 nAddr = 0;
//	UINT32 nSpace = 0;
//	
//	for(nAddr = FLASH_APP_START_ADDR; nAddr < (FLASH_APP_START_ADDR + FLASH_APP_LEN); nAddr += nSpace)
//	{
//		nSector = STMFLASH_GetFlashSector(nAddr);
//		status = FLASH_EraseSector(nSector, VoltageRange_3);
//		if(status != FLASH_COMPLETE)
//		{
//			printf("erase sector %d fail\r\n", nSector);
//			return e_Feedback_Fail;
//		}else{
//			printf("erase sector %d success\r\n", nSector);
//		}
//		nSpace = Get_Next_Sector_Space(nAddr);
//	}

//}


UINT32 Flash_Write_Data(UINT32 pWAddr, UINT8* pData, UINT16 nLen)
{
	UINT16 i = 0;
	UINT8* pAddr = (UINT8*)pWAddr;
	
	for(i = 0; i < nLen; i++)
	{
		*(pAddr++) = *(pData + i);
	}
	return (UINT32)pAddr;
}


UINT8 Flash_Fireware_Update(RECORD_PARAM* pParam, UINT32* pSrc, UINT32* pDst)
{	
	UINT16 i = 0; 
	
	UINT8 *pFireware = (UINT8 *)pSrc;
	UINT8 *pApp = (UINT8*)pDst;
	
	for(i = 0; i < pParam->nUpdataLen; i++)
	{
		*(pApp) =  *(pFireware);
		pFireware++;
		pApp++;
	}
	return e_Feedback_Success;
}

#endif






