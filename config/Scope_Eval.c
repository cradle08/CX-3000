// Scope_Eval.c

#ifndef    __SCOPE_EVAL_C__
#define    __SCOPE_EVAL_C__

#include "ChainHeader.h"
#include "stm32f4xx_rcc.h"




IO_ UINT16 g_ADC3_Value[EN_ADC_END] = {0};
//-----------------------------------------------------------------------------------------
// the const group

// com-definition
USART_TypeDef* CODE_ COM_USART[COM_NUM]       = {COMM1, COMM2, COMM3}; 
UINT32         CODE_ COM_USART_CLK[COM_NUM]   = {COM1_CLK, COM2_CLK, COM3_CLK};
UINT16         CODE_ COM_ET_IRQn[COM_NUM]     = {COM1_IRQn, COM2_IRQn, COM3_IRQn};
//
GPIO_TypeDef*  CODE_ COM_TX_PORT[COM_NUM]     = {COM1_TX_GPIO_PORT, COM2_TX_GPIO_PORT, COM3_TX_GPIO_PORT};
UINT16         CODE_ COM_TX_PIN[COM_NUM]      = {COM1_TX_PIN, COM2_TX_PIN, COM3_TX_PIN};
UINT32         CODE_ COM_TX_PORT_CLK[COM_NUM] = {COM1_TX_GPIO_CLK, COM2_TX_GPIO_CLK, COM3_TX_GPIO_CLK};
//
GPIO_TypeDef*  CODE_ COM_RX_PORT[COM_NUM]     = {COM1_RX_GPIO_PORT, COM2_RX_GPIO_PORT, COM3_RX_GPIO_PORT};
UINT16         CODE_ COM_RX_PIN[COM_NUM]      = {COM1_RX_PIN, COM2_RX_PIN, COM3_RX_PIN};
UINT32         CODE_ COM_RX_PORT_CLK[COM_NUM] = {COM1_RX_GPIO_CLK, COM2_RX_GPIO_CLK, COM3_RX_GPIO_CLK};
//
UINT8          CODE_ COM_AF_TX_PINSOURCE[COM_NUM] = {COM1_AF_TX_PIN_SOURCE, COM2_AF_TX_PIN_SOURCE, COM3_AF_TX_PIN_SOURCE};
UINT8          CODE_ COM_AF_RX_PINSOURCE[COM_NUM] = {COM1_AF_RX_PIN_SOURCE, COM2_AF_RX_PIN_SOURCE, COM3_AF_RX_PIN_SOURCE};
UINT8          CODE_ COM_AF_UART[COM_NUM] = {COM1_AF_UART, COM2_AF_UART, COM3_AF_UART};



// ----------------------------------------------------------out put----------------------
// Output def --- led and speaker
GPIO_TypeDef*  CODE_ OUT_PORT[O_OUTPUT_NUM]= 
{
//  OUT_STATUS_LED1_GPIO_PORT, 
//  OUT_STATUS_LED2_GPIO_PORT, 
    OUT_MCU_LED1_GPIO_PORT,
    OUT_MCU_LED2_GPIO_PORT,
    //
    OUT_LAN8720_RST_GPIO_PORT,
    OUT_MotorX_EN_GPIO_PORT,
	OUT_MotorX_DIR_GPIO_PORT,
	OUT_MotorX_CLK_GPIO_PORT,
	OUT_MotorY_EN_GPIO_PORT,
	OUT_MotorY_DIR_GPIO_PORT,
	OUT_MotorY_CLK_GPIO_PORT,
	OUT_VALVE_AIR_PORT,
	OUT_VALVE_LIQUID_PORT,
	OUT_BEEP_PORT
//  OUT_HEAT1_GPIO_PORT,
//	OUT_HEAT1_GPIO_PORT
};
UINT16 CODE_ OUT_PIN[O_OUTPUT_NUM]= 
{
    //
//  OUT_STATUS_LED1_GPIO_PIN, 
//  OUT_STATUS_LED2_GPIO_PIN, 
    OUT_MCU_LED1_GPIO_PIN,
    OUT_MCU_LED2_GPIO_PIN,
    //
    OUT_LAN8720_RST_GPIO_PIN,
    OUT_MotorX_EN_GPIO_PIN,
	OUT_MotorX_DIR_GPIO_PIN,
	OUT_MotorX_CLK_GPIO_PIN,
    OUT_MotorY_EN_GPIO_PIN,
	OUT_MotorY_DIR_GPIO_PIN,
	OUT_MotorY_CLK_GPIO_PIN,
	OUT_VALVE_AIR_PIN,
	OUT_VALVE_LIQUID_PIN,
	OUT_BEEP_LIQUID_PIN
	
//  OUT_HEAT1_GPIO_PIN,
//	OUT_HEAT1_GPIO_PIN
};
UINT32 CODE_ OUT_CLK[O_OUTPUT_NUM]= 
{

//  OUT_STATUS_LED1_GPIO_CLK, 
//  OUT_STATUS_LED2_GPIO_CLK, 
    OUT_MCU_LED1_GPIO_CLK,
    OUT_MCU_LED2_GPIO_CLK,
    //
    OUT_LAN8720_RST_GPIO_CLK,
    OUT_MotorX_EN_GPIO_CLK,
	OUT_MotorX_DIR_GPIO_CLK,
	OUT_MotorX_CLK_GPIO_CLK,
	OUT_MotorY_EN_GPIO_CLK,
	OUT_MotorY_DIR_GPIO_CLK,
	OUT_MotorY_CLK_GPIO_CLK,
	OUT_VALVE_AIR_CLK,
	OUT_VALVE_LIQUID_CLK,
	OUT_BEEP_LIQUID_CLK
//  OUT_HEAT1_GPIO_CLK,
//	OUT_HEAT1_GPIO_CLK
};




