#include "Nucleo.h"
#include "comunicacion.h"
#define USB_CON_SENSE_PIN PIN_B2
#include "usb_desc_cdc.h"
#include <usb_cdc.h>


unsigned int i = 0;
int COM_init(){
	usb_cdc_init();
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
   if(!COM_READY) return(-1);
   while(i < leng){
      if(wait == 1)
         usb_cdc_putc(buffer[i]);
      else
         usb_cdc_putc_fast(buffer[i]);
      i++;
   }
   return (i);
}

int8 COM_recive(char* buffer, unsigned int8 leng){
   i = 0;
   if(!COM_READY) return (-1);
   while(i < leng){
      if(usb_cdc_kbhit()){
         buffer[i] = usb_cdc_getc();
         i++;
      }
   }
   return(i);
}

void COM_printf(char* message){
	printf(usb_cdc_putc,"%s\n\r",message);
}
