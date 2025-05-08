//#include <stdio.h>
//#include <stdlib.h>
//#include <xc.h>
//#include <math.h>
//#include <p18f4620.h>
//#include <usart.h>
//#include <string.h>
//#include "main.h"
//#include "ST7735_TFT.h"
//#include "Interrupt.h"
//#include "utils.h"
//#include "Main_Screen.h"
//#include "I2C.h"
//#include "I2C_Support.h"
//#include "Fan_Support.h"
//
//
//#pragma config OSC = INTIO67
//#pragma config WDT = OFF
//#pragma config LVP = OFF
//#pragma config BOREN = OFF
//#pragma config CCP2MX = PORTC
//
//char tempSecond = 0xff;
//char second = 0x00;
//char minute = 0x00;
//char hour = 0x00;
//char dow = 0x00;
//char day = 0x00;
//char month = 0x00;
//char year = 0x00;
//char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
//char alarm_second, alarm_minute, alarm_hour, alarm_date;
//char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;
//
//#define _XTAL_FREQ  8000000             // Set operation for 8 Mhz
//
//short Nec_OK = 0;
//char Nec_Button;
//unsigned long long Nec_code = 0;
//
//int rpm;
//// PART A
//char duty_cycle = 100;
////char duty_cycle = 50;
//char FAN;
//
//
//// char array obtained thru find
//char array1[21]=
//{
//    0xa2, 0x62, 0xe2, 
//    0x22, 0x02, 0xc2, 
//    0xe0, 0xa8, 0x90,  
//    0x68, 0x98, 0xb0, 
//    0x30, 0x18, 0x7a, 
//    0x10, 0x38, 0x5a, 
//    0x42, 0x4a, 0x52
//};
//// controller characters
//char txt1[21][4] =
//{   
//    "CH-\0", "CH \0","CH+\0",
//    "|<<\0",">>|\0",">||\0",
//    " - \0"," + \0","EQ \0",
//    " 0 \0", "100\0","200\0",
//    " 1 \0"," 2 \0"," 3 \0",
//    " 4 \0"," 5 \0"," 6 \0",
//    " 7 \0"," 8 \0"," 9 \0"
//};
//
//// colors for LCD
//int color[21]=
//{
//    RD,RD,RD,
//    BU,BU,GR,
//    BU,BU,MA,
//    BK,BK,BK,
//    BK,BK,BK,
//    BK,BK,BK,
//    BK,BK,BK
//};
//
//// colors d1-d3 are RGBs setups
//int colord1[21] = 
//{
//    RED, RED, RED, 
//    CYAN, CYAN, GREEN, 
//    OFF, OFF, OFF, 
//    OFF, OFF, OFF, 
//    OFF, OFF, OFF, 
//    OFF, OFF, OFF, 
//    OFF, OFF, OFF
//};
//
//int colord2[21] = 
//{
//    OFF, OFF, OFF, 
//    OFF, OFF, OFF, 
//    BLUE, BLUE, PURPLE, 
//    WHITE, WHITE, WHITE, 
//    OFF, OFF, OFF, 
//    OFF, OFF, OFF, 
//    OFF, OFF, OFF
//};
//
//int colord3[21] = 
//{
//    OFF, OFF, OFF, 
//    OFF, OFF, OFF, 
//    OFF, OFF, OFF, 
//    OFF, OFF, OFF, 
//    WHITE, WHITE, WHITE, 
//    WHITE, WHITE, WHITE, 
//    WHITE, WHITE, WHITE
//};
//
//
//void main()
//{
//    Init_UART();
//    OSCCON = 0x70;                           // 8 Mhz
//    nRBPU  = 0;                              // Enable PORTB internal pull up resistor
//    TRISA  = 0x00;
//    TRISB  = 0x02;                           // mask first 2 bits
//    TRISC  = 0x01;                           // PORTC as output
//    TRISD  = 0x00;
//    TRISE  = 0x00;
//    
//    T3CON  = 0x03;
//    T3CONbits.TMR3ON = 1;
//    
//    ADCON1 = 0x0F;     
//    
//    Nec_code = 0x0;                         // Clear code
//
//    Initialize_LCD_Screen();
//    I2C_Init(100000);
//    DS1621_Init();
//    Init_Interrupt();    
//    
//    do_update_pwm(duty_cycle);
//    
//    FAN_EN = 1;
//    FAN_PWM = 1;
//    char duty_cycle = 100;
//    
//    while (1)
//    {
//        DS3231_Read_Time();
//        
//        if(tempSecond != second)
//        {
//            tempSecond = second;
//            signed char tempC = DS1621_Read_Temp();
//            signed char tempF = (tempC * 9 / 5) + 32;
//            //printf ("%02x:%02x:%02x %02x/%02x/%02x",hour,minute,second,month,day,year);
//            //printf (" Temperature = %d degreesC = %d degreesF\r\n\n", tempC, tempF);
//            
//            rpm = get_RPM();
//            printf ("RPM = %d dc = %d\r\n\n", rpm, duty_cycle); 
//        }
//
//        
//        if (Nec_OK == 1)
//        {
//            Nec_OK = 0;
//            
//            printf ("NEC_Button = %x \r\n", Nec_Button);  
//            Enable_INT_Interrupt();
//            
//            char found = 0xff;
//            
//            for( int i = 0; i < 21; i++ )               // Loop 21 times
//            {
//                if( Nec_Button == array1[i] )           // If Nec_Button is equal to content of array
//                {                                       // then
//                    found = i;                          // Return matching index
//                }
//            }
//            if (found == 5)                //If the button pressed is the play/pause button
//            {
//                Do_Beep();                      //Beep
//                printf ("button = %d \r\n", found);
//                Toggle_Fan();                   //Turn off/on fan
//            }
//            else if (found == 7)                 //If the button pressed is the + button
//            {
//                Do_Beep();                      //Beep
//                printf ("button = %d \r\n", found);
//                Increase_Speed();               //add 5 to duty_cycle
//            }
//            else if (found == 6)                  //If the button pressed is the - button
//            {
//                Do_Beep();                        //Beep
//                printf ("button = %d \r\n", found);//Print found button
//                Decrease_Speed();                 //subtract 5 from duty_cycle
//            }
//            else if (found==9)
//            {
//                DS3231_Setup_Time();
//            }
//        }
//    }
//}
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
#include "I2C.h"
#include "I2C_Support.h"
#include "Fan_Support.h"

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF
#pragma config CCP2MX = PORTC

