#include "key.h"
#include "sys.h"
#include "delay.h"

void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

u8 KEY_Scan(void)
{
	static u8 key_up = 1;
	if(key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0))
	{
		delay_ms(10);
		key_up = 0;
		if(KEY0 == 0)      return KEY_VAL0;
		else if(KEY1 == 0) return KEY_VAL1;
		else if(KEY2 == 0) return KEY_VAL2;
	}
	else if(KEY0 == 1 && KEY1 == 1 && KEY2 == 1)
	{
		key_up = 1;
	}
	return 0;
}
