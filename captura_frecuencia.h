#ifndef CAPTURA_FRECUENCIA
#define CAPTURA_FRECUENCIA
//canales de lectura
#define CANAL_1 1
#define CANAL_2 2

int CP_init_ccp();
int CP_leer_ccp(int canal, int32 *buffer);
#endif