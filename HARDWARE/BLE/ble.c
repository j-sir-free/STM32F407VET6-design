#include "ble.h"
#include "string.h"
#include "usart.h"
#include "usart2.h"
#include "sys.h" 
#include "delay.h"
#include "lcd.h"

// private parameter
char flag = 0;
char Switch_Flag = 1;


//HC-05配置
void hc_05_cfg_t(void)
{
	LCD_ShowString(0, 40, "AT+VERSION?");
	while(ble_send_cmd("AT+VERSION?", "OK", 100));
//	ble_send_cmd("AT+VERSION?", "OK", 100);
	LCD_ShowString(0, 60, "OK");
//	delay_ms(1000);
//	ble_send_cmd("AT+NAME?", "OK", 50);
//	LCD_ShowString(0, 60, "AT+NAME?");
//	delay_ms(1000);
//	ble_send_cmd("AT+IAC?", "OK", 50);
//	LCD_ShowString(0, 80, "AT+IAC?");
//	delay_ms(1000);
//	ble_send_cmd("AT+PSWD?", "OK", 50);
//	LCD_ShowString(0, 100, "AT+PSWD?");
//	delay_ms(1000);
//	ble_send_cmd("AT+UART?", "OK", 50);
//	LCD_ShowString(0, 120, "AT+UART?");
	delay_ms(1000);
	LCD_ShowString(0, 80, "AT+NAME=511-00");
	while(ble_send_cmd("AT+NAME=511-00", "OK", 100));	// 修改蓝牙的名称，注意区别！
//	ble_send_cmd("AT+NAME=511-00", "OK", 100);
	LCD_ShowString(0, 100, "OK");
	delay_ms(1000);
	LCD_ShowString(0, 120, "AT+PSWD=1234");
	while(ble_send_cmd("AT+PSWD=1234", "OK", 100));	// 修改连接密码,4位！
//	ble_send_cmd("AT+PSWD=6666", "OK", 100);
	LCD_ShowString(0, 140, "OK");
	delay_ms(1000);
	LCD_ShowString(0, 160, "AT+UART=38400,0,0");
	while(ble_send_cmd("AT+UART=38400,0,0", "OK", 100));
//	ble_send_cmd("AT+UART=38400,0,0", "OK", 100);
	LCD_ShowString(0, 180, "OK");
	delay_ms(1000);
	flag = 6;
//	while(ble_send_cmd("", "", 100));
}

//ESP8266退出透传模式   返回值:0,退出成功;1,退出失败
//配置wifi模块，通过想wifi模块连续发送3个+（每个+号之间 超过10ms,这样认为是连续三次发送+）
u8 esp8266_quit_trans(void)
{
	u8 result = 1;
	u2_printf("+++");
	delay_ms(1000);					//等待500ms太少 要1000ms才可以退出
	result = ble_send_cmd("AT","OK",20);//退出透传判断.
	if(result)
	{
		LCD_ShowString(20, 200, "quit_trans failed!");
		printf("quit_trans failed!");
	}
	else
	{
		LCD_ShowString(20, 200, "quit_trans success!");
		printf("quit_trans success!");
	}
	return result;
}


//向ESP8266发送命令
//cmd:发送的命令字符串;ack:期待的应答结果,如果为空,则表示不需要等待应答;waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果);1,发送失败
u8 ble_send_cmd(u8 *cmd,u8 *ack,u16 waittime)
{
	u8 res = 0; 
	u16 i = 0;
	USART2_RX_STA = 0;
	u2_printf("%s\r\n", cmd);	//发送命令
	if(ack && waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				if(flag != 6)
				{
					if(esp8266_check_cmd(ack))
					{
//						printf("ack:%s\r\n", (u8*)ack);
						switch(flag)
						{
							case 0x01:	LCD_ShowString(150, 40, ack);	break;
							case 0x02:	LCD_ShowString(20, 100, ack);	break;
							case 0x03:	LCD_ShowString(20, 140, ack);	break;
							case 0x04:	LCD_ShowString(20, 180, ack);	break;
							case 0x05:	LCD_ShowString(20, 220, ack);	break;
							default: break;
						}
						break;//得到有效数据 
					}
				}
				else if(flag == 6)
				{
					if(Switch_Flag == 1)
					{
						LCD_Fill(0, 40, 240, 320, BLACK);
						LCD_ShowString(0, 40, "Receive Message:");
						Switch_Flag = 0;
					}
					printf("[\t debug]%s\r\n", USART2_RX_BUF);
					LCD_Fill(0, 40, 240, 320, BLACK);
					LCD_ShowString(0, 40, "Receive Message:");
					LCD_ShowString(0, 80, USART2_RX_BUF);	
					for(i = 0; i < sizeof(USART2_RX_BUF); i++)
						USART2_RX_BUF[i] = 0;					
				}
				USART2_RX_STA = 0;
			} 		
		}
		if(waittime == 0)
		{
			res = 1; 
		}
		
			
	}
	return res;
} 




//ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果;其他,期待应答结果的位置(str的位置)
u8* esp8266_check_cmd(u8 *str)
{
	char *strx = 0;
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF] = 0;//添加结束符
//		for(i = 0; i < USART2_MAX_RECV_LEN; i++){
//			
//		}
		printf("[\t I]rec:%s\r\n", USART2_RX_BUF);
		strx = strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}


//向ESP8266发送数据
//cmd:发送的命令字符串;waittime:等待时间(单位:10ms)
//返回值:发送数据后，服务器的返回验证码
u8* esp8266_send_data(u8 *cmd,u16 waittime)
{
	char temp[5];
	char *ack = temp;
	USART2_RX_STA = 0;
	u2_printf("%s", cmd);	//发送命令
	if(waittime)		//需要等待应答
	{
		while(--waittime)	//等待倒计时
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)//接收到期待的应答结果
			{
				USART2_RX_BUF[USART2_RX_STA&0X7FFF] = 0;//添加结束符
				ack = (char*)USART2_RX_BUF;
				printf("ack:%s\r\n",(u8*)ack);
				USART2_RX_STA = 0;
				break;//得到有效数据 
			} 
		}
	}
	return (u8*)ack;
} 

