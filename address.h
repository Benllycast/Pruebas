/*direcciones del accelerometro mma7455*/
#ifdef testmma
	#define xoutl 0x00
	#define xouth 0x01//registros de medida de x a 10bit
	#define youtl 0x02
	#define youth 0x03//registros de medida de y a 10bit
	#define zoutl 0x04
	#define zouth 0x05//registros de medida de z a 10bit
	#define xout8 0x06
	#define yout8 0x07
	#define zout8 0x08//registros de medida a 8bit
	#define status 0x09
	#define detsrc 0x0A
	#define tout 0x0B
	#define reserved 0x0C
	#define i2cad 0x0D
	#define usrinf 0x0E
	#define whoami 0x0F
	#define xoffl 0x10
	#define xoffh 0x11
	#define yoffl 0x12
	#define yoffh 0x13
	#define zoffl 0x14
	#define zoffh 0x15
	#define mctl 0x16	/*selector gravedad (bit[3,2]) 8g = 00, 2g = 01, 4g = 10
							selector mode (bit[1,0]) stanby = 00, measurement= 01, level_detect= 10, pulse_detec= 11
	                		SPI mode (bit[4]) 4 wire = 0, 3 wire = 1
	                		data output pin (bit[5]) enable = 1 , disable = 0
	                	*/
	#define intrst 0x17
	#define ctl1 0x18
	#define ctl2 0x19
	#define ldth 0x1A
	#define pdth 0x1B
	#define pw 0x1C 
	#define lt 0x1D
	#define tw 0x1E
	#define reserved2 0x1F
#endif