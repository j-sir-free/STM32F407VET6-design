#ifndef __CHECK_H
#define __CHECK_H
#include "sys.h"

#define CKEY0 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define CKEY1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)
#define CKEY2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define CKEY3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define CKEY4 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)
#define CKEY5 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define CKEY6 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define CKEY7 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)

#define CKEY_Data_Write(num) PAout(0)=(num&0x01);PAout(1)=(num>>1)&0x01;PAout(2)=(num>>2)&0x01;PAout(3)=(num>>3)&0x01;
#define CKEY_Data_Read       (CKEY4|(CKEY5<<1)|(CKEY6<<2)|(CKEY7<<3))

void CHECK_Init(void);
u8 CHECK_Scan(void);

#endif
