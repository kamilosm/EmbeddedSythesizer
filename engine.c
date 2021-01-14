#include "engine.h"
#include "klaw.h"
#include "waves.h"
#include "parameters.h"
#include "DAC.h"
static const int16_t buffer_size=512;
static const int32_t sample_rate=40000;
// C Cis D Dis * E F Fis G * Gis A Ais B
static const int16_t buttons[3][4]={{262, 277, 294, 311}, {330, 349, 370, 392}, {415, 440, 466, 494}};

volatile int16_t pointer=0;
volatile int16_t max_pointer=0;
volatile int16_t max_pointer_new=0;
volatile int16_t sin_samples_count=0;
volatile int16_t sin_times_in_buffer=0; 

int16_t DAC_buffer_1[buffer_size];
int16_t DAC_buffer_2[buffer_size];

int16_t freq; //pomocnicza 

int16_t current_buffer=1;
int16_t first_time=1;
int16_t play=0;

int16_t generateBuffer = 0;
int16_t changeBuffer = 0;

void mainLoop (){
	while(1){
		if(generateBuffer==1){
			//eraseBuffer(buffer_size);
			//base note 
			generateBuffWithSignal(getBaseNoteVolume(), 1);
			//doDSP();
			generateBuffer=0;
			if(changeBuffer==1){
				current_buffer^=1; 
				changeBuffer=0;
			}
		}
		
	}
}
void doDSP(){
	//octave blend
	generateBuffWithSignal(getBaseNoteVolume(), 12);
}
void eraseBuffer(int16_t buffer_size){ // ?
		int16_t zero = 0;
	if(current_buffer==1){
		for(int16_t i=0;i<buffer_size;i++){
			DAC_buffer_2[i]=zero;
		}
	}
	else{
		for(int16_t i=0;i<buffer_size;i++){
			DAC_buffer_1[i]=zero;
		}
	}
	
}
void pressedButtonBuffReact(){
			play=1;
			freq = buttons[getR_pressed()][getC_pressed()];
			sin_samples_count = sample_rate/freq;
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
			//generateBuffWithSignal();
			//current_buffer^=1; 
			generateBuffer=1;
			changeBuffer=1;
	
		// to change - output
		// if i pressed the button i want the samples to play even when its not pressed
		// until theres no samples left in buffer and another buffer is empty
		// TODO 
}
void generateBuffWithSignal(float volume, int16_t pitch){
	// its the main dsp function
	// keep it simple for now
	// just put sin until no space left
	int16_t interval_size;
	//choose pitch
	if(pitch == 1){
		interval_size = sin_samples_count;
	}
	else if(pitch == 12){
		//octave
		interval_size = sin_samples_count/2;
	}
	
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
					getSinSamples((interval_size*base), sin_samples_count, volume, buff_choose);
				}
			break;
		case 'q':
			for(int16_t base=0;base<sin_times_in_buffer;base++){
				getSquareSamples((interval_size*base), sin_samples_count, volume, buff_choose);
			}
			break;
		case 's':
			for(int16_t base=0;base<sin_times_in_buffer;base++){
				getSawSamples((interval_size*base), sin_samples_count, volume, buff_choose);
			}
			break;
		case 't':
			for(int16_t base=0;base<sin_times_in_buffer;base++){
				getTriangleSamples((interval_size*base), sin_samples_count, volume, buff_choose);
			}
			break;
	}
}
void dacPlaySample(){
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
	if(getIs_pressed()==1 && pointer > (max_pointer*2)/3)
	{
		max_pointer_new=max_pointer;
		//generateBuffWithSignal();
		generateBuffer = 1;
	}
}