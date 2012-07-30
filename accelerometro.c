#include "Nucleo.h"
#include "accelerometro.h"

//configuracion inicial del MMA7455
int init_MMA(void){
  int error = 0;
  xyz_union xyz;
  unsigned int8 c1, c2;
  //mode: measurement; sensitivity: 4g
  c1 = (1<<MMA7455_GLVL1)|(1<<MMA7455_MODE1);
  error = write_MMA(MMA7455_MCTL, &c1);
  if (error != 0)
    return (error);

  error = read_MMA(MMA7455_MCTL, &c2);
  if (error != 0)
    return (error);

  if (c1 != c2)
    return (-99);
  else
    CONFIG.MODE_CONTROL_REGISTER =  c2;

  xyz.value.x = xyz.value.y = xyz.value.z = 0;
  error = write_MMA(MMA7455_XOFFL, (unsigned int8 *) &xyz, 6);
  if (error != 0)
    return (error);
  
  delay_ms(100);
  return 0;
}

//calibra el offset de los valores de X, Y, Z.
int calibrate_MMA(void){
  int x, y, z, error = 0;
  xyz_union xyz;
  int c1, c2;

  c1 = CONFIG.MODE_CONTROL_REGISTER.GLVL;
  if(c1 == 0){
    c2 = 16;
  }else if(c1 == 2){
    c2 = 32;
  }else{
    c2 = 64;
  }

  error = xyz_MMA(&x, &y, &z);
  if (error != 0)
    return (error);

  xyz.value.x = 2 * -x;        
  xyz.value.y = 2 * -y;
  xyz.value.z = 2 * -(z-c2);

  error = write_MMA(MMA7455_XOFFL, (unsigned int8 *) &xyz, 6);
  if (error != 0)
    return (error);

  delay_ms(200);
  
  error = xyz_MMA(&x, &y, &z);
  if (error != 0)
    return (error);

  xyz.value.x = 2 * -x;        
  xyz.value.y = 2 * -y;
  xyz.value.z = 2 * -(z-c2);

  error = write_MMA(MMA7455_XOFFL, (unsigned int8 *) &xyz, 6);
  if (error != 0)
    return (error);

  error = read_MMA(MMA7455_XOFFL, (unsigned int8 *) &xyz, 6);
  if (error != 0)
    return (error);

  CONFIG.XOFFL = xyz.reg.x_lsb;
  CONFIG.XOFFH = xyz.reg.x_msb;
  CONFIG.YOFFL = xyz.reg.x_lsb;
  CONFIG.YOFFH = xyz.reg.x_msb;
  CONFIG.ZOFFL = xyz.reg.x_lsb;
  CONFIG.ZOFFH = xyz.reg.x_msb;

  return 0;
}

void set_config(*CONFIG_MMA7455){
 return;
}// cambiar la configuracion incicial (del SPI y/o MMA)


int xyz_MMA( int *pX, int *pY, int *pZ){
  xyz_union xyz;
  int error;
  unsigned int8 c;

  do{

    error = read_MMA(MMA7455_STATUS,&c);
  }while(!bit_test(c, MMA7455_DRDY) && error == 0);
  if (error != 0)
    return (error);

  error = read_MMA(MMA7455_XOUTL, (unsigned int8 *)&xyz, 6);
  if (error != 0)
    return (error);

  // The output is 10-bits and could be negative.
  // To use the output as a 16-bit signed integer,
  // the sign bit (bit 9) is extended for the 16 bits.
  if (xyz.reg.x_msb & 0x02)    // Bit 9 is sign bit.
    xyz.reg.x_msb |= 0xFC;     // Stretch bit 9 over other bits.
  if (xyz.reg.y_msb & 0x02)
    xyz.reg.y_msb |= 0xFC;
  if (xyz.reg.z_msb & 0x02)
    xyz.reg.z_msb |= 0xFC;

  // The result is the g-force in units of 64 per 'g'.
  *pX = xyz.value.x;
  *pY = xyz.value.y;
  *pZ = xyz.value.z;

  return 0;
}
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
  //error = requesFROM(MMA7455_I2C_ADDRESS);      //envia la peticion de incio de lectura desde el maestro
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


int write_MMA(unsigned char address, int8 *value){
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


int write_MMA(unsigned char start_address, int8 *pData, int size){
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

