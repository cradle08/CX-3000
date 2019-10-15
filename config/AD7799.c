
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
	  GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;
	  GPIO_Init(ADC24BIT_CLK_PORT, &GPIO_InitStructure);
	  // mosi
	  GPIO_InitStructure.GPIO_Pin	 = ADC24BIT_MOSI_PIN; 
	  GPIO_InitStructure.GPIO_Mode 	 = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType	 = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;
	  GPIO_Init(ADC24BIT_MOSI_PORT, &GPIO_InitStructure);
	  // miso
	  GPIO_InitStructure.GPIO_Pin	 = ADC24BIT_MISO_PIN; 
	  GPIO_InitStructure.GPIO_Mode 	 = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType	 = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;
	  GPIO_Init(ADC24BIT_MISO_PORT, &GPIO_InitStructure);
	  // cs
	  GPIO_InitStructure.GPIO_Pin	 = ADC24BIT_CS_PIN; 
	  GPIO_InitStructure.GPIO_Mode 	 = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_OType	 = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_Speed  = GPIO_Speed_100MHz;
	  GPIO_InitStructure.GPIO_PuPd   = GPIO_PuPd_UP;
	  GPIO_Init(ADC24BIT_CS_PORT, &GPIO_InitStructure);
	
	  //GPIO_PinAFConfig(D_REGISTER_CLK_PORT,GPIO_PinSource3,GPIO_AF_SPI1); 
	  //GPIO_PinAFConfig(D_REGISTER_MOSI_PORT,GPIO_PinSource4,GPIO_AF_SPI1); 
	  GPIO_PinAFConfig(ADC24BIT_CLK_PORT,  ADC24BIT_CLK_AF_SRC,	   ADC24BIT_SPI_AF); 
	  GPIO_PinAFConfig(ADC24BIT_MOSI_PORT, ADC24BIT_MOSI_AF_SRC,   ADC24BIT_SPI_AF);
	  GPIO_PinAFConfig(ADC24BIT_MISO_PORT, ADC24BIT_MISO_AF_SRC,   ADC24BIT_SPI_AF); 
 	  GPIO_PinAFConfig(ADC24BIT_CS_PORT,   ADC24BIT_CS_AF_SRC,     ADC24BIT_SPI_AF); 
	  
	  RCC_APB1PeriphResetCmd(ADC24BIT_SPI_SRC, ENABLE);
	  RCC_APB1PeriphResetCmd(ADC24BIT_SPI_SRC, DISABLE);

	  SPI_Cmd(ADC24BIT_SPI, DISABLE);
	  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	  SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	  SPI_Init(ADC24BIT_SPI, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
	 
	  SPI_Cmd(ADC24BIT_SPI, ENABLE); //ʹ��SPI����
}


void ADC24Bit_Init(void)
{
	ADC24Bit_SPI_Init();
	AD7799_Init();
}

UINT16 ADC24Bit_SPI_GetByte(void)
{
    while (SPI_I2S_GetFlagStatus(ADC24BIT_SPI, SPI_I2S_FLAG_RXNE) == RESET){}//�ȴ���������  
	return SPI_I2S_ReceiveData(ADC24BIT_SPI);	   	
}

//
void ADC24Bit_SPI_SendByte(UINT8 nData)
{
    while (SPI_I2S_GetFlagStatus(ADC24BIT_SPI, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	SPI_I2S_SendData(ADC24BIT_SPI, nData);	
}


void SPI_Write(UINT8 *pBuf, UINT8 nCount)
{
	UINT8 i, nVal;
	for(i = 0; i < nCount; i++)
	{
		nVal = *(pBuf + i);
		ADC24Bit_SPI_SendByte(nVal);	
	}	
}


void SPI_Read(UINT8 *pBuf, UINT8 nCount)
{
	UINT8 i, nVal;
	for(i = 0; i < nCount; i++)
	{
		nVal = ADC24Bit_SPI_GetByte();
		*(pBuf + i) = nVal;
	}	
	
}
			 


UINT32 AD7799_GetRegisterValue(UINT8 regAddress, UINT8 size)
{
	UINT8 data[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
	UINT32 receivedData = 0x00;	
	data[0] = AD7799_COMM_READ |  AD7799_COMM_ADDR(regAddress);
	AD7799_CS_LOW;  
	SPI_Write(data,1);
	SPI_Read(data,size);
	AD7799_CS_HIGH;
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


void AD7799_SetReference(UINT8 state)
{
    UINT32 command = 0;
    command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
    command &= ~AD7799_CONF_REFDET(1);
    command |= AD7799_CONF_REFDET(state);
    AD7799_SetRegisterValue(AD7799_REG_CONF,  command, 2);

}



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
	
	AD7799_SetRegisterValue(AD7799_REG_IO,0,1);//����ͨ��3 Ϊad����
}



UINT8 AD7799_Init(void)
{
    UINT32 command, ID;
	
	ID=AD7799_GetRegisterValue(AD7799_REG_ID, 1);
	if( (ID& 0x0F) != AD7799_ID)
	{
		printf("AD7799 not ready at init\r\n");
	}
	AD7799_Calibrate();
	
    command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
    command &= ~AD7799_CONF_GAIN(0xFF);
     command |= AD7799_CONF_GAIN(AD7799_GAIN_2);  //command |= AD7799_CONF_GAIN(1);
    AD7799_SetRegisterValue(AD7799_REG_CONF,command,2);
    AD7799_SetReference(1);
    command = AD7799_GetRegisterValue(AD7799_REG_CONF,2);
    command &= ~AD7799_CONF_CHAN(0xFF);
    command |= AD7799_CONF_CHAN(AD7799_CH_AIN1P_AIN1M); //
    AD7799_SetRegisterValue(AD7799_REG_CONF,command,2);
    command = AD7799_GetRegisterValue(AD7799_REG_MODE,2);
    command &= ~AD7799_MODE_SEL(0xFF);
    command |= AD7799_MODE_SEL(AD7799_MODE_CONT); // continuous
    AD7799_SetRegisterValue(AD7799_REG_MODE,command,2);
	return 0;
}


UINT32 AD7799_Get_ADC_Value(void)
{
	return AD7799_GetRegisterValue(AD7799_REG_DATA,3);
}






























































