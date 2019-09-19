// Scope_Eval.c

#ifndef    __DRIVER_C__
#define    __DRIVER_C__

#include "ChainHeader.h"


IO_ UINT8 g_Elec_Status = 0;
//IO_ UINT16 g_ADC2_Value[ADC2_CHECK_NUM] = {0};
IO_ UINT16 g_ADC3_Value[ADC3_CHECK_NUM] = {0};

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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;                      
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
	//DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
	//while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
	DMA_Cmd(DMA2_Stream0, ENABLE);
}

// ADC1 PA5-WBC
void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	// PA5
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5;//GPIO_Pin_5, PA5, PA6
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_6;//GPIO_Pin_5, PA5, PA6
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//ADC1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
	// Common Set
	ADC_CommonInitStructure.ADC_Mode	= ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInit(&ADC_CommonInitStructure);
	// ADC Set
	ADC_InitStructure.ADC_Resolution	= ADC_Resolution_12b;
#if DOUBLE_ADC_CHANNEL
	ADC_InitStructure.ADC_ScanConvMode  = DISABLE;
	ADC_InitStructure.ADC_NbrOfConversion = 2;
#else
	ADC_InitStructure.ADC_ScanConvMode  = DISABLE;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
#endif
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC_DataAlign_Left;//ADC_DataAlign_Right;
	ADC_Init(ADC1, &ADC_InitStructure);	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC1_DMA_Config();
	ADC_DMACmd(ADC1, ENABLE);
	
#if DOUBLE_ADC_CHANNEL
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles); //ADC_SampleTime_3Cycles
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_3Cycles); //ADC_SampleTime_3Cycles
#else
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles); //ADC_SampleTime_3Cycles
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
	if (DMA_GetITStatus(DMA2_Stream3, DMA_IT_HTIF0) == SET)  
	{
		DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_HTIF0);
		ADC2_Status.nSFlag = 1;
		ADC2_Status.nID++;
	}
	
	if (DMA_GetITStatus(DMA2_Stream3, DMA_IT_TCIF0) == SET)  
	{
		DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_TCIF0);
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
	
	//DMA_DoubleBufferModeConfig(DMA2_Stream0,(uint32_t)&g_ADC_Buffer_2,DMA_Memory_0);//DMA_Memory_0?????
	//DMA_DoubleBufferModeCmd(DMA2_Stream0,ENABLE);
	DMA_Init(DMA2_Stream2, &DMA_InitStructure); 
		
	// NVIC
	DMA_ClearITPendingBit(DMA2_Stream3,DMA_IT_HTIF0);
	DMA_ClearITPendingBit(DMA2_Stream3,DMA_IT_TCIF0);
	DMA_ITConfig(DMA2_Stream3,DMA_IT_TC,ENABLE);
	DMA_ITConfig(DMA2_Stream3,DMA_IT_HT,ENABLE);	
		
	NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;                      
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
	//DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
	//while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
	DMA_Cmd(DMA2_Stream3, ENABLE);
}

// ADC1 PA5-WBC
void ADC2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	// PA5
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5;//GPIO_Pin_0, PB0, ADC2_IN8
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//ADC1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);
	// Common Set
	ADC_CommonInitStructure.ADC_Mode	= ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInit(&ADC_CommonInitStructure);
	// ADC Set
	ADC_InitStructure.ADC_Resolution	= ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode  = DISABLE;
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
	//ADC_SoftwareStartConv(ADC1);
}


u16 Get_Adc(UINT8 nCh, UINT8 nTime)   
{
	UINT8 i = 0;
	UINT32 nVal = 0;
	
	for(i = 0; i < nTime; i++)
	{
		ADC_RegularChannelConfig(ADC1, nCh, 1, ADC_SampleTime_480Cycles );	    
		ADC_SoftwareStartConv(ADC1);		
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
		nVal += ADC_GetConversionValue(ADC1);	
	}
	nVal /= nTime;
	return nVal;
}

