
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "circbuff.h"
#include "uart.h"
#include "debug.h"

#ifdef PROFILEKL25Z
#include "MKL25Z4.h"
#endif

#include "project3.h"
#include "binaryLogger.h"


extern CB_t * userbuff;        //define a pointer to our circular buffer structure
//extern uint8_t size;         //sets circular buffer size
extern CB_status status;       //define the circular buffer status structure

extern uint8_t dump_flag;      //define a flag to determine when to transmit data statistics


/* 1. when using kinetis IDE, you will have to add the -DPROJECT3 and -DPROFILEKL25Z tags to the complier
 * 2. you will also have to increase the size of the heap to as least 0x3500
 * 	   a. -Xlinker --defsym=__heap_size__=0x3500
 * 3. verify that system_MKL25Z4.h has the following to make UART operate:
 *     a. #define CLOCK_SETUP 1
 *     b. value for SYSTEM_MCG_C5_VALUE 0x41u and not 0x01u
 *     c. #define SYSTEM_SIM_SOPT2_VALUE should equal 0x04010000U not 0x01010000U
 * */

int main()
{

  #ifdef PROJECT3
 project3();
 #endif

  return 0;
}




