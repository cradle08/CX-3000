// Protocol.c

#ifndef  __PROTOCOL_C__
#define  __PROTOCOL_C__


#include "KernelHeader.h"



//-----------------------------------------------------------------------------------------
// definition

// attention: the micro-definition following should be set carefully by the user
// config the com port
#define PL_CLEAN_FLAG_TI()          {USART_ClearFlag(PROTOCOL_PORT, USART_FLAG_TC);}       //  { TI0 = 0; }            
#define PL_CLEAN_FLAG_RI()          {USART_ClearFlag(PROTOCOL_PORT, USART_FLAG_RXNE);}     //  { RI0 = 0; }
//
#define PL_CHECK_FLAG_TI()          ((RESET == USART_GetFlagStatus(PROTOCOL_PORT, USART_FLAG_TC))? 0: 1)     //  ( 1 == TI0 ? 1: 0 ) // attention: parentheses
#define PL_CHECK_FLAG_RI()          ((RESET == USART_GetFlagStatus(PROTOCOL_PORT, USART_FLAG_RXNE))? 0: 1)   //  ( 1 == RI0 ? 1: 0 )
#define PL_CHECK_FLAG_TXE()         ((RESET == USART_GetFlagStatus(PROTOCOL_PORT, USART_FLAG_TXE))? 0: 1)    // is transmit-buffer empty?
//
#define PL_ENABLE_TX_INT()          {USART_ITConfig(PROTOCOL_PORT, USART_IT_TC, ENABLE);}     //  { ES0 = 1; }
#define PL_DISABLE_TX_INT()         {USART_ITConfig(PROTOCOL_PORT, USART_IT_TC, DISABLE);}    //  { ES0 = 0; }
//
#define PL_ENABLE_RX_INT()          {USART_ITConfig(PROTOCOL_PORT, USART_IT_RXNE, ENABLE);}     //  { ES0 = 1; }
#define PL_DISABLE_RX_INT()         {USART_ITConfig(PROTOCOL_PORT, USART_IT_RXNE, DISABLE);}    //  { ES0 = 0; }
//
#define PL_SET_SBUF(CH)             {USART_SendData(PROTOCOL_PORT, CH);}                //  { SBUF0 = (CH); }
#define PL_GET_SBUF(CH)             {CH = (UINT8)USART_ReceiveData(PROTOCOL_PORT);}     //  { (CH) = SBUF0; }


//-----------------------------------------------------------------------------------------
// variables definition

//---------------------------------------
// static, only in the module
// for the net port
_STA_ IO_ enum eFlag                   XRAM_ m_NET_chBeBusy = e_False;                    // signs in the "frame-result"
_STA_ IO_ struct tProtocolBufferStatus XRAM_ m_NET_tRecvBufStatus;

//---------------------------------------
// global variables
//
IO_ UINT8 XRAM_ g_NET_achCmdRvBuf[PROTOCOL_RECV_BUF_LENGTH]; // received buffer
//
IO_ UINT8 XRAM_ g_achFbkSdBuf[PROTOCOL_SEND_BUF_LENGTH];     // transmitted buffer

IO_ UINT8 XRAM_ g_achFbkSdLogBuf[PROTOCOL_SEND_BUF_LENGTH]; 

//-----------------------------------------------------------------------------------------
// function definition: status

//
UINT8 PL_InitProtocol(void)
{
    BUF_Send_Init();
    BUF_Recv_Init();
    //
    NET_BUF_Send_Init();
    NET_BUF_Recv_Init();
    //
    PL_NET_ResetRecvComand();
    //
    return e_Feedback_Success;
}

//
UINT8 PL_CommandCheck(UINT8 * pStrCommand, UINT16 nLen, UINT8 * pResult)
{
    UINT16 nCharCount = 0;
    UINT8  nCheckResult = 0;

    for (nCharCount = 0; nCharCount < nLen; nCharCount++)
    {
        // sum
        nCheckResult += (UINT8)(*(pStrCommand + nCharCount));
    }

    *pResult = nCheckResult;

    return e_Feedback_Success;
}

//
INT16 PL_UnionTwoBytes_2(UINT8 chByteH, UINT8 chByteL)
{
    INT16 nValue = 0;

    nValue = ((chByteH << 8) + chByteL);

    return nValue;
}

UINT16 PL_UnionTwoBytes(UINT8 chByteH, UINT8 chByteL)
{
    UINT16 nValue = 0;

    nValue = (UINT16)((chByteH << 8) + chByteL);

    return nValue;
}

