#ifndef pit_h
#define pit_h
#include "MKL05Z4.h"
static const IRQn_Type myPIT_IRQn = PIT_IRQn; 

void pitInitialize(unsigned period);
void startPIT(void);
void stopPIT(void);
void PIT_IRQHandler(void);

#endif
