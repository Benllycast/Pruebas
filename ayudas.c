*********************************************** 
************** PIC_1**************************** 
*********************************************** 

#include <16f876.h> 
#ORG 0x1F00,0x1FFF {} //for the 8k 16F876/7 
#use delay(clock=10000000) 
#use rs232(baud=9600,xmit=PIN_C6,rcv=PIN_C7) 
#fuses HS,NOWDT,NOPROTECT 
#priority rda,ad,rtcc,timer1,timer2 

#byte PORTA = 0x05 
#byte PORTB = 0x06 
#byte PORTC = 0x07 
#byte T1CON = 0x10 
#bit T1CON_0 = T1CON.0 
#byte SSPBUF = 0x13 
#byte PIR1 = 0x0C 
#bit SSPIF = PIR1.3 

///////////////////////////////////////////////////////////////////////// 
///// VARIABLES //////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////// 

#bit led_rojo = PORTB.7 
#define led_verde PIN_B6 
int i = 0; 
static int dato[5]; 
int contador_dato = 0; 
short ack_spi = false; 
static int mensage[5]; 
int contador_mensage = 0; 

void main(void) { 


	SET_TRIS_A (0b101111); 
	SET_TRIS_B (0b00000100); 
	SET_TRIS_C (0b10010000); 

	setup_spi(spi_slave |spi_h_to_l | SPI_SS_DISABLED);//modo esclavo 
	SET_TRIS_C (0b10111000);//configuracion SDO,SDI,SCK como entradas para SPI a dos hilos 

	setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_8 ); 
	T1CON_0 = FALSE; 

	enable_INTERRUPTS(GLOBAL); 
	enable_INTERRUPTS(INT_TIMER1); 
	enable_INTERRUPTS(INT_SSP); 

	for(i =0;i<5;++i){ 
		output_high(led_verde); 
		led_rojo = true; 
		delay_ms(50); 
		output_low(led_verde); 
		led_rojo = false; 
		delay_ms(50); 
	} 

	INICIO: 

	while(true){ 
		if(led_rojo){
			delay_ms(70);led_rojo = false;
		}

		output_high(led_verde); // led indicador de transmision 
		disable_INTERRUPTS(INT_SSP); //deshabilito inte. para ke no entre por transmison 
		setup_spi(spi_master | spi_h_to_l | spi_clk_div_64 ); //cambio a modo master spi_write('8'); 
		spi_write(61); 
		sspif = false;//borro flag de interrup. producido por la transmision 
		sspbuf = 0; //borro registro ya ke sino cuando reciba por SPI el primer dato ke leera 
		//sera el ultimo ke transmitio.....¿? 
		setup_spi(spi_slave |spi_h_to_l | SPI_SS_DISABLED); //modo esclavo 
		SET_TRIS_C (0b10111000); //configuracion SDO,SDI,SCK como entradas para SPI a dos hilos 
		enable_INTERRUPTS(INT_SSP); 

		delay_ms(200); 
		output_low(led_verde); 
		delay_ms(1000); // pausa para recibir respuesta 

		if(ack_spi){ //si el ACK es correcto señalizo con led. 
			output_high(led_verde); 
			delay_ms(100); 
			output_low(led_verde); 
			ack_spi = false; 
		} 
	} 
} 

/////////////////////// INTERRUPCIONES ///////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////////// 

#int_ssp 
recepcion(){ 

	led_rojo = true; 
	if(spi_data_is_in())dato[contador_dato] = spi_read(); 
	else{contador_dato = 0;return;} 

	++contador_dato; 

	set_timer1(64000); 
	T1CON_0 = true; // activar TIMER1.
} 

#int_timer1 
procesar(){ 

	T1CON_0 = false; // desactivar TIMER1 
	set_timer1(64000); 

	if(dato[0]=='A') 
		if(dato[1]=='C') 
			if(dato[2]=='K')ack_spi = true; 



	printf("dato pic_1 = %c %c %c %u %u \n\r",dato[0],dato[1],dato[2],dato[3],dato[4]); 
	for(i=0;i<5;++i){
		dato = 0; 
		
	}
	contador_dato = 0; 
} 

