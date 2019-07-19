// Fpga.c

#ifndef  __FPGA_C__
#define  __FPGA_C__


#include "ChainHeader.h"


//-----------------------------------------------------------------------------------------
// definition
#define Bank1_SRAM1_ADDR    ((UINT32)0x60000000) 
#define Bank1_SRAM2_ADDR    ((UINT32)0x64000000) 
#define Bank1_SRAM3_ADDR    ((UINT32)0x68000000) 
#define Bank1_SRAM4_ADDR    ((UINT32)0x6C000000) 

//-----------------------------------------------------------------------------------------
// variable definition


//-----------------------------------------------------------------------------------------
// function definition

// initialization
void  FPGA_Init(void)
{	
	GPIO_InitTypeDef  GPIO_InitStructure;
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
	FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 

	// 1. enable the clocks
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);// the clocks of PD/PE/PF/PG  
	RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);    // the clock of the FSMC  
	// 2. AFIO configuration  
	GPIO_InitStructure.GPIO_Pin = (3<<0)|(3<<4)|(1<<7)|(0XFF<<8); // PD 0,1, 4,5, 7, 8,9,10,11,12,13,14,15 AF	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	   // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);	
	//
	GPIO_InitStructure.GPIO_Pin =  (0X1FF<<7);			   // PE 7~15 AF  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	   // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOE, &GPIO_InitStructure);	
	//
	GPIO_InitStructure.GPIO_Pin = (0X3F<<0)|(0XF<<12);	   // PF 0~5, 12~15 AF
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	   // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOF, &GPIO_InitStructure);	
	//
	GPIO_InitStructure.GPIO_Pin =(0X3F<<0)|(0x01<<12);   // PG 0~5, 12 AF
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	   // 100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOG, &GPIO_InitStructure); 
	// 3. AF-model mapping
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);  // PD0, AF12
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);  // PD1, AF12
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_FSMC);  // NCE1
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC); // PD15, AF12
	//
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);  // PE7, AF12
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC); // PE15, AF12
	//
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource0,GPIO_AF_FSMC);  // PF0, AF12
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource1,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource2,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource3,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource4,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource5,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource13,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource14,GPIO_AF_FSMC); 
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource15,GPIO_AF_FSMC); // PF15, AF12
	//
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource0,GPIO_AF_FSMC);  // PG0, AF12
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource1,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource2,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource3,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource4,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource5,GPIO_AF_FSMC);
	GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC); // PG12, NCE4 ,AF12	
	// 4. timing-struct configuration
	readWriteTiming.FSMC_AddressSetupTime = 0x01;	 // ADDSET, 1*HCLK(1/36M=27ns)
	readWriteTiming.FSMC_AddressHoldTime = 0x00;	 // ADDHLD, not be used in model-A
	readWriteTiming.FSMC_DataSetupTime = 0x0C;		 // DATAST, 9*HCLK(6*9=54ns)	 
	readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
	readWriteTiming.FSMC_CLKDivision = 0x00;
	readWriteTiming.FSMC_DataLatency = 0x01;
	readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 // access-model A	 
	// 5. FSMC configuration
	FSMC_NORSRAMStructInit(&FSMC_NORSRAMInitStructure);
	// 
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;						 // NE1
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;				 // FSMC_MemoryType_SRAM;	//SRAM	 
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;		 // 16bit	
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;	 // FSMC_BurstAccessMode_Disable; 
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;	 
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;	
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable; 	 // enable
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;	
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;		 // use the same timing-struct
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;	
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &readWriteTiming;
	// 1) init NE1
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	// 2) init NE4
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;						 // NE4
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	
	// 6. hardware enable
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);	
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);	
    //
}


