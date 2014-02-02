#ifdef CONFIGURACION_H

void iniciar_perifericos(){
	
	#ifdef COMUNICACION_H
	myerror = COM_init();
	//printf("\n\rusb E%d", myerror);
	#endif
	
	#ifdef MEMORIA_H
	MEMORIA_reset();
   myerror = MEMORIA_init_hw();
   myerror = MEMORIA_init();
   //printf("\n\rmem E%d", myerror);
	#endif
	
	#ifdef ANALOGO_DIGITAL_H
	myerror = AD_init_adc();
	#endif
	
	#ifdef CAPTURA_FRECUENCIA_H
	myerror = CP_init_ccp();
	#endif
	
	#ifdef DS1307_H
	ds1307_init(DS1307_OUT_ON_DISABLED_HIHG | DS1307_OUT_ENABLED | DS1307_OUT_1_HZ);
   //ds1307_set_date_time(0x0d, 0x01, 0x0d, 0x00, 0x0a, 0x2a, 0x00);
	#endif
	
	data.dia = data.mes = data.anio = 0;
   data.hor = data.min = data.seg = 0;
   data.value = data.sensor = data.no_data = 0;
   
}

#ifdef CAPTURA_FRECUENCIA_H
int sensor_activo(int sensor){
	if(sensor == CCP_REV){
		return (1);
	}else if(sensor == CCP_VEL){
		return (1);
	}else{
		return (0);
	}
}
#endif //CAPTURA_FRECUENCIA_H

void modo_configuracion(){
	printf("modo configuracion\n\r");
	
}

#endif	//CONFIGURACION_H
