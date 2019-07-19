// SamplingPort.c

#ifndef  __SAMPLING_PORT_C__
#define  __SAMPLING_PORT_C__


#include "KernelHeader.h"



//-----------------------------------------------------------------------------------------
// definition

// attention: the micro-definition following should be set carefully by the user

// config the com port
#define SPG_CLEAN_FLAG_TI()          {USART_ClearFlag(SAMPLE_PORT, USART_FLAG_TC);}       //  { TI0 = 0; }            
#define SPG_CLEAN_FLAG_RI()          {USART_ClearFlag(SAMPLE_PORT, USART_FLAG_RXNE);}     //  { RI0 = 0; }
//
#define SPG_CHECK_FLAG_TI()          ((RESET == USART_GetFlagStatus(SAMPLE_PORT, USART_FLAG_TC))? 0: 1)     //  ( 1 == TI0 ? 1: 0 ) // attention: parentheses
#define SPG_CHECK_FLAG_RI()          ((RESET == USART_GetFlagStatus(SAMPLE_PORT, USART_FLAG_RXNE))? 0: 1)   //  ( 1 == RI0 ? 1: 0 )
#define SPG_CHECK_FLAG_TXE()         ((RESET == USART_GetFlagStatus(SAMPLE_PORT, USART_FLAG_TXE))? 0: 1)    // is transmit-buffer empty?
//
#define SPG_ENABLE_TX_INT()          {USART_ITConfig(SAMPLE_PORT, USART_IT_TC, ENABLE);}     //  { ES0 = 1; }
#define SPG_DISABLE_TX_INT()         {USART_ITConfig(SAMPLE_PORT, USART_IT_TC, DISABLE);}    //  { ES0 = 0; }
//
#define SPG_ENABLE_RX_INT()          {USART_ITConfig(SAMPLE_PORT, USART_IT_RXNE, ENABLE);}     //  { ES0 = 1; }
#define SPG_DISABLE_RX_INT()         {USART_ITConfig(SAMPLE_PORT, USART_IT_RXNE, DISABLE);}    //  { ES0 = 0; }
//
#define SPG_SET_SBUF(CH)             {USART_SendData(SAMPLE_PORT, CH);}                //  { SBUF0 = (CH); }
#define SPG_GET_SBUF(CH)             {CH = (UINT8)USART_ReceiveData(SAMPLE_PORT);}     //  { (CH) = SBUF0; }


//-----------------------------------------------------------------------------------------
// variables definition

// static, only in the module
//
_STA_ IO_ struct tSampleBufferStatus XRAM_ m_tRecvBufStatus;  // struct of receiving
_STA_ IO_ UINT16 XRAM_ m_nTimeCount = 0;                        // timeout counting


// global variables
//
IO_ UINT8 XRAM_ g_SPG_achCmdRvBuf[SAMPLE_RECV_BUF_LENGTH];    // received buffer
// IO_ UINT8 XRAM_ g_SPG_achFbkSdBuf[SAMPLE_SEND_BUF_LENGTH];    // transmitted buffer



//-----------------------------------------------------------------------------------------
// function definition: status

//
UINT8 SPG_InitProtocol(void)
{
    SPG_Send_Init();
	SPG_Recv_Init();
	//
    SPG_ResetRecvComand();
   
    return e_Feedback_Success;
}

//
UINT8 SPG_ResetRecvComand(void)  
{
    m_tRecvBufStatus.eAble      = E_PL_BUF_DISABLE;
    m_tRecvBufStatus.eRecvState = E_SP_RECV_STATE_IDLE;

#if 0   // delete
    m_tRecvBufStatus.nCommandLength = 0;
    memset(g_SPG_achCmdRvBuf, 0, COMMAND_RECV_SPG_LENGTH);
#endif

    return e_Feedback_Success;
}

//
enum eProtocolBufAble SPG_IsRecvCommandValid(void)
{
    return m_tRecvBufStatus.eAble;
}

//
UINT8 SPG_GetDataLength(UINT16* nLen)
{
    *nLen = m_tRecvBufStatus.nDataLength;

    return e_Feedback_Success;   
}

