// Interrupt.h

#ifndef  __INTERRUPT_H__
#define  __INTERRUPT_H__


#include "MyType.h"



#define  INTERRUPT_OUTPUT        0


//-----------------------------------------------------------------------------------------
// the micro definition

// systick counter
#define IRQ_SysTick_ms       SysTick_Handler
// exti input, PG6
#define IRQ_MotorStatus      EXTI9_5_IRQHandler
// communicate
#define IRQ_ComCommand       USART1_IRQHandler     // COM_MAIN, com1(usart1)
#define IRQ_ComSampling      USART3_IRQHandler     // COM_SAMPLING, com2(usart3)
// system timer              
#define IRQ_SysTimer         TIM2_IRQHandler

//-----------------------------------------------------------------------------------------
// variables declaration 
// global 


//-----------------------------------------------------------------------------------------
//
/* cm3 core ticks */
void SysTick_Handler(void);         // core-m3

/* exti interrupt */
// void EXTI0_IRQHandler(void);     // EXTI Line 0
// void EXTI1_IRQHandler(void);     // EXTI Line 1
// void EXTI2_IRQHandler(void);     // EXTI Line 2
// void EXTI3_IRQHandler(void);     // EXTI Line 3
// void EXTI4_IRQHandler(void);     // EXTI Line 4
void EXTI9_5_IRQHandler(void);      // EXTI Line 9..5
// void EXTI15_10_IRQHandler(void);    // EXTI Line 15..10

/* usart interrupt */
void USART1_IRQHandler(void);       // USART1              // com1 
// void USART2_IRQHandler(void);    // USART2              // com2
void USART3_IRQHandler(void);       // USART3
void UART4_IRQHandler(void);        // UART4, not USART4   // com3

/* timer interrupt */
// void TIM1_BRK_IRQHandler(void);       // TIM1 Break
// void TIM1_UP_IRQHandler(void);        // TIM1 Update
// void TIM1_TRG_COM_IRQHandler(void);   // TIM1 Trigger and Commutation
// void TIM1_CC_IRQHandler(void);        // TIM1 Capture Compare
// void TIM8_BRK_IRQHandler(void);       // TIM8 Break
// void TIM8_UP_IRQHandler(void);        // TIM8 Update
// void TIM8_TRG_COM_IRQHandler(void);   // TIM8 Trigger and Commutation
// void TIM8_CC_IRQHandler(void);        // TIM8 Capture Compare
//
void TIM2_IRQHandler(void);              // TIM2
// void TIM3_IRQHandler(void);           // TIM3
// void TIM4_IRQHandler(void);           // TIM4
// void TIM5_IRQHandler(void);           // TIM5
//
// void TIM6_IRQHandler(void);           // TIM6
// void TIM7_IRQHandler(void);           // TIM7


//-----------------------------------------------------------------------------------------
//

//
_EXT_ UINT8  IT_SYS_DlyMs(UINT32 nDlyMs); 
//
_EXT_ UINT32 IT_SYS_GetTicks(void);
//
_EXT_ UINT8  IT_SYS_SetTicks(UINT32 nTicks);

#if 1
//
_EXT_ UINT8  IT_SYS_SetDlyMs(UINT32 nDlyMs);
//
_EXT_ enum eFlag IT_SYS_CheckDlyFin(void);
#endif


//
_EXT_ UINT32 IT_LIST_GetTicks(void);
_EXT_ UINT8  IT_LIST_SetTicks(UINT32 nTicks);
_EXT_ UINT32 IT_ADC_GetTicks(void);
_EXT_ UINT8  IT_ADC_SetTicks(UINT32 nTicks);


#endif




