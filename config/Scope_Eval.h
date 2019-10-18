// Scope_Eval.h

#ifndef    __SCOPE_EVAL_H__
#define    __SCOPE_EVAL_H__


#include "MyType.h"       // ---

#include <stdio.h>        // for "printf" debug


//-----------------------------------------------------------------------------------------
// macro defined for "printf" debugging
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


//-----------------------------------------------------------------------------------------
// SystemCoreClock 

// SYSCLK = (11.059200MHz * 240) / (8 * 2) = 165.888000 // changed by LHT

// 1、which is set in default value in function "SetSysClock()" of file "system_stm32f4xx.c"

// 2、HSE value is 11.0592MHz


// 3、SystemCoreClock: global variable defined if file "SystemCoreClock.h", 
//    so we can know the cclk

// 4、SystemCoreClockUpdate(): Updates the variable SystemCoreClock and must
//    be called whenever the core clock is changed during program execution.



//-----------------------------------------------------------------------------------------
// typedef data type 
typedef enum 
{
    E_COM_MAIN      = 0,
    E_COM_SAMPLING  = 1,
    E_COM_NULL      = 2,
  
} Com_TypeDef;   

typedef enum              // take attention of sequence
{
    O_STATUS_LED_1  = 0,
    O_STATUS_LED_2  = 1,
    O_MCU_LED_1     = 2,
    O_MCU_LED_2     = 3,
    O_LAN8720_RST   = 4,
    O_HEAT_1        = 5,
    O_HEAT_2        = 6,

	
} Output_TypeDef;

typedef enum 
{
    // for homing, common input
    I_HOME_X       = 0,
    I_HOME_Y       = 1,
    I_HOME_Z       = 2,
    I_HOME_M       = 3,
    I_HOME_5       = 4,
    I_HOME_6       = 5,
    // the motor status, to be used as a extinal-interrupt
    I_FEEDBACK_1   = 6, 

} Input_TypeDef;  

typedef enum
{
    IN_MODEL_GPIO = 0,
	IN_MODEL_EXTI = 1,	

} InModel_Typedef;


//-----------------------------------------------------------------------------------------
//
#define MS_OF_CCLK_COUNTING        8000   // 300

//-----------------------------------------------------------------------------------------
// com test in_out
#define COM_NUM                     3  // 
// PA9 - PA10
// brief Definition for COM port1, connected to USART1 
#define COMM1                       USART1
#define COM1_CLK                    RCC_APB2Periph_USART1
#define COM1_IRQn                   USART1_IRQn
#define COM1_TX_PIN                 GPIO_Pin_9
#define COM1_TX_GPIO_PORT           GPIOA
#define COM1_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA     
#define COM1_RX_PIN                 GPIO_Pin_10
#define COM1_RX_GPIO_PORT           GPIOA
#define COM1_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define COM1_AF_TX_PIN_SOURCE       GPIO_PinSource9
#define COM1_AF_RX_PIN_SOURCE       GPIO_PinSource10
#define COM1_AF_UART                GPIO_AF_USART1
// PC10 - PC11
// brief Definition for COM port2, connected to USART3
#define COMM2                       USART3
#define COM2_CLK                    RCC_APB1Periph_USART3
#define COM2_IRQn                   USART3_IRQn
#define COM2_TX_PIN                 GPIO_Pin_10
#define COM2_TX_GPIO_PORT           GPIOC
#define COM2_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define COM2_RX_PIN                 GPIO_Pin_11
#define COM2_RX_GPIO_PORT           GPIOC
#define COM2_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define COM2_AF_TX_PIN_SOURCE       GPIO_PinSource10
#define COM2_AF_RX_PIN_SOURCE       GPIO_PinSource11
#define COM2_AF_UART                GPIO_AF_USART3
// PC6 - PC7
// brief Definition for COM port3, connected to USART6   
#define COMM3                       USART6                
#define COM3_CLK                    RCC_APB2Periph_USART6
#define COM3_IRQn                   USART6_IRQn 
#define COM3_TX_PIN                 GPIO_Pin_6
#define COM3_TX_GPIO_PORT           GPIOC
#define COM3_TX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define COM3_RX_PIN                 GPIO_Pin_7
#define COM3_RX_GPIO_PORT           GPIOC
#define COM3_RX_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define COM3_AF_TX_PIN_SOURCE       GPIO_PinSource6
#define COM3_AF_RX_PIN_SOURCE       GPIO_PinSource7           
#define COM3_AF_UART                GPIO_AF_USART6
// COM defination
#define COM_DEBUG_OUTPUT            COMM1   
#define COM_MAIN                    COMM1      // 
#define COM_SAMPLING                COMM2      // 
#define COM_NULL                    COMM3      // 



