// Scope_Eval.h

#ifndef    __DRIVER_H__
#define    __DRIVER_H__

#include "MyType.h"       // ---
#include <stdio.h>        // for "printf" debug


#define PRESS_SENSOR_ADC_TYPE		0
//#if PRESS_SENSOR_ADC_TYPE
//	#define ADC3_CHECK_NUM			7
//#else
//	#define ADC3_CHECK_NUM			6
//#endif

#define ADC2_CHECK_NUM			2
#define ADC3_INIT_WITH_DMA		0
extern IO_ UINT8 g_Elec_Status;

void Delay_US(UINT32 us);


//---------------GPIO OUT------------
typedef enum              // take attention of sequence
{
    O_MCU_LED_1     		= 0,
    O_MCU_LED_2     		= 1,
    O_LAN8720_RST  			= 2,
	O_OUTIN_MOTOR_EN	 	= 3,
	O_OUTIN_MOTOR_DIR 		= 4,
	O_TURN_MOTOR_EN	 		= 5,
	O_TURN_MOTOR_DIR 		= 6,	
	O_COUNTER_ADJUST		= 7,
	OUTPUT_NUM      		= 8,
	
} Output_TypeDef;

// net rst
#define OUT_LAN8720_RST_GPIO_PIN        GPIO_Pin_3
#define OUT_LAN8720_RST_GPIO_PORT       GPIOD
#define OUT_LAN8720_RST_GPIO_CLK        RCC_AHB1Periph_GPIOD

// PE4  mcu_led_1 
#define OUT_MCU_LED1_GPIO_PIN           GPIO_Pin_4 
#define OUT_MCU_LED1_GPIO_PORT          GPIOE 
#define OUT_MCU_LED1_GPIO_CLK           RCC_AHB1Periph_GPIOE 
// PE5 mcu_led_2   
#define OUT_MCU_LED2_GPIO_PIN           GPIO_Pin_5 
#define OUT_MCU_LED2_GPIO_PORT          GPIOE 
#define OUT_MCU_LED2_GPIO_CLK           RCC_AHB1Periph_GPIOE 

// led and speaker
void EVAL_OutputInit(Output_TypeDef eOut);
void EVAL_OutputInit_All(void);
void EVAL_OutputSet(Output_TypeDef eOut);
void EVAL_OutputClr(Output_TypeDef eOut);
void EVAL_OutputToggle(Output_TypeDef eOut);

// 1 is cover, 0 is empty
void OC_Init(void);
UINT8 Get_Out_OC_Status(void);
UINT8 Get_In_OC_Status(void);

//UINT8 Get_Turn_Select_OC_Status(void); 
//UINT8 Get_Turn_Reset_OC_Status(void); 
//UINT8 Get_Out_OC_Status(void);
//UINT8 Get_In_OC_Status(void);


//---------------GPIO IN------------
// --------- OC for turn motor reset, and than select LED, PA12
#define TURN_RESET_OC_GPIO_PORT			GPIOA
#define TURN_RESET_OC_GPIO_PIN			GPIO_Pin_12
#define TURN_RESET_OC_GPIO_SRC			RCC_AHB1Periph_GPIOA

// OC LED select, PG6
#define TURN_SELECT_OC_GPIO_PORT		GPIOG
#define TURN_SELECT_OC_GPIO_PIN			GPIO_Pin_6
#define TURN_SELECT_OC_GPIO_SRC			RCC_AHB1Periph_GPIOG

// OC for in (cx3000), PC13
#define IN_OC_GPIO_PORT					GPIOC
#define IN_OC_GPIO_PIN					GPIO_Pin_13
#define IN_OC_GPIO_SRC					RCC_AHB1Periph_GPIOC

// OC  for Out(cx3000), PG9
#define OUT_OC_GPIO_PORT				GPIOG
#define OUT_OC_GPIO_PIN					GPIO_Pin_9
#define OUT_OC_GPIO_SRC					RCC_AHB1Periph_GPIOG


// PD8, check counter exist or not
#define COUNTER_CHECK_GPIO_PORT				GPIOD
#define COUNTER_CHECK_GPIO_PIN				GPIO_Pin_8
#define COUNTER_CHECK_GPIO_SRC				RCC_AHB1Periph_GPIOD
void Counter_Check_Init(void);
UINT8 Get_Counter_Check_Status(void);