void ADC3_GPIO_Init(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOF, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;//GPIO_Pin_5, PF6_ADC3_CH5, LED_CUR
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;//GPIO_Pin_3, PC3_ADC3_IN13, XK
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;//GPIO_Pin_9, PF9_ADC3_IN7, 56V_CUR
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;//GPIO_Pin_0, PC0_ADC123_IN10, Temperature
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;//GPIO_Pin_0, PA0_ADC3_IN0, CRP or HGB
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_2; // PC2_ADC123_IN12 , Press
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
		
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
		//ADC_DMACmd(ADC3, ENABLE);
		// 
		ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);  // CRP HGB
		ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 2, ADC_SampleTime_3Cycles);  // LED cur
		ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 3, ADC_SampleTime_3Cycles);  // 56v cur
		ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 4, ADC_SampleTime_3Cycles); // temperature
		ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 5, ADC_SampleTime_3Cycles); // press
		ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 6, ADC_SampleTime_3Cycles); // XK





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
		
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	 
	    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	
		// Common Set
		ADC_CommonInitStructure.ADC_Mode	= ADC_Mode_Independent;
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //ADC_DMAAccessMode_1; //ADC_DMAAccessMode_Disabled
		ADC_CommonInit(&ADC_CommonInitStructure);
		// ADC Set
		ADC_InitStructure.ADC_Resolution	= ADC_Resolution_12b;
		ADC_InitStructure.ADC_ScanConvMode  = DISABLE;//ENABLE;//DISABLE;
		ADC_InitStructure.ADC_NbrOfConversion = ADC3_CHECK_NUM;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//DISABLE;//ENABLE;// ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConvEdge_None;// ADC_ExternalTrigConv_T1_CC1; // ADC_ExternalTrigConvEdge_None
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC_DataAlign_Left;//ADC_DataAlign_Right;
		ADC_Init(ADC3, &ADC_InitStructure);	
		ADC_Cmd(ADC3, ENABLE);
		
//		ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_3Cycles);  // CRP HGB
//		ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 2, ADC_SampleTime_3Cycles);  // LED cur
//		ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 3, ADC_SampleTime_3Cycles);  // 56v cur
//		ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 4, ADC_SampleTime_3Cycles); // temperature
//		ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 5, ADC_SampleTime_3Cycles); // press
//		ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 6, ADC_SampleTime_3Cycles); // XK

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

//
UINT16 Get_Press_ADC(void)
{
	UINT16 nVal = 0, i;
	
#if ADC3_INIT_WITH_DMA
	nVal = g_ADC3_Value[0];
#else
	ADC_ClearFlag(ADC3,ADC_FLAG_EOC);
	ADC_Cmd(ADC3,ENABLE);
	for(i = 0; i < 5; i++)
	{
		ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 1, ADC_SampleTime_3Cycles );
		ADC_SoftwareStartConv(ADC3);		
		while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
		nVal += ADC_GetConversionValue(ADC3);	
	}
	nVal /= 5;
#endif
	return nVal;
}

//
UINT16 Get_XK_ADC(void)
{
	UINT16 nVal = 0, i;
	
#if ADC3_INIT_WITH_DMA
	nVal = g_ADC3_Value[1];
#else
	ADC_ClearFlag(ADC3,ADC_FLAG_EOC);
	ADC_Cmd(ADC3,ENABLE);
	for(i = 0; i < 5; i++)
	{
		ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 1, ADC_SampleTime_3Cycles ); 
		ADC_SoftwareStartConv(ADC3);		
		while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
		nVal += ADC_GetConversionValue(ADC3);	
	}
	nVal /= 5;
#endif
	return nVal;
}

