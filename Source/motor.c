#include "include.h"




uint8_t Motor_Status;
#define MOTER_CLOSE 1
#define MOTER_ACT 2
#define MOTER_OPEN 3
#define MOTER_OFF 4
#define MOTER_AUTO_ZERO 5

uint8_t OverCurrent_Status;
#define OVERCURRENT 1
#define NORMALCURRENT 0

uint16_t PWM_Count;
uint8_t PWM_Enable;
uint8_t Timer_Motor_TimeOver_Reg;
uint32_t Timer_Motor_TimeOver_Cnt;
uint8_t Timer_Motor_OverCurrent_Reg;
uint16_t Timer_Motor_OverCurrent_Cnt;

#define OVER_CURRENT_TIME 300 // 300* 0.2ms = 60ms
#define TIME_OVER_TIME 150000 // 60000 * 0.2ms = 12000ms


#define PWM_ALL_COUNT 5000 // 5000 * 0.2ms = 1000ms 
uint16_t PWM_Off_Count=4950;

int16_t PWM_Current_Value;
uint16_t PWM_Count;



void Motor_REG_Initialize(void)
{
  PWM_Count=0; 
  PWM_Enable=0;
  Timer_Motor_TimeOver_Reg=0;
  Timer_Motor_TimeOver_Cnt=0;
  Timer_Motor_OverCurrent_Reg=0;
  Timer_Motor_OverCurrent_Cnt=0;
  
}

uint8_t Pressure_Status;
uint8_t Pressure_Old_Status;
#define NO_PRESSURE 1
#define LOW_PRESSURE 2
#define LOW_PWM_PRESSURE 3
#define MIDDLE_PRESSURE 4
#define HIGH_PWM_PRESSURE 5
#define HIGH_PRESSURE 6

uint8_t Motor_Status;
uint8_t Motor_Close_Stop;
uint8_t Motor_Open_Stop;


#define LOW_PRESSURE_DIFF_VALUE 5
#define HIGH_PRESSURE_DIFF_VALUE 5
#define LOW_PWM_DIFF_VALUE 0
#define HIGH_PWM_DIFF_VALUE 0


//#define STANDARD_PRESSURE 50

void Motor_Get_Pressure_Status(void)
{
  
  if(System_Mode==S_Run_Mode){
    
    // ���� �з��� ���� �з�-3 ���� �۾����� 
    if((Real_Pressure_Value<Standard_Pressure_Value-LOW_PRESSURE_DIFF_VALUE)){//3
      
      if(Pressure_Status==LOW_PWM_PRESSURE){
        if(Real_Pressure_Value==Standard_Pressure_Value-LOW_PRESSURE_DIFF_VALUE-1){//4
          
        }
        else{
          Pressure_Status=LOW_PRESSURE;
        }
      }
      else Pressure_Status=LOW_PRESSURE;
    }
    // ���� �з��� ���� �з� -3�� ���� �з� ���̿� ������ 
    else if((Real_Pressure_Value>=Standard_Pressure_Value-LOW_PRESSURE_DIFF_VALUE) //3
            &&(Real_Pressure_Value<Standard_Pressure_Value-LOW_PWM_DIFF_VALUE))//0
      Pressure_Status=LOW_PWM_PRESSURE;
    // ���� �з��� ���ؾз� �̸� 
    /*
    else if((Real_Pressure_Value<=Standard_Pressure_Value+HIGH_PWM_DIFF_VALUE)
            &&(Real_Pressure_Value>=Standard_Pressure_Value-LOW_PWM_DIFF_VALUE))
    */
    else if(Real_Pressure_Value==Standard_Pressure_Value)
      Pressure_Status=MIDDLE_PRESSURE;
    // ���� �з��� ���� �з� +3�� ���ؾз� ���̿� ������ 
    else if((Real_Pressure_Value>=Standard_Pressure_Value+HIGH_PWM_DIFF_VALUE)&&
            (Real_Pressure_Value<Standard_Pressure_Value+HIGH_PRESSURE_DIFF_VALUE))
      Pressure_Status=HIGH_PWM_PRESSURE;
    // ���� �з��� ���� �з� +3 ���� ũ�� 200 �̳��� ������ 
    else if((Real_Pressure_Value>Standard_Pressure_Value+HIGH_PRESSURE_DIFF_VALUE))
    {
    
      if(Pressure_Status==HIGH_PWM_PRESSURE){
        if(Real_Pressure_Value==Standard_Pressure_Value+HIGH_PRESSURE_DIFF_VALUE+1){
        }
        else{
          Pressure_Status=HIGH_PRESSURE;
        }
      }
      else Pressure_Status=HIGH_PRESSURE;      
  
    }
    

    
  }
  else if(System_Mode==S_Stop_Mode)Pressure_Status=NO_PRESSURE;
  

}

