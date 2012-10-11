//https://www.ccsinfo.com/forum/viewtopic.php?t=35926
#include <18F24K20.h> 
#device adc=8 
#FUSES NOWDT, HS, NOPROTECT, NOBROWNOUT, NOPUT, NOCPD, STVREN 
#FUSES NODEBUG, NOLVP, NOWRT, NOWRTD, IESO, FCMEN, NOPBADEN, WRTC 
#FUSES NOWRTB, NOEBTR, NOEBTRB, NOCPB, LPT1OSC, MCLR 
#use delay(clock=8000000) 
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,errors) 

unsigned int16 FSR0; 
#locate FSR0 = 0x0FE9 

unsigned int8 POSTINC0; 
#locate POSTINC0 = 0x0FEE 

#define MYDIM 64 

int8 MyArray[MYDIM];  // Global Array 


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



//----------------------------------------------------------------------------- 
// Calculate the CRC16 value of a specified buffer. 
// 
// A very fast CRC-16 routine based on the CCITT polynome 0x1021. 
// This implementation is very small and fast. Using some specific features of 
// the polynome the resulting code is even faster than table driven algorithms. 
// 
// Original Code: Ashley Roll       www.digitalnemesis.com 
// Optimisations: Scott Dattalo     www.dattalo.com 
//----------------------------------------------------------------------------- 
int16 Calc_Crc16(int8 *Buffer, int16 Len) // Note: save 5 instructions by 
{                                         // changing 'int16 Len' to 'int8 Len'. 
  int8 Index; 
  union 
  { 
    int8  uByte[2]; 
    int16 uWord; 
  } CRC16; 

  CRC16.uWord = 0xFFFF;     // Initialise CRC to 0xFFFF 
  FSR0 = Buffer;            // Copy Buffer address to pointer register FSR0 

  do 
  { 
    #ASM 
    movf  POSTINC0,w        // load w with next databyte 
    xorwf CRC16.uByte[1],w  // (a^x):(b^y) 
    movwf Index             // 
    andlw 0xf0              // W = (a^x):0 
    swapf Index,f           // Index = (b^y):(a^x) 
    xorwf Index,f           // Index = (a^b^x^y):(a^x) = i2:i1 
  
                            // High byte 
    movf  Index,W 
    andlw 0xf0 
    xorwf CRC16.uByte[0],W 
    movwf CRC16.uByte[1] 

    rlcf  Index,W           // use rlf for PIC16 
    rlcf  Index,W           // use rlf for PIC16 
    xorwf CRC16.uByte[1],f 
    andlw 0xe0 
    xorwf CRC16.uByte[1],f 
  
    swapf Index,F 
    xorwf Index,W 
    movwf CRC16.uByte[0] 
    #ENDASM 
  } while (--Len); 
  
  return CRC16.uWord; 
} 



// **********  M A I N   P r o g . *********** 

void main() 
{ 
   int8 i; 
   int16 MyCRC; 

   for(i=0 ; i<MYDIM ; i++) { 
      MyArray[i] = i+1;   // fill the array 
   } 


//----calc CRC16 By C function----------------- 

   MyCRC = 0xFFFF; 
   for(i=0 ; i<MYDIM ; i++) { 
      MyCRC = crc_1021(MyCRC,MyArray[i]); 
   } 
   printf("C CRC16 value is %Lu\n\r", MyCRC);     // Send the result (C function) 


//----calc CRC16 by assembly function---------- 

   MyCRC = Calc_Crc16(&MyArray[0],MYDIM); 
   printf("ASM CRC16 value is %Lu\n\r", MyCRC);   // Send the result (assembly function) 

   while(1) ; 
}