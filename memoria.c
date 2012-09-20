#include "Nucleo.h"
#include <string.h>
#include "memoria.h"

#use rs232(stream=MEMORIA,baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)

short MEMORIA_OK = FALSE;                                   //comprueba que la memoria este inicializada
char response[MEMORIA_BUFFER];                              //buffer para almacenar la respuesta de la memoria
char *ini_buffer = &response;                               //puntero para recorrer el buffer de memoria
char info[5] = {0x00,0x00,0x00,0x00,0x00};                  //guarda la informacion del dispositivo de memoria conectado
char file_name[MEMORIA_NAME_LENG_LIMIT];                    //nombre del archivo actualmente utilizado
char handshaking = MEMORIA_HANDSHAKING;                     //opcion de handshaking
char append = MEMORIA_APPEND;                               //opcion para crear o agregar datos en un archivo
char performance = MEMORIA_HIGH_PERFORMANCE;                //opcion de velocidad de trasnmicion


///////////////////////////////////////////////////////////
//funcion global para la escritura de datos en un archivo
///////////////////////////////////////////////////////////
int MEMORIA_write(char *buffer, unsigned int size){         
   int i = 0;
   int enviados = 0;
   int32 tamano = 0;
   int car = 0;
   tamano = (int32)size;
   car = strlen(file_name);                                 //cantidad de caracteres a enviar
   fputc(MEMORIA_EXT_CMD,MEMORIA);                          //envia el @ a la memoria
   fputc(MEMORIA_CMD_WRITE_FILE,MEMORIA);                   //comando de escritura en archivo
   fputc((handshaking|append|performance),MEMORIA);         //opciones del comando
   
   for(i =0; (i < MEMORIA_NAME_LENG_LIMIT) && (i < car);i++)
      fputc(file_name[i],MEMORIA);                          //envia lel nombre de archivo a escribir

   fputc(0x00,MEMORIA);                                     //terminador de nombre de cadena

   fputc(make8(tamano,3),MEMORIA);                          //envia el primer byte del tamaño del buffer
   fputc(make8(tamano,2),MEMORIA);
   fputc(make8(tamano,1),MEMORIA);
   fputc(make8(tamano,0),MEMORIA);                          //envia el ultimo byte del tamaño del buffer

   enviados = MEMORIA_send(buffer,size);                    //envia los datos a la memoria
   if(enviados != size)                                     //si no se completa el envio retorna -1
      return(-1);
   
   return (enviados);                                       //si todo ok retorna la cantida de bytes enviados
}


///////////////////////////////////////////////////////////
//funcion global para leer datos guardados en un archivo
///////////////////////////////////////////////////////////
int MEMORIA_read(char *buffer, unsigned int size){

   return (0);
}


///////////////////////////////////////////////////////////
//funcion para enviar un solo byte y esperar la respuesta desde el dispostivo de memoria
///////////////////////////////////////////////////////////
int MEMORIA_send(char data){
   char respuesta = 0x00;

#ifdef MEMORIA_OUT_TIME                                     //si el tiempo de la espera es limitado
   unsigned long tiempo_limite = 0;                         //variable para tiempo maximo de espera
#endif

   if(!MEMORIA_OK){                                         //comprueba que la memoria este inicializada
      return(-1);
   }

   fputc(data,MEMORIA);                                     //envia un dato a la memoria

#ifdef MEMORIA_OUT_TIME                                     //si el tiempo de la espera es limitado
   
      while(!kbhit(MEMORIA) && (tiempo_limite < MEMORIA_OUT_TIME)){  //espera mientras llega un dato o no sobrepase el limite de tiempo
         tiempo_limite++;
      }
      if(tiempo_limite >= MEMORIA_OUT_TIME)                 //si se pasa el tiempo de espera retorna -1
         return(-1);

#endif 
   if(kbhit(MEMORIA))                                       //espera la respuesta de la memoria
         respuesta = fgetc(MEMORIA);                        //lee la respuesta de la memoria

   return ((respuesta == MEMORIA_ACK)? 0:1);                //retorna 1 o 0 dependiendo si es ACK
}


///////////////////////////////////////////////////////////
//funcion para enviar multiples bytes de datos hacia la memoria
///////////////////////////////////////////////////////////
int MEMORIA_send(char *data, unsigned int size){
   unsigned int indice = 0;
   char respuesta = MEMORIA_NOACK;

#ifdef MEMORIA_OUT_TIME                                     //si el tiempo de la espera es limitado
   unsigned long tiempo_limite = 0; 
#endif

   if(!MEMORIA_OK){                                         //comprueba que la memoria este inicializada
      return(-1);
   }
   do{
      fputc(data[indice],MEMORIA);                          //envia 1 byte de datos a la memoria
         
#ifdef MEMORIA_OUT_TIME                                     //si el tiempo de la espera es limitado

      while(!kbhit(MEMORIA) && (tiempo_limite < MEMORIA_OUT_TIME)){
         tiempo_limite++;
      }
      if(tiempo_limite >= MEMORIA_OUT_TIME)
         return(indice);

#endif

      if(kbhit(MEMORIA))
         respuesta = fgetc(MEMORIA);                        //lee la respuesta de la memoria
      
      indice++;                                             //aumenta el indice de bytes
   }while((indice < size) && (respuesta == MEMORIA_ACK));
   return (indice);                                         //retorna el total de bytes enviados a la memoria
}


