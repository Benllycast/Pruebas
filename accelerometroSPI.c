#include "Nucleo.h"
//#include "ACCspi.h"
#include "accelerometroSPI.h" 

CONFIG_MMA7455 CONFIG = {1,2,3,128,128,128,128,128,128};
//configuracion inicial del MMA7455
int init_MMA(void){
  int error = 0;
  xyz_union xyz;
  unsigned int8 c1 = 0, c2 = 0;
  
  //mode: measurement; sensitivity: 4g
  c1 = MMA7455_DRPD|MMA7455_2g|MMA7455_MEASURE;
  #ifdef testmma
  printf(usb_cdc_putc,"\n\rc1: %x %u",c1,c1);
  #endif
  error = write_MMA(MMA7455_MCTL, &c1);       //escribe la configuracion deseada en el accelerometro
  if (error != 0)
    return (-1);
  
  //lee la configuracion del accelerometro para comprobar el envio
  error = read_MMA(MMA7455_MCTL, &c2);
  if (error != 0)
    return (-2);
  #ifdef testmma
  printf(usb_cdc_putc,"\n\rc1: %x %u",c2,c2);
  #endif
  
  if (c1 != c2)
    return (-3);
  else
    CONFIG.MODE_CONTROL =  c2;

  xyz.value.x = xyz.value.y = xyz.value.z = 0;
  error = write_MMA(MMA7455_XOFFL, (unsigned int8 *) &xyz, 6);
  if (error != 0)
    return (-4);
  
  delay_ms(100);                  //espera 100 mls para hasta se hayan escrito todos lso datos
  return 0;
}

/**
para la calibracion de los ejes se supone que este
en una posicion nivelada horizontalmente evitando posible vibraciones
*/
int calibrate_MMA(void){
  int x, y, z, error = 0;
  xyz_union xyz;
  unsigned int8 c1 = 0, c2 = 0;

  c1 = (CONFIG.MODE_CONTROL & (MMA7455_GLVL1|MMA7455_GLVL0));   //selecciona los bit de GLVL
  if(c1 == (MMA7455_GLVL0 | MMA7455_GLVL1)){  // si es 8g
    c2 = 16;
  }else if(c1 == MMA7455_GLVL1){ //si es 4g
    c2 = 32;
  }else if(c1 == MMA7455_GLVL0){ //si es 2g
    c2 = 64;
  }else{
    c2 = 64;  //por defecto 2g
  }

  error = xyz_MMA(&x, &y, &z);    //se leen los valores de los ejes
  if (error != 0)
    return (error);
  
  //calcula offset para el ajuste a 0 de los ejes
  xyz.value.x = 2 * -x;        
  xyz.value.y = 2 * -y;
  xyz.value.z = 2 * -(z-c2);

  error = write_MMA(MMA7455_XOFFL, (unsigned int8 *) &xyz, 6);  //se envia el offset al accelerometro
  if (error != 0)
    return (error);

  delay_ms(200);
  
  error = xyz_MMA(&x, &y, &z);
  if (error != 0)
    return (error);

  //realiza nuevamente la operacion para el reajuste
  xyz.value.x = 2 * -x;        
  xyz.value.y = 2 * -y;
  xyz.value.z = 2 * -(z-c2);

  error = write_MMA(MMA7455_XOFFL, (unsigned int8 *) &xyz, 6);
  if (error != 0)
    return (error);

  //leen los datos enviados al accelerometro para almacenarlos en el microcontrolador
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
  int error = 0;
  unsigned int8 c;

  do{
    error = read_MMA(MMA7455_STATUS,&c);
  }while(!bit_test(c, 0) && error == 0);
  
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
  buffer: buffer de almacenamiento para los datos leidos
  size: cantidad de bytes a leer desde el esclavo

returna:
  int: 0 no error, 
      1 error de inicio de transmision, 
      2 error de peticion de datos,
      -11 error en la lectura desde el MMA 
*/
int read_MMA(char address, unsigned int8 *value){
  address = ((address & 0x3f)<<1);
  
  #ifdef testmma
  spi_xfer(address);
  delay_ms(20);
  *value = spi_xfer(0);
  delay_ms(20);
  #else
  spi_xfer(address);
  *value = spi_xfer(0);
  #endif
  return (0);
}

int read_MMA(char start_address, unsigned int8 *buffer,unsigned int size){
  unsigned int i = 0;  
  do{
    read_MMA((start_address+i), &buffer[i]);
    #ifdef testmma
    printf(usb_cdc_putc,"\n\rR:%x dt:%x",(start_address+i), buffer[i]);
    #endif
    i++;
  }while(i < size);  
  return (0);   //retorna 0 si succed
} //leer datos desde el mma


int write_MMA(char address, int8 *value){
    address = (((address & 0x3f)<<1) | 0x80);
    #ifdef testmma
    spi_xfer(address);
    delay_ms(20);
    spi_xfer(*value);
    delay_ms(20);
    #else
    spi_xfer(address);
    spi_xfer(*value);
    #endif
  return 0;
}


int write_MMA(char start_address, int8 *pData, unsigned int size){
  unsigned int i = 0;
  do{
    #ifdef testmma
    write_MMA((start_address+i),&pData[i]);
    printf(usb_cdc_putc,"\n\rW:%x dt:%x",(start_address+i),pData[i]);
    #else
    write_MMA((start_address+i),&pData[i]);
    #endif
    i++;
  }while(i < size);
  return 0;
} //escribir datos desde el mma
