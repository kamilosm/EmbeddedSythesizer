/*----------------------------------------------------------------------------
 *      Name:    pit.c
 *
 *      Purpose: Microprocessors Laboratory 2
 *      
 *      Author: Pawel Russek AGH University of Science and Technology
 *---------------------------------------------------------------------------*/
 
#include "pit.h"	
 /*----------------------------------------------------------------------------
  Function that initializes PIT timer
 *----------------------------------------------------------------------------*/
void pitInitialize(unsigned period) {

	// Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; 
	
	// Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;
	PIT->MCR |= PIT_MCR_FRZ_MASK;
	
	// Initialize PIT0 to count down from argument 
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(period);

	// No chaining
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;
	
	// Generate interrupts
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;

	/* Enable Interrupts */
	NVIC_SetPriority(myPIT_IRQn, 3); 

	NVIC_ClearPendingIRQ(myPIT_IRQn); 
	NVIC_EnableIRQ(myPIT_IRQn);	

}

void startPIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void stopPIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}


