// InterfaceScope.c

//#define TEST_DEVICE_B4  /* 测试设备编号 */




#ifndef    __INTERFACE_SCOPE_C__
#define    __INTERFACE_SCOPE_C__

#include "KernelHeader.h"




UINT16 g_Debug_Data[256] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0xA,0x14,0x1B,0x23,0x2D,0x32,0x39,0x41,0x46,0x4B,
	0x50,0x50,0x50,0x50,0x50,0x4D,0x50,0x50,0x50,0x57,
	0x5F,0x66,0x73,0x7F,0x87,0x91,0x9B,0xA2,0xAC,0xBE,
	0xC8,0xD7,0xE6,0xF5,0x104,0x11D,0x133,0x14C,0x16F,0x195,
	0x1BD,0x1F1,0x22B,0x264,0x2AD,0x2FD,0x357,0x3C5,0x442,0x4C6,
	0x557,0x5EB,0x66F,0x6E5,0x744,0x77D,0x791,0x780,0x741,0x6D8,
	0x64F,0x5A2,0x4E2,0x417,0x348,0x27D,0x1BF,0x110,0x73,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00
};

//-----------------------------------------------------------------------------------------

//--------------------------------
// output computing, attention: 0-on, 1-off
_STA_ IO_ UINT16 XRAM_ m_nIoValves = 0xFFFF; // 01 ~ 16

// for network testing
_STA_ IO_ UINT16 XRAM_ s_anBufNet[1500];
_STA_ IO_ UINT16 XRAM_ s_nDataLen = 0;
_STA_ IO_ UINT16 XRAM_ s_nStatus  = 0;



#ifdef  TEST_DEVICE_B4    // 设备B4专用参数 
_STA_ UINT32 s_nK = 1543600;
_STA_ UINT32 s_nB = 1131880000;
#else                    // 通用参数 
_STA_ UINT32 s_nK = 173280;
_STA_ UINT32 s_nB = 97380000;
#endif

//-----------------------------------------------------------------------------------------

// init
UINT8 MT_ALL_Init(void)
{
    // axis x
#if 0
    MV_InitPara(EN_Motor1, 5000, AXIS_X_SPEED_TOP, 100, 10);
#endif
    MV_InitPara(EN_Motor1,
                4000,
                8000,
                50,
                1);
    // axis y
    MV_InitPara(EN_Motor2, 4000, 8000, 100, 10);

    MV_Stop(EN_Motor1);
    MV_Stop(EN_Motor2);

    return e_Feedback_Success;
}

// axis x
enum eAxisXPos MT_X_get_posi(void)
{
    if (E_AXIS_X_POS_UNSURE == g_tAxisPosStatus.eAxisX)
    {
        if (0 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
        {
            g_tAxisPosStatus.eAxisX = E_AXIS_X_POS_HOME;
        }
        else if (0 == HW_LEVEL_GetOC(OC_OUT_CHANNEL))
        {
            g_tAxisPosStatus.eAxisX = E_AXIS_X_POS_CTRL;
        }
    }
    return g_tAxisPosStatus.eAxisX;
}

enum eAxisYPos MT_Y_get_posi(void)
{
    if (E_AXIS_Y_POS_UNSURE == g_tAxisPosStatus.eAxisY)
    {
        if (0 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL))
        {
            g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_HOME;
        }
    }

    return g_tAxisPosStatus.eAxisY;
}


//UINT8 MT_X_Home(void)
//{
//    struct tMvMotorPara  tMvoingPara;

//#ifdef  MOTO_DOBULE_ENABLE
//    MT_Y_MoveToPosRel();
//#endif

//    moto_work_stat(0, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */

//    // record the motor's para
//    tMvoingPara.nFreqMin = g_atMotorPara[EN_Motor1].nFreqMin;
//    tMvoingPara.nFreqMax = g_atMotorPara[EN_Motor1].nFreqMax;
//    tMvoingPara.nFreqInc = g_atMotorPara[EN_Motor1].nFreqInc;
//    tMvoingPara.nFreqSam = g_atMotorPara[EN_Motor1].nFreqSam;

//    // not detect the single of home at the begining, moving long diatance
//    if (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
//    {
//			  /* ******************** original code *********************
//        // long distance
//			  MV_InitPara(EN_Motor1, 500, 12000, 2000, 100);    // ZZC_0904   : MV_InitPara(EN_Motor1, 4000, 8000, 100, 10);
//        // OC is on the right, right step
//        MV_Move(EN_Motor1, 35000, e_Dir_Neg); // comes near the OC

//        while (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL)) // OC detection is not enable
//        {
//            if (e_True == MV_IsFinished(EN_Motor1))
//            {
//                MV_Stop(EN_Motor1);
//                collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

//                moto_work_stat(0, MOTO_WORK_STAT_FAIL);  // 动作执行失败
//                return e_Feedback_Error;
//            }
//        }
//        IT_SYS_DlyMs(3);      //
//        MV_Stop(EN_Motor1); 
//				****************************************************** */
//		// long distance
//		MV_InitPara(EN_Motor1, 1920, 10000, 100, 10);    // ZZC_0904   : MV_InitPara(EN_Motor1, 4000, 8000, 100, 10);
//        // OC is on the right, right step
//        MV_Move(EN_Motor1, 30000, e_Dir_Neg); // comes near the OC

//        /*while (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL)) // OC detection is not enable
//        {
//            if (e_True == MV_IsFinished(EN_Motor1))
//            {
//                MV_Stop(EN_Motor1);
//                collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

//                moto_work_stat(0, MOTO_WORK_STAT_FAIL);  // 动作执行失败
//                return e_Feedback_Error;
//            }
//        }*/
//		while (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))  //&& (0 == MV_IsFinished(EN_Motor1))
//		{
//            if (e_True == MV_IsFinished(EN_Motor1))
//            {
//                MV_Stop(EN_Motor1);
//                break;
//            }
//        }
//		MV_InitPara(EN_Motor1, 1920, 1920, 100, 10);    // ZZC_0904   : MV_InitPara(EN_Motor1, 4000, 8000, 100, 10);
//        // OC is on the right, right step
//        MV_Move(EN_Motor1, 5000, e_Dir_Neg); // comes near the OC
//		while (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))  //&& (0 == MV_IsFinished(EN_Motor1))
//		{
//            if (e_True == MV_IsFinished(EN_Motor1))
//            {
//                MV_Stop(EN_Motor1);
//                collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

//                moto_work_stat(0, MOTO_WORK_STAT_FAIL);  // 动作执行失败
//                return e_Feedback_Error;
//            }
//        }
//        //IT_SYS_DlyMs(1);      //
//				//MV_InitPara(EN_Motor1, 1920, 1920, 100, 10);    // ZZC_0904   : MV_InitPara(EN_Motor1, 4000, 8000, 100, 10);
//        // OC is on the right, right step
//        //MV_Move(EN_Motor1, 500, e_Dir_Neg); // comes near the OC
//				//while (1)  //&& (0 == MV_IsFinished(EN_Motor1))
//				//{
//        //    if (e_True == MV_IsFinished(EN_Motor1))
//        //    {
//        //        MV_Stop(EN_Motor1);
//        //        break;
//        //    }
//        //}
//        MV_Stop(EN_Motor1); 
//    }
//    else
//    {
//#if  0    /* 样本仓到位，不再运行 */
//        //------------------------------------------------------------------
//        // short distance, home detect
//        MV_InitPara(EN_Motor1, 2000, 3000, 100, 1);
//        //------------------------------------------
//        // OC is on the right, left step
//        MV_Move(EN_Motor1, 2000, e_Dir_Pos); // goes far away the OC
//        while (0 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
//        {
//            if (e_True == MV_IsFinished(EN_Motor1))
//            {
//                MV_Stop(EN_Motor1);
//                return e_Feedback_Error;
//            }
//        }
//        IT_SYS_DlyMs(3);      //
//        MV_Stop(EN_Motor1);
//#endif
//    }
//    //
//    g_tAxisPosStatus.nAxisX = 0;
//    g_tAxisPosStatus.eAxisX    = E_AXIS_X_POS_HOME;
//    //---------------------------------------------------
//    // set to the default moving parameters
//    MV_InitPara(EN_Motor1,
//                tMvoingPara.nFreqMin,
//                tMvoingPara.nFreqMax,
//                tMvoingPara.nFreqInc,
//                tMvoingPara.nFreqSam);
//    //
//    moto_work_stat(0, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
//    return e_Feedback_Success;
//}

UINT16 AddStep_To_MS(UINT32 nStep)
{
	UINT16  nTime;
	nTime = (nStep*15)/100;
	return nTime;
}