// ----------------------------------------------------------in put---------------------------------------
// Input def  --- commom input and exti interrupt
GPIO_TypeDef* CODE_ IN_PORT[I_INPUT_NUM]= 
{
    //
//    IN_HOME_1_GPIO_PORT,
//	IN_HOME_2_GPIO_PORT,
//	IN_HOME_3_GPIO_PORT,
//	IN_HOME_4_GPIO_PORT,
//	IN_HOME_5_GPIO_PORT,
//	IN_HOME_6_GPIO_PORT,
	//
	IN_MT_STATUS_GPIO_PORT,
	IN_MotorX_IN_OC_GPIO_PORT,
	IN_MotorX_OUT_OC_GPIO_PORT,
	IN_MotorY_IN_OC_GPIO_PORT,
	IN_MotorY_OUT_OC_GPIO_PORT,
	IN_ELEC_GPIO_PORT
};
UINT16 CODE_ IN_PIN[I_INPUT_NUM]=
{
    //
//    IN_HOME_1_GPIO_PIN,
//	IN_HOME_2_GPIO_PIN,
//	IN_HOME_3_GPIO_PIN,
//	IN_HOME_4_GPIO_PIN,
//	IN_HOME_5_GPIO_PIN,
//	IN_HOME_6_GPIO_PIN,
	//
	IN_MT_STATUS_GPIO_PIN,
	IN_MotorX_IN_OC_GPIO_PIN,
	IN_MotorX_OUT_OC_GPIO_PIN,
	IN_MotorY_IN_OC_GPIO_PIN,
	IN_MotorY_OUT_OC_GPIO_PIN,
	IN_ELEC_GPIO_PIN
};	
UINT32 CODE_ IN_CLK[I_INPUT_NUM]=
{
    //
//    IN_HOME_1_GPIO_CLK,
//	IN_HOME_2_GPIO_CLK,
//	IN_HOME_3_GPIO_CLK,
//	IN_HOME_4_GPIO_CLK,
//	IN_HOME_5_GPIO_CLK,
//	IN_HOME_6_GPIO_CLK,
	//
	IN_MT_STATUS_GPIO_CLK,
	IN_MotorX_IN_OC_GPIO_CLK,
	IN_MotorX_OUT_OC_GPIO_CLK,
	IN_MotorY_IN_OC_GPIO_CLK,
	IN_MotorY_OUT_OC_GPIO_CLK,
	IN_ELEC_GPIO_CLK
};
UINT16 CODE_ IN_ET_LINE[I_INPUT_NUM]=
{
    //
//    IN_HOME_1_ET_LINE,
//	IN_HOME_2_ET_LINE,
//	IN_HOME_3_ET_LINE,
//	IN_HOME_4_ET_LINE,
//	IN_HOME_5_ET_LINE,
//	IN_HOME_6_ET_LINE,
	//
	IN_MT_STATUS_ET_LINE,
	IN_MotorX_IN_OC_ET_LINE,
	IN_MotorX_OUT_OC_ET_LINE,
	IN_MotorY_IN_OC_ET_LINE,
	IN_MotorY_OUT_OC_ET_LINE,
	IN_ELEC_ET_LINE
	
};
UINT16 CODE_ IN_ET_PORT[I_INPUT_NUM]=
{
    //
//    IN_HOME_1_ET_PORT,
//	IN_HOME_2_ET_PORT,
//	IN_HOME_3_ET_PORT,
//	IN_HOME_4_ET_PORT,
//	IN_HOME_5_ET_PORT,
//	IN_HOME_6_ET_PORT,
	//
	IN_MT_STATUS_ET_PORT,
	IN_MotorX_IN_OC_ET_PORT,
	IN_MotorX_OUT_OC_ET_PORT,
	IN_MotorY_IN_OC_ET_PORT,
	IN_MotorY_OUT_OC_ET_PORT,
	IN_ELEC_ET_IRQn
};
UINT16 CODE_ IN_ET_PIN[I_INPUT_NUM]=
{
    //
//    IN_HOME_1_ET_PIN,
//	IN_HOME_2_ET_PIN,
//	IN_HOME_3_ET_PIN,
//	IN_HOME_4_ET_PIN,
//	IN_HOME_5_ET_PIN,
//	IN_HOME_6_ET_PIN,
	//
	IN_MT_STATUS_ET_PIN,
	IN_MotorX_IN_OC_ET_PIN,
	IN_MotorX_OUT_OC_ET_PIN,
	IN_MotorY_IN_OC_ET_PIN,
	IN_MotorY_OUT_OC_ET_PIN,
	IN_ELEC_ET_PIN
};
UINT16 CODE_ IN_ET_IRQn[I_INPUT_NUM]=
{
    //
//    IN_HOME_1_ET_IRQn,
//	IN_HOME_2_ET_IRQn,
//	IN_HOME_3_ET_IRQn,
//	IN_HOME_4_ET_IRQn,
//	IN_HOME_5_ET_IRQn,
//	IN_HOME_6_ET_IRQn,
	//
	IN_MT_STATUS_ET_IRQn,
	IN_MotorX_IN_OC_ET_IRQn,
	IN_MotorX_OUT_OC_ET_IRQn,
	IN_MotorY_IN_OC_ET_IRQn,
	IN_MotorY_OUT_OC_ET_IRQn,
	IN_ELEC_ET_IRQn
};

//-----------------------------------------------------------------------------------------
// define "USE_FULL_ASSERT" in the project options setting
void assert_failed(uint8_t* file, uint32_t line)
{ 
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    printf("Wrong parameters value: file %s on line %d\r\n", file, line);
	
    /* Infinite loop */
#if 1	
    while (1)
    {
    
    }
#endif

}

//-----------------------------------------------------------------------------------------
// macro defined for "printf" debugging
PUTCHAR_PROTOTYPE
{
	// Place your implementation of fputc here 
	// e.g. write a character to the USART 
#if 0
	
#else

	PL_COM_SendChar(ch);   // will be recovered

#endif

	return ch;
}



//void HSI_Sysclock_Init(void)
//{
//	RCC_DeInit();
//	RCC_HSICmd(ENABLE);
//	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

//	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
//	PWR->CR |= PWR_CR_VOS;
//	
//	RCC_HCLKConfig(RCC_SYSCLK_Div1);
//	RCC_PCLK2Config(RCC_HCLK_Div2);
//	RCC_PCLK1Config(RCC_HCLK_Div4);
//	
//	RCC_PLLCmd(DISABLE);
//	RCC_PLLConfig(RCC_PLLSource_HSI, 16, 336, 2, 4);
//	RCC_PLLCmd(ENABLE);
//	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
//	
//	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//	while(RCC_GetSYSCLKSource() != 0x08);
//}


//-----------------------------------------------------------------------------------------
// com control
void EVAL_ComInit(Com_TypeDef eCom, USART_InitTypeDef* USART_InitStruct)
{
    GPIO_InitTypeDef GPIO_InitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;

    // 1. enable GPIO clock 
	RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[eCom] | COM_RX_PORT_CLK[eCom], ENABLE);
    // 2. enable UART clock 
    if (eCom == E_COM_SAMPLING)   // com_2  
    {
        RCC_APB1PeriphClockCmd(COM_USART_CLK[eCom], ENABLE);  // APB1
    }
    else                 // com_1 or com_3
    {
        RCC_APB2PeriphClockCmd(COM_USART_CLK[eCom], ENABLE);  // APB2
    }
	// 3. configure AFIO  
	GPIO_PinAFConfig(COM_TX_PORT[eCom],COM_AF_TX_PINSOURCE[eCom],COM_AF_UART[eCom]); // tx-pin
	GPIO_PinAFConfig(COM_RX_PORT[eCom],COM_AF_RX_PINSOURCE[eCom],COM_AF_UART[eCom]); // rx-pin
    // 4. IO configuration 
    GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[eCom] | COM_RX_PIN[eCom]; // tx-pin and rx-pin
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       // AF model
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 50MHz 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // push output
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       // 
    GPIO_Init(COM_TX_PORT[eCom],&GPIO_InitStructure);
	// 5. USART configuration 
    USART_Init(COM_USART[eCom], USART_InitStruct);	
    // 6. Enable USART
    USART_Cmd(COM_USART[eCom], ENABLE);
	// 7. enable Receive and Transmit interrupts 
    USART_ITConfig(COM_USART[eCom], USART_IT_TC, ENABLE);      //  TX_INT = 1;
    USART_ITConfig(COM_USART[eCom], USART_IT_RXNE, ENABLE);    //  RX_INT = 1;	
	// 8. configure and enable USART interrupt  
    NVIC_InitStructure.NVIC_IRQChannel = COM_ET_IRQn[eCom];       
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	
}

//-----------------------------------------------------------------------------------------
// GPIO output contorl 
void EVAL_OutputInit(Output_TypeDef eOut)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
 
    // 1. enable the GPIO Clock  
    RCC_AHB1PeriphClockCmd(OUT_CLK[eOut], ENABLE);  // AHB1

    // 2. configure the GPIO pin
	GPIO_InitStructure.GPIO_Pin = OUT_PIN[eOut]; 
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
    GPIO_Init(OUT_PORT[eOut], &GPIO_InitStructure);  // init gpio
	
	//EVAL_OutputClr(eOut);
}

void EVAL_OutputSet(Output_TypeDef eOut)
{
    OUT_PORT[eOut]->BSRRL = OUT_PIN[eOut];
}

void EVAL_OutputClr(Output_TypeDef eOut)
{
    OUT_PORT[eOut]->BSRRH = OUT_PIN[eOut];
}

void EVAL_OutputToggle(Output_TypeDef eOut)
{
    OUT_PORT[eOut]->ODR ^= OUT_PIN[eOut];
}

//-----------------------------------------------------------------------------------------
// GPIO input contorl 
void EVAL_InputInit(Input_TypeDef eIn, InModel_Typedef eModel)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
  
    // 1. enable the input pin Clock 
    RCC_AHB1PeriphClockCmd(IN_CLK[eIn], ENABLE);
  
    // 2. configure the pin as input floating 
	GPIO_InitStructure.GPIO_Pin = IN_PIN[eIn];  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
    GPIO_Init(IN_PORT[eIn], &GPIO_InitStructure); 

    // 3. extinal-interrupt model 
    if (eModel == IN_MODEL_EXTI)     
    {
        // 1). enable the SYSCFG-clock
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);       
        // 2). Connects EXTI Line to Button GPIO Pin 
        SYSCFG_EXTILineConfig(IN_ET_PORT[eIn], IN_ET_PIN[eIn]);    
        // 3). Configure EXTI line 
        EXTI_InitStructure.EXTI_Line = IN_ET_LINE[eIn];
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;   
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  // all pins are rising detected         
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;               // enable interrupt line 		
        EXTI_Init(&EXTI_InitStructure);                           
        // 4). enable and set input EXTI Interrupt to the lowest priority 
        NVIC_InitStructure.NVIC_IRQChannel = IN_ET_IRQn[eIn];
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;        // enable interrupt
        NVIC_Init(&NVIC_InitStructure);    	
    }
}

