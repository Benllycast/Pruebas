#include "Nucleo.h"
#include "memoria.h"
#include "comunicacion.h"

#define debug_memoria 1

#define MEM_TX PIN_D6
#define MEM_RX PIN_D7
#use rs232(stream=MEMORIA,baud=9600,xmit=MEM_TX,rcv=MEM_RX ,bits=8,parity=N /*,DISABLE_INTS,FORCE_SW*/)
#use fixed_io(d_outputs=PIN_D6)
#define MEMORIA_PIN_RESET PIN_D5
#define time_delay 1000

short MEMORIA_OK = FALSE;
short MEMORIA_HW = FALSE;
short timeout_error = FALSE;
int8 MEM_proceso = INI_HW;
unsigned int i = 0;
unsigned int car = 0;
unsigned int32 tamano = 0;
char MEM_info[5] = {0x00,0x00,0x00,0x00,0x00};
char MEM_file_name[MEMORIA_NAME_LENG_LIMIT];
char MEM_handshaking = MEMORIA_DEFAULT_HANDSHAKING;
char MEM_append = MEMORIA_APPEND;
char MEM_performance = MEMORIA_HIGH_PERFORMANCE;
char MEM_RESPONSE = MEMORIA_NOACK;

#ifdef debug_memoria
void update_proceso(int8 proceso){
	MEM_proceso = proceso;
	printf(usb_cdc_putc,"\n\rmp> %d", MEM_proceso);
	return;
}
#endif

///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
int MEMORIA_reset(void){
	#ifdef debug_memoria
	update_proceso(INI_HW);
	#endif
	output_high(MEM_TX);  
   output_low(MEMORIA_PIN_RESET);
   delay_ms(5000);
   output_high(MEMORIA_PIN_RESET);
   delay_ms(5000);
   return (MEMORIA_init_hw());
}

///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
int MEMORIA_init_hw(void){

	fputc(MEMORIA_CMD_AUTOBAUD,MEMORIA);
   MEM_RESPONSE = MEMORIA_getc();
   if(MEM_RESPONSE != MEMORIA_ACK){
      return (1);
   }
   
   MEMORIA_HW = TRUE;
   MEMORIA_OK = FALSE;
   MEM_info[0] = 0x00;
   MEM_info[1] = 0x00;
   MEM_info[2] = 0x00;
   MEM_info[3] = 0x00;
   MEM_info[4] = 0x00;
   
   #ifdef debug_memoria
	update_proceso(INI_SW);
	#else
	MEM_proceso = INI_SW;
	#endif
	
   return (0);
}

///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
int MEMORIA_init(void){

   if(!MEMORIA_HW || (MEM_proceso != INI_SW)) return (-1);

	/*
	fputc(MEMORIA_CMD_VER_INFO, MEMORIA);
   MEM_info[0] = MEMORIA_getc();
   MEM_info[1] = MEMORIA_getc();
   MEM_info[2] = MEMORIA_getc();
   MEM_info[3] = MEMORIA_getc();
   MEM_info[4] = MEMORIA_getc();
   */
   
	fputc(MEMORIA_EXT_CMD,MEMORIA);
   fputc(MEMORIA_CMD_INITIALIZE,MEMORIA);
   MEM_RESPONSE = MEMORIA_getc();
   if(MEM_RESPONSE != MEMORIA_ACK){
      return (2);
   }else{
      MEMORIA_OK = TRUE;
   }
   
   #ifdef debug_memoria
	update_proceso(OPEN);
	#else
	MEM_proceso = OPEN;
	#endif
   return (0);
}

///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
int MEMORIA_open(char *filename, char modo){
   unsigned int ncar =0;

   if(!MEMORIA_OK || MEM_proceso != OPEN){
      return(-1);
   }

   if(modo == 'r')
	   #ifdef debug_memoria
		update_proceso(RD);
		#else
		MEM_proceso = RD;
		#endif
   else if(modo == 'w')
      #ifdef debug_memoria
		update_proceso(WR);
		#else
		MEM_proceso = WR;
		#endif
   else
      return(1);
   
   ncar = strlen(filename);
   car = (ncar <= MEMORIA_NAME_LENG_LIMIT)? ncar : MEMORIA_NAME_LENG_LIMIT;
   strncpy(MEM_file_name, filename, car);
   i = 0;
   timeout_error = FALSE;
   MEM_handshaking = MEMORIA_DEFAULT_HANDSHAKING;
   MEM_RESPONSE = MEMORIA_NOACK;
   return (0);
}

