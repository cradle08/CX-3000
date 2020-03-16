// Move.h

#ifndef  __MOVE_H__
#define  __MOVE_H__


#include "MyType.h"


//-----------------------------------------------------------------------------------------
// definition

// 
//#define MV_MOTOR_NUM   2  // 

// home-OC-detect-level
//#define CO_HOME_MT_X   0
//#define CO_HOME_MT_Y   0    // leave the home-OC

// max-distance
#define MAX_DITANCE_MT_X    ((UINT32)1000000)
#define MAX_DITANCE_MT_Y    ((UINT32)1000000)
// direction-negative
#define DIR_NEG_MT_1     1
#define DIR_NEG_MT_2     1
#define DIR_NEG_MT_3     1
#define DIR_NEG_MT_4     1
// direction-position
#define DIR_POS_MT_1     0
#define DIR_POS_MT_2     0
#define DIR_POS_MT_3     0
#define DIR_POS_MT_4     0

//-----------------------------------------------------------------------------------------
// enumeration, status of the movement

// index
enum eMvMotor
{
    EN_Motor1  		= 0,   // 
    EN_Motor2  		= 1,   //
	EN_Motor3  		= 2,   //
	EN_Motor4 		= 3,   //
	EN_Motor_End 	= 4,  // add
};

enum eMotorPhase		  
{
    Motor_Phase_IDLE    = 0,          // idle
	Motor_Phase_LOAD    = 1,	      // load value the frist time
    Motor_Phase_ACC     = 2,          // speed up
	Motor_Phase_EQU     = 3, 	      // speed keep
    Motor_Phase_DEC     = 4,          // speed reduction
    Motor_Phase_FIN     = 5,          // finish
};



// status of the movement
struct tMvMotorStatus          
{
	IO_ enum eDirection    eDir;    		// dir 
	IO_ enum eFlag         eFinish; 		// finish flag
	IO_ UINT32             nSteps;  		// total step
	
	IO_ UINT8			   bAble;  			// ture: monitor oc change status or not
	IO_ UINT8			   nOCIndex;		// oc index(number)
	IO_ UINT8			   nOCStatus; 		// oc status

    IO_ UINT32   		   nCurFreq;     	// current frequence of the motor     
    IO_ UINT32   		   nTimeLoad;    	// the next value reloaded of the timer     
    
    IO_ UINT32   		   nFreqMin;	   // the frequence at the begining or ending  
    IO_ UINT32   		   nFreqMax;	   // the max frequence of the motor     
    IO_ UINT32   		   nFreqInc;	   // the increment of frequence changing 
    IO_ UINT32   		   nFreqSam;       // the width of a step when frequence changing

    IO_ UINT32   		   nStepsAcc;	   // steps, phase speed up 
    IO_ UINT32   		   nStepsEqu;      // steps, phase speed keep
	IO_ UINT32   		   nStepsDec;	   // steps, phase speed reduction
	IO_ UINT32	 		   nStepsExecuted; 	// steps had gone
    //IO_ UINT32   		   nStepsTotal;    // steps, total at the movement

    IO_ enum eMotorPhase   ePhase;         // phase
};


// moving parameters of the step-motor
struct tMvMotorPara            
{
    IO_ UINT32   nFreqMin;	   // the frequence at the begining or ending  
    IO_ UINT32   nFreqMax;	   // the max frequence of the motor     
    IO_ UINT32   nFreqInc;	   // the increment of frequence changing 
    IO_ UINT32   nFreqSam;     // the width of a step when frequence changing
};


_EXT_ IO_ struct tMvMotorStatus XRAM_ g_atMotorStatus[EN_Motor_End]; // status
_EXT_ IO_ struct tMvMotorPara   XRAM_ g_atMotorPara[EN_Motor_End];   // paramenters


//-------------------------------------------------------------- CX2000_C API START --------------------
// function declaration, using the timer's interrupt

_EXT_ UINT8  MV_InitPara_V2(enum eMvMotor eMotor, 
                              UINT32 nFreqMin, 
                              UINT32 nFreqMax, 
                              UINT32 nFreqInc, 
                              UINT32 nFreqSam );

