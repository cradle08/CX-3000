// Scope_Eval.h

#ifndef    __DRIVER_H__
#define    __DRIVER_H__

#include "MyType.h"       // ---
#include <stdio.h>        // for "printf" debug


#define PRESS_SENSOR_ADC_TYPE				0
#if PRESS_SENSOR_ADC_TYPE
	#define ADC3_CHECK_NUM			7
#else
	#define ADC3_CHECK_NUM			6
#endif

#define ADC2_CHECK_NUM			2
#define ADC3_INIT_WITH_DMA		0

extern IO_ UINT8 g_Elec_Status;
//extern IO_ UINT16 g_ADC2_Value[ADC2_CHECK_NUM];
extern IO_ UINT16 g_ADC3_Value[ADC3_CHECK_NUM];

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
//
#define PUMP_DIR_PORT						GPIOD
#define PUMP_DIR_PIN						GPIO_Pin_4
#define PUMP_DIR_SRC						RCC_AHB1Periph_GPIOD

// mixing motor, PB10
#define MIXING_DIR_PORT						GPIOB
#define MIXING_DIR_PIN						GPIO_Pin_10
#define MIXING_DIR_SRC						RCC_AHB1Periph_GPIOB

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

#define TURN_MOTOR_PORT						GPIOB

// out in motor, not sure by now
//#define OUTIN_MOTOR_PORT_1					GPIO*
//#define OUTIN_MOTOR_PIN_1					GPIO_Pin_*
//#define OUTIN_MOTOR_SRC_1					RCC_AHB1Periph_GPIO*

//#define OUTIN_MOTOR_PORT_2					GPIO*
//#define OUTIN_MOTOR_PIN_2					GPIO_Pin_*
//#define OUTIN_MOTOR_SRC_2					RCC_AHB1Periph_GPIO*

//#define OUTIN_MOTOR_PORT_3					GPIO*
//#define OUTIN_MOTOR_PIN_3					GPIO_Pin_*
//#define OUTIN_MOTOR_SRC_3					RCC_AHB1Periph_GPIO*

//#define OUTIN_MOTOR_PORT_4					GPIO*
//#define OUTIN_MOTOR_PIN_4					GPIO_Pin_*
//#define OUTIN_MOTOR_SRC_4					RCC_AHB1Periph_GPIO*

// OC for turn motor reset, and than select LED, PB6?????
#define FIX_OC_CLK_PORT						GPIOB
#define FIX_OC_CLK_PIN						GPIO_Pin_6
#define FIX_OC_CLK_SRC						RCC_AHB1Periph_GPIOB

// OC for micro switch(cx3000), PD6
#define MICRO_OC_PORT					GPIOD
#define MICRO_OC_PIN					GPIO_Pin_6
#define MICRO_OC_SRC					RCC_AHB1Periph_GPIOD
#define MICRO_OC_EXIT_PORT				EXTI_PortSourceGPIOD
#define MICRO_OC_EXIT_PIN				EXTI_PinSource6
#define MICRO_OC_EXIT_LINE				EXTI_Line6
#define MICRO_OC_EXIT_FUNC				EXTI9_5_IRQHandler
#define MICRO_OC_EXIT_IRQ				EXTI9_5_IRQn

// OC  for Out(cx3000), PB6
#define OUT_OC_CLK_PORT						GPIOB
#define OUT_OC_CLK_PIN						GPIO_Pin_6
#define OUT_OC_CLK_SRC						RCC_AHB1Periph_GPIOB

// OC for in (cx3000), PB7
#define IN_OC_CLK_PORT						GPIOB
#define IN_OC_CLK_PIN						GPIO_Pin_7
#define IN_OC_CLK_SRC						RCC_AHB1Periph_GPIOB


