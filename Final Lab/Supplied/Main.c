#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>
#include <string.h>

#include "I2C.h"
#include "I2C_Support.h"
#include "Interrupt.h"
#include "Fan_Support.h"
#include "Main.h"
#include "ST7735_TFT.h"
#include "utils.h"

#pragma config OSC = INTIO67
#pragma config BOREN =OFF
#pragma config WDT=OFF
#pragma config LVP=OFF


void Set_D1_RGB(char);
void Set_D2_RGB(int);

void Test_Alarm();

char second = 0x00;
char minute = 0x00;
char hour = 0x00;
char dow = 0x00;
char day = 0x00;
char month = 0x00;
char year = 0x00;

char found;
char tempSecond = 0xff; 
signed char tempC, tempF;
char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
char alarm_second, alarm_minute, alarm_hour, alarm_date;
char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;
unsigned char fan_set_temp = 16;
unsigned char Nec_state = 0;
float volt;
char INT0_flag, INT1_flag, INT2_flag;


short Nec_ok = 0;
char Nec_Button;
char FAN;
char duty_cycle;
int rps;
int rpm;
int ALARMEN;
int alarm_mode, MATCHED,color;
char FANMODE = 0;


char buffer[31]     = "ECE3301L FinSP24/1/02 ";
char *nbr;
char *txt;
char tempC_Txt[]    = "+28";
char tempF_Txt[]    = "+82";
char time_Txt[]     = "08:18:25";
char date_Txt[]     = "05/07/24";
char alarm_time_Txt[] = "00:00:00";
char Alarm_SW_Txt[] = "OFF";
char Fan_Set_Temp_Txt[] = "16C";
char Fan_Mode_Txt[]  = "MANU";              // text storage for Fan Mode
char Fan_SW_Txt[]   = "OFF";                // text storage for Fan On/Off
char array1[21]={0xa2,0x62,0xe2,0x22,0x02,0xc2,0xe0,0xa8,0x90,0x68,0x98,0xb0,0x30,0x18,0x7a,0x10,0x38,0x5a,0x42,0x4a,0x52};

    
char DC_Txt[]       = "000";                // text storage for Duty Cycle value
char Volt_Txt[]     = "0.00V";
char RTC_ALARM_Txt[]= "0";                  //
char RPM_Txt[]      = "0000";               // text storage for RPM

char setup_time[]       = "00:00:00";
char setup_date[]       = "05/07/24";
char setup_alarm_time[] = "00:00:00"; 
char setup_fan_set_text[]   = "16C";



void Do_Init()                      // Initialize the ports 
{ 
    Init_UART();                    // Initialize the uart
    Init_ADC();
    OSCCON=0x70;                    // Set oscillator to 8 MHz 
    
    ADCON1=0x0E;
    TRISA = 0x01;
    TRISB = 0x07;
    TRISC = 0x01;
    TRISD = 0x00;
    TRISE = 0x00;
    PORTE = 0x00;

    FAN = 0;
    duty_cycle =100;
    RBPU=0;

    I2C_Init(100000); 

    DS1621_Init();
    Init_Interrupt();
    Turn_Off_Fan();
    fan_set_temp = 16;
}