#if 0//USE_STM32F407_ONLY
	UINT8 MT_X_Home(CALL_STYLE_E eCall) //UINT8 OutIn_Motor_Home(eModeType eMode)
	{
		UINT32 nCurTime = 0, nTempTime = 0, nDlyTime = 0;
		
		// start status msg
		if(eCall == e_NormalCheck_Call)
		{
			moto_work_stat_2(EN_MOTOR_OUTIN, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
		}
		// check outin motor in
		printf("oc out status =%d\r\n", HW_LEVEL_GetOC(OC_HOME_CHANNEL));
		if(EN_CLOSE == HW_LEVEL_GetOC(OC_HOME_CHANNEL)) // not at in position
		{
			printf("oc out status =%d\r\n", HW_LEVEL_GetOC(OC_HOME_CHANNEL));
			OutIn_Motor_Enable();
			//OutIn_Motor_In(); // in
			nCurTime = IT_SYS_GetTicks();
			nTempTime = nCurTime;
			#if OUTIN_MOTOR_USE_PWM
				OutIn_Motor_Run(e_Dir_Neg, OUTIN_MOTOR_PWM_LEVEL_BEST); // in
				while(nCurTime <= nTempTime + MOTO_SELF_CHECK_TIMEOUT)
				{
					if(EN_OPEN == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
					{
						IT_SYS_DlyMs(1);
						if(EN_OPEN == HW_LEVEL_GetOC(OC_HOME_CHANNEL))	break;
					}
					nCurTime = IT_SYS_GetTicks();					
				}
				//OutIn_Motor_Disable();
				OutIn_Motor_Run(e_Dir_Neg, OUTIN_MOTOR_PWM_LEVEL_CLOSE);
				printf("In OC triggle sTime=%d, eTime=%d, npress=%010d\r\n", (int)nTempTime, (int)nCurTime, (int)Get_Press_Value(GET_PRESS_NUM_FIVE));
				if(nCurTime >= nTempTime + MOTO_SELF_CHECK_TIMEOUT)
				{
					//printf("In time out,ST=%d, ET=%d\r\n", (int)nTempTime, (int)nCurTime);
					collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
					moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
					OutIn_Motor_Disable();
					return e_Feedback_Error;
				}
				// add step
				nDlyTime = AddStep_To_MS(g_Record_Param.nXAddStep);
				printf("addstep =%d, dlytime=%d\r\n", g_Record_Param.nXAddStep, (int)nDlyTime);
				OutIn_Motor_Enable();
				OutIn_Motor_Run(e_Dir_Neg, OUTIN_MOTOR_PWM_LEVEL_BEST);
				nCurTime = IT_SYS_GetTicks();
				nTempTime = nCurTime;
				while(nCurTime <= nTempTime + nDlyTime)
				{
					nCurTime = IT_SYS_GetTicks();
					IT_SYS_DlyMs(1);
				}
				//OutIn_Motor_Disable();
				OutIn_Motor_Run(e_Dir_Neg, OUTIN_MOTOR_PWM_LEVEL_CLOSE);
				printf("add step over sTime=%d, eTime=%d, npress=%010d\r\n", (int)nTempTime, (int)nCurTime, (int)Get_Press_Value(GET_PRESS_NUM_FIVE));
			#else
				for(i = 0; i < 35000; i++)
				{
					if(EN_OPEN == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
					{
						IT_SYS_DlyMs(2);
						if(EN_OPEN == HW_LEVEL_GetOC(OC_HOME_CHANNEL)) break;
					}
					OutIn_Motor_Run(OUTIN_MOTOR_PULSE_UP_TIME, OUTIN_MOTOR_PULSE_DOWN_TIME);
					nCurTime = IT_SYS_GetTicks();
					if(nCurTime >= nTempTime + OUTIN_MOTOR_HOME_TIME)
					{
						printf("In time out,ST=%d, ET=%d\r\n", (int)nTempTime, (int)nCurTime);
						collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
						moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
						OutIn_Motor_Disable();
						return e_Feedback_Error;
					}			
				}
				printf("step over i=%d, npress=%010d\r\n", (int)i, (int)Get_Press_Value(GET_PRESS_NUM_FIVE));
				// add step
				if(g_Record_Param.nXAddStep > 0)
				{
					for(i = 0; i < g_Record_Param.nXAddStep; i++)
					{
						OutIn_Motor_Run(OUTIN_MOTOR_PULSE_UP_TIME, OUTIN_MOTOR_PULSE_DOWN_TIME);
					}
				}
				printf("add step over i=%d, npress=%010d\r\n", (int)i, (int)Get_Press_Value(GET_PRESS_NUM_FIVE));
			#endif
		}else{
			//
		}
		OutIn_Motor_Disable();
		// end status msg
		if(eCall == e_NormalCheck_Call)
		{
			moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
		}
		
		return e_Feedback_Success;
	}
	
#else
	
	UINT8 MT_X_Home(CALL_STYLE_E eCall) // flag=0: normal check callback, =1: self check callback
	{
		struct tMvMotorPara  tMvoingPara;
		UINT32 nStep = 0;
	//	char buf[30] = {0}; UINT8 OC;

		if(EN_CLOSE == HW_LEVEL_GetOC(OC_HOME_CHANNEL)) 
		{
	#ifdef  MOTO_DOBULE_ENABLE
			//MT_Y_MoveToPosRel(eCall);  // for appv008
			moto_work_stat_2(1, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
			moto_work_stat_2(1, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
	#endif
		}else{ // not need move, but the status msg need feekback to app
			if(eCall == e_NormalCheck_Call)
			{
				moto_work_stat_2(1, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
				moto_work_stat_2(1, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
				return e_Feedback_Success;
			}
		}
		
		if(eCall == e_NormalCheck_Call)
		{
			//moto_work_stat(0, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */
			moto_work_stat_2(0, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);  /* 动作开始执行 */
		}
		// record the motor's para
		tMvoingPara.nFreqMin = g_atMotorPara[Motor_X].nFreqMin;
		tMvoingPara.nFreqMax = g_atMotorPara[Motor_X].nFreqMax;
		tMvoingPara.nFreqInc = g_atMotorPara[Motor_X].nFreqInc;
		tMvoingPara.nFreqSam = g_atMotorPara[Motor_X].nFreqSam;

		// not detect the single of home at the begining, moving long diatance
		if (EN_CLOSE == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
		{
			// long distance
			MV_InitPara(Motor_X, 1920, 10000, 100, 10);    // ZZC_0904   : MV_InitPara(EN_Motor1, 4000, 8000, 100, 10);
			// OC is on the right, right step
			MV_Move(Motor_X, 35000, e_Dir_Neg); // comes near the OC
			while (EN_CLOSE == HW_LEVEL_GetOC(OC_HOME_CHANNEL))  //&& (0 == MV_IsFinished(EN_Motor1))
			{			
				if (e_True == MV_IsFinished(Motor_X))
				{
					MV_Stop(Motor_X);
					break;
				}
			}
//			MV_InitPara(Motor_X, 1920, 1920, 100, 10);    // ZZC_0904   : MV_InitPara(EN_Motor1, 4000, 8000, 100, 10);
//			// OC is on the right, right step
//			MV_Move(Motor_X, 5000, e_Dir_Neg); // comes near the OC
//			while (EN_CLOSE == HW_LEVEL_GetOC(OC_HOME_CHANNEL))  //&& (0 == MV_IsFinished(EN_Motor1))
//			{
//				if (e_True == MV_IsFinished(Motor_X))
//				{
//					MV_Stop(Motor_X);
//					if(eCall == e_NormalCheck_Call)
//					{
//						collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
//						//moto_work_stat(0, MOTO_WORK_STAT_FAIL);  // 动作执行失败					
//						moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
//					}
//					return e_Feedback_Error;
//				}
//			}
			MV_Stop(Motor_X); 
			
			printf("stepexec=%d\r\n", (int)g_atMotorStatus[Motor_X].nStepsExecuted);
	#if 1 // add step 
	//		IT_SYS_DlyMs(1000);
	//		IT_SYS_DlyMs(1000);
			if(g_Record_Param.nXAddStep > 0)
			{
				MV_InitPara(Motor_X, 1920, 1920, 100, 10);
				MV_Move(Motor_X, (UINT32)g_Record_Param.nXAddStep, e_Dir_Neg); // comes near the OC 
				MTx_IoMinitor_Disnable(Motor_X);
				while(1)
				{
					MV_GetStepsExecuted(Motor_X, &nStep);
					if(g_Record_Param.nXAddStep - nStep <= 5)
					{
						MV_Stop(Motor_X);
						break;
					}
					//IT_SYS_DlyMs(2);
				}
			}
			printf("add stepexec=%d\r\n", (int)g_atMotorStatus[Motor_X].nStepsExecuted);
			printf("Moto X Add Step(%d) Finshed\r\n", (int)g_Record_Param.nXAddStep);
	#endif
			MV_Stop(Motor_X); 
			
			
		}
		else
		{
	#if  0    /* 样本仓到位，不再运行 */
			//------------------------------------------------------------------
			// short distance, home detect
			MV_InitPara(Motor_X, 2000, 3000, 100, 1);
			//------------------------------------------
			// OC is on the right, left step
			MV_Move(Motor_X, 2000, e_Dir_Pos); // goes far away the OC
			while (0 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
			{
				if (e_True == MV_IsFinished(Motor_X))
				{
					MV_Stop(Motor_X);
					return e_Feedback_Error;
				}
			}
			IT_SYS_DlyMs(3);      //
			MV_Stop(Motor_X);
	#endif
		}
		//
		g_tAxisPosStatus.nAxisX = 0;
		g_tAxisPosStatus.eAxisX    = E_AXIS_X_POS_HOME;
		//---------------------------------------------------
		// set to the default moving parameters
		MV_InitPara(Motor_X,
					tMvoingPara.nFreqMin,
					tMvoingPara.nFreqMax,
					tMvoingPara.nFreqInc,
					tMvoingPara.nFreqSam);
		//
		if(eCall == e_NormalCheck_Call)
		{
			//moto_work_stat(0, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
			moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
		}
		return e_Feedback_Success;
	}
#endif //USE_STM32F407_ONLY

	
//
UINT8 MT_X_Home_only(void)
{
	struct tMvMotorPara  tMvoingPara;

	//moto_work_stat(0, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */
	moto_work_stat_2(0, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);

	// record the motor's para
	tMvoingPara.nFreqMin = g_atMotorPara[EN_Motor1].nFreqMin;
	tMvoingPara.nFreqMax = g_atMotorPara[EN_Motor1].nFreqMax;
	tMvoingPara.nFreqInc = g_atMotorPara[EN_Motor1].nFreqInc;
	tMvoingPara.nFreqSam = g_atMotorPara[EN_Motor1].nFreqSam;

	
	// not detect the single of home at the begining, moving long diatance
	if (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL))
	{
		// long distance
		MV_InitPara(Motor_X, 4000, 8000, 100, 10);
		// OC is on the right, right step
		MV_Move(Motor_X, 35000, e_Dir_Neg); // comes near the OC

		while (1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL)) // OC detection is not enable
		{
			if (e_True == MV_IsFinished(Motor_X))
			{
				MV_Stop(Motor_X);
				collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

				//moto_work_stat(0, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
				moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
				return e_Feedback_Error;
			}
		}
		IT_SYS_DlyMs(3);      //
		MV_Stop(Motor_X);
	}
	else
	{
		;
	}
	//
	g_tAxisPosStatus.nAxisX = 0;
	g_tAxisPosStatus.eAxisX    = E_AXIS_X_POS_HOME;
	//---------------------------------------------------
	// set to the default moving parameters
	MV_InitPara(Motor_X,
				tMvoingPara.nFreqMin,
				tMvoingPara.nFreqMax,
				tMvoingPara.nFreqInc,
				tMvoingPara.nFreqSam);
	//
	//moto_work_stat(0, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
	moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
	return e_Feedback_Success;
}


	



#ifdef  MOTO_Y_VERSION_V4
/* 4.0版本结构 */
UINT8 MT_Y_Home(CALL_STYLE_E eCall)
{
	if(eCall == e_NormalCheck_Call && E_AXIS_Y_POS_UNSURE != g_tAxisPosStatus.eAxisY)
	{
		//moto_work_stat(1, MOTO_WORK_STAT_RUN);
		moto_work_stat_2(1, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
	}
    if (E_AXIS_Y_POS_UNSURE == g_tAxisPosStatus.eAxisY)
    {
        MT_Y_MoveToPosRel(eCall);
    }
    //
    if (E_AXIS_Y_POS_HOME == g_tAxisPosStatus.eAxisY)
    {
        MV_Move(Motor_Y, 2000, e_Dir_Pos);
        //
        g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_CTRL;
    }
    //

	if(eCall == e_NormalCheck_Call && E_AXIS_Y_POS_UNSURE != g_tAxisPosStatus.eAxisY)
	{
		//moto_work_stat(1, MOTO_WORK_STAT_OK);
		moto_work_stat_2(1, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
	}
    return e_Feedback_Success;
}

#else

UINT8 MT_Y_Home(void)
{
    struct tMvMotorPara  tMvoingPara;

    //moto_work_stat(1, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */
	moto_work_stat_2(1, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);

    // record the motor's para
    tMvoingPara.nFreqMin = g_atMotorPara[Motor_Y].nFreqMin;
    tMvoingPara.nFreqMax = g_atMotorPara[Motor_Y].nFreqMax;
    tMvoingPara.nFreqInc = g_atMotorPara[Motor_Y].nFreqInc;
    tMvoingPara.nFreqSam = g_atMotorPara[Motor_Y].nFreqSam;

    // not detect the single of home at the begining, moving long diatance
    if (1 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL))
    {
        // long distance
        MV_InitPara(Motor_Y, 4000, 8000, 100, 10);
        // OC is on the right, right step
        MV_Move(Motor_Y, 35000, e_Dir_Pos); // comes near the OC

        while (1 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL)) // OC detection is not enable
        {
            if (e_True == MV_IsFinished(Motor_Y))
            {
                MV_Stop(Motor_Y);
                collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

               //moto_work_stat(1, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
				moto_work_stat_2(1, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
                return e_Feedback_Error;
            }
        }
        IT_SYS_DlyMs(3);      //
        MV_Stop(Motor_Y);
    }
    else
    {
        ;
    }
    //
    g_tAxisPosStatus.nAxisY = 0;
    g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_HOME;
    //---------------------------------------------------
    // set to the default moving parameters
    MV_InitPara(Motor_Y,
                tMvoingPara.nFreqMin,
                tMvoingPara.nFreqMax,
                tMvoingPara.nFreqInc,
                tMvoingPara.nFreqSam);
    //
    //moto_work_stat(1, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
	moto_work_stat_2(1, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
    return e_Feedback_Success;
}
#endif

UINT8 MT_Y_MoveToPosRel_Self_Check(void)
{
	struct tMvMotorPara  tMvoingPara;

//    moto_work_stat(1, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */

    // record the motor's para
    tMvoingPara.nFreqMin = g_atMotorPara[Motor_Y].nFreqMin;
    tMvoingPara.nFreqMax = g_atMotorPara[Motor_Y].nFreqMax;
    tMvoingPara.nFreqInc = g_atMotorPara[Motor_Y].nFreqInc;
    tMvoingPara.nFreqSam = g_atMotorPara[Motor_Y].nFreqSam;

    // not detect the single of home at the begining, moving long diatance
    if (1 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL))
    {
        // long distance
        MV_InitPara(Motor_Y, 4000, 8000, 100, 10);
        // OC is on the right, right step
        MV_Move(Motor_Y, 35000, e_Dir_Neg); // comes far the OC

        while (1 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL)) // OC detection is not enable
        {
            if (e_True == MV_IsFinished(Motor_Y))
            {
                MV_Stop(Motor_Y);
 //               collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

 //               moto_work_stat(1, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
                return e_Feedback_Error;
            }
        }

        /* 增加临时运行时序 */
        MV_Move(Motor_Y, 600, e_Dir_Neg);  /* 继续运行 */
        while (e_False == MV_IsFinished(EN_Motor2));
        /* 增加临时运行时序 */

        IT_SYS_DlyMs(3);
        MV_Stop(Motor_Y);
    }
    else
    {
        /* 增加临时运行时序 */
        MV_InitPara(Motor_Y, 4000, 8000, 100, 10);
        MV_Move(Motor_Y, 20000, e_Dir_Pos);

        while (0 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL)) // OC detection is not enable
        {
            if (e_True == MV_IsFinished(Motor_Y))
            {
                MV_Stop(Motor_Y);

 //               collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
 //               moto_work_stat(1, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
                return e_Feedback_Error;
            }
        }

        MV_Move(Motor_Y, 600, e_Dir_Neg);  /* 继续运行 */
        while (e_False == MV_IsFinished(EN_Motor2));

        IT_SYS_DlyMs(3);
        MV_Stop(Motor_Y);
        /* 增加临时运行时序 */
    }

    //
    g_tAxisPosStatus.nAxisY = 0;
    g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_HOME;
    //---------------------------------------------------
    // set to the default moving parameters
    MV_InitPara(Motor_Y,
                tMvoingPara.nFreqMin,
                tMvoingPara.nFreqMax,
                tMvoingPara.nFreqInc,
                tMvoingPara.nFreqSam);
    //
//    moto_work_stat(1, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
    return e_Feedback_Success;

}

_EXT_ UINT8 MT_Y_Home_Self_Check(void)
{
  // moto_work_stat(1, MOTO_WORK_STAT_RUN);
    if (E_AXIS_Y_POS_UNSURE == g_tAxisPosStatus.eAxisY)
    {
        MT_Y_MoveToPosRel_Self_Check();
    }
    //
    if (E_AXIS_Y_POS_HOME == g_tAxisPosStatus.eAxisY)
    {
        MV_Move(EN_Motor2, 2000, e_Dir_Pos);
        //
        g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_CTRL;
    }
    //

  //  moto_work_stat(1, MOTO_WORK_STAT_OK);
    return e_Feedback_Success;
}




#if 0//USE_STM32F407_ONLY
	UINT8 MT_X_MoveToPosRel(CALL_STYLE_E eCall)
	{
		UINT32 nCurTime, nTempTime;
		INT32 nPress = 0;
		
		UINT32 StartTicks = 0, EndTicks = 0;
	//	UINT8 i = 0;
		IO_ UINT8 flag = 0;
		
		if(eCall == e_NormalCheck_Call)
		{
			//moto_work_stat(0, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */
			moto_work_stat_2(0, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
			nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
			printf("\r\n1 X out-check press at first: npress=%010d, addpress=%010d\r\n", (int)nPress, (int)g_Record_Param.nAddPress);
			//StartTicks = IT_SYS_GetTicks();
			if(nPress < PRESS_BUILD)
			{
				flag = 1; // low 
				Pump_Exec(e_Dir_Pos, PUMP_PWM_LEVEL_BEST);
				Valve_Air_Exec(EN_OPEN);
				Valve_Liquid_Exec(EN_CLOSE);
			}
		}
		//	
		printf("oc out status =%d\r\n", HW_LEVEL_GetOC(OC_OUT_CHANNEL));
		if(EN_CLOSE == HW_LEVEL_GetOC(OC_OUT_CHANNEL)) // not at out position
		{
			printf("oc out status =%d\r\n", HW_LEVEL_GetOC(OC_OUT_CHANNEL));
			OutIn_Motor_Enable();
			//OutIn_Motor_Out(); // out
			nCurTime = IT_SYS_GetTicks();
			nTempTime = nCurTime;
			#if OUTIN_MOTOR_USE_PWM
				OutIn_Motor_Run(e_Dir_Pos, OUTIN_MOTOR_PWM_LEVEL_BEST); // out
				while(nCurTime <= nTempTime + MOTO_SELF_CHECK_TIMEOUT)
				{
					if(EN_OPEN == HW_LEVEL_GetOC(OC_OUT_CHANNEL))
					{
						IT_SYS_DlyMs(1);
						if(EN_OPEN == HW_LEVEL_GetOC(OC_OUT_CHANNEL))	break;
					}
					nCurTime = IT_SYS_GetTicks();	
					// build press
					if(eCall == e_NormalCheck_Call && flag != 0) // check press
					{
						nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
						if(nPress >= PRESS_BUILD)
						{
							flag = 0;
							Pump_Exec(e_Dir_Pos, PUMP_PWM_LEVEL_CLOSE);
							Valve_Air_Exec(EN_CLOSE);
							Valve_Liquid_Exec(EN_CLOSE);		
							//printf("2 X out-check press at first: npress=%010d, addpress=%010d\r\n", (int)nPress, (int)g_Record_Param.nAddPress);
						}
					}
				}
				//OutIn_Motor_Disable();
				OutIn_Motor_Run(e_Dir_Pos, OUTIN_MOTOR_PWM_LEVEL_CLOSE);
				if(nCurTime > nTempTime + MOTO_SELF_CHECK_TIMEOUT) // cang not at out place after 10s
				{
					Pump_Exec(e_Dir_Pos, PUMP_PWM_LEVEL_CLOSE);
					Valve_Air_Exec(EN_CLOSE);
					Valve_Liquid_Exec(EN_CLOSE);
					
					collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
					moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
					OutIn_Motor_Disable();
					return e_Feedback_Error;
				}
			#else
				for(i = 0; i < 35000; i++)
				{
					if(EN_OPEN == HW_LEVEL_GetOC(OC_OUT_CHANNEL))
					{
						IT_SYS_DlyMs(2);
						if(EN_OPEN == HW_LEVEL_GetOC(OC_OUT_CHANNEL))
						{
							printf("oc out status =%d\r\n", HW_LEVEL_GetOC(OC_OUT_CHANNEL));
							break;
						}						
					}
					if(eCall == e_NormalCheck_Call && flag != 0) // check press
					{
						nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
						if(nPress >= PRESS_BUILD)
						{
							flag = 0;
							Pump_Exec(e_Dir_Pos, PUMP_PWM_LEVEL_CLOSE);
							Valve_Air_Exec(EN_CLOSE);
							Valve_Liquid_Exec(EN_CLOSE);		
							//printf("2 X out-check press at first: npress=%010d, addpress=%010d\r\n", (int)nPress, (int)g_Record_Param.nAddPress);
						}
					}
					OutIn_Motor_Run(OUTIN_MOTOR_PULSE_UP_TIME, OUTIN_MOTOR_PULSE_DOWN_TIME);
					nCurTime = IT_SYS_GetTicks();
					if(nCurTime >= nTempTime + OUTIN_MOTOR_HOME_TIME)
					{
						collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
						moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
						printf("Out-in motor out timeout\r\n");
						break;
					}			
				}
				printf("step over i=%d, npress=%010d\r\n", (int)i, (int)Get_Press_Value(GET_PRESS_NUM_FIVE));
			#endif
		}else{
			//
		}
		//OutIn_Motor_Disable();
		OutIn_Motor_Run(e_Dir_Pos, OUTIN_MOTOR_PWM_LEVEL_CLOSE);
		// check press again
		if(eCall == e_NormalCheck_Call)
		{
			nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
			printf("3 X out-check press at first: npress=%010d, addpress=%010d\r\n", (int)nPress, (int)g_Record_Param.nAddPress);
			//StartTicks = IT_SYS_GetTicks();
			if(nPress >= PRESS_BUILD)
			{
				Pump_Exec(e_Dir_Pos, PUMP_PWM_LEVEL_CLOSE);
				Valve_Air_Exec(EN_CLOSE);
				Valve_Liquid_Exec(EN_CLOSE);
			}else{ // press is not enough, create press 	
				Pump_Exec(e_Dir_Pos, PUMP_PWM_LEVEL_BEST);
				Valve_Air_Exec(EN_OPEN);
				Valve_Liquid_Exec(EN_CLOSE);
				StartTicks = IT_SYS_GetTicks();
				EndTicks = StartTicks;
				while((EndTicks - StartTicks) <= TIME_OVER_TS_BUILD_PRESS)
				{
					nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
					if(nPress >= PRESS_BUILD) 
					{
//						Pump_Exec(e_Dir_Pos, PUMP_PWM_LEVEL_CLOSE);
//						Valve_Air_Exec(EN_CLOSE);
//						Valve_Liquid_Exec(EN_CLOSE);
						break;
					}
					IT_SYS_DlyMs(5);
					EndTicks = IT_SYS_GetTicks();
				}
				// create press timeout chec
			}
		}
		Pump_Exec(e_Dir_Pos, PUMP_PWM_LEVEL_CLOSE);
		Valve_Air_Exec(EN_CLOSE);
		Valve_Liquid_Exec(EN_CLOSE);
		if(eCall == e_NormalCheck_Call)
		{
			if((EndTicks - StartTicks) > TIME_OVER_TS_BUILD_PRESS) // create press fail
			{
				//Msg_Return_Handle_16(e_Msg_Status, CMD_STATUS_BUILD_PRESS, e_Feedback_Fail);	
				moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_FAIL);
				printf("5 X out at last,build press error: t=%08d, npress=%010d, addpress=%010d\r\n", (int)(EndTicks - StartTicks), (int)nPress, (int)g_Record_Param.nAddPress);
			}else{
				moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
				printf("5 X out at last,build press success: t=%08d, npress=%010d, addpress=%010d\r\n", (int)(EndTicks - StartTicks), (int)nPress, (int)g_Record_Param.nAddPress);
			}
		}
		printf("6 X out-check press: npress=%010d, addpress=%010d\r\n", (int)nPress, (int)g_Record_Param.nAddPress);

		Pump_Exec(e_Dir_Pos, PUMP_PWM_LEVEL_CLOSE);
		OutIn_Motor_Disable();
//		#ifdef  MOTO_DOBULE_ENABLE
//			MT_Y_Home(eCall);
//		#endif	
		return e_Feedback_Success;
	}
	

#else 
	
	
	UINT8 MT_X_MoveToPosRel(CALL_STYLE_E eCall)
	{
		struct tMvMotorPara  tMvoingPara;
		INT32 nPress = 0;
		
		UINT32 StartTicks = 0, EndTicks = 0;
	//	UINT8 i = 0;
		IO_ UINT8 flag = 0;
		
		if(eCall == e_NormalCheck_Call)
		{
			//moto_work_stat(0, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */
			moto_work_stat_2(0, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
			nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
			printf("\r\n1 X out-check press at first: npress=%010d, addpress=%010d\r\n", (int)nPress, (int)g_Record_Param.nAddPress);
			//StartTicks = IT_SYS_GetTicks();
			if(nPress < PRESS_BUILD)
			{
				HW_PUMP_Pulse(PUMP_PRESS_FREQ, e_Dir_Pos);
				HW_Valve_On(INDEX_VALVE_PUMP);
				HW_Valve_Off(INDEX_VALVE_WBC);			
			}
		}
		// record the motor's para
		tMvoingPara.nFreqMin = g_atMotorPara[Motor_X].nFreqMin;
		tMvoingPara.nFreqMax = g_atMotorPara[Motor_X].nFreqMax;
		tMvoingPara.nFreqInc = g_atMotorPara[Motor_X].nFreqInc;
		tMvoingPara.nFreqSam = g_atMotorPara[Motor_X].nFreqSam;
		
		// not detect the single of home at the begining, moving long diatance
		if (EN_CLOSE == HW_LEVEL_GetOC(OC_OUT_CHANNEL))
		{
			// long distance
			MV_InitPara(Motor_X, 3000, 10000, 100, 10);   // ZZC_0904 : MV_InitPara(Motor_X, 4000, 8000, 100, 10); 
			// OC is on the right, right step
			MV_Move(Motor_X, 35000, e_Dir_Pos);  /* 靠近出仓光耦方向 */

			while(EN_CLOSE == HW_LEVEL_GetOC(OC_OUT_CHANNEL)) // OC detection is not enable
			{
				if(eCall == e_NormalCheck_Call)
				{
					//nPress = HW_ADC_SpiGetPress();
					nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
					if(nPress >= PRESS_BUILD)
					{
						if(flag == 0 || flag == 1)
						{
							HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);
							HW_Valve_Off(INDEX_VALVE_PUMP);
							HW_Valve_Off(INDEX_VALVE_WBC);
							flag = 2;
						}
					}else{
						if(flag == 0)
						{
							HW_PUMP_Pulse(PUMP_PRESS_FREQ, e_Dir_Pos);
							HW_Valve_On(INDEX_VALVE_PUMP);
							HW_Valve_Off(INDEX_VALVE_WBC);
							flag = 1;
						}
					}
				}
				//
				if (e_True == MV_IsFinished(Motor_X) &&  EN_CLOSE == HW_LEVEL_GetOC(OC_OUT_CHANNEL))
				{
					MV_Stop(Motor_X);
					if(eCall == e_NormalCheck_Call)
					{
						collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
						//moto_work_stat(0, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
						moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
					}
					break;
				}
			}
			printf("2 X out-build press over: npress=%010d, addpress=%010d\r\n", (int)nPress, (int)g_Record_Param.nAddPress);
			IT_SYS_DlyMs(3);      //
			MV_Stop(Motor_X);
		}
		else
		{
			;  /* 样本仓到位，不再运行 */
		}
		// 
		flag = 0;
		if(eCall == e_NormalCheck_Call)
		{
			HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);
			HW_Valve_Off(INDEX_VALVE_PUMP);
			HW_Valve_Off(INDEX_VALVE_WBC);	
			IT_SYS_DlyMs(500);
		}
		if(eCall == e_NormalCheck_Call)
		{
			StartTicks = IT_SYS_GetTicks();
			EndTicks = StartTicks;
			while((EndTicks - StartTicks) <= TIME_OVER_TS_BUILD_PRESS)
			{
				EndTicks = IT_SYS_GetTicks();
				nPress = Get_Press_Value(GET_PRESS_NUM_FIVE);
				if(nPress >= PRESS_BUILD) 
				{
					HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);
					HW_Valve_Off(INDEX_VALVE_PUMP);
					HW_Valve_Off(INDEX_VALVE_WBC);	
					break;
				}else{
					if(flag == 0)
					{
						HW_PUMP_Pulse(PUMP_PRESS_FREQ, e_Dir_Pos);
						HW_Valve_On(INDEX_VALVE_PUMP);
						HW_Valve_Off(INDEX_VALVE_WBC);
						flag = 1;
					}
				}
				IT_SYS_DlyMs(5);
			}
			printf("3 X out check press after build: npress=%010d, addpress=%010d\r\n", (int)nPress, (int)g_Record_Param.nAddPress);
			HW_PUMP_Pulse(PUMP_PRESS_OFF, e_Dir_Pos);
			HW_Valve_Off(INDEX_VALVE_PUMP);
			HW_Valve_Off(INDEX_VALVE_WBC);	
		}
		//
		g_tAxisPosStatus.nAxisX = 0;
		g_tAxisPosStatus.eAxisX    = E_AXIS_X_POS_CTRL;
		//---------------------------------------------------
		// set to the default moving parameters
		MV_InitPara(Motor_X,
					tMvoingPara.nFreqMin,
					tMvoingPara.nFreqMax,
					tMvoingPara.nFreqInc,
					tMvoingPara.nFreqSam);
		//EndTicks = IT_SYS_GetTicks();
		if(eCall == e_NormalCheck_Call)
		{
			if((EndTicks - StartTicks) > TIME_OVER_TS_BUILD_PRESS)
			{
				moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_FAIL);
				printf("4 X out at last,build press error: t=%08d, npress=%010d, addpress=%010d\r\n", (int)(EndTicks - StartTicks), (int)nPress, (int)g_Record_Param.nAddPress);
			}else{
				moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
				printf("4 X out at last,build press success: t=%08d, npress=%010d, addpress=%010d\r\n", (int)(EndTicks - StartTicks), (int)nPress, (int)g_Record_Param.nAddPress);
			}
		}
	#ifdef  MOTO_DOBULE_ENABLE
		//MT_Y_Home(eCall);
		moto_work_stat_2(1, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
		moto_work_stat_2(1, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
	#endif

		return e_Feedback_Success;
	}	
	
#endif // USE_STM32F407_ONLY

	



UINT8 MT_X_MoveToPosRel_only(void)
{
	struct tMvMotorPara  tMvoingPara;

   // moto_work_stat(0, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */
	moto_work_stat_2(0, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
	// record the motor's para
	tMvoingPara.nFreqMin = g_atMotorPara[EN_Motor1].nFreqMin;
	tMvoingPara.nFreqMax = g_atMotorPara[EN_Motor1].nFreqMax;
	tMvoingPara.nFreqInc = g_atMotorPara[EN_Motor1].nFreqInc;
	tMvoingPara.nFreqSam = g_atMotorPara[EN_Motor1].nFreqSam;

	// not detect the single of home at the begining, moving long diatance
	if (1 == HW_LEVEL_GetOC(OC_OUT_CHANNEL))
	{
		// long distance
		MV_InitPara(Motor_X, 4000, 8000, 100, 10);
		// OC is on the right, right step
		MV_Move(Motor_X, 35000, e_Dir_Pos);  /* 靠近出仓光耦方向 */

		while (1 == HW_LEVEL_GetOC(OC_OUT_CHANNEL)) // OC detection is not enable
		{
			if (e_True == MV_IsFinished(Motor_X))
			{
				MV_Stop(Motor_X);
				collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);

				//moto_work_stat(0, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
				moto_work_stat_2(0, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
				return e_Feedback_Error;
			}
		}
		IT_SYS_DlyMs(3);      //
		MV_Stop(Motor_X);
	}
	else
	{
		;  /* 样本仓到位，不再运行 */
	}
	//
	g_tAxisPosStatus.nAxisX = 0;
	g_tAxisPosStatus.eAxisX    = E_AXIS_X_POS_CTRL;
	//---------------------------------------------------
	// set to the default moving parameters
	MV_InitPara(EN_Motor1,
				tMvoingPara.nFreqMin,
				tMvoingPara.nFreqMax,
				tMvoingPara.nFreqInc,
				tMvoingPara.nFreqSam);
	//
	//moto_work_stat(0, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
	moto_work_stat_2(0, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);

	return e_Feedback_Success;
}





#ifdef  MOTO_Y_VERSION_V4
/* 4.0版本结构 */
UINT8 MT_Y_MoveToPosRel(CALL_STYLE_E eCall)
{
    struct tMvMotorPara  tMvoingPara;

	if(eCall == e_NormalCheck_Call)
	{
		//moto_work_stat(1, MOTO_WORK_STAT_RUN);  /* 动作开始执行 */
		moto_work_stat_2(1, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
	}
    // record the motor's para
    tMvoingPara.nFreqMin = g_atMotorPara[Motor_Y].nFreqMin;
    tMvoingPara.nFreqMax = g_atMotorPara[Motor_Y].nFreqMax;
    tMvoingPara.nFreqInc = g_atMotorPara[Motor_Y].nFreqInc;
    tMvoingPara.nFreqSam = g_atMotorPara[Motor_Y].nFreqSam;

    // not detect the single of home at the begining, moving long diatance
//	if(1 == HW_LEVEL_GetOC(OC_HOME_CHANNEL)) 
//	{
		if (1 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL))
		{
			// long distance
			MV_InitPara(Motor_Y, 4000, 8000, 100, 10);
			// OC is on the right, right step
			MV_Move(Motor_Y, 35000, e_Dir_Neg); // comes far the OC

			while (1 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL)) // OC detection is not enable
			{
				if (e_True == MV_IsFinished(Motor_Y))
				{
					MV_Stop(Motor_Y);
					if(eCall == e_NormalCheck_Call)
					{
						collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);	
						//moto_work_stat(1, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
						moto_work_stat_2(1, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
					}
					return e_Feedback_Error;
				}
			}

			/* 增加临时运行时序 */
			MV_Move(Motor_Y, 600, e_Dir_Neg);  /* 继续运行 */
			while (e_False == MV_IsFinished(Motor_Y));
			/* 增加临时运行时序 */

			IT_SYS_DlyMs(3);
			MV_Stop(Motor_Y);
		}
		else
		{
			/* 增加临时运行时序 */
			MV_InitPara(Motor_Y, 4000, 8000, 100, 10);
			MV_Move(Motor_Y, 20000, e_Dir_Pos);

			while (0 == HW_LEVEL_GetOC(OC_SAMPLE_RELEA_CHANNEL)) // OC detection is not enable
			{
				if (e_True == MV_IsFinished(Motor_Y))
				{
					MV_Stop(Motor_Y);
					if(eCall == e_NormalCheck_Call)
					{
						collect_return_hdl(COLLECT_RET_FAIL_SAMPLE);
						//moto_work_stat(1, MOTO_WORK_STAT_FAIL);  /* 动作执行失败 */
						moto_work_stat_2(1, MOTO_WORK_STAT_FAIL, e_BUILD_PRESS_SUCCESS);
					}
					return e_Feedback_Error;
				}
			}

			MV_Move(Motor_Y, 600, e_Dir_Neg);  /* 继续运行 */
			while (e_False == MV_IsFinished(Motor_Y));

			IT_SYS_DlyMs(3);
			MV_Stop(Motor_Y);
			/* 增加临时运行时序 */
		}
//	}

    //
    g_tAxisPosStatus.nAxisY = 0;
    g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_HOME;
    //---------------------------------------------------
    // set to the default moving parameters
    MV_InitPara(Motor_Y,
                tMvoingPara.nFreqMin,
                tMvoingPara.nFreqMax,
                tMvoingPara.nFreqInc,
                tMvoingPara.nFreqSam);
    //
	if(eCall == e_NormalCheck_Call)
	{
		//moto_work_stat(1, MOTO_WORK_STAT_OK);  /* 动作执行成功 */
		moto_work_stat_2(1, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
	}
    return e_Feedback_Success;
}

#else

UINT8 MT_Y_MoveToPosRel(void)
{
    // MV_InitPara(EN_Motor2, 4000, 8000, 100, 10);
    //

    //moto_work_stat(1, MOTO_WORK_STAT_RUN);
	moto_work_stat_2(1, MOTO_WORK_STAT_RUN, e_BUILD_PRESS_SUCCESS);
    if (E_AXIS_Y_POS_UNSURE == g_tAxisPosStatus.eAxisY)
    {
        MT_Y_Home();
    }
    //
    if (E_AXIS_Y_POS_HOME == g_tAxisPosStatus.eAxisY)
    {
        MV_Move(EN_Motor2, 2000, e_Dir_Neg);
        //
        g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_CTRL;
    }
    //

    //moto_work_stat(1, MOTO_WORK_STAT_OK);
	moto_work_stat_2(1, MOTO_WORK_STAT_OK, e_BUILD_PRESS_SUCCESS);
    return e_Feedback_Success;
}
#endif

//------------------------------
// FPGA INIT

// RESET
//

void  HW_FPGA_RST_H(void)
{
	  IO_ UINT32 IRAM_  nAddr     = 0;
      IO_ UINT16 IRAM_  anBuffer[2];
	
	  nAddr = (UINT32)FPGA_WR_MOTOR_PARA_OFFSET;
	  // value
	  anBuffer[0] = 0xFFFF;
	  // write to the fpga
	  FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
}

void  HW_FPGA_RST_L(void)
{
	  IO_ UINT32 IRAM_  nAddr     = 0;
      IO_ UINT16 IRAM_  anBuffer[2];
	
	  nAddr = (UINT32)FPGA_WR_MOTOR_PARA_OFFSET;
	  // value
	  anBuffer[0] = 0xFFFE;
	  // write to the fpga
	  FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
}
//------------------------------
// I/Os control

UINT8  HW_Valve_On(UINT8 chIndex)
{
	// if cx2000_c,cx3000
	return HW_Valve_On_V3(chIndex);
	// else cx2000_b
	//
}

UINT8  HW_Valve_On_V3(UINT8 chIndex)
{
	if(chIndex == INDEX_VALVE_PUMP) 
	{
		EVAL_OutputSet(O_Air_Walve);
	}else if(chIndex == INDEX_VALVE_WBC){ // liquild walve
		EVAL_OutputSet(O_Liquild_Walve);
	}
	return 0;
}




UINT8  HW_Valve_Off(UINT8 chIndex)
{
	// if cx2000_c,cx3000
	return HW_Valve_Off_V3(chIndex);
	// else cx2000_b
	//
}

//
UINT8 HW_Valve_Off_V3(UINT8 chIndex)
{
	if(chIndex == INDEX_VALVE_PUMP) 
	{
		EVAL_OutputClr(O_Air_Walve);
	}else if(chIndex == INDEX_VALVE_WBC){ // liquild walve
		EVAL_OutputClr(O_Liquild_Walve);
	}
	return 0;
}		



//------------------------------
// DC motor control
UINT8  HW_PUMP_Pulse(UINT32 nFreq, enum eDirection eDir)
{
	// if cx2000_c,cx3000
	return HW_PUMP_Pulse_V3(nFreq, eDir);
	// else cx2000_b
}

//
UINT8  HW_PUMP_Pulse_V3(UINT32 nFreq, enum eDirection eDir)
{
	if(nFreq >= PUMP_RRESS_MAX_FREQ){
		TIM_SetCompare1(PUMP_PWM_TIM, PUMP_RRESS_MAX_FREQ);
	}else{
		TIM_SetCompare1(PUMP_PWM_TIM, nFreq);
	}
	return 0;
}



	
//
UINT32 HW_PUMP_GetFeedbackPulse(void)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    //
    IO_ UINT32 XRAM_  nValue    = 0;

    //----- feedback pulse -----
    // address
    nAddr = (UINT32)FPGA_RD_FREQ_PUMP;
    // read from the fpga
    FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 2); // 2 half-word
    nValue = (UINT32)(((UINT32)anBuffer[1]) << 16) + anBuffer[0];

    return nValue;
}



_EXT_ UINT8  HW_ADJ_SetResistor_V3(UINT8 chIndex, UINT8 chValue)
{
	UINT16 nCmd =0;
	UINT8 i;
	
	DREGISTER1_CLK_1();
	Delay_US(2);
	DREGISTER1_CS_1();
	Delay_US(5);
	DREGISTER1_CS_0();
	Delay_US(10);
	nCmd = (UINT16)(( chIndex & 0x03) << 8) | chValue;
	printf("CMD =%X\r\n", nCmd);
	for(i = 0; i < DREGISTER_DATA_LEN; i++)
	{
		DREGISTER1_CLK_0();
		if(0x0200 == (nCmd & 0x0200))
		{
			DREGISTER1_MOSI_1();
		}else{
			DREGISTER1_MOSI_0();	
		}
		Delay_US(10);
		DREGISTER1_CLK_1();
		Delay_US(10);
		nCmd <<= 1;
	}
	DREGISTER1_CS_1();
	Delay_US(2);
	DREGISTER1_CLK_1();
	
	return chValue;
}



UINT8  HW_ADJ_SetResistor(UINT8 chIndex, UINT8 chValue)
{
	// cx2000 b
	//return HW_ADJ_SetResistor_V2(chIndex, chValue);
	// cx2000_c,cx3000
	return HW_ADJ_SetResistor_V3(chIndex, chValue);

}
 

//------------------------------
// the digtal adjustable resistor
UINT8  HW_ADJ_SetResistor_V2(UINT8 chIndex, UINT8 chValue)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    //
    // IO_ UINT32 XRAM_  nValue    = 0;

    // address
    switch (chIndex)
    {
        case 0:
        {
            nAddr = (UINT32)FPGA_WR_ADJ_RES_01;
            break;
        }
        case 1:
        {
            nAddr = (UINT32)FPGA_WR_ADJ_RES_02;
            break;
        }
        case 2:
        {
            nAddr = (UINT32)FPGA_WR_ADJ_RES_03;
            break;
        }
        case 3:
        {
            nAddr = (UINT32)FPGA_WR_ADJ_RES_04;
            break;
        }
        default:
        {
            // do nothing
            break;
        }
    }
    //
    anBuffer[0] = chValue & 0xFFFF;
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}

//------------------------------
// the SPI ADC control
UINT8  HW_ADC_SpiCtrl(enum eFlag bAble)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    // IO_ UINT32 XRAM_  nValue    = 0;

    // address
    nAddr = (UINT32)FPGA_WR_SPI_ADC_CTRL;
    // value
    if (e_False == bAble)
    {
        anBuffer[0] = (UINT16)(FPGA_WR_V_SPI_ADC_OFF & 0xFFFF);
    }
    else
    {
        anBuffer[0] = (UINT16)(FPGA_WR_V_SPI_ADC_ON & 0xFFFF);
    }
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}

//
UINT16 HW_ADC_SpiGetADC(UINT8 chIndex)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];

    switch (chIndex)
    {
        case 0:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_01;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        case 1:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_02;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        case 2:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_03;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        case 3:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_04;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        default:
        {
            //
            anBuffer[0] = 0;
            //
            break;
        }
    }
    //
    return (anBuffer[0] & 0x0FFF);  // the lower 12-bit
}

