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
#include "Interrupt.h"

#define NSPED_SW PORTAbits.RA3
#define NSLT_SW PORTAbits.RA5
#define EWPED_SW PORTAbits.RA4
#define EWLT_SW PORTCbits.RC0

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
void Do_Flashing(void);

#define _XTAL_FREQ  8000000             // Set operation for 8 Mhz
#define TMR_CLOCK   _XTAL_FREQ/4        // Timer Clock 2 Mhz

char dir;
char Count;                             // RAM variable for Second Count
char PED_Count;                         // RAM variable for Second Pedestrian Count

char  MODE;
char direction;
float volt;

extern char Light_Sensor;

char INT0_Flag;
char INT1_Flag;
char INT2_Flag;

char NS_PED_SW = 0;
char EW_PED_SW = 0;

char Flashing_Request = 0;
char Flashing_Status = 0;


void main(void)
{
    OSCCON = 0x70;                      // set the system clock to be 1MHz 1/4 of the 4MHz
    TRISA  = 0xF9;                       //inputs RA0/AN0-RA4 inputs,RA5 output
    TRISB  = 0x07;                       // outputs
    TRISC  = 0x01;                       // set PORTC as outputs
    TRISD  = 0x00;                       // set PORTD as outputs
    TRISE  = 0x00;                       // set PORTE as outputs
    Init_ADC();
    Init_UART();

    Initialize_LCD_Screen();                        // Initialize the TFT screen

    RBPU = 0;
    init_INTERRUPT();                       // check this?
    
    while(1)
    {
        // Process sensor readings and switch between day and night modes
        volt = Read_Volt(0); // Read the voltage or light sensor value
        Light_Sensor = volt < 2.5 ? 1 : 0; // Determine the mode based on sensor value

        if (volt < 2.5)
        {
            MODE = 1; // Day mode
            MODE_LED = 1;
            Day_Mode();
        }
        else
        {
            MODE = 0; // Night mode
            MODE_LED = 0;
            Night_Mode();
        }
        
        if (Flashing_Request == 1)
            
        {
            Flashing_Request = 0;
            Flashing_Status = 1;
            Do_Flashing();
         }
        
        if (INT0_Flag == 1)
        {
            INT0_Flag = 0; // Clear the flag
            printf("INT0 interrupt pin detected \r\n");
            // Handle INT0 interrupt (e.g., call a function or update state)
        }

        if (INT1_Flag == 1)
            
        {
            INT1_Flag = 0; // Clear the flag
            printf("INT1 interrupt pin detected \r\n");
            // Handle INT1 interrupt
        }

        if (INT2_Flag == 1)
            
        {
            INT2_Flag = 0; // Clear the flag
            printf("INT2 interrupt pin detected \r\n");
            // Handle INT2 interrupt
        }
       
    }
        
    
}

//    while (1)
//    {
//        // Do nothing,
//        if (INT0_Flag == 1)
//        {
//            INT0_Flag = 0; // clear the flag
//            printf("INT0 interrupt pin detected \r\n");
//            // print a message that INT0 has occurred
//        }
//        if (INT1_Flag == 1)
//        {
//            INT1_Flag = 0; // clear the flag
//            printf("INT1 interrupt pin detected \r\n");
//            // print a message that INT1 has occurred
//        }
//        if (INT2_Flag == 1)
//        {
//            INT2_Flag = 0; // clear the flag
//            printf("INT2 interrupt pin detected \r\n");
//            // print a message that INT2 has occurred
//        }
//    }


// while (1)
// {
//      Rcmd2red();
//      delay_ms(2);       
// }
//    volt = Read_Volt(0);                        // 
//
//    Light_Sensor = volt < 2.5 ? 1:0;                 // Mode = 1, Day_mode, Mode = 0 Night_mode
//
//    while(1)                                    // forever loop
//    {
//
//        if (Light_Sensor == 1)    
//        {
//            Day_Mode();                         // calls Day_Mode() function
//        }
//        else
//        {
//            EW_PED_SW = 0;
//            NS_PED_SW = 0;
//            Night_Mode();                       // calls Night_Mode() function
//        }
//     
//    } 
//}

void Set_NS(char color)                                               //This is the D1 LED
{
    direction = NS;
    update_LCD_color(direction, color);
    
    switch (color)
    {
        case OFF:       NS_RED = 0;        NS_GREEN = 0; break;       // Turns off the NS LED
        case RED:       NS_RED = 1;        NS_GREEN = 0; break;       // Sets NS LED RED
        case GREEN:     NS_RED = 0;        NS_GREEN = 1; break;       // sets NS LED GREEN
        case YELLOW:    NS_RED = 1;        NS_GREEN = 1; break;       // sets NS LED YELLOW
    }
}
void Set_NSLT(char color)                                             //This is the D3 LED
{
    direction = NSLT;
    update_LCD_color(direction, color);
    
    switch (color)
    {
        case OFF:       NSLT_RED = 0;      NSLT_GREEN = 0; break;     // Turns off the NSLT LED
        case RED:       NSLT_RED = 1;      NSLT_GREEN = 0; break;     // Sets NSLT LED RED
        case GREEN:     NSLT_RED = 0;      NSLT_GREEN = 1; break;     // sets NSLT LED GREEN
        case YELLOW:    NSLT_RED = 1;      NSLT_GREEN = 1; break;     // sets NSLT LED YELLOW
    }
}
void Set_EW(char color)                                               //This is the D2 LED
{
    direction = EW;
    update_LCD_color(direction, color);
    
    switch (color)
    {
        case OFF:      EW_RED = 0;        EW_GREEN = 0; break;        // Turns off the EW LED
        case RED:      EW_RED = 1;        EW_GREEN = 0; break;        // Sets EW LED RED
        case GREEN:    EW_RED = 0;        EW_GREEN = 1; break;        // sets EW LED GREEN
        case YELLOW:   EW_RED = 1;        EW_GREEN = 1; break;        // sets EW LED YELLOW
    }
}
void Set_EWLT(char color)                                             //This is the D4 LED
{
    direction = EWLT;
    update_LCD_color(direction, color);
    
    switch (color)
    {
        case OFF:     EWLT_RED = 0;      EWLT_GREEN = 0; break;       // Turns off the EWLT LED
        case RED:     EWLT_RED = 1;      EWLT_GREEN = 0; break;       // Sets EWLT LED RED
        case GREEN:   EWLT_RED = 0;      EWLT_GREEN = 1; break;       // sets EWLT LED GREEN
        case YELLOW:  EWLT_RED = 1;      EWLT_GREEN = 1; break;       // sets EWLT LED YELLOW
    }
}

