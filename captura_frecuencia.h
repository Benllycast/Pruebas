#ifndef CAPTURA_FRECUENCIA_H
#define CAPTURA_FRECUENCIA_H
//canales de lectura
#define CCP_CANAL_1 1
#define CCP_CANAL_2 2

int CP_init_ccp();
int CP_leer_ccp(int canal, int32 *buffer);
void CP_activar_captura(int canal);
void CP_desativar_captura();
int CP_ocupado();
int32 CP_obtener_resultado();

#endif