/*
INT32 HW_ADC_SpiGetPress(void)
{
    //
    UINT16 nAd    = 0;
    INT32 nValue = 0;
    double fValue = 0;

    nAd = HW_ADC_SpiGetADC(INDEX_PRESS);  // // adc, 0=HGB,1=WBC vol value, 2=RBC(wbc backup), 3=press,
    //

    fValue = nAd * ((double)s_nK);
    if (fValue <= (double)s_nB)
    {
		//return g_Record_Param.nAddPress;
        return 0;
    }

    fValue -= (double)s_nB;
	//nValue = (UINT32)fValue
    nValue = (INT32)fValue + g_Record_Param.nAddPress; 
	//printf("Get press: addP=%d,oriP=%d,P=%d",\
				(int)g_Record_Param.nAddPress, (int)fValue, (int)nValue);
    return nValue;
}
*/

// yaolan_
UINT8  HW_LED_On(UINT8 Index)
{
	if(Index == LED_CRP_INDEX)
	{
	
	}else if(Index == LED_HGB_INDEX){
		
		
	}
	return 0;	
}

// yaolan_
UINT8  HW_LED_Off(UINT8 Index)
{
	if(Index == LED_HGB_INDEX)
	{
	
	}else if(Index == LED_CRP_INDEX){
		
		
	}
	return 0;
}

