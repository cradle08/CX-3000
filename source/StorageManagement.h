// StorageManagement.h

#ifndef  __STORAGE_MANAGEMENT_H__
#define  __STORAGE_MANAGEMENT_H__


#include "MyType.h"

#define CX_2000_FLAG			   2
#define CX_3000_FLAG			   3
#define CX_BOOT_PLATFORM_VERSION   CX_3000_FLAG   // 2-cx2000, 3-cx3000


//-----------------------------------------------------------------------------------------



// 

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

#define	UPDATE_FINISHED	 			0x11
#define UPDATE_WAITING				0x33
#define	UPDATE_RESTART 	 			0x55
#define	UPDATE_NONE 	 			0xFF


#define FLASH_BASE_ADDR				0x08000000
#define FLASH_SECTOR_16K_LEN		0x4000
#define FLASH_SECTOR_64K_LEN		0x10000
#define FLASH_SECTOR_128K_LEN		0x20000





typedef void (*pAppFunc)(void);

#if(CX_BOOT_PLATFORM_VERSION == CX_2000_FLAG) 
	/* boot:96k(0x08000000-0x08003fff, 0x08008000-0x0801ffff), 
		app:128k(0x08020000-0x0803ffff),
		fireware:256k(0x08040000-0x0807ffff),
	*/
	#define APP_VECT_TAB_OFFSET			0x00020000  
	#define FLASH_APP_START_ADDR		0x08020000
	#define FLASH_APP_LEN				0x1FFFF
	#define FLASH_FIREWARE_START_ADDR	0x08040000
	#define FLASH_FIREWARE_LEN			0x3FFFF
#else// cx3000
	/* boot:96k(0x08000000-0x08003fff, 0x08008000-0x0801ffff), 
		app:512k(0x08020000-0x0809ffff),
		fireware:384k(0x080A0000-0x080fffff),
	*/
	#define APP_VECT_TAB_OFFSET			0x00020000  
	#define FLASH_APP_START_ADDR		0x08020000		// app start from this addr
	#define FLASH_APP_LEN				0x7FFFF
	#define FLASH_FIREWARE_START_ADDR	0x080A0000		// update file save from this addr
	#define FLASH_FIREWARE_LEN			0x6FFFF

#endif

typedef void (*pAppFunc)(void);

//sector 1 ,0x08004000-0x08007FFF=16k, 0x08005000-0x08007FFF=12k
#define FLASH_PARAM_SAVE_ADDR    0x08005000
#define FLASH_INIT_FLAG    0xabcd
#define RECORD_PARAM_LEN  (sizeof(RECORD_PARAM))
typedef struct {
	UINT16 nFlag;
	UINT8  nRegister_WBC;       // wbc value
	UINT8  nRegister_RBC;
	UINT8  nRegister_PLT;
	UINT8  nRegister_RBC_PLT; 
	UINT8  nRegister_HGB;
	UINT8  nRegister_CRP;
	UINT32 nUpdataLen;
	UINT16 nXAddStep; // moto x home, add step
	INT32  nAddPress; // press
	UINT32 nCrc;      // crc
	// CRP
	UINT16 nTime; // time to get data
	UINT16 nHZ;   // in one second how many data to get
	UINT16 nTotal_Num; // in nTime and nHz, nTotal_Num(nTime*nHz) will be get
	// update flag
	UINT8 nUpdate_Flag;
	
	
} RECORD_PARAM;
_EXT_ RECORD_PARAM g_Record_Param;


void Iint_SysParam(void);
void Set_Default_Param_Machine(RECORD_PARAM *pParam);
void Set_Default_Param(RECORD_PARAM *pParam);
UINT16 STMFLASH_GetFlashSector(UINT32 addr);
UINT8 Flash_Read_Param(RECORD_PARAM *pParam, UINT32 nLen);
UINT8 Flash_Write_Param(RECORD_PARAM *pParam, UINT32 nLen);
UINT8 Erase_Specify_Sector(UINT32 nStartAddr, UINT32 nLen);

UINT32 Flash_Write_Data(UINT32 pWAddr, UINT8* pData, UINT16 nLen);
UINT8 Flash_Fireware_Update(RECORD_PARAM* pParam, UINT32* pSrc, UINT32* pDst);


#endif




