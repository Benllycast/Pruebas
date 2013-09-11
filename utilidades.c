//#include "Nucleo.h"
#include "utilidades.h"

/*
#define MYDIM 64;
char MyArray[MYDIM];
*/

int16 crc_1021(int16 old_crc, int8 data)
{ 
  int16 crc;
  int16 x;
  x = make8(old_crc,1) ^ data;  //x = ((old_crc>>8) ^ data) & 0xff; 
  x ^= x>>4;
  crc = (old_crc << 8) ^ (x << 12) ^ (x <<5) ^ x; 
  crc &= 0xffff; 
  return crc; 
}

int16 calc_CRC(char *buffer, unsigned int8 leng){
	unsigned int8 i; 
   int16 MyCRC;
   
   MyCRC = 0xFFFF; 
   for(i=0 ; i<leng ; i++) { 
      MyCRC = crc_1021(MyCRC,buffer[i]); 
   }
   return (MyCRC);
}

#ifndef SIMULACION
int1 _debug_usb(void){
	if(COM_sense() == USB_OK){
      output_bit(INDICADOR_USB, 1);
      return (1);
	}else{
		output_bit(INDICADOR_USB, 0);
		return (0);
	}
}

#endif

