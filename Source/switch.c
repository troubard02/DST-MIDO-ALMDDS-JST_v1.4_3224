#include "include.h"



uint8_t Start_Switch_State, Start_Port_Value;


//#if MCU_TYPE

#define PORT_START_SWITCH 0x10 // PB4 


//#else 


//#define PORT_START_SWITCH 0x10 // PB4 

//#endif

uint8_t Switch_Hold_Timer_Reg;
uint16_t Switch_Hold_Timer_Cnt;





/*! \brief Switch_Get_Delay() rutine
 *   to prohibit chattering comfirm value one more time
 *   KEY_INIT_STATE : NO VALUE
 *   KEY_READY_STATE : Get Value
 *   KEY_COMPLETE_STATE : Comfirm Value 
 */
uint8_t Switch_Hold_Get_Delay(uint8_t Key_Mask_Unit,// PIN MAP
                                  uint8_t Key_Value, // current Pin Value
                                  uint8_t Key_State) // Switch_x_State
{

  if((Key_Value&Key_Mask_Unit)!=Key_Mask_Unit)// if input Key value
  {
    switch (Key_State){
    case KEY_INIT_STATE: Key_State=KEY_HIGH_READY_STATE; break;
    case KEY_HIGH_READY_STATE: Key_State=KEY_COMPLETE_STATE; break;
    case KEY_COMPLETE_STATE: break;// Key Input Complete
    case KEY_LOW_READY_STATE : Key_State=KEY_COMPLETE_STATE; break;
    default :
      Key_State=KEY_INIT_STATE;
      break;
    }
  }
  else { // if no any key value
    switch (Key_State){
    case KEY_INIT_STATE:  break;
    case KEY_HIGH_READY_STATE: Key_State=KEY_INIT_STATE; break;
    case KEY_COMPLETE_STATE: Key_State=KEY_LOW_READY_STATE; break;// Key Input Complete
    case KEY_LOW_READY_STATE : Key_State=KEY_INIT_STATE; break;
    default :
      Key_State=KEY_INIT_STATE;
      break;
    }


  } 

  return Key_State;
}
  

void Switch_Start_Check(void)
{
  Start_Port_Value = (PINB&PORT_START_SWITCH);
  Start_Switch_State = Switch_Hold_Get_Delay(PORT_START_SWITCH,
                                            Start_Port_Value,
                                            Start_Switch_State);
}

void Switch_Timer(void)
{

  Switch_Hold_Timer_Cnt++;
  if(Switch_Hold_Timer_Cnt>1000){ // 1000x0.1ms = 100ms
    Switch_Hold_Timer_Cnt=0;
    Switch_Start_Check();
  }


  /*
  if(Start_Switch_State==KEY_COMPLETE_STATE)// �⵿ ��ȣ�� ������ 
    System_Mode=S_Run_Mode;
  else System_Mode=S_Stop_Mode; // �⵿ ��ȣ�� ������ 
*/
}

