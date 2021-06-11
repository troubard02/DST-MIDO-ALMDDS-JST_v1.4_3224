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

}

uint8_t Sys_Run_Access_Enable;
void System_Operation(void)
{


  if(System_Initialize_Timer_Reg==1){

    if(Start_Switch_State==KEY_COMPLETE_STATE){
      if(Sys_Run_Access_Enable==0){
        System_Auto_Zero_Function();
        Sys_Run_Access_Enable=1;
      }
      System_Mode = S_Run_Mode;
    }
    else {
      Sys_Run_Access_Enable=0;
      System_Mode = S_Stop_Mode;
    }

  
  }

}


