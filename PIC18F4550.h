#ifndef REGISTROS_H
#define REGISTROS_H

struct TOSU {
   unsigned int TOSU:5;
} TOSU;
#byte TOSU = 0xFFF

#word TOS = 0xFFE

struct STKPTR {
   unsigned int STKPTR:5;
   unsigned int :1;
   unsigned int STKUNF:1;
   unsigned int STKFUL:1;
} STKPTR;
#byte STKPTR = 0xFFC

struct PCLATU {
   unsigned int PCU:5;
} PCLATU;
#byte PCLATU = 0xFFB

#byte PCLATH = 0xFFA

#byte PCL = 0xFF9

struct TBLPTRU {
   unsigned int TBLPTRU:5;
} TBLPTRU;
#byte TBLPTRU = 0xFF8

#word TBLPTR = 0xFF7

#byte TABLAT = 0xFF5

#word PROD = 0xFF4

struct INTCON {
   unsigned int RBIF:1;
   unsigned int INT0IF:1;
   unsigned int TMR0IF:1;
   unsigned int RBIE:1;
   unsigned int INT0IE:1;
   unsigned int TMR0IE:1;
   unsigned int PEIE:1;
   unsigned int GIE:1;
} INTCON;
#byte INTCON = 0xFF2

struct INTCON2 {
   unsigned int RBIP:1;
   unsigned int :1;
   unsigned int TMR0IP:1;
   unsigned int :1;
   unsigned int INTEDG:1;
   unsigned int INTEDG_2:1;
   unsigned int INTEDG_3:1;
   unsigned int RBPU:1;
} INTCON2;
#byte INTCON2 = 0xFF1

struct INTCON3 {
   unsigned int INT1IF:1;
   unsigned int INT2IF:1;
   unsigned int :1;
   unsigned int INT1IE:1;
   unsigned int INT2IE:1;
   unsigned int :1;
   unsigned int INT1IP:1;
   unsigned int INT2IP:1;
} INTCON3;
#byte INTCON3 = 0xFF0

#byte INDF0 = 0xFEF

#byte POSTINC0 = 0xFEE

#byte POSTDEC0 = 0xFED

#byte PREINC0 = 0xFEC

#byte PLUSW0 = 0xFEB

struct FSR0H {
   unsigned int FSR0H:4;
} FSR0H;
#byte FSR0H = 0xFEA

#byte FSR0L = 0xFE9

#byte WREG = 0xFE8

#byte INDF1 = 0xFE7

#byte POSTINC1 = 0xFE6

#byte POSTDEC1 = 0xFE5

#byte PREINC1 = 0xFE4

#byte PLUSW1 = 0xFE3

struct FSR1H {
   unsigned int FSR1H:4;
} FSR1H;
#byte FSR1H = 0xFE2

#byte FSR1L = 0xFE1

struct BSR {
   unsigned int BSR:4;
} BSR;
#byte BSR = 0xFE0

#byte INDF2 = 0xFDF

#byte POSTINC2 = 0xFDE

#byte POSTDEC2 = 0xFDD

#byte PREINC2 = 0xFDC

#byte PLUSW2 = 0xFDB

struct FSR2H {
   unsigned int FSR2H:4;
} FSR2H;
#byte FSR2H = 0xFDA

#byte FSR2L = 0xFD9

struct STATUS {
   unsigned int C:1;
   unsigned int DC:1;
   unsigned int Z:1;
   unsigned int OV:1;
   unsigned int N:1;
} STATUS;
#byte STATUS = 0xFD8

#word TMR0 = 0xFD7

struct T0CON {
   unsigned int T0PS:3;
   unsigned int PSA:1;
   unsigned int T0SE:1;
   unsigned int T0CS:1;
   unsigned int T08BIT:1;
   unsigned int TMR0ON:1;
} T0CON;
#byte T0CON = 0xFD5

struct OSCCON {
   unsigned int SCS:2;
   unsigned int IOFS:1;
   unsigned int OSTS:1;
   unsigned int IRCF:3;
   unsigned int IDLEN:1;
} OSCCON;
#byte OSCCON = 0xFD3

struct HLVDCON {
   unsigned int HLVDL:4;
   unsigned int HLVDEN:1;
   unsigned int IRVST:1;
   unsigned int :1;
   unsigned int VDIRMAG:1;
} HLVDCON;
#byte HLVDCON = 0xFD2

struct WDTCON {
   unsigned int SWDTEN:1;
} WDTCON;
#byte WDTCON = 0xFD1

