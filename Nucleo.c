#include <18F67J50.h>
#include "Nucleo.h"
#include <math.h>
#include <stdio.h>

#INT_SSP
void  SSP_isr(VOID) {
	  
}            

void main(){
   setup_adc_ports ( ALL_ANALOG|VSS_VDD ) ;
   setup_adc ( ADC_CLOCK_DIV_2|ADC_TAD_MUL_0 ) ;
   //setup_psp ( PSP_DISABLED ) ;
   setup_spi ( SPI_MASTER|SPI_L_TO_H|SPI_CLK_DIV_4 ) ;
   setup_spi2 ( SPI_SS_DISABLED ) ;
   setup_wdt ( WDT_OFF ) ;
   setup_timer_0 ( RTCC_INTERNAL ) ;
   setup_timer_1 ( T1_DISABLED ) ;
   setup_timer_2 ( T2_DISABLED, 0, 1 ) ;
   setup_timer_3 ( T3_DISABLED|T3_DIV_BY_1 ) ;
   setup_timer_4 ( T4_DISABLED, 0, 1 ) ;
   setup_ccp1 ( CCP_OFF ) ;
   setup_comparator ( NC_NC_NC_NC ); // This device COMP currently not supported by the PICWizard
   enable_interrupts ( INT_SSP ) ;
   enable_interrupts ( GLOBAL ) ;

   //Setup_Oscillator parameter not selected from Intr Oscillator Config tab
   // TODO: USER CODE!!

   while(true){
      printf("%s\n","coloque su codigo aqui" );
   }
}