//
UINT16 Get_56V_Cur_ADC(void)
{
	UINT16 nVal = 0, i;
	
#if ADC3_INIT_WITH_DMA
	//nVal = g_ADC3_Value[1];
#else
	ADC_ClearFlag(ADC3,ADC_FLAG_EOC);
	ADC_Cmd(ADC3,ENABLE);
	for(i = 0; i < 5; i++)
	{
		ADC_RegularChannelConfig(ADC3, ADC_Channel_7, 1, ADC_SampleTime_3Cycles ); 
		ADC_SoftwareStartConv(ADC3);		
		while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
		nVal += ADC_GetConversionValue(ADC3);	
	}
	nVal /= 5;
#endif
	return nVal;
}

//
UINT16 Get_LED_Cur_ADC(void)
{
	UINT16 nVal = 0, i;
	
#if ADC3_INIT_WITH_DMA
	//nVal = g_ADC3_Value[1];
#else
	ADC_ClearFlag(ADC3,ADC_FLAG_EOC);
	ADC_Cmd(ADC3,ENABLE);
	for(i = 0; i < 5; i++)
	{
		ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_3Cycles ); 
		ADC_SoftwareStartConv(ADC3);		
		while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
		nVal += ADC_GetConversionValue(ADC3);	
	}
	nVal /= 5;
#endif
	return nVal;
}

//
UINT32 HW_Get_ADC_HGB(void)
{
	UINT8 i;
	UINT32 nRet = 0;
	
#if USE_STM32F407_ONLY
	for(i = 0; i < 30; i++)
	{
		ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_480Cycles ); 
		ADC_SoftwareStartConv(ADC3);	
		while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
		nRet += ADC_GetConversionValue(ADC3);	
	}
	nRet /= 30;	
#else	
	nRet = HW_Get_ADC_Perip(0); // /* adc, 0=HGB,1=WBC vol value, 2=RBC(wbc backup,crp test), 3=press, */ 
	
#endif
	return nRet;
}

//
UINT32  HW_Get_ADC_CRP(void)
{
	UINT8 i;
	UINT32  nRet = 0;
	
#if USE_STM32F407_ONLY
	for(i = 0; i < 30; i++)
	{
		ADC_RegularChannelConfig(ADC3, ADC_Channel_0, 1, ADC_SampleTime_480Cycles ); 
		ADC_SoftwareStartConv(ADC3);		
		while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
		nRet = ADC_GetConversionValue(ADC3);	
	}
	nRet /= 30;
#else	
	nRet = HW_Get_ADC_Perip(2);  /* adc, 0=HGB,1=WBC vol value, 2=RBC(wbc backup,crp test), 3=press, */ 
#endif
	return nRet;
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

void Reset_Elec_Status(void)
{
	g_Elec_Status = 0;
}

void Set_Elec_Status(void)
{
	g_Elec_Status = 1;
}

UINT8 Get_Elec_Status(void)
{
	return g_Elec_Status;
}

//void ELEC_EXIT_FUNC(void)
//{
//	
//	IT_SYS_DlyMs(2);
//	if(ELEC_READ == 0) // low triggle
//	{
//		Set_Elec_Status();
//	}		 
//	EXTI_ClearITPendingBit(ELEC_EXIT_LINE);
//	printf("Elec Exit Func triggle, v=%d, status=%d\r\n", ELEC_READ, Get_Elec_Status());
//}

void Elec_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
  
    // 1. enable the input pin Clock 
    RCC_AHB1PeriphClockCmd(ELEC_SRC, ENABLE);
	
    // 2. configure the pin as input floating 
	GPIO_InitStructure.GPIO_Pin = ELEC_PIN;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
    GPIO_Init(ELEC_PORT, &GPIO_InitStructure); 

    // 3. extinal-interrupt model 
//    if (eModel == IN_MODEL_EXTI)     
//    {
        // 1). enable the SYSCFG-clock
        RCC_APB2PeriphClockCmd(ELEC_SRC, ENABLE);       
        // 2). Connects EXTI Line to Button GPIO Pin 
        SYSCFG_EXTILineConfig(ELEC_EXIT_SRC, ELEC_EXIT_PIN);    
        // 3). Configure EXTI line 
        EXTI_InitStructure.EXTI_Line = ELEC_EXIT_LINE;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;   
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  // all pins are rising detected         
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;               // enable interrupt line 		
        EXTI_Init(&EXTI_InitStructure);                           
        // 4). enable and set input EXTI Interrupt to the lowest priority 
        NVIC_InitStructure.NVIC_IRQChannel = ELEC_EXIT_NUM;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        // enable interrupt
        NVIC_Init(&NVIC_InitStructure);    	
//    }
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

void Beep(UINT16 nDelay)
{
	GPIO_SetBits(BEEP_PORT, BEEP_PIN);
	IT_SYS_DlyMs(nDelay);
	GPIO_ResetBits(BEEP_PORT, BEEP_PIN);
}

// pump
void Pump_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(PUMP_CLK_SRC|PUMP_DIR_SRC, ENABLE);
	// dir
	GPIO_InitStructure.GPIO_Pin = PUMP_DIR_PIN; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; //GPIO_PuPd_DOWN
	GPIO_Init(PUMP_DIR_PORT, &GPIO_InitStructure);
	GPIO_SetBits(PUMP_DIR_PORT, PUMP_DIR_PIN);
	
	Pump_PWM_Init(PUMP_PWM_TIM_ARR, PUMP_PWM_TIM_PSC); //84M/42=2M, 1M/25000=59.52
	//Pump_Speed_Set(24998);
	Pump_Speed_Set(PUMP_PWM_LEVEL_CLOSE);
	//TIM_SetCompare2(PUMP_PWM_TIM, PUMP_PWM_LEVEL_CLOSE);
}