struct RCON {
   unsigned int BOR:1;
   unsigned int POR:1;
   unsigned int PD:1;
   unsigned int TO:1;
   unsigned int RI:1;
   unsigned int :1;
   unsigned int SBOREN:1;
   unsigned int IPEN:1;
} RCON;
#byte RCON = 0xFD0

#word TMR1 = 0xFCF

struct T1CON {
   unsigned int TMR1ON:1;
   unsigned int TMR1CS:1;
   unsigned int T1SYNC:1;
   unsigned int T1OSCEN:1;
   unsigned int T1CKPS:2;
   unsigned int T1RUN:1;
   unsigned int RD16:1;
} T1CON;
#byte T1CON = 0xFCD

#byte TMR2 = 0xFCC

#byte PR2 = 0xFCB

struct T2CON {
   unsigned int T2CKPS:2;
   unsigned int TMR2ON:1;
   unsigned int TOUTPS:4;
} T2CON;
#byte T2CON = 0xFCA

#byte SSPBUF = 0xFC9

#byte SSPADD = 0xFC8

struct SSPSTAT {
   unsigned int BF:1;
   unsigned int UA:1;
   unsigned int R:1;
   unsigned int S:1;
   unsigned int P:1;
   unsigned int D:1;
   unsigned int CKE:1;
   unsigned int SMP:1;
} SSPSTAT;
#byte SSPSTAT = 0xFC7

struct SSPCON1 {
   unsigned int SSPM:4;
   unsigned int CKP:1;
   unsigned int SSPEN:1;
   unsigned int SSPOV:1;
   unsigned int WCOL:1;
} SSPCON1;
#byte SSPCON1 = 0xFC6

struct SSPCON2 {
   unsigned int SEN:1;
   unsigned int RSEN:1;
   unsigned int PEN:1;
   unsigned int RCEN:1;
   unsigned int ACKEN:1;
   unsigned int ACKDT:1;
   unsigned int ACKSTAT:1;
   unsigned int GCEN:1;
} SSPCON2;
#byte SSPCON2 = 0xFC5

#word ADRES = 0xFC4

struct ADCON0 {
   unsigned int ADON:1;
   unsigned int GO:1;
   unsigned int CHS:4;
} ADCON0;
#byte ADCON0 = 0xFC2

struct ADCON1 {
   unsigned int PCFG:4;
   unsigned int VCFG:2;
} ADCON1;
#byte ADCON1 = 0xFC1

struct ADCON2 {
   unsigned int ADCS:3;
   unsigned int ACQT:3;
   unsigned int :1;
   unsigned int ADFM:1;
} ADCON2;
#byte ADCON2 = 0xFC0

#word CCPR1 = 0xFBF

struct CCP1CON {
   unsigned int CCP1M:4;
   unsigned int DC1B:2;
   unsigned int P1M:2;
} CCP1CON;
#byte CCP1CON = 0xFBD

#word CCPR2 = 0xFBC

struct CCP2CON {
   unsigned int CCP2M:4;
   unsigned int DC2B:2;
} CCP2CON;
#byte CCP2CON = 0xFBA

struct BAUDCON {
   unsigned int ABDEN:1;
   unsigned int WUE:1;
   unsigned int :1;
   unsigned int BRG1:1;
   unsigned int TXCKP:1;
   unsigned int RXDTP:1;
   unsigned int RCMT:1;
   unsigned int ABDOVF:1;
} BAUDCON;
#byte BAUDCON = 0xFB8

struct ECCP1DEL {
   unsigned int PDC:7;
   unsigned int PRSEN:1;
} ECCP1DEL;
#byte ECCP1DEL = 0xFB7

struct ECCP1AS {
   unsigned int PSSBD:2;
   unsigned int PSSAC:2;
   unsigned int ECCPAS:3;
   unsigned int ECCPASE:1;
} ECCP1AS;
#byte ECCP1AS = 0xFB6

struct CVRCON {
   unsigned int CVR:4;
   unsigned int CVRSS:1;
   unsigned int CVRR:1;
   unsigned int CVROE:1;
   unsigned int CVREN:1;
} CVRCON;
#byte CVRCON = 0xFB5

struct CMCON {
   unsigned int CM:3;
   unsigned int CIS:1;
   unsigned int C1INV:1;
   unsigned int C2INV:1;
   unsigned int C1OUT:1;
   unsigned int C2OUT:1;
} CMCON;
#byte CMCON = 0xFB4

#word TMR3 = 0xFB3