void Motor_Change_Status_Process(void)
{
  
  /*  ��� ��ȭ�� ���� �� , Timer_Motor_TimeOver_Reg,
      Motor_Open_Stop, Motor_Close_Stop ���� */
  if(Pressure_Status!=Pressure_Old_Status){
    //������ ���ؼ� ��� ��ȭ�� ������ �ѹ� �����Ѵ�. 
    
    Motor_REG_Initialize();//���õ� Ÿ�̸ӵ��� ������ �ʱ�ȭ ���ش�. 
    
    switch(Pressure_Status){
    case NO_PRESSURE:
    case HIGH_PRESSURE:
      if((Pressure_Old_Status)==HIGH_PWM_PRESSURE){
        // HIGH_PWM ���� HIGH�� �ö� ��� 
        if(Motor_Status== MOTER_CLOSE){
          //���Ͱ� �̹� ���� ��쿡�� 
          Motor_Close_Stop=1;
        }
        else {
          //���Ͱ� �� ���� ��쿡�� 
          Timer_Motor_TimeOver_Reg=1;// Ÿ�̸� ���� 
          Motor_Open_Stop=0;//
        }
      }
      else {
        // ������ ��忡�� HIGH�� �ö� ��� 
          Timer_Motor_TimeOver_Reg=1;// Ÿ�̸� ���� 
          Motor_Open_Stop=0;//
      }
    break;
    case LOW_PRESSURE:
      if(Pressure_Old_Status==LOW_PWM_PRESSURE){ 
        // LOW_PWM ���� LOW�� ������ ��� 
        if(Motor_Status== MOTER_OPEN){
          // ���Ͱ� �̹� ���µ� �����̸� 
          Motor_Open_Stop=1;
        }
        else {
          // ���Ͱ� �� ���� �����̸� 
          Timer_Motor_TimeOver_Reg=1;
          Motor_Close_Stop=0;
        }
      }
      else {
        // ������ ��忡�� �ö�� �����̸� 
          Timer_Motor_TimeOver_Reg=1;//
          Motor_Close_Stop=0;//
      }
    
    break;
    case LOW_PWM_PRESSURE:
      Motor_Close_Stop=0;
      
    break;
    case MIDDLE_PRESSURE:

    break;
    case HIGH_PWM_PRESSURE:
      Motor_Open_Stop=0;
    break;
    }
    Pressure_Old_Status=Pressure_Status;
    
    
  }  
}





void Motor_Control(void)
{
  
  //if(Motor_Delay_Timer_Reg!=1){
  
  /* ������ ���¿� ���� ���͸� �����Ѵ�. */
  switch(Pressure_Status){
  case NO_PRESSURE:
    if(Motor_Close_Stop==0){ // ���Ͱ� �� ������ ���� �����̸� 
        Alarm2_Out();//���� 
    }
    else 
      Alarm2_Off();
    break;
  case LOW_PRESSURE:
    if(Motor_Open_Stop==0){// ���Ͱ� �� ������ ���� �����̸� 
      Alarm1_Out();//����
  }
    else 
      Alarm1_Off();
    break;
  case LOW_PWM_PRESSURE:
    PWM_Enable=1;
    break;
  case MIDDLE_PRESSURE:

    Alarm1_Off();
    Alarm2_Off();
    break;
  case HIGH_PWM_PRESSURE:
    PWM_Enable=1;
    break;
  case HIGH_PRESSURE:
    if(Motor_Close_Stop==0){
      Alarm2_Out();//����
    }
    else 
      Alarm2_Off();
    break;
  default:

    Alarm1_Off();
    Alarm2_Off();
    break;
  }
  
}

