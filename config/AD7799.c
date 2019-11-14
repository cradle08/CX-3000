
#include "ChainHeader.h"




void ADC24Bit_SPI_Init(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
	  SPI_InitTypeDef  SPI_InitStructure;
		
	  RCC_AHB1PeriphClockCmd(ADC24BIT_CLK_SRC|ADC24BIT_MOSI_SRC|ADC24BIT_MISO_SRC|ADC24BIT_CS_SRC, ENABLE);
	  RCC_APB1PeriphClockCmd(ADC24BIT_SPI_SRC, ENABLE); 
	  // clk
	  GPIO_InitStructure.GPIO_Pin	 = ADC24BIT_CLK_PIN; 
	  GPIO_InitStructure.GPIO_Mode 	 = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType	 = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;
	  GPIO_Init(ADC24BIT_CLK_PORT, &GPIO_InitStructure);
	  // mosi
	  GPIO_InitStructure.GPIO_Pin	 = ADC24BIT_MOSI_PIN; 
	  GPIO_InitStructure.GPIO_Mode 	 = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType	 = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;
	  GPIO_Init(ADC24BIT_MOSI_PORT, &GPIO_InitStructure);
	  // miso
	  GPIO_InitStructure.GPIO_Pin	 = ADC24BIT_MISO_PIN; 
	  GPIO_InitStructure.GPIO_Mode 	 = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType	 = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_DOWN;//GPIO_PuPd_UP;
	  GPIO_Init(ADC24BIT_MISO_PORT, &GPIO_InitStructure);
	  // cs
	  GPIO_InitStructure.GPIO_Pin	 = ADC24BIT_CS_PIN; 
	  GPIO_InitStructure.GPIO_Mode 	 = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType	 = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_NOPULL;
	  GPIO_Init(ADC24BIT_CS_PORT, &GPIO_InitStructure);
	
	  //GPIO_PinAFConfig(D_REGISTER_CLK_PORT,GPIO_PinSource3,GPIO_AF_SPI1); 
	  //GPIO_PinAFConfig(D_REGISTER_MOSI_PORT,GPIO_PinSource4,GPIO_AF_SPI1);
	  GPIO_PinAFConfig(ADC24BIT_MISO_PORT, ADC24BIT_MISO_AF_SRC,  ADC24BIT_SPI_AF); 	  
	  GPIO_PinAFConfig(ADC24BIT_CLK_PORT,  ADC24BIT_CLK_AF_SRC,	   ADC24BIT_SPI_AF); 
	  GPIO_PinAFConfig(ADC24BIT_MOSI_PORT, ADC24BIT_MOSI_AF_SRC,   ADC24BIT_SPI_AF);
 	  GPIO_PinAFConfig(ADC24BIT_CS_PORT,   ADC24BIT_CS_AF_SRC,     ADC24BIT_SPI_AF); 
	  
	  RCC_APB1PeriphResetCmd(ADC24BIT_SPI_SRC, ENABLE);
	  RCC_APB1PeriphResetCmd(ADC24BIT_SPI_SRC, DISABLE);

	  SPI_Cmd(ADC24BIT_SPI, DISABLE);
	  SPI_I2S_DeInit(ADC24BIT_SPI);
	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;//SPI_BaudRatePrescaler_256		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	  SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	  SPI_Init(ADC24BIT_SPI, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	  
	  SPI_Cmd(ADC24BIT_SPI, ENABLE); //ʹ��SPI����
	  
	  ADC24Bit_SPI_ReadWriteByte(0xff);
}

