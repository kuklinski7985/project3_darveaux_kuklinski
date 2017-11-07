
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "circbuff.h"
#include "uart.h"
#include "debug.h"
#include "MKL25Z4.h"
#include "project2.h"

#define PROJECT2


extern CB_t * userbuff;        // define a pointer to our circular buffer structure
extern uint8_t size;           //sets circular buffer size
extern CB_status status;		//define the circular buffer status structure

extern uint8_t dump_flag;		//define a flag to determine when to transmit data statistics


int main()
{

  #ifdef PROJECT2
 project2();
 #endif

  return 0;
}




