// MyType.h

#ifndef  __MY_TYPE_H__
#define  __MY_TYPE_H__


//------------------------------------------------------------------------------------------
// header file

// include in the compiler
#include <stdio.h>	         // printf sprintf vsprintf
#include <string.h>
#include <math.h>
#include <stdarg.h>	         // va_start va_arg va_end 

// about the MPU
#include "stm32f4xx.h"       // be related to hardware

//------------------------------------------------------------------------------------------
// debug output 

#define  BUILD_CODE_OUTPUT   0 


//------------------------------------------------------------------------------------------
// unify the data type

// 
#ifndef NULL
#define NULL    ((void *)0)
#endif
//
#ifndef FALSE
#define FALSE   (1 == 0)
#define false   FALSE
#endif
#ifndef TRUE
#define TRUE    (1 == 1)
#define true    TRUE
#endif

// the mark of IO
#define IN
#define OUT   
#define INOUT
#define BUFFER

// special
typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
typedef unsigned char   BOOL;
// common 
typedef unsigned char   UINT8;
typedef unsigned short	UINT16;
typedef unsigned long   UINT32;    
typedef signed char     INT8;
typedef signed short    INT16;
typedef signed long     INT32;   
typedef float           F32;


//------------------------------------------------------------------------------------------
// common enum

enum eFlag
{
   e_False   = 0,
   e_True    = 1,	
};

// the feedback of a function, common feedback
enum eFeedBack         
{
   e_Feedback_Success  = 0,   
   e_Feedback_Fail     = 1,   
   e_Feedback_Error    = 2,   
   e_Feedback_None     = 3,   // don't need to feedback any frame
};

// moving-direction
enum eDirection
{
    e_Dir_Neg  = 0,
    e_Dir_Pos  = 1,
};

// digtial-level
enum eLevel
{
    E_LEVEL_LOW   = 0,
	E_LEVEL_HIGHT = 1,	
};


//------------------------------------------------------------------------------------------
// definition 

// common operator
#define MAX_T(a, b) (((a) > (b)) ? (a) : (b))
#define MIN_T(a, b) (((a) < (b)) ? (a) : (b))
// 
#define SET_BIT_T(value, offset)      ( (value) = (value) | ( 0x01 << (offset) ) )       // set, or operator
#define CLR_BIT_T(value, offset)      ( (value) = (value) & ( ~( 0x01 << (offset) ) ) )  // reset, and operator
#define REVERSE_BIT_T(value, offset)  ( (value) = (value) ^ ( 0x01 << (offset) ) )       // reverse, exclusive-OR operator

// variables store
#define IO_       volatile    // the data is volatile
#if  (1 == 0)     // for 8-bit-BBW(bus band width) MPU, keil compile
#define IRAM_     data        // variables in the I-RAM
#define XRAM_     xdata       // variables in the X-RAM
#define CODE_     code        // variables in the flash
#else             // MDK compile
#define IRAM_     
#define XRAM_    
#define CODE_     const        
#endif

// variables/function calling
#define _EXT_     extern      // export function, be used in any moudle
#define _STA_     static      // be used only in the moudle which define it
// using interrupt
#if  (0 == 1)
#define _USE      interrupt   // the sign of interrupt calling
#elif(0 == 1)
#define _USE      using
#else
#define _USE      // none
#endif

//-------------------------------------------------------------------------------------
// attention: define the following micro-string in the "project options" -> "setting"
// 1. STM32F10X_HD or STM32F40_41xxx
// 2. USE_STDPERIPH_DRIVER
// 3. USE_FULL_ASSERT
//




#endif





