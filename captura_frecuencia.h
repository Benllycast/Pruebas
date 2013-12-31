#ifndef CAPTURA_FRECUENCIA_H
#define CAPTURA_FRECUENCIA_H
#ifndef REGISTROS_H
   #include "registros.h"
#endif
//canales de lectura
#define CCP_CANAL_1 1
#define CCP_CANAL_2 2

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
	Q=-1:   ccp inabilitado
   Q=0 :   capturando el tiempo de flanco de subida
   Q=1 :   capturando el tiempo de flanco de bajada
   Q=2 :   relizando el calculo de ancho de pulso o periodo
*/
int Q_CCP = -1;      //estados de la captura de la frecuencia
unsigned int overflow_t3_counter = 0;
unsigned int32 tiempo_inicial = 0, tiempo_final = 0;
int semaforo_ccp = 0;

int CP_init_ccp();
int CP_leer_ccp(int canal, int32 *buffer);
void CP_activar_captura(int canal);
void CP_desativar_captura();
int CP_ocupado();
int CP_done();
int32 CP_obtener_resultado();

#endif