typedef enum 
{
    I_LED_OC_RST       = 0,
    I_LED_OC_SEL       = 1,
    I_MOTOR_OC_IN      = 2,
    I_MOTOR_OC_OUT     = 3,
	I_COUNTER_CHECK	   = 4,
	INPUT_NUM     	   = 5,

} Input_TypeDef;  

void EVAL_InputInit(Input_TypeDef eIn);
void EVAL_InputInit_All(void);
UINT8 EVAL_GetInputStatus(Input_TypeDef eIn);




// Timer 2
UINT8  PF_InitTimer2(void);

// TIM3_CH3,     Out_In_Motor, PD8_clk(TIM3_CH3), PD9_Dir, PD10_Enable
// PD10_Enalb
#define OUTIN_MOTOR_EN_PORT				GPIOD
#define OUTIN_MOTOR_EN_PIN				GPIO_Pin_10
#define OUTIN_MOTOR_EN_SRC				RCC_AHB1Periph_GPIOD
// PD9_Dir
#define OUTIN_MOTOR_DIR_PORT			GPIOD
#define OUTIN_MOTOR_DIR_PIN 			GPIO_Pin_9
#define OUTIN_MOTOR_DIR_SRC				RCC_AHB1Periph_GPIOD
// PC8_Clk
#define OUTIN_MOTOR_CLK_PORT			GPIOC
#define OUTIN_MOTOR_CLK_PIN				GPIO_Pin_8
#define OUTIN_MOTOR_CLK_SRC				RCC_AHB1Periph_GPIOC
// TIM3_CH3
#define OUTIN_MOTOR_CLK_PIN_AF			GPIO_PinSource8
#define OUTIN_MOTOR_CLK_PORT_AF			GPIO_AF_TIM3
#define OUTIN_MOTOR_PWM_TIM				TIM3
#define OUTIN_MOTOR_PWM_TIM_SRC			RCC_APB1Periph_TIM3
#define OUTIN_MOTOR_PWM_TIM_ARR			800 //25000
#define OUTIN_MOTOR_PWM_TIM_PSC			11   //42    //84M/12=4M, 
#define OUTIN_MOTOR_PWM_LEVEL_CLOSE		0
#define OUTIN_MOTOR_PWM_LEVEL_BEST		400
#define OUTIN_MOTOR_PWM_LEVEL_HIGHEST	OUTIN_MOTOR_PWM_TIM_ARR
#define OUTIN_MOTOR_USE_PWM				1
#define	OUTIN_MOTOR_HOME_TIME			10000
// Time3_CH3  Out_In_Motor clk
void OutIn_Motor_PWM_Init(UINT32 Arr, UINT32 Psc);
// Out_In Motor
void OutIn_Motor_Init(void);
void OutIn_Motor_Speed_Set(UINT16 nSpeed);
void OutIn_Motor_Run(UINT8 nDir, UINT16 nFreq); //e_Dir_Pos=in, e_Dir_Neg=out
void OutIn_Motor_Enable(void);
void OutIn_Motor_Disable(void);
void OutIn_Motor_Out(void); //out
void OutIn_Motor_In(void); // in




// uart3 debug PA9, PA10  
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

void Debug_Uart_Init(void);
void COM_Init(void);


// elec switch PA11 (9-5 for micro switch)
#define ELEC_PORT							GPIOA
#define ELEC_PIN							GPIO_Pin_11
#define ELEC_SRC							RCC_AHB1Periph_GPIOA
#define ELEC_EXIT_LINE 						EXTI_Line11
#define ELEC_EXIT_SRC						EXTI_PortSourceGPIOA
#define ELEC_EXIT_PIN						EXTI_PinSource11
#define ELEC_EXIT_NUM						EXTI15_10_IRQn
//#define ELEC_EXIT_FUNC					EXTI15_10_IRQHandler
#define ELEC_READ							GPIO_ReadInputDataBit(ELEC_PORT, ELEC_PIN)
// elec ADC3_IN6 PF8
#define ELEC_ADC_PORT							GPIOF
#define ELEC_ADC_PIN							GPIO_Pin_8
#define ELEC_ADC_SRC							RCC_AHB1Periph_GPIOF
#define ELEC_ADC_CHANNEL						ADC_Channel_6
// elec
#define ELEC_USE_EXIT_MODE		0
UINT16 Get_Elec_ADC(void);
void Reset_Elec_Status(void);
void Set_Elec_Status(void);
UINT8 Get_Elec_Status(void);
void Elec_Init(void);


