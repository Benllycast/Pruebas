// Real Time Clock & NVRAM
// Hardware DS1307 of Dallas Maxim
// With interface I2C

#include <18f4550.h>
#fuses HS,MCLR,NOWDT,NOPROTECT,NOPUT,NOBROWNOUT,NOPBADEN,NOLVP,NOCPD,NODEBUG,NOWRT,NOVREGEN
#use delay(clock=20000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)

// Libreria DS1307.c //////////////////////////////////////////////////////////

#define RTC_SDA  PIN_B0
#define RTC_SCL  PIN_B1

#define USE_INTERRUPTS 1

#include <_ds1307.c>

///////////////////////////////////////////////////////////////////////////////

const char Version[]="1.0.D\0";

int1 flanco=0;
int1 dump,kdump;
int  n,i=0x00;
char c=0x00;
char rec=0x00;

byte sec;
byte min;
byte hrs;
byte day;
byte month;
byte yr;
byte dow;
char sdow[11];

// INTERRUPCION por RECEPCION SERIE -------------------------------------------

#int_rda
void serial_isr() {

   rec=0x00;
   if(kbhit()){
     rec=getc();
     if(rec=='o'){ dump=1; }
     if(rec=='f'){ dump=0; }
     rec=0x00;
   }
}

// INTERRUPCION por EXT2 Clock Out --------------------------------------------

#int_ext2
ext2_handler() {

   if(flanco==1){
      ext_int_edge(2,H_TO_L);
      output_high(PIN_E0);
   }else{
      ext_int_edge(2,L_TO_H);
      output_low(PIN_E0);
   }
   ++flanco;
}

//-----------------------------------------------------------------------------

void flash_porte(void){

   for(i=0;i<3;i++){
      output_e(0x07);
      delay_ms(75);
      output_e(0x00);
      delay_ms(75);
   }
}

void lee_y_transmite_date_and_time(void){

   ds1307_get_day_of_week((char*) sdow);
   ds1307_get_date(day,month,yr,dow);
   ds1307_get_time(hrs,min,sec);
   printf("\f\%s \%02d/\%02d/\%02d ",sdow,day,month,yr);
   printf("\%02d:\%02d:\%02d\r\n", hrs,min,sec);
}

void main() {

   disable_interrupts(global);
   disable_interrupts(int_timer1);
   disable_interrupts(int_rda);
   disable_interrupts(int_ext);
   disable_interrupts(int_ext1);
   disable_interrupts(int_ext2);
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_spi(FALSE);
   setup_psp(PSP_DISABLED);
   setup_counters(RTCC_INTERNAL,RTCC_DIV_2);
   setup_timer_0(RTCC_OFF);
   setup_timer_1(T1_INTERNAL | T1_DIV_BY_1);
   setup_timer_2(T2_DISABLED,0,1);
   setup_timer_3(T3_DISABLED);
   setup_comparator(NC_NC_NC_NC);
   setup_vref(FALSE);
   port_b_pullups(FALSE);
   set_tris_b(0b00000111);
   set_tris_e(0b00010000);
   output_e(0x00);
   set_tris_c(0b10000000);

   delay_ms(500);
   printf("\r\n");
   printf("[RRBOARD2] Real Time Clock version %s\r\n",version);
   printf("based on DS1307 Dallas Maxim Hardware\r\n\n");
   printf("Available RS232 commands:\r\n");
   printf("  [o] Dump Date & Time ON\r\n");
   printf("  [f] Dump Date & Time OFF\r\n\n");
   flash_porte();

   ext_int_edge(2,H_TO_L);
   flanco=0;

   enable_interrupts(int_ext2);
   enable_interrupts(int_rda);
   enable_interrupts(global);

   // Inicializa DS1307
   printf("Inicializando DS1307 ...\r\n\n");
   ds1307_init(DS1307_OUT_ON_DISABLED_HIHG | DS1307_OUT_ENABLED | DS1307_OUT_1_HZ);

   // Set date for -> 5 Agosto 2006 Viernes
   // Set time for -> 02:50:00
   // printf("Set Date & Time to ...\r\n");
   // ds1307_set_date_time(5,8,6,5,2,50,00);

   // Lee Fecha y Hora actual y transmite

   printf("Fecha y Hora actual en el DS1307\r\n\n");
   lee_y_transmite_date_and_time();
   printf("\r\n\n");

   // Test de RAM -> primero Write, segundo Read
   printf("Test de la NVRAM interna (Write and read) ...\r\n\n");
   for(i=Start_user_address_nvram;i<end_user_address_nvram+1;i++){
      ds1307_write_nvram_byte(i,i);
   }
   n=0;
   for(i=Start_user_address_nvram;i<end_user_address_nvram+1;i++){
      c=ds1307_read_nvram_byte(i);
      printf("%X ",c);
      if(++n==0x0f){
         n=0;
         printf("\r\n");
      }
   }
   printf("\r\n");

   dump = 0;
   kdump= 1;

   do{

      if(dump==1){
         if((dump!=kdump)&&dump==1){
            printf("Set Dump ON\r\n\n");
            kdump=dump;
         }
         delay_ms(1000);
         lee_y_transmite_date_and_time();
      }else{
         if((dump!=kdump)&&dump==0){
            printf("\r\nSet Dump OFF\r\n\n");
            kdump=dump;
         }
      }
   } while (TRUE);
}
