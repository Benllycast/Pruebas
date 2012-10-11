#include "Nucleo.h"
#include "analogo_digital.h"

#define ADC_TAD_MUL_2      0x08

int AD_init_adc(){
	setup_adc_ports(AN0_TO_AN2);

//corregir despues
/*
#if (getenv("CLOCK") == 4000000)
	setup_adc(ADC_CLOCK_DIV_8|ADC_TAD_MUL_2);
#elif (getenv("CLOCK") == 8000000)
	setup_adc(ADC_CLOCK_DIV_16|ADC_TAD_MUL_2);
#elif (getenv("CLOCK") == 10000000)
	setup_adc(ADC_CLOCK_DIV_32|ADC_TAD_MUL_2);
#elif (getenv("CLOCK") == 20000000)
	setup_adc(ADC_CLOCK_DIV_64|ADC_TAD_MUL_2);
#else
	setup_adc(ADC_CLOCK_DIV_32|ADC_TAD_MUL_2);
#endif
*/	
	setup_adc(ADC_CLOCK_DIV_32|ADC_TAD_MUL_2);
	set_tris_a(0x07);		//RB0 a RB1 entradas
	return 0;
}

int AD_leer_canal(int canal, int16 *buffer){
	int1 done;
	set_adc_channel(canal);
	read_adc(ADC_START_ONLY);
	
	do {
		done = adc_done();
	}while(!done);
	*buffer = read_adc(ADC_READ_ONLY);
	return 0;
}
