#include "CHECK.h"
#include "sys.h"
#include "delay.h"

void CHECK_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

u8 CHECK_Scan(void)
{
	u8 key = 0xFF;
	u8 temp;
	u32 moder_save;

	moder_save = GPIOA->MODER;
	GPIOA->MODER = (moder_save & ~0xF0UL) | 0x50UL;

	/* 列0: PA0=0 */
	CKEY_Data_Write(0xfe);
	temp = CKEY_Data_Read & 0x0F;
	if(temp != 0x0F)
	{
		delay_ms(10);
		temp = CKEY_Data_Read & 0x0F;
		if(temp != 0x0F)
		{
			switch(temp)
			{
				case 0x0E: key = 0;  break;
				case 0x0D: key = 1;  break;
				case 0x0B: key = 2;  break;
				case 0x07: key = 3;  break;
			}
			{ volatile u32 _t=0; while((CKEY_Data_Read & 0x0F)!=0x0F && ++_t<50000); }
		}
	}

	/* 列1: PA1=0 */
	CKEY_Data_Write(0xfd);
	temp = CKEY_Data_Read & 0x0F;
	if(temp != 0x0F)
	{
		delay_ms(10);
		temp = CKEY_Data_Read & 0x0F;
		if(temp != 0x0F)
		{
			switch(temp)
			{
				case 0x0E: key = 4;  break;
				case 0x0D: key = 5;  break;
				case 0x0B: key = 6;  break;
				case 0x07: key = 7;  break;
			}
			{ volatile u32 _t=0; while((CKEY_Data_Read & 0x0F)!=0x0F && ++_t<50000); }
		}
	}

	/* 列2: PA2=0 */
	CKEY_Data_Write(0xfb);
	temp = CKEY_Data_Read & 0x0F;
	if(temp != 0x0F)
	{
		delay_ms(10);
		temp = CKEY_Data_Read & 0x0F;
		if(temp != 0x0F)
		{
			switch(temp)
			{
				case 0x0E: key = 8;  break;
				case 0x0D: key = 9;  break;
				case 0x0B: key = 10; break;
				case 0x07: key = 11; break;
			}
			{ volatile u32 _t=0; while((CKEY_Data_Read & 0x0F)!=0x0F && ++_t<50000); }
		}
	}

	/* 列3: PA3=0 */
	CKEY_Data_Write(0xf7);
	temp = CKEY_Data_Read & 0x0F;
	if(temp != 0x0F)
	{
		delay_ms(10);
		temp = CKEY_Data_Read & 0x0F;
		if(temp != 0x0F)
		{
			switch(temp)
			{
				case 0x0E: key = 12; break;
				case 0x0D: key = 13; break;
				case 0x0B: key = 14; break;
				case 0x07: key = 15; break;
			}
			{ volatile u32 _t=0; while((CKEY_Data_Read & 0x0F)!=0x0F && ++_t<50000); }
		}
	}

	GPIOA->MODER = moder_save;
	return key;
}
