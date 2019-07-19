// Move.c

#ifndef  __MOVE_C__
#define  __MOVE_C__


#include "ChainHeader.h"


//-----------------------------------------------------------------------------------------
// global variable definition

// parameters and status
IO_ struct tMvMotorStatus XRAM_ g_atMotorStatus[MV_MOTOR_NUM]; // status
IO_ struct tMvMotorPara   XRAM_ g_atMotorPara[MV_MOTOR_NUM];   // paramenters

// direction position
_STA_ UINT32 CODE_ m_acnDirPos[MV_MOTOR_NUM] =
{
     DIR_POS_MT_X,
	 DIR_POS_MT_Y
};
// direction negation
_STA_ UINT32 CODE_ m_acnDirNeg[MV_MOTOR_NUM] = 
{
	 DIR_NEG_MT_X,
	 DIR_NEG_MT_Y
};

//-----------------------------------------------------------------------------------------
// function definition

//
UINT8  MV_InitPara(enum eMvMotor eMotor, 
                        UINT32 nFreqMin, 
                        UINT32 nFreqMax, 
                        UINT32 nFreqInc, 
                        UINT32 nFreqSam )
{
	IO_ UINT32 IRAM_  nAddr     = 0;  
    IO_ UINT16 IRAM_  anBuffer[8]; 
	
    // range check 
    if(eMotor >= MV_MOTOR_NUM)
    {
        return e_Feedback_Error;
	}
	// parameters check
	if( (0 == nFreqSam) ||
		(0 == nFreqInc) || 
		(nFreqMin > nFreqMax) ||
		(nFreqInc > nFreqMax) )
	{
        return e_Feedback_Error;
	}
		
	// record the motor's parameters
    g_atMotorPara[eMotor].nFreqMin   = nFreqMin;
	g_atMotorPara[eMotor].nFreqMax   = nFreqMax;
	g_atMotorPara[eMotor].nFreqInc   = nFreqInc;
    g_atMotorPara[eMotor].nFreqSam   = nFreqSam;

	//------------------------------------------
    // write the motor'a parameters to the FPGA
    // FQ_STR->min
	anBuffer[0] = (UINT16)( nFreqMin & 0xFFFF);
	anBuffer[1] = (UINT16)((nFreqMin >> 16) & 0xFFFF);
	// FQ_ACC->inc
	anBuffer[2] = (UINT16)( nFreqInc & 0xFFFF);
	anBuffer[3] = (UINT16)((nFreqInc >> 16) & 0xFFFF);
	// FQ_DEC->inc
	anBuffer[4] = (UINT16)( nFreqInc & 0xFFFF);
	anBuffer[5] = (UINT16)((nFreqInc >> 16) & 0xFFFF);
	// FQ_SAM->sam
    anBuffer[6] = (UINT16)( nFreqSam & 0xFFFF);
	anBuffer[7] = (UINT16)((nFreqSam >> 16) & 0xFFFF);
	// address
	nAddr = (UINT32)FPGA_WR_MOTOR_REGISTER_BASE + 
		            (eMotor * FPGA_WR_MOTOR_PARA_OFFSET) + 
		            FPGA_WR_MOTOR_FQ_STR;  // 12~19
    FPGA_WriteBuffer((UINT16*)anBuffer, nAddr, 8);
	//------------------------------------------

    return e_Feedback_Success; 
}