UINT8 EVAL_InputGetState(Input_TypeDef eIn)
{
    return GPIO_ReadInputDataBit(IN_PORT[eIn], IN_PIN[eIn]);
}


void HW_LEVEL_OC_Init(void)
{
	EVAL_InputInit(I_MotorX_IN_OC, IN_MODEL_GPIO);
	EVAL_InputInit(I_MotorX_OUT_OC, IN_MODEL_GPIO);
	EVAL_InputInit(I_MotorY_IN_OC, IN_MODEL_GPIO);
	EVAL_InputInit(I_MotorY_OUT_OC, IN_MODEL_GPIO);
}

void HW_Beep_Init(void)
{
	EVAL_OutputInit(O_Beep);
	EVAL_OutputClr(O_Beep);
}

void Beep(UINT8 nNum, UINT32 nTime)
{
	UINT8 i;
	if(nNum == 0) return;
	for(i = 0; i < nNum; i++)
	{
		EVAL_OutputSet(O_Beep);
		IT_SYS_DlyMs(nTime);
		EVAL_OutputClr(O_Beep);
		IT_SYS_DlyMs(nTime);
	}
}

void HW_Valve_Init()
{
	EVAL_OutputInit(O_Air_Walve);
	EVAL_OutputInit(O_Liquild_Walve);
	
	EVAL_OutputClr(O_Air_Walve);
	EVAL_OutputClr(O_Liquild_Walve);
}

void HW_ELEC_Init(void)
{
	EVAL_InputInit(I_ELEC, IN_MODEL_GPIO);
}

void HW_Press_Init(void)
{
#if (1 == 1)
	Press_I2C_Init();
#else
	//ADC
#endif
}




// i2c interface init
void Press_I2C_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(PRESS_I2C_SCL_SCLCLK_SRC|PRESS_I2C_SDA_SCLCLK_SRC, ENABLE);//使能GPIOB时钟
	// i2c scl
	GPIO_InitStructure.GPIO_Pin = PRESS_I2C_SCL_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PRESS_I2C_SCL_PORT, &GPIO_InitStructure);
	// i2c sda
	GPIO_InitStructure.GPIO_Pin = PRESS_I2C_SDA_PIN ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(PRESS_I2C_SDA_PORT, &GPIO_InitStructure);
	
	PRESS_I2C_SCL = 1;
	PRESS_I2C_SDA = 1;
}

void Press_I2C_Start(void)
{
	PRESS_I2C_SDA_OUT();     //sda线输出
	PRESS_I2C_SDA=1;
	Delay_US(4);	
	PRESS_I2C_SCL=1;
	Delay_US(4);
 	PRESS_I2C_SDA=0;//START:when CLK is high,DATA change form high to low 
	Delay_US(4);
}

void Press_I2C_Stop(void)
{
	PRESS_I2C_SDA_OUT();//sda线输出
	//PRESS_I2C_SCL=0;
	PRESS_I2C_SDA=0;
 	Delay_US(4); 
	PRESS_I2C_SCL=1;//STOP:when CLK is high DATA change form low to high
 	Delay_US(4); 
	PRESS_I2C_SDA=1;//发送I2C总线结束信号 
}

UINT8 Press_I2C_Wait_Ack(void)
{
	u8 ucErrTime=0;
	PRESS_I2C_SDA_IN();      //SDA设置为输入  
	PRESS_I2C_SDA=1;Delay_US(1);	   
	PRESS_I2C_SCL=1;Delay_US(1);	 
	while(PRESS_I2C_READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			Press_I2C_Stop();
			return 1;
		}
	}
	PRESS_I2C_SCL=0;//时钟输出0 	   
	return 0; 
}

void Press_I2C_Ack(void)
{
	PRESS_I2C_SCL=0;
	PRESS_I2C_SDA_OUT();
	PRESS_I2C_SDA=0;
	Delay_US(2);
	PRESS_I2C_SCL=1;
	Delay_US(2);
	PRESS_I2C_SCL=0;
}

void Press_I2C_NAck(void)
{
	PRESS_I2C_SCL=0;
	PRESS_I2C_SDA_OUT();
	PRESS_I2C_SDA=1;
	Delay_US(2);
	PRESS_I2C_SCL=1;
	Delay_US(2);
	PRESS_I2C_SCL=0;
}

void Press_I2C_Send_Byte(UINT8 nVal)
{
    u8 t;   
	PRESS_I2C_SDA_OUT(); 	    
    PRESS_I2C_SCL=0;//拉低时钟开始数据传输
	__disable_irq();
    for(t=0;t<8;t++)
    {              
        PRESS_I2C_SDA=(nVal&0x80)>>7;
        nVal<<=1; 	  
		Delay_US(2);   //对TEA5767这三个延时都是必须的
		PRESS_I2C_SCL=1;
		Delay_US(2); 
		PRESS_I2C_SCL=0;	
		Delay_US(2);
    }
	__enable_irq();
}

UINT8 Press_I2C_Read_Byte(UINT8 nAck)
{
	unsigned char i,receive=0;
	PRESS_I2C_SDA_IN();//SDA设置为输入
	__disable_irq();
    for(i=0;i<8;i++ )
	{
        PRESS_I2C_SCL=0; 
        Delay_US(2);
		PRESS_I2C_SCL=1;
        receive<<=1;
        if(PRESS_I2C_READ_SDA)receive++;   
		Delay_US(1); 
    }					 
    if (!nAck)
        Press_I2C_NAck();//发送nACK
    else
        Press_I2C_Ack(); //发送ACK   
	__enable_irq();
    return receive;
}

INT32 HW_Press_I2C(void)
{
	UINT8 I2c_Address = 0x29;
	UINT8 Read_Commond = 0xAA;
	UINT8 DLHR_DATA[7] = {0x00};
	UINT8 Status;
	INT32 Pressure_data,Tempertaure_data;
	
	
//	Press_I2C_Init();
	Press_I2C_Start();
	Press_I2C_Send_Byte((I2c_Address)<<1|0);//write
	Press_I2C_Ack();
	Press_I2C_Send_Byte(Read_Commond);//1mps
	Press_I2C_Wait_Ack();
	Press_I2C_Stop();
	
	IT_SYS_DlyMs(5);
	//Delay_US(250);
	__disable_irq();//__set_PRIMASK(0);
	Press_I2C_Start();
	Press_I2C_Send_Byte((I2c_Address)<<1|1);//read
	Press_I2C_Wait_Ack();
	DLHR_DATA[0] = Press_I2C_Read_Byte(1);//STATUS[7:0]
	DLHR_DATA[1] = Press_I2C_Read_Byte(1);//PRESSURE[23:16]
	DLHR_DATA[2] = Press_I2C_Read_Byte(1);//PRESSURE[15:8]
	DLHR_DATA[3] = Press_I2C_Read_Byte(1);//PRESSURE[7:0]
	DLHR_DATA[4] = Press_I2C_Read_Byte(1);//TEMPERATURE[23:16]
	DLHR_DATA[5] = Press_I2C_Read_Byte(1);//TEMPERATURE[15:8]
	DLHR_DATA[6] = Press_I2C_Read_Byte(0);//TEMPERATURE[7:0]
	Press_I2C_Stop();
	__enable_irq();//__set_PRIMASK(1);
	Status = DLHR_DATA[0];
	Pressure_data = (DLHR_DATA[1]<<16)|(DLHR_DATA[2]<<8)|DLHR_DATA[3];
	Tempertaure_data = (DLHR_DATA[4]<<16)|(DLHR_DATA[5]<<8)|DLHR_DATA[6];
	
	return Pressure_data;
}


void Delay_US(UINT32 us)
{
    UINT32 start, now, delta, reload, us_tick;
    start = SysTick->VAL;
    reload = SysTick->LOAD;
    us_tick = SystemCoreClock / 1000000UL;
    do {
        now = SysTick->VAL;
        delta = start > now ? start - now : reload + start - now;
    } while(delta < us_tick * us);
}



