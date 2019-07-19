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
	
	printf("Param: wbc=%d, addstep=%d, addpress=%010d\r\n",\
			g_Record_Param.nWBC, g_Record_Param.nXAddStep, (int)g_Record_Param.nAddPress);
	//Msg_Return_Handle_32(e_Msg_Data, CMD_DATA_WBC_VALUE, g_Record_Param.nWBC);
	//Msg_Return_Handle_32(e_Msg_Data, CMD_DATA_WBC_VALUE, g_Record_Param.nXAddStep);
	//Msg_Return_Handle_32(e_Msg_Data, CMD_DATA_WBC_VALUE, g_Record_Param.nAddPress);
	
	// wbc 
    //HW_ADJ_SetResistor(0, 255);  /*  */
	
	printf("wbc ticks=%d\n", (int)IT_SYS_GetTicks());
	HW_ADJ_SetResistor(0, g_Record_Param.nWBC); // (0-50K)-->(0-255)
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

