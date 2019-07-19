// Move.h

#ifndef  __MOVE_H__
#define  __MOVE_H__


#include "MyType.h"


//-----------------------------------------------------------------------------------------
// definition

// 
#define MV_MOTOR_NUM   2  // 

// home-OC-detect-level
#define CO_HOME_MT_X   0
#define CO_HOME_MT_Y   0    // leave the home-OC
// max-distance
#define MAX_DITANCE_MT_X    ((UINT32)1000000)
#define MAX_DITANCE_MT_Y    ((UINT32)1000000)
// direction-negative
#define DIR_NEG_MT_X     0
#define DIR_NEG_MT_Y     1
// direction-position
#define DIR_POS_MT_X     1
#define DIR_POS_MT_Y     0


//-----------------------------------------------------------------------------------------
// enumeration, status of the movement

// index
enum eMvMotor
{
    Motor_X  = 0,   // 
    Motor_Y  = 1,   //  
};


//-----------------------------------------------------------------------------------------
// struct, attention: the alignment of members in the struct

// status of the movement
struct tMvMotorStatus          
{
    IO_ enum eDirection  eDir;    // 
	IO_ enum eFlag       eFinish; // 
	IO_ UINT32           nSteps;  // 	
};

// moving parameters of the step-motor
struct tMvMotorPara            
{
    IO_ UINT32   nFreqMin;	   // the frequence at the begining or ending  
    IO_ UINT32   nFreqMax;	   // the max frequence of the motor     
    IO_ UINT32   nFreqInc;	   // the increment of frequence changing 
    IO_ UINT32   nFreqSam;     // the width of a step when frequence changing
};

//-----------------------------------------------------------------------------------------
// global variable declaration

_EXT_ IO_ struct tMvMotorStatus XRAM_ g_atMotorStatus[MV_MOTOR_NUM]; // status
_EXT_ IO_ struct tMvMotorPara   XRAM_ g_atMotorPara[MV_MOTOR_NUM];   // paramenters


//-----------------------------------------------------------------------------------------
// function declaration

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