//beep PE3	
#define BEEP_PORT							GPIOE
#define BEEP_PIN							GPIO_Pin_3
#define BEEP_SRC							RCC_AHB1Periph_GPIOE
void Beep_Init(void);
void Beep(UINT8 nNo, UINT16 nDelay);


// Pump PD13_CLK(TIM4_CH3), PD4_DIR
#define PUMP_CLK_PORT						GPIOD
#define PUMP_CLK_PIN						GPIO_Pin_13
#define PUMP_CLK_SRC						RCC_AHB1Periph_GPIOD
#define PUMP_CLK_PIN_AF						GPIO_PinSource13
#define PUMP_CLK_PORT_AF					GPIO_AF_TIM4
#define PUMP_PWM_TIM						TIM4
#define PUMP_PWM_TIM_SRC					RCC_APB1Periph_TIM4	
#define PUMP_PWM_TIM_ARR					24999 //25000
#define PUMP_PWM_TIM_PSC					41    //42
#define PUMP_PWM_LEVEL_CLOSE				0
#define PUMP_PWM_LEVEL_BEST					20000
#define PUMP_PWM_LEVEL_HIGHEST				PUMP_PWM_TIM_ARR
//
//#define PUMP_DIR_PORT						GPIOD
//#define PUMP_DIR_PIN						GPIO_Pin_4
//#define PUMP_DIR_SRC						RCC_AHB1Periph_GPIOD
//
void Pump_init(void);
void Pump_PWM_Init(UINT32 Arr, UINT32 Psc);
//void TIM4_PWM_Init(UINT32 Arr, UINT32 Psc);
void Pump_Speed_Set(UINT16 nSpeed);		// dir is invable
void Pump_AntiClockWise(void);
void Pump_ClockWise(void);
void Pump_Exec(UINT8 nDir, UINT16 nFreq);

// mixing motor, PB10
#define MIXING_DIR_PORT						GPIOB
#define MIXING_DIR_PIN						GPIO_Pin_10
#define MIXING_DIR_SRC						RCC_AHB1Periph_GPIOB
// mixing motor
void Mixing_Motor_Init(void);
void Mixing_Motor_Run(void);
void Mixing_Motor_Stop(void);

// --------- OC for micro switch(cx3000), PD6
#define MICRO_OC_PORT					GPIOD
#define MICRO_OC_PIN					GPIO_Pin_6
#define MICRO_OC_SRC					RCC_AHB1Periph_GPIOD
#define MICRO_OC_EXIT_PORT				EXTI_PortSourceGPIOD
#define MICRO_OC_EXIT_PIN				EXTI_PinSource6
#define MICRO_OC_EXIT_LINE				EXTI_Line6
#if USE_STM32F407_ONLY
#define MICRO_OC_EXIT_FUNC				EXTI9_5_IRQHandler
#endif
#define MICRO_OC_EXIT_IRQ				EXTI9_5_IRQn
void Micro_OC_Init(void);
UINT8 Get_Micro_OC_Status(void);
_EXT_ IO_ UINT8 g_Micro_Switch;



//switch 1_PF11, 2_PH6
#define VALVE_AIR_PORT						GPIOF
#define VALVE_AIR_PIN						GPIO_Pin_11
#define VALVE_AIR_SRC						RCC_AHB1Periph_GPIOF
//
#define VALVE_LIQUID_PORT					GPIOH
#define VALVE_LIQUID_PIN					GPIO_Pin_6
#define VALVE_LIQUID_SRC					RCC_AHB1Periph_GPIOH
// valve
void Valve_Init(void);
void Valve_Air_Exec(UINT8 nOpt);
void Valve_Liquid_Exec(UINT8 nOpt);
void Valve_Exec(UINT8 nIndex, UINT8 nOpt);



///////////////////////////////////////////////////////
//// Turn Motor
//#define STEP_NUMBER  0x03
//// turn motor, PB12, PB13, PB14, PB15
//#define TURN_MOTOR_PORT_1					GPIOB
//#define TURN_MOTOR_PIN_1					GPIO_Pin_12
//#define TURN_MOTOR_SRC_1					RCC_AHB1Periph_GPIOB

//#define TURN_MOTOR_PORT_2					GPIOB
//#define TURN_MOTOR_PIN_2					GPIO_Pin_13
//#define TURN_MOTOR_SRC_2					RCC_AHB1Periph_GPIOB

//#define TURN_MOTOR_PORT_3					GPIOB
//#define TURN_MOTOR_PIN_3					GPIO_Pin_14
//#define TURN_MOTOR_SRC_3					RCC_AHB1Periph_GPIOB

