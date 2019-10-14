// Scope_Eval.c

#ifndef    __SCOPE_EVAL_C__
#define    __SCOPE_EVAL_C__

#include "ChainHeader.h"
#include "stm32f4xx_rcc.h"


//-----------------------------------------------------------------------------------------
// definition


//-----------------------------------------------------------------------------------------
// the const group

// com-definition
USART_TypeDef* CODE_ COM_USART[COM_NUM]       = {COMM1, COMM2, COMM3}; 
UINT32         CODE_ COM_USART_CLK[COM_NUM]   = {COM1_CLK, COM2_CLK, COM3_CLK};
UINT16         CODE_ COM_ET_IRQn[COM_NUM]     = {COM1_IRQn, COM2_IRQn, COM3_IRQn};
//
GPIO_TypeDef*  CODE_ COM_TX_PORT[COM_NUM]     = {COM1_TX_GPIO_PORT, COM2_TX_GPIO_PORT, COM3_TX_GPIO_PORT};
UINT16         CODE_ COM_TX_PIN[COM_NUM]      = {COM1_TX_PIN, COM2_TX_PIN, COM3_TX_PIN};
UINT32         CODE_ COM_TX_PORT_CLK[COM_NUM] = {COM1_TX_GPIO_CLK, COM2_TX_GPIO_CLK, COM3_TX_GPIO_CLK};
//
GPIO_TypeDef*  CODE_ COM_RX_PORT[COM_NUM]     = {COM1_RX_GPIO_PORT, COM2_RX_GPIO_PORT, COM3_RX_GPIO_PORT};
UINT16         CODE_ COM_RX_PIN[COM_NUM]      = {COM1_RX_PIN, COM2_RX_PIN, COM3_RX_PIN};
UINT32         CODE_ COM_RX_PORT_CLK[COM_NUM] = {COM1_RX_GPIO_CLK, COM2_RX_GPIO_CLK, COM3_RX_GPIO_CLK};
//
UINT8          CODE_ COM_AF_TX_PINSOURCE[COM_NUM] = {COM1_AF_TX_PIN_SOURCE, COM2_AF_TX_PIN_SOURCE, COM3_AF_TX_PIN_SOURCE};
UINT8          CODE_ COM_AF_RX_PINSOURCE[COM_NUM] = {COM1_AF_RX_PIN_SOURCE, COM2_AF_RX_PIN_SOURCE, COM3_AF_RX_PIN_SOURCE};
UINT8          CODE_ COM_AF_UART[COM_NUM] = {COM1_AF_UART, COM2_AF_UART, COM3_AF_UART};

// Output def --- led and speaker
GPIO_TypeDef*  CODE_ OUT_PORT[OUTPUT_NUM]= 
{
    //
    OUT_STATUS_LED1_GPIO_PORT, 
    OUT_STATUS_LED2_GPIO_PORT, 
    OUT_MCU_LED1_GPIO_PORT,
    OUT_MCU_LED2_GPIO_PORT,
    //
    OUT_LAN8720_RST_GPIO_PORT,
    //
    OUT_HEAT1_GPIO_PORT,
	OUT_HEAT1_GPIO_PORT
};
UINT16 CODE_ OUT_PIN[OUTPUT_NUM]= 
{
    //
    OUT_STATUS_LED1_GPIO_PIN, 
    OUT_STATUS_LED2_GPIO_PIN, 
    OUT_MCU_LED1_GPIO_PIN,
    OUT_MCU_LED2_GPIO_PIN,
    //
    OUT_LAN8720_RST_GPIO_PIN,
    //
    OUT_HEAT1_GPIO_PIN,
	OUT_HEAT1_GPIO_PIN
};
UINT32 CODE_ OUT_CLK[OUTPUT_NUM]= 
{
    //
    OUT_STATUS_LED1_GPIO_CLK, 
    OUT_STATUS_LED2_GPIO_CLK, 
    OUT_MCU_LED1_GPIO_CLK,
    OUT_MCU_LED2_GPIO_CLK,
    //
    OUT_LAN8720_RST_GPIO_CLK,
    //
    OUT_HEAT1_GPIO_CLK,
	OUT_HEAT1_GPIO_CLK
};

