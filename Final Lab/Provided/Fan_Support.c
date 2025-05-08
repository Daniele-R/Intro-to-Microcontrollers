#include <p18f4620.h>
#include "Main.h"
#include "Fan_Support.h"
#include "stdio.h"

extern char FAN;
extern char FANMODE;
extern char duty_cycle;
extern char fan_set_temp;
extern signed char tempC;


int get_duty_cycle(int temp, int set_temp)
{	
// copy your code here
}

void Monitor_Fan()
{
// copy your code here
}

int get_RPM()
{
// copy your old code here
}

void Toggle_Fan()
{
// copy your old code here
}

void Turn_Off_Fan()
{
    printf ("Fan is turned off\r\n");
    FAN = 0;
    FAN_EN = 0;      

}

void Turn_On_Fan()
{
    printf ("Fan is turned on\r\n");
    FAN = 1;
    FAN_EN = 1;    
    do_update_pwm(duty_cycle);
}


void Increase_Duty_Cycle()
{
// copy your code here
}

void Decrease_Duty_Cycle()
{
// copy your code here
}




