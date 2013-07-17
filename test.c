// ====== test de captura frecuencia =====
#ifdef CAPTURA_FRECUENCIA_H
#ifdef SIMULACION
void test_ccp(){
	int32 buffer = 0;
	CP_leer_ccp(CCP_CANAL_1, &buffer);
   	printf("\n\rC1: %Lu", buffer);
   	CP_leer_ccp(CCP_CANAL_2, &buffer);
   	printf(" C2: %Lu", buffer);
   	delay_ms(500);
}
#else
//test de prueba real
#endif
#endif


// ====== test de Conversor Analogo Digital=====
#ifdef ANALOGO_DIGITAL_H
#ifdef SIMULACION
void test_ADC(void){
	int16 ejex = 0, ejey = 0, ejez = 0, vel = 0, rev = 0;
	AD_leer_canal(acc_eje_x, &ejex);
	AD_leer_canal(acc_eje_y, &ejey);
	AD_leer_canal(acc_eje_z, &ejez);
	AD_leer_canal(velocidad, &vel);
	AD_leer_canal(revolucion, &rev);
	printf("\n\rx:%LX\t\ty:%LX\t\tz:%LX\t\tv:%LX\t\tr:%LX",ejex, ejey, ejez, vel, rev);
	delay_ms(333);
	return;
}

#else
void test_ADC(void){
	int16 ejex = 0, ejey = 0, ejez = 0, vel = 0, rev = 0;
	if(_debug_usb()){
		//place your code here
		AD_leer_canal(acc_eje_x, &ejex);
		AD_leer_canal(acc_eje_y, &ejey);
		AD_leer_canal(acc_eje_z, &ejez);
		AD_leer_canal(velocidad, &vel);
		AD_leer_canal(revolucion, &rev);
		printf(usb_cdc_putc_fast,"\n\rx:%LX\t\ty:%LX\t\tz:%LX\t\tv:%LX\t\tr:%LX",ejex, ejey, ejez, vel, rev);
		delay_ms(333);
	}
	return;
}
#endif
#endif

#ifdef COMUNICACION_H
void test_comunicacion(void){
	printf(usb_cdc_putc,"\n\rhola mundo");
	delay_ms(500);
	return;
}
#endif

#ifdef MEMORIA_H
#define MAX 32
void test_memoria(void){
	char testfile[] = "test.txt";
	char mensaje[] = "0123456789";
	char buffer[MAX];
	unsigned int8 aux = 0, size = 0, myerror = 0;
	unsigned int32 tamano = 0;
	char op = 0;
	
   size = strlen(mensaje);
   memset(buffer, 0, MAX);
   while(_debug_usb()){
			aux++;
			printf(usb_cdc_putc_fast,"\n\rE %d", myerror);
			memset(buffer, 0, MAX);
			myerror = 0;
			tamano = 0;
			printf(usb_cdc_putc,"\n\rtest %u", aux);
			while(!(op = usb_cdc_getc()));
			switch(op){
				case '0':
					MEMORIA_reset();
					break;
				case '1':
					myerror = MEMORIA_init_hw();
					break;
				case '2':
					myerror = MEMORIA_init();
					break;
				case 'i':
					MEMORIA_getinfo();
					break;
				case 'e':
					myerror = MEMORIA_open(testfile, FILE_WR);
					break;
				case 'w':
					myerror = MEMORIA_write(size);
					break;
				case 's':
					myerror = MEMORIA_set_data(mensaje,size);
					break;
				case 'l':
					myerror = MEMORIA_open(testfile, FILE_RD);
					break;
				case 'r':
					tamano = MEMORIA_read(size);
					printf(usb_cdc_putc_fast,"\n\rt%Lu", tamano);
					break;
				case 'g':
					myerror = MEMORIA_get_data(buffer);
					printf(usb_cdc_putc_fast,"\n\r%d %s", myerror, buffer);
					break;
				case 'c':
					myerror = MEMORIA_cancel();
					break;
				case 'x':
					myerror = MEMORIA_close();
					break;
				default:
					printf(usb_cdc_putc_fast,"\n\r DESCONECTAR...");
					delay_ms(5000);
			}
			  	
	}
	return;
}
#endif
