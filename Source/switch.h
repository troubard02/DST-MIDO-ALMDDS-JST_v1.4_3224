#ifndef __SWITCH_H
#define __SWITCH_H




#define PORT_START_SWITCH 0x10 // PB4 
#define START_PORT_VALUE 0x10





void Switch_Timer(void);



typedef enum SW_STATE_enum{
  KEY_INIT_STATE=0,
  KEY_READY_STATE=1,
  KEY_COMPLETE_STATE=2,
  KEY_OVERDUE_STATE=3,
  KEY_OFF_STATE=4,
  KEY_HIGH_READY_STATE=5,
  KEY_LOW_READY_STATE=6,
}SW_STATE_t;





extern uint8_t Start_Switch_State;





#endif

