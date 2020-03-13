// Scope_Eval.h

#ifndef    __SCOPE_EVAL_H__
#define    __SCOPE_EVAL_H__


#include "MyType.h"       // ---
#include <stdio.h>        // for "printf" debug
#include "Scope_Move.h"

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
   // E_COM_NULL      = 2,
  
} Com_TypeDef;   





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
#define COM3_RX_GPIO_PORT           GPIOC// GPIOC
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
typedef enum              // take attention of sequence
{
//  O_STATUS_LED_1  = 0,
//  O_STATUS_LED_2  = 1,
    O_MCU_LED_1     = 0,
    O_MCU_LED_2     = 1,
    O_LAN8720_RST   = 2,
	O_MotorX_EN		= 3,
	O_MotorX_DIR	= 4,
	O_MotorX_CLK	= 5,
	O_MotorY_EN		= 6,
	O_MotorY_DIR	= 7,
	O_MotorY_CLK	= 8,
	O_Air_Walve		= 9,
	O_Liquild_Walve = 10,
	O_Beep			= 11,
	O_OUTPUT_NUM	= 12
	
} Output_TypeDef;
//
//#define OUTPUT_NUM                          7   // 4 + 1 + 2, // + 2 + 1

// 1. leds output
// PF7  out_led_1   // status: runing
//#define OUT_STATUS_LED1_GPIO_PIN            GPIO_Pin_7
//#define OUT_STATUS_LED1_GPIO_PORT           GPIOF
//#define OUT_STATUS_LED1_GPIO_CLK            RCC_AHB1Periph_GPIOC 
// PF8  out_led_2   // status: warning
//#define OUT_STATUS_LED2_GPIO_PIN            GPIO_Pin_8
//#define OUT_STATUS_LED2_GPIO_PORT           GPIOF
//#define OUT_STATUS_LED2_GPIO_CLK            RCC_AHB1Periph_GPIOF  
//0. PF9  mcu_led_1   // testing ===> PE4
#define OUT_MCU_LED1_GPIO_PIN               GPIO_Pin_4 //GPIO_Pin_9
#define OUT_MCU_LED1_GPIO_PORT              GPIOE //GPIOF
#define OUT_MCU_LED1_GPIO_CLK               RCC_AHB1Periph_GPIOE  //RCC_AHB1Periph_GPIOF 
//1. PF10 mcu_led_2   // testing ===> PE5
#define OUT_MCU_LED2_GPIO_PIN               GPIO_Pin_3 //GPIO_Pin_5
#define OUT_MCU_LED2_GPIO_PORT              GPIOE //GPIOF
#define OUT_MCU_LED2_GPIO_CLK               RCC_AHB1Periph_GPIOE  //RCC_AHB1Periph_GPIOF 

//2. PD3  the pin to reset the LAN8720
#define OUT_LAN8720_RST_GPIO_PIN            GPIO_Pin_3
#define OUT_LAN8720_RST_GPIO_PORT           GPIOD
#define OUT_LAN8720_RST_GPIO_CLK            RCC_AHB1Periph_GPIOD

//3. PD10 Motor X EN
#define OUT_MotorX_EN_GPIO_PIN            GPIO_Pin_10
#define OUT_MotorX_EN_GPIO_PORT           GPIOD
#define OUT_MotorX_EN_GPIO_CLK            RCC_AHB1Periph_GPIOD

//4. PD9 Motor X DIR
#define OUT_MotorX_DIR_GPIO_PIN            GPIO_Pin_9
#define OUT_MotorX_DIR_GPIO_PORT           GPIOD
#define OUT_MotorX_DIR_GPIO_CLK            RCC_AHB1Periph_GPIOD

//5. PC8 Motor X CLK
#define OUT_MotorX_CLK_GPIO_PIN            GPIO_Pin_8
#define OUT_MotorX_CLK_GPIO_PORT           GPIOC
#define OUT_MotorX_CLK_GPIO_CLK            RCC_AHB1Periph_GPIOC

//6. PD7 Motor Y EN
#define OUT_MotorY_EN_GPIO_PIN            GPIO_Pin_7
#define OUT_MotorY_EN_GPIO_PORT           GPIOD
#define OUT_MotorY_EN_GPIO_CLK            RCC_AHB1Periph_GPIOD

