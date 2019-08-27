// ScopeMain.c

#ifndef   __SCOPE_MAIN_C__
#define   __SCOPE_MAIN_C__

#include "KernelHeader.h"
#include "Scope_It.h"

void start_run(void)
{
	  HW_FPGA_RST_H();
	  IT_SYS_DlyMs(100);
	  HW_FPGA_RST_L();
	  IT_SYS_DlyMs(20);
	  HW_FPGA_RST_L();
	  IT_SYS_DlyMs(20);
	  HW_FPGA_RST_L();
	  IT_SYS_DlyMs(20);
	  HW_FPGA_RST_L();
	  
//	IT_SYS_DlyMs(1000);
//	IT_SYS_DlyMs(1000);
//	IT_SYS_DlyMs(1000);
//	IT_SYS_DlyMs(1000);
//	IT_SYS_DlyMs(1000);
	// read parameter form flash
	memset(&g_Record_Param, 0, RECORD_PARAM_LEN);
	if(e_Feedback_Fail == Flash_Read_Param(&g_Record_Param, sizeof(RECORD_PARAM)))
	{
		// maybe need to send a status msg to app
		printf("read record param fail, reset those\r\n");
		Set_Default_Param(&g_Record_Param);	
		//Flash_Write_Param(&g_Record_Param, RECORD_PARAM_LEN);
	}
	if(g_Record_Param.nFlag != FLASH_INIT_FLAG)
	{
		printf("record param flag error, reset those\r\n");
		Set_Default_Param(&g_Record_Param);	
		Flash_Write_Param(&g_Record_Param, RECORD_PARAM_LEN);
	}
//	Flash_Read_Param(&g_Record_Param, sizeof(RECORD_PARAM));
	
	printf("Param: wbc register=%d, addstep=%d, addpress=%010d\r\n",\
			g_Record_Param.nRegister_WBC, g_Record_Param.nXAddStep, (int)g_Record_Param.nAddPress);
	printf("CRP Param: Time=%d, HZ=%d, Total_Num=%d\r\n",\
			g_Record_Param.nTime, g_Record_Param.nHZ, (int)g_Record_Param.nTotal_Num);
	
	//Msg_Return_Handle_32(e_Msg_Data, CMD_DATA_WBC_VALUE, g_Record_Param.nWBC);
	//Msg_Return_Handle_32(e_Msg_Data, CMD_DATA_WBC_VALUE, g_Record_Param.nXAddStep);
	//Msg_Return_Handle_32(e_Msg_Data, CMD_DATA_WBC_VALUE, g_Record_Param.nAddPress);
	
	// wbc 
    //HW_ADJ_SetResistor(0, 255);  /*  */
	
	printf("wbc ticks=%d\n", (int)IT_SYS_GetTicks());
	HW_ADJ_SetResistor(0, g_Record_Param.nRegister_WBC); // (0-50K)-->(0-255)
    //HW_ADJ_SetResistor(0, 128);
	HW_ADJ_SetResistor(1, 255);  /*  */
    HW_ADJ_SetResistor(2, 255);  /*  */
    HW_ADJ_SetResistor(3, 255);  /*  */
    
    //----------------------------------------------
    // init, stuatus's parameters
    EVAL_OutputSet(O_MCU_LED_1);
    EVAL_OutputClr(O_MCU_LED_2);
    IT_SYS_DlyMs(200);
    EVAL_OutputClr(O_MCU_LED_1);
    EVAL_OutputSet(O_MCU_LED_2);
    IT_SYS_DlyMs(200);
    EVAL_OutputSet(O_MCU_LED_1);
    EVAL_OutputClr(O_MCU_LED_2);
    IT_SYS_DlyMs(200);
    EVAL_OutputClr(O_MCU_LED_1);
    EVAL_OutputSet(O_MCU_LED_2);
    IT_SYS_DlyMs(200);
    EVAL_OutputSet(O_MCU_LED_1);
    EVAL_OutputClr(O_MCU_LED_2);
    IT_SYS_DlyMs(200);
    EVAL_OutputClr(O_MCU_LED_1);
    EVAL_OutputSet(O_MCU_LED_2);
}

void test(void)
{
    ;
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
	DMA_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)g_ADC_Buffer;
	DMA_InitStructure.DMA_DIR					= DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize			= ADC_BUFFER_LEN;
	DMA_InitStructure.DMA_PeripheralInc			= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize		= DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode					= DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority				= DMA_Priority_High;
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
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_15Cycles); //ADC_SampleTime_3Cycles
#endif

	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	//ADC_SoftwareStartConv(ADC1);
}


//FlagStatus DMA_GetFlagStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG)
//uint16_t DMA_GetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Streamx)
void DMA2_Stream0_IRQHandler(void) 
{
	// half DMA_GetFlagStatus
	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_HTIF0) == SET)  
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_HTIF0);
		ADC_Status.nSFlag = 1;
		ADC_Status.nID++;
	}
	
	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET)  
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		ADC_Status.nSFlag = 2;
		ADC_Status.nID++;
	}
}



int main(void)
{
    PL_InitProtocol();
    SPG_InitProtocol();
    EVAL_Init();            /* platform */

    ETH_BSP_Config();       /* configure ethernet */
    LwIP_Init();            /* Initilaize the LwIP stack */
    udp_echoserver_init();  /* UDP echoserver */

	Iint_SysParam();
    //SYS_InitSystem();       /* parameters */  //yaolan_20190715
    //MT_ALL_Init();          /* the comtrol's interfece of motor */  //yaolan_20190715

    start_run();
	//ADC1_Init();
    /* test(); */
    printf("\r\n ...... Readying ......\r\n");
//	Clear_FPGA_FIFO_Data();
//	g_AirLight_Flag = e_Feedback_Fail;
//	if(e_Feedback_Success == AirLight_Self_Check(e_SelfCheck_Call))
//	{
		g_AirLight_Flag = e_Feedback_Success;
//	}
    while (1)
    {
        HW_LWIP_MainLine();
    }
}

#endif

