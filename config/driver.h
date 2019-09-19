// Scope_Eval.h

#ifndef    __DRIVER_H__
#define    __DRIVER_H__

#include "MyType.h"       // ---
#include <stdio.h>        // for "printf" debug


#define ADC2_CHECK_NUM			2
#define ADC3_CHECK_NUM			5
#define ADC3_INIT_WITH_DMA		0

extern IO_ UINT8 g_Elec_Status;
//extern IO_ UINT16 g_ADC2_Value[ADC2_CHECK_NUM];
extern IO_ UINT16 g_ADC3_Value[ADC3_CHECK_NUM];

// elec PF8
#define ELEC_PORT							GPIOF
#define ELEC_PIN							GPIO_Pin_8
#define ELEC_SRC							RCC_AHB1Periph_GPIOF
#define ELEC_EXIT_LINE 						EXTI_Line8
#define ELEC_EXIT_SRC						EXTI_PortSourceGPIOF
#define ELEC_EXIT_PIN						EXTI_PinSource8
#define ELEC_EXIT_NUM						EXTI9_5_IRQn
//#define ELEC_EXIT_FUNC						EXTI9_5_IRQHandler
#define ELEC_READ							GPIO_ReadInputDataBit(ELEC_PORT, ELEC_PIN)

//beep PE3	
#define BEEP_PORT							GPIOE
#define BEEP_PIN							GPIO_Pin_3
#define BEEP_SRC							RCC_AHB1Periph_GPIOE

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
#define PUMP_PWM_LEVEL_BEST					10000
#define PUMP_PWM_LEVEL_HIGHEST				25000

#define PUMP_DIR_PORT						GPIOD
#define PUMP_DIR_PIN						GPIO_Pin_4
#define PUMP_DIR_SRC						RCC_AHB1Periph_GPIOD

//switch 1_PF11, 2_PH6
#define VALVE_AIR_PORT						GPIOF
#define VALVE_AIR_PIN						GPIO_Pin_11
#define VALVE_AIR_SRC						RCC_AHB1Periph_GPIOF

#define VALVE_LIQUID_PORT					GPIOH
#define VALVE_LIQUID_PIN					GPIO_Pin_6
#define VALVE_LIQUID_SRC					RCC_AHB1Periph_GPIOH

// turn motor, PB12, PB13, PB14, PB15
#define TURN_MOTOR_PORT_1					GPIOB
#define TURN_MOTOR_PIN_1					GPIO_Pin_12
#define TURN_MOTOR_SRC_1					RCC_AHB1Periph_GPIOB

#define TURN_MOTOR_PORT_2					GPIOB
#define TURN_MOTOR_PIN_2					GPIO_Pin_13
#define TURN_MOTOR_SRC_2					RCC_AHB1Periph_GPIOB

#define TURN_MOTOR_PORT_3					GPIOB
#define TURN_MOTOR_PIN_3					GPIO_Pin_14
#define TURN_MOTOR_SRC_3					RCC_AHB1Periph_GPIOB

#define TURN_MOTOR_PORT_4					GPIOB
#define TURN_MOTOR_PIN_4					GPIO_Pin_15
#define TURN_MOTOR_SRC_4					RCC_AHB1Periph_GPIOB


// out in motor, not sure by now
#define OUTIN_MOTOR_PORT_1					GPIO*
#define OUTIN_MOTOR_PIN_1					GPIO_Pin_*
#define OUTIN_MOTOR_SRC_1					RCC_AHB1Periph_GPIO*

#define OUTIN_MOTOR_PORT_2					GPIO*
#define OUTIN_MOTOR_PIN_2					GPIO_Pin_*
#define OUTIN_MOTOR_SRC_2					RCC_AHB1Periph_GPIO*

#define OUTIN_MOTOR_PORT_3					GPIO*
#define OUTIN_MOTOR_PIN_3					GPIO_Pin_*
#define OUTIN_MOTOR_SRC_3					RCC_AHB1Periph_GPIO*

#define OUTIN_MOTOR_PORT_4					GPIO*
#define OUTIN_MOTOR_PIN_4					GPIO_Pin_*
#define OUTIN_MOTOR_SRC_4					RCC_AHB1Periph_GPIO*


//// fix motor, PD0_EN, PD1_Dir, PD2_CLK
//#define FIX_MOTOR_EN_PORT					GPIOD
//#define FIX_MOTOR_EN_PIN					GPIO_Pin_0
//#define FIX_MOTOR_EN_SRC					RCC_AHB1Periph_GPIOD

//#define FIX_MOTOR_DIR_PORT					GPIOD
//#define FIX_MOTOR_DIR_PIN					GPIO_Pin_1
//#define FIX_MOTOR_DIR_SRC					RCC_AHB1Periph_GPIOD

