#ifndef ANALOGO_DIGITAL_H
#define ANALOGO_DIGITAL_H
int AD_init_adc();
int AD_leer_canal(unsigned int canal, int16 *buffer);
#endif
