// Scope_Eval.c

/////#ifndef    __DRIVER_C__
//#define    __DRIVER_C__

#include "ChainHeader.h"


// ADC1
IO_ ADC_Status_InitTypeDef ADC1_Status = {0};
UINT16 g_ADC1_Buffer[ADC1_BUFFER_LEN_HALF] = {0};

// ADC2
IO_ ADC_Status_InitTypeDef ADC2_Status = {0};
UINT16 g_ADC2_Buffer[ADC2_BUFFER_LEN_HALF] = {0};


IO_ UINT8 g_Elec_Status = 0;
//IO_ UINT16 g_ADC2_Value[ADC2_CHECK_NUM] = {0};
IO_ UINT16 g_ADC3_Value[EN_ADC_END] = {0};

const unsigned int g_Turn_Motor_Table[4]={0xC000,0x6000,0x3000,0x9000};
IO_ UINT8 g_Turn_Position = EN_POSITION_LED_RESET;

IO_ UINT8 g_Micro_Switch = 0xFF;


//IO_ static UINT32  fac_us=0;							//us延时倍乘数			   
//IO_ static UINT32  fac_ms=0;	
//void delay_init()
//{

//	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
//	fac_us=SystemCoreClock/168;				//为系统时钟的1/8  
//	fac_ms=(u16)fac_us*1000;					//非OS下,代表每个ms需要的systick时钟数   
//}

//void delay_us(u32 nus)
//{		
//	u32 temp;	    	 
//	SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
//	SysTick->VAL=0x00;        					//清空计数器
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
//	do
//	{
//		temp=SysTick->CTRL;
//	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
//	SysTick->VAL =0X00;      					 //清空计数器	 
//}
////延时nms
////注意nms的范围
////SysTick->LOAD为24位寄存器,所以,最大延时为:
////nms<=0xffffff*8*1000/SYSCLK
////SYSCLK单位为Hz,nms单位为ms
////对72M条件下,nms<=1864 
//void delay_ms(u16 nms)
//{	 		  	  
//	u32 temp;		   
//	SysTick->LOAD=(u32)nms*fac_ms;				//时间加载(SysTick->LOAD为24bit)
//	SysTick->VAL =0x00;							//清空计数器
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数  
//	do
//	{
//		temp=SysTick->CTRL;
//	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
//	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
//	SysTick->VAL =0X00;       					//清空计数器	  	    
//} 


void Delay_US(UINT32 us)
{
    UINT32 start, now, delta, reload, us_tick;
    start = SysTick->VAL;
    reload = SysTick->LOAD;
    us_tick = SystemCoreClock / 1000000UL;
    do {
        now = SysTick->VAL;
        delta = start > now ? start - now : reload + start - now;
    } while(delta < us_tick * us);
}

//void Delay_US(UINT32 us)
//{
//    UINT32 start, now, delta, reload, us_tick;
//    start = SysTick->VAL;
//    reload = SysTick->LOAD;
//    us_tick = SystemCoreClock / 1000000UL;
//    do {
//        now = SysTick->VAL;
//		if(start > now) delta = start - now;
//		else delta = reload - now + start;
//        //delta = start > now ? start - now : reload + start - now;
//    } while(delta < us_tick * us);
//}



////---------------------------OUT PIN-------------------
GPIO_TypeDef*  CODE_ OUT_PORT[OUTPUT_NUM]= 
{
       
    OUT_MCU_LED1_GPIO_PORT,
    OUT_MCU_LED2_GPIO_PORT,
    OUT_LAN8720_RST_GPIO_PORT,
	OUTIN_MOTOR_EN_PORT,
	OUTIN_MOTOR_DIR_PORT,
	TURN_MOTOR_EN_PORT,
	TURN_MOTOR_DIR_PORT,
	COUNTER_SWITCH_PORT,
};

UINT16 CODE_ OUT_PIN[OUTPUT_NUM]= 
{
    OUT_MCU_LED1_GPIO_PIN,
    OUT_MCU_LED2_GPIO_PIN,
    OUT_LAN8720_RST_GPIO_PIN,
	OUTIN_MOTOR_EN_PIN,
	OUTIN_MOTOR_DIR_PIN,
	TURN_MOTOR_EN_PIN,
	TURN_MOTOR_DIR_PIN,
	COUNTER_SWITCH_PIN,
};

UINT32 CODE_ OUT_CLK[OUTPUT_NUM]= 
{
    //
    OUT_MCU_LED1_GPIO_CLK,
    OUT_MCU_LED2_GPIO_CLK,
    OUT_LAN8720_RST_GPIO_CLK,
	OUTIN_MOTOR_EN_SRC,
	OUTIN_MOTOR_DIR_SRC,
	TURN_MOTOR_EN_SRC,
	TURN_MOTOR_DIR_SRC,
	COUNTER_SWITCH_SRC,
};

void EVAL_OutputInit(Output_TypeDef eOut)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
 
    // 1. enable the GPIO Clock  
    RCC_AHB1PeriphClockCmd(OUT_CLK[eOut], ENABLE);  // AHB1

    // 2. configure the GPIO pin
	GPIO_InitStructure.GPIO_Pin = OUT_PIN[eOut]; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
    GPIO_Init(OUT_PORT[eOut], &GPIO_InitStructure);  // init gpio
}

void EVAL_OutputInit_All(void)
{
	UINT8 i;
	for(i = 0; i < OUTPUT_NUM; i++)
	{
		EVAL_OutputInit(i);
	}
}


void EVAL_OutputSet(Output_TypeDef eOut)
{
    OUT_PORT[eOut]->BSRRL = OUT_PIN[eOut];
}

void EVAL_OutputClr(Output_TypeDef eOut)
{
    OUT_PORT[eOut]->BSRRH = OUT_PIN[eOut];
}

void EVAL_OutputToggle(Output_TypeDef eOut)
{
    OUT_PORT[eOut]->ODR ^= OUT_PIN[eOut];
}


// commom input and exti interrupt
GPIO_TypeDef* CODE_ IN_PORT[INPUT_NUM]= 
{
	TURN_RESET_OC_GPIO_PORT,
	TURN_SELECT_OC_GPIO_PORT,
	IN_OC_GPIO_PORT,
	OUT_OC_GPIO_PORT,
	COUNTER_CHECK_GPIO_PORT,
};

UINT16 CODE_ IN_PIN[INPUT_NUM]=
{
	TURN_RESET_OC_GPIO_PIN,
	TURN_SELECT_OC_GPIO_PIN,
	IN_OC_GPIO_PIN,
	OUT_OC_GPIO_PIN,
	COUNTER_CHECK_GPIO_PIN,
};	

UINT32 CODE_ IN_CLK[INPUT_NUM]=
{
	TURN_RESET_OC_GPIO_SRC,
	TURN_SELECT_OC_GPIO_SRC,
	IN_OC_GPIO_SRC,
	OUT_OC_GPIO_SRC,
	COUNTER_CHECK_GPIO_SRC,
};


// GPIO input contorl 
void EVAL_InputInit(Input_TypeDef eIn)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
  
    // 1. enable the input pin Clock 
    RCC_AHB1PeriphClockCmd(IN_CLK[eIn], ENABLE);
  
    // 2. configure the pin as input floating 
	GPIO_InitStructure.GPIO_Pin = IN_PIN[eIn];  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
    GPIO_Init(IN_PORT[eIn], &GPIO_InitStructure); 
	GPIO_ResetBits(IN_PORT[eIn], IN_PIN[eIn]);
}

void EVAL_InputInit_All(void)
{
	UINT8 i;
	for(i = 0; i < INPUT_NUM; i++)
	{
		EVAL_InputInit(i);
	}
}
	
// Get GPIO Status
UINT8 EVAL_GetInputStatus(Input_TypeDef eIn)
{
    return GPIO_ReadInputDataBit(IN_PORT[eIn], IN_PIN[eIn]);
}




UINT8  PF_InitTimer2(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 

    // reset the timerX
	TIM_DeInit(TIM2); 
    // enable the clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	// interrupt configuration
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure); 
	//----------------------------------------------------------------
	// TIMx configuration  11059200 * 15 = (256*432*100) * 15 = 165888000  
	// attention: "== SYSCLK / 3"
	//            ".TIM_Period" is autoreload value, and must bigger 
	//            than 1000, or it case many interrupts too frequently
	//            that consumes all the interrupt-resource of the MCU
    TIM_TimeBaseStructure.TIM_Period = 6480;                      // 10ms, 100Hz = 648000 / 6480
    TIM_TimeBaseStructure.TIM_Prescaler = 256;                    // 165888000 / 256 = 648000 Hz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       // none   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   // up 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
    //----------------------------------------------
	// enable the autoreload 
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	// Clear TIMx update pending flag
    TIM_ClearFlag(TIM2, TIM_FLAG_Update); 
    // Enable TIMx Update interrupt 
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	// TIMx disable counter
    TIM_Cmd(TIM2, ENABLE);   

    
	return e_Feedback_Success;
}


void OutIn_Motor_PWM_Init(UINT32 Arr, UINT32 Psc)
{
	// TIM3_CH3
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(OUTIN_MOTOR_PWM_TIM_SRC,ENABLE);
	RCC_AHB1PeriphClockCmd(OUTIN_MOTOR_CLK_SRC, ENABLE);
	GPIO_PinAFConfig(OUTIN_MOTOR_CLK_PORT,OUTIN_MOTOR_CLK_PIN_AF, OUTIN_MOTOR_CLK_PORT_AF); 

	// PC8
	GPIO_InitStructure.GPIO_Pin = OUTIN_MOTOR_CLK_PIN;          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(OUTIN_MOTOR_CLK_PORT, &GPIO_InitStructure);        
	GPIO_SetBits(OUTIN_MOTOR_CLK_PORT, OUTIN_MOTOR_CLK_PIN);
	
	TIM_DeInit(OUTIN_MOTOR_PWM_TIM);
	TIM_TimeBaseStructure.TIM_Prescaler=Psc;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=Arr; 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(OUTIN_MOTOR_PWM_TIM,&TIM_TimeBaseStructure);

	// pwm
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_Low; 
	TIM_OC3Init(OUTIN_MOTOR_PWM_TIM, &TIM_OCInitStructure);  

	TIM_OC3PreloadConfig(OUTIN_MOTOR_PWM_TIM, TIM_OCPreload_Enable); 
	TIM_ARRPreloadConfig(OUTIN_MOTOR_PWM_TIM,ENABLE);
	TIM_Cmd(OUTIN_MOTOR_PWM_TIM, ENABLE);  //
}


// Out_In Motor
void OutIn_Motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	//RCC_AHB1PeriphClockCmd(OUTIN_MOTOR_EN_SRC|OUTIN_MOTOR_DIR_SRC|OUTIN_MOTOR_CLK_SRC, ENABLE);
	
	// enable
	EVAL_OutputInit(O_OUTIN_MOTOR_EN);
	EVAL_OutputClr(O_OUTIN_MOTOR_EN);
	//EVAL_OutputSet(O_OUTIN_MOTOR_EN);
	// dir
	EVAL_OutputInit(O_OUTIN_MOTOR_DIR);
	EVAL_OutputClr(O_OUTIN_MOTOR_DIR);
	// clk
	OutIn_Motor_PWM_Init(OUTIN_MOTOR_PWM_TIM_ARR, OUTIN_MOTOR_PWM_TIM_PSC); //84M/42=2M, 1M/25000=59.52
}


void OutIn_Motor_Speed_Set(UINT16 nSpeed) 
{
	if(nSpeed > OUTIN_MOTOR_PWM_LEVEL_HIGHEST){
		TIM_SetCompare3(OUTIN_MOTOR_PWM_TIM, OUTIN_MOTOR_PWM_LEVEL_HIGHEST);
	}else{
		TIM_SetCompare3(OUTIN_MOTOR_PWM_TIM, nSpeed);
	}
}

//e_Dir_Pos=Out, e_Dir_Neg=In
void OutIn_Motor_Run(UINT8 nDir, UINT16 nFreq)
{
	//if(nFreq > OUTIN_MOTOR_PWM_LEVEL_HIGHEST) return;
	//if(nDir != e_Dir_Neg || nDir != e_Dir_Pos) return;
	
	if(nDir == e_Dir_Pos){
		OutIn_Motor_Out();// out
	}else if(nDir == e_Dir_Neg){
		OutIn_Motor_In(); // in
	}
	//Pump_init();
	OutIn_Motor_Speed_Set(nFreq);
}

void OutIn_Motor_Enable(void)
{
	EVAL_OutputSet(O_OUTIN_MOTOR_EN);
}

void OutIn_Motor_Disable(void)
{
	EVAL_OutputClr(O_OUTIN_MOTOR_EN);
}

void OutIn_Motor_Out(void) //out
{
	EVAL_OutputClr(O_OUTIN_MOTOR_DIR);
}

void OutIn_Motor_In(void) // in
{
	EVAL_OutputSet(O_OUTIN_MOTOR_DIR);
}

void Turn_CLK_Set(void)
{
	GPIO_SetBits(TURN_MOTOR_CLK_PORT, TURN_MOTOR_CLK_PIN);
}

void Turn_CLK_Reset(void)
{
	GPIO_ResetBits(TURN_MOTOR_CLK_PORT, TURN_MOTOR_CLK_PIN);
}


#if TURN_MOTOR_USE_TIM_PWM
//void Turn_Motor_PWM_Init(UINT32 Arr, UINT32 Psc)
//{
//	// TIM4_CH3
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;

//	RCC_APB1PeriphClockCmd(TURN_MOTOR_PWM_TIM_SRC, ENABLE);
//	RCC_AHB1PeriphClockCmd(TURN_MOTOR_CLK_SRC, ENABLE);
//	GPIO_PinAFConfig(TURN_MOTOR_CLK_PORT, TURN_MOTOR_CLK_PIN_AF, TURN_MOTOR_CLK_PORT_AF); 

//	// PD14
//	GPIO_InitStructure.GPIO_Pin = TURN_MOTOR_CLK_PIN;          
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(TURN_MOTOR_CLK_PORT, &GPIO_InitStructure);        
//	GPIO_SetBits(TURN_MOTOR_CLK_PORT, TURN_MOTOR_CLK_PIN);
//	
//	TIM_DeInit(TURN_MOTOR_PWM_TIM);
//	TIM_TimeBaseStructure.TIM_Prescaler=Psc;  
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
//	TIM_TimeBaseStructure.TIM_Period=Arr; 
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	TIM_TimeBaseInit(TURN_MOTOR_PWM_TIM,&TIM_TimeBaseStructure);

//	// pwm
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_Low; 
//	TIM_OC3Init(TURN_MOTOR_PWM_TIM, &TIM_OCInitStructure);  

//	TIM_OC3PreloadConfig(TURN_MOTOR_PWM_TIM, TIM_OCPreload_Enable); 
//	TIM_ARRPreloadConfig(TURN_MOTOR_PWM_TIM, ENABLE);
//	TIM_Cmd(TURN_MOTOR_PWM_TIM, ENABLE);  //
//}
#endif


// Out_In Motor
void Turn_Motor_Init(void)
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
	//RCC_AHB1PeriphClockCmd(OUTIN_MOTOR_EN_SRC|OUTIN_MOTOR_DIR_SRC|OUTIN_MOTOR_CLK_SRC, ENABLE);
	
	// enable
	EVAL_OutputInit(O_TURN_MOTOR_EN);
	EVAL_OutputClr(O_TURN_MOTOR_EN);
	// dir
	EVAL_OutputInit(O_TURN_MOTOR_DIR);
	EVAL_OutputClr(O_TURN_MOTOR_DIR);
	// clk
#if TURN_MOTOR_USE_TIM_PWM
	Turn_Motor_PWM_Init(TURN_MOTOR_PWM_TIM_ARR, TURN_MOTOR_PWM_TIM_PSC); //84M/42=2M, 1M/25000=59.52
#else
	
	GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(TURN_MOTOR_CLK_SRC, ENABLE);  // AHB1
    // 2. configure the GPIO pin
	GPIO_InitStructure.GPIO_Pin = TURN_MOTOR_CLK_PIN; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
    GPIO_Init(TURN_MOTOR_CLK_PORT, &GPIO_InitStructure);  // init gpio
	GPIO_ResetBits(TURN_MOTOR_CLK_PORT, TURN_MOTOR_CLK_PIN);
#endif
}


#if TURN_MOTOR_USE_TIM_PWM
//void Turn_Motor_Speed_Set(UINT16 nSpeed) 
//{
//	if(nSpeed > TURN_MOTOR_PWM_LEVEL_HIGHEST){
//		TIM_SetCompare3(TURN_MOTOR_PWM_TIM, TURN_MOTOR_PWM_LEVEL_HIGHEST);
//	}else{
//		TIM_SetCompare3(TURN_MOTOR_PWM_TIM, nSpeed);
//	}
//}



//void Turn_Motor_Run(UINT8 nDir, UINT16 nFreq)
//{
//	if(nFreq > TURN_MOTOR_PWM_LEVEL_HIGHEST) return;
//	if(nDir != e_Dir_Neg && nDir != e_Dir_Pos) return;
//	
//	if(nDir == e_Dir_Pos){
//		EVAL_OutputSet(O_TURN_MOTOR_DIR);// click
//	}else if(nDir == e_Dir_Neg){
//		EVAL_OutputClr(O_TURN_MOTOR_DIR); //  auti-click
//	}
//	Turn_Motor_Speed_Set(nFreq);
//}

//// e_Dir_Pos=anti-click, e_Dir_Neg=click
//UINT8 Turn_Motor_Reset(void)
//{
//	 
//	if(EN_OPEN == EVAL_GetInputStatus(I_LED_OC_RST))
//	{
//		EVAL_OutputSet(O_TURN_MOTOR_EN);
//		Turn_Motor_Run(e_Dir_Pos, TURN_MOTOR_PWM_LEVEL_BEST);
//		while(EN_OPEN == EVAL_GetInputStatus(I_LED_OC_RST))
//		{
//			if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_RST)) break;
//			//printf("RST OC=%d\r\n", i);
//			IT_SYS_DlyMs(1);
//		}
//		EVAL_OutputClr(O_TURN_MOTOR_EN);
//		Turn_Motor_Run(e_Dir_Pos, TURN_MOTOR_PWM_LEVEL_CLOSE);
//	}
//}

