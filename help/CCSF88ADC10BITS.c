//ESTE PROGRAMA USA EL COMPILADOR CCS. LLEVA LAS DIRECTIVAS
//NECESARIAS PARA FUNCIONAR CON EL SOFTWARE "TINY"
//PUEDE CARGARSE EN LA MEMORIA FLASH DEL 16F88 A TRAVES 
//DEL PUERTO SERIAL CON ESE SOFTWARE. EL 16F88 DEBIO PREVIAMENTE 
//HABER SIDO CARGADO CON EL FIRMWARE BOOTLOADER TINY CON UN 
//PROGRAMADOR ICSP.EL TINY REQUIERE ESTE ENCABEZADO, ANTES
//DE LA FUNCION PRINCIPAL main
//#rom 0X0000={0X018A,0X2803}
//#built(reset=0x0003)

//PROGRAMA LEE EL CONVERTIDOR ANALOGICO DIGITAL CANAL 3, DE UNA 
//INTERFAZ CON UN POTENCIOMETRO DE 10K Y 25 VUELTAS. 
//CONECTADO DE 0 A 5 VOLTS.ESCRIBE EN 10 BITS,
//EL VALOR DE LOS REGISTROS ADRESL Y ADRESH EN EL PUERTO B Y
//DESPUES ESCRIBE EL VALOR BINARIO DE 10 BITS CONVERTIDO A
//DECIMAL ENTRE 0 Y 1023 EN EL LCD.EL CICLO DE LECTURA ES DE 2 SEG.
//LA RESOLUCION DEL CONVERTIDOR ES MENOR A 1 BIT, CON LECTURAS
//ESTABLES DURANTE TODO EL INTERVALO. 

#if defined(__PCM__)
#include "16F88ADC.h"
#include "key4.h"

#rom 0X0000={0X018A,0X2803}	//DATOS REQUERIDOS POR TINY					
#build(reset=0x0003)

main(void){
unsigned long int a;
char rim[5];
TRISA=0XF8;
TRISB=0;
ANSEL=8;
ADCON1=0;
ADFM=1;
CHS0=1;
CHS1=1;
CHS2=0;
ADCS0=0;
ADCS1=0;
ADON=1;
a=0;		//DUMMY RETRASO 4 MICROSEGS.
CICLO:
GO=1;
ESPERA:
IF(GO_DONE==1)GOTO ESPERA;
a=ADRESL+(256*ADRESH);
PORTB=ADRESL;
delay_ms(2000);
PORTB=ADRESH;
delay_ms(2000);
initLCD();
clearLCD();
sprintf(rim,"%lu",a);
stringLCD(rim);
delay_ms(2000);
GOTO CICLO;
} 