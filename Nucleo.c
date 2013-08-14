#include "Nucleo.h"
#use RTOS(timer=0, minor_cycle=10ms, statistics)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef SIMULACION
	#include "comunicacion.h"
#endif
#include "analogo_digital.h"
#include "ds1307.h"
#include "memoria.h"
#include "captura_frecuencia.h"
#include "utilidades.h"

int1 salida = 0;
int myerror = 0;
int numeros = 0;

/*======================= declaracion de tareas =======================*/
#task (rate=200ms, max=10ms) //Ejecutar cada 1 segundo y consumir como máximo 10ms
void Tarea1()
{
	if(_debug_usb()){
		printf(usb_cdc_putc_fast,"\n\rRTOS %u", numeros);
		numeros++;
	}
	rtos_yield();
}

#task (rate=1s, max=10ms) //Ejecutar cada 1 segundo y consumir como máximo 10ms
void Tarea2()
{	
   output_bit(INDICADOR_AMARILLO, salida);
   salida = !(salida);
   rtos_yield();
}

//#include "test.c"	// comentar esto en la aplicacion final

/*======================= configuracon de dispositivos =======================*/
void setup_devices(){
   myerror = COM_init();
   //myerror += MEMORIA_init_hw();
   //myerror += MEMORIA_init();
   //myerror = AD_init_adc();
   //myerror = CP_init_ccp();
   //ds1307_init(DS1307_OUT_ON_DISABLED_HIHG | DS1307_OUT_ENABLED | DS1307_OUT_1_HZ);
   //ds1307_set_date_time(0x0d, 0x01, 0x0d, 0x00, 0x0a, 0x2a, 0x00);
   
   /*=================== modulos hw no usados ========================*/
   setup_psp(PSP_DISABLED);
   setup_wdt(WDT_OFF);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
     
   #ifndef CAPTURA_FRECUENCIA_H
	setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
	setup_ccp1(CCP_OFF);
   #endif
   
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);   
   
   /*===================para los indicadores========================*/
   set_tris_e(0x00);
   set_tris_b(0x00);
   set_tris_c(0x80);		//configuracion para el modulo de memoria
   set_tris_d(0x48);
   output_bit(INDICADOR_USB, 0);
   output_bit(INDICADOR_AMARILLO, 1);
   output_low(SPI_SCL);
   output_high(SPI_SS);
   output_low(SPI_MOSI);
   output_high(SPI_MISO);
   //delay_ms(3000);
   return;
}

/*===========================================================================
||										 MAIN 													||
=============================================================================*/
void main(void) {
	setup_devices();
   rtos_run(); //A partir de aquí comenzará la ejecución de las tareas
}

#include "comunicacion.c"
#include "analogo_digital.c"
#include "ds1307.c"
#include "captura_frecuencia.c"
#include "memoria.c"
#include "utilidades.c"

/*======================= implementacion de tareas =======================*/