///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
int MEMORIA_cancel(void){

   if((MEM_proceso != GET) || (MEM_proceso != SET)){
      return(-1);
   }

   if(MEM_proceso == GET){
      fputc(MEMORIA_NOACK, MEMORIA);
      MEM_RESPONSE = MEMORIA_getc();
   }

   if(MEM_proceso == SET){
      while(tamano > 0 ){
         fputc(0x00,MEMORIA);
         --tamano;
      }
   }

   tamano = 0;
   #ifdef debug_memoria
	update_proceso(OPEN);
	#else
	MEM_proceso = OPEN;
	#endif
   MEM_handshaking = MEMORIA_DEFAULT_HANDSHAKING;
   return (0);
}

///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
int MEMORIA_write(unsigned int size){
   
   if(!MEMORIA_OK || MEM_proceso != WR)
      return(1);
   
   //buffer maximo de 100 bytes
   if((size > 0) && (size <= MAX_BUFFER))
      MEM_handshaking = MEMORIA_NO_HANDSHAKING;
   else
      return (2);
   
   tamano = (unsigned int32)size;
   fputc(MEMORIA_EXT_CMD,MEMORIA);
   fputc(MEMORIA_CMD_WRITE_FILE,MEMORIA);
   fputc((MEM_handshaking|MEM_append|MEM_performance),MEMORIA);
   
   for(i =0; (i < MEMORIA_NAME_LENG_LIMIT) && (i < car);i++)
      fputc(MEM_file_name[i],MEMORIA);

   fputc(0x00,MEMORIA);

   fputc(make8(tamano,3),MEMORIA);
   fputc(make8(tamano,2),MEMORIA);
   fputc(make8(tamano,1),MEMORIA);
   fputc(make8(tamano,0),MEMORIA);

   MEM_RESPONSE = MEMORIA_getc();
   if((MEM_RESPONSE == 0) && timeout_error)
      return (3);
   if(MEM_RESPONSE != MEMORIA_ACK){
      return (4);
   }
   #ifdef debug_memoria
	update_proceso(SET);
	#else
	MEM_proceso = SET;
	#endif
   return (0);
}

///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
int MEMORIA_set_data(char *data, unsigned int size){

   if(!MEMORIA_OK || (MEM_proceso != SET) || (tamano <= 0))
      return(-1);

   i = 0;
   while((tamano > 0)&&(i < size)){
      fputc(data[i],MEMORIA);
      i++;
      tamano--;
   }
   //si no hay mas datos
   if(tamano <= 0){
      MEM_RESPONSE = MEMORIA_getc();
      if(((MEM_RESPONSE == 0) && timeout_error)||(MEM_RESPONSE != MEMORIA_ACK)){
         return (2);
      }
      #ifdef debug_memoria
		update_proceso(CLOSE);
		#else
		MEM_proceso = CLOSE;
		#endif
   }
   return (i);
}

///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
char MEMORIA_putc(char c){
   fputc(c, MEMORIA);
   if(MEM_handshaking == MEMORIA_DEFAULT_HANDSHAKING){
      return (MEMORIA_getc());   
   }
   return (0);   
}