//
UINT8  MV_Move(enum eMvMotor eMotor, UINT32 nSteps, enum eDirection eDir)
{
    IO_ UINT32 IRAM_  nValue = 0;
	IO_ UINT32 IRAM_  nBase  = 0;
	IO_ UINT32 IRAM_  nAllow = 0;
	IO_ UINT32 IRAM_  nAcc   = 0;
	IO_ UINT32 IRAM_  nEqu   = 0;
	IO_ UINT32 IRAM_  nCount = 0;
	IO_ UINT32 IRAM_  nDirMap   = 0;
	IO_ UINT32 IRAM_  nAddr     = 0; 
    IO_ UINT16 IRAM_  anBuffer[8];        
	
	//------------------------------------------
    // 1. checking 
    // range check 
    if(eMotor >= MV_MOTOR_NUM)
    {
        g_atMotorStatus[eMotor].eFinish = e_True;
        return e_Feedback_Error;
	}
	// parameters check
    if( (0 == g_atMotorPara[eMotor].nFreqSam) ||
		(0 == g_atMotorPara[eMotor].nFreqInc) ||
		(g_atMotorPara[eMotor].nFreqMin > g_atMotorPara[eMotor].nFreqMax) ||
		(g_atMotorPara[eMotor].nFreqInc > g_atMotorPara[eMotor].nFreqMax) )
	{
	    g_atMotorStatus[eMotor].eFinish = e_True;
        return e_Feedback_Error;
	}
	
	//------------------------------------------
    // 2. record the status of the motor
    g_atMotorStatus[eMotor].eDir    = eDir;
    g_atMotorStatus[eMotor].eFinish = e_False;
    g_atMotorStatus[eMotor].nSteps  = nSteps;
	
	//------------------------------------------
    // 3. calculate the curve of the movement according to the parameters 
    // 1) stairs needed of two ways
    nBase  = (UINT32)( (g_atMotorPara[eMotor].nFreqMax - g_atMotorPara[eMotor].nFreqMin) / g_atMotorPara[eMotor].nFreqInc);     
	nAllow = (UINT32)( (nSteps / 2) / g_atMotorPara[eMotor].nFreqSam );
	// 2) select the minimum
	if(nBase >= nAllow)
	{
        nAcc = nAllow;
	}
	else
	{
        nAcc = nBase;
	}
	// 3) the steps of phase speed-up or phase speed-reduce
	nCount = (UINT32)( nAcc * g_atMotorPara[eMotor].nFreqSam );
   	// nAcc   = nCount; // attention: the value "nAcc" do not multiply by ".nFreqSam"
	// 4) the steps of phase speed-keep
   	nValue = (UINT32)( nCount * 2 );
	if(nValue > nSteps)
	{
	    nAcc = 0;
        nEqu = nSteps;
	}
	else
	{
	    // keep value "nAcc"
        nEqu = nSteps - nValue;
	}
	// 5) write to the FPGA, clear the buffer of the motor, and write new parameters
	//    attention: the precedence of following must demanded.
	// ->1. START ->stop // clear the buffers of last movement
	anBuffer[0] = (UINT16)( FPGA_WR_V_MOTOR_STOP & 0xFFFF);
	anBuffer[1] = (UINT16)((FPGA_WR_V_MOTOR_STOP >> 16) & 0xFFFF);
	// ->2. ST_ACC->nAcc
	anBuffer[2] = (UINT16)( nAcc & 0xFFFF);
	anBuffer[3] = (UINT16)((nAcc >> 16) & 0xFFFF);
	// ->3. ST_EQU->nEqu
	anBuffer[4] = (UINT16)( nEqu & 0xFFFF);
	anBuffer[5] = (UINT16)((nEqu >> 16) & 0xFFFF);
	// ->4. ST_DEC->nAcc
	anBuffer[6] = (UINT16)( nAcc & 0xFFFF);
	anBuffer[7] = (UINT16)((nAcc >> 16) & 0xFFFF);
	// address
    nAddr = (UINT32)FPGA_WR_MOTOR_REGISTER_BASE + 
		            (eMotor * FPGA_WR_MOTOR_PARA_OFFSET) + 
		            FPGA_WR_MOTOR_START;  // 4~11
    FPGA_WriteBuffer((UINT16*)anBuffer, nAddr, 8);
	
	// 6) write to the FPGA, generating a new movement
	// ->1. EN   ->unlock
    anBuffer[0] = (UINT16)( FPGA_WR_V_MOTOR_UNLOCK & 0xFFFF);
	anBuffer[1] = (UINT16)((FPGA_WR_V_MOTOR_UNLOCK >> 16) & 0xFFFF);
	// ->2. DIR  ->pos or neg
    if(e_Dir_Neg == eDir)
	{
        nDirMap = m_acnDirNeg[eMotor];
	}
	else
	{
        nDirMap = m_acnDirPos[eMotor];
	}
	anBuffer[2] = (UINT16)( nDirMap & 0xFFFF);
	anBuffer[3] = (UINT16)((nDirMap >> 16) & 0xFFFF);
	// ->3. START->start a movement immediately
    anBuffer[4] = (UINT16)( FPGA_WR_V_MOTOR_MOVE & 0xFFFF);
	anBuffer[5] = (UINT16)((FPGA_WR_V_MOTOR_MOVE >> 16) & 0xFFFF);
	// address
    nAddr = (UINT32)FPGA_WR_MOTOR_REGISTER_BASE + 
		            (eMotor * FPGA_WR_MOTOR_PARA_OFFSET) + 
		            FPGA_WR_MOTOR_EN;  // 0~5
    FPGA_WriteBuffer((UINT16*)anBuffer, nAddr, 6);

	return e_Feedback_Success;
}