//7. PD0 Motor Y DIR
#define OUT_MotorY_DIR_GPIO_PIN            GPIO_Pin_0
#define OUT_MotorY_DIR_GPIO_PORT           GPIOD
#define OUT_MotorY_DIR_GPIO_CLK            RCC_AHB1Periph_GPIOD

//8. PD14 Motor Y CLK
#define OUT_MotorY_CLK_GPIO_PIN            GPIO_Pin_14
#define OUT_MotorY_CLK_GPIO_PORT           GPIOD
#define OUT_MotorY_CLK_GPIO_CLK            RCC_AHB1Periph_GPIOD

//9. PF11 Air Walve
#define OUT_VALVE_AIR_PORT					GPIOF
#define OUT_VALVE_AIR_PIN					GPIO_Pin_11
#define OUT_VALVE_AIR_CLK    				RCC_AHB1Periph_GPIOF

// 10. PH6 Liquild Walve
#define OUT_VALVE_LIQUID_PORT				GPIOH
#define OUT_VALVE_LIQUID_PIN				GPIO_Pin_6
#define OUT_VALVE_LIQUID_CLK				RCC_AHB1Periph_GPIOH

// 11. PD6 Liquild Walve
#define OUT_BEEP_PORT						GPIOD
#define OUT_BEEP_LIQUID_PIN					GPIO_Pin_6
#define OUT_BEEP_LIQUID_CLK					RCC_AHB1Periph_GPIOD

//-----------------------------------------------------------------------------------------
typedef enum 
{
    // for homing, common input
//    I_HOME_X       = 0,
//    I_HOME_Y       = 1,
//    I_HOME_Z       = 2,
//    I_HOME_M       = 3,
//    I_HOME_5       = 4,
//    I_HOME_6       = 5,
	
    // the motor status, to be used as a extinal-interrupt, (PG7) use for cx2000 A and B version to get motor finished flag
    I_FEEDBACK_1   	= 0, 
	I_MotorX_IN_OC	= 1,
	I_MotorX_OUT_OC	= 2,
	I_MotorY_IN_OC	= 3,
	I_MotorY_OUT_OC	= 4,
	I_ELEC			= 5,
	I_INPUT_NUM	    = 6
} Input_TypeDef;  

//
typedef enum
{
    IN_MODEL_GPIO = 0,
	IN_MODEL_EXTI = 1,	

} InModel_Typedef;


// 2. the motor status oc-detect input
// 0. PG7     from the fpga, to be used as a external interrupt
#define IN_MT_STATUS_GPIO_PIN           GPIO_Pin_7           
#define IN_MT_STATUS_GPIO_PORT          GPIOG
#define IN_MT_STATUS_GPIO_CLK           RCC_AHB1Periph_GPIOG  
#define IN_MT_STATUS_ET_LINE            EXTI_Line7
#define IN_MT_STATUS_ET_PORT            EXTI_PortSourceGPIOG
#define IN_MT_STATUS_ET_PIN             EXTI_PinSource7
#define IN_MT_STATUS_ET_IRQn            EXTI9_5_IRQn


// 1. PG2 Motor X IN OC, so far, not use interrupt function at OC
#define IN_MotorX_IN_OC_GPIO_PIN           GPIO_Pin_2          
#define IN_MotorX_IN_OC_GPIO_PORT          GPIOG
#define IN_MotorX_IN_OC_GPIO_CLK           RCC_AHB1Periph_GPIOG  
#define IN_MotorX_IN_OC_ET_LINE            EXTI_Line2
#define IN_MotorX_IN_OC_ET_PORT            EXTI_PortSourceGPIOG
#define IN_MotorX_IN_OC_ET_PIN             EXTI_PinSource2
#define IN_MotorX_IN_OC_ET_IRQn            EXTI2_IRQn

// 2. PG3 Motor X OUT OC
#define IN_MotorX_OUT_OC_GPIO_PIN           GPIO_Pin_3          
#define IN_MotorX_OUT_OC_GPIO_PORT          GPIOG
#define IN_MotorX_OUT_OC_GPIO_CLK           RCC_AHB1Periph_GPIOG  
#define IN_MotorX_OUT_OC_ET_LINE            EXTI_Line3
#define IN_MotorX_OUT_OC_ET_PORT            EXTI_PortSourceGPIOG
#define IN_MotorX_OUT_OC_ET_PIN             EXTI_PinSource3
#define IN_MotorX_OUT_OC_ET_IRQn            EXTI3_IRQn

