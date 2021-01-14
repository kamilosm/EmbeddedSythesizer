/*-------------------------------------------------------------------------
					Embedded Synth 
					Authors: Krzysztof Jedrejasz, Kamil Maczuga
					Version: 
----------------------------------------------------------------------------*/

/* Notatki notateczki
*trzeba pomyslec jak rozwiazac kwestie LFO tak zeby miało prawo to działać przy naszym rozmiarz buffora
*czy te małe buffory są okej, czy może jednak zdecydować się na wypuszczanie próbek i DSP na żywo bez nich? może buffor kołowy,
ale on chyba nie zmienia nic
* jak to jest z tym mnożeniem przez floaty, czy będziemy to musieli później optymalizowac? 

Do zrobienia teraz: (najwięcej gwizdek najmniejszy priorytet czasowy)
- naprawić ten sin i reszte ehhhhh ***  
- dodać oktawy i belnd dla nich **
- pozmieniac nazwy bo nie tylko sin ****
- s4 ma mniejszą częstotliwość o 2 hs niż powinien? 
Zadania, które są bardziej pytaniami:
- optymalizacja funcji generujących wavey ****
- a gdyby tak przy niezmieniajacym się dzwieku zostać na jednym bufferze bez generowania? ****
- napisać komentarze do każdego pliku/ zreadagować te które już są, każdy plik powinien mieć u góry mają opisówkę po co właściwie jest *******
- unormować nazewnictwo ********************
- zmienne z klaw przenieść do engine?(C_pressed, R_pressed) ************************
- przenieść PIT_IRQHandler do pitu, jak z includem dac? ****************************
- czasem naciśnięcie button2 zwraca button 4(s4), dlaczego? ****************************
++++++++++++++++++++++od ostatniego commita+++++++++++++++++++++++++++++++++++++++++++
- rozdzielenie funkcjonalnosci do engine.h/c 
- przebudowanie logiki przemiatania klawiatury(chyba już wszystko jest tak jak ma być) 
*/
#include "frdm_bsp.h"
#include "engine.h"
#include "parameters.h"
#include "DAC.h"
#include "MKL05Z4.h"
#include "klaw.h"
#include "pit.h"
#include <stdio.h>
#include "math.h"
#include <string.h>
#include <stdlib.h>
// bus clock 20971520Hz
int16_t sweep_pointer=0;
int16_t sweep_freq=400;
int main (void) 
{
	sweep_pointer=sweep_freq;
	initializevar();
	initializeParameters();
	buttonsInitialize();
	pitInitialize(476);
	DAC_Init();
	startPIT();

  while(1)
	{
		
	}
}
void dacIterrupt(){
	dacPlaySample();
}
void PIT_IRQHandler() {
		
	// check to see which channel triggered interrupt 
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 0
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
			dacIterrupt();
			if(sweep_pointer==0){
				keypadSweep();
				sweep_pointer=sweep_freq;
			}
			sweep_pointer--;
	} else if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 1
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
	}
	//clear pending IRQ
	NVIC_ClearPendingIRQ(myPIT_IRQn);
	
}

