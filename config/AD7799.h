
#ifndef __AD7799_H__
#define __AD7799_H__

#include "MyType.h"  



/*AD7799 Registers*/
#define AD7799_REG_COMM		0 /* Communications Register(WO, 8-bit) */
#define AD7799_REG_STAT	    0 /* Status Register	    (RO, 8-bit) */
#define AD7799_REG_MODE	    1 /* Mode Register	     	(RW, 16-bit */
#define AD7799_REG_CONF	    2 /* Configuration Register (RW, 16-bit)*/
#define AD7799_REG_DATA	    3 /* Data Register	     	(RO, 16-/24-bit) */
#define AD7799_REG_ID	    4 /* ID Register	     	(RO, 8-bit) */
#define AD7799_REG_IO	    5 /* IO Register	     	(RO, 8-bit) */
#define AD7799_REG_OFFSET   6 /* Offset Register	    (RW, 24-bit */
#define AD7799_REG_FULLSALE	7 /* Full-Scale Register	(RW, 24-bit */
 
/* Communications Register Bit Designations (AD7799_REG_COMM) */
#define AD7799_COMM_WEN		(1 << 7) 			/* Write Enable */
#define AD7799_COMM_WRITE	(0 << 6) 			/* Write Operation */
#define AD7799_COMM_READ    (1 << 6) 			/* Read Operation */
#define AD7799_COMM_ADDR(x)	(((x) & 0x7) << 3)	/* Register Address */
#define AD7799_COMM_CREAD	(1 << 2) 			/* Continuous Read of Data Register */
/* Status Register Bit Designations (AD7799_REG_STAT) */
#define AD7799_STAT_RDY		(1 << 7) /* Ready */
#define AD7799_STAT_ERR		(1 << 6) /* Error (Overrange, Underrange) */
#define AD7799_STAT_CH3		(1 << 2) /* Channel 3 */
#define AD7799_STAT_CH2		(1 << 1) /* Channel 2 */
#define AD7799_STAT_CH1		(1 << 0) /* Channel 1 */
/* Mode Register Bit Designations (AD7799_REG_MODE) */
#define AD7799_MODE_SEL(x)		(((x) & 0x7) << 13)	/* Operation Mode Select */
#define AD7799_MODE_PSW(x)		(1 << 12)			/* Power Switch Control Bit */	
#define AD7799_MODE_RATE(x)		((x) & 0xF) 		/* Filter Update Rate Select */
/* AD7799_MODE_SEL(x) options */
#define AD7799_MODE_CONT		 0 /* Continuous Conversion Mode */
#define AD7799_MODE_SINGLE		 1 /* Single Conversion Mode */
#define AD7799_MODE_IDLE		 2 /* Idle Mode */
#define AD7799_MODE_PWRDN		 3 /* Power-Down Mode */
#define AD7799_MODE_CAL_INT_ZERO 4 /* Internal Zero-Scale Calibration */
#define AD7799_MODE_CAL_INT_FULL 5 /* Internal Full-Scale Calibration */
#define AD7799_MODE_CAL_SYS_ZERO 6 /* System Zero-Scale Calibration */
#define AD7799_MODE_CAL_SYS_FULL 7 /* System Full-Scale Calibration */

// update rates 
#define AD7799_MODE_UPDATE_4        0xF	/*4.17 */
#define AD7799_MODE_UPDATE_6        0xE	/* 6.25  */
#define AD7799_MODE_UPDATE_8        0xD /* 8.33  */
#define AD7799_MODE_UPDATE_10       0xC	/* 10  */
#define AD7799_MODE_UPDATE_12       0xB	/* 12.5  */
#define AD7799_MODE_UPDATE_16       0xA	/* 16.7  */
//#define AD7799_MODE_UPDATE_16     0x9	/* 16.7  */
#define AD7799_MODE_UPDATE_19       0x8	/* 19.6  */
#define AD7799_MODE_UPDATE_33       0x7	/* 33.2  */
#define AD7799_MODE_UPDATE_39       0x6	/* 39  */
#define AD7799_MODE_UPDATE_50       0x5	/* 50  */
#define AD7799_MODE_UPDATE_62       0x4	/* 62  */
#define AD7799_MODE_UPDATE_123      0x3	/* 123  */
#define AD7799_MODE_UPDATE_242      0x2	/* 242  */
#define AD7799_MODE_UPDATE_470      0x1	/* 470  */

