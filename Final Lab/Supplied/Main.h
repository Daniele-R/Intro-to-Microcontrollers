
#define _XTAL_FREQ      8000000

#define ACK             1
#define NAK             0

#define ACCESS_CFG      0xAC
#define START_CONV      0xEE
#define READ_TEMP       0xAA
#define CONT_CONV       0x02


//#define FAN_EN          PORTAbits.RA4           // <-- Need to change the assignments here
#define RTC_ALARM_NOT   PORTBbits.RB2           // <-- Need to change the assignments here
#define KEY_PRESSED     PORTDbits.RD7           // <-- Need to change the assignments here


#define OFF 0x00                                                                
#define RED 0x01                                                                
#define YELLOW 0x03                                                             
#define GREEN 0x02                                                              
#define BLUE 0x04                                                               
#define PURPLE 0x05                                                             
#define CYAN 0x06                                                               
#define WHT 0x07  

char D1[7] = {RED, YELLOW, GREEN, BLUE, PURPLE, CYAN, WHT};
char D2[7] = {RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHT};
char D3[7] = {RED, YELLOW, GREEN, BLUE, PURPLE, CYAN, WHT};

#define Ch_Minus        0
#define Channel         1
#define Ch_Plus         2
#define Prev            3
#define Next            4
#define Play_Pause      5
#define Minus           6
#define Plus            7
#define EQ              8
#define Zero_Button     9