//-----------------------------------------------------------------------------------------
// output: 
#define OUTPUT_NUM                          7   // 4 + 1 + 2, // + 2 + 1

// 1. leds output
// PF7  out_led_1   // status: runing
#define OUT_STATUS_LED1_GPIO_PIN            GPIO_Pin_7
#define OUT_STATUS_LED1_GPIO_PORT           GPIOF
#define OUT_STATUS_LED1_GPIO_CLK            RCC_AHB1Periph_GPIOC 
// PF8  out_led_2   // status: warning
#define OUT_STATUS_LED2_GPIO_PIN            GPIO_Pin_8
#define OUT_STATUS_LED2_GPIO_PORT           GPIOF
#define OUT_STATUS_LED2_GPIO_CLK            RCC_AHB1Periph_GPIOF  
// PF9  mcu_led_1   // testing ===> PE4
#define OUT_MCU_LED1_GPIO_PIN               GPIO_Pin_4 //GPIO_Pin_9
#define OUT_MCU_LED1_GPIO_PORT              GPIOE //GPIOF
#define OUT_MCU_LED1_GPIO_CLK               RCC_AHB1Periph_GPIOE  //RCC_AHB1Periph_GPIOF 
// PF10 mcu_led_2   // testing ===> PE5
#define OUT_MCU_LED2_GPIO_PIN               GPIO_Pin_5 //GPIO_Pin_10
#define OUT_MCU_LED2_GPIO_PORT              GPIOE //GPIOF
#define OUT_MCU_LED2_GPIO_CLK               RCC_AHB1Periph_GPIOE  //RCC_AHB1Periph_GPIOF 

// 2. the pin to reset the LAN8720
// PD3  
#define OUT_LAN8720_RST_GPIO_PIN            GPIO_Pin_3
#define OUT_LAN8720_RST_GPIO_PORT           GPIOD
#define OUT_LAN8720_RST_GPIO_CLK            RCC_AHB1Periph_GPIOD

// 3. heating
// PA4  heat_1
#define OUT_HEAT1_GPIO_PIN                  GPIO_Pin_4
#define OUT_HEAT1_GPIO_PORT                 GPIOA
#define OUT_HEAT1_GPIO_CLK                  RCC_AHB1Periph_GPIOA
// PA5  heat_2
#define OUT_HEAT2_GPIO_PIN                  GPIO_Pin_5
#define OUT_HEAT2_GPIO_PORT                 GPIOA
#define OUT_HEAT2_GPIO_CLK                  RCC_AHB1Periph_GPIOA

#if 0
// 4. control the output of the scope-light
// PA6  light, on/off
#define OUT_LIGHT_SW_GPIO_PIN               GPIO_Pin_6
#define OUT_LIGHT_SW_GPIO_PORT              GPIOA
#define OUT_LIGHT_SW_GPIO_CLK               RCC_AHB1Periph_GPIOA

// 5. the cs pin of the spi_flash
// PB6  spi, cs_1
#define OUT_SPI_CS1_GPIO_PIN                GPIO_Pin_6
#define OUT_SPI_CS1_GPIO_PORT               GPIOB
#define OUT_SPI_CS1_GPIO_CLK                RCC_AHB1Periph_GPIOB
// PB7  spi, cs_2
#define OUT_SPI_CS2_GPIO_PIN                GPIO_Pin_7
#define OUT_SPI_CS2_GPIO_PORT               GPIOB
#define OUT_SPI_CS2_GPIO_CLK                RCC_AHB1Periph_GPIOB
#endif


