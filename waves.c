#include "waves.h"

int16_t halfvalue = 2047;
int16_t max_value = 4095;
int16_t min_value = 0;

void getSinSamples(int16_t base,int16_t ssc, float bnv, int16_t *buffer_ref){ 
	// function returns sin prepared for generatin signal
	// based on frequency representation in ssc
	// base - element of which we start
	// *buffer_ref - refferency to a table where the result will be stored(buffer) 
	// scc - sin samples count local varriable for this function
	// nv - note volume - ratio of how much volume note will be getting
	int r = 4*sin_tabl/ssc;
	for(uint16_t i=0;i<ssc;i++){
		if(i<(ssc/4)){
			buffer_ref[base+i]=(sin_tab[i*r]);
		}
		else if(i<ssc/2){
			buffer_ref[base+i]=(sin_tab[sin_tabl-(r*(i-(ssc/4)))-1]);
		}
		else if(i<(ssc*3)/4){
			buffer_ref[base+i]=(sin_tab[sin_tabl-(i-(ssc/2))*r-1]-halfvalue);
		}
		else{
			buffer_ref[base+i]=(sin_tab[(r*(i-(3*ssc/4)))]-halfvalue);
		}
	}
}
void getSquareSamples(int16_t base,int16_t ssc, float bnv, int16_t *buffer_ref){
	// function returns square wave generated for given parameters
	for(uint16_t i=0;i<ssc;i++){
		if(i<(ssc/2)){
			buffer_ref[base+i]+=max_value*bnv;
		}else{
			buffer_ref[base+i]+=min_value;
		}
	}
}
void getSawSamples(int16_t base,int16_t ssc, float bnv, int16_t *buffer_ref){
	// function returns saw wave generated for given parameters
	int16_t step = (max_value/ssc)*bnv;
	buffer_ref[base]=min_value;
	for(uint16_t i=1;i<ssc;i++){
		buffer_ref[base+i]=(step+buffer_ref[base+i-1]);
	}
}
void getTriangleSamples(int16_t base,int16_t ssc, float bnv, int16_t *buffer_ref){
	// function returns triangle wave generated for given parameters
	int16_t step = bnv*((max_value*2)/ssc);
	int16_t ssc2= ssc/2;
	buffer_ref[base]=min_value;
	for(uint16_t i=1;i<ssc;i++){
		if(i<(ssc/2)){
			buffer_ref[base+i]=(step+buffer_ref[base+i-1]);
		}else{
			buffer_ref[base+i]=(buffer_ref[base+i-1]-step);
		}
	}
}
