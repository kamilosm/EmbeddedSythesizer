/*-------------------------------------------------------------------------
					Embedded Synth 
					Authors: Krzysztof Jedrejasz, Kamil Maczuga
					Version: 
----------------------------------------------------------------------------*/
/* Notatki notateczki
*trzeba pomyslec jak rozwiazac kwestie LFO tak zeby miało prawo to działać przy naszym rozmiarz buffora
*czy te małe buffory są okej, czy może jednak zdecydować się na wypuszczanie próbek i DSP na żywo bez nich? może buffor kołowy,
ale on chyba nie zmienia nic

Do zrobienia teraz:
- przetestować czy dzwiek juz idzie ok 
- dokończyć pozostałe funckje generujące przebiegi i je przetestować
- dodać oktawy i belnd dla nich
- naprawić te dwa warningi z getBaseNoteVolume
- notes shoould be played as long as button is clicked +- small delay

*/
#include "frdm_bsp.h"
#include "DAC.h"
#include "MKL05Z4.h"
#include "klaw.h"
#include "pit.h"
#include <stdio.h>
#include "math.h"
#include "waves.h"
#include <string.h>
#include <stdlib.h>
#include "parameters.h"
#define M_PI 3.14159265358979323846
static const int16_t buffer_size=512;
volatile int16_t pointer=0;
volatile int16_t max_pointer=0;
volatile int16_t max_pointer_new=0;
volatile int16_t sin_samples_count=0;
volatile int16_t sin_times_in_buffer=0; 

int16_t DAC_buffer_1[buffer_size];
int16_t DAC_buffer_2[buffer_size];

int current_buffer=1;
int first_time=1;

// bus clock 20971520Hz

void delay(int x){
	//10k to 1 ms 
	for(uint32_t i=0;i<(x*1000);i++)__nop();
}
int main (void) 
{
	initializevar();
	buttonsInitialize();
	pitInitialize(476);
	DAC_Init();
	startPIT();

  while(1)
	{
		
	}
}

void generateBuffWithSignal(){
	// its the main dsp function
	// keep it simple for now
	// just put sin until no space left
	for(uint8_t i=0;i<max_pointer_new;i++){// pretty bad but will do for now 
		if(current_buffer==1){
			// another pointer to avoid starting off in different position of sin in the next buffer would be great
			for(int16_t base=0;base<sin_times_in_buffer;base++){
				getSinSamples((sin_samples_count*base), sin_samples_count, getBaseNoteVolume(), DAC_buffer_2);
			}
		}
		else{
			for(int16_t base=0;base<sin_times_in_buffer;base++){
				getSinSamples((sin_samples_count*base), sin_samples_count, getBaseNoteVolume(), DAC_buffer_1);
			}
		}
	}
}
void dacIterrupt(){
	if(getIs_pressed()==1){// if key active 
		if(getC_pressed_previous()!=getC_pressed() && getR_pressed()!=getR_pressed_previous()){// if key changed
			sin_samples_count = 40000/buttons[getR_pressed()][getC_pressed()];
			sin_times_in_buffer=1;
			while(max_pointer_new<400 && (max_pointer_new+sin_samples_count)<=buffer_size){
				max_pointer_new += sin_samples_count;
				sin_times_in_buffer++;
			}
			if(first_time==1){
				max_pointer=max_pointer_new;
				pointer=0;
				first_time=0;
			}
			generateBuffWithSignal();
		}
		else if(pointer > (max_pointer*2)/3){//sprawdzic
			//pointer to high
			// start generating next buffer
			max_pointer_new=max_pointer;
			generateBuffWithSignal();
		}
		// to change - output
		// if i pressed the button i want the samples to play even when its not pressed
		// until theres no samples left in buffer and another buffer is empty
		// TODO 
	}
	if(current_buffer==0){
			DAC_Load_Trig(DAC_buffer_1[pointer++]);
		}
		else{
			DAC_Load_Trig(DAC_buffer_2[pointer++]);
	if(pointer==max_pointer){
			max_pointer=max_pointer_new;
			pointer = 0;
			current_buffer^=1;
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
