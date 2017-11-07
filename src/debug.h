
/**
* @file debug.h
* @brief This file is used to declare the functions used to provide
* a debugging interface for project 1.
* @author Andrew Kuklinski and Mason Darveaux
* @date 9/30/2017
**/

#include <stdint.h>

#ifndef debug_h_
#define debug_h_


/*Takes a pointer to memory and a length and prints the hex output.*/
void print_memory(uint8_t * start, uint32_t length);

#endif
