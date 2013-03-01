#include "Nucleo.h"
#include "comunicacion.h"
//#include "accelerometro.h"
//#include "analogo_digital.h"
//#include "captura_frecuencia.h"
#include "memoria.h"
//#include "ds1307.h"
//#include "utilidades.h"



#define INDICADOR_AZUL PIN_E0
#define INDICADOR_ROJO PIN_E1
#define INDICADOR_AMARILLO   PIN_E2
#define MAX 40

//extern CONFIG_MMA7455 CONFIG;
//extern unsigned int32 tiempo_inicial, tiempo_final;

int myerror = 0;
/*
byte hr = 0, min = 0, sec = 0, dia = 0, mes = 0, anio = 0, dow = 0;
int16 valor_16b = 0;
//int32 valor_32b;


enum modulos {ACC,VEL,REV,ACCM,SENP,CCP1,CCP2};
const char mod_to_str [][*] = {"ACC","VEL","REV","ACCM","SENP","CCP1","CCP2"};
*/
char testfile[] = "test.txt";
char mensaje[] = "prueba de escritura\n\r";
char buffer[MAX];


/*==========================funcriones de prueba==============================*/
/*
void test1(void);
void test2(void);
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
	//int myerror = 0;
   /*========================= configuracion del USB =========================*/
   myerror = COM_init();
   /*========================= configuracion del MMA7455 =====================*/
   //myerror += MEMORIA_init_hw();
   //myerror += MEMORIA_init();
   
   /*========================= conversor analogo/digital =====================*/
   //error = AD_init_adc();
   
   /*========================= modulo CPP ====================================*/
   //error = CP_init_ccp();
   
   /*========================= configuracion del Reloj Digital ===============*/
   //ds1307_init(DS1307_OUT_ON_DISABLED_HIHG | DS1307_OUT_ENABLED | DS1307_OUT_1_HZ);
   //ds1307_set_date_time(0x0d, 0x01, 0x0d, 0x00, 0x0a, 0x2a, 0x00);
   
   
   
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
   set_tris_d(0x80);		//configuracion para el modulo de memoria					
   output_bit(INDICADOR_AZUL, 0);
   output_bit(INDICADOR_ROJO, 1);
   output_bit(INDICADOR_AMARILLO, 1);
   delay_ms(3000);
   /*===============================================================*/
   return;
}
/*===============================funciones de debug===========================*/

int1 _debug_usb(void){
	if(COM_sense() == USB_OK){
		//sprintf(mensaje,"USB Conectado!!!");
      output_bit(INDICADOR_AZUL, 1);
      output_bit(INDICADOR_ROJO, 0);
      //COM_printf(mensaje);
      return (1);
	}else{
		//printf("no conectado :(\n\r");
		output_bit(INDICADOR_AZUL, 0);
		output_bit(INDICADOR_ROJO, 1);
		return (0);
	}
}

/*===========================================================================
||										 MAIN 													||
=============================================================================*/
void main(void) {
	unsigned int8 aux = 0, size = 0;
	unsigned int32 tamano = 0;
	char op = 0;
   setup_devices();
   size = strlen(mensaje);
   memset(buffer, 0, MAX);
   while(1){
		if(_debug_usb()){
			aux++;
			printf(usb_cdc_putc,"\n\rtest %u", aux);
			while(!(op = usb_cdc_getc()));
			switch(op){
				case '0':
					MEMORIA_reset();
					break;
				case '1':
					myerror = MEMORIA_init_hw();
					break;
				case '2':
					myerror = MEMORIA_init();
					break;
				case 'i':
					MEMORIA_getinfo();
					break;
				case 'e':
					myerror = MEMORIA_open(testfile, FILE_WR);
					break;
				case 'w':
					myerror = MEMORIA_write(size);
					break;
				case 's':
					myerror = MEMORIA_set_data(mensaje,size);
					break;
				case 'l':
					myerror = MEMORIA_open(testfile, FILE_RD);
					break;
				case 'r':
					tamano = MEMORIA_read(size);
					printf(usb_cdc_putc_fast,"\n\rt%Lu", tamano);
					break;
				case 'g':
					myerror = MEMORIA_get_data(buffer);
					printf(usb_cdc_putc_fast,"\n\r%d %s", myerror, buffer);
					break;
				case 'c':
					myerror = MEMORIA_cancel();
					break;
				case 'x':
					myerror = MEMORIA_close();
					break;
				default:
					delay_ms(5000);
			}
			printf(usb_cdc_putc_fast,"\n\rE%d", myerror);
			memset(buffer, 0, MAX);
			myerror = 0;
			tamano = 0;
		}
  	}
}

//=============================================================
/*
void test1(void){
	leer_aceleracion();
   leer_velocidad();
   leer_revoluciones();
	return;
}
*/
/*
void test2(void){
	char c = 0x00;
	unsigned int cant = 0;
	do{
		printf(usb_cdc_putc,"\n\rop: ");
		c = usb_cdc_getc();
		if(c == 13) 
			continue;
		else
			usb_cdc_putc(c);
		
		switch(c){
			case 'a':
				printf(usb_cdc_putc,"\n\rmsg: ");
				get_string_usb(mensaje, MAX);
				cant = strlen(mensaje);
				printf(usb_cdc_putc,"\n\rnw msg: %s", mensaje);
				break;
			case 'b':
				printf(usb_cdc_putc,"\n\rls msg: %s", mensaje);
				break;
			case 'w':
				myerror += MEMORIA_open(testfile,'w');
				sprintf(mensaje,"MEM open: %d", myerror);
   			COM_printf(mensaje);
				myerror += MEMORIA_write(cant);
				myerror += MEMORIA_set_data(mensaje, cant);
				myerror += MEMORIA_close();
				break;
			default:
				c = 'x';
		}
	}while(c != 'x');
	return;
}
//=============================================================

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