// 
UINT8  PF_InitTimer2(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 

    // reset the timerX
	TIM_DeInit(TIM2); 
    // enable the clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
	// interrupt configuration
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure); 
	//----------------------------------------------------------------
	// TIMx configuration  11059200 * 15 = (256*432*100) * 15 = 165888000  
	// attention: "== SYSCLK / 3"
	//            ".TIM_Period" is autoreload value, and must bigger 
	//            than 1000, or it case many interrupts too frequently
	//            that consumes all the interrupt-resource of the MCU
    TIM_TimeBaseStructure.TIM_Period = 6480;                      // 10ms, 100Hz = 648000 / 6480
    TIM_TimeBaseStructure.TIM_Prescaler = 256;                    // 165888000 / 256 = 648000 Hz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       // none   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   // up 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
    //----------------------------------------------
	// enable the autoreload 
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	// Clear TIMx update pending flag
    TIM_ClearFlag(TIM2, TIM_FLAG_Update); 
    // Enable TIMx Update interrupt 
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	// TIMx nable counter
    TIM_Cmd(TIM2, ENABLE);   

    
	return e_Feedback_Success;
}

// Timer 3
UINT8 PF_InitTimer3(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 

    // reset the timerX
	TIM_DeInit(TIM3); 
    // enable the clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	// interrupt configuration
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure); 
	//----------------------------------------------------------------
	// TIMx configuration  11059200 * 15 = (256*432*100) * 15 = 165888000  
	// attention: "== SYSCLK / 3"
	//            ".TIM_Period" is autoreload value, and must bigger 
	//            than 1000, or it case many interrupts too frequently
	//            that consumes all the interrupt-resource of the MCU
    TIM_TimeBaseStructure.TIM_Period = 6480;                      // 10ms, 100Hz = 648000 / 6480
    TIM_TimeBaseStructure.TIM_Prescaler = 256;                    // 165888000 / 256 = 648000 Hz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       // none   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   // up 
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
    //----------------------------------------------
	// enable the autoreload 
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	// Clear TIMx update pending flag
    TIM_ClearFlag(TIM3, TIM_FLAG_Update); 
    // Enable TIMx Update interrupt 
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	// TIMx disable counter
    TIM_Cmd(TIM3, DISABLE);   

	return e_Feedback_Success;
}



// Timer 4
UINT8 PF_InitTimer4(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 

    // reset the timerX
	TIM_DeInit(TIM4); 
    // enable the clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); 
	// interrupt configuration
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
    NVIC_Init(&NVIC_InitStructure); 
	//----------------------------------------------------------------
	// TIMx configuration  11059200 * 15 = (256*432*100) * 15 = 165888000  
	// attention: "== SYSCLK / 3"
	//            ".TIM_Period" is autoreload value, and must bigger 
	//            than 1000, or it case many interrupts too frequently
	//            that consumes all the interrupt-resource of the MCU
    TIM_TimeBaseStructure.TIM_Period = 6480;                      // 10ms, 100Hz = 648000 / 6480
    TIM_TimeBaseStructure.TIM_Prescaler = 256;                    // 165888000 / 256 = 648000 Hz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;       // none   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   // up 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); 
    //----------------------------------------------
	// enable the autoreload 
	TIM_ARRPreloadConfig(TIM4, ENABLE);
	// Clear TIMx update pending flag
    TIM_ClearFlag(TIM4, TIM_FLAG_Update); 
    // Enable TIMx Update interrupt 
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	// TIMx disable counter
    TIM_Cmd(TIM4, DISABLE);   
    
	return e_Feedback_Success;
}


// init motor io
void InitMotor_IO(enum eMvMotor eMotor)
{
	if(eMotor >= Motor_End)
	{
		printf("InitMotor_IO, Input Wrong Paramer eMotor = %d\r\n", eMotor);
		return;		
	}
	//
	if(eMotor == Motor_X)
	{
		EVAL_OutputInit(O_MotorX_EN);
        EVAL_OutputInit(O_MotorX_DIR);
        EVAL_OutputInit(O_MotorX_CLK);
		
	}else if(eMotor == Motor_Y){
		
		EVAL_OutputInit(O_MotorY_EN);
        EVAL_OutputInit(O_MotorY_DIR);
        EVAL_OutputInit(O_MotorY_CLK);
	}
}

void Motor_Dir_Pos(enum eMvMotor eMotor)
{
	if(eMotor >= Motor_End)
	{
		printf("Motor_Dir_Pos: Input Wrong Paramer eMotor = %d\r\n", eMotor);
		return;		
	}
	//
	if(eMotor == Motor_X) //in
	{
        EVAL_OutputSet(O_MotorX_DIR);
	}else if(eMotor == Motor_Y){
		
        EVAL_OutputSet(O_MotorY_DIR);
	}
}

void Motor_Dir_Neg(enum eMvMotor eMotor)
{
	if(eMotor >= Motor_End)
	{
		printf("Motor_Dir_Neg: Input Wrong Paramer eMotor = %d\r\n", eMotor);
		return;		
	}
	//
	if(eMotor == Motor_X) // out
	{
        EVAL_OutputClr(O_MotorX_DIR);		
	}else if(eMotor == Motor_Y){
		
        EVAL_OutputClr(O_MotorY_DIR);
	}
}

void Motor_Enable(enum eMvMotor eMotor)
{
	if(eMotor >= Motor_End)
	{
		printf("Motor_Enable: Input Wrong Paramer eMotor = %d\r\n", eMotor);
		return;		
	}
	//
	if(eMotor == Motor_X)
	{
		EVAL_OutputSet(O_MotorX_EN);		
	}else if(eMotor == Motor_Y){
		
		EVAL_OutputSet(O_MotorY_EN);
	}
}

void Motor_Disable(enum eMvMotor eMotor)
{
	if(eMotor >= Motor_End)
	{
		printf("Motor_Disable: Input Wrong Paramer eMotor = %d\r\n", eMotor);
		return;		
	}
	//
	if(eMotor == Motor_X)
	{
		EVAL_OutputClr(O_MotorX_EN);		
	}else if(eMotor == Motor_Y){
		
		EVAL_OutputClr(O_MotorY_EN);
	}
}

void Motor_Clk_Set(enum eMvMotor eMotor)
{
	if(eMotor >= Motor_End)
	{
		printf("Motor_CLK_Set: Input Wrong Paramer eMotor = %d\r\n", eMotor);
		return;		
	}
	//
	if(eMotor == Motor_X)
	{
        EVAL_OutputSet(O_MotorX_CLK);
		
	}else if(eMotor == Motor_Y){
	
        EVAL_OutputSet(O_MotorY_CLK);
	}
}

void Motor_Clk_Reset(enum eMvMotor eMotor)
{
	if(eMotor >= Motor_End)
	{
		printf("Motor_CLK_Clr: Input Wrong Paramer eMotor = %d\r\n", eMotor);
		return;		
	}
	//
	if(eMotor == Motor_X)
	{
        EVAL_OutputClr(O_MotorX_CLK);
		
	}else if(eMotor == Motor_Y){
		
        EVAL_OutputClr(O_MotorY_CLK);
	}
}



void InitMotor_Param(enum eMvMotor eMotor)
{
	if(eMotor >= Motor_End)
	{
		printf("InitMotor_Param, Input Wrong Paramer eMotor = %d\r\n", eMotor);
		return;		
	}
	
	if(eMotor == Motor_X)
	{
		 // timer3
		
	}else if(eMotor == Motor_Y){
		
	}
}


// init motor timer and io
void PF_InitMotor(enum eMvMotor eMotor)
{
	if(eMotor >= Motor_End)
	{
		printf("PF_InitMotor, Input Wrong Paramer eMotor = %d\r\n", eMotor);
		return;		
	}
	
	if(eMotor == Motor_X)
	{
		PF_InitTimer3(); // timer3
		
	}else if(eMotor == Motor_Y){
		PF_InitTimer4();
	}
	//
	InitMotor_IO(eMotor);
}




void IRQ_MotorX(void)
{
    MTx_PWM_ISR(Motor_X);

    return;
}

void IRQ_MotorY(void)
{
    MTx_PWM_ISR(Motor_Y);

    return;
}