// reset the hardware of the fpga
void   FPGA_ResetHardware(void)
{
    IO_ UINT32 IRAM_  nAddr  = 0; 
    IO_ UINT16 IRAM_  anBuffer[2];
    
	// address
    nAddr = (UINT32)FPGA_WR_MOTOR_REGISTER_BASE + 
		            FPGA_WR_FPGA_RESET;
    // set "1" to the named bit of a hard-reset register
    anBuffer[0] = (UINT16)( FPGA_WR_V_FPGA_RESET & 0xFFFF);
    FPGA_WriteBuffer((UINT16*)anBuffer, nAddr, 1);
	// wait some time
	IT_SYS_DlyMs(5);
	
	// set "0" to the named bit of a hard-reset register
    anBuffer[0] = (UINT16)( FPGA_WR_V_FPGA_RUN & 0xFFFF);
    FPGA_WriteBuffer((UINT16*)anBuffer, nAddr, 1);
	// wait some time
	IT_SYS_DlyMs(5);

	//
	HW_DIR_Ctrl(); 
	HW_ADC_SpiCtrl(e_True); 

	//==========================
	// switch off : WBC and RBC 
	HW_SW_AdcWBC(e_False);
	HW_SW_AdcRBC(e_False);
	//
	HW_PUMP_Pulse(0, e_Dir_Pos);  // off 
	HW_Valve_Off(0); // all the air way
	HW_Valve_Off(1); // RBC
	HW_Valve_Off(2); // WBC
	
	return;
}

// send a command to the FPGA's write-only register, and control it
void   FPGA_WriteBuffer(UINT16* pnBuffer, UINT32 nWriteAddr, UINT32 nNumHalfwordToWrite)
{
    for( ; nNumHalfwordToWrite != 0; nNumHalfwordToWrite--) /*!< while there is data to write */
    {
#if  FPGA_OUTPUT
        printf("write bus: addr-%d, data-%d\r\n", (int)(nWriteAddr), (int)(*pnBuffer));
#endif
	    // Transfer data to the memory 
	    *(UINT16*) (Bank1_SRAM1_ADDR + nWriteAddr) = *pnBuffer++; // attention: volatile variable
	
	    // Increment the address 
	    nWriteAddr += 2;
    }  
	//
	return;
}

// send a command to the FPGA's read-only register, and receive the data
void   FPGA_ReadBuffer(UINT16* pnBuffer, UINT32 nReadAddr, UINT32 nNumHalfwordToRead)
{
    for( ; nNumHalfwordToRead != 0; nNumHalfwordToRead--) /*!< while there is data to read */
    {
	    // Read a half-word from the memory
	    *pnBuffer++ = *(IO_ UINT16*) (Bank1_SRAM1_ADDR + nReadAddr);  // attention: volatile variable
#if  FPGA_OUTPUT
		printf("read bus: addr-%d, data-%d\r\n", (int)(nReadAddr), (int)(*(pnBuffer - 1)) );
#endif
	    // Increment the address   
	    nReadAddr += 2;
    } 
	//
	return;
} 


//----------------------

// writing the data buffer 
void   FPGA_DATA_WriteBuffer(UINT16* pnBuffer, UINT32 nWriteAddr, UINT32 nNumHalfwordToWrite)
{
	for( ; nNumHalfwordToWrite != 0; nNumHalfwordToWrite--) /*!< while there is data to write */
	{
#if  FPGA_OUTPUT
		printf("write the data bus: addr-%d, data-%d\r\n", (int)(nWriteAddr), (int)(*pnBuffer));
#endif
		// Transfer data to the memory 
		*(UINT16*) (Bank1_SRAM4_ADDR + nWriteAddr) = *pnBuffer++; // attention: volatile variable
	
		// Increment the address 
		nWriteAddr += 2;
	}  
	//
	return;
}


// reading the data buffer
void   FPGA_DATA_ReadBuffer(UINT16* pnBuffer, UINT32 nReadAddr, UINT32 nNumHalfwordToRead)
{
	for( ; nNumHalfwordToRead != 0; nNumHalfwordToRead--) /*!< while there is data to read */
	{
		// Read a half-word from the memory
		*pnBuffer++ = *(IO_ UINT16*) (Bank1_SRAM4_ADDR + nReadAddr);  // attention: volatile variable
#if  FPGA_OUTPUT
		printf("read the data bus: addr-%d, data-%d\r\n", (int)(nReadAddr), (int)(*(pnBuffer - 1)) );
#endif
		// Increment the address   
		nReadAddr += 2;
	} 
	//
	return;
}



#endif

