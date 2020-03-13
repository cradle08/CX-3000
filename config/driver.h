// Scope_Eval.h

#ifndef    __DRIVER_H__
#define    __DRIVER_H__

#include "MyType.h"       // ---
#include <stdio.h>        // for "printf" debug
#include "processScope.h"
#include "stm32f4xx_tim.h"

#define PRESS_SENSOR_ADC_TYPE		0
//#if PRESS_SENSOR_ADC_TYPE
//	#define ADC3_CHECK_NUM			7
//#else
//	#define ADC3_CHECK_NUM			6
//#endif
#define LED_525_DEFUALT_CUR_VALUE  2480   // HGB
#define LED_840_DEFUALT_CUR_VALUE  2480   // CRP


extern IO_ UINT8 g_Elec_Status;


/*
#define ADC2_CHECK_NUM			2
#define ADC3_INIT_WITH_DMA		0
void ADC1_Init(void);
void ADC2_Init(void);
void ADC3_Init(void);
*/


/*
void Delay_US(UINT32 us);
*/


/*
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
//// elec ADC3_IN6 PF8
//#define ELEC_ADC_PORT							GPIOF
//#define ELEC_ADC_PIN							GPIO_Pin_8
//#define ELEC_ADC_SRC							RCC_AHB1Periph_GPIOF
//#define ELEC_ADC_CHANNEL						ADC_Channel_6
// elec
#define ELEC_USE_EXIT_MODE		0
//UINT16 Get_Elec_ADC(void);
//UINT16 Get_Elec_V(void);
void Reset_Elec_Status(void);
void Set_Elec_Status(void);
UINT8 Get_Elec_Status(void);
void Elec_Init(void);
*/

/*
//beep PE3	
#define BEEP_PORT							GPIOE
#define BEEP_PIN							GPIO_Pin_3
#define BEEP_SRC							RCC_AHB1Periph_GPIOE
void Beep_Init(void);
void Beep(UINT8 nNo, UINT16 nDelay);
*/

/*
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
//#define PUMP_PWM_LEVEL_BEST					10000
#define PUMP_PWM_LEVEL_HIGHEST				25000
//
#define PUMP_DIR_PORT						GPIOD
#define PUMP_DIR_PIN						GPIO_Pin_4
#define PUMP_DIR_SRC						RCC_AHB1Periph_GPIOD
//
void Pump_init(void);
void Pump_PWM_Init(UINT32 Arr, UINT32 Psc);
//void TIM4_PWM_Init(UINT32 Arr, UINT32 Psc);
void Pump_Speed_Set(UINT16 nSpeed);
void Pump_AntiClockWise(void);
void Pump_ClockWise(void);
void Pump_Exec(UINT8 nDir, UINT16 nFreq);
*/


// mixing motor, PB10
#define MIXING_DIR_PORT						GPIOB
#define MIXING_DIR_PIN						GPIO_Pin_10
#define MIXING_DIR_SRC						RCC_AHB1Periph_GPIOB
// mixing motor
void Mixing_Motor_Init(void);
void Mixing_Motor_Run(void);
void Mixing_Motor_Stop(void);
void Mixing_Motor_Ctr(UINT8 nOpt);
void Mixing_Motor_Go_On(UINT32 nTime);


/*
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
*/


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

// PH12 CLK , 89_GPIO
#define TURN_MOTOR_CLK_PORT				GPIOH
#define TURN_MOTOR_CLK_PIN				GPIO_Pin_12
#define TURN_MOTOR_CLK_SRC				RCC_AHB1Periph_GPIOH

#define TURN_MOTOR_UP_TIME					40
#define TURN_MOTOR_DOWN_TIME				40//3000	
#define TURN_MOTOR_DISCARD_NUM				1000

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

void Turn_Motor_Init(void);
void Turn_Motor_Enable(void);
void Turn_Motor_Disable(void);
void Turn_Motor_Dir(UINT8 nDir); // e_Dir_Pos=clockwise. e_Dir_Neg=anti-clockwise
void Turn_Motor_Reset(void);	
void Turn_Motor_Run(UINT32 nSteps);
UINT8 Turn_Motor_Goto_Postion(UINT8 nDir, UINT32 nSteps);
UINT8 Turn_Motor_Select_LED(UINT8 nIndex);


