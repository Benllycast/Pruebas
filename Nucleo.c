#include "Nucleo.h"
#include "comunicacion.h"


//#include "accelerometro.h"
#include "analogo_digital.h"
//#include "captura_frecuencia.h"
//#include "memoria.h"
#include "ds1307.h"
//#include "utilidades.h"



#define INDICADOR_AZUL PIN_E0
#define INDICADOR_ROJO PIN_E1
#define INDICADOR_AMARILLO   PIN_E2
#define MAX 40

//extern CONFIG_MMA7455 CONFIG;
//extern unsigned int32 tiempo_inicial, tiempo_final;

byte hr = 0, min = 0, sec = 0, dia = 0, mes = 0, anio = 0, dow = 0;
int16 valor_16b = 0;
//int32 valor_32b;
char mensaje[40];
enum modulos {ACC,VEL,REV,ACCM,SENP,CCP1,CCP2};
const char mod_to_str [][*] = {"ACC","VEL","REV","ACCM","SENP","CCP1","CCP2"};

/*==========================funcriones de prueba==============================*/
void test1(void);
void leer_aceleracion(void);
void leer_velocidad(void);
void leer_revoluciones(void);

/*
void leer_accelerometro(void);
void leer_ccp1(void);
void leer_ccp2(void);
void leer_sensor_puertas(void);
*/


/*======================= configuracon de dispositivos =======================*/
void setup_devices(){
   int error = -1;
   
   /*========================= configuracion del MMA7455 =====================*/
   //error = MEMORIA_init();
   
   /*========================= conversor analogo/digital =====================*/
   //error = AD_init_adc();
   
   /*========================= modulo CPP ====================================*/
   //error = CP_init_ccp();
   
   /*========================= configuracion del Reloj Digital ===============*/
   //ds1307_init(DS1307_OUT_ON_DISABLED_HIHG | DS1307_OUT_ENABLED | DS1307_OUT_1_HZ);
   //ds1307_set_date_time(0x0d, 0x01, 0x0d, 0x00, 0x0a, 0x2a, 0x00);
   
   /*========================= configuracion del USB =========================*/
   error = COM_init();
   
   /*-------------------------------------------------------------------------*/
   setup_psp(PSP_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   //setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   //setup_ccp1(CCP_OFF);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   /*-------------------------------------------------------------------------*/
   
   /*===================para los indicadores========================*/
   set_tris_e(0x00);
   set_tris_b(0x00);
   output_bit(INDICADOR_AZUL, 0);
   output_bit(INDICADOR_ROJO, 1);
   output_bit(INDICADOR_AMARILLO, 1);
   delay_ms(3000);
   /*===============================================================*/
   return;
}

void _debug_usb(void){
	if(COM_sense() == USB_OK){
			sprintf(mensaje,"USB Conectado!!!");
         output_bit(INDICADOR_AZUL, 1);
         output_bit(INDICADOR_ROJO, 0);
         COM_printf(mensaje);
      }else{
      	//printf("no conectado :(\n\r");
         output_bit(INDICADOR_AZUL, 0);
         output_bit(INDICADOR_ROJO, 1);
      }
}
/*===========================================================================
||										 MAIN 													||
=============================================================================*/
void main(void)
{
   //char* mensaje = "hola mundo";
   setup_devices();   
   while(1){
   	_debug_usb();
   	//test1();
   	delay_ms(1000);
   }
}


//=============================================================

void test1(void){
	leer_aceleracion();
   leer_velocidad();
   leer_revoluciones();
	return;
}

void leer_aceleracion(void){
	hr = min = sec = 0;
	dia = mes = anio = 0;
   ds1307_get_time(hr, min, sec);
   ds1307_get_date(dia, mes, anio, dow);
   AD_leer_canal(CANAL_ACCELERACION , &valor_16b);
   sprintf(mensaje,"%d/%d/%d %d:%d:%d mod:%s val:%Ld",dia, mes, anio, hr, min, sec, mod_to_str[ACC], valor_16b);
   if(COM_READY)COM_printf(mensaje);
   return;
}

void leer_velocidad(void){
	hr = min = sec = 0;
	dia = mes = anio = 0;
   ds1307_get_time(hr, min, sec);
   ds1307_get_date(dia, mes, anio, dow);
   AD_leer_canal(CANAL_VELOCIDAD , &valor_16b);
   sprintf(mensaje,"%u/%u/%u %u:%u:%u mod:%s val:%Ld",dia, mes, anio, hr, min, sec, mod_to_str[VEL], valor_16b);
   if(COM_READY)COM_printf(mensaje);
   return;
}

void leer_revoluciones(void){
	hr = min = sec = 0;
	dia = mes = anio = 0;
   ds1307_get_time(hr, min, sec);
   ds1307_get_date(dia, mes, anio, dow);
   AD_leer_canal(CANAL_REVOLUCIONES , &valor_16b);
   sprintf(mensaje,"%u/%u/%u %u:%u:%u mod:%s val:%Ld",dia, mes, anio, hr, min, sec, mod_to_str[REV], valor_16b);
   if(COM_READY)COM_printf(mensaje);
   return;
}

/*
void leer_accelerometro(void){}
void leer_sensor_puertas(void){}

void leer_ccp1(void){
   ds1307_get_time(hr, min, sec);
   ds1307_get_date(dia, mes, anio, dow);
   CP_leer_ccp(CANAL_1 , &valor_32b);
   sprintf(mensaje,"%u/%u/%u %u:%u:%u mod:%s val:%Lu",dia, mes, anio, hr, min, sec, mod_to_str[CCP1], valor_32b);
   printf("%s\n\r", mensaje);
}

void leer_ccp2(void){
   ds1307_get_time(hr, min, sec);
   ds1307_get_date(dia, mes, anio, dow);
   CP_leer_ccp(CANAL_1 , &valor_32b);
   sprintf(mensaje,"%u/%u/%u %u:%u:%u mod:%s val:%Lu",dia, mes, anio, hr, min, sec, mod_to_str[CCP2], valor_32b);
   printf("%s\n\r", mensaje);
}
*/
