#include "Nucleo.h"
#include "comunicacion.h"
#include "accelerometro.h"
#include "analogo_digital.h"
#include "captura_frecuencia.h"
#include "memoria.h"
#include "ds1307.h"
#include "utilidades.h"


#define MAX 20;
//extern CONFIG_MMA7455 CONFIG;
//extern unsigned int32 tiempo_inicial, tiempo_final;
void setup_devices(){
   int error = -1;
   /////////////configuracion del MMA7455////////////////
   //error = init_MMA();
   //////////////////////////////////////////////////////
   //error = AD_init_adc();
   //////////////////////////////////////////////////////
   //error = CP_init_ccp();
   //////////////////////////////////////////////////////
   //error = MEMORIA_init();
   //////////////////////////////////////////////////////
   //ds1307_init(DS1307_OUT_ENABLED | DS1307_OUT_1_HZ);
   //////////////////////////////////////////////////////
   //////////////////////////////////////////////////////
   error = COM_init();
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
   char* mensaje = "hola mundo\n\r";
   setup_devices();
   while(1){
      if(COM_sense() != USB_NO_ATTACHED){
         COM_send(mensaje, strlen(mensaje));
         printf("%s", mensaje);
      }else
         printf("esperando a conectar....\n\r");
      
      delay_ms(500);
   }
}