void Pump_PWM_Init(UINT32 Arr, UINT32 Psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(PUMP_PWM_TIM_SRC,ENABLE);  	//TIM14时钟使能    
	RCC_AHB1PeriphClockCmd(PUMP_CLK_SRC, ENABLE); 	//使能PORTF时钟	
	GPIO_PinAFConfig(PUMP_CLK_PORT,PUMP_CLK_PIN_AF, PUMP_CLK_PORT_AF); 

	GPIO_InitStructure.GPIO_Pin = PUMP_CLK_PIN;           //GPIOF9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(PUMP_CLK_PORT, &GPIO_InitStructure);        
	GPIO_SetBits(PUMP_CLK_PORT, PUMP_CLK_PIN);
	
	TIM_DeInit(PUMP_PWM_TIM);
	TIM_TimeBaseStructure.TIM_Prescaler=Psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=Arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(PUMP_PWM_TIM,&TIM_TimeBaseStructure);//初始化定时器14

	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_Low; 
	//TIM_OCInitStructure.TIM_Pulse = ;
	TIM_OC2Init(PUMP_PWM_TIM, &TIM_OCInitStructure);  

	TIM_OC2PreloadConfig(PUMP_PWM_TIM, TIM_OCPreload_Enable); 
	TIM_ARRPreloadConfig(PUMP_PWM_TIM,ENABLE);
	TIM_Cmd(PUMP_PWM_TIM, ENABLE);  //
}
		

void Pump_AntiClockWise(void) // xi qi
{
	GPIO_SetBits(PUMP_DIR_PORT, PUMP_DIR_PIN);
}

void Pump_ClockWise(void) // zhu qi
{
	GPIO_ResetBits(PUMP_DIR_PORT, PUMP_DIR_PIN);
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
	if(nFreq > PUMP_PWM_TIM_ARR) return;
	///////if(nDir != e_Dir_Neg || nDir != e_Dir_Pos) return;
	
	if(nDir == e_Dir_Pos){
		Pump_AntiClockWise();
	}else if(nDir == e_Dir_Neg){
		Pump_ClockWise();
	}
	//
	//Pump_init();
	Pump_Speed_Set(nFreq);
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
	GPIO_SetBits(VALVE_AIR_PORT, VALVE_AIR_PIN);
	GPIO_SetBits(VALVE_LIQUID_PORT, VALVE_LIQUID_PIN);
}

