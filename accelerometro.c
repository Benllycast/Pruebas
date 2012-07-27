#include "accelerometro.h"

void config_MMA(){
   return;
} //setear la cofiguracion incial

void set_config(*CONFIG_MMA7455){
   return;
}// cambiar la configuracion incicial (del SPI y/o MMA)

int read_MMA(unsigned int8 start, int8 *buffer,int size){
   //condicion de inicio del MMA
   int error = -1, i = 0;
   int1 ack = 1;
   
   error = begin_transmicion(MMA7455_I2C_ADDRESS);//inicia la transmicion al MMA7455    
      error = i2c_write(start);//registro de inicio de lectura del MMA
      if(error!=0)
      return error;

   error = requesFROM(MMA7455_I2C_ADDRESS);
      if(error!=0)
      return error;
    
    i = 0;
    ack = 1;
    while(i2c_poll() && i<size){
       if((i+1) >= size)
          ack = 0;
       
       buffer[i++]=i2c_read(ack);
    }
    i2c_stop();

  if ( i != size)
    return (-11);

  return (0);
} //leer datos desde el mma

void write_MMA(){
      return;
} //escribir datos desde el mma

int8 begin_transmicion(unsigned int8 address){
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
