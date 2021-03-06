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

extern void usb_cdc_putc_fast(char c);
extern char usb_cdc_getc(void);
extern void usb_cdc_putc(char c);
extern void usb_cdc_get_discard(void);

//input.c ported to use CDC:
extern float get_float_usb();
extern signed long get_long_usb();
extern signed int get_int_usb();
extern void get_string_usb(char* s, unsigned int max);
extern BYTE gethex_usb();
extern BYTE gethex1_usb();

#define COM_READY (COM_sense() == USB_OK)
#endif
