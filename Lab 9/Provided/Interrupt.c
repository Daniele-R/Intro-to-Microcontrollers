#include <xc.h>
#include <p18f4620.h>
#include "Interrupt.h"

extern char INT0_Flag;
extern char INT1_Flag;
extern char INT2_Flag;
extern char NS_PED_SW;
extern char EW_PED_SW;
extern char Flashing_Request;

void init_INTERRUPT()
{

    // add code
}

void interrupt  high_priority chkisr()
{
    if (INTCONbits.INT0IF == 1) INT0_ISR(); // check if INT0 has occurred			
    // add more code
}

void INT0_ISR()
{
    INTCONbits.INT0IF=0; // Clear the interrupt flag
    INT0_Flag= 1;    
}

void INT1_ISR()
{
    // add more code     
}

void INT2_ISR()
{
    // add more code  
}

