// Interrupt.c

#ifndef  __INTERRUPT_C__
#define  __INTERRUPT_C__


#include "KernelHeader.h"  // attention: function-header



//-----------------------------------------------------------------------------------------
// variable definition

// 
_STA_ IO_ UINT32 XRAM_ m_nSysTicks    = 0;  /* 系统时钟，毫秒单位 */
_STA_ IO_ UINT32 XRAM_ m_nSysTicksEnd = 0;  /* 延时等待用，与系统时钟比较。赋值期望等待到的系统时钟值 */
//
_STA_ IO_ UINT32 XRAM_ m_nTickList = 0;     /*  */
_STA_ IO_ UINT32 XRAM_ m_nTickAdc  = 0;     /* ADC采集时间，毫秒单位 */

//-----------------------------------------------------------------------------------------
// function definition: static

// system timer processing, 1mS delay 
void IRQ_SysTick_ms(void)
{
    IO_ UINT8  XRAM_  ch = 0;
	
    // system time delay counter
	m_nSysTicks++;
	//
	m_nTickList++;
	m_nTickAdc++;
    // precision timer, do nothing   
	
	return;
}

//
void IRQ_SysTimer(void)
{
    _STA_ IO_ UINT32 XRAM_ s_count = 0;
	
#if INTERRUPT_OUTPUT
	printf("timer2 interrupt\r\n");
#endif

    if(SET == TIM_GetITStatus(TIM2, TIM_IT_Update))
    {
        // clear the bit
        TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update); 
		// disable TIM2 counter
        TIM_Cmd(TIM2, DISABLE);

		// period: 10ms
		s_count++;
		
        //----------------------------------------------
        // 1. protocol timeout process
    	if( 0 == (s_count % 5))   // 100ms
        {
			// sampling port timeout
    	    SPG_IsrTime();
			// timer get CRP data per 100ms
			if(g_CRP_Data.eEnable == e_True)
			{								
				if((g_CRP_Data.nTotal/(DATA_FRAME_NUM_4BYTE))%2 == 0)
				{
					g_CRP_Data.crpBuffer[g_CRP_Data.nIndex] = HW_Get_ADC_CRP();
					printf("ADC=%04d,3.3V=%05d,total=%04d,index=%04d\r\n", \
						(int)g_CRP_Data.crpBuffer[g_CRP_Data.nIndex],(int)g_CRP_Data.crpBuffer[g_CRP_Data.nIndex]*ADC_V_REF_VALUE_3_3/ADC_RESOLUTION_12, g_CRP_Data.nTotal, g_CRP_Data.nIndex);
					if(g_CRP_Data.nIndex >= DATA_FRAME_NUM_4BYTE - 1)
					{
						g_CRP_Data.eSend  = e_True;
						//printf("send true\r\n");
						g_CRP_Data.nIndex = 0;
						g_CRP_Data.nTotal++;
					}
				}else if((g_CRP_Data.nTotal/(DATA_FRAME_NUM_4BYTE))%2 == 1){
					g_CRP_Data.crpBuffer[DATA_FRAME_NUM_4BYTE + g_CRP_Data.nIndex] = HW_Get_ADC_CRP();
					printf("ADC=%04d,3.3V=%05d,total=%04d,index=%04d\r\n", \
						(int)g_CRP_Data.crpBuffer[DATA_FRAME_NUM_4BYTE + g_CRP_Data.nIndex],(int)g_CRP_Data.crpBuffer[DATA_FRAME_NUM_4BYTE + g_CRP_Data.nIndex]*ADC_V_REF_VALUE_3_3/ADC_RESOLUTION_12, g_CRP_Data.nTotal, g_CRP_Data.nIndex);
					if(g_CRP_Data.nIndex >= DATA_FRAME_NUM_4BYTE - 1)
					{
						g_CRP_Data.eSend  = e_True;
						//printf("send true\r\n");
						g_CRP_Data.nIndex = 0;
						g_CRP_Data.nTotal++;
					}
				}
				if(g_CRP_Data.eSend  == e_False)
				{
					g_CRP_Data.nIndex++;
					g_CRP_Data.nTotal++;
				}
			}
        }	
        
        //----------------------------------------------
        // 2. display
        
        if( 0 == (s_count % 50) )   // 500ms
        {
            EVAL_OutputToggle(O_MCU_LED_1); 
        }
     
        //----------------------------------------------
        // 3. temperature holding
        // 1) setting heating-model
		if( 0 == (s_count % 100) ) // 1000.ms
		{
            // HW_TEMP_IsrMonitoring();  // 100 * 10ms
		}
		// 2) generate pwm for adjusting the temperature-holding
		// HW_TEMP_IsrHeating();  // 100 times per second
		
		//----------------------------------------------
        // 4. the commands and the feedback-messages
        // 1) sorting the commands
        
		// 2) dealing with the feedback-messages, the slave module 
#if (1 == 1)		
		//PL_SLaveFeedBack();     
		SPG_WaitingFrame();
#else
        // attention: allow all the chars return back to the main-caller immedately, it
        //            will disturb the caller to determine others' perfect frame.
        MSG_SampleBoardFeedback();   
#endif

        //----------------------------------------------
        //----------------------------------------------
		// enable TIM2 counter
        TIM_Cmd(TIM2, ENABLE);
    }
	
    return;
}

