// ProcessScope.h

#ifndef    __PROCESS_SCOPE_H__
#define    __PROCESS_SCOPE_H__

#include  "MyType.h"
#include "InterfaceScope.h"

//----- control --------------------------------------
#define    CMD_CTRL_VALVE         0x00000000
#define    CMD_CTRL_PUMP          0x00000001
#define    CMD_CTRL_LED          0x00000002

#define    CMD_CTRL_MOT_IN        0x00000100
#define    CMD_CTRL_MOT_OUT       0x00000101
#define    CMD_CTRL_MOT_LOCK      0x00000102
#define    CMD_CTRL_MOT_RELEASE   0x00000103
#define    CMD_CTRL_MOT_IN_ONLY   0x00000104
#define    CMD_CTRL_MOT_OUT_ONLY  0x00000105
#define    CMD_CTRL_MOT_X_IN_ADD  0x00000106 
#define    CMD_CTRL_REGISTER      0x00000200
//#define    CMD_CTRL_WBC_SWITCH    0x00000300
#define    CMD_CTRL_WBC_ENABLE    0x00000300
#define    CMD_CTRL_WBC_PARA      0x00000400
#define    CMD_CTRL_DEBUG_WBC     0x00000500
#define    CMD_CTRL_TEST_WBC      0x00000501
#define    CMD_CTRL_TEST_RBC      0x00000502
#define    CMD_CTRL_TEST_PLT      0x00000503
#define    CMD_CTRL_TEST_RBC_PLT  0x00000504  // test cmd
#define    CMD_CTRL_TEST_HGB  	  0x00000505
#define    CMD_CTRL_TEST_CRP      0x00000506

#define    CMD_CTRL_PRESS_CONFIG  		0x00000600 
#define    CMD_CTRL_PRESS_ADD     		0x00000601 
#define    CMD_CTRL_MOT_OUT_CHECK 		0x00000700 
#define    CMD_CTRL_MOT_IN_CHECK  		0x00000701 // yaolan_20190220
#define    CMD_CTRL_WBC_48V_CHECK 		0x00000702 
#define    CMD_CTRL_BUILD_PRESS_CHECK   0x00000703 
#define    CMD_CTRL_AIRLIGHT_CHECK 		0x00000704 
#define    CMD_CTRL_GPUMP_CHECK    		0x00000705 
#define    CMD_CTRL_VALVE1_CHECK   		0x00000706 
#define    CMD_CTRL_VALVE2_CHECK   		0x00000707 
#define    CMD_CTRL_PART_TEST   		0x00000708
#define    CMD_CTRL_NET_TEST   		    0x00000808
// debug cmd
#define    CMD_CTRL_DEBUG_GET_PRESS 	0x00000709 // get the press value

//-----status--------------------------------------------------
#define    CMD_STATUS_PRESSURE    		0x00000000
#define    CMD_STATUS_ELECTRODE   		0x00000001
#define    CMD_STATUS_OC          		0x00000002
#define    CMD_STATUS_EDTION      		0x00000003
#define    CMD_STATUS_MOT         		0x00000004
#define    CMD_STATUS_MOT_WORK    		0x00000005
#define    CMD_STATUS_PUMP_SPEED  		0x00000006
#define    CMD_STATUS_MOTO_X_IN_ADD		0x00000007
#define    CMD_STATUS_WBC_SET    		0x00000010
#define    CMD_STATUS_MOT_OUT_CHECK  	0x01000000
#define    CMD_STATUS_MOT_IN_CHECK   	0x01000001
#define    CMD_STATUS_WBC_48V        	0x01000002
#define    CMD_STATUS_BUILD_PRESS    	0x01000003
#define    CMD_STATUS_AIRLIGHT_PRESS 	0x01000004
#define    CMD_STATUS_PART_TEST      	0x01000708
#define    CMD_STATUS_PRESS_DATA     	0x00000100
#define    CMD_STATUS_PRESS_ADD      	0x00000101

//----query--------------------------------------------------
#define    CMD_QUERY_PRESSURE     		0x00000000
#define    CMD_QUERY_ELECTRODE    		0x00000001
#define    CMD_QUERY_OC           		0x00000002
#define    CMD_QUERY_EDTION       		0x00000003
#define    CMD_QUERY_MOT_STAT     		0x00000004
#define    CMD_QUERY_PUMP_SPEED   		0x00000006
#define    CMD_QUERY_MOTO_IN_X_ADD  	0x00000007
#define    CMD_QUERY_WBC_VALUE      	0x00000008
#define    CMD_QUERY_PRESS_DATA   		0x00000100
#define    CMD_QUERY_PRESS_ADD   		0x00000101
#define    CMD_QUERY_AIRLIGHT_RESULT   	0x20000102

