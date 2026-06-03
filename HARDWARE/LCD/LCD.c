#include "lcd.h"
#include "delay.h"
#include "font.h"

void LCD_IO_ENABLE(void)//����IO��ʹ��
{
	//ʹ��PD��
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);	
	
	//ʹ��PB��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	
	
	//ʹ��PE��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE,&GPIO_InitStructure);	
}

u16 BACK_COLOR, POINT_COLOR;   //����ɫ������ɫ
void LCD_Writ_Bus(char VH,char VL)   //��������д�뺯��
{
  LCD_DataPortH(VH);	
	LCD_DataPortL(VL);		
	LCD_WR=0;
	LCD_WR=1; 
}
void LCD_WR_DATA8(char VH,char VL) //��������-8λ����
{
    LCD_RS=1;
	LCD_Writ_Bus(VH,VL);
}  
 void LCD_WR_DATA(int da)
{
    LCD_RS=1;
	LCD_Writ_Bus(da>>8,da);
}	  
void LCD_WR_REG(int da)	 
{	
    LCD_RS=0;
	LCD_Writ_Bus(da>>8,da);
}
 void LCD_WR_REG_DATA(int reg,int da)
{
    LCD_WR_REG(reg);
	LCD_WR_DATA(da);
}
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{ 
	 LCD_WR_REG(0x2A);
   LCD_WR_DATA(x1>>8);
   LCD_WR_DATA(x1);
   LCD_WR_DATA(x2>>8);
   LCD_WR_DATA(x2);
  
   LCD_WR_REG(0x2B);
   LCD_WR_DATA(y1>>8);
   LCD_WR_DATA(y1);
   LCD_WR_DATA(y2>>8);
   LCD_WR_DATA(y2);	
   LCD_WR_REG(0x2c);								 
}
void LCD_Init(void)
{

//����һ����Щ��������ñ����ʱ����ʾ����
   	LCD_CS =1;
	if(LCD_CS==0)
	{
	   LCD_WR_REG_DATA(0,0);
	   LCD_ShowString(0,0," ");
	   LCD_ShowNum(0,0,0,0);
	   LCD_Show2Num(0,0,0,0);
	   LCD_DrawPoint_big(0,0);
	   LCD_DrawRectangle(0,0,0,0);
	   Draw_Circle(0,0,0);
 	 }
	LCD_RD=1;
	LCD_WR=1;
	LCD_REST=0;
	delay_ms(20);	//�˴���֤Ҫ��5ms���ϲ��ܱ�֤��λ�ȶ�,�Ƽ�20ms
	LCD_REST=1;	
	delay_ms(20);	//�˴���֤Ҫ��5ms���ϲ��ܱ�֤��λ�ȶ�,�Ƽ�20ms		
	LCD_CS =0;  //��Ƭѡʹ��

  //************* Start Initial Sequence **********//
LCD_WR_REG(0xcf); 
LCD_WR_DATA(0x00);
LCD_WR_DATA(0xc1);
LCD_WR_DATA(0x30);

LCD_WR_REG(0xed); 
LCD_WR_DATA(0x64);
LCD_WR_DATA(0x03);
LCD_WR_DATA(0x12);
LCD_WR_DATA(0x81);

LCD_WR_REG(0xcb); 
LCD_WR_DATA(0x39);
LCD_WR_DATA(0x2c);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x34);
LCD_WR_DATA(0x02);

LCD_WR_REG(0xea); 
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x00);

LCD_WR_REG(0xe8); 
LCD_WR_DATA(0x85);
LCD_WR_DATA(0x10);
LCD_WR_DATA(0x79);

LCD_WR_REG(0xC0); //Power control
LCD_WR_DATA(0x23); //VRH[5:0]

LCD_WR_REG(0xC1); //Power control
LCD_WR_DATA(0x11); //SAP[2:0];BT[3:0]

LCD_WR_REG(0xC2);
LCD_WR_DATA(0x11);

LCD_WR_REG(0xC5); //VCM control
LCD_WR_DATA(0x3d);
LCD_WR_DATA(0x30);

LCD_WR_REG(0xc7); 
LCD_WR_DATA(0xaa);

LCD_WR_REG(0x3A); 
LCD_WR_DATA(0x55);

LCD_WR_REG(0x36); // Memory Access Control
LCD_WR_DATA(0x08);

LCD_WR_REG(0xB1); // Frame Rate Control
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x11);

LCD_WR_REG(0xB6); // Display Function Control
LCD_WR_DATA(0x0a);
LCD_WR_DATA(0xa2);

LCD_WR_REG(0xF2); // 3Gamma Function Disable
LCD_WR_DATA(0x00);

LCD_WR_REG(0xF7);
LCD_WR_DATA(0x20);

LCD_WR_REG(0xF1);
LCD_WR_DATA(0x01);
LCD_WR_DATA(0x30);

LCD_WR_REG(0x26); //Gamma curve selected
LCD_WR_DATA(0x01);

LCD_WR_REG(0xE0); //Set Gamma
LCD_WR_DATA(0x0f);
LCD_WR_DATA(0x3f);
LCD_WR_DATA(0x2f);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x10);
LCD_WR_DATA(0x0a);
LCD_WR_DATA(0x53);
LCD_WR_DATA(0xd5);
LCD_WR_DATA(0x40);
LCD_WR_DATA(0x0a);
LCD_WR_DATA(0x13);
LCD_WR_DATA(0x03);
LCD_WR_DATA(0x08);
LCD_WR_DATA(0x03);
LCD_WR_DATA(0x00);

