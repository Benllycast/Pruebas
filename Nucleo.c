#include "Nucleo.h"
// #include "analogo_digital.h"
// #include "comunicacion.h"
// #include "accelerometro.h"
//#include "captura_frecuencia.h"
#include "memoria.h"
// #include "ds1307.h"
// #include "utilidades.h"

#define acc_eje_x		0
#define acc_eje_y		1
#define acc_eje_z		2
#define velocidad 	3
#define revolucion	4

#define INDICADOR_AMARILLO	PIN_E0
#define INDICADOR_USB		PIN_E1

/*===============================funciones de debug===========================*/
#ifndef SIMULACION
int1 _debug_usb(void){
	if(COM_sense() == USB_OK){
      output_bit(INDICADOR_USB, 1);
      return (1);
	}else{
		output_bit(INDICADOR_USB, 0);
		return (0);
	}
}
#else
	#define _debug_usb() 1
#endif

#include "test.c"	// comentar esto en la aplicacion final


int myerror = 0;
#ifndef SIMULACION
void test_real(void);
#endif

/*======================= configuracon de dispositivos =======================*/
void setup_devices(){
	//int myerror = 0;
   /*========================= configuracion del USB =========================*/
   myerror = COM_init();
   /*========================= configuracion del MMA7455 =====================*/
   //myerror += MEMORIA_init_hw();
   //myerror += MEMORIA_init();
   
   /*========================= conversor analogo/digital =====================*/
   // myerror = AD_init_adc();
   
   /*========================= modulo CPP ====================================*/
   //myerror = CP_init_ccp();
   
   /*========================= configuracion del Reloj Digital ===============*/
   //ds1307_init(DS1307_OUT_ON_DISABLED_HIHG | DS1307_OUT_ENABLED | DS1307_OUT_1_HZ);
   //ds1307_set_date_time(0x0d, 0x01, 0x0d, 0x00, 0x0a, 0x2a, 0x00);
   
   
   
   /*-------------------------------------------------------------------------*/
   setup_psp(PSP_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   
   #ifndef CAPTURA_FRECUENCIA_H
	setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
	setup_ccp1(CCP_OFF);
   #endif
   
   /*-------------------------------------------------------------------------*/
   
   /*===================para los indicadores========================*/
   set_tris_e(0x00);
   set_tris_b(0x00);
   set_tris_c(0x80);		//configuracion para el modulo de memoria
   set_tris_d(0x48);
   output_bit(INDICADOR_USB, 0);
   output_bit(INDICADOR_AMARILLO, 1);
   ////////////////////////////////
   output_low(SPI_SCL);
   output_high(SPI_SS);
   output_low(SPI_MOSI);
   output_high(SPI_MISO);
   ////////////////////////////////
   //delay_ms(3000);
   /*===============================================================*/
   return;
}

/*===========================================================================
||										 MAIN 													||
=============================================================================*/
void main(void) {
	short execute = 0;
	setup_devices();
   while(1){
		if(_debug_usb()){
			test_comunicacion();
			test_memoria();
			//test_ccp();
		}else{
			output_bit(INDICADOR_AMARILLO, execute);
			execute = !execute;
			delay_ms(333);
		}
  	}
}

//=============================================================