//UINT8 Turn_Motor_Goto_Postion(UINT8 nDir, UINT32 nOCTimes)
//{
//	IO_ UINT8 i = 0;
//	
//	EVAL_OutputSet(O_TURN_MOTOR_EN);
//	Turn_Motor_Run(nDir, TURN_MOTOR_PWM_LEVEL_BEST);
//	while(1)
//	{
//		if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_SEL))
//		{
//			i++;
//			printf("i=%d ", i);
//			if(i < nOCTimes) 
//			{
//				if((DELAY_MOTOR) > 1000)
//				{
//					IT_SYS_DlyMs(DELAY_MOTOR/1000);
//					Delay_US(DELAY_MOTOR%1000);
//				}
//			}
//		}		
//		if(i >= nOCTimes) break;
//	}
//	EVAL_OutputClr(O_TURN_MOTOR_EN);
//	Turn_Motor_Run(nDir, TURN_MOTOR_PWM_LEVEL_CLOSE);
//	printf(", Real OC T=%d, Hope OC T=%d\r\n", i, (int)nOCTimes);
//	if(i > nOCTimes) 
//	{
//		return e_Feedback_Fail;
//	}
//	return e_Feedback_Success;
//}



//UINT8 Turn_Motor_Select_LED(UINT8 nIndex)
//{
//	IO_ UINT8 i = 0;
//	
//	//Turn_Motor_Reset();
//	if((EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_RST)) && (EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_SEL))) i = 1;
//	switch(nIndex)
//	{
//		case EN_LED0: //
//		{
//			printf("LED0 not use\r\n");
//		}
//		break;
//		case EN_LED1: // 
//		{
//			printf("LED1 not use\r\n");
//		}
//		break;
//		case EN_LED2: // back  // e_Dir_Neg=anti-click  
//		{
//			Turn_Motor_Goto_Postion(e_Dir_Pos, EN_LED2_TRIGGER_OC_TIMES + i);
//		}
//		break;
//		case EN_LED3: // 840nm CRP  // e_Dir_Neg=auti-click
//		{
//			if(i != 1)
//			{
//				Turn_Motor_Goto_Postion(e_Dir_Neg, EN_LED3_TRIGGER_OC_TIMES);
//			}
//		}
//		break;
//		case EN_LED4: // 580nm  // e_Dir_Pos=click
//		{
//			Turn_Motor_Goto_Postion(e_Dir_Pos, EN_LED4_TRIGGER_OC_TIMES + i);
//		}
//		break;
//		case EN_LED5: // 640nm red  // e_Dir_Neg=click
//		{
//			Turn_Motor_Goto_Postion(e_Dir_Pos, EN_LED5_TRIGGER_OC_TIMES + i);
//		}
//		break;
//		case EN_LED6: // 525nm green HGB  // e_Dir_Pos=click
//		{
//			Turn_Motor_Goto_Postion(e_Dir_Pos, EN_LED6_TRIGGER_OC_TIMES + i);
//		}
//		break;
//		case EN_LED7: // 340nm   e_Dir_Neg=auti-click
//		{
//			Turn_Motor_Goto_Postion(e_Dir_Neg, EN_LED7_TRIGGER_OC_TIMES + i);
//		}
//		break;
//		default:break;	
//	}
//}

#else

void Turn_Motor_Enable(void)
{
	EVAL_OutputSet(O_TURN_MOTOR_EN);
}

void Turn_Motor_Disable(void)
{
	EVAL_OutputClr(O_TURN_MOTOR_EN);
}

void Turn_Motor_Dir(UINT8 nDir)
{
	//if(nDir != e_Dir_Neg && nDir != e_Dir_Pos) return;
	
	if(nDir == e_Dir_Pos){
		EVAL_OutputSet(O_TURN_MOTOR_DIR);// clockwise
	}else if(nDir == e_Dir_Neg){
		EVAL_OutputClr(O_TURN_MOTOR_DIR); //  auti-clockwise
	}
}


void Turn_Motor_One_Step(UINT16 nUpTime, UINT16 nDownTime)
{
	Turn_CLK_Set();
	if(nUpTime/1000 >= 1)
	{
		IT_SYS_DlyMs(nUpTime/1000);
		Delay_US(nUpTime%1000);
	}else{
		Delay_US(nUpTime);
	}
	
	Turn_CLK_Reset();
	if(nDownTime/1000 >= 1)
	{
		IT_SYS_DlyMs(nDownTime/1000);
		Delay_US(nDownTime%1000);
	}else{
		Delay_US(nDownTime);
	}
}

void Turn_Motor_Run(UINT32 nSteps)
{
	UINT32 i = 0;
	Turn_Motor_Enable();
	for(i = 0; i < nSteps; i++)
	{
		if(nSteps > TURN_MOTOR_DISCARD_NUM)
		{
			if((i > nSteps -TURN_MOTOR_DISCARD_NUM) && (i < nSteps + TURN_MOTOR_DISCARD_NUM))
			{
				if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_SEL))
				{
					IT_SYS_DlyMs(1);
					if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_SEL)) break;
				}
			}
		}else{
			if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_SEL))
			{
				IT_SYS_DlyMs(1);
				if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_SEL)) break;
			}
		}
		Turn_Motor_One_Step(TURN_MOTOR_UP_TIME, TURN_MOTOR_DOWN_TIME);
	}
	Turn_Motor_Disable();

}

UINT8 Turn_Motor_Goto_Postion(UINT8 nDir, UINT32 nSteps)
{
	Turn_Motor_Dir(nDir);
	Turn_Motor_Run(nSteps);
}

void Turn_Motor_Reset()
{
	UINT32 i = 0;
	
	Turn_Motor_Enable();
//	if(g_Turn_Position == EN_POSITION_LED4 || g_Turn_Position == EN_POSITION_LED5 || g_Turn_Position == EN_POSITION_LED6)
//	{
		Turn_Motor_Dir(e_Dir_Neg);
//	}else{
//		Turn_Motor_Dir(e_Dir_Pos);
//	}
	//
	printf("Turn Motor Reset Start, Step=%d\r\n", (int)i);
	if(EN_OPEN == EVAL_GetInputStatus(I_LED_OC_RST))
	{
		for(i = 0; i < TURN_MOTOR_RESET_MAX_STEPS; i++)
		{
			if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_RST))
			{
				IT_SYS_DlyMs(1);
				if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_RST)) break;
			}				
			Turn_Motor_One_Step(TURN_MOTOR_UP_TIME, TURN_MOTOR_DOWN_TIME);
		}
	}
	Turn_Motor_Disable();
	printf("Turn Motor Reset End, Step=%d\r\n", (int)i);
}


void Turn_Motor_Pass_Kong(void)
{
	UINT32 i = 0;
	for(i = 0; i < TURN_MOTOR_DISCARD_NUM; i++)
	{
		Turn_Motor_One_Step(TURN_MOTOR_UP_TIME, TURN_MOTOR_DOWN_TIME);
	}
}

UINT8 Turn_Motor_Select_LED(UINT8 nIndex)
{
	Turn_Motor_Reset();
	IT_SYS_DlyMs(200);  
	if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_RST) && EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_RST)) 
	{
		if(nIndex == EN_LED3) return 0;
		//Turn_Motor_Pass_Kong();
	}
	switch(nIndex)
	{
		case EN_LED0:
		{
//			printf("LED0 Select Step=%d\r\n", EN_LED0_SELECT_STEP);
//			Turn_Motor_Goto_Postion(e_Dir_Neg, EN_LED0_SELECT_STEP);  
//			g_Turn_Position = EN_POSITION_LED0;
		}
		break;
		case EN_LED1:
		{
//			printf("LED1 Select Step=%d\r\n", EN_LED1_SELECT_STEP);
//			Turn_Motor_Goto_Postion(e_Dir_Pos, EN_LED1_SELECT_STEP);
//			g_Turn_Position = EN_POSITION_LED1;
		}
		break;
		case EN_LED2:
		{
			printf("LED2 Select Step=%d\r\n", EN_LED2_SELECT_STEP);
			Turn_Motor_Goto_Postion(e_Dir_Pos, EN_LED2_SELECT_STEP);
			g_Turn_Position = EN_POSITION_LED2;
		}
		break;
		case EN_LED3:
		{
			printf("LED3 Select Step=%d\r\n", EN_LED3_SELECT_STEP);
			Turn_Motor_Goto_Postion(e_Dir_Pos, EN_LED3_SELECT_STEP);
			g_Turn_Position = EN_POSITION_LED3;
		}
		break;
		case EN_LED4:
		{
			printf("LED4 Select Step=%d\r\n", EN_LED4_SELECT_STEP);
			Turn_Motor_Goto_Postion(e_Dir_Neg, EN_LED4_SELECT_STEP);
			g_Turn_Position = EN_POSITION_LED4;
		}
		break;
		case EN_LED5:
		{
			printf("LED5 Select Step=%d\r\n", EN_LED5_SELECT_STEP);
			Turn_Motor_Goto_Postion(e_Dir_Neg, EN_LED5_SELECT_STEP);
			g_Turn_Position = EN_POSITION_LED5;
		}
		break;
		case EN_LED6:
		{
			printf("LED6 Select Step=%d\r\n", EN_LED6_SELECT_STEP);
			Turn_Motor_Goto_Postion(e_Dir_Neg, EN_LED6_SELECT_STEP);
			g_Turn_Position = EN_POSITION_LED6;
		}
		break;
		case EN_LED7:
		{
			printf("LED7 Select Step=%d\r\n", EN_LED7_SELECT_STEP);
			Turn_Motor_Goto_Postion(e_Dir_Pos, EN_LED7_SELECT_STEP);
			g_Turn_Position = EN_POSITION_LED7;
		}
		break;
		default:break;
	}
}

#endif










#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
int _sys_exit(int x) 
{ 
	x = x; 
	return 0;
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif

void Debug_Uart_Init(void)
{
	COM_Init();
}


void COM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    // 1. enable GPIO clock 
	RCC_AHB1PeriphClockCmd(COM1_TX_GPIO_CLK | COM1_RX_GPIO_CLK | COM1_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(COM1_CLK, ENABLE);

	// 3. configure AFIO  
	GPIO_PinAFConfig(COM1_TX_GPIO_PORT, COM1_AF_TX_PIN_SOURCE, COM1_AF_UART); // tx-pin
	GPIO_PinAFConfig(COM1_RX_GPIO_PORT, COM1_AF_RX_PIN_SOURCE, COM1_AF_UART); // rx-pin
    // 4. IO configuration 
    GPIO_InitStructure.GPIO_Pin = COM1_RX_PIN | COM1_TX_PIN; // tx-pin and rx-pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // AF model
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 50MHz 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // push output
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       // 
    GPIO_Init(COM1_TX_GPIO_PORT,&GPIO_InitStructure);
	// uart init
	USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    // 
    USART_Init(COMM1, &USART_InitStructure);  
    // 6. Enable USART
    USART_Cmd(COMM1, ENABLE);
//	// 7. enable Receive and Transmit interrupts 
//    USART_ITConfig(COMM1, USART_IT_TC, ENABLE);      //  TX_INT = 1;
//    USART_ITConfig(COMM1, USART_IT_RXNE, ENABLE);    //  RX_INT = 1;	
//	// 8. configure and enable USART interrupt  
//    NVIC_InitStructure.NVIC_IRQChannel = COM1_IRQn;       
//    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&NVIC_InitStructure);	

}


void ADC1_DMA_Config()
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_DeInit(DMA2_Stream0);
	while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
	// DMA SET
	DMA_InitStructure.DMA_Channel 				= DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&ADC1->DR,
	DMA_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)g_ADC1_Buffer;
	DMA_InitStructure.DMA_DIR					= DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize			= ADC1_BUFFER_LEN;
	DMA_InitStructure.DMA_PeripheralInc			= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize		= DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode					= DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority				= DMA_Priority_High; //
	DMA_InitStructure.DMA_FIFOMode				= DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold			= DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst			= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
	
	//DMA_DoubleBufferModeConfig(DMA2_Stream0,(uint32_t)&g_ADC_Buffer_2,DMA_Memory_0);//DMA_Memory_0?????
	//DMA_DoubleBufferModeCmd(DMA2_Stream0,ENABLE);
	DMA_Init(DMA2_Stream0, &DMA_InitStructure); 
	
	// NVIC
	DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_HTIF0);
	DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
	DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
	DMA_ITConfig(DMA2_Stream0,DMA_IT_HT,ENABLE);	
		
	NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream0_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;                      
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
	//DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
	//while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
	DMA_Cmd(DMA2_Stream0, ENABLE);
}

#define WBC_USE_PA5    	1
// ADC1_IN5  PA5  WBC
void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_DeInit();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	// PA5
	#if WBC_USE_PA5
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5;//GPIO_Pin_5;//GPIO_Pin_5, PA5
	#else
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_6;	
	#endif
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//ADC1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
	// Common Set
	ADC_Cmd(ADC1, DISABLE);
	ADC_CommonInitStructure.ADC_Mode	= ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInit(&ADC_CommonInitStructure);
	// ADC Set
	ADC_InitStructure.ADC_Resolution	= ADC_Resolution_12b;
//#if DOUBLE_ADC_CHANNEL
//	ADC_InitStructure.ADC_ScanConvMode  = DISABLE;
//	ADC_InitStructure.ADC_NbrOfConversion = 2;
//#else
	ADC_InitStructure.ADC_ScanConvMode  = ENABLE;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
//#endif
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC_DataAlign_Left;//ADC_DataAlign_Right;
	ADC_Init(ADC1, &ADC_InitStructure);	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC1_DMA_Config();
	ADC_DMACmd(ADC1, ENABLE);
	
	#if WBC_USE_PA5
		ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles); //ADC_SampleTime_3Cycles
	#else
		ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_3Cycles); //ADC_SampleTime_3Cycles
	#endif
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	//ADC_SoftwareStartConv(ADC1);
}


//FlagStatus DMA_GetFlagStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG)
//uint16_t DMA_GetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Streamx)
// ADC1 DMA irq
void DMA2_Stream0_IRQHandler(void) 
{
	// half DMA_GetFlagStatus
	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_HTIF0) == SET)  
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_HTIF0);
		ADC1_Status.nSFlag = 1;
		ADC1_Status.nID++;
	}
	
	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET)  
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		ADC1_Status.nSFlag = 2;
		ADC1_Status.nID++;
	}
}

// ADC2 DMA irq
void DMA2_Stream3_IRQHandler(void) 
{
	// half DMA_GetFlagStatus
	if (DMA_GetITStatus(DMA2_Stream3, DMA_IT_HTIF3) == SET)  
	{
		DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_HTIF3);
		ADC2_Status.nSFlag = 1;
		ADC2_Status.nID++;
	}

	if (DMA_GetITStatus(DMA2_Stream3, DMA_IT_TCIF3) == SET)  
	{
		DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_TCIF3);
		ADC2_Status.nSFlag = 2;
		ADC2_Status.nID++;
	}
}

void ADC2_DMA_Config()
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_DeInit(DMA2_Stream3);
	while(DMA_GetCmdStatus(DMA2_Stream3) != DISABLE){}
	// DMA SET
	DMA_InitStructure.DMA_Channel 				= DMA_Channel_1;
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&ADC2->DR,
	DMA_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)g_ADC2_Buffer;
	DMA_InitStructure.DMA_DIR					= DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize			= ADC1_BUFFER_LEN;
	DMA_InitStructure.DMA_PeripheralInc			= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize		= DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode					= DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority				= DMA_Priority_High; //
	DMA_InitStructure.DMA_FIFOMode				= DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold			= DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst			= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream3, &DMA_InitStructure); 
		
	// NVIC
	DMA_ClearITPendingBit(DMA2_Stream3,DMA_IT_HTIF3);
	DMA_ClearITPendingBit(DMA2_Stream3,DMA_IT_TCIF3);
	DMA_ITConfig(DMA2_Stream3,DMA_IT_TC,ENABLE);
	DMA_ITConfig(DMA2_Stream3,DMA_IT_HT,ENABLE);	
		
	NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;                      
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
	//DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
	//while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
	DMA_Cmd(DMA2_Stream3, ENABLE);
}

// ADC2_IN8  PB0  RBC/PLT,  (if RBC/PLT depart, PA6-RBC,PB0-PLT)
void ADC2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_DeInit();
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	// PB0
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//ADC2
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);
	// Common Set
	ADC_Cmd(ADC2, DISABLE);
	//ADC_CommonInitStructure.ADC_Mode	= ADC_DualMode_RegSimult;									ADC_DualMode_RegSimult
	ADC_CommonInitStructure.ADC_Mode	= ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInit(&ADC_CommonInitStructure);
	// ADC Set
	ADC_InitStructure.ADC_Resolution	= ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode  = ENABLE;//DISABLE;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC_DataAlign_Left;//ADC_DataAlign_Right;
	ADC_Init(ADC2, &ADC_InitStructure);	
	ADC_Cmd(ADC2, ENABLE);
	
	ADC2_DMA_Config();
	ADC_DMACmd(ADC2, ENABLE);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_3Cycles); //ADC_SampleTime_3Cycles
	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
	//ADC_SoftwareStartConv(ADC2);
}


//u16 Get_Adc(UINT8 nCh, UINT8 nTime)   
//{
//	UINT8 i = 0;
//	UINT32 nVal = 0;
//	
//	for(i = 0; i < nTime; i++)
//	{
//		ADC_RegularChannelConfig(ADC1, nCh, 1, ADC_SampleTime_480Cycles );	    
//		ADC_SoftwareStartConv(ADC1);		
//		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
//		nVal += ADC_GetConversionValue(ADC1);	
//	}
//	nVal /= nTime;
//	return nVal;
//}