//
UINT32 PL_UnionFourBytes(UINT8 chByteHH, UINT8 chByteHL, UINT8 chByteLH, UINT8 chByteLL)
{
    UINT32 nValue  = 0;

    //
    nValue = (UINT32)(((UINT32)chByteHH) << 24) + (((UINT32)chByteHL) << 16) +
             (((UINT32)chByteLH) <<  8) + chByteLL;

    return nValue;
}



//-----------------------------------------------------------------------------------------
// function definition: for the uart port

////
//UINT8  PL_COM_TriggerSend(void)
//{
//    IO_ UINT8  XRAM_ ch = 'E';
//    IO_ UINT8  XRAM_ chReturn = e_Feedback_Success;

//    //---------------------------------
//    // 1. It must be added here to disable the interrupt of sending chars
//    PL_DISABLE_TX_INT();
//    // 2. checking
//    if (1 == PL_CHECK_FLAG_TXE())   // transmit buffer is empty
//    {
//        chReturn = BUF_Send_GetChar((UINT8 *)&ch);
//        if (e_Feedback_Success == chReturn)
//        {
//            PL_SET_SBUF(ch);
//        }
//        else
//        {
//            // do nothing
//        }
//    }
//    // 3. enable the interrupt of sending chars
//    PL_ENABLE_TX_INT();
//    //
//    return chReturn;
//}

//
//UINT8  PL_COM_SendChar(UINT8 ch)
//{
//#if 0
//    // --- test ---
//    // EVAL_OutputToggle(O_LED);

//    PL_DISABLE_TX_INT();
//    PL_CLEAN_FLAG_TI();
//    //
//    PL_SET_SBUF(ch);
//    //
//    while (0 == PL_CHECK_FLAG_TI());   // wait until bit "TI0" to be "1"
//    PL_CLEAN_FLAG_TI();
//    //
//    PL_ENABLE_TX_INT();
//    //
//    return e_Feedback_Success;
//#else
//    IO_ UINT8  XRAM_ chReturn = e_Feedback_Success;

//    chReturn = BUF_Send_PutChar(ch);
//    if (e_Feedback_Success == chReturn)
//    {
//        chReturn = PL_COM_TriggerSend();
//    }
//    //
//    return chReturn;
//#endif
//}

//
//UINT8  PL_COM_SendNChar(UINT8 * pStr, UINT16 nNum)
//{
//    IO_ UINT8  XRAM_ chReturn = e_Feedback_Success;

//    chReturn = BUF_Send_PutNChar(pStr, nNum);
//    if (e_Feedback_Success == chReturn)
//    {
//        chReturn = PL_COM_TriggerSend();
//    }
//    //
//    return chReturn;
//}

////
//UINT8  PL_COM_SendString(UINT8 * pStr)
//{
//    IO_ UINT8  XRAM_ chReturn = e_Feedback_Success;
//    IO_ UINT16 XRAM_ nCount   = 0;

//    while (*(pStr + nCount) != '\0')
//    {
//        chReturn = BUF_Send_PutChar(*(pStr + nCount));
//        nCount += 1;
//        //
//        if (e_Feedback_Success != chReturn)
//        {
//            break;
//        }
//    }
//    if (e_Feedback_Success == chReturn)
//    {
//        chReturn = PL_COM_TriggerSend();
//    }
//    //
//    return chReturn;
//}

//
//UINT8  PL_COM_SendValue(UINT32 nValue)
//{
//    UINT8 chI;
//    UINT8 chJ;
//    UINT8 chByte;
//    UINT8 achStr[16];
//    UINT32 nData;

//    //
//    nData = nValue;
//    chI = 0;
//    while (nData > 0)
//    {
//        achStr[chI] = (nData % 10) + '0';
//        nData       = nData / 10;
//        chI++;
//    }
//    //
//    achStr[chI] = '\0';                   // len + 1
//    // swap the string
//    for (chJ = 0; chJ < (chI / 2); chJ++) // times: len/2
//    {
//        chByte                = achStr[chJ];
//        achStr[chJ]           = achStr[chI - 1 - chJ];
//        achStr[chI - 1 - chJ] = chByte;
//    }
//    //
//    PL_COM_SendString(achStr);
//    //
//    return e_Feedback_Success;
//}


////
//void   PL_COM_IsrHandling(void)
//{
//    IO_ UINT8 IRAM_ chRecv = 'E';
//    IO_ UINT8 IRAM_ chReturn = e_Feedback_Success;

