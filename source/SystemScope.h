// SystemScope.h

#ifndef    __SYSTEM_SCOPE_H__
#define    __SYSTEM_SCOPE_H__


#include  "MyType.h"
#include "Scope_Move.h"

//-----------------------------------------------------------------------------------------

//yaolan_start
enum  eAxisPos
{
	E_AXIS_POS_HOME = 0,
	E_AXIS_POS_UNSURE = 1,
};  

//yaolan_end


#define ERR_PARA_NO_CONFIG                      0

// 
#define  ERR_COMMAND_NO_VALID                   1

// 
#define  ERR_AXIS_X_NO_RESET                    2
// 
#define  ERR_AXIS_X_HOME_FAIL                   3
// 
#define  ERR_AXIS_X_MOVEMENT_OVERTIME           4
// 
#define  ERR_AXIS_X_OVER_DISTANCE               5

#define  ERR_AXIS_M_OVER_DISTANCE              18

// 
#define  ERR_AXIS_ALL_MOVEMENT_OVERTIME        19

// 
#define  ERR_AXIS_AT_NO_RESET                  20
// 
#define  ERR_AXIS_AT_HOME_FAIL                 21



//-----------------------------------------------------------------------------------------
// 

#define INTERVAL_OPPO_X          200
#define INTERVAL_OPPO_Y          300
#define INTERVAL_OPPO_Z           30
#define INTERVAL_OPPO_M            0


//-----------------------------------------------------------------------------------------
// 

struct tOppoInterval
{
    __IO enum eDirection   eDir;
	__IO UINT16             nSteps;    
};

//
enum eAxisXPos
{
    E_AXIS_X_POS_UNSURE   = 0,
	E_AXIS_X_POS_HOME     = 1,
	E_AXIS_X_POS_CTRL     = 2,	
};

// 
enum eAxisYPos
{
    E_AXIS_Y_POS_UNSURE   = 0,
	E_AXIS_Y_POS_HOME     = 1,
	E_AXIS_Y_POS_CTRL     = 2,
		
};

// 
enum eAxisZPos
{
    E_AXIS_Z_POS_UNSURE  = 0,
	E_AXIS_Z_POS_HOME    = 1,
	E_AXIS_Z_POS_CTRL    = 2,
		
};

// 
enum eAxisMPos
{
    E_AXIS_M_POS_UNSURE   = 0,
	E_AXIS_M_POS_1        = 1,    // Î
	E_AXIS_M_POS_2        = 2, 
	E_AXIS_M_POS_3        = 3,
	E_AXIS_M_POS_4        = 4,
	E_AXIS_M_POS_HOME     = 5, 
	E_AXIS_M_POS_CTRL     = 6,
		
};
// 
enum eAxisATPos
{
    E_AXIS_AT_POS_UNSURE  = 0,
	E_AXIS_AT_POS_HOME    = 1,
	E_AXIS_AT_POS_CTRL    = 2,
};

// 
struct tAxisPosStatus
{
    enum eAxisXPos  eAxisX;
	enum eAxisYPos  eAxisY;	
    enum eAxisZPos  eAxisZ;
    enum eAxisMPos  eAxisM;
	enum eAxisATPos eAxisAT;

	UINT32 nAxisX;
    UINT32 nAxisY;
	UINT32 nAxisZ;
	UINT32 nAxisM;
	UINT32 nAxisAT;
};

// 
struct tOCLevel
{
    enum eLevel  eHomeAxisX;   // 
	enum eLevel  eHomeAxisY;
    enum eLevel  eHomeAxisZ;
	enum eLevel  eHomeAxisM;

};

// 
struct tSystemStatus
{
    enum eFlag eFlagSysParaLoad;                    // 
    enum eFlag eaFlagMotorParaLoad[EN_Motor_End];   // 
    
};

// 
struct tSyetemPara  // 54
{
    UINT8 chAxisXHome_d;       // 
	UINT8 chAxisXHome_3;
	UINT8 chAxisXHome_2;
	UINT8 chAxisXHome_1;
	UINT8 chAxisXHome_0;
	
