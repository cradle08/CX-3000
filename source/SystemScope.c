// SystemScope.c

#ifndef    __SYSTEM_SCOPE_C__
#define    __SYSTEM_SCOPE_C__

#include "KernelHeader.h"



//-----------------------------------------------------------------------------------------
//

#define POS_AT_HOME    1200



//-----------------------------------------------------------------------------------------
//

//
struct tAxisPosStatus  g_tAxisPosStatus;      //
struct tOCLevel        g_tOCLevel;            //
struct tSystemStatus   g_tSystemStatus;       //
struct tSyetemPara     g_tSystemPara;         //

struct tOppoInterval   g_atInterval[MV_MOTOR_NUM];  //


//
UINT32  g_nError;            //
UINT32  g_nFlag;             //
UINT16  g_nErrorParaSet[32]; //

//
static  UINT32 const m_nConstBitsMap[32] =
{
    0x00000001,
    0x00000002,
    0x00000004,
    0x00000008,
    0x00000010,
    0x00000020,
    0x00000040,
    0x00000080,
    0x00000100,
    0x00000200,
    0x00000400,
    0x00000800,
    0x00001000,
    0x00002000,
    0x00004000,
    0x00008000,
    0x00010000,
    0x00020000,
    0x00040000,
    0x00080000,
    0x00100000,
    0x00200000,
    0x00400000,
    0x00800000,
    0x01000000,
    0x02000000,
    0x04000000,
    0x08000000,
    0x10000000,
    0x20000000,
    0x40000000,
    0x80000000,
};

//
static  UINT16  const  m_nConstErrorMap[32] =
{
    4500,     //
    4501,
    4502,
    4503,
    4504,
    4505,
    4506,
    4507,
    4508,
    4509,
    4510,
    4511,
    4512,
    4513,
    4514,
    4515,
    4516,
    4517,
    4518,
    4519,
    4520,
    4521,
    4522,
    4523,
    4524,
    4525,
    4526,
    4527,
    4528,
    4529,
    4530,
    4531
};

#if 0
static UINT8 const m_constBuildVersion[]  = "Version 1.0";
static UINT8 const m_constBuildTime[]     = __DATE__"  "__TIME__;
#endif

//-----------------------------------------------------------------------------------------
//

#if 0

UINT8 SYS_GetBuildVersion(void)
{
    char strSend[50];
    sprintf(strSend, "%s", m_constBuildVersion); // 编译版本号
    SL_SendString(strSend);

    return e_Feedback_Success;
}

UINT8 SYS_GetBuildTime(void)
{
    char strSend50];
    sprintf(strSend, "%s", m_constBuildTime);
    SL_SendString(strSend);

    return e_Feedback_Success;
}
#endif

