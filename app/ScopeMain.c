// ScopeMain.c

#ifndef   __SCOPE_MAIN_C__
#define   __SCOPE_MAIN_C__

#include "KernelHeader.h"
#include "Scope_It.h"

extern __IO uint32_t  EthStatus;

void start_run(void)
{
#if !USE_STM32F407_ONLY
	  HW_FPGA_RST_H();
	  IT_SYS_DlyMs(100);
	  HW_FPGA_RST_L();
	  IT_SYS_DlyMs(20);
	  HW_FPGA_RST_L();
	  IT_SYS_DlyMs(20);
	  HW_FPGA_RST_L();
	  IT_SYS_DlyMs(20);
	  HW_FPGA_RST_L();
#endif 
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
#if !USE_STM32F407_ONLY
	printf("wbc ticks=%d\n", (int)IT_SYS_GetTicks());
	HW_ADJ_SetResistor(0, g_Record_Param.nRegister_WBC); // (0-50K)-->(0-255)
    //HW_ADJ_SetResistor(0, 128);
	HW_ADJ_SetResistor(1, 255);  /*  */
    HW_ADJ_SetResistor(2, 255);  /*  */
    HW_ADJ_SetResistor(3, 255);  /*  */
 #endif
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

//void APP_ADC_DMA_Config(EN_TypeADC eType)
//{
//	DMA_InitTypeDef DMA_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
//	if(eType == EN_ADC1){
//		DMA_DeInit(DMA2_Stream0);
//		while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
//	}else if(eType == EN_ADC2){
//		DMA_DeInit(DMA2_Stream3);
//		while(DMA_GetCmdStatus(DMA2_Stream3) != DISABLE){}
//	}else if(eType == EN_ADC3){
//		
//	}
//	// DMA SET
//	if(eType == EN_ADC1){
//		DMA_InitStructure.DMA_Channel 				= DMA_Channel_0;
//		DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&ADC1->DR;
//		DMA_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)g_ADC1_Buffer;
//	}else if(eType == EN_ADC2){
//		DMA_InitStructure.DMA_Channel 				= DMA_Channel_1;
//		DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&ADC2->DR;
//		DMA_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)g_ADC2_Buffer;
//	}else if(eType == EN_ADC3){
//		//
//	}
//	DMA_InitStructure.DMA_DIR					= DMA_DIR_PeripheralToMemory;
//	DMA_InitStructure.DMA_BufferSize			= ADC1_BUFFER_LEN;
//	DMA_InitStructure.DMA_PeripheralInc			= DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_HalfWord;
//	DMA_InitStructure.DMA_MemoryDataSize		= DMA_MemoryDataSize_HalfWord;
//	DMA_InitStructure.DMA_Mode					= DMA_Mode_Circular;
//	DMA_InitStructure.DMA_Priority				= DMA_Priority_High;
//	DMA_InitStructure.DMA_FIFOMode				= DMA_FIFOMode_Disable;
//	DMA_InitStructure.DMA_FIFOThreshold			= DMA_FIFOThreshold_Full;
//	DMA_InitStructure.DMA_MemoryBurst			= DMA_MemoryBurst_Single;
//	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
//	
//	//DMA_DoubleBufferModeConfig(DMA2_Stream0,(uint32_t)&g_ADC_Buffer_2,DMA_Memory_0);//DMA_Memory_0?????
//	//DMA_DoubleBufferModeCmd(DMA2_Stream0,ENABLE);
//	if(eType == EN_ADC1){
//		DMA_Init(DMA2_Stream0, &DMA_InitStructure); 
//		DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_HTIF0);
//		DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
//		DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
//		DMA_ITConfig(DMA2_Stream0,DMA_IT_HT,ENABLE);
//		NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream0_IRQn; 
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;   
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;                      
//		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//		NVIC_Init(&NVIC_InitStructure);
//		DMA_Cmd(DMA2_Stream0, ENABLE);
//		
//	}else if(eType == EN_ADC2){
//		DMA_Init(DMA2_Stream3, &DMA_InitStructure); 
//		DMA_ClearITPendingBit(DMA2_Stream3,DMA_IT_HTIF0);
//		DMA_ClearITPendingBit(DMA2_Stream3,DMA_IT_TCIF0);
//		DMA_ITConfig(DMA2_Stream3,DMA_IT_TC,ENABLE);
//		DMA_ITConfig(DMA2_Stream3,DMA_IT_HT,ENABLE);
//		NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream3_IRQn; 
//		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;   
//		NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;                      
//		NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//		NVIC_Init(&NVIC_InitStructure);
//		DMA_Cmd(DMA2_Stream3, ENABLE);
//	}else if(eType == EN_ADC3){
//		//...
//	}		
//	// NVIC
////	NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream0_IRQn; 
////	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01;   
////	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01;                      
////	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
////	NVIC_Init(&NVIC_InitStructure);
////	 //DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
////	 //DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
////	 //while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
////	DMA_Cmd(DMA2_Stream0, ENABLE);
//}


//// ADC1 and ADC2 use DMA, ADC3 Not Use
//void APP_ADC_Init(EN_TypeADC eType)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	ADC_CommonInitTypeDef ADC_CommonInitStructure;
//	ADC_InitTypeDef ADC_InitStructure;
//	
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOF, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3, ENABLE);
//	
//	if(eType == EN_ADC1)
//	{
//		// PA5
//		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5;//GPIO_Pin_5, PA5,  ADC1_IN5, WBC
//		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
//		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
//		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//		//PA6
//		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_6;//GPIO_Pin_6, PA6, ADC1_IN6, Maybe RBC
//		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
//		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
//		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//		//ADC1
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
//	}else if(eType == EN_ADC2){
//		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;//GPIO_Pin_0, PB0, PB0, ADC2_IN8
//		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
//		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
//		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOB, &GPIO_InitStructure);
//		//ADC2
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
//	}else if(eType == EN_ADC3){

