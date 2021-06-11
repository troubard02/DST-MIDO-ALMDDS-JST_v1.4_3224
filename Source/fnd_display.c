#include "include.h"


//#define START_PORT_VALUE 0x10

uint8_t FND_char[12] =
{ 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xF6, 0x02, 0x00 };
// 표시할 FND 0 ~ 9까지 배열값
uint8_t FND_value[3];         // FND 표시값
//unsigned char FND_select[3] = { 0xDC, 0xBC, 0x7C };
//unsigned char FND_select[3] = { 0x0B, 0x0D, 0x0E };
uint8_t FND_select[3] = { 0xC0,0xA0,0x60};
                                    // FND C1 ~ C3 1개씩 선택

uint8_t Display_Mode;
uint8_t Display_Number_Enable;

//-------------------------------------------------------------------
//                              FND Display
//-------------------------------------------------------------------

void FND_NUMtoCHAR_Transfer(int16_t reg)
{

  unsigned char a, b, c;

  if(reg>=0){

    a = reg / 100;
    reg = reg % 100;

    b = reg / 10;
    reg = reg % 10;

    if ( a == 0 ) a = 11;

    if ( ( a == 11 ) && ( b == 0 ) ) b = 11;

    c = reg;

    FND_value[0] = FND_char[a];
    FND_value[1] = FND_char[b];
    FND_value[2] = FND_char[c];
  }
  else {

    if(reg>(-100)){
      reg = abs(reg);

      b = reg / 10;
      reg = reg % 10;

      if ( b == 0 ) b = 11;

      c = reg;

      FND_value[0] = 0x02;
      FND_value[1] = FND_char[b];
      FND_value[2] = FND_char[c];
    }
    else {


    }
  }
}


unsigned int Fnd_Display_Cnt;
unsigned int Fnd_Digit_Mode;
unsigned int DISPLAY_MASK;


uint8_t SYSTEM_MODE;


void Fnd_Flicking_Rutine(uint8_t Fnd_Fir_Digit_Value,
                                    uint8_t Fnd_Sec_Digit_Value,
                                    uint8_t Fnd_thi_Digit_Value)
{
  
  if(Display_Delay_Cnt<1000){
    FND_value[0] = Fnd_Fir_Digit_Value;            // -
    FND_value[1] = Fnd_Sec_Digit_Value;            // -
    FND_value[2] = Fnd_thi_Digit_Value;            // -
  }
  else if((Display_Delay_Cnt>=1000)&&(Display_Delay_Cnt<2000)){
    FND_value[0] = 0x00;           // 
    FND_value[1] = 0x00;           // 
    FND_value[2] = 0x00;           //  
  }
  else if((Display_Delay_Cnt>=2000)&&(Display_Delay_Cnt<3000)){
    FND_value[0] = Fnd_Fir_Digit_Value;            // -
    FND_value[1] = Fnd_Sec_Digit_Value;            // -
    FND_value[2] = Fnd_thi_Digit_Value;            // -
  }
  else if((Display_Delay_Cnt>=3000)&&(Display_Delay_Cnt<4000)){
    FND_value[0] = 0x00;           // 
    FND_value[1] = 0x00;           // 
    FND_value[2] = 0x00;           //  
  }
  else if(Display_Delay_Cnt>4000){
    Display_Delay_Cnt=0;
    Display_Mode=D_Init_Mode;
  }      

}

void Fnd_Display_Timer(void)
{

  switch(Display_Mode){
  case D_Init_Mode:

    Display_Delay_Cnt++;
    if(Display_Delay_Cnt>2000){// 0.2 X 2000 = 400ms
      Display_Number_Enable=1;
      Display_Delay_Cnt=0;
    }      

    break;
  case D_Zero_Setting_Mode:
    if(System_Mode!=S_RUN_MODE){

      Display_Delay_Cnt++;
      //Fnd_Flicking_Rutine(FND_Z,FND__,FND_Z);
    }
    break;
  case D_High_Setting_Mode:
    if(System_Mode!=S_RUN_MODE){

      Display_Delay_Cnt++;
      //Fnd_Flicking_Rutine(FND_H,FND__,FND_H);
    }
    break;
  case D_Standard_Setting_Mode:
    Display_Delay_Cnt++;
    
    if(Display_Delay_Cnt>5000){
      Display_Delay_Cnt=0;
      Display_Mode=D_Init_Mode;
    }

    break;
  case D_AutoZero_Mode:

    Display_Delay_Cnt++;
    Fnd_Flicking_Rutine(0x02,0x02,0x02);
    break;
    
  }


}