// 3. PG4 Motor Y IN OC
#define IN_MotorY_IN_OC_GPIO_PIN           GPIO_Pin_4          
#define IN_MotorY_IN_OC_GPIO_PORT          GPIOG
#define IN_MotorY_IN_OC_GPIO_CLK           RCC_AHB1Periph_GPIOG  
#define IN_MotorY_IN_OC_ET_LINE            EXTI_Line4
#define IN_MotorY_IN_OC_ET_PORT            EXTI_PortSourceGPIOG
#define IN_MotorY_IN_OC_ET_PIN             EXTI_PinSource4
#define IN_MotorY_IN_OC_ET_IRQn            EXTI4_IRQn

// 4. PG5 Motor Y OUT OC
#define IN_MotorY_OUT_OC_GPIO_PIN           GPIO_Pin_5          
#define IN_MotorY_OUT_OC_GPIO_PORT          GPIOG
#define IN_MotorY_OUT_OC_GPIO_CLK           RCC_AHB1Periph_GPIOG  
#define IN_MotorY_OUT_OC_ET_LINE            EXTI_Line5
#define IN_MotorY_OUT_OC_ET_PORT            EXTI_PortSourceGPIOG
#define IN_MotorY_OUT_OC_ET_PIN             EXTI_PinSource5
#define IN_MotorY_OUT_OC_ET_IRQn            EXTI9_5_IRQn
// 5. Elec 
#define IN_ELEC_GPIO_PIN           			GPIO_Pin_11          
#define IN_ELEC_GPIO_PORT          			GPIOA
#define IN_ELEC_GPIO_CLK           			RCC_AHB1Periph_GPIOA
#define IN_ELEC_ET_LINE            			EXTI_Line11
#define IN_ELEC_ET_PORT            			EXTI_PortSourceGPIOA
#define IN_ELEC_ET_PIN             			EXTI_PinSource11
#define IN_ELEC_ET_IRQn            			EXTI15_10_IRQn


// adc1 and2 use dma, adc3 not
typedef enum{
	EN_ADC1 = 0,
	EN_ADC2 = 1,
	EN_ADC3 = 3
} EN_TypeADC;

#define ADC2_CHECK_NUM			2
#define ADC3_INIT_WITH_DMA		0
#define ADC_SMOOTH_NUM_5	5
#define ADC_SMOOTH_NUM_10	10
#define ADC_SMOOTH_NUM_20	20
#define ADC_SMOOTH_NUM_30	30

enum{
	EN_ADC_PRESS	= 0,
	EN_ADC_XK		= 1,
	EN_ADC_12V_N	= 2,
	EN_ADC_12V_P	= 3,
	EN_ADC_56V_CUR	= 4,
	EN_ADC_ELEC		= 5,
	EN_ADC_LED_CUR	= 6,
	EN_ADC_HGB		= 7,
	EN_ADC_CRP		= 8,
	EN_ADC_TEMP		= 9,
	EN_ADC_END		= 10,
};

extern IO_ UINT16 g_ADC3_Value[EN_ADC_END];

// XK ADC, PC3_ADC3_IN13	
#define XK_ADC_PORT							GPIOC
#define XK_ADC_PIN							GPIO_Pin_3
#define XK_ADC_SRC							RCC_AHB1Periph_GPIOC
#define XK_ADC_CHANNEL						ADC_Channel_13

//  PF9_ADC3_IN7, 56V_CUR
#define CUR_56V_ADC_PORT					GPIOF
#define CUR_56V_ADC_PIN						GPIO_Pin_9
#define CUR_56V_ADC_SRC						RCC_AHB1Periph_GPIOF
#define CUR_56V_ADC_CHANNEL					ADC_Channel_7

// PC0_ADC123_IN10, Temperature
#define TEMP_ADC_PORT						GPIOC
#define TEMP_ADC_PIN						GPIO_Pin_0
#define TEMP_ADC_SRC						RCC_AHB1Periph_GPIOC
#define TEMP_ADC_CHANNEL					ADC_Channel_10

// PC2_ADC123_IN12 , Press
//#if PRESS_SENSOR_ADC_TYPE
	#define PRESS_ADC_PORT					GPIOC
	#define PRESS_ADC_PIN					GPIO_Pin_2
	#define PRESS_ADC_SRC					RCC_AHB1Periph_GPIOC
	#define PRESS_ADC_CHANNEL				ADC_Channel_12
