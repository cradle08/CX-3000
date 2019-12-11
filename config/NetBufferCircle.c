// NetBufferCircle.c

#ifndef  __NET_BUFFER_CIRCLE_C__
#define  __NET_BUFFER_CIRCLE_C__


#include "ChainHeader.h"

// status of the buffer-circle
struct tBufferCircle				  
{
    UINT16 	              nGet;       // counter, the offset address to get a char
    UINT16                nPut;       // counter, the offset address to put a char
	// enum eFlag           eBusy;       // whether the buffer-circle is busy

    // attention: 1. the buffer-circle is showing following.
    //               1). empty:  nPut = nGet + 1,
    //               2). full:   nPut = nGet,
    //               3). part:   nPut > nGet + 1  or  nPut < nGet.
    //            2. the order to handle ".nGet" and ".nPut".
    //               1). first to put a char in the buffer, then ".nPut" increase 1,
    //               2). first ".nGet" increase 1, then to get a char from the buffer.
};


//-----------------------------------------------------------------------------------------
// variable definition

// for sending chars
_STA_ IO_ struct tBufferCircle XRAM_ m_send_tBufStatus = {0, 1}; // , e_False}; // attention: initialized value
_STA_ IO_ UINT8  XRAM_ m_send_achBufferCircle[NET_BUFFER_CIRCLE_LENGTH_SEND];

// for receiving chars
_STA_ IO_ struct tBufferCircle XRAM_ m_recv_tBufStatus = {0, 1}; // , e_False}; // attention: initialized value
_STA_ IO_ UINT8  XRAM_ m_recv_achBufferCircle[NET_BUFFER_CIRCLE_LENGTH_RECV];


//-----------------------------------------------------------------------------------------
// function definition


//------------- buffer circle for sending chars ------------------------
//
UINT8  NET_BUF_Send_Init(void)
{
    // attention: 1. nGet < NET_BUFFER_CIRCLE_LENGTH_SEND
    //            2. nPut < NET_BUFFER_CIRCLE_LENGTH_SEND
    //            3. empty: nPut = nGet + 1
    m_send_tBufStatus.nGet  = 0;
	m_send_tBufStatus.nPut  = 1;        
	// m_send_tBufStatus.eBusy = e_False;

	return e_Feedback_Success;
}

//
UINT8  NET_BUF_Send_PutChar(UINT8 ch)
{
#if 0
    // busy
    if(e_True == m_send_tBufStatus.eBusy)
    {
        return e_Feedback_Fail;
	}
	//---------------------------
	m_send_tBufStatus.eBusy = e_True;
#endif

	// the buffer is full
	if(m_send_tBufStatus.nPut == m_send_tBufStatus.nGet)  
	{
	    // m_send_tBufStatus.eBusy = e_False;
        return e_Feedback_Fail;
	}	
    // 1.
    m_send_achBufferCircle[m_send_tBufStatus.nPut] = ch;
	// 2.
	m_send_tBufStatus.nPut += 1; // inc 1
	if(m_send_tBufStatus.nPut >= NET_BUFFER_CIRCLE_LENGTH_SEND)
	{
        m_send_tBufStatus.nPut = 0; // circle
	}
    //
	// m_send_tBufStatus.eBusy = e_False;
    //---------------------------
    
    return e_Feedback_Success;
}

//
UINT8  NET_BUF_Send_GetChar(UINT8* pch)
{
#if 0
    // busy
    if(e_True == m_send_tBufStatus.eBusy)
    {
        return e_Feedback_Fail;
	}
	//---------------------------
	m_send_tBufStatus.eBusy = e_True;
#endif

	// the buffer is empty
	if( ( m_send_tBufStatus.nPut == (m_send_tBufStatus.nGet+1) ) ||  
        ( (0==m_send_tBufStatus.nPut) && ((NET_BUFFER_CIRCLE_LENGTH_SEND-1)==m_send_tBufStatus.nGet) ) )
	{
	    // m_send_tBufStatus.eBusy = e_False;
        return e_Feedback_Fail;
	}
    // 1.
    m_send_tBufStatus.nGet += 1; // inc 1
	if(m_send_tBufStatus.nGet >= NET_BUFFER_CIRCLE_LENGTH_SEND)
	{
        m_send_tBufStatus.nGet = 0; // circle
	}
	// 2.
	*pch = m_send_achBufferCircle[m_send_tBufStatus.nGet];
	// m_send_tBufStatus.eBusy = e_False;
	//--------------------------- 
	
    return e_Feedback_Success;
}