// Input def  --- commom input and exti interrupt
GPIO_TypeDef* CODE_ IN_PORT[INPUT_NUM]= 
{
    //
    IN_HOME_1_GPIO_PORT,
	IN_HOME_2_GPIO_PORT,
	IN_HOME_3_GPIO_PORT,
	IN_HOME_4_GPIO_PORT,
	IN_HOME_5_GPIO_PORT,
	IN_HOME_6_GPIO_PORT,
	//
	IN_MT_STATUS_GPIO_PORT
};
UINT16 CODE_ IN_PIN[INPUT_NUM]=
{
    //
    IN_HOME_1_GPIO_PIN,
	IN_HOME_2_GPIO_PIN,
	IN_HOME_3_GPIO_PIN,
	IN_HOME_4_GPIO_PIN,
	IN_HOME_5_GPIO_PIN,
	IN_HOME_6_GPIO_PIN,
	//
	IN_MT_STATUS_GPIO_PIN
};	
UINT32 CODE_ IN_CLK[INPUT_NUM]=
{
    //
    IN_HOME_1_GPIO_CLK,
	IN_HOME_2_GPIO_CLK,
	IN_HOME_3_GPIO_CLK,
	IN_HOME_4_GPIO_CLK,
	IN_HOME_5_GPIO_CLK,
	IN_HOME_6_GPIO_CLK,
	//
	IN_MT_STATUS_GPIO_CLK
};
UINT16 CODE_ IN_ET_LINE[INPUT_NUM]=
{
    //
    IN_HOME_1_ET_LINE,
	IN_HOME_2_ET_LINE,
	IN_HOME_3_ET_LINE,
	IN_HOME_4_ET_LINE,
	IN_HOME_5_ET_LINE,
	IN_HOME_6_ET_LINE,
	//
	IN_MT_STATUS_ET_LINE
};
UINT16 CODE_ IN_ET_PORT[INPUT_NUM]=
{
    //
    IN_HOME_1_ET_PORT,
	IN_HOME_2_ET_PORT,
	IN_HOME_3_ET_PORT,
	IN_HOME_4_ET_PORT,
	IN_HOME_5_ET_PORT,
	IN_HOME_6_ET_PORT,
	//
	IN_MT_STATUS_ET_PORT
};
UINT16 CODE_ IN_ET_PIN[INPUT_NUM]=
{
    //
    IN_HOME_1_ET_PIN,
	IN_HOME_2_ET_PIN,
	IN_HOME_3_ET_PIN,
	IN_HOME_4_ET_PIN,
	IN_HOME_5_ET_PIN,
	IN_HOME_6_ET_PIN,
	//
	IN_MT_STATUS_ET_PIN
};
UINT16 CODE_ IN_ET_IRQn[INPUT_NUM]=
{
    //
    IN_HOME_1_ET_IRQn,
	IN_HOME_2_ET_IRQn,
	IN_HOME_3_ET_IRQn,
	IN_HOME_4_ET_IRQn,
	IN_HOME_5_ET_IRQn,
	IN_HOME_6_ET_IRQn,
	//
	IN_MT_STATUS_ET_IRQn
};


//-----------------------------------------------------------------------------------------
// global variables


//-----------------------------------------------------------------------------------------
// define "USE_FULL_ASSERT" in the project options setting
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	
    /* Infinite loop */
#if 1	
    while (1)
    {
    
    }
#endif

}

//-----------------------------------------------------------------------------------------
// macro defined for "printf" debugging
PUTCHAR_PROTOTYPE
{
	// Place your implementation of fputc here 
	// e.g. write a character to the USART 
#if 0
	
#else

	PL_COM_SendChar(ch);   // will be recovered

#endif

	return ch;
}



//void HSI_Sysclock_Init(void)
//{
//	RCC_DeInit();
//	RCC_HSICmd(ENABLE);
//	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