//#endif

// Optical path Signal acquisiton ADC,   SIG1 ==> PF7_ADC3_IN5 ,CRP
#define SIG1_ADC_PORT						GPIOF
#define SIG1_ADC_PIN						GPIO_Pin_7
#define SIG1_ADC_SRC						RCC_AHB1Periph_GPIOF
#define SIG1_ADC_CHANNEL					ADC_Channel_5

// Optical path Signal acquisiton ADC,   SIG2 ==> PA0_ADC3_IN0 ,HGB
#define SIG2_ADC_PORT						GPIOA
#define SIG2_ADC_PIN						GPIO_Pin_0
#define SIG2_ADC_SRC						RCC_AHB1Periph_GPIOA
#define SIG2_ADC_CHANNEL					ADC_Channel_0

// 12V N
#define CUR12N_ADC_PORT						GPIOA
#define CUR12N_ADC_PIN						GPIO_Pin_3
#define CUR12N_ADC_SRC						RCC_AHB1Periph_GPIOA
#define CUR12N_ADC_CHANNEL					ADC_Channel_3
// 12V P
#define CUR12P_ADC_PORT						GPIOF
#define CUR12P_ADC_PIN						GPIO_Pin_10
#define CUR12P_ADC_SRC						RCC_AHB1Periph_GPIOF
#define CUR12P_ADC_CHANNEL					ADC_Channel_10

// LED Cur ADC, PF6_ADC3_CH4
#define LED_CUR_ADC_PORT					GPIOF
#define LED_CUR_ADC_PIN						GPIO_Pin_6
#define LED_CUR_ADC_SRC						RCC_AHB1Periph_GPIOF
#define LED_CUR_ADC_CHANNEL					ADC_Channel_4
// elec ADC3_IN6 PF8
#define ELEC_ADC_PORT							GPIOF
#define ELEC_ADC_PIN							GPIO_Pin_8
#define ELEC_ADC_SRC							RCC_AHB1Periph_GPIOF
#define ELEC_ADC_CHANNEL						ADC_Channel_6

static IO_ UINT8 g_ADC3_IN[EN_ADC_END] = \
	{PRESS_ADC_CHANNEL,   XK_ADC_CHANNEL,	   CUR12N_ADC_CHANNEL, \
	 CUR12P_ADC_CHANNEL,  CUR_56V_ADC_CHANNEL, ELEC_ADC_CHANNEL, \
	 LED_CUR_ADC_CHANNEL, SIG2_ADC_CHANNEL,    SIG1_ADC_CHANNEL, \
	 TEMP_ADC_CHANNEL};



void ADC1_Init(void);
void ADC2_Init(void);
void ADC3_Init(void);
	 

//UINT16 Get_Press(void);
UINT16 Get_ADC3_Channel_Value(UINT8 nIndex, UINT8 nCount);
UINT16 HW_Press_ADC(void);
UINT16 Get_XK_ADC(void);
UINT16 Get_XK_V(void);
UINT16 Get_12V_N_ADC(void);
UINT16 Get_12V_P_ADC(void);
UINT16 Get_56V_Cur_ADC(void);
UINT16 Get_56V_Cur_V(void);
//UINT16 Get_LED_Cur_ADC(void);
UINT16 Get_Temp_ADC(void);	 
UINT32  HW_Get_ADC_HGB(void);
UINT32  HW_Get_ADC_CRP(void);
	 
UINT16 HW_Elec_ADC(void);
UINT16 HW_Elec_V(void);



// total initialization for the code block
extern void EVAL_Init(void);
// com control
static void EVAL_ComInit(Com_TypeDef eCom, USART_InitTypeDef* USART_InitStruct);
// GPIO output contorl 
static void EVAL_OutputInit(Output_TypeDef eGpio);
extern void EVAL_OutputSet(Output_TypeDef eGpio);
extern void EVAL_OutputClr(Output_TypeDef eGpio);
extern void EVAL_OutputToggle(Output_TypeDef eGpio);
// GPIO input contorl
static void EVAL_InputInit(Input_TypeDef eIn, InModel_Typedef eModel);
extern UINT8 EVAL_InputGetState(Input_TypeDef eIn);

