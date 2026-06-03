#ifndef __LED_H
#define __LED_H
#include "sys.h"

#define LED_Data_Write(num) PEout(8)=(num&0x01);PEout(9)=(num>>1)&0x01;PEout(10)=(num>>2)&0x01;PEout(11)=(num>>3)&0x01;PEout(12)=(num>>4)&0x01;PEout(13)=(num>>5)&0x01;PEout(14)=(num>>6)&0x01;PEout(15)=(num>>7)&0x01;

#define LED1 PEout(8)
#define LED2 PEout(9)
#define LED3 PEout(10)
#define LED4 PEout(11)
#define LED5 PEout(12)
#define LED6 PEout(13)
#define LED7 PEout(14)
#define LED8 PEout(15)

void LED_clear(void);
void LED_Init(void);
void LED_on(void);
#endif