// yaolan_   the context is same with HW_ADC_SpiGetADC
UINT16 HW_Get_ADC_Perip(UINT8 Index)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];

    switch (Index)
    {
        case 0:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_01;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        case 1:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_02;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        case 2:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_03;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        case 3:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_04;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        default:
        {
            //
            anBuffer[0] = 0;
            //
            break;
        }
    }
    //
    return (anBuffer[0] & 0x0FFF);  // the lower 12-bit	return 0;
}

// yaolan_   the context is same with HW_ADC_SpiGetADC
UINT16 HW_Get_ADC_V_Cur(UINT8 Index)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];

    switch (Index)
    {
        case 0:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_01;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        case 1:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_02;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        case 2:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_03;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        case 3:
        {
            //
            nAddr = (UINT32)FPGA_RD_SPI_ADC_04;
            //
            FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
            //
            break;
        }
        default:
        {
            //
            anBuffer[0] = 0;
            //
            break;
        }
    }
    //
    return (anBuffer[0] & 0x0FFF);  // the lower 12-bit	return 0;
}

UINT16 Get_V_HGB_LED(void)
{
	UINT16 nVal, nRet;
	
	nVal = HW_Get_ADC_Perip(ADC_V_LED_HGB_INDEX);
	nRet = nVal*ADC_V_REF_VALUE_5/ADC_RESOLUTION_12;
	printf("HGB: ADC=%d, V=%d\r\n", (int)nVal, (int)nRet);
	return nRet;
}

