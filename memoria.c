#include "Nucleo.h"
#include "memoria.h"
#use rs232(stream=MEMORIA,baud=9600,parity=N,xmit=PIN_D6,rcv=PIN_D7,bits=8,FORCE_SW,DISABLE_INTS)

short MEMORIA_OK = FALSE;
short MEMORIA_HW = FALSE;
short timeout_error = FALSE;
int MEM_proceso = INI_HW;
unsigned int i = 0;
unsigned int car = 0;
unsigned long time_delay = 50000;
int16 MEMORIA_PIN_RESET = PIN_D5;
unsigned int32 tamano = 0;
char MEM_info[5] = {0x00,0x00,0x00,0x00,0x00};
char MEM_file_name[MEMORIA_NAME_LENG_LIMIT];
char MEM_handshaking = MEMORIA_DEFAULT_HANDSHAKING;
char MEM_append = MEMORIA_APPEND;
char MEM_performance = MEMORIA_HIGH_PERFORMANCE;
char MEM_RESPONSE = MEMORIA_NOACK;

///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
int MEMORIA_reset(void){
   output_low(MEMORIA_PIN_RESET);
   delay_ms(5);
   return (MEMORIA_init_hw());
}

///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
int MEMORIA_init_hw(void){
   output_float(MEMORIA_PIN_RESET);
   MEMORIA_HW = TRUE;
   MEMORIA_OK = FALSE;
   MEM_info[0] = 0x00;
   MEM_info[1] = 0x00;
   MEM_info[2] = 0x00;
   MEM_info[3] = 0x00;
   MEM_info[4] = 0x00;
   MEM_proceso = INI_SW;
   return (0);
}

///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
int MEMORIA_init(void){

   if(!MEMORIA_HW || MEM_proceso != INI_SW)
      return (-1);

   delay_ms(550);
   fputc(MEMORIA_CMD_AUTOBAUD,MEMORIA);  
   MEM_RESPONSE = MEMORIA_getc();
   if(((MEM_RESPONSE == 0) && timeout_error)||(MEM_RESPONSE != MEMORIA_ACK)){
      return (1);
   }

   fputc(MEMORIA_CMD_INITIALIZE,MEMORIA);
   MEM_RESPONSE = MEMORIA_getc();
   if(((MEM_RESPONSE == 0) && timeout_error)||(MEM_RESPONSE != MEMORIA_ACK)){
      return (2);
   }else{
      MEMORIA_OK = TRUE;
   }

   fputc(MEMORIA_CMD_VER_INFO, MEMORIA);
   MEM_info[0] = MEMORIA_getc();
   MEM_info[1] = MEMORIA_getc();
   MEM_info[2] = MEMORIA_getc();
   MEM_info[3] = MEMORIA_getc();
   MEM_info[4] = MEMORIA_getc();

   MEM_proceso = OPEN;
   return (0);
}      //inicializa el dispositivo

///////////////////////////////////////////////////////////
//
///////////////////////////////////////////////////////////
int MEMORIA_open(char *filename, char modo){
   unsigned int ncar =0;

   if(!MEMORIA_OK || MEM_proceso != OPEN){
      return(-1);
   }

   if(modo == 'r')
      MEM_proceso = RD;
   else if(modo == 'w')
      MEM_proceso = WR;
   else
      return(-1);
   
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
   MEM_proceso = OPEN;
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
   MEM_proceso = SET;
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
         return (-1);
      }
      MEM_proceso = CLOSE;
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
   MEM_proceso = GET;
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
      MEM_proceso = CLOSE;
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
   MEM_proceso = OPEN;
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
   long timeout;
   timeout_error=FALSE;
   timeout=0;
   while(!kbhit()&&(++timeout < time_delay)) // 1/2
      delay_us(time_delay);

   if(kbhit())
      return(fgetc(MEMORIA));
   else {
      timeout_error=TRUE;
      return(0);
   }
}
