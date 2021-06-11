#include "include.h"



#define ALARM1_MASKB 0x01
#define ALARM1_MASKD 0x01
#define ALARM2_MASKB1 0x02
#define ALARM2_MASKB2 0x04
#define ALARM2_MASKB 0x06

uint8_t Temp_PortB,Temp_PortD;

void Alarm1_Off(void)
{
  
  ///////////////////// ALARM2 ALL OFF /////////////////////////

  __disable_interrupt();
  Temp_PortB = PINB&(~ALARM2_MASKB); // VDD : PB1, OFF
  Temp_PortB |= START_PORT_VALUE; // START PULL UP
  PORTB = Temp_PortB;  
  __enable_interrupt();
  
  __delay_cycles(10);
  ///////////////////// ALARM1 OFF ///////////////////////////// 
  __disable_interrupt();
  Temp_PortB = PINB&(~ALARM1_MASKB); // VDD : PB0, OFF
  Temp_PortB |= START_PORT_VALUE; // START PULL UP
  PORTB = Temp_PortB;
  Temp_PortD = PIND&(~ALARM1_MASKD); // VDD : PD0, OFF
  PORTD = Temp_PortD;  
  
  
  Temp_PortD = PIND&(~ALARM1_MASKD); // GND : PD0, ON
  Temp_PortD |= ALARM1_MASKD;
  PORTD = Temp_PortD;
  
  
  
  
  Temp_PortB = PINB&(~ALARM2_MASKB2); // GND : PB2, ON
  Temp_PortB |= ALARM2_MASKB2;
  Temp_PortB |= START_PORT_VALUE; // START PULL UP
  PORTB = Temp_PortB;
  __enable_interrupt();
  
}

void Alarm2_Off(void)
{
  
  
  ////////////////////// ALARM1  All OFF//////////////////////
  
  __disable_interrupt();
  Temp_PortB = PINB&(~ALARM1_MASKB); // VDD : PB0, OFF
  Temp_PortB |= START_PORT_VALUE; // START PULL UP
  PORTB = Temp_PortB;
  Temp_PortD = PIND&(~ALARM1_MASKD); // VDD : PD0, OFF
  PORTD = Temp_PortD;  
  __enable_interrupt();
  __delay_cycles(10);
  
  /////////////////////// ALARM2 OFF //////////////////////////
  __disable_interrupt();
  Temp_PortB = PINB&(~ALARM2_MASKB1); // VDD : PB1, OFF
  Temp_PortB |= START_PORT_VALUE; // START PULL UP
  PORTB = Temp_PortB;
  __enable_interrupt();
  __delay_cycles(10);
  __disable_interrupt();
  Temp_PortB = PINB&(~ALARM2_MASKB2); // GND : PB2, ON
  Temp_PortB |= ALARM2_MASKB2;
  Temp_PortB |= START_PORT_VALUE; // START PULL UP
  PORTB = Temp_PortB;
  
  Temp_PortD = PIND&(~ALARM1_MASKD); // GND : PD0, ON
  Temp_PortD |= ALARM1_MASKD;
  PORTD = Temp_PortD;
  __enable_interrupt();  
}


void Alarm1_Out(void)
{
  
  
  if(Motor_Status==MOTER_OPEN){
    //Alarm_All_Off();
    Alarm1_Off();
    
  }
  else {
    
  ///////////////////// ALARM2 OFF /////////////////////////  
    
  __disable_interrupt();
  Temp_PortB = PINB&(~ALARM2_MASKB); // ALARM2 OFF 
  Temp_PortB |= START_PORT_VALUE; // START PULL UP
  PORTB = Temp_PortB;
  __enable_interrupt(); 
  
  __delay_cycles(10);
  ///////////////////// ALARM1 ON //////////////////////////
  
  __disable_interrupt();
  Temp_PortB = PINB&(~ALARM1_MASKB); // VDD : PB0,  ON
  Temp_PortB |= ALARM1_MASKB;
  Temp_PortB |= START_PORT_VALUE; // START PULL UP
  PORTB = Temp_PortB;
  
  Temp_PortD = PIND&(~ALARM1_MASKD); // GND : PD0, ON
  Temp_PortD |= ALARM1_MASKD;
  PORTD = Temp_PortD;
  __enable_interrupt(); 
  
  }
  
}

void Alarm2_Out(void)
{
  
  
  if(Motor_Status==MOTER_CLOSE){
    Alarm2_Off();
    //Alarm_All_Off();
    
  }
  else {
  
  ///////////////////// ALARM1 OFF /////////////////////////
  __disable_interrupt();
  Temp_PortB = PINB&(~ALARM1_MASKB); // VDD : PB0, OFF
  Temp_PortB |= START_PORT_VALUE; // START PULL UP
  PORTB = Temp_PortB;
  
  Temp_PortD = PIND&(~ALARM1_MASKD); // GND : PD0, OFF
  PORTD = Temp_PortD;
  __enable_interrupt(); 
  
  __delay_cycles(10);
  ///////////////////// ALARM2 ON //////////////////////////
  __disable_interrupt();
  Temp_PortB = PINB&(~ALARM2_MASKB); // VDD : PB1, ON
  Temp_PortB |= ALARM2_MASKB;        // GND : PB2, ON
  Temp_PortB |= START_PORT_VALUE; // START PULL UP
  PORTB = Temp_PortB;
  __enable_interrupt(); 
  
  }
  
}
