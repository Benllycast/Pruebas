//#include "Nucleo.h"
#include "comunicacion.h"

unsigned int indice = 0;
int COM_init(){
	usb_detach(); 
	usb_cdc_init();
   usb_init_cs();
   /*usb_task();
   if(usb_attached()){
      if(usb_enumerated()){
         return (USB_OK);
      }else{
         return(USB_NO_ENUMERATED);
      }
   }
   return (USB_NO_ATTACHED);*/
   return COM_sense();
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

//de aqui en adelante hay que quitar estas funciones que no se utilizan
int8 COM_send(char* buffer, unsigned int8 leng, unsigned short wait = 1){
   indice = 0;
   if(!COM_READY) return(-1);
   while(indice < leng){
      if(wait == 1)
         usb_cdc_putc(buffer[indice]);
      else
         usb_cdc_putc_fast(buffer[indice]);
      indice++;
   }
   return (indice);
}

int8 COM_recive(char* buffer, unsigned int8 leng){
   indice = 0;
   if(!COM_READY) return (-1);
   while(indice < leng){
      if(usb_cdc_kbhit()){
         buffer[indice] = usb_cdc_getc();
         indice++;
      }
   }
   return(indice);
}

void COM_printf(char* message){
	printf(usb_cdc_putc,"%s\n\r",message);
}