void ADC3_GPIO_Init(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	#if PRESS_SENSOR_ADC_TYPE
		RCC_AHB1PeriphClockCmd(TEMP_ADC_SRC|CUR_56V_ADC_SRC|XK_ADC_SRC| \
				LED_CUR_ADC_SRC|ELEC_ADC_SRC|SIG1_ADC_SRC|SIG2_ADC_SRC| \
				CUR12N_ADC_SRC|CUR12P_ADC_SRC|PRESS_ADC_SRC, ENABLE);
	#else
		RCC_AHB1PeriphClockCmd(TEMP_ADC_SRC|CUR_56V_ADC_SRC|XK_ADC_SRC| \
				CUR12N_ADC_SRC|CUR12P_ADC_SRC|LED_CUR_ADC_SRC|ELEC_ADC_SRC|SIG1_ADC_SRC|SIG2_ADC_SRC, ENABLE);
	#endif
	// PF6_ADC3_IN4, LED_CUR
	GPIO_InitStructure.GPIO_Pin		= LED_CUR_ADC_PIN;  
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(LED_CUR_ADC_PORT, &GPIO_InitStructure);
	// PC3_ADC3_IN13, XK
	GPIO_InitStructure.GPIO_Pin		= XK_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(XK_ADC_PORT, &GPIO_InitStructure);
	// PF9_ADC3_IN7, 56V_CUR
	GPIO_InitStructure.GPIO_Pin		= CUR_56V_ADC_PIN;	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(CUR_56V_ADC_PORT, &GPIO_InitStructure);
	// PC0_ADC123_IN10, Temperature
	GPIO_InitStructure.GPIO_Pin		= TEMP_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(TEMP_ADC_PORT, &GPIO_InitStructure);
	//elec ADC3_IN6 PF8
	GPIO_InitStructure.GPIO_Pin		= ELEC_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(ELEC_ADC_PORT, &GPIO_InitStructure);
	// SIG1 ==> PF7_ADC3_IN5 ,CRP
	GPIO_InitStructure.GPIO_Pin		= SIG1_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(SIG1_ADC_PORT, &GPIO_InitStructure);
	// SIG2 ==> PA0_ADC3_IN0 ,HGB
	GPIO_InitStructure.GPIO_Pin		= SIG2_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(SIG2_ADC_PORT, &GPIO_InitStructure);
	
	//  PA3_ADC3_IN3 ,12V_N
	GPIO_InitStructure.GPIO_Pin		= CUR12N_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(CUR12N_ADC_PORT, &GPIO_InitStructure);
	
	//  PF10_ADC3_IN8 ,12V_N
	GPIO_InitStructure.GPIO_Pin		= CUR12P_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(CUR12P_ADC_PORT, &GPIO_InitStructure);
	
	#if PRESS_SENSOR_ADC_TYPE
		GPIO_InitStructure.GPIO_Pin		= PRESS_ADC_PIN; // PC2_ADC123_IN12 , Press
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_Init(PRESS_ADC_PORT, &GPIO_InitStructure);
	#endif	
}

#if ADC3_INIT_WITH_DMA
	void ADC3_DMA_Config(void)
	{
		DMA_InitTypeDef DMA_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
		DMA_DeInit(DMA2_Stream1);
		while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}
		// DMA SET
		DMA_InitStructure.DMA_Channel 				= DMA_Channel_2;
		DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&ADC3->DR,
		DMA_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)g_ADC3_Value;
		DMA_InitStructure.DMA_DIR					= DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize			= ADC3_CHECK_NUM;
		DMA_InitStructure.DMA_PeripheralInc			= DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize		= DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode					= DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority				= DMA_Priority_Medium; //
		DMA_InitStructure.DMA_FIFOMode				= DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold			= DMA_FIFOThreshold_Full;
		DMA_InitStructure.DMA_MemoryBurst			= DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
		
		//DMA_DoubleBufferModeConfig(DMA2_Stream0,(uint32_t)&g_ADC_Buffer_2,DMA_Memory_0);//DMA_Memory_0?????
		//DMA_DoubleBufferModeCmd(DMA2_Stream0,ENABLE);
		DMA_Init(DMA2_Stream1, &DMA_InitStructure); 
			
		// NVIC
	//	DMA_ClearITPendingBit(DMA2_Stream2,DMA_IT_HTIF0);
	//	DMA_ClearITPendingBit(DMA2_Stream2,DMA_IT_TCIF0);
	//	DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
	//	DMA_ITConfig(DMA2_Stream2,DMA_IT_HT,ENABLE);	
			
	//	NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream0_IRQn; 
	//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;   
	//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;                      
	//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	//	NVIC_Init(&NVIC_InitStructure);
		//DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
		//DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
		//while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
		DMA_Cmd(DMA2_Stream1, ENABLE);
	}

	// ADC3 with DMA func
	void ADC3_Init(void)
	{
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
		ADC_InitTypeDef ADC_InitStructure;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
		
		ADC3_GPIO_Init();
		
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);
		
		// Common Set
		ADC_CommonInitStructure.ADC_Mode	= ADC_Mode_Independent;
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
		ADC_CommonInit(&ADC_CommonInitStructure);
		// ADC Set
		ADC_InitStructure.ADC_Resolution	= ADC_Resolution_12b;
		ADC_InitStructure.ADC_ScanConvMode  = ENABLE;//DISABLE;
		ADC_InitStructure.ADC_NbrOfConversion = ADC3_CHECK_NUM;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;// ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConv_T1_CC1;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC_DataAlign_Left;//ADC_DataAlign_Right;
		ADC_Init(ADC3, &ADC_InitStructure);	
		//ADC_Cmd(ADC3, ENABLE);
		
		ADC3_DMA_Config();
		//ADC_DMACmd(ADC3, ENABLE);		// 
		
		ADC_RegularChannelConfig(ADC3, LED_CUR_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);  // LED cur
		ADC_RegularChannelConfig(ADC3, CUR_56V_ADC_CHANNEL, 2, ADC_SampleTime_3Cycles);  // 56v cur
		ADC_RegularChannelConfig(ADC3, TEMP_ADC_CHANNEL, 3, ADC_SampleTime_3Cycles);     // temperature
		ADC_RegularChannelConfig(ADC3, XK_ADC_CHANNEL, 4, ADC_SampleTime_3Cycles); 		 // XK
		ADC_RegularChannelConfig(ADC3, SIG1_ADC_CHANNEL, 5, ADC_SampleTime_3Cycles);  	 // CRP
		ADC_RegularChannelConfig(ADC3, SIG2_ADC_CHANNEL, 6, ADC_SampleTime_3Cycles);  	 // HGB
		#if PRESS_SENSOR_ADC_TYPE
			ADC_RegularChannelConfig(ADC3, PRESS_ADC_CHANNEL, 7, ADC_SampleTime_3Cycles);	 // press
		#endif

		ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE); 
		ADC_DMACmd(ADC3, ENABLE);
		ADC_Cmd(ADC3, ENABLE);
		ADC_SoftwareStartConv(ADC3); 
	}
	
#else 
	// ADC3 without DMA func
	void ADC3_Init(void)
	{
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
		ADC_InitTypeDef ADC_InitStructure;
		
		//ADC_DeInit();
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
		//
		ADC3_GPIO_Init();
		
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	 
//	    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	
		// Common Set
		ADC_CommonInitStructure.ADC_Mode	= ADC_Mode_Independent;
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //ADC_DMAAccessMode_1; //ADC_DMAAccessMode_Disabled
		ADC_CommonInit(&ADC_CommonInitStructure);
		// ADC Set
		ADC_InitStructure.ADC_Resolution	= ADC_Resolution_12b;
		ADC_InitStructure.ADC_ScanConvMode  = DISABLE;//ENABLE;//DISABLE;
		ADC_InitStructure.ADC_NbrOfConversion = 1;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//DISABLE;//ENABLE;// ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConvEdge_None;// ADC_ExternalTrigConv_T1_CC1; // ADC_ExternalTrigConvEdge_None
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC_DataAlign_Left;//ADC_DataAlign_Right;
		ADC_Init(ADC3, &ADC_InitStructure);	
		ADC_Cmd(ADC3, ENABLE);
		
		
//		ADC_RegularChannelConfig(ADC3, LED_CUR_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);  // LED cur
//		ADC_RegularChannelConfig(ADC3, CUR_56V_ADC_CHANNEL, 2, ADC_SampleTime_3Cycles);  // 56v cur
//		ADC_RegularChannelConfig(ADC3, TEMP_ADC_CHANNEL, 3, ADC_SampleTime_3Cycles);     // temperature
//		ADC_RegularChannelConfig(ADC3, XK_ADC_CHANNEL, 4, ADC_SampleTime_3Cycles); 		 // XK
//		ADC_RegularChannelConfig(ADC3, SIG1_ADC_CHANNEL, 5, ADC_SampleTime_3Cycles);  	 // CRP
//		ADC_RegularChannelConfig(ADC3, SIG2_ADC_CHANNEL, 6, ADC_SampleTime_3Cycles);  	 // HGB
//		#if PRESS_SENSOR_ADC_TYPE
//			ADC_RegularChannelConfig(ADC3, PRESS_ADC_CHANNEL, 7, ADC_SampleTime_3Cycles);	 // press
//		#endif


		//ADC2_DMA_Config();
		//ADC_DMACmd(ADC3, ENABLE);
		
		//ADC_RegularChannelConfig(ADC3, ADC_Channel_4, 1, ADC_SampleTime_480Cycles); //ADC_SampleTime_3Cycles
		//ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_480Cycles); //ADC_SampleTime_3Cycles
		
		//ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE); 
	  //ADC_DMACmd(ADC3, ENABLE);
		//ADC_Cmd(ADC3, ENABLE);
		//ADC_SoftwareStartConv(ADC3); 
	}
	
#endif //ADC3_INIT_WITH_DMA

	
void Disable_ADC(EN_TypeADC eType)
{
	if(eType == EN_ADC1){
		//ADC_Cmd(ADC1, DISABLE);
		ADC_DMACmd(ADC1, DISABLE);
		DMA_Cmd(DMA2_Stream0, DISABLE);
		
	}else if(eType == EN_ADC2){
		ADC_Cmd(ADC2, DISABLE);
		ADC_DMACmd(ADC2, DISABLE);
		DMA_Cmd(DMA2_Stream3, DISABLE);
		
	}else if(eType == EN_ADC3){
		//
	}
}


void Reset_ADC_InitDataType(void)
{
	memset((void*)&ADC1_Status, 0, sizeof(ADC_Status_InitTypeDef));
	memset((void*)&ADC2_Status, 0, sizeof(ADC_Status_InitTypeDef));		
}


void Eable_ADC(EN_TypeADC eType)
{
	if(eType == EN_ADC1){
		Reset_ADC_InitDataType();
		ADC1_Init(); //APP_ADC_Init(EN_ADC1);
		ADC_SoftwareStartConv(ADC1);
	}else if(eType == EN_ADC2){
		Reset_ADC_InitDataType();
		//DMA_Cmd(DMA2_Stream3, DISABLE);
		ADC2_Init(); //APP_ADC_Init(EN_ADC2);
		ADC_SoftwareStartConv(ADC2);
	}else if(eType == EN_ADC3){
		
	}
}

UINT16 Get_ADC3_Channel_Value(UINT8 nIndex, UINT8 nCount)
{
	UINT32 nVal = 0;
	UINT8 i;
	
#if ADC3_INIT_WITH_DMA
	nVal = g_ADC3_Value[nIndex];
#else
	ADC_ClearFlag(ADC3,ADC_FLAG_EOC);
	ADC_Cmd(ADC3,ENABLE);
	for(i = 0; i < nCount; i++)
	{
		ADC_RegularChannelConfig(ADC3, g_ADC3_IN[nIndex], 1, ADC_SampleTime_3Cycles ); 
		ADC_SoftwareStartConv(ADC3);		
		while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
		nVal += ADC_GetConversionValue(ADC3);	
	}
	nVal /= nCount;
#endif
	return nVal;
	
}

//
UINT16 Get_Press_ADC(void)
{
	UINT16 nVal = 0;
	
#if ADC3_INIT_WITH_DMA
	nVal = g_ADC3_Value[EN_ADC_PRESS];
#else
	#if PRESS_SENSOR_ADC_TYPE
		nVal = Get_ADC3_Channel_Value(EN_ADC_PRESS, ADC_SMOOTH_NUM_5);
	#else
		// IIC API ...
		
	#endif
#endif
	return nVal;
}

//
UINT16 Get_XK_ADC(void)
{
	UINT16 nVal = 0;
	
#if ADC3_INIT_WITH_DMA
	nVal = g_ADC3_Value[EN_ADC_XK];
#else
	nVal = Get_ADC3_Channel_Value(EN_ADC_XK, ADC_SMOOTH_NUM_5);
#endif
	return nVal;
}

UINT16 Get_12V_N_ADC(void)
{
	UINT16 nVal = 0, i;
	
#if ADC3_INIT_WITH_DMA
	nVal = g_ADC3_Value[EN_ADC_12V_N];
#else
	nVal = Get_ADC3_Channel_Value(EN_ADC_12V_N, ADC_SMOOTH_NUM_5);
#endif
	return nVal;
}

UINT16 Get_12V_P_ADC(void)
{
	UINT16 nVal = 0, i;
	
#if ADC3_INIT_WITH_DMA
	nVal = g_ADC3_Value[EN_ADC_12V_P];
#else
	nVal = Get_ADC3_Channel_Value(EN_ADC_12V_P, ADC_SMOOTH_NUM_5);
#endif
	return nVal;
}

//
UINT16 Get_56V_Cur_ADC(void)
{
	UINT16 nVal = 0;
	
#if ADC3_INIT_WITH_DMA
	//nVal = g_ADC3_Value[EN_ADC_56V_CUR];
#else
	nVal = Get_ADC3_Channel_Value(EN_ADC_56V_CUR, ADC_SMOOTH_NUM_5);
#endif
	return nVal;
}

//
UINT16 Get_LED_Cur_ADC(void)
{
	UINT16 nVal = 0;
	
#if ADC3_INIT_WITH_DMA
	//nVal = g_ADC3_Value[EN_ADC_LED_CUR];
#else
	nVal = Get_ADC3_Channel_Value(EN_ADC_LED_CUR, ADC_SMOOTH_NUM_5);
#endif
	return nVal;
}

UINT16 Get_Temp_ADC(void)
{
	UINT16 nVal = 0, i;
	
#if ADC3_INIT_WITH_DMA
	//nVal = g_ADC3_Value[EN_ADC_LED_CUR];
#else
	nVal = Get_ADC3_Channel_Value(EN_ADC_TEMP, ADC_SMOOTH_NUM_5);
#endif
	return nVal;
}

//
UINT32 HW_Get_ADC_HGB(void)
{
	UINT32 nVal = 0;
	
#if USE_STM32F407_ONLY
	//nVal = Get_ADC3_Channel_Value(EN_ADC_HGB, ADC_SMOOTH_NUM_30);
	nVal = AD7799_Get_ADC_Value(AD7799_Get_Out_Data());
#else	
	nVal = HW_Get_ADC_Perip(0); // /* adc, 0=HGB,1=WBC vol value, 2=RBC(wbc backup,crp test), 3=press, */ 
	
#endif
	return nVal;
}

//
UINT32  HW_Get_ADC_CRP(void)
{
	UINT32  nVal = 0;
	
#if USE_STM32F407_ONLY
	//nVal = Get_ADC3_Channel_Value(EN_ADC_CRP, ADC_SMOOTH_NUM_30);
	nVal = AD7799_Get_ADC_Value(AD7799_Get_Out_Data());
#else	
	nVal = HW_Get_ADC_Perip(2);  /* adc, 0=HGB,1=WBC vol value, 2=RBC(wbc backup,crp test), 3=press, */ 
#endif
	return nVal;
}

//
UINT16  Get_HGB_Value(void)
{
	UINT16  nRet;
	UINT32  nVal;
	
	nVal = HW_Get_ADC_CRP();
	nRet = nVal*ADC_V_REF_VALUE_5/ADC_RESOLUTION_12;
	printf("HGB: ADC=%d, V=%d\r\n", (int)nVal, (int)nRet);
	return nRet;
}

//
UINT32  Get_CRP_Value(void)
{
	UINT32 nVal, nRet;
	
	nVal = HW_Get_ADC_CRP();
	nRet = nVal*ADC_V_REF_VALUE_5/ADC_RESOLUTION_24;
	//printf("CRP: ADC=%d, V=%d\r\n", (int)nVal, (int)nRet);
	return nRet;
}

	

void Press_Init(void)
{
	Press_I2C_Init();
}

// i2c interface init
void Press_I2C_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(PRESS_I2C_SCL_SCLCLK_SRC|PRESS_I2C_SDA_SCLCLK_SRC|PRESS_I2C_EOC_SRC, ENABLE);//使能GPIOB时钟
	// i2c scl
	GPIO_InitStructure.GPIO_Pin = PRESS_I2C_SCL_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PRESS_I2C_SCL_PORT, &GPIO_InitStructure);
	// i2c sda
	GPIO_InitStructure.GPIO_Pin = PRESS_I2C_SDA_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PRESS_I2C_SDA_PORT, &GPIO_InitStructure);
	// eoc status
	GPIO_InitStructure.GPIO_Pin = PRESS_I2C_EOC_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(PRESS_I2C_EOC_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(PRESS_I2C_EOC_PORT, PRESS_I2C_EOC_PIN);
	
	PRESS_I2C_SCL = 1;
	PRESS_I2C_SDA = 1;
}

