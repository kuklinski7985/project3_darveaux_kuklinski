/**
* @file uart.c
* @brief defines uart operation and provides functions for uart operation and initialization
* @author Andrew Kuklinski and Mason Darveaux
* @date 10/25/2017
**/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "timerInit.h"
#include "MKL25Z4.h"
#include "core_cm0plus.h"


void my_LPTMR_init(){
	//enables access to the LPTMR
	SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

	//sets prescale to 0, which divides the prescaler clock by 2
	LPTMR0_PSR |= LPTMR_PSR_PRESCALE(0);

	//bypassing the prescale and glitch mode
	LPTMR0_PSR |= LPTMR_PSR_PBYP_MASK;

	//selecting a clock source
	LPTMR0_PSR |= LPTMR_PSR_PCS();      //NEED TO SELECT A CLOCK SOURCE

	//sets the compare value for clock
	LPTMR0_CMR = LPTMR_CMR_COMPARE();    //NEED COMPARE VALUE

	//


}
void myTPM_init()
{
	/**********clock to TPM setting**************/
	//Setting internal reference clock and enabling for use as MCGIRCLK
	MCG-> C1 = MCG_C1_IREFS_MASK | MCG_C1_IRCLKEN_MASK;

	//Setting internal reference clock as fast (vs. slow)
	MCG -> C2 |= MCG_C2_IRCS_MASK;

	//system clock gating control register, clock gate to TPM0 module
	SIM -> SCGC6 |=SIM_SCGC6_TPM0_MASK;

	//Setting clock to be MCGIRCLK into TPM
	SIM-> SOPT2 |= SIM_SOPT2_TPMSRC(3);

	/******TPM configuration settings********/
	//Turns on DMA
	TPM0->SC |= TPM_SC_DMA_MASK;

	//sets prescale to divide by 1
	TPM0->SC |= TPM_SC_PS_MASK;

	//LPTPM counter increments on every LPTPM counter clock
	TPM0->SC &= TPM_SC_CMOD_MASK;

	//enable TOF interrupts
	TPM0->SC |= TPM_SC_TOIE_MASK;

	//setting counter to input capture
	TPM0_C0SC &=(TPM_CnSC_MSB_MASK | TPM_CnSC_MSA_MASK);

	//setting the input capture (MS0B:MS0A) to count on the rising edge
	TPM0_C0SC &= TPM_CnSC_ELSB_MASK;
	TPM0_C0SC |= TPM_CnSC_ELSA_MASK;

	//enable channel interrupts
	TPM0_C0SC |= TPM_CnSC_CHIE_MASK;

	//enable DMA
	TPM0_C0SC |= TPM_CnSC_DMA_MASK;

	return;

}
