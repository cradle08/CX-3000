/**
  ******************************************************************************
  * @file    udp_echoserver.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    31-July-2013
  * @brief   UDP echo server
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "LwipHeader.h"     // lwip configuration
#include "lwip/pbuf.h"
#include "lwip/udp.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>

#include "Protocol.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define UDP_SERVER_PORT    SERVER_PORT   /* define the UDP local connection port */
#define UDP_CLIENT_PORT    CLIENT_PORT   /* define the UDP remote connection port */


//-----------------------------------------------------------------------------------------
// variable
struct ip_addr  s_tAddr;   // attention: it can't be a pointer, but an entity


u16_t           s_nPort;
struct udp_pcb* s_ptUpcb;  // attention: it will be assigned an entity then by "udp_new()"



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize the server application.
  * @param  None
  * @retval None
  */
void   udp_echoserver_init(void)
{  
    u8  remoteip[4];   // for debuging
    err_t err;

    // attention: point to the IP of itself !
    IP4_ADDR(&s_tAddr, IP_REMOTE_ADDR0, IP_REMOTE_ADDR1, IP_REMOTE_ADDR2, IP_REMOTE_ADDR3);
    // s_tAddr.addr = (u32_t)0x4601a8c0;
	s_nPort      = UDP_CLIENT_PORT;

	// debug 
	remoteip[0]= (u8)  (s_tAddr.addr)&0xff;       // IADDR4
	remoteip[1]= (u8) ((s_tAddr.addr)>>8)&0xff;   // IADDR3
	remoteip[2]= (u8) ((s_tAddr.addr)>>16)&0xff;  // IADDR2
	remoteip[3]= (u8) ((s_tAddr.addr)>>24)&0xff;  // IADDR1 
	//
    printf("\r\n Udp_Echoserver_Init--- remote addr: 0x%x   ip: %d.%d.%d.%d  port: %d \r\n", s_tAddr.addr, remoteip[0], remoteip[1], remoteip[2], remoteip[3], s_nPort);
    // Create a new UDP control block  
    s_ptUpcb = udp_new();
    //
    if(NULL != s_ptUpcb)
    {
 #if   0    
        // Connect to the remote client 
		// --- attention: the address is not sure
		IP4_ADDR(s_ptAddr, 192, 168, 1, 70);
        
        err = udp_bind(s_ptUpcb, s_ptAddr, UDP_SERVER_PORT);
		err = udp_connect(s_ptUpcb, s_ptAddr, UDP_CLIENT_PORT);
#else		
        // Bind the upcb to the UDP_PORT port 
        // Using IP_ADDR_ANY allow the upcb to be used by any local interface 
        err = udp_bind(s_ptUpcb, IP_ADDR_ANY, UDP_SERVER_PORT);
#endif     
        if(ERR_OK == err)
        {
            // Set a receive callback for the upcb 
            udp_recv(s_ptUpcb, udp_echoserver_receive_callback, NULL);
        }
        else
        {
            udp_remove(s_ptUpcb);
            printf("\r\n Can not bind pcb ! \r\n");
        }
   }
   else
   {
       printf("\r\n Can not create pcb ! \r\n");
   } 
}

// send data by the UDP server, use the lastest ip and port
UINT8  udp_echoserver_senddata(UINT8* pchBuf, UINT16 nLen)
{
	struct pbuf *ptr;

	// allocate the memory
	ptr = pbuf_alloc(PBUF_TRANSPORT, nLen, PBUF_POOL);  
	// checking
	if(NULL != ptr)
	{
	    // assign
		ptr->payload = (void*)pchBuf; 
		//-----------------------------------------------
		// debug
		// printf("\r\n addr 0x%x, port %d\r\n", s_tAddr.addr, s_nPort);
#if 0		
		// Connect to the remote client
        udp_connect(s_ptUpcb, &s_tAddr, s_nPort);
		// send data
		udp_send(s_ptUpcb, ptr);	
		// free the UDP connection, so we can accept new clients
        udp_disconnect(s_ptUpcb);
#else
		udp_sendto(s_ptUpcb, ptr, &s_tAddr, s_nPort);
#endif
		//-----------------------------------------------
		// attention: to free the memoty
		pbuf_free(ptr); 

		//
	    return e_Feedback_Success;
	}
	else
	{
		//
	    return e_Feedback_Fail;
	}
	
} 


/**
  * @brief This function is called when an UDP datagrm has been received on the port UDP_PORT.
  * @param arg user supplied argument (udp_pcb.recv_arg)
  * @param pcb the udp_pcb which received data
  * @param p the packet buffer that was received
  * @param addr the remote IP address from which the packet was received
  * @param port the remote port from which the packet was received
  * @retval None
  */
void   udp_echoserver_receive_callback(void *arg, struct udp_pcb *upcb, struct pbuf *p, struct ip_addr *addr, u16_t port)
{
    u8  remoteip[4];    // for debuging
	u16 data_len = 0;
	struct pbuf* q;
 
	// no empty
	if(p != NULL)	
	{
		// ransack the chain of pbuf 
		for(q=p; q!=NULL; q=q->next)  
		{
		    data_len = NET_BUF_Recv_GetLenRemain();
			if(q->len > data_len) 
			{
			    // ignore some data 
				printf("\r\n the recv-buf has overruned %d ! \r\n", q->len);
		        NET_BUF_Recv_PutNChar(q->payload, data_len);
				//
			}
			else
			{	
			    // oK
			    NET_BUF_Recv_PutNChar(q->payload, q->len);
				//
				//-------------------------
				// for testing  yaolan_20191112
				//PL_COM_SendNChar(q->payload, q->len);
				//PL_COM_SendString("\r\n");
			}
		}
		//-----------------------------------------------------------
		// attention: to free the memory with the introduction of 
		//            the function parameters
		pbuf_free(p);
		//
		// record the IP and the port
		s_tAddr.addr =  addr->addr; 				        
		s_nPort      =  port; 
#if 1		    
		//-----------------------------------------------------------
		// debug 
		remoteip[0]= (u8)  (addr->addr)&0xff; 		  // IADDR4
		remoteip[1]= (u8) ((addr->addr)>>8)&0xff;   // IADDR3
		remoteip[2]= (u8) ((addr->addr)>>16)&0xff;  // IADDR2
		remoteip[3]= (u8) ((addr->addr)>>24)&0xff;  // IADDR1 
		//
		printf("\r\n callback    addr: 0x%x     ip: %d.%d.%d.%d  port: %d \r\n", s_tAddr.addr, remoteip[0], remoteip[1], remoteip[2], remoteip[3], port);
        //
#endif
	}
	else
	{
	    // attention: it is better not to disconnect here !
		// udp_disconnect(upcb); 

		// debug
		printf("\r\n The ethnet buf is empty ! \r\n");
	} 
	
	return;
} 







/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/


