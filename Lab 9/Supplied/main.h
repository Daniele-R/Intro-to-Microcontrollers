//colors
#define OFF             0               // Defines OFF as decimal value 0
#define RED             1               // Defines RED as decimal value 1
#define GREEN           2               // Defines GREEN as decimal value 2
#define YELLOW          3               // Defines YELLOW as decimal value 3

#define Semester        0               // Type 0 for Sp, 1 for Fa
#define Year            24              // Type 2 digits year
#define Session_Number  1               // Type Session Number 1 through 5
#define Table_Number    07              // Type Table Number from 01 through 14
        

#define NS_RED      PORTAbits.RA1       // Defines NS_RED as PORTA bits RA1
#define NS_GREEN    PORTAbits.RA2       // Defines NS_GREEN as PORTA bit RA2

#define NSLT_RED    PORTBbits.RB3       // Defines NS_LT RED as PORTB bit RB3
#define NSLT_GREEN  PORTBbits.RB4       // Defines NS_LT GREEN as PORTB bit RB4

#define EW_RED      PORTBbits.RB5       // Defines EW_RED as PORTB bit RB5
#define EW_GREEN    PORTBbits.RB6       // Defines EW_GREEN as PORTB bit RB6

#define EWLT_RED    PORTBbits.RB7       // Defines EWLT_RED as PORTB bit RB7
#define EWLT_GREEN  PORTEbits.RE0       // Defines EWLT_GREEN as PORTE bit RE0

#define NS_LT_SW    PORTAbits.RA5       // Defines NS_LT as PORTA bit RA5 for left turn
// #define NS_PED_SW   PORTAbits.RA3       // Defines EW_LT as PORTA bit RA3 for Ped-switch
#define EW_LT_SW    PORTCbits.RC0       // Defines EW_PED as PORTC bit RA0 for left turn
// #define EW_PED_SW   PORTAbits.RA4       // Defines NS_PED as PORTA bit RA4 for Ped-switch

#define MODE_LED    PORTEbits.RE1       // Defines MODE_LED as PORTD bit RD3,to differentiate day/night mode
#define SEC_LED     PORTEbits.RE2       // Defines SEC_LED as PORTD bit RD7

/* PUT THE NUMBERS FROM THE DAY N NIGHT MODE (PED WAITING) */

#define PEDESTRIAN_NS_WAIT  8
#define NS_WAIT             7
#define EW_LT_WAIT          8
#define PEDESTRIAN_EW_WAIT  7
#define EW_WAIT             6
#define NS_LT_WAIT          7

#define NIGHT_NS_WAIT       6
#define NIGHT_EW_LT_WAIT    7
#define NIGHT_EW_WAIT       6
#define NIGHT_NS_LT_WAIT    8

void init_UART();
void putch(char);
void Init_ADC(void);
unsigned int Get_full_ADC(void);
void Select_ADC_Channel(char);
void Wait_One_Second();
void Wait_Half_Second();
void Wait_N_Seconds (char);
void Set_NS(char);
void Set_NSLT(char);
void Set_EW(char);
void Set_EWLT(char);
void PED_Control(char, char);
void Wait_One_Second_With_Beep();
void Activate_Buzzer();
void Deactivate_Buzzer();
void Night_Mode();
void Day_Mode();