UINT32 Get_V_CRP_LED(void)
{
	UINT16 nVal, nRet;
	
	nVal = HW_Get_ADC_Perip(ADC_V_LED_CRP_INDEX);
	nRet = nVal*ADC_V_REF_VALUE_5/ADC_RESOLUTION_24;
//	printf("CRP: ADC=%d, V=%d\r\n", (int)nVal, (int)nRet);
	return nRet;
}


UINT32 Get_WBC_V_Value(void)
{
	// if cx2000_c,cx3000
	return Get_WBC_V_Value_V3();
	//else cx2000_b
	//
}




UINT8 Get_WBC_V_Status(UINT32 nV)
{
	UINT8 i, num = 0;
	UINT16 nWBC;
	for(i = 0; i < 5; i++)
	{
		nWBC = Get_WBC_V_Value();
		if(nWBC <= nV) num++;
//		printf("wbc_v = %d\r\n", nWBC);
//		IT_SYS_DlyMs(1);
	}
	if(num >= 3) return EN_WBC_V_LOW;
	else return EN_WBC_V_HIGH;
}

/*
UINT8 Get_DRegister_Value(UINT8 nChannel)
{
	UINT8 nVal = 0;
	switch(nChannel)
	{
		case EN_DREGISTER_WBC:
		{
			nVal = 0;
		}
		break;
		case EN_DREGISTER_PLT_RBC:
		{
			nVal = 0;
		}
		break;
		case EN_DREGISTER_TOUSHE:
		{
			nVal = 0;
		}
		break;
		case EN_DREGISTER_SANSHE:
		{
			nVal = 0;
		}
		break;
		case EN_DREGISTER_OUTIN_HGB:
		{
			nVal = 0;
		}
		break;	
	}
	return nVal;
}

*/


UINT32 Get_Light_Path_V(UINT8 nChannel)
{
//	UINT8 i = 0;
	UINT32 nADC, nVal = 0;
	
	if(nChannel == EN_LIGHT_SCATTER)
	{
		AD7799_SetChannel(AD7799_CH_AIN1P_AIN1M);
		nADC = AD7799_Get_Out_Data();
		nADC = AD7799_Get_Out_Data();
		nVal = AD7799_Get_ADC_Value(nADC);
		printf("AD7799 AIN1 ADC=%d\r\n", (int)nVal);
	
	}else if(nChannel == EN_LIGHT_SCATTER){
		AD7799_SetChannel(AD7799_CH_AIN2P_AIN2M);
		nADC = AD7799_Get_Out_Data();
		nADC = AD7799_Get_Out_Data();
		nVal = AD7799_Get_ADC_Value(nADC);
		printf("AD7799 AIN2 ADC=%d\r\n", (int)nVal);
		
	}else if(nChannel == EN_LIGHT_SCATTER){
		AD7799_SetChannel(AD7799_CH_AIN2P_AIN2M);	
		nADC = AD7799_Get_Out_Data();
		nADC = AD7799_Get_Out_Data();
		nVal = AD7799_Get_ADC_Value(nADC);
		printf("AD7799 AIN3 ADC=%d\r\n", (int)nVal);
	}	
	return 0;
}




/*
// form fpga
INT32 HW_Get_Press_ADC(UINT8 Index)
{
	UINT16 nAd    = 0;
    UINT32 nValue = 0;
    double fValue = 0;
	
    nAd = HW_Get_ADC_Perip(Index);  // 0: HGB, 1: press1

    fValue = nAd * ((double)s_nK);
    if (fValue <= (double)s_nB)
    {
        return 0;
    }

    fValue -= (double)s_nB;
    nValue = (UINT32)fValue;
    return nValue;
}
*/

// yaolan_
void HW_EN_ADC_HGB(enum eFlag flag)
{
	// e_True  e_False
	if(flag == e_True)
	{
		// switch on : WBC and RBC
		HW_SW_AdcWBC(e_True);
		HW_SW_AdcRBC(e_True);
		// get the ADC data
		HW_EN_WBC(e_True);
	}else if(flag == e_False){
		// switch off : WBC and RBC
		HW_SW_AdcWBC(e_False);
		HW_SW_AdcRBC(e_False);
		// close the ADC channel
		HW_EN_WBC(e_False);
	}
}

// yaolan_
void HW_EN_ADC_CRP(enum eFlag flag)
{
	// e_True  e_False
	if(flag == e_True)
	{
		// switch on : WBC and RBC
		HW_SW_AdcWBC(e_True);
		HW_SW_AdcRBC(e_True);
		// get the ADC data
		HW_EN_WBC(e_True);
	}else if(flag == e_False){
		// switch off : WBC and RBC
		HW_SW_AdcWBC(e_False);
		HW_SW_AdcRBC(e_False);
		// close the ADC channel
		HW_EN_WBC(e_False);
	}
}


UINT8 Send_Data_HGB(UINT32 nCmd, UINT32* pData, UINT16 nLen)
{
    //  net data feedback(big-end)
	UINT8 chReturn;
	IO_ UINT16 pos, i = 0, j = 0;
	UINT32 nData;
	// head
    s_anBufNet[0] = 0x4243;
    s_anBufNet[1] = 0x5757;
	// cmd
	s_anBufNet[2] = (((nCmd >> 24)&0x00FF) | ((nCmd >> 8)&0xFF00));
	s_anBufNet[3] = (((nCmd >> 8)&0x00FF) | ((nCmd << 8)&0xFF00));
	
	// black value
	if(nCmd == CMD_DATA_TEST_HGB)
	{
		s_anBufNet[4] = HGB_BLACK_VALUE;
		pos = 4;
	}else if(nCmd == CMD_DATA_CALIBRATE_HGB){
		pos = 4;	
	}
	// data
	for(i = 0, j = 0; i < nLen; i++)
	{
//		nData = *(pData + i);
//		//printf("send HGB_V: %d=0x%x\r\n", nData, nData);
//		s_anBufNet[pos+i] = (nData>>8)|(nData<<8);
		
		nData = *(pData + i);
		s_anBufNet[pos+j] = (((nData >> 24)&0x00FF) | ((nData >> 8)&0xFF00));
		j++;
		s_anBufNet[pos+j] = (((nData >> 8)&0x00FF) | ((nData << 8)&0xFF00));
		j++;
	}
	// send data
	chReturn = udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((nLen*2 + pos) * 2));
	if (e_Feedback_Fail == chReturn)
	{
		IT_SYS_DlyMs(10);
		udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((nLen*2 + pos) * 2));
		printf("HGB Msg Send Fail\r\n");
	}
	return chReturn;
}

UINT8 Send_Data_CRP(UINT32 nCmd, IO_ UINT32* pData, UINT16 nLen)
{
    //  net data feedback(big-end)
	UINT8 chReturn;
	UINT16 i, j = 0;
	IO_ UINT16 pos;
	UINT32 nData;
	
	LwIP_Periodic_Handle(IT_SYS_GetTicks());
	
	// head
    s_anBufNet[0] = 0x4243;
    s_anBufNet[1] = 0x5757;
	// cmd
	s_anBufNet[2] = (((nCmd >> 24)&0x00FF) | ((nCmd >> 8)&0xFF00));
	s_anBufNet[3] = (((nCmd >> 8)&0x00FF) | ((nCmd << 8)&0xFF00));
	
	if(nCmd == CMD_DATA_TEST_CRP)
	{
		nData =  nLen*4;
		s_anBufNet[4] = (nData >> 8)|(nData << 8);
		pos = 5;
	}else if(nCmd == CMD_DATA_CALIBRATE_CRP){
		pos = 4;
	}
	//
	for(i = 0, j = 0; i < nLen; i++)
	{
		nData = *(pData + i);
		s_anBufNet[pos+j] = (((nData >> 24)&0x00FF) | ((nData >> 8)&0xFF00));
		j++;
		s_anBufNet[pos+j] = (((nData >> 8)&0x00FF) | ((nData << 8)&0xFF00));
		j++;
	}
	//
	chReturn = udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((nLen*2 + pos) * 2));
	if (e_Feedback_Fail == chReturn)
	{
		IT_SYS_DlyMs(10);
		udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((nLen*2 + pos) * 2));
		printf("HGB Msg Send Fail\r\n");
	}

	return 0;
}


//---------------------------------------------------------------------------------------


// * 10000
INT32 HW_ADC_SpiGetPress(void)
{
    //
    UINT16 nAd    = 0;
    INT32 nValue = 0;
    double fValue = 0;
#if USE_STM32F407_ONLY
	nAd = 0.66*HW_Press_ADC();  // v div to 2 at cx2000_c,cx3000
	//printf("P_adc=%d\r\n", nAd);
#else
	 nAd = HW_ADC_SpiGetADC(INDEX_PRESS);  // 0: HGB, 1: press1, use FPGA
#endif    //

    fValue = nAd * ((double)s_nK);
    if (fValue <= (double)s_nB)
    {
		//return g_Record_Param.nAddPress;
        return 0;
    }

    fValue -= (double)s_nB;
	//nValue = (UINT32)fValue
    nValue = (INT32)fValue + g_Record_Param.nAddPress; 
	//printf("Get press: addP=%d,oriP=%d,P=%d",\
				(int)g_Record_Param.nAddPress, (int)fValue, (int)nValue);
    return nValue;
}

//
UINT32 HW_ADC_PressPara(UINT32 nK, UINT32 nB)
{
    s_nK = nK;
    s_nB = nB;
    //
    return e_Feedback_Success;
}


UINT8 HW_LEVEL_GetOC(UINT8 chIndex)
{
	// cx2000_c,cx3000
	return HW_LEVEL_GetOC_V3(chIndex);
	// cx2000_b
	//return HW_LEVEL_GetOC_V2(chIndex);
}



// get the level of the OC and the electrode
// 预留的悬空位返回状态1表示光耦未被遮挡 
UINT8  HW_LEVEL_GetOC_V2(UINT8 chIndex)
{
	IO_ UINT32 IRAM_  nAddr 	= 0;
	IO_ UINT16 IRAM_  anBuffer[2];
	IO_ UINT8  IRAM_  chValue   = 1;  /* 预留的悬空位返回状态1表示光耦未被遮挡 */

	//
	nAddr = (UINT32)FPGA_RD_OC_01_03;
	//
	FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
	//
	switch (chIndex)
	{
		case 0:
		{
			if (0 != (anBuffer[0] & 0x0001))
				chValue = 0x01;
			else
				chValue = 0x00;
			break;
		}
		case 1:
		{
			if (0 != (anBuffer[0] & 0x0002))
				chValue = 0x01;
			else
				chValue = 0x00;
			break;
		}
		case 2:
		{
			if (0 != (anBuffer[0] & 0x0004))
				chValue = 0x01;
			else
				chValue = 0x00;
			break;
		}
		case 3:
		{
			if (0 != (anBuffer[0] & 0x0008))
				chValue = 0x01;
			else
				chValue = 0x00;
			break;
		}
		default:
		{
			break;
		}
	}
	return chValue;
}


	


UINT8  HW_LEVEL_GetElectrode(UINT8 chIndex)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    IO_ UINT8  IRAM_  chValue;

    //
    nAddr = (UINT32)FPGA_RD_ELECTRODE_01_04;
    //
    FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    switch (chIndex)
    {
        case 0:
        {
            if (0 != (anBuffer[0] & 0x0001))
                chValue = 0x01;
            else
                chValue = 0x00;
            break;
        }
        case 1:
        {
            if (0 != (anBuffer[0] & 0x0002))
                chValue = 0x01;
            else
                chValue = 0x00;
            break;
        }
        case 2:
        {
            if (0 != (anBuffer[0] & 0x0004))
                chValue = 0x01;
            else
                chValue = 0x00;
            break;
        }
        case 3:
        {
            if (0 != (anBuffer[0] & 0x0008))
                chValue = 0x01;
            else
                chValue = 0x00;
            break;
        }
        default:
        {
            break;
        }
    }

    return chValue;
}

/* 滤波方式获取电极状态 */
#define  ELECTRODE_GET_FILTER_NUM  5
UINT8 hw_filter_get_electrode(UINT8 chIndex)
{
	// cx2000_c, cx3000
	return hw_filter_get_electrode_V3(chIndex);
	// cx2000_b
	// return hw_filter_get_electrode_V2(chIndex);
}


UINT8 hw_filter_get_electrode_V2(UINT8 chIndex)
{
    IO_ UINT8 n, cnt = 0;
    for (n = 0; n < ELECTRODE_GET_FILTER_NUM; n++)
    {
		#if USE_STM32F407_ONLY
		if(1 == hw_filter_get_electrode_V3(chIndex))
		#else
		if (1 == HW_LEVEL_GetElectrode(chIndex))
		#endif
        {
            cnt += 1;
        }
    }
    if (cnt > (ELECTRODE_GET_FILTER_NUM / 2))  /* 以超过半数的值为准 */
    {
        return  1;
    }
    return  0;
}

