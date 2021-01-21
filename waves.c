#include "waves.h"

int16_t halfvalue = 2047;
int16_t max_value = 4095;
int16_t min_value = 0;
void getSinSamplesWithOctave(int16_t base, int16_t ssc, float bnv, float onv, volatile int16_t* buffer_ref){ 
	// function returns sin prepared for generatin signal
	// based on frequency representation in ssc
	// base - element of which we start
	// *buffer_ref - refferency to a table where the result will be stored(buffer) ------ update
	// scc - sin samples count local varriable for this function
	// bnv - note volume - ratio of how much volume note will be getting   ------- update
	// onv - octave note volume
	
	/*
		Wersja z mniejszym uzyciem pamieci gdzie mamy tylko 1/4 sinusa zapisana
		musielismy jednak z niej zrezygnowac z powodu tego jak duzo takie dzialania kosztowaly cykli procesora
	int r=(4*sin_tabl)/ssc;
	int16_t ssc1_8 = ssc/8;
	int16_t ssc1_4 = ssc/4;
	int16_t ssc3_8 = ssc*3/8;
	int16_t ssc1_2 = ssc/2;
	int16_t ssc5_8 = ssc*5/8;
	int16_t ssc3_4 = ssc*3/4;
	int16_t ssc7_8 = ssc*7/8;
	if(i<ssc1_4){
		buff_ref[base+i] = bnv*(sin_tab[i*r]);
		if(i<(ssc1_8)){
			buff_ref[base+i] += onv*(sin_tab[i*2*r]);
		}
		else{
			buff_ref[base+i] += onv*(sin_tab[sin_tabl-(r*(i-(ssc1_8))*2)-1]);
		}
	}
	else if(i<ssc1_2){
		buff_ref[base+i] = bnv*(sin_tab[sin_tabl-(r*(i-(ssc1_4)))-1]);
		if(i<(ssc3_8){
			buff_ref[base+i] += onv*(sin_tab[sin_tabl-2*(i-(ssc1_4))*r-1]-halfvalue);
		}
		else{
			buff_ref[base+i] += onv*(sin_tab[(r*2*(i-(ssc3_8)))]-halfvalue);
		}
	}
	else if(i<ssc3_4){
		buff_ref[base+i] = bnv*(sin_tab[sin_tabl-(i-(ssc/2))*r-1]-halfvalue);
		if(i<ssc5_8){
				buff_ref[base+i] += onv*(sin_tab[(i-ssc1_2)*2*r]);
		}
		else{
			buff_ref[base+i] += onv*(sin_tab[sin_tabl-(r*(i-(ssc5_8))*2)-1]);
		}
	}
	else{
		buff_ref[base+i] = bnv*(sin_tab[(r*(i-(3*ssc/4)))]-halfvalue);
		if(i<ssc7_8){
			buff_ref[base+i] += onv*(sin_tab[sin_tabl-2*(i-(ssc3_4))*r-1]-halfvalue);
		}
		else{
			buff_ref[base+i] += onv*(sin_tab[(r*2*(i-(ssc7_8)))]-halfvalue);
		}
	}
}*/
	int16_t r = (sin_tabl/ssc);
	for(volatile uint16_t i=0;i<ssc;i++){
		buffer_ref[base+i] = bnv*(sin_tab[i*r])+onv*(sin_tab[((i*2)%ssc)*r]);
	}
}
void getSinSamples(int16_t base, int16_t ssc, float bnv, volatile int16_t* buffer_ref){ 
	// function returns sin prepared for generatin signal
	// based on frequency representation in ssc
	int16_t r = (sin_tabl/ssc);
	for(volatile uint16_t i=0;i<ssc;i++){
			buffer_ref[base+i] = bnv*(sin_tab[i*r]);
		}
	
}
void getSquareSamplesWithOctave(int16_t base, int16_t ssc, float bnv, float onv, volatile int16_t* buffer_ref){
	// function returns square wave generated for given parameters
	// but this one also contains octave higher same wave shape generator
	int16_t ssc1_4= ssc/4;
	int16_t ssc2= ssc/2;
	int16_t ssc3_4= ssc*3/4;
	int16_t base_square = max_value*bnv;
	int16_t octave_square = max_value*onv;
	int16_t min_value_base = min_value*bnv;
	int16_t min_value_octave = min_value*onv;
	
	for(volatile uint16_t i=0;i<ssc;i++){
		if(i<ssc1_4){buffer_ref[base+i]= base_square + octave_square;}
		else if (i<ssc2){buffer_ref[base+i] = base_square + min_value_octave;}
		else if (i<ssc3_4){buffer_ref[base+i] = min_value_base + octave_square;}
		else{buffer_ref[base+i]= min_value_base + min_value_octave;}
	}
}
void getSquareSamples(int16_t base,int16_t ssc, float bnv,volatile int16_t *buffer_ref){
	// function returns square wave generated for given parameters
	int16_t big_square = max_value*bnv;
	int16_t min_value_base = min_value*bnv;
	for(volatile uint16_t i=0;i<ssc;i++){
		if(i<(ssc/2)){
			buffer_ref[base+i]=big_square;
		}else{
			buffer_ref[base+i]=min_value_base;
		}
	}
}
void getSawSamplesWithOctave(int16_t base, int16_t ssc, float bnv, float onv, volatile int16_t* buffer_ref){
	// function returns saw wave generated for given parameters
	// but this one also contains octave higher same wave shape generator
	int16_t step = (max_value/ssc)*bnv;
	int16_t step_oc = (max_value/(ssc/2))*onv;
	buffer_ref[base] = min_value;
	
	int16_t ssc2=ssc/2;
	for(volatile uint16_t i=1;i<ssc;i++){
		if (i==ssc2){buffer_ref[base+i] = step+buffer_ref[base+i-1]-(step_oc*i);}
		else {buffer_ref[base+i] = step+buffer_ref[base+i-1]+step_oc;}
	}
}
void getSawSamples(int16_t base,int16_t ssc, float bnv,volatile int16_t *buffer_ref){
	// function returns saw wave generated for given parameters
	int16_t step = (max_value/ssc)*bnv;
	buffer_ref[base] = min_value;
	for(volatile uint16_t i=1;i<ssc;i++){
		buffer_ref[base+i] = (step+buffer_ref[base+i-1]);
	}
}
void getTriangleSamplesWithOctave(int16_t base, int16_t ssc, float bnv, float onv, volatile int16_t* buffer_ref){
	// function returns triangle wave generated for given parameters
	// but this one also contains octave higher same wave shape generator
	int16_t step = bnv *2*(max_value/ssc);
	int16_t step_oc = onv *4*(max_value/ssc);
	int16_t ssc1_4= ssc/4;
	int16_t ssc2= ssc/2;
	int16_t ssc3_4= ssc*3/4;
	buffer_ref[base]=min_value*bnv+min_value*onv;
	for(volatile uint16_t i=1;i<ssc;i++){	
		if(i<=ssc1_4){buffer_ref[base+i]=step+buffer_ref[base+i-1]+step_oc;}
		else if(i<ssc2){buffer_ref[base+i]=step+buffer_ref[base+i-1]-step_oc; }
		else if(i<ssc3_4){buffer_ref[base+i]=buffer_ref[base+i-1]-step+step_oc; }
		else{buffer_ref[base+i]=buffer_ref[base+i-1]-step-step_oc;}
	}
}
void getTriangleSamples(int16_t base,int16_t ssc, float bnv,volatile int16_t *buffer_ref){
	// function returns triangle wave generated for given parameters
	int16_t step = bnv*((max_value*2)/ssc);
	int16_t ssc2= ssc/2;
	buffer_ref[base]=min_value;
	for(volatile uint16_t i=1;i<ssc;i++){
		if(i<(ssc/2)){
			buffer_ref[base+i]=(step+buffer_ref[base+i-1]);
		}else{
			buffer_ref[base+i]=(buffer_ref[base+i-1]-step);
		}
	}
}
