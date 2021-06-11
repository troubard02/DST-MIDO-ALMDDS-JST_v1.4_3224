#include "include.h"

__no_init __eeprom  int EEP_AutoZero_ADC_Point;
int16_t ADC_AutoZero_Point;

uint16_t ADC_raw_value; 
uint16_t ADC_Standard_Set_Value;
uint16_t ADC_Motor_Switch_Value;
uint16_t ADC_Overcurrent_Value;

uint8_t AD_CompleteFlag;

uint8_t ADC_Delay_Timer_Reg;
uint16_t ADC_Delay_Timer_Cnt;

int16_t Standard_Pressure_Value;
int16_t Stadnard_Pressure_OldValue;

int16_t Real_Pressure_Value;

#pragma   vector=ADC_vect
__interrupt void ADC_CONVERSION_VECT()
{
  
  
  uint16_t i;
  uint8_t ADC_LowByte, ADC_HighByte;
  
  ADC_LowByte =ADCL;          // A/D값을 저장할 임시변수  LSB
  ADC_HighByte =ADCH;          // A/D값을 저장할 임시변수  MSB
  i = ADC_HighByte;                   // A/D  MSB 임시 변수에 저장             
  i= ( 0X0FFF & ( i << 8 ) );  // A/D값을 MSB(상위)로 보냄
  ADC_raw_value = ( i | ADC_LowByte );   // CHAR를 INT로 변환
  AD_CompleteFlag=1;        

  __disable_interrupt();
  if(ADMUX==0x40){ // ADC0 Motor Switch Value

    ADC_Motor_Switch_Value = ADC_raw_value;
    ADC_Delay_Timer_Reg=1;

    Motor_Get_LimitSwitch_Value();
    
    ADCSRA = 0;
    ADMUX=0x41;   

  }
  else if (ADMUX==0x41){ // Overcurrent Check
    ADC_Overcurrent_Value = ADC_raw_value;  
    ADC_Delay_Timer_Reg=1;

    ADCSRA =0;
    ADMUX=0x42;
  }
  else if (ADMUX==0x42){ // VR2 

    ADC_Standard_Set_Value = ADC_raw_value;
    ADC_Delay_Timer_Reg=1;
    ADCSRA =0;
    ADMUX=0x40;

  }
  else {
    ADC_Delay_Timer_Reg=1;
    ADCSRA =0;
    ADMUX=0x40;
  }
  
  __enable_interrupt();
    
  
}


void ADC_Get_Standard_Pressure_Value(void)
{
  
  uint16_t Temp_Value;
  
  /* 40~ 60 - 5point 
  Temp_Value = (ADC_Standard_Set_Value/250);// 분해능 1000/250 = 4 
  Standard_Pressure_Value = 40+ Temp_Value*5; // 4 * 5 = 20 
  */
  /* 40~60 - 1point  */
  Temp_Value = (ADC_Standard_Set_Value/50);// 분해능 1000/50 = 20 
  Standard_Pressure_Value = 40+ Temp_Value;
  
  /* 70~80 - 1point  
  Temp_Value = (ADC_Standard_Set_Value/50);// 분해능 1000/50 = 20 
  Standard_Pressure_Value = 70+ Temp_Value;  
  */
  /* 20~80 - 1point 
  Temp_Value = (ADC_Standard_Set_Value/16);// 분해능 1000/16 = 60
  Standard_Pressure_Value = 20+ Temp_Value; 
  */
  if(Standard_Pressure_Value!=Stadnard_Pressure_OldValue){    

    Display_Delay_Cnt=0;
    Display_Mode = D_Standard_Setting_Mode;
    Stadnard_Pressure_OldValue=Standard_Pressure_Value;
  }
  
  //Standard_Pressure_Value = 50;
  
}


void ADC_Delay_Timer(void)
{
  //__disable_interrupt();
  if(ADC_Delay_Timer_Reg==1){
    ADC_Delay_Timer_Cnt++;
    if(ADC_Delay_Timer_Cnt>2){
      ADC_Delay_Timer_Reg=2;
      ADC_Delay_Timer_Cnt=0;

    ADCSRA =ADCA_SETTING;

      //ADCSRA = ADCA_SETTING; //ADC_START
    }
  }
  //__enable_interrupt();
}



void ADC_Auto_Zero_Process(void)
{

  
  //if(abs(Pressure_PA_Value)<50){ // Modify 2020-07-06

    __disable_interrupt();
    ADC_AutoZero_Point = Pressure_PA_Value;
    __enable_interrupt();
    EEP_AutoZero_ADC_Point = ADC_AutoZero_Point;
    
/*
    if(Real_Pressure_Value>=0){
      ADC_AutoZero_Point = Real_Pressure_Value;

    }
    else {
      

    }*/
    Display_Delay_Cnt=0;
    Display_Mode=D_AutoZero_Mode;
  //}

}




void ADC_Initialize(void)
{

  //ADCSRA = ADCA_SETTING;
  
//#if MCU_TYPE
    //ADCSR =ADCA_SETTING;
//#else 
    ADCSRA =ADCA_SETTING;
//#endif    
    
  //ADCSRA = ADCA_SETTING; //ADC_START
  ADMUX = 0x40; // AREF, ADC5 Pressrue Value input
  
}