//#define FIX_MOTOR_CLK_PORT					GPIOD
//#define FIX_MOTOR_CLK_PIN					GPIO_Pin_2
//#define FIX_MOTOR_CLK_SRC					RCC_AHB1Periph_GPIOD

//// out_in motor, PD10_EN, PD11_Dir, (PD12_CLK or PC8_CLK_PWM_CH3)
//#define OUTIN_MOTOR_EN_PORT					GPIOD
//#define OUTIN_MOTOR_EN_PIN					GPIO_Pin_10
//#define OUTIN_MOTOR_EN_SRC					RCC_AHB1Periph_GPIOD

//#define OUTIN_MOTOR_DIR_PORT				GPIOD
//#define OUTIN_MOTOR_DIR_PIN 				GPIO_Pin_11
//#define OUTIN_MOTOR_DIR_SRC					RCC_AHB1Periph_GPIOD

//// PD12_CLK or PC8_CLK_PWM_CH3
//#define OUTIN_MOTOR_USE_PWM					1
//#if OUTIN_MOTOR_USE_PWM
//	#define OUTIN_MOTOR_CLK_PORT			GPIOC
//	#define OUTIN_MOTOR_CLK_PIN				GPIO_Pin_8
//	#define OUTIN_MOTOR_CLK_SRC				RCC_AHB1Periph_GPIOC
//	#define OUTIN_MOTOR_CLK_PIN_AF			GPIO_PinSource8
//	#define OUTIN_MOTOR_CLK_PORT_AF			GPIO_AF_TIM3
//	#define OUTIN_MOTOR_PWM_TIM				TIM3
//	#define OUTIN_MOTOR_PWM_TIM_SRC			RCC_APB1Periph_TIM3
//	#define OUTIN_MOTOR_PWM_TIM_ARR			1000 //25000
//	#define OUTIN_MOTOR_PWM_TIM_PSC			11   //42    //84M/12=4M, 7M/1000=7k
//	#define OUTIN_MOTOR_PWM_LEVEL_CLOSE		0
//	#define OUTIN_MOTOR_PWM_LEVEL_BEST		500
//	#define OUTIN_MOTOR_PWM_LEVEL_HIGHEST	1000
//#else
//	#define OUTIN_MOTOR_CLK_PORT			GPIOD
//	#define OUTIN_MOTOR_CLK_PIN				GPIO_Pin_12
//	#define OUTIN_MOTOR_CLK_SRC				RCC_AHB1Periph_GPIOD
//#endif


// OC for fix motor(cx2000) | OC for cang2(cx3000), PG3
#define FIX_OC_CLK_PORT						GPIOG
#define FIX_OC_CLK_PIN						GPIO_Pin_3
#define FIX_OC_CLK_SRC						RCC_AHB1Periph_GPIOG

// OC for out motor(cx2000), | OC for cang2(cx3000), PG4
#define OUT_OC_CLK_PORT						GPIOG
#define OUT_OC_CLK_PIN						GPIO_Pin_4
#define OUT_OC_CLK_SRC						RCC_AHB1Periph_GPIOG

// OC for in motor(cx2000), | OC for motor(cx3000), PG5
#define IN_OC_CLK_PORT						GPIOG
#define IN_OC_CLK_PIN						GPIO_Pin_5
#define IN_OC_CLK_SRC						RCC_AHB1Periph_GPIOG

// Digital Register(SPI2), PI2_CLK,PI3_MOSI,PI0_CS
#define D_REGISTER_CLK_PORT					GPIOI
#define D_REGISTER_CLK_PIN					GPIO_Pin_2
#define D_REGISTER_CLK_SRC					RCC_AHB1Periph_GPIOC

#define D_REGISTER_MOSI_PORT				GPIOI
#define D_REGISTER_MOSI_PIN					GPIO_Pin_3
#define D_REGISTER_MOSI_SRC					RCC_AHB1Periph_GPIOC

#define D_REGISTER_CS_PORT					GPIOI
#define D_REGISTER_CS_PIN					GPIO_Pin_0
#define D_REGISTER_CS_SRC					RCC_AHB1Periph_GPIOC
#define D_REGISTER_SPI						SPI3
#define D_REGISTER_SPI_SRC 					RCC_APB1Periph_SPI2
//#define D_REGISTER_CLK_SPI_PINSRC 

