#include "Nucleo.h"
#include <string.h>
#include "memoria.h"

#use rs232(stream=MEMORIA,baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)

short MEMORIA_OK = FALSE;
char response[MEMORIA_BUFFER];
char *ini_buffer = &response;
char info[5] = {0x00,0x00,0x00,0x00,0x00};
char file_name[12];
char handshaking = MEMORIA_HANDSHAKING;
char append = MEMORIA_APPEND;
char performance = MEMORIA_HIGH_PERFORMANCE;

int MEMORIA_write(char *buffer, unsigned int size){
   int i = 0;
   int enviados = 0;
   long tamano;
   char *ptrTamano;

   tamano = (long)size;
   *ptrTamano =  &tamano;
   
   fputc(MEMORIA_EXT_CMD,MEMORIA);
   fputc(MEMORIA_CMD_WRITE_FILE,MEMORIA);
   fputc((handshaking|append|performance),MEMORIA);
   
   for(i =0; i < MEMORIA_NAME_LENG_LIMIT;i++)
      fputc(file_name[i],MEMORIA);

   fputc(0x00,MEMORIA);
   
   for(i = 0; i < sizeof(tamano) ;i++)
      fputc(ptrTamano[i], MEMORIA);

   enviados = MEMORIA_send(buffer,size);
   if(enviados != size)
      return(-1);
   
   return (enviados);
}

int MEMORIA_read(char *buffer, unsigned int size){

   return (0);
}

int MEMORIA_send(char data){
   char respuesta = 0x00;
   fputc(data,MEMORIA);
#ifndef MEMORIA_OUT_TIME
      /////////////////////////////////
      if(kbhit(MEMORIA))
         respuesta = fgetc(MEMORIA);
#else   /////////////////////////////////
      while(!kbhit(MEMORIA) && (tiempo_limite < MEMORIA_OUT_TIME)){
         tiempo_limite++;
      }
      if(tiempo_limite >= MEMORIA_OUT_TIME)
         return(-1);
      /////////////////////////////////
      respuesta = fgetc(MEMORIA);
#endif 

   return ((respuesta == MEMORIA_ACK)? 0:1);
}

int MEMORIA_send(char *data, unsigned int size){
   unsigned int indice = 0;
   char respuesta = MEMORIA_NOACK;

#ifdef MEMORIA_OUT_TIME
   unsigned long tiempo_limite = 0; 
#endif

   do{
      fputc(data[indice],MEMORIA);
         
#ifndef MEMORIA_OUT_TIME
      /////////////////////////////////
      if(kbhit(MEMORIA))
         respuesta = fgetc(MEMORIA);
#else   /////////////////////////////////
      while(!kbhit(MEMORIA) && (tiempo_limite < MEMORIA_OUT_TIME)){
         tiempo_limite++;
      }
      if(tiempo_limite >= MEMORIA_OUT_TIME)
         return(indice);
      /////////////////////////////////
      respuesta = fgetc(MEMORIA);
#endif          
   }while((++indice < size) && (respuesta == MEMORIA_ACK));
   return (indice);      
}

int recivir(char *data , unsigned int size){
   return (0);
}

int MEMORIA_setparam(unsigned int param, char *value, unsigned int size){return (0);}

int MEMORIA_init(){
   char ack = MEMORIA_NOACK;

   MEMORIA_reset();
   delay_ms(550);
   
   fputc(MEMORIA_CMD_AUTOBAUD,MEMORIA);
   
   if(kbhit(MEMORIA))
      ack = fgetc(MEMORIA);
   if(ack != MEMORIA_ACK)
      return (-1);

   fputc(MEMORIA_CMD_VER_INFO, MEMORIA);
   
   while(!kbhit(MEMORIA));         //se espera a que se recivan datos desde el dispositivo

   info[0] = fgetc(MEMORIA);
   info[1] = fgetc(MEMORIA);
   info[2] = fgetc(MEMORIA);
   info[3] = fgetc(MEMORIA);
   info[4] = fgetc(MEMORIA);

   fputc(MEMORIA_CMD_INITIALIZE,MEMORIA);

   if(kbhit(MEMORIA))
      ack = fgetc(MEMORIA);
   if(ack != MEMORIA_ACK){
      return (-2);
   }else{
      MEMORIA_OK = TRUE;
   }
   return (0);

}      //inicializa el dispositivo
void MEMORIA_reset(){
   //resetea el dispositivo
}
int open(char *name_file){

#ifdef MEMORIA_COMPROBAR_ARCHIVO
   /////////////////////////////
   char temp_name[12], temp_name2[12];
   char c = 0x00;
   int i = 0, match = -1;
   bool find = FALSE;
   int ncar = strleng(name_file);

   if(!MEMORIA_OK){
      return(-1);
   }
   
   
   if(ncar > MEMORIA_NAME_LENG_LIMIT){
      ncar = MEMORIA_NAME_LENG_LIMIT;
   }
   strncpy(temp_name, name_file, ncar);
   
   fputc(MEMORIA_EXT_CMD, MEMORIA);
   fputc(MEMORIA_CMD_LIST_DIRECTORY, MEMORIA);
   for(i = 0; i < ncar; i++){
      fputc(temp_name[i], MEMORIA);
   }
   fputc(MEMORIA_NAME_TERMINATOR, MEMORIA);
   
   i = 0;
   do{
      if(kbhit(MEMORIA))
         c = fgetc(MEMORIA);
      
      if(isalnum(c) && !find){
         strcat(temp_name2,&c);
         i++;
      }

      if(c == MEMORIA_DELIMITADOR && !find){
         match = strncmp(temp_name,temp_name2,i);
         if(match == 0){
            find = TRUE;
         }
         strcpy(temp_name2, "");
         i = 0;
      }
   }while(c != MEMORIA_ACK || c!= MEMORIA_NOACK);
   
   if(find){
      strcpy(file_name, temp_name);
      return(1);
   }else{
      return(0);
   }
   /////////////////////////////
#else   
   /////////////////////////////
   int ncar = strlen(name_file);

   if(!MEMORIA_OK){
      return(-1);
   }
   
   if(ncar > MEMORIA_NAME_LENG_LIMIT){
      ncar = MEMORIA_NAME_LENG_LIMIT;
   }
   strncpy(file_name, name_file, ncar);
   return (1);
   /////////////////////////////
#endif
}   //abre el buffer de lectura del archivo
void close(){}   //cierra el buffer de lectura del archivo