_EXT_ UINT8  MV_Move_V2(enum eMvMotor eMotor, UINT32 nSteps, enum eDirection eDir);
UINT8  MV_Stop_V2(enum eMvMotor eMotor);
_EXT_ UINT8  MV_Wait_V2(enum eMvMotor eMotor, UINT32 nTimeout, enum eFlag bFlag);
_EXT_ UINT8  MV_GetStepsExecuted_V2(enum eMvMotor eMotor, UINT32* pnSteps);
_EXT_ enum eFlag  MV_IsFinished_V2(enum eMvMotor eMotor); // not need at CX2000_C API


//--------------------------------------------------------------- CX2000_C API END ----------------------



//-------------------------------------------------------------- CX2000_C API START --------------------
// function declaration, using the timer's interrupt

void MTx_PWM_ISR(enum eMvMotor eMotor);

_EXT_ UINT8  MV_InitPara_V3(enum eMvMotor eMotor, 
                              UINT32 nFreqMin, 
                              UINT32 nFreqMax, 
                              UINT32 nFreqInc, 
                              UINT32 nFreqSam );

_EXT_ UINT8  MV_Move_V3(enum eMvMotor eMotor, UINT32 nSteps, enum eDirection eDir);
UINT8  MV_Stop_V3(enum eMvMotor eMotor);
_EXT_ UINT8  MV_Wait_V3(enum eMvMotor eMotor, UINT32 nTimeout, enum eFlag bFlag);
_EXT_ UINT8  MV_GetStepsExecuted_V3(enum eMvMotor eMotor, UINT32* pnSteps);
_EXT_ enum eFlag  MV_IsFinished_V3(enum eMvMotor eMotor); // not need at CX2000_C API

//--------------------------------------------------------------- CX2000_C API END ----------------------




// initialize the moving parameters of the motor driver
_EXT_ UINT8  MV_InitPara(enum eMvMotor eMotor, 
                              UINT32 nFreqMin, 
                              UINT32 nFreqMax, 
                              UINT32 nFreqInc, 
                              UINT32 nFreqSam );
// start a movement, according the moving parameters set before
_EXT_ UINT8  MV_Move(enum eMvMotor eMotor, UINT32 nSteps, enum eDirection eDir);
// stop the movemnet immediately
_EXT_ UINT8  MV_Stop(enum eMvMotor eMotor);
// waiting the movement until it has finished
_EXT_ UINT8  MV_Wait(enum eMvMotor eMotor, UINT32 nTimeout, enum eFlag bFlag);
// get the steps has executed at any time 
_EXT_ UINT8  MV_GetStepsExecuted(enum eMvMotor eMotor, UINT32* pnSteps);
// get the moving status
_EXT_ enum eFlag  MV_IsFinished(enum eMvMotor eMotor);
// handle the status of the motors
_EXT_ UINT8  MV_IsrMoveHandling(void);


/******************************************** Driver IF Form CX3000 ************************************************/
UINT8  MTx_DriverEnable(enum eMvMotor eMotor, enum eFlag bAble);
// 
void MV_InitMotorTimer(enum eMvMotor eMotor);

// initialize the moving parameters of the motor driver
_EXT_ UINT8  MV_InitPara(enum eMvMotor eMotor, 
                              UINT32 nFreqMin, 
                              UINT32 nFreqMax, 
                              UINT32 nFreqInc, 
                              UINT32 nFreqSam );
// start a movement, according the moving parameters set before
_EXT_ UINT8  MV_Move(enum eMvMotor eMotor, UINT32 nSteps, enum eDirection eDir);
// stop the movemnet immediately
_EXT_ UINT8  MV_Stop(enum eMvMotor eMotor);
// waiting the movement until it has finished
_EXT_ UINT8  MV_Wait(enum eMvMotor eMotor, UINT32 nTimeout, enum eFlag bFlag);
// get the steps has executed at any time 
_EXT_ UINT8  MV_GetStepsExecuted(enum eMvMotor eMotor, UINT32* pnSteps);
// get the moving status
_EXT_ enum eFlag  MV_IsFinished(enum eMvMotor eMotor);
// handle the status of the motors
_EXT_ UINT8  MV_IsrMoveHandling(void);









#endif