void Fnd_Display(void)
{

  //FND_NUMtoCHAR_Transfer(123);



  switch(Display_Mode){
  case D_Init_Mode:

    switch(System_Mode){
    case S_INIT_MODE:
    case S_STOP_MODE:
      FND_NUMtoCHAR_Transfer(0);
      //FND_value[0] = FND_o;
      //FND_value[1] = FND_F;
      //FND_value[2] = FND_F;
      break;
    case S_RUN_MODE:
      if(Display_Number_Enable==1){
        Display_Number_Enable=0;
        if(Real_Pressure_Value<=200){
          if(abs(Real_Pressure_Value)<6)// 0~5 ==> Dispaly 0
            Real_Pressure_Value=0;
        }
        else {
          Real_Pressure_Value = 200;
        }
        if(Real_Pressure_Value<0)// disable minus (negative) number
          FND_NUMtoCHAR_Transfer(0);
        else 
          FND_NUMtoCHAR_Transfer(Real_Pressure_Value);
      }
      break;
    }    
    break;
    
  case D_Zero_Setting_Mode:
    break;
  case D_High_Setting_Mode:
    break;
  case D_Standard_Setting_Mode:
    FND_NUMtoCHAR_Transfer(Standard_Pressure_Value);
    break;
  }



  
/*
  switch(Display_Mode){
  case D_Init_Mode:      
    if(System_Mode==S_Run_Mode){
      if(Display_Number_Enable==1){
        if(Real_Pressure_Value<200){

          if(abs(Real_Pressure_Value)<3){
            FND_NUMtoCHAR_Transfer(0);
          }
          else {
            FND_NUMtoCHAR_Transfer(Real_Pressure_Value);
          }

        }
        else {
          FND_value[0]=FND_P;
          FND_value[1]=FND__;
          FND_value[2]=FND_u;
        }
            
        Display_Number_Enable=0;
      }
    }
    else
      //if(Display_Number_Enable==1){
        FND_NUMtoCHAR_Transfer(0);
        //Display_Number_Enable=0;
      //}
    break;
      
  case D_Standard_Setting_Mode:
    FND_NUMtoCHAR_Transfer(Standard_Pressure_Value);
      
    break;

  }
*/
}

uint16_t Display_Delay_Cnt;
uint8_t Display_Delay_Reg;


void Fnd_All_Off(void)
{
  PORTD &= ~(0xFC); 
  //PORTB &= ~(0xE0); // PB3 -> FND_5

}

void Fnd_Get_Port_Value(uint8_t Fnd_Value_Buf,
                                  uint8_t Fnd_Digit_Buf)
{
  uint8_t TempD_Port,TempB_Port;


  if((Fnd_Value_Buf&0x02)==0x02)// if FND's PD1 is ON
    PORTB |=0x08;
  else PORTB &= ~(0x08);

  TempD_Port = PIND&0x03; Fnd_Value_Buf = Fnd_Value_Buf&0xFC;
  TempD_Port = TempD_Port+Fnd_Value_Buf; 
  PORTD = TempD_Port;


  TempB_Port = PINB&0x1F; Fnd_Digit_Buf = Fnd_Digit_Buf&0xE0;
  TempB_Port = TempB_Port+Fnd_Digit_Buf; 
  PORTB = TempB_Port;  

}