//#define TURN_MOTOR_PORT_4					GPIOB
//#define TURN_MOTOR_PIN_4					GPIO_Pin_15
//#define TURN_MOTOR_SRC_4					RCC_AHB1Periph_GPIOB
//#define TURN_MOTOR_PORT						GPIOB

//#define TURN_MOTOR_POWER_PORT				GPIOH
//#define TURN_MOTOR_POWER_PIN				GPIO_Pin_9
//#define TURN_MOTOR_POWER_SRC				RCC_AHB1Periph_GPIOH

//#define	OUTIN_MOTOR_HOME_TIME					10000
//// turn motor 
//#define TURN_MOTOR_MAX_ANTI_CLOCKWISE_STEP		500
//#define TURN_MOTOR_MAX_CLOCKWISE_STEP			500
//#define TURN_MOTOR_MAX_DELAY					4000
//#define TURN_MOTOR_MIN_DELAY					3000//3000


//enum{
//	EN_LED0 = 0,
//	EN_LED1 = 1,
//	EN_LED2 = 2,
//	EN_LED3 = 3,
//	EN_LED4 = 4,
//	EN_LED5 = 5,
//	EN_LED6 = 6,
//	EN_LED7 = 7,
//	EN_LED_END = 8,
//};

//#define HGB_LED_INDEX	EN_LED6
//#define CRP_LED_INDEX   EN_LED3

//enum{
//	EN_POSITION_LED0 = 0,
//	EN_POSITION_LED1 = 1,
//	EN_POSITION_LED2 = 2,
//	EN_POSITION_LED3 = 3,
//	EN_POSITION_LED4 = 4,
//	EN_POSITION_LED5 = 5,
//	EN_POSITION_LED6 = 6,
//	EN_POSITION_LED7 = 7,
//	EN_POSITION_LED_RESET = 8,
//	EN_POSITION_LED_UNSURE = 9
//};

//// form reset postion to select position
//enum {
//	EN_LED0_SELECT_STEP		= 0,	
//	EN_LED1_SELECT_STEP		= 0,
//	EN_LED2_SELECT_STEP		= 70,
//	EN_LED3_SELECT_STEP		= 4,
//	EN_LED4_SELECT_STEP		= 62,
//	EN_LED5_SELECT_STEP		= 134,
//	EN_LED6_SELECT_STEP		= 203,
//	EN_LED7_SELECT_STEP		= 138,
//};
//#define LED_SELETCT_STEP_DIFF  5



//// turn motor
////void Turn_Motor_Init(void);
//void Turn_Motor_Break(void);
//void Turn_Motor_Free(void);
//UINT8 Turn_Motor_Reset(void);
//UINT8 Turn_Motor_ClockWise(UINT32 nStep);
//UINT8 Turn_Motor_Anti_ClockWise(UINT32 nStep);
//void Turn_Motor_Goto_Postion(UINT8 nOpt, UINT32 nStep);
//void Turn_Motor_Select_LED(UINT8 nIndex);
//void Turn_Motor_Power(UINT8 nOpt);
///////////////////////////////////////////////////////////////


// TIM5_CH3, turn motor
// PD7_Enalb
#define TURN_MOTOR_EN_PORT				GPIOD
#define TURN_MOTOR_EN_PIN				GPIO_Pin_7
#define TURN_MOTOR_EN_SRC				RCC_AHB1Periph_GPIOD
// PD0_Dir
#define TURN_MOTOR_DIR_PORT				GPIOD
#define TURN_MOTOR_DIR_PIN 				GPIO_Pin_0
#define TURN_MOTOR_DIR_SRC				RCC_AHB1Periph_GPIOD

// PD14_Clk
//#define TURN_MOTOR_CLK_PORT				GPIOD
//#define TURN_MOTOR_CLK_PIN				GPIO_Pin_14
//#define TURN_MOTOR_CLK_SRC				RCC_AHB1Periph_GPIOD
// PH12 CLK
#define TURN_MOTOR_CLK_PORT				GPIOH
#define TURN_MOTOR_CLK_PIN				GPIO_Pin_12
#define TURN_MOTOR_CLK_SRC				RCC_AHB1Periph_GPIOH