//
UINT8 SPG_CommandCheck(UINT8* pStrCommand, UINT16 nLen, UINT8* pResult)
{
    UINT16 nCharCount = 0;
    UINT8  nCheckResult = 0;

    for(nCharCount = 0; nCharCount < nLen; nCharCount++)
    {
        // sum
        nCheckResult += (UINT8)(*(pStrCommand + nCharCount)); 
    }
   
    *pResult = nCheckResult;

    return e_Feedback_Success;
}

//
UINT16 SPG_UnionTwoBytes(UINT8 chByteH, UINT8 chByteL)
{
    UINT16 nValue = 0;

	nValue = (UINT16)((chByteH << 8) + chByteL);

	return nValue;
}

//
UINT32 SPG_UnionFourBytes(UINT8 chByteHH, UINT8 chByteHL, UINT8 chByteLH, UINT8 chByteLL)
{
    UINT32 nValue  = 0;

	// 
	nValue = (UINT32) (((UINT32)chByteHH ) << 24) + (((UINT32)chByteHL ) << 16) + 
	                  (((UINT32)chByteLH ) << 8) + chByteLL;

	return nValue;
}



//-----------------------------------------------------------------------------------------
// function definition: transmit-receive

//
UINT8  SPG_TriggerSend(void)
{
    IO_ UINT8  XRAM_ ch = 'E';
	IO_ UINT8  XRAM_ chReturn = e_Feedback_Success;

    //---------------------------------
    // 1. It must be added here to disable the interrupt of sending chars
    SPG_DISABLE_TX_INT();
	// 2. checking
    if( 1 == SPG_CHECK_FLAG_TXE() )  // transmit buffer is empty
    {
    	chReturn = SPG_Send_GetChar((UINT8*)&ch);
        if( e_Feedback_Success == chReturn )
        {
            SPG_SET_SBUF(ch);
    	}
    	else
    	{
            // do nothing
    	}
    }
	// 3. enable the interrupt of sending chars
	SPG_ENABLE_TX_INT();

	return chReturn;
}

//
UINT8  SPG_SendChar(UINT8 ch)
{
    IO_ UINT8  XRAM_ chReturn = e_Feedback_Success;
    
    chReturn = SPG_Send_PutChar(ch);
	if( e_Feedback_Success == chReturn)
	{
        chReturn = SPG_TriggerSend();
	}
	
	return chReturn;    
}

//
UINT8  SPG_SendNChar(UINT8* pStr, UINT16 nNum)
{
    IO_ UINT8  XRAM_ chReturn = e_Feedback_Success;
	
	chReturn = SPG_Send_PutNChar(pStr, nNum);
	if( e_Feedback_Success == chReturn)
	{
        chReturn = SPG_TriggerSend();
	}
   
    return chReturn;

}

//
UINT8  SPG_SendString(UINT8* pStr)
{
	IO_ UINT8  XRAM_ chReturn = e_Feedback_Success;
	IO_ UINT16 XRAM_ nCount   = 0;
   
	while(*(pStr + nCount) != '\0')
	{
		chReturn = SPG_Send_PutChar(*(pStr + nCount));		
		nCount += 1;
		//
		if( e_Feedback_Success != chReturn )
		{
			break;
		}
	}
	if( e_Feedback_Success == chReturn)
	{
		chReturn = SPG_TriggerSend();
	}
   
	return chReturn;
}


