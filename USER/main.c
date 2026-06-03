/*********************************
超声波测距系统 — STM32F407VET6 + HC-SR04
**********************************/
#include "sys.h"
#include "delay.h"
#include "hcsr04.h"
#include "timer.h"
#include "lcd.h"
#include "key.h"
#include "CHECK.h"
#include "beep.h"
#include "usart.h"
#include "usart2.h"
#include "stdio.h"

#define K0  (((GPIOC->IDR) >> 11) & 1)
#define K1  (((GPIOC->IDR) >> 12) & 1)
#define K2  (((GPIOC->IDR) >> 13) & 1)
#define ECHO (((GPIOA->IDR) >> 12) & 1)

int main(void)
{
	u16 dist   = 0;
	u16 h_th   = 2200;
	u16 l_th   = 1000;
	u8  alarm  = 0;
	u8  state  = 0;
	u16 input  = 0;
	u8  icount = 0;
	u16 tick   = 0;
	u8  aflg   = 0;
	u8  kv, ck;
	char buf[32];

	delay_init(168);
	TIM2_init();
	LCD_IO_ENABLE();
	LCD_Init();
	UltrasonicWave_Init();
	KEY_Init();
	CHECK_Init();
	BEEP_Init();
	uart_init(115200);
	usart2_init(38400);

	LCD_Clear(WHITE);
	BACK_COLOR = WHITE;
	POINT_COLOR = BLACK;

	LCD_ShowString(10, 200, "Name: XXX");
	LCD_ShowString(10, 220, "ID: 2330110xxx");
	xianshi();
	while(1)
	{
		/* ===== 显示 ===== */
		POINT_COLOR = (aflg && (tick & 1)) ? RED : BLACK;
		BACK_COLOR = WHITE;

		sprintf(buf, "Dist: %4d mm ", dist);
		LCD_ShowString(10, 0, (u8 *)buf);
		sprintf(buf, "H_TH: %4d mm ", h_th);
		LCD_ShowString(10, 40, (u8 *)buf);
		sprintf(buf, "L_TH: %4d mm ", l_th);
		LCD_ShowString(10, 80, (u8 *)buf);
		sprintf(buf, "Alarm: %s  ", alarm ? "ON " : "OFF");
		LCD_ShowString(10, 120, (u8 *)buf);

		if(state == 1 || state == 2)
		{
			sprintf(buf, "Input: %4d   ", input);
			LCD_ShowString(10, 160, (u8 *)buf);
		}
		else
		{
			LCD_ShowString(10, 160, "               ");
		}

		/* ===== 按键 ===== */
		kv = 0;
		if(K0 == 0) { delay_ms(10); if(K0 == 0) kv = 1; }
		if(K1 == 0) { delay_ms(10); if(K1 == 0) kv = 2; }
		if(K2 == 0) { delay_ms(10); if(K2 == 0) kv = 3; }

		/* ===== 状态机 ===== */
		if(kv == 1 && state == 0)      { state=1; input=0; icount=0; }
		else if(kv == 2 && state == 0) { state=2; input=0; icount=0; }
		else if(kv == 3 && state == 0) { alarm = !alarm; }
		else if(kv == 1 && state == 1) { if(icount) h_th=input; state=0; }
		else if(kv == 2 && state == 2) { if(icount) l_th=input; state=0; }

		/* ===== 矩阵键盘 ===== */
		ck = CHECK_Scan();
		if((state == 1 || state == 2) && ck <= 9 && icount < 4)
		{
			input = input * 10 + ck;
			icount++;
		}

		/* ===== 测距 ===== */
		if(ECHO == 1)
			dist = 0;
		else
		{
			double v = UltrasonicWave_Measure();
			if(v > 0 && v < 9990) dist = (u16)(v * 10.0);
		}

		/* ===== 告警 ===== */
		aflg = 0;
		if(alarm && dist > 0)
		{
			if(dist < l_th) aflg = 1;
			if(dist > h_th) aflg = 2;
		}
		if(aflg)
		{
			BEEP = 1; delay_ms(100); BEEP = 0;
			printf("ALARM: Dist=%dmm H_TH=%dmm L_TH=%dmm Type=%s\r\n",
				dist, h_th, l_th, aflg==1 ? "LOW" : "HIGH");
			u2_printf("ALARM: Dist=%dmm H_TH=%dmm L_TH=%dmm Type=%s\r\n",
				dist, h_th, l_th, aflg==1 ? "LOW" : "HIGH");
		}

		tick++;
		delay_ms(200);
	}
}
