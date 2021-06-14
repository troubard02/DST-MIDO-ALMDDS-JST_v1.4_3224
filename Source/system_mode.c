#include "include.h"


__no_init __eeprom  int EEP_Zero_ADC_Point;
__no_init __eeprom  int EEP_Span_ADC_Point;     


uint8_t System_Mode;

void RD_EEP_Value(void)
{

  ADC_AutoZero_Point = EEP_AutoZero_ADC_Point;
  //if(abs(ADC_AutoZero_Point)>=30)
    //ADC_AutoZero_Point =0;
}







uint8_t System_Initialize_Timer_Reg;
uint16_t System_Initialize_Timer_Cnt;
void System_Initialize_Timer(void)
{

  if(System_Initialize_Timer_Reg==0){

    System_Initialize_Timer_Cnt++;
    if(System_Initialize_Timer_Cnt>3000){
      System_Initialize_Timer_Cnt=0;
      System_Initialize_Timer_Reg=1;
    }
  }

}




void System_Auto_Zero_Function(void)
{

  __disable_interrupt();
  ADC_AutoZero_Point = Pressure_PA_Value;
  __enable_interrupt();
  EEP_AutoZero_ADC_Point = ADC_AutoZero_Point;

  
  
  //Display_Delay_Cnt=0;
  //Display_Mode=D_AutoZero_Mode;  

}

//////////////////////////////////////////////////////////////////////////
/////////////////// 1 Minute Delay Switch On AZ Function /////////////////
//////////////////////////////////////////////////////////////////////////

uint8_t Sys_AZ_Delay_Timer_Reg;
uint8_t Sys_AZ_1S_Timer_Cnt;
uint16_t Sys_AZ_Delay_Timer_Cnt;
uint8_t Sys_Run_Access_Enable;


void Sys_AZ_Delay_Timer_Stop(void)
{

  Sys_AZ_1S_Timer_Cnt=0;
  Sys_AZ_Delay_Timer_Cnt=0;
  Sys_AZ_Delay_Timer_Reg=0;


}

void Sys_AZ_Delay_Timer_Start(void)
{

  Sys_AZ_1S_Timer_Cnt=0;
  Sys_AZ_Delay_Timer_Cnt=0;
  Sys_AZ_Delay_Timer_Reg=1;

}

void Sys_AZ_Delay_Timer(void)
{


  /* Make 1 Minute Timer */
  if(Sys_AZ_Delay_Timer_Reg==1){


    Sys_AZ_Delay_Timer_Cnt++;
    if(Sys_AZ_Delay_Timer_Cnt>5000){

      Sys_AZ_Delay_Timer_Cnt=0;
      Sys_AZ_1S_Timer_Cnt++;

    }

    if(Sys_AZ_1S_Timer_Cnt>60){

      Sys_AZ_1S_Timer_Cnt=0;
      Sys_AZ_Delay_Timer_Reg=2; // Complete 1 Minute Timer 
      
      //Sys_Run_Access_Enable=0;
    }

  }

}


//////////////////////////////////////////////////////////////////////////
////////////////////// 1 Hour Interval AZ Function ///////////////////////
//////////////////////////////////////////////////////////////////////////


void System_Interval_Auto_Zero_Function(void)
{

  if(abs(Pressure_PA_Value)<15){

    __disable_interrupt();
    ADC_AutoZero_Point = Pressure_PA_Value;
    __enable_interrupt();
    EEP_AutoZero_ADC_Point = ADC_AutoZero_Point;




  }
  
  //Display_Delay_Cnt=0;
  //Display_Mode=D_AutoZero_Mode;

}



uint8_t Sys_AZ_Interval_Timer_Reg;
uint16_t Sys_AZ_Interval_1S_Timer_Cnt;
uint16_t Sys_AZ_Interval_Timer_Cnt;

uint8_t Sys_AZ_Interval_Flag;

void Sys_AZ_Interval_Timer_Stop(void)
{

  Sys_AZ_Interval_1S_Timer_Cnt=0;
  Sys_AZ_Interval_Timer_Cnt=0;
  Sys_AZ_Interval_Timer_Reg=0;

}

void Sys_AZ_Interval_Timer_Start(void)
{

  Sys_AZ_Interval_1S_Timer_Cnt=0;
  Sys_AZ_Interval_Timer_Cnt=0;
  Sys_AZ_Interval_Timer_Reg=1;

}

void Sys_AZ_Interval_Timer(void)
{


  /* Make 1 Minute Timer */
  if(Sys_AZ_Interval_Timer_Reg==1){


    Sys_AZ_Interval_Timer_Cnt++;
    if(Sys_AZ_Interval_Timer_Cnt>5000){

      Sys_AZ_Interval_Timer_Cnt=0;
      Sys_AZ_Interval_1S_Timer_Cnt++;

    }
    
    // Complete 1 Hour Timer 
    if(Sys_AZ_Interval_1S_Timer_Cnt>3600){ // 60*60= 3600

      Sys_AZ_Interval_1S_Timer_Cnt=0;
      //Sys_AZ_Interval_Timer_Reg=2; 
      Sys_AZ_Interval_Flag=1;
      Sys_AZ_Interval_Timer_Start();
      //Sys_Run_Access_Enable=0;
    }

  }

}



void System_Operation(void)
{


  if(System_Initialize_Timer_Reg==1){

    if(Start_Switch_State==KEY_COMPLETE_STATE){
      if(Sys_Run_Access_Enable==0){
        System_Auto_Zero_Function();
        Sys_Run_Access_Enable=1;
        Sys_AZ_Delay_Timer_Start();

      }

      Sys_AZ_Interval_Timer_Reg=1;

      if(Sys_AZ_Interval_Flag==1){
        Sys_AZ_Interval_Flag=0;

        System_Interval_Auto_Zero_Function();

      }


      System_Mode = S_Run_Mode;
    }
    else {

      /* 1 Minute Timer Complete & Start Signal Off */
      if(Sys_AZ_Delay_Timer_Reg==2)
        Sys_Run_Access_Enable=0;

      /* Stop 1 Hour Timer */
      Sys_AZ_Interval_Timer_Stop();

      System_Mode = S_Stop_Mode;
    }

  
  }

}