uint8_t Motor_Status_old;
uint8_t Motor_Status_Cnt;


void Motor_Get_LimitSwitch_Value(void)
{
  
  if((ADC_Motor_Switch_Value>400)&&(ADC_Motor_Switch_Value<550)){
    if(Motor_Status_old!=Motor_Status){
      if(Motor_Status_old==MOTER_ACT)
        Motor_Status=MOTER_ACT;
      Motor_Status_old = MOTER_ACT;
    }
    else Motor_Status_old = MOTER_ACT;
  }
  else if((ADC_Motor_Switch_Value>580)&&(ADC_Motor_Switch_Value<=800)){
    

    if(Motor_Status_old!=Motor_Status){
      if(Motor_Status_old==MOTER_OPEN)
        Motor_Status=MOTER_OPEN;
      Motor_Status_old = MOTER_OPEN;
    }    
    else Motor_Status_old = MOTER_OPEN;
  }
  else if(ADC_Motor_Switch_Value>800){


    
    if(Motor_Status_old!=Motor_Status){
      if(Motor_Status_old==MOTER_CLOSE)
        Motor_Status=MOTER_CLOSE;
      Motor_Status_old = MOTER_CLOSE;
    }    
    else Motor_Status_old = MOTER_CLOSE;
  }
  
  else if(ADC_Motor_Switch_Value<200){

    
    if(Motor_Status_old!=Motor_Status){
      if(Motor_Status_old==MOTER_AUTO_ZERO)
        Motor_Status=MOTER_AUTO_ZERO;
      Motor_Status_old = MOTER_AUTO_ZERO;
    }    
    else Motor_Status_old = MOTER_AUTO_ZERO;
  }
}
  
  


void Motor_AutoZero_Process(void)
{
  
  if(Motor_Status == MOTER_AUTO_ZERO)
    ADC_Auto_Zero_Process();
  
}


void Motor_Get_OverCurrent_Value(void)
{
  
  /* ����Ŀ��Ʈ�� �Է��� �޾� Timer_Motor_OverCurrent_Reg�� �����Ѵ�.. */
  
  if(ADC_Overcurrent_Value>200){
    if(Timer_Motor_OverCurrent_Reg!=1){
      Timer_Motor_OverCurrent_Reg=1;
    }
  }
  else {
    Timer_Motor_OverCurrent_Reg=0;
    Timer_Motor_OverCurrent_Cnt=0;
  }
    
}


