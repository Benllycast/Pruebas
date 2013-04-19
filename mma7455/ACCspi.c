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
extern CONFIG_MMA7455 CONFIG;
unsigned int i, 
				value = 1,
				buffer[]={0xaa,0xaa,0xaa,0xaa,0xaa};
void test1(){
	//test para comprobar la escritura
	
	for(i = 0; i < 32;++i){
		printf("escritura %d- ad: %x dt: %x\n\r",i,i, i);
		write_MMA(i, &i);
		//delay_ms(1000);
	}
	return;
}

void test2(){
	//test para probrar la lectura
	for(i = 0; i < 32;++i){
		read_MMA(i, &value);
		printf("lectura %d- ad: %x dt: %x\n\r",i,i, value);
		value = 0;
		//delay_ms(1000);
	}
	return;
}

void test3(){
	//test para probrar la escritura multibyte
	printf("\n\rescribiendo buffer");
	write_MMA(0, buffer,sizeof(buffer));
	memset(buffer,0,sizeof(buffer));
	return;
}

void test4(){
	//test para probrar la lectura multibyte
	printf("\n\rLeyendo buffer");
	read_MMA(0, buffer,sizeof(buffer));
	for(i = 0; i < sizeof(buffer); i++){
		if(buffer[i] == 0xaa)
			printf("\n\r%d OK",i);
		else 
			printf("\n\r%d ERROR",i);
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
		printf("\n\r====Test InitMMA=====");
		init_MMA();          
		delay_ms(10000);
	}while(1);
}
