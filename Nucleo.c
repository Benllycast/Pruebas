#include "Nucleo.h"
#define use_rtos
#ifdef use_rtos
	#use RTOS(timer=0, minor_cycle=50ms, statistics)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comunicacion.h"
//#include "memoria.h"
#include "ds1307.h"
#include "analogo_digital.h"
//#include "captura_frecuencia.h"
#include "utilidades.h"
#include "configuracion.h"
//#use rs232(baud=9600,parity=N,xmit=PIN_XMIT,rcv=PIN_RCV,bits=8)

#ifdef use_rtos
void activar_tareas();
void desactivar_tareas();


#ifdef MEMORIA_H
void guardar(){
	unsigned int nBytes = 0, escritos = 0;
	if(input(PIN_LOG) == LOG_ENABLE){
		sprintf(buffer_log,LOG_LINE,
				data.dia, data.mes, data.anio,
				data.hor, data.min, data.seg,
				data.sensor, data.no_data, data.value, data.crc
				);
			
		nBytes = strlen(buffer_log);
		if((myerror = MEMORIA_open(testfile, FILE_WR)) != 0){
			printf(usb_cdc_putc_fast,"\n\rMO%d",myerror);
		}else{
			if( (myerror = MEMORIA_write(nBytes)) != 0 ){
				printf(usb_cdc_putc_fast,"\n\rMW%d",myerror);
			}else{
				escritos = MEMORIA_set_data(buffer_log, nBytes);
				printf(usb_cdc_putc_fast,"\n\rMS%d", escritos);
				MEMORIA_close();
			}
		}
	}
	if(_debug_usb()){printf(usb_cdc_putc_fast,"%s %u",buffer_log, nBytes);}
}
#else
void guardar(){
	if(_debug_usb()){
		if(input(PIN_LOG) == LOG_ENABLE){
			#ifdef SIMULACION
			printf("\n\r%u/%u/%u(%u:%u:%u) S:%u N:%u V:%Lu",
				data.dia, data.mes, data.anio,
				data.hor, data.min, data.seg,
				data.sensor, data.no_data, data.value);
			#else
			printf(usb_cdc_putc_fast,"\n\r%u/%u/%u(%u:%u:%u) S:%u N:%u V:%Lu",
				data.dia, data.mes, data.anio,
				data.hor, data.min, data.seg,
				data.sensor, data.no_data, data.value);
			#endif
		} else {
			#ifndef SIMULACION
			if(_debug_usb()){printf(usb_cdc_putc_fast,noLog);}
			#else
			printf(nolog);
			#endif
		}
	}
}
#endif	//MEMORIA_H

#task (rate=500ms, max=5ms)
void proceso2()
{	
	salida = (salida)? 0 : 1;
	if(salida) output_bit(INDICADOR_RUN, 1);
	else output_bit(INDICADOR_RUN, 0);
   //rtos_yield();
}
/*
#task (rate=100ms, max=50ms)
void testMemoria(){
	data.sensor = 255;
	data.value = rand();
	data.crc = rand();
	++data.no_data;
	guardar();
	rtos_yield();
} 
*/

#ifdef DS1307_H
#task (rate=500ms, max=50ms)
void reloj(){
	ds1307_get_date(data.dia, data.mes, data.anio, DS_vic);
	ds1307_get_time(data.hor, data.min, data.seg);
	//rtos_yield();
}
#endif	//DS1307_H

#ifdef ANALOGO_DIGITAL_H
	#task (rate=200ms, max=20ms)
	void Tarea1()
	{
		AD_leer_canal(ACC_x,&lectura);
		data.sensor = ACC_x;
		data.value = lectura;
		++data.no_data;
		guardar();
		//rtos_yield();
	}
	
	#task (rate=200ms, max=20ms)
	void Tarea2(){
		AD_leer_canal(ACC_y,&lectura);
		data.sensor = ACC_y;
		data.value = lectura;
		++data.no_data;
		guardar();
		//rtos_yield();
	}
	
	
	#task (rate=200ms, max=20ms)
	void Tarea3(){
		AD_leer_canal(ACC_z,&lectura);
		data.sensor = ACC_z;
		data.value = lectura;
		++data.no_data;
		guardar();
		//rtos_yield();
	}
	
	#task (rate=200ms, max=20ms)
	void leer_AD_VEL(){
		AD_leer_canal(AD_VEL,&lectura);
		data.sensor = AD_VEL;
		data.value = lectura;
		++data.no_data;
		guardar();
		//rtos_yield();
	}
	
	#task (rate=200ms, max=20ms)
	void leer_AD_REV(){
		AD_leer_canal(AD_REV,&lectura);
		data.sensor = AD_REV;
		data.value = lectura;
		++data.no_data;
		guardar();
		//rtos_yield();
	}