void MTx_IoMinitor(enum eMvMotor eMotor)
{
    UINT8  XRAM_ chCurLevel;

    // check
    if(eMotor >= Motor_End)
    {
		return;  // error
	}
	
	// 1. for home-OC input
    if(e_False != g_atMotorStatus[eMotor].bAble)
	{
	    //-----------------------------------------
	    // 1) get the current level
        chCurLevel = HW_LEVEL_GetOC(g_atMotorStatus[eMotor].nOCIndex);
		//-----------------------------------------
		// 2) excute the special movement according to the status of the level's change 
		if(g_atMotorStatus[eMotor].nOCStatus != chCurLevel)
		{ 
			// stop immediately
			//if(0 == g_atMotorStatus[eMotor].nStepsSlow) 
			//{
				g_atMotorStatus[eMotor].nStepsAcc   = 0;
				g_atMotorStatus[eMotor].nStepsEqu   = 0;
				g_atMotorStatus[eMotor].nStepsDec   = 0;
				g_atMotorStatus[eMotor].nSteps = 0;
			//}

			//-----------------------------------------
			// 3) attention: only for one time !!!
			g_atMotorStatus[eMotor].bAble = e_False;
			g_atMotorStatus[eMotor].nOCStatus = chCurLevel;
		}
	} // end of if

	// 2. for other's IO input
	return;
}	


void HW_PUMP_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(PUMP_PWM_TIM_SRC, ENABLE);  	
	RCC_AHB2PeriphClockCmd(PUMP_CLK_SRC, ENABLE); 
	GPIO_PinAFConfig(PUMP_CLK_PORT, PUMP_CLK_PIN_AF, PUMP_CLK_PORT_AF); 
	
	GPIO_InitStructure.GPIO_Pin = PUMP_CLK_PIN;          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        
	GPIO_Init(PUMP_CLK_PORT, &GPIO_InitStructure);        
	GPIO_SetBits(PUMP_CLK_PORT, PUMP_CLK_PIN);
	
	//168/84=2M, 2M/25000=800Hz(12.5ms)
	TIM_DeInit(PUMP_PWM_TIM);
	TIM_TimeBaseStructure.TIM_Prescaler=83;  
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period= PUMP_RRESS_MAX_FREQ - 1; // 25000 - 1 自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(PUMP_PWM_TIM,&TIM_TimeBaseStructure);

	//初始化TIM14 Channel1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM_OCPolarity_Low; 
	//TIM_OCInitStructure.TIM_Pulse = ;
	TIM_OC1Init(PUMP_PWM_TIM, &TIM_OCInitStructure);  

	TIM_OC1PreloadConfig(PUMP_PWM_TIM, TIM_OCPreload_Enable); 
	TIM_ARRPreloadConfig(PUMP_PWM_TIM, ENABLE);
	TIM_Cmd(PUMP_PWM_TIM, ENABLE);  //
	
	TIM_SetCompare1(PUMP_PWM_TIM, PUMP_PRESS_OFF);
}




void HW_ADJ_Resistor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(D_REGISTER_CLK_SRC|D_REGISTER_MOSI_SRC|D_REGISTER_CS_SRC, ENABLE);
	//cs
	GPIO_InitStructure.GPIO_Pin = D_REGISTER_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(D_REGISTER_CS_PORT, &GPIO_InitStructure);
	GPIO_SetBits(D_REGISTER_CS_PORT, D_REGISTER_CS_PIN);
	
	//MISO	
//	GPIO_InitStructure.GPIO_Pin = D_REGISTER_MISO_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO_Mode_IN;
//	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_DOWN; //GPIO_PuPd_UP
//	GPIO_Init(ADC24BIT_MISO_PORT, &GPIO_InitStructure);
//	GPIO_ResetBits(D_REGISTER_MISO_PORT, D_REGISTER_MISO_PIN);
	
	// MOSI
	GPIO_InitStructure.GPIO_Pin = D_REGISTER_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(D_REGISTER_MOSI_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(D_REGISTER_MOSI_PORT, D_REGISTER_MOSI_PIN);
	// clk
	GPIO_InitStructure.GPIO_Pin = D_REGISTER_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(D_REGISTER_CLK_PORT, &GPIO_InitStructure);
	GPIO_SetBits(D_REGISTER_CLK_PORT, D_REGISTER_CLK_PIN);

}




void ADC1_DMA_Config()
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_DeInit(DMA2_Stream0);
	while(DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
	// DMA SET
	DMA_InitStructure.DMA_Channel 				= DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&ADC1->DR,
	DMA_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)g_ADC1_Buffer;
	DMA_InitStructure.DMA_DIR					= DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize			= ADC1_BUFFER_LEN;
	DMA_InitStructure.DMA_PeripheralInc			= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize		= DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode					= DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority				= DMA_Priority_High; //
	DMA_InitStructure.DMA_FIFOMode				= DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold			= DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst			= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
	
	//DMA_DoubleBufferModeConfig(DMA2_Stream0,(uint32_t)&g_ADC_Buffer_2,DMA_Memory_0);//DMA_Memory_0?????
	//DMA_DoubleBufferModeCmd(DMA2_Stream0,ENABLE);
	DMA_Init(DMA2_Stream0, &DMA_InitStructure); 
	
	// NVIC
	DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_HTIF0);
	DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
	DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
	DMA_ITConfig(DMA2_Stream0,DMA_IT_HT,ENABLE);	
		
	NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream0_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;                      
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
	//DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
	//while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
	DMA_Cmd(DMA2_Stream0, ENABLE);
}


#define WBC_USE_PA5    	1
// ADC1_IN5  PA5  WBC
void ADC1_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_DeInit();
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	// PA5
	#if WBC_USE_PA5
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_5;//GPIO_Pin_5;//GPIO_Pin_5, PA5
	#else
		GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_6;	
	#endif
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//ADC1
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
	// Common Set
	ADC_Cmd(ADC1, DISABLE);
	ADC_CommonInitStructure.ADC_Mode	= ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInit(&ADC_CommonInitStructure);
	// ADC Set
	ADC_InitStructure.ADC_Resolution	= ADC_Resolution_12b;
//#if DOUBLE_ADC_CHANNEL
//	ADC_InitStructure.ADC_ScanConvMode  = DISABLE;
//	ADC_InitStructure.ADC_NbrOfConversion = 2;
//#else
	ADC_InitStructure.ADC_ScanConvMode  = ENABLE;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
//#endif
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC_DataAlign_Left;//ADC_DataAlign_Right;
	ADC_Init(ADC1, &ADC_InitStructure);	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC1_DMA_Config();
	ADC_DMACmd(ADC1, ENABLE);
	
	#if WBC_USE_PA5
		ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles); //ADC_SampleTime_3Cycles
	#else
		ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_3Cycles); //ADC_SampleTime_3Cycles
	#endif
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
	//ADC_SoftwareStartConv(ADC1);
}


//FlagStatus DMA_GetFlagStatus(DMA_Stream_TypeDef* DMAy_Streamx, uint32_t DMA_FLAG)
//uint16_t DMA_GetCurrDataCounter(DMA_Stream_TypeDef* DMAy_Streamx)
// ADC1 DMA irq
void DMA2_Stream0_IRQHandler(void) 
{
	// half DMA_GetFlagStatus
	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_HTIF0) == SET)  
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_HTIF0);
		ADC1_Status.nSFlag = 1;
		ADC1_Status.nID++;
	}
	
	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0) == SET)  
	{
		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
		ADC1_Status.nSFlag = 2;
		ADC1_Status.nID++;
	}
}

// ADC2 DMA irq
void DMA2_Stream3_IRQHandler(void) 
{
	// half DMA_GetFlagStatus
	if (DMA_GetITStatus(DMA2_Stream3, DMA_IT_HTIF3) == SET)  
	{
		DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_HTIF3);
		ADC2_Status.nSFlag = 1;
		ADC2_Status.nID++;
	}

	if (DMA_GetITStatus(DMA2_Stream3, DMA_IT_TCIF3) == SET)  
	{
		DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_TCIF3);
		ADC2_Status.nSFlag = 2;
		ADC2_Status.nID++;
	}
}