void Press_I2C_Start(void)
{
	PRESS_I2C_SDA_OUT();     //sda线输出
	PRESS_I2C_SDA=1;
	Delay_US(4);	
	PRESS_I2C_SCL=1;
	Delay_US(4);
 	PRESS_I2C_SDA=0;//START:when CLK is high,DATA change form high to low 
	Delay_US(4);
}

void Press_I2C_Stop(void)
{
	PRESS_I2C_SDA_OUT();//sda线输出
	//PRESS_I2C_SCL=0;
	PRESS_I2C_SDA=0;
 	Delay_US(4); 
	PRESS_I2C_SCL=1;//STOP:when CLK is high DATA change form low to high
 	Delay_US(4); 
	PRESS_I2C_SDA=1;//发送I2C总线结束信号 
}

UINT8 Press_I2C_Wait_Ack(void)
{
	u8 ucErrTime=0;
	
	if(PRESS_I2C_EOC_STATUS == 0) // 0: not ready, 1: ready
	{
		Delay_US(10);
		if(PRESS_I2C_EOC_STATUS == 0)
		{
			Delay_US(10);
		}
	}
	PRESS_I2C_SDA_IN();      //SDA设置为输入  
	PRESS_I2C_SDA=1;Delay_US(1);	   
	PRESS_I2C_SCL=1;Delay_US(1);	 
	while(PRESS_I2C_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			Press_I2C_Stop();
			return 1;
		}
	}
	PRESS_I2C_SCL=0;//时钟输出0 	   
	return 0; 
}

void Press_I2C_Ack(void)
{
	PRESS_I2C_SCL=0;
	PRESS_I2C_SDA_OUT();
	PRESS_I2C_SDA=0;
	Delay_US(2);
	PRESS_I2C_SCL=1;
	Delay_US(2);
	PRESS_I2C_SCL=0;
}

void Press_I2C_NAck(void)
{
	PRESS_I2C_SCL=0;
	PRESS_I2C_SDA_OUT();
	PRESS_I2C_SDA=1;
	Delay_US(2);
	PRESS_I2C_SCL=1;
	Delay_US(2);
	PRESS_I2C_SCL=0;
}

void Press_I2C_Send_Byte(UINT8 nVal)
{
    u8 t;   
	
	if(PRESS_I2C_EOC_STATUS == 0) // 0: not ready, 1: ready
	{
		Delay_US(10);
		if(PRESS_I2C_EOC_STATUS == 0)
		{
			Delay_US(10);
		}
	}
	PRESS_I2C_SDA_OUT(); 	    
    PRESS_I2C_SCL=0;//拉低时钟开始数据传输
	__disable_irq();
    for(t=0;t<8;t++)
    {              
        PRESS_I2C_SDA=(nVal&0x80)>>7;
        nVal<<=1; 	  
		Delay_US(2);   //对TEA5767这三个延时都是必须的
		PRESS_I2C_SCL=1;
		Delay_US(2); 
		PRESS_I2C_SCL=0;	
		Delay_US(2);
    }
	__enable_irq();
}

UINT8 Press_I2C_Read_Byte(UINT8 nAck)
{
	unsigned char i,receive=0;
	
	Delay_US(5);
	if(PRESS_I2C_EOC_STATUS == 0) // 0: not ready, 1: ready
	{
		Delay_US(10);
		if(PRESS_I2C_EOC_STATUS == 0)
		{
			Delay_US(10);
		}
	}
	PRESS_I2C_SDA_IN();//SDA设置为输入
	__disable_irq();
    for(i=0;i<8;i++ )
	{
        PRESS_I2C_SCL=0; 
        Delay_US(2);
		PRESS_I2C_SCL=1;
        receive<<=1;
        if(PRESS_I2C_READ_SDA)receive++;   
		Delay_US(1); 
    }					 
    if (!nAck)
        Press_I2C_NAck();//发送nACK
    else
        Press_I2C_Ack(); //发送ACK   
	__enable_irq();
    return receive;
}

INT32 Get_Press_I2C(void)
{
	UINT8 I2c_Address = 0x29;
	UINT8 Read_Commond = 0xAA;
	UINT8 DLHR_DATA[7] = {0x00};
	UINT8 Status;
	INT32 Pressure_data,Tempertaure_data;
	
//	Press_I2C_Init();
	Press_I2C_Start();
	Press_I2C_Send_Byte((I2c_Address)<<1|0);//write
	Press_I2C_Ack();
	Press_I2C_Send_Byte(Read_Commond);//1mps
	Press_I2C_Wait_Ack();
	Press_I2C_Stop();
	
	IT_SYS_DlyMs(5);
	//Delay_US(250);
	__disable_irq();//__set_PRIMASK(0);
	Press_I2C_Start();
	Delay_US(10);
	Press_I2C_Send_Byte((I2c_Address)<<1|1);//read
	Press_I2C_Wait_Ack();
	DLHR_DATA[0] = Press_I2C_Read_Byte(1);//STATUS[7:0]
	DLHR_DATA[1] = Press_I2C_Read_Byte(1);//PRESSURE[23:16]
	DLHR_DATA[2] = Press_I2C_Read_Byte(1);//PRESSURE[15:8]
	DLHR_DATA[3] = Press_I2C_Read_Byte(1);//PRESSURE[7:0]
	DLHR_DATA[4] = Press_I2C_Read_Byte(1);//TEMPERATURE[23:16]
	DLHR_DATA[5] = Press_I2C_Read_Byte(1);//TEMPERATURE[15:8]
	DLHR_DATA[6] = Press_I2C_Read_Byte(0);//TEMPERATURE[7:0]
	Press_I2C_Stop();
	__enable_irq();//__set_PRIMASK(1);
	Status = DLHR_DATA[0];
	Pressure_data = (DLHR_DATA[1]<<16)|(DLHR_DATA[2]<<8)|DLHR_DATA[3];
	Tempertaure_data = (DLHR_DATA[4]<<16)|(DLHR_DATA[5]<<8)|DLHR_DATA[6];
//	printf("%d\r\n", Status);
	return Pressure_data;
}



void Reset_Elec_Status(void)
{
#if ELEC_USE_EXIT_MODE
	g_Elec_Status = 0;
#else
	GPIO_ResetBits(ELEC_PORT, ELEC_PIN);
#endif
}

void Set_Elec_Status(void)
{
#if ELEC_USE_EXIT_MODE
	g_Elec_Status = 1;
#else
	GPIO_SetBits(ELEC_PORT, ELEC_PIN);
#endif
}

UINT8 Get_Elec_Status(void)
{
#if ELEC_USE_EXIT_MODE
	return g_Elec_Status;
#else
	return GPIO_ReadInputDataBit(ELEC_PORT, ELEC_PIN);
#endif
}


#if ELEC_USE_EXIT_MODE
void ELEC_EXIT_FUNC(void)
{
	//IT_SYS_DlyMs(2);
	if(ELEC_READ == 0) // low triggle
	{
		Set_Elec_Status();
	}		 
	EXTI_ClearITPendingBit(ELEC_EXIT_LINE);
	printf("Elec Exit Func triggle, v=%d, status=%d\r\n", ELEC_READ, Get_Elec_Status());
}
#endif


void Elec_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
//    EXTI_InitTypeDef EXTI_InitStructure;
#if ELEC_USE_EXIT_MODE
    NVIC_InitTypeDef NVIC_InitStructure;
#endif
    // 1. enable the input pin Clock 
    RCC_AHB1PeriphClockCmd(ELEC_SRC, ENABLE);
	
    // 2. configure the pin as input floating 
	GPIO_InitStructure.GPIO_Pin = ELEC_PIN;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;//GPIO_PuPd_DOWN; 
    GPIO_Init(ELEC_PORT, &GPIO_InitStructure); 
//	GPIO_SetBits(ELEC_PORT, ELEC_PIN);
    // 3. extinal-interrupt model 
#if ELEC_USE_EXIT_MODE
	// 1). enable the SYSCFG-clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);       
	// 2). Connects EXTI Line to Button GPIO Pin 
	SYSCFG_EXTILineConfig(ELEC_EXIT_SRC, ELEC_EXIT_PIN);    
	// 3). Configure EXTI line 
	EXTI_InitStructure.EXTI_Line = ELEC_EXIT_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;   
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  // all pins are rising detected         
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;               // enable interrupt line 		
	EXTI_Init(&EXTI_InitStructure);                           
	// 4). enable and set input EXTI Interrupt to the lowest priority 
	NVIC_InitStructure.NVIC_IRQChannel = ELEC_EXIT_NUM;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        // enable interrupt
	NVIC_Init(&NVIC_InitStructure);    	
#endif

}

UINT16 Get_Elec_ADC(void)
{
	UINT16 nVal = 0, i;
	
#if ADC3_INIT_WITH_DMA
	//nVal = g_ADC3_Value[1];
#else
	ADC_ClearFlag(ADC3,ADC_FLAG_EOC);
	ADC_Cmd(ADC3,ENABLE);
	for(i = 0; i < 5; i++)
	{
		ADC_RegularChannelConfig(ADC3, ELEC_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles ); 
		ADC_SoftwareStartConv(ADC3);		
		while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
		nVal += ADC_GetConversionValue(ADC3);	
	}
	nVal /= 5;
#endif
	return nVal;

}

// beep
void Beep_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(BEEP_SRC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
}

void Beep(UINT8 nNo, UINT16 nDelay)
{
	UINT8 i;
	if(nNo == 0) return;
	if(nNo > 1)
	{
		for(i = 0; i < nNo; i++)
		{
			GPIO_SetBits(BEEP_PORT, BEEP_PIN);
			IT_SYS_DlyMs(nDelay);
			GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
			IT_SYS_DlyMs(nDelay);
		}
	}else if(nNo == 1){
		GPIO_SetBits(BEEP_PORT, BEEP_PIN);
		IT_SYS_DlyMs(nDelay);
		GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
	}
}

// pump
void Pump_init(void)
{
//	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(PUMP_CLK_SRC, ENABLE); //RCC_AHB1PeriphClockCmd(PUMP_CLK_SRC|PUMP_DIR_SRC, ENABLE);
//	// dir
//	GPIO_InitStructure.GPIO_Pin = PUMP_DIR_PIN; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //GPIO_PuPd_DOWN
//	GPIO_Init(PUMP_DIR_PORT, &GPIO_InitStructure);
//	GPIO_SetBits(PUMP_DIR_PORT, PUMP_DIR_PIN);
	
	Pump_PWM_Init(PUMP_PWM_TIM_ARR, PUMP_PWM_TIM_PSC); //84M/42=2M, 1M/25000=59.52
	//Pump_Speed_Set(24998);
//	Pump_Speed_Set(PUMP_PWM_LEVEL_CLOSE);
	//TIM_SetCompare2(PUMP_PWM_TIM, PUMP_PWM_LEVEL_CLOSE);
}


void Pump_PWM_Init(UINT32 Arr, UINT32 Psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(PUMP_PWM_TIM_SRC,ENABLE);  	
	RCC_AHB1PeriphClockCmd(PUMP_CLK_SRC, ENABLE); 
	GPIO_PinAFConfig(PUMP_CLK_PORT,PUMP_CLK_PIN_AF, PUMP_CLK_PORT_AF); 

	GPIO_InitStructure.GPIO_Pin = PUMP_CLK_PIN;          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(PUMP_CLK_PORT, &GPIO_InitStructure);        
	GPIO_SetBits(PUMP_CLK_PORT, PUMP_CLK_PIN);
	
	TIM_DeInit(PUMP_PWM_TIM);
	TIM_TimeBaseStructure.TIM_Prescaler=Psc;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=Arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(PUMP_PWM_TIM,&TIM_TimeBaseStructure);

	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_Low; 
	//TIM_OCInitStructure.TIM_Pulse = ;
	TIM_OC2Init(PUMP_PWM_TIM, &TIM_OCInitStructure);  

	TIM_OC2PreloadConfig(PUMP_PWM_TIM, TIM_OCPreload_Enable); 
	TIM_ARRPreloadConfig(PUMP_PWM_TIM,ENABLE);
	TIM_Cmd(PUMP_PWM_TIM, ENABLE);  //
	
	TIM_SetCompare2(PUMP_PWM_TIM, PUMP_PWM_LEVEL_CLOSE);
}
		

void Pump_AntiClockWise(void) // xi qi
{
//	GPIO_SetBits(PUMP_DIR_PORT, PUMP_DIR_PIN);
}

void Pump_ClockWise(void) // zhu qi
{
//	GPIO_ResetBits(PUMP_DIR_PORT, PUMP_DIR_PIN);
}

void Pump_Speed_Set(UINT16 nSpeed) // 0-499
{
	if(nSpeed > PUMP_PWM_LEVEL_HIGHEST){
		TIM_SetCompare2(PUMP_PWM_TIM, PUMP_PWM_LEVEL_HIGHEST);
	}else{
		TIM_SetCompare2(PUMP_PWM_TIM, nSpeed);
	}
}

void Pump_Exec(UINT8 nDir, UINT16 nFreq)
{
	if(nFreq > PUMP_PWM_TIM_ARR) 
		nFreq = PUMP_PWM_LEVEL_HIGHEST;
	
	if(nDir == e_Dir_Pos){
		Pump_AntiClockWise();
	}else if(nDir == e_Dir_Neg){
		Pump_ClockWise();
	}
	//
	//Pump_init();
	Pump_Speed_Set(nFreq);
}

// mixing motor
void Mixing_Motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(MIXING_DIR_SRC, ENABLE);
	//
	GPIO_InitStructure.GPIO_Pin = MIXING_DIR_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;
	GPIO_Init(MIXING_DIR_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(MIXING_DIR_PORT, MIXING_DIR_PIN);
}

void Mixing_Motor_Run(void)
{
	GPIO_SetBits(MIXING_DIR_PORT, MIXING_DIR_PIN);
}

void Mixing_Motor_Stop(void)
{
	GPIO_ResetBits(MIXING_DIR_PORT, MIXING_DIR_PIN);
}

// valve
void Valve_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(VALVE_AIR_SRC|VALVE_LIQUID_SRC, ENABLE);
	// valve air
	GPIO_InitStructure.GPIO_Pin = VALVE_AIR_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(VALVE_AIR_PORT, &GPIO_InitStructure);
	// valve liquid
	GPIO_InitStructure.GPIO_Pin = VALVE_LIQUID_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(VALVE_LIQUID_PORT, &GPIO_InitStructure);
	//
	GPIO_ResetBits(VALVE_AIR_PORT, VALVE_AIR_PIN);
	GPIO_ResetBits(VALVE_LIQUID_PORT, VALVE_LIQUID_PIN);
}

void Valve_Air_Exec(UINT8 nOpt)
{
	if(nOpt == EN_OPEN)
	{
		GPIO_SetBits(VALVE_AIR_PORT, VALVE_AIR_PIN);
	}else if(nOpt == EN_CLOSE){
		GPIO_ResetBits(VALVE_AIR_PORT, VALVE_AIR_PIN);
		
	}
}

void Valve_Liquid_Exec(UINT8 nOpt)
{
	if(nOpt == EN_OPEN)
	{
		GPIO_SetBits(VALVE_LIQUID_PORT, VALVE_LIQUID_PIN);
	}else if(nOpt == EN_CLOSE){
		GPIO_ResetBits(VALVE_LIQUID_PORT, VALVE_LIQUID_PIN);		
	}
}

void Valve_Exec(UINT8 nIndex, UINT8 nOpt)
{
	if(nIndex == EN_VALVE_LIQUID)
	{
		Valve_Liquid_Exec(nOpt);
	}else if(nIndex == EN_VALVE_AIR){
		Valve_Air_Exec(nOpt);
	}

}

//void Turn_Motor_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_AHB1PeriphClockCmd(TURN_MOTOR_SRC_1|TURN_MOTOR_SRC_2|TURN_MOTOR_SRC_3|TURN_MOTOR_SRC_4|TURN_MOTOR_POWER_SRC, ENABLE);
//	// turn pin 1
//	GPIO_InitStructure.GPIO_Pin = TURN_MOTOR_PIN_1; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;;
//	GPIO_Init(TURN_MOTOR_PORT_1, &GPIO_InitStructure);
//	GPIO_ResetBits(TURN_MOTOR_PORT_1, TURN_MOTOR_PIN_1);
//	// turn pin 2
//	GPIO_InitStructure.GPIO_Pin = TURN_MOTOR_PIN_2; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;
//	GPIO_Init(TURN_MOTOR_PORT_2, &GPIO_InitStructure);
//	GPIO_ResetBits(TURN_MOTOR_PORT_2, TURN_MOTOR_PIN_2);
//	// turn pin 3
//	GPIO_InitStructure.GPIO_Pin = TURN_MOTOR_PIN_3; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;;
//	GPIO_Init(TURN_MOTOR_PORT_3, &GPIO_InitStructure);
//	GPIO_ResetBits(TURN_MOTOR_PORT_3, TURN_MOTOR_PIN_3);
//	// turn pin 4
//	GPIO_InitStructure.GPIO_Pin = TURN_MOTOR_PIN_4; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;//GPIO_PuPd_NOPULL;;
//	GPIO_Init(TURN_MOTOR_PORT_4, &GPIO_InitStructure);
//	GPIO_ResetBits(TURN_MOTOR_PORT_4, TURN_MOTOR_PIN_4);
//	
//	// turn motor power control, PH9
//	GPIO_InitStructure.GPIO_Pin = TURN_MOTOR_POWER_PIN; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
//	GPIO_Init(TURN_MOTOR_POWER_PORT, &GPIO_InitStructure);
//	GPIO_ResetBits(TURN_MOTOR_POWER_PORT, TURN_MOTOR_POWER_PIN);
//}

//void Turn_Motor_Power(UINT8 nOpt)
//{
//	if(EN_OPEN == nOpt)
//	{
//		GPIO_SetBits(TURN_MOTOR_POWER_PORT, TURN_MOTOR_POWER_PIN);
//	}else if(EN_CLOSE == nOpt){
//		GPIO_ResetBits(TURN_MOTOR_POWER_PORT, TURN_MOTOR_POWER_PIN);
//	}
//}

