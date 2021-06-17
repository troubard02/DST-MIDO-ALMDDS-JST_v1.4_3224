#include "include.h"







#define FOSC 8000000
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

uint8_t USART_Mode;
#define RX_MODE 0 
#define TX_MODE 1

#define STX_HEADFILE 0x02 // '@'


uint8_t USART_Rxc_Complete_Flag,USART_Txc_Complete_Flag;

uint8_t USART_Check_Flag;

uint8_t USART_Txc_Buf[10];
uint8_t USART_Txc_Cnt;

uint8_t USART_Rxc_Buf[10];
uint8_t USART_Rxc_Cnt;


#define USART_TRANSMIT_BUFFER_SIZE 10
#define USART_RECEIVE_BUFFER_SIZE 10

#define USART_TRANSMIT_DATA_SIZE 5
#define USART_RECEIVE_DATA_SIZE 5


#define TIMER_ON 1
#define TIMER_OFF 0
#define TIMER_STOP 2

#define USART_TIMEROUT 500

uint8_t USART_Order;
uint8_t USART_Data;

uint8_t USART_Txd_Start_Flag;

typedef struct USART_BASIC_STRUCT           
{                          
  unsigned char Txd_Count;
  unsigned char Rxd_Count;
  unsigned char Txd_Buffer[USART_TRANSMIT_BUFFER_SIZE+1];
  unsigned char Rxd_Buffer[USART_RECEIVE_BUFFER_SIZE+1];
  unsigned char Schedule_M_Timer_REG;
  unsigned int Schedule_M_Timer_CNT;

  unsigned char Schedule_TX_Timer_Reg;
  unsigned int Schedule_TX_Timer_Cnt;

  unsigned char Schedule_RX_Timer_Reg;
  unsigned int Schedule_RX_Timer_Cnt;

  unsigned char Txd_Checksum;
  unsigned char Rxd_Checksum;

 
}USART_BASIC_STRUCT;

struct USART_BASIC_STRUCT USART_STRUCT;

uint8_t Usart_Txd_Buf[10]={0x01,0x02,0x03,0x04};
uint16_t Usart_Txd_Cnt;
#pragma   vector=USART_RX_vect
__interrupt void USART_RX_VECT()
{

  
  //uint8_t USART_Rxc_Data;
  __disable_interrupt();
  /*

  USART_Rxc_Data = UDR0;
  
  Usart_Rxd_Buf[Usart_Rxd_Cnt++]=USART_Rxc_Data;
  if(Usart_Rxd_Cnt>490)Usart_Rxd_Cnt=0;
  

  
  if(USART_Mode == RX_MODE){

    if(USART_STRUCT.Rxd_Count==0){
      if(USART_Rxc_Data == STX_HEADFILE){
        USART_STRUCT.Rxd_Buffer[0]=STX_HEADFILE;
        USART_STRUCT.Rxd_Count++;
        USART_STRUCT.Schedule_RX_Timer_Cnt=0;
        USART_STRUCT.Schedule_RX_Timer_Reg=1;
      }
    }
    else {
      if(USART_STRUCT.Rxd_Count<=USART_RECEIVE_DATA_SIZE){
        USART_STRUCT.Rxd_Buffer[USART_STRUCT.Rxd_Count++] = USART_Rxc_Data;
        if(USART_STRUCT.Rxd_Count==USART_RECEIVE_DATA_SIZE){
          USART_Rxc_Complete_Flag=1;
        }
      }
      
    }
  }*/
  __enable_interrupt();
  
}

#pragma   vector=USART_TX_vect
__interrupt void USART_TX_VECT()
{
  __disable_interrupt();

  if(USART_Mode==TX_MODE){

  
    //while(1){
      //if((UCSR0A&0x20)==0x20)break;
    //}

    if(USART_STRUCT.Txd_Count<USART_TRANSMIT_DATA_SIZE){
      UDR0 = USART_STRUCT.Txd_Buffer[USART_STRUCT.Txd_Count];
      USART_STRUCT.Txd_Count++;
    }
    else {
      USART_Txc_Complete_Flag=1;
    }
  
  }
  
}

void Usart_Reg_Initialize(void)
{

  USART_STRUCT.Schedule_RX_Timer_Cnt=0;
  USART_STRUCT.Schedule_RX_Timer_Reg=0;
  USART_STRUCT.Schedule_TX_Timer_Cnt=0;
  USART_STRUCT.Schedule_TX_Timer_Reg=0;

  USART_STRUCT.Rxd_Count=0;
  USART_STRUCT.Txd_Count=0;

}