// Press IIC APi
// press check, PH4_SCl, PH5_SDA I2C, I2C
// PH4
#define PRESS_I2C_SCL_PORT					GPIOH
#define PRESS_I2C_SCL_PIN					GPIO_Pin_4
#define PRESS_I2C_SCL_SCLCLK_SRC			RCC_AHB1Periph_GPIOH
// PH5
#define PRESS_I2C_SDA_PORT					GPIOH
#define PRESS_I2C_SDA_PIN					GPIO_Pin_5
#define PRESS_I2C_SDA_SCLCLK_SRC			RCC_AHB1Periph_GPIOH
//
// PB1 EOC
#define PRESS_I2C_EOC_PORT					GPIOB
#define PRESS_I2C_EOC_PIN					GPIO_Pin_1
#define PRESS_I2C_EOC_SRC					RCC_AHB1Periph_GPIOB

#define PRESS_I2C_SDA_IN()  				{PRESS_I2C_SDA_PORT->MODER&=~(3<<(5*2));PRESS_I2C_SDA_PORT->MODER|=0<<5*2;}	//PB9输入模式
#define PRESS_I2C_SDA_OUT() 				{PRESS_I2C_SDA_PORT->MODER&=~(3<<(5*2));PRESS_I2C_SDA_PORT->MODER|=1<<5*2;} //PB9输出模式
#define PRESS_I2C_EOC_STATUS				GPIO_ReadInputDataBit(PRESS_I2C_EOC_PORT, PRESS_I2C_EOC_PIN)
//

#define PRESS_I2C_SCL    					 PHout(4) //SCL 
#define PRESS_I2C_SDA   	 				 PHout(5) //SDA	 
#define PRESS_I2C_READ_SDA  				 PHin(5)  //SDA 
//
//void Press_Init(void);
void Press_I2C_Init(void);
void Press_I2C_Start(void);
void Press_I2C_Stop(void);
UINT8 Press_I2C_Wait_Ack(void);
void Press_I2C_Ack(void);
void Press_I2C_NAck(void);
void Press_I2C_Send_Byte(UINT8 nVal);
UINT8 Press_I2C_Read_Byte(UINT8 nAck);
INT32 HW_Press_I2C(void);
void HW_Press_Init(void);


// Pump  TIM9_PE5_CH1 
#define PUMP_CLK_PORT						GPIOE
#define PUMP_CLK_PIN						GPIO_Pin_5
#define PUMP_CLK_SRC						RCC_AHB1Periph_GPIOE
#define PUMP_CLK_PIN_AF						GPIO_PinSource5
#define PUMP_CLK_PORT_AF					GPIO_AF_TIM9
#define PUMP_PWM_TIM						TIM9
#define PUMP_PWM_TIM_SRC					RCC_APB2Periph_TIM9  
//#define PUMP_PWM_TIM_ARR					24999 //25000
//#define PUMP_PWM_TIM_PSC					41    //42
//#define PUMP_PWM_LEVEL_CLOSE				0
//#define PUMP_PWM_LEVEL_BEST				10000
//#define PUMP_PWM_LEVEL_HIGHEST			25000
void HW_PUMP_Init(void);


// --------- cx3000 Digital Register(SPI2), PI1_CLK,PI3_MOSI,PI0_CS 
#define D_REGISTER_CLK_PORT					GPIOI
#define D_REGISTER_CLK_PIN					GPIO_Pin_1
#define D_REGISTER_CLK_SRC					RCC_AHB1Periph_GPIOI
#define D_REGISTER_CLK_AF_SRC 				GPIO_PinSource1
// MOSI
#define D_REGISTER_MOSI_PORT				GPIOI
#define D_REGISTER_MOSI_PIN					GPIO_Pin_3
#define D_REGISTER_MOSI_SRC					RCC_AHB1Periph_GPIOI
#define D_REGISTER_MOSI_AF_SRC 				GPIO_PinSource3
// CS
#define D_REGISTER_CS_PORT					GPIOI
#define D_REGISTER_CS_PIN					GPIO_Pin_0
#define D_REGISTER_CS_SRC					RCC_AHB1Periph_GPIOI
#define D_REGISTER_CS_AF_SRC 				GPIO_PinSource0
//// MISO Digital Register is 3lines SP, not need MISO
//#define D_REGISTER_MISO_PORT				GPIOI
//#define D_REGISTER_MISO_PIN				GPIO_Pin_2
//#define D_REGISTER_MISO_SRC				RCC_AHB1Periph_GPIOI
//#define D_REGISTER_MISO_AF_SRC 			GPIO_PinSource2
//
#define D_REGISTER_SPI						SPI2
#define D_REGISTER_SPI_SRC 					RCC_APB1Periph_SPI2
#define D_REGISTER_SPI_AF 					GPIO_AF_SPI2