/* Configuration Register Bit Designations (AD7799_REG_CONF) */
#define AD7799_CONF_BO_EN	  (1 << 13) 			/* Burnout Current Enable */
#define AD7799_CONF_UNIPOLAR  (1 << 12) 			/* Unipolar/Bipolar Enable */
#define AD7799_CONF_GAIN(x)	  (((x) & 0x7) << 8) 	/* Gain Select */
#define AD7799_CONF_REFDET(x) (((x) & 0x1) << 5) 	/* Reference detect function */
#define AD7799_CONF_BUF		  (1 << 4) 				/* Buffered Mode Enable */
#define AD7799_CONF_CHAN(x)	  ((x) & 0x7) 			/* Channel select */
/* AD7799_CONF_GAIN(x) options */
#define AD7799_GAIN_1       0
#define AD7799_GAIN_2       1
#define AD7799_GAIN_4       2
#define AD7799_GAIN_8       3
#define AD7799_GAIN_16      4
#define AD7799_GAIN_32      5
#define AD7799_GAIN_64      6
#define AD7799_GAIN_128     7
/* AD7799_CONF_REFDET(x) options */
#define AD7799_REFDET_ENA   1	
#define AD7799_REFDET_DIS   0
/* AD7799_CONF_CHAN(x) options */
#define AD7799_CH_AIN1P_AIN1M	0 /* AIN1(+) - AIN1(-) */
#define AD7799_CH_AIN2P_AIN2M	1 /* AIN2(+) - AIN2(-) */
#define AD7799_CH_AIN3P_AIN3M	2 /* AIN3(+) - AIN3(-) */
#define AD7799_CH_AIN1M_AIN1M	3 /* AIN1(-) - AIN1(-) */
#define AD7799_CH_AVDD_MONITOR	7 /* AVDD Monitor */
/* ID Register Bit Designations (AD7799_REG_ID) */
#define AD7799_ID			0x9
#define AD7799_ID_MASK		0xF
/* IO (Excitation Current Sources) Register Bit Designations (AD7799_REG_IO) */
#define AD7799_IOEN			(1 << 6)
#define AD7799_IO1(x)		(((x) & 0x1) << 4)
#define AD7799_IO2(x)		(((x) & 0x1) << 5)


#define AD7799_SET_CHAN_1_CMD  (AD7799_CONF_GAIN(AD7799_GAIN_1) | AD7799_CONF_BUF | AD7799_CH_AIN1P_AIN1M) 
#define AD7799_SET_CHAN_2_CMD  (AD7799_CONF_GAIN(AD7799_GAIN_1) | AD7799_CONF_BUF | AD7799_CH_AIN2P_AIN2M) 


// AD7799 for 24bits HGB and CRP data, SPI3_CLK_PC10, SPI3_MOSI_PB5(PC12),
// SPI3_MISO_PB4(PC11)	SPI3_CS_PA15(PC13)
//#define ADC24BIT_CLK_PORT					GPIOC
//#define ADC24BIT_CLK_PIN					GPIO_Pin_10
//#define ADC24BIT_CLK_SRC					RCC_AHB1Periph_GPIOC
//#define ADC24BIT_CLK_AF_SRC 				GPIO_PinSource10

//#define ADC24BIT_MOSI_PORT					GPIOB
//#define ADC24BIT_MOSI_PIN					GPIO_Pin_5
//#define ADC24BIT_MOSI_SRC					RCC_AHB1Periph_GPIOB
//#define ADC24BIT_MOSI_AF_SRC 				GPIO_PinSource5

//#define ADC24BIT_MISO_PORT					GPIOB
//#define ADC24BIT_MISO_PIN					GPIO_Pin_4
//#define ADC24BIT_MISO_SRC					RCC_AHB1Periph_GPIOB
//#define ADC24BIT_MISO_AF_SRC 				GPIO_PinSource4

//#define ADC24BIT_CS_PORT					GPIOA
//#define ADC24BIT_CS_PIN						GPIO_Pin_15
//#define ADC24BIT_CS_SRC						RCC_AHB1Periph_GPIOA
//#define ADC24BIT_CS_AF_SRC 					GPIO_PinSource15


//#define ADC24BIT_CLK_PORT					GPIOC
//#define ADC24BIT_CLK_PIN					GPIO_Pin_10
//#define ADC24BIT_CLK_SRC					RCC_AHB1Periph_GPIOC
//#define ADC24BIT_CLK_AF_SRC 				GPIO_PinSource10

//#define ADC24BIT_MOSI_PORT					GPIOB
//#define ADC24BIT_MOSI_PIN					GPIO_Pin_5
//#define ADC24BIT_MOSI_SRC					RCC_AHB1Periph_GPIOB
//#define ADC24BIT_MOSI_AF_SRC 				GPIO_PinSource5