struct T3CON {
   unsigned int TMR3ON:1;
   unsigned int TMR3CS:1;
   unsigned int T3SYNC:1;
   unsigned int T3CCP:1;
   unsigned int T3CKPS:2;
   unsigned int T3CCP_2:1;
   unsigned int RD16:1;
} T3CON;
#byte T3CON = 0xFB1

#byte SPBRGH = 0xFB0

#byte SPBRG = 0xFAF

#byte RCREG = 0xFAE

#byte TXREG = 0xFAD

struct TXSTA {
   unsigned int TX9D:1;
   unsigned int TRMT:1;
   unsigned int BRGH:1;
   unsigned int SENDB:1;
   unsigned int SYNC:1;
   unsigned int TXEN:1;
   unsigned int TX:1;
   unsigned int CSRC:1;
} TXSTA;
#byte TXSTA = 0xFAC

struct RCSTA {
   unsigned int RX9D:1;
   unsigned int OERR:1;
   unsigned int FERR:1;
   unsigned int ADDEN:1;
   unsigned int CREN:1;
   unsigned int SREN:1;
   unsigned int RX:1;
   unsigned int SPEN:1;
} RCSTA;
#byte RCSTA = 0xFAB

#byte EEADR = 0xFA9

#byte EEDATA = 0xFA8

#byte EECON2 = 0xFA7

struct EECON1 {
   unsigned int RD:1;
   unsigned int WR:1;
   unsigned int WREN:1;
   unsigned int WRERR:1;
   unsigned int FREE:1;
   unsigned int :1;
   unsigned int CFGS:1;
   unsigned int EEPGD:1;
} EECON1;
#byte EECON1 = 0xFA6

struct IPR2 {
   unsigned int CCP2IP:1;
   unsigned int TMR3IP:1;
   unsigned int HLVDIP:1;
   unsigned int BCLIP:1;
   unsigned int EEIP:1;
   unsigned int USBIP:1;
   unsigned int CMIP:1;
   unsigned int OSCFIP:1;
} IPR2;
#byte IPR2 = 0xFA2

struct PIR2 {
   unsigned int CCP2IF:1;
   unsigned int TMR3IF:1;
   unsigned int HLVDIF:1;
   unsigned int BCLIF:1;
   unsigned int EEIF:1;
   unsigned int USBIF:1;
   unsigned int CMIF:1;
   unsigned int OSCFIF:1;
} PIR2;
#byte PIR2 = 0xFA1

struct PIE2 {
   unsigned int CCP2IE:1;
   unsigned int TMR3IE:1;
   unsigned int HLVDIE:1;
   unsigned int BCLIE:1;
   unsigned int EEIE:1;
   unsigned int USBIE:1;
   unsigned int CMIE:1;
   unsigned int OSCFIE:1;
} PIE2;
#byte PIE2 = 0xFA0

struct IPR1 {
   unsigned int TMR1IP:1;
   unsigned int TMR2IP:1;
   unsigned int CCP1IP:1;
   unsigned int SSPIP:1;
   unsigned int TXIP:1;
   unsigned int RCIP:1;
   unsigned int ADIP:1;
   unsigned int SPPIP:1;
} IPR1;
#byte IPR1 = 0xF9F

struct PIR1 {
   unsigned int TMR1IF:1;
   unsigned int TMR2IF:1;
   unsigned int CCP1IF:1;
   unsigned int SSPIF:1;
   unsigned int TXIF:1;
   unsigned int RCIF:1;
   unsigned int ADIF:1;
   unsigned int SPPIF:1;
} PIR1;
#byte PIR1 = 0xF9E

struct PIE1 {
   unsigned int TMR1IE:1;
   unsigned int TMR2IE:1;
   unsigned int CCP1IE:1;
   unsigned int SSPIE:1;
   unsigned int TXIE:1;
   unsigned int RCIE:1;
   unsigned int ADIE:1;
   unsigned int SPPIE:1;
} PIE1;
#byte PIE1 = 0xF9D

struct OSCTUNE {
   unsigned int TUN:5;
   unsigned int :2;
   unsigned int HF256DIV:1;
} OSCTUNE;
#byte OSCTUNE = 0xF9B

struct TRISE {
   unsigned int TRISE0:1;
   unsigned int TRISE1:1;
   unsigned int TRISE2:1;
   unsigned int TRISE3:1;
   unsigned int TRISE4:1;
   unsigned int TRISE5:1;
   unsigned int TRISE6:1;
   unsigned int TRISE7:1;
} TRISE;
#byte TRISE = 0xF96