#if !USE_STM32F407_ONLY
void IRQ_MotorStatus(void)
{
    IO_ UINT32 IRAM_  nStatus = 0;  
    IO_ UINT32 IRAM_  nAddr   = 0; 
    IO_ UINT16 IRAM_  anBuffer[2]; 
	//
	IO_ UINT8  IRAM_  achBuf[40];
	IO_ UINT16 IRAM_  nLen = 0;
	IO_ UINT8  IRAM_  chCtrl = 0;
	
#if INTERRUPT_OUTPUT
	printf("fpga interrupt\r\n");
#endif

    // Movement finish, rising level
    if(RESET != EXTI_GetITStatus(IN_MT_STATUS_ET_LINE))
    {
        // Clear the  EXTI line 10 pending bit
        EXTI_ClearITPendingBit(IN_MT_STATUS_ET_LINE);

		//----------------------------------------------
		// attention: the Priority of the exti-interrupt must be set 
		//            higher than all the timers
		
        // handling the feedbacking of the movement
	    MV_IsrMoveHandling();

		//----------------------------------------------
		
	}
	// 
	return;
}
#endif

//--------------------------------------------------
// handling the exti-interrupts of the OC input ?
//


// COMx interrupt, command translation
void IRQ_ComCommand(void)
{
    //----------------------------------------------
    // to main board
    PL_COM_IsrHandling();
    //----------------------------------------------
    EVAL_OutputToggle(O_MCU_LED_1);
    
	return;
}


// COMx interrupt, sampling port
void IRQ_ComSampling(void)
{
    //----------------------------------------------
    // to auto-sampling board
    SPG_IsrHandling();
    //----------------------------------------------
    EVAL_OutputToggle(O_MCU_LED_2);
	
	return;
}



//-----------------------------------------------------------------------------------------
// function definition: extern

//
UINT8  IT_SYS_DlyMs(UINT32 nDlyMs)
{
    m_nSysTicksEnd = m_nSysTicks + nDlyMs;
	while(m_nSysTicks < m_nSysTicksEnd);

	return e_Feedback_Success;
}

//
UINT32 IT_SYS_GetTicks(void)
{
    return m_nSysTicks;
}


//
UINT8  IT_SYS_SetTicks(UINT32 nTicks)
{
	m_nSysTicks = nTicks;
	//
	return e_Feedback_Success;
}

#if 1
//
UINT8  IT_SYS_SetDlyMs(UINT32 nDlyMs)
{
    m_nSysTicksEnd = m_nSysTicks + nDlyMs;

	return e_Feedback_Success;
}

//
enum eFlag IT_SYS_CheckDlyFin(void)
{
    if( m_nSysTicks >= m_nSysTicksEnd )
    {
        return e_True;
	}
	else
	{
        return e_False;
	}
}

#endif

//
//
UINT32 IT_LIST_GetTicks(void)
{
	return m_nTickList;
}

UINT8  IT_LIST_SetTicks(UINT32 nTicks)
{
	m_nTickList = nTicks;
	//
	return e_Feedback_Success;
}

UINT32 IT_ADC_GetTicks(void)
{
	return m_nTickAdc;
}

UINT8  IT_ADC_SetTicks(UINT32 nTicks)
{
	m_nTickAdc = nTicks;
	//
	return e_Feedback_Success;
}


#endif


