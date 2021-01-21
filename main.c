/*-------------------------------------------------------------------------
					Embedded Synth 
					Authors: Krzysztof Jedrejasz, Kamil Maczuga
					Version: 
----------------------------------------------------------------------------*/

/* Notatki notateczki

Do zrobienia teraz: (najwięcej gwizdek najmniejszy priorytet czasowy) 
- potencjometr do regulowania mocy okatawy i nuty bazowej *
- implementacja najprostszej formy delay *
- szkielet teoretyczny envelope **

Zadania, które są bardziej pytaniami:
- przenieść PIT_IRQHandler do pitu, jak z includem dac? ****************************
- jak rozwiązać kwestie parametrów dodatkowych?(potencjometry - kilka podłączonych na przerwaniu? 

++++++++++++++++++++++od ostatniego commita+++++++++++++++++++++++++++++++++++++++++++
- code cleaned refactored and commented properly
*/
#include "frdm_bsp.h"
#include "engine.h"
#include "parameters.h"
#include "DAC.h"
#include "MKL05Z4.h"
#include "klaw.h"
#include "pit.h"
#include <stdio.h>
#include "math.h"
#include <string.h>
#include <stdlib.h>
// bus clock 20971520Hz
int16_t sweep_pointer=0;
int16_t sweep_freq=400;
int main (void) 
{
	// starting pit and initializing all the necesary things
	sweep_pointer=sweep_freq;
	initializevar();
	initializeParameters();
	buttonsInitialize();
	// this should give us a pit frequency around the 40kHz value
	pitInitialize(476);
	DAC_Init();
	startPIT();

  mainLoop();
}
void dacIterrupt(){
	dacPlaySample();
}
void PIT_IRQHandler() {
		
	// check to see which channel triggered interrupt 
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 0
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
			dacIterrupt();
			if(sweep_pointer==0){
				keypadSweep();
				sweep_pointer=sweep_freq;
			}
			sweep_pointer--;
	} else if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 1
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
	}
	//clear pending IRQ
	NVIC_ClearPendingIRQ(myPIT_IRQn);
	
}

