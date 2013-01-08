#ifndef COMUNICACION_H
#define COMUNICACION_H
#define USB_OK	1
#define USB_NO_ENUMERATED 0
#define USB_NO_ATTACHED -1

int8 COM_init();
int8 COM_sense();
int8 COM_send(char* buffer, unsigned int8 leng, unsigned short wait = 1);
int8 COM_send(char* buffer, unsigned int8 leng);
int8 COM_recive(char* buffer, unsigned int8 leng);
void COM_printf(char* message);

#define COM_READY (COM_sense() == USB_OK)
#endif
