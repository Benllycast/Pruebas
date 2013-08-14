//#include "Nucleo.h"
#include "ds1307.h"



char days_of_week[7][11]={"Lunes\0","Martes\0","Miércoles\0","Jueves\0","Viernes\0","Sábado\0","Domingo\0"};

byte ds1307_bin2bcd(byte binary_value);
byte ds1307_bcd2bin(byte bcd_value);

void ds1307_init(int val){

  byte seconds = 0;

#ifndef USE_INTERRUPTS
  disable_interrupts(global);
#endif

  i2c_start();
  i2c_write(0xD0);
  i2c_write(0x00);
  i2c_start();
  i2c_write(0xD1);
  seconds = ds1307_bcd2bin(i2c_read(0));
  i2c_stop();
  seconds &= 0x7F;

  delay_us(3);

  i2c_start();
  i2c_write(0xD0);
  i2c_write(0x00);
  i2c_write(ds1307_bin2bcd(seconds));
  i2c_start();
  i2c_write(0xD0);
  i2c_write(0x07);
  i2c_write(val);
  i2c_stop();

#ifndef USE_INTERRUPTS
  enable_interrupts(global);
#endif

}

void ds1307_set_date_time(byte day, byte mth, byte year, byte dow, byte hr, byte min, byte sec){

#ifndef USE_INTERRUPTS
  disable_interrupts(global);
#endif

  sec &= 0x7F;
  hr &= 0x3F;

  i2c_start();
  i2c_write(0xD0);
  i2c_write(0x00);
  i2c_write(ds1307_bin2bcd(sec));
  i2c_write(ds1307_bin2bcd(min));
  i2c_write(ds1307_bin2bcd(hr));
  i2c_write(ds1307_bin2bcd(dow));
  i2c_write(ds1307_bin2bcd(day));
  i2c_write(ds1307_bin2bcd(mth));
  i2c_write(ds1307_bin2bcd(year));
  i2c_stop();

#ifndef USE_INTERRUPTS
  enable_interrupts(global);
#endif

}

void ds1307_get_date(byte &day, byte &mth, byte &year, byte &dow){

#ifndef USE_INTERRUPTS
  disable_interrupts(global);
#endif

  i2c_start();
  i2c_write(0xD0);
  i2c_write(0x03);
  i2c_start();
  i2c_write(0xD1);
  dow = ds1307_bcd2bin(i2c_read() & 0x7f);
  day = ds1307_bcd2bin(i2c_read() & 0x3f);
  mth = ds1307_bcd2bin(i2c_read() & 0x1f);
  year = ds1307_bcd2bin(i2c_read(0));
  i2c_stop();

#ifndef USE_INTERRUPTS
  enable_interrupts(global);
#endif

}

void ds1307_get_time(byte &hr, byte &min, byte &sec){

#ifndef USE_INTERRUPTS
  disable_interrupts(global);
#endif

  i2c_start();
  i2c_write(0xD0);
  i2c_write(0x00);
  i2c_start();
  i2c_write(0xD1);
  sec = ds1307_bcd2bin(i2c_read() & 0x7f);
  min = ds1307_bcd2bin(i2c_read() & 0x7f);
  hr = ds1307_bcd2bin(i2c_read(0) & 0x3f);
  i2c_stop();

#ifndef USE_INTERRUPTS
  enable_interrupts(global);
#endif

}


char ds1307_read_nvram_byte(char addr){

  char retval;

#ifndef USE_INTERRUPTS
  disable_interrupts(global);
#endif

  i2c_start();
  i2c_write(0xD0);
  i2c_write(addr);

  i2c_start();
  i2c_write(0xD1);
  retval = i2c_read(0);
  i2c_stop();

return(retval);

#ifndef USE_INTERRUPTS
  enable_interrupts(global);
#endif

}

void ds1307_write_nvram_byte(char addr, char value){

#ifndef USE_INTERRUPTS
  disable_interrupts(global);
#endif

  i2c_start();
  i2c_write(0xD0);
  i2c_write(addr);
  i2c_write(value);
  i2c_stop();

#ifndef USE_INTERRUPTS
  enable_interrupts(global);
#endif

}

void ds1307_get_day_of_week(char* ptr){

  byte lday;
  byte lmonth;
  byte lyr;
  byte ldow;
  ds1307_get_date(lday,lmonth,lyr,ldow);
  sprintf(ptr,"%s",days_of_week[ldow]);
}

///////////////////////////////////////////////////////////////////////////////

byte ds1307_bin2bcd(byte binary_value){

  byte temp;
  byte retval;

  temp = binary_value;
  retval = 0;
  while(1){
    if(temp >= 10){
      temp -= 10;
      retval += 0x10;
    }else{
      retval += temp;
      break;
    }
  }
  return(retval);
}

byte ds1307_bcd2bin(byte bcd_value){

  byte temp;

  temp = bcd_value;
  temp >>= 1;
  temp &= 0x78;
  return(temp + (temp >> 2) + (bcd_value & 0x0f));
}

///////////////////////////////////////////////////////////////////////////////
 
