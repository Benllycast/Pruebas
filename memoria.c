//#include "Nucleo.h"
#ifdef MEMORIA_H

//#include "memoria.h"
#include "comunicacion.h"

//borrar para quitar el debug de memoria
#define debug_memoria 1	

// #define MEM_RX PIN_D7
// #define MEM_TX PIN_D6	
#define MEMORIA_PIN_RESET PIN_D5
#use rs232(uart1, stream=MEMORIA, BAUD=9600, TIMEOUT=1000)
#define time_delay 1000

short MEMORIA_OK = FALSE;
short MEMORIA_HW = FALSE;
short timeout_error = FALSE;
short read_flag = 0; //utilizada por un bug en la lectura de archivos desde dispositivo de memoria
int8 MEM_proceso = INI_HW;
unsigned int i = 0;
unsigned int car = 0;
unsigned int32 tamano = 0;
char MEM_info[5] = {0x00,0x00,0x00,0x00,0x00};
char MEM_file_name[MEMORIA_NAME_LENG_LIMIT];
char MEM_handshaking = MEMORIA_DEFAULT_HANDSHAKING;
char MEM_append = MEMORIA_APPEND;
//char MEM_performance = MEMORIA_HIGH_PERFORMANCE;	//cambiar a low performance
char MEM_performance = MEMORIA_LOW_PERFORMANCE;
char MEM_RESPONSE = MEMORIA_NOACK;

#ifdef debug_memoria
void update_proceso(int8 proceso){
	MEM_proceso = proceso;
	printf(usb_cdc_putc_fast,"\n\rMp>%d", MEM_proceso);
	return;
}
#endif

/*	======================================
	FUNCIONES PARA EL INICIO DEL MODULO DE MEMORIA
	======================================
*/
/*==================== reset de memoria ======================*/
int MEMORIA_reset(void){
	#ifdef debug_memoria
	usb_cdc_putc_fast('r');
	#endif
	output_float(PIN_D6);
   output_low(MEMORIA_PIN_RESET);
   delay_ms(1000);
   read_flag = 0;
   output_high(MEMORIA_PIN_RESET);
   delay_ms(3000);
   return(0);
}

/*==================== autobaudrate ======================*/
int MEMORIA_init_hw(void){
	int8 envios = 10;
	#ifdef debug_memoria
	update_proceso(INI_HW);
	#else
	MEM_proceso = INI_HW;
	#endif
	
	do{
		MEMORIA_putc(MEMORIA_CMD_AUTOBAUD);
		MEM_RESPONSE = MEMORIA_getc();
		envios --;
		delay_ms(200);
	}while((envios > 0) && (MEM_RESPONSE != MEMORIA_ACK) );                                                       
   
   if(MEM_RESPONSE != MEMORIA_ACK){                                
		return (1);
	}
   
   MEMORIA_HW = TRUE;
   MEMORIA_OK = FALSE;
   read_flag = 0;
   #ifdef debug_memoria
	update_proceso(INI_SW);
	#else
	MEM_proceso = INI_SW;
	#endif
	
   return (0);
}

/*==================== iniciar memoria ======================*/
int MEMORIA_init(void){

   if(!MEMORIA_HW || (MEM_proceso != INI_SW)) return (1);
	
	MEMORIA_putc(MEMORIA_EXT_CMD);
   MEMORIA_putc(MEMORIA_CMD_INITIALIZE);
   
   MEM_RESPONSE = MEMORIA_getc();
   
   if(MEM_RESPONSE != MEMORIA_ACK) return(2);
   else MEMORIA_OK = TRUE;
   
   read_flag = 0;
   #ifdef debug_memoria
	update_proceso(OPEN);
	#else
	MEM_proceso = OPEN;
	#endif
   return (0);
}

/*====================get info======================*/
void MEMORIA_getinfo(){

	MEMORIA_putc(MEMORIA_CMD_VER_INFO);
   MEM_info[0] = MEMORIA_getc();
   MEM_info[1] = MEMORIA_getc();
   MEM_info[2] = MEMORIA_getc();
   MEM_info[3] = MEMORIA_getc();
   MEM_info[4] = MEMORIA_getc();
   #ifdef debug_memoria
   printf(usb_cdc_putc_fast,"\n\rMemInf: %x %x %x %x %x", MEM_info[0],MEM_info[1],MEM_info[2],MEM_info[3],MEM_info[4]);
   #endif
   return;
}



