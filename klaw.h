#ifndef KLAW_H
#define KLAW_H

#include "frdm_bsp.h"
#include "engine.h"
static const IRQn_Type myPORT_IRQn = PORTB_IRQn;
//def button pins
#define R4 2
#define R3 3
#define R2 4
#define C4 6
#define C3 7
#define C2 8
#define C1 9
#define R1 10

void initializevar(void);

void buttonIterrupt(void);
void keypadSweep(void);
void buttonsInitialize(void);

void waveSweep(void);

// encapsulation

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
	
#endif  /* KLAW_H */
