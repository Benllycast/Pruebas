#ifndef MEMORIA_H
#define MEMORIA_H

#define MEMORIA_BUFFER 32

#define MEMORIA_CMD_AUTOBAUD	0x55
#define MEMORIA_CMD_VER_INFO	0x56
#define MEMORIA_CMD_FAT_PROTECT	0x59
#define MEMORIA_CMD_INITIALIZE	0x69

#define MEMORIA_EXT_CMD		0x40
#define MEMORIA_CMD_READ_FILE	0x61
#define MEMORIA_CMD_WRITE_FILE	0x74
#define MEMORIA_CMD_ERASE_FILE	0X65
#define MEMORIA_CMD_LIST_DIRECTORY	0X64

#define MEMORIA_FAT_VALUE	0x08

#define MEMORIA_ACK 	0x06
#define MEMORIA_NOACK	0x15

#define MEMORIA_ON 		0x01
#define MEMORIA_OFF 	0x00

#define MEMORIA_NAME_LENG_LIMIT	12
#define MEMORIA_NAME_TERMINATOR 0x00;
#define MEMORIA_DELIMITADOR 	0x0a

#define MEMORIA_NO_APPEND 0X00
#define MEMORIA_APPEND 0X80

#define MEMORIA_NO_HANDSHAKING 0x00
#define MEMORIA_HANDSHAKING 0x01

#define MEMORIA_HIGH_PERFORMANCE 0x00
#define MEMORIA_LOW_PERFORMANCE 0x40
//#define MEMORIA_OUT_TIME	65535	//modificar para colocar tiempos mas largos
//#define MEMORIA_COMPROBAR_ARCHIVO

int MEMORIA_write(char *buffer, unsigned int size);
int MEMORIA_read(char *buffer, unsigned int size);
int MEMORIA_send(char data);
int MEMORIA_send(char *data, unsigned int size);
int MEMORIA_recive(char *data , unsigned int size);
int MEMORIA_setparam(unsigned int param, char *value, unsigned int size);
int MEMORIA_init();
void MEMORIA_reset();
int MEMORIA_select_file(char *file_name);
#endif