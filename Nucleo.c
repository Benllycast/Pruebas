#include "Nucleo.h"
#include <stdio.h>
#include <stdlib.h>
#include "accelerometro.h"
#include "analogo_digital.h"
#include "captura_frecuencia.h"

extern CONFIG_MMA7455 CONFIG;
//extern unsigned int32 tiempo_inicial, tiempo_final;
void setup_devices(){
   int error = -1;
   /////////////configuracion del MMA7455////////////////
   //error = init_MMA();
   //////////////////////////////////////////////////////
   //error = AD_init_adc();
   //////////////////////////////////////////////////////
   error = CP_init_ccp();
   printf("init_error = %d\n\r", error);
   setup_psp(PSP_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   //setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   //setup_ccp1(CCP_OFF);
   //setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   return;
}

void main()
{
   int32 value = 0;
   int error = 0;
   setup_devices();
   // TODO: USER CODE!!  
   while(error == 0){
      error = CP_leer_ccp(CANAL_2, &value);
      printf("CCP canal 2: %Lu \n\r", value);
      delay_ms(250);
   }
}

