#include <p18f4620.h>
#include "Main.h"
#include "Fan_Support.h"
#include "stdio.h"
#include "Main_Screen.h"

extern char FAN;
extern char duty_cycle;
char D2_color[] = {0x08, 0x10, 0x18, 0x20, 0x28, 0x30};

int get_RPM()
{
 int RPS = TMR3L / 2; // read the count. Since there are 2 pulses per rev
 // then RPS = count /2
TMR3L = 0; // clear out the count
return (RPS * 60); // return RPM = 60 * RPS
} 


void Toggle_Fan()
{
    FAN = (!FAN);                                       //when pressed, fan switches from 0 to 1 or vice versa
    if(FAN == 1) Turn_On_Fan();                          //when fan 1 call turn on fan
    else Turn_Off_Fan();    
}

void Turn_Off_Fan()
{
    FAN = 0;                                            //fan = 0
    FAN_EN = 0;                                         //Fan now disabled
}

void Turn_On_Fan()
{
    FAN = 1;                                            //fan = 1
    do_update_pwm(duty_cycle);                          //update duty cycle for proper fan setup
    FAN_EN = 1;                                         //fan now enabled
}

void Increase_Speed()
{
    if(duty_cycle == 100)                               //when duty cycle 100 we can no longer increase
    {                                                   //two beeps done for audio cue
        Do_Beep();
        Do_Beep();
        do_update_pwm(duty_cycle);
    }     
    else                                                //when duty cycle is not 100 normal operation ensues
    {
        duty_cycle += 5;                                //increase by 5 for each call to increase
        do_update_pwm(duty_cycle);                      //update pwm for proper operation
    }


}

void Decrease_Speed()
{
    if(duty_cycle == 0)                                 //when duty cycle 0 we can no longer decrease
    {                                                   //two beeps for an audio cue
        Do_Beep();
        Do_Beep();
        do_update_pwm(duty_cycle);
    }     
    else                                                //when duty cycle is not 0 we can do normal operation
    {
        duty_cycle -= 5;                                //decrease by 5 for each call to increase
        do_update_pwm(duty_cycle);                      //update pwm for proper operation
    }
}

void do_update_pwm(char duty_cycle) 
{
    float dc_f;
    int dc_I;
    PR2 = 0b00000100 ; // set the frequency for 25 Khz
    T2CON = 0b00000111 ; //
    dc_f = ( 4.0 * duty_cycle / 20.0) ; // calculate factor of duty cycle versus a 25 Khz
    // signal
    dc_I = (int) dc_f; // get the integer part
    if (dc_I > duty_cycle) dc_I++; // round up function
    CCP1CON = ((dc_I & 0x03) << 4) | 0b00001100;
    CCPR1L = (dc_I) >> 2;
} 

void Set_DC_RGB(int duty_upper)
{
    PORTA = (PORTA & 0x10);
    if (duty_upper >= 1 && duty_upper <= 6)
    {
        PORTA = (PORTA & 0x10) | (duty_upper << 1);
    }
    else if(duty_upper >= 7)
    {
        PORTA = (PORTA & 0x10) | (0x07 << 1);
    }
}

void Set_RPM_RGB(int rpm)
{
    PORTB = PORTB & 0x40;
    if (rpm > 0 && rpm < 3000) 
    {
        PORTB = (PORTB & 0x40) | ((D2_color[rpm / 500]));
    }
    else if (rpm >= 3000)
    {
        PORTB = (PORTB & 0x40) | 0x38;
    }
}


void Set_Temp_RGB(signed char tempC)
{
    PORTE = OFF;
    signed char temp = (tempC - 14) / 2;
    if (temp > 0 && temp <= 6)
    {
        PORTE = temp;
    }
    else if(temp > 6)
    {
        PORTE = WHITE;
    }
}

void toggle_fan_mode(signed char tempC)
{
    if(FAN_MODE == 1) //automatic fan speed mode
    {
        FAN_EN = 1;  //makes sure fan is on
        signed char temp = (tempC - 15) / 2; //formula to get the duty cycle to change according to temp

        
        if (temp > 0 && temp <= 9)
        {
            duty_cycle = 10 * (temp); //duty cycle is correlated to temp by a factor of 10
            do_update_pwm(duty_cycle); //update pwm whenever duty cycle is changed
        }
        else if(temp > 9)
        {
            duty_cycle = 100;
            do_update_pwm(duty_cycle); //update pwm whenever duty cycle is changed
        }
        else if (tempC < 16)
        {
            duty_cycle = 0;
            do_update_pwm(duty_cycle);
        }
    }    
}


//void toggle_fan_mode(signed char tempC)
//{   
//    
//    if(FAN_MODE == 1 && FAN == 1 && FAN_EN == 1)
//    {            
//        printf("\n\nMODO DIABLO ACTIVATED\r\n\n");
//        
//        if (tempC < 16) 
//        {
//        duty_cycle = 0;
//        } 
//        else if (tempC > 16 && tempC <= 18) {
//        duty_cycle = 10;
//        } 
//        else if (tempC > 18 && tempC <= 20) {
//        duty_cycle = 20; 
//        } 
//        else if (tempC > 20 && tempC <= 22) {
//        duty_cycle = 30;
//        } 
//        else if (tempC > 22 && tempC <= 24) {
//        duty_cycle = 40;
//        } 
//        else if (tempC > 24 && tempC <= 26) {
//        duty_cycle = 50;
//        } 
//        else if (tempC > 26 && tempC <= 28) {
//        duty_cycle = 60; 
//        } 
//        else if (tempC > 28 && tempC <= 30) {
//        duty_cycle = 70;
//        } 
//        else if (tempC > 30 && tempC <= 32) {
//        duty_cycle = 80;
//        } 
//        else if (tempC > 32 && tempC <= 34) {
//        duty_cycle = 90;
//        } 
//        else if (tempC > 34) {
//        duty_cycle = 100; 
//        }
//    }
//    else
//    {
//        return;
//    }
//    do_update_pwm(duty_cycle); 
//}