void PED_Control(char direction, char Num_Sec)
{ 
    for(char i = Num_Sec - 1; i>0 ; i--)
    {
        update_LCD_PED_Count(direction, i);
        Wait_One_Second_With_Beep();
    }
    update_LCD_PED_Count(direction, 0);
    Wait_One_Second_With_Beep();
    
    if (direction == EW)
    {
        EW_PED_SW = 0;
    }
    if (direction == NS)
    {
        NS_PED_SW = 0;
    }
}

void Day_Mode()
{
    MODE_LED = 1;
    MODE = 1;
       
    Set_EW(RED);                                    //Set East-West LED to red
    Set_EWLT(RED);                                  //Set East-West left turn LED to red
    Set_NSLT(RED);                                  //Set North-south left turn LED to red
    Set_NS(GREEN);                                  //Set North-South LED to green  

    if (NS_PED_SW == 1)                              
    {
        PED_Control(0, PEDESTRIAN_NS_WAIT);                          //Part 1a
    }
    Wait_N_Seconds(NS_WAIT);                              //Part 2
    
    Set_NS(YELLOW);                                 //Part 3
    Wait_N_Seconds(3);
    
    Set_NS(RED);                                    //Part 4

    if (EWLT_SW == 1)                               //Part 5
    {
        Set_EWLT(GREEN);                            //Part 6
        Wait_N_Seconds(EW_LT_WAIT);                          

        Set_EWLT(YELLOW);                           //Part 7
        Wait_N_Seconds(3);

        Set_EWLT(RED);                              //Part 8
    }

    Set_EW(GREEN);                                  //Part 9

    if (EW_PED_SW == 1)
    {
        PED_Control(EW, PEDESTRIAN_EW_WAIT);                          //Part 9a
    }

    Wait_N_Seconds(EW_WAIT);                              //Part 10

    Set_EW(YELLOW);                                 //Part 11
    Wait_N_Seconds(3);

    Set_EW(RED);                                    //Part 12

    if (NSLT_SW == 1)                               //Part 13
    {
        Set_NSLT(GREEN);                            //Part 14
        Wait_N_Seconds(NS_LT_WAIT);
        
        Set_NSLT(YELLOW);                           //Part 15
        Wait_N_Seconds(3);

        Set_NSLT(RED);                              //Part 16
    }
                                                    //Day_Mode() Finished
}

void Night_Mode()
{ 
    MODE_LED = 0;
    MODE = 0;
    
    NS_PED_SW = 0;
    EW_PED_SW = 0;
    
    Set_NSLT(RED);                                  //Set North-south left turn LED to red
    Set_EW(RED);                                    //Set East-West LED to red
    Set_EWLT(RED);                                  //Set East-west left turn LED to red  
    Set_NS(GREEN);                                  //Set North-South LED to green  

    Wait_N_Seconds(NIGHT_NS_WAIT);                              //Part 2

    Set_NS(YELLOW);                                 //Part 3
    Wait_N_Seconds(3);

    Set_NS(RED);                                    //Part 4

    if (EWLT_SW == 1)                               //Part 5
    {
        Set_EWLT(GREEN);                            //Part 6
        Wait_N_Seconds(NIGHT_EW_LT_WAIT);

        Set_EWLT(YELLOW);                           //Part 7
        Wait_N_Seconds(3);

        Set_EWLT(RED);                              //Part 8
    }
    
    Set_EW(GREEN);                                  //Part 9
    Wait_N_Seconds(NIGHT_EW_WAIT);  

    Set_EW(YELLOW);                                 //Part 10
    Wait_N_Seconds(3);

    Set_EW(RED);                                    //Part 11
    

    if (NSLT_SW == 1)                               //Part 12
    {
        Set_NSLT(GREEN);                            //Part 13
        Wait_N_Seconds(NIGHT_NS_LT_WAIT);

        Set_NSLT(YELLOW);                           //Part 14
        Wait_N_Seconds(3);

        Set_NSLT(RED);                              //Part 15
    }
                                                    //End Night_Mode()
}
void Wait_N_Seconds (char seconds)
{
    char I;
    for (I = seconds; I > 0; I--)
    {
		update_LCD_count(direction, I);
        Wait_One_Second();          			// calls Wait_One_Second for x number of times
    }
	update_LCD_count(direction, 0);
}

void Do_Flashing() 
{
    while (Flashing_Status == 1) 
    {
        if (Flashing_Request == 0) 
        {
          
            Set_NS(RED);
            Set_EW(RED);
            Set_NSLT(RED); 
            Set_EWLT(RED); 

            Wait_One_Second();

            Set_NS(OFF);
            Set_EW(OFF);
            Set_NSLT(OFF); 
            Set_EWLT(OFF); 

            Wait_One_Second();
            
        } 
        
        else 
        {
            Flashing_Request = 0;
            Flashing_Status = 0;
        }
    }
}