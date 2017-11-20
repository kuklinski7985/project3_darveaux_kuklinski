/**
* @file profilingFxn.h
* @brief creating profiling fxns for standard library and self created fuxns, with and without DMA and optimization
* @author Andrew Kuklinski and Mason Darveaux
* @date 10/25/2017
**/
#include <stdio.h>
#include <stdint.h>

#ifndef profilingFxn_h_
#define profilingFxn_h_


/**
 *@brief prints profiling times for standard library memmove
 *
 *@param none
 *
 *@return VOID
 */
void profile_All(uint16_t bytesMoved);

/**
 *@brief
 *
 *@param none
 *
 *@return VOID
 */
uint32_t getValueLength(uint16_t length);

#endif /*__profilingFxn_h_*/