void Fnd_Put_Data_Timer(void)
{
  

  Fnd_Display_Cnt++;
  
  switch(Fnd_Digit_Mode){
  case 0: // 처음에 먼저 FND를 끈다. 
    Fnd_All_Off();
    if(Fnd_Display_Cnt>2){ Fnd_Digit_Mode=1; Fnd_Display_Cnt=0; }
    break;
  case 1: // 첫번째 자릿수 ON 

    Fnd_Get_Port_Value(FND_value[0],FND_select[0]);
    
    if(Fnd_Display_Cnt>10){ Fnd_Digit_Mode=2; Fnd_Display_Cnt=0; }
    break;    
  case 2:    

    Fnd_All_Off();
    if(Fnd_Display_Cnt>2){ Fnd_Digit_Mode=3; Fnd_Display_Cnt=0; }
    break;
  case 3:

    Fnd_Get_Port_Value(FND_value[1],FND_select[1]);
    
    if(Fnd_Display_Cnt>10){ Fnd_Digit_Mode=4; Fnd_Display_Cnt=0; }
    break;    
  case 4:    

    Fnd_All_Off();
    if(Fnd_Display_Cnt>2){ Fnd_Digit_Mode=5; Fnd_Display_Cnt=0; }
    break;
  case 5:

    Fnd_Get_Port_Value(FND_value[2],FND_select[2]);
    
    if(Fnd_Display_Cnt>10){ Fnd_Digit_Mode=0; Fnd_Display_Cnt=0; }
    break;    
  }    
  

  /*
  switch(System_Mode){
  case S_Init_Mode:
    
    break;
  case S_Run_Mode:
    // 300ms마다 압력값을 디스플레이 해준다. 
    
    switch(Display_Mode){
    case D_Init_Mode:
      
      Display_Delay_Cnt++;
      if(Display_Delay_Cnt>2000){// 0.2 X 2000 = 400ms
        Display_Number_Enable=1;
        Display_Delay_Cnt=0;
      }      
      
      break;
    case D_Standard_Setting_Mode:
      
      Display_Delay_Cnt++;
      
      if(Display_Delay_Cnt>5000){
        Display_Delay_Cnt=0;
        Display_Mode=D_Init_Mode;
      }
      
      break;
      
    case D_AutoZero_Mode:
      
      Display_Delay_Cnt++;
      if(Display_Delay_Cnt<1000){
        FND_value[0] = 0x02;            // -
        FND_value[1] = 0x02;            // -
        FND_value[2] = 0x02;            // -
      }
      else if((Display_Delay_Cnt>=1000)&&(Display_Delay_Cnt<2000)){
        FND_value[0] = 0x00;           // 
        FND_value[1] = 0x00;           // 
        FND_value[2] = 0x00;           //  
      }
      else if((Display_Delay_Cnt>=2000)&&(Display_Delay_Cnt<3000)){
        FND_value[0] = 0x02;            // -
        FND_value[1] = 0x02;            // -
        FND_value[2] = 0x02;            // -
      }
      else if((Display_Delay_Cnt>=3000)&&(Display_Delay_Cnt<4000)){
        FND_value[0] = 0x00;           // 
        FND_value[1] = 0x00;           // 
        FND_value[2] = 0x00;           //  
      }
      else if(Display_Delay_Cnt>4000){
        Display_Delay_Cnt=0;
        Display_Mode=D_Init_Mode;
      }      
      break;      
      
    }
    


    break;
  case S_Stop_Mode:
    switch(Display_Mode){
    case D_Init_Mode:
      Display_Number_Enable=1;
      break;
    case D_Standard_Setting_Mode:
      Display_Delay_Cnt++;
      if(Display_Delay_Cnt>5000){
        Display_Delay_Cnt=0;
        Display_Mode=D_Init_Mode;
      }
      
      break;
    case D_AutoZero_Mode:
      
      Display_Delay_Cnt++;
      if(Display_Delay_Cnt<1000){
        FND_value[0] = 0x02;            // -
        FND_value[1] = 0x02;            // -
        FND_value[2] = 0x02;            // -
      }
      else if((Display_Delay_Cnt>=1000)&&(Display_Delay_Cnt<2000)){
        FND_value[0] = 0x00;           // 
        FND_value[1] = 0x00;           // 
        FND_value[2] = 0x00;           //  
      }
      else if((Display_Delay_Cnt>=2000)&&(Display_Delay_Cnt<3000)){
        FND_value[0] = 0x02;            // -
        FND_value[1] = 0x02;            // -
        FND_value[2] = 0x02;            // -
      }
      else if((Display_Delay_Cnt>=3000)&&(Display_Delay_Cnt<4000)){
        FND_value[0] = 0x00;           // 
        FND_value[1] = 0x00;           // 
        FND_value[2] = 0x00;           //  
      }
      else if(Display_Delay_Cnt>4000){
        Display_Delay_Cnt=0;
        Display_Mode=D_Init_Mode;
      }      
      break;
      
    }
    
    break;
  }
  */
  
}