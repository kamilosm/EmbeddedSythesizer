#include "engine.h"
#include "klaw.h"
#include "waves.h"
#include "parameters.h"
#include "DAC.h"
// 512 is currently the highest buffer size SRAM allows us to use comfrotably
static const int16_t buffer_size=512;
// for our purposes higher sample rate would be useless, keep in mind this value gets pretty heavy on cpu because it makes
// time for wave generators shorter to make a full buffer
static const int32_t sample_rate=40000;

// C Cis D Dis * E F Fis G * Gis A Ais B
static const int16_t buttons[3][4]={{262, 277, 294, 311}, {330, 349, 370, 392}, {415, 440, 466, 494}};
//pointer used for interating within the buffers
volatile int16_t pointer=0;
// max pointer for current buffer in use
// because were not using full buffers for every base frequency at this point
volatile int16_t max_pointer=0;
// this one is for next buffer being generated
volatile int16_t max_pointer_new=0;
// this one tells us how many samples one interval of eg sin is made of
volatile int16_t sin_samples_count=0;
// how many intervals of base singal the buffers contains
volatile int16_t sin_times_in_buffer=0; 

// acctual buffers
volatile int16_t DAC_buffer_1[buffer_size];
volatile int16_t DAC_buffer_2[buffer_size];

// frequency choosed by pushing button
int16_t freq;  

// buffer being currently used(val=1 -> DAC_buffer_2, val=0 -> DAC_buffer_1) 
int16_t current_buffer=1;
// flag tells if its the forst time wave been requested to generate after the boot 
int16_t first_time=1;
// is syth currently playing? 
int16_t play=0;

// flags used to pass the infromation to generate | change buffer in mainLoop 
int16_t generateBuffer = 0;
int16_t changeBuffer = 0;

void mainLoop (){
	// purpose of this loop and flags we used was to take the sample generating out of DAC iterupt. 
	// This allows us to avoid glithes in sound - otherwise first iterupt would last for a longer than expected, and while
	// it lasts next iterupt(interval based on sample_rate but too short with any realistic value)
	while(1){
		if(generateBuffer==1){
			generateBuffWithSignal(getBaseNoteVolume(),getOctaveVolume());
			generateBuffer=0;
			if(changeBuffer==1){
				current_buffer^=1; 
				changeBuffer=0;
			}
		}
		
	}
}
/*
void eraseBuffer(int16_t buffer_size){ 
	// this function is not currently used in project, because we found it to be inefficient to perform operations on a whole
	// buffer - rather we decided to switch to samples, or chunks of data
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
*/
void pressedButtonBuffReact(){
	// in react to pressed button this function prepares to generate buffers, setting up the pointers, intervals etc
	// this function only reacts to frequency buttons not functional buttons  
	play=1;
	// choose frequency
	freq = buttons[getR_pressed()][getC_pressed()];
	// determine the interval lenght in samples
	sin_samples_count = sample_rate/freq;
	max_pointer_new=sin_samples_count;
	sin_times_in_buffer=1;
	// determine the max pointer, it should be a interval length times x, but less than buffer size allocated 
	while(max_pointer_new<400 && (max_pointer_new+sin_samples_count)<=buffer_size){
		max_pointer_new += sin_samples_count;
		sin_times_in_buffer++;
	}
	// zero the pointer and take new max pointer value instantly on the first run
	if(first_time==1){
		max_pointer=max_pointer_new;
		pointer=0;
		first_time=0;
	}
	generateBuffer=1;
	changeBuffer=1;		
}
void generateBuffWithSignal(float volume, float volume_octave){
	// function populates the buffer with data
	// first choose the buffer 
	volatile int16_t* buff_choose ;
	if(current_buffer==1){
		buff_choose = DAC_buffer_2;
	}
	else{
		buff_choose = DAC_buffer_1;
	}
	// mode determines which waveshape will be used
	switch(getMode()){
		case 'S':
			// we generate whole interval every time in this loop
			for(volatile int16_t base=0;base<sin_times_in_buffer;base++){
				// base pointer from this wave functions will populate the data
				volatile int16_t base_i = base * sin_samples_count;
				// if octave volume is too low use the functions that do not use them, this way we save a lot of computing time
				if(volume_octave<0.05){
					getSinSamples(base_i, sin_samples_count, volume, buff_choose);
				}else{
					getSinSamplesWithOctave(base_i, sin_samples_count, volume, volume_octave, buff_choose);
				}
			}
			break;
		case 'q':
			// same but with the square wave 
			for(volatile int16_t base=0;base<sin_times_in_buffer;base++){
				volatile int16_t base_i = base * sin_samples_count;
			  if(volume_octave<0.05)
				  {getSquareSamples(base_i, sin_samples_count, volume, buff_choose);}
				else
					{getSquareSamplesWithOctave(base_i, sin_samples_count, volume, volume_octave, buff_choose);}
			}
			break;
		case 's':
			// same but with the saw shaped wave 
			for(volatile int16_t base=0;base<sin_times_in_buffer;base++){
				volatile int16_t base_i = base * sin_samples_count;
				if(volume_octave<0.05)
					{getSawSamples(base_i, sin_samples_count, volume, buff_choose);}
				else
					{getSawSamplesWithOctave(base_i, sin_samples_count, volume, volume_octave, buff_choose);}
			}
			break;
		case 't':
			// same but with the triangle shaped wave
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
	// function responsible for actuall playing of the samples in buffer
	if(play){
		if(pointer>=max_pointer && max_pointer!=0){
			// stop playing if there are no unplayed samples in the buffer and no frequency button is pressed that the moment
			if(getIs_pressed()==0){
				play=0;
				return;
			}
			// if not start playing another buffers samples
			max_pointer=max_pointer_new;
			pointer = 0;
			current_buffer^=1;
		} 	
		// give the value to right buffer
		if(current_buffer==0){
			DAC_Load_Trig(DAC_buffer_1[pointer++]);
		}
		else{
			DAC_Load_Trig(DAC_buffer_2[pointer++]);
		}
	}
	// start generating next buffer after current one is played in x% 
	if(getIs_pressed()==1 && pointer > (max_pointer*2)/3)
	{
		max_pointer_new=max_pointer;
		generateBuffer = 1;
	}
}
void changeWaveShape(uint8_t C_pressed_wave)
{
	// change the wave shape based on which functional button is pressed
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