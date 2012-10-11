#ifndef COMUNICACION_H
#define COMUNICACION_H
#define USB_OK	0
#define USB_NO_ENUMERATED 1
#define USB_NO_ATTACHED -1

int8 COM_init();
int8 COM_sense();
int8 COM_send(char* buffer, unsigned int8 leng, unsigned short wait = 1);
int8 COM_send(char* buffer, unsigned int8 leng);
int8 COM_recive(char* buffer, unsigned int8 leng);
#endif
