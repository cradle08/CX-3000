// InterruptBufferCircle.c

#ifndef  __INTERRUPT_BUFFER_CIRCLE_C__
#define  __INTERRUPT_BUFFER_CIRCLE_C__


#include "ChainHeader.h"



//-----------------------------------------------------------------------------------------
// variable definition

// store the chars which are fetched from the flash
_STA_ IO_ struct tBufferCircle XRAM_ m_int_tBufStatus = {0, 1}; // , e_False}; // attention: initialized value
_STA_ IO_ UINT8  XRAM_ m_int_achBufferCircle[INTERRUPT_BUFFER_CIRCLE_LENGTH];


//-----------------------------------------------------------------------------------------
// function definition


//------------- buffer circle for sending chars ------------------------
//
UINT8  INT_BUF_Init(void)
{
    // attention: 1. nGet < INTERRUPT_BUFFER_CIRCLE_LENGTH
    //            2. nPut < INTERRUPT_BUFFER_CIRCLE_LENGTH
    //            3. empty: nPut = nGet + 1
    m_int_tBufStatus.nGet  = 0;
	m_int_tBufStatus.nPut  = 1;        
	// m_int_tBufStatus.eBusy = e_False;

	return e_Feedback_Success;
}

//
UINT8  INT_BUF_PutChar(UINT8 ch)
{
#if 0
    // busy
    if(e_True == m_int_tBufStatus.eBusy)
    {
        return e_Feedback_Fail;
	}
	//---------------------------
	m_int_tBufStatus.eBusy = e_True;
#endif

	// the buffer is full
	if(m_int_tBufStatus.nPut == m_int_tBufStatus.nGet)  
	{
	    // m_int_tBufStatus.eBusy = e_False;
        return e_Feedback_Fail;
	}	
    // 1.
    m_int_achBufferCircle[m_int_tBufStatus.nPut] = ch;
	// 2.
	m_int_tBufStatus.nPut += 1; // inc 1
	if(m_int_tBufStatus.nPut >= INTERRUPT_BUFFER_CIRCLE_LENGTH)
	{
        m_int_tBufStatus.nPut = 0; // circle
	}
    //
	// m_int_tBufStatus.eBusy = e_False;
    //---------------------------
    
    return e_Feedback_Success;
}

//
UINT8  INT_BUF_GetChar(UINT8* pch)
{
#if 0
    // busy
    if(e_True == m_int_tBufStatus.eBusy)
    {
        return e_Feedback_Fail;
	}
	//---------------------------
	m_int_tBufStatus.eBusy = e_True;
#endif

	// the buffer is empty
	if( ( m_int_tBufStatus.nPut == (m_int_tBufStatus.nGet+1) ) ||  
        ( (0==m_int_tBufStatus.nPut) && ((INTERRUPT_BUFFER_CIRCLE_LENGTH-1)==m_int_tBufStatus.nGet) ) )
	{
	    // m_int_tBufStatus.eBusy = e_False;
        return e_Feedback_Fail;
	}
    // 1.
    m_int_tBufStatus.nGet += 1; // inc 1
	if(m_int_tBufStatus.nGet >= INTERRUPT_BUFFER_CIRCLE_LENGTH)
	{
        m_int_tBufStatus.nGet = 0; // circle
	}
	// 2.
	*pch = m_int_achBufferCircle[m_int_tBufStatus.nGet];
	// m_int_tBufStatus.eBusy = e_False;
	//--------------------------- 
	
    return e_Feedback_Success;
}

//
UINT8  INT_BUF_PutNChar(UINT8* pchBuf, UINT16 nLen)
{
    IO_ UINT16 XRAM_ nCount = 0;
#if 0	
    // busy
    if(e_True == m_int_tBufStatus.eBusy)
    {
        return e_Feedback_Fail;
	}
	//--------------------------- 
	m_int_tBufStatus.eBusy = e_True;
#endif

	nCount = INT_BUF_GetLenRemain();
	if(nLen > nCount) // the buffer remained is not enough 
	{
        // m_int_tBufStatus.eBusy = e_False;
		return e_Feedback_Fail;
	}
	for(nCount = 0; nCount < nLen; nCount++)
	{
	    // 1.
        m_int_achBufferCircle[m_int_tBufStatus.nPut] = *(pchBuf + nCount);
		// 2.
		m_int_tBufStatus.nPut += 1; // inc 1
		if(m_int_tBufStatus.nPut >= INTERRUPT_BUFFER_CIRCLE_LENGTH)
		{
            m_int_tBufStatus.nPut = 0;  // circle
		}
		// printf("+");  ---------for testing-----------
	}
	// 
	// m_int_tBufStatus.eBusy = e_False;
	//--------------------------- 
	
    return e_Feedback_Success;
}