*********************************************** 
************** PIC_2**************************** 
*********************************************** 

#include <16f877A.h> 
#ORG 0x1F00,0x1FFF {} //for the 8k 16F876/7 
#use delay(clock=10000000) 
#use rs232(baud=9600,xmit=PIN_C6,rcv=PIN_C7) 
#fuses HS,NOWDT,NOPROTECT 
#priority rda,ad,rtcc,timer1,timer2 



#byte PORTA = 0x05 
#byte PORTB = 0x06 
#byte PORTC = 0x07 
#byte T1CON = 0x10 
#bit T1CON_0 = T1CON.0 
#byte SSPBUF = 0x13 
#byte PIR1 = 0x0C 
#bit SSPIF = PIR1.3 

///////////////////////////////////////////////////////////////////////// 
///// VARIABLES //////////////////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////// 

#define led_blanco PIN_B6 
#bit led_rojo = PORTB.7 

int i = 0; 
static int dato[5]; 
int contador_dato = 0; 
short recepcion_spi = false; 

void main(void) { 

	SET_TRIS_A (0b111111); 
	SET_TRIS_B (0b00010100); 
	SET_TRIS_D (0b00000000); 

	setup_spi(spi_slave |spi_h_to_l | SPI_SS_DISABLED); 
	SET_TRIS_C (0b10111000); //configuracion SDO,SDI,SCK como entradas para SPI a dos hilos 

	setup_timer_1 ( T1_INTERNAL | T1_DIV_BY_8 ); 
	T1CON_0 = FALSE; 

	enable_INTERRUPTS(GLOBAL); 
	enable_INTERRUPTS(INT_TIMER1); 
	enable_INTERRUPTS(INT_SSP); 

	for(i =0;i<5;++i){ 
		output_high(led_blanco); 
		led_rojo = true; 
		delay_ms(50); 
		output_low(led_blanco); 
		led_rojo = false; 
		delay_ms(50); 
	} 

	INICIO: 

	while(true){ 
		if(led_rojo){
			delay_ms(70);led_rojo = false;
		} 
		if(recepcion_spi){ 
			output_high(led_blanco); 
			delay_ms(200); 
			output_low(led_blanco); 
			recepcion_spi = false; 

			disable_INTERRUPTS(INT_SSP); 
			setup_spi(spi_master | spi_h_to_l | spi_clk_div_64 ); // modo master 
			spi_write('A'); 
			spi_write('C'); 
			spi_write('K'); 
			sspif = false; //borro flag interrupcion 
			sspbuf = 0; 
			setup_spi(spi_slave |spi_h_to_l | SPI_SS_DISABLED); //modo eslcavo de nuevo 
			SET_TRIS_C (0b10111000); //configuracion SDO,SDI,SCK como entradas para SPI a dos hilos 
			enable_INTERRUPTS(INT_SSP); //habilito otra vez interrupcion 
		} 
	} 

} 
/////////////////////// INTERRUPCIONES ///////////////////////////////////////////// 
///////////////////////////////////////////////////////////////////////////////////// 

#int_ssp 
recepcion(){ 

	led_rojo = true; 

	if(spi_data_is_in())dato[contador_dato] = spi_read(); 
	else{contador_dato = 0;return;} 
	++contador_dato; 

	set_timer1(64000); 
	T1CON_0 = true; // activar TIMER1. 

} 

#int_timer1 
procesar(){ 

	T1CON_0 = false; // desactivar TIMER1 
	set_timer1(64000); 

	if(dato[0]=='8') 
	if(dato[1]==61)recepcion_spi = true; 

	printf("dato pic_2= %c %u %u %u %u\n\r",dato[0],dato[1],dato[2],dato[3],dato[4]); 
	printf("tiempo = %lu\n\r",tiempo); 
	for(i=0;i<5;++i)
		dato = 0; 
	
	contador_dato = 0; 

} 



Saludos!!