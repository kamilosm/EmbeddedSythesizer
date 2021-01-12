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
- naprawić ten sin i reszte ehhhhh
- dodać oktawy i belnd dla nich
- notes shoould be played as long as button is clicked +- small delay
- przerwa między przyciskami o tej samej nucie
- pozmieniac nazwy bo nie tylko sin 
- optymalizacja funcji generujących wavey
- a gdyby tak przy niezmieniajacym się dzwieku zostać na jednym ubfferze bez generowania? 
*************od ostatniego commita
- niepotrzebny for usunięty 
- bląd z generowaniem sinusa usunięty
- saw wave, saw, triangle
- sin buff generates correctly
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

static const int sample_rate=40000;

int16_t DAC_buffer_1[buffer_size];
int16_t DAC_buffer_2[buffer_size];

int16_t current_buffer=1;
int16_t first_time=1;
int16_t play=0;


// bus clock 20971520Hz

void delay(int x){
	//10k to 1 ms 
	for(uint32_t i=0;i<(x*1000);i++)__nop();
}
int main (void) 
{
	initializevar();
	initializeParameters();
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
	int16_t* buff_choose ;
	if(current_buffer==1){
		buff_choose = DAC_buffer_2;
	}
	else{
		buff_choose = DAC_buffer_1;
	}
	switch(getMode()){
		case 'S':
				for(int16_t base=0;base<sin_times_in_buffer;base++){
					getSinSamples((sin_samples_count*base), sin_samples_count, getBaseNoteVolume(), buff_choose);
				}
			break;
		case 'q':
			for(int16_t base=0;base<sin_times_in_buffer;base++){
				getSquareSamples((sin_samples_count*base), sin_samples_count, getBaseNoteVolume(), buff_choose);
			}
			break;
		case 's':
			for(int16_t base=0;base<sin_times_in_buffer;base++){
				getSawSamples((sin_samples_count*base), sin_samples_count, getBaseNoteVolume(), buff_choose);
			}
			break;
		case 't':
			for(int16_t base=0;base<sin_times_in_buffer;base++){
				getTriangleSamples((sin_samples_count*base), sin_samples_count, getBaseNoteVolume(), buff_choose);
			}
			break;
	}
	/*
	if(current_buffer==1){
		for(int16_t base=0;base<sin_times_in_buffer;base++){
			getSinSamples((sin_samples_count*base), sin_samples_count, getBaseNoteVolume(), DAC_buffer_2);
		}
	}
	else{
		for(int16_t base=0;base<sin_times_in_buffer;base++){
			getSinSamples((sin_samples_count*base), sin_samples_count, getBaseNoteVolume(), DAC_buffer_1);
		}
	}
	*/
}
void dacIterrupt(){
	if(getIs_pressed()==1){// if key active 
		if(getC_pressed_previous()!=getC_pressed() && getR_pressed()!=getR_pressed_previous()){// if key changed
			play=1;
			sin_samples_count = sample_rate/buttons[getR_pressed()][getC_pressed()];
			max_pointer_new=sin_samples_count;
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
			current_buffer^=1; // zle
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
	if(play){
		if(pointer>=max_pointer && max_pointer!=0){

			if(getIs_pressed()==0){
				play=0;
				return; // wazne 
			}
			max_pointer=max_pointer_new;
			pointer = 0;
			current_buffer^=1;
		} 	
		if(current_buffer==0){
			DAC_Load_Trig(DAC_buffer_1[pointer++]);
		}
		else{
			DAC_Load_Trig(DAC_buffer_2[pointer++]);

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