// LED1-8, PI4--PI11
// LED1
#define LED1_PORT							GPIOI
#define LED1_PIN							GPIO_Pin_4
#define LED1_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED2
#define LED2_PORT							GPIOI
#define LED2_PIN							GPIO_Pin_5
#define LED2_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED3
#define LED3_PORT							GPIOI
#define LED3_PIN							GPIO_Pin_6
#define LED3_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED4
#define LED4_PORT							GPIOI
#define LED4_PIN							GPIO_Pin_7
#define LED4_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED5
#define LED5_PORT							GPIOI
#define LED5_PIN							GPIO_Pin_8
#define LED5_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED6
#define LED6_PORT							GPIOI
#define LED6_PIN							GPIO_Pin_9
#define LED6_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED7
#define LED7_PORT							GPIOI
#define LED7_PIN							GPIO_Pin_10
#define LED7_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED8
#define LED8_PORT							GPIOI
#define LED8_PIN							GPIO_Pin_11
#define LED8_CLK_SRC						RCC_AHB1Periph_GPIOI
// LED Cur Control Switch, PB8
#define LED_CUR_SWITCH_PORT					GPIOB
#define LED_CUR_SWITCH_PIN					GPIO_Pin_8
#define LED_CUR_SWITCH_SRC					RCC_AHB1Periph_GPIOB
// LED Cur adjust, DAC, PA4
#define LED_CUR_ADJUST_PORT					GPIOA
#define LED_CUR_ADJUST_PIN					GPIO_Pin_4
#define LED_CUR_ADJUST_SRC					RCC_AHB1Periph_GPIOB

// press check, I2C


#define FIX_MOTOR_PULSE_UP_TIME				155
#define FIX_MOTOR_PULSE_DOWN_TIME			155
#define OUTIN_MOTOR_PULSE_UP_TIME			60
#define OUTIN_MOTOR_PULSE_DOWN_TIME			60

#define	OUTIN_MOTOR_HOME_TIME				10000

// for oc, 0-in, 1-lift
enum{
	EN_CLOSE	= 0,
	EN_OPEN		= 1
};

enum{
	EN_VALVE_AIR	= 0,
	EN_VALVE_LIQUID = 1
};

enum{
	EN_ANTI_CLOCK_WISE = 0,
	EN_CLOCK_WISE = 1
};

enum{
	EN_LED1 = 1,
	EN_LED2 = 2,
	EN_LED3 = 3,
	EN_LED4 = 4,
	EN_LED5 = 5,
	EN_LED6 = 6,
	EN_LED7 = 7,
	EN_LED8 = 8
};

typedef enum{
	EN_MODE_NORMAL = 0,
	EN_MODE_SELF_CHECK = 1
}eModeType;

void ADC1_Init(void);
void ADC2_Init(void);
void ADC3_Init(void);
UINT16 Get_XK_ADC(void);
UINT16 Get_Press_ADC(void);
UINT16 Get_56V_Cur_ADC(void);
UINT16 Get_LED_Cur_ADC(void);
UINT32  HW_Get_ADC_HGB(void);
UINT32  HW_Get_ADC_CRP(void);
// get HGB CRP adc data
UINT16  Get_HGB_Value(void);
UINT32  Get_CRP_Value(void);


void Reset_Elec_Status(void);
void Set_Elec_Status(void);
UINT8 Get_Elec_Status(void);
void Elec_Init(void);

void Beep_Init(void);
void Beep(UINT16 nDelay);

void Pump_init(void);
void Pump_PWM_Init(UINT32 Arr, UINT32 Psc);
//void TIM4_PWM_Init(UINT32 Arr, UINT32 Psc);
void Pump_Speed_Set(UINT16 nSpeed);
void Pump_AntiClockWise(void);
void Pump_ClockWise(void);
void Pump_Exec(UINT8 nDir, UINT16 nFreq);

void Valve_Init(void);
void Valve_Air_Exec(UINT8 nOpt);
void Valve_Liquid_Exec(UINT8 nOpt);
void Valve_Exec(UINT8 nIndex, UINT8 nOpt);


void OC_Init(void);
UINT8 Get_Fix_OC_Status(void);
UINT8 Get_Out_OC_Status(void);
UINT8 Get_In_OC_Status(void);

void LED_Init(void);
void LEC_Cur_Adjust_DAC_Init(void);
void LED_Cur_Switch(UINT8 nOpt);
void LED_Exec(UINT8 nIndex, UINT8 nOpt);

void DResistor_Init(void);
void DResistor_Set(UINT8 nIndex, UINT8 nVal);


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
//void OutIn_Motor_AntiClockWise(void); //out
//void OutIn_Motor_ClockWise(void); // in
//void OutIn_Motor_Run(UINT16 nUp, UINT16 nDown);
//UINT8 OutIn_Motor_Home(eModeType eMode);
//UINT8 OutIn_Motor_Out(eModeType eMode);























#endif




