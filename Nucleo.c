#include "Nucleo.h"
#include <stdio.h>
#include <stdlib.h>
#include "address.h"
/**
enum registros{
   xoutl = 0x00, xouth = 0x01,//registros de medida de x a 10bit
   youtl = 0x02, youth = 0x03,//registros de medida de y a 10bit
   zoutl = 0x04, zouth = 0x05,//registros de medida de z a 10bit
   xout8 = 0x06, yout8 = 0x07, zout8 = 0x08,//registros de medida a 8bit
   status= 0x09,
   detsrc= 0x0A,
   tout  = 0x0B,
   reserved= 0x0C,
   i2cad = 0x0D,
   usrinf= 0x0E, whoami= 0x0F,
   xoffl = 0x10, xoffh = 0x11,
   yoffl = 0x12, yoffh = 0x13,
   zoffl = 0x14, zoffh = 0x15,
   mctl  = 0x16,
   intrst = 0x17,
   ctl1  = 0x18, ctl2 = 0x19,
   ldth  = 0x1A, pdth = 0x1B,
   pw    = 0x1C, lt = 0x1D, tw = 0x1E,
   reserved2 = 0x1F
};
*/
void main()
{
   char count = 0x1F;
   char direcciones = 0x00;
   char respuesta = 0x00;

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
   while(true){
      for (direcciones = 0x00; direcciones < count; direcciones++){
         /* code */
         spi_write(direcciones);
         //delay_ms(30);
         respuesta = spi_read();
         printf("respuesta: %c\n",respuesta );
      }
      direcciones = 0x00;
      /*aqui va el codigo*/
      /*for (i = 0; i < count; ++i){
         printf("%d \n\r",i );
      }*/
      delay_ms(500);
   }
}