struct TRISD {
   unsigned int TRISD0:1;
   unsigned int TRISD1:1;
   unsigned int TRISD2:1;
   unsigned int TRISD3:1;
   unsigned int TRISD4:1;
   unsigned int TRISD5:1;
   unsigned int TRISD6:1;
   unsigned int TRISD7:1;
} TRISD;
#byte TRISD = 0xF95

struct TRISC {
   unsigned int TRISC0:1;
   unsigned int TRISC1:1;
   unsigned int TRISC2:1;
   unsigned int TRISC3:1;
   unsigned int TRISC4:1;
   unsigned int TRISC5:1;
   unsigned int TRISC6:1;
   unsigned int TRISC7:1;
} TRISC;
#byte TRISC = 0xF94

struct TRISB {
   unsigned int TRISB0:1;
   unsigned int TRISB1:1;
   unsigned int TRISB2:1;
   unsigned int TRISB3:1;
   unsigned int TRISB4:1;
   unsigned int TRISB5:1;
   unsigned int TRISB6:1;
   unsigned int TRISB7:1;
} TRISB;
#byte TRISB = 0xF93

struct TRISA {
   unsigned int TRISA0:1;
   unsigned int TRISA1:1;
   unsigned int TRISA2:1;
   unsigned int TRISA3:1;
   unsigned int TRISA4:1;
   unsigned int TRISA5:1;
   unsigned int TRISA6:1;
   unsigned int TRISA7:1;
} TRISA;
#byte TRISA = 0xF92

struct LATE {
   unsigned int LATE0:1;
   unsigned int LATE1:1;
   unsigned int LATE2:1;
   unsigned int LATE3:1;
   unsigned int LATE4:1;
   unsigned int LATE5:1;
   unsigned int LATE6:1;
   unsigned int LATE7:1;
} LATE;
#byte LATE = 0xF8D

struct LATD {
   unsigned int LATD0:1;
   unsigned int LATD1:1;
   unsigned int LATD2:1;
   unsigned int LATD3:1;
   unsigned int LATD4:1;
   unsigned int LATD5:1;
   unsigned int LATD6:1;
   unsigned int LATD7:1;
} LATD;
#byte LATD = 0xF8C

struct LATC {
   unsigned int LATC0:1;
   unsigned int LATC1:1;
   unsigned int LATC2:1;
   unsigned int LATC3:1;
   unsigned int LATC4:1;
   unsigned int LATC5:1;
   unsigned int LATC6:1;
   unsigned int LATC7:1;
} LATC;
#byte LATC = 0xF8B

struct LATB {
   unsigned int LATB0:1;
   unsigned int LATB1:1;
   unsigned int LATB2:1;
   unsigned int LATB3:1;
   unsigned int LATB4:1;
   unsigned int LATB5:1;
   unsigned int LATB6:1;
   unsigned int LATB7:1;
} LATB;
#byte LATB = 0xF8A

struct LATA {
   unsigned int LATA0:1;
   unsigned int LATA1:1;
   unsigned int LATA2:1;
   unsigned int LATA3:1;
   unsigned int LATA4:1;
   unsigned int LATA5:1;
   unsigned int LATA6:1;
   unsigned int LATA7:1;
} LATA;
#byte LATA = 0xF89

struct PORTE {
   unsigned int :7;
   unsigned int RDPU:1;
} PORTE;
#byte PORTE = 0xF84

struct PORTD {
   unsigned int RD0:1;
   unsigned int RD1:1;
   unsigned int RD2:1;
   unsigned int RD3:1;
   unsigned int RD4:1;
   unsigned int RD5:1;
   unsigned int RD6:1;
   unsigned int RD7:1;
} PORTD;
#byte PORTD = 0xF83

struct PORTC {
   unsigned int RC0:1;
   unsigned int RC1:1;
   unsigned int RC2:1;
   unsigned int RC3:1;
   unsigned int RC4:1;
   unsigned int RC5:1;
   unsigned int RC6:1;
   unsigned int RC7:1;
} PORTC;
#byte PORTC = 0xF82

struct PORTB {
   unsigned int RB0:1;
   unsigned int RB1:1;
   unsigned int RB2:1;
   unsigned int RB3:1;
   unsigned int RB4:1;
   unsigned int RB5:1;
   unsigned int RB6:1;
   unsigned int RB7:1;
} PORTB;
#byte PORTB = 0xF81

struct PORTA {
   unsigned int RA0:1;
   unsigned int RA1:1;
   unsigned int RA2:1;
   unsigned int RA3:1;
   unsigned int RA4:1;
   unsigned int RA5:1;
   unsigned int RA6:1;
   unsigned int RA7:1;
} PORTA;
#byte PORTA = 0xF80