//cx3000 Digital Register(SPI2), PI1_CLK,PI3_MOSI,PI0_CS
#define D_REGISTER_CLK_PORT					GPIOI
#define D_REGISTER_CLK_PIN					GPIO_Pin_1
#define D_REGISTER_CLK_SRC					RCC_AHB1Periph_GPIOI
#define D_REGISTER_CLK_AF_SRC 				GPIO_PinSource1

#define D_REGISTER_MOSI_PORT				GPIOI
#define D_REGISTER_MOSI_PIN					GPIO_Pin_3
#define D_REGISTER_MOSI_SRC					RCC_AHB1Periph_GPIOI
#define D_REGISTER_MOSI_AF_SRC 				GPIO_PinSource3

#define D_REGISTER_CS_PORT					GPIOI
#define D_REGISTER_CS_PIN					GPIO_Pin_0
#define D_REGISTER_CS_SRC					RCC_AHB1Periph_GPIOI
#define D_REGISTER_CS_AF_SRC 				GPIO_PinSource0

#define D_REGISTER_SPI						SPI2
#define D_REGISTER_SPI_SRC 					RCC_APB1Periph_SPI2
#define D_REGISTER_SPI_AF 					GPIO_AF_SPI2

	
// AD7799 for 24bits HGB and CRP data, SPI3_CLK_PC10, SPI3_MOSI_PB5(PC12),
// SPI3_MISO_PB4(PC11)	SPI3_CS_PA15(PC13)
#define ADC24BIT_CLK_PORT					GPIOC
#define ADC24BIT_CLK_PIN					GPIO_Pin_10
#define ADC24BIT_CLK_SRC					RCC_AHB1Periph_GPIOC
#define ADC24BIT_CLK_AF_SRC 				GPIO_PinSource10

#define ADC24BIT_MOSI_PORT					GPIOB
#define ADC24BIT_MOSI_PIN					GPIO_Pin_5
#define ADC24BIT_MOSI_SRC					RCC_AHB1Periph_GPIOB
#define ADC24BIT_MOSI_AF_SRC 				GPIO_PinSource5

#define ADC24BIT_MISO_PORT					GPIOB
#define ADC24BIT_MISO_PIN					GPIO_Pin_4
#define ADC24BIT_MISO_SRC					RCC_AHB1Periph_GPIOB
#define ADC24BIT_MISO_AF_SRC 				GPIO_PinSource4

#define ADC24BIT_CS_PORT					GPIOA
#define ADC24BIT_CS_PIN						GPIO_Pin_15
#define ADC24BIT_CS_SRC						RCC_AHB1Periph_GPIOA
#define ADC24BIT_CS_AF_SRC 					GPIO_PinSource15

#define ADC24BIT_SPI						SPI3
#define ADC24BIT_SPI_SRC 					RCC_APB1Periph_SPI3
#define ADC24BIT_SPI_AF 					GPIO_AF_SPI3

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
#define LED_CUR_ADJUST_SRC					RCC_AHB1Periph_GPIOA  
#define LED_CUR_ADJUST_DAC_CH				DAC_Channel_1
// LED Cur ADC, PF6_ADC3_CH4
#define LED_CUR_ADC_PORT					GPIOF
#define LED_CUR_ADC_PIN						GPIO_Pin_6
#define LED_CUR_ADC_SRC						RCC_AHB1Periph_GPIOF
#define LED_CUR_ADC_CHANNEL					ADC_Channel_4
// LED Select group, PH13-A0, PH14-A1, PH15-A2
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
#if PRESS_SENSOR_ADC_TYPE
	#define PRESS_ADC_PORT					GPIOC
	#define PRESS_ADC_PIN					GPIO_Pin_2
	#define PRESS_ADC_SRC					RCC_AHB1Periph_GPIOC
	#define PRESS_ADC_CHANNEL				ADC_Channel_12