///////////////////////////////////////////////////////////
//funcion general para recivir datos desde la memoria
///////////////////////////////////////////////////////////
int recivir(char *data , unsigned int size){
   return (0);
}


///////////////////////////////////////////////////////////
//funcion general para configurar las opciones de la memoria
///////////////////////////////////////////////////////////
int MEMORIA_setparam(unsigned int param, char *value, unsigned int size){return (0);}


///////////////////////////////////////////////////////////
//funcion general para incializar el dispositivo de memoria
///////////////////////////////////////////////////////////
int MEMORIA_init(){
   char ack = MEMORIA_NOACK;

   MEMORIA_reset();                                         //envia un reset a la memoria para inicializarlar nuevamente    
   delay_ms(550);                                           //espera de 550ms para el envio de comandos
   
   fputc(MEMORIA_CMD_AUTOBAUD,MEMORIA);                     //configura el baudorate de la memoria
   
   if(kbhit(MEMORIA))                                       //espera la respuesa desde el dispositivo
      ack = fgetc(MEMORIA);
   if(ack != MEMORIA_ACK)
      return (-1);                                          //si repuesta  != ACK retorna -1

   fputc(MEMORIA_CMD_VER_INFO, MEMORIA);                    //envia el comando de informacion del dispositivo
   
   while(!kbhit(MEMORIA));                                  //se espera a que se recivan datos desde el dispositivo

   info[0] = fgetc(MEMORIA);                                //guarda la informacion del dispositivo
   info[1] = fgetc(MEMORIA);
   info[2] = fgetc(MEMORIA);
   info[3] = fgetc(MEMORIA);
   info[4] = fgetc(MEMORIA);

   fputc(MEMORIA_CMD_INITIALIZE,MEMORIA);                   //incializa la memoria en modo FAT16

   if(kbhit(MEMORIA))
      ack = fgetc(MEMORIA);

   if(ack != MEMORIA_ACK){
      return (-2);
   }else{
      MEMORIA_OK = TRUE;
   }
   return (0);

}      //inicializa el dispositivo


///////////////////////////////////////////////////////////
//funcion general para enviar una señal de reset al dispositivo de memoria
///////////////////////////////////////////////////////////
void MEMORIA_reset(){
   //resetea el dispositivo
}


///////////////////////////////////////////////////////////
//funcion general para seleccionar el archivo de trabajo
///////////////////////////////////////////////////////////
int MEMORIA_select_file(char *file_name){

#ifdef MEMORIA_COMPROBAR_ARCHIVO                            //si comprobar el archivo antes de crearlo
 
   char temp_name[12], temp_name2[12];
   char c = 0x00;
   int i = 0, match = -1;
   short ENCUENTRA = FALSE, new_char = FALSE;
   unsigned int ncar = strlen(file_name);


   if(!MEMORIA_OK){                                         //comprueba que la memoria este inicializada
      return(-1);
   }
   
   
   if(ncar > MEMORIA_NAME_LENG_LIMIT){
      ncar = MEMORIA_NAME_LENG_LIMIT;
   }
   strncpy(temp_name, file_name, ncar);
   
   fputc(MEMORIA_EXT_CMD, MEMORIA);
   fputc(MEMORIA_CMD_LIST_DIRECTORY, MEMORIA);
   for(i = 0; i < ncar; i++){
      fputc(temp_name[i], MEMORIA);
   }
   fputc(0x00, MEMORIA);
   
   i = 0;
   do{
      if(kbhit(MEMORIA)){
         c = fgetc(MEMORIA);
         new_char = TRUE;  
      }else{
         new_char = FALSE;
      }
         
      
      if(new_char && isalnum(c) && !ENCUENTRA ){
         strcat(temp_name2,&c);
         i++;
      }

      if(new_char && (c == MEMORIA_DELIMITADOR) && !ENCUENTRA){
         match = strncmp(temp_name,temp_name2,i);
         if(match == 0){
            ENCUENTRA = TRUE;
         }
         strcpy(temp_name2, "");
         i = 0;
      }
   }while(c != MEMORIA_ACK || c!= MEMORIA_NOACK);
   
   if(ENCUENTRA){
      strcpy(file_name, temp_name);
      return(1);
   }else{
      return(0);
   }

#else   
   
   unsigned int ncar =0;

   if(!MEMORIA_OK){                                               //comprueba que la memoria este inicializada
      return(-1);
   }
   ncar = strlen(file_name);
   ncar = (ncar <= MEMORIA_NAME_LENG_LIMIT)? ncar : MEMORIA_NAME_LENG_LIMIT;
   strncpy(file_name, file_name, ncar);
   return (1);
 
#endif
}   
void close(){}   //cierra el buffer de lectura del archivo