//
UINT8  NET_BUF_Send_PutNChar(UINT8* pchBuf, UINT16 nLen)
{
    IO_ UINT16 XRAM_ nCount = 0;
#if 0	
    // busy
    if(e_True == m_send_tBufStatus.eBusy)
    {
        return e_Feedback_Fail;
	}
	//--------------------------- 
	m_send_tBufStatus.eBusy = e_True;
#endif

	nCount = NET_BUF_Send_GetLenRemain();
	if(nLen > nCount) // the buffer remained is not enough 
	{
        // m_send_tBufStatus.eBusy = e_False;
		return e_Feedback_Fail;
	}
	for(nCount = 0; nCount < nLen; nCount++)
	{
	    // 1.
        m_send_achBufferCircle[m_send_tBufStatus.nPut] = *(pchBuf + nCount);
		// 2.
		m_send_tBufStatus.nPut += 1; // inc 1
		if(m_send_tBufStatus.nPut >= NET_BUFFER_CIRCLE_LENGTH_SEND)
		{
            m_send_tBufStatus.nPut = 0;  // circle
		}
	}
	// 
	// m_send_tBufStatus.eBusy = e_False;
	//--------------------------- 
	
    return e_Feedback_Success;
}

//
UINT8  NET_BUF_Send_GetNChar(UINT8* pchBuf, UINT16 nLen)
{
    IO_ UINT16 XRAM_ nCount = 0;
#if 0	
    // busy
    if(e_True == m_send_tBufStatus.eBusy)
    {
        return e_Feedback_Fail;
	}
	//--------------------------- 
	m_send_tBufStatus.eBusy = e_True;
#endif

	nCount = NET_BUF_Send_GetLenConsume();
	if(nLen > nCount) // the buffer consumed is not enough 
	{
        // m_send_tBufStatus.eBusy = e_False;
		return e_Feedback_Fail;
	}
	for(nCount = 0; nCount < nLen; nCount++)
	{   
	    // 1.
		m_send_tBufStatus.nGet += 1; // inc 1
		if(m_send_tBufStatus.nGet >= NET_BUFFER_CIRCLE_LENGTH_SEND)
		{
            m_send_tBufStatus.nGet = 0; // circle
		}
		// 2.
		*(pchBuf + nCount) = m_send_achBufferCircle[m_send_tBufStatus.nGet];
	}
	// 
	// m_send_tBufStatus.eBusy = e_False;
	//--------------------------- 
	
    return e_Feedback_Success;
}

//
UINT16 NET_BUF_Send_GetLenRemain(void)
{
    IO_ UINT16 XRAM_ nLen = 0;
    // attention: the flag ".eBusy" is not checked here !
    
#if (1 == 0)
	// 1. the buffer is full
    if(m_send_tBufStatus.nPut == m_send_tBufStatus.nGet)                   
    {
        return 0;
	}
	// 2. the buffer is empty
	else if( ( m_send_tBufStatus.nPut == (m_send_tBufStatus.nGet+1) ) ||   
             ( (0==m_send_tBufStatus.nPut) && ((NET_BUFFER_CIRCLE_LENGTH_SEND-1)==m_send_tBufStatus.nGet) ) )
	{
        return (NET_BUFFER_CIRCLE_LENGTH_SEND-1);
	}
	// 3. the buffer is part used
	else 
	{
	    // attention: for buffer-full definition: .nPut = .nGet,
		//            and the D-value have no changes
        if(m_send_tBufStatus.nPut > (m_send_tBufStatus.nGet+1) )
        {
            nLen = (NET_BUFFER_CIRCLE_LENGTH_SEND + m_send_tBufStatus.nGet) - m_send_tBufStatus.nPut;
		}
		else  // .nPut < .nGet
		{
            nLen = m_send_tBufStatus.nGet - m_send_tBufStatus.nPut;
		}
	}
#else
    nLen = NET_BUF_Send_GetLenConsume();
    return ((NET_BUFFER_CIRCLE_LENGTH_SEND-1) - nLen);
#endif	
}

