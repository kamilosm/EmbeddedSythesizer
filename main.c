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
volatile int max_pointer=999;
volatile int sin_samples_count=0;

int* sin_buff;
int* DAC_buffer_1;
int* DAC_buffer_2;

int current_buffer=2;

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
int* getSinSamples(int ssc){
	// function returns sin prepared for generatin signal
	// based on frequency representation in ssc
	//scc - sin samples count local varriable for this function
	int sin_sampled_loc[ssc];
	int r = 4*sin_tabl/ssc;
	for(uint8_t i=0;i<ssc;i++){
		if(i<(ssc/4)){
			sin_sampled_loc[i]=sin_tab[i*r];
		}
		else if(i<ssc/2){
			sin_sampled_loc[i]=sin_tab[sin_tabl-(r*(i-(ssc/4)))];
		}
		else if(i<(ssc*3)/4){
			sin_sampled_loc[i]=-sin_tab[(i-(ssc/2))*r];
		}
		else{
			sin_sampled_loc[i]=-sin_tab[sin_tabl-(r*(i-(3*ssc/4)))];
		}
	}
	return sin_sampled_loc;// JAK TO WŁAŚCIWIE ROZWIĄZAĆ? TODO 
}
void generateBuffWithSignal(){
	// its the main dsp function
	// keep it simple for now
	// just put sin until no space left
	for(uint8_t i=0;i<max_pointer;i++){// pretty bad but will do for now 
		if(current_buffer==1){
			// another pointer to avoid starting off in different position of sin in the next buffer would be great
			DAC_buffer_1[i]=sin_buff[i%sin_samples_count];
		}
		else{
			DAC_buffer_2[i]=sin_buff[i%sin_samples_count];
		}
	}
}
void dacIterrupt(){
	if(is_pressed==1){// if key active 
		if(C_pressed_previous!=C_pressed && R_pressed!=R_pressed_previous){// if key changed
			sin_samples_count = 40000/buttons[R_pressed][C_pressed];
			while(max_pointer<1000){
				max_pointer += sin_samples_count;
			}
			sin_buff = getSinSamples(sin_samples_count);
			generateBuffWithSignal();
		}
		else if(pointer > (max_pointer*2)/3){
			//pointer to high
			// start generating next buffer
			generateBuffWithSignal();
		}
		// to change - output
		// if i pressed the button i want the samples to play even when its not pressed
		// until theres no samples left in buffer and another buffer is empty
		// TODO 
		DAC_Load_Trig(sin_tab[pointer++]);
	if(pointer>990){
		pointer = 0;
	}
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