_EXT_ UINT8 hw_filter_get_electrode_V3(UINT8 chIndex)
{
	UINT8 nVal = 0xFF;
	if(chIndex == INDEX_ELECTRODE)
	{
		nVal = EVAL_InputGetState(I_ELEC);
	}// else if
	
	return nVal;
}


// the ADC of the slave CPU
UINT16 HW_ADC_SlaveGetADC(void)
{
    _STA_ UINT16 nValue = 0;
    //
    if (E_PL_BUF_ENABLE == SPG_IsRecvCommandValid())
    {
        nValue = (((UINT16)g_SPG_achCmdRvBuf[1]) << 8) + g_SPG_achCmdRvBuf[2];
        //
        SPG_ResetRecvComand();
    }
    else
    {
        nValue = nValue;
    }
    //
    return nValue;
}

//------------------------------
// to control the direction
UINT8  HW_DIR_Ctrl(void)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    IO_ UINT8  IRAM_  chValue;

    //
    nAddr = (UINT32)FPGA_WR_DIRECTION_BOARD;
    //
    anBuffer[0] = (UINT16)FPGA_WR_V_BOARD_1_OUT + FPGA_WR_V_BOARD_2_IN;
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}

// to control the switcher
UINT8  HW_SW_AdcWBC(enum eFlag bSw)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    IO_ UINT8  IRAM_  chValue;
    //
    nAddr = (UINT32)FPGA_WR_SW_WBC;
    //
    if (e_False == bSw)
    {
        anBuffer[0] = (UINT16)FPGA_WR_V_SW_WBC_OFF;
    }
    else
    {
        anBuffer[0] = (UINT16)FPGA_WR_V_SW_WBC_ON;
    }
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}

UINT8  HW_SW_AdcRBC(enum eFlag bSw)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    IO_ UINT8  IRAM_  chValue;
    //
    nAddr = (UINT32)FPGA_WR_SW_RBC;
    //
    if (e_False == bSw)
    {
        anBuffer[0] = (UINT16)FPGA_WR_V_SW_WBC_OFF;
    }
    else
    {
        anBuffer[0] = (UINT16)FPGA_WR_V_SW_WBC_ON;
    }
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}

//------------------------------
// enable the WBC
UINT8  HW_EN_WBC(enum eFlag bOn)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    IO_ UINT8  IRAM_  chValue;
    //
    nAddr = (UINT32)FPGA_WR_ENABLE_WBC;
    //
    if (e_False == bOn)
    {
        anBuffer[0] = (UINT16)FPGA_WR_V_ENABLE_WBC_OFF;
    }
    else
    {
        anBuffer[0] = (UINT16)FPGA_WR_V_ENABLE_WBC_ON;
    }
    // write the fpga
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}


/*
UINT8 HW_Clear_Data_Channel(eTestMode eMode)
{
	switch(eMode)
	{
		case EN_WBC_TEST:
		{
		
		}
		break;
		case EN_RBC_TEST:
		{
		
		}
		break;
		case EN_PLT_TEST:
		{
		
		}
		break;
		case EN_RBC_PLT_TEST:
		{
		
		}
		break;
		default:break;
	}
	
	return 0;
}
*/

void HW_Start_BC(eTestMode eMode)
{
	switch(eMode)
	{
		case EN_WBC_TEST:
		{
			printf("---WBC MODE---\r\n");
			Eable_ADC(EN_ADC1);
		}
		break;
		case EN_RBC_TEST:
		{
			printf("---RBC MODE---\r\n");
			Eable_ADC(EN_ADC2);
//			Disable_ADC(EN_ADC2);
//			Eable_ADC(EN_ADC2);
			
		}
		break;
		case EN_PLT_TEST:
		{
			printf("---PLT MODE---\r\n");
			Eable_ADC(EN_ADC2);
		}
		break;
		case EN_RBC_PLT_TEST:
		{
			printf("---RBC PLT MODE---\r\n");
			Eable_ADC(EN_ADC2);
		}
		break;
		default:break;
	}
}

void HW_End_BC(eTestMode eMode)
{
	switch(eMode)
	{
		case EN_WBC_TEST:
		{
			Disable_ADC(EN_ADC1);
		}
		break;
		case EN_RBC_TEST:
		{
			Disable_ADC(EN_ADC2);
		}
		break;
		case EN_PLT_TEST:
		{
			Disable_ADC(EN_ADC2);
		}
		break;
		case EN_RBC_PLT_TEST:
		{
			Disable_ADC(EN_ADC2);
		}
		break;
		default:break;
	}

}


//
void  HW_Start_WBC(void)
{
    HW_SW_AdcWBC(e_True);          /* switch on : WBC and RBC */
    HW_SW_AdcRBC(e_False);		   // RBC is back channel for WBC
    HW_EN_WBC(e_True);             /* get the ADC data */
}

//
void  HW_End_WBC(void)
{
    HW_SW_AdcWBC(e_False);
    HW_SW_AdcRBC(e_False);	// RBC is back channel for WBC
    // close the ADC channel
    HW_EN_WBC(e_False);
}


void HW_Enable_Data_Channel(eTestMode eMode)
{
	//CX2000_C,CX3000
	HW_Start_BC(eMode);
	//CX2000_B
	//HW_Start_WBC();
}



// 
void HW_Disable_Data_Channel(eTestMode eMode)
{
	//CX2000_C,CX3000
	HW_End_BC(eMode);
	//CX2000_B
	//HW_End_WBC();

}


//
//void Send_Last_FIFO_Data(void)
//{
//	UINT8 i;
//	for(i = 0; i < 9; i++) // FPGA FIFO is 4K, the last data should less than 4K
//	{
//		HW_LWIP_Working(IT_LIST_GetTicks(), IT_ADC_GetTicks(), EN_SEND_FPGA_DATA);
//	}
//}

//void Clear_FPGA_FIFO_Data(void)
//{
//	UINT8 i;
//	for(i = 0; i < 8; i++)
//	{
//		HW_DATA_ClearData(((UINT16 *)(s_anBufNet)), (UINT16 *)&s_nDataLen, (UINT16 *)&s_nStatus);
//	}
//	printf("Clear_FPGA_FIFO_Data Finished\r\n");
//}

//
UINT8 HW_DATA_ClearData(UINT16 * pnData, UINT16 * pnLen, UINT16 * pnStatus)
{
	IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    //-----------------------------------
    // 1. lock the data-buffer
    nAddr       = (UINT32)FPGA_WR_FLAG_FSMC_READING;
    anBuffer[0] = 0x0001;
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
	
//	//-----------------------------------
//	// 2. read the status of the "flag ready"
//	nAddr = (UINT32)FPGA_RD_FLAG_FSMC_READY;
//	FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
//	//
//	*pnStatus = anBuffer[0] >> 4;
////	printf("%X,", anBuffer[0]);
//	// no ready
//	if (0 == (anBuffer[0] & 0x0001))
//	{
//		//-----------------------------------
//		// 4. relase the data-buffer
//		nAddr       = (UINT32)FPGA_WR_FLAG_FSMC_READING;
//		anBuffer[0] = 0x0000;
//		FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
//		//
//		return e_Feedback_Error;
//	}
//    //-----------------------------------
    // 3.
    nAddr = (UINT32)PFGA_RD_DATA_BASE;
    *pnLen = 256;
    FPGA_DATA_ReadBuffer(pnData, nAddr, 256);
    //-----------------------------------
    // 4. relase the data-buffer
    nAddr       = (UINT32)FPGA_WR_FLAG_FSMC_READING;
    anBuffer[0] = 0x0000;
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}

//------------------------------
//
UINT8  HW_DATA_GetData(UINT16 * pnData, UINT16 * pnLen, UINT16 * pnStatus)
{
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];
    //-----------------------------------
    // 1. lock the data-buffer
    nAddr       = (UINT32)FPGA_WR_FLAG_FSMC_READING;
    anBuffer[0] = 0x0001;
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
	
	//-----------------------------------
	// 2. read the status of the "flag ready"
	nAddr = (UINT32)FPGA_RD_FLAG_FSMC_READY;
	FPGA_ReadBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
	//
	*pnStatus = anBuffer[0] >> 4;
//	printf("%X,", anBuffer[0]);
	// no ready
	if (0 == (anBuffer[0] & 0x0001))
	{
		//-----------------------------------
		// 4. relase the data-buffer
		nAddr       = (UINT32)FPGA_WR_FLAG_FSMC_READING;
		anBuffer[0] = 0x0000;
		FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
		//
		return e_Feedback_Error;
	}
    //-----------------------------------
    // 3.
    nAddr = (UINT32)PFGA_RD_DATA_BASE;
    *pnLen = 256;
    FPGA_DATA_ReadBuffer(pnData, nAddr, 256);
    //-----------------------------------
    // 4. relase the data-buffer
    nAddr       = (UINT32)FPGA_WR_FLAG_FSMC_READING;
    anBuffer[0] = 0x0000;
    FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
    //
    return e_Feedback_Success;
}


//
UINT8  Send_Packet_Test(void)
{
    UINT8 chReturn = 0;
	
    s_anBufNet[0] = 0x5344;
    s_anBufNet[1] = 0x4457;
	s_anBufNet[2] = 0x00;
	s_anBufNet[3] = 0x00;

	g_Udp_Count++;
	s_anBufNet[2] = (UINT16)(((g_Udp_Count&0xFF000000) >> 24) | ((g_Udp_Count&0x00FF0000) >> 8));
	s_anBufNet[3] = (UINT16)(((g_Udp_Count&0x000000FF) << 8) | ((g_Udp_Count&0x0000FF00)>>8));
	
	chReturn = udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((256 + 4) * 2));
	if (e_Feedback_Fail == chReturn)
	{
		g_Send_Fail++;
		printf("send fail: f=%d,s=%d\r\n", (int)g_Send_Fail, (int)g_Udp_Count);
		IT_SYS_DlyMs(1);
	}
	return chReturn;

}


_EXT_ UINT8  HW_LWIP_Working_Recv_Handle(UINT32 nTickList, UINT32 nTickAdc)
{
    UINT8 chReturn = 0;
    //=====================================================
    // 1. lwip handing
    // 1) period task
    // ---check if any packet received
    if (ETH_CheckFrameReceived())
    {
        // for debug, the "arp frame"
        // printf("-");
        // process received ethernet packet
        LwIP_Pkt_Handle();
        // udp_echoserver_senddata("abcd", 4);
			  //--------------------------------------------
        // 2) message handling
        PL_NET_CheckingFrame((UINT8 *)g_NET_achCmdRvBuf);
        if (E_PL_BUF_ENABLE == PL_NET_IsRecvCommandValid())
        {
            chReturn = MSG_Handling_MsgHandle((UINT8 *)g_NET_achCmdRvBuf, (UINT8 *)g_achFbkSdBuf);
            //
            PL_NET_ResetRecvComand();
        }
    }
    // ---handle periodic timers for LwIP
    LwIP_Periodic_Handle(IT_SYS_GetTicks());
    return chReturn;
}


UINT8 ADC_Send(UINT32 nCmd, UINT32 nId, UINT16 * pData)
{
	UINT16 i, nData;

	LwIP_Periodic_Handle(IT_SYS_GetTicks());
	s_anBufNet[0] = 0x5344;
    s_anBufNet[1] = 0x4457;
#if 0 //cx2000
	s_anBufNet[2] = (((nId>>24)&0x00FF)|((nId>>8)&0xFF00));
	s_anBufNet[3] = (((nId>>8)&0x00FF) |((nId<<8)&0xFF00));

	//memmove(&s_anBufNet[4], pData, 256);
	for(i = 0; i < 256; i++)
	{
//		nData = *(pData + i);
//		s_anBufNet[4 + i] = (nData>>8)|(nData<<8);
		s_anBufNet[4 + i] = *(pData + i);
		
//		s_anBufNet[6 + i] = *(pData + i);;
	}
	udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((256 + 4) * 2));
	
#else //cx3000
	
	s_anBufNet[2] = (((nCmd>>24)&0x00FF)|((nCmd>>8)&0xFF00));
	s_anBufNet[3] = (((nCmd>>8)&0x00FF) |((nCmd<<8)&0xFF00));
	
	s_anBufNet[4] = (((nId>>24)&0x00FF)|((nId>>8)&0xFF00));
	s_anBufNet[5] = (((nId>>8)&0x00FF) |((nId<<8)&0xFF00));

	//memmove(&s_anBufNet[4], pData, 256);
	for(i = 0; i < 256; i++)
	{
		nData = *(pData + i);
		s_anBufNet[6 + i] = ((nData&0XFF00)>>8)|((nData&0x00FF)<<8);
		
//		s_anBufNet[6 + i] = *(pData + i);;
	}
	udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((256 + 6) * 2));
#endif
	return 0;
}


UINT8 Poll_SendDMA_ADC1_Data(UINT32 nCmd)
{
//	memset((void*)&ADC_Status, 0, sizeof(ADC_Status_InitTypeDef));
//	ADC1_Init();
//	ADC_SoftwareStartConv(ADC1);
//	while(ADC_Status.nID < 40000)
//	{
		if(ADC1_Status.nSFlag == 1)
		{
			ADC_Send(nCmd, ADC1_Status.nID, g_ADC1_Buffer);
			ADC1_Status.nSFlag = 0xFF;
			memset(g_ADC1_Buffer, 0, ADC1_BUFFER_LEN_HALF);
			ADC1_Status.nSendID++;
			return e_Feedback_Success;
		}else if(ADC1_Status.nSFlag == 2){
			ADC_Send(nCmd, ADC1_Status.nID, &g_ADC1_Buffer[ADC1_BUFFER_LEN_HALF]);
			ADC1_Status.nSFlag = 0xFF;
			memset(&g_ADC1_Buffer[ADC1_BUFFER_LEN_HALF], 0, ADC1_BUFFER_LEN_HALF);
			ADC1_Status.nSendID++;
			return e_Feedback_Success;
		}	
		return e_Feedback_Fail;
//	}
//	ADC_Cmd(ADC1, DISABLE);
//	ADC_DMACmd(ADC1, DISABLE);
//	DMA_Cmd(DMA2_Stream0, DISABLE);
	
//	//IT_SYS_DlyMs(2);
//	collect_return_hdl(COLLECT_RET_SUCESS);
//	printf("adc end: id=%d, sendid=%d, T=%d\r\n", \
			(int)ADC1_Status.nID, (int)ADC1_Status.nSendID, (int)IT_SYS_GetTicks());
