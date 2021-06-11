#ifndef __PS_33D_H
#define __PS_33D_H



void Twi_Timer(void);
void TWI_Operation(void);



void Twi_Adc_Push_Coef(void);
int16_t Twi_Adc_Cal_Real_Value(int32_t value,
                                     int32_t ADC_Zero_Set_Point,
                                     int16_t Scale_Zero_Set_Value,
                                     float Cal_Pressure_Coff);
void Twi_Adc_Get_Pressure_Sensor_Value(void);

extern int16_t Pressure_PA_Value;


#endif