char tempSecond = 0xff;
char second = 0x00;
char minute = 0x00;
char hour = 0x00;
char dow = 0x00;
char day = 0x00;
char month = 0x00;
char year = 0x00;
char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
char alarm_second, alarm_minute, alarm_hour, alarm_date;
char setup_alarm_second, setup_alarm_minute, setup_alarm_hour; 


#define _XTAL_FREQ  8000000             // Set operation for 8 Mhz


short Nec_OK = 0;
char Nec_Button;
unsigned long long Nec_code;

char array1[21]={0xa2,0x62,0xe2,0x22,0x02,0xc2,0xe0,0xa8,0x90,0x68,0x98,0xb0,0x30,0x18,0x7a,0x10,0x38,0x5a,0x42,0x4a,0x52};
char txt1[21][4] ={"CH-\0"," CH\0","CH+\0","|<<\0",">>|\0",">||\0","VL-\0","VL+\0"," EQ\0"," 0 \0","100\0","200\0"," 1 \0"," 2 \0"," 3 \0"," 4 \0"," 5 \0", " 6 \0"," 7 \0"," 8 \0", " 9 \0"};
int color[21]={RD, RD, RD, BU, BU, GR, MA, MA, MA, WH, WH, WH, WH, WH, WH, WH, WH, WH ,WH, WH, WH};

char duty_cycle = 50;
char FAN = 0;
int rpm;
    

