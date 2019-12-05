// InterfaceScope.h

#ifndef    __INTERFACE_SCOPE_H__
#define    __INTERFACE_SCOPE_H__

#include  "MyType.h"
#include  "ProcessScope.h"
//-----------------------------------------------------------------------------------------
// 宏定义

//yaolan_start

#define  OC_AXIS_POS_INDEX	 0
//enum ADC_Channel
//{
//	EN_Data_CHANNEL_WBC 		= 1,
//	EN_Data_CHANNEL_RBC 		= 2,
//	EN_Data_CHANNEL_PLT 		= 3,
//	EN_Data_CHANNEL_RBC_PLT     = 4,
//	EN_Data_CHANNEL_END 		= 5,
//};

//yaolan_end




#define  OC_HOME_CHANNEL          0          /* 进仓到位光耦通道号 */
#define  OC_OUT_CHANNEL           1          /* 出仓到位光耦通道号 */
#define  OC_SAMPLE_RELEA_CHANNEL  2          /* 样本芯片释放光耦通道号 */
#define  OC_SAMPLE_HOLD_CHANNEL   3          /* 样本芯片释放光耦通道号(暂时未实现) */
#define  INDEX_PRESS              3          /* 气压检测ADC通道 ,at FPGA is 4*/
#define  INDEX_ADC_48V            1          /* adc, 0=HGB,1=WBC vol value, 2=RBC(wbc backup), 3=press, */ 
//------------------------------
// 
#define AXIS_X_SPEED_HOME    100000
#define AXIS_X_SPEED_TOP     120000
//
#define AXIS_Y_SPEED_HOME    100000
#define AXIS_Y_SPEED_TOP     120000
//

//------------------------------

// 显微镜的镜头数
#define SCOPE_POS_NUM  3

// 灯光的开关宏定义
//#define LAMP_ON()    {EVAL_DP_PwrOn(e_True);}
//#define LAMP_OFF()   {EVAL_DP_PwrOn(e_False);}

//------------------------------
// 编码器的校检次数
#define CORRECT_TIME_X  0
#define CORRECT_TIME_Y  0
#define CORRECT_TIME_Z  0
#define CORRECT_TIME_M  0

//------------------------------
// definition
// temperature's range, X 100 
//
#define TEMP_VALUE_MIN           500   //  5℃ * 100
#define TEMP_VALUE_MAX           8000  // 80℃ * 100
//
#define TEMP_ERR_AUTO_TIMES      10    // before shut down 

//#define EN_VALVE_AIR 		 0  // yaolan_20190220
//#define EN_VALVE_LIQUID     		 1

#define MOTO_SELF_CHECK_TIMEOUT   10000


//-----------------------------------------------------------------------------------------
// data type

// heating type
enum eHeatType
{
    e_HT_OFF       = 0,
	e_HT_HEAT_03   = 1,
	e_HT_HEAT_06   = 2,
	e_HT_HEAT_09   = 3,
	e_HT_HEAT_12   = 4,
	e_HT_HEAT_15   = 5,
	e_HT_HEAT_18   = 6,
	e_HT_HEAT_20   = 7,
	e_HT_HEAT_24   = 8,
};

struct tTempKeep
{
    IO_ enum eFlag  eHold;
	IO_ UINT16      nTemp; 
};


typedef enum{
	// get or clear for HW_DATA_GetData
	EN_GET_FPGA_DATA   = 0,
	EN_CLEAR_FPGA_DATA = 1,
	// send or drop flag for HW_LWIP_Working
	EN_SEND_FPGA_DATA  = 2,
	EN_DROP_FPGA_DATA  = 3
}EN_FPGA_DATA_FLAG;


//-----------------------------------------------------------------------------------------
// yaolan_start

// led
UINT8  HW_LED_On(UINT8 index);
UINT8  HW_LED_Off(UINT8 index);

// perpher adc
UINT16 HW_Get_ADC_Perip(UINT8 Index);
INT32 HW_Get_Press(UINT8 Index);

// current v
UINT16 HW_Get_ADC_V_Cur(UINT8 Index);
UINT16 Get_V_HGB_LED(void);
UINT32 Get_V_CRP_LED(void);
UINT16 Get_XK_V_Value(void);
UINT16 Get_48_V_Value(void);

// enable HGB CRP adc channel
void HW_EN_ADC_HGB(enum eFlag flag);
void HW_EN_ADC_CRP(enum eFlag flag);


UINT8 Send_Data_HGB(UINT32 nCmd, UINT32* pData, UINT16 nLen);
UINT8 Send_Data_CRP(UINT32 nCmd, IO_ UINT32* pData, UINT16 nLen);

