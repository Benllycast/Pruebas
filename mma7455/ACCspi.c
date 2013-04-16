#include "ACCspi.h"
#include "accelerometro.h"
#include <stdio.h>
#include <stdlib.h>
/*
#define SPI_MISO   PIN_D4
#define SPI_MOSI   PIN_D5
#define SPI_SCL   PIN_D6
#define SPI_SS   PIN_D7
*/
//#use spi(MASTER, MODE=0, DI=SPI_MISO, DO=SPI_MOSI, CLK=SPI_SCL, ENABLE=SPI_SS, BITS=8, BAUD=9600)
#use rs232(uart1, baud=9600)

void test1(){
	//test para comprobar la escritura
	unsigned int i, value = 1;
	for(i = 0; i < 32;++i){
		printf("escritura %d- ad: %x dt: %x\n\r",i,i, i);
		write_MMA(i, &i);
	}
	return;
}

void test2(){
	unsigned int i, value = 1;
	for(i = 0; i < 32;++i){
		read_MMA(i, &value);
		printf("lectura %d- ad: %x dt: %x\n\r",i,i, value);
		value = 0;
	}
	return;
}
void main()
{
   setup_adc_ports(AN0_TO_AN2|VSS_VDD);
   setup_adc(ADC_CLOCK_DIV_2);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_wdt(WDT_ON);
   setup_timer_0(RTCC_INTERNAL);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_timer_3(T3_DISABLED|T3_DIV_BY_1);
   setup_ccp1(CCP_OFF);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
//Setup_Oscillator parameter not selected from Intr Oscillator Config tab

   // TODO: USER CODE!!
   
	do{
		/*printf("=== Test Escritura ===\n\r");
		test1();*/
		printf("=== Test Lectura ===\n\r");
		test2();
		delay_ms(10000);
	}while(1);
}