//#use i2c(master, sda=RTC_SDA, scl=RTC_SCL)
/*
char days_of_week[7][11]={"Lunes\0","Martes\0","Miércoles\0","Jueves\0","Viernes\0","Sábado\0","Domingo\0"};

byte ds1307_bin2bcd(byte binary_value);
byte ds1307_bcd2bin(byte bcd_value);

void ds1307_init(int val){

   byte seconds = 0;

#ifndef USE_INTERRUPTS
   disable_interrupts(global);
#endif

   i2c_start();
   i2c_write(0xD0);
   i2c_write(0x00);
   i2c_start();
   i2c_write(0xD1);
   seconds = ds1307_bcd2bin(i2c_read(0));
   i2c_stop();
   seconds &= 0x7F;

   delay_us(3);

   i2c_start();
   i2c_write(0xD0);
   i2c_write(0x00);
   i2c_write(ds1307_bin2bcd(seconds));
   i2c_start();
   i2c_write(0xD0);
   i2c_write(0x07);
   i2c_write(val);
   i2c_stop();

#ifndef USE_INTERRUPTS
   enable_interrupts(global);
#endif

}

void ds1307_set_date_time(byte day, byte mth, byte year, byte dow, byte hr, byte min, byte sec){

#ifndef USE_INTERRUPTS
   disable_interrupts(global);
#endif

  sec &= 0x7F;
  hr &= 0x3F;

  i2c_start();
  i2c_write(0xD0);
  i2c_write(0x00);
  i2c_write(ds1307_bin2bcd(sec));
  i2c_write(ds1307_bin2bcd(min));
  i2c_write(ds1307_bin2bcd(hr));
  i2c_write(ds1307_bin2bcd(dow));
  i2c_write(ds1307_bin2bcd(day));
  i2c_write(ds1307_bin2bcd(mth));
  i2c_write(ds1307_bin2bcd(year));
  i2c_stop();

#ifndef USE_INTERRUPTS
   enable_interrupts(global);
#endif

}

void ds1307_get_date(byte &day, byte &mth, byte &year, byte &dow){

#ifndef USE_INTERRUPTS
   disable_interrupts(global);
#endif

  i2c_start();
  i2c_write(0xD0);
  i2c_write(0x03);
  i2c_start();
  i2c_write(0xD1);
  dow  = ds1307_bcd2bin(i2c_read() & 0x7f);
  day  = ds1307_bcd2bin(i2c_read() & 0x3f);
  mth  = ds1307_bcd2bin(i2c_read() & 0x1f);
  year = ds1307_bcd2bin(i2c_read(0));
  i2c_stop();

#ifndef USE_INTERRUPTS
   enable_interrupts(global);
#endif

}

void ds1307_get_time(byte &hr, byte &min, byte &sec){

#ifndef USE_INTERRUPTS
   disable_interrupts(global);
#endif

  i2c_start();
  i2c_write(0xD0);
  i2c_write(0x00);
  i2c_start();
  i2c_write(0xD1);
  sec = ds1307_bcd2bin(i2c_read() & 0x7f);
  min = ds1307_bcd2bin(i2c_read() & 0x7f);
  hr  = ds1307_bcd2bin(i2c_read(0) & 0x3f);
  i2c_stop();

#ifndef USE_INTERRUPTS
   enable_interrupts(global);
#endif

}


char ds1307_read_nvram_byte(char addr){

   char retval;

#ifndef USE_INTERRUPTS
   disable_interrupts(global);
#endif

   i2c_start();
   i2c_write(0xD0);
   i2c_write(addr);

   i2c_start();
   i2c_write(0xD1);
   retval = i2c_read(0);
   i2c_stop();

   return(retval);

#ifndef USE_INTERRUPTS
   enable_interrupts(global);
#endif

}

void ds1307_write_nvram_byte(char addr, char value){

#ifndef USE_INTERRUPTS
   disable_interrupts(global);
#endif

   i2c_start();
   i2c_write(0xD0);
   i2c_write(addr);
   i2c_write(value);
   i2c_stop();

#ifndef USE_INTERRUPTS
   enable_interrupts(global);
#endif

}

void ds1307_get_day_of_week(char* ptr){

   byte lday;
   byte lmonth;
   byte lyr;
   byte ldow;
   ds1307_get_date(lday,lmonth,lyr,ldow);
   sprintf(ptr,"%s",days_of_week[ldow]);
}

///////////////////////////////////////////////////////////////////////////////

byte ds1307_bin2bcd(byte binary_value){

  byte temp;
  byte retval;

  temp = binary_value;
  retval = 0;
  while(1){
    if(temp >= 10){
      temp -= 10;
      retval += 0x10;
    }else{
      retval += temp;
      break;
    }
  }
  return(retval);
}

byte ds1307_bcd2bin(byte bcd_value){

  byte temp;

  temp = bcd_value;
  temp >>= 1;
  temp &= 0x78;
  return(temp + (temp >> 2) + (bcd_value & 0x0f));
}
*/
///////////////////////////////////////////////////////////////////////////////
