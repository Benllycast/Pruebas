#ifndef UTILIDADES_H
#define UTILIDADES_H

#define INDICADOR_AMARILLO	PIN_E0
#define INDICADOR_USB		PIN_E1
#define INDICADOR_RUN		PIN_E2
int16 calc_CRC(char *buffer, unsigned int8 leng);

#ifndef SIMULACION
	int1 _debug_usb();
	//#define PUERTO usb_cdc_putc_fast
#else
	#define _debug_usb() 1
	#use rs232(stream=PUERTO,baud=9600,parity=N,xmit=PIN_XMIT,rcv=PIN_RCV,bits=8) 
#endif

#endif
