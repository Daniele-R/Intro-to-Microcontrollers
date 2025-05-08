#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <string.h>


#include "main.h"
#include "utils.h"
#include "Main_Screen.h"
#include "ST7735_TFT.h"


char buffer[31];                        // general buffer for display purpose
char *nbr;                              // general pointer used for buffer
char *txt;
char Timing_Txt[] = "SP24/1/01";



void Initialize_LCD_Screen(void)
{
    LCD_Reset();
    TFT_GreenTab_Initialize();
    fillScreen(ST7735_BLACK);
  
    /* TOP HEADER FIELD */
    txt = buffer;

    strcpy(txt, "LAB 10 ");  
    drawtext(50, 2, txt, ST7735_WHITE, ST7735_BLACK, TS_1);

    if (Semester == 0)
     {
        Timing_Txt[0] = 'S';  
        Timing_Txt[1] = 'P';         
     }
     else 
     {
        Timing_Txt[0] = 'F';   
        Timing_Txt[1] = 'A';       
     }
     Timing_Txt[2] = Year/10 + '0';
     Timing_Txt[3] = Year%10 + '0';
     Timing_Txt[5] = Session_Number%10 + '0';  
     Timing_Txt[7] = Table_Number/10 + '0';
     Timing_Txt[8] = Table_Number%10 + '0'; 
     drawtext(1, Flash_Y, Timing_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
}