UINT8 HW_WBC_GetData(UINT16* pnData, UINT16* pnLen, UINT16* pnStatus);
UINT8 HW_RBC_GetData(UINT16* pData, UINT16* pLen, UINT16* pStatus);
UINT8 HW_PLT_GetData(UINT16* pData, UINT16* pLen, UINT16* pStatus);
UINT8 HW_RBC_PLT_GetData(UINT16* pData, UINT16* pLen, UINT16* pStatus);
UINT8 Data_Circle_Handle(eTestMode eMode);
UINT8 Poll_SendDMA_ADC1_Data(UINT32 nCmd);
UINT8 Poll_SendDMA_ADC2_Data(UINT32 nCmd);

// yaolan_end
//-----------------------------------------------------------------------------------------


UINT16 AddStep_To_MS(UINT32 nStep);
// 电机的控制接口声明
// init
_EXT_ UINT8 MT_ALL_Init(void);
// axis x
_EXT_ enum eAxisXPos MT_X_get_posi(void);
//_EXT_ UINT8 MT_X_Home(void);
_EXT_ UINT8 MT_X_Home(CALL_STYLE_E eCall);
//_EXT_ UINT8 MT_X_Home_2(CALL_STYLE_E eCall);
_EXT_ UINT8 MT_X_Home_only(void);

UINT8 MT_X_MoveToPosRel(CALL_STYLE_E eCall);
//_EXT_ UINT8 MT_X_MoveToPosRel(void);
_EXT_ UINT8 MT_X_MoveToPosRel_only(void);

enum eAxisYPos MT_Y_get_posi(void);
UINT8 MT_Y_Home(CALL_STYLE_E eCall);
UINT8 MT_Y_MoveToPosRel(CALL_STYLE_E eCall);

//------------------------------
_EXT_ void  HW_FPGA_RST_H(void);

_EXT_ void  HW_FPGA_RST_L(void);
// I/Os control

//
_EXT_ UINT8  HW_Valve_On(UINT8 chIndex);
//
_EXT_ UINT8  HW_Valve_Off(UINT8 chIndex);

//------------------------------
// DC motor control
//
_EXT_ UINT8  HW_PUMP_Pulse(UINT32 nFreq, enum eDirection eDir);
// 
_EXT_ UINT32 HW_PUMP_GetFeedbackPulse(void);

//------------------------------
// the digtal adjustable resistor 
_EXT_ UINT8  HW_ADJ_SetResistor(UINT8 chIndex, UINT8 chValue);


//------------------------------
// the SPI ADC control
_EXT_ UINT8  HW_ADC_SpiCtrl(enum eFlag bAble);
_EXT_ UINT16 HW_ADC_SpiGetADC(UINT8 chIndex);
_EXT_ INT32 HW_ADC_SpiGetPress(void);
_EXT_ UINT32 HW_ADC_PressPara(UINT32 nK, UINT32 nB);

//------------------------------
// get the level of the OC and the electrode
_EXT_ UINT8  HW_LEVEL_GetOC(UINT8 chIndex);
_EXT_ UINT8  HW_LEVEL_GetElectrode(UINT8 chIndex);
_EXT_ UINT8 hw_filter_get_electrode(UINT8 chIndex);

//------------------------------
// the ADC of the slave CPU 
_EXT_ UINT16 HW_ADC_SlaveGetADC(void);

//------------------------------
// to control the direction 
_EXT_ UINT8  HW_DIR_Ctrl(void);

//------------------------------
// to control the switcher
_EXT_ UINT8  HW_SW_AdcWBC(enum eFlag bSw);
_EXT_ UINT8  HW_SW_AdcRBC(enum eFlag bSw);

//------------------------------
// enable the WBC
_EXT_ UINT8  HW_EN_WBC(enum eFlag bOn);
_EXT_ void  HW_Start_WBC(void);
_EXT_ void  HW_End_WBC(void);


void Send_Last_FIFO_Data(void);
//------------------------------
// get the data from the data buffer 
_EXT_ UINT8  HW_DATA_GetData(UINT16* pnData, UINT16* pnLen, UINT16* pnStatus);
_EXT_ UINT8 HW_DATA_ClearData(UINT16 * pnData, UINT16 * pnLen, UINT16 * pnStatus);
void Clear_FPGA_FIFO_Data(void);
//------------------------------
// 
_EXT_ UINT8  HW_LWIP_Working(UINT32 nTickList, UINT32 nTickAdc, EN_FPGA_DATA_FLAG eFlag);
_EXT_ UINT8  HW_LWIP_MainLine(void);
//
_EXT_ UINT8  HW_LWIP_Working_Recv_Handle(UINT32 nTickList, UINT32 nTickAdc);

_EXT_ UINT8  Send_Packet_Test(void);

//-----------------------------------------------------------------------------------------
// reset by software 
_EXT_ UINT8 MT_RESET_Software(void);


//-----------------------------------------------------------------------------------------
//
_EXT_ UINT8  MSG_SampleBoardFeedback(void);



#endif

