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
   CONF_CANAL_ACTIVO = ds1307_read_nvram_byte(CONF_DIR_CANAl);
   //CONF_CANAL_ACTIVO = 0b00000000;
    #endif
    
   data.dia = data.mes = data.anio = 0;
   data.hor = data.min = data.seg = 0;
   data.value = data.sensor = data.no_data = 0;
   data.crc = 0;
   
}


int sensor_activo(int sensor){
    switch(sensor){
        case CCP_REV:
            return (bit_test(CONF_CANAL_ACTIVO, CONF_CCP_REV_BIT));
        case CCP_VEL:
            return (bit_test(CONF_CANAL_ACTIVO, CONF_CCP_VEL_BIT));
        case AD_VEL:
            return (bit_test(CONF_CANAL_ACTIVO, CONF_AD_VEL_BIT));
        case AD_REV:
            return (bit_test(CONF_CANAL_ACTIVO, CONF_AD_REV_BIT));
        default:
            return (0);
    }
}


void modo_configuracion(){
    //printf( cout "\n\rmodo configuracion\n\r" );
   while(_debug_usb()){
            while(!(data.sensor = cin()));    //cambiar con usb
            switch(data.sensor){
                case CONF_SET_FECHA:
                    putc(CONF_SET_FECHA);
                    //printf( cout "\n\rFECHA: %c", data.sensor);
                    data.dia = cin();
                    data.mes = cin();
                    data.anio = cin();
                    data.no_data = cin();
                    data.hor = cin();
                    data.min = cin();
                    data.seg = cin();
                    ds1307_set_date_time(data.dia, data.mes, data.anio, data.no_data,data.hor, data.min, data.seg);
                    /*printf( cout "\n\r%u/%u/%u(%u:%u:%u) S:%u N:%u V:%Lu",
                                data.dia, data.mes, data.anio,
                                data.hor, data.min, data.seg,
                                data.sensor, data.no_data, data.value);*/
                    putc(CONF_ACK);
                    break;
                case CONF_GET_FECHA:
                    putc(CONF_GET_FECHA);
                    ds1307_get_date(data.dia, data.mes, data.anio, data.no_data);
                    ds1307_get_time(data.hor, data.min, data.seg);
                    putc(data.dia);
                    putc(data.mes);
                    putc(data.anio);
                    putc(data.no_data);
                    putc(data.hor);
                    putc(data.min);
                    putc(data.seg);
                    putc(CONF_ACK);
                    break;
                case CONF_SET_DATO:
                    putc(CONF_SET_DATO);
                    data.sensor = cin(); //lee la direccion
                    data.no_data = cin(); // lee el valor
                    ds1307_write_nvram_byte(data.sensor, data.no_data);
                    putc(CONF_ACK);
                    break;
                case CONF_GET_DATO:
                    putc(CONF_GET_DATO);
                    data.sensor = cin(); //lee la direccion
                    data.no_data = ds1307_read_nvram_byte(data.sensor); //lee el valor de la ram
                    putc(data.no_data);
                    putc(CONF_ACK);
                    break;
                case CONF_SET_CANAL:
                    putc(CONF_SET_CANAL);
                    data.no_data = cin();
                    ds1307_write_nvram_byte(CONF_DIR_CANAL, data.no_data);
                    //printf( cout "\n\rCANAL: %u", data.no_data);
                    putc(CONF_ACK);
                    break;
                case CONF_GET_CANAL:
                    putc(CONF_GET_CANAL);
                    data.no_data = ds1307_read_nvram_byte(CONF_DIR_CANAL);
                    putc(data.no_data);
                    putc(CONF_ACK);
                    break;
                default:
                    putc(CONF_NOACK);
            }
            if(data.sensor == CONF_SALIR) {printf( cout "\n\rDesconectar...");break;}
    }
    return;
}

#endif    //CONFIGURACION_H