//void Turn_Motor_Break(void);
//void Turn_Motor_Free(void);
//UINT8 Turn_Motor_Reset(void);
//UINT8 Turn_Motor_ClockWise(UINT32 nStep);
//UINT8 Turn_Motor_Anti_ClockWise(UINT32 nStep);
//void Turn_Motor_Goto_Postion(UINT8 nOpt, UINT32 nStep);
//void Turn_Motor_Select_LED(UINT8 nIndex);
//void Turn_Motor_Power(UINT8 nOpt);


// OC for micro switch(cx3000), PD6
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
void Micro_OC_Exit_Disable(void);

// PD8, check counter exist or not
#define COUNTER_CHECK_GPIO_PORT				GPIOD
#define COUNTER_CHECK_GPIO_PIN				GPIO_Pin_8
#define COUNTER_CHECK_GPIO_SRC				RCC_AHB1Periph_GPIOD
void Counter_Check_Init(void);
UINT8 Get_Counter_Check_Status(void);



// PE7 counter switch 
#define COUNTER_SWITCH_PORT			GPIOE
#define COUNTER_SWITCH_PIN 			GPIO_Pin_7
#define COUNTER_SWITCH_SRC			RCC_AHB1Periph_GPIOE
// PE10 PWM
#define COUNTER_PWM_PORT			GPIOE
#define COUNTER_PWM_PIN				GPIO_Pin_10
#define COUNTER_PWM_SRC				RCC_AHB1Periph_GPIOE
// TIM1 CH1N PE10(sumulation signal)
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



/*
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
*/


enum{
	EN_CLOSE	= 0,
	EN_OPEN		= 1
};


// LED1-8, PI4--PI11
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
//// LED Cur ADC, PF6_ADC3_CH4
//#define LED_CUR_ADC_PORT					GPIOF
//#define LED_CUR_ADC_PIN						GPIO_Pin_6
//#define LED_CUR_ADC_SRC						RCC_AHB1Periph_GPIOF
//#define LED_CUR_ADC_CHANNEL					ADC_Channel_4
// LED Select group, PH15-A2, PH14-A1, PH13-A0,    A0-A1-A2
#define LED_SELECT_A0_PORT					GPIOH
#define LED_SELECT_A0_PIN					GPIO_Pin_13
#define LED_SELECT_A0_SRC					RCC_AHB1Periph_GPIOH
// LED Select group, PH14-A1
#define LED_SELECT_A1_PORT					GPIOH
#define LED_SELECT_A1_PIN					GPIO_Pin_14
#define LED_SELECT_A1_SRC					RCC_AHB1Periph_GPIOH
// LED Select group, PH15-A2
#define LED_SELECT_A2_PORT					GPIOH
#define LED_SELECT_A2_PIN					GPIO_Pin_15
#define LED_SELECT_A2_SRC					RCC_AHB1Periph_GPIOH
#define LED_SELECT_PORT						GPIOH
//
void LED_Init(void);
void LED_Cur_DAC_Init(void);
void LED_Cur_Switch(UINT8 nOpt);
void LED_Cur_ADC_Check_Channel(UINT16 nIndex); // chose whick channel through cd4051
UINT16 Get_LED_Cur_ADC(void);      // ADC
void LED_Cur_DAC_Set(UINT16 nVal); // DAC
void LED_Cur_Auto_Adjust(UINT16 nVal); 
void LED_Exec(UINT8 nIndex, UINT8 nOpt);
void LED_All_Reset(void);