//		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;//GPIO_Pin_5, PF6_ADC3_CH5, LED_CUR
//		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
//		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
//		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOF, &GPIO_InitStructure);
//		
//		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;//GPIO_Pin_3, PC3_ADC3_IN13, XK
//		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
//		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
//		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOC, &GPIO_InitStructure);
//		
//		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;//GPIO_Pin_9, PF9_ADC3_IN7, 56V_CUR
//		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
//		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
//		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOF, &GPIO_InitStructure);
//		
//		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;//GPIO_Pin_0, PC0_ADC123_IN10, Temperature
//		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
//		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
//		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOC, &GPIO_InitStructure);
//		
//		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;//GPIO_Pin_0, PA0_ADC3_IN0, CRP or HGB
//		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
//		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
//		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
//		GPIO_Init(GPIOA, &GPIO_InitStructure);
//		
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);
//	}
//	// Common Set
//	ADC_CommonInitStructure.ADC_Mode	= ADC_Mode_Independent;
//	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
//	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
//	if(eType == EN_ADC3){ // not use dma
//		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
//	}else{ // adc1 and adc2 use dma 
//		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
//	}
//	ADC_CommonInit(&ADC_CommonInitStructure);
//	
//	// ADC Set
//	ADC_InitStructure.ADC_Resolution	= ADC_Resolution_12b;
//	ADC_InitStructure.ADC_ScanConvMode  = DISABLE;
//	if(eType == EN_ADC3){ 
//		ADC_InitStructure.ADC_NbrOfConversion = 5;		// 5 adc IN
//		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
//	}else{
//		ADC_InitStructure.ADC_NbrOfConversion = 1;
//		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//	}
//	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
//	ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConv_T1_CC1;
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC_DataAlign_Left;//ADC_DataAlign_Right;
//	if(eType == EN_ADC1){ 
//		ADC_Init(ADC1, &ADC_InitStructure);	
//		ADC_Cmd(ADC1, ENABLE);
//		APP_ADC_DMA_Config(EN_ADC1);
//		ADC_DMACmd(ADC1, ENABLE);
//		ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles); //PA5_ADC1_IN5,WBC  
//		//ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_3Cycles); //PA6_ADC1_IN6,RBC   // RBC and WBC not Getting in same time
//	}else if(eType == EN_ADC2){ 
//		ADC_Init(ADC2, &ADC_InitStructure);	
//		ADC_Cmd(ADC2, ENABLE);
//		APP_ADC_DMA_Config(EN_ADC2);
//		ADC_DMACmd(ADC2, ENABLE);		
//		ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_3Cycles); //PB0_ADC2_IN8, RBC   // PLT, Maybe Getting data in same time with RBC	
//	}else if(eType == EN_ADC3){ 
//		ADC_Init(ADC1, &ADC_InitStructure);
//		ADC_Cmd(ADC1, ENABLE);
//		//ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 2, ADC_SampleTime_3Cycles); // ...
//	}

//	
//#if DOUBLE_ADC_CHANNEL
//	
//	
//#else
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_15Cycles); //PA5_ADC1_IN5,WBC  //ADC_SampleTime_3Cycles
//#endif

//	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
//	//ADC_SoftwareStartConv(ADC1);
//}


////FlagStatus DMA_GetFlagStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG)
////uint16_t DMA_GetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Streamx)
//void DMA2_Stream0_IRQHandler(void) 
//{
//	// half DMA_GetFlagStatus
//	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_HTIF0) == SET)  
//	{
//		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_HTIF0);
//		ADC1_Status.nSFlag = 1;
//		ADC1_Status.nID++;
//	}
//	
//	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET)  
//	{
//		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
//		ADC1_Status.nSFlag = 2;
//		ADC1_Status.nID++;
//	}
//}


//void DMA2_Stream3_IRQHandler(void) 
//{
//	// half DMA_GetFlagStatus
//	if (DMA_GetITStatus(DMA2_Stream3, DMA_IT_HTIF0) == SET)  
//	{
//		DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_HTIF0);
//		ADC2_Status.nSFlag = 1;
//		ADC2_Status.nID++;
//	}
//	
//	if (DMA_GetITStatus(DMA2_Stream3, DMA_IT_TCIF0) == SET)  
//	{
//		DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_TCIF0);
//		ADC2_Status.nSFlag = 2;
//		ADC2_Status.nID++;
//	}
//}


#if 0
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	Beep_Init();
	Micro_OC_Init();
	while(1);
}
#else
int main(void)
{
    PL_InitProtocol();
    SPG_InitProtocol();
    EVAL_Init();            /* platform */
	
	printf("Waiting Net Cable Link...");
	while(EthStatus != (ETH_INIT_FLAG | ETH_LINK_FLAG))
	{
		ETH_BSP_Config();
		IT_SYS_DlyMs(200);
	}
   // ETH_BSP_Config();       /* configure ethernet */
    LwIP_Init();            /* Initilaize the LwIP stack */
    udp_echoserver_init();  /* UDP echoserver */
	
	Iint_SysParam();
#if !USE_STM32F407_ONLY
    SYS_InitSystem();       /* parameters */  //yaolan_20190715
    MT_ALL_Init();          /* the comtrol's interfece of motor */  //yaolan_20190715
#endif
    start_run();

    /* test(); */
    printf("\r\n ... Readying ...Data=%s...sv=%s...\r\n", __DATE__, softver_edtion);
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













#endif
