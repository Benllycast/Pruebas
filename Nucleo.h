/*
configuracione de hardware
oscilador: 20Mhz. con PLL:activo para dividir por 5 y obtener 4Mhz, aumentarlo a 96Mhz y dividirlo a 48Mhz para la frecuencia del modulo USB;
frecuencia de la CPU:24 Mhz; el FUSE CPIDIV3 para dividir los 96Mhz hasta 24MHz como frecuencia de entradad de la CPU 


*/

#ifndef NUCLEO_H
#define NUCLEO_H
#include <18F4550.h>
#device adc=10

/*
#include <18F4620.h>
//#include "registros.h"
#device adc=10
*/

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES WDT128                	//Watch Dog Timer uses 1:128 Postscale
#FUSES HSPLL                  //High speed Osc (> 4mhz for PCM/PCH) (>10mhz for PCD) with PLL enable
#FUSES NOPROTECT             	//Code not protected from reading
#FUSES NOBROWNOUT            	//No brownout reset
#FUSES BORV20                	//Brownout reset at 2.1V
#FUSES NOPUT                 	//No Power Up Timer
#FUSES NOCPD                 	//No EE protection
#FUSES STVREN                	//Stack full/underflow will cause reset

#FUSES NODEBUG               	//No Debug mode for ICD
#FUSES NOLVP                 	//No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NOWRT                 	//Program memory not write protected
#FUSES NOWRTD                	//Data EEPROM not write protected
#FUSES IESO                  	//Internal External Switch Over mode enabled
#FUSES FCMEN                 	//Fail-safe clock monitor enabled
#FUSES PBADEN                	//PORTB pins are configured as analog input channels on RESET
#FUSES NOWRTC                	//configuration not registers write protected
#FUSES NOWRTB                	//Boot block not write protected
#FUSES NOEBTR                	//Memory not protected from table reads

#FUSES NOEBTRB               	//Boot block not protected from table reads
#FUSES NOCPB                 	//No Boot Block code protection
#FUSES MCLR                  	//Master Clear pin enabled
#FUSES LPT1OSC               	//Timer1 configured for low-power operation
#FUSES NOXINST               	//Extended set extension and Indexed Addressing mode disabled (Legacy mode)
#FUSES PLL5							//PLL enable div by 5 input Osc
#FUSES CPUDIV3						//postscaler PLL div by 4                      
#FUSES USBDIV						//enable USBDIV, div output PLL by 2
#FUSES VREGEN						//internal regulator USB enable
#FUSES ICPRT						
#FUSES CCP2C1						//CPP input/output multiplexed whit RC1

//cambiar el valor del clock si se cambia la frecuencia de la CPU
#use delay(clock=24000000)
#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
#use i2c(master, sda=PIN_B0, scl=PIN_B1, FORCE_HW, RESTART_WDT)                    //directiva de compilador para el uso del bus I2C del microcontrolador
#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
#endif
