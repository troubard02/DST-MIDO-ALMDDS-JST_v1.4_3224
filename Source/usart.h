#ifndef __USART_H
#define __USART_H





void Usart_Timer(void);
void Uasrt_Initialize(void);
void Usart_Operation(void);
void Usart_Transaction_Delay_Timer(void);


#define U2X0 1
#define RXDIE0 7
#define TXCIE0 6
#define UDRIE0 5
#define RXEN0 4
#define TXEN0 3
#define UCSZ02 2
#define RXB80 1
#define TXB80 0


#define UMSEL01 7 // USART Mode 
#define UMSEL00 6 // 00 : Asyc Mode, 01 Sync Amode, 11 :Master SPI
#define UPM01 5   // Parity Bit 
#define UPM00 4   // 00 : Disable, 01 :Reserved, 10 : Enabled, Even Parity, 11 : Enabled, Odd Parity
#define USBS0 3   // Stop Bit Select, 0 : 1-bit, 1 : 2-bit
#define UCSZ01 2  // Character Size 
#define UCSZ00 1  // 011 : 8-bit, 111 : 9-bit,010 : 7-bit, 001 : 6-bit, 000: 5-bit
#define UCPOL0 0  // Rising or Falling Edge





#endif
