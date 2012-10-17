#include "Nucleo.h"
#include "comunicacion.h"
#include "accelerometro.h"
#include "analogo_digital.h"
#include "captura_frecuencia.h"
#include "memoria.h"
#include "ds1307.h"
#include "utilidades.h"


#define MAX 40;
//extern CONFIG_MMA7455 CONFIG;
//extern unsigned int32 tiempo_inicial, tiempo_final;
byte hr = 0, min = 0, sec = 0, dia = 0, mes = 0, anio = 0, dow = 0;
int16 valor_16b;
int32 valor_32b;

char mensaje[40];
enum modulos {ACC,VEL,REV,ACCM,SENP,CCP1,CCP2};
char * mod_to_str(unsigned int8);
void leer_aceleracion(void);
void leer_velocidad(void);
void leer_revoluciones(void);
void leer_accelerometro(void);
void leer_sensor_puertas(void);
void leer_ccp1(void);
void leer_ccp2(void);

void setup_devices(){
   int error = -1;
   /////////////configuracion del MMA7455////////////////
   //error = init_MMA();
   //////////////////////////////////////////////////////
   error = AD_init_adc();
   //////////////////////////////////////////////////////
   error = CP_init_ccp();
   //////////////////////////////////////////////////////
   //error = MEMORIA_init();
   //////////////////////////////////////////////////////
   ds1307_init(DS1307_OUT_ENABLED | DS1307_OUT_1_HZ);
   //////////////////////////////////////////////////////
   //////////////////////////////////////////////////////
   //error = COM_init();
   //////////////////////////////////////////////////////
   setup_psp(PSP_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   //setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   //setup_ccp1(CCP_OFF);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   printf("usb error =%d\n\r", error);
   return;
}

void main()
{
   //char* mensaje = "hola mundo\n\r";
   //int16 value = 0;
   
   setup_devices();
   //ds1307_set_date_time(0x0c, 0x0a, 0x0c, 0x01, 0x0a, 0x28, 0x00);
   while(1){
      /*
      if(COM_sense() != USB_NO_ATTACHED){
         COM_send(mensaje, strlen(mensaje));
         printf("%s", mensaje);
      }else
         printf("esperando a conectar....\n\r");
      */
      leer_aceleracion();
      leer_velocidad();
      leer_revoluciones();
      leer_ccp1();
      leer_ccp2();
      delay_ms(500);
   }
}
char * mod_to_str(unsigned int8 mod){
   char *str_mod;
   switch(mod){
      case ACC:
         str_mod = "ACC";
         break;
      case VEL:
         str_mod = "VEL";
         break;
      case REV:
         str_mod = "REV";
         break;
      case ACCM:
         str_mod = "ACCM";
         break;
      case SENP:
         str_mod = "SENP";
         break;
      case CCP1:
         str_mod = "CCP1";
         break;
      case CCP2:
         str_mod = "CCP2";
         break;
      default:
         str_mod = "UDF";
   }

   return &str_mod;
}

void leer_aceleracion(void){
   ds1307_get_time(hr, min, sec);
   ds1307_get_date(dia, mes, anio, dow);
   AD_leer_canal(CANAL_ACCELERACION , &valor_16b);
   sprintf(mensaje,"%u/%u/%u %u:%u:%u mod:%u val:%Ld",dia, mes, anio, hr, min, sec, ACC, valor_16b);
   printf("%s\n\r", mensaje);
   return;
}

void leer_velocidad(void){
   ds1307_get_time(hr, min, sec);
   ds1307_get_date(dia, mes, anio, dow);
   AD_leer_canal(CANAL_VELOCIDAD , &valor_16b);
   sprintf(mensaje,"%u/%u/%u %u:%u:%u mod:%u val:%Ld",dia, mes, anio, hr, min, sec, VEL, valor_16b);
   printf("%s\n\r", mensaje);
   return;
}

void leer_revoluciones(void){
   ds1307_get_time(hr, min, sec);
   ds1307_get_date(dia, mes, anio, dow);
   AD_leer_canal(CANAL_REVOLUCIONES , &valor_16b);
   sprintf(mensaje,"%u/%u/%u %u:%u:%u mod:%u val:%Ld",dia, mes, anio, hr, min, sec, REV, valor_16b);
   printf("%s\n\r", mensaje);
   return;
}

void leer_accelerometro(void){}
void leer_sensor_puertas(void){}

void leer_ccp1(void){
   ds1307_get_time(hr, min, sec);
   ds1307_get_date(dia, mes, anio, dow);
   CP_leer_ccp(CANAL_1 , &valor_32b);
   sprintf(mensaje,"%u/%u/%u %u:%u:%u mod:%u val:%Lu",dia, mes, anio, hr, min, sec, CCP1, valor_32b);
   printf("%s\n\r", mensaje);
}

void leer_ccp2(void){
   ds1307_get_time(hr, min, sec);
   ds1307_get_date(dia, mes, anio, dow);
   CP_leer_ccp(CANAL_1 , &valor_32b);
   sprintf(mensaje,"%u/%u/%u %u:%u:%u mod:%u val:%Lu",dia, mes, anio, hr, min, sec, CCP2, valor_32b);
   printf("%s\n\r", mensaje);
}
