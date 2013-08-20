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

char testfile[] = "prueba";
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

void save(){
	char * bData;
	int nBytes = 0, escritos = 0;
	/*if(_debug_usb()){
		//printf(usb_cdc_putc,"\n\r%u/%u/%u(%u:%u:%u) S:%u N:%u V:%Lu",
		printf("\n\r%u/%u/%u(%u:%u:%u) S:%u N:%u V:%Lu",
			DS_dia, DS_mes,DS_anio,
			DS_hor,DS_min,DS_sec, sensor, no_data, value);
	}  */  
	printf("\n\r%u/%u/%u(%u:%u:%u) S:%u N:%u V:%Lu",
			data.dia, data.mes, data.anio,
			data.hor, data.min, data.seg,
			data.sensor, data.no_data, data.value);
	bData = (char *)&data;
	nBytes = sizeof(data);
	if((myerror = MEMORIA_open(testfile, FILE_WR)) != 0){
		printf("\n\rE%u", myerror);
	}else{
		if( (myerror = MEMORIA_write(nBytes)) != 0 ){
			printf("\n\rE%u", myerror);
		}else{
			MEMORIA_set_data(bData, nBytes);
			printf("\n\rW%u", escritos);
			MEMORIA_close();
		}
	}
}
/*======================= declaracion de tareas =======================*/

#task (rate=120ms, max=10ms) 
void Tarea1()
{
	AD_leer_canal(0,&lectura);
	data.sensor = 0;
	data.value = lectura;
	++data.no_data;
	save();
	rtos_yield();
}

#task (rate=130ms, max=10ms)
void Tarea2()
{
	AD_leer_canal(1,&lectura);
	data.sensor = 1;
	data.value = lectura;
	++data.no_data;
	save();
	rtos_yield();
}

#task (rate=130ms, max=10ms)
void Tarea3()
{
	AD_leer_canal(2,&lectura);
	data.sensor = 2;
	data.value = lectura;
	++data.no_data;
	save();
	rtos_yield();
}

/*
#task (rate=40, max=10ms)
void rpm(){
	if(semaforo_ccp == 0){
		CP_activar_captura(CCP_CANAL_1);
	}
	//rtos_await(Q_CCP == 2);                       
	if(Q_CCP != 2){
		rtos_yield();	
	}
	resultado = CP_obtener_resultado();
	CP_desativar_captura();
	save(4, (numeros++), resultado);                                
	rtos_yield();
}*/

#task (rate=1s, max=10ms)
void Indicador()
{	
   output_bit(INDICADOR_AMARILLO, salida);
   salida = !(salida);
   rtos_yield();
}

#task (rate=500ms, max=10ms)
void reloj(){
	ds1307_get_date(data.dia, data.mes, data.anio, DS_vic);
	ds1307_get_time(data.hor, data.min, data.seg);
	rtos_yield();
}


//#include "test.c"	// comentar esto en la aplicacion final

/*======================= configuracon de dispositivos =======================*/
void setup_devices(){
   myerror = COM_init();
   myerror = MEMORIA_init_hw();
   myerror = MEMORIA_init();
   myerror = AD_init_adc();
   //myerror = CP_init_ccp();
   ds1307_init(DS1307_OUT_ON_DISABLED_HIHG | DS1307_OUT_ENABLED | DS1307_OUT_1_HZ);
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
   rtos_run(); //A partir de aquí comenzará la ejecución de las tareas
}

#include "comunicacion.c"
#include "analogo_digital.c"
#include "ds1307.c"
#include "captura_frecuencia.c"
#include "memoria.c"
#include "utilidades.c"

/*======================= implementacion de tareas =======================*/