//-----data----------------------------------------------------- 
#define    CMD_DATA_MOTO_IN_X_ADD   	0x30000007
#define    CMD_DATA_WBC_VALUE       	0x30000008
#define    CMD_DATA_PRESS_ADD       	0x30000101
#define    CMD_DATA_ORG           		0x00000000
#define    CMD_DATA_ALY          		0x00000001
#define    CMD_DATA_WAVE         		0x00000002
#define    CMD_DATA_AIRLIGHT_RESULT		0x30000102


#define  COLLECT_RET_SUCESS            	0x0000      /* 采集成功 */
#define  COLLECT_RET_FAIL_TIMEOVER     	0x0001      /* 因超时导致采集失败 */
#define  COLLECT_RET_FAIL_PUMP         	0x0002      /* 因负压组件问题导致采集失败 */
#define  COLLECT_RET_FAIL_ELECTRODE    	0x0003      /* 因定量电极组件问题导致采集失败 */
#define  COLLECT_RET_FAIL_CHIP         	0x0004      /* 因未检测到待检芯片导致采集失败 */
#define  COLLECT_RET_FAIL_SAMPLE       	0x0005      /* 因芯片仓行程受阻导致采集失败 */
#define  COLLECT_RET_FAIL_NONE_HOME    	0x0006      /* 因未进仓导致采集失败 */
#define  COLLECT_RET_FAIL_AIR_COKE     	0x0007      /* 气嘴漏气 */  // yaolan_20190409
#define  COLLECT_RET_FAIL_WBC_ELECTRODE	0x0008	   /* 检测电极异常 */ // yaolan_20190409
#define  COLLECT_RET_FAIL_WBC_TOUCH  	0x0009	   /* 检测电极异常 */ // yaolan_20190605
#define  COLLECT_RET_FAIL_WBC_BSK	  	0x000a	   /* 计数池宝石孔异常*/ // yaolan_20190605
#define  COLLECT_RET_FAIL_OTHER        	0x0050      /* 因其他异常导致采集失败 */
#define  COLLECT_RET_FAIL_HGB_V			0x000b      // the HGB current v is low
#define  COLLECT_RET_FAIL_CRP_V			0x000b      // the CRP current v is low
//#define  COLLECT_RET_SUCCESS_AIRLIGHT     0x1001      /* 密闭性好 */  // yaolan

#define PUMP_SELF_CHECK_TIME           5000

//-------------------------------------------------------------------------------------------
//yaolan_start
#define ADC_PRESS_INDEX       	0
#define ADC_ELEC_INDEX        	1
#define ADC_TEMP_INDEX     	  	2

#define ADC_V_XK_INDEX        	0
#define ADC_V_CUR_INDEX   	  	1
#define ADC_V_LED_CRP_INDEX   	2
#define ADC_V_LED_HGB_INDEX   	3

#define LED_HGB_INDEX   	  	0
#define LED_CRP_INDEX   	  	1
#define LED_STATUS_OPEN   	  	1
#define LED_STATUS_CLOSE 	  	0
#define HGB_BLACK_VALUE			0x1111

#define CRP_TOTAL_DATA_NUM		1200		//at 120s, per 0.1s get one data,total num is 120*10=1200
#define DATA_FRAME_NUM_4BYTE	128			// send data number a time
#define DATA_FRAME_NUM_2BYTE	256			// send data number a time
#define DATA_FRAME_NUM_1BYTE	512			// send data number a time

#define ADC_V_REF_VALUE_3_3	  	3300   		// 3300mv=3.3v
#define ADC_V_REF_VALUE_48	  	48000  		// 48000mv=48v
#define ADC_V_REF_VALUE_5	  	5000	 	// 5000mv=5v
#define ADC_RESOLUTION_12     	4095		// 12b adc resolution
#define ADC_RESOLUTION_24     	16777215	// 24b adc resolution
#define ELEC_TRIGGER_THRESHOLD  3722  		// 3/3.3*4095 = 3722(3v)

#define HGB_MIX_THRESHOLD_V		4500		// 4500mv
#define CRP_MIX_THRESHOLD_V		4500		// 4500mv
#define ELEC_STATUS_OPEN     1
#define ELEC_STATUS_CLOSE    0



struct CRP_DataType{
	UINT8 eEnable;
	UINT8 eSend;
	UINT16 nTotal;
	UINT16 nIndex;
	UINT16 crpBuffer[DATA_FRAME_NUM_2BYTE*2]; // 256*2*2=1024
};
extern IO_ struct CRP_DataType g_CRP_Data;

//yaolan_end
//----------------------------------------------------------------------------------------------



