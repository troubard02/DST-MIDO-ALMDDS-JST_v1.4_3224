#include "include.h"



uint8_t Twi_Write_Buf[10]={0x98,0xAA};
uint8_t Twi_Read_Buf[10]={0x99,0x00};
uint8_t Twi_Data_Buf[10];

uint8_t Twi_Delay_Timer_Reg;
uint16_t Twi_Delay_Timer_Cnt;


uint32_t Twi_Raw_Value;

void Twi_Timer(void)
{
  if(Twi_Delay_Timer_Reg==1){
    Twi_Delay_Timer_Cnt++;
    if(Twi_Delay_Timer_Cnt>60){
      Twi_Delay_Timer_Cnt=0;
      Twi_Delay_Timer_Reg=2;
    }
  }
}

void TWI_Operation(void)
{
  TWI_Start_Transceiver_With_Data(Twi_Write_Buf,2);
  Twi_Delay_Timer_Reg=1;
  while(Twi_Delay_Timer_Reg){
  
    if(Twi_Delay_Timer_Reg==2)break;
  }
  TWI_Start_Transceiver_With_Data(Twi_Read_Buf,5);
  while(TWI_Transceiver_Busy());                // Wait until TWI is ready for next transmission.

  for(uint16_t i=0;i<5;i++)
    Twi_Data_Buf[i]=TWI_buf[i];

  __disable_interrupt();
  Twi_Raw_Value = Twi_Data_Buf[2];
  Twi_Raw_Value = (Twi_Raw_Value<<8)+Twi_Data_Buf[3];
  Twi_Raw_Value = (Twi_Raw_Value<<8)+Twi_Data_Buf[4];
  __enable_interrupt();
  
}


int16_t Scale_Zero_Set_1_Value,Scale_Span_Set_1_Value;

#define SCALE_ZERO_SET_1 -1000
#define SCALE_SPAN_SET_1 1000

float Cal_Pressure_1_Coef,Cal_Pressure_2_Coef,Cal_Pressure_3_Coef;
int32_t ADC_Zero_Set_1_Point,ADC_Span_Set_1_Point;

#define ADC_ZERO_SET_1  2516582
#define ADC_SPAN_SET_1 14260633



int16_t Pressure_PA_Value;

void Twi_Adc_Push_Coef(void)
{
  Scale_Zero_Set_1_Value = SCALE_ZERO_SET_1;
  Scale_Span_Set_1_Value = SCALE_SPAN_SET_1;

  ADC_Zero_Set_1_Point = ADC_ZERO_SET_1;
  ADC_Span_Set_1_Point = ADC_SPAN_SET_1;

  Cal_Pressure_1_Coef = (float)(Scale_Span_Set_1_Value-Scale_Zero_Set_1_Value)
    /((int32_t)ADC_Span_Set_1_Point - (int32_t)ADC_Zero_Set_1_Point);
}


int16_t Twi_Adc_Cal_Real_Value(int32_t value,
                                     int32_t ADC_Zero_Set_Point,
                                     int16_t Scale_Zero_Set_Value,
                                     float Cal_Pressure_Coff)
{
  
  int16_t Real_Value;
  __disable_interrupt();
  Real_Value = (int32_t)(((int32_t)value-(int32_t)ADC_Zero_Set_Point) 
                     * Cal_Pressure_Coff);
  Real_Value = Scale_Zero_Set_Value + Real_Value;    
  __enable_interrupt();  

  return Real_Value;

}

void Twi_Adc_Get_Pressure_Sensor_Value(void)
{
  Pressure_PA_Value = Twi_Adc_Cal_Real_Value(Twi_Raw_Value,
                                         ADC_Zero_Set_1_Point,
                                         Scale_Zero_Set_1_Value,
                                         Cal_Pressure_1_Coef);

  __disable_interrupt();
  Real_Pressure_Value = Pressure_PA_Value - ADC_AutoZero_Point;

  
  __enable_interrupt();
}

