#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H



#define BIT0 0x01
#define BIT1 0x02
#define BIT2 0x04
#define BIT3 0x08
#define BIT4 0x10
#define BIT5 0x20
#define BIT6 0x40
#define BIT7 0x80


void GPIO_PortA_OutHigh(uint8_t PAx);
void GPIO_PortA_OutLow(uint8_t PAx);
void GPIO_PortB_OutHigh(uint8_t PAx);
void GPIO_PortB_OutLow(uint8_t PAx);
void GPIO_PortC_OutHigh(uint8_t PAx);
void GPIO_PortC_OutLow(uint8_t PAx);
void GPIO_PortD_OutHigh(uint8_t PAx);
void GPIO_PortD_OutLow(uint8_t PAx);




#endif




