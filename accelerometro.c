struct CONFIG_ACCELEROMETRO
{
	/* data */
	struct {
		unsigned reserved : 1;
		unsigned DRPD : 1;
		unsigned SPI3W : 1;
		unsigned STON : 1;
		unsigned GLVL : 2; 	/* 00 = 8g, 01 = 4g, 01 = 2g*/
		unsigned MODE : 2;	/* 00 = stanby_mode, 01 = measure mode, 10 = level detection, 11 = pulse detection*/
	} _MODE_CONTROL_REGISTER;
	
	struct {
		unsigned reserved : 6;
		unsigned CLRINT2 : 1;
		unsigned CLRINT1 : 1;
	} _INTERRUP_LACHT_RESET;
	
	struct {
		unsigned DFBW: 1;
		unsigned THOPT: 1;
		unsigned ZDA: 1;
		unsigned YDA: 1;
		unsigned XDA: 1;
		unsigned INTRG: 2;
		unsigned INTPIN: 1;  
	} _CONTROL_1;

	struct {
		unsigned reserved: 4;
		unsigned DRVO: 1;
		unsigned PDPL: 1;
		unsigned LDOL: 1;
	} _CONTROL_2;

	char _LEVEL_DETECTION_THREHOLDS;
	char _PULSE_DETECTION_THREHOLDS;
	char _PULSE_DUATION;
	char _LANTENCY_TIME;
	char _TIME_WINDOWS;
};