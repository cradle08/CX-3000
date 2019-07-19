// StorageManagement.h

#ifndef  __STORAGE_MANAGEMENT_H__
#define  __STORAGE_MANAGEMENT_H__


#include "MyType.h"


//-----------------------------------------------------------------------------------------



// definition

#define FM_OUTPUT              0

//
#define FM_LOAD_COMMAND_SIZE   260
//
#if 0
#define FM_FLAG_ADDR           0
#define FM_FLAG_SIZE           4
#define FM_FLAG_1              0x12
#define FM_FLAG_2              0x34
#define FM_FLAG_3              0x56
#define FM_FLAG_4              0x78
#endif
//
#define FM_PARA_CHECK_LIST          0x1234
#define FM_PARA_CHECK_NUM           4
#define FM_PARA_RW_ADDR_SCOPE       0    
#define FM_PARA_RW_ADDR_SAMPLE      200   
#define FM_PARA_LEN_SCOPE           54    // 54 chars
#define FM_PARA_LEN_SAMPLE          72    // 72 chars


//_EXT_ UINT8 XRAM_ g_const_ach[4]; 



//FLASH 扇区的起始地址
#define ADDR_FLASH_SECTOR_0     ((u32)0x08000000) 	//扇区0起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_1     ((u32)0x08004000) 	//扇区1起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_2     ((u32)0x08008000) 	//扇区2起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_3     ((u32)0x0800C000) 	//扇区3起始地址, 16 Kbytes  
#define ADDR_FLASH_SECTOR_4     ((u32)0x08010000) 	//扇区4起始地址, 64 Kbytes  
#define ADDR_FLASH_SECTOR_5     ((u32)0x08020000) 	//扇区5起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_6     ((u32)0x08040000) 	//扇区6起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_7     ((u32)0x08060000) 	//扇区7起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_8     ((u32)0x08080000) 	//扇区8起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_9     ((u32)0x080A0000) 	//扇区9起始地址, 128 Kbytes  
#define ADDR_FLASH_SECTOR_10    ((u32)0x080C0000) 	//扇区10起始地址,128 Kbytes  
#define ADDR_FLASH_SECTOR_11    ((u32)0x080E0000) 	//扇区11起始地址,128 Kbytes  

 //sector 2 ,0x08008000-0x0800BFFF=16k, 0x08008000-0x0800BFFF=12k
#define FLASH_SAVE_ADDR    0x08009000
#define FLASH_INIT_FLAG    0xabcd
#define RECORD_PARAM_LEN  (sizeof(RECORD_PARAM))
typedef struct {
	UINT16 nFlag;
	UINT8 nWBC;       // wbc value
	UINT16 nXAddStep; // moto x home, add step
	INT32 nAddPress; // press
	UINT32 nCrc;      // crc
} RECORD_PARAM;
_EXT_ RECORD_PARAM g_Record_Param;

	
void Set_Default_Param(RECORD_PARAM *pParam);
UINT16 STMFLASH_GetFlashSector(UINT32 addr);
UINT8 Flash_Read_Param(RECORD_PARAM *pParam, UINT32 nLen);
UINT8 Flash_Write_Param(RECORD_PARAM *pParam, UINT32 nLen);


#endif




