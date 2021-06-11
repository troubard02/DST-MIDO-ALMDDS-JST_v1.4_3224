#ifndef __ADC_H
#define __ADC_H



extern int16_t Standard_Pressure_Value;
extern uint16_t ADC_Motor_Switch_Value;
extern uint16_t ADC_Overcurrent_Value;
extern int16_t Real_Pressure_Value;

extern __no_init __eeprom  int EEP_AutoZero_ADC_Point;
extern int16_t ADC_AutoZero_Point;

#define ADCA_SETTING 0xCC


void ADC_Initialize(void);
void ADC_Delay_Timer(void);
void ADC_Get_Standard_Pressure_Value(void);
void ADC_Auto_Zero_Process(void);

#endif