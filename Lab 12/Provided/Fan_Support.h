int get_RPM();
void Toggle_Fan();
void Turn_Off_Fan();
void Turn_On_Fan();
void Increase_Speed();
void Decrease_Speed();
void do_update_pwm(char) ;
void Set_DC_RGB(int);
void Set_RPM_RGB(int);

#define FAN_EN			PORTxbits.Rxy
#define FAN_PWM			PORTxbits.Rxy
#define FAN_MODE		PORTxbits.RXy




