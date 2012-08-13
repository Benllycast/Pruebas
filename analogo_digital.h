#ifndef ANALOGO_DIGITAL_H
#define ANALOGO_DIGITAL_H


#ifndef CANAL_VELOCIDAD
	#define CANAL_VELOCIDAD 0
#endif
#ifndef CANAL_ACCELERACION
	#define CANAL_ACCELERACION 1
#endif
#ifndef CANAL_REVOLUCIONES
	#define CANAL_REVOLUCIONES 2
#endif

int AD_init_adc();
int AD_leer_canal(int canal, long *buffer);
#endif