void ADC2_DMA_Config()
{
	DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	DMA_DeInit(DMA2_Stream3);
	while(DMA_GetCmdStatus(DMA2_Stream3) != DISABLE){}
	// DMA SET
	DMA_InitStructure.DMA_Channel 				= DMA_Channel_1;
	DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&ADC2->DR,
	DMA_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)g_ADC2_Buffer;
	DMA_InitStructure.DMA_DIR					= DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize			= ADC1_BUFFER_LEN;
	DMA_InitStructure.DMA_PeripheralInc			= DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize		= DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode					= DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority				= DMA_Priority_High; //
	DMA_InitStructure.DMA_FIFOMode				= DMA_FIFOMode_Disable;
	DMA_InitStructure.DMA_FIFOThreshold			= DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst			= DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream3, &DMA_InitStructure); 
		
	// NVIC
	DMA_ClearITPendingBit(DMA2_Stream3,DMA_IT_HTIF3);
	DMA_ClearITPendingBit(DMA2_Stream3,DMA_IT_TCIF3);
	DMA_ITConfig(DMA2_Stream3,DMA_IT_TC,ENABLE);
	DMA_ITConfig(DMA2_Stream3,DMA_IT_HT,ENABLE);	
		
	NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00;   
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00;                      
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	//DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
	//DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
	//while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
	DMA_Cmd(DMA2_Stream3, ENABLE);
}

// ADC2_IN8  PB0  RBC/PLT,  (if RBC/PLT depart, PA6-RBC,PB0-PLT)
void ADC2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_DeInit();
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	// PB0
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	//ADC2
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);
	// Common Set
	ADC_Cmd(ADC2, DISABLE);
	//ADC_CommonInitStructure.ADC_Mode	= ADC_DualMode_RegSimult;									ADC_DualMode_RegSimult
	ADC_CommonInitStructure.ADC_Mode	= ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
	ADC_CommonInit(&ADC_CommonInitStructure);
	// ADC Set
	ADC_InitStructure.ADC_Resolution	= ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode  = ENABLE;//DISABLE;
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC_DataAlign_Left;//ADC_DataAlign_Right;
	ADC_Init(ADC2, &ADC_InitStructure);	
	ADC_Cmd(ADC2, ENABLE);
	
	ADC2_DMA_Config();
	ADC_DMACmd(ADC2, ENABLE);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_8, 1, ADC_SampleTime_3Cycles); //ADC_SampleTime_3Cycles
	ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
	//ADC_SoftwareStartConv(ADC2);
}


//u16 Get_Adc(UINT8 nCh, UINT8 nTime)   
//{
//	UINT8 i = 0;
//	UINT32 nVal = 0;
//	
//	for(i = 0; i < nTime; i++)
//	{
//		ADC_RegularChannelConfig(ADC1, nCh, 1, ADC_SampleTime_480Cycles );	    
//		ADC_SoftwareStartConv(ADC1);		
//		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
//		nVal += ADC_GetConversionValue(ADC1);	
//	}
//	nVal /= nTime;
//	return nVal;
//}

void ADC3_GPIO_Init(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	#if PRESS_SENSOR_ADC_TYPE
		RCC_AHB1PeriphClockCmd(TEMP_ADC_SRC|CUR_56V_ADC_SRC|XK_ADC_SRC| \
				LED_CUR_ADC_SRC|ELEC_ADC_SRC|SIG1_ADC_SRC|SIG2_ADC_SRC| \
				CUR12N_ADC_SRC|CUR12P_ADC_SRC|PRESS_ADC_SRC, ENABLE);
	#else
		RCC_AHB1PeriphClockCmd(TEMP_ADC_SRC|CUR_56V_ADC_SRC|XK_ADC_SRC| \
				CUR12N_ADC_SRC|CUR12P_ADC_SRC|LED_CUR_ADC_SRC|ELEC_ADC_SRC|SIG1_ADC_SRC|SIG2_ADC_SRC, ENABLE);
	#endif
	// PF6_ADC3_IN4, LED_CUR
	GPIO_InitStructure.GPIO_Pin		= LED_CUR_ADC_PIN;  
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(LED_CUR_ADC_PORT, &GPIO_InitStructure);
	// PC3_ADC3_IN13, XK
	GPIO_InitStructure.GPIO_Pin		= XK_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(XK_ADC_PORT, &GPIO_InitStructure);
	// PF9_ADC3_IN7, 56V_CUR
	GPIO_InitStructure.GPIO_Pin		= CUR_56V_ADC_PIN;	
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(CUR_56V_ADC_PORT, &GPIO_InitStructure);
	// PC0_ADC123_IN10, Temperature
	GPIO_InitStructure.GPIO_Pin		= TEMP_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(TEMP_ADC_PORT, &GPIO_InitStructure);
	//elec ADC3_IN6 PF8
	GPIO_InitStructure.GPIO_Pin		= ELEC_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(ELEC_ADC_PORT, &GPIO_InitStructure);
	// SIG1 ==> PF7_ADC3_IN5 ,CRP
	GPIO_InitStructure.GPIO_Pin		= SIG1_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(SIG1_ADC_PORT, &GPIO_InitStructure);
	// SIG2 ==> PA0_ADC3_IN0 ,HGB
	GPIO_InitStructure.GPIO_Pin		= SIG2_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(SIG2_ADC_PORT, &GPIO_InitStructure);
	
	//  PA3_ADC3_IN3 ,12V_N
	GPIO_InitStructure.GPIO_Pin		= CUR12N_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(CUR12N_ADC_PORT, &GPIO_InitStructure);
	
	//  PF10_ADC3_IN8 ,12V_N
	GPIO_InitStructure.GPIO_Pin		= CUR12P_ADC_PIN;		
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(CUR12P_ADC_PORT, &GPIO_InitStructure);
	
	#if PRESS_SENSOR_ADC_TYPE
		GPIO_InitStructure.GPIO_Pin		= PRESS_ADC_PIN; // PC2_ADC123_IN12 , Press
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
		GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
		GPIO_Init(PRESS_ADC_PORT, &GPIO_InitStructure);
	#endif	
}

#if ADC3_INIT_WITH_DMA
	void ADC3_DMA_Config(void)
	{
		DMA_InitTypeDef DMA_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
		DMA_DeInit(DMA2_Stream1);
		while(DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}
		// DMA SET
		DMA_InitStructure.DMA_Channel 				= DMA_Channel_2;
		DMA_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&ADC3->DR,
		DMA_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)g_ADC3_Value;
		DMA_InitStructure.DMA_DIR					= DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize			= ADC3_CHECK_NUM;
		DMA_InitStructure.DMA_PeripheralInc			= DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc 			= DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize	= DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize		= DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode					= DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority				= DMA_Priority_Medium; //
		DMA_InitStructure.DMA_FIFOMode				= DMA_FIFOMode_Disable;
		DMA_InitStructure.DMA_FIFOThreshold			= DMA_FIFOThreshold_Full;
		DMA_InitStructure.DMA_MemoryBurst			= DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
		
		//DMA_DoubleBufferModeConfig(DMA2_Stream0,(uint32_t)&g_ADC_Buffer_2,DMA_Memory_0);//DMA_Memory_0?????
		//DMA_DoubleBufferModeCmd(DMA2_Stream0,ENABLE);
		DMA_Init(DMA2_Stream1, &DMA_InitStructure); 
			
		// NVIC
	//	DMA_ClearITPendingBit(DMA2_Stream2,DMA_IT_HTIF0);
	//	DMA_ClearITPendingBit(DMA2_Stream2,DMA_IT_TCIF0);
	//	DMA_ITConfig(DMA2_Stream2,DMA_IT_TC,ENABLE);
	//	DMA_ITConfig(DMA2_Stream2,DMA_IT_HT,ENABLE);	
			
	//	NVIC_InitStructure.NVIC_IRQChannel=DMA2_Stream0_IRQn; 
	//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;   
	//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02;                      
	//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	//	NVIC_Init(&NVIC_InitStructure);
		//DMA_ClearITPendingBit(DMA2_Stream0,DMA_IT_TCIF0);
		//DMA_ITConfig(DMA2_Stream0,DMA_IT_TC,ENABLE);
		//while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}
		DMA_Cmd(DMA2_Stream1, ENABLE);
	}

	// ADC3 with DMA func
	void ADC3_Init(void)
	{
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
		ADC_InitTypeDef ADC_InitStructure;
		
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
		
		ADC3_GPIO_Init();
		
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);
		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);
		
		// Common Set
		ADC_CommonInitStructure.ADC_Mode	= ADC_Mode_Independent;
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;
		ADC_CommonInit(&ADC_CommonInitStructure);
		// ADC Set
		ADC_InitStructure.ADC_Resolution	= ADC_Resolution_12b;
		ADC_InitStructure.ADC_ScanConvMode  = ENABLE;//DISABLE;
		ADC_InitStructure.ADC_NbrOfConversion = ADC3_CHECK_NUM;
		ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;// ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConv_T1_CC1;
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC_DataAlign_Left;//ADC_DataAlign_Right;
		ADC_Init(ADC3, &ADC_InitStructure);	
		//ADC_Cmd(ADC3, ENABLE);
		
		ADC3_DMA_Config();
		//ADC_DMACmd(ADC3, ENABLE);		// 
		
		ADC_RegularChannelConfig(ADC3, LED_CUR_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);  // LED cur
		ADC_RegularChannelConfig(ADC3, CUR_56V_ADC_CHANNEL, 2, ADC_SampleTime_3Cycles);  // 56v cur
		ADC_RegularChannelConfig(ADC3, TEMP_ADC_CHANNEL, 3, ADC_SampleTime_3Cycles);     // temperature
		ADC_RegularChannelConfig(ADC3, XK_ADC_CHANNEL, 4, ADC_SampleTime_3Cycles); 		 // XK
		ADC_RegularChannelConfig(ADC3, SIG1_ADC_CHANNEL, 5, ADC_SampleTime_3Cycles);  	 // CRP
		ADC_RegularChannelConfig(ADC3, SIG2_ADC_CHANNEL, 6, ADC_SampleTime_3Cycles);  	 // HGB
		#if PRESS_SENSOR_ADC_TYPE
			ADC_RegularChannelConfig(ADC3, PRESS_ADC_CHANNEL, 7, ADC_SampleTime_3Cycles);	 // press
		#endif

		ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE); 
		ADC_DMACmd(ADC3, ENABLE);
		ADC_Cmd(ADC3, ENABLE);
		ADC_SoftwareStartConv(ADC3); 
	}
	
