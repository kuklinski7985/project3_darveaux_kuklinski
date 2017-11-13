
/**
* @file project2.h
* @brief Contains function prototypes for the implention of project2
* @author Andrew Kuklinski and Mason Darveaux
* @date 10/25/2017
**/


#include <stdio.h>
#include <stdint.h>

#ifndef project3_h_
#define project3_h_


/**
 *@brief calls functions, configures interrupts, and initializes the circular buffer struct in order to implement project 2,
 *
 *@param none
 *
 *@return VOID
 */
void project3(void);

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

#endif /*__project2 _h__*/