//void Turn_Motor_Break(void)//
//{
//  	TURN_MOTOR_PORT->ODR|=0xF000;	//
//}

//void Turn_Motor_Free(void)//
//{
//  	TURN_MOTOR_PORT->ODR&=(~0xF000); //
//}


//UINT8 Turn_Motor_Reset(void)
//{
//	UINT32 nStep = 0, nOutStatus, nTurnStep, DelayTime;
//	
//	DelayTime = TURN_MOTOR_MAX_DELAY;
//	nStep = TURN_MOTOR_MAX_ANTI_CLOCKWISE_STEP;
//	while(nStep)
//	{
//		if(nStep < 480)
//		{
//			if(EVAL_GetInputStatus(I_LED_OC_RST)== EN_CLOSE)
//			{
//				g_Turn_Position = EN_POSITION_LED_RESET;
//				printf("reset step = %d\r\n", (int)nStep);
//				return e_Feedback_Success;
//			}
//		}
//		nOutStatus = TURN_MOTOR_PORT->IDR;
//		nOutStatus &= 0x0FFF;
//		nTurnStep = (nStep & 0x03);
//		
//		TURN_MOTOR_PORT->ODR = nOutStatus|g_Turn_Motor_Table[nTurnStep];
//		
//		if(DelayTime > TURN_MOTOR_MIN_DELAY) DelayTime -= 10;
//		if(DelayTime/1000 >= 1)
//		{
//			IT_SYS_DlyMs(DelayTime/1000);
//			Delay_US(DelayTime%1000);
//		}else{
//			Delay_US(DelayTime);
//		}
//		nStep--;
//	}
//	return e_Feedback_Fail;
//}


//void RunTestMotor(signed int StepValue)
//{
//	UINT32 nStep = 0, nOutStatus, nTurnStep, DelayTime;
//	
//	Turn_Motor_Power(EN_OPEN);

//	DelayTime = TURN_MOTOR_MAX_DELAY;
//	nOutStatus = TURN_MOTOR_PORT->IDR;
//	nOutStatus &= 0x0FFF;
//	if (StepValue>0)
//	{
//		while(StepValue--)
//		{	      
//			if (DelayTime > TURN_MOTOR_MIN_DELAY)
//				DelayTime -= 10;
//			
//			nStep++;
//			nStep &= STEP_NUMBER;

//			TURN_MOTOR_PORT->ODR = nOutStatus | g_Turn_Motor_Table[nStep];
//			if(DelayTime/1000 >= 1)
//			{
//				IT_SYS_DlyMs(DelayTime/1000);
//				Delay_US(DelayTime%1000);
//			}else{
//				Delay_US(DelayTime);
//			}
//	  	  	//if(GPIO_ReadInputDataBit(OUT_CARD_POS_PORT, OUT_CARD_POS_PIN)==1) //检测光电开关
//			//{			 
//			//	break;
//		    //}
//		}
//	}
//	else
//	{
//	  while (StepValue++)
//	  { 
//		if (DelayTime > TURN_MOTOR_MIN_DELAY)
//			DelayTime -=10;
//			
//		nStep--;
//		nStep &= STEP_NUMBER;
//			
//		TURN_MOTOR_PORT->ODR = nOutStatus|g_Turn_Motor_Table[nStep];			 
//		delay_us(mTestMotorDelay);
//		if(GPIO_ReadInputDataBit(TEST_MOTOT_POS_PORT, TEST_MOTOT_POS_PIN)==1) //检测光电开关
//		{			 
//			break;
//		}
//  	  }
//	  if( StepValue > 0  )
//	  {
//		 UnbrakeTestMotor();
//		 while(1)
//		 {
//			SendMessageFrame(CMD_INFORMATION,INFO_HMOTOR_ERROR);
//				//yaolan MessageBox( NULL, p_cMotorError, p_cCompany, p_cOk, NULL ); 
//				//delay_ms(2000);
//		 }
//	  }
//	}
//	UnbrakeTestMotor();
//	Turn_Motor_Power(EN_CLOSE);
//}

//UINT8 Turn_Motor_Anti_ClockWise(UINT32 nStep)
//{
//	UINT32 nTemp = 0, nOutStatus, nTurnStep, DelayTime;
//	
//	DelayTime = TURN_MOTOR_MAX_DELAY;
//	nTemp = nStep + LED_SELETCT_STEP_DIFF; // -
//	while(nTemp > 0)
//	{
//		if(g_Turn_Position == EN_POSITION_LED_RESET) // if at reset position,
//		{
//			if(nTemp <= 2*LED_SELETCT_STEP_DIFF)
//			{
//				if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_SEL)) 
//				{
//					g_Turn_Position = EN_POSITION_LED_UNSURE; /////////////
//					printf("Anti Stop Steps = %d \r\n", (int)nTemp);
//					return e_Feedback_Success;
//				}
//			}			
//		}else{
//			if(nTemp < (nStep - 10))
//			{
//				if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_SEL)) 
//				{
//					g_Turn_Position = EN_POSITION_LED_UNSURE; /////////////
//					printf("Anti Stop Steps = %d \r\n", (int)nTemp);
//					return e_Feedback_Success;
//				}
//			}
//		}
//		nOutStatus = TURN_MOTOR_PORT->IDR;
//		nOutStatus &= 0x0FFF;
//		nTurnStep = (nTemp & 0x03);
//		
//		TURN_MOTOR_PORT->ODR = nOutStatus|g_Turn_Motor_Table[nTurnStep];
//		
//		if(DelayTime > TURN_MOTOR_MIN_DELAY) DelayTime -= 10;
//		if(DelayTime/1000 >= 1)
//		{
//			IT_SYS_DlyMs(DelayTime/1000);
//			Delay_US(DelayTime%1000);
//		}else{
//			Delay_US(DelayTime);
//		}
//		nTemp--;
//	}
//}


//UINT8 Turn_Motor_ClockWise(UINT32 nStep)
//{
//	UINT32 nTemp = 0, nOutStatus, nTurnStep, DelayTime;
//	
//	DelayTime = TURN_MOTOR_MAX_DELAY;
//	while(nTemp < nStep)
//	{
//		if(g_Turn_Position == EN_POSITION_LED_RESET)// if at reset position,
//		{
//			if((nTemp >= nStep - LED_SELETCT_STEP_DIFF) && (nTemp <= nStep + LED_SELETCT_STEP_DIFF))
//			{
//				if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_SEL)) 
//				{
//					g_Turn_Position = EN_POSITION_LED_UNSURE; 
//					printf("Stop Steps = %d \r\n", (int)nTemp);
//					return e_Feedback_Success;
//				}
//			}
//		}else{
//			if(nTemp > 10){
//				if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_SEL)) 
//				{
//					g_Turn_Position = EN_POSITION_LED_UNSURE; 
//					printf("Stop Steps = %d \r\n", (int)nTemp);
//					return e_Feedback_Success;
//				}
//			}
//		}
//		nOutStatus = TURN_MOTOR_PORT->IDR;
//		nOutStatus &= 0x0FFF;
//		nTurnStep = (nTemp & 0x03);
//		
//		TURN_MOTOR_PORT->ODR = nOutStatus|g_Turn_Motor_Table[nTurnStep];
//		
//		if(DelayTime > TURN_MOTOR_MIN_DELAY) DelayTime -= 10;
//		if(DelayTime/1000 >= 1)
//		{
//			IT_SYS_DlyMs(DelayTime/1000);
//			Delay_US(DelayTime%1000);
//		}else{
//			Delay_US(DelayTime);
//		}
//		nTemp++;
//	}
//}

//void Turn_Motor_Goto_Postion(UINT8 nOpt, UINT32 nStep)
//{
//	if(nOpt == EN_CLOCK_WISE)
//	{
//		Turn_Motor_ClockWise(nStep);
//	}else if(nOpt == EN_ANTI_CLOCK_WISE){
//		Turn_Motor_Anti_ClockWise(nStep);
//	}
//}

//void Turn_Motor_Select_LED(UINT8 nIndex)
//{
//	Turn_Motor_Reset();
//	IT_SYS_DlyMs(200);
//	switch(nIndex)
//	{
//		case EN_LED0:
//		{
//			printf("LED0 Select Step=%d\r\n", EN_LED0_SELECT_STEP);
//			Turn_Motor_Goto_Postion(EN_ANTI_CLOCK_WISE, EN_LED0_SELECT_STEP);  
//			g_Turn_Position = EN_POSITION_LED0;
//		}
//		break;
//		case EN_LED1:
//		{
//			printf("LED1 Select Step=%d\r\n", EN_LED1_SELECT_STEP);
//			Turn_Motor_Goto_Postion(EN_CLOCK_WISE, EN_LED1_SELECT_STEP);
//			g_Turn_Position = EN_POSITION_LED1;
//		}
//		break;
//		case EN_LED2:
//		{
//			printf("LED2 Select Step=%d\r\n", EN_LED2_SELECT_STEP);
//			Turn_Motor_Goto_Postion(EN_CLOCK_WISE, EN_LED2_SELECT_STEP);
//			g_Turn_Position = EN_POSITION_LED2;
//		}
//		break;
//		case EN_LED3:
//		{
//			printf("LED3 Select Step=%d\r\n", EN_LED3_SELECT_STEP);
//			Turn_Motor_Goto_Postion(EN_CLOCK_WISE, EN_LED3_SELECT_STEP);
//			g_Turn_Position = EN_POSITION_LED3;
//		}
//		break;
//		case EN_LED4:
//		{
//			printf("LED4 Select Step=%d\r\n", EN_LED4_SELECT_STEP);
//			Turn_Motor_Goto_Postion(EN_ANTI_CLOCK_WISE, EN_LED4_SELECT_STEP);
//			g_Turn_Position = EN_POSITION_LED4;
//		}
//		break;
//		case EN_LED5:
//		{
//			printf("LED5 Select Step=%d\r\n", EN_LED5_SELECT_STEP);
//			Turn_Motor_Goto_Postion(EN_ANTI_CLOCK_WISE, EN_LED5_SELECT_STEP);
//			g_Turn_Position = EN_POSITION_LED5;
//		}
//		break;
//		case EN_LED6:
//		{
//			printf("LED6 Select Step=%d\r\n", EN_LED6_SELECT_STEP);
//			Turn_Motor_Goto_Postion(EN_CLOCK_WISE, EN_LED6_SELECT_STEP);
//			g_Turn_Position = EN_POSITION_LED6;
//		}
//		break;
//		case EN_LED7:
//		{
//			printf("LED7 Select Step=%d\r\n", EN_LED7_SELECT_STEP);
//			Turn_Motor_Goto_Postion(EN_CLOCK_WISE, EN_LED7_SELECT_STEP);
//			g_Turn_Position = EN_POSITION_LED7;
//		}
//		break;
//		default:break;
//	}
//}

// for micro oc exit interrupt
//void MICRO_OC_EXIT_FUNC(void)
void EXTI9_5_IRQHandler(void)
{
	g_Micro_Switch = EN_OPEN;
	if(RESET != EXTI_GetITStatus(MICRO_OC_EXIT_LINE))
    {      
		// read micro oc status
		Delay_US(500);Delay_US(500);
		if(Get_Micro_OC_Status() == EN_OPEN)
		{
			EXTI_ClearITPendingBit(MICRO_OC_EXIT_LINE);
			return;
		}			
		Beep(1, 300);
	    EXTI_ClearITPendingBit(MICRO_OC_EXIT_LINE);
		if(Get_Micro_OC_Status() == EN_CLOSE)
		{
			g_Micro_Switch = EN_CLOSE;
		}
		printf("Mirco Switch IRQ, S=%d\r\n", g_Micro_Switch);
		EXTI_ClearITPendingBit(MICRO_OC_EXIT_LINE);
	}
}


void Micro_OC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(MICRO_OC_SRC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	// micro switch, with EXIT interupt
	
	GPIO_InitStructure.GPIO_Pin = MICRO_OC_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(MICRO_OC_PORT, &GPIO_InitStructure);
	//GPIO_ResetBits(MICRO_OC_PORT, MICRO_OC_PIN);
	SYSCFG_EXTILineConfig(MICRO_OC_EXIT_PORT, MICRO_OC_EXIT_PIN);
	
	EXTI_InitStructure.EXTI_Line = MICRO_OC_EXIT_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // or down 
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = MICRO_OC_EXIT_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

UINT8 Get_Micro_OC_Status(void)
{
	return GPIO_ReadInputDataBit(MICRO_OC_PORT, MICRO_OC_PIN);
}


void OC_Init(void)
{
	EVAL_InputInit(I_LED_OC_RST);
	EVAL_InputInit(I_LED_OC_SEL);
	EVAL_InputInit(I_MOTOR_OC_IN);
	EVAL_InputInit(I_MOTOR_OC_OUT);	
	Micro_OC_Init();
}

UINT8 Get_Out_OC_Status(void)
{
	return EVAL_GetInputStatus(I_MOTOR_OC_OUT);
}

UINT8 Get_In_OC_Status(void)
{
	return EVAL_GetInputStatus(I_MOTOR_OC_IN);
}

// OC for fix motor, OC for out_in motor
//void OC_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_AHB1PeriphClockCmd(TURN_RESET_OC_CLK_SRC|TURN_SELECT_OC_CLK_SRC|OUT_OC_CLK_SRC|IN_OC_CLK_SRC, ENABLE);
//	// turn reset
//	GPIO_InitStructure.GPIO_Pin = TURN_RESET_OC_CLK_PIN; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(TURN_RESET_OC_CLK_PORT, &GPIO_InitStructure);
//	GPIO_ResetBits(TURN_RESET_OC_CLK_PORT, TURN_RESET_OC_CLK_PIN);
//	// turn select
//	GPIO_InitStructure.GPIO_Pin = TURN_SELECT_OC_CLK_PIN; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(TURN_SELECT_OC_CLK_PORT, &GPIO_InitStructure);
//	GPIO_ResetBits(TURN_SELECT_OC_CLK_PORT, TURN_SELECT_OC_CLK_PIN);
//	// oc for out
//	GPIO_InitStructure.GPIO_Pin = OUT_OC_CLK_PIN; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(OUT_OC_CLK_PORT, &GPIO_InitStructure);
//	GPIO_ResetBits(OUT_OC_CLK_PORT, OUT_OC_CLK_PIN);
//	// oc for in
//	GPIO_InitStructure.GPIO_Pin = IN_OC_CLK_PIN; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//	GPIO_Init(IN_OC_CLK_PORT, &GPIO_InitStructure);
//	GPIO_ResetBits(IN_OC_CLK_PORT, IN_OC_CLK_PIN);
//	// micro oc
//	Micro_OC_Init();
//}


//	
//UINT8 Get_Turn_Reset_OC_Status(void)
//{
//	return GPIO_ReadInputDataBit(TURN_RESET_OC_CLK_PORT, TURN_RESET_OC_CLK_PIN);
//}

//UINT8 Get_Turn_Select_OC_Status(void)
//{
//	return GPIO_ReadInputDataBit(TURN_SELECT_OC_CLK_PORT, TURN_SELECT_OC_CLK_PIN);
//}

//UINT8 Get_Out_OC_Status(void)
//{
//	return GPIO_ReadInputDataBit(OUT_OC_CLK_PORT, OUT_OC_CLK_PIN);
//}

//UINT8 Get_In_OC_Status(void)
//{
//	return GPIO_ReadInputDataBit(IN_OC_CLK_PORT, IN_OC_CLK_PIN);
//}


void Counter_Check_Init(void)
{
	EVAL_InputInit(I_COUNTER_CHECK);
}

UINT8 Get_Counter_Check_Status(void)
{
	return EVAL_GetInputStatus(I_COUNTER_CHECK);

}

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(LED1_CLK_SRC|LED2_CLK_SRC|LED3_CLK_SRC|LED4_CLK_SRC|LED5_CLK_SRC \
		|LED6_CLK_SRC|LED7_CLK_SRC|LED0_CLK_SRC|LED_CUR_SWITCH_SRC|LED_SELECT_A0_SRC| \
		LED_SELECT_A1_SRC|LED_SELECT_A2_SRC, ENABLE);
	
	//LED0
	GPIO_InitStructure.GPIO_Pin   = LED0_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_DOWN;;
	GPIO_Init(LED0_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED0_PORT, LED0_PIN);
	// LED1
	GPIO_InitStructure.GPIO_Pin   = LED1_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED1_PORT, LED1_PIN);
	//LED2
	GPIO_InitStructure.GPIO_Pin   = LED2_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LED2_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED2_PORT, LED2_PIN);
	// LED3
	GPIO_InitStructure.GPIO_Pin   = LED3_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(LED3_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED3_PORT, LED3_PIN);
	//LED4
	GPIO_InitStructure.GPIO_Pin   = LED4_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_DOWN;
	GPIO_Init(LED4_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED4_PORT, LED4_PIN);
	// LED5
	GPIO_InitStructure.GPIO_Pin   = LED5_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_DOWN;;
	GPIO_Init(LED5_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED5_PORT, LED5_PIN);
	//LED6
	GPIO_InitStructure.GPIO_Pin   = LED6_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_DOWN;;
	GPIO_Init(LED6_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED6_PORT, LED6_PIN);
	// LED7
	GPIO_InitStructure.GPIO_Pin   = LED7_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_DOWN;;
	GPIO_Init(LED7_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED7_PORT, LED7_PIN);

	// Control Switch, PB8
	GPIO_InitStructure.GPIO_Pin   = LED_CUR_SWITCH_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_UP;//GPIO_PuPd_DOWN;;
	GPIO_Init(LED_CUR_SWITCH_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED_CUR_SWITCH_PORT, LED_CUR_SWITCH_PIN);
	// LED cur select A0
	GPIO_InitStructure.GPIO_Pin   = LED_SELECT_A0_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_DOWN;;
	GPIO_Init(LED_SELECT_A0_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
	// LED cur select A1
	GPIO_InitStructure.GPIO_Pin   = LED_SELECT_A1_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_DOWN;;
	GPIO_Init(LED_SELECT_A1_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
	// LED cur select A2
	GPIO_InitStructure.GPIO_Pin   = LED_SELECT_A2_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//GPIO_PuPd_DOWN;;
	GPIO_Init(LED_SELECT_A2_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);
	
	// LED Cur Adjust, DAC
	LED_Cur_DAC_Init();
}

void LED_Cur_DAC_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	
	RCC_AHB1PeriphClockCmd(LED_CUR_ADJUST_SRC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	   
	GPIO_InitStructure.GPIO_Pin = LED_CUR_ADJUST_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(LED_CUR_ADJUST_PORT, &GPIO_InitStructure);

	DAC_InitType.DAC_Trigger=DAC_Trigger_None;
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;
	DAC_Init(LED_CUR_ADJUST_DAC_CH,&DAC_InitType);	 
	DAC_Cmd(LED_CUR_ADJUST_DAC_CH, ENABLE);  
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);  
}

