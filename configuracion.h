#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#define LOG_ENABLE	1
#define LOG_DISABLE	0
#define LOG_LINE ("\n\r%X:%X:%X:%X:%X:%X:%X:%X:%LX:%LX")
int1 salida = 0;

#ifdef CAPTURA_FRECUENCIA_H
	int canal_ccp = CCP_CANAL_1;
#endif	//CAPTURA_FRECUENCIA_H

int myerror = 0;
int16 lectura = 0;
char noLog[] = "\n\rno se puede guardar";
char testfile[] = "prueba";
char buffer_log[39];
//int32 resultado = 0;
struct Log {
	byte dia;
	byte mes;
	byte anio;
	byte hor;
	byte min;
	byte seg;
	int8 sensor;
	int8 no_data;
	int32 value;
	int16 crc;
} data;

void iniciar_perifericos();
int sensor_activo(int sensor);
void modo_configuracion();
#endif
