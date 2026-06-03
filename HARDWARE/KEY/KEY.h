#ifndef __KEY_H
#define __KEY_H
#include "sys.h"

#define KEY0 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)
#define KEY1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)
#define KEY2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)

#define KEY_VAL0  1
#define KEY_VAL1  2
#define KEY_VAL2  3

void KEY_Init(void);
u8 KEY_Scan(void);

#endif
