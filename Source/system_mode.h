#ifndef __SYSTEM_MODE_H
#define __SYSTEM_MODE_H


extern __no_init __eeprom  int EEP_Zero_ADC_Point;
extern __no_init __eeprom  int EEP_Span_ADC_Point; 



extern uint8_t System_Mode;
#define S_INIT_MODE 0
#define S_RUN_MODE 1
#define S_STOP_MODE 2

enum{
  S_Init_Mode,
  S_Run_Mode,
  S_Stop_Mode,
  S_OverCurrent_Error_Mode,
};

void RD_EEP_Value(void);
void System_Initialize_Timer(void);
void System_Operation(void);


#endif