//-----------------------------------------------------------------------------------------
// function definition: system
#if  0
//
void   SPG_WaitingFrame(void) // waiting a frame
{ 
	_STA_ IO_ UINT8  IRAM_ ch = 'E';	 
	_STA_ IO_ UINT16 IRAM_ nComandIndex = 0;   // record the index of reciving chars' buffer

	// attention: it must break out, the buffer of a command frame is using now
	if(E_PL_BUF_ENABLE == m_tRecvBufStatus.eAble)
	{
		return;
	}
	// has get a char from the buffer circle successfully
	while(e_Feedback_Success == SPG_Recv_GetChar((UINT8*)&ch))    // not BUF_Recv_GetChar
	{
#if SAMPLE_PORT_OUTPUT	
	    printf("sp - 0x%0.2X \r\n", ch);
#endif
		switch(m_tRecvBufStatus.eRecvState)
		{
			// 
			case  E_PL_RECV_STATE_IDLE:  // idle
			{
				// when enable to receive chars by the caller after handling the command
				if(E_PL_BUF_DISABLE == m_tRecvBufStatus.eAble)	  
				{
					  m_tRecvBufStatus.eRecvState = E_PL_RECV_STATE_HEAD; 
					  m_tRecvBufStatus.nDataLength = 0;
						
					  nComandIndex = 0; 	 // clean, reset to "0"
					  m_nTimeCount = 0; 	 // reset the timeout-counting	 
					  //  
					  // don't break, and continue to receive the header-char 
				}
				else
				{		
					  break;				 // break out "switch"						
				}	 
						 
			}
			//
			case  E_PL_RECV_STATE_HEAD:  // header
			{
				m_tRecvBufStatus.eAble = E_PL_BUF_DISABLE;	 // begin to receive chars, the current command is diaable
				// "0 == nComandIndex"
				g_SPG_achCmdRvBuf[nComandIndex] = ch;
				
				// attention: enhance the ability of anti-interference, check the header
				if(PROTOCOL_HEAD_CHAR != ch)
				{	 
					m_tRecvBufStatus.eRecvState = E_PL_RECV_STATE_IDLE;
					// error mark
					// SYS_ErrorMark((UINT8)ERR_COMMAND_CONFUSED_BYTE, 0);
				}
				else
				{
					m_tRecvBufStatus.eRecvState = E_PL_RECV_STATE_LENGTH;
					m_nTimeCount = 0;	   // reset the timeout-counting 
				}
				//
				break;
			}
			//
			case  E_PL_RECV_STATE_LENGTH: // length of a frame
			{
				nComandIndex += 1;
					
				g_SPG_achCmdRvBuf[nComandIndex] = ch; 
				if(ch < PROTOCOL_MIN_LENGTH) 
				{
					m_tRecvBufStatus.eRecvState = E_PL_RECV_STATE_IDLE;
				}
				else
				{
					m_tRecvBufStatus.nDataLength = ch; 
					m_tRecvBufStatus.eRecvState = E_PL_RECV_STATE_INDEX;
					m_nTimeCount = 0;	  // reset the timeout-counting  
				}
				//
				break;
			}	
			// 
			case  E_PL_RECV_STATE_INDEX: // index of a module
			{
				nComandIndex += 1;
		 
				g_SPG_achCmdRvBuf[nComandIndex] = ch;

				if( (PROTOCOL_INDEX_MIAN != ch) && (PROTOCOL_INDEX_SAMPLING != ch) )   // for only one channel?
				{
					m_tRecvBufStatus.eRecvState = E_PL_RECV_STATE_IDLE;
				}
				else
				{
					m_tRecvBufStatus.eRecvState = E_PL_RECV_STATE_COMMAND;
					m_nTimeCount = 0;	 // reset the timeout-counting 
				}
				//
				break;
			}
			//
			case  E_PL_RECV_STATE_COMMAND:	// the command
			{	
				nComandIndex += 1;	

				g_SPG_achCmdRvBuf[nComandIndex] = ch;
				
				m_tRecvBufStatus.eRecvState = E_PL_RECV_STATE_PARA;
				m_nTimeCount = 0;		 // reset the timeout-counting 
				//
				break;		
			}		
			//
			case  E_PL_RECV_STATE_PARA: // the parameters of a frame
			{
				nComandIndex += 1;	 // -1-1 = -2£¬
				if( ( nComandIndex <= (m_tRecvBufStatus.nDataLength - 2) ) && 
					( nComandIndex <= (PROTOCOL_RECV_BUF_LENGTH - 2) ) )  
				{
					g_SPG_achCmdRvBuf[nComandIndex] = ch;
					m_tRecvBufStatus.eRecvState = E_PL_RECV_STATE_PARA;
					//
					break;
				}
				else
				{
					m_tRecvBufStatus.eRecvState = E_PL_RECV_STATE_CHECK;	
					m_nTimeCount = 0;	 // reset the timeout-counting 
					//
					// don't break, and continue to receive the check-char
				}
			}
			//
			case  E_PL_RECV_STATE_CHECK:  // tail
			{
				// adding "nComandIndex" by 1 is not allowed here
				g_SPG_achCmdRvBuf[nComandIndex] = ch;
				//
				m_tRecvBufStatus.eRecvState = E_PL_RECV_STATE_IDLE;
				m_nTimeCount = 0;	   // reset the timeout-counting 
				//
				m_tRecvBufStatus.eAble		= E_PL_BUF_ENABLE;		   // has received a new frame
				// 
				// PL_SendNChar(g_SPG_achCmdRvBuf, (m_tRecvBufStatus.nCommandLength + 5) );  // testing
				// attention: when finish handing a command, the user must call "PL_ResetRecvComand()"
				//			  to enable receiving another new frame.

				// display, receive a frame from the main MCU
				// HW_LED2_Toggle();
				
				//
				break;
	
			}	 
			//
			default:
			{
				m_tRecvBufStatus.eRecvState 	= E_PL_RECV_STATE_IDLE;
				m_tRecvBufStatus.eAble			= E_PL_BUF_DISABLE;
				//
				break;
			}
		   
		} // end of switch	 

		// attention: has get a whole frame, it must break of "while"
		if( E_PL_BUF_ENABLE == m_tRecvBufStatus.eAble )
		{
			return;
		}
			   
	} // end of "while(e_Feedback_Success == BUF_Recv_GetChar(&ch))"
			   
	// test  --- trigger led output
	//	  
   
	return;
}
#else
void   SPG_WaitingFrame(void) // waiting a frame
{ 
	_STA_ IO_ UINT8  IRAM_ ch = 'E';	 
	_STA_ IO_ UINT16 IRAM_ nComandIndex = 0;   // record the index of reciving chars' buffer

	// attention: it must break out, the buffer of a command frame is using now
	if(E_PL_BUF_ENABLE == m_tRecvBufStatus.eAble)
	{
		return;
	}
	// has get a char from the buffer circle successfully
	while(e_Feedback_Success == SPG_Recv_GetChar((UINT8*)&ch))	  // not BUF_Recv_GetChar
	{
#if SAMPLE_PORT_OUTPUT	
		printf("sp - 0x%0.2X \r\n", ch);
#endif
		switch(m_tRecvBufStatus.eRecvState)
		{
			// 
			case  E_SP_RECV_STATE_IDLE:  // idle
			{
				// when enable to receive chars by the caller after handling the command
				if(E_PL_BUF_DISABLE == m_tRecvBufStatus.eAble)	  
				{
					  m_tRecvBufStatus.eRecvState = E_SP_RECV_STATE_HEAD; 
					  m_tRecvBufStatus.nDataLength = 0;
						
					  nComandIndex = 0; 	 // clean, reset to "0"
					  m_nTimeCount = 0; 	 // reset the timeout-counting	 
					  //  
					  // don't break, and continue to receive the header-char 
				}
				else
				{		
					  break;				 // break out "switch"						
				}	 
						 
			}
			//
			case  E_SP_RECV_STATE_HEAD:  // header
			{
				m_tRecvBufStatus.eAble = E_PL_BUF_DISABLE;	 // begin to receive chars, the current command is diaable
				// "0 == nComandIndex"
				g_SPG_achCmdRvBuf[nComandIndex] = ch;
				
				// attention: enhance the ability of anti-interference, check the header
				if(SAMPLE_HEAD_CHAR != ch)
				{	 
					m_tRecvBufStatus.eRecvState = E_SP_RECV_STATE_IDLE;
					// error mark
					// SYS_ErrorMark((UINT8)ERR_COMMAND_CONFUSED_BYTE, 0);
				}
				else
				{
					m_tRecvBufStatus.eRecvState = E_SP_RECV_STATE_DATA1;
					m_nTimeCount = 0;	   // reset the timeout-counting 
				}
				//
				break;
			}
			//
			case  E_SP_RECV_STATE_DATA1: // 
			{
				nComandIndex += 1;
				// 	
				g_SPG_achCmdRvBuf[nComandIndex] = ch; 
				//
				m_tRecvBufStatus.eRecvState = E_SP_RECV_STATE_DATA2;
				//
				m_nTimeCount = 0;	 // reset the timeout-counting 
				//
				break;
			}	
			// 
			case  E_SP_RECV_STATE_DATA2: // index of a module
			{
				nComandIndex += 1;
		 		// 
				g_SPG_achCmdRvBuf[nComandIndex] = ch;
				//
				m_tRecvBufStatus.eRecvState = E_SP_RECV_STATE_TAIL;
				//
				m_nTimeCount = 0;	 // reset the timeout-counting 
				//
				break;
			}	
			//
			case  E_SP_RECV_STATE_TAIL:  // tail
			{
				nComandIndex += 1;
				// 
				g_SPG_achCmdRvBuf[nComandIndex] = ch;
				//
				m_tRecvBufStatus.eRecvState = E_SP_RECV_STATE_IDLE;
				m_nTimeCount = 0;	   // reset the timeout-counting 
				//
				if(SAMPLE_TAIL_CHAR != ch)
				{
					m_tRecvBufStatus.eAble		= E_PL_BUF_DISABLE;
				}
				else
				{
					m_tRecvBufStatus.eAble		= E_PL_BUF_ENABLE;		   // has received a new frame
				}
				
				// 
				// PL_SendNChar(g_SPG_achCmdRvBuf, (m_tRecvBufStatus.nCommandLength + 5) );  // testing
				// attention: when finish handing a command, the user must call "PL_ResetRecvComand()"
				//			  to enable receiving another new frame.

				// display, receive a frame from the main MCU
				// HW_LED2_Toggle();
				
				//
				break;
	
			}	 
			//
			default:
			{
				m_tRecvBufStatus.eRecvState 	= E_SP_RECV_STATE_IDLE;
				m_tRecvBufStatus.eAble			= E_PL_BUF_DISABLE;
				//
				break;
			}
		   
		} // end of switch	 

		// attention: has get a whole frame, it must break of "while"
		if( E_PL_BUF_ENABLE == m_tRecvBufStatus.eAble )
		{
			return;
		}
			   
	} // end of "while(e_Feedback_Success == BUF_Recv_GetChar(&ch))"
			   
	// test  --- trigger led output
	//	  
   
	return;
}