#define  TIMING_1    (50)
#define  TIMING_2    (TIMING_1 + 8000)
#define  TIMING_3    (TIMING_2 + 1000)
#define  TIMING_4    (TIMING_3 + 15000)     // 15000
#define  TIMING_5    (TIMING_4 + 3000)
#define  TIMING_6    (TIMING_5 + 50)
#define  TIMING_7    (TIMING_6 + 5000)

//-----------

#define  INDEX_ELECTRODE        1    /* 电极检测通道 */
#define  INDEX_VALVE_PUMP       0    /* 泵气阀阀通道 */
#define  INDEX_VALVE_WBC        1    /* WBC气阀通道 */

//-----------//
#ifdef  SAMPLE_CHIP_70UM
//#define  ELECTRODE_WASTE               0                   /* 检测池溢出即退出检测流程 */
#define  PRESS_BUILD                   350000000   /* 35kPa 适合检测的气压 */
//define  PUMP_PRESS_OFF                0                   /* 气压泵停止工作 */
//#define  PUMP_PRESS_FREQ               10000               /* 气压泵驱动频率 */

#define  TIME_OVER_TS_BUILD_PRESS      20000       /* 10秒  负压泵建立负压超时时间 */
#ifdef   DEBUG_TEST
#define  TIME_OVER_TS_ADC              35000       /* 35秒  临时调试用 */
#else
#define  TIME_OVER_TS_ADC              40000       /* 40秒  数据采集超时时间 */
#endif
#define  TIME_TS_ACTION_OFF            12000       /* 12秒   检测期间关阀时间 */
#define  TIME_TS_ACTION_ON             15000       /* 15秒  检测期间开阀时间 */
//-----------//
#elif defined SAMPLE_CHIP_95UM
//-----------//
#define  ELECTRODE_WASTE               0                   /* 检测池溢出即退出检测流程 */
#define  PUMP_PRESS_OFF                0                   /* 气压泵停止工作 */
#define  PUMP_PRESS_FREQ               10000 //21000               /* 气压泵驱动频率 */

#ifdef   DEBUG_TEST
//#define  PRESS_BUILD                   600000000   /* 60kPa 适合检测的气压 */
//#define  TIME_OVER_TS_BUILD_PRESS      60000       /* 60秒  负压泵建立负压超时时间 */
//#define  TIME_OVER_TS_ADC              20000       /* 15秒  临时调试用 */
#else
#define  PRESS_BUILD                   230000000   /* 30kPa->22kPa 适合检测的气压 */
#define  TIME_OVER_TS_BUILD_PRESS      15000       /* 10秒  负压泵建立负压超时时间 */
#define  TIME_OVER_TS_ADC              25000       /* 25秒  数据采集超时时间 */
#endif

#define  TIME_TS_ACTION_TIMEOUT        24000 //18000
#define  TIME_TS_ACTION_OFF            4500 //4000        /* 4秒   检测期间关阀时间 */
#define  TIME_TS_ACTION_ON             7000        /* 7秒   检测期间开阀时间 */
#endif

//-----------------------------------------------------------------------
#define BUILD_PRESS_MIN                225000000 //21.5kpa
#define BUILD_PRESS_RIGHT              PRESS_BUILD // (UINT32)220000000 //22kpa
#define BUILD_PRESS_MAX                235000000  //22.5kpa
#define BUILD_PRESS_DEVIATION          20000000   //2kpa
#define COUNT_MIN_PRESS                140000000  //15kpa
#define TIME_AIRLIGHT_CHECK            20000       /* 20秒  密闭性检测延时时间 */
#define GET_PRESS_NUM_FIVE			   5
#define GET_PRESS_NUM_THREE			   3
//-----------//

#define COUNT_WBC_XK_CHECK_V   	   	   1500    //1.5v xiao kong 
#define COUNT_WBC_TOUCH_CHECK_V   	   2000    //1.8v
#define COUNT_WBC_START_V   		   2300    //2.3v
#define PRESS_PRECISION_FACTOR         (0xF4240)  //1000000

#define PART_TEST_CHECK_DELAY			200 //ms

enum {
	MOTO_OUT_IN_TEST    	= 0, // only test moto out and in  at the normal count presss
	WBC_ELEC_TEST       	= 1, // only test wbc elec function  at the normal count presss
	CHECK_ELEC_TEST      	= 2, // only test check elec function  at the normal count presss
	AIRFAUCET_TEST       	= 3,  // only test airlight at the normal count presss
	WBC_AND_CHECK_ELEC_TEST = 4, // wbc elec and check elec test together
	VALVE_TEST 				= 5  // valve test
};

typedef enum {
	EN_WBC_V_LOW  = 0,
	EN_WBC_V_HIGH = 1
} EN_WBC_V_STA;

_EXT_ IO_ UINT32 g_Udp_Count, g_Frame_Count, g_Send_Fail;
_EXT_ IO_ UINT8 g_AirLight_Flag;