void Usart_Send_Start_Byte(uint8_t Data)
{
  //while(!(UCSRA&(0x20)==0x20))

  while(1){
    if((UCSR0A&0x20)==0x20)break;
  }
  UDR0 = Data;

}


uint8_t Usart_Transaction_Delay_Timer_Reg;
uint16_t Usart_Transaction_Delay_Timer_Cnt;
void Usart_Transaction_Delay_Timer(void)
{
  /*
  if(Usart_Transaction_Delay_Timer_Reg==1){
    Usart_Transaction_Delay_Timer_Cnt++;
    if(Usart_Transaction_Delay_Timer_Cnt>5){
      Usart_Transaction_Delay_Timer_Cnt=0;
      Usart_Transaction_Delay_Timer_Reg=2;
    }
  }
  if(Usart_Transaction_Delay_Timer_Reg==2){

    UCSRB = (0<<RXCIE)|(1<<TXCIE)|(0<<RXEN)|(1<<TXEN);
      
    USART_Mode = TX_MODE;
    Usart_Send_Start_Byte(STX_HEADFILE);
    USART_STRUCT.Txd_Count++;
    USART_STRUCT.Schedule_TX_Timer_Reg=TIMER_ON;

    Usart_Transaction_Delay_Timer_Reg=0;

  }
*/


}


uint8_t Usart_Delay_Timer_Reg;
uint16_t Usart_Delay_Timer_Cnt;

void Usart_Timer(void)
{

  if(Usart_Delay_Timer_Reg==1){
    Usart_Delay_Timer_Cnt++;
    if(Usart_Delay_Timer_Cnt>50){
      Usart_Delay_Timer_Cnt=0; 
      
      USART_STRUCT.Txd_Count=0;
      for(uint16_t i=0;i<USART_TRANSMIT_DATA_SIZE;i++)
        USART_STRUCT.Txd_Buffer[i] = Usart_Txd_Buf[i];

      Usart_Send_Start_Byte(STX_HEADFILE);
      USART_STRUCT.Txd_Count++;

    }
  }
      
}


void Usart_Operation(void)
{
  uint16_t Usart_CheckSum_Value;
  uint8_t Usart_CheckSum;
  
  uint8_t Pressure_Value;
    
  __disable_interrupt();
 
  if(Real_Pressure_Value>200)Pressure_Value=200;
  else if(Real_Pressure_Value<0)Pressure_Value=0;
  else Pressure_Value = Real_Pressure_Value;
  Usart_Txd_Buf[0] = STX_HEADFILE;
  Usart_Txd_Buf[1] = System_Mode;  
  Usart_Txd_Buf[2] = Standard_Pressure_Value;
  Usart_Txd_Buf[3] = Pressure_Value;

 
  Usart_CheckSum_Value = Usart_Txd_Buf[0] + 
                         Usart_Txd_Buf[1] +
                         Usart_Txd_Buf[2] +
                         Usart_Txd_Buf[3];
  Usart_CheckSum = (uint8_t)Usart_CheckSum_Value;

  Usart_Txd_Buf[4] = Usart_CheckSum;
  __enable_interrupt();
  /*
  if(Usart_Delay_Timer_Reg==2){
    Usart_Send_Start_Byte(0xAB);
    Usart_Delay_Timer_Reg=1;
  }*/
/*
  if(USART_Txd_Start_Flag==1){
    Usart_Send_Start_Byte(STX_HEADFILE);
    USART_Txd_Start_Flag=0;
  }

*/

}




void Uasrt_Initialize(void)
{

  uint16_t ubrr=103;

  /* Set Baud Rate */
  UBRR0H=(uint8_t)(ubrr>>8);
  UBRR0L=(uint8_t)ubrr;

  UCSR0A = (1<<U2X0);

  /* Enable Receiver and Transmitter  
  UCSRB = (1<<RXCIE)|(1<<TXCIE)|(1<<RXEN)|(1<<TXEN);*/

  /* Enable Transmitter Module */
  UCSR0B = (0<<RXDIE0)|(1<<TXCIE0)|(0<<RXEN0)|(1<<TXEN0);

  /* Enable Receiver Module 
  UCSRB = (1<<RXCIE)|(0<<TXCIE)|(1<<RXEN)|(0<<TXEN);*/
  
  /* Set Frame Format : 8data, 1 stop bit, none parity bit */
  UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)
          |(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCSZ00);

  USART_Mode = TX_MODE;
  Usart_Delay_Timer_Reg=1;


}