void LED_Cur_DAC_Set(UINT16 nVal)
{
//	UINT16 nTemp;
//	nTemp = nVal*4095 / 3300;
	DAC_SetChannel1Data(DAC_Align_12b_R, nVal);
}

//
//void LED_Cur_Auto_Adjust(UINT16 nVal)
//{
//	UINT16 nTempADC;
//	UINT32 nCurTick, nTempTick;
//	
//	LED_Cur_Switch(EN_OPEN);
//	LED_Cur_DAC_Set(0);
//	
//	nCurTick = IT_SYS_GetTicks();
//	nTempTick = nCurTick;
//	nTempADC = Get_LED_Cur_ADC();
//	//nTempV = nTempADC*3300/4095;
//	while(nCurTick < nTempTick + 5000)
//	{
//		if(nTempADC - nVal > 3)
//		{
//			LED_Cur_DAC_Set(nTempADC - 3);
//		}else if(nTempADC - nVal < 3){
//			LED_Cur_DAC_Set(nTempADC + 3);
//		}else{
//			// not need to adjust
//			break;
//		}
//		nTempADC = Get_LED_Cur_ADC();
//		IT_SYS_DlyMs(5);
//	}
//}


//  func exec after LED Cur and LED is open
void LED_Cur_Auto_Adjust(UINT16 nCurrent) // nCurrent is the current need to set
{
	UINT16 nTempADC , nDACSet;
	UINT32 nCurTick, nTempTick;
	
	LED_Cur_DAC_Set(0);
	IT_SYS_DlyMs(10);
	nTempADC = Get_LED_Cur_ADC();
	//
	nDACSet = nCurrent*100*3300/4095; // nCurrent to DAC value
	LED_Cur_DAC_Set(nDACSet);
	//
	nCurTick = IT_SYS_GetTicks();
	nTempTick = nCurTick;
	while(nCurTick < nTempTick + 5000)
	{
		if(nDACSet - nTempADC > 3)
		{
			LED_Cur_DAC_Set(nTempADC + 3);
		}else if(nDACSet - nTempADC < 3){
			LED_Cur_DAC_Set(nTempADC);  // keep
		}else{
			// not need to adjust
			break;
		}
		nTempADC = Get_LED_Cur_ADC();
		IT_SYS_DlyMs(5);
	}
}

void LED_Cur_Switch(UINT8 nOpt)
{
	if(nOpt == EN_OPEN){
		GPIO_SetBits(LED_CUR_SWITCH_PORT, LED_CUR_SWITCH_PIN);
	}else if(nOpt == EN_CLOSE){
		GPIO_ResetBits(LED_CUR_SWITCH_PORT, LED_CUR_SWITCH_PIN);
	}	
}



//void LED_Cur_ADC_Check_Channel(UINT16 nIndex)
//{
//	switch(nIndex)
//	{
//		case EN_LED0:
//		{
//			GPIO_ResetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
//			GPIO_ResetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
//			GPIO_ResetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);
//		}
//		break;
//		case EN_LED1:
//		{
//			GPIO_SetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
//			GPIO_ResetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
//			GPIO_ResetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
//		}
//		break;
//		case EN_LED2:
//		{
//			GPIO_ResetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
//			GPIO_SetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
//			GPIO_ResetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
//		}
//		break;
//		case EN_LED3:
//		{
//			GPIO_SetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
//			GPIO_SetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
//			GPIO_ResetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
//		}
//		break;
//		case EN_LED4:
//		{
//			GPIO_ResetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
//			GPIO_ResetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
//			GPIO_SetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
//		}
//		break;
//		case EN_LED5:
//		{
//			GPIO_SetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
//			GPIO_ResetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
//			GPIO_SetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
//		}
//		break;
//		case EN_LED6:
//		{
//			GPIO_ResetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
//			GPIO_SetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
//			GPIO_SetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
//		}
//		break;
//		case EN_LED7:
//		{
//			GPIO_SetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
//			GPIO_SetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
//			GPIO_SetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
//		}
//		break;
//	}
//}

void LED_Cur_ADC_Check_Channel(UINT16 nIndex)
{
	switch(nIndex)
	{
		case EN_LED0:
		{
			GPIO_ResetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
			GPIO_ResetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
			GPIO_ResetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);
		}
		break;
		case EN_LED1:
		{
			GPIO_SetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
			GPIO_ResetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
			GPIO_ResetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
		}
		break;
		case EN_LED2:
		{
			GPIO_ResetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
			GPIO_SetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
			GPIO_ResetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
		}
		break;
		case EN_LED3:
		{
			GPIO_SetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
			GPIO_SetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
			GPIO_ResetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
		}
		break;
		case EN_LED4:
		{
			GPIO_ResetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
			GPIO_ResetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
			GPIO_SetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
		}
		break;
		case EN_LED5:
		{
			GPIO_SetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
			GPIO_ResetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
			GPIO_SetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
		}
		break;
		case EN_LED6:
		{
			GPIO_ResetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
			GPIO_SetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
			GPIO_SetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
		}
		break;
		case EN_LED7:
		{
			GPIO_SetBits(LED_SELECT_A0_PORT, LED_SELECT_A0_PIN);
			GPIO_SetBits(LED_SELECT_A1_PORT, LED_SELECT_A1_PIN);
			GPIO_SetBits(LED_SELECT_A2_PORT, LED_SELECT_A2_PIN);		
		}
		break;
	}
}


void LED_Exec(UINT8 nIndex, UINT8 nOpt)
{
	switch(nIndex)
	{
		case EN_LED0:
		{
			if(nOpt == EN_OPEN){
				GPIO_SetBits(LED0_PORT, LED0_PIN);
			}else if(nOpt == EN_CLOSE){
				GPIO_ResetBits(LED0_PORT, LED0_PIN);
			}
		}
		break;	
		case EN_LED1:
		{
			if(nOpt == EN_OPEN){
				GPIO_SetBits(LED1_PORT, LED1_PIN);
			}else if(nOpt == EN_CLOSE){
				GPIO_ResetBits(LED1_PORT, LED1_PIN);
			}
		}
		break;
		case EN_LED2:
		{
			if(nOpt == EN_OPEN){
				GPIO_SetBits(LED2_PORT, LED2_PIN);
			}else if(nOpt == EN_CLOSE){
				GPIO_ResetBits(LED2_PORT, LED2_PIN);
			}
		}
		break;
		case EN_LED3:
		{
			if(nOpt == EN_OPEN){
				GPIO_SetBits(LED3_PORT, LED3_PIN);
			}else if(nOpt == EN_CLOSE){
				GPIO_ResetBits(LED3_PORT, LED3_PIN);
			}		
		}
		break;
		case EN_LED4:
		{
			if(nOpt == EN_OPEN){
				GPIO_SetBits(LED4_PORT, LED4_PIN);
			}else if(nOpt == EN_CLOSE){
				GPIO_ResetBits(LED4_PORT, LED4_PIN);
			}
		}
		break;
		case EN_LED5:
		{
			if(nOpt == EN_OPEN){
				GPIO_SetBits(LED5_PORT, LED5_PIN);
			}else if(nOpt == EN_CLOSE){
				GPIO_ResetBits(LED5_PORT, LED5_PIN);
			}
		}
		break;
		case EN_LED6:
		{
			if(nOpt == EN_OPEN){
				GPIO_SetBits(LED6_PORT, LED6_PIN);
			}else if(nOpt == EN_CLOSE){
				GPIO_ResetBits(LED6_PORT, LED6_PIN);
			}
		}
		break;
		case EN_LED7:
		{
			if(nOpt == EN_OPEN){
				GPIO_SetBits(LED7_PORT, LED7_PIN);
			}else if(nOpt == EN_CLOSE){
				GPIO_ResetBits(LED7_PORT, LED7_PIN);
			}			
		}
		break;
		default:break;	
	}
}

void LED_All_Reset(void)
{
	UINT8 i;
	for(i = 0; i < EN_LED_END; i++)
	{
		LED_Exec(i, EN_CLOSE);
	}
}

//// fix motor
//void Fix_Motor_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_AHB1PeriphClockCmd(FIX_MOTOR_EN_SRC|FIX_MOTOR_DIR_SRC|FIX_MOTOR_CLK_SRC, ENABLE);
//	// en
//	GPIO_InitStructure.GPIO_Pin = FIX_MOTOR_EN_PIN; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(FIX_MOTOR_EN_PORT, &GPIO_InitStructure);
//	GPIO_ResetBits(FIX_MOTOR_EN_PORT, FIX_MOTOR_EN_PIN);
//	// dir
//	GPIO_InitStructure.GPIO_Pin = FIX_MOTOR_DIR_PIN; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(FIX_MOTOR_DIR_PORT, &GPIO_InitStructure);
//	GPIO_ResetBits(FIX_MOTOR_DIR_PORT, FIX_MOTOR_DIR_PIN);
//	// clk
//	GPIO_InitStructure.GPIO_Pin = FIX_MOTOR_CLK_PIN; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(FIX_MOTOR_CLK_PORT, &GPIO_InitStructure);
//	
//	GPIO_ResetBits(FIX_MOTOR_EN_PORT, FIX_MOTOR_EN_PIN);
//	GPIO_ResetBits(FIX_MOTOR_DIR_PORT, FIX_MOTOR_DIR_PIN);
//	GPIO_ResetBits(FIX_MOTOR_CLK_PORT, FIX_MOTOR_CLK_PIN);
//}

//void Fix_Motor_Enable(void)
//{
//	GPIO_ResetBits(FIX_MOTOR_EN_PORT, FIX_MOTOR_EN_PIN);
//}

//void Fix_Motor_Disable(void)
//{
//	GPIO_SetBits(FIX_MOTOR_EN_PORT, FIX_MOTOR_EN_PIN);
//}

//void Fix_Motor_AntiClockWise(void) // fixable
//{
//	GPIO_SetBits(FIX_MOTOR_DIR_PORT, FIX_MOTOR_DIR_PIN);
//}

//void Fix_Motor_ClockWise(void) // free
//{
//	GPIO_ResetBits(FIX_MOTOR_DIR_PORT, FIX_MOTOR_DIR_PIN);
//}

//void Fix_Motor_Run(UINT16 nUp, UINT16 nDown)
//{
//	GPIO_SetBits(FIX_MOTOR_CLK_PORT, FIX_MOTOR_CLK_PIN);
//	Delay_US(nUp);
//	GPIO_ResetBits(FIX_MOTOR_CLK_PORT, FIX_MOTOR_CLK_PIN);
//	Delay_US(nDown);
//}


//void OutIn_Motor_PWM_Init(UINT32 Arr, UINT32 Psc)
//{
//	// TIM3_CH3
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
//	TIM_OCInitTypeDef  TIM_OCInitStructure;

//	RCC_APB1PeriphClockCmd(OUTIN_MOTOR_PWM_TIM_SRC,ENABLE);  	//TIM14时钟使能    
//	RCC_AHB1PeriphClockCmd(OUTIN_MOTOR_CLK_SRC, ENABLE); 	//使能PORTF时钟	
//	GPIO_PinAFConfig(OUTIN_MOTOR_CLK_PORT,OUTIN_MOTOR_CLK_PIN_AF, OUTIN_MOTOR_CLK_PORT_AF); 

//	GPIO_InitStructure.GPIO_Pin = OUTIN_MOTOR_CLK_PIN;           //GPIOC8
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
//	GPIO_Init(OUTIN_MOTOR_CLK_PORT, &GPIO_InitStructure);        
//	GPIO_SetBits(OUTIN_MOTOR_CLK_PORT, PUMP_CLK_PIN);
//	
//	TIM_DeInit(OUTIN_MOTOR_PWM_TIM);
//	TIM_TimeBaseStructure.TIM_Prescaler=Psc;  //定时器分频
//	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
//	TIM_TimeBaseStructure.TIM_Period=Arr;   //自动重装载值
//	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	TIM_TimeBaseInit(OUTIN_MOTOR_PWM_TIM,&TIM_TimeBaseStructure);//初始化定时器14

//	//初始化TIM3 Channel1 PWM模式	 
//	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
//	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
//	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_Low; 
//	//TIM_OCInitStructure.TIM_Pulse = ;
//	TIM_OC3Init(OUTIN_MOTOR_PWM_TIM, &TIM_OCInitStructure);  

//	TIM_OC3PreloadConfig(OUTIN_MOTOR_PWM_TIM, TIM_OCPreload_Enable); 
//	TIM_ARRPreloadConfig(OUTIN_MOTOR_PWM_TIM,ENABLE);
//	TIM_Cmd(OUTIN_MOTOR_PWM_TIM, ENABLE);  //
//}
//	

//// Out_In Motor
//void OutIn_Motor_Init(void)
//{
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	RCC_AHB1PeriphClockCmd(OUTIN_MOTOR_EN_SRC|OUTIN_MOTOR_DIR_SRC|OUTIN_MOTOR_CLK_SRC, ENABLE);
//	// en
//	GPIO_InitStructure.GPIO_Pin = OUTIN_MOTOR_EN_PIN; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(OUTIN_MOTOR_EN_PORT, &GPIO_InitStructure);
//	GPIO_ResetBits(OUTIN_MOTOR_EN_PORT, OUTIN_MOTOR_EN_PIN);
//	// dir
//	GPIO_InitStructure.GPIO_Pin = OUTIN_MOTOR_DIR_PIN; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(OUTIN_MOTOR_DIR_PORT, &GPIO_InitStructure);
//	GPIO_ResetBits(OUTIN_MOTOR_DIR_PORT, OUTIN_MOTOR_DIR_PIN);
//	// clk
//	#if OUTIN_MOTOR_USE_PWM
//		OutIn_Motor_PWM_Init(OUTIN_MOTOR_PWM_TIM_ARR, OUTIN_MOTOR_PWM_TIM_PSC); //84M/42=2M, 1M/25000=59.52
//	#else
//		GPIO_InitStructure.GPIO_Pin = OUTIN_MOTOR_CLK_PIN; 
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//		GPIO_Init(OUTIN_MOTOR_CLK_PORT, &GPIO_InitStructure);
//		GPIO_ResetBits(OUTIN_MOTOR_CLK_PORT, OUTIN_MOTOR_CLK_PIN);
//	#endif
//}

//void OutIn_Motor_Enable(void)
//{
//	GPIO_ResetBits(OUTIN_MOTOR_EN_PORT, OUTIN_MOTOR_EN_PIN);
//}

//void OutIn_Motor_Disable(void)
//{
//	GPIO_SetBits(OUTIN_MOTOR_EN_PORT, OUTIN_MOTOR_EN_PIN);
//}

//void OutIn_Motor_Out(void) // out
//{
//	GPIO_SetBits(OUTIN_MOTOR_DIR_PORT, OUTIN_MOTOR_DIR_PIN);
//}

//void OutIn_Motor_In(void) // in
//{
//	GPIO_ResetBits(OUTIN_MOTOR_DIR_PORT, OUTIN_MOTOR_DIR_PIN);
//}

//void OutIn_Motor_Run(UINT16 nUp, UINT16 nDown)
//{
//	GPIO_SetBits(OUTIN_MOTOR_CLK_PORT, OUTIN_MOTOR_CLK_PIN);
//	Delay_US(nUp);
//	GPIO_ResetBits(OUTIN_MOTOR_CLK_PORT, OUTIN_MOTOR_CLK_PIN);
//	Delay_US(nDown);
//}


//void OutIn_Motor_Speed_Set(UINT16 nSpeed) // 0-499
//{
//	if(nSpeed > OUTIN_MOTOR_PWM_LEVEL_HIGHEST){
//		TIM_SetCompare3(OUTIN_MOTOR_PWM_TIM, OUTIN_MOTOR_PWM_LEVEL_HIGHEST);
//	}else{
//		TIM_SetCompare3(OUTIN_MOTOR_PWM_TIM, nSpeed);
//	}
//}

//void OutIn_Motor_Exec(UINT8 nDir, UINT16 nFreq)
//{
//	if(nFreq > PUMP_PWM_TIM_ARR) return;
//	///////if(nDir != e_Dir_Neg || nDir != e_Dir_Pos) return;
//	
//	if(nDir == e_Dir_Pos){
//		OutIn_Motor_Out(); // out
//	}else if(nDir == e_Dir_Neg){
//		OutIn_Motor_In(); //in
//	}
//	//
//	//Pump_init();
//	OutIn_Motor_Speed_Set(nFreq);
//}