/*
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

#define PRESS_I2C_SDA_IN()  {PRESS_I2C_SDA_PORT->MODER&=~(3<<(5*2));PRESS_I2C_SDA_PORT->MODER|=0<<5*2;}	//PB9输入模式
#define PRESS_I2C_SDA_OUT() {PRESS_I2C_SDA_PORT->MODER&=~(3<<(5*2));PRESS_I2C_SDA_PORT->MODER|=1<<5*2;} //PB9输出模式
#define PRESS_I2C_EOC_STATUS	 GPIO_ReadInputDataBit(PRESS_I2C_EOC_PORT, PRESS_I2C_EOC_PIN)
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
*/




/*

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

// led cur adjust
#define HGB_LED_CUR_ADJUST_VALUE	8
#define CRP_LED_CUR_ADJUST_VALUE	8
#define HGB_LED_NUM					0
// for oc, 0-in, 1-lift

enum{
	 INDEX_VALVE_PUMP		= 0,
	 INDEX_VALVE_WBC	= 1
};

enum{
	EN_ANTI_CLOCK_WISE = 0,
	EN_CLOCK_WISE = 1
};


typedef enum{
	EN_MODE_NORMAL = 0,
	EN_MODE_SELF_CHECK = 1
}eModeType;


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

static IO_ UINT8 g_ADC3_IN[EN_ADC_END] = \
	{PRESS_ADC_CHANNEL,   XK_ADC_CHANNEL,	   CUR12N_ADC_CHANNEL, \
	 CUR12P_ADC_CHANNEL,  CUR_56V_ADC_CHANNEL, ELEC_ADC_CHANNEL, \
	 LED_CUR_ADC_CHANNEL, SIG2_ADC_CHANNEL,    SIG1_ADC_CHANNEL, \
	 TEMP_ADC_CHANNEL};

extern IO_ UINT16 g_ADC3_Value[EN_ADC_END];

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

// get HGB CRP adc data
UINT16  Get_HGB_Value(void);
UINT32  Get_CRP_Value(void);

*/





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





/*
// --------- OC for turn motor reset, and than select LED, PG0
#define TURN_RESET_OC_GPIO_PORT			GPIOG
#define TURN_RESET_OC_GPIO_PIN			GPIO_Pin_0
#define TURN_RESET_OC_GPIO_SRC			RCC_AHB1Periph_GPIOG

// OC LED select, PG6
#define TURN_SELECT_OC_GPIO_PORT		GPIOG
#define TURN_SELECT_OC_GPIO_PIN			GPIO_Pin_6
#define TURN_SELECT_OC_GPIO_SRC			RCC_AHB1Periph_GPIOG


// OC for in (cx3000), PG2
#define IN_OC_GPIO_PORT					GPIOG
#define IN_OC_GPIO_PIN					GPIO_Pin_2
#define IN_OC_GPIO_SRC					RCC_AHB1Periph_GPIOG

// OC  for Out(cx3000), PG3
#define OUT_OC_GPIO_PORT				GPIOG
#define OUT_OC_GPIO_PIN					GPIO_Pin_3
#define OUT_OC_GPIO_SRC					RCC_AHB1Periph_GPIOG

// OC for in (cx3000), PG4
#define CLAMP_IN_OC_GPIO_PORT			GPIOG
#define CLAMP_IN_OC_GPIO_PIN			GPIO_Pin_4
#define CLAMP_IN_OC_GPIO_SRC			RCC_AHB1Periph_GPIOG

// OC  for Out(cx3000), PG5
#define CLAMP_OUT_OC_GPIO_PORT			GPIOG
#define CLAMP_OUT_OC_GPIO_PIN			GPIO_Pin_5
#define CLAMP_OUT_OC_GPIO_SRC			RCC_AHB1Periph_GPIOG

void  OC_Init(void);
UINT8 Get_Micro_OC_Status(void); // 0: in, 1: out
UINT8 Get_Turn_Select_OC_Status(void); 
UINT8 Get_Turn_Reset_OC_Status(void); 
UINT8 Get_Out_OC_Status(void);
UINT8 Get_In_OC_Status(void);
*/



//----------------OUT IN Motor, TIM3, Out_In_Motor, 
/*

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
*/




// api for cx2000


// phase of the movement



//
void Driver_Debug(UINT8 nIndex);














































#endif




