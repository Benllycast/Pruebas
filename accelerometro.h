
#ifndef ACCELEROMETRO_H

#define ACCELEROMETRO_H

#define MMA7455_XOUTL 0x00      // Read only, Output Value X LSB
#define MMA7455_XOUTH 0x01      // Read only, Output Value X MSB
#define MMA7455_YOUTL 0x02      // Read only, Output Value Y LSB
#define MMA7455_YOUTH 0x03      // Read only, Output Value Y MSB
#define MMA7455_ZOUTL 0x04      // Read only, Output Value Z LSB
#define MMA7455_ZOUTH 0x05      // Read only, Output Value Z MSB
#define MMA7455_XOUT8 0x06      // Read only, Output Value X 8 bits
#define MMA7455_YOUT8 0x07      // Read only, Output Value Y 8 bits
#define MMA7455_ZOUT8 0x08      // Read only, Output Value Z 8 bits
#define MMA7455_STATUS 0x09     // Read only, Status Register
#define MMA7455_DETSRC 0x0A     // Read only, Detection Source Register
#define MMA7455_TOUT 0x0B       // Temperature Output Value (Optional)
#define MMA7455_RESERVED1 0x0C  // Reserved
#define MMA7455_I2CAD 0x0D      // Read/Write, I2C Device Address
#define MMA7455_USRINF 0x0E     // Read only, User Information (Optional)
#define MMA7455_WHOAMI 0x0F     // Read only, "Who am I" value (Optional)
#define MMA7455_XOFFL 0x10      // Read/Write, Offset Drift X LSB
#define MMA7455_XOFFH 0x11      // Read/Write, Offset Drift X MSB
#define MMA7455_YOFFL 0x12      // Read/Write, Offset Drift Y LSB
#define MMA7455_YOFFH 0x13      // Read/Write, Offset Drift Y MSB
#define MMA7455_ZOFFL 0x14      // Read/Write, Offset Drift Z LSB
#define MMA7455_ZOFFH 0x15      // Read/Write, Offset Drift Z MSB
#define MMA7455_MCTL 0x16       // Read/Write, Mode Control Register
#define MMA7455_INTRST 0x17     // Read/Write, Interrupt Latch Reset
#define MMA7455_CTL1 0x18       // Read/Write, Control 1 Register
#define MMA7455_CTL2 0x19       // Read/Write, Control 2 Register
#define MMA7455_LDTH 0x1A       // Read/Write, Level Detection Threshold Limit Value
#define MMA7455_PDTH 0x1B       // Read/Write, Pulse Detection Threshold Limit Value
#define MMA7455_PD 0x1C         // Read/Write, Pulse Duration Value
#define MMA7455_LT 0x1D         // Read/Write, Latency Time Value (between pulses)
#define MMA7455_TW 0x1E         // Read/Write, Time Window for Second Pulse Value
#define MMA7455_RESERVED2 0x1F  // Reserved

// Defines for the bits, to be able to change 
// between bit number and binary definition.
// By using the bit number, programming the MMA7455 
// is like programming an AVR microcontroller.
// But instead of using "(1<<X)", or "_BV(X)", 
// the Arduino "bit(X)" is used.
#define MMA7455_D0 0
#define MMA7455_D1 1
#define MMA7455_D2 2
#define MMA7455_D3 3
#define MMA7455_D4 4
#define MMA7455_D5 5
#define MMA7455_D6 6
#define MMA7455_D7 7

// Status Register
#define MMA7455_DRDY MMA7455_D0
#define MMA7455_DOVR MMA7455_D1
#define MMA7455_PERR MMA7455_D2

// Mode Control Register
#define MMA7455_MODE0 MMA7455_D0
#define MMA7455_MODE1 MMA7455_D1
#define MMA7455_GLVL0 MMA7455_D2
#define MMA7455_GLVL1 MMA7455_D3
#define MMA7455_STON MMA7455_D4
#define MMA7455_SPI3W MMA7455_D5
#define MMA7455_DRPD MMA7455_D6

// Control 1 Register
#define MMA7455_INTPIN MMA7455_D0
#define MMA7455_INTREG0 MMA7455_D1
#define MMA7455_INTREG1 MMA7455_D2
#define MMA7455_XDA MMA7455_D3
#define MMA7455_YDA MMA7455_D4
#define MMA7455_ZDA MMA7455_D5
#define MMA7455_THOPT MMA7455_D6
#define MMA7455_DFBW MMA7455_D7

// Control 2 Register
#define MMA7455_LDPL MMA7455_D0
#define MMA7455_PDPL MMA7455_D1
#define MMA7455_DRVO MMA7455_D2

// Interrupt Latch Reset Register
#define MMA7455_CLR_INT1 MMA7455_D0
#define MMA7455_CLR_INT2 MMA7455_D1

// Detection Source Register
#define MMA7455_INT1 MMA7455_D0
#define MMA7455_INT2 MMA7455_D1
#define MMA7455_PDZ MMA7455_D2
#define MMA7455_PDY MMA7455_D3
#define MMA7455_PDX MMA7455_D4
#define MMA7455_LDZ MMA7455_D5
#define MMA7455_LDY MMA7455_D6
#define MMA7455_LDX MMA7455_D7