//-----------------------------------------------------------------------------------------
// input --- common or exti interrupt
#define INPUT_NUM                       7   // 6 + 1

// 1. the home-detect of the 6 motors
// PB8     home oc, motor 1
#define IN_HOME_1_GPIO_PIN              GPIO_Pin_8
#define IN_HOME_1_GPIO_PORT             GPIOB
#define IN_HOME_1_GPIO_CLK              RCC_AHB1Periph_GPIOB 
#define IN_HOME_1_ET_LINE               EXTI_Line8   
#define IN_HOME_1_ET_PORT               EXTI_PortSourceGPIOB
#define IN_HOME_1_ET_PIN                EXTI_PinSource8  
#define IN_HOME_1_ET_IRQn               EXTI9_5_IRQn

// PB9     home oc, motor 2
#define IN_HOME_2_GPIO_PIN              GPIO_Pin_9
#define IN_HOME_2_GPIO_PORT             GPIOB
#define IN_HOME_2_GPIO_CLK              RCC_AHB1Periph_GPIOB 
#define IN_HOME_2_ET_LINE               EXTI_Line9   
#define IN_HOME_2_ET_PORT               EXTI_PortSourceGPIOB
#define IN_HOME_2_ET_PIN                EXTI_PinSource9
#define IN_HOME_2_ET_IRQn               EXTI9_5_IRQn

// PB10    home oc, motor 3
#define IN_HOME_3_GPIO_PIN              GPIO_Pin_10
#define IN_HOME_3_GPIO_PORT             GPIOB
#define IN_HOME_3_GPIO_CLK              RCC_AHB1Periph_GPIOB 
#define IN_HOME_3_ET_LINE               EXTI_Line10   
#define IN_HOME_3_ET_PORT               EXTI_PortSourceGPIOB
#define IN_HOME_3_ET_PIN                EXTI_PinSource10
//#define IN_HOME_3_ET_IRQn               EXTI15_10_IRQn

// PB11    home oc, motor 4
#define IN_HOME_4_GPIO_PIN              GPIO_Pin_11
#define IN_HOME_4_GPIO_PORT             GPIOB
#define IN_HOME_4_GPIO_CLK              RCC_AHB1Periph_GPIOB 
#define IN_HOME_4_ET_LINE               EXTI_Line11   
#define IN_HOME_4_ET_PORT               EXTI_PortSourceGPIOB
#define IN_HOME_4_ET_PIN                EXTI_PinSource11
//#define IN_HOME_4_ET_IRQn               EXTI15_10_IRQn

// PB12    home oc, motor 5
#define IN_HOME_5_GPIO_PIN              GPIO_Pin_12
#define IN_HOME_5_GPIO_PORT             GPIOB
#define IN_HOME_5_GPIO_CLK              RCC_AHB1Periph_GPIOB 
#define IN_HOME_5_ET_LINE               EXTI_Line12   
#define IN_HOME_5_ET_PORT               EXTI_PortSourceGPIOB
#define IN_HOME_5_ET_PIN                EXTI_PinSource12
//#define IN_HOME_5_ET_IRQn               EXTI15_10_IRQn

// PB13    home oc, motor 6
#define IN_HOME_6_GPIO_PIN              GPIO_Pin_13
#define IN_HOME_6_GPIO_PORT             GPIOB
#define IN_HOME_6_GPIO_CLK              RCC_AHB1Periph_GPIOB 
#define IN_HOME_6_ET_LINE               EXTI_Line13   	
#define IN_HOME_6_ET_PORT               EXTI_PortSourceGPIOB
#define IN_HOME_6_ET_PIN                EXTI_PinSource13
//#define IN_HOME_6_ET_IRQn               EXTI15_10_IRQn

// 2. the motor status oc-detect input
// PG7     from the fpga, to be used as a external interrupt
#define IN_MT_STATUS_GPIO_PIN           GPIO_Pin_7           
#define IN_MT_STATUS_GPIO_PORT          GPIOG
#define IN_MT_STATUS_GPIO_CLK           RCC_AHB1Periph_GPIOG  
#define IN_MT_STATUS_ET_LINE            EXTI_Line7
#define IN_MT_STATUS_ET_PORT            EXTI_PortSourceGPIOG
#define IN_MT_STATUS_ET_PIN             EXTI_PinSource7
#define IN_MT_STATUS_ET_IRQn            EXTI9_5_IRQn


