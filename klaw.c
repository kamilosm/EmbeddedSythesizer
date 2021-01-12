#include "klaw.h"
#include "frdm_bsp.h"
#include "pit.h"
// TODO przycisk musi sie odklikiwac 
uint8_t R_pressed;
uint8_t C_pressed;
uint8_t R_pressed_previous;
uint8_t C_pressed_previous;
uint8_t is_pressed;

void initializevar(){
	R_pressed=10;
	C_pressed=10;
	R_pressed_previous=0;
	C_pressed_previous=0;
	is_pressed=0;
}

void buttonIterrupt(){
	for(uint8_t i=0;i<3;i++){
		PORTB->PCR[(R4+i)] |=  PORT_PCR_PE_MASK |		
											 PORT_PCR_PS_MASK;			
	}
	for(uint8_t i=0;i<4;i++){
		PTB->PDDR |= (1<<(C4+i));
		PTB->PCOR |= (1<<(C4+i)); 
	}
}
void maskin(){
	for(uint8_t i=0;i<3;i++){
		PORTB->PCR[(R4+i)] |=  PORT_PCR_IRQC(0xA);	
	}
}
void buttonsInitialize(void){
	SIM->SCGC5 |=  SIM_SCGC5_PORTB_MASK;
		// pins PTB2-PTB8 rows 4-2, cols 4-1
	PORTB->PCR[R4] |= PORT_PCR_MUX(1);      	
	PORTB->PCR[R3] |= PORT_PCR_MUX(1);      	
	PORTB->PCR[R2] |= PORT_PCR_MUX(1); 
	PORTB->PCR[C4] |= PORT_PCR_MUX(1);
	PORTB->PCR[C3] |= PORT_PCR_MUX(1);
	PORTB->PCR[C2] |= PORT_PCR_MUX(1);
	PORTB->PCR[C1] |= PORT_PCR_MUX(1);
	
	buttonIterrupt();
	maskin();
	
	NVIC_ClearPendingIRQ(myPORT_IRQn);				
	NVIC_EnableIRQ(myPORT_IRQn);							
	NVIC_SetPriority (myPORT_IRQn, 3);
	
}
void keypadSweep(){ // working fine
	//set rows as input and columns as output
	for(uint8_t i=0;i<3;i++){
		PORTB->PCR[(R4+i)] |=  PORT_PCR_PE_MASK |		
											 PORT_PCR_PS_MASK;	
		PTB->PDDR &= ~(1<<(R4+i));
	}
	for(uint8_t i=0;i<4;i++){
		PTB->PDDR |= (1<<(C4+i));
		PTB->PCOR |= (1<<(C4+i)); 
	}
	
	for(uint8_t i=0;i<3;i++){
		if(( PTB->PDIR & (1<<(R4+i)) ) ==0)
		{
			is_pressed=1;
			R_pressed_previous = R_pressed;
			R_pressed=i;
		}
	}
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
	
	for(uint8_t i=0;i<4;i++){
		if(( PTB->PDIR & (1<<(C4+i)) ) ==0)
		{
			C_pressed_previous=C_pressed;
			C_pressed=i;
		}
	}
	// pressed key defined by C_pressed and R_pressed combined
}
void PORTB_IRQHandler(void){		
	keypadSweep();
	for(uint8_t i=0;i<3;i++){
		PORTB->PCR[(R4+i)] |= PORT_PCR_ISF_MASK;	
	} 
	buttonIterrupt();
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