#else 
	// ADC3 without DMA func
	void ADC3_Init(void)
	{
		ADC_CommonInitTypeDef ADC_CommonInitStructure;
		ADC_InitTypeDef ADC_InitStructure;
		
		//ADC_DeInit();
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
		//
		ADC3_GPIO_Init();
		
//		RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	 
//	    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	
		// Common Set
		ADC_CommonInitStructure.ADC_Mode	= ADC_Mode_Independent;
		ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
		ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
		ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; //ADC_DMAAccessMode_1; //ADC_DMAAccessMode_Disabled
		ADC_CommonInit(&ADC_CommonInitStructure);
		// ADC Set
		ADC_InitStructure.ADC_Resolution	= ADC_Resolution_12b;
		ADC_InitStructure.ADC_ScanConvMode  = DISABLE;//ENABLE;//DISABLE;
		ADC_InitStructure.ADC_NbrOfConversion = 1;
		ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//DISABLE;//ENABLE;// ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
		ADC_InitStructure.ADC_ExternalTrigConv  = ADC_ExternalTrigConvEdge_None;// ADC_ExternalTrigConv_T1_CC1; // ADC_ExternalTrigConvEdge_None
		ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//ADC_DataAlign_Left;//ADC_DataAlign_Right;
		ADC_Init(ADC3, &ADC_InitStructure);	
		ADC_Cmd(ADC3, ENABLE);
		
		
//		ADC_RegularChannelConfig(ADC3, LED_CUR_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);  // LED cur
//		ADC_RegularChannelConfig(ADC3, CUR_56V_ADC_CHANNEL, 2, ADC_SampleTime_3Cycles);  // 56v cur
//		ADC_RegularChannelConfig(ADC3, TEMP_ADC_CHANNEL, 3, ADC_SampleTime_3Cycles);     // temperature
//		ADC_RegularChannelConfig(ADC3, XK_ADC_CHANNEL, 4, ADC_SampleTime_3Cycles); 		 // XK
//		ADC_RegularChannelConfig(ADC3, SIG1_ADC_CHANNEL, 5, ADC_SampleTime_3Cycles);  	 // CRP
//		ADC_RegularChannelConfig(ADC3, SIG2_ADC_CHANNEL, 6, ADC_SampleTime_3Cycles);  	 // HGB
//		#if PRESS_SENSOR_ADC_TYPE
//			ADC_RegularChannelConfig(ADC3, PRESS_ADC_CHANNEL, 7, ADC_SampleTime_3Cycles);	 // press
//		#endif


		//ADC2_DMA_Config();
		//ADC_DMACmd(ADC3, ENABLE);
		
		//ADC_RegularChannelConfig(ADC3, ADC_Channel_4, 1, ADC_SampleTime_480Cycles); //ADC_SampleTime_3Cycles
		//ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_480Cycles); //ADC_SampleTime_3Cycles
		
		//ADC_DMARequestAfterLastTransferCmd(ADC3, ENABLE); 
	  //ADC_DMACmd(ADC3, ENABLE);
		//ADC_Cmd(ADC3, ENABLE);
		//ADC_SoftwareStartConv(ADC3); 
	}
	
#endif //ADC3_INIT_WITH_DMA



UINT16 Get_ADC3_Channel_Value(UINT8 nIndex, UINT8 nCount)
{
	UINT32 nVal = 0;
	UINT8 i;
	
#if ADC3_INIT_WITH_DMA
	nVal = g_ADC3_Value[nIndex];
#else
	ADC_ClearFlag(ADC3,ADC_FLAG_EOC);
	ADC_Cmd(ADC3,ENABLE);
	for(i = 0; i < nCount; i++)
	{
		ADC_RegularChannelConfig(ADC3, g_ADC3_IN[nIndex], 1, ADC_SampleTime_3Cycles ); 
		ADC_SoftwareStartConv(ADC3);		
		while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
		nVal += ADC_GetConversionValue(ADC3);	
	}
	nVal /= nCount;
#endif
	return nVal;
}

//
UINT16 HW_Press_ADC(void)
{
	UINT16 nVal = 0;
	
#if ADC3_INIT_WITH_DMA
	nVal = g_ADC3_Value[EN_ADC_PRESS];
#else
	#if PRESS_SENSOR_ADC_TYPE
		nVal = Get_ADC3_Channel_Value(EN_ADC_PRESS, ADC_SMOOTH_NUM_5);
	#else
		// IIC API ...
		
	#endif
#endif
	return nVal;
}

//
UINT16 Get_XK_ADC(void)
{
	UINT16 nVal = 0;
	
#if ADC3_INIT_WITH_DMA
	nVal = g_ADC3_Value[EN_ADC_XK];
#else
	nVal = Get_ADC3_Channel_Value(EN_ADC_XK, ADC_SMOOTH_NUM_5);
#endif
	return nVal;
}


UINT16 Get_XK_V(void)
{
	return Get_XK_ADC()*ADC_V_REF_VALUE_3_3/ADC_RESOLUTION_12;
}

UINT16 Get_12V_N_ADC(void)
{
	UINT16 nVal = 0;
	
#if ADC3_INIT_WITH_DMA
	nVal = g_ADC3_Value[EN_ADC_12V_N];
#else
	nVal = Get_ADC3_Channel_Value(EN_ADC_12V_N, ADC_SMOOTH_NUM_5);
#endif
	return nVal;
}

UINT16 Get_12V_P_ADC(void)
{
	UINT16 nVal = 0;
	
#if ADC3_INIT_WITH_DMA
	nVal = g_ADC3_Value[EN_ADC_12V_P];
#else
	nVal = Get_ADC3_Channel_Value(EN_ADC_12V_P, ADC_SMOOTH_NUM_5);
#endif
	return nVal;
}

//
UINT16 Get_56V_Cur_ADC(void)
{
	UINT16 nVal = 0;
	
#if ADC3_INIT_WITH_DMA
	//nVal = g_ADC3_Value[EN_ADC_56V_CUR];
#else
	nVal = Get_ADC3_Channel_Value(EN_ADC_56V_CUR, ADC_SMOOTH_NUM_5);
#endif
	return nVal;
}

UINT16 Get_56V_Cur_V(void)
{
	return Get_56V_Cur_ADC()*ADC_V_REF_VALUE_3_3/ADC_RESOLUTION_12;
}

//
UINT16 Get_LED_Cur_ADC(void)
{
	UINT16 nVal = 0;
	
#if ADC3_INIT_WITH_DMA
	//nVal = g_ADC3_Value[EN_ADC_LED_CUR];
#else
	nVal = Get_ADC3_Channel_Value(EN_ADC_LED_CUR, ADC_SMOOTH_NUM_5);
#endif
	return nVal;
}

