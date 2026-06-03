#ifndef __ESP8266_H
#define __ESP8266_H
#include "sys.h"
#include "stdio.h"	

//用户配置AT指令
extern char restart[];
extern char cwmode[];
extern char cwlap[];
extern char cwjap[];
extern char cifsr[];
extern char cipmux[];
extern char cipstart[];
extern char cipsend[];
extern char cipserver[];
extern char cwlif[];
extern char cipstatus[];
extern char cipsto[];
extern char cipmode[];
extern char test[];
extern char flag;

//函数

u8 ble_send_cmd(u8 *cmd,u8 *ack,u16 waittime);
u8* esp8266_send_data(u8 *cmd,u16 waittime);
u8* esp8266_check_cmd(u8 *str);
u8 esp8266_quit_trans(void);	
void hc_05_cfg_t(void);
u8 ESP8266_REC_DATA(char flag, u16 waittime);
#endif
