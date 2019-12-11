// ScopeMain.c

#ifndef   __SCOPE_MAIN_C__
#define   __SCOPE_MAIN_C__

#include "KernelHeader.h"
//#include "Scope_It.h"

extern __IO uint32_t  EthStatus;


void test(void)
{
    ;
}



void start_run(void)
{

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
	

}



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
 //   PL_InitProtocol();
//    SPG_InitProtocol();
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
	
//	Iint_SysParam();
//#if !USE_STM32F407_ONLY
//    SYS_InitSystem();       /* parameters */  //yaolan_20190715
//    MT_ALL_Init();          /* the comtrol's interfece of motor */  //yaolan_20190715
//#endif
    start_run();

    /* test(); */
    printf("\r\n ... Readying ...Data=%s...Time=%s...SV=%s...\r\n", __DATE__, __TIME__, softver_edtion);
//	Clear_FPGA_FIFO_Data();
//	g_AirLight_Flag = e_Feedback_Fail;
//	if(e_Feedback_Success == AirLight_Self_Check(e_SelfCheck_Call))
//	{
//		g_AirLight_Flag = e_Feedback_Success;
//	}
    while (1)
    {
        HW_LWIP_MainLine();
    }
}
#endif













#endif
