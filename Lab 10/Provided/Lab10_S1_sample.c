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
#pragma config CCP2MX = PORTBE


#define _XTAL_FREQ  8000000             // Set operation for 8 Mhz


short Nec_OK = 0;
char Nec_Button;


char array1[21]={0xa2, };
char txt1[21][4] ={"CH-\0", "CH+\0","VL+\0"," 0 \0"};
int color[21]={RD};



void main()
{
    init_UART();
    OSCCON = 0x70;                          // 8 Mhz
    nRBPU = 0;                              // Enable PORTB internal pull up resistor
    TRISB = 0x01;
    TRISC = 0x00;                           // PORTC as output
    TRISD = 0x00;
    ADCON1 = 0x0F;                          //
 
    Initialize_LCD_Screen();
    Init_Interrupt();

    Nec_code = 0x0;                         // Clear code
    
    while(1)
    {
         if (Nec_OK == 1)
        {
            Nec_OK = 0;
            Enable_INT_Interrupt();
            printf ("NEC_Button = %x \r\n", Nec_Button);  

            char found = 0xff;
            
            // add code here to look for code using a FOR loop
            
            if (found != 0xff) 
            {
				printf ("Key Location = %d \r\n\n", found);      
                fillCircle(Circle_X, Circle_Y, Circle_Size, color[found]); 
                drawCircle(Circle_X, Circle_Y, Circle_Size, ST7735_WHITE);  
                drawtext(Text_X, Text_Y, txt1[found], ST7735_WHITE, ST7735_BLACK,TS_1);
   
			// add code to output color for the RGB LEDS
			
			// add code to handle the KEY_PRESSED LED and do the buzzer sound
            
            }
        }
    }
}


