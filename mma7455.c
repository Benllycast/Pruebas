/**
codigo de prueba del accelerometro mma7455l 
*/
#include "mma7455.h"
#include <stdio.h>
#include <stdlib.h>
#define testmma  
#include "address.h" /*definicion de registros de mma7455*/
#define _num_reg 0x20
unsigned int8 memory[_num_reg];
/*
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
                data output pin (bit[5]) enable = 1 , disable = 0-----
   intrst= 0x17,
   ctl1= 0x18,ctl2= 0x19,
   ldth= 0x1A, pdth= 0x1B,
   pw= 0x1C, lt= 0x1D, tw= 0x1E,
   reserved2= 0x1F
};
*/

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
void write_data(unsigned int8 address){
   int8 data = 0;
   bit_clear(address, 7);
   address>>=1;
   while(!spi_data_is_in());   
   if(spi_data_is_in())
      data = spi_read();   
   if(address >= 0x00 && address <0x20){
      memory[address] = data;
   }
}

void read_data(unsigned int8 &address){
   bit_clear(address, 7);
   address>>=1;
   if(address >= 0x00 && address <0x20){
      printf("sl rp: %X \n\r",memory[ address ]);
      spi_write(memory[ address ]);
   }else{
      spi_write(-1);
   }
   return;
}

void main()
{
   unsigned int8 address = 0x00;
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
   #ifdef testmma
      memory[0x00] = xouth;   memory[0x01] = xoutl;
      memory[0x02] = youth;   memory[0x03] = youtl;
      memory[0x04] = zouth;   memory[0x05] = zoutl;
      
      memory[0x06] = xout8;
      memory[0x07] = yout8;
      memory[0x08] = zout8;

      memory[0x09] = status;
      memory[0x0A] = detsrc;
      memory[0x0B] = tout;
      memory[0x0C] = reserved;
      memory[0x0D] = i2cad;
      memory[0x0E] = usrinf;
      memory[0x0F] = whoami;

      memory[0x10] = xoffl;   memory[0x11] = xoffh;
      memory[0x12] = yoffl;   memory[0x13] = yoffh;
      memory[0x14] = zoffl;   memory[0x15] = zoffh;
      
      memory[0x16] = mctl;
      memory[0x17] = intrst;
      memory[0x18] = ctl1;   memory[0x19] = ctl2;
      memory[0x1A] = ldth;   memory[0x1B] = pdth;
      memory[0x1C] = pw;   memory[0x1D] = lt;   memory[0x1E] = tw;
      memory[0X1F] = reserved2;
  #endif
   
   while(true)
   {
      while(!spi_data_is_in()){;}  
      if(spi_data_is_in()){
         address = spi_read();
         if(!bit_test(address, 7)){
            //lectura desde el maestro
            //filterIN(address);
            printf("%s: %X ,", "Rd", address);
            read_data(address);
         }else{
            //escritura desde el maestro
            //printf("%s: %X \n\r", "Wd", address);
            spi_write(address);
         }
         //printf("ACK: %d\n\r", address);
      }
      // else{
      //    //leerGravedad();
      //    printf("NACK\n\r");
      // }
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
   //printf("X: %Lu ", value);
   
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
   //printf("Y: %Lu ", value);
   
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
   //printf("Z: %Lu\n\r", value);
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
