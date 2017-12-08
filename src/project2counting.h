/**
* @file project2couting.h
* @brief
* @author Andrew Kuklinski and Mason Darveaux
* @date 12/08/2017
**/


#include <stdio.h>
#include <stdint.h>
#include "core_cm0plus.h"

#ifndef project2couting_h_
#define project2couting_h_


/**
 *@brief processes received data
 *
 *@param none
 *
 *@return VOID
 */

void process_Data();

/**
 *@brief transmits data statistics to terminal
 *
 *@param none
 *
 *@return VOID
 */

void transmit_table();



#endif /*project2couting_h_*/