//
UINT16 NET_BUF_Send_GetLenConsume(void)
{
    IO_ UINT16 XRAM_ nLen = 0;
    // attention: the flag ".eBusy" is not checked here !

	// 1. the buffer is full
    if(m_send_tBufStatus.nPut == m_send_tBufStatus.nGet)                   
    {
        return (NET_BUFFER_CIRCLE_LENGTH_SEND-1);
	}
	// 2. the buffer is empty
	else if( ( m_send_tBufStatus.nPut == (m_send_tBufStatus.nGet+1) ) ||   
             ( (0==m_send_tBufStatus.nPut) && ((NET_BUFFER_CIRCLE_LENGTH_SEND-1)==m_send_tBufStatus.nGet) ) )
	{
        return 0;
	}
	// 3. the buffer is part used
	else 
	{
        // attention: for buffer-emputy definition: .nPut = .nGet + 1,
        //            and the D-value must dec "1"
        if(m_send_tBufStatus.nPut > (m_send_tBufStatus.nGet+1) )
        {           
            nLen = m_send_tBufStatus.nPut - (m_send_tBufStatus.nGet+1);
		}
		else  // .nPut < .nGet
		{
			nLen = (NET_BUFFER_CIRCLE_LENGTH_SEND+m_send_tBufStatus.nPut) - (m_send_tBufStatus.nGet+1);
		}
		//
		return nLen;
	}
	
}



//------------- buffer circle for receiving chars ------------------------
#if 1
//
UINT8  NET_BUF_Recv_Init(void)
{
    // attention: 1. nGet < NET_BUFFER_CIRCLE_LENGTH_RECV
    //            2. nPut < NET_BUFFER_CIRCLE_LENGTH_RECV
    //            3. empty: nPut = nGet + 1
    m_recv_tBufStatus.nGet  = 0;
	m_recv_tBufStatus.nPut  = 1;        
	// m_recv_tBufStatus.eBusy = e_False;

	return e_Feedback_Success;
}

//
UINT8  NET_BUF_Recv_PutChar(UINT8 ch)
{
#if 0
    // busy
    if(e_True == m_recv_tBufStatus.eBusy)
    {
        return e_Feedback_Fail;
	}
	//---------------------------
	m_recv_tBufStatus.eBusy = e_True;
#endif

	// the buffer is full
	if(m_recv_tBufStatus.nPut == m_recv_tBufStatus.nGet)  
	{	    
	    // m_recv_tBufStatus.eBusy = e_False;
        return e_Feedback_Fail;
	}	
    // 1.
    m_recv_achBufferCircle[m_recv_tBufStatus.nPut] = ch;
	// 2.
	m_recv_tBufStatus.nPut += 1; // inc 1
	if(m_recv_tBufStatus.nPut >= NET_BUFFER_CIRCLE_LENGTH_RECV)
	{
        m_recv_tBufStatus.nPut = 0; // circle
	}
    //
	// m_recv_tBufStatus.eBusy = e_False;
    //---------------------------
    
    return e_Feedback_Success;
}

//
UINT8  NET_BUF_Recv_GetChar(UINT8* pch)
{
#if 0
    // busy
    if(e_True == m_recv_tBufStatus.eBusy)
    {
        return e_Feedback_Fail;
	}
	//---------------------------
	m_recv_tBufStatus.eBusy = e_True;
#endif

	// the buffer is empty
	if( ( m_recv_tBufStatus.nPut == (m_recv_tBufStatus.nGet+1) ) ||  
        ( (0==m_recv_tBufStatus.nPut) && ((NET_BUFFER_CIRCLE_LENGTH_RECV-1)==m_recv_tBufStatus.nGet) ) )
	{
	    // m_recv_tBufStatus.eBusy = e_False;
        return e_Feedback_Fail;
	}
    // 1.
    m_recv_tBufStatus.nGet += 1; // inc 1
	if(m_recv_tBufStatus.nGet >= NET_BUFFER_CIRCLE_LENGTH_RECV)
	{
        m_recv_tBufStatus.nGet = 0; // circle
	}
	// 2.
	*pch = m_recv_achBufferCircle[m_recv_tBufStatus.nGet];
	// m_recv_tBufStatus.eBusy = e_False;
	//--------------------------- 
	
    return e_Feedback_Success;
}

//
UINT8  NET_BUF_Recv_PutNChar(UINT8* pchBuf, UINT16 nLen)
{
    IO_ UINT16 XRAM_ nCount = 0;
#if 0	
    // busy
    if(e_True == m_recv_tBufStatus.eBusy)
    {
        return e_Feedback_Fail;
	}
	//--------------------------- 
	m_recv_tBufStatus.eBusy = e_True;
#endif

	nCount = NET_BUF_Recv_GetLenRemain();
	if(nLen > nCount) // the buffer remained is not enough 
	{
        // m_recv_tBufStatus.eBusy = e_False;
		return e_Feedback_Fail;
	}
	for(nCount = 0; nCount < nLen; nCount++)
	{
	    // 1.
        m_recv_achBufferCircle[m_recv_tBufStatus.nPut] = *(pchBuf + nCount);
		// 2.
		m_recv_tBufStatus.nPut += 1; // inc 1
		if(m_recv_tBufStatus.nPut >= NET_BUFFER_CIRCLE_LENGTH_RECV)
		{
            m_recv_tBufStatus.nPut = 0;  // circle
		}
	}
	// 
	// m_recv_tBufStatus.eBusy = e_False;
	//--------------------------- 
	
    return e_Feedback_Success;
}