//
UINT8  INT_BUF_GetNChar(UINT8* pchBuf, UINT16 nLen)
{
    IO_ UINT16 XRAM_ nCount = 0;
#if 0	
    // busy
    if(e_True == m_int_tBufStatus.eBusy)
    {
        return e_Feedback_Fail;
	}
	//--------------------------- 
	m_int_tBufStatus.eBusy = e_True;
#endif

	nCount = INT_BUF_GetLenConsume();
	if(nLen > nCount) // the buffer consumed is not enough 
	{
        // m_int_tBufStatus.eBusy = e_False;
		return e_Feedback_Fail;
	}
	for(nCount = 0; nCount < nLen; nCount++)
	{   
	    // 1.
		m_int_tBufStatus.nGet += 1; // inc 1
		if(m_int_tBufStatus.nGet >= INTERRUPT_BUFFER_CIRCLE_LENGTH)
		{
            m_int_tBufStatus.nGet = 0; // circle
		}
		// 2.
		*(pchBuf + nCount) = m_int_achBufferCircle[m_int_tBufStatus.nGet];
	}
	// 
	// m_int_tBufStatus.eBusy = e_False;
	//--------------------------- 
	
    return e_Feedback_Success;
}

//
UINT16 INT_BUF_GetLenRemain(void)
{
    IO_ UINT16 XRAM_ nLen = 0;
    // attention: the flag ".eBusy" is not checked here !
    
#if (1 == 0)
	// 1. the buffer is full
    if(m_int_tBufStatus.nPut == m_int_tBufStatus.nGet)                   
    {
        return 0;
	}
	// 2. the buffer is empty
	else if( ( m_int_tBufStatus.nPut == (m_int_tBufStatus.nGet+1) ) ||   
             ( (0==m_int_tBufStatus.nPut) && ((INTERRUPT_BUFFER_CIRCLE_LENGTH-1)==m_int_tBufStatus.nGet) ) )
	{
        return (INTERRUPT_BUFFER_CIRCLE_LENGTH-1);
	}
	// 3. the buffer is part used
	else 
	{
	    // attention: for buffer-full definition: .nPut = .nGet,
		//            and the D-value have no changes
        if(m_int_tBufStatus.nPut > (m_int_tBufStatus.nGet+1) )
        {
            nLen = (INTERRUPT_BUFFER_CIRCLE_LENGTH + m_int_tBufStatus.nGet) - m_int_tBufStatus.nPut;
		}
		else  // .nPut < .nGet
		{
            nLen = m_int_tBufStatus.nGet - m_int_tBufStatus.nPut;
		}
	}
#else
    nLen = INT_BUF_GetLenConsume();
    return ((INTERRUPT_BUFFER_CIRCLE_LENGTH-1) - nLen);
#endif	
}

//
UINT16 INT_BUF_GetLenConsume(void)
{
    IO_ UINT16 XRAM_ nLen = 0;
    // attention: the flag ".eBusy" is not checked here !

	// 1. the buffer is full
    if(m_int_tBufStatus.nPut == m_int_tBufStatus.nGet)                   
    {
        return (INTERRUPT_BUFFER_CIRCLE_LENGTH-1);
	}
	// 2. the buffer is empty
	else if( ( m_int_tBufStatus.nPut == (m_int_tBufStatus.nGet+1) ) ||   
             ( (0==m_int_tBufStatus.nPut) && ((INTERRUPT_BUFFER_CIRCLE_LENGTH-1)==m_int_tBufStatus.nGet) ) )
	{
        return 0;
	}
	// 3. the buffer is part used
	else 
	{
        // attention: for buffer-emputy definition: .nPut = .nGet + 1,
        //            and the D-value must dec "1"
        if(m_int_tBufStatus.nPut > (m_int_tBufStatus.nGet+1) )
        {           
            nLen = m_int_tBufStatus.nPut - (m_int_tBufStatus.nGet+1);
		}
		else  // .nPut < .nGet
		{
			nLen = (INTERRUPT_BUFFER_CIRCLE_LENGTH+m_int_tBufStatus.nPut) - (m_int_tBufStatus.nGet+1);
		}
		//
		return nLen;
	}
	
}



#endif





