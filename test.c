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
	printf(usb_cdc_putc,"hola mundo");
	delay_ms(500);
	return;
}
#endif
