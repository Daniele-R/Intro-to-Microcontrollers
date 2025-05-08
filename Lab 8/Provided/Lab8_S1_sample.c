#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <string.h>

#include "main.h"
#include "utils.h"
#include "Main_Screen.h"
#include "ST7735_TFT.h"


void Set_EW(char color);
void Set_EW_LT(char color);
void Set_NS(char color);
void Set_NS_LT(char color);
void PED_Control( char Direction, char Num_Sec);
void Day_Mode();
void Night_Mode();
void Wait_N_Seconds (char);
void Wait_One_Second_With_Beep();
void Wait_One_Second();


#define _XTAL_FREQ  8000000             // Set operation for 8 Mhz
#define TMR_CLOCK   _XTAL_FREQ/4        // Timer Clock 2 Mhz

char dir;
char Count;                             // RAM variable for Second Count
char PED_Count;                         // RAM variable for Second Pedestrian Count

char  MODE;
char direction;
float volt;

extern char Light_Sensor;

void main(void)
{
    OSCCON = 0x70;                      // set the system clock to be 1MHz 1/4 of the 4MHz
    TRISA = 0x1F;                       //inputs RA0/AN0-RA4 inputs,RA5 output
    TRISB = 0x07;                       // outputs
    TRISC = 0x00;                       // set PORTC as outputs
    TRISD = 0x00;                       // set PORTD as outputs
    TRISE = 0x00;                       // set PORTE as outputs
    Init_ADC();
    Init_UART();

    Initialize_LCD_Screen();                        // Initialize the TFT screen

//    while (1)
//    {
//        Rcmd2red();
//        delay_ms(2);
//        
//    }

    volt = Read_Volt(0);                        // 

    Light_Sensor = volt < 2.5 ? 1:0;                 // Mode = 1, Day_mode, Mode = 0 Night_mode

    while(1)                                    // forever loop
    {

        if (Light_Sensor == 1)    
        {
            Day_Mode();                         // calls Day_Mode() function
        }
        else
        {
            Night_Mode();                       // calls Night_Mode() function
        }
     
    } 
}


void Set_NS(char color)
{
    // add code here
}

void Set_NS_LT(char color)
{
    // add code here
}

void Set_EW(char color)
{
    // add code here
}

void Set_EW_LT(char color)
{
    // add code here
}

void PED_Control( char direction, char Num_Sec)
{ 
    // add code here
}

void Day_Mode()
{
    // add code here
}

void Night_Mode()
{ 
    // add code here
}

void Wait_One_Second()							//creates one second delay and blinking asterisk
{
    SEC_LED = 1;
    Draw_Star();
    Wait_Half_Second();                         // Wait for half second (or 500 msec)
    SEC_LED = 0;
    Erase_Star();
    Wait_Half_Second();                         // Wait for half second (or 500 msec)
	// add code here
}

void Wait_One_Second_With_Beep()				//creates one second delay as well as sound buzzer
{
    SEC_LED = 1;
    Draw_Star();
    Activate_Buzzer();
    Wait_Half_Second();                         // Wait for half second (or 500 msec)

    SEC_LED = 0;
    Erase_Star();
    Deactivate_Buzzer();
    Wait_Half_Second();                         // Wait for half second (or 500 msec)
	// add code here;
}
 

void Wait_N_Seconds (char seconds)
{
    char I;
    for (I = seconds; I> 0; I--)
    {
		// add code here;
        Wait_One_Second();          			// calls Wait_One_Second for x number of times
        
    }
	// add code here;
}
  
}