//	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
//	PWR->CR |= PWR_CR_VOS;
//	
//	RCC_HCLKConfig(RCC_SYSCLK_Div1);
//	RCC_PCLK2Config(RCC_HCLK_Div2);
//	RCC_PCLK1Config(RCC_HCLK_Div4);
//	
//	RCC_PLLCmd(DISABLE);
//	RCC_PLLConfig(RCC_PLLSource_HSI, 16, 336, 2, 4);
//	RCC_PLLCmd(ENABLE);
//	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
//	
//	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//	while(RCC_GetSYSCLKSource() != 0x08);
//}

//-----------------------------------------------------------------------------------------
// initialization for the code block
void EVAL_Init(void)
{
    USART_InitTypeDef USART_InitStructure;	
	RCC_ClocksTypeDef  tClockTree;
	
	//-------------------------------------------
    // 1. update the system's clock
 // HSI_Sysclock_Init(); //SystemCoreClockUpdate(); //todo use-HSE
	RCC_GetClocksFreq(&tClockTree);
#if 1
    // printf("\r\n--- SystemCoreClock = %d ---\r\n", SystemCoreClock);
    printf("\r\n sys-clk = %d", tClockTree.SYSCLK_Frequency);
    printf("\r\n AHB-clk = %d", tClockTree.HCLK_Frequency);
    printf("\r\n APB1-clk = %d", tClockTree.PCLK1_Frequency);
    printf("\r\n APB2-clk = %d", tClockTree.PCLK2_Frequency);
    printf("\r\n ");
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
	USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    // 
    EVAL_ComInit(E_COM_MAIN, &USART_InitStructure);  
    //yaolan_20190715  EVAL_ComInit(E_COM_SAMPLING, &USART_InitStructure);  
	// EVAL_ComInit(E_COM_NULL, &USART_InitStructure);  

	//-------------------------------------------
	// 5. Initialize  IO output  
	// led and the reset pin of the FPGA 
    // EVAL_OutputInit(O_STATUS_LED_1);
	//EVAL_OutputInit(O_STATUS_LED_2);
	EVAL_OutputInit(O_MCU_LED_1);
	EVAL_OutputInit(O_MCU_LED_2);
	EVAL_OutputInit(O_LAN8720_RST);
	//EVAL_OutputInit(O_HEAT_1);//yaolan_20190715
	//EVAL_OutputInit(O_HEAT_2);//yaolan_20190715
	
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
//	// ...?
//	ADC_SoftwareStartConv(ADC1);
//	IT_SYS_DlyMs(10);
//	Disable_ADC(EN_ADC1);
//	memset((void*)&ADC1_Status, 0, sizeof(ADC_Status_InitTypeDef));	
//	
	ADC2_Init();//APP_ADC_Init(EN_ADC2);
		
	ADC3_Init();//APP_ADC_Init(EN_ADC3);
	
	Elec_Init();
	Beep_Init();
	Pump_init();
	Valve_Init();
	OC_Init();
	Press_Init();
//	Fix_Motor_Init();
//	OutIn_Motor_Init();
#endif
	Beep(400);
}


//-----------------------------------------------------------------------------------------
// com control
void EVAL_ComInit(Com_TypeDef eCom, USART_InitTypeDef* USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;

    // 1. enable GPIO clock 
	RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[eCom] | COM_RX_PORT_CLK[eCom], ENABLE);
    // 2. enable UART clock 
    if (eCom == E_COM_SAMPLING)   // com_2  
    {
        RCC_APB1PeriphClockCmd(COM_USART_CLK[eCom], ENABLE);  // APB1
    }
    else                 // com_1 or com_3
    {
        RCC_APB2PeriphClockCmd(COM_USART_CLK[eCom], ENABLE);  // APB2
    }
	// 3. configure AFIO  
	GPIO_PinAFConfig(COM_TX_PORT[eCom],COM_AF_TX_PINSOURCE[eCom],COM_AF_UART[eCom]); // tx-pin
	GPIO_PinAFConfig(COM_RX_PORT[eCom],COM_AF_RX_PINSOURCE[eCom],COM_AF_UART[eCom]); // rx-pin
    // 4. IO configuration 
    GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[eCom] | COM_RX_PIN[eCom]; // tx-pin and rx-pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // AF model
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 50MHz 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // push output
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       // 
    GPIO_Init(COM_TX_PORT[eCom],&GPIO_InitStructure);
	// 5. USART configuration 
    USART_Init(COM_USART[eCom], USART_InitStruct);	
    // 6. Enable USART
    USART_Cmd(COM_USART[eCom], ENABLE);
	// 7. enable Receive and Transmit interrupts 
    USART_ITConfig(COM_USART[eCom], USART_IT_TC, ENABLE);      //  TX_INT = 1;
    USART_ITConfig(COM_USART[eCom], USART_IT_RXNE, ENABLE);    //  RX_INT = 1;	
	// 8. configure and enable USART interrupt  
    NVIC_InitStructure.NVIC_IRQChannel = COM_ET_IRQn[eCom];       
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
}

