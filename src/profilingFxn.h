/**
* @file profilingFxn.h
* @brief creating profiling fxns for standard library and self created fuxns,
* with and without DMA and optimization
* @author Andrew Kuklinski and Mason Darveaux
* @date 12/08/2017
**/
#include <stdio.h>
#include <stdint.h>

#ifndef profilingFxn_h_
#define profilingFxn_h_


/**
 *@brief prints profiling times for all memmove and memset versions for KL25Z
 *
 *@param bytesMoved: accepts number of bytes to move in profile test
 *
 *@return VOID
 */
void profile_All_KL25Z(uint16_t bytesMoved);

/**
 *@brief  prints profiling teims for all memmove and memset version for host and BBB
 *
 *@param bytesMoved: accepts number of bytes to move in profile test
 *
 *@return VOID
 */
void profile_All_BBB(uint16_t bytesMoved);

/**
 *@brief
 *
 *@param length: accepts a value to test the length
 *
 *@return number of digits the value length contains
 */
uint32_t getValueLength(uint32_t length);


#endif /*__profilingFxn_h_*/
