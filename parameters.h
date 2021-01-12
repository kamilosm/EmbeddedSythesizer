#ifndef sintable
#define sintable
#include "MKL05Z4.h"

//parameters // change to divider eg x/100? 
char mode='S'; // s - sinwave, S - saw, q-square, t-triangle
float base_note_volume=0.6; // 0 -> 1


//getters setters
char getMode(void);
void setMode(char);
float getBaseNoteVolume(void);
void setBaseNoteVolume(float);
#endif
