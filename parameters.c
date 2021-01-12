#include "parameters.h"

//parameters // change to divider eg x/100? 
char mode; // S - sinwave, s - saw, q-square, t-triangle
float base_note_volume; // 0 -> 1

// initialization
void initializeParameters(){
	mode = 'S';
	base_note_volume = 0.7;
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
