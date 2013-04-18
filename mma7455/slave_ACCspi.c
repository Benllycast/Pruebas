#include "slave_ACCspi.h"
#define DATA_IN   PIN_D4
#define DATA_OUT   PIN_D5
#define DATA_SCL   PIN_D6
#define DATA_SS   PIN_D7
#define SPI_MODE_0  (SPI_L_TO_H | SPI_XMIT_L_TO_H) 
#define SPI_MODE_1  (SPI_L_TO_H) 
#define SPI_MODE_2  (SPI_H_TO_L) 
#define SPI_MODE_3  (SPI_H_TO_L | SPI_XMIT_L_TO_H) 
//#use spi(SLAVE, STREAM=PUERTO, MODE=0, DI=DATA_IN, DO=DATA_OUT, CLK=DATA_SCL, ENABLE=DATA_SS, BITS=8, BAUD=9600)
char memoria_mma[32];
void main()
{
	int8 addres = 0, data= 0, in = 0, w = 0, i = 0;
   setup_adc_ports(AN0_AN1_AN3);
   setup_adc(ADC_CLOCK_DIV_2);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SLAVE|SPI_MODE_3);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_1);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   setup_ccp1(CCP_OFF);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
	setup_uart(9600);
   // TODO: USER CODE!!
   //memset(memoria_mma, 0, sizeof(memoria_mma));
   for(i = 0; i < sizeof(memoria_mma);++i)memoria_mma[i] = i;
   do{
   	if(spi_data_is_in()){
   		in = spi_read();
         addres = ((in >> 1) & 0x3F);
         if((in & 0x80)){
            while(!spi_data_is_in());
            data = spi_read();
            memoria_mma[addres] = data;
            printf("\n\rW: ad: %x dt: %x",addres,data);
         }else{
            spi_write(memoria_mma[addres]);
            printf("\n\rR: ad: %x dt: %x",addres,memoria_mma[addres]);
            while(!spi_data_is_in());
         }
   		//printf("\n\rrc: %x tr:%x",r,p);
   	}
   }while(1);
}