// I2C Device Address Register
#define MMA7455_I2CDIS MMA7455_D7



// Default I2C address for the MMA7455
#define MMA7455_I2C_ADDRESS 0x1D
#define MMA7455_I2C_ADDRESS_READ 0X3B
#define MMA7455_I2C_ADDRESS_WRITE 0X3A


// When using an union for the registers and
// the axis values, the byte order of the accelerometer
// should match the byte order of the compiler and AVR chip.
// Both have the lower byte at the lower address,
// so they match.
// This union is only used by the low level functions.
typedef union xyz_union
{
  struct
  {
    unsigned int8 x_lsb;
    unsigned int8 x_msb;
    unsigned int8 y_lsb;
    unsigned int8 y_msb;
    unsigned int8 z_lsb;
    unsigned int8 z_msb;
  } reg;
  struct
  {
    int x;
    int y;
    int z;
  } value;
};

volatile struct CONFIG_MMA7455
{
   /* data */
   struct {
      unsigned reserved : 1;
      unsigned DRPD : 1;      //data ready status pin ouput INT1/DRDY pin: 0 = enable , 1 = disable.
      unsigned SPI3W : 1;     //modo SPI 4hilos = 0, 3 hilos=1 .
      unsigned STON : 1;      //selft test enable=1, disable=0 .
      unsigned GLVL : 2;      // 00 = 8g, 10 = 4g, 01 = 2g.
      unsigned MODE : 2;      // 00 = stanby_mode, 01 = measure mode, 10 = level detection, 11 = pulse detection.
   } _MODE_CONTROL_REGISTER;
   /*
   struct {
      unsigned reserved : 6;
      unsigned CLRINT2 : 1;//limpia INT1 bit in $0A
      unsigned CLRINT1 : 1;//limpia INT2 bit in $0A
   } _INTERRUP_LACHT_RESET;
   */
   struct {
      unsigned DFBW: 1;     //filtro digital 0 = 62 Hz rate,1 = 125 Hz rate.
      unsigned THOPT: 1;    //tipo de valor en el umbral de detecccion (LD only): absoluto = 0, valor Integer con signo = 1.
      unsigned ZDA: 1;      //deteccion en eje Z: enable = 0, disable = 1.
      unsigned YDA: 1;      //deteccion en eje X: enable = 0, disable = 1.
      unsigned XDA: 1;      //deteccion en eje Y: enable = 0, disable = 1.
      unsigned INTRG: 2;    //configuracion de INT1, INT2 para LD o PD ver tabla1.
      unsigned INTPIN: 1;   //configuraciones pines de interrupciones con bit de interrupciones (ver tabla2).
   } _CONTROL_1;

   struct {
      unsigned reserved: 5;      
      unsigned LDPL: 1;     //tipo de deteccion en LD: 0 = MOV, 1 = CL
      unsigned PDPL: 1;     //tipo de deteccion en PD: 0 = MOV, 1 = CL
      unsigned DRVO: 1;
   } _CONTROL_2;

   unsigned int8 _XOFFL;
   unsigned int8 _XOFFH;
   unsigned int8 _YOFFL;
   unsigned int8 _YOFFH;
   unsigned int8 _ZOFFL;
   unsigned int8 _ZOFFH;/*,//no implemntado
   char _LEVEL_DETECTION_THREHOLDS;
   char _PULSE_DETECTION_THREHOLDS;
   char _PULSE_DURATION;
   char _LANTENCY_TIME;
   char _TIME_WINDOWS;
   */
} config_acelerometro = {
   0b01001001,//0:reserved
   0b00000000,
   0b00000000,
   0x00,
   0x00,
   0x00,
   0x00,
   0x00/*,//no implmentado
   0x01,
   0x01,
   0x00,
   0x00,
   0x00*/
};//variable para la configuracion del accelerometro

#use i2c(master, sda=PIN_C4, scl=PIN_C3, FORCE_HW, RESTART_WDT)                    //directiva de compilador par ale uso del bus I2C del microcontrolador
//configuracion incial del MMA7455
int config_MMA();

//cambio de configuracion del MMA7455
void set_config(*CONFIG_MMA7455);

//Lectura de datosd desde el MMA7455
int read_MMA(unsigned char address, int8 *value);
int read_MMA(unsigned char start_address, int8 *buffer,int size);

//Escritura de datos en el MMA7455
int write_MMA(unsigned char address, unsigned int8 *value);
int write_MMA(unsigned char start_address, unsigned int8 *pData, int size);
/*
int1 begin_transmision(unsigned int8 address);
int1 end_transmision(bool);
int8 requesFROM(unsigned int8 address);
*/
#endif

/*
DEFINICIONES:
MOV = deteccion de movimiento
CL = deteccion de caida libre

tabla 1: configuracion de INTRG
------------------------
INT2TRG      INT1   INT2
00         LD       PD
01         PD       LD
10         sPD     dPD
------------------------

tabla2: configuracion de pines de interrupciones
-------------------
INTPIN
0       INT1bit  --> INT1pin, INT2bit  --> INT2pin
1       INT1bit  --> INT2pin, INT2bit  --> INT1pin
-------------------
*/