void Initialization()
{
    Init_UART();
    OSCCON = 0x70;                          // 8 Mhz
    nRBPU = 0;                              // Enable PORTB internal pull up resistor
    TRISA = 0x00;                           //set output
    TRISB = 0x02;
    TRISC = 0x01;                           // RC0 input, rest as output
    TRISD = 0x00;
    TRISE = 0x00;
    ADCON1 = 0x0F;                          //
    PORTA = 0;
    PORTB = 0;
    PORTE = 0;

    TMR3L = 0;
 
    Initialize_LCD_Screen();
    I2C_Init(100000);
    DS1621_Init(); 
    Init_Interrupt();
    T3CON = 0x02;
    Nec_code = 0x00;                         // Clear code
    FAN_EN = 0;
    FAN_PWM = 1;
    FAN_MODE = 0;
    do_update_pwm(duty_cycle); 
   
    T3CONbits.TMR3ON = 1;
}
//
//void da_loop()
//{
//    Initialization();
//    printf("\n\n\nINSIDE THE LOOP BABY\r\n\n\n");
//    
//    
//    
//    while(1)
//    {
//        signed char tempC = DS1621_Read_Temp();
//        toggle_fan_mode(tempC);
//        signed char tempC = DS1621_Read_Temp();
//        signed char tempF = (tempC * 9 / 5) + 32; // read and convert temperature data to C and F
//        
//        printf(" Temperature = %d degreesC = %d degreesF\r\n\n", tempC, tempF);
//        printf("RPM = %d dc = %d \r\n\n", rpm, duty_cycle);
//        
//        Wait_One_Sec();
//        
//        if (Nec_OK == 1)
//        {
//            Nec_OK = 0;
//            Enable_INT_Interrupt();
//            printf("NEC_Button = %x \r\n", Nec_Button); 
//
//            char found = 0xff;                                                                       // new variable to determine the column each button belongs to on our remote
//            
//            for(char i = 0; i<21; i++)                                                          // Filters through entire array which represents every button
//            {
//                if(Nec_Button == array1[i])                                                      // Checks to find a corresponding button
//                {
//                    found = i;
//                    i = 21;
//                }
//            }
//            
//            if (found != 0xff) 
//            {
//                KEY_PRESSED = 1;
//                Do_Beep();                                       // Audio cue of button being registered
//                KEY_PRESSED = 0;
//                
//                if(found == 1) 
//                { 
//                    FAN_MODE = !FAN_MODE;
//                    exit();
//                }
//                if(found == 5) Toggle_Fan();
//                if(found !=5 | found != 1) exit();
//            }  
//        }
//    }
//    
//}

void main()
{
    Initialization();
    
    printf("\n\n\nMAIN FUNCTION\n\n\n");
    while(1)
    {
        DS3231_Read_Time(); //reads rtc
        if(tempSecond != second) 
        {
            tempSecond = second;
            signed char tempC = DS1621_Read_Temp();
            signed char tempF = (tempC * 9 / 5) + 32; // read and convert temperature data to C and F
            char duty_upper = duty_cycle / 10;
            rpm = get_RPM();
            Set_DC_RGB(duty_upper);
            Set_RPM_RGB(rpm);
            Set_Temp_RGB(tempC);
            toggle_fan_mode(tempC);

            printf ("%02x:%02x:%02x %02x/%02x/%02x",hour,minute,second,month,day,year);
            printf (" Temperature = %d degreesC = %d degreesF\r\n\n", tempC, tempF);
            printf("RPM = %d dc = %d \r\n\n", rpm, duty_cycle);
        }

         if (Nec_OK == 1)
        {
            Nec_OK = 0;
            Enable_INT_Interrupt();
            printf("NEC_Button = %x \r\n", Nec_Button); 

            char found = 0xff;                                                                       // new variable to determine the column each button belongs to on our remote
            
            for(char i = 0; i<21; i++)                                                          // Filters through entire array which represents every button
            {
                if(Nec_Button == array1[i])                                                      // Checks to find a corresponding button
                {
                    found = i;
                    i = 21;
                }
            }
            
            
            if (found != 0xff) 
            {
                //display information on LCD
				printf ("Key Location = %d \r\n\n", found);      
                fillCircle(Circle_X, Circle_Y, Circle_Size, color[found]); 
                drawCircle(Circle_X, Circle_Y, Circle_Size, ST7735_WHITE);  
                drawtext(Text_X, Text_Y, txt1[found], ST7735_WHITE, ST7735_BLACK,TS_1);
                
                KEY_PRESSED = 1;
                Do_Beep();                                       // Audio cue of button being registered
                KEY_PRESSED = 0;
                
                if(found == 1) FAN_MODE = (!FAN_MODE);   
                if(found == 5) Toggle_Fan();
                if(found == 6) Decrease_Speed();                 // - button pressed, decrease speed
                if(found == 7) Increase_Speed();                 // + button pressed, increase speed
                if(found == 8) DS3231_Setup_Time();              // EQ button pressed, resets time
                
            }  
        }
    }
}