void Iint_SysParam(void)
{
	memset((void*)&g_CRP_Data, 0, sizeof(struct CRP_DataType));
	g_CRP_Data.eEnable = e_False;

}

	
UINT8 SYS_InitSystem(void)
{
    //
    g_nError = 0;
    //
    g_nFlag  = 0;
    //
    // memset(g_nErrorParaSet, 0x00, 2 * 32);

    g_tAxisPosStatus.eAxisX = E_AXIS_X_POS_UNSURE;
    g_tAxisPosStatus.eAxisY = E_AXIS_Y_POS_UNSURE;
    g_tAxisPosStatus.eAxisZ = E_AXIS_Z_POS_UNSURE;
    g_tAxisPosStatus.eAxisM = E_AXIS_M_POS_UNSURE;
    g_tAxisPosStatus.nAxisX = 0;
    g_tAxisPosStatus.nAxisY = 0;
    g_tAxisPosStatus.nAxisZ = 0;
    g_tAxisPosStatus.nAxisM = 0;

    g_tOCLevel.eHomeAxisX   = E_LEVEL_LOW;
    g_tOCLevel.eHomeAxisY   = E_LEVEL_LOW;
    g_tOCLevel.eHomeAxisZ   = E_LEVEL_LOW;
    g_tOCLevel.eHomeAxisM   = E_LEVEL_LOW;

    g_tSystemStatus.eFlagSysParaLoad             = e_False;
    g_tSystemStatus.eaFlagMotorParaLoad[Motor_X] = e_False;
    g_tSystemStatus.eaFlagMotorParaLoad[Motor_Y] = e_False;
    //g_tSystemStatus.eaFlagMotorParaLoad[Motor_Z] = e_False;
    //g_tSystemStatus.eaFlagMotorParaLoad[Motor_M] = e_False;
    // g_tSystemStatus.eaFlagMotorParaLoad[Motor_AT] = e_False;

    // {
    //
    g_tSystemPara.chAxisXHome_d = (UINT8)0;
    g_tSystemPara.chAxisXHome_3 = (UINT8)0;
    g_tSystemPara.chAxisXHome_2 = (UINT8)0;
    g_tSystemPara.chAxisXHome_1 = (UINT8)0;
    g_tSystemPara.chAxisXHome_0 = (UINT8)0;
    //
    g_tSystemPara.chAxisYHome_d = (UINT8)0;       // -20.0000 = 0x30d40
    g_tSystemPara.chAxisYHome_3 = (UINT8)0;
    g_tSystemPara.chAxisYHome_2 = (UINT8)0;
    g_tSystemPara.chAxisYHome_1 = (UINT8)0;
    g_tSystemPara.chAxisYHome_0 = (UINT8)0;
    //
    g_tSystemPara.chAxisZHome_d = (UINT8)0;       // -80.0000 = 0x0c3500
    g_tSystemPara.chAxisZHome_3 = (UINT8)0;
    g_tSystemPara.chAxisZHome_2 = (UINT8)0;
    g_tSystemPara.chAxisZHome_1 = (UINT8)0;
    g_tSystemPara.chAxisZHome_0 = (UINT8)0;
    //
    g_tSystemPara.chAxisMHome_d = (UINT8)0;       // -20.0000 = 0x30d40
    g_tSystemPara.chAxisMHome_3 = (UINT8)0;
    g_tSystemPara.chAxisMHome_2 = (UINT8)0;
    g_tSystemPara.chAxisMHome_1 = (UINT8)0;
    g_tSystemPara.chAxisMHome_0 = (UINT8)0;
    //
    g_tSystemPara.chAxisXPosMax_3 = (UINT8)((MAX_DITANCE_MT_X >> 24) & 0xFF);
    g_tSystemPara.chAxisXPosMax_2 = (UINT8)((MAX_DITANCE_MT_X >> 16) & 0xFF);
    g_tSystemPara.chAxisXPosMax_1 = (UINT8)((MAX_DITANCE_MT_X >>  8) & 0xFF);
    g_tSystemPara.chAxisXPosMax_0 = (UINT8)((MAX_DITANCE_MT_X >>  0) & 0xFF);
    //
    g_tSystemPara.chAxisYPosMax_3 = (UINT8)((MAX_DITANCE_MT_Y >> 24) & 0xFF);
    g_tSystemPara.chAxisYPosMax_2 = (UINT8)((MAX_DITANCE_MT_Y >> 16) & 0xFF);
    g_tSystemPara.chAxisYPosMax_1 = (UINT8)((MAX_DITANCE_MT_Y >>  8) & 0xFF);
    g_tSystemPara.chAxisYPosMax_0 = (UINT8)((MAX_DITANCE_MT_Y >>  0) & 0xFF);
    //
    //g_tSystemPara.chAxisZPosMax_3 = (UINT8)((MAX_DITANCE_MT_Z >> 24) & 0xFF);
    //g_tSystemPara.chAxisZPosMax_2 = (UINT8)((MAX_DITANCE_MT_Z >> 16) & 0xFF);
    //g_tSystemPara.chAxisZPosMax_1 = (UINT8)((MAX_DITANCE_MT_Z >>  8) & 0xFF);
    //g_tSystemPara.chAxisZPosMax_0 = (UINT8)((MAX_DITANCE_MT_Z >>  0) & 0xFF);
    //
    //g_tSystemPara.chAxisMPosMax_3 = (UINT8)((MAX_DITANCE_MT_M >> 24) & 0xFF);
    //g_tSystemPara.chAxisMPosMax_2 = (UINT8)((MAX_DITANCE_MT_M >> 16) & 0xFF);
    //g_tSystemPara.chAxisMPosMax_1 = (UINT8)((MAX_DITANCE_MT_M >>  8) & 0xFF);
    //g_tSystemPara.chAxisMPosMax_0 = (UINT8)((MAX_DITANCE_MT_M >>  0) & 0xFF);
    //
    g_tSystemPara.chLumGrade  = (UINT8)0;

    //
    g_tSystemPara.chValX_h  = (UINT8)((INTERVAL_OPPO_X >>  8) & 0xFF);
    g_tSystemPara.chValX_l  = (UINT8)((INTERVAL_OPPO_X >>  0) & 0xFF);
    g_tSystemPara.chValY_h  = (UINT8)((INTERVAL_OPPO_Y >>  8) & 0xFF);
    g_tSystemPara.chValY_l  = (UINT8)((INTERVAL_OPPO_Y >>  0) & 0xFF);
    g_tSystemPara.chValZ_h  = (UINT8)((INTERVAL_OPPO_Z >>  8) & 0xFF);
    g_tSystemPara.chValZ_l  = (UINT8)((INTERVAL_OPPO_Z >>  0) & 0xFF);
    g_tSystemPara.chValM_h  = (UINT8)((INTERVAL_OPPO_M >>  8) & 0xFF);
    g_tSystemPara.chValM_l  = (UINT8)((INTERVAL_OPPO_M >>  0) & 0xFF);
    //
    g_tSystemPara.chAxisATHome_d  = (UINT8)0;          // -
    g_tSystemPara.chAxisATHome_3  = (UINT8)((POS_AT_HOME >>  24) & 0xFF);
    g_tSystemPara.chAxisATHome_2  = (UINT8)((POS_AT_HOME >>  16) & 0xFF);
    g_tSystemPara.chAxisATHome_1  = (UINT8)((POS_AT_HOME >>   8) & 0xFF);
    g_tSystemPara.chAxisATHome_0  = (UINT8)((POS_AT_HOME >>   0) & 0xFF);
    //
    //g_tSystemPara.chAxisATPosMax_3  =  (UINT8)((MAX_DITANCE_MT_AT >> 24) & 0xFF);
    //g_tSystemPara.chAxisATPosMax_2  =  (UINT8)((MAX_DITANCE_MT_AT >> 16) & 0xFF);
    //g_tSystemPara.chAxisATPosMax_1  =  (UINT8)((MAX_DITANCE_MT_AT >>  8) & 0xFF);
    //g_tSystemPara.chAxisATPosMax_0  =  (UINT8)((MAX_DITANCE_MT_AT >>  0) & 0xFF);
    // }
    g_atInterval[Motor_X].eDir     = e_Dir_Neg;
    g_atInterval[Motor_X].nSteps   = (UINT16)INTERVAL_OPPO_X;
    g_atInterval[Motor_Y].eDir     = e_Dir_Neg;
    g_atInterval[Motor_Y].nSteps   = (UINT16)INTERVAL_OPPO_Y;
//	g_atInterval[Motor_Z].eDir     = e_Dir_Neg;
//	g_atInterval[Motor_Z].nSteps   = (UINT16)INTERVAL_OPPO_Z;
//	g_atInterval[Motor_M].eDir     = e_Dir_Neg;
//	g_atInterval[Motor_M].nSteps   = (UINT16)INTERVAL_OPPO_M;
#if 0
    // 2015_07_20-added
    IT_SYS_DlyMs(1000);
    HW_PARA_Update();
#endif
    return e_Feedback_Success;
}