void Motor_Timer(void)
{
  
   /////////////// OVERCURRENT SENSEING TIMER ////////////////////
  /*System_Mode <= S_OverCurrent_Error_Mode */
  
  if(Timer_Motor_OverCurrent_Reg==1){
    Timer_Motor_OverCurrent_Cnt++;
    if(Timer_Motor_OverCurrent_Cnt==5000){// 0.2ms * = 2000

      Timer_Motor_OverCurrent_Cnt=0;
      Timer_Motor_OverCurrent_Reg=2;
      Alarm1_Off();
      Alarm2_Off();
      
      Motor_REG_Initialize();
      //System_Mode=S_OverCurrent_Error_Mode;
      //PORTC |= 0x08;// Ȯ�� ��ȣ 
      
      switch(Pressure_Status){
      case NO_PRESSURE: // if the system is in off mode..
        Motor_Close_Stop=1; 
        PORTC&=(~0x08);
        
        break;
      case LOW_PRESSURE: // if the system is in run mode.. and low pressure mode  
        Motor_Open_Stop=1;
        PORTC |= 0x08;// Ȯ�� ��ȣ 
        break;
      case HIGH_PRESSURE:
        Motor_Close_Stop=1;
        //PORTC&=(~0x08);
        break;
      default:
        break;
      }
      
    }
  }

  if(Pressure_Status==HIGH_PRESSURE)PORTC |= 0x08;// Ȯ�� ��ȣ 
  ////////////////// MOTOR TIMEOVER TIMER ///////////////////////
  /**/
  if(Timer_Motor_TimeOver_Reg==1){
    Timer_Motor_TimeOver_Cnt++;
    
    if((Timer_Motor_TimeOver_Cnt<TIME_OVER_TIME)){

      switch(Pressure_Status){
      case NO_PRESSURE:
      case HIGH_PRESSURE:
        if(Motor_Status == MOTER_CLOSE){
          Motor_Close_Stop=1;
        }
        if(Motor_Status == MOTER_ACT){
          Motor_Close_Stop=0;
        }
        
        //if(Pressure_Status==NO_PRESSURE)PORTC&=(~0x08);
        break;
      case LOW_PRESSURE:
        if(Motor_Status == MOTER_OPEN){//������ ������ Ȯ�ν�ȣ�� �����ش�. 
          Motor_Open_Stop=1;
          PORTC |= 0x08;// Ȯ�� ��ȣ 
        }
        if(Motor_Status == MOTER_ACT){
          Motor_Open_Stop=0;
        }          
        break;
      default:
        break;
      }
  
    }
    
    if(Timer_Motor_TimeOver_Cnt>=TIME_OVER_TIME ){
      Timer_Motor_TimeOver_Cnt=0;
      Timer_Motor_TimeOver_Reg=2;
      if((Pressure_Status==NO_PRESSURE)||(Pressure_Status==HIGH_PRESSURE))
        Motor_Close_Stop=1;
      else if((Pressure_Status==LOW_PRESSURE))
        Motor_Open_Stop=1;
      
      if(Pressure_Status==NO_PRESSURE)PORTC&=(~0x08);
    }
    
  }
    
  if(System_Mode==S_Stop_Mode)
    if(Motor_Status == MOTER_CLOSE) //0V �� ������ 
    PORTC&=(~0x08);

  
  if(PWM_Enable==1){

    switch(Pressure_Status){
    case LOW_PRESSURE:
      
      break;
    case LOW_PWM_PRESSURE:
      
      if(Real_Pressure_Value > Standard_Pressure_Value-6){
        PWM_Off_Count = 4850;//970ms  /* 2020-02-09 Modify */
        //PWM_Off_Count = 4500;// 900ms
      }
      
      
      else { /* 2020-02-09 Modify */
        //PWM_Off_Count = Standard_Pressure_Value*5;
        PWM_Off_Count = 4500;// 900ms
      }
      

      break;
    case MIDDLE_PRESSURE:
      break;
    case HIGH_PWM_PRESSURE:
      
      if(Real_Pressure_Value < Standard_Pressure_Value+6){
        PWM_Off_Count = 4850;//970ms  /* 2020-02-09 Modify */ 
        //PWM_Off_Count = 4500;// 900ms 
      }
      
      else { /* 2020-02-09 Modify */
        //PWM_Off_Count = Standard_Pressure_Value*5;
        PWM_Off_Count = 4500;// 900ms
      }
      

      break;
    case HIGH_PRESSURE:
      break;
    case NO_PRESSURE:
      break;
    default:
      break;
    }
    
    
    
    PWM_Count++;
    if(PWM_Count<PWM_Off_Count){// ������ 4500
      Alarm1_Off();
      Alarm2_Off();
    }
    else if((PWM_Count>=PWM_Off_Count)&&(PWM_Count<PWM_ALL_COUNT)){ // ������ 4500 5000
      if(Pressure_Status==LOW_PWM_PRESSURE)
        if(Motor_Open_Stop==0){
          if(Motor_Status == MOTER_OPEN){// ������ ������ Ȯ�ν�ȣ�� �����ش�. 
            PORTC |= 0x08;// Ȯ�� ��ȣ 
            Alarm1_Off();
          }
          else Alarm1_Out();
        }
        else Alarm1_Off();
      else if(Pressure_Status==HIGH_PWM_PRESSURE)
        if(Motor_Close_Stop==0){
          if(Motor_Status == MOTER_CLOSE)
            Alarm2_Off();
          else Alarm2_Out();
        }
        else Alarm2_Off();
    }
    if(PWM_Count>=PWM_ALL_COUNT)PWM_Count=0;
  }
  

}



void Motor_Operation(void)
{
  
  Motor_Get_Pressure_Status();
  Motor_Change_Status_Process();
  Motor_Control();
  
  Motor_AutoZero_Process();
  Motor_Get_OverCurrent_Value();
  
}