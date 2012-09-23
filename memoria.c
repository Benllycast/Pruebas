#include "Nucleo.h"
#include "memoria.h"
#use rs232(stream=MEMORIA,baud=9600,parity=N,xmit=PIN_D6,rcv=PIN_D7,bits=8,FORCE_SW,DISABLE_INTS)

short MEMORIA_OK = FALSE;
short MEMORIA_HW = FALSE;                                      //comprueba que la memoria este inicializada
short timeout_error = FALSE;
int MEM_proceso = -1;                                          //puede ser de escritura o lectura
unsigned int i = 0;
unsigned int car = 0;
unsigned long time_delay = 50000;
int16 MEMORIA_PIN_RESET = PIN_D5;
unsigned int32 tamano = 0;
char MEM_info[5] = {0x00,0x00,0x00,0x00,0x00};                 //guarda la informacion del dispositivo de memoria conectado
char MEM_file_name[MEMORIA_NAME_LENG_LIMIT];                   //nombre del archivo actualmente utilizado
char MEM_handshaking = 0x01;                                   //opcion de handshaking
char MEM_append = MEMORIA_APPEND;                              //opcion para crear o agregar datos en un archivo
char MEM_performance = MEMORIA_HIGH_PERFORMANCE;               //opcion de velocidad de trasnmicion
char MEM_RESPONSE = MEMORIA_NOACK;

///////////////////////////////////////////////////////////
//funcion general para incializar el dispositivo de memoria
///////////////////////////////////////////////////////////
int MEMORIA_init(void){

   if(!MEMORIA_HW)
      return (-1);

   MEMORIA_reset();
   delay_ms(550);
   
   fputc(MEMORIA_CMD_AUTOBAUD,MEMORIA);  
   MEM_RESPONSE = MEMORIA_getc();
   if((!MEM_RESPONSE && timeout_error)||(MEM_RESPONSE != MEMORIA_ACK)){
      return (1);
   }

   fputc(MEMORIA_CMD_INITIALIZE,MEMORIA);
   MEM_RESPONSE = MEMORIA_getc();
   if((!MEM_RESPONSE && timeout_error)||(MEM_RESPONSE != MEMORIA_ACK)){
      return (2);
   }else{
      MEMORIA_OK = TRUE;
   }

   fputc(MEMORIA_CMD_VER_INFO, MEMORIA);                    //envia el comando de informacion del dispositivo
   MEM_info[0] = MEMORIA_getc();
   MEM_info[1] = MEMORIA_getc();
   MEM_info[2] = MEMORIA_getc();
   MEM_info[3] = MEMORIA_getc();
   MEM_info[4] = MEMORIA_getc();
   
   return (0);
}      //inicializa el dispositivo
///////////////////////////////////////////////////////////
//funcion general para señal de reset al dispositivo de memoria
///////////////////////////////////////////////////////////
void MEMORIA_reset(void){
   output_low(MEMORIA_PIN_RESET);
   delay_ms(5);
   output_float(MEMORIA_PIN_RESET);
}

///////////////////////////////////////////////////////////
//funcion para cancelar la operaciones en un archivo 
///////////////////////////////////////////////////////////
int MEMORIA_cancel(void){

   if(MEM_proceso == RD){
      fputc(MEMORIA_NOACK, MEMORIA);
      MEM_RESPONSE = MEMORIA_getc();
   }

   if(MEM_proceso == WR){
      return (-1);
   }

   tamano = 0;
   MEM_proceso = (-1);
   MEM_handshaking = MEMORIA_DEFAULT_HANDSHAKING;
   return (0);
}

///////////////////////////////////////////////////////////
//funcion general para inicializar el pin de reset del dispositivo de memoria
///////////////////////////////////////////////////////////
int MEMORIA_init_hw(){
   //aqui se inicializa el hardwar de la memoria
   /*
   {
      CODIGO DE INICIALIZACION DEL PIN_DE_RESET DEL
      DISPOSITIVO DE MEMORIA;     
   }
   */
   output_float(MEMORIA_PIN_RESET);
   MEMORIA_HW = TRUE;
   
   /*
   
   MEMORIA_OK = FALSE;
   timeout_error = FALSE;
   MEM_proceso = -1;
   i = 0;
   car = 0;
   time_delay = 50000;
   MEMORIA_PIN_RESET = PIN_D5;
   tamano = 0;
   MEM_info[0] = 0x00;
   MEM_info[1] = 0x00;
   MEM_info[2] = 0x00;
   MEM_info[3] = 0x00;
   MEM_info[4] = 0x00;
   strcpy(MEM_file_name,"");
   MEM_handshaking = 0x01;
   MEM_append = MEMORIA_APPEND;
   MEM_performance = MEMORIA_HIGH_PERFORMANCE;
   MEM_RESPONSE = MEMORIA_NOACK;

   */
   return (0);
}
///////////////////////////////////////////////////////////
//funcion global para la escritura de datos en un archivo
///////////////////////////////////////////////////////////
int MEMORIA_write(unsigned int size){

   if(!MEMORIA_OK || MEM_proceso != WR){
      return(-1);
   }
   if(size <= 100){
      MEM_handshaking = MEMORIA_NO_HANDSHAKING;
   }else{
      MEM_handshaking = MEMORIA_MAX_HANDSHAKING;
   }
   
   tamano = (unsigned int32)size;
   car = strlen(MEM_file_name);

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
   if(!MEM_RESPONSE && timeout_error)
      return (-1);
   if(MEM_RESPONSE != MEMORIA_ACK){
      return (-1);
   }
   return (0);
}

