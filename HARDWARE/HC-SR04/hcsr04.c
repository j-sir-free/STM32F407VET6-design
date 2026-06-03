#include "hcsr04.h"
#include "delay.h"
#include "timer.h"

void UltrasonicWave_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void UltrasonicWave_StartMeasure(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
	delay_us(20);
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
}

double UltrasonicWave_Measure(void)
{
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 1);
	UltrasonicWave_StartMeasure();
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 0);
	TIM_SetCounter(TIM2, 0);
	overflow_cnt = 0;
	TIM_Cmd(TIM2, ENABLE);
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) == 1);
	TIM_Cmd(TIM2, DISABLE);
	return (double)(overflow_cnt * 50000 + TIM_GetCounter(TIM2)) / 58.8;
}
