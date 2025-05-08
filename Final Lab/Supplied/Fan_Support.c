#include <p18f4620.h>
#include "Main.h"
#include "Fan_Support.h"
#include "stdio.h"
#include "I2C_Support.h"


extern char FAN;
extern char FANMODE;
extern char duty_cycle;
extern char fan_set_temp;
extern signed char tempC;



int get_duty_cycle(int temp, int set_temp)
{	
    
    if(temp < set_temp) 
    {
        duty_cycle = 0;
    }
    else if(temp >= set_temp)
    {
        int diff_temp = (temp - 16);
        duty_cycle = diff_temp * 5;
        
        if(duty_cycle > 100)
        {
          duty_cycle = 100;   
        }
        
        
    }
    return duty_cycle;

}

void Monitor_Fan()
{
       
        if (FAN_MODE == 1) 
    { 
        FAN_EN = 1;
         
        if (tempC < fan_set_temp)     //ambient temp is less than set temp
        {
            FAN_EN = 0;
            duty_cycle = 0; //if duty cycle needs to be 0 in off (but it wont allow manual change)
            do_update_pwm(duty_cycle); 
        } 
        
        else                            //ambient temp is greater than set temp
        {
            duty_cycle = get_duty_cycle(tempC, fan_set_temp); // Calculate duty cycle based on temperatures
            do_update_pwm(duty_cycle); // Apply the new duty cycle
            Turn_On_Fan();  // Activate the fan
            int rpm = get_RPM(); // Measure RPM of the fan
        }
        
       
    }
 
    
}  
    

int get_RPM()
{
     int RPM = TMR3L / 2; // read the count. Since there are 2 pulses per rev
    // then RPS = count /2
    TMR3L = 0; // clear out the count
    return (RPM * 60); // return RPM = 60 * RPS
}

void Toggle_Fan()
{
    FAN = !FAN;
    if (FAN == 1) Turn_On_Fan();
    else Turn_Off_Fan();
}

void Turn_Off_Fan()
{
    printf ("Fan is turned off\r\n");
   
    FAN = 0;
    FAN_EN = 0;   
    FAN_MODE = 0;
   // PORTCbits.RC0 =0;

}

void Turn_On_Fan()
{
    printf ("Fan is turned on\r\n");
    FAN = 1;
    do_update_pwm(duty_cycle);
    FAN_EN = 1;    
   
}


void Increase_Duty_Cycle()
{
    if(duty_cycle == 100)
    {
        Do_Beep();
        Do_Beep();
        do_update_pwm(duty_cycle);
    }
    else
    {
        duty_cycle += 5;
        do_update_pwm(duty_cycle);
    }
}


void Decrease_Duty_Cycle()
{
    if(duty_cycle == 0)
    {
        Do_Beep();
        Do_Beep();
        do_update_pwm(duty_cycle);
    }
    else
    {
        duty_cycle -= 5;
        do_update_pwm(duty_cycle);
    }
}