///////////////////////////////////////////////////////////
//funcion para enviar multiples bytes de datos hacia la memoria
///////////////////////////////////////////////////////////
int MEMORIA_set_data(char *data){
//tamano buffer mañimo de 100 bytes
   if(!MEMORIA_OK || (MEM_proceso != WR))
      return(-1);
   
   i = 0;
   if(MEM_handshaking == MEMORIA_NO_HANDSHAKING){
      while((tamano > 0) && (i < MEM_handshaking)){
         fputc(data[i],MEMORIA);
         i++;
         tamano--;
      }
   }else{
      while((tamano > 0)){
         fputc(data[i],MEMORIA);
         i++;
         tamano--;
      }
   }

   //si alcanzo el total de datos
   if(tamano == 0){
      MEM_RESPONSE = MEMORIA_getc();
      if((!MEM_RESPONSE && timeout_error)||(MEM_RESPONSE != MEMORIA_ACK)){
         return (-1);
      }
      MEM_proceso = (-1);
   }
   return (i);
}

///////////////////////////////////////////////////////////
//funcion global para leer datos guardados en un archivo 
///////////////////////////////////////////////////////////
unsigned int32 MEMORIA_read(void){
   char Umsb = 0, Ulsb = 0, Lmsb = 0,Llsb = 0;

   if(!MEMORIA_OK || (MEM_proceso != RD)){                                         //comprueba que la memoria este inicializada
      return(0);
   }

   fputc(MEMORIA_EXT_CMD,MEMORIA);                          //envia el @ a la memoria
   fputc(MEMORIA_CMD_READ_FILE,MEMORIA);                    //comando de lectura en archivo
   fputc(MEMORIA_MAX_HANDSHAKING,MEMORIA);                   //opciones del comando
   
   for(i =0; (i < MEMORIA_NAME_LENG_LIMIT) && (i < car);i++)
      fputc(MEM_file_name[i],MEMORIA);                          //envia el nombre de archivo a escribir

   fputc(0x00,MEMORIA);                                     //terminador de nombre de cadena

   Umsb = MEMORIA_getc();
   if(!Umsb && timeout_error) return (0);
   Ulsb = MEMORIA_getc();
   if(!Ulsb && timeout_error) return (0);
   Lmsb = MEMORIA_getc();
   if(!Lmsb && timeout_error) return (0);
   Llsb = MEMORIA_getc();
   if(!Llsb && timeout_error) return (0);

   tamano = make32(Umsb,Ulsb,Lmsb,Llsb);
   return (tamano);                                              
}


///////////////////////////////////////////////////////////
//funcion general para recivir datos desde la memoria
///////////////////////////////////////////////////////////
int MEMORIA_get_data(char *buffer){   
   char c;
   
   if(!MEMORIA_OK || (MEM_proceso != RD)){                                         //comprueba que la memoria este inicializada
      return(-1);
   }else{
      
      i = 0;
      fputc(MEMORIA_ACK, MEMORIA);//envia un ACK para recivir nuevos datos

      while((i < MEMORIA_MAX_HANDSHAKING) && (tamano > 0 /*hay mas datos*/)){
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
         MEM_proceso = (-1);
      }
      return (i);   
   }   
}

///////////////////////////////////////////////////////////
//funcion general para configurar las opciones de la memoria
///////////////////////////////////////////////////////////
int MEMORIA_set_param(unsigned int param, char *value, unsigned int size){return (0);}

///////////////////////////////////////////////////////////
//funcion general para seleccionar el archivo de trabajo
///////////////////////////////////////////////////////////
int MEMORIA_open(char *filename, char modo){
   unsigned int ncar =0;

   if(!MEMORIA_OK){
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
   MEM_proceso = (-1);
   
   return (0);
}

///////////////////////////////////////////////////////////
//funcion general para saber si la mmeoria esta actualmente en un proceso de trabajo
///////////////////////////////////////////////////////////
int MEMORIA_is_busy(void){

   if(MEM_proceso != (-1)){                                         //comprueba que la memoria este inicializada
      return(1);
   }
   return (0);
} 

///////////////////////////////////////////////////////////
//funcion general cerrar el actual archivo de trabajo
///////////////////////////////////////////////////////////
void close(){
   strcpy (MEM_file_name,"");
   i = 0;
   car = 0;
   timeout_error = FALSE;
   MEM_handshaking = MEMORIA_DEFAULT_HANDSHAKING;
   MEM_RESPONSE = MEMORIA_NOACK;
   MEM_proceso = (-1);
}//cierra el buffer de lectura del archivo

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
