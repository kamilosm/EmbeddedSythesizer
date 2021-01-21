#include "parameters.h"

//parameters 
char mode; // S - sinwave, s - saw, q-square, t-triangle
float base_note_volume; // 0 -> 1
float octave_volume;
// initialization of the parameters
void initializeParameters(){
	mode = 'S';
	base_note_volume = 0.5;
	octave_volume = 0.5;
}

// encapsulation
char getMode(){
	return mode;
}
void setMode(char m){
	mode = m;
}
float getBaseNoteVolume(){
	return base_note_volume;
}
void setBaseNoteVolume(float bnv){
	base_note_volume = bnv;
}
float getOctaveVolume(){
	return octave_volume;
}
void setOctaveVolume(float ov){
	octave_volume = ov;
} 
