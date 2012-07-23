#include "Nucleo.h"
#include <stdio.h>
#include <stdlib.h>
#include "address.h"
#define READ  0b00000000
#define WRITE 0b10000000

unsigned int8 respuesta = 0x00;
char* SSPSTAT = 0x0FC7;
char* SSPBUF = 0x0FC9;
void write_mma(int8 address){
   while(!spi_data_is_in());
}

unsigned int8 read_mma(unsigned int8 address){
   address<<=1;
   address|=READ;
   //swap(address);
   spi_write(address);
   //while(!spi_data_is_in()){;} //comprobar con un tiempo espesifico
   if(spi_data_is_in()){
      respuesta = spi_read();
      return respuesta;
   }      
   return -1;
}


void main()
{
   unsigned int8 count = 0x1F;
   unsigned int8 direcciones = 0x00;
   

   setup_adc_ports(NO_ANALOGS|VSS_VDD);
   setup_adc(ADC_CLOCK_DIV_2|ADC_TAD_MUL_0);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_MASTER|SPI_L_TO_H|SPI_CLK_DIV_16);
   setup_wdt(WDT_OFF);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   setup_ccp1(CCP_OFF);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
//Setup_Oscillator parameter not selected from Intr Oscillator Config tab
   // TODO: USER CODE!!
   set_tris_A(0b00001111);
   output_a(0xf0);
   delay_ms(1000);
   while(true){
      for (direcciones = 0x00; direcciones < count; direcciones++){
         /* code */
         printf("ms rq: %X , ",direcciones);
         if (read_mma(direcciones) != -1)
         {            
            printf("rp: %X \n\r",respuesta);
         }         
      }
      direcciones = 0x00;
      /*aqui va el codigo*/
      /*for (i = 0; i < count; ++i){
         printf("%d \n\r",i );
      }*/
      
   }
}
