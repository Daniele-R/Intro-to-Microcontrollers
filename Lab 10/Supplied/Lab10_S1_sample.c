#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>
#include <string.h>
#include "main.h"
#include "ST7735_TFT.h"
#include "Interrupt.h"
#include "utils.h"
#include "Main_Screen.h"



#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF
#pragma config CCP2MX = PORTC


#define _XTAL_FREQ  8000000             // Set operation for 8 Mhz

short Nec_OK = 0;
char Nec_Button;
unsigned long long Nec_code = 0;

// char array obtained thru find
char array1[21]=
{
    0xa2, 0x62, 0xe2, 
    0x22, 0x02, 0xc2, 
    0xe0, 0xa8, 0x90,  
    0x68, 0x98, 0xb0, 
    0x30, 0x18, 0x7a, 
    0x10, 0x38, 0x5a, 
    0x42, 0x4a, 0x52
};
// controller characters
char txt1[21][4] =
{   
    "CH-\0", "CH \0","CH+\0",
    "|<<\0",">>|\0",">||\0",
    " - \0"," + \0","EQ \0",
    " 0 \0", "100\0","200\0",
    " 1 \0"," 2 \0"," 3 \0",
    " 4 \0"," 5 \0"," 6 \0",
    " 7 \0"," 8 \0"," 9 \0"
};

// colors for LCD
int color[21]=
{
    RD,RD,RD,
    BU,BU,GR,
    BU,BU,MA,
    BK,BK,BK,
    BK,BK,BK,
    BK,BK,BK,
    BK,BK,BK
};

// colors d1-d3 are RGBs setups
int colord1[21] = 
{
    RED, RED, RED, 
    CYAN, CYAN, GREEN, 
    OFF, OFF, OFF, 
    OFF, OFF, OFF, 
    OFF, OFF, OFF, 
    OFF, OFF, OFF, 
    OFF, OFF, OFF
};

int colord2[21] = 
{
    OFF, OFF, OFF, 
    OFF, OFF, OFF, 
    BLUE, BLUE, PURPLE, 
    WHITE, WHITE, WHITE, 
    OFF, OFF, OFF, 
    OFF, OFF, OFF, 
    OFF, OFF, OFF
};

int colord3[21] = 
{
    OFF, OFF, OFF, 
    OFF, OFF, OFF, 
    OFF, OFF, OFF, 
    OFF, OFF, OFF, 
    WHITE, WHITE, WHITE, 
    WHITE, WHITE, WHITE, 
    WHITE, WHITE, WHITE
};



void main()
{
    Init_UART();
    OSCCON = 0x70;                           // 8 Mhz
    nRBPU  = 0;                              // Enable PORTB internal pull up resistor
    TRISA  = 0x00;
    TRISB  = 0x02;                           // mask first 2 bits
    TRISC  = 0x00;                           // PORTC as output
    TRISD  = 0x00;
    TRISE  = 0x00;
    
    ADCON1 = 0x0F;     

    Initialize_LCD_Screen();
    Init_Interrupt();

    Nec_code = 0x0;                         // Clear code
    
//    while (1)
//    {
//        KEY_PRESSED = 0;
//        Wait_One_Sec();
//        KEY_PRESSED = 1;
//        Wait_One_Sec();
//        
//    }
    while(1)
    {   
        if (Nec_OK == 1)
        {
            Nec_OK = 0;
            
            printf ("NEC_Button = %x \r\n", Nec_Button);  
            Enable_INT_Interrupt();
            
            char found = 0xff;
            
            for( int i = 0; i < 21; i++ )               // Loop 21 times
            {
                if( Nec_Button == array1[i] )           // If Nec_Button is equal to content of array
                {                                       // then
                    found = i;                          // Return matching index
                }
            }
            
            // unnecessary
            //char OutA = PORTA & 0x02;                   // Grab contents of PORTB and mask bit 1 
            //char OutB = PORTB & 0x70;  
            
            if (found != 0xff) 
            {
				printf ("Key Location = %d \r\n\n", found);      
                fillCircle(Circle_X, Circle_Y, Circle_Size, color[found]); 
                drawCircle(Circle_X, Circle_Y, Circle_Size, ST7735_WHITE);  
                drawtext(Text_X, Text_Y, txt1[found], ST7735_WHITE, ST7735_BLACK,TS_1);
                
                PORTA = colord1[found] << 1 ;           // Shift left by 1 to match RA1-RA3
                PORTB = colord2[found] << 3;            // Shift left by 3 to match RB3-RB5
                PORTE = colord3[found]; 
                
                KEY_PRESSED = 1;                        // KEY PRESSED LED turns on
                Activate_Buzzer();                      // Activate Buzzer
                Wait_One_Sec();                         // Wait one second
                Deactivate_Buzzer();                    // Deactivate Buzzer
                KEY_PRESSED = 0;                        // KEY PRESSED LED turn off
            }
        }
    }
}