//    // attention: when enable the interrupt of transmitting, the flag of transmit-finished
//    //			  must be clean, or it will trigger another interrupt of transmission.

//    // has received a char
//    if (1 == PL_CHECK_FLAG_RI())
//    {
//        PL_CLEAN_FLAG_RI(); 	  // clean the flag
//        PL_DISABLE_RX_INT();	  // disable interrupt
//        //----------------------
//        PL_GET_SBUF(chRecv);	  // get the char from the buffer
//        chReturn = BUF_Recv_PutChar(chRecv); // put the char in the buffer-circle of receiving chars
//        if (e_Feedback_Success != chReturn)
//        {
//            // 2014_11_29-added
//            // SYS_ErrorMark((UINT8)ERR_COMMAND_RECEIVE_OVERFLOW, 0);
//        }
//        //----------------------
//        PL_ENABLE_RX_INT(); 	  // enable the interrupt of receiving

//    } // end of "else if(1 == RI0)"

//    if (1 == PL_CHECK_FLAG_TI())
//    {
//        PL_CLEAN_FLAG_TI(); 	  // clean the flag
//        PL_DISABLE_TX_INT();	  // disable interrupt
//        //----------------------
//        if (1 == PL_CHECK_FLAG_TXE()) // 2015_03_05-17shi-added, load the data when the buffer is empty
//        {
//            // or it will ignore some chars when sending chars too flast.
//            chReturn = BUF_Send_GetChar((UINT8 *)&chRecv);
//            //
//            if (e_Feedback_Success == chReturn)
//            {
//                PL_SET_SBUF(chRecv);  // perpare to send
//            }
//        }
//        //----------------------
//        PL_ENABLE_TX_INT(); 	  // enable interrupt
//    }

//    // test  --- trigger led output
//    //

//    return;
//}

//-----------------------------------------------------------------------------------------
// function definition: for the net port
//
UINT8  PL_NET_SendChar(UINT8 ch)
{
    IO_ UINT8  XRAM_ chReturn = e_Feedback_Success;

    chReturn = NET_BUF_Send_PutChar(ch);
    if (e_Feedback_Success != chReturn)
    {
        // do nothing
        // printf("\r\n NET send error");
    }
    //
    return chReturn;
}

UINT8  PL_NET_SendNChar(UINT8 * pStr, UINT16 nNum)
{
    IO_ UINT8  XRAM_ chReturn = e_Feedback_Success;

    chReturn = NET_BUF_Send_PutNChar(pStr, nNum);
    if (e_Feedback_Success != chReturn)
    {
        // do nothing
        // printf("\r\n NET send N error");
    }
    //
    return chReturn;
}

UINT8  PL_NET_SendString(UINT8 * pStr)
{
    IO_ UINT8  XRAM_ chReturn = e_Feedback_Success;
    IO_ UINT16 XRAM_ nCount   = 0;

    while (*(pStr + nCount) != '\0')
    {
        chReturn = NET_BUF_Send_PutChar(*(pStr + nCount));
        nCount += 1;
        //
        if (e_Feedback_Success != chReturn)
        {
            break;
        }
    }
    if (e_Feedback_Success != chReturn)
    {
        // do nothing
        // printf("\r\n NET send str error");
    }
    //
    return chReturn;
}

#if 1
//
UINT8  PL_NET_SendResult(UINT8 chType, UINT32 nCommand, UINT8 * pchData, UINT16 nDataLen) // feedback
{
    //-----------------------------------------------
    // the frame consists of the following:
    //	  content		   length		 range
    // 1) header 1:		   1 byte		 0
    // 2) header 2:		   1 byte		 1
    // 3) header 3:		   1 byte		 2
    // 4) header type:	   1 byte		 3
    // 5) command 1:	   1 byte		 4
    // 5) command 2:	   1 byte		 5
    // 5) command 3:	   1 byte		 6
    // 5) command 4:	   1 byte		 7
    // 6) data content:    N byte		 ~ 7+N

    UINT8  chReturn;
    UINT16 nNumCount = 0;
    //
    m_NET_chBeBusy = e_True;
    //
    g_NET_achCmdRvBuf[0] = (UINT8)PROTOCOL_HEAD_SEND_01;
    g_NET_achCmdRvBuf[1] = (UINT8)PROTOCOL_HEAD_SEND_02;
    g_NET_achCmdRvBuf[2] = (UINT8)PROTOCOL_HEAD_SEND_03;
    g_NET_achCmdRvBuf[3] = (UINT8)chType;
    //
    g_NET_achCmdRvBuf[4] = (UINT8)((nCommand >> 24) & 0x000000FF);
    g_NET_achCmdRvBuf[5] = (UINT8)((nCommand >> 16) & 0x000000FF);
    g_NET_achCmdRvBuf[6] = (UINT8)((nCommand >>  8) & 0x000000FF);
    g_NET_achCmdRvBuf[7] = (UINT8)((nCommand >>  0) & 0x000000FF);
    //
    for (nNumCount = 0; nNumCount < nDataLen; nNumCount++)
    {
        g_NET_achCmdRvBuf[8 + nNumCount] = *(pchData + nNumCount);
    }
    //
    chReturn = udp_echoserver_senddata((UINT8 *)&g_NET_achCmdRvBuf[0], (nDataLen + 8));
    if (e_Feedback_Success != chReturn)
    {
		//yaolan_20191112
        //PL_COM_SendString("*** Frame back error ! ***\r\n");
    }
    else
    {
        // frame back success !
//		for(nNumCount = 0; nNumCount < 8; nNumCount++)
//		{
//			printf("%X", g_NET_achCmdRvBuf[nNumCount]);
//		}
    }
    //
    m_NET_chBeBusy = e_False;
    //
    return e_Feedback_Success;
}
#endif