///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
unsigned int32 MEMORIA_read(unsigned int num_bytes){
   char Umsb = 0, Ulsb = 0, Lmsb = 0,Llsb = 0;

   if(!MEMORIA_OK || (MEM_proceso != RD)){
      return(0);
   }

   if((num_bytes > 0) && (num_bytes <= MAX_BUFFER))
      MEM_handshaking = (char)num_bytes;
   else
      return (2);
   
   
   fputc(MEMORIA_EXT_CMD,MEMORIA);
   fputc(MEMORIA_CMD_READ_FILE,MEMORIA);
   fputc(MEM_handshaking,MEMORIA);
   
   for(i =0; (i < MEMORIA_NAME_LENG_LIMIT) && (i < car); ++i)
      fputc(MEM_file_name[i],MEMORIA);

   fputc(0x00,MEMORIA);

   Umsb = MEMORIA_getc();
   if(!Umsb && timeout_error) return (0);
   Ulsb = MEMORIA_getc();
   if(!Ulsb && timeout_error) return (0);
   Lmsb = MEMORIA_getc();
   if(!Lmsb && timeout_error) return (0);
   Llsb = MEMORIA_getc();
   if(!Llsb && timeout_error) return (0);

   tamano = make32(Umsb,Ulsb,Lmsb,Llsb);
   #ifdef debug_memoria
	update_proceso(GET);
	#else
	MEM_proceso = GET;
	#endif
   return (tamano);                                              
}


///////////////////////////////////////////////////////////
//funcion general para recivir datos desde la memoria
///////////////////////////////////////////////////////////
int MEMORIA_get_data(char *buffer){   
   char c;
   
   if(!MEMORIA_OK || (MEM_proceso != GET) || (tamano <= 0))
      return(-1);

   i = 0;
   fputc(MEMORIA_ACK, MEMORIA);                                                  //envia un ACK para recivir nuevos datos
   while((i < MEM_handshaking) && (tamano > 0 /*hay mas datos*/)){
      c = MEMORIA_getc();
      
      if(!c && timeout_error)
         return (-1);
      
      buffer[i] = c;
      i++;
      tamano--;
   }

   //si alcanzo el total de datos
   if(tamano == 0){
      MEM_RESPONSE = MEMORIA_getc();
      if(((MEM_RESPONSE == 0) && timeout_error)||(MEM_RESPONSE != MEMORIA_ACK)){
         return (-1);
      }
      #ifdef debug_memoria
		update_proceso(CLOSE);
		#else
		MEM_proceso = CLOSE;
		#endif
   }
   return (i);   
}

///////////////////////////////////////////////////////////
//funcion general cerrar el actual archivo de trabajo
///////////////////////////////////////////////////////////
int MEMORIA_close(void){

   if(!MEMORIA_OK || (MEM_proceso != CLOSE))
      return (-1);

   strcpy (MEM_file_name,"");
   i = 0;
   car = 0;
   timeout_error = FALSE;
   MEM_handshaking = MEMORIA_DEFAULT_HANDSHAKING;
   MEM_RESPONSE = MEMORIA_NOACK;
   #ifdef debug_memoria
	update_proceso(OPEN);
	#else
	MEM_proceso = OPEN;
	#endif
   return(0);
}

///////////////////////////////////////////////////////////
//funcion general para configurar las opciones de la memoria
///////////////////////////////////////////////////////////
int MEMORIA_set_param(unsigned int param, char *value, unsigned int size){return (0);}


///////////////////////////////////////////////////////////
//funcion general para saber si la mmeoria esta actualmente en un proceso de trabajo
///////////////////////////////////////////////////////////
int MEMORIA_is_busy(void){
   return ((MEM_proceso == OPEN )? 0 : 1);
} 

///////////////////////////////////////////////////////////
//funcion general obtener un caracter trasmitido desde la memoria
///////////////////////////////////////////////////////////
char MEMORIA_getc(void){
	char c = 0x00;
   unsigned int8 timeout;
   timeout_error=FALSE;
   timeout = 0;
   
   while(!kbhit(MEMORIA) && (timeout < 10)){
   	delay_ms(time_delay);
   	timeout++;
   	#ifdef debug_memoria
   	printf(usb_cdc_putc,"\n\rt: %u", timeout);
   	#endif
   }

   if(kbhit(MEMORIA))
      c = fgetc(MEMORIA);
   else {
      timeout_error=TRUE;
   }
   #ifdef debug_memoria
   printf(usb_cdc_putc,"\n\rm: %x", c);
   #endif
   return(c);
}