/*	======================================
	FUNCIONES PARA EL MANEJO DE ARCHIVOS
	======================================
*/

/*====================abrir archivo======================*/

int MEMORIA_open(char* filename, short modo){

   if(!MEMORIA_OK) return(1);

   if(modo){
      #ifdef debug_memoria
		update_proceso(WR);
		#else
		MEM_proceso = WR;
		#endif
   }else{
		#ifdef debug_memoria
		update_proceso(RD);
		#else
		MEM_proceso = RD;
		#endif
   }
   
   car = strlen(filename);
   car = (car <= MEMORIA_NAME_LENG_LIMIT)? car : MEMORIA_NAME_LENG_LIMIT; 
   strncpy(MEM_file_name, filename, car);
   
   i = 0;
   timeout_error = FALSE;
   MEM_handshaking = MEMORIA_DEFAULT_HANDSHAKING;
   MEM_RESPONSE = MEMORIA_NOACK;
   return (0);
}

/*==================== cancelar proceso ======================*/

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

/*==================== enviar comando de escritura ======================*/
/*
 *BUG: puede retornar 1 0 en la primera escritura del archivo
 *TODO: corregir para primera escritura 
*/
int MEMORIA_write(unsigned int size){
   
   if(!MEMORIA_OK)return(-1);
   if(MEM_proceso != WR)return(-2);
      
   
   //buffer maximo de 100 bytes
   if((size > 0) && (size <= MAX_BUFFER))
      MEM_handshaking = MEMORIA_NO_HANDSHAKING;
   else
      return (-3);
   
   tamano = (unsigned int32)size;
   //tamano = (int32)size;
   MEMORIA_putc(MEMORIA_EXT_CMD);
   MEMORIA_putc(MEMORIA_CMD_WRITE_FILE);
   MEMORIA_putc((MEM_handshaking | MEM_append | MEM_performance));
   
   for(i = 0; i < car; i++)
      MEMORIA_putc(MEM_file_name[i]);

   MEMORIA_putc(0x00);

   MEMORIA_putc(make8(tamano,3));
   MEMORIA_putc(make8(tamano,2));
   MEMORIA_putc(make8(tamano,1));
   MEMORIA_putc(make8(tamano,0));

   MEM_RESPONSE = MEMORIA_getc();
   //reparacion de bug a primera escritura
   if(MEM_RESPONSE == 0x00){
   	MEM_RESPONSE = MEMORIA_getc();
   }
   
   if(MEM_RESPONSE != MEMORIA_ACK){
      return (-4);
   }
   #ifdef debug_memoria
	update_proceso(SET);
	#else
	MEM_proceso = SET;
	#endif
   return (0);
}

/*==================== enviar datos de escritura ======================*/
/*
 * corrwgir deacuerdo a MEMORIA_write()
 */
int MEMORIA_set_data(char *data, unsigned int size){

   if(!MEMORIA_OK)return(-5);
   if(MEM_proceso != SET) return(-6);
   if(tamano <= 0)return(-7);

   i = 0;
   while((tamano > 0)&&(i < size)){
      MEMORIA_putc(data[i]);
      i++;
      tamano--;
   }
   
   //si no hay mas datos
   if(tamano <= 0){
      MEM_RESPONSE = MEMORIA_getc();
      if(MEM_RESPONSE != MEMORIA_ACK){
         return (-8);
      }
      #ifdef debug_memoria
		update_proceso(CLOSE);
		#else
		MEM_proceso = CLOSE;
		#endif
   }
   return (i);
}

/*==================== enviar comando de lectura ======================*/