#define TURN_MOTOR_USE_TIM_PWM		0
#if TURN_MOTOR_USE_TIM_PWM
//	// TIM5_CH3
//	#define TURN_MOTOR_CLK_PIN_AF			GPIO_PinSource12
//	#define TURN_MOTOR_CLK_PORT_AF			GPIO_AF_TIM5
//	#define TURN_MOTOR_PWM_TIM				TIM5
//	#define TURN_MOTOR_PWM_TIM_SRC			RCC_APB1Periph_TIM5
//	#define TURN_MOTOR_PWM_TIM_ARR			1000 
//	#define TURN_MOTOR_PWM_TIM_PSC			11   //42    //84M/12=4M, 7M/1000=7k
//	#define TURN_MOTOR_PWM_LEVEL_CLOSE		0
//	#define TURN_MOTOR_PWM_LEVEL_BEST		400
//	#define TURN_MOTOR_PWM_LEVEL_HIGHEST	TURN_MOTOR_PWM_TIM_ARR
//	#define MOTOR_RESOLUTION				8*10
//	#define DELAY_MOTOR  					((TURN_MOTOR_PWM_TIM_ARR*MOTOR_RESOLUTION)/7)
#else
	#define TURN_MOTOR_UP_TIME					40
	#define TURN_MOTOR_DOWN_TIME				40//3000	
	#define TURN_MOTOR_DISCARD_NUM				1000
#endif

enum{
	EN_LED0 = 0,	//
	EN_LED1 = 1,	//
	EN_LED2 = 2,	// back
	EN_LED3 = 3,	// 840nm CRP
	EN_LED4 = 4,	// 580nm
	EN_LED5 = 5,	// 640nm red
	EN_LED6 = 6,	// 525nm green HGB
	EN_LED7 = 7,	// 340nm
	EN_LED_END = 8,
};

#define HGB_LED_INDEX	EN_LED6
#define CRP_LED_INDEX   EN_LED3


#if TURN_MOTOR_USE_TIM_PWM
//// e_Dir_Pos=click, e_Dir_Neg=auti-click
//enum{
//	EN_LED0_TRIGGER_OC_TIMES = 0,
//	EN_LED1_TRIGGER_OC_TIMES = 0,
//	EN_LED2_TRIGGER_OC_TIMES = 1, // e_Dir_Neg=auti-click
//	EN_LED3_TRIGGER_OC_TIMES = 1, // e_Dir_Neg=auti-click
//	EN_LED4_TRIGGER_OC_TIMES = 1, // e_Dir_Pos=click
//	EN_LED5_TRIGGER_OC_TIMES = 2, // e_Dir_Pos=click
//	EN_LED6_TRIGGER_OC_TIMES = 3, // e_Dir_Pos=click
//	EN_LED7_TRIGGER_OC_TIMES = 2, // e_Dir_Neg=auti-click
//};
//// Time4_CH3  Turn Motor clk
//void Turn_Motor_PWM_Init(UINT32 Arr, UINT32 Psc);

#else
extern  IO_ UINT8 g_Turn_Position;
#define TURN_MOTOR_RESET_MAX_STEPS	60000
enum {
	EN_LED0_SELECT_STEP		= 0,	
	EN_LED1_SELECT_STEP		= 0,
	EN_LED2_SELECT_STEP		= 8945,
	EN_LED3_SELECT_STEP		= 260,
	EN_LED4_SELECT_STEP		= 8185,
	EN_LED5_SELECT_STEP		= 16755,
	EN_LED6_SELECT_STEP		= 25205,
	EN_LED7_SELECT_STEP		= 17411,
};

enum{
	EN_POSITION_LED0 = 0,
	EN_POSITION_LED1 = 1,
	EN_POSITION_LED2 = 2,
	EN_POSITION_LED3 = 3,
	EN_POSITION_LED4 = 4,
	EN_POSITION_LED5 = 5,
	EN_POSITION_LED6 = 6,
	EN_POSITION_LED7 = 7,
	EN_POSITION_LED_RESET = 8,
	EN_POSITION_LED_UNSURE = 9
};
void Turn_CLK_Set(void);
void Turn_CLK_Reset(void);
#endif