void Valve_Air_Exec(UINT8 nOpt)
{
	if(nOpt == EN_OPEN)
	{
		GPIO_ResetBits(VALVE_AIR_PORT, VALVE_AIR_PIN);
	}else if(nOpt == EN_CLOSE){
		GPIO_SetBits(VALVE_AIR_PORT, VALVE_AIR_PIN);
	}
}

void Valve_Liquid_Exec(UINT8 nOpt)
{
	if(nOpt == EN_OPEN)
	{
		GPIO_ResetBits(VALVE_LIQUID_PORT, VALVE_LIQUID_PIN);
	}else if(nOpt == EN_CLOSE){
		GPIO_SetBits(VALVE_LIQUID_PORT, VALVE_LIQUID_PIN);
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

// OC for fix motor, OC for out_in motor
void OC_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(FIX_OC_CLK_SRC|OUT_OC_CLK_SRC|IN_OC_CLK_SRC, ENABLE);
  // fix oc
  GPIO_InitStructure.GPIO_Pin = FIX_OC_CLK_PIN; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(FIX_OC_CLK_PORT, &GPIO_InitStructure);
  // oc for out
  GPIO_InitStructure.GPIO_Pin = OUT_OC_CLK_PIN; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(OUT_OC_CLK_PORT, &GPIO_InitStructure);
  // oc for in
  GPIO_InitStructure.GPIO_Pin = IN_OC_CLK_PIN; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(IN_OC_CLK_PORT, &GPIO_InitStructure);
}

UINT8 Get_Fix_OC_Status(void)
{
	return GPIO_ReadInputDataBit(FIX_OC_CLK_PORT, FIX_OC_CLK_PIN);
}

UINT8 Get_Out_OC_Status(void)
{
	return GPIO_ReadInputDataBit(OUT_OC_CLK_PORT, OUT_OC_CLK_PIN);
}

UINT8 Get_In_OC_Status(void)
{
	return GPIO_ReadInputDataBit(IN_OC_CLK_PORT, IN_OC_CLK_PIN);
}

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(LED1_CLK_SRC|LED2_CLK_SRC|LED3_CLK_SRC|LED4_CLK_SRC|LED5_CLK_SRC \
		|LED6_CLK_SRC|LED7_CLK_SRC|LED8_CLK_SRC|LED_CUR_SWITCH_SRC, ENABLE);
	// LED1
	GPIO_InitStructure.GPIO_Pin   = LED1_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LED1_PORT, &GPIO_InitStructure);
	//LED2
	GPIO_InitStructure.GPIO_Pin   = LED2_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LED2_PORT, &GPIO_InitStructure);
	// LED3
	GPIO_InitStructure.GPIO_Pin   = LED3_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LED3_PORT, &GPIO_InitStructure);
	//LED4
	GPIO_InitStructure.GPIO_Pin   = LED4_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LED4_PORT, &GPIO_InitStructure);
	// LED5
	GPIO_InitStructure.GPIO_Pin   = LED5_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LED5_PORT, &GPIO_InitStructure);
	//LED6
	GPIO_InitStructure.GPIO_Pin   = LED6_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LED6_PORT, &GPIO_InitStructure);
	// LED7
	GPIO_InitStructure.GPIO_Pin   = LED7_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LED7_PORT, &GPIO_InitStructure);
	//LED8
	GPIO_InitStructure.GPIO_Pin   = LED8_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LED8_PORT, &GPIO_InitStructure);
	// Control Switch
	GPIO_InitStructure.GPIO_Pin   = LED_CUR_SWITCH_PIN; 
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LED_CUR_SWITCH_PORT, &GPIO_InitStructure);
	// LED Cur Adjust, DAC
	LEC_Cur_Adjust_DAC_Init();
	
}