//	
//	//
//	nParaLen = 0;
//	pchFbkBuf[nParaLen++] = 0x44; pchFbkBuf[nParaLen++] = 0x53; pchFbkBuf[nParaLen++] = 0x57; 
//	pchFbkBuf[nParaLen++] = 0x44; pchFbkBuf[nParaLen++] = 0x30; pchFbkBuf[nParaLen++] = 0x00;
//	pchFbkBuf[nParaLen++] = 0x02; pchFbkBuf[nParaLen++] = 0x01;
//	pchFbkBuf[nParaLen++] = 0x00; pchFbkBuf[nParaLen++] = 0x00;
//	
//	nParaLen = 15;
//	strncpy((char*)&pchFbkBuf[10],"wbc adc test\r\n", nParaLen);
//	pchFbkBuf[8]  = (nParaLen - 10) >> 8;
//	pchFbkBuf[9] = (nParaLen - 10);
//	if(e_Feedback_Fail == udp_echoserver_senddata((UINT8 *)pchFbkBuf, nParaLen))
//	{
//		IT_SYS_DlyMs(1);
//	}
//	printf("debug msg len: %d\r\n", nParaLen);
//	nParaLen = 0;
}

//UINT8 Poll_SendDMA_ADC2_Data(UINT32 nCmd)
//{

//	if(ADC2_Status.nSFlag == 1)
//	{
//		//ADC_Send(CMD_DATA_NET_TEST, ADC_Status.nID, g_ADC_Buffer);
//		ADC_Send(nCmd, ADC2_Status.nID, g_ADC2_Buffer);
//		ADC1_Status.nSFlag = 0xFF;
//		//printf()
//		memset(g_ADC2_Buffer, 0, ADC2_BUFFER_LEN_HALF);
//		ADC2_Status.nSendID++;
//		return e_Feedback_Success;
//	}else if(ADC2_Status.nSFlag == 2){
//		ADC_Send(nCmd, ADC2_Status.nID, &g_ADC2_Buffer[ADC2_BUFFER_LEN_HALF]);
//		ADC1_Status.nSFlag = 0xFF;
//		memset(&g_ADC2_Buffer[ADC2_BUFFER_LEN_HALF], 0, ADC2_BUFFER_LEN_HALF);
//		ADC2_Status.nSendID++;
//		return e_Feedback_Success;
//	}	
//	return e_Feedback_Fail;
//	//printf("adc1 end: id=%d, sendid=%d, T=%d\r\n", \
//			(int)ADC2_Status.nID, (int)ADC2_Status.nSendID, (int)IT_SYS_GetTicks());
//}


UINT8 Get_ADC1_Buffer(UINT16* pData, UINT16* pLen)
{
	UINT16 i;
	if(ADC1_Status.nSFlag == 1)
	{
		//ADC_Send(nCmd, ADC1_Status.nID, g_ADC1_Buffer);
		for(i = 0; i < ADC1_BUFFER_LEN_HALF; i++)
		{
			//*(pData + i) = g_ADC1_Buffer[i] >> 8 | g_ADC1_Buffer[i] << 8;
			*(pData + i) = g_ADC1_Buffer[i];
		}
		*pLen = ADC1_BUFFER_LEN_HALF;
		ADC1_Status.nSFlag = 0xFF;
		memset(g_ADC1_Buffer, 0, ADC1_BUFFER_LEN_HALF);
		ADC1_Status.nSendID++;
		return e_Feedback_Success;
	}else if(ADC1_Status.nSFlag == 2){
		//ADC_Send(nCmd, ADC1_Status.nID, &g_ADC1_Buffer[ADC1_BUFFER_LEN_HALF]);
		for(i = 0; i < ADC1_BUFFER_LEN_HALF; i++)
		{
			//*(pData + i) = g_ADC1_Buffer[ADC1_BUFFER_LEN_HALF + i] >> 8 | g_ADC1_Buffer[ADC1_BUFFER_LEN_HALF + i] << 8;
			*(pData + i) = g_ADC1_Buffer[ADC1_BUFFER_LEN_HALF + i];
		}
		*pLen = ADC1_BUFFER_LEN_HALF;
		ADC1_Status.nSFlag = 0xFF;
		memset(&g_ADC1_Buffer[ADC1_BUFFER_LEN_HALF], 0, ADC1_BUFFER_LEN_HALF);
		ADC1_Status.nSendID++;
		return e_Feedback_Success;
	}	
	return e_Feedback_Fail;	
}


UINT8 Get_ADC2_Buffer(UINT16* pData, UINT16* pLen)
{
	UINT16 i;
	if(ADC2_Status.nSFlag == 1)
	{
		for(i = 0; i < ADC2_BUFFER_LEN_HALF; i++)
		{
			*(pData + i) = g_ADC2_Buffer[i];
		}
		*pLen = ADC1_BUFFER_LEN_HALF;
		ADC2_Status.nSFlag = 0xFF;
		memset(g_ADC2_Buffer, 0, ADC2_BUFFER_LEN_HALF);
		ADC2_Status.nSendID++;
		return e_Feedback_Success;
	}else if(ADC2_Status.nSFlag == 2){
		
		for(i = 0; i < ADC2_BUFFER_LEN_HALF; i++)
		{
			*(pData + i) = g_ADC2_Buffer[ADC2_BUFFER_LEN_HALF + i];
		}
		*pLen = ADC1_BUFFER_LEN_HALF;
		ADC2_Status.nSFlag = 0xFF;
		memset(&g_ADC2_Buffer[ADC2_BUFFER_LEN_HALF], 0, ADC2_BUFFER_LEN_HALF);
		ADC2_Status.nSendID++;
		return e_Feedback_Success;
	}	
	return e_Feedback_Fail;
}
	
UINT8 HW_WBC_GetData(UINT16* pData, UINT16* pLen, UINT16* pStatus)
{
	UINT8 nRet;
#if	SIMUATION_TEST
	memmove(pData, &g_Debug_Data, 512);
	*pLen = 256;
#else	
	#if USE_STM32F407_ONLY
		nRet = Get_ADC1_Buffer(pData, pLen);
	#else 
		//nRet = Poll_SendDMA_ADC1_Data(CMD_DATA_TEST_WBC);
	#endif
#endif
	return nRet;
}


UINT8 HW_RBC_GetData(UINT16* pData, UINT16* pLen, UINT16* pStatus)
{
	UINT8 nRet;
#if	SIMUATION_TEST
		memmove(pData, &g_Debug_Data, 512);
		*pLen = 256;
#else
	#if USE_STM32F407_ONLY
		nRet = Get_ADC2_Buffer(pData, pLen);
	#else 
		//nRet = Poll_SendDMA_ADC2_Data(CMD_DATA_TEST_RBC);
	#endif
#endif
	return nRet;
}

UINT8 HW_PLT_GetData(UINT16* pData, UINT16* pLen, UINT16* pStatus)
{
	UINT8 nRet;
#if	SIMUATION_TEST
	memmove(pData, &g_Debug_Data, 512);
	*pLen = 256;
#else	
	#if USE_STM32F407_ONLY
		nRet = Get_ADC2_Buffer(pData, pLen);
	#else 
		//nRet = Poll_SendDMA_ADC2_Data(CMD_DATA_TEST_PLT);
	#endif
#endif
	return nRet;
}

UINT8 HW_RBC_PLT_GetData(UINT16* pData, UINT16* pLen, UINT16* pStatus)
{
	UINT8 nRet;
#if	SIMUATION_TEST
	memmove(pData, &g_Debug_Data, 512);
	*pLen = 256;
#else	
	#if USE_STM32F407_ONLY
		nRet = Get_ADC2_Buffer(pData, pLen); 
	#else 
		//nRet = Poll_SendDMA_ADC2_Data(CMD_DATA_TEST_RBC_PLT); 
	#endif
#endif
	return nRet;
}


UINT8  HW_LWIP_Working(UINT32 nTickList, UINT32 nTickAdc,  EN_FPGA_DATA_FLAG eFlag, IO_ eTestMode eMode)
{
	// cx2000
	//HW_LWIP_Working_V2(nTickList, nTickAdc,  eFlag);
	
	// cx2000_c,cx3000
	Data_Circle_Handle(eMode);
	
	return Data_Circle_Handle(eMode);
}

UINT8  HW_LWIP_Working_V2(UINT32 nTickList, UINT32 nTickAdc,  EN_FPGA_DATA_FLAG eFlag)
{
    IO_ UINT8 chReturn;
	IO_ UINT16 nTemp;
    //=====================================================
    // 1. lwip handing
    // 1) period task
    // ---check if any packet received
//    if (ETH_CheckFrameReceived())
//    {
//        // for debug, the "arp frame"
//        // printf("-");
//        // process received ethernet packet
//        LwIP_Pkt_Handle();
//        // udp_echoserver_senddata("abcd", 4);
//			  //--------------------------------------------
//        // 2) message handling
//        PL_NET_CheckingFrame((UINT8 *)g_NET_achCmdRvBuf);
//        if (E_PL_BUF_ENABLE == PL_NET_IsRecvCommandValid())
//        {
//            MSG_Handling((UINT8 *)g_NET_achCmdRvBuf, (UINT8 *)g_achFbkSdBuf);
//            //
//            PL_NET_ResetRecvComand();
//        }
//    }
    // ---handle periodic timers for LwIP
    LwIP_Periodic_Handle(IT_SYS_GetTicks());

    //=====================================================
#if 1
    // 2. data feedback
    s_anBufNet[0] = 0x5344;
    s_anBufNet[1] = 0x4457;
	s_anBufNet[2] = 0x00;
	s_anBufNet[3] = 0x00;
	
    chReturn = HW_DATA_GetData(((UINT16 *)(s_anBufNet + 4)), (UINT16 *)&s_nDataLen, (UINT16 *)&s_nStatus);
    // s_anBufNet[3] = s_nStatus;
  
    if (e_Feedback_Success == chReturn)
    {
		g_Frame_Count++;
		if(eFlag == EN_SEND_FPGA_DATA){
			
			g_Udp_Count++;
			s_anBufNet[2] = (UINT16)(((g_Udp_Count&0xFF000000) >> 24) | ((g_Udp_Count&0x00FF0000) >> 8));
			s_anBufNet[3] = (UINT16)(((g_Udp_Count&0x000000FF) << 8) | ((g_Udp_Count&0x0000FF00)>>8));
			chReturn = udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((s_nDataLen + 4) * 2));
			if (e_Feedback_Fail == chReturn)
			{
				g_Send_Fail++;
				IT_SYS_DlyMs(1);
			}
		}
//		else if(eFlag == EN_DROP_FPGA_DATA){// else do not send to app
			
//		}
        //----------------------------------------------------------
        // debug..., printf the get data via serial , 20190315
//      PL_COM_SendNChar(((UINT8 *)(s_anBufNet + 0)), ((s_nDataLen + 4) * 2));
    }else{
		g_Frame_Count++;
	}
		
#endif

#if 0
    //-----------------------------------------------------
    // for testing:
    // take attention: the array "s_achBufNet" must be put
    //				   out of the function "main", or it will
    //				   case error.
    chReturn = udp_echoserver_senddata((UINT8 *)g_achFbkSdBuf, 1200);
    if (e_Feedback_Fail == chReturn)
    {
        IT_SYS_DlyMs(1);
    }
#endif

    return e_Feedback_Success;
}


