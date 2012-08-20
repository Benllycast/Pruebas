#include "Nucleo.h"
#include "captura_frecuencia.h"
#ifndef REGISTROS_H
	#include "registros.h"
#endif
//tipos de capturas de los modulos CCP
int MODO_CCP1 = CCP_CAPTURE_RE;
int MODO_CCP2 = CCP_CAPTURE_RE;

int Q_CCP = 0;		//estados de la captura de la frecuencia
/*
	Q=0 :	capturando el tiempo de flanco de subida
	Q=1 :	capturando el tiempo de flanco de bajada
	Q=2 :	relizando el calculo de ancho de pulso o periodo
*/

/*se utiliza el TIMER3 como contador de tiempo del modulo CCP
	TIMER3 con reloj interno (Fosc/4)
	con divicion de frecuencia por 1
	TIMER3 para ambos modulos CCP1 y CCP2
*/
int MODO_TIMER_CCP = T3_INTERNAL|T3_DIV_BY_1|T3_CCP1_TO_2;

unsigned int overflow_t3_counter = 0;
unsigned int32 tiempo_inicial = 0, tiempo_final = 0;

#INT_TIMER3
void timer3_isr(void){
	overflow_t3_counter++;
}

#INT_CCP1
void ccp1_isr(void){
	if(Q_CCP == 0){
		Q_CCP = 1;
		//tiempo_inicial = CCP_1;
		tiempo_inicial = (0x10000*overflow_t3_counter)+CCP_1;
	}else if(Q_CCP == 1){
		disable_interrupts(INT_CCP1);
		disable_interrupts(INT_TIMER3);
		Q_CCP = 2;
		//tiempo_final = CCP_1;
		tiempo_final = (0x10000*overflow_t3_counter)+CCP_1;
		
	}
}

#INT_CCP2
void ccp2_isr(void){
	if(Q_CCP == 0){
		Q_CCP = 1;
		tiempo_inicial = (0x10000*overflow_t3_counter)+(int32)CCP_2;
	}else if(Q_CCP == 1){
		tiempo_final = (0x10000*overflow_t3_counter)+(int32)CCP_2;
		disable_interrupts(INT_CCP2);
		Q_CCP = 2;
	}
}

int CP_init_ccp(){
	//configurar el timer1
	setup_timer_3(MODO_TIMER_CCP);
	T3CON.TMR3ON = 0;
	setup_ccp1(MODO_CCP1);
	set_tris_c(0b00000110)  ;                       
	return 0;
}

int CP_leer_ccp(int canal, int32 *buffer){
	//CODIGO DE MANEJO DE CCP
	int32 resultado = 0;
	set_timer3(0x0000);		//se resetea el timer  a 0
	T3CON.TMR3ON = 1;
	enable_interrupts(GLOBAL);		//habilita las interrupciones globales
	enable_interrupts(INT_TIMER3);

	if(canal == CANAL_1){
		enable_interrupts(INT_CCP1);	//si es el canal 1 se habilita la interrupcion del modulo CCP1
	}else if(canal == CANAL_2){
		enable_interrupts(INT_CCP2);	//si es el canal 2 se habilita la interrupcion del modulo CCP2
	}else{return (1);}
	while(Q_CCP != 2){;} 				//espera a que se carguen los valores de los tiempos
	resultado = tiempo_final - tiempo_inicial;		//se calcula el periodo del pulso
	Q_CCP = 0;							//regreso al estado inicial para la proxima lectura
	T3CON.TMR3ON = 0;					//se desactiva del TIMER3 para no generar interrupciones
	overflow_t3_counter = 0;
	*buffer = resultado;
	return (0);
}