UINT8 SYS_ResetSystem(void)
{
    //
    //
    //

    return e_Feedback_Success;
}

UINT8 SYS_ErrorAnalyze(UINT8 * pchSaveBuf, UINT16 * pnErrorNum)
{
    UINT16  nCount    = 0;
    UINT16  nErrorNum = 0;


    if (0x00000000 == g_nError & 0xFFFFFFFF) //
    {
        *pnErrorNum = 0;
        return e_Feedback_Success;
    }

    nErrorNum = 0;
    for (nCount = 0; nCount < 32; nCount++) //
    {
        if (0x00000000 != (m_nConstBitsMap[nCount] & g_nError))
        {
            //
            *(pchSaveBuf + (4 * nErrorNum + 0)) = (UINT8)((m_nConstErrorMap[nCount] >> 8) & 0xFF);
            *(pchSaveBuf + (4 * nErrorNum + 1)) = (UINT8)(m_nConstErrorMap[nCount] & 0xFF);
            //
            *(pchSaveBuf + (4 * nErrorNum + 2)) = (UINT8)((g_nErrorParaSet[nCount] >> 8) & 0xFF);
            *(pchSaveBuf + (4 * nErrorNum + 3)) = (UINT8)(g_nErrorParaSet[nCount] & 0xFF);

            nErrorNum += 1;	 // 1
        }

    }

    *pnErrorNum = nErrorNum;  //

    g_nError = 0x00000000;	 //


    return e_Feedback_Success;
}

UINT8 SYS_ErrorMark(UINT8 chErrorIndex, UINT16 nParaValue)
{
    if (chErrorIndex >= 32)	 //
    {
        return e_Feedback_Fail;
    }

    g_nError |= m_nConstBitsMap[chErrorIndex];
    g_nErrorParaSet[chErrorIndex] = nParaValue;

    return e_Feedback_Success;
}

#if 0
UINT8 SYS_ErrorClear(UINT8 chErrorIndex)
{
    if (chErrorIndex >= 32)   // 错误码位的序号出错
    {
        return e_Feedback_Fail;
    }

    g_nError &= ~m_nConstBitsMap[chErrorIndex];
    g_nErrorParaSet[chErrorIndex] = 0x0000;         // 清空参数值

    return e_Feedback_Success;
}

UINT8 SYS_FlagMark(UINT8 chFlagIndex)
{
    if (chFlagIndex >= 32)   // 错误码位的序号出错
    {
        return e_Feedback_Fail;
    }

    g_nFlag |= m_nConstBitsMap[chFlagIndex];

    return e_Feedback_Success;
}

UINT8 SYS_FlagClear(UINT8 chFlagIndex)
{
    if (chFlagIndex >= 32)   // 错误码位的序号出错
    {
        return e_Feedback_Fail;
    }

    g_nFlag &= ~m_nConstBitsMap[chFlagIndex];

    return e_Feedback_Success;
}
#endif

//
enum eFlag SYS_HasError(void)
{
    if (0 == g_nError)
    {
        return e_False;
    }
    else
    {
        return e_True;
    }
}


#endif

