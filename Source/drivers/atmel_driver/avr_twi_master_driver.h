/*****************************************************************************
* Atmel Corporation
* File              : TWI_master.h
* Revision          : 1.13
* Date              : 24. mai 2004 11:31:22 
* Updated by        : ltwa
* Support mail      : avr@atmel.com
* Supported devices : All devices with a TWI module can be used.
* AppNote           : AVR315 - TWI master Implementation
* Description       : Header file for TWI_master.c
*                     Include this file in the application.
****************************************************************************/


/****************************************************************************
  TWI Status/Control register definitions
****************************************************************************/
#ifndef __TWI_MASTER_H
#define __TWI_MASTER_H

#include "include.h"
//#include <stdint.h>
//#include "main.h"



#define TWI_BUFFER_SIZE 16   // Set this to the largest message size that will be sent including address byte.
#define TWI_BITRATE 100       //TWI_BITRATE in kHz

extern uint8_t TWI_buf[TWI_BUFFER_SIZE];     // Transceiver buffer

#define TWI_PSBR (F_CPU/2000/TWI_BITRATE-8)// TWI Bit rate Register setting.
#if TWI_PSBR > 0x3FC0
   #define TWI_TWPS 0x03
   #define TWI_TWBR 0xFF
   #define MESSAGE 'a'
#elif TWI_PSBR > 0xFF0
   #define TWI_TWPS 0x03
   #define TWI_TWBR TWI_PSBR/64
   #define MESSAGE 'b'
#elif TWI_PSBR > 0x3FC
   #define TWI_TWPS 0x02
   #define TWI_TWBR TWI_PSBR/16
   #define MESSAGE 'c'
#elif TWI_PSBR > 0xFF
   #define TWI_TWPS 0x01
   #define TWI_TWBR TWI_PSBR/4
   #define MESSAGE 'd'
#else
   #define TWI_TWPS 0x00
   #define TWI_TWBR TWI_PSBR
   #define MESSAGE 'e'
#endif

/****************************************************************************
  Global definitions
****************************************************************************/

union TWI_statusReg_f                    // Status byte holding flags.
{
   volatile uint8_t all;
   struct
   {
       volatile uint8_t lastTransOK:1;
       volatile uint8_t unusedBits:7;
   };
};

extern union TWI_statusReg_f TWI_statusReg;

/****************************************************************************
  Function definitions
****************************************************************************/
void TWI_Master_Initialise(void);
uint8_t TWI_Transceiver_Busy(void);
uint8_t TWI_Get_State_Info(void);
void TWI_Start_Transceiver_With_Data(uint8_t *, uint8_t);
void TWI_Start_Transceiver(void);
uint8_t TWI_Get_Data_From_Transceiver(uint8_t *, uint8_t);

/****************************************************************************
  Bit and byte definitions
****************************************************************************/
#define TWI_READ_BIT  0       // Bit position for R/W bit in "address byte".
#define TWI_ADR_BITS  1       // Bit position for LSB of the slave address bits in the init byte.

#define TRUE          1
#define FALSE         0

/****************************************************************************
  TWI State codes
****************************************************************************/
// General TWI Master status codes
#define TWI_START                  0x08  // START has been transmitted  
#define TWI_REP_START              0x10  // Repeated START has been transmitted
#define TWI_ARB_LOST               0x38  // Arbitration lost

// TWI Master Transmitter status codes
#define TWI_MTX_ADR_ACK            0x18  // SLA+W has been tramsmitted and ACK received
#define TWI_MTX_ADR_NACK           0x20  // SLA+W has been tramsmitted and NACK received 
#define TWI_MTX_DATA_ACK           0x28  // Data byte has been tramsmitted and ACK received
#define TWI_MTX_DATA_NACK          0x30  // Data byte has been tramsmitted and NACK received 

// TWI Master Receiver status codes
#define TWI_MRX_ADR_ACK            0x40  // SLA+R has been tramsmitted and ACK received
#define TWI_MRX_ADR_NACK           0x48  // SLA+R has been tramsmitted and NACK received
#define TWI_MRX_DATA_ACK           0x50  // Data byte has been received and ACK tramsmitted
#define TWI_MRX_DATA_NACK          0x58  // Data byte has been received and NACK tramsmitted

// TWI Slave Transmitter status codes
#define TWI_STX_ADR_ACK            0xA8  // Own SLA+R has been received; ACK has been returned
#define TWI_STX_ADR_ACK_M_ARB_LOST 0xB0  // Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
#define TWI_STX_DATA_ACK           0xB8  // Data byte in TWDR has been transmitted; ACK has been received
#define TWI_STX_DATA_NACK          0xC0  // Data byte in TWDR has been transmitted; NOT ACK has been received
#define TWI_STX_DATA_ACK_LAST_BYTE 0xC8  // Last data byte in TWDR has been transmitted (TWEA = '0'); ACK has been received

// TWI Slave Receiver status codes
#define TWI_SRX_ADR_ACK            0x60  // Own SLA+W has been received ACK has been returned
#define TWI_SRX_ADR_ACK_M_ARB_LOST 0x68  // Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
#define TWI_SRX_GEN_ACK            0x70  // General call address has been received; ACK has been returned
#define TWI_SRX_GEN_ACK_M_ARB_LOST 0x78  // Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_ACK       0x80  // Previously addressed with own SLA+W; data has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_NACK      0x88  // Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
#define TWI_SRX_GEN_DATA_ACK       0x90  // Previously addressed with general call; data has been received; ACK has been returned
#define TWI_SRX_GEN_DATA_NACK      0x98  // Previously addressed with general call; data has been received; NOT ACK has been returned
#define TWI_SRX_STOP_RESTART       0xA0  // A STOP condition or repeated START condition has been received while still addressed as Slave

// TWI Miscellaneous status codes
#define TWI_NO_STATE               0xF8  // No relevant state information available; TWINT = '0'
#define TWI_BUS_ERROR              0x00  // Bus error due to an illegal START or STOP condition



// TWI TWCR 
#define TWINT 7
#define TWEA  6
#define TWSTA 5
#define TWSTO 4
#define TWWC  3
#define TWEN  2
#define TWIE  0

#endif