unsigned int32 MEMORIA_read(unsigned int num_bytes){
   char Umsb = 0, Ulsb = 0, Lmsb = 0,Llsb = 0, dummy= 0;

   if(!MEMORIA_OK)return(-1);
   if(MEM_proceso != RD)return(-2);

   if((num_bytes > 0) && (num_bytes <= MAX_BUFFER))
      MEM_handshaking = num_bytes;
   else
      return (-3);
   
   
   MEMORIA_putc(MEMORIA_EXT_CMD);
   MEMORIA_putc(MEMORIA_CMD_READ_FILE);
   MEMORIA_putc(MEM_handshaking);
   
   for(i = 0; i < car; i++)
      MEMORIA_putc(MEM_file_name[i]);
	
	fputc(0x00, MEMORIA);
	// hay un bug al leer por primera vez un archivo
	// genera 2 ACK antes del tamaño del archivo
	if(read_flag == 0){
		dummy = fgetc(MEMORIA);
		dummy = fgetc(MEMORIA);
		read_flag = 1;
	}
	//bug de comunicacion
	
	Umsb = fgetc(MEMORIA);
   Ulsb = fgetc(MEMORIA);
   Lmsb = fgetc(MEMORIA);
   Llsb = fgetc(MEMORIA);
   
   tamano = make32(Umsb,Ulsb,Lmsb,Llsb);
   #ifdef debug_memoria
   printf(usb_cdc_putc_fast,"\n\r==%x %x %x %x", Umsb, Ulsb,Lmsb,Llsb);
	update_proceso(GET);
	#else
	MEM_proceso = GET;
	#endif
   return (tamano);
}


/*==================== obtener datos de la lectura ======================*/

int MEMORIA_get_data(char *buffer){   
   char c = 0x00;

   if(!MEMORIA_OK) return(-1);
   if(MEM_proceso != GET) return(-2);
   if(tamano <= 0) return(-3);

   i = 0;
   MEMORIA_putc(MEMORIA_ACK);//envia un ACK para recivir nuevos datos
   while((i < MEM_handshaking) && (tamano > 0 )){
      c = MEMORIA_getc();
      buffer[i] = c;
      i++;
      tamano--;
   }

   //si alcanzo el total de datos
   if(tamano == 0){
      MEM_RESPONSE = MEMORIA_getc();
      if(MEM_RESPONSE != MEMORIA_ACK){
         return (-4);
      }
      #ifdef debug_memoria
		update_proceso(CLOSE);
		#else
		MEM_proceso = CLOSE;
		#endif
   }
   return (i);   
}

/*==================== cerrar el archivo abierto ======================*/

int MEMORIA_close(void){
	/*
	* hay un bug en esta funcion que no contempla limpiar
	* la configuracion cuando no ha iniciado el hardware o
	* despues de un reset
	*/
	/*
   if(!MEMORIA_OK)return (1);
   if(MEM_proceso != CLOSE) return(2);
   */
   strcpy (MEM_file_name,"");	//cambiar para inicializar a 0
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

/*	======================================
	FUNCIONES FUNCIONES DE CONTROL Y BAJO NIVEL
	======================================
*/

/*==================== memoria ocupada =======================*/
/*comprueba que no se este realizando un porceso anterior en la memoria*/
int MEMORIA_is_busy(void){
   return ((MEM_proceso == OPEN )? 0 : 1);
} 

/*==================== colocar un caracter en el bus======================*/
void MEMORIA_putc(char c){
   #ifdef debug_memoria
	printf(usb_cdc_putc_fast, "\n\r>%X", c);
	#endif
   fputc(c, MEMORIA);
   #ifdef debug_memoria
	printf(usb_cdc_putc_fast, " e%X", rs232_errors);
	#endif
   return;
}

/*==================== obtiene un caracter del bus======================*/
char MEMORIA_getc(void){
	char c = 0x00, cont = 3;
   timeout_error=FALSE;
   while(!kbhit(MEMORIA) && cont > 0 && !c){
   	c = fgetc(MEMORIA);
   	#ifdef debug_memoria
   	usb_cdc_putc_fast('*');
   	#endif
   	cont--;
   }
   
   if(!c) c = fgetc(MEMORIA);
   
   #ifdef debug_memoria
   printf(usb_cdc_putc_fast,"\n\r<%x e%x", c, rs232_errors);
   #endif
   return(c);
}
#endif	// MEMORIA_H
