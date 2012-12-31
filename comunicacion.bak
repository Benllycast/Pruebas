#include "Nucleo.h"
#include "comunicacion.h"
#define USB_CON_SENSE_PIN PIN_B2
#include "usb_desc_cdc.h"
#include <usb_cdc.h>


unsigned int i = 0;
int8 com_usb_state = -1;
int COM_init(){
	usb_init_cs();
	usb_task();
	if(usb_attached()){
		if(usb_enumerated()){
			return (USB_OK);
		}else{
			return(USB_NO_ENUMERATED);
		}
	}
	return (USB_NO_ATTACHED);
}

int8 COM_sense(){
	usb_task();
	if(usb_attached()){
		if(usb_enumerated()){
			return (USB_OK);
		}else{
			return(USB_NO_ENUMERATED);
		}
	}	
	return(USB_NO_ATTACHED);
}

int8 COM_send(char* buffer, unsigned int8 leng, unsigned short wait = 1){
	i = 0;
	com_usb_state = COM_sense();
	if(com_usb_state == USB_OK){
		while(i < leng){
			if(wait == 1)
				usb_cdc_putc(buffer[i]);
			else
				usb_cdc_putc_fast(buffer[i]);
			i++;
		}
		return (i);
	}else if(com_usb_state == USB_NO_ENUMERATED){
		return (USB_NO_ENUMERATED);
	}
	return(-1);
}

int8 COM_recive(char* buffer, unsigned int8 leng){
	i = 0;
	com_usb_state = COM_sense();
	if(com_usb_state == USB_OK){
		while(i < leng){
			if(usb_cdc_kbhit()){
				buffer[i] = usb_cdc_getc();    
				i++;
			}
		}
		return(i);
	}else if(com_usb_state == USB_NO_ENUMERATED){
		return (USB_NO_ENUMERATED);
	}
	return(-1);
}
