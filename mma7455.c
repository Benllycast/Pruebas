/**
codigo de prueba del accelerometro mma7455l 
*/
#include "mma7455.h"
  #include <stdio.h>
  #include <stdlib.h>
#define _num_reg 0x20
int8 memory[_num_reg], data, instr, address;

enum registros{
   xoutl = 0x00, xouth = 0x01,//registros de medida de x a 10bit
   youtl= 0x02, youth= 0x03,//registros de medida de y a 10bit
   zoutl= 0x04, zouth= 0x05,//registros de medida de z a 10bit
   xout8= 0x06, yout8= 0x07, zout8= 0x08,//registros de medida a 8bit
   status= 0x09,
   detsrc= 0x0A,
   tout= 0x0B,
   reserved= 0x0C,
   i2cad= 0x0D,
   usrinf= 0x0E, whoami= 0x0F,
   xoffl= 0x10, xoffh= 0x11,
   yoffl= 0x12, yoffh= 0x13,
   zoffl= 0x14, zoffh= 0x15,
   mctl= 0x16,/*selector gravedad (bit[3,2]) 8g = 00, 2g = 01, 4g = 10
                selector mode (bit[1,0]) stanby = 00, measurement= 01, level_detect= 10, pulse_detec= 11
                SPI mode (bit[4]) 4 wire = 0, 3 wire = 1
                data output pin (bit[5]) enable = 1 , disable = 0*/
   intrst= 0x17,
   ctl1= 0x18,ctl2= 0x19,
   ldth= 0x1A, pdth= 0x1B,
   pw= 0x1C, lt= 0x1D, tw= 0x1E,
   reserved2= 0x1F
};
/*configuracion de los ejes de lectura*/
union Ejes{
   char x;
   char y;
   char z;
} eje;

void leerGravedad();

/*TODO/////////////////////
void leer(Ejes a);
/*
void write_data(void)
PURPOSE: Reads data from SPI and writes it to memory
PARAMS: none
RETURNS: None
*/
void write_data(int8 address){
   while(!spi_data_is_in());
   
   data = spi_read();
   if(instr >= 0x00 && instr <0x20){
      memory[address] = data;
   }
}



void main()
{
   setup_adc_ports(AN0_AN1_AN3);
   setup_adc(ADC_CLOCK_DIV_32);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SLAVE|SPI_L_TO_H|SPI_CLK_DIV_16);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   // TODO: USER CODE!!
   
   while(true)
   {  
      if(spi_data_is_in()){
         //while(!spi_data_is_in()){;};
         instr = spi_read(instr);
         /*if( bit_test(instr, 7)){
            //lectura desde el maestro
            bit_clear(instr, 7);
            if(instr >= 0x00 && instr <0x20){
               data = memory[ instr ];
               spi_write(data);
            }else{
               spi_write(-1);
            }
         }else{
            //escritura desde el maestro
            bit_clear(instr, 7);
            write_data(instr);
         }*/
         printf("ACK: %d\n\r", instr);
      }else{
         //leerGravedad();
         printf("NACK\n");
      }
   }
}
void leerGravedad(){
   int16 value = 0;
   int8 byte_high, byte_low;
   
   //leer canal 0
   set_adc_channel(0);
   delay_us(30);
   read_adc( ADC_START_ONLY);
   delay_us(20);
   value = read_adc( ADC_READ_ONLY);
   byte_high = make8(value, 1);
   byte_low = make8(value, 0);
   //coloca en el registro x
   memory[xoutl] = byte_low;
   memory[xouth] = byte_high;
   printf("X: %Lu ", value);
   
   //leer canal 1
   set_adc_channel(1);
   delay_us(30);
   read_adc( ADC_START_ONLY);
   delay_us(20);
   value = read_adc( ADC_READ_ONLY);
   byte_high = make8(value, 1);
   byte_low = make8(value, 0);
   //coloca en el registro y
   memory[ youtl ] = byte_low;
   memory[ youth ] = byte_high;
   printf("Y: %Lu ", value);
   
   //leer canal 3
   set_adc_channel(3);
   delay_us(30);
   read_adc( ADC_START_ONLY);
   delay_us(20);
   value = read_adc( ADC_READ_ONLY);
   byte_high = make8(value, 1);
   byte_low = make8(value, 0);
   //coloca en el registro z
   memory[ zoutl ] = byte_low;
   memory[ zouth ] = byte_high;
   printf("Z: %Lu\n\r", value);
   //printf("\n\rxoutl: %u xouth: %u\n\ryoutl: %u youth: %u\n\rzoutl: %u zouth: %u",memory[xoutl],memory[xouth],memory[youtl],memory[youth],memory[zoutl],memory[zouth]);
}
/*
void leer(Ejes eje){
   switch (eje){
      case 'x':
      //leer el eje x
         break;
      case 'y':
      //leer el eje y
         break;
      case 'z':
      //leer el eje z
         break;
   }
}*/