#define DREGISTER_CLK_1()	GPIO_SetBits(D_REGISTER_CLK_PORT, D_REGISTER_CLK_PIN)
#define DREGISTER_CLK_0()	GPIO_ResetBits(D_REGISTER_CLK_PORT, D_REGISTER_CLK_PIN)
#define DREGISTER_CS_1()	GPIO_SetBits(D_REGISTER_CS_PORT, D_REGISTER_CS_PIN)
#define DREGISTER_CS_0()	GPIO_ResetBits(D_REGISTER_CS_PORT, D_REGISTER_CS_PIN)
#define DREGISTER_MOSI_1()	GPIO_SetBits(D_REGISTER_MOSI_PORT, D_REGISTER_MOSI_PIN)
#define DREGISTER_MOSI_0()	GPIO_ResetBits(D_REGISTER_MOSI_PORT, D_REGISTER_MOSI_PIN)
// 
enum {
	EN_DRESISTOR_CHAN0 = 0,
	EN_DRESISTOR_CHAN1 = 1,
	EN_DRESISTOR_CHAN2 = 2,
	EN_DRESISTOR_CHAN3 = 3,
};
#define DREGISTER_DATA_LEN	10
void HW_ADJ_Resistor_Init(void);




//-----------------------------------------------------------------------------------------
void Delay_US(UINT32 us);

// timer interrupt
_EXT_ UINT8  PF_InitTimer2(void);
// Motor timer
UINT8 PF_InitTimer3(void); // Motor X
UINT8 PF_InitTimer4(void); // Motor Y

#define IRQ_MotorX          TIM3_IRQHandler
#define IRQ_MotorY          TIM4_IRQHandler

// function declaration, using the timer's interrupt
TIM_TypeDef*  CODE_ MTx_Timer[Motor_End] = {TIM3, TIM4};
// 3.timer control
// 1) timer init
//#define MTx_TIMER_INIT(ch)               {PF_InitMotorTimer(ch);}
// 2) enable the interrupt 
#define MTx_TIMER_INTERRUPT_ON(ch)         {TIM_ITConfig(MTx_Timer[ch], TIM_IT_Update, ENABLE);}
// 3) disable the interrupt
#define MTx_TIMER_INTERRUPT_OFF(ch)        {TIM_ITConfig(MTx_Timer[ch], TIM_IT_Update, DISABLE);}
// 4) clean the flag of interrupt
#define MTx_TIMER_INTERRUPT_FLAG_CLEAR(ch) {TIM_ClearITPendingBit(MTx_Timer[ch] , TIM_FLAG_Update);}
// 5) counting is on 
#define MTx_TIMER_COUNT_ON(ch)             {TIM_Cmd(MTx_Timer[ch], ENABLE);} 
// 6) counting is off
#define MTx_TIMER_COUNT_OFF(ch)            {TIM_Cmd(MTx_Timer[ch], DISABLE);}
// 7) change the reloaded value and don't effect the counter's value
#define MTx_TIMER_LOAD(ch, VALUE)          {TIM_SetAutoreload(MTx_Timer[ch], (VALUE));} // {TIM_SetCounter(MTx_Timer[ch], (VALUE));}  

// timer3,4 irq handler
void MTx_IoMinitor(enum eMvMotor eMotor);
void PF_InitMotorTimer(enum eMvMotor eMotor);
void InitMotor_IO(enum eMvMotor eMotor);

void Motor_Dir_Pos(enum eMvMotor eMotor); // in
void Motor_Dir_Neg(enum eMvMotor eMotor); // out
void Motor_Enable(enum eMvMotor eMotor);
void Motor_Disable(enum eMvMotor eMotor);
void Motor_Clk_Set(enum eMvMotor eMotor);
void Motor_Clk_Reset(enum eMvMotor eMotor);


void PF_InitMotor(enum eMvMotor eMotor);
void HW_LEVEL_OC_Init(void);
void HW_Valve_Init(void);
UINT8  HW_PUMP_Pulse_V3(UINT32 nFreq, enum eDirection eDir);
void HW_ELEC_Init(void);

void HW_Beep_Init(void);
void Beep(UINT8 nNum, UINT32 nTime);













#endif
