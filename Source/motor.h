#ifndef MOTOR_H
#define MOTOR_H

extern uint8_t Motor_Status;
#define MOTER_CLOSE 1
#define MOTER_ACT 2
#define MOTER_OPEN 3
#define MOTER_OFF 4
#define MOTER_AUTO_ZERO 5


void Motor_REG_Initialize(void);
void Motor_Get_Pressure_Status(void);
void Motor_Change_Status_Process(void);
void Motor_Control(void);
void Motor_Get_LimitSwitch_Value(void);
void Motor_Get_OverCurrent_Value(void);
void Motor_Operation(void);
void Motor_Timer(void);
void Motor_AutoZero_Process(void);
void Motor_Delay_Timer(void);

#endif