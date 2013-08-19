
#include "captura_frecuencia.h"
#ifndef REGISTROS_H
   #include "registros.h"
#endif

#INT_TIMER3
void timer3_isr(void){
   ++overflow_t3_counter;
}

#INT_CCP1
void ccp1_isr(void){
   if(Q_CCP == -1){
   	setup_ccp1(CCP_CAPTURE_FE);
   	Q_CCP = 0;
   }else if(Q_CCP == 0){
   	tiempo_final = (65536*overflow_t3_counter)+CCP_1;
   	Q_CCP = 1;
   }else if(Q_CCP == 1){
   	disable_interrupts(INT_CCP1);
   	tiempo_inicial = tiempo_final;
   	tiempo_final = (65536*overflow_t3_counter)+CCP_1;
   	Q_CCP = 2;
   }
}

#INT_CCP2
void ccp2_isr(void){
   if(Q_CCP == -1){
   	setup_ccp1(CCP_CAPTURE_FE);
   	Q_CCP = 0;
   }else if(Q_CCP == 0){
   	tiempo_final = (65536*overflow_t3_counter)+CCP_2;
   	Q_CCP = 1;
   }else if(Q_CCP == 1){
   	disable_interrupts(INT_CCP2);
   	tiempo_inicial = tiempo_final;
   	tiempo_final = (65536*overflow_t3_counter)+CCP_2;
   	Q_CCP = 2;
   }
}

int CP_init_ccp(){
   //configurar el timer1
   setup_timer_3(MODO_TIMER_CCP);
   T3CON.TMR3ON = 0;
   set_timer3(0);
   setup_ccp1(MODO_CCP1);
   setup_ccp2(MODO_CCP2);
   TRISC.TRISC1 = TRISC.TRISC2 = 1;                       
   return 0;
}

void CP_activar_captura(int canal){
	//CODIGO DE MANEJO DE CCP
	semaforo_ccp = 1;
   enable_interrupts(GLOBAL);      	//habilita las interrupciones globales
   enable_interrupts(INT_TIMER3);
   tiempo_inicial = tiempo_final = 0;
   set_timer3(0);      						//se reset timer  a 0
   T3CON.TMR3ON = 1;
   (canal == CCP_CANAL_1)? enable_interrupts(INT_CCP1) : enable_interrupts(INT_CCP2);
}

void CP_desativar_captura(){
	//disable_interrupts(INT_CCP1);
   //disable_interrupts(INT_CCP2);
   disable_interrupts(GLOBAL);
   setup_ccp2(MODO_CCP1);
   setup_ccp2(MODO_CCP2);
   Q_CCP = -1;                     //regreso al estado inicial para la proxima lectura
   T3CON.TMR3ON = 0;               //se desactiva del TIMER3 para no generar interrupciones
   overflow_t3_counter = 0;
   semaforo_ccp = 0;
}

int CP_ocupado(){ return (semaforo_ccp != 0);}

int32 CP_obtener_resultado(){
	return (tiempo_final - tiempo_inicial);
}

int CP_leer_ccp(int canal, int32 *buffer){
   //CODIGO DE MANEJO DE CCP
   enable_interrupts(GLOBAL);      	//habilita las interrupciones globales
   enable_interrupts(INT_TIMER3);
   set_timer3(0);      					//se reset timer  a 0
   T3CON.TMR3ON = 1;
   (canal == CCP_CANAL_1)? enable_interrupts(INT_CCP1) : enable_interrupts(INT_CCP2);
   //while(Q_CCP != 2){;}              //espera a que se carguen los valores de los tiempos
   disable_interrupts(INT_CCP1);
   disable_interrupts(INT_CCP2);
   setup_ccp2(MODO_CCP1);
   setup_ccp2(MODO_CCP2);
   Q_CCP = -1;                     //regreso al estado inicial para la proxima lectura
   T3CON.TMR3ON = 0;               //se desactiva del TIMER3 para no generar interrupciones
   overflow_t3_counter = 0;
   //resultado = tiempo_final - tiempo_inicial;      //se calcula el periodo del pulso
   *buffer = tiempo_final - tiempo_inicial;
   return (0);
}
