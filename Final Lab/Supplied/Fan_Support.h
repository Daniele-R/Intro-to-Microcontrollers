int get_duty_cycle(int , int );
int get_RPM();
void Toggle_Fan();
void Turn_Off_Fan();
void Turn_On_Fan();
void Increase_Duty_Cycle();
void Decrease_Duty_Cycle();
void do_update_pwm(char) ;
void Set_DC_RGB(int);
void Set_RPM_RGB(int);
void Set_TempC_RGB(signed char);
void toggle_fan_mode(signed char tempC);


#define FAN_EN			PORTAbits.RA4
#define FAN_MODE		PORTBbits.RB6