void ADC24Bit_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(ADC24BIT_CLK_SRC|ADC24BIT_MOSI_SRC|ADC24BIT_MISO_SRC|ADC24BIT_CS_SRC, ENABLE);
	//cs
	GPIO_InitStructure.GPIO_Pin = ADC24BIT_CS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ADC24BIT_CS_PORT, &GPIO_InitStructure);
	GPIO_SetBits(ADC24BIT_CS_PORT, ADC24BIT_CS_PIN);
	
	//Di	
	GPIO_InitStructure.GPIO_Pin = ADC24BIT_MISO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//GPIO_Mode_IN;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//GPIO_PuPd_DOWN; //GPIO_PuPd_UP
	GPIO_Init(ADC24BIT_MISO_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(ADC24BIT_MISO_PORT, ADC24BIT_MISO_PIN);
	// Do
	GPIO_InitStructure.GPIO_Pin = ADC24BIT_MOSI_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ADC24BIT_MOSI_PORT, &GPIO_InitStructure);
	GPIO_ResetBits(ADC24BIT_MOSI_PORT, ADC24BIT_MOSI_PIN);
	// clk
	GPIO_InitStructure.GPIO_Pin = ADC24BIT_CLK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(ADC24BIT_CLK_PORT, &GPIO_InitStructure);
	GPIO_SetBits(ADC24BIT_CLK_PORT, ADC24BIT_CLK_PIN);
}

void ADC24Bit_Init(void)
{
#if AD7799_USE_SPI_COMMUNICATION
	ADC24Bit_SPI_Init();
#else
	ADC24Bit_GPIO_Init();
#endif
	AD7799_Init();
}

//UINT16 ADC24Bit_SPI_GetByte(void)
//{
////	//__disable_irq();
////    while (SPI_I2S_GetFlagStatus(ADC24BIT_SPI, SPI_I2S_FLAG_RXNE) == RESET){}//�ȴ���������  
////	return SPI_I2S_ReceiveData(ADC24BIT_SPI);	 
////	//__enable_irq();

//		UINT8 nRetry = 0;
//	while (SPI_I2S_GetFlagStatus(ADC24BIT_SPI, SPI_I2S_FLAG_TXE) == RESET)
//	{
//		nRetry ++;
//		if(nRetry > 200) return 0;
//	}
//	nRetry = 0;
//	SPI_I2S_SendData(ADC24BIT_SPI, nData);	
//	//
//	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
//	{
//		nRetry ++;
//		if(nRetry > 200) return 0;
//	}
//	nRetry = 0;
//	return SPI_I2S_ReceiveData(SPI2); 
//		
//}

//
UINT8 ADC24Bit_SPI_ReadWriteByte(UINT8 nData)
{
//	//__disable_irq();
//    while (SPI_I2S_GetFlagStatus(ADC24BIT_SPI, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
//	SPI_I2S_SendData(ADC24BIT_SPI, nData);	
//	//__enable_irq();
	
	UINT8 nRetry = 0;
	while (SPI_I2S_GetFlagStatus(ADC24BIT_SPI, SPI_I2S_FLAG_TXE) == RESET)
	{
		nRetry ++;
		if(nRetry > 200) return 0;
	}
	nRetry = 0;
	SPI_I2S_SendData(ADC24BIT_SPI, nData);	
	//
	while(SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)
	{
		nRetry ++;
		if(nRetry > 200) return 0;
	}
	nRetry = 0;
	return SPI_I2S_ReceiveData(SPI2); 
	
}


void SPI_Write(UINT8 *pBuf, UINT8 nCount)
{
	UINT8 i, nVal;
#if AD7799_USE_SPI_COMMUNICATION
	for(i = 0; i < nCount; i++)
	{
		nVal = *(pBuf + i);
		ADC24Bit_SPI_ReadWriteByte(nVal);	
	}	
#else
	UINT8 j;
	
	__disable_irq();
	AD_SCK_1();
	Delay_US(20);//__nop();
//	AD_CS_1();  /////////////
//	Delay_US(10);//__nop();
	AD_CS_0();
	Delay_US(20);

	for(i=0;i<nCount;i++)
 	{
	 	nVal = *(pBuf + i);
		for(j=0; j<8; j++)
		{
			AD_SCK_0();
			if(0x80 == (nVal & 0x80))
			{
				AD_DI_1();	  //Send one to SDO pin
			}
			else
			{
				AD_DI_0();	  //Send zero to SDO pin
			}
			Delay_US(15);//__nop();
			AD_SCK_1();
			Delay_US(15);//__nop();
			nVal <<= 1;	//Rotate data
		}
	}
	AD_CS_1();
	//Delay_US(2);
	__enable_irq();
	
#endif
	
}