struct UEP15 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP15;
#byte UEP15 = 0xF7F

struct UEP14 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP14;
#byte UEP14 = 0xF7E

struct UEP13 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP13;
#byte UEP13 = 0xF7D

struct UEP12 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP12;
#byte UEP12 = 0xF7C

struct UEP11 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP11;
#byte UEP11 = 0xF7B

struct UEP10 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP10;
#byte UEP10 = 0xF7A

struct UEP9 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP9;
#byte UEP9 = 0xF79

struct UEP8 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP8;
#byte UEP8 = 0xF78

struct UEP7 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP7;
#byte UEP7 = 0xF77

struct UEP6 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP6;
#byte UEP6 = 0xF76

struct UEP5 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP5;
#byte UEP5 = 0xF75

struct UEP4 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP4;
#byte UEP4 = 0xF74

struct UEP3 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP3;
#byte UEP3 = 0xF73

struct UEP2 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP2;
#byte UEP2 = 0xF72

struct UEP1 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP1;
#byte UEP1 = 0xF71

struct UEP0 {
   unsigned int EPSTALL:1;
   unsigned int EPINEN:1;
   unsigned int EPOUTEN:1;
   unsigned int EPCONDIS:1;
   unsigned int EPHSHK:1;
} UEP0;
#byte UEP0 = 0xF70

struct UCFG {
   unsigned int PPB:2;
   unsigned int FSEN:1;
   unsigned int UTRDIS:1;
   unsigned int UPUEN:1;
   unsigned int :1;
   unsigned int UOEMON:1;
   unsigned int UTEYE:1;
} UCFG;
#byte UCFG = 0xF6F

struct UADDR {
   unsigned int ADDR:7;
} UADDR;
#byte UADDR = 0xF6E

struct UCON {
   unsigned int :1;
   unsigned int SUSPND:1;
   unsigned int RESUME:1;
   unsigned int USBEN:1;
   unsigned int PKTDIS:1;
   unsigned int SE:1;
   unsigned int PPBRST:1;
} UCON;
#byte UCON = 0xF6D

struct USTAT {
   unsigned int :1;
   unsigned int PPBI:1;
   unsigned int DIR:1;
   unsigned int ENDP:4;
} USTAT;
#byte USTAT = 0xF6C

struct UEIE {
   unsigned int PIDEE:1;
   unsigned int CRC5EE:1;
   unsigned int CRC16EE:1;
   unsigned int DFN8EE:1;
   unsigned int BTOEE:1;
   unsigned int :2;
   unsigned int BTSEE:1;
} UEIE;
#byte UEIE = 0xF6B

struct UEIR {
   unsigned int PIDEF:1;
   unsigned int CRC5EF:1;
   unsigned int CRC16EF:1;
   unsigned int DFN8EF:1;
   unsigned int BTOEF:1;
   unsigned int :2;
   unsigned int BTSEF:1;
} UEIR;
#byte UEIR = 0xF6A

struct UIE {
   unsigned int URSTIE:1;
   unsigned int UERRIE:1;
   unsigned int ACTIVIE:1;
   unsigned int TRNIE:1;
   unsigned int IDLEIE:1;
   unsigned int STALLIE:1;
   unsigned int SOFIE:1;
} UIE;
#byte UIE = 0xF69

struct UIR {
   unsigned int URSTIF:1;
   unsigned int UERRIF:1;
   unsigned int ACTIVIF:1;
   unsigned int TRNIF:1;
   unsigned int IDLEIF:1;
   unsigned int STALLIF:1;
   unsigned int SOFIF:1;
} UIR;
#byte UIR = 0xF68

struct UFRMH {
   unsigned int FRM:3;
} UFRMH;
#byte UFRMH = 0xF67

#byte UFRML = 0xF66

struct SPPCON {
   unsigned int SPPEN:1;
   unsigned int SPPOWN:1;
} SPPCON;
#byte SPPCON = 0xF65

struct SPPEPS {
   unsigned int ADDR:4;
   unsigned int SPPBUSY:1;
   unsigned int :1;
   unsigned int WRSPP:1;
   unsigned int RDSPP:1;
} SPPEPS;
#byte SPPEPS = 0xF64

struct SPPCFG {
   unsigned int WS:4;
   unsigned int CLK1EN:1;
   unsigned int CSEN:1;
   unsigned int CLKCFG:2;
} SPPCFG;
#byte SPPCFG = 0xF63

#byte SPPDATA = 0xF62

#endif
