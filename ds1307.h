///////////////////////////////////////////////////////////////////////////////////////
/// ds1307.c                                                                        ///
/// Driver for Real Time Clock                                                      ///
/// modified by Redpic 08/2006                                                      ///
/// http://picmania.garcia-cuervo.net                                               ///
///                                                                                 ///
/// void ds1307_init(val)                                                           ///
///   - Enable oscillator without clearing the seconds register                     ///
///     used when PIC loses power and DS1307 run from 3V BAT                        ///
///   - Config Control Register with next parameters:                               ///
///            DS1307_ALL_DISABLED All disabled                                     ///
///            DS1307_OUT_ON_DISABLED_HIHG Out to Hight on Disable Out              ///
///            DS1307_OUT_ENABLED Out Enabled                                       ///
///            DS1307_OUT_1_HZ Freq. Out to 1 Hz                                    ///
///            DS1307_OUT_4_KHZ Freq. Out to 4.096 Khz                              ///
///            DS1307_OUT_8_KHZ Freq. Out to 8.192 Khz                              ///
///            DS1307_OUT_32_KHZ Freq. Out to 32.768 Khz                            ///
///                                                                                 ///
///            Example init:                                                        ///
///                    ds1307_init(DS1307_ALL_DISABLED);                            ///
///                    ds1307_init(DS1307_OUT_ENABLED | DS1307_OUT_1_HZ);           ///
///                                                                                 ///
/// void ds1307_set_date_time(day,mth,year,dow,hour,min,sec) - Set the date/time    ///
///                                                                                 ///
/// void ds1307_get_date(day,mth,year,dow) - Get the date                           ///
///                                                                                 ///
/// void ds1307_get_time(hr,min,sec) - Get the time                                 ///
///                                                                                 ///
/// char ds1307_read_nvram_byte(char addr) - Read byte in address                   ///
///                                                                                 ///
/// void ds1307_write_nvram_byte(char addr, char value) - Write byte in address     ///
///                                                                                 ///
/// void ds1307_get_day_of_week(char* ptr) - Get string Day Of Week                 ///
///                                                                                 ///
/// If defined USE_INTERRUPTS all functions disable Global Interrupts on starts and ///
/// enable Global on ends else usar can do it hiself                                ///
///                                                                                 ///
///////////////////////////////////////////////////////////////////////////////////////
#ifndef DS1307_H
#define DS1307_H



#ifndef RTC_SDA
#define RTC_SDA PIN_B0
#define RTC_SCL PIN_B1
#endif

#use i2c(master, sda=RTC_SDA, scl=RTC_SCL)

#define DS1307_ALL_DISABLED 0b00000000 // All disabled
#define DS1307_OUT_ON_DISABLED_HIHG 0b10000000 // Out to Hight on Disable Out
#define DS1307_OUT_ENABLED 0b00010000 // Out Enabled
#define DS1307_OUT_1_HZ 0b00000000 // Freq. Out to 1 Hz
#define DS1307_OUT_4_KHZ 0b00000001 // Freq. Out to 4.096 Khz
#define DS1307_OUT_8_KHZ 0b00000010 // Freq. Out to 8.192 Khz
#define DS1307_OUT_32_KHZ 0b00000011 // Freq. Out to 32.768 Khz

#define Start_user_address_nvram 0x08
#define End_user_address_nvram 0x3f


void ds1307_init(int val);
void ds1307_set_date_time(byte day, byte mth, byte year, byte dow, byte hr, byte min, byte sec);
void ds1307_get_date(byte &day, byte &mth, byte &year, byte &dow);
void ds1307_get_time(byte &hr, byte &min, byte &sec);
char ds1307_read_nvram_byte(char addr);
void ds1307_write_nvram_byte(char addr, char value);
void ds1307_get_day_of_week(char* ptr);
#endif
