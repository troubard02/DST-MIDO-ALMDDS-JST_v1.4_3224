#include "include.h"





void Gpio_Initialize(void)
{
    
  DDRB = 0xEF;  // PB0 : MSIGA, PB1: SSIGB, PB2 : MSIGB, PB3 : FND_5
                // PB4 : Start Signal Input PB5 : FND DIGIT1, PB6 : FND DIGIT2, PB7 : FND DIGIT3

  PORTB = 0x10; // PB4 : PULL-UP
  DDRC = 0x08;  // PC0 : Limit Switch in(ADC), PC1 : Limit Current Value in(A/D), 
                // PC2 : VR2 IN(A/D), PC3 : ST Signal Out, PC4 : SDA 
                // PC5 : SCL, PC6 : RST
  PORTC = 0x00; // PC3 : PULL-UP
  DDRD = 0xff;  // PD0 : SSIGB , PD1 : USART TXD, PD2..PD7 : FND A..G  
  PORTD = 0x00;



}





void Timer2_Initialize(void)
{

  TCCR2A = 0x02;
  TCCR2B = 0x03;
  OCR2A = 50;
  OCR2B = 50;
  TIMSK2 = 0x06;

}


uint16_t Timer2_A_Cnt,Timer2_B_Cnt;

#pragma   vector=TIMER2_COMPA_vect // 0.2ms 
__interrupt void timer2_isr1()
{ 
  __disable_interrupt();

  Motor_Timer();
  ADC_Delay_Timer();
  Switch_Timer();
  Fnd_Put_Data_Timer();
  Fnd_Display_Timer();

  System_Initialize_Timer();
  Sys_AZ_Delay_Timer();
  Sys_AZ_Interval_Timer();
  __enable_interrupt();
}


#pragma   vector=TIMER2_COMPB_vect // 0.2ms 
__interrupt void timer2_isr2()
{ 
  __disable_interrupt();
  Timer2_B_Cnt++;
  Usart_Timer();
  Twi_Timer();
  __enable_interrupt();
}