UINT32 Get_Udp_Count(void);
void Reset_Udp_Count(UINT32 nVal);
void Add_Udp_Count(void);




#define DEBUG_INFO_UP_LOAD
#ifdef  DEBUG_INFO_UP_LOAD
#define DEBUG_INFO_UP_LEN    1500
#define DEBUG_INFO_TEMP_LEN  150
void Append_Debug_Info(INT8 *pInfo, INT8 *pTemp, UINT16 *pInfoLen);
#endif



//	_STA_ IO_ UINT16 XRAM_ s_sDInfo[DEBUG_INFO_UP_LEN];
//	_STA_ IO_ UINT16 XRAM_ s_nDILen = 0;
//-----------------------------------------------------------------------------------------

// yaolan_start
UINT8 HGB_Test_Exec(void);
UINT8 CRP_Test_Exec(void);
UINT8 LED_Test_Exec(UINT8 Index, UINT8 nFlag);

//yaolan_end


_EXT_ UINT8 MSG_Handling(UINT8* pchCmdBuf, UINT8* pchFbkBuf);
_EXT_ UINT8 MSG_Handling_MsgHandle(UINT8* pchCmdBuf, UINT8* pchFbkBuf);

/* 采集报告处理 */
void collect_return_hdl(UINT16 stat);
/* 报告状态结果数据 */
void Msg_Return_Handle_8(EN_MSG_TYPE eType, UINT32 nCmd, INT8 nResult);
void Msg_Return_Handle_16(EN_MSG_TYPE eType, UINT32 nCmd, INT16 nResult);
void Msg_Return_Handle_32(EN_MSG_TYPE eType, UINT32 nCmd, INT32 nResult);
void Msg_Return_Handle_String(EN_MSG_TYPE eType, UINT32 nCmd, UINT8 *pRst, UINT8 nLen);
////
//void Status_Return_Handle_8(UINT32 nCmd, UINT8 nResult);
//void Status_Return_Handle_16(UINT32 nCmd, UINT16 nResult);
//void Status_Return_Handle_32(UINT32 nCmd, UINT32 nResult);
//// return query data
//void Data_Return_Handle_8(UINT32 nCmd, UINT8 nResult);
//void Data_Return_Handle_16(UINT32 nCmd, UINT16 nResult);
//void Data_Return_Handle_32(UINT32 nCmd, UINT32 nResult);

/* 汇报电机执行过程 */
void moto_work_stat(UINT8 mot_num, MOTO_WORK_STAT_E stat);
void moto_work_stat_2(UINT8 mot_num, MOTO_WORK_STAT_E stat, BUILD_PRESS_E stat2);
/* 上报电机进出仓自检时间 */
void Moto_Work_Time(MOTO_WORK_DIR eDir, UINT16 nStatus);

//-----------------------------------------------------------------------------------------
// yaolan_20190220
_EXT_ UINT8 MT_X_IN_Self_Check(CALL_STYLE_E eCall);
_EXT_ UINT8 MT_X_OUT_Self_Check(CALL_STYLE_E eCall);
_EXT_ UINT8 MT_Y_Home_Self_Check(void);
_EXT_ UINT16 Get_XK_V_Value(void);
_EXT_ UINT32 WBC_48V_Self_Check(void);
UINT8 MT_Y_MoveToPosRel_Self_Check(void); 
_EXT_ UINT8 Negative_Pressure_Self_Check(void);
_EXT_ UINT8 Valve1_Self_Check(void);
_EXT_ UINT8 Valve2_Self_Check(void);
_EXT_ UINT8 Pump_Self_Check(void);
//_EXT_ UINT8 Valve_Self_Check(void);
INT32 Build_Press_Self_Check(void);
UINT8 AirLight_Self_Check(CALL_STYLE_E eCall);
//
UINT8 Get_WBC_V_Status(UINT32 nV);
void Return_Press_Value(void);
INT32 Get_Press_Value(UINT8 nNum);

_EXT_ UINT8 MSG_Testing(void);
//
#ifdef DEBUG_INFO_UP_LOAD
_EXT_ UINT8 MSG_TestingFunc(UINT8 *pDInfo, UINT16 *pDILen, UINT8 nChannel);
#else
_EXT_ UINT8 MSG_TestingFunc(void);
#endif

void Part_Test_Exec(UINT8 nNo, UINT32 nNum);
void Part_Test_Moto_X(UINT8 nNo, UINT32 nNum);
void Part_Test_WBC_Elec(UINT8 nNo, UINT32 nNum);
void Part_Test_AirFaucet(UINT8 nNo, UINT32 nNum);

void Send_Packets_Test(UINT16 Time, UINT32 Num); // at time send num packet test

#endif