void Turn_Motor_Init(void);
#if TURN_MOTOR_USE_TIM_PWM
//	// Turn Motor
//	void Turn_Motor_Speed_Set(UINT16 nSpeed);
//	void Turn_Motor_Run(UINT8 nDir, UINT16 nFreq);
//	UINT8 Turn_Motor_Reset(void);
//	UINT8 Turn_Motor_Select_LED(UINT8 nIndex);
//	UINT8 Turn_Motor_Goto_Postion(UINT8 nDir, UINT32 nOCTimes);
#else
	void Turn_Motor_Enable(void);
	void Turn_Motor_Disable(void);
	void Turn_Motor_Dir(UINT8 nDir); // e_Dir_Pos=clockwise. e_Dir_Neg=anti-clockwise
	void Turn_Motor_Reset();	
	void Turn_Motor_Run(UINT32 nSteps);
	UINT8 Turn_Motor_Goto_Postion(UINT8 nDir, UINT32 nSteps);
	UINT8 Turn_Motor_Select_LED(UINT8 nIndex);
#endif



//// --------- OC for turn motor reset, and than select LED, PB6
//#define TURN_RESET_OC_CLK_PORT			GPIOG
//#define TURN_RESET_OC_CLK_PIN			GPIO_Pin_4
//#define TURN_RESET_OC_CLK_SRC			RCC_AHB1Periph_GPIOG

//// OC LED select
//#define TURN_SELECT_OC_CLK_PORT			GPIOG
//#define TURN_SELECT_OC_CLK_PIN			GPIO_Pin_5
//#define TURN_SELECT_OC_CLK_SRC			RCC_AHB1Periph_GPIOG

//// OC  for Out(cx3000), P**???? //todo...
//#define OUT_OC_CLK_PORT					GPIOI
//#define OUT_OC_CLK_PIN					GPIO_Pin_1
//#define OUT_OC_CLK_SRC					RCC_AHB1Periph_GPIOI
//// OC for in (cx3000), PB7
//#define IN_OC_CLK_PORT					GPIOB
//#define IN_OC_CLK_PIN					GPIO_Pin_7
//#define IN_OC_CLK_SRC					RCC_AHB1Periph_GPIOB




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
//
void DResistor_Init(void);
void DRegister_Write(UINT8 nIndex, UINT8 nVal);


// --------- LED1-8, PI4--PI11
// LED0, null
#define LED0_PORT							GPIOI
#define LED0_PIN							GPIO_Pin_4
#define LED0_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED1, null
#define LED1_PORT							GPIOI
#define LED1_PIN							GPIO_Pin_5
#define LED1_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED2, backup
#define LED2_PORT							GPIOI
#define LED2_PIN							GPIO_Pin_6
#define LED2_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED3, 840nm CRP
#define LED3_PORT							GPIOI
#define LED3_PIN							GPIO_Pin_7
#define LED3_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED4, 580nm
#define LED4_PORT							GPIOI
#define LED4_PIN							GPIO_Pin_8
#define LED4_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED5, 640nm red
#define LED5_PORT							GPIOI
#define LED5_PIN							GPIO_Pin_9
#define LED5_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED6, 525nm HGB green
#define LED6_PORT							GPIOI
#define LED6_PIN							GPIO_Pin_10
#define LED6_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED7, 340nm
#define LED7_PORT							GPIOI
#define LED7_PIN							GPIO_Pin_11
#define LED7_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED Cur Control Switch, PB8
#define LED_CUR_SWITCH_PORT					GPIOB
#define LED_CUR_SWITCH_PIN					GPIO_Pin_8
#define LED_CUR_SWITCH_SRC					RCC_AHB1Periph_GPIOB

// LED Cur adjust, DAC, PA4
#define LED_CUR_ADJUST_PORT					GPIOA
#define LED_CUR_ADJUST_PIN					GPIO_Pin_4
#define LED_CUR_ADJUST_SRC					RCC_AHB1Periph_GPIOA  
#define LED_CUR_ADJUST_DAC_CH				DAC_Channel_1
// LED Cur ADC, PF6_ADC3_CH4
#define LED_CUR_ADC_PORT					GPIOF
#define LED_CUR_ADC_PIN						GPIO_Pin_6
#define LED_CUR_ADC_SRC						RCC_AHB1Periph_GPIOF
#define LED_CUR_ADC_CHANNEL					ADC_Channel_4
// LED Select group, PH15-A0, PH14-A1, PH13-A2
#define LED_SELECT_A0_PORT					GPIOH
#define LED_SELECT_A0_PIN					GPIO_Pin_15
#define LED_SELECT_A0_SRC					RCC_AHB1Periph_GPIOH
// LED Select group, PH14-A1
#define LED_SELECT_A1_PORT					GPIOH
#define LED_SELECT_A1_PIN					GPIO_Pin_14
#define LED_SELECT_A1_SRC					RCC_AHB1Periph_GPIOH
// LED Select group, PH15-A2
#define LED_SELECT_A2_PORT					GPIOH
#define LED_SELECT_A2_PIN					GPIO_Pin_13
#define LED_SELECT_A2_SRC					RCC_AHB1Periph_GPIOH
#define LED_SELECT_PORT						GPIOH
//
void LED_Init(void);
void LED_Cur_Switch(UINT8 nOpt);
void LED_Exec(UINT8 nIndex, UINT8 nOpt);
void LED_All_Reset(void);

