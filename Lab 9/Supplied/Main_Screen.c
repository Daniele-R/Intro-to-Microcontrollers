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

char EW_Count[]     = "00";             // text storage for EW Count
char EWLT_Count[]   = "00";             // text storage for EW Left Turn Count
char NS_Count[]     = "00";             // text storage for NS Count
char NSLT_Count[]   = "00";             // text storage for NS Left Turn Count

char PED_EW_Count[] = "00";             // text storage for EW Pedestrian Count
char PED_NS_Count[] = "00";             // text storage for NS Pedestrian Count

char SW_EWPED_Txt[] = "0";              // text storage for EW Pedestrian Switch
char SW_EWLT_Txt[]  = "0";              // text storage for EW Left Turn Switch
char SW_NSPED_Txt[] = "0";              // text storage for NS Pedestrian Switch
char SW_NSLT_Txt[]  = "0";              // text storage for NS Left Turn Switch

char Light_Sensor_Voltage_Txt[]  = "2.5V"; // text storage for Light Sensor
char Light_Sensor_Mode_Txt[] = "-D";     //

char Act_Mode_Txt[]  = "D";             // text storage for Actual Mode
char FlashingS_Txt[] = "0";             // text storage for Flashing Status
char FlashingR_Txt[] = "0";             // text storage for Flashing Request

char Timing_Txt[] = "SP24/1/01 000000 0000";

char SW_EWPED;                          // RAM variable for EW Pedestrian Switch
char SW_EWLT;                           // RAM variable for EW Left Turn Switch
char SW_NSPED;                          // RAM variable for NS Pedestrian Switch
char SW_NSLT;                           // RAM variable for NS Left Turn Switch
char Light_Sensor;                      // RAM variable for Light Sensor
extern char MODE;

char NS_PED_SW = 0;
char EW_PED_SW = 0;

extern char Flashing_Request;
extern char Flashing_Status;