//-----------------------------------------------------------------------------------------
// spi flash, 2 chips
// 
#if 0

// AF pin mapping
#define sFLASH_SPI_SCK_AF                GPIO_AF_SPI1
#define sFLASH_SPI_MISO_AF               GPIO_AF_SPI1
#define sFLASH_SPI_MOSI_AF               GPIO_AF_SPI1
#define sFLASH_SPI_SCK_SOURCE            GPIO_PinSource3  // 3
#define sFLASH_SPI_MISO_SOURCE           GPIO_PinSource4  // 4
#define sFLASH_SPI_MOSI_SOURCE           GPIO_PinSource5  // 5

// SPI channel
#define sFLASH_SPI                       SPI1                // or SPI3  // attention: choose SPI1
#define sFLASH_SPI_CLK                   RCC_APB2Periph_SPI1 // RCC_APB1Periph_SPI3
// PB3   clk
#define sFLASH_SPI_SCK_GPIO_PIN          GPIO_Pin_3                  
#define sFLASH_SPI_SCK_GPIO_PORT         GPIOB                       
#define sFLASH_SPI_SCK_GPIO_CLK          RCC_AHB1Periph_GPIOB
// PB4   miso
#define sFLASH_SPI_MISO_GPIO_PIN         GPIO_Pin_4                 
#define sFLASH_SPI_MISO_GPIO_PORT        GPIOB                       
#define sFLASH_SPI_MISO_GPIO_CLK         RCC_AHB1Periph_GPIOB
// PB5   mosi
#define sFLASH_SPI_MOSI_GPIO_PIN         GPIO_Pin_5                  
#define sFLASH_SPI_MOSI_GPIO_PORT        GPIOB                      
#define sFLASH_SPI_MOSI_GPIO_CLK         RCC_AHB1Periph_GPIOB
// PB6   cs1
#define sFLASH_CS1_GPIO_PIN              GPIO_Pin_7                 
#define sFLASH_CS1_GPIO_PORT             GPIOB                       
#define sFLASH_CS1_GPIO_CLK              RCC_AHB1Periph_GPIOB
// PB7   cs2
#define sFLASH_CS2_GPIO_PIN              GPIO_Pin_6                  
#define sFLASH_CS2_GPIO_PORT             GPIOB                      
#define sFLASH_CS2_GPIO_CLK              RCC_AHB1Periph_GPIOB

#endif

//-----------------------------------------------------------------------------------------
// global variables

// adc1 and2 use dma, adc3 not
typedef enum{
	EN_ADC1 = 0,
	EN_ADC2 = 1,
	EN_ADC3 = 3
} EN_TypeADC;
	
//void APP_ADC_Init(EN_TypeADC eType);
//void APP_ADC_DMA_Config(EN_TypeADC eType);
//void APP_ADC1_Init(void);
//void APP_ADC2_Init(void);
//void APP_ADC3_Init(void);

//-----------------------------------------------------------------------------------------
// total initialization for the code block
extern void EVAL_Init(void);

//-----------------------------------------------------------------------------------------
// com control
static void EVAL_ComInit(Com_TypeDef eCom, USART_InitTypeDef* USART_InitStruct);

//-----------------------------------------------------------------------------------------
// GPIO output contorl 
static void EVAL_OutputInit(Output_TypeDef eGpio);
extern void EVAL_OutputSet(Output_TypeDef eGpio);
extern void EVAL_OutputClr(Output_TypeDef eGpio);
extern void EVAL_OutputToggle(Output_TypeDef eGpio);

//-----------------------------------------------------------------------------------------
// GPIO input contorl
static void EVAL_InputInit(Input_TypeDef eIn, InModel_Typedef eModel);
extern UINT8 EVAL_InputGetState(Input_TypeDef eIn);

//-----------------------------------------------------------------------------------------
// timer interrupt
_EXT_ UINT8  PF_InitTimer2(void);






#endif


