#ifndef MEMORIA_H
#define MEMORIA_H

#define MAX_BUFFER 		100

#define INI_HW	0
#define INI_SW	1
#define OPEN	2
#define RD		3
#define GET		4
#define WR		5
#define SET		6
#define CLOSE	7

#define MEMORIA_CMD_AUTOBAUD	0x55
#define MEMORIA_CMD_VER_INFO	0x56
#define MEMORIA_CMD_FAT_PROTECT	0x59
#define MEMORIA_CMD_INITIALIZE	0x69

#define MEMORIA_EXT_CMD	0x40
#define MEMORIA_CMD_READ_FILE	0x61
#define MEMORIA_CMD_WRITE_FILE	0x74

#define MEMORIA_FAT_VALUE	0x08

#define MEMORIA_ACK 		0x06
#define MEMORIA_NOACK		0x15

#define MEMORIA_ON 			0x01
#define MEMORIA_OFF 		0x00

#define MEMORIA_NAME_LENG_LIMIT	12
#define MEMORIA_NAME_TERMINATOR 0x00
#define MEMORIA_DELIMITADOR 	0x0a

#define MEMORIA_NO_APPEND 	0X00
#define MEMORIA_APPEND 		0X80

#define MEMORIA_NO_HANDSHAKING 		0x00
#define MEMORIA_DEFAULT_HANDSHAKING 0X01
#define MEMORIA_MAX_HANDSHAKING		0x32

#define MEMORIA_HIGH_PERFORMANCE 	0x00
#define MEMORIA_LOW_PERFORMANCE 	0x40

#define FILE_WR 1
#define FILE_RD 0

//extern char MEM_info[5];
int MEMORIA_reset(void);
int MEMORIA_init_hw(void);
void MEMORIA_getinfo(void);
int MEMORIA_init(void);
int MEMORIA_open(char* filename, short modo);
int MEMORIA_cancel(void);
int MEMORIA_write(unsigned int size);
int MEMORIA_set_data(char* data, unsigned int size);
void MEMORIA_putc(char c);
unsigned int32 MEMORIA_read(unsigned int num_bytes);
int MEMORIA_get_data(char* buffer);
int MEMORIA_close(void);
int MEMORIA_is_busy(void);
char MEMORIA_getc(void);
#endif
