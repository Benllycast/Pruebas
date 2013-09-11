#include "Nucleo.h"
#use RTOS(timer=0, minor_cycle=50ms, statistics)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comunicacion.h"
//#include "memoria.h"
#include "ds1307.h"
#include "utilidades.h"
//#use rs232(baud=9600,parity=N,xmit=PIN_XMIT,rcv=PIN_RCV,bits=8)
const char noLOg[] = "\n\rno se puede guardar";
const char testfile[] = "prueba";
int1 salida = 0;
int myerror = 0;
int16 lectura = 0;
//int32 resultado = 0;
struct Log {
	byte dia;
	byte mes;
	byte anio;
	byte hor;
	byte min;
	byte seg;
	int8 sensor;
	int8 no_data;
	int32 value;
	int16 crc;
} data;

void guardar(){
	unsigned int nBytes = (sizeof(testfile)-1), escritos = 0;
	if(input(PIN_LOG) == LOG_ENABLE){
		//myerror = MEMORIA_open(testfile, FILE_WR);
		printf(usb_cdc_putc,"\n\roE %d %d %d", data.hor, data.min, data.seg);
		//myerror = MEMORIA_write(nBytes)  ;                                
		printf(usb_cdc_putc_fast,"\n\rwE %d %d %d", data.dia, data.mes, data.anio);
		//escritos = MEMORIA_set_data(testfile, nBytes);
		printf(usb_cdc_putc,"\n\rs%d", escritos);
		//MEMORIA_close();
	}else{
		printf(usb_cdc_putc,noLog);
	}
	
}

#task (rate=250ms, max=50ms)
void proceso2()
{	
	salida = (salida)? 0 : 1;
	if(salida) output_bit(INDICADOR_RUN, 1);
	else output_bit(INDICADOR_RUN, 0);
	if(_debug_usb()) guardar();	
   rtos_yield();
}


#task (rate=500ms, max=50ms)
void reloj(){
	ds1307_get_date(data.dia, data.mes, data.anio, DS_vic);
	ds1307_get_time(data.hor, data.min, data.seg);
	rtos_yield();
}
//#include "test.c"	// comentar esto en la aplicacion final

/*======================= configuracon de dispositivos =======================*/
void setup_devices(){
   myerror = COM_init();
   //printf("\n\rusb E%d", myerror);
   //MEMORIA_reset();
   //myerror = MEMORIA_init_hw();
   //myerror = MEMORIA_init();
   //printf("\n\rmem E%d", myerror);
   //myerror = AD_init_adc();
   //myerror = CP_init_ccp();
   ds1307_init(DS1307_OUT_ON_DISABLED_HIHG | DS1307_OUT_ENABLED | DS1307_OUT_1_HZ);
   ds1307_set_date_time(0x0d, 0x01, 0x0d, 0x00, 0x0a, 0x2a, 0x00);
   
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
   set_tris_d(CONFIG_PORT_D);
   output_bit(INDICADOR_USB, 0);
   output_bit(INDICADOR_AMARILLO, 1);
   output_bit(INDICADOR_RUN, 1);
   //output_low(SPI_SCL);
   //output_high(SPI_SS);
   //output_low(SPI_MOSI);
   //output_high(SPI_MISO);
   //delay_ms(3000);
   data.dia = data.mes = data.anio = 0;
   data.hor = data.min = data.seg = 0;
   data.value = data.sensor = data.no_data = 0;   
   return;
}

/*===========================================================================
||										 MAIN 													||
=============================================================================*/
void main(void) {
	setup_devices();
	_debug_usb();
   rtos_run(); //A partir de aquí comenzará la ejecución de las tareas
}

/*======================= implementacion de tareas =======================*/
#include "comunicacion.c"
//#include "analogo_digital.c"
#include "ds1307.c"
//#include "captura_frecuencia.c"
//#include "memoria.c"
#include "utilidades.c"