UINT16 Get_Temp_ADC(void)
{
	UINT16 nVal = 0;
	
#if ADC3_INIT_WITH_DMA
	//nVal = g_ADC3_Value[EN_ADC_LED_CUR];
#else
	nVal = Get_ADC3_Channel_Value(EN_ADC_TEMP, ADC_SMOOTH_NUM_5);
#endif
	return nVal;
}

//
UINT32 HW_Get_ADC_HGB(void)
{
	UINT32 nVal = 0;
	
#if USE_STM32F407_ONLY
	//nVal = Get_ADC3_Channel_Value(EN_ADC_HGB, ADC_SMOOTH_NUM_30);
	nVal = AD7799_Get_ADC_Value(AD7799_Get_Out_Data());
#else	
	nVal = HW_Get_ADC_Perip(0); // /* adc, 0=HGB,1=WBC vol value, 2=RBC(wbc backup,crp test), 3=press, */ 
	
#endif
	return nVal;
}

//
UINT32  HW_Get_ADC_CRP(void)
{
	UINT32  nVal = 0;
	
#if USE_STM32F407_ONLY
	//nVal = Get_ADC3_Channel_Value(EN_ADC_CRP, ADC_SMOOTH_NUM_30);
	nVal = AD7799_Get_ADC_Value(AD7799_Get_Out_Data());
#else	
	nVal = HW_Get_ADC_Perip(2);  /* adc, 0=HGB,1=WBC vol value, 2=RBC(wbc backup,crp test), 3=press, */ 
#endif
	return nVal;
}

//
UINT16  Get_HGB_Value(void)
{
	UINT16  nRet;
	UINT32  nVal;
	
	nVal = HW_Get_ADC_CRP();
	nRet = nVal*ADC_V_REF_VALUE_5/ADC_RESOLUTION_12;
	printf("HGB: ADC=%d, V=%d\r\n", (int)nVal, (int)nRet);
	return nRet;
}

//
UINT32  Get_CRP_Value(void)
{
	UINT32 nVal, nRet;
	
	nVal = HW_Get_ADC_CRP();
	nRet = nVal*ADC_V_REF_VALUE_5/ADC_RESOLUTION_24;
	//printf("CRP: ADC=%d, V=%d\r\n", (int)nVal, (int)nRet);
	return nRet;
}


//
UINT16 HW_Elec_ADC(void)
{
	UINT16 nVal = 0, i;
	
#if ADC3_INIT_WITH_DMA
	//nVal = g_ADC3_Value[1];
#else
	ADC_ClearFlag(ADC3,ADC_FLAG_EOC);
	ADC_Cmd(ADC3,ENABLE);
	for(i = 0; i < 5; i++)
	{
		ADC_RegularChannelConfig(ADC3, ELEC_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles ); 
		ADC_SoftwareStartConv(ADC3);		
		while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));
		nVal += ADC_GetConversionValue(ADC3);	
	}
	nVal /= 5;
#endif
	return nVal;

}

UINT16 HW_Elec_V(void)
{
	return HW_Elec_ADC()*ADC_V_REF_VALUE_3_3/ADC_RESOLUTION_12;
}











//-----------------------------------------------------------------------------------------
// initialization for the code block
void EVAL_Init(void)
{
    USART_InitTypeDef USART_InitStructure;	
	RCC_ClocksTypeDef  tClockTree;
	
	//-------------------------------------------
    // 1. update the system's clock
 // HSI_Sysclock_Init(); //SystemCoreClockUpdate(); //todo use-HSE
	RCC_GetClocksFreq(&tClockTree);
#if 0
    // printf("\r\n--- SystemCoreClock = %d ---\r\n", SystemCoreClock);
    printf("\r\n SYS-clk = %d", tClockTree.SYSCLK_Frequency);
    printf("\r\n AHB-clk = %d", tClockTree.HCLK_Frequency);
    printf("\r\n APB1-clk = %d", tClockTree.PCLK1_Frequency);
    printf("\r\n APB2-clk = %d", tClockTree.PCLK2_Frequency);
   // printf("\r\n ");
#endif
	
    //-------------------------------------------
    // 2. Initialize and start the SysTick counter and its interrupt. 
    //    take attention: when want to use IRQ_DelayMs(), this must be called first !!!
	if(SysTick_Config(SystemCoreClock / 1000))   // Setup SysTick Timer for 1 millisecond interrupts. 
    { 
        // Capture error / 
	    printf("\r\nSysTick_Config error\r\n");
        while (1);
    }
    // attentio: set to the higher priority
	NVIC_SetPriority(SysTick_IRQn, 0);
    // IT_SYS_DlyMs(100);
    
    //-------------------------------------------
	// 3. priority setting
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
    //-------------------------------------------   
	// Disable the JTAG interface and enable the SWJ interface. // GPIO_Remap_SWJ_JTAGDisable	
	// Enable the AFIO Clock  
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// mapping
    // GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);     // AFIO->MAPR |= 0x02000000; // GPIO_Remap_SWJ_JTAGDisable

    /*******************************************/
    // 4. initialize  com 
	USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    // 
    EVAL_ComInit(E_COM_MAIN, &USART_InitStructure);  
    //yaolan_20190715  EVAL_ComInit(E_COM_SAMPLING, &USART_InitStructure);  
	// EVAL_ComInit(E_COM_NULL, &USART_InitStructure);  

	//-------------------------------------------
	// 5. Initialize  IO output  
	// led and the reset pin of the FPGA 
    // EVAL_OutputInit(O_STATUS_LED_1);
	//EVAL_OutputInit(O_STATUS_LED_2);
	EVAL_OutputInit(O_MCU_LED_1);
	EVAL_OutputInit(O_MCU_LED_2);
	EVAL_OutputInit(O_LAN8720_RST);
	//EVAL_OutputInit(O_HEAT_1);//yaolan_20190715
	//EVAL_OutputInit(O_HEAT_2);//yaolan_20190715


	//-------------------------------------------
    // 6. FPGA init
    //IT_SYS_DlyMs(500); // attention: waiting the FPGA to be ready
#if !USE_STM32F407_ONLY
	FPGA_Init();
	FPGA_ResetHardware();
#endif

	//-------------------------------------------
	// 7. initialize  IO input --- normal and exti interrupt 
	// input
	//EVAL_InputInit(I_HOME_X, IN_MODEL_GPIO);      // //yaolan_20190715
	//EVAL_InputInit(I_HOME_Y, IN_MODEL_GPIO);      // //yaolan_20190715
	//EVAL_InputInit(I_HOME_Z, IN_MODEL_GPIO);      // //yaolan_20190715
	//EVAL_InputInit(I_HOME_M, IN_MODEL_GPIO);      // //yaolan_20190715
#if !USE_STM32F407_ONLY
	EVAL_InputInit(I_FEEDBACK_1, IN_MODEL_EXTI);  //EXTI15-10 PB13// EXTI9_5_IRQn  EXTI_Line7
#endif
	//-------------------------------------------
	// 8. Initialize the spi-flash 
    // SPI_FLASH_Init();
    //-------------------------------------------
	// 9. the timer of the system messages
	
#if USE_STM32F407_ONLY

	ADC1_Init();//APP_ADC_Init(EN_ADC1);
//	// ...?
//	ADC_SoftwareStartConv(ADC1);
//	IT_SYS_DlyMs(10);
//	Disable_ADC(EN_ADC1);
//	memset((void*)&ADC1_Status, 0, sizeof(ADC_Status_InitTypeDef));	



	ADC2_Init();//APP_ADC_Init(EN_ADC2);
		
	ADC3_Init();//APP_ADC_Init(EN_ADC3);
	
	PF_InitTimer2();
	Elec_Init();
	Beep_Init();
	HW_Pump_init();
	HW_Valve_Init();
	HW_OC_Init();
	Press_Init();
	Turn_Motor_Init();
	Mixing_Motor_Init();
	LED_Init();
	LED_Cur_DAC_Init();
	ADC24Bit_Init();
	DResistor_Init();
	//OutIn_Motor_Init();
	
	Counter_Check_Init();
	Counter_Adjust_Init();
	
#endif
	Beep(1, 400);
}












#endif