void LED_Cur_DAC_Init(void);
void LED_Cur_ADC_Check_Channel(UINT16 nIndex); // chose whick channel through cd4051
UINT16 Get_LED_Cur_ADC(void);      // ADC
void LED_Cur_DAC_Set(UINT16 nVal); // DAC
void LED_Cur_Auto_Adjust(UINT16 nCurrent); 


// ---------  XK ADC, PC3_ADC3_IN13	
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


// ---------  press check, PH4_SCl, PH5_SDA I2C, I2C
// PH4
#define PRESS_I2C_SCL_PORT					GPIOH
#define PRESS_I2C_SCL_PIN					GPIO_Pin_4
#define PRESS_I2C_SCL_SCLCLK_SRC			RCC_AHB1Periph_GPIOH
// PH5
#define PRESS_I2C_SDA_PORT					GPIOH
#define PRESS_I2C_SDA_PIN					GPIO_Pin_5
#define PRESS_I2C_SDA_SCLCLK_SRC			RCC_AHB1Periph_GPIOH
//
#define PRESS_I2C_SDA_IN()  {PRESS_I2C_SDA_PORT->MODER&=~(3<<(5*2));PRESS_I2C_SDA_PORT->MODER|=0<<5*2;}	//PB9输入模式
#define PRESS_I2C_SDA_OUT() {PRESS_I2C_SDA_PORT->MODER&=~(3<<(5*2));PRESS_I2C_SDA_PORT->MODER|=1<<5*2;} //PB9输出模式
//
#define PRESS_I2C_SCL    					 PHout(4) //SCL 
#define PRESS_I2C_SDA   	 				 PHout(5) //SDA	 
#define PRESS_I2C_READ_SDA  				 PHin(5)  //SDA 
//
void Press_Init(void);
void Press_I2C_Init(void);
void Press_I2C_Start(void);
void Press_I2C_Stop(void);
UINT8 Press_I2C_Wait_Ack(void);
void Press_I2C_Ack(void);
void Press_I2C_NAck(void);
void Press_I2C_Send_Byte(UINT8 nVal);
UINT8 Press_I2C_Read_Byte(UINT8 nAck);
INT32 Get_Press_I2C(void);

// parameter define
#define FIX_MOTOR_PULSE_UP_TIME					155
#define FIX_MOTOR_PULSE_DOWN_TIME				155
#define OUTIN_MOTOR_PULSE_UP_TIME				60
#define OUTIN_MOTOR_PULSE_DOWN_TIME				60



// --------- led cur adjust
#define HGB_LED_CUR_ADJUST_VALUE					8
#define CRP_LED_CUR_ADJUST_VALUE					8

// for oc, 0-in, 1-lift
enum{
	EN_CLOSE	= 0,
	EN_OPEN		= 1
};

enum{
	 EN_VALVE_AIR		= 0,
	 EN_VALVE_LIQUID	= 1
};

//enum{
//	EN_ANTI_CLOCK_WISE = 0,
//	EN_CLOCK_WISE = 1
//};


typedef enum{
	EN_MODE_NORMAL = 0,
	EN_MODE_SELF_CHECK = 1
}eModeType;


typedef struct{
	UINT8 nSFlag;
	UINT16 nPos;
	UINT32 nID;
	UINT32 nSendID;
}ADC_Status_InitTypeDef;

// ADC1
#define ADC1_BUFFER_LEN			  	 512
#define ADC1_BUFFER_LEN_HALF	  	(ADC1_BUFFER_LEN/2)
extern IO_ ADC_Status_InitTypeDef 	 ADC1_Status;
extern  UINT16 g_ADC1_Buffer[ADC1_BUFFER_LEN_HALF];

// ADC2
#define ADC2_BUFFER_LEN			 	 512
#define ADC2_BUFFER_LEN_HALF	 	(ADC2_BUFFER_LEN/2)
extern IO_ ADC_Status_InitTypeDef 	 ADC2_Status;
extern  UINT16 g_ADC2_Buffer[ADC2_BUFFER_LEN_HALF];


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

