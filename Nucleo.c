#include "Nucleo.h"
#include <stdio.h>
#include <stdlib.h>
#include "accelerometro.h"
#include "analogo_digital.h"

extern CONFIG_MMA7455 CONFIG;
void setup_devices(){
   int error = -1;
   /////////////configuracion del MMA7455////////////////
   //error = init_MMA();
   //////////////////////////////////////////////////////
   error = AD_init_adc();
   
   printf("init_error = %d\n\r", error);
   setup_psp(PSP_DISABLED);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   setup_ccp1(CCP_OFF);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   return;
}

void main()
{
   long value = 0;
   int error = 0;
   setup_devices();
   // TODO: USER CODE!!  
   while(error == 0){
      error = AD_leer_canal(CANAL_VELOCIDAD, &value);
      printf("VEL: %Ld \t", value);
      error = AD_leer_canal(CANAL_ACCELERACION, &value);
      printf("ACC: %Ld \t", value);
      error = AD_leer_canal(CANAL_REVOLUCIONES, &value);
      printf("RPM: %Ld \n\r", value);      
   }
}