void SPI_Read(UINT8 *pBuf, UINT8 nCount)
{
	
	UINT8 i, j, nVal;
#if AD7799_USE_SPI_COMMUNICATION
	for(i = 0; i < nCount; i++)
	{
		nVal = ADC24Bit_SPI_ReadWriteByte(0);
		*(pBuf + i) = nVal;
	}	
#else
	UINT8 nTemp;
	
	__disable_irq();
	AD_SCK_1();
	Delay_US(20);//__nop();
//	AD_CS_1();
//	Delay_US(10);//__nop();
	AD_CS_0();
	Delay_US(20);//__nop();

	for(j=0; j<nCount; j++)
	{
		for(i=0; i<8; i++)
		{
		    AD_SCK_0();
			nVal <<= 1;		//Rotate data
			Delay_US(15);//__nop();
			nTemp = AD_DO();			//Read SDI of AD7799
			AD_SCK_1();	
			Delay_US(15);
			if(nTemp)
			{
				nVal |= 1;	
			}
			Delay_US(15);//__nop();
		}
		*(pBuf + j )= nVal;
	}	 
	AD_CS_1();
	__enable_irq();
	
#endif
	
}


UINT32 AD7799_GetRegisterValue(UINT8 regAddress, UINT8 size)
{
	UINT8 data[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	UINT32 receivedData = 0x00;	
	data[0] = AD7799_COMM_READ |  AD7799_COMM_ADDR(regAddress);
	AD7799_CS_LOW;  
	__disable_irq();
	SPI_Write(data,1);
	Delay_US(5);
	SPI_Read(data,size);
	AD7799_CS_HIGH;
	__enable_irq();
	if(size == 1)
	{
		receivedData += (data[0] << 0);
	}
	if(size == 2)
	{
		receivedData += (data[0] << 8);
		receivedData += (data[1] << 0);
	}
	if(size == 3)
	{
		receivedData += (data[0] << 16);
		receivedData += (data[1] << 8);
		receivedData += (data[2] << 0);
	}
        return receivedData;
}


void AD7799_SetRegisterValue(UINT8 regAddress,
                             UINT32 regValue, 
                             UINT8 size)
{
    UINT8 data[5] = {0x03, 0x00, 0x00, 0x00, 0x00};	
	    data[0] = AD7799_COMM_WRITE |  AD7799_COMM_ADDR(regAddress);
    if(size == 1)
    {
        data[1] = (UINT8)regValue;
    }
    if(size == 2)
    {
		data[2] = (UINT8)((regValue & 0x0000FF) >> 0);
        data[1] = (UINT8)((regValue & 0x00FF00) >> 8);
    }
    if(size == 3)
    {
		  data[3] = (UINT8)((regValue & 0x0000FF) >> 0);
		  data[2] = (UINT8)((regValue & 0x00FF00) >> 8);
          data[1] = (UINT8)((regValue & 0xFF0000) >> 16);
    }
    AD7799_CS_LOW;	    
    SPI_Write(data,(1 + size));
    AD7799_CS_HIGH;
}


void AD7799_Reset(void)
{
	UINT8 dataToSend[5] = {0x03, 0xff, 0xff, 0xff, 0xff};
	AD7799_CS_LOW;	    
	SPI_Write(dataToSend,4);
	AD7799_CS_HIGH;	
}


//void AD7799_SetReference(UINT8 state)
//{
//    UINT32 command = 0;
//    command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
//    command &= ~AD7799_CONF_REFDET(1);
//    command |= AD7799_CONF_REFDET(state);
//    AD7799_SetRegisterValue(AD7799_REG_CONF,  command, 2);
//}



//UINT8 AD7799_Init(void)
//{ 
//	UINT8 status = 0x01;
//	u32 ID=AD7799_GetRegisterValue(AD7799_REG_ID, 1);
//	if( (ID& 0x0F) != AD7799_ID)
//	{
//		status = 0x0;
//		printf("AD7799 not ready at init\r\n");
//	}
//	
//	return(status);
//}


void AD7799_Calibrate(void)
{
	AD7799_SetRegisterValue(AD7799_REG_CONF,0x2030,2);//�ڲ�ͨ��У׼
	AD7799_SetRegisterValue(AD7799_REG_MODE,0x9005,2);IT_SYS_DlyMs(5);
	AD7799_SetRegisterValue(AD7799_REG_MODE,0xb005,2);IT_SYS_DlyMs(5);
	AD7799_SetRegisterValue(AD7799_REG_MODE,0xd005,2);IT_SYS_DlyMs(5);
	AD7799_SetRegisterValue(AD7799_REG_MODE,0xf005,2);IT_SYS_DlyMs(5);
	
	AD7799_SetRegisterValue(AD7799_REG_CONF,0x2031,2);
	AD7799_SetRegisterValue(AD7799_REG_MODE,0x9005,2);IT_SYS_DlyMs(5);
	AD7799_SetRegisterValue(AD7799_REG_MODE,0xb005,2);IT_SYS_DlyMs(5);
	AD7799_SetRegisterValue(AD7799_REG_MODE,0xd005,2);IT_SYS_DlyMs(5);
	AD7799_SetRegisterValue(AD7799_REG_MODE,0xf005,2);IT_SYS_DlyMs(5);
	AD7799_SetRegisterValue(AD7799_REG_CONF,0x2032,2);
	AD7799_SetRegisterValue(AD7799_REG_MODE,0x9005,2);IT_SYS_DlyMs(5);
	AD7799_SetRegisterValue(AD7799_REG_MODE,0xb005,2);IT_SYS_DlyMs(5);
	AD7799_SetRegisterValue(AD7799_REG_MODE,0xd005,2);IT_SYS_DlyMs(5);
	AD7799_SetRegisterValue(AD7799_REG_MODE,0xf005,2);IT_SYS_DlyMs(5);
	
	//AD7799_SetRegisterValue(AD7799_REG_IO,0,1);//����ͨ��3 Ϊad����
}



UINT8 AD7799_Init(void)
{
	UINT8 i;
    IO_ UINT32 nCmdM, nCmdM_Value, nCmdC, nCmdC_Value,ID;
#if !AD7799_USE_SPI_COMMUNICATION	
	AD_CS_0();
	Delay_US(5);
	AD_DI_1();
	Delay_US(5);
	AD_SCK_1();
	IT_SYS_DlyMs(200);

#else
	ID=AD7799_GetRegisterValue(AD7799_REG_ID, 1);
	if( (ID& 0x0F) != AD7799_ID)
	{
		printf("AD7799 not ready at init\r\n");
	}
#endif
	//AD7799_Calibrate();
	for(i = 0; i < 10; i++)
	{
		nCmdM_Value = AD7799_GetRegisterValue(AD7799_REG_MODE, 2);
		printf("\r\nAD_M_1: %d\r\n", (int)nCmdM_Value);
		IT_SYS_DlyMs(5);
		// mode and updateR set, continuous Coversion Mode and 50Hz Update Rate(default:16.7)
		nCmdM = AD7799_MODE_CONT | AD7799_MODE_RATE(AD7799_MODE_UPDATE_50);
		printf("Set CmdM Value=%d\r\n", (int)nCmdM);
		IT_SYS_DlyMs(5);
		AD7799_SetRegisterValue(AD7799_REG_MODE, nCmdM, 2);
		IT_SYS_DlyMs(5);
		nCmdM_Value = AD7799_GetRegisterValue(AD7799_REG_MODE, 2);
		printf("AD_M_2: %d\r\n", (int)nCmdM_Value);
		IT_SYS_DlyMs(5);
		
		nCmdC_Value = AD7799_GetRegisterValue(AD7799_REG_CONF, 2);
		printf("AD_COF_1: %d\r\n", (int)nCmdC_Value);
		IT_SYS_DlyMs(5);
		// config: Gain 0(in-amp not used,2.5V),  use buf, channel 1, unipolar
		nCmdC = AD7799_CONF_GAIN(AD7799_GAIN_CONF) | AD7799_CONF_REFDET(AD7799_REFDET_DIS)|\
			AD7799_CONF_BUF | AD7799_CH_AIN1P_AIN1M;
		printf("Set CmdC Value=%d\r\n", (int)nCmdC);
		IT_SYS_DlyMs(5);
		AD7799_SetRegisterValue(AD7799_REG_CONF, nCmdC, 2);
		IT_SYS_DlyMs(5);
		nCmdC_Value = AD7799_GetRegisterValue(AD7799_REG_CONF, 2);
		printf("AD_COF_2: %d\r\n", (int)nCmdC_Value);
		if(nCmdM == nCmdM_Value && nCmdC == nCmdC_Value){
			break;
		}else{
			AD7799_Reset();
		}
		IT_SYS_DlyMs(100);
	}
	return 0;
}


void AD7799_SetChannel(UINT32 channel)
{
    UINT32 command;
    command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
    command &= ~AD7799_CONF_CHAN(0xFF);
    command |= AD7799_CONF_CHAN(channel);
	
    AD7799_SetRegisterValue(AD7799_REG_CONF, command, 2);
}

void AD7799_SetMode(UINT32 nMode, UINT8 nRate)
{
    unsigned long command;
    command = AD7799_GetRegisterValue(AD7799_REG_MODE,2);
    command &= ~AD7799_MODE_SEL(0xFF); 
    command |= AD7799_MODE_SEL(nMode);
	command &= 0XFFF0;
	command |= nRate;			//�������
	
    AD7799_SetRegisterValue(AD7799_REG_MODE, command, 2);
}

void AD7799_SetGain(UINT32 nGain)
{
    unsigned long command;
    command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
	
    command &= ~AD7799_CONF_GAIN(0xFF);
    command |= AD7799_CONF_GAIN(nGain);
    AD7799_SetRegisterValue(AD7799_REG_CONF, command, 2);
}

void AD7799_SetBurnoutCurren(UINT8 nOpt)//����BO
{
    unsigned long command;
    command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
	command &= ~0X2000;
	
	if(nOpt)
		command |= 0X2000;
	
    AD7799_SetRegisterValue(AD7799_REG_CONF, command, 2);
}

void AD7799_SetBufMode(u8 nOpt)		//����buf	
{
    unsigned long command;
    command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
	command &= ~0X10;
	
	if(nOpt)
		command |= 0X10;
    AD7799_SetRegisterValue(AD7799_REG_CONF, command, 2);
}

UINT32 AD7799_Get_Out_Data(void)
{
	return AD7799_GetRegisterValue(AD7799_REG_DATA,3);
}


UINT32 AD7799_Get_ADC_Value(UINT32 nData)
{
	UINT32  nADC;
	//nData = AD7799_GetRegisterValue(AD7799_REG_DATA,3);
	nADC = 2*(nData - 0x7FFFFF)/AD7799_GAIN_VALUE;
}


double AD7799_Get_Value(UINT32 nData)
{
	UINT32 value = (nData - 0X800000);
	return (float)((float)value*(float)AD7799_REF_MV)/(0X800000*AD7799_GAIN_VALUE);	//0X800000:2.048V    0X000000:0V

//	return (float)(((float)(nData/0x7FFFFF)-1)*AD7799_REF_MV/AD7799_GAIN_VALUE);
}




























































