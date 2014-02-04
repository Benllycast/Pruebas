#ifndef CONFIGURACION_H
#define CONFIGURACION_H

// opciones de configuracion
#define CONF_SALIR	'9'
#define CONF_FECHA	'1'
#define CONF_DATO		'2'
#define CONF_CANAL	'3'

#define CONF_ACK		0x06
#define CONF_NOACK	0x15

//direccion del byte de canales activos y bit de canales
#define CONF_DIR_CANAl		0x08
#define CONF_CCP_VEL_BIT	0
#define CONF_CCP_REV_BIT	1
#define CONF_CCP_BIT			2
#define CONF_AD_VEL_BIT		3
#define CONF_AD_REV_BIT		4

// numeracion de sensores
#define ACC_x		0
#define ACC_y		1
#define ACC_z		2
#define AD_VEL 	3
#define AD_REV		4
#define CCP_REV 	5
#define CCP_VEl 	6

// para el switch de log
#define LOG_ENABLE	1
#define LOG_DISABLE	0
#define LOG_LINE ("\n\r%X:%X:%X:%X:%X:%X:%X:%X:%LX:%LX")

// definicion de pines de indicadores
#define INDICADOR_AMARILLO	PIN_E0
#define INDICADOR_USB		PIN_E1
#define INDICADOR_RUN		PIN_E2

// esto es para debug
#ifdef SIMULACION
	#define cout	
	#define cin		getc              
#else
	#define cout	usb_cdc_putc_fast,
	#define cin		usb_cdc_getc
	#define putc	usb_cdc_putc
#endif

// definciones utilizadas en el nucleo
int1 salida = 0;
byte CONF_CANAL_ACTIVO = 0;

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
