#include "Nucleo.h"
#include "accelerometro.h"
#include "analogo_digital.h"
#include "captura_frecuencia.h"
#include "memoria.h"
#include "ds1307.h"

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
   ds1307_init(DS1307_OUT_ENABLED | DS1307_OUT_1_HZ);
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
   return;
}

void main()
{
   byte dia = 0x01, mes = 0x01, ano = 0x0C, 
         hora = 0x01, min = 0x01, sec = 0x01, dow = 0x00; 
   setup_devices();
   ds1307_set_date_time(dia, mes, ano, dow, hora, min, sec);
   // TODO: USER CODE!!  
   while(1){
      ds1307_get_date(dia, mes, ano, dow);
      printf("fecha: %u/%u/%u dow: %u \n\r",dia, mes, ano, dow);
      delay_ms(99);
      ds1307_get_time(hora, min, sec);
      printf("hora: %u:%u:%u\n\r", hora, min, sec);
      delay_ms(900);
   }
}

