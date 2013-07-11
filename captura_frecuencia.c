#include "Nucleo.h"
#include "captura_frecuencia.h"
#ifndef REGISTROS_H
   #include "registros.h"
#endif

/*se utiliza el TIMER3 como contador de tiempo del modulo CCP
   TIMER3 con reloj interno (Fosc/4)
   con divicion de frecuencia por 1
   TIMER3 para ambos modulos CCP1 y CCP2
*/
int MODO_TIMER_CCP = T3_INTERNAL|T3_DIV_BY_8|T3_CCP1_TO_2;

//tipos de capturas de los modulos CCP
int MODO_CCP1 = CCP_CAPTURE_RE;
int MODO_CCP2 = CCP_CAPTURE_RE;

/*
   Q=0 :   capturando el tiempo de flanco de subida
   Q=1 :   capturando el tiempo de flanco de bajada
   Q=2 :   relizando el calculo de ancho de pulso o periodo
*/
int Q_CCP = -1;      //estados de la captura de la frecuencia
unsigned int overflow_t3_counter = 0;
unsigned int32 tiempo_inicial = 0, tiempo_final = 0;

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
   TRISC.TRISC1 = TRISC.TRISC2 = 1  ;                       
   return 0;
}

int CP_leer_ccp(int canal, int32 *buffer){
   //CODIGO DE MANEJO DE CCP
   //int32 resultado = 0;
   enable_interrupts(GLOBAL);      	//habilita las interrupciones globales
   enable_interrupts(INT_TIMER3);
   set_timer3(0);      					//se reset timer  a 0
   T3CON.TMR3ON = 1;
   /*if(canal == CANAL_1){
      enable_interrupts(INT_CCP1);   //si es el canal 1 se habilita la interrupcion del modulo CCP1
   }else if(canal == CANAL_2){
      enable_interrupts(INT_CCP2);   //si es el canal 2 se habilita la interrupcion del modulo CCP2
   }else{return (1);}*/
   (canal == CCP_CANAL_1)? enable_interrupts(INT_CCP1) : enable_interrupts(INT_CCP2);
   while(Q_CCP != 2){;}             //espera a que se carguen los valores de los tiempos
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