//-----------------------------------------------------------------------------------------
// GPIO output contorl 
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

//-----------------------------------------------------------------------------------------
// GPIO input contorl 
void EVAL_InputInit(Input_TypeDef eIn, InModel_Typedef eModel)
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
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
    GPIO_Init(IN_PORT[eIn], &GPIO_InitStructure); 

    // 3. extinal-interrupt model 
    if (eModel == IN_MODEL_EXTI)     
    {
        // 1). enable the SYSCFG-clock
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);       
        // 2). Connects EXTI Line to Button GPIO Pin 
        SYSCFG_EXTILineConfig(IN_ET_PORT[eIn], IN_ET_PIN[eIn]);    
        // 3). Configure EXTI line 
        EXTI_InitStructure.EXTI_Line = IN_ET_LINE[eIn];
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;   
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  // all pins are rising detected         
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;               // enable interrupt line 		
        EXTI_Init(&EXTI_InitStructure);                           
        // 4). enable and set input EXTI Interrupt to the lowest priority 
        NVIC_InitStructure.NVIC_IRQChannel = IN_ET_IRQn[eIn];
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        // enable interrupt
        NVIC_Init(&NVIC_InitStructure);    	
    }


}

UINT8 EVAL_InputGetState(Input_TypeDef eIn)
{
    return GPIO_ReadInputDataBit(IN_PORT[eIn], IN_PIN[eIn]);
}


//// adc for cur check
//void  Adc_Init(EN_TypeADC eType)
//{    
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	ADC_CommonInitTypeDef ADC_CommonInitStructure;
//	ADC_InitTypeDef       ADC_InitStructure;

//	ADC_DeInit();
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOF, ENABLE);//使能GPIOA时钟
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2, ENABLE); //ADC1 2
//	if(eType == EN_ADC1)
//	{
//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//PC2_CH2  HGB
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOC, &GPIO_InitStructure);

//		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PC3_CH3  CRP
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
//		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOC, &GPIO_InitStructure);
//		
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
//	}else if(eType == EN_ADC2){
//		
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,ENABLE);	  //ADC2复位
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2,DISABLE);	//复位结束	 
//	}else if(eType == EN_ADC3){

//	}
//	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
//	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
//	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //DMA失能
//	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
//	ADC_CommonInit(&ADC_CommonInitStructure);//初始化
//	//
//	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式	
//	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
//	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;//ADC_ExternalTrigConv_None; ///
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
//	ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1 
//	//
//	if(eType == EN_ADC1)
//	{
//		ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化
//		ADC_Cmd(ADC1, ENABLE);//开启AD转换器	
//	}else if(eType == EN_ADC2){
//		ADC_Init(ADC2, &ADC_InitStructure);//ADC初始化
//		ADC_Cmd(ADC2, ENABLE);//开启AD转换器	
//	}else if(eType == EN_ADC3){
//		// ...
//	}
//	// ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_480Cycles ); //ADC1,ADC通道,480个周期,提高采样时间可以提高精确度		
//}	


//-----------------------------------------------------------------------------------------
// 
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


#endif