void Initialize_LCD_Screen()
{
  LCD_Reset();
  TFT_GreenTab_Initialize();

  fillScreen(ST7735_BLACK);
  
  txt = buffer;
  /* MODE FIELD */
  strcpy(txt, "Mode:");
  drawtext(2, Mode_Y, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(35,Mode_Y, Act_Mode_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  strcpy(txt, "LSV:");
  drawtext(50, Mode_Y, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(75,Mode_Y, Light_Sensor_Voltage_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  strcpy(txt, "-D");  
  drawtext(100,Mode_Y, Light_Sensor_Mode_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);  
  
  /* SECOND UPDATE FIELD */
  strcpy(txt, "*");
  drawtext(120, Mode_Y, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
      
  /* MISCELLANEOUS UPDATE FIELD */  
  strcpy(txt, "NSP NSLT EWP EWLT R S");
  drawtext(1,  Switch_Txt_Y, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(8,  Switch_Txt_Y+9, SW_NSPED_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(35, Switch_Txt_Y+9, SW_NSLT_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(60, Switch_Txt_Y+9, SW_EWPED_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(85, Switch_Txt_Y+9, SW_EWLT_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(108, Switch_Txt_Y+9, FlashingR_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);  
  drawtext(119, Switch_Txt_Y+9, FlashingS_Txt, ST7735_WHITE, ST7735_BLACK, TS_1); 
  

  /* NORTH/SOUTH UPDATE FIELD */
  strcpy(txt, "NORTH/SOUTH");
  drawtext  (XTXT, NS_Txt_Y  , txt, NS_Color, ST7735_BLACK, TS_1);
  drawRect  (XTXT, NS_Cir_Y-8, 60, 18, NS_Color);
  drawCircle(XRED, NS_Cir_Y  , Circle_Size, ST7735_RED);
  drawCircle(XYEL, NS_Cir_Y  , Circle_Size, ST7735_YELLOW);
  fillCircle(XGRN, NS_Cir_Y  , Circle_Size, ST7735_GREEN);
  drawtext  (XCNT, NS_Count_Y, NS_Count, NS_Color, ST7735_BLACK, TS_2);
    
  /* NORTH/SOUTH LEFT TURN UPDATE FIELD */
  strcpy(txt, "N/S LT");
  drawtext  (XTXT, NSLT_Txt_Y, txt, NSLT_Color, ST7735_BLACK, TS_1);
  drawRect  (XTXT, NSLT_Cir_Y-8, 60, 18, NSLT_Color);
  fillCircle(XRED, NSLT_Cir_Y, Circle_Size, ST7735_RED);
  drawCircle(XYEL, NSLT_Cir_Y, Circle_Size, ST7735_YELLOW);
  drawCircle(XGRN, NSLT_Cir_Y, Circle_Size, ST7735_GREEN);  
  drawtext  (XCNT, NSLT_Count_Y, NSLT_Count, NSLT_Color, ST7735_BLACK, TS_2);
  
  /* EAST/WEST UPDATE FIELD */
  strcpy(txt, "EAST/WEST");
  drawtext  (XTXT, EW_Txt_Y, txt, EW_Color, ST7735_BLACK, TS_1);
  drawRect  (XTXT, EW_Cir_Y-8, 60, 18, EW_Color);
  fillCircle(XRED, EW_Cir_Y, Circle_Size, ST7735_RED);
  drawCircle(XYEL, EW_Cir_Y, Circle_Size, ST7735_YELLOW);
  drawCircle(XGRN, EW_Cir_Y, Circle_Size, ST7735_GREEN);  
  drawtext  (XCNT, EW_Count_Y, EW_Count, EW_Color, ST7735_BLACK, TS_2);

  /* EAST/WEST LEFT TURN UPDATE FIELD */
  strcpy(txt, "E/W LT");
  drawtext  (XTXT, EWLT_Txt_Y, txt, EWLT_Color, ST7735_BLACK, TS_1);
  drawRect  (XTXT, EWLT_Cir_Y-8, 60, 18, EWLT_Color);  
  fillCircle(XRED, EWLT_Cir_Y, Circle_Size, ST7735_RED);
  drawCircle(XYEL, EWLT_Cir_Y, Circle_Size, ST7735_YELLOW);
  drawCircle(XGRN, EWLT_Cir_Y, Circle_Size, ST7735_GREEN);   
  drawtext  (XCNT, EWLT_Count_Y, EWLT_Count, EWLT_Color, ST7735_BLACK, TS_2);

  /* NORTH/SOUTH PEDESTRIAM UPDATE FIELD */
  strcpy(txt, "PNS");  
  drawtext(3, NS_Txt_Y, txt, NS_Color, ST7735_BLACK, TS_1);
  drawtext(2, PED_NS_Count_Y, PED_NS_Count, NS_Color, ST7735_BLACK, TS_2);
 
  /* EAST/WEST PEDESTRIAM UPDATE FIELD */  
  
  strcpy(txt, "PEW");  
  drawtext(3, EW_Txt_Y, txt, EW_Color, ST7735_BLACK, TS_1);
  drawtext(2, PED_EW_Count_Y, PED_EW_Count, EW_Color, ST7735_BLACK, TS_2);
   
  if (Semester == 0)
  {
     Timing_Txt[0] = 'S';  
     Timing_Txt[1] = 'P';         
  }
  else 
  {
     Timing_Txt[0] = 'F';   
     Timing_Txt[1] = 'S';       
  }
  Timing_Txt[2] = Year/10 + '0';
  Timing_Txt[3] = Year%10 + '0';
  Timing_Txt[5] = Session_Number%10 + '0';  
  Timing_Txt[7] = Table_Number/10 + '0';
  Timing_Txt[8] = Table_Number%10 + '0';  
  Timing_Txt[10] = PEDESTRIAN_NS_WAIT + '0';
  Timing_Txt[11] = NS_WAIT + '0';
  Timing_Txt[12] = EW_LT_WAIT + '0';
  Timing_Txt[13] = PEDESTRIAN_EW_WAIT + '0';
  Timing_Txt[14] = EW_WAIT + '0';
  Timing_Txt[15] = NS_LT_WAIT + '0';
  Timing_Txt[17] = NIGHT_NS_WAIT + '0';
  Timing_Txt[18] = NIGHT_EW_LT_WAIT + '0';
  Timing_Txt[19] = NIGHT_EW_WAIT + '0';
  Timing_Txt[20] = NIGHT_NS_LT_WAIT + '0';
  drawtext(1, Flash_Y, Timing_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
}

// change the ST7735_BLACK to colors so it fills the void with color instead of black

void update_LCD_color(char direction, char color)
{
    char Circle_Y;
    Circle_Y = NS_Cir_Y + direction * 30;    
    
    if (color == Color_Off)     //if Color off make all circles black but leave outline
    {
        fillCircle(XRED, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XYEL, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XGRN, Circle_Y, Circle_Size, ST7735_BLACK); 
        drawCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);            
        drawCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        drawCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN);                       
    }    
    
    if (color == Color_Red)     //if Color off make all circles black but leave outline
    {
        fillCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);
        fillCircle(XYEL, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XGRN, Circle_Y, Circle_Size, ST7735_BLACK); 
        drawCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);            
        drawCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        drawCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN);                       
    }    

    if (color == Color_Yellow)     //if Color off make all circles black but leave outline
    {
        fillCircle(XRED, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        fillCircle(XGRN, Circle_Y, Circle_Size, ST7735_BLACK); 
        drawCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);            
        drawCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        drawCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN);                       
    }    
    
    if (color == Color_Green)     //if Color off make all circles black but leave outline
    {
        fillCircle(XRED, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XYEL, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN); 
        drawCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);            
        drawCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        drawCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN);                       
    }    
    	
}

void update_LCD_PED_Count(char direction, char count)
{
   switch (direction)
   {
    case NS:       
        PED_NS_Count[0] = count/10  + '0';          // PED count upper digit
        PED_NS_Count[1] = count%10  + '0';          // PED Lower
        drawtext(PED_Count_X, PED_NS_Count_Y, PED_NS_Count, NS_Color, ST7735_BLACK, TS_2);         //Put counter digit on screen
        break;
    case EW:       
        PED_EW_Count[0] = count/10  + '0';          // PED count upper digit
        PED_EW_Count[1] = count%10  + '0';          // PED Lower
        drawtext(PED_Count_X, PED_EW_Count_Y, PED_EW_Count, EW_Color, ST7735_BLACK, TS_2);        
        break;
   }
}

void update_LCD_count(char direction, char count)
{
   switch (direction)                   //update traffic light no ped time
   {
    case NS:       
        NS_Count[0] = count/10  + '0';
        NS_Count[1] = count%10  + '0';
        drawtext(XCNT, NS_Count_Y, NS_Count, NS_Color, ST7735_BLACK, TS_2); 
        break;
    case EW:        
        EW_Count[0] = count/10  + '0';
        EW_Count[1] = count%10  + '0';
        drawtext(XCNT, EW_Count_Y, EW_Count, EW_Color, ST7735_BLACK, TS_2);                
        break;
    case EWLT:        
        EWLT_Count[0] = count/10  + '0';
        EWLT_Count[1] = count%10  + '0';
        drawtext(XCNT, EWLT_Count_Y, EWLT_Count, EWLT_Color, ST7735_BLACK, TS_2);                
        break;               
    case NSLT:        
        NSLT_Count[0] = count/10  + '0';
        NSLT_Count[1] = count%10  + '0';
        drawtext(XCNT, NSLT_Count_Y, NSLT_Count, NSLT_Color, ST7735_BLACK, TS_2);                
        break;
    }  
}

void update_LCD_misc()
{
    int nStep = get_full_ADC();         // calculates the # of steps for analog conversion
    float volt = Read_Volt (0);         // Read light sensor 
    Light_Sensor = volt < 2.5 ? 1:0;    // Mode = 1, Day_mode, Mode = 0 Night_mode
    Light_Sensor_Voltage_Txt[0] = ((int) volt + '0');
    Light_Sensor_Voltage_Txt[2] = ((((int) (volt*10))%10) + '0');
    if (Light_Sensor == 0) Light_Sensor_Mode_Txt[1] = 'N'; else Light_Sensor_Mode_Txt[1] = 'D';  
    if (MODE == 0) Act_Mode_Txt[0] = 'N'; else Act_Mode_Txt[0] = 'D';

    SW_EWPED = EW_PED_SW;
    SW_EWLT = EW_LT_SW;    
    SW_NSPED = NS_PED_SW;
    SW_NSLT = NS_LT_SW;
    
//    if (MODE == 0)
//    {
//        NS_PED_SW = 0;
//    }

    if (SW_NSPED == 0) SW_NSPED_Txt[0] = '0'; else SW_NSPED_Txt[0] = '1';               
    if (SW_EWPED == 0) SW_EWPED_Txt[0] = '0'; else SW_EWPED_Txt[0] = '1';
    if (SW_EWLT == 0)  SW_EWLT_Txt[0]  = '0'; else SW_EWLT_Txt[0]  = '1';
    if (SW_NSLT == 0)  SW_NSLT_Txt[0]  = '0'; else SW_NSLT_Txt[0]  = '1';
	
    if (Flashing_Request == 0) FlashingR_Txt[0] = '0'; else FlashingR_Txt[0] = '1';
    if (Flashing_Status == 0) FlashingS_Txt[0] = '0'; else FlashingS_Txt[0] = '1';
    
    drawtext(35,  Mode_Y, Act_Mode_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);      
    drawtext(75,  Mode_Y, Light_Sensor_Voltage_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(100, Mode_Y, Light_Sensor_Mode_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);

    drawtext(8,  Switch_Txt_Y+9, SW_NSPED_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);       
    drawtext(35, Switch_Txt_Y+9, SW_NSLT_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
	drawtext(60,  Switch_Txt_Y+9, SW_EWPED_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(85,  Switch_Txt_Y+9, SW_EWLT_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(108, Switch_Txt_Y+9, FlashingR_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);  
    drawtext(119, Switch_Txt_Y+9, FlashingS_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);             
}


void Draw_Star()
{
    strcpy(txt,"*");
    drawtext(120,Star_Y,txt,ST7735_WHITE,ST7735_BLACK,TS_1);
}

void Erase_Star()
{
    strcpy(txt," ");
    drawtext(120,Star_Y,txt,ST7735_WHITE,ST7735_BLACK,TS_1);
}