//UINT8 OutIn_Motor_Out(eModeType eMode)
//{
//	UINT32 nCurTime = 0, nTempTime, i;
//	INT32 nPress = 0;
//	
//	if(eMode == EN_MODE_NORMAL)
//	{
//		moto_work_stat_2(0, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
//		nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
//		printf("1 X out-check press at first: npress=%010d, addpress=%010d\r\n", (int)nPress, (int)g_Record_Param.nAddPress);
//		if(nPress < PRESS_BUILD)
//		{
//			HW_PUMP_Pulse(PUMP_PRESS_FREQ, e_Dir_Pos);
//			HW_Valve_On(EN_VALVE_AIR);
//			HW_Valve_Off(EN_VALVE_LIQUID);			
//		}
//	}
//	// check i
//	if(EN_OPEN == Get_Out_OC_Status())
//	{
//		
//		// oc out
//		Fix_Motor_Enable();
//		OutIn_Motor_Out(); // out
//		nCurTime = IT_SYS_GetTicks();
//		nTempTime = nCurTime;
//		for(i = 0; i < 35000; i++)
//		{
//			if(EN_CLOSE == Get_Out_OC_Status())
//			{
//				IT_SYS_DlyMs(5);
//				if(EN_CLOSE == Get_Out_OC_Status()) break;
//			}
//			OutIn_Motor_Run(OUTIN_MOTOR_PULSE_UP_TIME, OUTIN_MOTOR_PULSE_DOWN_TIME);
//			nCurTime = IT_SYS_GetTicks();
//			if(nCurTime >= nTempTime + OUTIN_MOTOR_HOME_TIME)
//			{
//				collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
//				moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
//				break;
//			}			
//		}
//	
//	}
//	// check outin motor 
//	if(EN_CLOSE == Get_In_OC_Status())
//	{
//		// in
//		Fix_Motor_Enable();
//		OutIn_Motor_In(); // in
//		nCurTime = IT_SYS_GetTicks();
//		nTempTime = nCurTime;
//		for(i = 0; i < 35000; i++)
//		{
//			if(EN_CLOSE == Get_In_OC_Status())
//			{
//				IT_SYS_DlyMs(5);
//				if(EN_CLOSE == Get_In_OC_Status()) break;
//			}
//			OutIn_Motor_Run(OUTIN_MOTOR_PULSE_UP_TIME, OUTIN_MOTOR_PULSE_DOWN_TIME);
//			nCurTime = IT_SYS_GetTicks();
//			if(nCurTime >= nTempTime + OUTIN_MOTOR_HOME_TIME)
//			{
//				collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
//				moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
//				break;
//			}			
//		}
//		// add step
//		if(g_Record_Param.nXAddStep > 0)
//		{
//			for(i = 0; i < g_Record_Param.nXAddStep; i++)
//			{
//				OutIn_Motor_Run(OUTIN_MOTOR_PULSE_UP_TIME, OUTIN_MOTOR_PULSE_DOWN_TIME);
//			}
//		}
//	}else{
//		//
//	}
//	// end status msg
//	if(eMode == EN_MODE_NORMAL)
//	{
//		moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
//	}
//	
//	return e_Feedback_Success;
//}


void DResistor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(D_REGISTER_CLK_SRC|D_REGISTER_MOSI_SRC|D_REGISTER_CS_SRC, ENABLE);
	//cs
	GPIO_InitStructure.GPIO_Pin = D_REGISTER_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(D_REGISTER_CS_PORT, &GPIO_InitStructure);
	GPIO_SetBits(D_REGISTER_CS_PORT, D_REGISTER_CS_PIN);
	
	//MISO	
//	GPIO_InitStructure.GPIO_Pin = D_REGISTER_MISO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO_Mode_IN;
//	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_DOWN; //GPIO_PuPd_UP
//	GPIO_Init(ADC24BIT_MISO_PORT, &GPIO_InitStructure);
//	GPIO_ResetBits(D_REGISTER_MISO_PORT, D_REGISTER_MISO_PIN);
	
	// MOSI
	GPIO_InitStructure.GPIO_Pin = D_REGISTER_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(D_REGISTER_MOSI_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(D_REGISTER_MOSI_PORT, D_REGISTER_MOSI_PIN);
	// clk
	GPIO_InitStructure.GPIO_Pin = D_REGISTER_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(D_REGISTER_CLK_PORT, &GPIO_InitStructure);
	GPIO_SetBits(D_REGISTER_CLK_PORT, D_REGISTER_CLK_PIN);

}

#define DREGISTER_DATA_LEN	10
void DRegister_Write(UINT8 nIndex, UINT8 nVal)
{
	UINT16 nCmd =0;
	UINT8 i;
	
	DREGISTER_CLK_1();
	Delay_US(2);
	DREGISTER_CS_1();
	Delay_US(5);
	DREGISTER_CS_0();
	Delay_US(10);
	nCmd = (UINT16)(( nIndex & 0x03) << 8) | nVal;
	printf("CMD =%X\r\n", nCmd);
	for(i = 0; i < DREGISTER_DATA_LEN; i++)
	{
		DREGISTER_CLK_0();
		if(0x0200 == (nCmd & 0x0200))
		{
			DREGISTER_MOSI_1();
		}else{
			DREGISTER_MOSI_0();	
		}
		Delay_US(10);
		DREGISTER_CLK_1();
		Delay_US(10);
		nCmd <<= 1;
	}
	DREGISTER_CS_1();
	Delay_US(2);
	DREGISTER_CLK_1();
}


// counter adjust init
void Counter_Adjust_Init(void)
{
	EVAL_OutputInit(O_COUNTER_ADJUST);
	Counter_Adjust_PWM_Init(COUNTER_PWM_TIM_ARR, COUNTER_PWM_TIM_PSC);
}

// TIM1 CH1N PWM
void Counter_Adjust_PWM_Init(UINT32 Arr,UINT32 Psc)
{		 					 
	// TIM1_CH1N
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(COUNTER_PWM_TIM_SRC, ENABLE);
	RCC_AHB1PeriphClockCmd(COUNTER_PWM_SRC | COUNTER_SWITCH_SRC, ENABLE);
	// PE10
	GPIO_PinAFConfig(COUNTER_PWM_PORT, COUNTER_PWM_PIN_AF, COUNTER_PWM_PORT_AF); 
	GPIO_InitStructure.GPIO_Pin = COUNTER_PWM_PIN;          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(COUNTER_PWM_PORT, &GPIO_InitStructure);        
	GPIO_ResetBits(COUNTER_PWM_PORT, COUNTER_PWM_PIN);
	
	TIM_DeInit(COUNTER_PWM_TIM);
	TIM_TimeBaseStructure.TIM_Prescaler=Psc;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=Arr; 
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(COUNTER_PWM_TIM,&TIM_TimeBaseStructure);

	// pwm
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_Low; 
	TIM_OC3Init(OUTIN_MOTOR_PWM_TIM, &TIM_OCInitStructure);  


//	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//??????     ?,?????????TIM_OCPolarity ?????,?1?????
//	TIM_OCInitStructure.TIM_Pulse = 2000; //????1 CCR1(?????)
//	TIM_OC1Init(TIM1, &TIM_OCInitStructure); //Ch1???
//	TIM_OCInitStructure.TIM_Pulse = 5000;
//	TIM_OC4Init(TIM1, &TIM_OCInitStructure);//??4
//	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //??TIM1?CCR1????????,CCR???????????

	TIM_OC1NPolarityConfig(COUNTER_PWM_TIM, TIM_OCPreload_Enable); 
	//TIM_OC3PreloadConfig(COUNTER_PWM_TIM, TIM_OCPreload_Enable); 
	TIM_ARRPreloadConfig(COUNTER_PWM_TIM, ENABLE);
	TIM_Cmd(COUNTER_PWM_TIM, ENABLE);  //
	
	TIM_SetCompare1(COUNTER_PWM_TIM, COUNTER_PWM_LEVEL_CLOSE);
}


void Counter_Adjust_Start(UINT32 nFreq)
{
	UINT32 nArr = 0;
	if(nFreq == COUNTER_PWM_LEVEL_CLOSE)
	{
		Counter_Adjust_Switch(EN_CLOSE);
		printf("Counter Adjust Open nFreq = %d\r\n", COUNTER_PWM_LEVEL_CLOSE);
		TIM_SetCompare1(COUNTER_PWM_TIM, COUNTER_PWM_LEVEL_CLOSE);
	}else{
		nArr = COUNTER_PWM_DEFAULT_FREQ/nFreq;
		printf("Counter Adjust Open nFreq = %d\r\n", (int)nFreq);
		Counter_Adjust_Switch(EN_OPEN);
		TIM_SetCompare1(COUNTER_PWM_TIM, nArr);
	}
}

void Counter_Adjust_Switch(UINT8 nOpt)
{
	if(nOpt == EN_OPEN)
	{
		EVAL_OutputSet(O_COUNTER_ADJUST);
	}else if(nOpt == EN_CLOSE){
		EVAL_OutputClr(O_COUNTER_ADJUST);
	}
}






void EVAL_Init(void)
{
    USART_InitTypeDef USART_InitStructure;	
	RCC_ClocksTypeDef  tClockTree;
	
	//-------------------------------------------
    // 1. update the system's clock
 // HSI_Sysclock_Init(); //SystemCoreClockUpdate(); //todo use-HSE
	RCC_GetClocksFreq(&tClockTree);
	Debug_Uart_Init();
#if 1
    // printf("\r\n--- SystemCoreClock = %d ---\r\n", SystemCoreClock);
    printf("\r\n SYS-clk = %d", tClockTree.SYSCLK_Frequency);
    printf("\r\n AHB-clk = %d", tClockTree.HCLK_Frequency);
    printf("\r\n APB1-clk = %d", tClockTree.PCLK1_Frequency);
    printf("\r\n APB2-clk = %d", tClockTree.PCLK2_Frequency);
   // printf("\r\n ");
#endif
	
    //-------------------------------------------
    // 2. Initialize and start the SysTick counter and its interrupt. 
    //    take attention: when want to use IRQ_DelayMs(), this must be called first !!!
	if(SysTick_Config(SystemCoreClock / 1000))   // Setup SysTick Timer for 1 millisecond interrupts. 
    { 
        // Capture error / 
	    printf("\r\nSysTick_Config error\r\n");
        while (1);
    }
    // attentio: set to the higher priority
	NVIC_SetPriority(SysTick_IRQn, 0);
    // IT_SYS_DlyMs(100);
    
    //-------------------------------------------
	// 3. priority setting
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
    //-------------------------------------------   
	// Disable the JTAG interface and enable the SWJ interface. // GPIO_Remap_SWJ_JTAGDisable	
	// Enable the AFIO Clock  
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// mapping
    // GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     // AFIO->MAPR |= 0x02000000; // GPIO_Remap_SWJ_JTAGDisable

    /*******************************************/
    // 4. initialize  com 
//	USART_InitStructure.USART_BaudRate = 115200;
//    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//    USART_InitStructure.USART_StopBits = USART_StopBits_1;
//    USART_InitStructure.USART_Parity = USART_Parity_No;
//    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//    // 
//    EVAL_ComInit(E_COM_MAIN, &USART_InitStructure);  


	//-------------------------------------------
	// 5. Initialize  IO output  

	EVAL_OutputInit(O_MCU_LED_1);
	EVAL_OutputInit(O_MCU_LED_2);
	EVAL_OutputInit(O_LAN8720_RST);
	
	
	
	//-------------------------------------------
    // 6. FPGA init
    //IT_SYS_DlyMs(500); // attention: waiting the FPGA to be ready
#if !USE_STM32F407_ONLY
	FPGA_Init();
	FPGA_ResetHardware();
 #endif

	//-------------------------------------------
	// 7. initialize  IO input --- normal and exti interrupt 
	// input
	//EVAL_InputInit(I_HOME_X, IN_MODEL_GPIO);      // //yaolan_20190715
	//EVAL_InputInit(I_HOME_Y, IN_MODEL_GPIO);      // //yaolan_20190715
	//EVAL_InputInit(I_HOME_Z, IN_MODEL_GPIO);      // //yaolan_20190715
	//EVAL_InputInit(I_HOME_M, IN_MODEL_GPIO);      // //yaolan_20190715
#if !USE_STM32F407_ONLY
	EVAL_InputInit(I_FEEDBACK_1, IN_MODEL_EXTI);  //EXTI15-10 PB13// EXTI9_5_IRQn  EXTI_Line7
#endif
	//-------------------------------------------
	// 8. Initialize the spi-flash 
    // SPI_FLASH_Init();
    //-------------------------------------------
	// 9. the timer of the system messages
	PF_InitTimer2();
#if USE_STM32F407_ONLY

	ADC1_Init();//APP_ADC_Init(EN_ADC1);

	ADC2_Init();//APP_ADC_Init(EN_ADC2);
		
	ADC3_Init();//APP_ADC_Init(EN_ADC3);
	
	Elec_Init();
	Beep_Init();
	
	OC_Init();
	OutIn_Motor_Init();
	Turn_Motor_Init();
	// Counter 
	Counter_Check_Init(); 
	Counter_Adjust_Init();
	
	Pump_init();
	Press_Init();
	Valve_Init();
	
	Turn_Motor_Init();
	Mixing_Motor_Init();
	LED_Init();
	LED_Cur_DAC_Init();
	ADC24Bit_Init();
	
	DResistor_Init();
	
#endif
	Beep(1, 400);
}






