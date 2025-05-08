
#include <xc.h>
#include <p18f4620.h>
#include "Interrupt.h"

unsigned char bit_count;
unsigned int Time_Elapsed;

unsigned long long Nec_code;
unsigned char Nec_State = 0;

extern char Nec_Button;
extern short Nec_OK;

void Init_Interrupt()
{
    INTCON3bits.INT1IF = 0;                 // Clear external interrupt INT1IF
    INTCON2bits.INTEDG1 = 0;                // Edge programming for INT 1 falling edge H to L
    INTCON3bits.INT1IE = 1;                 // Enable external interrupt INT1IE

    TMR1H = 0;                              // Reset Timer1
    TMR1L = 0;                              //
    PIR1bits.TMR1IF = 0;                    // Clear timer 1 interrupt flag
    PIE1bits.TMR1IE = 1;                    // Enable Timer 1 interrupt
    INTCONbits.PEIE = 1;                    // Enable Peripheral interrupt
    INTCONbits.GIE = 1;                     // Enable global interrupts
}

void Enable_INT_Interrupt()
{
    INTCON3bits.INT1IE = 1;          		// Enable external interrupt
    INTCON2bits.INTEDG1 = 0;        		// Edge programming for INT1 falling edge
}    

void interrupt high_priority chkisr()
{
    if (PIR1bits.TMR1IF == 1) TIMER1_isr();
    if (INTCON3bits.INT1IF == 1) INTx_isr();
}

void TIMER1_isr(void)
{
    Nec_State = 0;                          // Reset decoding process
    INTCON2bits.INTEDG1 = 0;                // Edge programming for INT 1 falling edge
    T1CONbits.TMR1ON = 0;                   // Disable T1 Timer
    PIR1bits.TMR1IF = 0;                    // Clear interrupt flag
}

void Reset_Nec_State()
{
    Nec_State = 0;
    T1CONbits.TMR1ON = 0;
}

void INTx_isr(void)
{
    INTCON3bits.INT2IF = 0;                  // Clear external interrupt INT1IF
    if (Nec_state != 0)
    {
        Time_Elapsed = (TMR1H << 8) | TMR1L;// Store Timer1 value
        TMR1H = 0;                          // Reset Timer1
        TMR1L = 0;
    }
    
    switch(Nec_state)
    {
        case 0 :
        {
                                            // Clear Timer 1
            TMR1H = 0;                      // Reset Timer1
            TMR1L = 0;                      //
            PIR1bits.TMR1IF = 0;            //
            T1CON = 0x90;                   // Program Timer1 mode with count = 1usec using System clock running at 8Mhz
            T1CONbits.TMR1ON = 1;           // Enable Timer 1
            bit_count = 0;                  // Force bit count (bit_count) to 0
            Nec_code = 0;                   // Set Nec_code = 0
            Nec_State = 1;                  // Set Nec_State to state 1
            INTCON2bits.INTEDG1 = 1;        // Change Edge interrupt of INT 1 to Low to High            
            return;
        }
        
        case 1 :
        {

        }
        
        case 2 :                            // Add your code here
        {

        }
        
        case 3 :                            // Add your code here
        {

        }
        
        case 4 :                            // Add your code here
        {

        }
    }
}