//
UINT8  NET_BUF_Recv_GetNChar(UINT8* pchBuf, UINT16 nLen)
{
    IO_ UINT16 XRAM_ nCount = 0;
#if 0	
    // busy
    if(e_True == m_recv_tBufStatus.eBusy)
    {
        return e_Feedback_Fail;
	}
	//--------------------------- 
	m_recv_tBufStatus.eBusy = e_True;
#endif

	nCount = NET_BUF_Recv_GetLenConsume();
	if(nLen > nCount) // the buffer consumed is not enough 
	{
        // m_recv_tBufStatus.eBusy = e_False;
		return e_Feedback_Fail;
	}
	for(nCount = 0; nCount < nLen; nCount++)
	{   
	    // 1.
		m_recv_tBufStatus.nGet += 1; // inc 1
		if(m_recv_tBufStatus.nGet >= NET_BUFFER_CIRCLE_LENGTH_RECV)
		{
            m_recv_tBufStatus.nGet = 0; // circle
		}
		// 2.
		*(pchBuf + nCount) = m_recv_achBufferCircle[m_recv_tBufStatus.nGet];
	}
	// 
	// m_recv_tBufStatus.eBusy = e_False;
	//--------------------------- 
	
    return e_Feedback_Success;
}

//
UINT16 NET_BUF_Recv_GetLenRemain(void)
{
    IO_ UINT16 XRAM_ nLen = 0;
    // attention: the flag ".eBusy" is not checked here !
    
#if (1 == 0)
	// 1. the buffer is full
    if(m_recv_tBufStatus.nPut == m_recv_tBufStatus.nGet)                   
    {
        return 0;
	}
	// 2. the buffer is empty
	else if( ( m_recv_tBufStatus.nPut == (m_recv_tBufStatus.nGet+1) ) ||   
             ( (0==m_recv_tBufStatus.nPut) && ((NET_BUFFER_CIRCLE_LENGTH_RECV-1)==m_recv_tBufStatus.nGet) ) )
	{
        return (NET_BUFFER_CIRCLE_LENGTH_RECV-1);
	}
	// 3. the buffer is part used
	else 
	{
	    // attention: for buffer-full definition: .nPut = .nGet,
		//            and the D-value have no changes
        if(m_recv_tBufStatus.nPut > (m_recv_tBufStatus.nGet+1) )
        {
            nLen = (NET_BUFFER_CIRCLE_LENGTH_RECV + m_recv_tBufStatus.nGet) - m_recv_tBufStatus.nPut;
		}
		else  // .nPut < .nGet
		{
            nLen = m_recv_tBufStatus.nGet - m_recv_tBufStatus.nPut;
		}
	}
#else
    nLen = NET_BUF_Recv_GetLenConsume();
    return ((NET_BUFFER_CIRCLE_LENGTH_RECV-1) - nLen);
#endif	
}

//
UINT16 NET_BUF_Recv_GetLenConsume(void)
{
    IO_ UINT16 XRAM_ nLen = 0;
    // attention: the flag ".eBusy" is not checked here !

	// 1. the buffer is full
    if(m_recv_tBufStatus.nPut == m_recv_tBufStatus.nGet)                   
    {
        return (NET_BUFFER_CIRCLE_LENGTH_RECV-1);
	}
	// 2. the buffer is empty
	else if( ( m_recv_tBufStatus.nPut == (m_recv_tBufStatus.nGet+1) ) ||   
             ( (0==m_recv_tBufStatus.nPut) && ((NET_BUFFER_CIRCLE_LENGTH_RECV-1)==m_recv_tBufStatus.nGet) ) )
	{
        return 0;
	}
	// 3. the buffer is part used
	else 
	{
        // attention: for buffer-emputy definition: .nPut = .nGet + 1,
        //            and the D-value must dec "1"
        if(m_recv_tBufStatus.nPut > (m_recv_tBufStatus.nGet+1) )
        {           
            nLen = m_recv_tBufStatus.nPut - (m_recv_tBufStatus.nGet+1);
		}
		else  // .nPut < .nGet
		{
			nLen = (NET_BUFFER_CIRCLE_LENGTH_RECV+m_recv_tBufStatus.nPut) - (m_recv_tBufStatus.nGet+1);
		}
		//
		return nLen;
	}
	
}

#endif





#endif