	UINT8 chAxisYHome_d;       // 
	UINT8 chAxisYHome_3;
	UINT8 chAxisYHome_2;
	UINT8 chAxisYHome_1;
	UINT8 chAxisYHome_0;
	
	UINT8 chAxisZHome_d;       // 
	UINT8 chAxisZHome_3;
	UINT8 chAxisZHome_2;
	UINT8 chAxisZHome_1;
	UINT8 chAxisZHome_0;
	
	UINT8 chAxisMHome_d;       // 
	UINT8 chAxisMHome_3;
	UINT8 chAxisMHome_2;
	UINT8 chAxisMHome_1;
	UINT8 chAxisMHome_0;

    UINT8 chAxisXPosMax_3;     // 
	UINT8 chAxisXPosMax_2;
	UINT8 chAxisXPosMax_1;
	UINT8 chAxisXPosMax_0;

	UINT8 chAxisYPosMax_3;       // 
	UINT8 chAxisYPosMax_2;
	UINT8 chAxisYPosMax_1;
	UINT8 chAxisYPosMax_0;

	UINT8 chAxisZPosMax_3;       // 
	UINT8 chAxisZPosMax_2;
	UINT8 chAxisZPosMax_1;
	UINT8 chAxisZPosMax_0;

	UINT8 chAxisMPosMax_3;       // 
	UINT8 chAxisMPosMax_2;
	UINT8 chAxisMPosMax_1;
	UINT8 chAxisMPosMax_0;
	   
	UINT8 chLumGrade;        //  

	UINT8 chValX_h;          // 
	UINT8 chValX_l;
	UINT8 chValY_h;
	UINT8 chValY_l;
	UINT8 chValZ_h;
	UINT8 chValZ_l;
	UINT8 chValM_h;
	UINT8 chValM_l;

	UINT8 chAxisATHome_d;   // 
	UINT8 chAxisATHome_3;
	UINT8 chAxisATHome_2;
	UINT8 chAxisATHome_1;
	UINT8 chAxisATHome_0;

	UINT8 chAxisATPosMax_3;  // 
	UINT8 chAxisATPosMax_2;
	UINT8 chAxisATPosMax_1;
	UINT8 chAxisATPosMax_0;
};


//-----------------------------------------------------------------------------------------
// 

// 
_EXT_ struct tAxisPosStatus  g_tAxisPosStatus;      // 
_EXT_ struct tOCLevel        g_tOCLevel;            // 
_EXT_ struct tSystemStatus   g_tSystemStatus;       // 
_EXT_ struct tSyetemPara     g_tSystemPara;         // 

// 
_EXT_ struct tOppoInterval   g_atInterval[EN_Motor_End];  // 

// 
_EXT_ UINT32  g_nError;            // 
// _EXT_ UINT32  g_nFlag;             // 
_EXT_ UINT16  g_nErrorParaSet[32]; // (SYS_ErrorMark())



//-----------------------------------------------------------------------------------------
// 

#if 0
_EXT_ UINT8 SYS_GetBuildVersion(void);

_EXT_ UINT8 SYS_GetBuildTime(void);
#endif

void Iint_SysParam(void);

_EXT_ UINT8 SYS_InitSystem(void);            // 

_EXT_ UINT8 SYS_ResetSystem(void);           // 

_EXT_ UINT8 SYS_ErrorAnalyze(UINT8* pchSaveBuf, UINT16* pnErrorNum);  // 

_EXT_ UINT8 SYS_ErrorMark(UINT8 chErrorIndex, UINT16 nParaValue);     // 

#if 0
_EXT_ UINT8 SYS_ErrorClear(UINT8 chErrorIndex);                       // 

_EXT_ UINT8 SYS_FlagMark(UINT8 chFlagIndex);                          // 

_EXT_ UINT8 SYS_FlagClear(UINT8 chFlagIndex);                         // 
#endif

_EXT_ enum eFlag SYS_HasError(void);


#endif

