#ifndef CAPTURA_FRECUENCIA
#define CAPTURA_FRECUENCIA
//canales de lectura
#define CANAL_1 1
#define CANAL_2 2

//tipos de capturas de los modulos CCP
int MODO_CCP1 = CCP_CAPTURE_RE;
int MODO_CCP2 = CCP_CAPTURE_RE;
int Q_CCP = 0;		//estados de la captura de la frecuencia
/*
	Q=0 :	capturando el tiempo de flanco de subida
	Q=1 :	capturando el tiempo de flanco de bajada
	Q=2 :	relizando el calculo de ancho de pulso o periodo
*/
int MODO_TIMER_CCP = T3_INTERNAL|T3_DIV_BY_1|T3_CCP1_TO_2;

int overflow_t3_counter = 0;
int32 tiempo_inicial = 0, tiempo_final = 0;

int CP_init_ccp();
int CP_leer_ccp(int canal, int32 *buffer);
#endif