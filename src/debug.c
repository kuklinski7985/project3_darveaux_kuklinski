/**
* @file debug.c
* @brief This file is used to allow for debugging funcntionality by printing 
* data values at specific memory locations to the terminal interface.
* @author Andrew Kuklinski and Mason Darveaux
* @date 9/30/2017
**/


#include <stdio.h>
#include <stdint.h>
#include "debug.h"

/*function definitions for Project 1 ECEN5813, Fall 2017*/

void print_memory(uint8_t * start, uint32_t length)
{
  //loop prints values starting from the start location in memory
  //and continuing for the number of bytes specified by length
  for(int i=0; i < length; i++)
  {
    #ifdef VERBOSE
    printf("Address: %p | Hex Value: 0x%02x \n",start, *start);
    #endif
    start++;
  }
  #ifdef VERBOSE
  printf("\n");
  #endif

}