void main() 
{
    Do_Init();                                                  // Initialization  
    Initialize_Screen();

    TMR3L = 0x00;                   
    T3CON = 0x03;
    DS3231_Turn_Off_Alarm();                                    
    DS3231_Read_Alarm_Time();                                   // Read alarm time for the first time

    tempSecond = 0xff;
    
    while (1)
    {
        DS3231_Read_Time(); 

        if(tempSecond != second)
        {
            tempSecond = second;
            rpm = get_RPM();            

            volt = Read_Volt(0);
            tempC = DS1621_Read_Temp();
            tempF = (tempC * 9 / 5) + 32;
            Set_D1_RGB(duty_cycle);
            Set_D2_RGB(rpm);
            
            printf ("%02x:%02x:%02x %02x/%02x/%02x",hour,minute,second,month,day,year);
            printf (" Temp = %d C = %d F", tempC, tempF);
            printf (" alarm = %d match = %d", RTC_ALARM_NOT, MATCHED);
            printf (" RPM = %d  dc = %d\r\n", rpm, duty_cycle);
            Monitor_Fan();
            Test_Alarm();
            Update_Screen();
        }
        
        if (check_for_button_input() == 1)
        {
            printf ("button\r\n");
            Nec_ok = 0;
            switch (found)
            {
                case Ch_Minus:        
                    Do_Beep_Good();
                    Do_Setup_Time();
                    break;
                
                case Channel:           
                    Do_Beep_Good();
                    Do_Setup_Alarm_Time();
                    break;
                    
                case Ch_Plus:
                    Do_Beep_Good();
                    Setup_Temp_Fan();            
                    break;
                    
                case Play_Pause:
                    Do_Beep_Good();
                    Toggle_Fan();
                    break;           

                case Minus: 
                    Do_Beep_Good();                    
                    Decrease_Duty_Cycle();
                    break;
                    
                case Plus:
                    Do_Beep_Good();                    
                    Increase_Duty_Cycle();
                    break;
                    
                case Zero_Button:
                    if (FANMODE == 0) 
                    {
                        FAN_MODE = 1;
                        FANMODE = 1;
                    }
                    else 
                    {
                        FAN_MODE = 0;
                        FANMODE = 0;
                    }
                    Do_Beep_Good();
                    break;
                    
                default:
                    Do_Beep_Bad();
                    break;
            }
        }    
        if (INT0_flag == 1)
        {
            INT0_flag = 0;
            if (ALARMEN == 0) ALARMEN = 1;
            else ALARMEN = 0;
        }
        
    }
}
void Test_Alarm()
{
    if (alarm_mode == 0 && ALARMEN == 1)        
    {
        DS3231_Turn_On_Alarm();
        alarm_mode = 1;
    }
    else if (alarm_mode == 1 && ALARMEN == 0)        
    {
        DS3231_Turn_Off_Alarm();
        alarm_mode = 0;
       
        PORTE = 0x00; 
        Deactivate_Buzzer();         
    }    
    else if (alarm_mode == 1 && ALARMEN == 1)        
    {
        if(RTC_ALARM_NOT == 0)
        {
            MATCHED = 1;
            Activate_Buzzer(); 
        }
        else 
        {
            MATCHED = 0;
            Deactivate_Buzzer();
            color = 0;
            Set_RGB_Color(0);
             do_update_pwm(duty_cycle);
        }
            if (MATCHED == 1)
            {
                Set_RGB_Color(color++);
                if(color > 7) color = 0;               
                volt = Read_Volt(0);
                
                if(volt > 3)
                {
                    MATCHED = 0;
                    color = 0;
                    Set_RGB_Color(0);
                    Deactivate_Buzzer();
                    do_update_pwm(duty_cycle);
                    alarm_mode = 0;
                }
            }        
    }
}
void Set_D1_RGB(char duty_cycle)
{
    char color = (duty_cycle >= 70) ? 7 : duty_cycle / 10;      // DC RGB color based off duty cycle
    PORTA = (PORTA & 0xF1) | (color << 1);
}

void Set_D2_RGB(int rpm)
{
    
    PORTB = PORTB & 0x40; 					// Preserve bit 6 of PORTB and clear other bits
    if (rpm > 0 && rpm < 3000) 
    {
        PORTB = (PORTB & 0x40) | ((D2[rpm / 500]) << 3); 	// Set RGB color based on D2 array and shift left by 3 
    }
    else if (rpm >= 3000)
    {
        PORTB = (PORTB & 0x40) | 0x38; 				// Set RGB to maximum value when RPM is 3000 or more
    }
   
    
}