#endif


//
void   SPG_IsrHandling(void) 
{ 
	IO_ UINT8 IRAM_ chRecv = 'E';  
	IO_ UINT8 IRAM_ chReturn = e_Feedback_Success;
	  
	// attention: when enable the interrupt of transmitting, the flag of transmit-finished
	//			  must be clean, or it will trigger another interrupt of transmission.

	// has received a char
	if(1 == SPG_CHECK_FLAG_RI())
	{
		SPG_CLEAN_FLAG_RI(); 	  // clean the flag
		SPG_DISABLE_RX_INT();	  // disable interrupt
		//----------------------
		SPG_GET_SBUF(chRecv);	  // get the char from the buffer
		SPG_Recv_PutChar(chRecv); // put the char in the buffer-circle of receiving chars
		 

		//----------------------   
		SPG_ENABLE_RX_INT(); 	  // enable the interrupt of receiving
		   
	} // end of "else if(1 == RI0)"
	   
	if(1 == SPG_CHECK_FLAG_TI())
	{
		SPG_CLEAN_FLAG_TI(); 	  // clean the flag
		SPG_DISABLE_TX_INT();	  // disable interrupt
		//----------------------	
		if(1 == SPG_CHECK_FLAG_TXE()) // 2015_03_05-17shi-added, load the data when the buffer is empty
        {                             // or it will ignore some chars when sending chars too flast.
            chReturn = SPG_Send_GetChar((UINT8*)&chRecv);
			//
    		if( e_Feedback_Success == chReturn )
    		{
    			SPG_SET_SBUF(chRecv);  // perpare to send		 
    		}
    	}
		//----------------------
		SPG_ENABLE_TX_INT(); 	  // enable interrupt
	}

	// test  --- trigger led output
	//	  
   
	return;
}