LCD_WR_REG(0xE1); //Set Gamma
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x10);
LCD_WR_DATA(0x03);
LCD_WR_DATA(0x0f);
LCD_WR_DATA(0x05);
LCD_WR_DATA(0x2c);
LCD_WR_DATA(0xa2);
LCD_WR_DATA(0x3f);
LCD_WR_DATA(0x05);
LCD_WR_DATA(0x0e);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x37);
LCD_WR_DATA(0x3c);
LCD_WR_DATA(0x0F);
LCD_WR_REG(0x11); //Exit Sleep
delay_ms(80);//�˴�����Ҫ��ʱ80ms���߸���
LCD_WR_REG(0x29); //display on
}
//��������
//Color:Ҫ���������ɫ
void LCD_Clear(u16 Color)
{
	u8 VH,VL;
	u16 i,j;
	VH=Color>>8;
	VL=Color;	
	Address_set(0,0,LCD_W-1,LCD_H-1);
    for(i=0;i<LCD_W;i++)
	 {
	  for (j=0;j<LCD_H;j++)
	   	{
        	 LCD_WR_DATA8(VH,VL);
	    }

	  }
}
//����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(u16 x,u16 y)
{
	Address_set(x,y,x,y);//���ù��λ�� 
	LCD_WR_DATA(POINT_COLOR); 	    
} 	 
//��һ�����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint_big(u16 x,u16 y)
{
	LCD_Fill(x-1,y-1,x+1,y+1,POINT_COLOR);
} 
//��ָ�����������ָ����ɫ
//�����С:
//  (xend-xsta)*(yend-ysta)
void LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{          
	u16 i,j; 
	Address_set(xsta,ysta,xend,yend);      //���ù��λ�� 
	for(i=ysta;i<=yend;i++)
	{													   	 	
		for(j=xsta;j<=xend;j++)LCD_WR_DATA(color);//���ù��λ�� 	    
	} 					  	    
}  
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//������
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(u16 x0,u16 y0,u8 r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a);             //3           
		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0-a,y0+b);             //1       
		LCD_DrawPoint(x0-b,y0-a);             //7           
		LCD_DrawPoint(x0-a,y0-b);             //2             
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0-b);             //5
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-b,y0+a);             
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 
		LCD_DrawPoint(x0+a,y0+b);
	}
} 
//��ָ��λ����ʾһ���ַ�

//num:Ҫ��ʾ���ַ�:" "--->"~"
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
//��ָ��λ����ʾһ���ַ�

//num:Ҫ��ʾ���ַ�:" "--->"~"

//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 mode)
{
    u8 temp;
    u8 pos,t;
	u16 x0=x;
	u16 colortemp=POINT_COLOR;      
    if(x>LCD_W-16||y>LCD_H-16)return;	    
	//���ô���		   
	num=num-' ';//�õ�ƫ�ƺ��ֵ
	Address_set(x,y,x+8-1,y+16-1);      //���ù��λ�� 
	if(!mode) //�ǵ��ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{ 
			temp=Font8x16[(u16)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)POINT_COLOR=colortemp;
				else POINT_COLOR=BACK_COLOR;
				LCD_WR_DATA(POINT_COLOR);	
				temp>>=1; 
				x++;
		    }
			x=x0;
			y++;
		}	
	}else//���ӷ�ʽ
	{
		for(pos=0;pos<16;pos++)
		{
		    temp=Font8x16[(u16)num*16+pos];		 //����1608����
			for(t=0;t<8;t++)
		    {                 
		        if(temp&0x01)LCD_DrawPoint(x+t,y+pos);//��һ����     
		        temp>>=1; 
		    }
		}
	}
	POINT_COLOR=colortemp;	    	   	 	  
}   
//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//color:��ɫ
//num:��ֵ(0~4294967295);	
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len)
{         	
	u8 t,temp;
	u8 enshow=0;
	num=(u16)num;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+8*t,y,' ',0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+8*t,y,temp+48,0); 
	}
} 
//��ʾ2������
//x,y:�������
//num:��ֵ(0~99);	 
void LCD_Show2Num(u16 x,u16 y,u16 num,u8 len)
{         	
	u8 t,temp;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
	 	LCD_ShowChar(x+8*t,y,temp+'0',0); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//*p:�ַ�����ʼ��ַ
//��16����
void LCD_ShowString(u16 x,u16 y,const u8 *p)
{         
    while(*p!='\0')
    {       
      if(x>LCD_W-16){x=0;y+=16;}
      if(y>LCD_H-16){y=x=0;LCD_Clear(RED);}
      LCD_ShowChar(x,y,*p,0);
      x+=8;
      p++;
    }  
}
void showhanzi(unsigned int x,unsigned int y,unsigned char index)	
{  
	unsigned char i,j;
	unsigned char *temp=hanzi;    
  Address_set(x,y,x+31,y+31); //��������     
	temp+=index*128;	
	for(j=0;j<128;j++)
	{
		for(i=0;i<8;i++)
		{ 		     
		 	if((*temp&(1<<i))!=0)
			{
				LCD_WR_DATA(POINT_COLOR);
			} 
			else
			{
				LCD_WR_DATA(BACK_COLOR);
			}   
		}
		temp++;
	 }
}
u16 vx=15489,vy=11106;
void xianshi()//��ʾ��Ϣ
{   
	
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;	
	showhanzi(10,250,0);  
	showhanzi(45,250,1);  
	showhanzi(80,250,2);  
	
}