//#define ADC24BIT_MISO_PORT					GPIOB
//#define ADC24BIT_MISO_PIN					GPIO_Pin_4
//#define ADC24BIT_MISO_SRC					RCC_AHB1Periph_GPIOB
//#define ADC24BIT_MISO_AF_SRC 				GPIO_PinSource4

//#define ADC24BIT_CS_PORT					GPIOA
//#define ADC24BIT_CS_PIN						GPIO_Pin_15
//#define ADC24BIT_CS_SRC						RCC_AHB1Periph_GPIOA
//#define ADC24BIT_CS_AF_SRC 					GPIO_PinSource15

// PC10_SP3_CLK
#define ADC24BIT_CLK_PORT					GPIOC
#define ADC24BIT_CLK_PIN					GPIO_Pin_10
#define ADC24BIT_CLK_SRC					RCC_AHB1Periph_GPIOC
#define ADC24BIT_CLK_AF_SRC 				GPIO_PinSource10

// PC12_SP3_MOSI
#define ADC24BIT_MOSI_PORT					GPIOC
#define ADC24BIT_MOSI_PIN					GPIO_Pin_12
#define ADC24BIT_MOSI_SRC					RCC_AHB1Periph_GPIOB
#define ADC24BIT_MOSI_AF_SRC 				GPIO_PinSource12

// PC11_SP3_MISO
#define ADC24BIT_MISO_PORT					GPIOC
#define ADC24BIT_MISO_PIN					GPIO_Pin_11
#define ADC24BIT_MISO_SRC					RCC_AHB1Periph_GPIOB
#define ADC24BIT_MISO_AF_SRC 				GPIO_PinSource11

// PA15_SP3_CS
#define ADC24BIT_CS_PORT					GPIOA
#define ADC24BIT_CS_PIN						GPIO_Pin_15
#define ADC24BIT_CS_SRC						RCC_AHB1Periph_GPIOA
#define ADC24BIT_CS_AF_SRC 					GPIO_PinSource15



#define ADC24BIT_SPI						SPI3
#define ADC24BIT_SPI_SRC 					RCC_APB1Periph_SPI3
#define ADC24BIT_SPI_AF 					GPIO_AF_SPI3

#define AD7799_CS_LOW	GPIO_SetBits(ADC24BIT_CS_PORT, ADC24BIT_CS_PIN)
#define AD7799_CS_HIGH	GPIO_ResetBits(ADC24BIT_CS_PORT, ADC24BIT_CS_PIN)

#define AD_SCK_1()	GPIO_SetBits(ADC24BIT_CLK_PORT, ADC24BIT_CLK_PIN)
#define AD_SCK_0()	GPIO_ResetBits(ADC24BIT_CLK_PORT, ADC24BIT_CLK_PIN)
#define AD_CS_1()	GPIO_SetBits(ADC24BIT_CS_PORT, ADC24BIT_CS_PIN)
#define AD_CS_0()	GPIO_ResetBits(ADC24BIT_CS_PORT, ADC24BIT_CS_PIN)
#define AD_DI_1()	GPIO_SetBits(ADC24BIT_MOSI_PORT, ADC24BIT_MOSI_PIN)
#define AD_DI_0()	GPIO_ResetBits(ADC24BIT_MOSI_PORT, ADC24BIT_MOSI_PIN)
#define AD_DO()		GPIO_ReadInputDataBit(ADC24BIT_MISO_PORT, ADC24BIT_MOSI_PIN)


//#define ADC_RDY_DAT (AD_DO)


#define AD7799_USE_SPI_COMMUNICATION		0
//
void ADC24Bit_Init(void);
void ADC24Bit_SPI_Init(void);
UINT16 ADC24Bit_SPI_GetByte(void);
void ADC24Bit_SPI_SendByte(UINT8 nData);
void ADC24Bit_GPIO_Init(void);


void SPI_Read(UINT8 *pBuf, UINT8 nCount);
void SPI_Write(UINT8 *pBuf, UINT8 nCount);
UINT8 AD7799_Init(void);
UINT32 AD7799_GetRegisterValue(UINT8 regAddress, UINT8 size);
void AD7799_SetRegisterValue(UINT8 regAddress,
                             UINT32 regValue, 
                             UINT8 size);
void AD7799_Reset(void);


UINT32 AD7799_Get_ADC_Value(void);




































#endif //  __AD7799_H__
