/*=========================Configuracion del Hardware==========================

oscilador: 20Mhz. con PLL:activo para dividir por 5 y obtener 4Mhz, aumentandolo a 96Mhz-> div:2 a 48Mhz para la frecuencia del modulo USB;
frecuencia de la CPU:24 Mhz; el FUSE CPIDIV3 para dividir los 96Mhz hasta 24MHz como frecuencia de entradad de la CPU

==============================================================================*/

/*=================== FILE HEADER ===================================*/
#ifndef NUCLEO_H
#define NUCLEO_H
#include <18F4550.h>
//#include "PIC18F4550.h"
#device adc=10
//#define SIMULACION 1	// comentar esto si se prueba en forma real

/*====================fuses de configuracion del dispositivo==================*/
#fuses HSPLL,NOWDT,NOPROTECT,NOLVP,NODEBUG,USBDIV,PLL5,CPUDIV4,VREGEN, NOPBADEN
/*
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
#FUSES NOLVP                 	//No low voltage programing, B3(PIC16) or B5(PIC18) used for I/O
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
#FUSES PLL5							//(PLL prescaler) PLL enable div by 5 input Osc
#FUSES CPUDIV3						//postscaler PLL div by 4 (whit pll enable)                     
#FUSES USBDIV						//enable USBDIV, USB clock source come from PLL divide by 2
#FUSES VREGEN						//internal regulator USB enable
#FUSES ICPRT						
#FUSES CCP2C1						//CPP input/output multiplexed whit RC1


/*============================ DEFINICIONES DE PINES =========================*/
#define PIN_SDA	PIN_B0
#define PIN_SCL	PIN_B1
//#define PIN_XMIT	PIN_C6
//#define PIN_RCV	PIN_C7

#define PIN_XMIT	PIN_D0
#define PIN_RCV	PIN_D1

#define SPI_SS		PIN_D7
#define SPI_MISO	PIN_D6
#define SPI_MOSI	PIN_D5
#define SPI_SCL	PIN_D4


/*=================== CONFIGURACION DEL RELOJ DE TRABAJO =====================*/
#use delay(clock=16000000)	//cambiar el valor del clock si se cambia la frecuencia de la CPU

/*=================== CONFIGURACION LIBRERIAS DE COMUNICACION ================*/
#use rs232(baud=9600,parity=N,xmit=PIN_XMIT,rcv=PIN_RCV,bits=8)
//directiva de compilador para el uso del bus I2C del microcontrolador
#use i2c(master, sda=PIN_B0, scl=PIN_B1)
//#use fixed_io(b_outputs=PIN_B0, PIN_B1)
//#use i2c(master, FORCE_HW)
/*=================== LIBRERIAS ESTANDAR PARA EL MANEJO DE DATOS =============*/

#endif	//ifndef NUCLEO_H
