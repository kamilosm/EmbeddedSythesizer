#include "waves.h"
void getSinSamples(int16_t base,int16_t ssc, float bnv, int16_t *buffer_ref){ 
	// function returns sin prepared for generatin signal
	// based on frequency representation in ssc
	// base - element of which we start
	// *buffer_ref - refferency to a table where the result will be stored(buffer) 
	// scc - sin samples count local varriable for this function
	// nv - note volume - ratio of how much volume note will be getting
	int r = 4*sin_tabl/ssc;
	for(uint8_t i=0;i<ssc;i++){
		if(i<(ssc/4)){
			buffer_ref[base+i]=sin_tab[i*r];
		}
		else if(i<ssc/2){
			buffer_ref[base+i]=sin_tab[sin_tabl-(r*(i-(ssc/4)))];
		}
		else if(i<(ssc*3)/4){
			buffer_ref[base+i]=-sin_tab[(i-(ssc/2))*r];
		}
		else{
			buffer_ref[base+i]=-sin_tab[sin_tabl-(r*(i-(3*ssc/4)))];
		}
	}
}
void getSquareSamples(int16_t base,int16_t ssc, float bnv, int16_t *buffer_ref){ // TODO
	// function returns square wave generated for given parameters
	
}
void getSawSamples(int16_t base,int16_t ssc, float bnv, int16_t *buffer_ref){ //TODO
	// function returns saw wave generated for given parameters
}
void getTriangleSamples(int16_t base,int16_t ssc, float bnv, int16_t *buffer_ref){ //TODO
	// function returns triangle wave generated for given parameters
}