//
void   SPG_IsrTime(void)
{
    // when has finished receiving a frame, timeout-counting is ignore
    if(E_PL_BUF_ENABLE == m_tRecvBufStatus.eAble)
		return;

	// handling the timeout-event
    m_nTimeCount += 1;
	if(m_nTimeCount >= PROTOCOL_TIMEOUT)
	{
        m_nTimeCount = 0;
	    m_tRecvBufStatus.eRecvState = E_SP_RECV_STATE_IDLE;
		// don't enable receiving chars of the buffer here
	}

	return;
}


UINT8  SPG_Communicate(UINT8* pchBufIn, UINT16 nLenIn, UINT8* pchBufOut, UINT16* pnLenOut)
{
#if 0
    IO_ UINT8  XRAM_ chReturn  = e_Feedback_Success;
	//
	IO_ UINT8  XRAM_ chByte    = 0;
	IO_ UINT8  XRAM_ chCheck   = 0;
	IO_ UINT16 XRAM_ nFrameLen = 0;
	IO_ UINT16 XRAM_ nCount    = 0;
	//
	IO_ UINT16 XRAM_ nHalfWord1 = 0;
	IO_ UINT16 XRAM_ nHalfWord2 = 0;

	// begin to sand string to the slave mcu
	SPG_InitProtocol();  // SPG_ResetRecvComand();
	//
	chReturn = SPG_SendNChar(pchBufIn, nLenIn);
	// waiting the feedback result
	IT_SYS_SetDlyMs(1000);    // time space, 1000ms
	while(E_PL_BUF_DISABLE == SPG_IsRecvCommandValid()) 
	{
	    // waiting for a whole frame
	    SPG_WaitingFrame();
		if(e_True == IT_SYS_CheckDlyFin())
		{
		   SPG_ResetRecvComand();	   // clear the buffer
		   // error mark
		   // SYS_ErrorMark((UINT8)ERR_SLAVE_COM_OVER_TIME, 0);
		   return e_Feedback_Fail;	   // Over time, return execute fail
		   // break;
		}
	}
	// the frame length, adds extra 5 bytes
	nFrameLen = g_SPG_achCmdRvBuf[2] + 5;
	// check the ecc of the frame		
	PL_CommandCheck((UINT8*)(g_SPG_achCmdRvBuf + 1), (nFrameLen - 3), &chByte); // -2-1
	if(chByte != g_SPG_achCmdRvBuf[nFrameLen - 2])   // nFrameLen - 1 - 1 
	{
		// err
		// error mark
		// SYS_ErrorMark((UINT8)ERR_SLAVE_FRAME_CHECK, 0);
		return e_Feedback_Error;
	}
	if( BIT_CTRL_ACK_ERR == (BYTE_CTRL_ACK & g_SPG_achCmdRvBuf[1]) )
	{
		for(nCount = 0; nCount < (g_SPG_achCmdRvBuf[2] - 2); nCount = nCount + 4) // the error number
		{
		    nHalfWord2 = PL_UnionTwoBytes(g_SPG_achCmdRvBuf[5 + nCount], g_SPG_achCmdRvBuf[6 + nCount]);
			nHalfWord1 = SYS_GetTheErrIndex(nHalfWord2);
			nHalfWord2 = PL_UnionTwoBytes(g_SPG_achCmdRvBuf[7 + nCount], g_SPG_achCmdRvBuf[8 + nCount]);
            // error mark
			SYS_ErrorMark(nHalfWord1, nHalfWord2);
		}
		// record errors
		return e_Feedback_Error;
	}
	else
	{
		for(nCount = 0; nCount < (g_SPG_achCmdRvBuf[2] - 2); nCount++)
		{
            *(pchBufOut + nCount) = g_SPG_achCmdRvBuf[5 + nCount];
		}
		*pnLenOut = nCount;
	}
#endif
	return e_Feedback_Success;
}


#endif