typedef enum{
	EN_ADC1 = 0,
	EN_ADC2 = 1,
	EN_ADC3 = 3
} EN_TypeADC;

static IO_ UINT8 g_ADC3_IN[EN_ADC_END] = \
	{PRESS_ADC_CHANNEL,   XK_ADC_CHANNEL,	   CUR12N_ADC_CHANNEL, \
	 CUR12P_ADC_CHANNEL,  CUR_56V_ADC_CHANNEL, ELEC_ADC_CHANNEL, \
	 LED_CUR_ADC_CHANNEL, SIG2_ADC_CHANNEL,    SIG1_ADC_CHANNEL, \
	 TEMP_ADC_CHANNEL};

extern IO_ UINT16 g_ADC3_Value[EN_ADC_END];

	 
void Reset_ADC_InitDataType(void);
void Eable_ADC(EN_TypeADC eType);
void Disable_ADC(EN_TypeADC eType);
void ADC1_Init(void);
void ADC2_Init(void);
void ADC3_Init(void);

//UINT16 Get_Press(void);
UINT16 Get_ADC3_Channel_Value(UINT8 nIndex, UINT8 nCount);
UINT16 Get_Press_ADC(void);
UINT16 Get_XK_ADC(void);
UINT16 Get_12V_N_ADC(void);
UINT16 Get_12V_P_ADC(void);
UINT16 Get_56V_Cur_ADC(void);
//UINT16 Get_LED_Cur_ADC(void);
UINT16 Get_Temp_ADC(void);	 
UINT32  HW_Get_ADC_HGB(void);
UINT32  HW_Get_ADC_CRP(void);

// get HGB CRP adc data
UINT16  Get_HGB_Value(void);
UINT32  Get_CRP_Value(void);


// counter adjust

// PE7 counter switch 
#define COUNTER_SWITCH_PORT			GPIOE
#define COUNTER_SWITCH_PIN 			GPIO_Pin_7
#define COUNTER_SWITCH_SRC			RCC_AHB1Periph_GPIOE
// PE10 PWM
#define COUNTER_PWM_PORT			GPIOE
#define COUNTER_PWM_PIN				GPIO_Pin_10
#define COUNTER_PWM_SRC				RCC_AHB1Periph_GPIOE

// TIM1 CH1N PE10
#define COUNTER_PWM_PIN_AF			GPIO_PinSource10
#define COUNTER_PWM_PORT_AF			GPIO_AF_TIM1
#define COUNTER_PWM_TIM				TIM1
#define COUNTER_PWM_TIM_SRC			RCC_APB2Periph_TIM1
#define COUNTER_PWM_TIM_ARR			200 
#define COUNTER_PWM_TIM_PSC			6     //42M/7=6M,  6M/200=30k
#define COUNTER_PWM_LEVEL_CLOSE		0
#define COUNTER_PWM_DEFAULT_FREQ    6000000
#define COUNTER_PWM_LEVEL_HIGHEST	OUTIN_MOTOR_PWM_TIM_ARR
void Counter_Adjust_Init(void);
void Counter_Adjust_Start(UINT32 nFreq);
void Counter_Adjust_Switch(UINT8 nOpt);
void Counter_Adjust_PWM_Init(UINT32 Arr,UINT32 Psc);

//

void EVAL_Init(void);
void Driver_Debug(UINT8 nIndex);



//void Fix_Motor_Init(void);
//void Fix_Motor_Enable(void);
//void Fix_Motor_Disable(void);
//void Fix_Motor_AntiClockWise(void); // fixable
//void Fix_Motor_ClockWise(void); // free
//void Fix_Motor_Run(UINT16 nUp, UINT16 nDown);

//void OutIn_Motor_Init(void);
//void OutIn_Motor_PWM_Init(UINT32 Arr, UINT32 Psc);
//void OutIn_Motor_Speed_Set(UINT16 nSpeed); // 0-499
//void OutIn_Motor_Exec(UINT8 nDir, UINT16 nFreq);
//
//void OutIn_Motor_Enable(void);
//void OutIn_Motor_Disable(void);
//void OutIn_Motor_Out(void); //out
//void OutIn_Motor_In(void); // in
//void OutIn_Motor_Run(UINT16 nUp, UINT16 nDown);
//UINT8 OutIn_Motor_Home(eModeType eMode);
//UINT8 OutIn_Motor_Out(eModeType eMode);

































#endif




