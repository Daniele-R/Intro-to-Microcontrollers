#include <xc.h>
#include <p18f4620.h>
#include <stdio.h>
#include <stdlib.h>
#include "Interrupt.h"

extern char INT0_Flag;
extern char INT1_Flag;
extern char INT2_Flag;
extern char NS_PED_SW;
extern char EW_PED_SW;
extern char MODE;
extern char Flashing_Request;

int INT0_flag, INT1_flag, INT2_flag = 0;

void init_INTERRUPT()
{
    INTCONbits.INT0IF   = 0 ; // Clear INT0IF
    INTCON3bits.INT1IF  = 0;  // Clear INT1IF
    INTCON3bits.INT2IF  = 0;  // Clear INT2IF
    
    INTCON2bits.INTEDG0 = 0 ; // INT0 EDGE falling
    INTCON2bits.INTEDG1 = 0;  // INT1 EDGE falling
    INTCON2bits.INTEDG2 = 0;  // INT2 EDGE falling
    
    INTCONbits.INT0IE   = 1;   // Set INT0 IE
    INTCON3bits.INT1IE  = 1;   // Set INT1 IE
    INTCON3bits.INT2IE  = 1;   // Set INT2 IE
    
    INTCONbits.GIE      = 1;       // Set the Global Interrupt Enable
}

void interrupt  high_priority chkisr()
{
    if (INTCONbits.INT0IF == 1) INT0_ISR(); // check if INT0 has occurred			
    if (INTCON3bits.INT1IF == 1) INT1_ISR();
    if (INTCON3bits.INT2IF == 1) INT2_ISR();
}

// PART A
//void INT0_ISR()
//{
//    INTCONbits.INT0IF=0; // Clear the interrupt flag
//    INT0_Flag= 1;    
//}
//
//void INT1_ISR()
//{
//    INTCON3bits.INT1IF=0; // Clear the interrupt flag
//    INT1_flag = 1; // set software INT1_flag  
//}

// PART B
void INT0_ISR()
{
    printf("INT0 detected\r\n");
    INTCONbits.INT0IF = 0; // Clear the interrupt flag
    INT0_Flag = 1;
    
    if (MODE == 1) 
    {
        INT0_Flag = 1;
        NS_PED_SW = 1;
    }
    else if (MODE == 0) 
    {
        INT0_Flag = 0;
        NS_PED_SW = 0;
    } // set software INT0_flag
}

void INT1_ISR()
{
    printf("INT1 detected\r\n");
    INTCON3bits.INT1IF = 0; // Clear the interrupt flag
    INT1_Flag = 1;
    
    if (MODE == 1) 
    {
        INT1_Flag = 1;
        EW_PED_SW = 1;
    }
    else if (MODE == 0)
    {
        INT1_Flag = 0;
        EW_PED_SW = 0;
    }
}

void INT2_ISR()
{
    printf("INT2 detected\r\n");
    INTCON3bits.INT2IF = 0; // Clear the interrupt flag
    INT2_flag = 1; // set software INT2_flag
    Flashing_Request = 1;  
}


