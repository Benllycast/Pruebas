#include "Nucleo.h"
#include "accelerometro.h"


void config_MMA(){
 return;
} //setear la cofiguracion incial

void set_config(*CONFIG_MMA7455){
 return;
}// cambiar la configuracion incicial (del SPI y/o MMA)


/*/////////////////////////////////////////////
read_MMA(): funcion para la lectura de datos desde el accelerometro MMA7455
            hasta el dispositivo maestro
parametros:
  start: direccion de inicial desde se realizara la lectura
  buffer: buffer de almacenamiento de los datos leidos
  size: cantidad de bytes a leer desde el esclavo

returna:
  int: 0 no error, 
      1 error de inicio de transmision, 
      2 error de peticion de datos,
      -11 error en la lectura desde el MMA 
*/
int read_MMA(unsigned char address, int8 *value){
  int error = -1;
   //condicion de inicio de lectura en el MMA7455 
  //error = begin_transmision(MMA7455_I2C_ADDRESS); //inicia la transmicion al MMA7455    
  i2c_start();                                    //señal de start del bus i2c
  error = i2c_write(MMA7455_I2C_ADDRESS_WRITE);   //se envia la direccion del MMA7455 con escritura
  error = i2c_write(address);                       //envia el registro inicial de lectura del MMA
  if(error!=0){return 1;}                         //si existe un error sale de la funcion
  
  //peticion de datos desde el MMA7455
  //error = requesFROM(MMA7455_I2C_ADDRESS);      //enivia la peticion de incio de lectura desde el maestro
  i2c_start();                                    //señal de restart del bus i2c
  error = i2c_write(MMA7455_I2C_ADDRESS_READ);    //se envia la direccion del MMA7455 con lectura  
  if(error!=0){return 2;} 

  while(!i2c_poll());
  value = i2c_read(0);
  i2c_stop();
  return 0;
}

int read_MMA(unsigned char start_address, unsigned int8 *buffer,int size){
  int error = -1, i = 0;
  int1 ack = 1;
  
  //condicion de inicio de lectura en el MMA7455 
  //error = begin_transmision(MMA7455_I2C_ADDRESS); //inicia la transmicion al MMA7455    
  i2c_start();                                    //señal de start del bus i2c
  error = i2c_write(MMA7455_I2C_ADDRESS_WRITE);   //se envia la direccion del MMA7455 con escritura
  error = i2c_write(start_address);                       //envia el registro inicial de lectura del MMA
  if(error!=0){return 1;}                         //si existe un error sale de la funcion
  
  //peticion de datos desde el MMA7455
  //error = requesFROM(MMA7455_I2C_ADDRESS);      //enivia la peticion de incio de lectura desde el maestro
  i2c_start();                                    //señal de restart del bus i2c
  error = i2c_write(MMA7455_I2C_ADDRESS_READ);    //se envia la direccion del MMA7455 con lectura  
  if(error!=0){return 2;}                         //si error en reques sale de la funcion

  i = 0;
  ack = 1;
  while(i2c_poll() && i<size){                    //espera minetras llega un dato desde el MMA7455
    if((i+1) >= size){ack = 0;}                   //si es el ultimo byte envia NACK
    buffer[i++]=i2c_read(ack);                    //lee un byte del bus i2c y envia un ACK
  }

  i2c_stop();                                     //señal de stop en el bus i2c
  
  if ( i != size){return (3);}                    //retorna 3 si no se completo la lectura
  
  return (0);   //retorna 0 si succed
} //leer datos desde el mma


int write_MMA(unsigned char address, unsigned int8 *value){
  int error = 0;
  //condicion de inicio de escritura en el MMA7455
  i2c_start();                                    //señal de start del bus i2c
  error = i2c_write(MMA7455_I2C_ADDRESS_WRITE);   //se envia la direccion del MMA7455 con escritura
  error = i2c_write(address);               //envia el registro inicial de escritura del MMA
  if(error!=0){return 1;}
  error = i2c_write(value);               //envia el registro inicial de escritura del MMA
  if(error!=0){return 2;}
  i2c_stop();
  return 0;
}


int write_MMA(unsigned char start_address, unsigned int8 *pData, int size){
  int error = -1, i = 0;
  int1 ack = 0;

  //condicion de inicio de escritura en el MMA7455
  i2c_start();                                    //señal de start del bus i2c
  error = i2c_write(MMA7455_I2C_ADDRESS_WRITE);   //se envia la direccion del MMA7455 con escritura
  error = i2c_write(start_address);               //envia el registro inicial de escritura del MMA
  if(error!=0){return 1;}                         //si error en reques sale de la funcion
  do{
    ack = i2c_write(pData[i++]);                  //escribe un byte y espera un ACK
  }while(ack == 0 && i < size);
  i2c_stop();
  if(i != size){return 2;}                        //si no se completo la escritura retorna error 2
  
  return 0;//retorna 0 si succed
} //escribir datos desde el mma
/*
int8 begin_transmision(unsigned int8 address){     
  unsigned int8 out = 0;
  int8 error = -1;
  out = (address << 1);
  bit_clear(out, 0);
  i2c_start();
  error = i2c_write(out);
  return error;
} //seleccion del dispositivo en el bus spi

int1 end_transmicion(){
  return;
} // deseleccion del dispositivo en el bus spi

int8 requesFROM(unsigned int8 address){
  unsigned int8 out = 0;
  int8 error = -1;
  out = (address << 1);
  bit_set(out, 0);
  i2c_start();
  error = i2c_write(out);
  return error;
}

int8 bit(int8 pos){
  int8 bits = 1;
  return (bits<<pos);
}
*/
