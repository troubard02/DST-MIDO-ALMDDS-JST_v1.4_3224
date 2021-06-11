

#include "include.h"

/*! \brief Trans_Int_to_Char_HighByte(int int_value)
 
* value 값의 high byte를 리턴해준다. 

*/


uint8_t Trans_Int_to_Char_HighByte(uint16_t int_value)
{
  uint8_t temp_value;
  temp_value = int_value>>8;
  return temp_value;
}

/*! \brief Trans_Int_to_Char_LowByte(int int_value)
 
* value 값의 low byte를 리턴해준다. 

*/


uint8_t Trans_Int_to_Char_LowByte(uint16_t int_value)
{
  uint8_t temp_value;
  temp_value = int_value;
  return temp_value;
  
}

/*! \brief Trans_Char_to_Int(char char_high_value,char char_low_value)
 
* 두 byte를 합쳐서 하나의 integer 값을 만든다. 

*/



uint16_t Trans_Char_to_Int(uint8_t char_high_value,uint8_t char_low_value)
{
  uint16_t temp_value;
  temp_value = char_high_value;
  temp_value = temp_value<<8;
  temp_value |=char_low_value;
  
  return temp_value; 
}



int16_t Trans_Scale_Cal_Value(int16_t value,
                                          int16_t ADC_Zero_Set_Point,
                                          int16_t Scale_Zero_Set_Value,
                                          float Cal_Pressure_Coff)
{
  int16_t Real_Value;
  __disable_interrupt();
  Real_Value = (int)(((int)value-(int)ADC_Zero_Set_Point) 
                     * Cal_Pressure_Coff);
    Real_Value = Scale_Zero_Set_Value + Real_Value;    
  __enable_interrupt();  

  return Real_Value;

}


