// KernelHeader.h

#ifndef __KERNEL_HEADER_H__
#define __KERNEL_HEADER_H__


// toolchain 
#include "ChainHeader.h"        
// kernel code
#include "Protocol.h"
#include "SamplingPort.h"
#include "InterfaceScope.h"       
#include "ProcessScope.h"
#include "SystemScope.h"
//
#include "StorageManagement.h"


#define USE_STM32F407_ONLY		1	// if = 0, use FPGA
#define SIMUATION_TEST          0


//#define DOUBLE_ADC_CHANNEL    1
// discard
//#define DATA_TEST_ONLY			1
//#define HGB_DEBUG_FLAG			1
//#define HGB_BEBUG_FPGA			0

//#define CRP_DEBUG_FLAG			1
//#define CRP_BEBUG_FPGA			0

//#define WBC_DEBUG_FPGA			0
//#define RBC_DEBUG_FPGA			0
//#define PLT_DEBUG_FPGA			0
//#define RBC_PLT_DEBUG_FPGA		0




#endif