//UINT8  HW_LWIP_Working_V3(UINT32 nTickList, UINT32 nTickAdc,  EN_FPGA_DATA_FLAG eFlag, IO_ eTestMode eMode)
UINT8 Data_Circle_Handle(IO_ eTestMode eMode)
{
	IO_ UINT8 nRet = 0xFF;
	UINT32 nCmd;
	
	LwIP_Periodic_Handle(IT_SYS_GetTicks());
	//
	s_anBufNet[0] = 0x5344;
	s_anBufNet[1] = 0x4457;
	//
	switch(eMode)
	{
		case EN_WBC_TEST:
		{
			nCmd = CMD_DATA_TEST_WBC;
			s_anBufNet[2] = (((nCmd>>24)&0x00FF)|((nCmd>>8)&0xFF00));
			s_anBufNet[3] = (((nCmd>>8)&0x00FF) |((nCmd<<8)&0xFF00));
			nRet = HW_WBC_GetData(((UINT16 *)(s_anBufNet + 6)), (UINT16 *)&s_nDataLen, (UINT16 *)&s_nStatus);
			//nRet = HW_DATA_GetData(((UINT16 *)(s_anBufNet + 4)), (UINT16 *)&s_nDataLen, (UINT16 *)&s_nStatus);
		}
		break;
		case EN_RBC_TEST:
		{
			nCmd = CMD_DATA_TEST_RBC;
			s_anBufNet[2] = (((nCmd>>24)&0x00FF)|((nCmd>>8)&0xFF00));
			s_anBufNet[3] = (((nCmd>>8)&0x00FF) |((nCmd<<8)&0xFF00));
			nRet = HW_RBC_GetData(((UINT16 *)(s_anBufNet + 6)), (UINT16 *)&s_nDataLen, (UINT16 *)&s_nStatus);
		}
		break;
		case EN_PLT_TEST:
		{
			nCmd = CMD_DATA_TEST_PLT;
			s_anBufNet[2] = (((nCmd>>24)&0x00FF)|((nCmd>>8)&0xFF00));
			s_anBufNet[3] = (((nCmd>>8)&0x00FF) |((nCmd<<8)&0xFF00));
			nRet = HW_PLT_GetData(((UINT16 *)(s_anBufNet + 6)), (UINT16 *)&s_nDataLen, (UINT16 *)&s_nStatus);
		}
		break;
		case EN_RBC_PLT_TEST:
		{
			nCmd = CMD_DATA_TEST_RBC_PLT;
			s_anBufNet[2] = (((nCmd>>24)&0x00FF)|((nCmd>>8)&0xFF00));
			s_anBufNet[3] = (((nCmd>>8)&0x00FF) |((nCmd<<8)&0xFF00));
			nRet = HW_RBC_PLT_GetData(((UINT16 *)(s_anBufNet + 6)), (UINT16 *)&s_nDataLen, (UINT16 *)&s_nStatus);
		}
		break;
		case EN_MODE_END:
		{
			;
		}
		break;
		default:break;
	}
	//
	if (e_Feedback_Success == nRet)
    {
		g_Udp_Count++;
		s_anBufNet[4] = (((g_Udp_Count>>24)&0x00FF)|((g_Udp_Count>>8)&0xFF00));
		s_anBufNet[5] = (((g_Udp_Count>>8)&0x00FF) |((g_Udp_Count<<8)&0xFF00));
		nRet = udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((s_nDataLen + 6) * 2));
		if (e_Feedback_Fail == nRet)
		{
			g_Send_Fail++;
			//IT_SYS_DlyMs(1);
		}//else if(eFlag == EN_DROP_FPGA_DATA){// else do not send to app
			
//		}
//      //debug..., printf the get data via serial , 20190315
//      PL_COM_SendNChar(((UINT8 *)(s_anBufNet + 0)), ((s_nDataLen + 4) * 2));
    }//else{
		// data not ready
	//}
	return 0;
}


//
//UINT8  HW_LWIP_Working(UINT32 nTickList, UINT32 nTickAdc,  EN_FPGA_DATA_FLAG eFlag)
//{
//    IO_ UINT8 chReturn;
//	IO_ UINT16 nTemp;
//    LwIP_Periodic_Handle(IT_SYS_GetTicks());

//    //=====================================================
//#if 1
//    // 2. data feedback
//    s_anBufNet[0] = 0x5344;
//    s_anBufNet[1] = 0x4457;
//	// cmd
//	s_anBufNet[2] = 0x00;
//	s_anBufNet[3] = 0x00;
//	
//    chReturn = HW_DATA_GetData(((UINT16 *)(s_anBufNet + 4)), (UINT16 *)&s_nDataLen, (UINT16 *)&s_nStatus);
//    // s_anBufNet[3] = s_nStatus;
//  
//    if (e_Feedback_Success == chReturn)
//    {
//		g_Frame_Count++;
//		if(eFlag == EN_SEND_FPGA_DATA){
//			
//			g_Udp_Count++;
//			s_anBufNet[2] = (((g_Udp_Count>>24)&0x00FF)|((g_Udp_Count>>8)&0xFF00));
//			s_anBufNet[3] = (((g_Udp_Count>>8)&0x00FF) |((g_Udp_Count<<8)&0xFF00));
//			chReturn = udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((s_nDataLen + 4) * 2));
//			if (e_Feedback_Fail == chReturn)
//			{
//				g_Send_Fail++;
//				//IT_SYS_DlyMs(1);
//			}
//		}
//    }else{
//		g_Frame_Count++;
//	}
//		
//#endif

//#if 0
//    //-----------------------------------------------------
//    // for testing:
//    // take attention: the array "s_achBufNet" must be put
//    //				   out of the function "main", or it will
//    //				   case error.
//    chReturn = udp_echoserver_senddata((UINT8 *)g_achFbkSdBuf, 1200);
//    if (e_Feedback_Fail == chReturn)
//    {
//        IT_SYS_DlyMs(1);
//    }
//#endif

//    return e_Feedback_Success;
//}


UINT8  HW_LWIP_MainLine(void)
{
//    UINT8 chReturn;
    //=====================================================
    // 1. lwip handing
    // 1) period task
    // ---check if any packet received
	//Micro_Switch_Check();
    if (ETH_CheckFrameReceived())
    {
        // for debug, the "arp frame"
        // printf("-");
        // process received ethernet packet
        LwIP_Pkt_Handle();
        // udp_echoserver_senddata("abcd", 4);
        //--------------------------------------------
        // 2) message handling
        PL_NET_CheckingFrame((UINT8 *)g_NET_achCmdRvBuf);
        if (E_PL_BUF_ENABLE == PL_NET_IsRecvCommandValid())
        {
            MSG_Handling((UINT8 *)g_NET_achCmdRvBuf, (UINT8 *)g_achFbkSdBuf);
            //
            PL_NET_ResetRecvComand();
        }
    }
    // ---handle periodic timers for LwIP
    LwIP_Periodic_Handle(IT_SYS_GetTicks());

#if 0
    //=====================================================
    // 2. data feedback
    s_anBufNet[0] = 0x5344;
    s_anBufNet[1] = 0x4457;
    s_anBufNet[2] = 0x0001;
    s_anBufNet[3] = 0x0000;
   
	chReturn = HW_DATA_GetData((UINT16 *)(s_anBufNet + 4), (UINT16 *)&s_nDataLen, (UINT16 *)&s_nStatus); 
    s_anBufNet[3] = s_nStatus;
    //
    if (e_Feedback_Success  == chReturn)
    {
       chReturn = udp_echoserver_senddata(((UINT8 *)(s_anBufNet + 0)), ((s_nDataLen + 4) * 2));
        if (e_Feedback_Fail == chReturn)
        {
            IT_SYS_DlyMs(1);
        }
//----------------------------------------------------------
//       debug..., send the last get data via serial
//       PL_COM_SendNChar(((UINT8 *)(s_anBufNet + 0)), ((s_nDataLen + 4) * 2));
    }
#endif	


#if 0
    
    // for testing:
    // take attention: the array "s_achBufNet" must be put
    //				   out of the function "main", or it will
    //				   case error.
    chReturn = udp_echoserver_senddata(UINT8 *)(g_achFbkSdBuf, 1200);
    if (e_Feedback_Fail == chReturn)
    {
        IT_SYS_DlyMs(1);
    }
#endif
    return e_Feedback_Success;
}



// reset by software
UINT8 MT_RESET_Software(void)
{
    // attention: no return !
    FPGA_ResetHardware();

    NVIC_SystemReset();

    return e_Feedback_Success;
}


//
UINT8  MSG_SampleBoardFeedback(void)
{
    IO_ UINT8  XRAM_ chData;

    //
    while (1)
    {
        if (e_Feedback_Success != SPG_Recv_GetChar((UINT8 *)&chData))
        {
            break;
        }
        else
        {
            PL_COM_SendChar(chData);
        }
    }

    return e_Feedback_Success;
}


//------------------------------------------------------------------CX2000_B API START---------------------------


UINT8  HW_Valve_On_V2(UINT8 chIndex)
{
    IO_ UINT8  XRAM_ chOffset = 0;
    IO_ UINT32 IRAM_  nAddr     = 0;
    IO_ UINT16 IRAM_  anBuffer[2];

    // attention: 0-on, 1-off.

    // fpga
    /* if( (chIndex >= 0) && (chIndex <= 5) )    // 0 ~ 5 */
    if (chIndex <= 5)   /* 0 ~ 5 */
    {
        chOffset = chIndex - 0;
        //m_nIoValves |= ((UINT16)1 << chOffset);
        m_nIoValves &= ~((UINT16)1 << chOffset);
        // address
        nAddr = (UINT32)FPGA_WR_VALVE_01_06;
        // value
        anBuffer[0] = m_nIoValves;
        // write to the fpga
        FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
        //
        printf("valve %d on\r\n", chIndex);
    }
    // main mcu
    else if ((chIndex >= 6) && (chIndex <= 20))
    {
        // keep
    }
    // error
    else
    {
        SYS_ErrorMark((UINT8)ERR_COMMAND_NO_VALID, chIndex);
        return e_Feedback_Error;
    }
    return e_Feedback_Success;
}

//
UINT8  HW_Valve_Off_V2(UINT8 chIndex)
{
    IO_ UINT8  XRAM_ chOffset = 0;
    IO_ UINT32 IRAM_  nAddr 	= 0;
    IO_ UINT16 IRAM_  anBuffer[2];

    // attention: 0-on, 1-off.

    // fpga
    /* if( (chIndex >= 0) && (chIndex <= 5) )    // 0 ~ 5 */
    if (chIndex <= 5)
    {
        /* if( (chIndex >= 0) && (chIndex <= 15) ) */
        if (chIndex <= 15)
        {
            chOffset = chIndex - 0;
            m_nIoValves |= ((UINT16)1 << chOffset);
            //m_nIoValves &= ~((UINT16)1 << chOffset);
            // address
            nAddr = (UINT32)FPGA_WR_VALVE_01_06;
            // value
            anBuffer[0] = m_nIoValves;
        }
        // write to the fpga
        FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
        //
        printf("valve %d off\r\n", chIndex);
    }
    // main mcu
    else if ((chIndex >= 6) && (chIndex <= 20))
    {
        // keep
    }
    // error
    else
    {
        SYS_ErrorMark((UINT8)ERR_COMMAND_NO_VALID, chIndex);
        return e_Feedback_Error;
    }

    return e_Feedback_Success;
}

//
UINT8  HW_PUMP_Pulse_V2(UINT32 nFreq, enum eDirection eDir)
{
	IO_ UINT32 IRAM_  nAddr 	= 0;
	IO_ UINT16 IRAM_  anBuffer[2];
	//
	IO_ UINT32 XRAM_  nFqCnt    = 0;

	//----- 1. direction -----
	// address
	nAddr = (UINT32)FPGA_WR_PUMP_DIR;
	// value
	if (e_Dir_Neg == eDir)
	{
		anBuffer[0] = 0x00000000;
	}
	else
	{
		anBuffer[0] = 0x00000001;
	}
	// write the fpga
	FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word
	//----- 2. frequence -----
	// address
	nAddr = (UINT32)FPGA_WR_PUMP_FQ_CNT;
	// value
	// attentio: the fpga's sysclk = 25000000Hz
#if 0  // 2015_04_08-11shi-changed by LHT	
	nFqCnt = 12500000 / (nFreq + 1);      // nFreq != 0, half-freq-count, 12.5MHz/nFreq
#else
	if (nFreq >= 25000)
	{
		nFqCnt = 25000;
	}
	else
	{
		nFqCnt = nFreq;
	}
#endif
	anBuffer[0] = (UINT16)(nFqCnt & 0xFFFF);
	anBuffer[1] = (UINT16)((nFqCnt >> 16) & 0xFFFF);
	// write the fpga
	FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 2); // 2 half-word
	//----- 3. run or stop -----
	// address
	nAddr = (UINT32)FPGA_WR_PUMP_RUN;
	// value
	if (0 == nFreq)  // stop
	{
		anBuffer[0] = 0x00000000;
		//
		printf("pump off\r\n");
	}
	else             // run
	{
		anBuffer[0] = 0x00000001;
		//
		printf("pump on at %0.5d ticks per ms (total 25000 ticks per ms)\r\n", (int)nFqCnt);
	}
	// write the fpga
	FPGA_WriteBuffer((UINT16 *)anBuffer, nAddr, 1); // 1 half-word

	return e_Feedback_Success;
}


UINT32 Get_WBC_V_Value_V2(void)
{
	UINT16 nWord;
	UINT32 nValue;

	nWord = HW_ADC_SpiGetADC(INDEX_ADC_48V);
	nValue = nWord*10000/4096;
//	printf("WBC_V: adc=%d, wbc_v=%d\r\n", nWord, (int)nValue);
	return nValue;
}






//------------------------------------------------------------------CX2000_B API START---------------------------


//------------------------------------------------------------------CX2000_C API START---------------------------
INT32 Get_Press_Value(UINT8 nNum)
{
	UINT8 i;
	INT32 nPress = 0;
	for(i = 0; i < nNum; i++)
	{
		nPress += HW_Press_Value();
	}
	nPress /= nNum;
	return nPress;
}


INT32 HW_Press_Value(void)
{
	
	INT32 nVal = 0;
	
#if PRESS_SENSOR_TYPE_ADC  
	 // ADC Interface 
	 nVal = HW_ADC_SpiGetPress();
#else
	 // IIC Interface Press Sensor
	 nVal = HW_Press_I2C();

#endif
	 return nVal;
}

// get the level of the OC and the electrode
// 预留的悬空位返回状态1表示光耦未被遮挡 
UINT8  HW_LEVEL_GetOC_V3(UINT8 chIndex)
{
	UINT8 nVal;
	switch(chIndex)
	{
		case OC_HOME_CHANNEL: // Motor 3 IN OC
		{
			nVal =  EVAL_InputGetState(I_Motor3_IN_OC);
		}
		break;
		case OC_OUT_CHANNEL: // Motor 3 OUT OC
		{
			nVal =  EVAL_InputGetState(I_Motor3_OUT_OC);
		}
		break;
		case OC_SAMPLE_RELEA_CHANNEL: // Motor 4 IN OC
		{
			nVal = EVAL_InputGetState(I_Motor4_IN_OC);
		}
		break;
		case OC_SAMPLE_HOLD_CHANNEL: // Motor 4 OUT OC
		{
			nVal = EVAL_InputGetState(I_Motor4_OUT_OC);
		}
		break;
		default:break;
	}
	return nVal;
}

UINT32 Get_WBC_V_Value_V3(void)
{
	UINT16 nVal;

	nVal = HW_XK_V();
	return nVal;
}



//------------------------------------------------------------------CX2000_C API END---------------------------

#endif
