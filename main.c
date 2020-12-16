/*-------------------------------------------------------------------------
					Technika Mikroprocesorowa 2 - laboratorium
					Lab 4 - Ćwiczenie 1: PWM
					autor: Mariusz Sokołowski
					wersja: 02.11.2020r.
----------------------------------------------------------------------------*/
#include "frdm_bsp.h"
#include "DAC.h"
#include "MKL05Z4.h"
#include "klaw.h"
#include "pit.h"
#include <stdio.h>
#include "math.h"
#include "sintable.h"
#include <string.h>
#include <stdlib.h>
#define M_PI 3.14159265358979323846
volatile int16_t pointer=0;
// bus clock 20971520Hz
void delay(int x){//10k to 1 ms 
	for(uint32_t i=0;i<(x*1000);i++)__nop();
}
int main (void) 
{
	buttonsInitialize();
	pitInitialize(476);
	DAC_Init();
	startPIT();
	
  while(1)
	{
		
	}
}
void dacIterrupt(){
	DAC_Load_Trig(sintabl[pointer++]);
	if(pointer>990){
		pointer = 0;
	}
}
void PIT_IRQHandler() {
		
	// check to see which channel triggered interrupt 
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 0
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
			dacIterrupt();
	} else if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 1
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
	}
	//clear pending IRQ
	NVIC_ClearPendingIRQ(myPIT_IRQn);
	
}