#endif	//ANALOGO_DIGITAL_H

#ifdef CAPTURA_FRECUENCIA_H
	#task (rate=300ms, max=50ms)
	void rpm(){
		// agregar una condicion para comprobar que esta tarea este abilitada para
		// ejecutarse
		
		if(!CP_ocupado()){
			desactivar_tareas();
			if(canal_ccp == CCP_CANAL_1){
				canal_ccp = (sensor_activo(CCP_VEL))? CCP_CANAL_2 : CCP_CANAL_1;
			}else	if(canal_ccp == CCP_CANAL_2){
				canal_ccp = (sensor_activo(CCP_REV))? CCP_CANAL_1 : CCP_CANAL_2;
			}
			
			//canal_ccp = (canal_ccp == CCP_CANAL_1)? CCP_CANAL_2 : CCP_CANAL_1;
			CP_activar_captura(canal_ccp);
		}
		//rtos_await(Q_CCP == 2);                       
		if(!CP_done()){
			rtos_yield();
		}  
		data.value = CP_obtener_resultado();
		CP_desativar_captura();
		data.sensor = (canal_ccp == CCP_CANAL_1)? CCP_REV: CCP_VEL;
		data.no_data++;
		guardar();
		activar_tareas();
		//rtos_yield();
	}
#endif	//CAPTURA_FRECUENCIA_H

void activar_tareas(){
	rtos_enable(reloj);
	rtos_enable(proceso2);
	#ifdef ANALOGO_DIGITAL_H
	rtos_enable(Tarea1);
	rtos_enable(Tarea2);
	rtos_enable(Tarea3);
	rtos_enable(leer_AD_REV);
	rtos_enable(leer_AD_VEL);
	#endif	//ANALOGO_DIGITAL_H
}

void desactivar_tareas(){
	rtos_disable(reloj);
	rtos_disable(proceso2);
	#ifdef ANALOGO_DIGITAL_H
	rtos_disable(Tarea1);
	rtos_disable(Tarea2);
	rtos_disable(Tarea3);
	rtos_disable(leer_AD_REV);
	rtos_disable(leer_AD_VEL);
	#endif	//ANALOGO_DIGITAL_H
}

#endif	//use_rtos

//#include "test.c"	// comentar esto en la aplicacion final

/*======================= configuracon de dispositivos =======================*/
void setup_devices(){
	iniciar_perifericos();
	
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
   set_tris_c(CONFIG_PORT_C);		//configuracion para el modulo de memoria y CCP. mirar Nucleo.h
   set_tris_d(CONFIG_PORT_D);
   output_bit(INDICADOR_USB, 0);
   output_bit(INDICADOR_AMARILLO, 1);
   output_bit(INDICADOR_RUN, 1);
   //output_low(SPI_SCL);
   //output_high(SPI_SS);
   //output_low(SPI_MOSI);
   //output_high(SPI_MISO);
   //delay_ms(3000);
   
   
   return;
}

/*===========================================================================
||										 MAIN 													||
=============================================================================*/
void main(void) {
	setup_devices();
	#ifdef use_rtos
	_debug_usb();
	if(input(PIN_LOG) == LOG_ENABLE){
		modo_configuracion();	// si esta el log activo entra en modo configuracion
	}else{
   	rtos_run(); //A partir de aquí comenzará la ejecución de las tareas
	}
   #else
   while(1){
		if(_debug_usb()){
			test_comunicacion();
			test_reloj();
			//test_ADC();
			//test_memoria();
			//test_ccp();
			delay_ms(333);
		}else{
			salida = (salida)? 0 : 1;
			if(salida) output_bit(INDICADOR_RUN, 1);
			else output_bit(INDICADOR_RUN, 0);
			delay_ms(333);
		}
  	}
   #endif
}

/*======================= implementacion de tareas =======================*/
#include "comunicacion.c"
#include "analogo_digital.c"
#include "ds1307.c"
#include "captura_frecuencia.c"
#include "memoria.c"
#include "utilidades.c"
#include "configuracion.c"