void Debug_Send_String(UINT8 *pStr, UINT16 nLen)
{
	// UINT8  chReturn;
	//UINT8 buf[30] = {0};
	m_NET_chBeBusy = e_True;
	memcpy((char*)g_NET_achCmdRvBuf, pStr, nLen);
	//chReturn = udp_echoserver_senddata(buf, strlen((char*)buf));
	udp_echoserver_senddata((UINT8*)g_NET_achCmdRvBuf, strlen((char*)g_NET_achCmdRvBuf));
	 m_NET_chBeBusy = e_False;
}

void Debug_Send_32(UINT32 nValue)
{
	UINT16 i = 0;
	m_NET_chBeBusy = e_True;
    g_NET_achCmdRvBuf[i++] = nValue >> 24;
	g_NET_achCmdRvBuf[i++] = nValue >> 16;
	g_NET_achCmdRvBuf[i++] = nValue >> 8;
	g_NET_achCmdRvBuf[i++] = nValue;

	udp_echoserver_senddata((UINT8 *)&g_NET_achCmdRvBuf[0], i);
	m_NET_chBeBusy = e_False;
}


//
void   PL_NET_CheckingFrame(UINT8 * pchBuf)
{
    IO_ UINT8  XRAM_ ch;
    IO_ UINT16 XRAM_ nCnt;
    IO_ UINT16 XRAM_ nLength;   // record the index of reciving chars' buffer

    //
    m_NET_tRecvBufStatus.eAble = E_PL_BUF_DISABLE;
    //
    nLength = NET_BUF_Recv_GetLenConsume();
    if (nLength < 8)
    {
        NET_BUF_Recv_Init();
        return;
    }
    if (nLength >= 200)
    {
        NET_BUF_Recv_Init();
        return;
    }
    for (nCnt = 0; nCnt < nLength; nCnt++)
    {
        NET_BUF_Recv_GetChar((UINT8 *)&ch);
        *(pchBuf + nCnt) = ch;
    }
    //
    if ((PROTOCOL_HEAD_RECV_01 != *(pchBuf + 0)) ||
            (PROTOCOL_HEAD_RECV_02 != *(pchBuf + 1)) ||
            (PROTOCOL_HEAD_RECV_03 != *(pchBuf + 2)))
    {
        NET_BUF_Recv_Init();
        return;
    }
    //
    if ((PROTOCOL_HEAD_RECV_WR != *(pchBuf + 3)) &&
            (PROTOCOL_HEAD_RECV_RD != *(pchBuf + 3)))
    {
        NET_BUF_Recv_Init();
        return;
    }

    //
    m_NET_tRecvBufStatus.eAble = E_PL_BUF_ENABLE;

    // test  --- trigger led output
    //
    NET_BUF_Recv_Init();

    return;
}


//
enum eFlag PL_NET_IsFrameTransmitting(void)
{
    return m_NET_chBeBusy;  // when equal to "e_True", it means a frame is transmitting
}

//
UINT8 PL_NET_ResetRecvComand(void)
{
    m_NET_tRecvBufStatus.eAble      = E_PL_BUF_DISABLE;
    //
    return e_Feedback_Success;
}


//
enum eProtocolBufAble PL_NET_IsRecvCommandValid(void)
{
    return m_NET_tRecvBufStatus.eAble;
}


#endif

