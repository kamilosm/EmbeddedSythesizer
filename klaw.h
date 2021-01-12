#ifndef KLAW_H
#define KLAW_H

#include "frdm_bsp.h"
static const IRQn_Type myPORT_IRQn = PORTB_IRQn;
//def button pins
#define R4 2
#define R3 3
#define R2 4
#define C4 6
#define C3 7
#define C2 8
#define C1 9

void initializevar(void);

void buttonIterrupt(void);
void keypadSweep(void);
void buttonsInitialize(void);
// C Cis D Dis * E F Fis G * Gis A Ais B
static const float buttons[3][4]={{262, 277.58, 294.09, 311.58}, {330.11, 349.74, 370.54, 392.57}, {415.91, 440.64, 466.84, 494.6}};

uint8_t getIs_pressed(void);
uint8_t getR_pressed(void);
uint8_t getC_pressed(void);
uint8_t getR_pressed_previous(void);
uint8_t getC_pressed_previous(void);

void setIs_pressed(uint8_t);
void setR_pressed(uint8_t);
void setC_pressed(uint8_t);
void setR_pressed_previous(uint8_t);
void setC_pressed_previous(uint8_t);
void maskin(void);
	
#endif  /* KLAW_H */
