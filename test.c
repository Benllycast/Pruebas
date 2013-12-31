// ====== test de captura frecuencia =====
#ifdef CAPTURA_FRECUENCIA_H
	void test_ccp(){
		int32 buffer = 0;
		int done = 0;
		CP_activar_captura(CCP_CANAL_1);
		done = CP_done();
		while(!done){
			//printf(usb_cdc_putc_fast,"\n\rEsperando...");
			done = CP_done();
		}
		buffer = CP_obtener_resultado();
		CP_desativar_captura();
		#ifdef SIMULACION
		printf("\n\rC1: %Lu", buffer);
		#else
		printf(usb_cdc_putc_fast,"\n\rC1: %Lu", buffer);
		#endif      
	}
#endif


// ====== test de Conversor Analogo Digital=====
#ifdef ANALOGO_DIGITAL_H
	void test_ADC(void){
		int16 ejex = 0, ejey = 0, ejez = 0, vel = 0, rev = 0;
		AD_leer_canal(ACC_x, &ejex);
		AD_leer_canal(ACC_y, &ejey);
		AD_leer_canal(ACC_z, &ejez);
		AD_leer_canal(AD_VEL, &vel);
		AD_leer_canal(AD_REV, &rev);
		#ifdef SIMULACION
		printf("\n\rx:%LX\t\ty:%LX\t\tz:%LX\t\tv:%LX\t\tr:%LX",ejex, ejey, ejez, vel, rev);
		#else
		printf(usb_cdc_putc_fast,"\n\rx:%LX\t\ty:%LX\t\tz:%LX\t\tv:%LX\t\tr:%LX",ejex, ejey, ejez, vel, rev);
		#endif
		delay_ms(333);
		return;
	}
#endif

#ifdef COMUNICACION_H
void test_comunicacion(void){
	printf(usb_cdc_putc,"\n\rhola mundo");
	return;
}
#endif

#ifdef MEMORIA_H
#define MAX 32

void test_memoria(void){
	//char testfile[] = "test.txt";  //el nombre es mejor dejarlo sin punto                              
	char testfile[] = "prueba";
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

#ifdef DS1307_H
void test_reloj(){
	byte dia = 0, mes = 0, anio = 0;
   byte hor = 0, min = 0, seg = 0;
	ds1307_get_date(dia, mes, anio, DS_vic);
	ds1307_get_time(hor, min, seg);
	#ifdef SIMULACION
	printf("\n\rH %d/%d/%d %d:%d:%d",dia, mes, anio, hor, min, seg);
	#else
	printf(usb_cdc_putc_fast,"\n\rH %d/%d/%d %d:%d:%d",dia, mes, anio, hor, min, seg);
	#endif
}
#endif
