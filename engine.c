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

volatile int16_t DAC_buffer_1[buffer_size];
volatile int16_t DAC_buffer_2[buffer_size];

int16_t freq; //pomocnicza 

int16_t current_buffer=1;
int16_t first_time=1;
int16_t play=0;

int16_t generateBuffer = 0;
int16_t changeBuffer = 0;

void mainLoop (){
	while(1){
		if(generateBuffer==1){
			generateBuffWithSignal(getBaseNoteVolume(),getOctaveVolume()) ;
			doDSP();
			generateBuffer=0;
			if(changeBuffer==1){
				current_buffer^=1; 
				changeBuffer=0;
			}
		}
		
	}
}
void doDSP(){
	
	
}
void eraseBuffer(int16_t buffer_size){ // ?
	if(current_buffer==1){
		for(volatile int16_t i=0;i<buffer_size;i++){
			DAC_buffer_2[i]=0;
		}
	}
	else{
		for(volatile int16_t i=0;i<buffer_size;i++){
			DAC_buffer_1[i]=0;
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
			generateBuffer=1;
			changeBuffer=1;
	
		// to change - output
		// if i pressed the button i want the samples to play even when its not pressed
		// until theres no samples left in buffer and another buffer is empty
		// TODO 
}
void generateBuffWithSignal(float volume, float volume_octave){
	// just put sin until no space left
	
	volatile int16_t* buff_choose ;
	if(current_buffer==1){
		buff_choose = DAC_buffer_2;
	}
	else{
		buff_choose = DAC_buffer_1;
	}
	switch(getMode()){
		case 'S':
			for(volatile int16_t base=0;base<sin_times_in_buffer;base++){	
				volatile int16_t base_i = base * sin_samples_count;
				if(volume_octave<0.05){
					getSinSamples(base_i, sin_samples_count, volume, buff_choose);
				}else{
					getSinSamplesWithOctave(base_i, sin_samples_count, volume, volume_octave, buff_choose);
				}
			}
			break;
		case 'q':
			for(volatile int16_t base=0;base<sin_times_in_buffer;base++){
				volatile int16_t base_i = base * sin_samples_count;
			  if(volume_octave<0.05)
				  {getSquareSamples(base_i, sin_samples_count, volume, buff_choose);}
				else
					{getSquareSamplesWithOctave(base_i, sin_samples_count, volume, volume_octave, buff_choose);}
			}
			break;
		case 's':
			for(volatile int16_t base=0;base<sin_times_in_buffer;base++){
				volatile int16_t base_i = base * sin_samples_count;
				if(volume_octave<0.05)
					{getSawSamples(base_i, sin_samples_count, volume, buff_choose);}
				else
					{getSawSamplesWithOctave(base_i, sin_samples_count, volume, volume_octave, buff_choose);}
			}
			break;
		case 't':
			for(volatile int16_t base=0;base<sin_times_in_buffer;base++){
				volatile int16_t base_i = base * sin_samples_count;
				if(volume_octave<0.05)
					{getTriangleSamples(base_i, sin_samples_count, volume, buff_choose);}
				else
					{getTriangleSamplesWithOctave(base_i, sin_samples_count, volume, volume_octave, buff_choose);}
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
		generateBuffer = 1;
	}
}
void changeWaveShape(uint8_t C_pressed_wave)
{
	switch(C_pressed_wave){
		case 0:
			setMode('S');
			break;
		case 1:
			setMode('s');
			break;
		case 2:
			setMode('q');
			break;
		case 3:
			setMode('t');
			break;
	}
}