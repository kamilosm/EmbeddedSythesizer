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

static uint8_t R_pressed=10;
static uint8_t C_pressed=10;
static uint8_t R_pressed_previous=0;
static uint8_t C_pressed_previous=0;
static uint8_t is_pressed=0;
void buttonIterrupt(void);
void keypadSweep(void);
void buttonsInitialize(void);
// C Cis D Dis * E F Fis G * Gis A Ais B
static const float buttons[3][4]={{262, 277.58, 294.09, 311.58}, {330.11, 349.74, 370.54, 392.57}, {415.91, 440.64, 466.84, 494.6}};

#endif  /* KLAW_H */