//
UINT8  MV_Stop(enum eMvMotor eMotor)
{
    IO_ UINT32 IRAM_  nAddr  = 0; 
    IO_ UINT16 IRAM_  anBuffer[2]; 

	// range check 
    if(eMotor >= MV_MOTOR_NUM)
    {
        return e_Feedback_Error;
	}

    // write to the fpga
	// --->1. START->stop
    anBuffer[0] = (UINT16)( FPGA_WR_V_MOTOR_STOP & 0xFFFF);
	anBuffer[1] = (UINT16)((FPGA_WR_V_MOTOR_STOP >> 16) & 0xFFFF);
	// address
    nAddr = (UINT32)FPGA_WR_MOTOR_REGISTER_BASE + 
		            (eMotor * FPGA_WR_MOTOR_PARA_OFFSET) + 
		            FPGA_WR_MOTOR_START;  // 4~5
    FPGA_WriteBuffer((UINT16*)anBuffer, nAddr, 2);
    // --->2. EN   ->lock
	anBuffer[0] = (UINT16)( FPGA_WR_V_MOTOR_LOCK & 0xFFFF);
	anBuffer[1] = (UINT16)((FPGA_WR_V_MOTOR_LOCK >> 16) & 0xFFFF);
	// address
    nAddr = (UINT32)FPGA_WR_MOTOR_REGISTER_BASE + 
		            (eMotor * FPGA_WR_MOTOR_PARA_OFFSET) + 
		            FPGA_WR_MOTOR_EN;     // 0~1
    FPGA_WriteBuffer((UINT16*)anBuffer, nAddr, 2);
    g_atMotorStatus[eMotor].eFinish = e_True;
	//---------------------------------------
	// for testing
    // printf("%d - motor stop\r\n", eMotor);
	
    return e_Feedback_Success;
}

//
UINT8  MV_Wait(enum eMvMotor eMotor, UINT32 nTimeout, enum eFlag bFlag)
{
    IO_ UINT8  XRAM_ chReturn  = e_Feedback_Success;

    // range check 
    if(eMotor >= MV_MOTOR_NUM)
    {
        return e_Feedback_Error;
	}
	
    if(e_False == bFlag)
    {
        while( e_False == g_atMotorStatus[eMotor].eFinish); 
	}
	else
	{
	    IT_SYS_SetDlyMs(nTimeout);
        while( e_False == g_atMotorStatus[eMotor].eFinish)
        {
            if(e_True == IT_SYS_CheckDlyFin())
            {
                chReturn  = e_Feedback_Fail;
				MV_Stop(eMotor);
                break;
			}
		}
	}

	//------------------
	// stop
	// MV_Stop(eMotor);

	return chReturn;
}

//
UINT8  MV_GetStepsExecuted(enum eMvMotor eMotor, UINT32* pnSteps)
{
    IO_ UINT32 IRAM_  nAddr  = 0; 
    IO_ UINT16 IRAM_  anBuffer[2]; 

    // range check 
    if(eMotor >= MV_MOTOR_NUM)
    {
        *pnSteps = 0;
        return e_Feedback_Error;
	}
    // address, read-only
    nAddr = (UINT32)FPGA_RD_MOTOR_REGISTER_BASE + 
		            (eMotor * FPGA_RD_MOTOR_PARA_OFFSET);  // 2x ~ 2x+1 (x:0~4)
    // read the fpga
    FPGA_ReadBuffer((UINT16*)anBuffer, nAddr, 2);
	// 
	*pnSteps = (UINT32)( ((UINT32)anBuffer[1]) << 16) + anBuffer[0];

	return e_Feedback_Success;
}

// 
enum eFlag  MV_IsFinished(enum eMvMotor eMotor)
{
    // range check 
    if(eMotor >= MV_MOTOR_NUM)
    {
        return e_False;
	}
	
    return g_atMotorStatus[eMotor].eFinish;
}

// handle the status of the motors
UINT8  MV_IsrMoveHandling(void)
{
	IO_ UINT8  IRAM_  ch	  = 0;
	IO_ UINT32 IRAM_  nOffset = 0;
	//
	IO_ UINT32 IRAM_  nStatus = 0;	
	IO_ UINT32 IRAM_  nAddr   = 0; 
	IO_ UINT16 IRAM_  anBuffer[2]; 
	
	//------------------------------------
	// Movement finish, rising level		
	// address
	nAddr = (UINT32)FPGA_RD_MOTOR_REGISTER_BASE + 
					FPGA_RD_STATUS_00_01;	  // 38
	FPGA_ReadBuffer((UINT16*)anBuffer, nAddr, 1);
	//
	nStatus = anBuffer[0];
	//----------------------------------------------
	// for testing
	// printf("mt-status: 0x%0.4X \r\n", (int)nStatus);
	// 
	for(ch = 0; ch < MV_MOTOR_NUM; ch++)
	{
		// 
		nOffset = (UINT32)(1 << ch);  // get the mark	 
		//
		if(nOffset & nStatus)
		{
			//----------------------------------------------
			// for testing
			// printf("offset: %d \r\n", (int)nOffset);
			MV_Stop( (enum eMvMotor)ch );
			g_atMotorStatus[ch].eFinish  = e_True;		// attention: it must be set here		
			
		} // end of "if(nOffset & nStatus)"
		
	} // end of "for"
	
	// 
	return e_Feedback_Success;
}



#endif