#endif

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
#define PRESS_I2C_SDA_IN()  {PRESS_I2C_SDA_PORT->MODER&=~(3<<(5*2));PRESS_I2C_SDA_PORT->MODER|=0<<5*2;}	//PB9输入模式
#define PRESS_I2C_SDA_OUT() {PRESS_I2C_SDA_PORT->MODER&=~(3<<(5*2));PRESS_I2C_SDA_PORT->MODER|=1<<5*2;} //PB9输出模式
//
#define PRESS_I2C_SCL    					 PHout(4) //SCL 
#define PRESS_I2C_SDA   	 				 PHout(5) //SDA	 
#define PRESS_I2C_READ_SDA  				 PHin(5)  //SDA 

// parameter define
#define FIX_MOTOR_PULSE_UP_TIME					155
#define FIX_MOTOR_PULSE_DOWN_TIME				155
#define OUTIN_MOTOR_PULSE_UP_TIME				60
#define OUTIN_MOTOR_PULSE_DOWN_TIME				60

#define	OUTIN_MOTOR_HOME_TIME					10000
// turn motor 
#define TURN_MOTOR_MAX_ANTI_CLOCKWISE_STEP		5000
#define TURN_MOTOR_MAX_CLOCKWISE_STEP			5000
#define TURN_MOTOR_MAX_DELAY					4000
#define TURN_MOTOR_MIN_DELAY					3000
// led cur adjust
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

enum {
	EN_LED1_POSTION		= 300,
	EN_LED2_POSTION		= 600,
	EN_LED3_POSTION		= 900,
	EN_LED4_POSTION		= 1200,
	EN_LED5_POSTION		= 1500,
	EN_LED6_POSTION		= 1800,
	EN_LED7_POSTION		= 2100,
	EN_LED8_POSTION		= 2400,	
};

typedef enum{
	EN_MODE_NORMAL = 0,
	EN_MODE_SELF_CHECK = 1
}eModeType;

void ADC1_Init(void);
void ADC2_Init(void);
void ADC3_Init(void);

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

//UINT16 Get_Press(void);
UINT16 Get_Press_ADC(void);
UINT16 Get_XK_ADC(void);
UINT16 Get_56V_Cur_ADC(void);
UINT16 Get_LED_Cur_ADC(void);
UINT32  HW_Get_ADC_HGB(void);
UINT32  HW_Get_ADC_CRP(void);
// get HGB CRP adc data
UINT16  Get_HGB_Value(void);
UINT32  Get_CRP_Value(void);

// elec
UINT16 Get_Elec_ADC(void);
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

// mixing motor
void Mixing_Motor_Init(void);
void Mixing_Motor_Run(void);
void Mixing_Motor_Stop(void);

// valve
void Valve_Init(void);
void Valve_Air_Exec(UINT8 nOpt);
void Valve_Liquid_Exec(UINT8 nOpt);
void Valve_Exec(UINT8 nIndex, UINT8 nOpt);

// turn motor
void Turn_Motor_Init(void);
UINT8 Turn_Motor_Reset(void);
void Turn_Motor_Goto_Postion(UINT32 nStep);
void Turn_Motor_Select_LED(UINT8 nIndex);

//
void OC_Init(void);
UINT8 Get_Micro_OC_Status(void);
UINT8 Get_Fix_OC_Status(void);
UINT8 Get_Out_OC_Status(void);
UINT8 Get_In_OC_Status(void);

//
void LED_Init(void);
void LED_Cur_DAC_Init(void);
void LED_Cur_Switch(UINT8 nOpt);
void LED_Cur_DAC_Set(UINT16 nVal); // adc
void LED_Cur_Auto_Adjust(UINT16 nVal); // adc
void LED_Exec(UINT8 nIndex, UINT8 nOpt);

//
void ADC24Bit_Init(void);
void ADC24Bit_SPI_Init(void);
UINT32 ADC24Bit_Get_ADC(void);

// 
void DResistor_Init(void);
void DRegister_SPI_Init(void);
void DResistor_Set(UINT8 nIndex, UINT8 nVal);

//
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




