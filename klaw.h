#ifndef KLAW_H
#define KLAW_H

#include "frdm_bsp.h"
static const IRQn_Type myPORT_IRQn = PORTB_IRQn;
//definicje przycisk�w
#define BUTTON_1_POS 1
#define BUTTON_5_POS 2
#define BUTTON_9_POS 3


void buttonsInitialize(void);

#endif  /* KLAW_H */
