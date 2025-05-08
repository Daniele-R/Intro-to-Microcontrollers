void Init_ADC(void);
void Init_UART(void);
void Do_Beep(void);
void Do_Beep_Good(void);
void Do_Beep_Bad(void);
void Activate_Buzzer(void);
void Activate_Buzzer_4KHz(void);
void Activate_Buzzer_2KHz(void);
void Activate_Buzzer_500Hz(void);
void Deactivate_Buzzer(void);
void Wait_Half_Second();
void Wait_One_Sec();

void Wait_One_Sec_Soft(void);
void do_update_pwm(char);
void Set_RGB_Color(char);
void Prog_ADCON0(char ch);

void putch (char);
float Read_Volt(char);
unsigned int get_full_ADC();
char check_for_button_input();
char bcd_2_dec (char);
int  dec_2_bcd (char);
