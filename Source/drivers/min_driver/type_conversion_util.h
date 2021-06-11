#ifndef TYPE_CONVERSION_UTILL_H
#define TYPE_CONVERSION_UTILL_H





uint8_t Trans_Int_to_Char_HighByte(uint16_t int_value);
uint8_t Trans_Int_to_Char_LowByte(uint16_t int_value);
uint16_t Trans_Char_to_Int(uint8_t char_high_value,uint8_t char_low_value);


int16_t Trans_Scale_Cal_Value(int16_t value,
                                          int16_t ADC_Zero_Set_Point,
                                          int16_t Scale_Zero_Set_Value,
                                          float Cal_Pressure_Coff);






#endif