void Driver_Debug(UINT8 nIndex, UINT32 nData)
{
//	UINT32 nCurTime, nTempTime;
	IO_ UINT16 T1 = 0, T2 = 0;
	IO_ UINT32 i = 0, j = 0,val = 0;
	UINT32 nPress, nADC = 0;
	switch(nIndex)
	{
		case 0:  // OC  and OutIn motor , close is 1, open is 0 , e_Dir_Pos=in, e_Dir_Neg=out
		{
			printf("start\r\n");
			OutIn_Motor_Init();
			for(i = 0; i < 10; i++)
			{
				printf("Turn_Reset_OC=%d, Turn_Selct=%d, OUI_OC=%d, IN_OC=%d\r\n", EVAL_GetInputStatus(I_LED_OC_RST), EVAL_GetInputStatus(I_LED_OC_SEL),\
				EVAL_GetInputStatus(I_MOTOR_OC_OUT), EVAL_GetInputStatus(I_MOTOR_OC_IN));
				IT_SYS_DlyMs(500);
				//IT_SYS_DlyMs(500);
			}
			EVAL_OutputSet(O_OUTIN_MOTOR_EN);
			OutIn_Motor_Run(e_Dir_Neg, 200); //OutIn_Motor_Run(e_Dir_Pos, 500);
			
			T1 = EVAL_GetInputStatus(I_MOTOR_OC_IN);
			T2 = EVAL_GetInputStatus(I_MOTOR_OC_OUT);
			printf("T1=%d, T2=%d\r\n", T1, T2);
			while(EN_CLOSE ==  T2 && EN_CLOSE == T1)
			{
				T1 = EVAL_GetInputStatus(I_MOTOR_OC_IN);
				T2 = EVAL_GetInputStatus(I_MOTOR_OC_OUT);
				printf("T1=%d, T2=%d\r\n", T1, T2);
				IT_SYS_DlyMs(5);
			}
			EVAL_OutputClr(O_OUTIN_MOTOR_EN);
			OutIn_Motor_Run(e_Dir_Pos, 0);
			printf("end\r\n");
		}
		break;
		case 1: // turn motor
		{
			printf("start\r\n");
			Turn_Motor_Init();
			LED_Init();
			LED_Cur_Switch(EN_OPEN);

//			
//			if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_RST) && EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_RST)) 
//			{
//				if(nIndex == EN_LED3) return 0;
//				Turn_Motor_Pass_Kong();
//			}

			i = EN_LED6;
			printf("LED Select Step=%d\r\n", EN_LED2_SELECT_STEP);
			LED_Exec(i, EN_OPEN);
			Turn_Motor_Select_LED(i);
			LED_Cur_Switch(EN_OPEN);
			
			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
			//LED_Exec(i, EN_CLOSE);
			//Turn_Motor_Goto_Postion(e_Dir_Pos, EN_LED2_SELECT_STEP);
			
			
			//Turn_Motor_Enable();
//			Turn_Motor_Dir(e_Dir_Neg); //e_Dir_Pos=anti
//			//
//			for(i = 0; i < 60000; i++)
//			{
//				if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_RST)) break;
//				Turn_Motor_One_Step(TURN_MOTOR_UP_TIME, TURN_MOTOR_DOWN_TIME);
//			}
//			printf("Reset step = %d\r\n", (int)i);
//			
//			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
			
//			Turn_Motor_Dir(e_Dir_Neg); //e_Dir_Pos=anti
//			for(i = 0; i < 50000; i++)
//			{
//				//T1 = 0; T2=0;
//				if(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_SEL)) 
//				{
//					printf("kong Had Checked, pass it\r\n");
//					T1 = i;
//					for(j = 0; j < 1000; j++)//while(EN_CLOSE == EVAL_GetInputStatus(I_LED_OC_SEL))
//					{
//						i++;
//						Turn_Motor_One_Step(TURN_MOTOR_UP_TIME, TURN_MOTOR_DOWN_TIME);
//					}
//					T2 = i;
//					printf("kong had pass had,j=%d, T1=%d, T2=%d, i=%d\r\n\r\n", (int)j, T1, T2, (int)i);
//					IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//					IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//				}
//				Turn_Motor_One_Step(TURN_MOTOR_UP_TIME, TURN_MOTOR_DOWN_TIME);
//			}
			printf("end i=%d\r\n", (int)i);
			printf("end\r\n");
		}
		break;
		case 2: // pump valve
		{
			printf(" start\r\n");
			Turn_Motor_Enable();
			for(i = 2; i < EN_LED_END; i++)
			{
				printf("i=%d\r\n", (int)i);
				Turn_Motor_Select_LED(i);
				IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
				IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
			}
//			Pump_init();
//			Valve_Init();
//			
//			Pump_Speed_Set(PUMP_PWM_LEVEL_BEST);
//			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//			Pump_Speed_Set(PUMP_PWM_LEVEL_CLOSE);
//			
//			for(i = 0; i < 3; i++)
//			{
//				printf("Air Valve i =%d\r\n", i);
//				Valve_Air_Exec(EN_OPEN);
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//				Valve_Air_Exec(EN_CLOSE);
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//			}
//			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//			for(i = 0; i <3; i++)
//			{
//				printf("Liquid Valve i =%d\r\n", i);
//				Valve_Liquid_Exec(EN_OPEN);
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//				Valve_Liquid_Exec(EN_CLOSE);
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//			}
//			printf("Valve end\r\n");
		}
		break;
		case 3: // Mixing motor
		{
			printf(" start\r\n");
//			for(i = 0; i <3; i++)
//			{
//				printf("i =%d\r\n", i);
//				Mixing_Motor_Run();
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//				Mixing_Motor_Stop();
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//			}
			Counter_Adjust_Init();
			Counter_Adjust_Start(30000);
			printf("Valve end\r\n");
		}
		break;
		case 4: // press
		{
			printf("I2C press start\r\n");
			for(i = 0; i < 10; i++)
			{
				nPress = Get_Press_I2C();
				printf("press value=%d, nPress=%d\r\n", (int)nPress, (int)(nPress/2 - 4194304));
			}
			printf("I2C press end\r\n");
		}
		break;
		case 5: // ADCs include cur 
		{
			printf("ADC start\r\n");
			ADC3_Init();
			IT_SYS_DlyMs(100);
			
			val = Get_56V_Cur_ADC();
			printf("56V, ADC=%d, V=%d\r\n", (int)val, (int)val*3300/4095);  
		
			val = Get_XK_ADC();
			printf("XK, ADC=%d, V=%d\r\n", (int)val, (int)val*3300/4095); 
			
			val = Get_LED_Cur_ADC();
			printf("LED CUR, ADC=%d, V=%d\r\n", (int)val, (int)val*3300/4095); 
//			
//			val = HW_Get_ADC_HGB();
//			printf("HGB, ADC=%d, V=%d\r\n", val, val*3300/4095); 
//			
//			val = HW_Get_ADC_CRP();
//			printf("CRP, ADC=%d, V=%d\r\n", val, val*3300/4095); 
//						
			val = Get_Elec_ADC();
			printf("ELEC, ADC=%d, V=%d\r\n", (int)val, (int)val*3300/4095); 
			
			val = Get_12V_N_ADC();  //2.5V
			printf("12V N, ADC=%d, V=%d\r\n", (int)val, (int)val*3300/4095); 
			
			val = Get_12V_P_ADC();
			printf("12V_P, ADC=%d, V=%d\r\n", (int)val, (int)val*3300/4095); 
			printf("ADC end\r\n");
			IT_SYS_DlyMs(100);
		}
		break;
		case 6: // LED OPEN by turn
		{
			printf("start\r\n");
			LED_Init();
			LED_Cur_Switch(EN_OPEN);
			for(i = 0; i < EN_LED_END; i++)
			{
				printf("i=%d\r\n", (int)i);
				LED_Exec(i, EN_OPEN);
				IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
				LED_Exec(i, EN_CLOSE);
			}
			printf("end\r\n");
		}
		break;
		case 7: // HGB CRP LED
		{
			printf("start\r\n");
			LED_Init();
			LED_Cur_Switch(EN_OPEN);
			Turn_Motor_Init();
			IT_SYS_DlyMs(100);
			
			printf("HGB start\r\n");
	//		Turn_Motor_Reset();
			LED_Exec(HGB_LED_INDEX, EN_OPEN);
			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
			Turn_Motor_Select_LED(HGB_LED_INDEX);
			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
			LED_Exec(HGB_LED_INDEX, EN_CLOSE);
			printf("HGB end\r\n");
			
			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
			
			printf("CRP start\r\n");
	//	Turn_Motor_Reset();
			LED_Exec(CRP_LED_INDEX, EN_OPEN);
			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
			Turn_Motor_Select_LED(CRP_LED_INDEX);
			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
			LED_Exec(CRP_LED_INDEX, EN_CLOSE);
			printf("CRP end\r\n");
			
			printf("end\r\n");

		}
		break;
		case 8:
		{	
			printf("start\r\n");
			LED_Cur_ADC_Check_Channel(HGB_LED_INDEX);
			LED_Cur_DAC_Set(0);
			nADC = Get_LED_Cur_ADC();
			printf("LED Cur ADC =%d\r\n", (int)nADC);
			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
			val = 0;
			for(i = 0; i < 4095;)
			{
				i += 100;
				LED_Cur_DAC_Set(i);
				IT_SYS_DlyMs(200);
				printf("i=%d LED Cur ADC=%d, DAC=%d\r\n", (int)val, (int)Get_LED_Cur_ADC(), (int)i);
				val++;
				IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
				//IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
			}
			printf(" nd\r\n");
		
		}
		break;
		case 9: // pump, wave
		{
			printf("start\r\n");
			DRegister_Write(0, 250);
			IT_SYS_DlyMs(500);
			DRegister_Write(1, 250);
			IT_SYS_DlyMs(500);
			DRegister_Write(2, 250);
			IT_SYS_DlyMs(500);
			DRegister_Write(3, 250);
			
//			for(val = 50; val <= 250; val += 50)
//			{
//				for(i = 0; i < 40000; i++)
//				{
//					DRegister_Write(0, 0x55);
//					printf("i=%d, val=%d, val=%X\r\n", i, val, val);
//					IT_SYS_DlyMs(10);
//					//IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//					//IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//				}
//			}
			
			//WBC_48V_Self_Check();
			//Valve1_Self_Check();
			//Valve2_Self_Check();
	
//			
//			Valve_Liquid_Exec(EN_OPEN);
//			IT_SYS_DlyMs(500);
//			Valve_Liquid_Exec(EN_CLOSE);
//			IT_SYS_DlyMs(500);
//			
			//-----pump
//			Valve_Air_Exec(EN_OPEN);
//			Valve_Liquid_Exec(EN_OPEN);
//			IT_SYS_DlyMs(500);
			
			//HW_Valve_On(EN_VALVE_AIR);
//			Valve_Air_Exec(EN_CLOSE);
//			Valve_Liquid_Exec(EN_CLOSE);
//			TIM_SetCompare2(PUMP_PWM_TIM, PUMP_PWM_LEVEL_CLOSE);//Pump_Exec(e_Dir_Pos, PUMP_PWM_LEVEL_CLOSE);

			//----------- out in
//			OutIn_Motor_Enable();
//			OutIn_Motor_Out();
//			//OutIn_Motor_In();
//			for(i = 0; i < 8000; i++){
//				OutIn_Motor_Run(OUTIN_MOTOR_PULSE_UP_TIME, OUTIN_MOTOR_PULSE_DOWN_TIME);
//			}
			//---------- press
			//Pump_Exec(e_Dir_Pos, 14999);
			//Pump_Self_Check();

			//Get_Press_Value(5);
			printf("end\r\n");
		}
		break;
		case 10: // DAC
		{
			LED_Cur_DAC_Init();
			for(i = 0; i < 4095;)
			{
				//v = 3300*i/4095;
				printf("i = %d, v = %d, V = %d\r\n",(int)i, (int)i*3300/4095, DAC_GetDataOutputValue(DAC_Channel_1));
				DAC_SetChannel1Data(DAC_Align_12b_R, i);//LED_Cur_Adjust_Set(i);
				IT_SYS_DlyMs(500);
				IT_SYS_DlyMs(500);
				IT_SYS_DlyMs(500);
				IT_SYS_DlyMs(500);
				i += 100;
			}
		}
		break;
		case 11:  // LED cur
		{

			
//			printf("start\r\n");
//			Turn_Motor_Power(EN_OPEN);
//			Turn_Motor_ClockWise(20000);
//			Turn_Motor_Power(EN_CLOSE);
	
//			__enable_irq();
//			for(i = 0; i < 10; i++)
//			{
//				Fix_Motor_Enable();
//				Fix_Motor_ClockWise();
//				IT_SYS_DlyMs(200);
//				Fix_Motor_AntiClockWise();
//				Fix_Motor_Disable();
//				IT_SYS_DlyMs(200);
//			}
//			Fix_Motor_AntiClockWise();
//			Fix_Motor_Disable();
//			for(i = 0; i < 2000; i++)
//			{
//				Fix_Motor_Run(300, 600);
//			}
//			printf("Fix Motor end\r\n");
			printf("end\r\n");
		}
		break;
		case 12: //C   AD7799
		{
			printf("AD7799 start\r\n");
//			ADC24Bit_Init();
//			LED_Cur_Switch(EN_OPEN);
//			LED_Exec(EN_LED1, EN_OPEN);
//			Turn_Motor_Select_LED(EN_LED1);
//			IT_SYS_DlyMs(200);

//			for(i = 0; i < 5000; i++)
//			{
//				nADC = AD7799_Get_ADC_Value(AD7799_Get_Out_Data());
//     			printf("AD7799 CH1, adc=%d, V=%6.2f\r\n", (int)nADC, AD7799_Get_Value(nADC));
//				
//				AD7799_CS_HIGH;
//				IT_SYS_DlyMs(100);
//				AD7799_CS_LOW;
//				IT_SYS_DlyMs(10);
				
//				printf("PC7=%d\r\n", GPIO_ReadInputDataBit(ADC24BIT_CS_PORT, ADC24BIT_CS_PIN));
//				IT_SYS_DlyMs(10);
				
//				nADC = AD7799_GetRegisterValue(AD7799_REG_MODE, 2);
//				printf("\r\nAD_M_1: %d\r\n", (int)nADC);
//				IT_SYS_DlyMs(10);
//				// mode and updateR set, continuous Coversion Mode and 50Hz Update Rate(default:16.7)
//				nADC = AD7799_MODE_CONT | AD7799_MODE_RATE(AD7799_MODE_UPDATE_50);
//				AD7799_SetRegisterValue(AD7799_REG_MODE, nADC, 2);
//				IT_SYS_DlyMs(10);
//				nADC = AD7799_GetRegisterValue(AD7799_REG_MODE, 2);
//				printf("AD_M_2: %d\r\n", (int)nADC);
//				IT_SYS_DlyMs(10);
				
//					nADC = AD7799_MODE_CONT | AD7799_MODE_RATE(AD7799_MODE_UPDATE_50);
//					//printf("D=%d\r\n", (UINT32)nADC);
//					AD7799_SetRegisterValue(AD7799_REG_MODE, nADC, 2);
//				ADC24Bit_Init();
				
//       			nADC = AD7799_Get_ADC_Value(AD7799_Get_Out_Data());
//				printf("AD7799 CH1, adc=%d, V=%6.2f\r\n", (int)nADC, AD7799_Get_Value(nADC));
//				IT_SYS_DlyMs(5);
//			}

			
			for(i = 0; i < 10; i++)
			{
				// CH1_W3
				//DRegister_Write(2, nData);  
				AD7799_SetChannel(AD7799_CH_AIN1P_AIN1M);
				nADC = AD7799_Get_Out_Data();
				printf("AD7799 CH1, register=%d, adc=%d, V=%6.2f\r\n", (int)nADC, (int)AD7799_Get_ADC_Value(nADC), AD7799_Get_Value(nADC));
				IT_SYS_DlyMs(500);
				IT_SYS_DlyMs(500);

				// CH2_W4
				//DRegister_Write(2, nData);  
				AD7799_SetChannel(AD7799_CH_AIN2P_AIN2M);
				nADC = AD7799_Get_Out_Data();
				printf("AD7799 CH2, register=%d, adc=%d, V=%6.2f\r\n", (int)nADC, (int)AD7799_Get_ADC_Value(nADC), AD7799_Get_Value(nADC));
				IT_SYS_DlyMs(500);
				IT_SYS_DlyMs(500);
				
			}
			printf("AD7799 end\r\n");
		}
		break;
		case 13: //D
		{
			printf("start\r\n");

			for(i = 0; i < EN_LED_END; i++)
			{
				printf("i = %d\r\n", i);
				LED_All_Reset();
				LED_Exec(i, EN_OPEN); // open led
				LED_Cur_ADC_Check_Channel(i);
				DAC_SetChannel1Data(DAC_Align_12b_R, 2480);
				LED_Cur_Switch(EN_OPEN);
				IT_SYS_DlyMs(500);
				IT_SYS_DlyMs(500);
				IT_SYS_DlyMs(500);
				IT_SYS_DlyMs(500);
				LED_Exec(i, EN_CLOSE);
			}
			
//			for(i = 0; i < 30; i++)
//			{
//				LED_Cur_Switch(EN_OPEN);
//				LED_Exec(EN_LED1, EN_OPEN);LED_Exec(EN_LED2, EN_OPEN);LED_Exec(EN_LED3, EN_OPEN);LED_Exec(EN_LED4, EN_OPEN);
//				LED_Exec(EN_LED5, EN_OPEN);LED_Exec(EN_LED6, EN_OPEN);LED_Exec(EN_LED7, EN_OPEN);LED_Exec(EN_LED0, EN_OPEN);
//				IT_SYS_DlyMs(100);
//				printf("i =%d\r\n", i);
//				//LED_Exec(i, EN_CLOSE)
//				LED_Exec(EN_LED1, EN_CLOSE);LED_Exec(EN_LED2, EN_CLOSE);LED_Exec(EN_LED3, EN_CLOSE);LED_Exec(EN_LED4, EN_CLOSE);
//				LED_Exec(EN_LED5, EN_CLOSE);LED_Exec(EN_LED6, EN_CLOSE);LED_Exec(EN_LED7, EN_CLOSE);LED_Exec(EN_LED0, EN_CLOSE);
//				LED_Cur_Switch(EN_CLOSE);
//				IT_SYS_DlyMs(100);
//			}
			
//			LED_Cur_Switch(EN_OPEN);
//			printf("power open\r\n");
//			for(i = 0; i < 10; i++)
//			{
//				val = Get_LED_Cur_ADC();
//				printf("LED CUR ADC=%d,V=%d\r\n", val, val*3300/4096);
//			}
			
			printf("end\r\n");
//			printf("ADC1 and ADC2 test start\r\n");
//			UINT32 nCurTicks, nLstTicks;
//			Eable_ADC(EN_ADC1);
//			Eable_ADC(EN_ADC2);
//			nCurTicks = IT_SYS_GetTicks();
//			nLstTicks = nCurTicks;
//			while (nCurTicks <= (nLstTicks + 10))
//			{
//				Data_Circle_Handle(EN_WBC_TEST);
//				Data_Circle_Handle(EN_RBC_PLT_TEST);
//				nCurTicks = IT_SYS_GetTicks();
//			}
//			Disable_ADC(EN_ADC1);
//			Disable_ADC(EN_ADC1);
//			printf("ADC1 and ADC2 test end\r\n");
		}
		break;
		case 14: // E, turn motor
		{
			LED_Init();
			printf("init\r\n");
			LED_Cur_Switch(EN_OPEN);
			LED_Exec(5, EN_OPEN);
				
			printf("start\r\n");
			Turn_Motor_Init();
//			Turn_Motor_Power(EN_OPEN);

	//		Turn_Motor_Reset();
			printf("reset finished\r\n");
			
			for(i = 0; i < 6; i++)
			{
//				Turn_Motor_Select_LED(i);
				IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//				Turn_Motor_Reset();
				IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
			}
			
			
//			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//			Turn_Motor_ClockWise(TURN_MOTOR_MAX_CLOCKWISE_STEP);
//			printf("111\r\n");
//			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//			Turn_Motor_ClockWise(TURN_MOTOR_MAX_CLOCKWISE_STEP);
//			printf("222\r\n");
//			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//			Turn_Motor_ClockWise(TURN_MOTOR_MAX_CLOCKWISE_STEP);
//			printf("333\r\n");
//			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);

//			Turn_Motor_Reset();
//			printf("reset finished\r\n");
//			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//			Turn_Motor_Anti_ClockWise(TURN_MOTOR_MAX_CLOCKWISE_STEP);
//			printf("444\r\n");
//			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//			Turn_Motor_Anti_ClockWise(TURN_MOTOR_MAX_CLOCKWISE_STEP);
//			printf("555\r\n");
//			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//			Turn_Motor_Anti_ClockWise(TURN_MOTOR_MAX_CLOCKWISE_STEP);
//			printf("666\r\n");
//			IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//			
//			Turn_Motor_Power(EN_CLOSE);
			printf("end\r\n");
		}
		break;
		case 15: //F , mix motor
		{
			printf("F startd M=%d\r\n", g_Test_Mode);
			LED_All_Reset();
			i = EN_LED6;
//			Turn_Motor_Power(EN_OPEN);
			LED_Exec(i, EN_OPEN); // open led
//			Turn_Motor_Select_LED(HGB_LED_INDEX); // select led 
			LED_Cur_ADC_Check_Channel(i);
			
			DAC_SetChannel1Data(DAC_Align_12b_R, 2480);
			LED_Cur_Switch(EN_OPEN);
			
			//LED_Cur_Auto_Adjust(HGB_LED_CUR_ADJUST_VALUE);
//			g_Test_Mode = EN_HGB_TEST;
//			printf("HGB Mode Set Finished M=%d\r\n", g_Test_Mode);
//			for(i = 0; i < 4095;)
//			{
//				//v = 3300*i/4095;
//				printf("i = %d, DAC_V = %d, ADC = %d\r\n",(int)i, (int)i*3300/4095, Get_LED_Cur_ADC());
//				DAC_SetChannel1Data(DAC_Align_12b_R, i);//LED_Cur_Adjust_Set(i);
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//				i += 100;
//			}
//			Mixing_Motor_Init();
//			printf("start\r\n");
//			for(i = 0; i < 30; i++)
//			{
//				Mixing_Motor_Run();
//				printf("i = %d\r\n", i);
//				IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//				Mixing_Motor_Stop();
//				IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);IT_SYS_DlyMs(500);
//			}
//			printf("end\r\n");
		}
		break;
		default:break;	
	}	
}






//#endif

