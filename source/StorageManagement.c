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
	pParam->nFlag     = FLASH_INIT_FLAG;
	pParam->nWBC      = 20; // wbc garter value
	pParam->nXAddStep = 0;  // add step for moto x go home
	pParam->nAddPress = 0;     
	pParam->nCrc      = 1;  // this value need to compute
}


UINT8 Flash_Read_Param(RECORD_PARAM *pParam, UINT32 nLen)
{
	UINT32 i;
	UINT8 *pAddr = (UINT8 *)FLASH_SAVE_ADDR;
	UINT8 *pData = (UINT8*)pParam;
	
	for(i = 0; i < nLen; i++)
	{
		*(pData) =  *(pAddr);
		pAddr++;
		pData++;
	}
	return e_Feedback_Success;
}


UINT8 Flash_Write_Param(RECORD_PARAM *pParam, UINT32 nLen)
{
	UINT8 *pAddr = (UINT8*)FLASH_SAVE_ADDR;
	UINT8 *pData = (UINT8*)pParam;
	UINT16 index;
	
	FLASH_Status status = FLASH_COMPLETE;
	FLASH_Unlock();
	FLASH_DataCacheCmd(DISABLE);
	
	status = FLASH_EraseSector(STMFLASH_GetFlashSector(FLASH_SAVE_ADDR), VoltageRange_3);
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




#endif



