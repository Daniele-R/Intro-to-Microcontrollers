#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <p18f4620.h>

#include "main.h"
#include "utils.h"
#include "ST7735_TFT.h"
#include "Main_Screen.h"

extern char found;
extern char Nec_code1;
extern short nec_ok;
extern char array1[21];
extern char duty_cycle;


void Init_ADC()
{
    ADCON0 = 0x01;
    ADCON1 = 0x0E; 
    ADCON2 = 0xA9;
}

void Init_UART(void)
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF &
    USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX &
    USART_BRGH_HIGH, 25);
    OSCCON = 0x70;
}

void putch (char c)
{
    while (!TRMT);
    TXREG = c;
}

void Wait_One_Sec()                       //creates one second delay and blinking asterisk
{
    Wait_Half_Second();                         // Wait for half second (or 500 msec)
    Wait_Half_Second();                         // Wait for half second (or 500 msec)

}
void Do_Beep(void)
{
    Activate_Buzzer();
    Wait_One_Sec();
    Deactivate_Buzzer();
    Wait_One_Sec();

}


void Do_Beep_Good(void)
{
										// to be added in later lab

}

void Do_Beep_Bad(void)
{
										// to be added in later lab

}

void Activate_Buzzer(void)
{
    PR2 = 0b11111001 ;
    T2CON = 0b00000101 ;
    CCPR2L = 0b01001010 ;
    CCP2CON = 0b00111100 ;
}

void Activate_Buzzer_500Hz(void)
{
										// to be added in later lab
}

void Activate_Buzzer_2KHz(void)
{
										// to be added in later lab
}

void Activate_Buzzer_4KHz(void)
{
										// to be added in later lab
}

void Deactivate_Buzzer(void)
{
    CCP2CON = 0x0;
	PORTCbits.RC1 = 0;
}

  
void Wait_Half_Second()
{
    T0CON = 0x03;                   // Timer 0, 16-bit mode, prescaler 1:16
    TMR0L = 0xDB;                   // set the lower byte of TMR
    TMR0H = 0x0B;                   // set the upper byte of TMR
    INTCONbits.TMR0IF = 0;          // clear the Timer 0 flag
    T0CONbits.TMR0ON = 1;           // Turn on the Timer 0
    while (INTCONbits.TMR0IF == 0); // wait for the Timer Flag to be 1 for done
    T0CONbits.TMR0ON = 0;           // turn off the Timer 0
}


void Wait_One_Sec_Soft(void)
{
    for (int k=0;k<0xffff;k++);
}

void do_update_pwm(char duty_cycle) 
{ 
										// to be added in later lab
}

void Set_RGB_Color(char color)
{
										// to be added in later lab
}

float Read_Volt(char ADC_Channel)
{
	ADCON0 = ADC_Channel * 4 + 1;		//
	int nStep = get_full_ADC();
    float volt = nStep * 5 /1024.0;
	return (volt);
}

unsigned int get_full_ADC(void)
{
unsigned int result;
   ADCON0bits.GO=1;                     // Start Conversion
   while(ADCON0bits.DONE==1);           // wait for conversion to be completed
   result = (ADRESH * 0x100) + ADRESL;  // combine result of upper byte and
                                        // lower byte into result
   return result;                       // return the result.
}

char check_for_button_input(void)
{ 
										// to be added in later lab
}

char bcd_2_dec (char bcd)
{
    int dec;
    dec = ((bcd >> 4) * 10) + (bcd & 0x0f);
    return dec;
}

int dec_2_bcd (char dec)
{
    int bcd;
    bcd = ((dec / 10) << 4) + (dec % 10);
    return bcd;
}



