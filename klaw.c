#include "klaw.h"
#include "frdm_bsp.h"
#include "pit.h"

uint8_t R_pressed;
uint8_t C_pressed;
uint8_t R_pressed_previous;
uint8_t C_pressed_previous;
uint8_t is_pressed;
uint8_t prev_pressed=0;

uint8_t C_pressed_wave;
uint8_t R_wave=0;
void initializevar(){
	// initialize values pressed set to unrealistic value for a simple reason: 
	// we want it to not match the first actual value given by the keypad
	R_pressed=10;
	C_pressed=10;
	R_pressed_previous=0;
	C_pressed_previous=0;
	// set flag to false
	is_pressed=0;
}
void buttonsInitialize(void){
	SIM->SCGC5 |=  SIM_SCGC5_PORTB_MASK;
		// set all the pins to GPIO
		// it could be done in the loop reducing the code redundance but we wanted to show that the code is designed for a simple
		// adaptation to changes
	PORTB->PCR[R4] |= PORT_PCR_MUX(1);      	
	PORTB->PCR[R3] |= PORT_PCR_MUX(1);      	
	PORTB->PCR[R2] |= PORT_PCR_MUX(1); 
	PORTB->PCR[C4] |= PORT_PCR_MUX(1);
	PORTB->PCR[C3] |= PORT_PCR_MUX(1);
	PORTB->PCR[C2] |= PORT_PCR_MUX(1);
	PORTB->PCR[C1] |= PORT_PCR_MUX(1);
	PORTB->PCR[R1] |= PORT_PCR_MUX(1);
	
}

void keypadSweep(){ 
	// sweep the keypad to find out if and which button was pressed
	prev_pressed=is_pressed;
	is_pressed=0;
	//set rows as input and columns as output
	for(uint8_t i=0;i<4;i++){ //
		PORTB->PCR[(R4+i)] |=  PORT_PCR_PE_MASK |		
											 PORT_PCR_PS_MASK;	
		PTB->PDDR &= ~(1<<(R4+i));
	}
	PORTB->PCR[10] |=  PORT_PCR_PE_MASK |		
											 PORT_PCR_PS_MASK;	
	PTB->PDDR &= ~(1<<10);
	
	for(uint8_t i=0;i<4;i++){
		PTB->PDDR |= (1<<(C4+i));
		PTB->PCOR |= (1<<(C4+i)); 
	}
	// check if any row is pressed and if it is store the number of button
	for(uint8_t i=0;i<3;i++){
		if(( PTB->PDIR & (1<<(R4+i)) ) ==0)
		{
			is_pressed=1;
			R_pressed_previous = R_pressed;
			R_pressed=i;	
			break;
		}
	}
		 if(( PTB->PDIR & (1<<(10)) ) ==0){
			R_wave=10;}
		
	
	//set cols as input and rows as output
	
	for(uint8_t i=0;i<4;i++){
		PORTB->PCR[(C4+i)] |=  PORT_PCR_PE_MASK |		
											 PORT_PCR_PS_MASK;	
		PTB->PDDR &= ~(1<<(C4+i));
		PTB->PSOR |= (1<<(C4+i));
	}
	for(uint8_t i=0;i<3;i++){
		PTB->PDDR |= (1<<(R4+i));
		PTB->PCOR |= (1<<(R4+i));
	}
		PTB->PDDR |= (1<<(10));
		PTB->PCOR |= (1<<(10));

	for(uint8_t i=0;i<4;i++){
		if(( PTB->PDIR & (1<<(C4+i)) ) ==0)
		{
			if (R_wave ==10){
				// if a functional button was pressed change the wave shape
				C_pressed_wave=i;
				changeWaveShape(C_pressed_wave);
				R_wave=0;
				break;		
			}
			else
			{
				// else it was the frequency button so we change it
				C_pressed_previous=C_pressed;
				C_pressed=i;
				R_wave=0;
				break;	
			}
		}
	}
	// pressed key defined by C_pressed and R_pressed combined
	
	// generate buffer
	if(getIs_pressed()==1){// if key active 
		if((getC_pressed_previous()!=getC_pressed() | getR_pressed()!=getR_pressed_previous()) | (prev_pressed==0)){ 
			// not the same key that was already pressed(just holding a key)
			pressedButtonBuffReact();
		}
	}
}

uint8_t getIs_pressed(){
	return is_pressed;
}
uint8_t getR_pressed(){
	return R_pressed;
}
uint8_t getC_pressed(){
	return C_pressed;
}
uint8_t getR_pressed_previous(){
	return R_pressed_previous;
}
uint8_t getC_pressed_previous(){
	return C_pressed_previous;
}

void setIs_pressed(uint8_t i){
	is_pressed=i;
}
void setR_pressed(uint8_t i){
	R_pressed=i;
}
void setC_pressed(uint8_t i){
	C_pressed=i;
}
void setR_pressed_previous(uint8_t i){
	R_pressed_previous=i;
}
void setC_pressed_previous(uint8_t i){
	C_pressed_previous=i;
}
