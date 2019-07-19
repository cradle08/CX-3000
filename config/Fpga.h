// Fpga.h

#ifndef  __FPGA_H__
#define  __FPGA_H__


#include "MyType.h"


#define  FPGA_OUTPUT                   0 
//-----------------------------------------------------------------------------------------
// definition
// attention: 16-bits data bus accessing, 32-bits-width parameter,
//            but address is encoded by bytes!

//--------------------- write only parameters ----------------------//

// motor-control
#define  FPGA_WR_MOTOR_REGISTER_BASE   0
#define  FPGA_WR_MOTOR_PARA_OFFSET    40  // 40 * 2 = 80
// attention: not half-word, but one full-word (4 bytes)
#define  FPGA_WR_MOTOR_EN              0  // 0`3
#define  FPGA_WR_MOTOR_DIR             4  // 4`7
#define  FPGA_WR_MOTOR_START           8  // 8`11
#define  FPGA_WR_MOTOR_ST_ACC         12  // 12`15
#define  FPGA_WR_MOTOR_ST_EQU         16  // 16`19
#define  FPGA_WR_MOTOR_ST_DEC         20  // 20`23
#define  FPGA_WR_MOTOR_FQ_STR         24  // 24`27
#define  FPGA_WR_MOTOR_FQ_ACC         28  // 28`31
#define  FPGA_WR_MOTOR_FQ_DEC         32  // 32`35
#define  FPGA_WR_MOTOR_FQ_SAM         36  // 36`39
//
#define  FPGA_WR_V_MOTOR_LOCK        (UINT16)0x0001    // level high is on low power
#define  FPGA_WR_V_MOTOR_UNLOCK      (UINT16)0x0000
#define  FPGA_WR_V_MOTOR_MOVE        (UINT16)0x0001
#define  FPGA_WR_V_MOTOR_STOP        (UINT16)0x0000

// reset the fpga
#define  FPGA_WR_FPGA_RESET          80 // 40 * 2 
//
#define  FPGA_WR_V_FPGA_RESET        (UINT16)0x0001
#define  FPGA_WR_V_FPGA_RUN          (UINT16)0x0000

// valves-driver
#define  FPGA_WR_VALVE_01_06         82  // 80+2  

// mixing motor control
#define  FPGA_WR_PUMP_RUN            84  // offset: 82 + 2, zone: 84~85, value: 1-run 0-stop
#define  FPGA_WR_PUMP_DIR            86  // zone: 86~87
#define  FPGA_WR_PUMP_FQ_CNT         88  // zone: 88~91, len: 4 bytes

// the digtal adjustable resistor 
#define  FPGA_WR_ADJ_RES_01          92  // 88 + 4                   
#define  FPGA_WR_ADJ_RES_02          94   
#define  FPGA_WR_ADJ_RES_03          96  
#define  FPGA_WR_ADJ_RES_04          98 


// to control the SPI ADC
#define  FPGA_WR_SPI_ADC_CTRL        100 
//                                                     // AD4  AD3  AD2  AD1  config
#define  FPGA_WR_V_SPI_ADC_ON        (UINT16)0x001F    // bit4\bit3\bit2\bit1\bit0
#define  FPGA_WR_V_SPI_ADC_OFF       (UINT16)0x0000

// to control the direction of the boards
#define  FPGA_WR_DIRECTION_BOARD     102
//
#define  FPGA_WR_V_BOARD_1_IN        (UINT16)0x0001
#define  FPGA_WR_V_BOARD_1_OUT       (UINT16)0x0000
#define  FPGA_WR_V_BOARD_2_IN        (UINT16)0x0002
#define  FPGA_WR_V_BOARD_2_OUT       (UINT16)0x0000

// to control the switcher of the ADCs
#define  FPGA_WR_SW_WBC     		 104
#define  FPGA_WR_V_SW_WBC_ON        (UINT16)0x0001
#define  FPGA_WR_V_SW_WBC_OFF       (UINT16)0x0000
//
#define  FPGA_WR_SW_RBC              106
#define  FPGA_WR_V_SW_RBC_ON        (UINT16)0x0001
#define  FPGA_WR_V_SW_RBC_OFF       (UINT16)0x0000

// enable the WBC  
#define  FPGA_WR_ENABLE_WBC          108
#define  FPGA_WR_V_ENABLE_WBC_ON    (UINT16)0x0001
#define  FPGA_WR_V_ENABLE_WBC_OFF   (UINT16)0x0000

// the flag of reading the FSMC port 
#define  FPGA_WR_FLAG_FSMC_READING         110	
#define  FPGA_WR_V_FLAG_FSMC_READING_ON   (UINT16)0x0001
#define  FPGA_WR_V_FLAG_FSMC_READING_OFF  (UINT16)0x0000



//--------------------- read only parameters ----------------------//

// the motors' pulses recorded
#define  FPGA_RD_MOTOR_REGISTER_BASE   0 
#define  FPGA_RD_MOTOR_PARA_OFFSET     4  // 4 bytes
//
#define  FPGA_RD_MOTOR_STEPS_0         0  // 0`3
#define  FPGA_RD_MOTOR_STEPS_1         4  // 4`7

// feedback whether the movement has finished or not 
#define  FPGA_RD_STATUS_00_01          8  
//
#define  FPGA_RD_V_MOTOR_FIN_0       (UINT16)0x0001
#define  FPGA_RD_V_MOTOR_FIN_1       (UINT16)0x0002

// the electrode status
#define  FPGA_RD_ELECTRODE_01_04      10

// the OC input 
#define  FPGA_RD_OC_01_03             12 

// read the SPI ADC
#define  FPGA_RD_SPI_ADC_01           14
#define  FPGA_RD_SPI_ADC_02           16
#define  FPGA_RD_SPI_ADC_03           18
#define  FPGA_RD_SPI_ADC_04           20


// read the feedback frequency of the DC motor
#define  FPGA_RD_FREQ_PUMP            22   // 22~25, 4 bytes 

// the status of the FSMC 
#define  FPGA_RD_FLAG_FSMC_READY      26  


//--------------------- to read the data buffer ----------------------//
#define  PFGA_RD_DATA_BASE            0



//-----------------------------------------------------------------------------------------
// function declaration

// initialization
_EXT_ void   FPGA_Init(void);
// reset the hardware of the fpga
_EXT_ void   FPGA_ResetHardware(void);
// send a command to the FPGA's write-only register, and control it
_EXT_ void   FPGA_WriteBuffer(UINT16* pnBuffer, UINT32 nWriteAddr, UINT32 nNumHalfwordToWrite);
// send a command to the FPGA's read-only register, and receive the data
_EXT_ void   FPGA_ReadBuffer(UINT16* pnBuffer, UINT32 nReadAddr, UINT32 nNumHalfwordToRead);

//----------------------
// writing the data buffer 
_EXT_ void   FPGA_DATA_WriteBuffer(UINT16* pnBuffer, UINT32 nWriteAddr, UINT32 nNumHalfwordToWrite);
// reading the data buffer
_EXT_ void   FPGA_DATA_ReadBuffer(UINT16* pnBuffer, UINT32 nReadAddr, UINT32 nNumHalfwordToRead);



#endif

