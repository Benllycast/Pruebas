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
	printf( cout "\n\rmodo configuracion\n\r" );
   while(_debug_usb()){
			while(!(data.sensor = cin()));	//cambiar con usb
			switch(data.sensor){                    
				case CONF_FECHA:
					putc(CONF_ACK);
					printf( cout "\n\rFECHA: %c", data.sensor);
					data.dia = cin();
					data.mes = cin();
					data.anio = cin();
					data.no_data = cin();
					data.hor = cin();
					data.min = cin();
					data.seg = cin();
					ds1307_set_date_time(data.dia, data.mes, data.anio, data.no_data,data.hor, data.min, data.seg);
					printf( cout "\n\r%u/%u/%u(%u:%u:%u) S:%u N:%u V:%Lu",
								data.dia, data.mes, data.anio,
								data.hor, data.min, data.seg,
								data.sensor, data.no_data, data.value);
					break;
				case CONF_DATO:
					putc(CONF_ACK);
					printf( cout "\n\rDATO: %c", data.sensor);
					break;
				case CONF_CANAL:
					putc(CONF_ACK);
					data.no_data = cin();
					ds1307_write_nvram_byte(CONF_ADD_CANAL, data.no_data);
					printf( cout "\n\rCANAL: %u", data.no_data);
					break;
				default:
					putc(CONF_NOACK);
			}
			if(data.sensor == CONF_SALIR) {printf( cout "\n\rDesconectar...");break;}
	}
	return;
}

#endif	//CONFIGURACION_H