void LEC_Cur_Adjust_DAC_Init(void)
{
	//

}

void LED_Cur_Switch(UINT8 nOpt)
{
	if(nOpt == EN_OPEN){
		GPIO_SetBits(LED_CUR_SWITCH_PORT, LED_CUR_SWITCH_PIN);
	}else if(nOpt == EN_CLOSE){
		GPIO_ResetBits(LED_CUR_SWITCH_PORT, LED_CUR_SWITCH_PIN);
	}	
}


void LED_Exec(UINT8 nIndex, UINT8 nOpt)
{
	switch(nIndex)
	{
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
		case EN_LED8:
		{
			if(nOpt == EN_OPEN){
				GPIO_SetBits(LED8_PORT, LED8_PIN);
			}else if(nOpt == EN_CLOSE){
				GPIO_ResetBits(LED8_PORT, LED8_PIN);
			}
		}
		break;	
		default:break;	
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

//void OutIn_Motor_AntiClockWise(void) // out
//{
//	GPIO_SetBits(OUTIN_MOTOR_DIR_PORT, OUTIN_MOTOR_DIR_PIN);
//}

//void OutIn_Motor_ClockWise(void) // in
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
//		OutIn_Motor_AntiClockWise(); // out
//	}else if(nDir == e_Dir_Neg){
//		OutIn_Motor_ClockWise(); //in
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
//			HW_Valve_On(INDEX_VALVE_PUMP);
//			HW_Valve_Off(INDEX_VALVE_WBC);			
//		}
//	}
//	// check i
//	if(EN_OPEN == Get_Out_OC_Status())
//	{
//		
//		// oc out
//		Fix_Motor_Enable();
//		OutIn_Motor_AntiClockWise(); // out
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
//		OutIn_Motor_ClockWise(); // in
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

// Digital Register
void DRegister_SPI_Init(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  SPI_InitTypeDef  SPI_InitStructure;
		
	  RCC_AHB1PeriphClockCmd(D_REGISTER_CLK_SRC|D_REGISTER_MOSI_SRC|D_REGISTER_CS_SRC, ENABLE);
	  RCC_APB1PeriphClockCmd(D_REGISTER_SPI_SRC, ENABLE); 
		// clk
	  GPIO_InitStructure.GPIO_Pin = D_REGISTER_CLK_PIN; 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	  GPIO_Init(D_REGISTER_CLK_PORT, &GPIO_InitStructure);
	  // mosi
	  GPIO_InitStructure.GPIO_Pin = D_REGISTER_MOSI_PIN; 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	  GPIO_Init(D_REGISTER_MOSI_PORT, &GPIO_InitStructure);
	  // cs
	  GPIO_InitStructure.GPIO_Pin = D_REGISTER_CS_PIN; 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	  GPIO_Init(D_REGISTER_CS_PORT, &GPIO_InitStructure);
	
	  //GPIO_PinAFConfig(D_REGISTER_CLK_PORT,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为 SPI1
	  //GPIO_PinAFConfig(D_REGISTER_MOSI_PORT,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为 SPI1
 
	  //这里只针对SPI口初始化
	  RCC_APB1PeriphResetCmd(D_REGISTER_SPI_SRC,ENABLE);//复位SPI1
	  RCC_APB1PeriphResetCmd(D_REGISTER_SPI_SRC,DISABLE);//停止复位SPI1

	  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	  SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	  SPI_Init(D_REGISTER_SPI, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
	 
	  SPI_Cmd(D_REGISTER_SPI, ENABLE); //使能SPI外设

	  //SPI1_ReadWriteByte(0xff);//启动传输		
}

void DResistor_Init(void)
{
	DRegister_SPI_Init();

}

void DRegister_SetSpeed(u8 SPI_BaudRatePrescaler)
{
//  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//判断有效性
	SPI1->CR1&=0XFFC7;//位3-5清零，用来设置波特率
	SPI1->CR1|=SPI_BaudRatePrescaler;	//设置SPI1速度 
	SPI_Cmd(SPI3,ENABLE); //使能SPI1
} 

void DRegister_Write(UINT16 nCmd)
{		 			 
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET){}//等待发送区空  
	SPI_I2S_SendData(SPI3, nCmd);	    
}

void DResistor_Set(UINT8 nIndex, UINT8 nVal)
{
	UINT16 nCmd =0, nCh = 0x01;
	
	nCmd = ((1 << 15) | ( nVal << 7));
	DRegister_Write(nCmd);

}


void Driver_Debug(UINT8 nIndex)
{
	UINT32 nCurTime, nTempTime;
	UINT16 i = 0, val = 0;
	switch(nIndex)
	{
		case 0: //beep
		{
			Beep(200);
		}
		break;
		case 1: // pump
		{
			printf("PUMP start\r\n");
			for(i = 0; i < 10; i++)
			{
				Pump_ClockWise();
				IT_SYS_DlyMs(100);
				Pump_AntiClockWise();
				IT_SYS_DlyMs(100);
			}
			Pump_ClockWise();
			val = 50;
			for(i = 0; i < 9; i++)
			{
				printf("PUMP val =%d\r\n", val);
				Pump_Speed_Set(280);
				IT_SYS_DlyMs(500);
				IT_SYS_DlyMs(500);
				IT_SYS_DlyMs(500);
				IT_SYS_DlyMs(500);
				val += 50;
			}
			printf("PUMP end\r\n");
		}
		break;
		case 2: // valve
		{
			printf("Valve start\r\n");
			Valve_Air_Exec(EN_OPEN);
			IT_SYS_DlyMs(500);
			IT_SYS_DlyMs(500);
			Valve_Air_Exec(EN_CLOSE);
			IT_SYS_DlyMs(500);
			IT_SYS_DlyMs(500);
			Valve_Liquid_Exec(EN_OPEN);
			IT_SYS_DlyMs(500);
			IT_SYS_DlyMs(500);
			Valve_Liquid_Exec(EN_CLOSE);
			IT_SYS_DlyMs(500);
			IT_SYS_DlyMs(500);
			printf("Valve start\r\n");
		
		}
		break;
		case 3: //oc
		{
			for(i = 0; i < 10; i++)
			{
				printf("FIX_OC=%d, OUI_OC=%d, IN_OC=%d\r\n", Get_Fix_OC_Status(),\
					Get_Out_OC_Status(), Get_In_OC_Status());
				IT_SYS_DlyMs(500);
				IT_SYS_DlyMs(500);
			}
		
		}
		break;
		case 4:  // fix motor
		{
			printf("Fix Motor start\r\n");
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
			
		}
		break;
		case 5: // out in motor
		{
			printf("OutIn start\r\n");
//			for(i = 0; i < 10; i++)
//			{
//				OutIn_Motor_Enable();
//				OutIn_Motor_ClockWise();
//				IT_SYS_DlyMs(200);
//				OutIn_Motor_Disable();
//				OutIn_Motor_AntiClockWise();
//				IT_SYS_DlyMs(200);
//			}
//			OutIn_Motor_Disable();
//			OutIn_Motor_ClockWise();
//			for(i = 0; i < 2000; i++)
//			{
//				OutIn_Motor_Run(300, 500);
//			}
//			printf("OutIn end\r\n");
		}
		break;
		case 6: // elec
		{
			printf("Elec Exit Func triggle, v=%d, status=%d\r\n", ELEC_READ, Get_Elec_Status());
		}
		break;
		case 7:
		{
//			printf("start\r\n");
//			OutIn_Motor_Enable();
//			for(i = 0; i < 5; i++)
//			{
//				OutIn_Motor_Exec(e_Dir_Pos,  OUTIN_MOTOR_PWM_LEVEL_BEST);
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//				OutIn_Motor_Exec(e_Dir_Neg,  OUTIN_MOTOR_PWM_LEVEL_BEST);
//				IT_SYS_DlyMs(500);
//				IT_SYS_DlyMs(500);
//			}
//			OutIn_Motor_Exec(e_Dir_Neg,  OUTIN_MOTOR_PWM_LEVEL_CLOSE);
////			printf("2\r\n");
//			IT_SYS_DlyMs(500);
//			IT_SYS_DlyMs(500);
////			IT_SYS_DlyMs(500);
//			printf("3\r\n");
//			for(i = 0; i < 6000; i++)
//			{
//				OutIn_Motor_Run(70, 70);
//			}
//			OutIn_Motor_Disable();
//			printf("end\r\n");

		}
		break;
		case 8:
		{
			//MT_X_Home(e_NormalCheck_Call); 
//			
//			nCurTime = IT_SYS_GetTicks();
//			nTempTime = nCurTime;
//			OutIn_Motor_Enable();
//			OutIn_Motor_Exec(e_Dir_Neg, OUTIN_MOTOR_PWM_LEVEL_BEST);
//			while(nCurTime <= nTempTime + 50)
//			{
//				IT_SYS_DlyMs(1);
//				nCurTime = IT_SYS_GetTicks();
//			}
//			printf("end: sTime=%d, eTime=%d\r\n", (int)nTempTime, (int)nCurTime);
//			OutIn_Motor_Exec(e_Dir_Pos, OUTIN_MOTOR_PWM_LEVEL_CLOSE);			
		}
		break;
		case 9:
		{
			
			//WBC_48V_Self_Check();
			//Valve1_Self_Check();
			//Valve2_Self_Check();
			printf(" start\r\n");
//			
//			Valve_Liquid_Exec(EN_OPEN);
//			IT_SYS_DlyMs(500);
//			Valve_Liquid_Exec(EN_CLOSE);
//			IT_SYS_DlyMs(500);
//			
			//-----pump
			Valve_Air_Exec(EN_OPEN);
			Valve_Liquid_Exec(EN_OPEN);
			IT_SYS_DlyMs(500);
			
			//Pump_AntiClockWise();
			TIM_SetCompare2(PUMP_PWM_TIM, PUMP_PWM_LEVEL_BEST);//Pump_Exec(e_Dir_Pos, PUMP_PWM_LEVEL_BEST);
			IT_SYS_DlyMs(500);
			IT_SYS_DlyMs(500);
			IT_SYS_DlyMs(500);
			Valve_Air_Exec(EN_CLOSE);
			Valve_Liquid_Exec(EN_CLOSE);
			TIM_SetCompare2(PUMP_PWM_TIM, PUMP_PWM_LEVEL_CLOSE);//Pump_Exec(e_Dir_Pos, PUMP_PWM_LEVEL_CLOSE);

			
			//----------- out in
//			OutIn_Motor_Enable();
//			OutIn_Motor_AntiClockWise();
//			//OutIn_Motor_ClockWise();
//			for(i = 0; i < 8000; i++){
//				OutIn_Motor_Run(OUTIN_MOTOR_PULSE_UP_TIME, OUTIN_MOTOR_PULSE_DOWN_TIME);
//			}
			//---------- press
			printf("press=%010d, xk=%d\r\n", Get_Press_ADC(), Get_XK_ADC());
			printf("press=%010d\r\n", (int)Get_Press_Value(5));
			
			//Pump_Exec(e_Dir_Pos, 14999);
			//Pump_Self_Check();
			printf(" end\r\n");
			//Get_Press_Value(5);
		}
		break;
		default:break;	
	}	
}






#endif

