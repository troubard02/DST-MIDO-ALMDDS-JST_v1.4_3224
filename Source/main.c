#include "include.h"


int main( void )
{

  Gpio_Initialize();
  Timer2_Initialize();
  ADC_Initialize();
  Uasrt_Initialize();
  RD_EEP_Value();
  Twi_Adc_Push_Coef();
  TWI_Master_Initialise();
  __enable_interrupt();    

  
  while(1)
  {

    ADC_Get_Standard_Pressure_Value();
    TWI_Operation();
    Fnd_Display();
    Twi_Adc_Get_Pressure_Sensor_Value();
    Usart_Operation();
    Motor_Operation();
    System_Operation();
  }

}

